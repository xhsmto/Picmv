#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
#include <vector>
using namespace std;
#include <math.h>
// _mkdir �ȂǗp
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>

// 2�_�Ԃ̋�����Ԃ�
extern double nykGetDist(double x1, double x2, double y1, double y2, double z1, double z2);

// SJIS��1�o�C�g�ڂ��ǂ����𔻒f���� 2�o�C�g�ڂ̉\��������
extern bool iskanji1(unsigned char c);

// hsv �J���[�X�y�[�X���� RGB �J���[�X�y�[�X�ɕϊ�����
extern COLORREF hsv2rgb(double h, double s, double v);

// �w�肵����؂蕶���ŕ��������؂��Ĕz��ɂ��ĕԂ�
// src �� ch(1����)�ŋ�؂��� vector �Ƃ��ĕԂ�
extern void StringToArray(const CString& src, const CString& ch, vector<CString>& ret);

// name �Ƃ������O�̃f�B���N�g�������B�e�f�B���N�g�����Ȃ��Ƃ��͎���쐬����
extern bool nykMakeDir(const CString& name);

// ini�t�@�C�������s�t�@�C���Ɠ����f�B���N�g���ɍ쐬
extern void SetupIniFile(CWinApp* app, const CString& iniName);
extern void SetupIniFileLocalAppData(CWinApp* app, const CString& iniName);


// �^�C���X�^���v���X�V����B�X�V�E�쐬������ t �ɃZ�b�g����B
extern void	nykSetTimeStamp(CString fileName, time_t t_in);

// �t�H���_�I���_�C�A���O���ȒP�Ɏg�� 
extern bool nykGetFolder(HWND parent, CString& folder, const CString& default_folder, const CString& title_text);


// ����
extern int nykRandi(int min, int max);
extern double nykRandd(double min, double max);

extern bool nykFileReadOnlyOff(const CString& fileName);

extern int nykGetFileSize(const CString& fileName);

extern void CommandLineToArray(const CString& src, vector<CString>& ret);


// outname �o�͂������t�@�C���� �㏑�������
// exifname EXIF��񂪓����Ă�JPEG�t�@�C���� outname�Ɠ����ł��\��Ȃ� outname�ƈႤ�ꍇ�ی삳���
// image �o�͂������摜
bool WriteJpegWithExif(const CString& outname, const CString& exifname, CxImage& image_in, int q);

extern bool Escape4FileName(CString& name);
