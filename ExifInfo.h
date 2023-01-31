// ExifInfo.h: ExifInfo クラスのインターフェイス
// EXIF ファイルから情報を取り出すだけのクラス
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXIFINFO_H__342AFADC_91D1_4E60_9B98_32CB004691D7__INCLUDED_)
#define AFX_EXIFINFO_H__342AFADC_91D1_4E60_9B98_32CB004691D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <time.h>

class ExifInfo {
private:
	time_t shotTime;
	int orientation;
	int width, height;
	//	int fileSize;
	int errCode;
	bool swMotorola;
	int read2(unsigned char* buf);
	long read4(unsigned char* buf);
public:
	ExifInfo();
	virtual ~ExifInfo();
	// 名前 name を持つファイルを調査する。エラーが起きると 0 を返す。また getErrCode でエラー内容がわかる
	int loadExifFile(CString name);
	int loadExifFileLocal(CString name);
	inline time_t getShotTime(void) { return shotTime; }
	inline int getOrientation(void) { return orientation; }
	inline int getWidth(void) { return width; }
	inline int getHeight(void) { return height; }
	//	inline int getFileSize(void) { return fileSize; }
	inline int getErrCode(void) { return errCode; }
	CString maker;
	CString model;
};


#if 0
class ExifExtra {
private:
	bool bActive;
	bool bError;
public:
	ExifExtra();
	~ExifExtra();
	CString maker;
	CString model;
	int orientation;
	time_t shotTime;
	bool Get(const CString& fullpath, bool bForce = false);
	CString fullpath;
};
#endif

#endif // !defined(AFX_EXIFINFO_H__342AFADC_91D1_4E60_9B98_32CB004691D7__INCLUDED_)
