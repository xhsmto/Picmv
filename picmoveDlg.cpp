// picmoveDlg.cpp : インプリメンテーション ファイル
// cximage.lib jasper.lib Jpeg.lib png.lib Tiff.lib zlib.lib glu32.lib glaux.lib opengl32.lib
#include <vector>
using namespace std;

#include "stdafx.h"
#include "DcimList.h"
#include "picmove.h"
#include "picmoveDlg.h"
#include "ExifInfo.h"
#include "eject.h"
#include "NykDrive.h"
#include "glType.h"
#include "picmoveOption.h"
#include "CofirmDialog.h"
#include "picmove.h"

#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include "PicglExport.h"
#include "WsDlg.h"
#include "NamigRulesDlg.h"
#include "version.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*enum {
	TIMER_ID_PRG = 101,
	TIMER_ID_PRGBAR,
	TIMER_ID_DETECT
};*/
#define TIMER_ID_PRG 101
#define TIMER_ID_PRGBAR 102
#define TIMER_ID_DETECT 103
#define TIMER_ID_GLDRAW 104
#define TIMER_ID_AUTOQUIT1 105
#define TIMER_ID_AUTOQUIT2 106
//#define TIMER_ID_START_MESSAGE 105

static const int scroll_pic_max_height = 1200;


static void nykEnableWindow(CWnd *pCwnd, bool enable)
{
	if (pCwnd) pCwnd->EnableWindow(enable);
}

void CPicmoveDlg::nykGetDcimDirs(CString &dir)
{
	dir = "";
	CNykDrive d;
	d.setDriveInfoRemovableOnly();
	for (int i = 0; i < d.getNumberDrive(); i++) {
		driveElement e;
		d.getDriveInfo(i, &e);
		CString d_text;
		if (e.fMount && e.fDcim) {
			if (this->m_CHECK_OTHERTHANDCIM) {
				d_text.Format("%c:\\", e.driveLetter);
			}
			else {
				d_text.Format("%c:\\DCIM\\", e.driveLetter);
			}
			dir += d_text;
			if (i <= d.getNumberDrive() - 2) dir+= ",";
		}
	}
	// 最後の文字が「,」だったら削除する
	while (1) {
		int len = dir.GetLength();
		if (len <= 0) break;
		if (dir.GetAt(len - 1)  == ',') {
			CString tmp = dir;
			dir = tmp.Left(len - 1);
		}
		else {
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われている CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_EDIT_X32X64;
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_EDIT_X32X64 = _T("");
	//}}AFX_DATA_INIT

#ifdef NYK_X64
	m_EDIT_X32X64 = _T("Picmv(x64) " NYK_VERSION_NUMBER " Build " __DATE__ " " __TIME__);
#else
	m_EDIT_X32X64 = _T("Picmv(x86) " NYK_VERSION_NUMBER " Build " __DATE__ " " __TIME__);
#endif

#ifdef NYK_RELEASEZIP
	m_EDIT_X32X64 += " ZIP";
#endif
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_EDIT_X32X64, m_EDIT_X32X64);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// メッセージ ハンドラがありません。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPicmoveDlg ダイアログ

CPicmoveDlg::CPicmoveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPicmoveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPicmoveDlg)
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	this->nowCoping = false;
	this->popup_height = 0;
	prgbar_percent = 0;
	prgbar_local = 0;
	dropFileNum = 0;	// ドラッグアンドドロップではない
	b_confirm = 1;
	ok_scroll = true;
	digital_counter = 0;
	digital_mode = 0;
	time(&sampleTime);
	bAutoExec = false;
	old_type_string = "";
//	force_inhibit_auto_detect = false;
}

void CPicmoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPicmoveDlg)
	DDX_Control(pDX, IDC_COMBO_THUMBNAIL, m_COMBO_THUMBNAIL);
	DDX_Control(pDX, IDC_STATIC_THUMB_COLOR, m_STATIC_THUMB_COLOR);
	DDX_Control(pDX, IDC_EDIT_OUTDIR, m_EDIT_OUTDIR_CTL);
	DDX_Control(pDX, IDC_STATIC_DIGITAL, m_digital);
	DDX_Control(pDX, IDC_BUTTON_EXEC, m_button_exec);
	DDX_Control(pDX, IDC_GL, m_gl);
	DDX_Control(pDX, IDC_PICBAR, m_picbar);
	DDX_Control(pDX, IDC_PICTURE_BOX, m_picture_box);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPicmoveDlg, CDialog)
	//{{AFX_MSG_MAP(CPicmoveDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_EXEC, OnButtonExec)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_CHECK_THUMBNAIL, OnCheckThumbnail)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_RADIO_AUTO_DETECT, OnRadioAutoDetect)
	ON_BN_CLICKED(IDC_RADIO_MANUAL_DETECT, OnRadioManualDetect)
	ON_BN_CLICKED(IDC_BUTTON_EJECT, OnButtonEject)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_INDIR, OnChangeEditIndir)
	ON_BN_CLICKED(IDC_GL, OnGl)
	ON_BN_CLICKED(IDC_BUTTON_REFER_INDIR, OnButtonReferIndir)
	ON_BN_CLICKED(IDC_BUTTON_REFER_OUTDIR, OnButtonReferOutdir)
	ON_BN_CLICKED(IDC_BUTTON_REFER_THUMBDIR, OnButtonReferThumbdir)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_HELP_ALL, OnButtonHelpAll)
	ON_BN_CLICKED(IDC_BUTTON_OPTION, OnButtonOption)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_STATIC_DIGITAL, OnStaticDigital)
	ON_EN_CHANGE(IDC_EDIT_OUTDIR, OnChangeEditOutdir)
	ON_EN_CHANGE(IDC_EDIT_NAMING, OnChangeEditNaming)
	ON_EN_CHANGE(IDC_EDIT_THUMB_DIR, OnChangeEditThumbDir)
	ON_EN_CHANGE(IDC_EDIT_THUMBNAMING, OnChangeEditThumbnaming)
	ON_BN_CLICKED(IDC_PICTURE_BOX, OnPictureBox)
	ON_BN_CLICKED(IDC_RADIO_OVERWRITE, OnRadioOverwrite)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_BUTTON_NAMING_HELP, OnButtonNamingHelp)
	ON_BN_CLICKED(IDC_BUTTON_NAMING_HELP2, OnButtonNamingHelp2)
	ON_CBN_EDITCHANGE(IDC_COMBO_THUMBNAIL, OnEditchangeComboThumbnail)
	ON_CBN_SELCHANGE(IDC_COMBO_THUMBNAIL, OnSelchangeComboThumbnail)
	ON_BN_CLICKED(IDC_STATIC_THUMB_COLOR, OnStaticThumbColor)
	ON_BN_CLICKED(IDC_CHECK_VIDEO_DIR, OnCheckVideoDir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPicmoveDlg メッセージ ハンドラ

/*

  メンバ関数の追加　OpenBitmapFile(CString strFileName) 
void CBmpLoaderView::OpenBitmapFile(CString strFileName)
{

BITMAP bmp; // ﾋﾞｯﾄﾏｯﾌﾟ構造体
HBITMAP hBmp; // ﾋﾞｯﾄﾏｯﾌﾟﾊﾝﾄﾞﾙ
CRect cClient; // ｸﾗｲｱﾝﾄ領域

// ｸﾗｲｱﾝﾄ領域を得る
GetClientRect(&cClient);

// ﾋﾞｯﾄﾏｯﾌﾟのﾊﾝﾄﾞﾙ取得
hBmp = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance,strFileName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
// ﾊﾝﾄﾞﾙのﾁｪｯｸ
if(hBmp == NULL)
{

nykMessageBox(_T("読み込みに失敗しました"));
return ;
}

// ﾒﾓﾘDCに割り当てられているﾋﾞｯﾄﾏｯﾌﾟを外す
dcMem.SelectObject(pOldBitmap);

// 先ほどまで割り当てられていたビットマップの情報を消去
pBitmap->DeleteObject(); 

// ビットマップハンドルからCBitmapのﾎﾟｲﾝﾀを取得
pBitmap = (CBitmap*)CBitmap::FromHandle(hBmp);

// ビットマップﾟをメモリDCに割り当て 
pOldBitmap = dcMem.SelectObject(pBitmap);

//更新処理
InvalidateRect(&cClient);
}  

解説

*/





BOOL CPicmoveDlg::OnInitDialog()
{

	// NG
TRACE("init start\n");

	CDialog::OnInitDialog();

	// "バージョン情報..." メニュー項目をシステム メニューへ追加します。

	// IDM_ABOUTBOX はコマンド メニューの範囲でなければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_OPTION, NYK_STRINGS103 /* "オプション..."*/);
			pSysMenu->AppendMenu(MF_STRING, IDM_SAVECONFIG, NYK_STRINGS104 /* "設定保存(&S)"*/);
			pSysMenu->AppendMenu(MF_STRING, IDM_STOP, NYK_STRINGS105 /* "中止" */);
			pSysMenu->AppendMenu(MF_STRING, IDM_EXECUTE, NYK_STRINGS106 /* "実行(&C)"*/);
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_MINIMIZE, NYK_STRINGS146 /* "ウィンドウサイズ"*/);
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_VISIT, "www6.plala.or.jp/nyk...");
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}


	m_acl = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));



	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定

	

//	AddNotifyIcon();
	// TODO: 特別な初期化を行う時はこの場所に追加してください。

	this->auto_indir = ""; // 
	this->loadIniFile();

	this->m_counter_exec++;
	{
		CTime t(this->m_last_exec);
		time_t nowt;
		time(&nowt);
		CTime now(nowt);
		this->m_last_exec = nowt;

		if (t.GetYear() == now.GetYear() && t.GetMonth() == now.GetMonth() && t.GetDay() == now.GetDay()) {
		}
		else {
			this->m_counter_byday = 1;
		}

	}

	if (!DoParseCommandLine()) {
		bAutoExec = false;	
	}

	if (this->b_auto_detect) {
		nykGetDcimDirs(this->auto_indir);
		this->setDialogParamAutoInDirOnly();
	}

TRACE("init m0\n");

	// メモリ上に仮想画面をつくる
	CDC *dc = this->GetDC();
	CRect rect;
	m_picture_box.GetClientRect(&rect);
	prg_memdc.CreateCompatibleDC(dc);	
	prg_bitmap.CreateCompatibleBitmap(dc, rect.Width(), rect.Height() + scroll_pic_max_height);
	old_prg_bitmap = prg_memdc.SelectObject(&prg_bitmap);

TRACE("init m1\n");
	// 初期画面をロードしてくる
	HBITMAP hBmp = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance,"Picmv.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	if(hBmp) {
		CBitmap *initialBitmap = (CBitmap*)CBitmap::FromHandle(hBmp);
		CBrush brush(initialBitmap);
		prg_memdc.FillRect(&rect, &brush);
		::DeleteObject(hBmp);
	}
	else {
		prg_memdc.FillSolidRect(0,0,rect.Width(), rect.Height() + scroll_pic_max_height, RGB(128,128,128));
	}
TRACE("init m1.5\n");

	m_picbar.GetClientRect(&rect);
	prgbar_memdc.CreateCompatibleDC(dc);	
	prgbar_bitmap.CreateCompatibleBitmap(dc, rect.Width(), rect.Height());
	old_prgbar_bitmap = prgbar_memdc.SelectObject(&prgbar_bitmap);
	prgbarDraw();

// digital
	// まずはリソースからよみこみ
	this->digital_rsrc.LoadBitmap(IDB_BITMAP_DIGITAL);
				CDC tmp_memdc;
	digital_rsrc_memdc.CreateCompatibleDC(dc);
	old_digital_rsrc = digital_rsrc_memdc.SelectObject(&digital_rsrc);

	// 仮想画面を作る
	this->m_digital.GetClientRect(&rect);
	digital_memdc.CreateCompatibleDC(dc);
	digital_bitmap.CreateCompatibleBitmap(dc, rect.Width(), rect.Height());
	old_digital_bitmap = digital_memdc.SelectObject(&digital_bitmap);
	// 描画する
	digitalPaint();

	this->ReleaseDC(dc);

TRACE("init m2\n");

	this->updateGrayButtons();
TRACE("init m3\n");

	// GL関係
	gl.InitGL(&this->m_gl);
	gl.SetHwnd(this->m_gl.GetSafeHwnd());
 TRACE("init m3.5\n");
	if (b_auto_detect) gl.Type(auto_indir);
	else gl.Type(indir);
	gl.DrawGL();

	
	// GL へmessageを仮投入
//	gl.SetMessage("PICMV");

TRACE("init m4\n");

	SetTimers();
//	_beginthread(glThread, 0, this);
TRACE("init m5\n");

//	CWinApp* pApp = AfxGetApp();
//	HICON hIconExec = pApp->LoadIcon(IDI_ICON_EXECUTE);
//	this->m_button_exec.SetIcon(hIconExec);

	confirmDialog.Create(IDD_COFIRMDIALOG_DIALOG, NULL);
	rulesDlg.Create(IDD_DIALOG_NAMING_RULES, NULL);

TRACE("init end\n");
	SetDlgItemText(IDC_STATIC_PROGRESS_TEXT, NYK_STRINGS107 /* "JPGファイルやフォルダのウィンドウへの\nドラッグアンドドロップも受け付けます" */);
	
	{
		NamingRuleTip.Create(this, TTS_ALWAYSTIP);
		NamingRuleTip.AddTool(GetDlgItem(IDC_BUTTON_NAMING_HELP), NYK_STRINGS148);
		NamingRuleTip.AddTool(GetDlgItem(IDC_BUTTON_NAMING_HELP2), NYK_STRINGS148);
		NamingRuleTip.SetDelayTime(0);
	}

	if (bAutoExec) {
		ShowWindow(SW_SHOWNORMAL);
		UpdateWindow();
		this->OnButtonExec();
	}

	gl.SetMessage("PICMV");

	return FALSE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

void CPicmoveDlg::SetTimers(void)
{
	this->SetTimer(TIMER_ID_PRG, m_picbar_time, NULL);
	this->SetTimer(TIMER_ID_PRGBAR, m_picbar_time, NULL);
	this->SetTimer(TIMER_ID_DETECT, m_detect_time, NULL);
	this->SetTimer(TIMER_ID_GLDRAW, m_gl_time, NULL);
//	this->SetTimer(TIMER_ID_START_MESSAGE, 1500, NULL);
}

void CPicmoveDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
//	switch (nID & 0xFFF0) {
	switch (nID) {
	case IDM_ABOUTBOX:
		{
			CAboutDlg dlgAbout;
			gl.SetMessage("1.51");
			dlgAbout.DoModal();
		}
		break;
	case IDM_OPTION:
		OnButtonOption();
		break;
	case IDM_EXECUTE:
		this->OnButtonExec();
		break;
	case IDM_SAVECONFIG:
		this->OnButtonSave();
		break;
	case IDM_VISIT:
		ShellExecute( NULL, NULL, "http://www6.plala.or.jp/nyk/Picmv.html", NULL, NULL, SW_SHOWDEFAULT);
		break;
	case IDM_MINIMIZE:
		{
			CWsDlg dlg;
			CPicmoveApp *app = (CPicmoveApp*)AfxGetApp();
			dlg.m_CHECK_SMALLWINDOW = app->bMiniSize;
			if (dlg.DoModal() == IDOK) {
				app->bMiniSize = dlg.m_CHECK_SMALLWINDOW;
				app->saveIniFile();
			}
		}
		break;
	default:
		{
			CDialog::OnSysCommand(nID, lParam);
		}
		break;
	}
}


void CPicmoveDlg::prgPaint(void)
{
	CDC *dc = this->m_picture_box.GetDC();
	CRect rect;

	this->m_picture_box.GetClientRect(&rect);
	dc->BitBlt(0,0,rect.Width(),rect.Height(),&this->prg_memdc,0,0,SRCCOPY);
//	CBrush br(RGB(0,0,0));
//	dc->FrameRect(rect, &br);	// あやしい
	this->ReleaseDC(dc);
}

void CPicmoveDlg::prgbarPaint(void)
{
	CDC *dc = this->m_picbar.GetDC();
	CRect rect, rect_inside;

	this->m_picbar.GetClientRect(&rect);
	dc->BitBlt(0,0,rect.Width(),rect.Height(),&this->prgbar_memdc,0,0,SRCCOPY);
	this->ReleaseDC(dc);
}



// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CPicmoveDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
		gl.DrawGL();
		prgPaint();
		prgbarDraw();
		prgbarPaint();
		digitalPaint();

		CDC *dc = this->m_STATIC_THUMB_COLOR.GetDC();
		CRect rect;
		m_STATIC_THUMB_COLOR.GetClientRect(&rect);
		if (thumb_method == THUMB_METHOD_WH) {
			dc->FillSolidRect(&rect, this->thumbpad);
		}
		else {
			dc->FillSolidRect(&rect, RGB(192,192,192));
		}
		this->ReleaseDC(dc);

	}
}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CPicmoveDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/*
	RGBQUAD GetPixelColor(long x,long y, bool bGetAlpha = true);
	void	SetPixelColor(long x,long y,RGBQUAD c, bool bSetAlpha = false);
	void	SetPixelColor(long x,long y,COLORREF cr);
  */
/*
static bool Autocontrast(const CString &filename, int jpeg_q)
{
	CxImage image;
	image.Load(filename);
	if (!(image.IsValid())) return false;

	int w = image.GetWidth();
	int h = image.GetHeight();

	int r0, r1, g1, g0, b1, b0;
	r0 = g0 = b0 = 255;
	r1 = g1 = b1 = 0;


	// まず最小値と最大値を調査
	int x,y;
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			RGBQUAD rgb;
			rgb = image.GetPixelColor(x, y);
			if (rgb.rgbRed < r0) r0 = rgb.rgbRed;
			if (rgb.rgbBlue < b0) b0 = rgb.rgbBlue;
			if (rgb.rgbGreen < g0) g0 = rgb.rgbGreen;
			if (rgb.rgbRed > r1) r1 = rgb.rgbRed;
			if (rgb.rgbBlue > b1) b1 = rgb.rgbBlue;
			if (rgb.rgbGreen > g1) g1 = rgb.rgbGreen;
		}
	}
	TRACE("R %d-%d G %d-%d B %d-%d\n", r0,r1, g0,g1, b0,b1);
	// 調査結果に基づいて画像をいじるよ
	int rw = r1-r0;
	int gw = g1-g0;
	int bw = b1-b0;
	if (rw == 0 || gw == 0 || bw == 0) return false;
	double rd, gd, bd;
	rd = 255.0 / rw;
	gd = 255.0 / gw;
	bd = 255.0 / bw;

	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			RGBQUAD rgb;
			rgb = image.GetPixelColor(x, y);
			rgb.rgbRed  = (rgb.rgbRed  - r0) * rd;
			rgb.rgbBlue = (rgb.rgbBlue - b0) * bd;
			rgb.rgbGreen= (rgb.rgbGreen- g0) * gd;
			image.SetPixelColor(x, y, rgb);
		}
	}

	image.SetJpegQuality(jpeg_q);
	image.Save(filename, CXIMAGE_FORMAT_JPG);

	return true;
}
*/

// ret 0->イジェクトに成功 -1→イジェクト可能なものがない 1以上→戻り値分のイジェクトに失敗
static int EjectDrives(CString dirList)
{
	// dirList はカンマ区切りなので整理する

	dirList.TrimLeft();
	dirList.TrimRight();

	vector<CString> dir;
	StringToArray(dirList, ",", dir);

	TRACE("EjectDrives %s\n", dirList);

	int c_fail = 0, c_success = 0;
	// ディレクトリのリストができたところでそれぞれの exif 情報をもとめつつ dcim に保存する
	vector<CString>::iterator i;
	for (i = dir.begin(); i != dir.end(); i++) {
		if (i->GetLength() <= 0) continue;
		int c = i->GetAt(0);
		char szRootName[] = " :\\";
		szRootName[0] = c;
		if (GetDriveType(szRootName) != DRIVE_REMOVABLE){
			continue;
		}
 
		TRACE("eject %c drive\n", c);
		if (!EjectVolume(c)) {
			c_fail++;
			CString eject_error_text;
			eject_error_text.Format(NYK_STRINGS108 /* "%c ドライブをイジェクトできませんでした\n現在他のプログラムで %c ドライブを参照してはいないでしょうか"*/,
				c,c);
			::AfxMessageBox(eject_error_text, MB_OK | MB_ICONEXCLAMATION);
		}
		else {
			c_success++;
		}
	}
	if (c_success == 0 && c_fail == 0) return -1;
	if (c_success >= 1 && c_fail == 0) return 0;
	return c_fail;
}


void CPicmoveDlg::saveIniFile(void)
{
	this->getDialogParam();

	CWinApp* pApp = AfxGetApp();
	CString section = "ProgramData";
	
	pApp->WriteProfileString(section, "indir", indir);
	pApp->WriteProfileString(section, "outdir", outdir);
	pApp->WriteProfileString(section, "thumbdir", thumbdir);
	pApp->WriteProfileString(section, "naming", naming);
	pApp->WriteProfileString(section, "thumbnaming", thumbnaming);
	pApp->WriteProfileString(section, "video_dir", video_dir);
	pApp->WriteProfileInt(section,		"thumbwidth", thumbwidth);
	pApp->WriteProfileInt(section,		"thumbheight", thumbheight);
	pApp->WriteProfileInt(section,		"thumbpad", this->thumbpad);
	pApp->WriteProfileInt(section,		"thumb_method", this->thumb_method);

	// チェックボタン
	pApp->WriteProfileInt(section, "b_thumb",			b_thumb);
	pApp->WriteProfileInt(section, "b_deleteok",		b_deleteok);
//	pApp->WriteProfileInt(section, "b_autocontrast",	b_autocontrast);
	pApp->WriteProfileInt(section, "b_orientation",		b_orientation);
	pApp->WriteProfileInt(section, "b_eject",			b_eject);
	pApp->WriteProfileInt(section, "b_autoquit",		b_autoquit);

	// ラジオボタン
	pApp->WriteProfileInt(section, "b_auto_detect",		b_auto_detect);
	pApp->WriteProfileInt(section, "b_manual_detect",	b_manual_detect);
	pApp->WriteProfileInt(section, "b_add_serial",		b_add_serial);
	pApp->WriteProfileInt(section, "b_nocopy",			b_nocopy);
	pApp->WriteProfileInt(section, "b_overwrite",			b_overwrite);
	pApp->WriteProfileInt(section, "b_confirm",			b_confirm);
	pApp->WriteProfileInt(section, "b_video_dir",	b_video_dir);	// 拡張子

	// 詳細オプション
	pApp->WriteProfileInt(section, "m_detect_time",		m_detect_time);
	pApp->WriteProfileInt(section, "m_gl_time",			m_gl_time);
	pApp->WriteProfileInt(section, "m_picbar_time",		m_picbar_time);
	pApp->WriteProfileInt(section, "m_thumb_jpeg_q",	m_thumb_jpeg_q);
	pApp->WriteProfileInt(section, "m_jpeg_q",			m_jpeg_q);
	pApp->WriteProfileInt(section, "m_bilinear",		m_bilinear);
//	pApp->WriteProfileInt(section, "m_speed",			m_speed);
	pApp->WriteProfileString(section, "m_serialChar",		m_serialChar);
	pApp->WriteProfileInt(section, "m_serialColumn",	m_serialColumn);
	pApp->WriteProfileInt(section, "m_glPicture",		m_glPicture);

	pApp->WriteProfileString(section, "m_extensions",	m_extensions);	// 拡張子
	pApp->WriteProfileString(section, "m_extensions_video",	m_extensions_video);	// 拡張子

	pApp->WriteProfileInt(section, "m_ex_upper",		m_ex_upper);	// 拡張子
	pApp->WriteProfileInt(section, "m_SMARTREPLACE",	m_SMARTREPLACE);	// 拡張子

	pApp->WriteProfileInt(section, "m_CHECK_PICGL_SUPPORT", this->m_CHECK_PICGL_SUPPORT);
	pApp->WriteProfileString(section, "m_EDIT_PICGL_DIR", this->m_EDIT_PICGL_DIR);
	pApp->WriteProfileInt(section, "m_ALWAYSSN", this->m_ALWAYSSN);

	pApp->WriteProfileInt(section, "m_CHECK_OTHERTHANDCIM", m_CHECK_OTHERTHANDCIM);


	// 2008.7.10
	pApp->WriteProfileInt(section, "m_counter_byday", this->m_counter_byday );
	pApp->WriteProfileInt(section, "m_counter_odd", this->m_counter_odd);
	pApp->WriteProfileInt(section, "m_counter_exec", this->m_counter_exec);
	pApp->WriteProfileInt(section, "m_last_exec", this->m_last_exec);
	pApp->WriteProfileInt(section, "b_auto_start", this->b_auto_start);
}

void CPicmoveDlg::saveIniFileForExit(void)
{
	this->getDialogParam();

	CWinApp* pApp = AfxGetApp();
	CString section = "ProgramData";

	// 2008.7.10
	pApp->WriteProfileInt(section, "m_counter_byday", this->m_counter_byday );
	pApp->WriteProfileInt(section, "m_counter_odd", this->m_counter_odd);
	pApp->WriteProfileInt(section, "m_counter_exec", this->m_counter_exec);
	pApp->WriteProfileInt(section, "m_last_exec", this->m_last_exec);
}


void CPicmoveDlg::loadIniFile(void)
{
	CWinApp* pApp = AfxGetApp();
	CString section = "ProgramData";
	
	CString indir_default, outdir_default, outdir_default_video, thumbdir_default;
	char cd_buf[4096];

	if (getcwd(cd_buf, 4096)) {
		TRACE("cwd %s\n", cd_buf);
		if (strlen(cd_buf) == 3) {
			if (cd_buf[2] == '\\') cd_buf[2] = 0;
		}
		indir_default.Format("%s\\in\\", cd_buf);
	}
	else {		indir_default = "C:\\pic\\in\\";	}

	{
		char path[MAX_PATH]; 
		::SHGetSpecialFolderPath(AfxGetMainWnd()->GetSafeHwnd(), path, CSIDL_PERSONAL, FALSE); 
		CString paths(path);
		paths += "\\My Pictures\\%Y%m\\";
		outdir_default = paths;
		thumbdir_default = paths + "thumb_out\\%Y%m\\";
	}

	{
		char path[MAX_PATH]; 
		::SHGetSpecialFolderPath(AfxGetMainWnd()->GetSafeHwnd(), path, CSIDL_PERSONAL, FALSE); 
		CString paths(path);
		paths += "\\My Videos\\%Y%m\\";
		outdir_default_video = paths;
	}


	indir =			pApp->GetProfileString(section, "indir", indir_default);
	outdir =		pApp->GetProfileString(section, "outdir", outdir_default);
	thumbdir =		pApp->GetProfileString(section, "thumbdir", thumbdir_default);
	naming =		pApp->GetProfileString(section, "naming", "%y%m%d_%H%M%S");
	thumbnaming =	pApp->GetProfileString(section, "thumbnaming", "%y%m%d_%H%M%S");
	video_dir =	pApp->GetProfileString(section, "video_dir", outdir_default_video);
	thumbwidth =	pApp->GetProfileInt(section,	"thumbwidth", 128);

	thumbheight=	pApp->GetProfileInt(section,		"thumbheight", 128);
	thumbpad=		pApp->GetProfileInt(section,		"thumbpad", 0);
	thumb_method =	pApp->GetProfileInt(section,		"thumb_method", 0);


	// チェックボタン
	b_thumb =			pApp->GetProfileInt(section, "b_thumb", 0);
	b_deleteok =		pApp->GetProfileInt(section, "b_deleteok", 0);
//	b_autocontrast =	pApp->GetProfileInt(section, "b_autocontrast", 0);
	b_autocontrast =	0;
	b_orientation =		pApp->GetProfileInt(section, "b_orientation", 0);
	b_eject =			pApp->GetProfileInt(section, "b_eject",	1);
	b_autoquit =		pApp->GetProfileInt(section, "b_autoquit", 0);
	b_confirm =			pApp->GetProfileInt(section, "b_confirm", 0);

	// ラジオボタン
	b_auto_detect =		pApp->GetProfileInt(section, "b_auto_detect", 1);
	b_manual_detect =	pApp->GetProfileInt(section, "b_manual_detect", 0);
	b_add_serial =		pApp->GetProfileInt(section, "b_add_serial", 0);
	b_nocopy =			pApp->GetProfileInt(section, "b_nocopy", 1);
	b_overwrite =			pApp->GetProfileInt(section, "b_overwrite", 0);
	b_video_dir =			pApp->GetProfileInt(section, "b_video_dir", 0);

	// 詳細オプション
	m_detect_time =		pApp->GetProfileInt(section, "m_detect_time",	1500);
	m_bilinear =		pApp->GetProfileInt(section, "m_bilinear",	1	);
	m_gl_time =			pApp->GetProfileInt(section, "m_gl_time",		16	);
	m_jpeg_q =			pApp->GetProfileInt(section, "m_jpeg_q",		90	);
	m_picbar_time =		pApp->GetProfileInt(section, "m_picbar_time",	45	);
	m_thumb_jpeg_q =	pApp->GetProfileInt(section, "m_thumb_jpeg_q",90	);
//	m_speed =			pApp->GetProfileInt(section, "m_speed",			50	);
	m_extensions =		pApp->GetProfileString(section, "m_extensions",		"JPG");
	m_extensions_video =		pApp->GetProfileString(section, "m_extensions_video",		"AVI,MOV,MPG,MP4,3GP,MTS");
	m_serialChar =		pApp->GetProfileString(section, "m_serialChar",		"_");
	m_serialColumn =	pApp->GetProfileInt(section, "m_serialColumn",	2);
	m_glPicture =		pApp->GetProfileInt(section, "m_glPicture",	1);
	this->gl.SetGlPicture((int)m_glPicture);

	m_ex_upper =		pApp->GetProfileInt(section, "m_ex_upper",	2);
	m_SMARTREPLACE =	pApp->GetProfileInt(section, "m_SMARTREPLACE",	0);

	this->m_CHECK_PICGL_SUPPORT =	pApp->GetProfileInt(section, "m_CHECK_PICGL_SUPPORT", 0);
	this->m_EDIT_PICGL_DIR =		pApp->GetProfileString(section, "m_EDIT_PICGL_DIR", "C:\\Program Files\\Picgl");
	this->m_ALWAYSSN = pApp->GetProfileInt(section, "m_ALWAYSSN", 0);

	// 2008.7.10
	this->m_counter_byday = pApp->GetProfileInt(section, "m_counter_byday", 0);
	this->m_counter_odd = pApp->GetProfileInt(section, "m_counter_odd", 0);
	this->m_counter_exec = pApp->GetProfileInt(section, "m_counter_exec", 0);
	this->b_auto_start = pApp->GetProfileInt(section, "b_auto_start", 0);
	this->m_last_exec = pApp->GetProfileInt(section, "m_last_exec", 0);

	m_CHECK_OTHERTHANDCIM = pApp->GetProfileInt(section, "m_CHECK_OTHERTHANDCIM", 0);



	this->setDialogParam();
}

