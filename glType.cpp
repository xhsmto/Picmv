// glType.cpp: CglType クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "picmove.h"
#include "gltypedat.h"
#include "glType.h"
#include <math.h>
#include <vector>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

static int GlobalFrameCounter = 0;


CglAscii::CglAscii() {
	unsigned char b[] = {
		0x00,0x60,0x60,0x00,0x00,//"dot"
		0x20,0x10,0x08,0x04,0x02,//"slash"
		0x3E,0x41,0x41,0x41,0x3E,//"0"
		0x00,0x42,0x7F,0x40,0x00,//"1"
		0x72,0x49,0x49,0x49,0x46,//"2"
		0x22,0x41,0x49,0x49,0x36,//"3"
		0x18,0x14,0x12,0x7F,0x10,//"4"
		0x27,0x45,0x45,0x45,0x39,//"5"
		0x3E,0x49,0x49,0x49,0x32,//"6"
		0x03,0x01,0x61,0x19,0x07,//"7"
		0x36,0x49,0x49,0x49,0x36,//"8"
		0x26,0x49,0x49,0x49,0x3E,//"9"
		0x00,0x00,0x12,0x00,0x00,//"colon"
	};
	int i;
	for(i = 0; i < 128 * 8; i++) {
		Bits[i] = 0;
	}

	for(i = '.'; i <= ':'; i++) {
		for(int x = 0; x < 5; x++) {
			unsigned char src = b[5 * (i - '.') + x];
			for(int y = 0; y < 8; y++) {
				unsigned char* dst = &Bits[i * 8 + (7 - y)];
				if((src >> y) & 1) {
					*dst |= (1 << (7 - x));
				}
			}
		}
	}
}

CglAscii::~CglAscii() {}
/*
void glBitmap(
	GLsizei width , GLsizei height ,
	GLfloat xorig , GLfloat yorig ,
	GLfloat xmove , GLfloat ymove ,
	const GLubyte * bitmap
);
*/

void CglAscii::DrawString(double x, double y, double z, const CString& text) {
	glRasterPos3d(x, y, z);
	for(int i = 0; i < text.GetLength(); i++) {
		int c = text.GetAt(i);
		glBitmap(8, 8, 0, 0, 6, 0, &Bits[c * 8]);
	}
}

CglAscii g_ascii;

static void DrawPlane(double x0, double y0, double x1, double y1, double z, double div) {
	for(double x = x0; x < x1; x += div) {
		glBegin(GL_TRIANGLE_STRIP);
		for(double y = y0; y < y1; y += div) {
			glVertex3d(x + div, y, z);
			glVertex3d(x, y, z);
		}
		glEnd();
	}
}

void DrawCube(double access);

CglPicture::CglPicture() {
	active = false;
	x = y = z = 0.0;
	tgt_x = tgt_y = tgt_z = 0.0;
	tgt_x = 5.0;
	deg_y = 0.0;
	//	deg_z_tgt = deg_z = 0.0;
	x = 5.0;
	aspect_ratio = 1.0;
	mytexid = 0;
	bTextureLoaded = false;
}

CglPicture::~CglPicture() {
	// void glDeleteTextures(GLsizei n , const GLuint * textures);
	if(bTextureLoaded) {
		GLuint tex[1];
		tex[0] = mytexid;
		glDeleteTextures(1, &tex[0]);
	}
}

void CglPicture::Action(void) {
	double d = 0.1;

	x += (tgt_x - x) * d;
	y += (tgt_y - y) * d;
	z += (tgt_z - z) * d;
	//	deg_z += (deg_z_tgt - deg_z) * d;
}

//double camera_tgt_x, camera_tgt_y, camera_tgt_z;

void CglPicture::Draw(void) {
	glPushMatrix();
	glTranslated(x, y, z);
	glRotated(deg_y, 0, 1, 0);
	glScaled(1.0, 1.0 / aspect_ratio, 0.1);
	glTranslated(-0.5, 0.0, 0.0);

	// 座布団
	float zz = 0.95f;
	//	glDisable(GL_LIGHTING);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glColor4d(0.5, 0.5, 0.5, 1.0);
	glVertex3f(-1.3f, 1.5f, zz);
	glVertex3f(4.0f, 1.5f, zz);
	glColor4d(0.2, 0.2, 0.2, 1.0);
	glVertex3f(4.0f, -1.5f, zz);
	glVertex3f(-1.3f, -1.5f, zz);
	glEnd();
	//	glEnable(GL_LIGHTING);

		// 写真
	glBindTexture(GL_TEXTURE_2D, mytexid);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glColor4d(1.0, 1.0, 1.0, 1.0);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1, 1, 1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1, 1, 1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1, -1, 1);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1, -1, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// ヒストグラム
	glPopMatrix();
	glPushMatrix();
	glTranslated(x, y, z);
	glRotated(deg_y, 0, 1, 0);
	glTranslated(3.2, -0.6, 0.0);
	glScaled(2.5, 0.1, 0.1);

	glColor4d(1.0, 1.0, 1.0, 1.0);
	glBegin(GL_QUAD_STRIP);
	glNormal3f(0, 0, 1);
	for(int i = 0; i < 64; i++) {
		double x = (double) i / 64 - 1.0;
		double y0 = -1.0;
		double y1 = y0 + this->hist[i] * 0.01;
		glVertex3d(x, y0, 1.0);
		glVertex3d(x, y1, 1.0);
	}
	glEnd();
	glPopMatrix();
}

//static unsigned char test_buf[G_PICTURE_SIZE*G_PICTURE_SIZE*3];

