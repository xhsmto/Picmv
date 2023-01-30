// picmove.h : PICMOVE アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_PICMOVE_H__B0349E29_331D_4CE1_8590_4BE719E03512__INCLUDED_)
#define AFX_PICMOVE_H__B0349E29_331D_4CE1_8590_4BE719E03512__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CPicmoveApp:
// このクラスの動作の定義に関しては picmove.cpp ファイルを参照してください。
//

class CPicmoveApp : public CWinApp
{
private:
	HANDLE hMSP;    // 多重起動防止用Mutex Handle
	bool LoadMyStrings(void);

public:
	int bMiniSize;
	bool LoadLanguageDll(const CString &);
	bool requestStop;
//	int lang;
	CPicmoveApp();
	void saveIniFile(void);
	void loadIniFile(void);
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPicmoveApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CPicmoveApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CString NYK_STRINGS102;
extern CString NYK_STRINGS103;
extern CString NYK_STRINGS104;
extern CString NYK_STRINGS105;
extern CString NYK_STRINGS106;
extern CString NYK_STRINGS107;
extern CString NYK_STRINGS108;
extern CString NYK_STRINGS109;
extern CString NYK_STRINGS110;
extern CString NYK_STRINGS111;
extern CString NYK_STRINGS112;
extern CString NYK_STRINGS113;
extern CString NYK_STRINGS114;
extern CString NYK_STRINGS115;
extern CString NYK_STRINGS116;
extern CString NYK_STRINGS117;
extern CString NYK_STRINGS118;
extern CString NYK_STRINGS119;
extern CString NYK_STRINGS120;
extern CString NYK_STRINGS121;
extern CString NYK_STRINGS122;
extern CString NYK_STRINGS123;
extern CString NYK_STRINGS124;
extern CString NYK_STRINGS125;
extern CString NYK_STRINGS126;
extern CString NYK_STRINGS127;
extern CString NYK_STRINGS128;
extern CString NYK_STRINGS129;
extern CString NYK_STRINGS130;
extern CString NYK_STRINGS131;
extern CString NYK_STRINGS132;
extern CString NYK_STRINGS133;
extern CString NYK_STRINGS134;
extern CString NYK_STRINGS135;
extern CString NYK_STRINGS136;
extern CString NYK_STRINGS137;
extern CString NYK_STRINGS138;
extern CString NYK_STRINGS139;
extern CString NYK_STRINGS140;
extern CString NYK_STRINGS141;
extern CString NYK_STRINGS142;
extern CString NYK_STRINGS143;
extern CString NYK_STRINGS144;
extern CString NYK_STRINGS145;

extern CString NYK_STRINGS146;
extern CString NYK_STRINGS147;
extern CString NYK_STRINGS148;
extern CString NYK_STRINGS149;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_PICMOVE_H__B0349E29_331D_4CE1_8590_4BE719E03512__INCLUDED_)
