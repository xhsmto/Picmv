// DcimList.cpp: CDcimList クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

// デジカメの画像ファイルのリストを格納・管理するクラス
#include <vector>
using namespace std;

#include "stdafx.h"
#include "picmove.h"
#include "DcimList.h"
#include "ExifInfo.h"

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#include <algorithm>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*
Dcim::Dcim()
{
	copyDone = false;
}
*/

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CDcimList::CDcimList()
{
	dcim.clear();
	extensions = "JPG";
	extensions_video = "AVI";
//	m_outDirs = 1;
}

CDcimList::~CDcimList()
{
	TRACE("dcimlist deconstractor\n");
	dcim.clear();
}

void CDcimList::Clear(void)
{
	dcim.clear();
}

static bool nykExtensionsCheck(const CString &src_in, const CString &ext_list)
{
	TRACE("in %s %s\n", src_in, ext_list);
	if (src_in.GetLength() == 0) return false;
	if (ext_list.GetLength() == 0) return false;
	
	CString src = src_in;
	src.MakeUpper();

	// 拡張子だけ取り出す
	int c = src.ReverseFind('.');
	if (c == -1) return false; // 拡張子がない場合
	src = src.Mid(c+1);
	// src="AVI" とか

	int n = 0;
	while (1) {
		int c = ext_list.Find(',', n);
		if (c == -1) {
			TRACE("F %s %s\n", ext_list.Mid(n), src);
			if (ext_list.Mid(n) == src) return true;
			return false;
		}
		TRACE("X %s %s\n", ext_list.Mid(n,c-n), src);
		if (ext_list.Mid(n, c-n) == src) return true;
		n = c + 1;
	}
}

static int nykGetFileType(const CString &fileName, const CString &extension, const CString &extension_for_video)
{
	CString name = fileName;
	int c = name.ReverseFind('.');
	if (c == -1) { return NYK_FILE_INVALID; }// 拡張子がない場合
	CString ext = name.Mid(c);
	if (ext.CompareNoCase(".JPG") == 0) return NYK_FILE_JPG;
	else if (ext.CompareNoCase(".JPEG") == 0) return NYK_FILE_JPG;
	else if (nykExtensionsCheck(name, extension)) return NYK_FILE_OTHER_IMAGE;
	else if (nykExtensionsCheck(name, extension_for_video)) return NYK_FILE_OTHER_VIDEO;
	// ここへきているということは拡張子はあるが、該当するのにないよ
	return  NYK_FILE_INVALID;

}


static int SearchJpeg(CString dir, vector<Dcim> &tgt, void (*callback)(const CString&), const CString& extensions_in, const CString& extensions_in2)
{
	WIN32_FIND_DATA fd;
	HANDLE h;
	
	TRACE("dir = %s\n", dir);
	
	CString tgtname = dir + "*.*";
	callback(tgtname);

    h = FindFirstFile(tgtname,&fd);
    if(INVALID_HANDLE_VALUE == h) {
		return 0;
	}
	do {
		{
			CPicmoveApp* app = (CPicmoveApp*)AfxGetApp();
			if (app->requestStop) { FindClose(h); return 0; }
		}

		if (strcmp(fd.cFileName, ".") == 0) continue;
		if (strcmp(fd.cFileName, "..") == 0) continue;
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {	// ディレクトリだったら
			if (strcmp(fd.cFileName, "PREVIEW") == 0) continue;
			SearchJpeg(dir + fd.cFileName + "\\", tgt, callback, extensions_in, extensions_in2);
			continue;
		}
		TRACE("FILE = %s\n", fd.cFileName);
		Dcim tmp;
		tmp.dir = dir;
		tmp.orgName = fd.cFileName;
		if ((!nykExtensionsCheck(tmp.orgName, extensions_in)) && (!nykExtensionsCheck(tmp.orgName, extensions_in2))) continue;
		tmp.filesize =  (fd.nFileSizeHigh * MAXDWORD) + fd.nFileSizeLow;
		CTime t(fd.ftLastWriteTime);
		tmp.fileTime = t.GetTime();
		tmp.fileFormat = nykGetFileType(tmp.orgName, extensions_in, extensions_in2);
		tgt.push_back(tmp);
	} while(FindNextFile(h, &fd) != 0);
	FindClose(h);
	return 1;
}

