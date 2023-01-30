// ExifInfo.cpp: ExifInfo クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "picmove.h"
#include "ExifInfo.h"
//#include "D:\nyk\Cximage\ximajpg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

ExifInfo::ExifInfo()
{
	shotTime = 0;
	width = 0;
	height = 0;
	orientation = 0;
//	fileSize = 0;
	errCode = 0;
	swMotorola = false;
	maker = "_Maker";
	model = "_Model";
}

ExifInfo::~ExifInfo()
{

}

int ExifInfo::read2(unsigned char *buf)
{
	int ret;
	if (swMotorola) {
		ret = (int)buf[0] << 8;
		ret += (int)buf[1];
	}
	else {
		ret = (int)buf[1] << 8;
		ret += (int)buf[0];
	}
	return ret;
}


long ExifInfo::read4(unsigned char *buf)
{
	long ret;
	if (swMotorola) {
//		TRACE("MOTOROLA\n");
		ret = (long)buf[0] << 24;
		ret += (long)buf[1] << 16;
		ret += (long)buf[2] << 8;
		ret += (long)buf[3];
	}
	else {
//		TRACE("INTEL\n");
		ret = (long)buf[3] << 24;
		ret += (long)buf[2] << 16;
		ret += (long)buf[1] << 8;
		ret += (long)buf[0];
	}
//	TRACE("read4 %x\n", ret);
	return ret;
}

// 0123456789012345678
// 2005:02:10 08:32:35 -> time_t に変換する
static time_t exifdate2time(CString date_str)
{
//	TRACE("len %d\n", date_str.GetLength());	
	if (date_str.GetLength() != 19) return -1;

	int year, mon, day, hour, min, sec;
	year = atoi(date_str.Mid(0, 4));
	mon  = atoi(date_str.Mid(5, 2));
	day  = atoi(date_str.Mid(8, 2));
	hour = atoi(date_str.Mid(11, 2));
	min  = atoi(date_str.Mid(14, 2));
	sec  = atoi(date_str.Mid(17, 2));

	if (year < 1971) year = 1971;
	if (mon < 1) mon = 1;
	else if (mon > 12) mon = 12;
	if (day < 1) day = 1;
	else if (day > 31) day = 31;

	if (hour < 0) hour = 0;
	else if (hour > 24) hour = 24;
	if (min < 0) min = 0;
	else if (min >= 60) min = 59;
	if (sec < 0) sec = 0;
	else if (sec >= 60) sec = 59;

	CTime t(year, mon, day, hour, min, sec);

//	TRACE("%s->%d/%d/%d  %d:%d:%d\n", date_str,
//		t.GetYear(), t.GetMonth(), t.GetDay(), 
//		t.GetHour(), t.GetMinute(), t.GetSecond());

	return t.GetTime();
}

int ExifInfo::loadExifFile(CString name)
{
	
	TRACE("****************** %s\n", name);
	int ret = loadExifFileLocal(name);
	if (ret) return ret;

	TRACE("################## %s\n", name);

	// 失敗しているので次の手
	{
		FILE *fp = fopen(name, "rb");
		if (fp == NULL) return 0;
	
		CxImageJPG image;
		image.DecodeExif(fp);
		fclose(fp);

		if (image.m_exif) {
			if (image.m_exifinfo.IsExif) {
				this->shotTime = exifdate2time(image.m_exifinfo.DateTime);
				if (shotTime == -1) {
					errCode = 10;
					return 0;
				}
				this->orientation = image.m_exifinfo.Orientation;

				maker = image.m_exifinfo.CameraMake;
				maker.TrimRight();
				maker.TrimLeft();
				int fs = maker.Find(" ");
				if (fs != -1) maker = maker.Left(fs);
				model = image.m_exifinfo.CameraModel;
				model.TrimRight();
				model.TrimLeft();
				
				errCode = 0;
				ret = 1;
			}
		}
	}
	return ret;
}