static void argError(const CString &arg1, const CString &arg2)
{
	CString mes;
	mes.Format("Illegal Option Error:\n%s\n%s\n", arg1, arg2);
	AfxMessageBox(mes);
}

bool CPicmoveDlg::DoParseCommandLine()
{
//	char buf[MAX_PATH];
//	::GetCurrentDirectory(MAX_PATH, buf);
//	this->MessageBox(buf);

	
	CWinApp* pApp = AfxGetApp();
	CString cmd(pApp->m_lpCmdLine);
	if (cmd == "") return true;

	vector<CString> ret;
	CommandLineToArray(cmd, ret);

	CString arg1, arg2, arg2low;
	for (int i = 0; i < ret.size(); i++) {
		arg1 = ret.at(i);
		bool bLast = false;
		if (i == ret.size() - 1) bLast = true;
		if (bLast) {
			arg2 = "";
		}
		else {
			arg2 = ret.at(i+1);
		}
		arg1.MakeLower();
		arg2low = arg2;
		arg2low.MakeLower();

		if (arg1 == "-confirm") {
			if (arg2low == "on")		{ this->b_confirm = 1; }
			else if (arg2low == "off") { this->b_confirm = 0; }
			else { argError(arg1, arg2low); return false;}
			i++;
		}
		else if (arg1 == "-autoquit") {
			if (arg2low == "on")		{ this->b_autoquit = 1; }
			else if (arg2low == "off") { this->b_autoquit = 0; }
			else { argError(arg1, arg2low); return false;}
			i++;
		}
		else if (arg1 == "-autodetect") {
			if (arg2low == "on")		{ this->b_auto_detect = 1; b_manual_detect = 0;}
			else if (arg2low == "off") { this->b_auto_detect = 0; b_manual_detect = 1;}
			else { argError(arg1, arg2low); return false;}
			i++;
		}
		else if (arg1 == "-deleteorg") {
			if (arg2low == "on")		{ this->b_deleteok = 1; }
			else if (arg2low == "off") { this->b_deleteok = 0; }
			else { argError(arg1, arg2low); return false;}
			i++;
		}
		else if (arg1 == "-makethumbnails") {
			if (arg2low == "on")		{ this->b_thumb = 1; }
			else if (arg2low == "off") { this->b_thumb = 0; }
			else { argError(arg1, arg2low); return false;}
			i++;
		}
		else if (arg1 == "-rotatebyexif") {
			if (arg2low == "on")		{ this->b_orientation = 1; }
			else if (arg2low == "off") { this->b_orientation = 0; }
			else { argError(arg1, arg2low); return false;}
			i++;
		}
		else if (arg1 == "-smartcopy") {
			if (arg2low == "on")		{ this->m_SMARTREPLACE = 1; }
			else if (arg2low == "off") { this->m_SMARTREPLACE = 0; }
			else { argError(arg1, arg2low); return false;}
			i++;
		}
		else if (arg1 == "-addsn") {
			if (arg2low == "on")		{ this->m_ALWAYSSN = 1; }
			else if (arg2low == "off") { this->m_ALWAYSSN = 0; }
			else { argError(arg1, arg2low); return false;}
			i++;
		}
		else if (arg1 == "-indir") {
			if (arg2 != "") { indir = arg2; }
			else { argError(arg1, arg2);return false; }
			i++;
		}
		else if (arg1 == "-outdir") {
			if (arg2 != "") { outdir = arg2; }
			else { argError(arg1, arg2); return false;}
			i++;
		}
		else if (arg1 == "-outfile") {
			if (arg2 != "") { naming = arg2; }
			else { argError(arg1, arg2);return false; }
			i++;
		}
		else if (arg1 == "-thumbnailoutdir") {
			if (arg2 != "") { thumbdir = arg2; }
			else { argError(arg1, arg2); return false;}
			i++;
		}
		else if (arg1 == "-thumbnailoutfile") {
			if (arg2 != "") { thumbnaming = arg2; }
			else { argError(arg1, arg2); return false;}
			i++;
		}
		else if (arg1 == "-accept") {
			if (arg2low != "") { m_extensions = arg2; }
			else { argError(arg1, arg2low); return false;}
			i++;
		}
		else if (arg1 == "-thumbnailwidth") {
			if (arg2 != "") { thumbwidth = atoi(arg2); }
			else { argError(arg1, arg2); return false;}
			i++;
		}
		else if (arg1 == "-ow_sndigits") {
			if (arg2 != "") { m_serialColumn = atoi(arg2); }
			else { argError(arg1, arg2); return false;}
			i++;
		}
		else if (arg1 == "-ow_snchar") {
			if (arg2 != "") { m_serialChar = arg2; }
			else { argError(arg1, arg2); return false;}
			i++;
		}
		else if (arg1 == "-ow_addsn") {
			b_nocopy = 0;
			b_add_serial = 1;
			b_overwrite = 0;
		}
		else if (arg1 == "-ow_nocopy") {
			b_nocopy = 1;
			b_add_serial = 0;
			b_overwrite = 0;
		}
		else if (arg1 == "-autoexec") {
			bAutoExec = true;
		}
		else if (arg1.GetLength() >= 3 && arg1.GetAt(1) == ':' &&  arg1.GetAt(2) == '\\') {
			indir = arg1;
		}
		else {
			CString mes;
			mes.Format("Unknown option : %s", arg1);
			AfxMessageBox(mes);
			bAutoExec = false;
			return false;
		}
	}
	this->setDialogParam();
	return true;
}


// ディレクトリ系は最後が \ で終わってるかチェックし、終わってなければ追加する
static void nykRegularDir(CString &dir)
{
	dir.TrimLeft();
	dir.TrimRight();
	int len = dir.GetLength();
	if (len <= 0) {
		return;
	}
	int lc = dir.GetAt(len - 1);
	if (lc == '\\') {
		if (len >= 2) {
			if (iskanji1(dir.GetAt(len - 2))) {
				// SJIS の2バイト目だったのでスルー
			}
			else {
				return;
			}
		}
		else {
			return;
		}
	}
	dir += "\\"; // 　/　:　*　?　"　<　>　|
	dir.Replace('*', '_');
	// \　/　:　*　?　"　<　>　|
	dir.Replace('?', '_');
	dir.Replace('<', '_');
	dir.Replace('>', '_');
	dir.Replace('|', '_');
	dir.Replace('"', '_');
}

// カンマ区切りのディレクトリ名を正規化する
static void nykRegularDirList(CString &dirList)
{
	dirList.TrimLeft();
	dirList.TrimRight();

	vector<CString> dir;
	StringToArray(dirList, ",", dir);

	CString out = "";

	vector<CString>::iterator i;
	for (i = dir.begin(); i != dir.end(); i++) {
		CString name = *i;
		nykRegularDir(name);
		out += name;
		if (i+1 != dir.end()) {
			out+=",";
		}
	}
	dirList = out;
}

// 拡張子を常に削除する
static void nykRegularJpegName(CString &name)
{
	if (name.GetLength() == 0) {
		name = "%y%m%d_%H%M%S";
		return;
	}
	name.TrimLeft();
	name.TrimRight();
	name.Replace(':', '_');
	name.Replace('*', '_');
	// \　/　:　*　?　"　<　>　|
	name.Replace('?', '_');
	name.Replace('<', '_');
	name.Replace('>', '_');
	name.Replace('|', '_');
	name.Replace('"', '_');



	int c = name.ReverseFind('.');
	if (c == -1) { return; }// 拡張子がない場合

	if (name.GetLength() - c <= 4) {	// 拡張子っぽい
		if (name.Mid(c).CompareNoCase(".jpg") == 0) {
			name = name.Left(c);
		}
	}
}

void CPicmoveDlg::getDialogParam(void)
{
	GetDlgItemText(IDC_EDIT_INDIR, indir);
	GetDlgItemText(IDC_EDIT_OUTDIR, outdir);
	GetDlgItemText(IDC_EDIT_THUMB_DIR, thumbdir);
	GetDlgItemText(IDC_EDIT_VIDEO_DIR, video_dir);
	// ディレクトリ系は最後が \ で終わってるかチェックし、終わってなければ追加する
	nykRegularDirList(indir);
	nykRegularDir(outdir);
	nykRegularDir(thumbdir);
	nykRegularDirList(video_dir);

	GetDlgItemText(IDC_EDIT_NAMING, naming);
	GetDlgItemText(IDC_EDIT_THUMBNAMING, thumbnaming);

	nykRegularJpegName(naming);
	nykRegularJpegName(thumbnaming);

	CString thumbwidth_text;
	GetDlgItemText(IDC_EDIT_THUMBWIDTH, thumbwidth_text);
	thumbwidth= atoi(thumbwidth_text);
	if (thumbwidth <= 16) thumbwidth = 16;
	if (thumbwidth >= 3200) thumbwidth = 3200;
	thumbwidth_text.Format("%d", thumbwidth);
	SetDlgItemText(IDC_EDIT_THUMBWIDTH, thumbwidth_text);

	CString thumbheight_text;
	GetDlgItemText(IDC_EDIT_THUMBWIDTH, thumbheight_text);
	thumbheight= atoi(thumbheight_text);
	if (thumbheight <= 16) thumbheight = 16;
	if (thumbheight >= 3200) thumbheight = 3200;
	thumbheight_text.Format("%d", thumbheight);
	SetDlgItemText(IDC_EDIT_THUMBHEIGHT, thumbheight_text);

//	CComboBox *cb = (CComboBox*)GetDlgItem(IDC_COMBO_THUMBNAIL);
	this->thumb_method = this->m_COMBO_THUMBNAIL.GetCurSel();


	CButton *b;
	b = (CButton*)GetDlgItem(IDC_CHECK_THUMBNAIL);			b_thumb =			b->GetCheck();
	b = (CButton*)GetDlgItem(IDC_CHECK_DELETEOK);			b_deleteok =		b->GetCheck();
//	b = (CButton*)GetDlgItem(IDC_CHECK_AUTOCONTRAST);		b_autocontrast =	b->GetCheck();
	b = (CButton*)GetDlgItem(IDC_CHECK_ORIENTATION);		b_orientation =		b->GetCheck();
	b = (CButton*)GetDlgItem(IDC_CHECK_EJECT);				b_eject =			b->GetCheck();
	b = (CButton*)GetDlgItem(IDC_CHECK_AUTOQUIT);			b_autoquit =		b->GetCheck();
	b = (CButton*)GetDlgItem(IDC_CHECK_CONFIRM);			b_confirm =			b->GetCheck();
	b = (CButton*)GetDlgItem(IDC_CHECK_AUTOSTART);			this->b_auto_start =			b->GetCheck();

	// ラジオボタン
	b = (CButton*)GetDlgItem(IDC_RADIO_AUTO_DETECT);		b_auto_detect=		b->GetCheck();
	b = (CButton*)GetDlgItem(IDC_RADIO_MANUAL_DETECT);		b_manual_detect=	b->GetCheck();
	b = (CButton*)GetDlgItem(IDC_RADIO_ADD_SERIAL);			b_add_serial=		b->GetCheck();
	b = (CButton*)GetDlgItem(IDC_RADIO_NOCOPY);				b_nocopy=			b->GetCheck();
	b = (CButton*)GetDlgItem(IDC_RADIO_OVERWRITE);				b_overwrite=			b->GetCheck();
	b = (CButton*)GetDlgItem(IDC_CHECK_VIDEO_DIR);				b_video_dir=			b->GetCheck();

	setDialogParam();
}

void CPicmoveDlg::updateGrayEjectButton(void)
{
	CButton *b;
	b = (CButton*)GetDlgItem(IDC_RADIO_AUTO_DETECT);		b_auto_detect=		b->GetCheck();
	CString tmp;
	GetDlgItemText(IDC_EDIT_AUTO_INDIR, tmp);
	bool enable;
	if (this->nowCoping || b_auto_detect == 0 || (b_auto_detect == 1 && tmp == ""))  enable = false;
	else enable = true;

	nykEnableWindow(GetDlgItem(IDC_BUTTON_EJECT), enable);
}

bool CPicmoveDlg::mediaDetected(CString &type_string_in)
{
	CString type_string = type_string_in;
	type_string.MakeUpper();
	old_type_string.MakeUpper();
	if (old_type_string == type_string) {	// 更新なし
		return false;
	}
	// 頭一文字から drive_letter= 'A'-'Z'
	vector<CString> old_dirs;
	StringToArray(old_type_string, ",", old_dirs);
	vector<CString> new_dirs;
	StringToArray(type_string, ",", new_dirs);
	old_type_string = type_string_in;

	// INSERT 処理 - 追加されたものを探す
	vector<CString>::iterator i, j;
	for (i = new_dirs.begin(); i != new_dirs.end(); i++) {
		if (i->GetLength() == 0) continue;
		int hit = false;
		for (j = old_dirs.begin(); j != old_dirs.end(); j++) {
			// 頭一文字のみを比較
			if (i->GetLength() >= 1 && j->GetLength() >= 1) {
				if (toupper(i->GetAt(0)) == toupper(j->GetAt(0))) { hit = true; break; }
			}
		}
		if (!hit) { // ヒットがない=古いものの中に新しいのが見つからない=INSERT
			return true;
//			int c = i->GetAt(0);
//			if (c >= 'A' && c <= 'Z') { 
//				int c_idx = c - 'A';
//				gl.SetMessage(CString((char)c));
//				TRACE("INSERT %c\n", c);
//			}
		}
	}
	return false;
}

