// picmoveDlg.h : �w�b�_�[ �t�@�C��
//


#if !defined(AFX_PICMOVEDLG_H__05E6BBFF_6931_41CB_A0B2_41A91D88F5AC__INCLUDED_)
#define AFX_PICMOVEDLG_H__05E6BBFF_6931_41CB_A0B2_41A91D88F5AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DcimList.h"
#include "glType.h"

#include <vector>
using namespace std;

#include "CofirmDialog.h"
#include "NamigRulesDlg.h"
//#include "PicProgress1.h"
//#include "nykbutton.h"

enum {
	THUMB_METHOD_WIDTH,
	THUMB_METHOD_HEIGHT,
	THUMB_METHOD_WH
};

/////////////////////////////////////////////////////////////////////////////
// CPicmoveDlg �_�C�A���O

class CPicmoveDlg : public CDialog
{
private:

// �_�C�A���O�̃f�[�^
	CString indir, outdir, thumbdir, video_dir;
	CString naming, thumbnaming;
	int b_thumb, b_deleteok, b_autocontrast, b_orientation, b_eject, b_autoquit;
	int thumbwidth;
	int thumbheight;
	COLORREF thumbpad;
	int thumb_method;
	CString auto_indir;
	int b_auto_detect, b_manual_detect; //IDC_RADIO_AUTO_DETECT
	int b_add_serial, b_nocopy, b_overwrite;
	int b_confirm; 
	// add 2008.7.10 
	int b_auto_start;
	int b_video_dir;
	int m_counter_byday, m_counter_odd, m_counter_exec;
	time_t m_last_exec;
// �ڍ׃I�v�V����
	int m_detect_time;
	int m_bilinear;
	int m_gl_time ;
	int m_jpeg_q ;
	int m_picbar_time;
	int m_thumb_jpeg_q;
	int m_ex_upper;	// 0=������ 1=�啶�� 2=���̂܂�
	int m_SMARTREPLACE;
	int m_ALWAYSSN;
//	int m_speed;
	CString m_extensions;
	CString m_extensions_video;
	CString m_serialChar;
	int m_serialColumn;
	int m_glPicture;
// Picgl �֌W
	int m_CHECK_PICGL_SUPPORT;
	CString m_EDIT_PICGL_DIR;

	int m_CHECK_OTHERTHANDCIM;


//	bool force_inhibit_auto_detect;
// �_�C�A���O�̃f�[�^�����܂�
// ���܃R�s�[���Ă��邩�ǂ���
	bool nowCoping;
	CDcimList DcimList;
// �v���O���X�p
	CBitmap prg_bitmap;
	CBitmap *old_prg_bitmap;
	CDC prg_memdc;
	CBitmap prgbar_bitmap;
	CBitmap *old_prgbar_bitmap;
	CDC prgbar_memdc;
	double prgbar_percent, prgbar_local;
	int prgbarCounter;

// �f�W�^���p
	int digital_counter;
	// 0=remain 1=percent 2=normal
	int digital_mode;
	CBitmap digital_bitmap;
	CBitmap *old_digital_bitmap;
	CDC digital_memdc;
// �f�W�^���\�[�X�p
	CBitmap digital_rsrc;
	CDC digital_rsrc_memdc;
	CBitmap *old_digital_rsrc;

// GL�p

	CglType gl;
// popup�p
	int popup_height;

	// �h���b�O�A���h�h���b�v�p
	int dropFileNum;
	vector<CString> dropFile;

	void SetTimers(void);
	void StartThread(void);
	bool ok_scroll;

	CCofirmDialog confirmDialog;
	CNamigRulesDlg rulesDlg;
	void PreviewNaming(bool);
	time_t sampleTime;
	HACCEL m_acl;       //����̓w�b�_�Ő錾���Ă���

	bool bAutoExec;
	CString old_type_string;
public:
	CPicmoveDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

	inline int getDigitalCounter(void) { return digital_counter; }
	inline void setDigitalCounter(int cnt_in) { digital_counter = cnt_in; }
	void getDialogParam(void);
	void setDialogParam(void);
	void saveIniFile(void);
	void saveIniFileForExit(void);
	void loadIniFile(void);
	void setDialogParamAutoInDirOnly(void);
	bool mediaDetected(CString &media);
	bool autoStart(void);
	void updateGrayEjectButton(void);
	static void theThread(void *ptr);
	static void glThread(void *ptr);
	static void stopTheThread(void *ptr);
	void updateGrayButtons(void);
	void prgPaint(void);
	void prgbarPaint(void);	// paint��memdc������ۂɓ]������
	void prgbarDraw(void);	// draw��memdc�ɏ�������
	void digitalPaint(void);
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CPicmoveDlg)
	enum { IDD = IDD_PICMOVE_DIALOG };
	CComboBox	m_COMBO_THUMBNAIL;
	CStatic	m_STATIC_THUMB_COLOR;
	CEdit	m_EDIT_OUTDIR_CTL;
	CStatic	m_digital;
	CButton	m_button_exec;
	CStatic	m_gl;
	CStatic	m_picbar;
	CStatic	m_picture_box;
	CProgressCtrl	m_Progress;
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CPicmoveDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CPicmoveDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonExec();
	afx_msg void OnButtonSave();
	afx_msg void OnCheckThumbnail();
	afx_msg void OnCheckVideoDir();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnButtonStop();
	afx_msg void OnRadioAutoDetect();
	afx_msg void OnRadioManualDetect();
	afx_msg void OnButtonEject();
	afx_msg void OnDestroy();
	afx_msg void OnChangeEditIndir();
	afx_msg void OnGl();
	afx_msg void OnButtonReferIndir();
	afx_msg void OnButtonReferOutdir();
	afx_msg void OnButtonReferThumbdir();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonHelpAll();
	afx_msg void OnButtonOption();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnStaticDigital();
	afx_msg void OnChangeEditOutdir();
	afx_msg void OnChangeEditNaming();
	afx_msg void OnChangeEditThumbDir();
	afx_msg void OnChangeEditThumbnaming();
	afx_msg void OnPictureBox();
	afx_msg void OnRadioOverwrite();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnButtonNamingHelp();
	afx_msg void OnButtonNamingHelp2();
	afx_msg void OnEditchangeComboThumbnail();
	afx_msg void OnSelchangeComboThumbnail();
	afx_msg void OnStaticThumbColor();
	afx_msg void OnMenuitemExecute();
	afx_msg void OnMenuitemExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool DoParseCommandLine();
// �c�[���`�b�v�֌W
private:
	CToolTipCtrl NamingRuleTip;
private:
	void nykGetDcimDirs(CString &dir);
	void AddNotifyIcon();
	HWND m_hWndCopy;
	CMenu m_menuRightClickTrayIcon;
	//	HICON m_hIcon;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_PICMOVEDLG_H__05E6BBFF_6931_41CB_A0B2_41A91D88F5AC__INCLUDED_)