// exif ファイルを調べて値をセットする
int ExifInfo::loadExifFileLocal(CString name)
{
	FILE *fp;

	errCode = 0;

	fp = fopen(name, "rb");
	if (fp == NULL) { 
		errCode = 2; return 0;
	}

	// SOI と APP1 で始まっているかどうかチェックする
	// 先頭の16バイトを読み込む
	unsigned char topbuf[16];
	int topbuf_count = (int)fread(&topbuf[0], 1, 16, fp);
	if (topbuf_count != 16) {
		fclose(fp);	errCode = 2; return 0;
	}

	if (!(topbuf[0] == 0xff && topbuf[1]==0xd8 && topbuf[2]==0xff && topbuf[3]==0xe1)) {
		fclose(fp);	errCode = 3; return 0;
	}
	if (!(topbuf[6]=='E'&&topbuf[7]=='x'&&topbuf[8]=='i'&&topbuf[9]=='f'&&
		topbuf[10]==0&&topbuf[11]==0)) {
		fclose(fp);	errCode = 4; return 0;
	}

	// 先頭OK
	// まずAPP1ヘッダを読み込む
	int size_app1 = (topbuf[4] << 8) + topbuf[5];
	size_app1 += 4;
	rewind(fp);

	if (size_app1 < 32) {
		fclose(fp);	errCode = 9; return 0;
	}

	TRACE("size_app1 %d %x\n", size_app1, size_app1);

	unsigned char *buf = new unsigned char[size_app1];
	unsigned char *buf_top = buf;
	unsigned char *tiff_top = buf_top + 12;
	unsigned char *pSubIfd = 0;

	int buf_count = (int)fread(buf, 1, size_app1, fp);
	if (buf_count != size_app1) {
		delete [] buf;
		fclose(fp);	errCode = 5; return 0;
	}
	// APP1ヘッダ読み込めたよ	
	if (buf[12] == 'I' && buf[13] == 'I') {
		swMotorola = false;
	}
	else if (buf[12] == 'M' && buf[13] == 'M') {
		swMotorola = true;
	}
	else {
		delete [] buf;
		fclose(fp);	errCode = 6; return 0;
	}
	
	// これでインテルかモトローラかがはっきりした
//	if (read4(buf_top + 0x10) != 8) { // 8以外はよまないよ
//		delete [] buf;
//		fclose(fp); errCode = 8; return 0;
//	}
//	int entries = read2(buf_top + 0x14);
//	unsigned char *buf_tiff = buf_top + 0x16;

	int IFDoffset = read4(buf_top + 0x10);
	int entries = read2(buf_top + 0x0c + IFDoffset);
	unsigned char *buf_tiff = buf_top + 0x0c + IFDoffset + 2;


	for (int i = 0; i < entries; i++) {
		if (buf_tiff-buf_top > size_app1) goto errExit;
		int d_tag, d_format;
		long d_numdata, d_data;
//		TRACE("entry %d\n", i);
		d_tag = read2(buf_tiff); buf_tiff+=2;
		d_format = read2(buf_tiff); buf_tiff+=2;
		d_numdata = read4(buf_tiff); buf_tiff+=4;
		TRACE("T %d/%d d_tag:%x Format %x Num %x\n", i, entries, 
			d_tag, d_format, d_numdata);
		switch (d_tag) {
		case 0x132:
			d_data = read4(buf_tiff); 
			if (d_format == 0x02 && d_numdata == 0x14) {
				char datebuf[0x20];
				strncpy(datebuf, (const char*)(tiff_top + d_data), 0x14);
				TRACE("DATE %s\n", datebuf);
				this->shotTime = exifdate2time(datebuf);
				if (shotTime == -1) {
					errCode = 10;
					goto errExit;
				}
			}
			else {
				this->shotTime = -1;
				TRACE("DATE FAILED\n");
				errCode = 10;
				goto errExit;
			}
			break;
		case 0x10f:	// maker
			d_data = read4(buf_tiff); 
			if (d_numdata >= 5) {
				this->maker = CString((LPCTSTR)(tiff_top + d_data), d_numdata-1);
			}
			else {
				this->maker = "";
				unsigned char cc[4];
				cc[0] = d_data >> 24;
				cc[1] = (d_data >> 16) & 0xff;
				cc[2] = (d_data >> 8) & 0xff;
				cc[3] = d_data & 0xff;
				for (int i = 0; i < d_numdata; i++) {
					this->maker+= cc[i];
				}
			}
			maker.TrimRight();
			maker.TrimLeft();
			{
				int fs = maker.Find(" ");
				if (fs != -1) maker = maker.Left(fs);
			}

			TRACE("MAKER = [%s]\n", maker);
			break;
		case 0x110:	// model
			d_data = read4(buf_tiff); 
			if (d_numdata >= 5) {
				this->model = CString((LPCTSTR)(tiff_top + d_data), d_numdata-1);
			}
			else {
				this->model = "";
				unsigned char cc[4];
				cc[0] = d_data >> 24;
				cc[1] = (d_data >> 16) & 0xff;
				cc[2] = (d_data >> 8) & 0xff;
				cc[3] = d_data & 0xff;
				for (int i = 0; i < d_numdata; i++) {
					this->model+= cc[i];
				}
			}
			model.TrimRight();
			model.TrimLeft();
			TRACE("MODEL = [%s]\n", model);
			break;
		case 0x112:
			d_data = read2(buf_tiff); 
			this->orientation = d_data;
			TRACE("ORIENTATION %d\n", this->orientation); 
			break;
		case 0x8769: // ExifIFDPointer  
			d_data = read4(buf_tiff); 
			if (d_format == 0x04 && d_numdata == 1) {
				TRACE("top %x\n", d_data + tiff_top);
				pSubIfd = d_data + tiff_top;
			}
			else {
				pSubIfd = 0;
			}
			TRACE("ExifIFDPointer %x\n", d_data);
			break;
		default:
			break;
		}
		buf_tiff+=4;
	}
	if (pSubIfd) {
		TRACE("--- read subifd --- %x %x %x %x\n", *(pSubIfd+0),*(pSubIfd+1),*(pSubIfd+2),*(pSubIfd+3));
		buf_tiff = pSubIfd;
		int entries = read2(buf_tiff); buf_tiff+=2;
		for (int i = 0; i < entries; i++) {
			int d_tag, d_format;
			long d_numdata, d_data;
			d_tag = read2(buf_tiff); buf_tiff+=2;
			d_format = read2(buf_tiff); buf_tiff+=2;
			d_numdata = read4(buf_tiff); buf_tiff+=4;
			TRACE("T %d/%d d_tag:%x Format %x Num %x\n", i, entries, 
				d_tag, d_format, d_numdata);
			switch (d_tag) {
			case 0x9003:
//			case 0x9004:
				d_data = read4(buf_tiff); 
				if (d_format == 0x02 && d_numdata == 0x14) {
					char datebuf[0x20];
					strncpy(datebuf, (const char*)(tiff_top + d_data), 0x14);
					TRACE("DATE %s\n", datebuf);
					this->shotTime = exifdate2time(datebuf);
				}
				else {
					this->shotTime = -1;
					TRACE("DATE FAILED\n");
					errCode = 10;
					goto errExit;
				}
				break;
			default:
				break;
			}
			buf_tiff+=4;
		}
	}


errExit:
	delete [] buf;

	fclose(fp);
	if (errCode == 0) return 1;
	else return 0;

}



