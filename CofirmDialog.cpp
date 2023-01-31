// CofirmDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "picmove.h"
#include "CofirmDialog.h"
#include "DcimList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCofirmDialog ダイアログ


CCofirmDialog::CCofirmDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCofirmDialog::IDD, pParent) {
	//{{AFX_DATA_INIT(CCofirmDialog)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
//	text = "";
	dcimList.Clear();
	result = -1;
	bInit = false;
}


void CCofirmDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCofirmDialog)
	DDX_Control(pDX, IDC_LIST_CONFIRM, m_listConfirm);
	DDX_Control(pDX, IDC_CONFIRM_AREA, m_confirmArea);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCofirmDialog, CDialog)
	//{{AFX_MSG_MAP(CCofirmDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCofirmDialog メッセージ ハンドラ

BOOL CCofirmDialog::OnInitDialog() {
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください
//	this->SetDlgItemText(IDC_EDIT_CONFIRM, text);
	CDC* dc = this->GetDC();
	CRect rect;
	m_confirmArea.GetClientRect(&rect);
	confirmArea_memdc.CreateCompatibleDC(dc);
	confirmArea_bitmap.CreateCompatibleBitmap(dc, rect.Width(), rect.Height());
	old_confirmArea_bitmap = confirmArea_memdc.SelectObject(&confirmArea_bitmap);
	this->ReleaseDC(dc);



	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CCofirmDialog::SetData(CDcimList& dcimList_in) {
	dcimList = dcimList_in;


	CListCtrl& listControl = this->m_listConfirm;
	listControl.DeleteAllItems();

	// レポート形式のリストコントロール(listControl)へカラムヘッダーとデータを挿入するサンプル
#define WORK_SIZE 8192
	TCHAR   work[WORK_SIZE + 2];
	// 元ディレクトリ、元名前 新ディレクトリ 新ファイル名    
		// カラム挿入処理
	LV_COLUMN listcol;
	listcol.mask = LVCF_TEXT | LVCF_WIDTH;

	/*	{
			for (int i = 1; i <=4; i++) {
				listControl.DeleteColumn(i);
			}
		}*/

	int column;
	if(!bInit) {
		//	char *column_txt[] = {"元ディレクトリ", "元ファイル名", "コピー先ディレクトリ", "新ファイル名"};
		for(column = 1; column <= 4; column++) {
			//			listcol.pszText = column_txt[column-1];
			char mes[512];
			switch(column) {
			case 1: listcol.cx = 220; strcpy(mes, NYK_STRINGS142); listcol.pszText = mes; break;
			case 2: listcol.cx = 170; strcpy(mes, NYK_STRINGS143); listcol.pszText = mes; break;
			case 3: listcol.cx = 220; strcpy(mes, NYK_STRINGS144); listcol.pszText = mes; break;
			case 4: listcol.cx = 170; strcpy(mes, NYK_STRINGS145); listcol.pszText = mes; break;
			default: listcol.cx = 100; break;
			}
			listControl.InsertColumn(column, &listcol);
		}
	}
	bInit = true;

	// データの挿入
	LV_ITEM         lvitem;
	int max = dcimList.GetSizeList();
	for(int i = 0; i < max; i++) {
		Dcim d;
		CString text;
		dcimList.GetDcim(&d, i);
		// 行の挿入
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = i;
		lvitem.iSubItem = 0;
		_stprintf(work, _T("%d-1"), i);
		lvitem.pszText = work;
		listControl.InsertItem(&lvitem);
		// 列データの設定
		for(column = 1; column <= 4; column++) {
			lvitem.iSubItem = column - 1;
			//            _stprintf(work, _T("%d-%d"), i, column);
			switch(column) {
			case 1: strncpy(work, d.dir, WORK_SIZE); break;
			case 2: strncpy(work, d.orgName, WORK_SIZE); break;
			case 3: strncpy(work, d.newDir, WORK_SIZE); break;
			case 4:
				strncpy(work, d.newName, WORK_SIZE);
				break;
				//			case 4: 
				//				{
				//					strncpy(work, d.newThumbName, WORK_SIZE); 
				//				}
				//				break;
			}
			lvitem.pszText = work;
			listControl.SetItem(&lvitem);
		}

	}
	result = -1;

}

// confirmarea の仮想画面への描画をおこなう
void CCofirmDialog::drawConfirmArea(void) {
	//	confirmArea_memdc.TextOut(0,0,"AAA");
	int max = dcimList.GetSizeList();
	for(int i = 0; i < max; i++) {
		Dcim d;
		CString text;
		dcimList.GetDcim(&d, i);
		text = d.dir + d.orgName + "->" + d.newDir + d.newName + "\n";

		confirmArea_memdc.TextOut(0, i * 16, text);
	}
}

// confirmarea の転送をおこなう
void CCofirmDialog::transConfirmArea(void) {
	CDC* dc = this->m_confirmArea.GetDC();
	CRect rect;

	this->m_confirmArea.GetClientRect(&rect);
	dc->BitBlt(0, 0, rect.Width(), rect.Height(), &this->confirmArea_memdc, 0, 0, SRCCOPY);
	this->ReleaseDC(dc);
}

void CCofirmDialog::OnPaint() {
	//	CPaintDC dc(this); // 描画用のデバイス コンテキスト

		// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
		// 描画用メッセージとして CDialog::OnPaint() を呼び出してはいけません

	CDialog::OnPaint();
	//	drawConfirmArea();	
	//	transConfirmArea();	
}

void CCofirmDialog::OnDestroy() {
	CDialog::OnDestroy();

	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	this->confirmArea_memdc.SelectObject(this->old_confirmArea_bitmap);
}

void CCofirmDialog::OnOK() {
	// TODO: この位置にその他の検証用のコードを追加してください
	// 0=cancel 1=ok -1=idle
	result = 1;

	CDialog::OnOK();
}

void CCofirmDialog::OnCancel() {
	// TODO: この位置に特別な後処理を追加してください。
	result = 0;

	CDialog::OnCancel();
}
