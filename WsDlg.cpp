// WsDlg.cpp : インプリメンテーション ファイル

#include "stdafx.h"
#include "picmove.h"
#include "WsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWsDlg ダイアログ

CWsDlg::CWsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWsDlg::IDD, pParent) {
	//{{AFX_DATA_INIT(CWsDlg)
	m_CHECK_SMALLWINDOW = FALSE;
	//}}AFX_DATA_INIT
}

void CWsDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWsDlg)
	DDX_Check(pDX, IDC_CHECK_SMALLWINDOW, m_CHECK_SMALLWINDOW);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWsDlg, CDialog)
	//{{AFX_MSG_MAP(CWsDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWsDlg メッセージ ハンドラ
