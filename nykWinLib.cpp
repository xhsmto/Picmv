#include "stdafx.h"
#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
#include <vector>
using namespace std;
#include <math.h>
// _mkdir �ȂǗp
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

// ����
int nykRandi(int min, int max) {
	double ret = min + (max - min) * ((double) rand() / RAND_MAX);
	return (int) ret;
}

double nykRandd(double min, double max) {
	double ret = min + (max - min) * ((double) rand() / RAND_MAX);
	return ret;
}

// SJIS�����̂P�o�C�g�ڂ̃R�[�h
const BYTE SJIS1_S1 = 0x81;
const BYTE SJIS1_E1 = 0x9f;
const BYTE SJIS1_S2 = 0xe0;
const BYTE SJIS1_E2 = 0xf5;

// SJIS�����̂Q�o�C�g�ڂ̃R�[�h
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

// �w�肵����؂蕶���ŕ��������؂��Ĕz��ɂ��ĕԂ�
// src �� ch(1����)�ŋ�؂��� vector �Ƃ��ĕԂ�
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


// �R�}���h���C�����X�y�[�X�ŕ������ĕԂ� "" �̒��̃X�y�[�X�͕������Ȃ�
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
			if(m.GetAt(len - 1) == 0x22) { // �Ō�̕�����"��������
				bKakko = false;
				tmp += " " + m.Left(len - 1);
				ret.push_back(tmp);
			}
			else {
				tmp += CString(" ") + m;
			}
		}
		else {
			if(m.GetAt(0) == 0x22) {	// 0�����ڂ��h��������
				if(m.GetAt(len - 1) == 0x22) { // �Ō�̕�����"��������
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

// SJIS��1�o�C�g�ڂ��ǂ����𔻒f���� 2�o�C�g�ڂ̉\��������
bool iskanji1(unsigned char c) {
	// 0x81 �` 0x9F�A���邢�� 0xE0 �` 0xFF 
	return (c >= 0x81 && c <= 0x9f) || (c >= 0xe0 && c <= 0xfc);
}


// hsv �J���[�X�y�[�X���� RGB �J���[�X�y�[�X�ɕϊ�����
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

// name �Ƃ������O�̃f�B���N�g�������B�e�f�B���N�g�����Ȃ��Ƃ��͎���쐬����
bool nykMakeDir(const CString& name) {
	// aaa\bbb\ccc\ �������� 1. aaa\ 2. aaa\bbb\ 3. aaa\bbb\ccc\ �Ə��ɍ쐬����
	TRACE("mkdir0 %s\n", name);

	// ���łɃf�B���N�g�������݂��Ă���?
	DWORD atr;
	atr = GetFileAttributes(name);
	TRACE("atr %d\n", atr);
	if(atr != -1) {
		if(atr & FILE_ATTRIBUTE_DIRECTORY) { TRACE("mkdir: already exist %s\n", name); return true; }	// ���łɑ��݂��Ă���
		else { TRACE("mkdir: %s is file\n", name); return false; } // �����t�@�C�������݂��Ă��A����͂�΂�
	}
	// �f�B���N�g���͑��݂��Ă��Ȃ�
	TRACE("mkdir %s\n", name);

	for(int i = 0; i < name.GetLength(); i++) {
		int c = name.GetAt(i);

		if(c == '\\') {
			// \ ���������̂ŁA�f�B���N�g���쐬����
			if(i != 0) {
				CString dir = name.Left(i);
				atr = GetFileAttributes(name);
				if(atr != -1) { TRACE("mkdir3: alread exist %s\n", dir); continue; }
				_mkdir(dir);
				TRACE("mkdir2 %s\n", dir);
			}
		}
		else if(i != name.GetLength() - 1) { // i ���Ō�̕������w���Ă��Ȃ����
			if(IsSJIS1(c) && IsSJIS2(name.GetAt(i + 1))) i++;	// SJIS�悯
		}
	}
	TRACE("mkdir4 end\n");
	return true;
}

// ini�t�@�C�������s�t�@�C���Ɠ����f�B���N�g���ɍ쐬
void SetupIniFile(CWinApp* app, const CString& iniName) {
	char dir[MAX_PATH];
	::GetModuleFileName(NULL, dir, MAX_PATH);  // ���s�t�@�C���̃p�X���擾
	char* pdest = strrchr(dir, '\\');          // ���s�t�@�C���̃p�X����
	if(pdest != NULL)	pdest[1] = '\0';       // ���s�t�@�C���������؂���
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

	free((void*) app->m_pszProfileName);             // �������̉��
	app->m_pszProfileName = _tcsdup(csIniFilePath);
}

static void ferr_msg(int num) {
	CString strMsg;

	switch(num) {
	case CFileException::none:
		strMsg = "�G���[�͂���܂���B";
		break;
	case CFileException::generic:
		strMsg = "���̃G���[�͒�`����Ă��܂���B";
		break;
	case CFileException::fileNotFound:
		strMsg = "�t�@�C����������܂���B";
		break;
	case CFileException::badPath:
		strMsg = "�p�X�����Ԉ���Ă��邩���݂��Ȃ��p�X�����w�肵�Ă��܂��B";
		break;
	case CFileException::tooManyOpenFiles:
		strMsg = "�I�[�v�����傤�Ƃ���t�@�C�������������܂��B";
		break;
	case CFileException::accessDenied:
		strMsg = "�t�@�C���A�N�Z�X���֎~����Ă��܂��B"
			"�ύX���悤�Ƃ���t�@�C���Ɠ����̃t�@�C�������ɑ��݂��܂��B";
		break;
	case CFileException::invalidFile:
		strMsg = "�t�@�C���n���h���������܂��͕s���ł��B";
		break;
	case CFileException::removeCurrentDir:
		strMsg = "��ƒ��̃J�����g�f�B���N�g���͍폜�ł��܂���B";
		break;
	case CFileException::directoryFull:
		strMsg = "�f�B���N�g���������ς��ł��B";
		break;
	case CFileException::badSeek:
		strMsg = "�t�@�C���|�C���^�̐ݒ�ɃG���[�������ăt�@�C���V�[�N���ł��܂���B";
		break;
	case CFileException::hardIO:
		strMsg = "�n�[�h�E�F�A�ɃG���[������܂��B";
		break;
	case CFileException::sharingViolation:
		strMsg = "SHARE.EXE�������[�h�ł��邩�A�܂��͋��L�̈悪���b�N����Ă��܂��B";
		break;
	case CFileException::lockViolation:
		strMsg = "���b�N�ςݗ̈�̃��b�N�v�������܂����B";
		break;
	case CFileException::diskFull:
		strMsg = "�f�B�X�N�������ς��ł��B";
		break;
	case CFileException::endOfFile:
		strMsg = "�t�@�C���̏I���ɓ��B���܂����B";
		break;
	}
	TRACE(strMsg);
	TRACE("\n");
}

// �^�C���X�^���v���X�V����B�X�V�E�쐬������ t �ɃZ�b�g����B
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

// �t�@�C���̃��[�h�I�����[�������͂���
bool nykFileReadOnlyOff(const CString& fileName) {
	CFileStatus status;
	CFile::GetStatus(fileName, status);
	status.m_attribute &= (~0x01);	// readonly �͂���
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
		// wParam �� TRUE  �Ȃ�p�X��
		//          FALSE �Ȃ�pidl
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
		bi.lpfn = BrowseCallbackProc;        // �R�[���o�b�N�֐����w�肵�܂�

		pidl = SHBrowseForFolder(&bi);

		if(pidl) {
			if(SHGetPathFromIDList(pidl, szDir)) {
				// �����Ƀt�H���_���I�����ꂽ���̏������L�q���Ă��������B
				// ��Ƃ��āA�I�����ꂽ�t�H���_�̃p�X��\�����܂��B
//				MessageBox(szDir, "�I���t�H���_", MB_OK);
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

// outname �o�͂������t�@�C���� �㏑�������
// exifname EXIF��񂪓����Ă�JPEG�t�@�C���� outname�Ɠ����ł��\��Ȃ� outname�ƈႤ�ꍇ�ی삳���
// image �o�͂������摜
bool WriteJpegWithExif(const CString& outname, const CString& exifname, CxImage& image_in, int q) {
	CxImageJPG image;


	FILE* fp = fopen(exifname, "rb");
	if(!fp) return false;
	image.Decode(fp);
	fclose(fp);
	if(!image.IsValid()) return false;

	image.Copy(image_in, true, false, false);
	if(!image.IsValid()) return false;

	if(outname.CompareNoCase(exifname) == 0) {	// �������O
	}
	else {	// �Ⴄ���O
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

// �t�@�C�����Ɏg���Ȃ����� \/:*?"<>|
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
