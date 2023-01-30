#if !defined(AFX_NAMIGRULESDLG_H__ACF8EF7D_7F03_4451_9428_891995E8F1BC__INCLUDED_)
#define AFX_NAMIGRULESDLG_H__ACF8EF7D_7F03_4451_9428_891995E8F1BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NamigRulesDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CNamigRulesDlg ダイアログ

class CNamigRulesDlg : public CDialog
{
// コンストラクション
public:
	CNamigRulesDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CNamigRulesDlg)
	enum { IDD = IDD_DIALOG_NAMING_RULES };
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CNamigRulesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CNamigRulesDlg)
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void SetHelpText(const CString &text_in);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_NAMIGRULESDLG_H__ACF8EF7D_7F03_4451_9428_891995E8F1BC__INCLUDED_)
