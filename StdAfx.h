
#if !defined(AFX_STDAFX_H__A7056F2A_D36E_4BD3_BADD_0B07956AC05B__INCLUDED_)
#define AFX_STDAFX_H__A7056F2A_D36E_4BD3_BADD_0B07956AC05B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂��B

#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h>         // MFC �̊g������
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �R���� �R���g���[�� �T�|�[�g
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
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STDAFX_H__A7056F2A_D36E_4BD3_BADD_0B07956AC05B__INCLUDED_)