bool CPicmoveDlg::autoStart()
{
	// 何かインサートされたの?
	if (mediaDetected(auto_indir)) {
		gl.SetMessage("AUTOSTART");

		ShowWindow(SW_SHOWNORMAL);
		UpdateWindow();

		this->OnButtonExec();
//		TRACE("autoStart -> true\n");
		return true;
	}
//		TRACE("autoStart -> false\n");
	return false;
}

void CPicmoveDlg::setDialogParamAutoInDirOnly(void)
{
	SetDlgItemText(IDC_EDIT_AUTO_INDIR, auto_indir);
	updateGrayEjectButton();
	gl.Type(auto_indir);
}


void CPicmoveDlg::setDialogParam(void)
{
	SetDlgItemText(IDC_EDIT_INDIR, indir);
	SetDlgItemText(IDC_EDIT_OUTDIR, outdir);
	SetDlgItemText(IDC_EDIT_THUMB_DIR, thumbdir);
	SetDlgItemText(IDC_EDIT_VIDEO_DIR, video_dir);
	SetDlgItemText(IDC_EDIT_NAMING, naming);
	SetDlgItemText(IDC_EDIT_THUMBNAMING, thumbnaming);
	SetDlgItemText(IDC_EDIT_AUTO_INDIR, auto_indir);

	CString thumbwidth_text;
	thumbwidth_text.Format("%d", thumbwidth);
	SetDlgItemText(IDC_EDIT_THUMBWIDTH, thumbwidth_text);
	CString thumbheight_text;
	thumbheight_text.Format("%d", thumbheight);
	SetDlgItemText(IDC_EDIT_THUMBHEIGHT, thumbheight_text);

//	CComboBox *cb = (CComboBox*)GetDlgItem(IDC_COMBO_THUMBNAIL);
	this->m_COMBO_THUMBNAIL.SetCurSel(this->thumb_method);

	CButton *b;
	b = (CButton*)GetDlgItem(IDC_CHECK_THUMBNAIL);			b->SetCheck(b_thumb);
	b = (CButton*)GetDlgItem(IDC_CHECK_DELETEOK);			b->SetCheck(b_deleteok);
//	b = (CButton*)GetDlgItem(IDC_CHECK_AUTOCONTRAST);		b->SetCheck(b_autocontrast);
	b = (CButton*)GetDlgItem(IDC_CHECK_ORIENTATION);		b->SetCheck(b_orientation);
	b = (CButton*)GetDlgItem(IDC_CHECK_EJECT);				b->SetCheck(b_eject);
	b = (CButton*)GetDlgItem(IDC_CHECK_AUTOQUIT);			b->SetCheck(b_autoquit);
	b = (CButton*)GetDlgItem(IDC_CHECK_CONFIRM);			b->SetCheck(b_confirm);
	b = (CButton*)GetDlgItem(IDC_CHECK_AUTOSTART);			b->SetCheck(this->b_auto_start);
	b = (CButton*)GetDlgItem(IDC_CHECK_VIDEO_DIR);			b->SetCheck(this->b_video_dir);

	// ラジオボタン
	b = (CButton*)GetDlgItem(IDC_RADIO_AUTO_DETECT);		b->SetCheck(b_auto_detect);
	b = (CButton*)GetDlgItem(IDC_RADIO_MANUAL_DETECT);		b->SetCheck(b_manual_detect);
	b = (CButton*)GetDlgItem(IDC_RADIO_ADD_SERIAL);			b->SetCheck(b_add_serial);
	b = (CButton*)GetDlgItem(IDC_RADIO_NOCOPY);				b->SetCheck(b_nocopy);
	b = (CButton*)GetDlgItem(IDC_RADIO_OVERWRITE);				b->SetCheck(b_overwrite);

//	TRACE("auto %d man %d\n", b_auto_detect, b_manual_detect);
}

void CPicmoveDlg::updateGrayButtons(void)
{
	bool enable = this->nowCoping ? false : true;

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL){
		UINT e = enable ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_GRAYED;
		UINT d = (!enable) ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_GRAYED;
		pSysMenu->EnableMenuItem(IDM_OPTION, e);
		pSysMenu->EnableMenuItem(IDM_SAVECONFIG, e);
		pSysMenu->EnableMenuItem(IDM_STOP, d);
		pSysMenu->EnableMenuItem(IDM_EXECUTE, e);
	}
	
	nykEnableWindow(GetDlgItem(IDC_EDIT_OUTDIR), enable);
	nykEnableWindow(GetDlgItem(IDC_EDIT_NAMING), enable);

	bool video_enable;
	CButton *b = (CButton*)GetDlgItem(IDC_CHECK_VIDEO_DIR);
	if (b->GetCheck()) { // サムネイル作成にチェック
		video_enable = enable;
	}
	else {
		video_enable = false;
	}
	nykEnableWindow(GetDlgItem(IDC_EDIT_VIDEO_DIR), video_enable);


	nykEnableWindow(GetDlgItem(IDC_CHECK_THUMBNAIL), enable);

	bool thumb_enable;
	{
	CButton *b = (CButton*)GetDlgItem(IDC_CHECK_THUMBNAIL);
	if (b->GetCheck()) { // サムネイル作成にチェック
		thumb_enable = enable;
	}
	else {
		thumb_enable = false;
	}
	}
	nykEnableWindow(GetDlgItem(IDC_EDIT_THUMBNAMING), thumb_enable);
	nykEnableWindow(GetDlgItem(IDC_EDIT_THUMB_DIR), thumb_enable);
	nykEnableWindow(GetDlgItem(IDC_COMBO_THUMBNAIL), thumb_enable);

	nykEnableWindow(GetDlgItem(IDC_EDIT_THUMBWIDTH), thumb_enable && (thumb_method==THUMB_METHOD_WIDTH||thumb_method==THUMB_METHOD_WH));
	nykEnableWindow(GetDlgItem(IDC_EDIT_THUMBHEIGHT), thumb_enable && (thumb_method==THUMB_METHOD_HEIGHT||thumb_method==THUMB_METHOD_WH));
	nykEnableWindow(GetDlgItem(IDC_STATIC_THUMB_COLOR), thumb_enable && thumb_method==THUMB_METHOD_WH);

	nykEnableWindow(GetDlgItem(IDC_BUTTON_REFER_THUMBDIR), thumb_enable);

	
	nykEnableWindow(GetDlgItem(IDC_CHECK_DELETEOK), enable);
//	nykEnableWindow(GetDlgItem(IDC_CHECK_AUTOCONTRAST), enable);
	nykEnableWindow(GetDlgItem(IDC_CHECK_ORIENTATION), enable);
	nykEnableWindow(GetDlgItem(IDC_CHECK_EJECT), enable);
	nykEnableWindow(GetDlgItem(IDC_CHECK_AUTOQUIT), enable);
	nykEnableWindow(GetDlgItem(IDC_CHECK_CONFIRM), enable);
	nykEnableWindow(GetDlgItem(IDC_CHECK_VIDEO_DIR), enable);

	nykEnableWindow(GetDlgItem(IDOK), enable);
	nykEnableWindow(GetDlgItem(IDC_BUTTON_OPTION), enable);
	nykEnableWindow(GetDlgItem(IDC_BUTTON_SAVE), enable);
	nykEnableWindow(GetDlgItem(IDC_BUTTON_EXEC), enable);
	nykEnableWindow(GetDlgItem(IDC_BUTTON_STOP), !enable);

	nykEnableWindow(GetDlgItem(IDC_BUTTON_REFER_OUTDIR), enable);
	

	// 自動認識
	nykEnableWindow(GetDlgItem(IDC_RADIO_AUTO_DETECT), enable);	
	b = (CButton*)GetDlgItem(IDC_RADIO_AUTO_DETECT);
//	nykEnableWindow(GetDlgItem(IDC_BUTTON_DETECT), b->GetCheck() && enable);
	nykEnableWindow(GetDlgItem(IDC_EDIT_AUTO_INDIR), b->GetCheck() && enable);
	nykEnableWindow(GetDlgItem(IDC_CHECK_AUTOSTART), b->GetCheck() && enable);

	nykEnableWindow(GetDlgItem(IDC_RADIO_MANUAL_DETECT), enable);
	b = (CButton*)GetDlgItem(IDC_RADIO_MANUAL_DETECT);
	nykEnableWindow(GetDlgItem(IDC_EDIT_INDIR), b->GetCheck() && enable);
	nykEnableWindow(GetDlgItem(IDC_BUTTON_REFER_INDIR), enable && b->GetCheck());
	nykEnableWindow(GetDlgItem(IDC_BUTTON_CLEAR), enable && b->GetCheck());
	
	nykEnableWindow(GetDlgItem(IDC_RADIO_ADD_SERIAL), enable);
	nykEnableWindow(GetDlgItem(IDC_RADIO_NOCOPY), enable);
	nykEnableWindow(GetDlgItem(IDC_RADIO_OVERWRITE), enable);

	updateGrayEjectButton();			
}



// ファイルが同一かどうか確認する
static bool nykIsSameFile(const CString &file1, const CString &file2)
{
	// まずサイズが同一でなければ即、別ファイルと考える
	TRACE("SAMEFILE? %s %s\n", file1, file2);

	int f1 = nykGetFileSize(file1);
	if (f1 == -1) {TRACE("R1\n"); return false; }
	int f2 = nykGetFileSize(file2);
	if (f2 == -1) {TRACE("R2\n");return false; }
	if (f1 != f2) {
//		TRACE("ファイルサイズが違う %d %d\n", f1, f2);
		return false;
	}
	
	// そうしたら全内容の比較
	FILE *fp1 = fopen(file1, "rb");
	if (fp1 == NULL) {TRACE("R3\n"); return false;}
	FILE *fp2 = fopen(file2, "rb");
	if (fp2 == NULL) {TRACE("R4\n"); fclose(fp1); return false; }

	bool ret = true;
	unsigned char buf1, buf2;
	while (1) {
		if (feof(fp1) || feof(fp2)) break;
		fread(&buf1, 1, 1, fp1);
		fread(&buf2, 1, 1, fp2);
		if (feof(fp1) || feof(fp2)) break;
		if (buf1 != buf2) {
			ret = false;
			break;
		}
	}
	fclose(fp1);
	fclose(fp2);

//	TRACE("内容の比較結果 %d\n", ret);
	return ret;
}

static bool nykFileNameAddSerial(CString &fileName, 
								 const CString &serialChar, int serialColumn, 
								 int bSmartReplace)
{
	int c = fileName.ReverseFind('.');
	if (c == -1) return false;
	
	CString orgFileNameLeft  = fileName.Left(c);
	CString orgFileNameRight = fileName.Mid(c);
	CString newFileName;

	if (serialColumn <= 1) serialColumn = 1;
	else if (serialColumn >= 10) serialColumn = 9;

	int max;
	if (serialColumn <= 1)      max = 9;
	else if (serialColumn == 2) max = 99;
	else if (serialColumn == 3) max = 999;
	else if (serialColumn == 4) max = 9999;
	else if (serialColumn == 5) max = 99999;
	else if (serialColumn == 6) max = 999999;
	else if (serialColumn == 7) max = 9999999;
	else if (serialColumn == 8) max = 99999999;
	else                        max = 999999999;

	TRACE("nykFileNameAddSerial %s\n", fileName);
	CString fmt;
	fmt.Format(  "%%0%dd", serialColumn);
	for (int i = 1; i <= max; i++) {
		if (i == 0) {
			newFileName = fileName;
		}
		else {
			newFileName.Format("%s%s" + fmt + "%s", orgFileNameLeft, serialChar, i, orgFileNameRight);
		}
		if (::GetFileAttributes(newFileName) != -1) {
			// 空きファイル名ではないが同一性をチェックする
			if (bSmartReplace) {
				if (nykIsSameFile(fileName, newFileName)) {	// 同一ファイルか
					// 同一ファイルならもうコピーする必要はない
					return false;
				}
			}
		}
		else {	// 空きファイル名をみつけた
			fileName = newFileName;
			return true;
		}
	}
	return false;
}

/*
static bool nykFileNameAddSerialTo1(CString &fileName,  const CString &serialChar, int serialColumn)
{
	int c = fileName.ReverseFind('.');
	if (c == -1) return false;
	
	CString orgFileNameLeft  = fileName.Left(c);
	CString orgFileNameRight = fileName.Mid(c);

	if (serialColumn <= 1) serialColumn = 1;
	else if (serialColumn >= 10) serialColumn = 9;

	CString fmt;
	fmt.Format(  "%%0%dd", serialColumn);
	fileName.Format("%s%s" + fmt + "%s", orgFileNameLeft, serialChar, 1, orgFileNameRight);
	return true;
}
*/

void CPicmoveDlg::glThread(void *ptr) 
{
/*	CPicmoveDlg *my = (CPicmoveDlg*)ptr;

	while (1) {
		TRACE("draw\n");
		my->gl.DrawGL();
		my->m_gl.Invalidate(FALSE);
		Sleep(100);
	}
*/
}


static void callbackfunc_load(const CString &in)
{
	CPicmoveApp *app = (CPicmoveApp*)AfxGetApp();
	CPicmoveDlg *dlg = (CPicmoveDlg*)(app->m_pMainWnd);
	CString text = in;
	int c = text.ReverseFind('\\');
	if (c != -1) text = text.Left(c);
	dlg->SetDlgItemText(IDC_STATIC_PROGRESS_TEXT, NYK_STRINGS109 /* "ファイル存在情報を収集中...\n "*/+text);
}

static void callbackfunc_exif(const CString &in, int count, int max)
{
	CPicmoveApp *app = (CPicmoveApp*)AfxGetApp();
	CPicmoveDlg *dlg = (CPicmoveDlg*)(app->m_pMainWnd);
	CString text = in;
	dlg->SetDlgItemText(IDC_STATIC_PROGRESS_TEXT, NYK_STRINGS110 /* "撮影情報を収集中...\n "*/+text);

	dlg->setDigitalCounter(count);	dlg->digitalPaint();

}

static bool nykCopyFile(const CString &infile, const CString &outfile, bool dummy_f)
{
	if (infile.CompareNoCase(outfile)==0) return false;
	TRACE("COPY %s %s: ", infile, outfile);
	int ret = ::CopyFile(infile, outfile, false);
	TRACE("%d\n", ret);
	if (!ret) return false;
	return nykFileReadOnlyOff(outfile);
}


