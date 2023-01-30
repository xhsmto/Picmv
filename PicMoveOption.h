#if !defined(AFX_PICMOVEOPTION_H__0310541E_E975_4AEB_8D9D_96E43BE97F56__INCLUDED_)
#define AFX_PICMOVEOPTION_H__0310541E_E975_4AEB_8D9D_96E43BE97F56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PicMoveOption.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CPicMoveOption �_�C�A���O

class CPicMoveOption : public CDialog
{
// �R���X�g���N�V����
public:
	CPicMoveOption(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
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


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CPicMoveOption)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CPicMoveOption)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditExtensions();
	afx_msg void OnChangeEditSerialColumn();
	afx_msg void OnButtonPicglDir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_PICMOVEOPTION_H__0310541E_E975_4AEB_8D9D_96E43BE97F56__INCLUDED_)
