
#if !defined(AFX_STDAFX_H__A7056F2A_D36E_4BD3_BADD_0B07956AC05B__INCLUDED_)
#define AFX_STDAFX_H__A7056F2A_D36E_4BD3_BADD_0B07956AC05B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します。

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分
#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <GL/gl.h>
#include <GL/glu.h>

#include <ximage.h>
#include <ximajpg.h>


#include "nykWinLib.h"

#define WM_TRAYICONMESSAGE (WM_USER+1)

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
//#define new  ::new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif	// _DEBUG


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__A7056F2A_D36E_4BD3_BADD_0B07956AC05B__INCLUDED_)