void CglPicture::SetImage(void* buf, int width, int height, double aspect_ratio_in, int mytexid_in) {
	// testimage を作ってみる
	//	TRACE("SetImage %d x %d\n", width, height);
	/*	{
			for (int y = 0; y < G_PICTURE_SIZE; y++) {
			for (int x = 0; x < G_PICTURE_SIZE; x++) {
				unsigned char *p = &test_buf[3 * (x + y * G_PICTURE_SIZE)];
				*(p+0) = x;
				*(p+1) = y;
				*(p+2) = 0;
			}
			}
		}*/
		//	buf = test_buf;


		// 置き換える前に現在のテクスチャをクリア
		//	GLuint tmp_texid_array[1];
		//	tmp_texid_array[0] = mytexid;
		//	glDeleteTextures(1, &tmp_texid_array[0]);

		/*
		void glTexSubImage2D(
			GLenum target, GLint level,
			GLint xoffset, GLint yoffset,
			GLsizei width, GLsizei height,
			GLenum format, GLenum type, const GLvoid *pixels
		);
		*/


	aspect_ratio = aspect_ratio_in;
	mytexid = mytexid_in;

	glBindTexture(GL_TEXTURE_2D, mytexid);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if(bTextureLoaded) {
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buf);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buf);
	}
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	active = true;
}

void CglPicture::Init(void) {}


//
CglCamera::CglCamera() {
	near_z = 0.1; far_z = 100.0; fovy = 40; aspect_ratio = 1.0;
	d_cam = 0.1; d_lookat = 0.15;
	//	d_cam = 0.07; d_lookat = 0.3;
}

CglCamera::~CglCamera() {}

void CglCamera::Set(const CglVector3& v_cam_in, const CglVector3& v_lookat_in) {
	v_cam = v_cam_in;
	v_lookat = v_lookat_in;
}

void CglCamera::SetLocal(const CglVector3& v_cam_in, const CglVector3& v_lookat_in) {
	v_cam_local = v_cam_in;
	v_lookat_local = v_lookat_in;
}

void CglCamera::Action(void) {
	double d = d_cam;

	v_cam_local.x += (v_cam.x - v_cam_local.x) * d;
	v_cam_local.y += (v_cam.y - v_cam_local.y) * d;
	v_cam_local.z += (v_cam.z - v_cam_local.z) * d;

	d = d_lookat;
	v_lookat_local.x += (v_lookat.x - v_lookat_local.x) * d;
	v_lookat_local.y += (v_lookat.y - v_lookat_local.y) * d;
	v_lookat_local.z += (v_lookat.z - v_lookat_local.z) * d;
}

void CglCamera::Project(void) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspect_ratio, near_z, far_z); // fovy aspect near far

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(this->v_cam_local.x, v_cam_local.y, v_cam_local.z,	/* 視点位置   */
		v_lookat_local.x, v_lookat_local.y, v_lookat_local.z,
		0.0, 1.0, 0.0);	/* 上方向     */
}


//
CglMediaElement::CglMediaElement() {
	x = y = z = 0.0;
	tgt_x = tgt_y = tgt_z = 0.0;
	rx = ry = rz = 0.0;
	megabyte_text = "";
	drive_letter = 0;
	mode = MEDIA_MODE_DEATH;
	access = 1.0;
}

CglMediaElement::~CglMediaElement() {}

void CglMediaElement::Action(void) {
	if(access > 0.0 && access <= 1.0) access -= 0.05;
	double d;
	switch(mode) {
	case MEDIA_MODE_DEATH: return;
	case MEDIA_MODE_EXIST: return;
	case MEDIA_MODE_INSERT:
		d = 0.1;
		break;
	case MEDIA_MODE_EJECT:
		d = 0.005;
		break;
	}

	x += (tgt_x - x) * d;
	y += (tgt_y - y) * d;
	z += (tgt_z - z) * d;

	double dist = nykGetDist(x, tgt_x, y, tgt_y, z, tgt_z);
	switch(mode) {
	case MEDIA_MODE_INSERT:
		if(dist < 0.1) {
			mode = MEDIA_MODE_EXIST;
			x = tgt_x;
			y = tgt_y;
			z = tgt_z;
		}
		break;
	case MEDIA_MODE_EJECT:
		if(dist < 0.1) {
			mode = MEDIA_MODE_DEATH;
			x = tgt_x;
			y = tgt_y;
			z = tgt_z;
		}
		break;
	}
}

void CglMediaElement::Draw(void) {
	if(mode == MEDIA_MODE_DEATH) return;

	static double deg = 0;
	deg += 2.0;


	// ドライブレター描画	
	glPushMatrix();
	glTranslated(x - 0.15, y + 0.4, z + 0.2);
	glScaled(0.5, 0.5, 0.5);
	glColor4d(1.0, 1.0, 0.0, 1.0);
	::DrawChar(drive_letter);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x + 0.15, y + 0.4, z + 0.2);
	glScaled(0.5, 0.5, 0.5);
	glColor4d(1.0, 1.0, 0.0, 1.0);
	::DrawChar(':');
	glPopMatrix();

	// テキスト描画
	glColor4d(1.0, 0.0, 0.0, 0.5);
	for(int i = 0; i < megabyte_text.GetLength(); i++) {
		glPushMatrix();
		glTranslated(x, y, z);
		glTranslated((i - 1) * 0.175, -0.2, 0.2);
		glScaled(0.25, 0.25, 0.25);
		::DrawChar(megabyte_text.GetAt(i));
		glPopMatrix();

	}

	// メディアのキューブ描画
	glPushMatrix();
	glTranslated(x, y, z + 0.1);
	glScaled(0.45, 1.0, 0.025);
	::DrawCube(access);
	glPopMatrix();

}

