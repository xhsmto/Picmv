// picmove_en.h : PICMOVE_EN アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_PICMOVE_EN_H__0B23D181_9EBE_49F9_827D_9CF8A4854645__INCLUDED_)
#define AFX_PICMOVE_EN_H__0B23D181_9EBE_49F9_827D_9CF8A4854645__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CPicmove_enApp
// このクラスの動作の定義に関しては picmove_en.cpp ファイルを参照してください。
//

class CPicmove_enApp : public CWinApp
{
public:
	CPicmove_enApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPicmove_enApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CPicmove_enApp)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_PICMOVE_EN_H__0B23D181_9EBE_49F9_827D_9CF8A4854645__INCLUDED_)
