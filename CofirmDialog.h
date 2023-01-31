#if !defined(AFX_COFIRMDIALOG_H__B2C3A39D_12E1_4D32_A709_9432CA7B3226__INCLUDED_)
#define AFX_COFIRMDIALOG_H__B2C3A39D_12E1_4D32_A709_9432CA7B3226__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CofirmDialog.h : �w�b�_�[ �t�@�C��
//
#include "DcimList.h"

/////////////////////////////////////////////////////////////////////////////
// CCofirmDialog �_�C�A���O

class CCofirmDialog : public CDialog {
	// �R���X�g���N�V����
public:
	CCofirmDialog(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	// �_�C�A���O �f�[�^
		//{{AFX_DATA(CCofirmDialog)
	enum { IDD = IDD_COFIRMDIALOG_DIALOG };
	CListCtrl	m_listConfirm;
	CStatic	m_confirmArea;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CCofirmDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CCofirmDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void SetData(CDcimList& dcimList_in);
	// 0=cancel 1=ok -1=idle
	inline int GetResult(void) { return result; }
	inline void SetResult(int result_in) { result = result_in; }
private:
	//	CString text;
	int result;
	CBitmap confirmArea_bitmap;
	CBitmap* old_confirmArea_bitmap;
	CDC confirmArea_memdc;
	CDcimList dcimList;
	void drawConfirmArea(void);
	void transConfirmArea(void);
	bool bInit;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_COFIRMDIALOG_H__B2C3A39D_12E1_4D32_A709_9432CA7B3226__INCLUDED_)
