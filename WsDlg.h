#if !defined(AFX_WSDLG_H__C4A51F9B_BBB5_4255_B50C_9D965DF2BFB6__INCLUDED_)
#define AFX_WSDLG_H__C4A51F9B_BBB5_4255_B50C_9D965DF2BFB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WsDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CWsDlg �_�C�A���O

class CWsDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CWsDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CWsDlg)
	enum { IDD = IDD_DIALOG_WIDOWSIZE };
	BOOL	m_CHECK_SMALLWINDOW;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CWsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CWsDlg)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ����܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_WSDLG_H__C4A51F9B_BBB5_4255_B50C_9D965DF2BFB6__INCLUDED_)