void CDcimList::LoadList(CString dirList, void (*callback)(const CString&))
{
	// dirList はカンマ区切りなので整理する
	dirList.TrimLeft();
	dirList.TrimRight();

	vector<CString> dir;
	StringToArray(dirList, ",", dir);

	this->dcim.clear();

//	int counter = 0;
	// ディレクトリのリストができたところでそれぞれの exif 情報をもとめつつ dcim に保存する
	vector<CString>::iterator i;
	for (i = dir.begin(); i != dir.end(); i++) {
		TRACE("dir:: '%s'\n", *i);
		CString basedir = *i;
		SearchJpeg(basedir, this->dcim, callback, extensions, extensions_video);
		{
			CPicmoveApp* app = (CPicmoveApp*)AfxGetApp();
			if (app->requestStop) { return ; }
		}
	}
	// これで対象ファイルのリストができた
	//this->SortByName();
	TRACE("---- dcim ----\n");
	vector<Dcim>::iterator j;
	for (j = dcim.begin(); j != dcim.end(); j++) {
		TRACE("%s %s fs:%f t:%d\n", j->dir, j->orgName, j->filesize, j->fileTime);
	}
}

void CDcimList::LoadListFromDragFiles(vector<CString> &dropFile_in, void (*callback)(const CString&))
{
	this->dcim.clear();

	vector<CString>::iterator i;

	for (i = dropFile_in.begin(); i != dropFile_in.end(); i++) {
		if (i->GetLength() <= 0) continue;
		int atr = GetFileAttributes(*i);
		if (atr == -1) continue;  // なんかエラーでた
		// *i はディレクトリかどうか確認する
		else if (atr & FILE_ATTRIBUTE_DIRECTORY) { // ディレクトリだった
			CString basedir;
			basedir = *i;
			basedir += "\\";
			TRACE("%s is dir\n", basedir);
			SearchJpeg(basedir, this->dcim, callback, extensions, extensions_video);
		}
		// それではファイルでしょう
		else if ((!nykExtensionsCheck(*i, extensions)) && (!nykExtensionsCheck(*i, extensions_video))) {
			TRACE("NOT JPEGAVI FILE %s\n", *i);
			continue;
		}
		else { // ということは jpeg ファイル
			TRACE("JPEG FILE %s\n", *i);

			char drive_c[_MAX_DRIVE], dir_c[_MAX_DIR], fname_c[_MAX_FNAME], ext_c[_MAX_EXT];
			_splitpath(*i, drive_c, dir_c, fname_c, ext_c);	// 分解するよ
			CString drive(drive_c), dir(dir_c), fname(fname_c), ext(ext_c);
			Dcim tmp;

			tmp.dir = drive + dir;
			tmp.orgName = fname + ext;
			tmp.fileFormat = nykGetFileType(tmp.orgName, extensions, extensions_video);
			struct _stat buf;
			
			if (::_stat(*i, &buf) == -1) {
				TRACE("_stat error\n");
				continue;
			}
			else {
				tmp.fileTime = buf.st_mtime;
				tmp.filesize = buf.st_size;
			}
			dcim.push_back(tmp);
		}

		// 中止要請あるか?
		{
			CPicmoveApp* app = (CPicmoveApp*)AfxGetApp();
			if (app->requestStop) { return ; }
		}
	}
	//this->SortByName();
}


int CDcimList::GetSizeList(void)
{
	return (int)dcim.size();
}

void CDcimList::GetDcim(Dcim* ret, int index)
{
	*ret = dcim[index];
}

void CDcimList::SetDcim(Dcim* val, int index)
{
	dcim[index] = *val;
}
/*
static bool getExtension(const CString &text, CString &ext_out)
{
	char drive_c[_MAX_DRIVE], dir_c[_MAX_DIR], fname_c[_MAX_FNAME], ext_c[_MAX_EXT];
	_splitpath(text, drive_c, dir_c, fname_c, ext_c);	// 分解するよ
	CString ext(ext_c);
	ext_out = ext;
	return (ext_out.GetLength() != 0) ? true : false;
}
*/

