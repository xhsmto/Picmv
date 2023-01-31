#include "stdafx.h"
#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#include <vector>
using namespace std;
#include <math.h>
// _mkdir など用
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

// 乱数
int nykRandi(int min, int max) {
	double ret = min + (max - min) * ((double) rand() / RAND_MAX);
	return (int) ret;
}

double nykRandd(double min, double max) {
	double ret = min + (max - min) * ((double) rand() / RAND_MAX);
	return ret;
}

// SJIS文字の１バイト目のコード
const BYTE SJIS1_S1 = 0x81;
const BYTE SJIS1_E1 = 0x9f;
const BYTE SJIS1_S2 = 0xe0;
const BYTE SJIS1_E2 = 0xf5;

// SJIS文字の２バイト目のコード
const BYTE SJIS2_S1 = 0x40;
const BYTE SJIS2_E1 = 0x7e;
const BYTE SJIS2_S2 = 0x80;
const BYTE SJIS2_E2 = 0xfc;

BOOL IsSJIS1(BYTE c) {
	c ^= 0x20;
	if(c >= (SJIS1_S1 ^ 0x20) && c <= (SJIS1_E2 ^ 0x20)) return TRUE;
	return FALSE;
}

BOOL IsSJIS2(BYTE c) {
	if((c >= SJIS2_S1 && c <= SJIS2_E1) || (c >= SJIS2_S2 && c <= SJIS2_E2)) return TRUE;
	return FALSE;
}

// 指定した区切り文字で文字列を区切って配列にして返す
// src を ch(1文字)で区切って vector として返す
void StringToArray(const CString& src, const CString& ch, vector<CString>& ret) {

	ret.clear();

	if(src.GetLength() == 0) return;
	int n = 0;
	while(1) {
		int c = src.Find(ch, n);
		if(c == -1) {
			ret.push_back(src.Mid(n));
			break;
		}
		ret.push_back(src.Mid(n, c - n));
		n = c + 1;
	}
}


// コマンドラインをスペースで分解して返す "" の中のスペースは分解しない
void CommandLineToArray(const CString& src, vector<CString>& ret) {
	ret.clear();
	vector<CString> ret1;
	StringToArray(src, " ", ret1);
	//            F   T   T         T     F
	//	          0   1   2         3     4
		// ret1 = aaa "My Documents dddd" xxxx
		// ret  = aaa  My Documents dddd  xxxx
	//	          0    1                  2
	bool bKakko = false;
	CString tmp = "";
	int i;
	for(i = 0; i < ret1.size(); i++) {
		CString m = ret1.at(i);
		int len = m.GetLength();
		if(len == 0) continue;
		if(bKakko) {
			if(m.GetAt(len - 1) == 0x22) { // 最後の文字が"だったら
				bKakko = false;
				tmp += " " + m.Left(len - 1);
				ret.push_back(tmp);
			}
			else {
				tmp += CString(" ") + m;
			}
		}
		else {
			if(m.GetAt(0) == 0x22) {	// 0文字目が”だったら
				if(m.GetAt(len - 1) == 0x22) { // 最後の文字が"だったら
					ret.push_back(m.Mid(1, len - 2));
				}
				else {
					bKakko = true;
					tmp = m.Mid(1);
				}
			}
			else {
				ret.push_back(m);
			}
		}
		//		for (int k = 0; k < ret.size(); k++) {
		//			TRACE("-- %d: [%s]\n", k, ret.at(k));
		//		}

	}

	for(i = 0; i < ret.size(); i++) {
		ret.at(i).TrimLeft();
		ret.at(i).TrimRight();
		//		TRACE("%d: [%s]\n", i, ret.at(i));
	}

}


double nykGetDist(double x1, double x2, double y1, double y2, double z1, double z2) {
	double dist = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2);
	dist = sqrt(dist);
	return dist;
}

// SJISの1バイト目かどうかを判断する 2バイト目の可能性もある
bool iskanji1(unsigned char c) {
	// 0x81 ～ 0x9F、あるいは 0xE0 ～ 0xFF 
	return (c >= 0x81 && c <= 0x9f) || (c >= 0xe0 && c <= 0xfc);
}


// hsv カラースペースから RGB カラースペースに変換する
COLORREF hsv2rgb(double h, double s, double v) {
	double* r, * g, * b;
	double rr, gg, bb;
	double hf, hi, p1, p2, p3;

	r = &rr;
	g = &gg;
	b = &bb;

	h = fmod(h, 360);
	if(h < 0) h += 360;
	h /= 60;

	hf = modf(h, &hi);
	p1 = v * (1 - s);
	p2 = v * (1 - s * hf);
	p3 = v * (1 - s * (1 - hf));

	switch((int) hi) {
	case 0: *r = v * 255; *g = p3 * 255; *b = p1 * 255; break;
	case 1: *r = p2 * 255; *g = v * 255; *b = p1 * 255; break;
	case 2: *r = p1 * 255; *g = v * 255; *b = p3 * 255; break;
	case 3: *r = p1 * 255; *g = p2 * 255; *b = v * 255; break;
	case 4: *r = p3 * 255; *g = p1 * 255; *b = v * 255; break;
	case 5: *r = v * 255; *g = p1 * 255; *b = p2 * 255; break;
	}
	return RGB(rr, gg, bb);
}

