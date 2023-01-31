#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#include <vector>
using namespace std;
#include <math.h>
// _mkdir など用
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>

// 2点間の距離を返す
extern double nykGetDist(double x1, double x2, double y1, double y2, double z1, double z2);

// SJISの1バイト目かどうかを判断する 2バイト目の可能性もある
extern bool iskanji1(unsigned char c);

// hsv カラースペースから RGB カラースペースに変換する
extern COLORREF hsv2rgb(double h, double s, double v);

// 指定した区切り文字で文字列を区切って配列にして返す
// src を ch(1文字)で区切って vector として返す
extern void StringToArray(const CString& src, const CString& ch, vector<CString>& ret);

// name という名前のディレクトリを作る。親ディレクトリがないときは自ら作成する
extern bool nykMakeDir(const CString& name);

// iniファイルを実行ファイルと同じディレクトリに作成
extern void SetupIniFile(CWinApp* app, const CString& iniName);
extern void SetupIniFileLocalAppData(CWinApp* app, const CString& iniName);


// タイムスタンプを更新する。更新・作成両方を t にセットする。
extern void	nykSetTimeStamp(CString fileName, time_t t_in);

// フォルダ選択ダイアログを簡単に使う 
extern bool nykGetFolder(HWND parent, CString& folder, const CString& default_folder, const CString& title_text);


// 乱数
extern int nykRandi(int min, int max);
extern double nykRandd(double min, double max);

extern bool nykFileReadOnlyOff(const CString& fileName);

extern int nykGetFileSize(const CString& fileName);

extern void CommandLineToArray(const CString& src, vector<CString>& ret);


// outname 出力したいファイル名 上書きされる
// exifname EXIF情報が入ってるJPEGファイル名 outnameと同じでも構わない outnameと違う場合保護される
// image 出力したい画像
bool WriteJpegWithExif(const CString& outname, const CString& exifname, CxImage& image_in, int q);

extern bool Escape4FileName(CString& name);
