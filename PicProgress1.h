#if !defined(AFX_PICPROGRESS1_H__D15C2C98_2A8E_4F79_B0C2_6BD45C12B071__INCLUDED_)
#define AFX_PICPROGRESS1_H__D15C2C98_2A8E_4F79_B0C2_6BD45C12B071__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PicProgress1.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPicProgress ウィンドウ

class CPicProgress : public CWnd
{
// コンストラクション
public:
	CPicProgress();

// アトリビュート
public:

// オペレーション
public:

	void Regist(const CString& fileName);
	void Clear(void);



// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CPicProgress)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CPicProgress();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CPicProgress)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_PICPROGRESS1_H__D15C2C98_2A8E_4F79_B0C2_6BD45C12B071__INCLUDED_)

