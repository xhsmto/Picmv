// PicProgress1.cpp : �C���v�������e�[�V���� �t�@�C��
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
// CPicProgress ���b�Z�[�W �n���h��

void CPicProgress::OnTimer(UINT nIDEvent) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	

	CWnd::OnTimer(nIDEvent);
}

void CPicProgress::OnPaint() 
{
	CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	
	// �`��p���b�Z�[�W�Ƃ��� CWnd::OnPaint() ���Ăяo���Ă͂����܂���

	CRect rect;
	this->GetWindowRect(&rect);
	dc.FillSolidRect(&rect, RGB(255,10,10));

}

void CPicProgress::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
	CWnd::OnLButtonDown(nFlags, point);
}

BOOL CPicProgress::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	
//	return CWnd::PreCreateWindow(cs);

	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.style = WS_CHILD | WS_VISIBLE;
	cs.lpszClass = AfxRegisterWndClass(0, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(GetStockObject(WHITE_BRUSH)), NULL);

	return TRUE;

}
