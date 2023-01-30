#if !defined(AFX_WSDLG_H__C4A51F9B_BBB5_4255_B50C_9D965DF2BFB6__INCLUDED_)
#define AFX_WSDLG_H__C4A51F9B_BBB5_4255_B50C_9D965DF2BFB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WsDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CWsDlg ダイアログ

class CWsDlg : public CDialog
{
// コンストラクション
public:
	CWsDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CWsDlg)
	enum { IDD = IDD_DIALOG_WIDOWSIZE };
	BOOL	m_CHECK_SMALLWINDOW;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CWsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CWsDlg)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_WSDLG_H__C4A51F9B_BBB5_4255_B50C_9D965DF2BFB6__INCLUDED_)
