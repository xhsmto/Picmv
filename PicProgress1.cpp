// PicProgress1.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "picmove.h"
#include "PicProgress1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPicProgress

CPicProgress::CPicProgress()
{
	
}

CPicProgress::~CPicProgress()
{

}


BEGIN_MESSAGE_MAP(CPicProgress, CWnd)
	//{{AFX_MSG_MAP(CPicProgress)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPicProgress メッセージ ハンドラ

void CPicProgress::OnTimer(UINT nIDEvent) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	

	CWnd::OnTimer(nIDEvent);
}

void CPicProgress::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	
	// 描画用メッセージとして CWnd::OnPaint() を呼び出してはいけません

	CRect rect;
	this->GetWindowRect(&rect);
	dc.FillSolidRect(&rect, RGB(255,10,10));

}

void CPicProgress::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	CWnd::OnLButtonDown(nFlags, point);
}

BOOL CPicProgress::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
//	return CWnd::PreCreateWindow(cs);

	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.style = WS_CHILD | WS_VISIBLE;
	cs.lpszClass = AfxRegisterWndClass(0, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(GetStockObject(WHITE_BRUSH)), NULL);

	return TRUE;

}