// フルパスのファイル名からパス部分をカットする
static void nykGetFilenameOnly(const CString &filename, CString &scroll_text)
{
	// 表問題に注意!
	if (filename.GetLength() <= 1) { 
		scroll_text = "";
		return;
	}
	int c = filename.ReverseFind('\\');
	if (c == -1) {
		scroll_text = filename;
	}
	else {
		scroll_text = filename.Mid(c+1);
	}
}


extern void GetAppDir(CString &dirOut);

static bool RealRotateJpeg(bool bClockWise, const CString &fn)
{
	// jpgrotw 90 aaa.jpg
	TRACE("Lossless %d %s\n", bClockWise, fn);
	CString cmd, baseDir;

	GetAppDir(baseDir);

	if (bClockWise)	cmd = baseDir + "jpgrotw 90 " + fn;
	else			cmd = baseDir + "jpgrotw 270 " + fn;

	char cmdline[8192];
	strcpy(cmdline, cmd);

	char cwd[8192];
	GetTempPath(8192, cwd);

	STARTUPINFO sinfo;
	PROCESS_INFORMATION pinfo;

	ZeroMemory( &sinfo, sizeof( STARTUPINFO ) );
	ZeroMemory( &pinfo, sizeof( PROCESS_INFORMATION ) );
	sinfo.cb = sizeof( STARTUPINFO );
	sinfo.dwFlags = STARTF_USESHOWWINDOW;
	sinfo.wShowWindow = SW_SHOWNORMAL;

	if ( !CreateProcess( NULL,
						 cmdline,
						 NULL,
						 NULL,
						 FALSE,
						 0,
						 NULL,
						 cwd,
						 &sinfo,
						 &pinfo ) ) { 
		// 起動失敗
		return false;
	}
	WaitForInputIdle( pinfo.hProcess, INFINITE );
	WaitForSingleObject( pinfo.hProcess, INFINITE );
	
	TRACE("Lossless OK\n");
	return true;
}