// name という名前のディレクトリを作る。親ディレクトリがないときは自ら作成する
bool nykMakeDir(const CString& name) {
	// aaa\bbb\ccc\ だったら 1. aaa\ 2. aaa\bbb\ 3. aaa\bbb\ccc\ と順に作成する
	TRACE("mkdir0 %s\n", name);

	// すでにディレクトリが存在している?
	DWORD atr;
	atr = GetFileAttributes(name);
	TRACE("atr %d\n", atr);
	if(atr != -1) {
		if(atr & FILE_ATTRIBUTE_DIRECTORY) { TRACE("mkdir: already exist %s\n", name); return true; }	// すでに存在していた
		else { TRACE("mkdir: %s is file\n", name); return false; } // 同名ファイルが存在してるよ、これはやばい
	}
	// ディレクトリは存在していない
	TRACE("mkdir %s\n", name);

	for(int i = 0; i < name.GetLength(); i++) {
		int c = name.GetAt(i);

		if(c == '\\') {
			// \ を見つけたので、ディレクトリ作成する
			if(i != 0) {
				CString dir = name.Left(i);
				atr = GetFileAttributes(name);
				if(atr != -1) { TRACE("mkdir3: alread exist %s\n", dir); continue; }
				_mkdir(dir);
				TRACE("mkdir2 %s\n", dir);
			}
		}
		else if(i != name.GetLength() - 1) { // i が最後の文字を指していなければ
			if(IsSJIS1(c) && IsSJIS2(name.GetAt(i + 1))) i++;	// SJISよけ
		}
	}
	TRACE("mkdir4 end\n");
	return true;
}

// iniファイルを実行ファイルと同じディレクトリに作成
void SetupIniFile(CWinApp* app, const CString& iniName) {
	char dir[MAX_PATH];
	::GetModuleFileName(NULL, dir, MAX_PATH);  // 実行ファイルのパスを取得
	char* pdest = strrchr(dir, '\\');          // 実行ファイルのパスから
	if(pdest != NULL)	pdest[1] = '\0';       // 実行ファイル名だけ切り取る
	TRACE("dir %s\n", dir);
	::SetCurrentDirectory(dir);
	strcat(dir, iniName);
	free((void*) app->m_pszProfileName);
	app->m_pszProfileName = _tcsdup(dir);
}

void SetupIniFileLocalAppData(CWinApp* app, const CString& iniName) {
	CString csAppDataDir;
	CString csIniFilePath;
	CString csIniFileFolder;

	LPTSTR pBuf = csAppDataDir.GetBufferSetLength(MAX_PATH);
	SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, pBuf);
	csAppDataDir.ReleaseBuffer();

	csIniFileFolder.Format("%s\\nyk\\Picmv\\", csAppDataDir);
	nykMakeDir(csIniFileFolder);

	csIniFilePath.Format("%s%s", csIniFileFolder, iniName);

	free((void*) app->m_pszProfileName);             // メモリの解放
	app->m_pszProfileName = _tcsdup(csIniFilePath);
}

static void ferr_msg(int num) {
	CString strMsg;

	switch(num) {
	case CFileException::none:
		strMsg = "エラーはありません。";
		break;
	case CFileException::generic:
		strMsg = "このエラーは定義されていません。";
		break;
	case CFileException::fileNotFound:
		strMsg = "ファイルが見つかりません。";
		break;
	case CFileException::badPath:
		strMsg = "パス名が間違っているか存在しないパス名を指定しています。";
		break;
	case CFileException::tooManyOpenFiles:
		strMsg = "オープンしょうとするファイル数が多すぎます。";
		break;
	case CFileException::accessDenied:
		strMsg = "ファイルアクセスが禁止されています。"
			"変更しようとするファイルと同名のファイルが既に存在します。";
		break;
	case CFileException::invalidFile:
		strMsg = "ファイルハンドルが無効または不正です。";
		break;
	case CFileException::removeCurrentDir:
		strMsg = "作業中のカレントディレクトリは削除できません。";
		break;
	case CFileException::directoryFull:
		strMsg = "ディレクトリがいっぱいです。";
		break;
	case CFileException::badSeek:
		strMsg = "ファイルポインタの設定にエラーがあってファイルシークができません。";
		break;
	case CFileException::hardIO:
		strMsg = "ハードウェアにエラーがあります。";
		break;
	case CFileException::sharingViolation:
		strMsg = "SHARE.EXEが未ロードであるか、または共有領域がロックされています。";
		break;
	case CFileException::lockViolation:
		strMsg = "ロック済み領域のロック要求をしました。";
		break;
	case CFileException::diskFull:
		strMsg = "ディスクがいっぱいです。";
		break;
	case CFileException::endOfFile:
		strMsg = "ファイルの終わりに到達しました。";
		break;
	}
	TRACE(strMsg);
	TRACE("\n");
}