void CglMediaElement::Insert(double x_in, double y_in, double z_in,
	double tgt_x_in, double tgt_y_in, double tgt_z_in,
	CString& megabyte_text_in, int drive_letter_in) {
	x = x_in; y = y_in; z = z_in;
	tgt_x = tgt_x_in; tgt_y = tgt_y_in; tgt_z = tgt_z_in;
	megabyte_text = megabyte_text_in;
	drive_letter = drive_letter_in;
	mode = MEDIA_MODE_INSERT;

}

void CglMediaElement::Eject(
	double tgt_x_in, double tgt_y_in, double tgt_z_in) {
	tgt_x = tgt_x_in; tgt_y = tgt_y_in; tgt_z = tgt_z_in;
	mode = MEDIA_MODE_EJECT;
}



//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CglType::CglType() {
	old_type_string = "";
	mode = 0;
	fInit = false;
	for(int i = 0; i < PIC_BUFFERS; i++) {
		request_update_buf[i] = false;
		request_aspect_ratio[i] = 1.0;
		time(&request_time[i]);
	}
	counter_picidx = 0;
	aspect_ratio = 1.0;
	time(&now_time);
	time(&now_time_local);
	realTimeClock = true;
	clock_y = clock_local_y = -50;
	programSwitch = 0;
	messageText = "";
	messageTimer = 0;
}

CglType::~CglType() {

}

// 指定した区切り文字で文字列を区切って配列にして返す
// src を ch(1文字)で区切って vector として返す
/*
static void StringToArray(const CString &src, const CString &ch, vector<CString> &ret)
{
	ret.clear();

	if (src.GetLength() == 0) return;

	int n = 0;
	while (1) {
		int c = src.Find(ch, n);
		if (c == -1) {
			ret.push_back(src.Mid(n));
			break;
		}
		ret.push_back(src.Mid(n, c-n));
		n = c + 1;
	}
}
*/

static void	nykGetFreeSpaceText(int c_idx, CString& text) {
	ULARGE_INTEGER freeu, total, free;
	CString dir;
	dir.Format("%c:\\", c_idx + 'A');
	if(!::GetDiskFreeSpaceEx(dir, &freeu, &total, &free)) {
		text = "N/A";
		return;
	}

	signed __int64 m = total.QuadPart;
	double md = m;
	int mega = 0.5 + md / 1000.0 / 1000.0;
	if(mega >= 950) {
		int giga = 0.5 + (double) mega / 1000.0;
		text.Format("%dG", giga);
	}
	else {
		text.Format("%d", mega);
	}
}


// I:\DCIM\,J:\DCIM\ とかはいってくる
void CglType::Type(CString& type_string_in) {
	//	return;
	if(!fInit) return;

	TRACE("gltype %s\n", type_string_in);
	CString type_string = type_string_in;
	type_string.MakeUpper();
	old_type_string.MakeUpper();
	if(old_type_string == type_string) {	// 更新なし
		return;
	}
	TRACE("Gltype %s\n", type_string);

	// 頭一文字から drive_letter= 'A'-'Z'

	vector<CString> old_dirs;
	StringToArray(old_type_string, ",", old_dirs);
	vector<CString> new_dirs;
	StringToArray(type_string, ",", new_dirs);

	int look_min = 30;
	int look_max = -1;

	// INSERT 処理 - 追加されたものを探す
	vector<CString>::iterator i, j;
	for(i = new_dirs.begin(); i != new_dirs.end(); i++) {
		if(i->GetLength() == 0) continue;
		int hit = false;
		for(j = old_dirs.begin(); j != old_dirs.end(); j++) {
			// 頭一文字のみを比較
			if(i->GetLength() >= 1 && j->GetLength() >= 1) {
				if(toupper(i->GetAt(0)) == toupper(j->GetAt(0))) { hit = true; break; }
			}
		}
		if(!hit) { // ヒットがない=古いものの中に新しいのが見つからない=INSERT

			int c = i->GetAt(0);
			if(c >= 'A' && c <= 'Z') {
				int c_idx = c - 'A';
				CString text;
				nykGetFreeSpaceText(c_idx, text);
				media[c_idx].Insert(c_idx, -5.0, 0.0,
					c_idx, 0.0, 0.0,
					text, c);

			}
		}
	}

	// EJECT 処理 - 削除されたものを探す
	for(i = old_dirs.begin(); i != old_dirs.end(); i++) {
		if(i->GetLength() == 0) continue;
		int hit = false;
		for(j = new_dirs.begin(); j != new_dirs.end(); j++) {
			// 頭一文字のみを比較
			if(i->GetLength() >= 1 && j->GetLength() >= 1) {
				if(toupper(i->GetAt(0)) == toupper(j->GetAt(0))) { hit = true; break; }
			}
		}
		if(!hit) { // ヒットがない=EJECT
			int c = i->GetAt(0);
			if(c >= 'A' && c <= 'Z') {
				int c_idx = c - 'A';
				media[c_idx].Eject(c_idx, -10.0, 0.0);
			}
		}
	}

	look_min = 30;
	look_max = -1;
	for(i = new_dirs.begin(); i != new_dirs.end(); i++) {
		if(i->GetLength() >= 1) {
			int c = i->GetAt(0);
			if(c >= 'A' && c <= 'Z') {
				int c_idx = c - 'A';
				if(look_min > c_idx) look_min = c_idx;
				if(look_max < c_idx) look_max = c_idx;
			}
		}
	}
	for(i = old_dirs.begin(); i != old_dirs.end(); i++) {
		if(i->GetLength() >= 1) {
			int c = i->GetAt(0);
			if(c >= 'A' && c <= 'Z') {
				int c_idx = c - 'A';
				if(look_min > c_idx) look_min = c_idx;
				if(look_max < c_idx) look_max = c_idx;
			}
		}
	}

	if(look_max != -1) {
		CglVector3 pos, lookat;
		int avr = (look_min + look_max) / 2.0;
		int width = abs(look_max - look_min);
		double z;
		if(width <= 7) z = 3.0;
		else { z = width / 2; }
		pos.x = avr; pos.y = 0.0; pos.z = z;
		lookat.x = avr; lookat.y = 0.0; lookat.z = 0.0;
		camera.Set(pos, lookat);
	}
	old_type_string = type_string;
}