////////////////////////////////////////////////////////////////////////////////////////


#if 0
ExifExtra::ExifExtra()
{
	fullpath = "";

	bActive = false;
	bError = false;

	maker = "";
	model = "";
}

ExifExtra::~ExifExtra()
{
}


bool ExifExtra::Get(const CString &fullpath_in, bool bForce)
{
	if (!bForce) {
		if (fullpath == fullpath_in) {
			if (!bActive) return false;
			if (bError) return false;
			return true;
		}
	}

	fullpath = fullpath_in;
	bActive = true;
	bError = true;


	{
		FILE *fp = fopen(fullpath, "rb");
		if (fp == NULL) return false;
	
		CxImageJPG image;
		bool retdecode = image.DecodeExif(fp);
		fclose(fp);

		if (image.m_exifinfo.CameraMake[0] == 0) {
			return false;
		}

		if (retdecode && image.m_exif) {
			if (image.m_exifinfo.IsExif) {
				{
					maker = image.m_exifinfo.CameraMake;
					maker.TrimRight();
					maker.TrimLeft();
					int fs = maker.Find(" ");
					if (fs != -1) maker = maker.Left(fs);
				}
				model = image.m_exifinfo.CameraModel;
				model.TrimRight();
				model.TrimLeft();
				bError = false;
				bActive = true;
			}
		}
	}

	return !bError;
}
#endif