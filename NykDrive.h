// NykDrive.h: CNykDrive クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NYKDRIVE_H__6178AB81_28D4_4932_A9E1_85BD605CBF99__INCLUDED_)
#define AFX_NYKDRIVE_H__6178AB81_28D4_4932_A9E1_85BD605CBF99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace std;

class driveElement {
public:
	driveElement();
	~driveElement();
	char driveLetter;
	bool fMount;
	bool fDcim;
	int type;
//	long mediaSize, remainSize;

};

class CNykDrive  
{
private:
	vector<driveElement> drives_local;
public:
	CNykDrive();
	virtual ~CNykDrive();
	
// リムーバルドライブのみをオブジェクトに格納する
	bool setDriveInfoRemovableOnly(void);
// ドライブ数を取得する
	int getNumberDrive(void);
// ドライブ情報を取得する
	bool getDriveInfo(int index, driveElement *element_in);
};

#endif // !defined(AFX_NYKDRIVE_H__6178AB81_28D4_4932_A9E1_85BD605CBF99__INCLUDED_)
