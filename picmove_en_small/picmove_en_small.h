// picmove_en_small.h : PICMOVE_EN_SMALL アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_PICMOVE_EN_SMALL_H__651D58A5_CB08_4BCB_BF1A_597686475851__INCLUDED_)
#define AFX_PICMOVE_EN_SMALL_H__651D58A5_CB08_4BCB_BF1A_597686475851__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CPicmove_en_smallApp
// このクラスの動作の定義に関しては picmove_en_small.cpp ファイルを参照してください。
//

class CPicmove_en_smallApp : public CWinApp
{
public:
	CPicmove_en_smallApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPicmove_en_smallApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CPicmove_en_smallApp)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_PICMOVE_EN_SMALL_H__651D58A5_CB08_4BCB_BF1A_597686475851__INCLUDED_)