void CPicmoveDlg::theThread(void *ptr) 
{
	{	// スレッドでは終了時に自動変数が解放されないので、スコープアウトで解放させる!

	time_t t;
	srand(time(&t));

	CPicmoveApp *app = (CPicmoveApp*)AfxGetApp();
	CPicmoveDlg *my = (CPicmoveDlg*)ptr;
	
	my->digital_counter = 0;	my->digitalPaint();
	my->getDialogParam();

	my->SetDlgItemText(IDC_STATIC_PROGRESS_TEXT2, "-/-");
	my->SetDlgItemText(IDC_STATIC_PROGRESS_TEXT, NYK_STRINGS111 /* "準備しています..."*/);
	my->m_Progress.SetPos(0);
	my->prg_memdc.SetTextColor(RGB(255,255,255));
	my->prg_memdc.SetBkColor(RGB(0,0,0));

	CString indir_list;
	// indir に列挙されたディレクトリから再帰的にコピーすべきファイルのリストを作成する
	if (my->dropFileNum == 0) { // ドラッグアンドドロップでない場合
		if (my->b_auto_detect) { indir_list = my->auto_indir; }
		else if (my->b_manual_detect) { indir_list = my->indir; }
		else indir_list = "";
		my->DcimList.SetExtensions(my->m_extensions);
		my->DcimList.SetExtensionsVideo(my->m_extensions_video);
		my->DcimList.LoadList(indir_list, callbackfunc_load);
	}
	else { // ドラッグアンドドロップの場合
		indir_list = "";
		my->DcimList.SetExtensions(my->m_extensions);
		my->DcimList.SetExtensionsVideo(my->m_extensions_video);
		my->DcimList.LoadListFromDragFiles(my->dropFile, callbackfunc_load);
	}
	// EXIF から撮影日時の情報を読み出す
	my->SetDlgItemText(IDC_STATIC_PROGRESS_TEXT, NYK_STRINGS112 /* "EXIFヘッダから撮影情報を収集中... "*/);
	if (!app->requestStop) my->DcimList.SetExifInfomation(my->naming, my->thumbnaming, my->outdir, my->thumbdir, my->video_dir, callbackfunc_exif, my->m_ex_upper, my->m_counter_byday);


	// ここで確認する
	if (my->b_confirm && !app->requestStop && my->DcimList.GetSizeList() != 0) { // 確認が必要か?
		my->confirmDialog.SetData(my->DcimList);
		my->confirmDialog.ShowWindow(SW_SHOWNORMAL);
		my->confirmDialog.SetFocus();
		my->confirmDialog.UpdateWindow();
		// ほんとに確認しなければ
		// 0=cancel 1=ok -1=idle
		my->confirmDialog.SetResult(-1);
		
		my->SetDlgItemText(IDC_STATIC_PROGRESS_TEXT, NYK_STRINGS113 /* "確認待ちしています..."*/);
		while (1) {
			if (my->confirmDialog.GetResult() != -1) break;
			if (app->requestStop) break;
			Sleep(100);
		}
		my->confirmDialog.ShowWindow(SW_HIDE);
		if (my->confirmDialog.GetResult() == 0) app->requestStop = true;
	}

	CFont font;
	font.CreateFont(10,5,0,0,FW_NORMAL,false,false,0,SHIFTJIS_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY ,
		FIXED_PITCH, "ＭＳゴシック");	// ここをどうするのか?
	CFont *pOldFont = my->prg_memdc.SelectObject(&font);

//	CString indir_list = "";

	// コピーしたよフラグを false にセットする
	int max = my->DcimList.GetSizeList();
	double total_file_size = 0.0;
	int i;
	for (i = 0; i < max; i++) {
		Dcim d;
		my->DcimList.GetDcim(&d, i);
		d.copyDone = false;
//		if (d.errCode == 0)	total_file_size += d.filesize;
		total_file_size += d.filesize;
		my->DcimList.SetDcim(&d, i);
	}
	time_t osBinaryTime;  // C ランタイム時刻 (<time.h>で定義されています)。
	time(&osBinaryTime) ;  // オペレーティング システムから現在時刻を
							 // 取得します。
	CTime startTime(osBinaryTime);  // C ランタイム時刻から CTime を構築します。
	CTime endTime(osBinaryTime);  // C ランタイム時刻から CTime を構築します。
	
	// 処理本番
	int c_err = 0, c_ok = 0, c_del = 0, c_thumb = 0, c_rotate = 0;
	int c_add_serial = 0, c_add_serial_thumb = 0, c_nocopy = 0;
	double transfered_file_size = 0;
	my->m_Progress.SetRange(0, max);
	
	CPicglExport *picgl;
	if (my->m_CHECK_PICGL_SUPPORT) {
		picgl = new CPicglExport(my->m_EDIT_PICGL_DIR + "\\", "picmv");
		picgl->Load();
		picgl->DeleteExpFile();
	}
	for (i = 0; i < max && !app->requestStop; i++) {
		my->digital_counter = max - i;
		my->digitalPaint();

		my->prgbarCounter = 0;
		if (app->requestStop) { break; }
		Dcim d;
		my->DcimList.GetDcim(&d, i);
		time_t fileTime = d.fileTime; // タイムスタンプ保持のため取得しておく
		TRACE("%d: %s %s -> %s\n", i, d.dir, d.orgName, d.newName);
	
		time(&osBinaryTime) ;  // オペレーティング システムから現在時刻を
		endTime = osBinaryTime;
		CTimeSpan spanTime(endTime.GetTime() - startTime.GetTime());
	
		my->m_Progress.SetPos(i);
		my->prgbar_percent = (double)i / max * 100.0;
		CString progress_text2;
//		progress_text2.Format("%4d/\n%4d", i+1, max);
		progress_text2.Format("%4d/\n%4d", i, max);
		my->SetDlgItemText(IDC_STATIC_PROGRESS_TEXT2, progress_text2);

//		if (d.errCode != 0) continue; // exif読み取りとかでエラーがでている
		CString org, newf, thumbf;
		org = d.dir + d.orgName;
		
		nykMakeDir(d.newDir);
		if (d.fileFormat == NYK_FILE_JPG || d.fileFormat == NYK_FILE_OTHER_IMAGE) {
			for (int i2 = 1; i2 < my->DcimList.GetCountOutDirs(); i2++) {
				nykMakeDir(d.newDirEx[i2]);
			}
		}
		else if (d.fileFormat == NYK_FILE_OTHER_VIDEO) {
			for (int i2 = 1; i2 < my->DcimList.GetCountOutDirsVideo(); i2++) {
				nykMakeDir(d.newDirEx[i2]);
			}
		}

		if (my->b_thumb) nykMakeDir(d.newThumbDir);
		newf = d.newDir + d.newName;
		thumbf = d.newThumbDir + d.newThumbName;
//		bool bGoNewf = true, bGoThumbf = true;
		bool bSkip = false;

		if (my->b_add_serial) {	// シリアルナンバを付加する設定の場合
			if (my->m_ALWAYSSN) {
				if (nykFileNameAddSerial(newf, my->m_serialChar, my->m_serialColumn, my->m_SMARTREPLACE)) { 
					c_add_serial++; 
				}
				else {
					bSkip = true;
				}
			}
			else {
				if (::GetFileAttributes(newf) != -1) { // ファイルがすでに存在した場合
					if (my->m_SMARTREPLACE) { // ファイルが同一か確認する
						if (nykIsSameFile(org, newf)) bSkip = true;
						else {

							if (nykFileNameAddSerial(newf, my->m_serialChar, my->m_serialColumn, my->m_SMARTREPLACE)) { 
								c_add_serial++; 
							}
						}
					}
					else if (nykFileNameAddSerial(newf, my->m_serialChar, my->m_serialColumn, my->m_SMARTREPLACE)) { 
						c_add_serial++; 
					}
					else {
						bSkip = true;
					}
				}
			}
			
			if (my->m_ALWAYSSN) {
				if (nykFileNameAddSerial(thumbf, my->m_serialChar, my->m_serialColumn, my->m_SMARTREPLACE)) { 
					c_add_serial_thumb++; 
				}
				else {
//					bSkip = true;
				}
			}
			else {
				if (::GetFileAttributes(thumbf) != -1) { // サムネイルがすでに存在した場合
					if (my->m_SMARTREPLACE) { // ファイルが同一か確認する
						if (nykIsSameFile(org, newf)) {
						}
						else {
							if (nykFileNameAddSerial(thumbf, my->m_serialChar, my->m_serialColumn, my->m_SMARTREPLACE)) { 
								c_add_serial_thumb++; 
							}
						}
					}
					else if (nykFileNameAddSerial(thumbf, my->m_serialChar, my->m_serialColumn, my->m_SMARTREPLACE)) { 
						c_add_serial_thumb++; 
					}
					else {
	//					bSkip = true;
					}
				}
			}
		}
		else if (my->b_nocopy || my->m_SMARTREPLACE) {  // 重複時はコピーしない設定または
														// 重複時は上書きでスマートリプレイス
			if (::GetFileAttributes(newf) != -1) {  // 重複している!
				if (my->m_SMARTREPLACE) {
					// ファイルが同一か確認する
					if (nykIsSameFile(org, newf)) bSkip = true;
				}
				else { // ファイルがなんであろうが重複しているのでスキップする
					bSkip = true;			
				}
			}
		}
		else {	// 重複時に上書きする設定でスマートリプレイスオフの場合  ... 何もしない
		}

		if (bSkip) {
			c_nocopy++; 
			int timeup = 0;
			while (my->ok_scroll == false) {
				Sleep(1);
				timeup++;
				if (timeup > 1000) break;
			}
			my->ok_scroll = false;
			CRect rect;
			my->m_picture_box.GetClientRect(&rect);
			double width = rect.Width();
			double height = width / 64.0 * 10.0;
			if (my->popup_height) { // 画像が残っている場合は全部かきだしてしまおう
				my->prg_memdc.ScrollDC(0, -my->popup_height, NULL, NULL, NULL, NULL);
			}
			CRect rect2(0,rect.bottom,width,rect.bottom+height);
			CBrush br(RGB(64,128,64));
			my->prg_memdc.FillRect(&rect2, &br);
			CBrush br_f(RGB(0,0,0));
			my->prg_memdc.FrameRect(&rect2, &br_f);
			CString scroll_text;
			nykGetFilenameOnly(newf, scroll_text);
			my->prg_memdc.TextOut(1,rect.bottom, "SKIP:" + scroll_text);
			my->prgPaint();
			my->popup_height = height;
			my->ok_scroll = true;

			CString progress_text;
			progress_text.Format(NYK_STRINGS114 /* "スキップ:\n%s -> %s"*/, org, newf);
			my->SetDlgItemText(IDC_STATIC_PROGRESS_TEXT, progress_text);
			continue; 
		}

		CString progress_text;
		progress_text.Format(NYK_STRINGS115 /* "%s ->\n%s\n" \
			"コピー済み %3.1lf / %3.1lf Mbytes\n" \
			"経過時間 %d:%02d:%02d" */,
			org, newf, 
			transfered_file_size/1024.0/1024.0, total_file_size/1024.0/1024.0,
			spanTime.GetHours(), spanTime.GetMinutes(), spanTime.GetSeconds());
		if (transfered_file_size) {
			time_t p_time = (double)total_file_size * ((double)spanTime.GetTotalSeconds() / (double)transfered_file_size);
			TRACE("p_time1 %d %f %d %f\n", p_time, (double)total_file_size, spanTime.GetTotalSeconds(), (double)transfered_file_size);
			p_time -= spanTime.GetTotalSeconds();
			TRACE("p_time2 %d\n", p_time);
			if (p_time >= 0) {
				CTimeSpan prospectTime(p_time);
				CString prospect_text;
				prospect_text.Format(NYK_STRINGS116 /* " 予想終了時刻まであと %d:%02d:%02d" */, 
					prospectTime.GetHours(), prospectTime.GetMinutes(), prospectTime.GetSeconds());
				progress_text+=prospect_text;
			}
		}
		my->SetDlgItemText(IDC_STATIC_PROGRESS_TEXT, progress_text);
		TRACE("COPY %s -> %s\n", org, newf);

		int access_text = org.GetAt(0);
		if (access_text >= 'a' && access_text <= 'z') access_text -= 'a';
		else if (access_text >= 'A' && access_text <= 'Z') access_text -= 'A';
		my->gl.Access(access_text, 2.0);
		//d.fileFormat

		bool bCopyRet = nykCopyFile(org, newf, false);
		int numDirs = 0;
		if (d.fileFormat == NYK_FILE_JPG || d.fileFormat == NYK_FILE_OTHER_IMAGE) {
			numDirs = my->DcimList.GetCountOutDirs();
		}
		else {
			numDirs = my->DcimList.GetCountOutDirsVideo();
		}
		if (numDirs >= 2) {
			// newfを分解する
			CString newDirtyName = newf.Mid(d.newDir.GetLength());
			for (int i2 = 1; i2 < numDirs; i2++) {
				if (nykCopyFile(org, d.newDirEx[i2]+newDirtyName, false) == false) {
					bCopyRet = false;
				}
			}
		}

		if (bCopyRet == false) { // コピー失敗
			my->gl.Access(access_text, 0.0);
			d.copyDone = false;
			my->DcimList.SetDcim(&d, i);
			c_err++;
		}
		else {	// 正常終了
			

			if (my->m_CHECK_PICGL_SUPPORT) {
				picgl->Add(newf);
			}	
			my->gl.Access(access_text, 2.0);
			d.copyDone = true;
			transfered_file_size += d.filesize;
//			TRACE("filesizes %d\n", d.filesize);
			my->DcimList.SetDcim(&d, i);
			if (my->b_deleteok)	{
				DeleteFile(org);
				c_del++;
			}
			c_ok++;

			my->gl.Access(access_text, 2.0);
			CxImage kimage;
			
			switch (d.fileFormat) {
			case NYK_FILE_JPG:
				// オリエンテーションをみて回転
				if (my->b_orientation) {
					if (d.orientation == 6) { // 右に90度回転する
						CxImage image;
						image.Load(newf);
						if (image.IsValid()) {
							int w = image.GetWidth() %  8; int h = image.GetHeight() % 8;
							if (  w ==0 && h == 0) {

								RealRotateJpeg(true, newf);
							}
							else {
								image.RotateRight();
								image.SetJpegQuality(my->m_jpeg_q);
								image.Save(newf, CXIMAGE_FORMAT_JPG);
							}
							c_rotate++;
						}
					}
					else if (d.orientation == 8) { // 左に90度回転する
						CxImage image;
						image.Load(newf);
						if (image.IsValid()) {
							int w = image.GetWidth() %  8; int h = image.GetHeight() % 8;
							if (  w ==0 && h == 0) {
								RealRotateJpeg(false, newf);
							}
							else {
								image.RotateLeft();
								image.SetJpegQuality(my->m_jpeg_q);
								image.Save(newf, CXIMAGE_FORMAT_JPG);
							}
							c_rotate++;
						}
					}
				}

				my->gl.Access(access_text, 2.0);
				kimage.SetJpegScale(8);	// 1,2,4,8	06.06.30 追加
				kimage.Load(newf);
				// スクロール画像作成
				{
					CxImage image;
					image = kimage;
					if (image.IsValid()) {
						int timeup = 0;
						while (my->ok_scroll == false) {
							Sleep(1);
							timeup++;
							if (timeup > 1000) break;
						}
						my->ok_scroll = false;
						
						CRect rect;
						my->m_picture_box.GetClientRect(&rect);
						double w = image.GetWidth();
						double h = image.GetHeight();
						double width = rect.Width();
//						width = 128;
						double height = h / (w / width);

						TRACE("WWWW %f x %f\n", width, height);
						if (my->m_bilinear == 1) {
							if (image.GetBpp() <= 8) image.IncreaseBpp(24);
							image.Resample(width, height, 0);
							image.UnsharpMask(); //float radius = 5.0, float amount = 0.5, int threshold = 0);
						}
						if (my->popup_height) {
							my->prg_memdc.ScrollDC(0, -my->popup_height, NULL, NULL, NULL, NULL);
						}

						// 仮想画面に jpg 画像を書き込む
						CRect rect2(0,rect.bottom,width,rect.bottom+height);
						image.Draw(my->prg_memdc.GetSafeHdc(), rect2, rect2);
						CBrush br(RGB(0,0,0));
						my->prg_memdc.FrameRect(&rect2, &br);
						CString scroll_text;
						nykGetFilenameOnly(newf, scroll_text);
						my->prg_memdc.TextOut(1,rect.bottom, scroll_text);
						my->prgPaint();
						my->popup_height = height;
						my->ok_scroll = true;
					}
					else {
						CRect rect;
						my->m_picture_box.GetClientRect(&rect);
						
						my->prg_memdc.TextOut(1,rect.bottom, "JPEG LOAD ERROR");
						my->prgPaint();
						my->popup_height = 30;
					}
				}
				TRACE("END SCROLLIMG\n");
				my->gl.Access(access_text, 1.0);
				if (app->requestStop) { break; }

				// GL用にテクスチャ生成(無駄多すぎ?)
				if (my->m_glPicture) {
					CxImage image;
					image = kimage;
					double ratio;
					ratio = (double)image.GetWidth() / image.GetHeight();
					TRACE("ratio %f\n", ratio);
					if (image.IsValid()) {
						if (image.GetBpp() <= 8) image.IncreaseBpp(24);
						image.Resample(G_PICTURE_SIZE, G_PICTURE_SIZE, 0);
						image.UnsharpMask(); //float radius = 5.0, float amount = 0.5, int threshold = 0);
						my->gl.SetPicture(&image, G_PICTURE_SIZE, G_PICTURE_SIZE, ratio, 0, d.shotTime);
					}
				}
				if (app->requestStop) { break; }

				// 色調補正
//				my->gl.Access(access_text, 2.0);
//				if (my->b_autocontrast) {
//					Autocontrast(newf, my->m_jpeg_q);
//				}
//				my->gl.Access(access_text, 1.0);
//				if (app->requestStop) {  break; }

				// サムネイル作成
				my->gl.Access(access_text, 2.0);
				if (my->b_thumb) {
					CxImage image;
					image.Load(newf);	// 06.06.30 追加
//					image= kimage;
					if (image.IsValid()) {
						if (image.GetBpp() <= 8) image.IncreaseBpp(24);
						double w = image.GetWidth();
						double h = image.GetHeight();
						double h2, w2;
						switch (my->thumb_method) {
						case THUMB_METHOD_WIDTH:
							h2 = h / (w / my->thumbwidth);
							w2 = my->thumbwidth;
							image.Resample(w2, h2, 0);
							break;
						case THUMB_METHOD_HEIGHT:
							h2 = my->thumbheight;
							w2 = w / (h / my->thumbheight);
							image.Resample(w2, h2, 0);
							break;
						case THUMB_METHOD_WH:
							RGBQUAD color;
							color.rgbRed = my->thumbpad & 0xff;
							color.rgbGreen = (my->thumbpad >> 8) &0xff;
							color.rgbBlue = (my->thumbpad >> 16) &0xff;
							image.Thumbnail(my->thumbwidth, my->thumbheight, color);
							break;
						}
						image.UnsharpMask(); //float radius = 5.0, float amount = 0.5, int threshold = 0);
						image.SetJpegQuality(my->m_thumb_jpeg_q);
						{
							image.Save(thumbf, CXIMAGE_FORMAT_JPG);
							nykSetTimeStamp(thumbf, fileTime);
							c_thumb++;
						}
					}
				}
				break;
			default: // JPEG ファイル以外の場合はコピーするだけ 空のサムネイルを用意する
//				TRACE("JPEGファイル以外\n");
				// スクロール画像作成
				{
					int timeup = 0;
					while (my->ok_scroll == false) {
						Sleep(1);
						timeup++;
						if (timeup > 1000) break;
					}
					my->ok_scroll = false;
					CRect rect;
					my->m_picture_box.GetClientRect(&rect);
					double width = rect.Width();
					double height = width / 64.0 * 48.0;
					if (my->popup_height) { // 画像が残っている場合は全部かきだしてしまおう
						my->prg_memdc.ScrollDC(0, -my->popup_height, NULL, NULL, NULL, NULL);
					}
					CRect rect2(0,rect.bottom,width,rect.bottom+height);
					CBrush br(RGB(64,64,128));
					my->prg_memdc.FillRect(&rect2, &br);
					CBrush br_f(RGB(0,0,0));
					my->prg_memdc.FrameRect(&rect2, &br_f);
					CString scroll_text;
					nykGetFilenameOnly(newf, scroll_text);
					my->prg_memdc.TextOut(1,rect.bottom, scroll_text);
					my->prgPaint();
					my->popup_height = height;
					my->ok_scroll = true;
				}
				break;
			}
			my->gl.Access(access_text, 1.0);
			TRACE("TimeStamping START\n");
			nykSetTimeStamp(newf, fileTime);
			TRACE("TimeStamping END\n");
		}
		my->m_Progress.SetPos(i+1);
		my->prgbar_percent = (double)i / max * 100.0;
	}
	my->digital_counter = max-i;
	my->digitalPaint();
	CString progress_text2;
	progress_text2.Format("%4d/\n%4d", i, max);
	my->SetDlgItemText(IDC_STATIC_PROGRESS_TEXT2, progress_text2);

	if (my->m_CHECK_PICGL_SUPPORT) {
		picgl->Save();
		delete picgl;
	}	

	my->m_counter_odd+=c_ok;
	if (c_ok != 0) my->m_counter_byday++;

// 後処理
	if (!app->requestStop) {	// 中止ボタンをおされなかったとき
		my->prgPaint();
		time(&osBinaryTime) ;  // オペレーティング システムから現在時刻を
		endTime = osBinaryTime;
		CTimeSpan spanTime(endTime.GetTime() - startTime.GetTime());
		CString copy_done_txt;
		copy_done_txt.Format(NYK_STRINGS117 /* "終了しました\n正常%d個 元ファイル削除%d個 サムネイル%d個 回転%d個 エラー%d個\n" \
			"コピー終了 %3.1lf Mbytes\n" \
			"時間 %d:%02d:%02d"*/, 
			c_ok, c_del, c_thumb, c_rotate, c_err, 
			transfered_file_size/1024.0/1024.0,
			spanTime.GetHours(), spanTime.GetMinutes(), spanTime.GetSeconds()
			);
		if (c_add_serial != 0 || c_add_serial_thumb != 0) {
			CString copy_done_txt2;
			copy_done_txt2.Format(NYK_STRINGS118 /* "\n同名のためシリアル番号付加%d個 サムネイルにシリアル番号付加%d個"*/,
				c_add_serial, c_add_serial_thumb);
			copy_done_txt += copy_done_txt2;
		}
		if (c_nocopy != 0) {
			CString copy_done_txt2;
			copy_done_txt2.Format(NYK_STRINGS119 /* "\nスキップ%d個"*/,
				c_nocopy);
			copy_done_txt += copy_done_txt2;
		}
		my->SetDlgItemText(IDC_STATIC_PROGRESS_TEXT, copy_done_txt);

		int eject_result = -1;
		// eject 処理
		if (my->b_eject) {
			TRACE("--- eject start\n");
			{
				CString indir_list;
				if (my->dropFileNum == 0) { // ドラッグアンドドロップでない場合
					if (my->b_auto_detect) { indir_list = my->auto_indir; }
					else if (my->b_manual_detect) { indir_list = my->indir; }
					else indir_list = "";
				}
				else { // ドラッグアンドドロップの場合
					indir_list = "";
				}
				eject_result = EjectDrives(indir_list);
			}
			if (my->b_auto_detect) {
				my->nykGetDcimDirs(my->auto_indir);
				my->setDialogParamAutoInDirOnly();
			}

			TRACE("--- eject end\n");
		}

		my->nowCoping = false;
		my->gl.SetMode(0);
		if (c_ok) {	
			my->gl.SetMessage("FINISHED");
			my->prgbar_percent = 100.0;
			my->prgbarDraw();
			my->prgbarPaint();
			time(&osBinaryTime) ;  // オペレーティング システムから現在時刻を
			endTime = osBinaryTime;
			spanTime = endTime.GetTime() - startTime.GetTime();
			CString end_text;
			end_text.Format(NYK_STRINGS120 /* "終了しました(%3.1lf Mbytes / %d:%02d:%02d)\n"*/, 
				transfered_file_size/1024.0/1024.0,
				spanTime.GetHours(), spanTime.GetMinutes(), spanTime.GetSeconds());
	// // ret 0->イジェクトに成功 -1→イジェクト可能なものがない 1以上→戻り値分のイジェクトに失敗
			if (eject_result == 0) end_text+=NYK_STRINGS121 /* "\nメモリカードはイジェクトされましたので、安全に抜くことができます" */;
	
			my->nowCoping = true;
			if (my->b_autoquit && eject_result <= 0 && c_err == 0) {
				for (int i = 0; i < 3; i++) {
					my->SetDlgItemText(IDC_STATIC_PROGRESS_TEXT, "");
					Sleep(70);
					my->SetDlgItemText(IDC_STATIC_PROGRESS_TEXT, end_text);
					Sleep(500);
				}
			}
			else {

				// ::AfxMessageBox(end_text, MB_OK | MB_ICONINFORMATION); // バックグラウンドに移動してしまう needfix
				::MessageBox(my->m_hWnd, end_text, "Picmv", MB_OK | MB_ICONINFORMATION);
			}

		}
		else { 
			my->prgbar_percent = 0.0;
			my->prgbarDraw();
			my->prgbarPaint();
			CString nocopy_msg_text = NYK_STRINGS122 /* "何もコピーしませんでした\n" */;
			if (c_nocopy != 0) {
				CString nocopy_msg_text2;
				nocopy_msg_text2.Format(NYK_STRINGS123 /* "%d個の同名ファイル処理をスキップしました\n"*/, c_nocopy);
				nocopy_msg_text += nocopy_msg_text2;
				if (eject_result == 0) nocopy_msg_text+=NYK_STRINGS124 /* "\nメモリカードはイジェクトされましたので、安全に抜くことができます"*/ ;
			}
			my->nowCoping = true;
			if (my->b_autoquit && eject_result <= 0 && c_err == 0) {
				for (int i = 0; i < 6; i++) {
					my->SetDlgItemText(IDC_STATIC_PROGRESS_TEXT, "");
					Sleep(70);
					my->SetDlgItemText(IDC_STATIC_PROGRESS_TEXT, nocopy_msg_text);
					Sleep(500);
				}
			}
			else {
				// ::AfxMessageBox(nocopy_msg_text, MB_OK | MB_ICONEXCLAMATION); // バックグラウンドに移動してしまう needfix
				::MessageBox(my->m_hWnd, nocopy_msg_text, "Picmv", MB_OK | MB_ICONEXCLAMATION);
			}
		}

		my->nowCoping = false; // 移動
		my->updateGrayButtons();
		my->prgbar_percent = 0.0;
		my->prgbarDraw();
		my->prgbarPaint();
		my->prgPaint();

		if (my->b_autoquit ) {
			/*

				::PostMessage(my->m_hWnd, WM_CLOSE, 0, 0);
			*/
			my->SetTimer(TIMER_ID_AUTOQUIT1, 100, NULL);

		}
		{
			if (my->popup_height) {
				my->prg_memdc.ScrollDC(0, -my->popup_height, NULL, NULL, NULL, NULL);
			}
			CRect rect;
			my->m_picture_box.GetClientRect(&rect);
			CRect rect_down = rect;
			rect_down.top = rect.bottom;
			rect_down.bottom = rect.bottom + scroll_pic_max_height;
			HBITMAP hBmp = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance,"Picmv.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
			if(hBmp) {
				CDC tmp_memdc;
				tmp_memdc.CreateCompatibleDC(&my->prg_memdc);
				CBitmap *old = tmp_memdc.SelectObject((CBitmap*)CBitmap::FromHandle(hBmp));
				my->prg_memdc.BitBlt(rect_down.left, rect_down.top, rect_down.Width(), rect_down.Height(), 
					&tmp_memdc, 0, 0, SRCCOPY); 
				tmp_memdc.SelectObject(old);
				::DeleteObject(hBmp);
			}
			else {
				CBrush brush(RGB(128,128,128));
				my->prg_memdc.FillRect(&rect_down, &brush);
			}
			my->popup_height = rect.Height();
			TRACE("height %d\n", rect.Height());
		}
	}
	else { // 中止ボタンをおされたとき
//		stopTheThread(my);
		my->SetDlgItemText(IDC_STATIC_PROGRESS_TEXT, NYK_STRINGS125 /* "中止します..."*/);
		TRACE("stop\n");
		CPicmoveApp *app = (CPicmoveApp*)AfxGetApp();
		CPicmoveDlg *my = (CPicmoveDlg*)ptr;

		app->requestStop = false;
		my->nowCoping = false;
		my->updateGrayButtons();
		my->prgbarDraw();
		my->prgbarPaint();
		my->prgPaint();
		my->gl.SetMode(0);
//		my->gl.DrawGL(); // vista 落ちる


		my->prgbar_percent = 0.0;
		my->prgbarDraw();
		my->prgbarPaint();
		my->prgPaint();
//		Sleep(1000);
		TRACE("stop2\n");
		my->SetDlgItemText(IDC_STATIC_PROGRESS_TEXT, NYK_STRINGS126 /* "中止しました" */);
		my->gl.SetMessage("CANCELED");

		//::AfxMessageBox(NYK_STRINGS126 /* "中止しました" */, MB_OK | MB_ICONEXCLAMATION);	// なぜかこれをいれないとブラックアウト // バックグラウンドに移動してしまう needfix
		::MessageBox(my->m_hWnd, NYK_STRINGS126, "Picmv", MB_OK | MB_ICONINFORMATION);
	}
	my->prg_memdc.SelectObject(&pOldFont);
	}
//	_endthread();
}

