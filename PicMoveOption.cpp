// PicMoveOption.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "picmove.h"
#include "PicMoveOption.h"
#include "DcimList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPicMoveOption ダイアログ

CPicMoveOption::CPicMoveOption(CWnd* pParent /*=NULL*/)
	: CDialog(CPicMoveOption::IDD, pParent) {
	//{{AFX_DATA_INIT(CPicMoveOption)
	m_detect_time = 0;
	m_bilinear = FALSE;
	m_gl_time = 0;
	m_jpeg_q = 0;
	m_picbar_time = 0;
	m_thumb_jpeg_q = 0;
	m_extensions = _T("");
	m_serialChar = _T("");
	m_serialColumn = _T("");
	m_glPicture = FALSE;
	m_ex_upper = -1;
	m_SMARTREPLACE = FALSE;
	m_CHECK_PICGL_SUPPORT = FALSE;
	m_EDIT_PICGL_DIR = _T("");
	m_ALWAYSSSN = FALSE;
	m_CHECK_OTHERTHANDCIM = FALSE;
	//}}AFX_DATA_INIT
}

void CPicMoveOption::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPicMoveOption)
	DDX_Slider(pDX, IDC_SLIDER_DETECT_TIME, m_detect_time);
	DDX_Check(pDX, IDC_CHECK_BILINEAR, m_bilinear);
	DDX_Slider(pDX, IDC_SLIDER_GL_TIME, m_gl_time);
	DDX_Slider(pDX, IDC_SLIDER_JPEG_Q, m_jpeg_q);
	DDX_Slider(pDX, IDC_SLIDER_PICBAR_TIME, m_picbar_time);
	DDX_Slider(pDX, IDC_SLIDER_THUMB_JPEG_Q, m_thumb_jpeg_q);
	DDX_Text(pDX, IDC_EDIT_EXTENSIONS, m_extensions);
	DDX_Text(pDX, IDC_EDIT_EXTENSIONS_VIDEO, m_extensions_video);
	DDV_MaxChars(pDX, m_extensions, 256);
	DDX_Text(pDX, IDC_EDIT_SERIAL_CHAR, m_serialChar);
	DDV_MaxChars(pDX, m_serialChar, 255);
	DDX_Text(pDX, IDC_EDIT_SERIAL_COLUMN, m_serialColumn);
	DDV_MaxChars(pDX, m_serialColumn, 1);
	DDX_Check(pDX, IDC_CHECK_GLPICTURE, m_glPicture);
	DDX_Radio(pDX, IDC_RADIO_EX_UPPER, m_ex_upper);
	DDX_Check(pDX, IDC_CHECK_SMARTREPLACE, m_SMARTREPLACE);
	DDX_Check(pDX, IDC_CHECK_PICGL_SUPPORT, m_CHECK_PICGL_SUPPORT);
	DDX_Text(pDX, IDC_EDIT_PICGL_DIR, m_EDIT_PICGL_DIR);
	DDX_Check(pDX, IDC_CHECK_ALWAYSSN, m_ALWAYSSSN);
	DDX_Check(pDX, IDC_CHECK_OTHERTHANDCIM, m_CHECK_OTHERTHANDCIM);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPicMoveOption, CDialog)
	//{{AFX_MSG_MAP(CPicMoveOption)
	ON_EN_CHANGE(IDC_EDIT_EXTENSIONS, OnChangeEditExtensions)
	ON_EN_CHANGE(IDC_EDIT_SERIAL_COLUMN, OnChangeEditSerialColumn)
	ON_BN_CLICKED(IDC_BUTTON_PICGL_DIR, OnButtonPicglDir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPicMoveOption メッセージ ハンドラ



BOOL CPicMoveOption::OnInitDialog() {
	CDialog::OnInitDialog();
	//	m_detect_time_c.SetRange(0, 2);

	return false;
}

void CPicMoveOption::OnChangeEditExtensions() {
	// TODO: これが RICHEDIT コントロールの場合、コントロールは、 lParam マスク
	// 内での論理和の ENM_CHANGE フラグ付きで CRichEditCrtl().SetEventMask()
	// メッセージをコントロールへ送るために CDialog::OnInitDialog() 関数をオーバー
	// ライドしない限りこの通知を送りません。

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
//	TRACE("%s\n", m_extensions);
	CString ext;
	GetDlgItemText(IDC_EDIT_EXTENSIONS, ext);
	TRACE("[%s]->", ext);
	nykRegularExtensions(ext);
	TRACE("[%s]\n", ext);
}

void CPicMoveOption::OnChangeEditSerialColumn() {
	// TODO: これが RICHEDIT コントロールの場合、コントロールは、 lParam マスク
	// 内での論理和の ENM_CHANGE フラグ付きで CRichEditCrtl().SetEventMask()
	// メッセージをコントロールへ送るために CDialog::OnInitDialog() 関数をオーバー
	// ライドしない限りこの通知を送りません。

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString ext;
	GetDlgItemText(IDC_EDIT_SERIAL_COLUMN, ext);
	ext.TrimLeft();
	ext.TrimRight();
	int c = atoi(ext);
	if(c <= 1) c = 1;
	if(c >= 6) c = 6;
	ext.Format("%d", c);
	SetDlgItemText(IDC_EDIT_SERIAL_COLUMN, ext);

}

void CPicMoveOption::OnButtonPicglDir() {
	CString folder, default_folder, title_text;

	UpdateData(TRUE);
	//	title_text = "入力元ディレクトリの追加";
	title_text = NYK_STRINGS141;

	default_folder = this->m_EDIT_PICGL_DIR;
	if(nykGetFolder(this->m_hWnd, folder, default_folder, title_text)) {
		if(folder != "") {
			m_EDIT_PICGL_DIR = folder;
			UpdateData(FALSE);
		}
	}
}
