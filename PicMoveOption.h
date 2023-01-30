#if !defined(AFX_PICMOVEOPTION_H__0310541E_E975_4AEB_8D9D_96E43BE97F56__INCLUDED_)
#define AFX_PICMOVEOPTION_H__0310541E_E975_4AEB_8D9D_96E43BE97F56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PicMoveOption.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPicMoveOption ダイアログ

class CPicMoveOption : public CDialog
{
// コンストラクション
public:
	CPicMoveOption(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CPicMoveOption)
	enum { IDD = IDD_PICMOVEOPTION_DIALOG };
	int		m_detect_time;
	BOOL	m_bilinear;
	int		m_gl_time;
	int		m_jpeg_q;
	int		m_picbar_time;
	int		m_thumb_jpeg_q;
	CString	m_extensions;
	CString	m_extensions_video;
	CString	m_serialChar;
	CString	m_serialColumn;
	BOOL	m_glPicture;
	int		m_ex_upper;
	BOOL	m_SMARTREPLACE;
	BOOL	m_CHECK_PICGL_SUPPORT;
	CString	m_EDIT_PICGL_DIR;
	BOOL	m_ALWAYSSSN;
	BOOL	m_CHECK_OTHERTHANDCIM;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPicMoveOption)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPicMoveOption)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditExtensions();
	afx_msg void OnChangeEditSerialColumn();
	afx_msg void OnButtonPicglDir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_PICMOVEOPTION_H__0310541E_E975_4AEB_8D9D_96E43BE97F56__INCLUDED_)
