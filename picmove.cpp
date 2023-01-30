// picmove.cpp : アプリケーション用クラスの定義を行います。
//

#include "stdafx.h"
#include "picmove.h"
#include "picmoveDlg.h"
#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPicmoveApp

BEGIN_MESSAGE_MAP(CPicmoveApp, CWinApp)
	//{{AFX_MSG_MAP(CPicmoveApp)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()




/////////////////////////////////////////////////////////////////////////////
// CPicmoveApp クラスの構築

CPicmoveApp::CPicmoveApp()
{
	// TODO: この位置に構築用のコードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
	requestStop = false;
	time_t t;
	srand(time(&t));
	this->bMiniSize = 0;
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CPicmoveApp オブジェクト

CPicmoveApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPicmoveApp クラスの初期化



CString NYK_STRINGS102;
CString NYK_STRINGS103;
CString NYK_STRINGS104;
CString NYK_STRINGS105;
CString NYK_STRINGS106;
CString NYK_STRINGS107;
CString NYK_STRINGS108;
CString NYK_STRINGS109;
CString NYK_STRINGS110;
CString NYK_STRINGS111;
CString NYK_STRINGS112;
CString NYK_STRINGS113;
CString NYK_STRINGS114;
CString NYK_STRINGS115;
CString NYK_STRINGS116;
CString NYK_STRINGS117;
CString NYK_STRINGS118;
CString NYK_STRINGS119;
CString NYK_STRINGS120;
CString NYK_STRINGS121;
CString NYK_STRINGS122;
CString NYK_STRINGS123;
CString NYK_STRINGS124;
CString NYK_STRINGS125;
CString NYK_STRINGS126;
CString NYK_STRINGS127;
CString NYK_STRINGS128;
CString NYK_STRINGS129;
CString NYK_STRINGS130;
CString NYK_STRINGS131;
CString NYK_STRINGS132;
CString NYK_STRINGS133;
CString NYK_STRINGS134;
CString NYK_STRINGS135;
CString NYK_STRINGS136;
CString NYK_STRINGS137;
CString NYK_STRINGS138;
CString NYK_STRINGS139;
CString NYK_STRINGS140;
CString NYK_STRINGS141;
CString NYK_STRINGS142;
CString NYK_STRINGS143;
CString NYK_STRINGS144;
CString NYK_STRINGS145;

CString NYK_STRINGS146;
CString NYK_STRINGS147;
CString NYK_STRINGS148;
CString NYK_STRINGS149;

bool CPicmoveApp::LoadMyStrings(void)
{
	NYK_STRINGS102.LoadString(IDS_STRING102);
	NYK_STRINGS103.LoadString(IDS_STRING103);
	NYK_STRINGS104.LoadString(IDS_STRING104);
	NYK_STRINGS105.LoadString(IDS_STRING105);
	NYK_STRINGS106.LoadString(IDS_STRING106);
	NYK_STRINGS107.LoadString(IDS_STRING107);
	NYK_STRINGS108.LoadString(IDS_STRING108);
	NYK_STRINGS109.LoadString(IDS_STRING109);
	NYK_STRINGS110.LoadString(IDS_STRING110);
	NYK_STRINGS111.LoadString(IDS_STRING111);
	NYK_STRINGS112.LoadString(IDS_STRING112);
	NYK_STRINGS113.LoadString(IDS_STRING113);
	NYK_STRINGS114.LoadString(IDS_STRING114);
	NYK_STRINGS115.LoadString(IDS_STRING115);
	NYK_STRINGS116.LoadString(IDS_STRING116);
	NYK_STRINGS117.LoadString(IDS_STRING117);
	NYK_STRINGS118.LoadString(IDS_STRING118);
	NYK_STRINGS119.LoadString(IDS_STRING119);
	NYK_STRINGS120.LoadString(IDS_STRING120);
	NYK_STRINGS121.LoadString(IDS_STRING121);
	NYK_STRINGS122.LoadString(IDS_STRING122);
	NYK_STRINGS123.LoadString(IDS_STRING123);
	NYK_STRINGS124.LoadString(IDS_STRING124);
	NYK_STRINGS125.LoadString(IDS_STRING125);
	NYK_STRINGS126.LoadString(IDS_STRING126);
	NYK_STRINGS127.LoadString(IDS_STRING127);
	NYK_STRINGS128.LoadString(IDS_STRING128);
	NYK_STRINGS129.LoadString(IDS_STRING129);
	NYK_STRINGS130.LoadString(IDS_STRING130);
	NYK_STRINGS131.LoadString(IDS_STRING131);
	NYK_STRINGS132.LoadString(IDS_STRING132);
	NYK_STRINGS133.LoadString(IDS_STRING133);
	NYK_STRINGS134.LoadString(IDS_STRING134);
	NYK_STRINGS135.LoadString(IDS_STRING135);
	NYK_STRINGS136.LoadString(IDS_STRING136);
	NYK_STRINGS137.LoadString(IDS_STRING137);
	NYK_STRINGS138.LoadString(IDS_STRING138);
	NYK_STRINGS139.LoadString(IDS_STRING139);
	NYK_STRINGS140.LoadString(IDS_STRING140);
	NYK_STRINGS141.LoadString(IDS_STRING141);
	NYK_STRINGS141.LoadString(IDS_STRING141);
	NYK_STRINGS142.LoadString(IDS_STRING142);
	NYK_STRINGS143.LoadString(IDS_STRING143);
	NYK_STRINGS144.LoadString(IDS_STRING144);
	NYK_STRINGS145.LoadString(IDS_STRING145);

	NYK_STRINGS146.LoadString(IDS_STRING146);
	NYK_STRINGS147.LoadString(IDS_STRING147);
	NYK_STRINGS148.LoadString(IDS_STRING148);
	NYK_STRINGS149.LoadString(IDS_STRING149);

	return true;
}

void GetAppDir(CString &dirOut)
{
   char dir[MAX_PATH];
    ::GetModuleFileName(NULL, dir, MAX_PATH);  // 実行ファイルのパスを取得
    char* pdest = strrchr(dir, '\\');          // 実行ファイルのパスから SJIS 表問題!
    if (pdest != NULL)	pdest[1] = '\0';       // 実行ファイル名だけ切り取る
    dirOut = dir;
}

bool CPicmoveApp::LoadLanguageDll(const CString &dllName_in)
{
	bool ret = true;
//	this->lang = 0;
	{
		CString baseDir, dllName;
		::GetAppDir(baseDir);
		dllName = baseDir + dllName_in; //"picmove_en.dll";
		if (::GetFileAttributes(dllName) != -1) {
			HINSTANCE returnHandle = ::LoadLibrary(dllName);
			if (returnHandle) {
				AfxSetResourceHandle(returnHandle);
//				this->lang = 1;
				ret = true;
			}
		}
		else {
			ret =false;
		}
	}
	LoadMyStrings();
	return ret;
}


BOOL CPicmoveApp::InitInstance()
{

	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さくしたけ
	//  れば以下の特定の初期化ルーチンの中から不必要なものを削除して
	//  ください。

#ifdef NYK_RELEASEZIP
	SetupIniFile(this, "PicMove.ini");
#else
	SetupIniFileLocalAppData(this, "Picmv.ini");
#endif

	loadIniFile();

	if (bMiniSize) {
		bool ret = LoadLanguageDll("picmove_jp_small.dll");
		if (!ret) LoadLanguageDll("picmove_en_small.dll");
	}
	else {
		LoadLanguageDll("picmove_en.dll");
	}

    hMSP = CreateMutex(NULL, TRUE, "picmove_picmove");
    // すでに起動しているか判定
    if(GetLastError() == ERROR_ALREADY_EXISTS){        // すでに起動している。        
//		MessageBox(NULL, NYK_STRINGS102 /*"すでに Picmv は起動しております。" */, "Picmv", MB_OK);
        ReleaseMutex(hMSP);
        CloseHandle(hMSP);

		HWND h;
		h = ::FindWindow(NULL, "Picmv");
        if(h) {
//            pWnd->IsIconic() ? pWnd->ShowWindow(SW_SHOWNORMAL) : pWnd->BringWindowToTop();
			::ShowWindow(h, SW_SHOWNORMAL);
//			::BringWindowToTop(h);
			SetForegroundWindow(h);
//			::MessageBox(NULL, "AAA", "AAA", MB_OK);
            return FALSE;
        }
        return FALSE;   
    }

#ifdef _AFXDLL
	Enable3dControls();			// 共有 DLL 内で MFC を使う場合はここをコールしてください。
#else
	Enable3dControlsStatic();	// MFC と静的にリンクする場合はここをコールしてください。
#endif

	


	CPicmoveDlg dlg;
	m_pMainWnd = &dlg;

	TRACE("Dialog start\n");

	int nResponse = dlg.DoModal();
	TRACE ("response %d\n", nResponse);
	if (nResponse == IDOK)
	{
		// TODO: ダイアログが <OK> で消された時のコードを
		//       記述してください。
//CClientDC
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ダイアログが <ｷｬﾝｾﾙ> で消された時のコードを
		//       記述してください。
	}



	ReleaseMutex( hMSP );    
	CloseHandle( hMSP );    

	// ダイアログが閉じられてからアプリケーションのメッセージ ポンプを開始するよりは、
	// アプリケーションを終了するために FALSE を返してください。

//	delete [] iniFileName;
	return FALSE;
}

void CPicmoveApp::saveIniFile(void)
{
	CWinApp* pApp = AfxGetApp();
	CString section = "ProgramData2";
	
	pApp->WriteProfileInt(section,		"bMiniSize", bMiniSize);
}

void CPicmoveApp::loadIniFile(void)
{
	CWinApp* pApp = AfxGetApp();
	CString section = "ProgramData2";

	bMiniSize =	pApp->GetProfileInt(section,	"bMiniSize", 0);
}