// タイムスタンプを更新する。更新・作成両方を t にセットする。
void nykSetTimeStamp(CString fileName, time_t t_in) {
	TRACE("********** nykSetTimeStamp start\n");
	CFileStatus rStatus;
	CTime t(t_in);
	if(CFile::GetStatus(fileName, rStatus)) {
		rStatus.m_ctime = rStatus.m_mtime = t;
		for(int i = 0; i < 3; i++) {
			int err = 0;
			TRY{
				CFile::SetStatus(fileName, rStatus);
			}
				CATCH(CFileException, e) {
				TRACE("CFileError %d\n", e->m_cause);
				ferr_msg(e->m_cause);
				err = 1;
			}
			END_CATCH
				if(err == 0) { TRACE("OK\n"); break; }
			TRACE("NG\n");
			Sleep(500);
		}
	}
	else {
		//		nykMessageBox("settimestamp error : "+fileName, MB_OK);
		TRACE("settimestamp error : " + fileName);
	}
	TRACE("********** nykSetTimeStamp end\n");
}

// ファイルのリードオンリー属性をはずす
bool nykFileReadOnlyOff(const CString& fileName) {
	CFileStatus status;
	CFile::GetStatus(fileName, status);
	status.m_attribute &= (~0x01);	// readonly はずし
	for(int i = 0; i < 3; i++) {
		int err = 0;
		TRY{
			CFile::SetStatus(fileName, status);
		}
			CATCH(CFileException, e) {
			TRACE("CFileError %d\n", e->m_cause);
			ferr_msg(e->m_cause);
			err = 1;
		}
		END_CATCH
			if(err == 0) return true;
		Sleep(500);
	}
	return false;
}


static char nyk_get_folder_default[MAX_PATH];

static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData) {
	switch(uMsg) {
	case BFFM_INITIALIZED:
		// wParam が TRUE  ならパス名
		//          FALSE ならpidl
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM) nyk_get_folder_default);
		break;
	default:
		break;
	}
	return 0;
}

bool nykGetFolder(HWND parent, CString& folder, const CString& default_folder, const CString& title_text) {
	BROWSEINFO bi;
	TCHAR szDir[MAX_PATH];
	LPITEMIDLIST pidl;
	LPMALLOC pMalloc;

	strcpy(nyk_get_folder_default, default_folder); //.GetBuffer(MAX_PATH));

	if(SUCCEEDED(SHGetMalloc(&pMalloc))) {
		ZeroMemory(&bi, sizeof(bi));
		bi.hwndOwner = parent;
		bi.pidlRoot = 0;
		bi.pszDisplayName = 0;
		bi.lpszTitle = title_text; //.GetBuffer(256);
		bi.ulFlags = BIF_RETURNONLYFSDIRS;
		//		bi.ulFlags = BIF_USENEWUI;
		bi.lpfn = BrowseCallbackProc;        // コールバック関数を指定します

		pidl = SHBrowseForFolder(&bi);

		if(pidl) {
			if(SHGetPathFromIDList(pidl, szDir)) {
				// ここにフォルダが選択された時の処理を記述してください。
				// 例として、選択されたフォルダのパスを表示します。
//				MessageBox(szDir, "選択フォルダ", MB_OK);
				folder = szDir;
			}
			pMalloc->Free(pidl);
		}
		pMalloc->Release();
		return true;
	}
	return false;
}

int nykGetFileSize(const CString& name) {
	struct _stat buf;
	if(_stat(name, &buf) == 0) {
		return buf.st_size;
	}
	return -1;
}

// outname 出力したいファイル名 上書きされる
// exifname EXIF情報が入ってるJPEGファイル名 outnameと同じでも構わない outnameと違う場合保護される
// image 出力したい画像
bool WriteJpegWithExif(const CString& outname, const CString& exifname, CxImage& image_in, int q) {
	CxImageJPG image;


	FILE* fp = fopen(exifname, "rb");
	if(!fp) return false;
	image.Decode(fp);
	fclose(fp);
	if(!image.IsValid()) return false;

	image.Copy(image_in, true, false, false);
	if(!image.IsValid()) return false;

	if(outname.CompareNoCase(exifname) == 0) {	// 同じ名前
	}
	else {	// 違う名前
		if(!::CopyFile(exifname, outname, false)) {
			return false;
		}
	}

	fp = fopen(outname, "r+b");
	if(!fp) return false;
	image.SetJpegQuality(q);
	if(!image.Encode(fp)) { fclose(fp); return false; }
	fclose(fp);

	return true;
}

// ファイル名に使えない文字 \/:*?"<>|
bool Escape4FileName(CString& name) {
	name.Replace("\\", "");
	name.Replace("/", "");
	name.Replace(":", "");
	name.Replace("*", "");
	name.Replace("?", "");
	name.Replace("<", "");
	name.Replace(">", "");
	name.Replace("|", "");
	name.Replace("\x22", "");
	return true;
}
