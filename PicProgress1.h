#if !defined(AFX_PICPROGRESS1_H__D15C2C98_2A8E_4F79_B0C2_6BD45C12B071__INCLUDED_)
#define AFX_PICPROGRESS1_H__D15C2C98_2A8E_4F79_B0C2_6BD45C12B071__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PicProgress1.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CPicProgress �E�B���h�E

class CPicProgress : public CWnd
{
// �R���X�g���N�V����
public:
	CPicProgress();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

	void Regist(const CString& fileName);
	void Clear(void);



// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CPicProgress)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CPicProgress();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
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
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_PICPROGRESS1_H__D15C2C98_2A8E_4F79_B0C2_6BD45C12B071__INCLUDED_)