void CPicmoveDlg::StartThread(void)
{
//	OnGl();
	CPicmoveApp *app = (CPicmoveApp*)AfxGetApp();
	CPicmoveDlg *my = this;
	

	// 確認ダイアログ スレッド中にダイアログはだせない?

	app->requestStop = false;
	this->nowCoping = true;
	this->updateGrayButtons();
	this->prgPaint();
	gl.SetMode(1);
//	gl.DrawGL(); // vista 落ちる

	_beginthread(theThread, 0, this);
}

void CPicmoveDlg::OnButtonExec() 
{
	if (this->nowCoping) {
	}
	else {
		gl.SetRealTimeClock(true);

		dropFileNum = 0;	// ドラッグアンドドロップではない
		this->getDialogParam();
		if (this->b_auto_detect && this->auto_indir == "") {
			AfxMessageBox(NYK_STRINGS127 /* "メモリカードが一枚もマウントされていません\nイジェクト済みの場合は挿入し直してください\n" */, MB_OK | MB_ICONSTOP);
			return;
		}
		StartThread();
	}
}

void CPicmoveDlg::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	TRACE("DROP\n");
	if (this->nowCoping) {
		AfxMessageBox(NYK_STRINGS128 /* "コピー中はドラッグアンドドロップできません\n"*/, MB_OK | MB_ICONSTOP);
		return;
	}

	this->getDialogParam();

	// まずドラッグされたものをリストアップする
    dropFileNum = ::DragQueryFile(hDropInfo, 0xffffffff, NULL, 0); 
	dropFile.clear();
	//
    for(int i = 0; i < dropFileNum; i++) { 
        char DropFileName[MAX_PATH];             // ドロップファイル名
        ::DragQueryFile(hDropInfo, i, DropFileName, sizeof(DropFileName)); 
		TRACE("DropFileName %s\n", DropFileName);
		dropFile.push_back(DropFileName);
    } 
    ::DragFinish(hDropInfo); 

	StartThread();

	CWnd ::OnDropFiles(hDropInfo);
}


void CPicmoveDlg::OnButtonStop() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if (this->nowCoping) {	
		CPicmoveApp *app = (CPicmoveApp*)AfxGetApp();
		app->requestStop = true;
		nykEnableWindow(GetDlgItem(IDC_BUTTON_STOP), false);
	}
}

void CPicmoveDlg::OnButtonSave() 
{
	TRACE("SAVE1\n");
	this->saveIniFile();
	TRACE("SAVE2\n");
	SetDlgItemText(IDC_STATIC_PROGRESS_TEXT, NYK_STRINGS129 /* "設定とオプションを保存しました"*/);
	TRACE("SAVE3\n");

}

void CPicmoveDlg::OnCheckThumbnail() 
{
	updateGrayButtons();
}

void CPicmoveDlg::OnCheckVideoDir() 
{
	updateGrayButtons();
}


BOOL CPicmoveDlg::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	return CDialog::PreCreateWindow(cs);
}

void CPicmoveDlg::OnClose() 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if (this->nowCoping) {
		::AfxMessageBox(NYK_STRINGS130 /* "コピー中は終了できません\n今すぐ終了したい場合は中止ボタンでコピーを中止してください"*/,
			MB_OK | MB_ICONSTOP);
		return;
	}
	CDialog::OnClose();
}

void CPicmoveDlg::prgbarDraw(void)
{
	CRect prgbar_rect;
	int &count = prgbarCounter;
	if (prgbar_percent > 99.99999) {prgbar_local = prgbar_percent = 100.0; count = 0.0; }
	if (prgbar_percent < 0.01) prgbar_local = prgbar_percent = 0.0;
	m_picbar.GetClientRect(&prgbar_rect);
	prgbar_memdc.FillSolidRect(prgbar_rect, RGB(0,0,0));
	prgbar_rect.right = prgbar_local * prgbar_rect.Width() * 0.01 - 1;
	prgbar_rect.left++;
	prgbar_rect.top++;
	prgbar_rect.bottom--;
	if (prgbar_rect.left >= prgbar_rect.right) return ; //prgbar_rect.right = prgbar_rect.left;
	COLORREF rgb;
	double x, v, h;
	if (this->nowCoping) {
		x = (cos(count/100.0) + 1.0) * 0.5; // 0-1
		v = x * 0.4 + 0.6;
	}
	else {
		v = 1.0;
	}
	h = prgbar_percent * 0.01 * 110 + 240;
	rgb = hsv2rgb(h, v, 1.0);
	
	prgbar_memdc.FillSolidRect(prgbar_rect, rgb);
	CBrush brush(hsv2rgb(h, 0.5, 1.0));
	prgbar_memdc.FrameRect(prgbar_rect, &brush);

	count+=20;
}

void CPicmoveDlg::digitalPaint(void)
{
	CRect rect;
	m_digital.GetClientRect(&rect);	
	//              元                                                  先
	// BOOL BitBlt( int x, int y, int nWidth, int nHeight, CDC* pSrcDC, int xSrc, int ySrc, DWORD dwRop );
	char digital_text[16];
//igital_counter = 0;
	if (digital_counter < 0) {
		sprintf(digital_text, "   ");
	}
	else if (digital_counter >= 1000) {
		sprintf(digital_text, "%03d", digital_counter % 1000);
	}
	else {
		sprintf(digital_text, "%3d", digital_counter % 1000);
	}
	int dx = 4, dy = 4;
	for (int i = 0; i < 3; i++) {
		int c = digital_text[i];
		if (c == ' ') c = 10;
		else c = c - '0';
		digital_memdc.BitBlt(i*20+dx,0+dy,20,32,&digital_rsrc_memdc, c*20,0,SRCCOPY);
	}
	// ここまで仮想画面への書き込み

	CDC *dc = this->m_digital.GetDC();
	dc->BitBlt(0,0,rect.Width(),rect.Height(),&this->digital_memdc,0,0,SRCCOPY);
	this->ReleaseDC(dc);

}


/*
void CPicmoveDlg::OnButtonDetect() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	nykGetDcimDirs(this->auto_indir);
	this->setDialogParam();
}
*/

void CPicmoveDlg::OnRadioAutoDetect() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	nykGetDcimDirs(this->auto_indir);
	this->getDialogParam();
	this->updateGrayButtons();
	gl.Type(auto_indir);

}

void CPicmoveDlg::OnRadioManualDetect() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	this->getDialogParam();
	this->updateGrayButtons();
	gl.Type(indir);
	
}


void CPicmoveDlg::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
//	TRACE("AAA %d\n", nIDEvent);
//	if (this->nowCoping) {

	switch (nIDEvent) {
	case TIMER_ID_AUTOQUIT1:
		if (!nowCoping && b_autoquit) {
			gl.SetMessage("AUTOQUIT");
			SetTimer(TIMER_ID_AUTOQUIT2, 1500, NULL);
		}
		KillTimer(TIMER_ID_AUTOQUIT1);
		break;
	case TIMER_ID_AUTOQUIT2:
		if (!nowCoping && b_autoquit) {
			::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		}
		break;
	case TIMER_ID_PRG:	// スクロールするアレ
		if (popup_height && ok_scroll) {
			ok_scroll = false;
			CRect rect;
			int d = -popup_height * 0.2;
			if (d == 0) d = -1;
			this->m_picture_box.GetClientRect(&rect);
			this->prg_memdc.ScrollDC(0, d, NULL, NULL, NULL, NULL);
			popup_height += d;
			this->prgPaint();
			ok_scroll = true;
		}
		break;
	case TIMER_ID_PRGBAR:	// 横棒グラフ
		if (!nowCoping) {prgbar_local = prgbar_percent; break;}
		if (prgbar_percent == 0.0) prgbar_local = 0.0;
		else if (prgbar_percent > 99.9) prgbar_local = 100.0;
		else if (fabs(prgbar_local - prgbar_percent) < 0.1) prgbar_local = prgbar_percent;
		else prgbar_local += (prgbar_percent - prgbar_local) * 0.1;
		prgbarDraw();
		prgbarPaint();
//		return;
		break;
	case TIMER_ID_DETECT:
		if (this->b_auto_detect && !this->nowCoping) {

			nykGetDcimDirs(this->auto_indir);
			this->setDialogParamAutoInDirOnly();
			if (this->b_auto_start) {
				if (this->autoStart())	{
					//if ((this->IsIconic())) {
						//this->ShowWindow(SW_SHOWNORMAL);
						this->SetForegroundWindow();
					//}
				}
			}
		}
//		return;
		break;
	case TIMER_ID_GLDRAW:
		gl.DrawGL();
		break;
/*	case TIMER_ID_START_MESSAGE:
		gl.SetMessage("0.87");
		this->KillTimer(TIMER_ID_START_MESSAGE);
		break;*/
	}

	CDialog::OnTimer(nIDEvent);
}

void CPicmoveDlg::OnButtonEject() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if (this->b_auto_detect) {
		if (this->auto_indir == "") {
			AfxMessageBox(NYK_STRINGS131 /* "メモリカードがありません"*/, MB_OK | MB_ICONSTOP);
			return;
		}
		EjectDrives(this->auto_indir);
		nykGetDcimDirs(this->auto_indir);
		this->setDialogParamAutoInDirOnly();
	}
}


void CPicmoveDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	this->saveIniFileForExit();

	prg_memdc.SelectObject(old_prg_bitmap);
	prgbar_memdc.SelectObject(old_prgbar_bitmap);
	digital_memdc.SelectObject(old_digital_bitmap);
	digital_rsrc_memdc.SelectObject(old_digital_rsrc);
	gl.DestroyGL();		


}

void CPicmoveDlg::OnChangeEditIndir() 
{
	// TODO: これが RICHEDIT コントロールの場合、コントロールは、 lParam マスク
	// 内での論理和の ENM_CHANGE フラグ付きで CRichEditCrtl().SetEventMask()
	// メッセージをコントロールへ送るために CDialog::OnInitDialog() 関数をオーバー
	// ライドしない限りこの通知を送りません。
	
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	GetDlgItemText(IDC_EDIT_INDIR, indir);
	// ディレクトリ系は最後が \ で終わってるかチェックし、終わってなければ追加する
	nykRegularDirList(indir);

	if (b_auto_detect) gl.Type(auto_indir);
	else gl.Type(indir);
}

void CPicmoveDlg::OnGl() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	if (b_auto_detect) {
		gl.Touch(this->auto_indir);
	}
	else {
		gl.Touch(indir);
	}
}

void CPicmoveDlg::OnButtonReferIndir() 
{
	CString folder, default_folder, title_text;

	this->getDialogParam();
	title_text = NYK_STRINGS132 /* "入力元ディレクトリの追加"*/;
	default_folder = "";
	if (nykGetFolder(this->m_hWnd, folder, default_folder, title_text)) {
		if (folder != "") {
			if (indir.Right(1) != ","  && indir.GetLength() != 0) indir += ",";
			indir += folder;
			nykRegularDirList(indir);
			this->setDialogParam();
			gl.Type(indir);
		}
	}
}

void CPicmoveDlg::OnButtonClear() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	this->getDialogParam();
	int c;
	c = indir.ReverseFind(',');
	if (c <= 0) {
		indir = "";	
	}
	else {
		CString tmp;
		tmp = indir;
		indir = tmp.Left(c);
	}
	nykRegularDirList(indir);
	this->setDialogParam();
	gl.Type(indir);
}

void CPicmoveDlg::OnButtonReferOutdir() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString folder, default_folder, title_text;

	this->getDialogParam();
	title_text = NYK_STRINGS133 /* "出力ディレクトリの指定"*/;
	default_folder = this->outdir;
	if (nykGetFolder(this->m_hWnd, folder, default_folder, title_text)) {
		if (folder != "") {
			outdir = folder;
			this->setDialogParam();
		}
	}
}

void CPicmoveDlg::OnButtonReferThumbdir() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString folder, default_folder, title_text;

	this->getDialogParam();
	title_text = NYK_STRINGS134 /* "サムネイル出力ディレクトリの指定"*/;
	default_folder = this->thumbdir;
	if (nykGetFolder(this->m_hWnd, folder, default_folder, title_text)) {
		if (folder != "") {
			thumbdir = folder;
			this->setDialogParam();
		}
	}
}