void CglType::DestroyGL(void) {
	wglMakeCurrent(0, 0);
	wglDeleteContext(m_GLRC);
	if(m_GLDC) delete m_GLDC;
}

bool CglType::InitGL(CWnd* cwnd) {
	CRect rect;
	TRACE("InitGL start\n");
	cwnd->GetClientRect(&rect);
	m_GLDC = new CClientDC(cwnd);		// Get device context  内部で this から GetDC() している CDC*
	HDC hdcGL = m_GLDC->GetSafeHdc();	// リソースで上位を透過にしなくてはまった
	TRACE("InitGL 0.2\n");

	SetDCPixelFormat(hdcGL);		// Set OpenGL pixel format
	TRACE("InitGL 0.5\n");
	m_GLRC = wglCreateContext(hdcGL);	// Create rendering context
	if(!m_GLRC) {
		AfxMessageBox("OpenGLの初期化に失敗しました");
		//        exit(1);
		return false;
	}
	TRACE("InitGL 1\n");

	wglMakeCurrent(hdcGL, m_GLRC);	// Current context set
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	aspect_ratio = (double) rect.Width() / rect.Height();
	camera.SetAspectRatio(aspect_ratio);
	cameraMessage.SetAspectRatio(aspect_ratio);

	{	// 一応カメラ位置もセットしておく
		CglVector3 pos, lookat;
		pos.x = 5.0; pos.y = 0.0; pos.z = 3.0;
		lookat.x = pos.x; lookat.y = pos.y; lookat.z = 0.0;
		camera.Set(pos, lookat);
		camera.SetLocal(pos, lookat);
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LIGHTING);   // Enable lighting
	glEnable(GL_LIGHT0);     // Light0 ON

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	//	glEnable(GL_BLEND);	// 半透明
	//	glEnable(GL_LINE_SMOOTH);	// 半透明
	//	glEnable(GL_POLYGON_SMOOTH);	// 半透明

	GLfloat v[4] = { 1.0f,1.0f,1.0f,1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, v);                               /* 白い光源 */
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);/* 鏡面反射処理をリアルに */
	glEnable(GL_NORMALIZE);                                  /* 法線計算を正規化 */

	static float position[] = { 0.0f, 0.2f, 1.0f, 0.0f };
	//	static float ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
	static float ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//	static float ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	GLfloat v2[2][4] = { {1.0f,0.2f,0.2f,1.0f},{1.0f,1.0f,1.0f,1.0f} };
	//	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,v2[1]);             /* 赤の反射 */
	glMaterialfv(GL_FRONT, GL_SPECULAR, v2[1]);             /* RGBすべて鏡面光反射 */
	glMaterialf(GL_FRONT, GL_SHININESS, 64);         /* 鏡面反射指数を中程度に */

	//	picture.Init();
	glGenTextures(PIC_BUFFERS, texid);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for(int i = 0; i < PIC_BUFFERS; i++) {
		glBindTexture(GL_TEXTURE_2D, texid[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	fInit = true;
	TRACE("InitGL end\n");
	return true;
}

int CglType::SetDCPixelFormat(HDC hdc) {
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// Size of this structure
		1,				// Version number
		PFD_DRAW_TO_WINDOW |		// Flags
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,			// RGBA pixel values
		24,				// 24-bit color
		0, 0, 0, 0, 0, 0,			// Don't care about these
		0, 0,				// No alpha buffer
		0, 0, 0, 0, 0,			// No accumulation buffer
		32,				// 32-bit depth buffer
		0,				// No stencil buffer
		0,				// No auxiliary buffers
		PFD_MAIN_PLANE,			// Layer type
		0,				// Reserved (must be 0)
		0, 0, 0				// No layer masks
	};

	int nPixelFormat;
	TRACE("SetDCPixelFormat 0\n");
	nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	TRACE("SetDCPixelFormat 0.5\n");
	if(SetPixelFormat(hdc, nPixelFormat, &pfd) == FALSE) {
		// SetPixelFormat error
		return FALSE;
	}

	TRACE("SetDCPixelFormat 1\n");
	if(DescribePixelFormat(hdc, nPixelFormat,
		sizeof(PIXELFORMATDESCRIPTOR), &pfd) == 0) {
		// DescribePixelFormat error
		return FALSE;
	}

	TRACE("SetDCPixelFormat 2\n");
	if(pfd.dwFlags & PFD_NEED_PALETTE) {
		// Need palete !
	}
	return TRUE;
}

static void DrawGrid(int mode) {

	glDisable(GL_LIGHTING);
	glLineWidth(1);

	glBegin(GL_LINES);
	if(mode == 0)	glColor3d(0.0, 0.4, 0.0);
	else glColor3d(0.4, 0.0, 0.0);

	int z = 0;
	double x0 = 0, x1 = 26;
	double y0 = -2, y1 = 2;
	for(double x = x0; x <= x1; x += 0.25) {
		glVertex3d(x, y0, z); glVertex3d(x, y1, z);
	}
	for(double y = y0; y <= y1; y += 0.25) {
		glVertex3d(x0, y, z); glVertex3d(x1, y, z);
	}
	glEnd();

	glEnable(GL_LIGHTING);
}

#define NYK_PI 3.1415926535897932384
//#define PIPE_RADIUS 7.0
#define PIPE_RADIUS 7.0
#define PIPE_Y0 5.0
#define PIPE_Y1 10.0

static void DrawHand2(void) {
	glBegin(GL_QUADS);

	// Z
	glNormal3f(0, 0, 1);
	glVertex3f(-1, 1, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(1, -1, 0);
	glVertex3f(-1, -1, 0);
	/*
		glNormal3f(0,0,-1);
		glVertex3f(-1, 1,-1);
		glVertex3f( 1, 1,-1);
		glVertex3f( 1,-1,-1);
		glVertex3f(-1,-1,-1);

		/// Ｘ軸に垂直な面
		glNormal3f(1,0,0);
		glVertex3f(1,-1,-1);
		glVertex3f(1, 1,-1);
		glVertex3f(1, 1, 1);
		glVertex3f(1,-1, 1);

		glNormal3f(-1,0,0);
		glVertex3f(-1,-1,-1);
		glVertex3f(-1, 1,-1);
		glVertex3f(-1, 1, 1);
		glVertex3f(-1,-1, 1);

		/// Ｙ軸に垂直な面
		glNormal3f(0,1,0);
		glVertex3f(-1, 1,-1);
		glVertex3f( 1, 1,-1);
		glVertex3f( 1, 1, 1);
		glVertex3f(-1, 1, 1);

		glNormal3f(0,-1,0);
		glVertex3f(-1,-1,-1);
		glVertex3f( 1,-1,-1);
		glVertex3f( 1,-1, 1);
		glVertex3f(-1,-1, 1);
	*/
	glEnd();
}

static void DrawHand(void) {
	glBegin(GL_LINES);
	glVertex3d(0, -1, 0);
	glVertex3d(0, 1, 0);
	glEnd();
}


static void DrawClock(time_t t_in, bool blur) {
	CTime t(t_in);
	int hh = t.GetHour() % 12;
	int mm = t.GetMinute();
	int ss = t.GetSecond();
	static int old_ss;
	static time_t old_t_in = 0;

	if(old_t_in == 0) {
		old_t_in = t_in;
		old_ss = ss;
	}

	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glTranslated(0.0, 0.3, 0.0);

	// ざぶとん
	glColor4d(0.0, 0.0, 0.0, 0.5);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0, 0, 0);
	for(int i = 0; i <= 12; i++) {
		double d = 2.0 * NYK_PI * (-i / 12.0);
		glVertex3d(2.5 * cos(d), 2.5 * sin(d), 0);
	}
	glEnd();

	// hour
	glColor4d(1.0, 1.0, 1.0, 1.0);
	glLineWidth(4);
	glPushMatrix();
	glRotated(-(double) (hh + mm / 60.0 + ss / 3600.0) / 12 * 360, 0, 0, 1);
	//	glScaled(1,1,1);
	glTranslated(0, 0.7, 0);
	DrawHand();
	glPopMatrix();

	// Min
	glLineWidth(3);
	glPushMatrix();
	glRotated(-(double) (mm + ss / 60.0) / 60 * 360, 0, 0, 1);
	glScaled(1, 1.4, 1);
	glTranslated(0, 0.7, 0);
	DrawHand();
	glPopMatrix();

	// ascii
	glColor4d(1.0, 1.0, 1.0, 1.0);
	CString ascii_clock;
	ascii_clock.Format("%02d.%02d.%02d", t.GetYear() % 100, t.GetMonth(), t.GetDay());
	g_ascii.DrawString(-1.85, -3.0, 0, ascii_clock);

	// Sec
	glLineWidth(1);
	//	if (t_in == old_t_in) glColor4d(1.0,0.0,0.0,1.0);
	//	else if (t_in < old_t_in) glColor4d(0.0,0.0,1.0,1.0);
	//	else glColor4d(1.0,0.0,1.0,1.0);
	glColor4d(1.0, 0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslated(0, 0, 0.0);
	glRotated(-(double) ss / 60 * 360, 0, 0, 1);
	glScaled(1, 1.4, 1);
	glTranslated(0, 0.7, 0);
	DrawHand();
	glPopMatrix();

	if(blur) {
		// 秒針残像
		if(t_in - old_t_in >= 1) { // 進み
			glColor4d(0.0, 0.0, 0.0, 0.0);
			glBegin(GL_TRIANGLE_FAN);
			glVertex3d(0, 0, 0);
			int max;
			if(old_ss > ss) max = ss + 60;
			else max = ss;
			for(int ss2 = old_ss; ss2 <= max; ss2++) {
				double d = (-ss2 + 15) / 60.0 * 2.0 * NYK_PI;
				double a = 1.0 * (double) (ss2 - old_ss) / (max - ss2);
				glColor4d(0.5, 0.0, 0.0, a);
				glVertex3d(2.4 * cos(d), 2.4 * sin(d), 0);
			}
			glEnd();
		}
		else if(t_in - old_t_in <= 1) { // 遅れ
			glColor4d(0.0, 0.0, 0.0, 0.0);
			glBegin(GL_TRIANGLE_FAN);
			glVertex3d(0, 0, 0);
			int min;
			if(old_ss < ss) min = ss - 60;	// ex ss=50 old_ss=10
			else min = ss;
			for(int ss2 = min; ss2 <= old_ss; ss2++) {
				double d = (-ss2 + 15) / 60.0 * 2.0 * NYK_PI;
				double a = 1.0 * (double) (ss2 - min) / (old_ss - ss2);
				glColor4d(0.5, 0.0, 0.0, a);
				glVertex3d(2.4 * cos(d), 2.4 * sin(d), 0);
			}
			glEnd();
		}
	}

	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);
	old_ss = ss;
	old_t_in = t_in;
}

static void DrawPictureGrid(int mode) {
	glDisable(GL_LIGHTING);
	glLineWidth(1);

	glBegin(GL_LINES);
	if(mode == 0)	glColor3d(0.0, 0.4, 0.0);
	else glColor3d(0.4, 0.0, 0.0);

	// 円柱を描いてみる
	double radius = PIPE_RADIUS;
	double y0 = PIPE_Y0, y1 = PIPE_Y1;
	for(int d = 0; d <= 355; d += 5) {
		double r = (double) d / 180 * NYK_PI;
		double r2 = (double) (d + 5) / 180 * NYK_PI;
		// まず縦線
		double x = cos(r) * radius;
		double z = sin(r) * radius;
		double x2 = cos(r2) * radius;
		double z2 = sin(r2) * radius;
		glVertex3d(x, y0, z);
		glVertex3d(x, y1, z);
		// で、横線
		for(double y = y0; y <= y1; y += 0.5) {
			glVertex3d(x, y, z);
			glVertex3d(x2, y, z2);
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

#define NYK_MESSAGE_REMAIN_TIME (30)

void CglType::SetMessage(const CString& message_in) {
	messageTimer = NYK_MESSAGE_REMAIN_TIME;
	messageText = message_in;
	programSwitch = 1;
	CglVector3 v_cam, v_lookat;
	v_cam.x = 0;
	v_cam.y = 0;
	v_cam.z = 4;
	v_lookat.x = 0;
	v_lookat.y = 0;
	v_lookat.z = 0;
	cameraMessage.Set(v_cam, v_lookat);
	//	v_cam.x = nykRandd(-1,1);
	//	v_cam.y = nykRandd(-1,1);
	//	v_cam.z = nykRandd(0,3);
	//	v_lookat.x = nykRandd(-1,1);
	//	v_lookat.y = nykRandd(-1,1);
	//	v_lookat.z = nykRandd(0,3);
	// 教訓　スレッドごとに乱数のシードはあるみたい。あたりまえか。
	v_cam.x = -0.99; //nykRandd(-1,1);
	v_cam.y = 0.127; //nykRandd(-1,1);
	v_cam.z = 0.5799; //nykRandd(0,3);
	v_lookat.x = 0.617; // nykRandd(-1,1);
	v_lookat.y = 0.17; // nykRandd(-1,1);
	v_lookat.z = 1.4396; //nykRandd(0,3);
	//	TRACE("XYZXYZ %f %f %f   %f %f %f\n", v_cam.x,v_cam.y,v_cam.z, v_lookat.x,v_lookat.y,v_lookat.z);
		// XYZXYZ -0.997497 0.127171 0.579913   0.617481 0.170019 1.439619
	cameraMessage.SetLocal(v_cam, v_lookat);
}

/*
void CglType::DrawGL(void)
{
	switch (this->programSwitch) {
	case 0:
		DrawGL_NormalProgram();
		break;
	case 1:
	default:
		DrawGL_MessageProgram();
		messageTimer--;
		if (messageTimer <= 0) programSwitch = 0;
		break;
	}
}

void CglType::DrawGL_MessageProgram(void)
{
	if (!fInit) return;
	static double cnt = 0;
	cnt += 0.05;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;

	cameraMessage.Action();
	cameraMessage.Project();

	glEnable(GL_LIGHTING);
	float position[4];

	position[0] = cameraMessage.GetX();
	position[1] = cameraMessage.GetY()+cos(cnt)*5.0;//0f; //sin(cnt)*10.0;
	position[2] = cameraMessage.GetZ();//n(cnt)*1.0;
	position[3] = 1.0f;	//
	glLightfv(GL_LIGHT0, GL_POSITION, position);

//	DrawGrid(0);

//	messageText = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcC0123";

	if (messageText.GetLength() >= 1) {
		double x = -messageText.GetLength() * 1.0 * 0.5, y = 0, z = 0;
		glColor4d(0.7, 0.7,0.7, 1.0);
		for (int i = 0; i < messageText.GetLength(); i++) {
			int ch = messageText.GetAt(i);
			glPushMatrix();
			glTranslated(x,y, z);
			glScaled(1.0,1.0,0.5);
			::DrawChar(ch);
			glPopMatrix();
			x += 1.0;
		}

	}
	//	glFinish();
	SwapBuffers( m_GLDC->GetSafeHdc());	// Double buffer

	GlobalFrameCounter++;
}
*/

void CglType::DrawGL(void) {
	if(!fInit) return;
	static double rrr = 0.0;
	static double cnt = 2.5;
	cnt += 0.01;

	if(mode == 0) glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	else glClearColor(0.15f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera.Action();
	camera.Project();

	glEnable(GL_LIGHTING);
	float position[4];

	position[0] = camera.GetX() + sin(cnt) * 3;
	//	position[1] = camera.GetY()-2.2;
	position[1] = camera.GetY() + cos(cnt) * 3;
	position[2] = camera.GetZ() + cos(cnt * 2) * 1;
	position[3] = 1.0f;
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	DrawGrid(mode);

	for(int c = 0; c < 26; c++) {
		media[c].Action();
		media[c].Draw();
	}

	if(this->m_glPicture) {
		glDisable(GL_DEPTH_TEST);
		DrawPictureGrid(mode);
		glEnable(GL_DEPTH_TEST);
		for(int i = 0; i < PIC_BUFFERS; i++) {
			if(request_update_buf[i]) {
				realTimeClock = false;

				// ヒストグラムコピー
				for(int j = 0; j < 64; j++) {
					picture[i].hist[j] = this->histgram[i][j];
				}
				// テクスチャーセット
				picture[i].SetImage(this->tmp_buf[i], G_PICTURE_SIZE, G_PICTURE_SIZE, request_aspect_ratio[i], texid[i]);
				rrr += nykRandd(0.9, 1.2);
				double r = rrr;
				double x = 0.9 * PIPE_RADIUS * cos(r);
				double z = 0.9 * PIPE_RADIUS * sin(r);
				double y = nykRandd(PIPE_Y0, PIPE_Y1);
				double tr = nykRandd(0.0, 1.3);

				picture[i].x = x * tr;// + nykRandd(-9,9);
				picture[i].y = y + nykRandd(-3, 3);
				picture[i].z = z * tr;
				picture[i].tgt_x = x;
				picture[i].tgt_y = y;
				picture[i].tgt_z = z;
				picture[i].deg_y = 180 + 90.0 - r / NYK_PI * 180.0;

				CglVector3 pos, lookat;
				lookat.x = picture[i].tgt_x;
				lookat.y = picture[i].tgt_y;
				lookat.z = picture[i].tgt_z;
				pos.x = PIPE_RADIUS * 0.6 * cos(r);
				pos.y = picture[i].tgt_y;
				pos.z = PIPE_RADIUS * 0.6 * sin(r);

				camera.Set(pos, lookat);
				now_time = request_time[i];

				request_update_buf[i] = false;
			}
			if(picture[i].GetActive()) {
				picture[i].Action();
				glPushMatrix();
				picture[i].Draw();
				glPopMatrix();
			}
		}
	}
	// 時計描画
	if(mode && m_glPicture) {
		clock_y = 0.0;
	}
	else {
		clock_y = -6.0;
	}
	clock_local_y += (clock_y - clock_local_y) * 0.15;
	if(clock_local_y > -5.0) {
		glDisable(GL_LIGHTING);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		double m = 2.8;
		glOrtho(-m * aspect_ratio, m * aspect_ratio, -m, m, 0.1, 20);

		//		glClear(GL_DEPTH_BUFFER_BIT) ;
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 11,	/* 視点位置   */
			0, 0, 0, 0.0, 1.0, 0.0);	/* 上方向     */
		glTranslated(-10, clock_local_y, 0);
		if(this->realTimeClock) {
			time_t t;
			time(&t);
			DrawClock(t, false);
			now_time = now_time_local = t;
		}
		else {
			DrawClock(now_time_local, true);
		}
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		if(now_time != now_time_local) {
			double d = (now_time - now_time_local) * 0.6;
			if(fabs(d) < 1) {
				now_time_local = now_time;
			}
			else {
				now_time_local += (time_t) d;
			}
		}
	}

	// メッセージ
	if(this->programSwitch == 1) {
		messageTimer--;
		if(messageTimer <= 0) programSwitch = 0;

		glClear(GL_DEPTH_BUFFER_BIT);

		cameraMessage.Action();
		cameraMessage.Project();
		glEnable(GL_BLEND);	// 半透明
		//		glEnable(GL_POLYGON_SMOOTH);	// 半透明
		glEnable(GL_LIGHTING);
		float position[4];

		position[0] = cameraMessage.GetX();
		position[1] = cameraMessage.GetY() + cos(cnt) * 5.0;
		position[2] = cameraMessage.GetZ();
		position[3] = 1.0f;	//
		glLightfv(GL_LIGHT0, GL_POSITION, position);

		if(messageText.GetLength() >= 1) {
			double x = -messageText.GetLength() * 1.0 * 0.5, y = 0, z = 0;
			double alpha = (double) messageTimer / NYK_MESSAGE_REMAIN_TIME;
			alpha *= 2.0; if(alpha > 1.0) alpha = 1.0;
			glColor4d(1.0, 1.0, 1.0, alpha);
			for(int i = 0; i < messageText.GetLength(); i++) {
				int ch = messageText.GetAt(i);
				glPushMatrix();
				glTranslated(x, y, z);
				glScaled(1.0, 1.0, 0.5);
				::DrawChar(ch);
				glPopMatrix();
				x += 1.0;
			}
		}
		glDisable(GL_BLEND);	// 半透明
		glDisable(GL_POLYGON_SMOOTH);	// 半透明

	}

	//	glFinish();
	SwapBuffers(m_GLDC->GetSafeHdc());	// Double buffer

	GlobalFrameCounter++;
}

void CglType::Access(int drive_in, double access_in) {
	if(drive_in < 0) return;
	if(drive_in >= 26) return;
	media[drive_in].access = access_in;
}

void CglType::Touch(CString& type_string_in) {
	CString type_string = type_string_in;
	type_string.MakeUpper();
	old_type_string.MakeUpper();

	vector<CString> new_dirs;
	StringToArray(type_string, ",", new_dirs);

	int look_min = 30;
	int look_max = -1;
	vector<CString>::iterator i;
	for(i = new_dirs.begin(); i != new_dirs.end(); i++) {
		if(i->GetLength() >= 1) {
			int c = i->GetAt(0);
			if(c >= 'A' && c <= 'Z') {
				int c_idx = c - 'A';
				if(look_min > c_idx) look_min = c_idx;
				if(look_max < c_idx) look_max = c_idx;
			}
		}
	}

	if(look_max != -1) {
		CglVector3 pos, lookat;
		int avr = (look_min + look_max) / 2.0;
		int width = abs(look_max - look_min);
		double z;
		if(width <= 7) z = 3.0;
		else { z = width / 2; }
		pos.x = avr; pos.y = 0.0; pos.z = z;
		lookat.x = avr; lookat.y = 0.0; lookat.z = 0.0;
		pos.z += 2.0 - 4.0 * (double) rand() / RAND_MAX;
		pos.x += 2.0 - 4.0 * (double) rand() / RAND_MAX;
		//		pos.z += 2.0 - 1.0 * (double)rand() / RAND_MAX; 
		//		pos.x += 2.0 - 1.0 * (double)rand() / RAND_MAX; 
		camera.Set(pos, lookat);
	}
}


void CglType::SetPicture(CxImage* image_in, int width, int height, double aspect_ratio_in, int pic_idx_dummy, time_t t) {
	int pic_idx = counter_picidx % PIC_BUFFERS;

	for(int i = 0; i < 64; i++) {
		histgram[pic_idx][i] = 0;
	}
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			RGBQUAD rgb;
			rgb = image_in->GetPixelColor(x, y, false);
			unsigned char* p = &tmp_buf[pic_idx][3 * (x + y * width)];
			*(p + 0) = rgb.rgbRed;
			*(p + 1) = rgb.rgbGreen;
			*(p + 2) = rgb.rgbBlue;
			histgram[pic_idx][((rgb.rgbRed + rgb.rgbBlue + rgb.rgbGreen) / 3) >> 2]++;

		}
	}
	request_update_buf[pic_idx] = true;
	request_aspect_ratio[pic_idx] = aspect_ratio_in;
	request_time[pic_idx] = t;
	counter_picidx++;
}



static void DrawChar(int c) {
	if(!(c >= 0x21 && c <= 0x5f)) return;

	int index = c - 0x21;
	nykGLPoint* poi_top = nyk_gltype_point_data[index];
	nykGLPolyIndex* poly_top = nyk_gltype_poly_index[index];

	glBegin(GL_POLYGON);

	for(nykGLPolyIndex* poly = poly_top; *poly != -2; poly++) {
		if(*poly == -1) {
			glEnd();
			glBegin(GL_POLYGON);
			//			glMaterialfv(GL_FRONT,GL_AMBIENT,green);
		}
		else {
			nykGLPoint* poi = &poi_top[*poly - 1];
			glNormal3d(poi->nx, poi->ny, poi->nz);
			glVertex3d(poi->x, poi->y, poi->z);
		}

	}
	glEnd();
}

static void DrawCube(double access) {
	int counter = GlobalFrameCounter;


	if(access < 1.0) {
		//		double c = 0.3 + access / 0.7;
		glColor4d(0.3, 0.3, 0.3, 1.0);
	}
	else {
		if((counter % 20) < 4) {
			glColor4d(0.2, 0.2, 0.2, 1.0);
		}
		else {
			double d;
			d = 1.0 - (counter % 20) / 40.0;
			glColor4d(d, 0.3, 0.3, 1.0);
		}
	}

	/// Z軸に垂直な面
//lMaterialfv(GL_FRONT,GL_AMBIENT,red);

	// いちばんの face
/*	glVertex3f(-1, 1,1);
	glVertex3f( 1, 1,1);
	glVertex3f( 1,-1,1);
	glVertex3f(-1,-1,1); */

	double z = 1.0;
	double x0 = -1, x1 = 1;
	double y0 = -1, y1 = 1;
	double div = 0.25;

	glNormal3f(0, 0, 1);
	for(double x = x0; x < x1; x += div) {
		glBegin(GL_TRIANGLE_STRIP);
		for(double y = y0; y <= y1; y += div) {
			glVertex3d(x + div, y, z);
			glVertex3d(x, y, z);
		}
		glEnd();
	}


	glBegin(GL_QUADS);

	glNormal3f(0, 0, -1);

	glVertex3f(-1, 1, -1);
	glVertex3f(1, 1, -1);
	glVertex3f(1, -1, -1);
	glVertex3f(-1, -1, -1);

	/// Ｘ軸に垂直な面
//	glMaterialfv(GL_FRONT,GL_AMBIENT,green);
	glNormal3f(1, 0, 0);

	glVertex3f(1, -1, -1);
	glVertex3f(1, 1, -1);
	glVertex3f(1, 1, 1);
	glVertex3f(1, -1, 1);

	glNormal3f(-1, 0, 0);

	glVertex3f(-1, -1, -1);
	glVertex3f(-1, 1, -1);
	glVertex3f(-1, 1, 1);
	glVertex3f(-1, -1, 1);

	/// Ｙ軸に垂直な面
//	glMaterialfv(GL_FRONT,GL_AMBIENT,blue);
	glNormal3f(0, 1, 0);

	glVertex3f(-1, 1, -1);
	glVertex3f(1, 1, -1);
	glVertex3f(1, 1, 1);
	glVertex3f(-1, 1, 1);

	glNormal3f(0, -1, 0);

	glVertex3f(-1, -1, -1);
	glVertex3f(1, -1, -1);
	glVertex3f(1, -1, 1);
	glVertex3f(-1, -1, 1);

	glEnd();
}