static void extractC2Time(int c, CString &outname, CTime &t, const CString &text, 
						  const CString &maker, const CString &model, const CString &ext_in, int counter_by_day)
{
	CString str;

	switch (c) {
	case 'p':
		{
			char path[MAX_PATH*2]; 
			::SHGetSpecialFolderPath(AfxGetMainWnd()->GetSafeHwnd(), path, CSIDL_PERSONAL, FALSE); 
			CString paths(path);
			paths += "\\My Pictures";
			outname += paths;
		}
		break;
	case 'e':
		{
			CString ext = ext_in;
			ext.MakeLower();
			outname += ext;
		}
		break;
	case 'E':
		{
			CString ext = ext_in;
			ext.MakeUpper();
			outname += ext;
		}
		break;
/*	case 'f':
		{

		}
		break;*/
	case 'k':	
		outname += maker; 
		break;
	case 'l':	
		outname += model; 
		break;
	case 'g':	// 平成 or 昭和
		{
			int y = t.GetYear();
			int g = 0;
			if (y >= 1989) {
				g = y - 1989 + 1;
			}
			else if (y >= 1926) {
				g = y - 1926 + 1;
			}
			str.Format("%02d", g); outname += str; 
		}
		break;
	case 'Y':	str.Format("%04d", t.GetYear()); outname += str; break;
	case 'y':	str.Format("%02d", t.GetYear() % 100); outname += str; break;
	case 'm':	str.Format("%02d", t.GetMonth()); outname += str; break;
	case 'd':	str.Format("%02d", t.GetDay()); outname += str; break;
	case 'H':	str.Format("%02d", t.GetHour()); outname += str; break;
	case 'M':	str.Format("%02d", t.GetMinute()); outname += str; break;
	case 'S':	str.Format("%02d", t.GetSecond()); outname += str; break;
	case 'w':
		{
			char *weekstr[] = { "", "Sun","Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
			outname += weekstr[t.GetDayOfWeek()];
		}
		break;
	case 'W':
		{
			char *weekstr[] = { "", "日","月", "火", "水", "木", "金", "土"};
			outname += weekstr[t.GetDayOfWeek()];
		}
		break;
	case 'o':
		outname += text;
		break;
	case 'c':
		str.Format("%03d", counter_by_day);
		outname += str;
		break;
	case '%':
		outname += "%";
		break;
	default:
		str.Format("%%%c", c);
		outname += str;
		break;
	}
}


// fileName から拡張子だけを返す
static CString nykGetStrExt(const CString& fileName)
{
	CString ret = "";
	int c = fileName.ReverseFind('.');
	if (c == -1) return ret;
	ret = fileName.Mid(c+1);
	return ret;
}

// 大小文字変換 0=大文字 1=小文字 2=そのまま
static CString nykExUpper(const CString &src, int sw_ex_upper)
{
	CString ret = src;
	switch (sw_ex_upper) {
	case 0:
		ret.MakeUpper();
		break;
	case 1:
		ret.MakeLower();
		break;
	case 2:
	default:
		break;
	}
	return ret;
}

void makeDcimFileName(CString& outname, time_t t_in, const CString &naming_text, time_t t_now, 
	 const CString &ext, const CString &original_name, const CString &maker, const CString &model, const CString &ext2,
	 int counter_by_day)
{
	CTime t(t_in);
	CTime now(t_now);

	outname = "";
	int i;

	for (i = 0; i < naming_text.GetLength(); i++) {
		int c = naming_text.GetAt(i);

		if (c == '%') {
			if (i == naming_text.GetLength()-1) { // 最後の文字だったら
				break;
			}
			i++;
			int c2 = naming_text.GetAt(i);
			if (c2 == 'a') {
				if (i == naming_text.GetLength()-1) { // 最後の文字だったら
					break;
				}
				i++;
				int c3 = naming_text.GetAt(i);
				extractC2Time(c3, outname, now, "", maker,model, ext2, counter_by_day);
			}
			else {
				extractC2Time(c2, outname, t, original_name, maker,model, ext2, counter_by_day);
			}
		}
		else {
			CString str((char)c, 1);
//			TRACE("str=[%s] ", str);
			outname += str;
//			TRACE("out=[%s]\n", outname);
		}
	}
	if (ext != "") {
		outname += ".";
		outname += ext;
	}
	TRACE("makeDcimFileName: %s -> %s\n",naming_text, outname);
}

/*
static bool pr_dcim(const Dcim &a, const Dcim &b)
{
	return a.shotTime < b.shotTime;
}


static bool pr_dcim_name(const Dcim &a, const Dcim &b)	// 2007.6.11
{
	return a.orgName < b.orgName;
}
*/

static bool pr_dcim_time2name(const Dcim &a, const Dcim &b)	// 2007.6.17
{
	if (a.shotTime != b.shotTime) return a.shotTime < b.shotTime;
	return a.orgName < b.orgName;
}

/*
void CDcimList::SortByName()
{
	TRACE("AAAAAAAAAAAA sort by name\n");
	vector<Dcim>::iterator j;
	for ( j = dcim.begin(); j != dcim.end(); j++) {
		TRACE("NOTSORT %s %s fs:%d t:%d shot:%d\n", j->dir, j->orgName, j->filesize, j->fileTime, j->shotTime);
	}
	sort(dcim.begin(), dcim.end(), pr_dcim_name);
	TRACE("BBBBBBBBBBBB sort by name\n");
	for ( j = dcim.begin(); j != dcim.end(); j++) {
		TRACE("SORTED %s %s fs:%d t:%d shot:%d\n", j->dir, j->orgName, j->filesize, j->fileTime, j->shotTime);
	}
}*/

// ex_upper=0 拡張子を大文字 1 拡張子を小文字 2 拡張子そのまま
void CDcimList::SetExifInfomation(const CString &naming_text, const CString &thumb_naming_text, const CString &naming_dir,
								  const CString &thumb_naming_dir, const CString &naming_video_dir, void (*callback)(const CString&, int, int), int ex_upper,
								  int counter_by_day)
{
	vector<CString> naming_dirs;
	StringToArray(naming_dir, ",", naming_dirs);
//	this->m_outDirs = naming_dirs.size();
//	if (m_outDirs > NEW_DIR_EX_MAX) m_outDirs = NEW_DIR_EX_MAX;
	int c_outDirs = naming_dirs.size();
	if (c_outDirs > NEW_DIR_EX_MAX) c_outDirs = NEW_DIR_EX_MAX;

	vector<CString> naming_video_dirs;
	StringToArray(naming_video_dir, ",", naming_video_dirs);
	int c_outDirsVideo = naming_video_dirs.size();
	if (c_outDirsVideo > NEW_DIR_EX_MAX) c_outDirsVideo = NEW_DIR_EX_MAX;

	m_outDirs = c_outDirs;
	m_outDirsVideo = c_outDirsVideo;

	time_t now;
	now = time(&now);
	TRACE("--- exif info ---\n");
	vector<Dcim>::iterator j;
	int count = 0;
	int max = (int)dcim.size();
	for (j = dcim.begin(); j != dcim.end(); j++) {
		//TRACE("* %s %s fs:%d t:%d\n", j->dir, j->orgName, j->filesize, j->fileTime);
		
		CString callback_text;
		callback_text.Format("%d/%d (%1.0lf%%) %s", count, max, 100.0*(double)count/max, j->dir+j->orgName);
		callback(callback_text, count, max);
		count++;

		{
			CPicmoveApp* app = (CPicmoveApp*)AfxGetApp();
			if (app->requestStop) { return ; }
		}

		if (j->fileFormat == NYK_FILE_JPG) {		
			ExifInfo e;
			int ret = e.loadExifFile(j->dir + j->orgName);
			j->errCode = e.getErrCode();
			if (ret) { // Exif 取得に成功した
				j->height = e.getHeight();
				j->width = e.getWidth();
				j->orientation = e.getOrientation();
				j->shotTime = e.getShotTime();
				j->maker = e.maker;
				j->model = e.model;
				Escape4FileName(j->maker);
				Escape4FileName(j->model);
				//TRACE("MODEL->%s\n", j->model);
			}
			else { //・Exif日時が取得できなかった場合や、未設定だった場合は、更新日時でリネームします。
				j->height = j->width = 0;
				j->orientation = 0;
				j->shotTime = j->fileTime;
				j->maker = "_Maker";
				j->model = "_Model";
				TRACE("EXIFERROR: ");
			}
		}
		else {
			j->height = j->width = 0;
			j->orientation = 0;
			j->shotTime = j->fileTime;
			j->maker = "_Maker";
			j->model = "_Model";
			TRACE("EXIFERROR: NOJPEG ");
		}
		
		CString orgNameBase;	// 拡張子だけはずす
		int c = j->orgName.ReverseFind('.');
		if (c != -1) orgNameBase = j->orgName.Left(c);
		else orgNameBase = j->orgName;
		
		CString orgNameDir;		// ラストの\をはずす
		if (j->dir.GetLength() != 0) {
			if (j->dir.GetAt(j->dir.GetLength()-1) == '\\') {
				orgNameDir = j->dir.Left(j->dir.GetLength()-2);
			}
			else {
				orgNameDir = j->dir;
			}
		}
		else {
			orgNameDir = "";
		}

		makeDcimFileName(j->newName, j->shotTime, naming_text, now, nykExUpper(nykGetStrExt(j->orgName), ex_upper), orgNameBase, j->maker,j->model, nykExUpper(nykGetStrExt(j->orgName), ex_upper), counter_by_day);
		makeDcimFileName(j->newThumbName, j->shotTime, thumb_naming_text, now, nykExUpper("JPG", ex_upper), orgNameBase, j->maker,j->model, nykExUpper("JPG", ex_upper), counter_by_day);

		if (j->fileFormat == NYK_FILE_OTHER_VIDEO) {
			for (int i = 0; i < c_outDirsVideo; i++) {
				makeDcimFileName(j->newDirEx[i], j->shotTime, naming_video_dirs[i], now, "", j->dir, j->maker,j->model, nykExUpper(nykGetStrExt(j->orgName), ex_upper), counter_by_day);
				if (i == 0) j->newDir = j->newDirEx[i];
			}
		}
		else {
			for (int i = 0; i < c_outDirs; i++) {
				makeDcimFileName(j->newDirEx[i], j->shotTime, naming_dirs[i], now, "", j->dir, j->maker,j->model, nykExUpper(nykGetStrExt(j->orgName), ex_upper), counter_by_day);
				if (i == 0) j->newDir = j->newDirEx[i];
			}
		}

		makeDcimFileName(j->newThumbDir, j->shotTime, thumb_naming_dir, now, "", j->dir, j->maker,j->model, nykExUpper(nykGetStrExt(j->orgName), ex_upper), counter_by_day);
		TRACE("** %s %s %s -> %s %s\n", j->model, j->dir, j->orgName, j->newDir, j->newName);
	}
//	sort(dcim.begin(), dcim.end(), pr_dcim);
//	sort(dcim.begin(), dcim.end(), pr_dcim_name);
/*	sort(dcim.begin(), dcim.end(), pr_dcim_name);

	TRACE("-- SORTED2 ---\n");
	for (j = dcim.begin(); j != dcim.end(); j++) {
		TRACE("S2 %s %s fs:%d t:%d st:%d\n", j->dir, j->orgName, j->filesize, j->fileTime, j->shotTime);
	}

	sort(dcim.begin(), dcim.end(), pr_dcim);

	TRACE("-- SORTED3 ---\n");
	for (j = dcim.begin(); j != dcim.end(); j++) {
		TRACE("S3 %s %s fs:%d t:%d st:%d\n", j->dir, j->orgName, j->filesize, j->fileTime, j->shotTime);
	}*/

	sort(dcim.begin(), dcim.end(), pr_dcim_time2name);

}

// ,区切り拡張子文字列のエラーチェックをする 何もないときは JPG 追加する
void nykRegularExtensions(CString &extensions_in)
{
	CString out;

	extensions_in.Remove(' ');

	if (extensions_in.GetLength() == 0) { extensions_in = "JPG"; return; }
	vector<CString> ret;
	StringToArray(extensions_in, ",", ret);
	if (ret.size() == 0) { extensions_in = "JPG"; return; }

	out = "";
	vector<CString>::iterator i;
	for (i = ret.begin(); i != ret.end(); i++) {
		TRACE("[%s]%d", *i, i->GetLength());
		if (i->GetLength() == 0) continue;
		if (i->Find("?") != -1) continue; // ワイルドカードはつかえない
		if (i->Find("*") != -1) continue; // ワイルドカードはつかえない
		i->TrimLeft();
		i->TrimRight();
		out += *i;
		out += ",";
	}
	TRACE("\n");
	if (out.GetLength() == 0) { extensions_in = "JPG"; return; }
	if (out.GetAt(out.GetLength()-1) == ',') {
		CString tmp = out.Left(out.GetLength() -1);
		out = tmp;
	}
	extensions_in = out;
	extensions_in.MakeUpper();
}

void nykRegularExtensionsVideo(CString &extensions_in)
{
	CString out;

	extensions_in.Remove(' ');

	if (extensions_in.GetLength() == 0) {  return; }
	vector<CString> ret;
	StringToArray(extensions_in, ",", ret);
	if (ret.size() == 0) { extensions_in = ""; return; }

	out = "";
	vector<CString>::iterator i;
	for (i = ret.begin(); i != ret.end(); i++) {
		TRACE("[%s]%d", *i, i->GetLength());
		if (i->GetLength() == 0) continue;
		if (i->Find("?") != -1) continue; // ワイルドカードはつかえない
		if (i->Find("*") != -1) continue; // ワイルドカードはつかえない
		i->TrimLeft();
		i->TrimRight();
		out += *i;
		out += ",";
	}
	TRACE("\n");
	if (out.GetLength() == 0) { extensions_in = ""; return; }
	if (out.GetAt(out.GetLength()-1) == ',') {
		CString tmp = out.Left(out.GetLength() -1);
		out = tmp;
	}
	extensions_in = out;
	extensions_in.MakeUpper();
}


// 対象となる拡張子をセットする
void CDcimList::SetExtensions(const CString &extensions_in)
{
	extensions = extensions_in;
	nykRegularExtensions(extensions);
}

void CDcimList::SetExtensionsVideo(const CString &extensions_in)
{
	extensions_video = extensions_in;
	nykRegularExtensionsVideo(extensions_video);
}

