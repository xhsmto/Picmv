// NamigRulesDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "picmove.h"
#include "NamigRulesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNamigRulesDlg ダイアログ


CNamigRulesDlg::CNamigRulesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNamigRulesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNamigRulesDlg)
	//}}AFX_DATA_INIT
}


void CNamigRulesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNamigRulesDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNamigRulesDlg, CDialog)
	//{{AFX_MSG_MAP(CNamigRulesDlg)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNamigRulesDlg メッセージ ハンドラ

void CNamigRulesDlg::SetHelpText(const CString &text_in)
{
	this->SetDlgItemText(IDC_EDIT_NAMING_RULES, text_in);
	InvalidateRect(NULL, false);
	UpdateWindow();
}	

void CNamigRulesDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
//	AfxMessageBox("Destroy");
	
}

void CNamigRulesDlg::OnClose() 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
//	AfxMessageBox("Close");
//	CDialog::OnClose();
	this->ShowWindow(SW_HIDE);
}