void CPicmoveDlg::OnButtonHelpAll() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
//	WinExec("notepad.exe readme.txt", SW_SHOWNORMAL);
	ShellExecute( NULL, NULL, "Picmv.chm", NULL, NULL, SW_SHOWDEFAULT);
}

void CPicmoveDlg::OnButtonOption() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CPicMoveOption dlgOption;

	
	dlgOption.m_picbar_time = (m_picbar_time - 16.0) / 200.0 * 100.0;
	dlgOption.m_gl_time = (m_gl_time - 16.0) / 200.0 * 100.0;
	dlgOption.m_detect_time = (m_detect_time - 1000.0) / 4000.0 * 100.0;
	dlgOption.m_jpeg_q = m_jpeg_q;
	dlgOption.m_thumb_jpeg_q = m_thumb_jpeg_q;
	dlgOption.m_bilinear = m_bilinear;
	dlgOption.m_ex_upper = m_ex_upper;
	dlgOption.m_ALWAYSSSN = this->m_ALWAYSSN;
//	dlgOption.m_speed = m_speed;
	nykRegularExtensions(m_extensions);
	nykRegularExtensions(m_extensions_video);
	dlgOption.m_extensions = m_extensions;
	dlgOption.m_extensions_video = m_extensions_video;
	dlgOption.m_serialChar = this->m_serialChar;
	CString column_text;
	column_text.Format("%d", this->m_serialColumn);
	dlgOption.m_serialColumn = column_text;
	dlgOption.m_SMARTREPLACE = this->m_SMARTREPLACE;
	dlgOption.m_CHECK_PICGL_SUPPORT = this->m_CHECK_PICGL_SUPPORT;
	dlgOption.m_EDIT_PICGL_DIR = this->m_EDIT_PICGL_DIR;
	dlgOption.m_CHECK_OTHERTHANDCIM = this->m_CHECK_OTHERTHANDCIM;
	int old_glPicture = dlgOption.m_glPicture = m_glPicture;
	int response = dlgOption.DoModal();
	if (response == IDOK) {
		m_ex_upper = dlgOption.m_ex_upper;
		m_picbar_time = (dlgOption.m_picbar_time / 100.0) * 200.0 + 16.0;
		m_gl_time = (dlgOption.m_gl_time / 100.0) * 200.0 + 16.0;
		m_detect_time = (dlgOption.m_detect_time / 100.0) * 4000.0 + 1000.0;
		m_jpeg_q = dlgOption.m_jpeg_q;
		m_thumb_jpeg_q = dlgOption.m_thumb_jpeg_q;
		m_bilinear = dlgOption.m_bilinear;
//		m_speed = dlgOption.m_speed;
		m_extensions = dlgOption.m_extensions;
		m_extensions_video = dlgOption.m_extensions_video;
		nykRegularExtensions(m_extensions);
		nykRegularExtensions(m_extensions_video);
		m_serialChar = dlgOption.m_serialChar;
		m_serialColumn = atoi(dlgOption.m_serialColumn);
		m_glPicture = dlgOption.m_glPicture;
		this->m_ALWAYSSN = dlgOption.m_ALWAYSSSN;
		this->gl.SetGlPicture((int)m_glPicture);
		this->m_SMARTREPLACE = dlgOption.m_SMARTREPLACE;
		this->m_CHECK_PICGL_SUPPORT = dlgOption.m_CHECK_PICGL_SUPPORT;
		this->m_EDIT_PICGL_DIR = dlgOption.m_EDIT_PICGL_DIR;
		this->m_CHECK_OTHERTHANDCIM = dlgOption.m_CHECK_OTHERTHANDCIM;
//			MessageBox("OK");
		SetTimers();
		if (b_auto_detect) {
			nykGetDcimDirs(auto_indir);
			setDialogParamAutoInDirOnly();
		}
		if (old_glPicture && !m_glPicture)	{
			OnGl();
		}
	}
}


void CPicmoveDlg::OnStaticDigital() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	digital_mode++;
	if (digital_mode >= 3) digital_mode = 0;
	digitalPaint();
}

// extern void makeDcimFileName(CString& outname, time_t t_in, const CString &naming_text, time_t t_now,  const CString &ext, const CString &original_name);

void CPicmoveDlg::PreviewNaming(bool thumbnail)
{
	CString indir_tmp, outdir_tmp, thumbdir_tmp;
	GetDlgItemText(IDC_EDIT_INDIR, indir_tmp);
	GetDlgItemText(IDC_EDIT_OUTDIR, outdir_tmp);
	GetDlgItemText(IDC_EDIT_THUMB_DIR, thumbdir_tmp);
	// ディレクトリ系は最後が \ で終わってるかチェックし、終わってなければ追加する
	nykRegularDirList(indir_tmp);
	nykRegularDir(outdir_tmp);
	nykRegularDir(thumbdir_tmp);

	CString naming_tmp, thumbnaming_tmp;
	GetDlgItemText(IDC_EDIT_NAMING, naming_tmp);
	GetDlgItemText(IDC_EDIT_THUMBNAMING, thumbnaming_tmp);

	nykRegularJpegName(naming_tmp);
	nykRegularJpegName(thumbnaming_tmp);

	time_t t, now;
	t = sampleTime;
	time(&now);
	CString dir_out, name_out, t_dir_out, t_name_out;

	makeDcimFileName(name_out, t, naming_tmp, now,  "JPG", "SAMPLE00001", "Maker", "Model", "JPG", this->m_counter_byday);
	makeDcimFileName(dir_out, t,  outdir_tmp, now,  "", "D:\\SAMPLE\\DCIM", "Maker", "Model", "JPG", this->m_counter_byday);
	makeDcimFileName(t_name_out, t, thumbnaming_tmp, now,  "JPG", "SAMPLE00001", "Maker", "Model", "JPG", this->m_counter_byday);
	makeDcimFileName(t_dir_out, t,  thumbdir_tmp, now,  "", "D:\\SAMPLE\\DCIM", "Maker", "Model", "JPG", this->m_counter_byday);

	CString text1, text2, text3;
	CTime ct(t);
	if (thumbnail) {
		text1.Format(NYK_STRINGS137 /* "撮影日時が%4d年%02d月%02d日%02d:%02d:%02dの場合\n"*/, ct.GetYear(), ct.GetMonth(), ct.GetDay(), ct.GetHour(), ct.GetMinute(), ct.GetSecond());
		text2.Format(NYK_STRINGS138 /* "出力ディレクトリ: [%s]\n"*/, t_dir_out);
		text3.Format(NYK_STRINGS139 /* "出力ファイル名: [%s]\n"*/, t_name_out);
		this->SetDlgItemText(IDC_STATIC_PROGRESS_TEXT, NYK_STRINGS135 /* "サムネイル命名規則プレビュー:\n"*/+text1+text2+text3);
//		gl.SetMessage(t_name_out);

	}
	else {
		text1.Format(NYK_STRINGS137 /* "撮影日時が%4d年%02d月%02d日%02d:%02d:%02dの場合\n"*/, ct.GetYear(), ct.GetMonth(), ct.GetDay(), ct.GetHour(), ct.GetMinute(), ct.GetSecond());
		text2.Format(NYK_STRINGS138 /* "出力ディレクトリ: [%s]\n"*/, dir_out);
		text3.Format(NYK_STRINGS139 /* "出力ファイル名: [%s]\n"*/, name_out);
		this->SetDlgItemText(IDC_STATIC_PROGRESS_TEXT, NYK_STRINGS136 /* "命名規則プレビュー:\n"*/+text1+text2+text3);
	}
}

void CPicmoveDlg::OnChangeEditOutdir() 
{
	// 実際に名前が変わったときだけにしたい
	CString outdir_tmp;
	GetDlgItemText(IDC_EDIT_OUTDIR, outdir_tmp);
	nykRegularDir(outdir_tmp);
	if (this->outdir == outdir_tmp) return;
	outdir = outdir_tmp;

	PreviewNaming(false);
}

void CPicmoveDlg::OnChangeEditNaming() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString naming_tmp;
	GetDlgItemText(IDC_EDIT_NAMING, naming_tmp);
	nykRegularJpegName(naming_tmp);
	if (this->naming == naming_tmp) return;
	naming = naming_tmp;

	PreviewNaming(false);
}

void CPicmoveDlg::OnChangeEditThumbDir() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CButton *b;
	b = (CButton*)GetDlgItem(IDC_CHECK_THUMBNAIL);
	if (b->GetCheck() == 0) return;
	CString thumbdir_tmp;
	GetDlgItemText(IDC_EDIT_THUMB_DIR, thumbdir_tmp);
	nykRegularDir(thumbdir_tmp);
	if (this->thumbdir == thumbdir_tmp) return;
	thumbdir = thumbdir_tmp;

	PreviewNaming(true);
}

void CPicmoveDlg::OnChangeEditThumbnaming() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CButton *b;
	b = (CButton*)GetDlgItem(IDC_CHECK_THUMBNAIL);
	if (b->GetCheck() == 0) return;
	CString thumbnaming_tmp;
	GetDlgItemText(IDC_EDIT_THUMBNAMING, thumbnaming_tmp);
	nykRegularJpegName(thumbnaming_tmp);
	if (this->thumbnaming == thumbnaming_tmp) return;
	thumbnaming = thumbnaming_tmp;

	PreviewNaming(true);
}

void CPicmoveDlg::OnPictureBox() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
//	gl.SetMessage("PICMV0.85");	
}

void CPicmoveDlg::OnRadioOverwrite() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

/*	CString mes =	"上書き設定は非推奨です。最悪でファイル消失の危険があります。\n\n" \
					"なぜなら、同時刻 EXIF 情報をもった写真や、EXIF なしで同時刻タイムスタンプをもった写真を\n" \
					"一括コピーする場合に、同一ファイル名になってしまったファイル群が、\n" \
					"上書きで最後のコピーだけが生き残る場合があるからです。\n\n" \
					"このとき、もしも「元ファイルを削除」にチェックをいれていた場合、ファイルを失うことになります。よってチェックをオフにします。\n";
*/
//	CString mes;
//	mes.LoadString(IDS_STRING140);
	::AfxMessageBox(NYK_STRINGS140, MB_OK | MB_ICONEXCLAMATION);

	b_deleteok = 0;
	CButton *b = (CButton*)GetDlgItem(IDC_CHECK_DELETEOK);			
	b->SetCheck(b_deleteok);
}


BOOL CPicmoveDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	NamingRuleTip.RelayEvent(pMsg);

   // アクセラレータ
    if(m_acl != NULL){
        if(::TranslateAccelerator(m_hWnd, m_acl, pMsg)){
            return TRUE;
        }
    }

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CPicmoveDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
//	return CDialog::OnHelpInfo(pHelpInfo);
	return true;
}

void CPicmoveDlg::OnButtonNamingHelp() 
{
	this->rulesDlg.SetHelpText(NYK_STRINGS147);
	this->rulesDlg.UpdateWindow();
	this->rulesDlg.ShowWindow(SW_SHOWNORMAL);
}

void CPicmoveDlg::OnButtonNamingHelp2() 
{
	this->rulesDlg.SetHelpText(NYK_STRINGS147);
	this->rulesDlg.UpdateWindow();
	this->rulesDlg.ShowWindow(SW_SHOWNORMAL);
}

void CPicmoveDlg::OnEditchangeComboThumbnail() 
{
}

void CPicmoveDlg::OnSelchangeComboThumbnail() 
{
	this->thumb_method = this->m_COMBO_THUMBNAIL.GetCurSel();
//	CString mes; mes.Format("%d", thumb_method);
//	AfxMessageBox(mes);
	this->updateGrayButtons();
	InvalidateRect(NULL, false);	
}

void CPicmoveDlg::OnStaticThumbColor() 
{
	CColorDialog dlg(this->thumbpad);
	
	dlg.m_cc.Flags |= CC_FULLOPEN;
	if (dlg.DoModal() == IDOK) {
		this->thumbpad = dlg.m_cc.rgbResult;
	}
	InvalidateRect(NULL, false);	
}


void CPicmoveDlg::AddNotifyIcon()
{
//	m_menuRightClickTrayIcon.LoadMenu(IDR_MENU1);
#if 0
	// 通知領域のアイコンを作成
	NOTIFYICONDATA notif;
	char cToolTipMsg[9] = "Picmv";
	notif.cbSize = sizeof(NOTIFYICONDATA);
	notif.hWnd = m_hWnd;
	notif.uID = 0;
	notif.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	notif.uCallbackMessage = WM_TRAYICONMESSAGE;
	notif.hIcon = m_hIcon;
	strcpy(notif.szTip, cToolTipMsg);

	Shell_NotifyIcon(NIM_ADD, &notif);

	m_hWndCopy = m_hWnd;
#endif
}



void CPicmoveDlg::PostNcDestroy() 
{
#if 0
	// トレイアイコンの削除
	NOTIFYICONDATA notif;

	notif.cbSize = sizeof(NOTIFYICONDATA);
	notif.hWnd = m_hWndCopy;
	notif.uID = 0;
	notif.uFlags = 0;

	Shell_NotifyIcon(NIM_DELETE, &notif);
#endif	
	CDialog::PostNcDestroy();
}

LRESULT CPicmoveDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
#if 0
	switch( message )
	{
		case WM_TRAYICONMESSAGE:  //アイコンからのメッセージ。
			if( lParam == WM_LBUTTONDOWN ){
				ShowWindow(SW_SHOWNORMAL);				// 左クリック時の処理
			}
			else if( lParam == WM_RBUTTONDOWN ){
				// 右クリック時の処理
				//POINT poi;
				//poi.x = 100; poi.y = 100;
				//::GetCursorPos(&poi);
				//ClientToScreen(&poi);
				//CMenu *subMenu = &m_menuRightClickTrayIcon; //.GetSubMenu(0);

				//m_menuRightClickTrayIcon.TrackPopupMenu(TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RIGHTBUTTON, poi.x, poi.y, this);

				CMenu    *popupMenuP = NULL;
				CMenu    cMenu;
				POINT point;
				point.x = 128;
				point.y = 128;
    
				::GetCursorPos(&point);
				// メニューをロード
				cMenu.LoadMenu(IDR_MENU1);
				// サブメニューを取得
				popupMenuP = cMenu.GetSubMenu(0);
				// メニューをポップアップ
				//ClientToScreen(&point);
				popupMenuP->TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, this);
				// メニューを破棄
				cMenu.DestroyMenu();


			
			}
			return 0;
		default:
			break;
	}       
#endif	
	return CDialog::WindowProc(message, wParam, lParam);
}

void CPicmoveDlg::OnMenuitemExecute() 
{
//	ShowWindow(SW_SHOWNORMAL);
//	this->OnButtonExec();	
}

void CPicmoveDlg::OnMenuitemExit() 
{
//	::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
}
