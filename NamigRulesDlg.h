#if !defined(AFX_NAMIGRULESDLG_H__ACF8EF7D_7F03_4451_9428_891995E8F1BC__INCLUDED_)
#define AFX_NAMIGRULESDLG_H__ACF8EF7D_7F03_4451_9428_891995E8F1BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NamigRulesDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CNamigRulesDlg �_�C�A���O

class CNamigRulesDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CNamigRulesDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CNamigRulesDlg)
	enum { IDD = IDD_DIALOG_NAMING_RULES };
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CNamigRulesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CNamigRulesDlg)
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void SetHelpText(const CString &text_in);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_NAMIGRULESDLG_H__ACF8EF7D_7F03_4451_9428_891995E8F1BC__INCLUDED_)
