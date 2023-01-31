// glType.h: CglType クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLTYPE_H__4989CEB4_AD96_4CC9_AF71_9A77F3AFF400__INCLUDED_)
#define AFX_GLTYPE_H__4989CEB4_AD96_4CC9_AF71_9A77F3AFF400__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MEDIA_MODE_DEATH (0)
#define MEDIA_MODE_INSERT (1)
#define MEDIA_MODE_EJECT (2)
#define MEDIA_MODE_EXIST (3)

//#define G_PICTURE_SIZE 128
#define G_PICTURE_SIZE 128

class CglVector3 {
public:
	CglVector3() { x = y = z = 0.0; }
	~CglVector3() {}
	double x, y, z;
};

class CglAscii {
private:
	GLubyte Bits[8 * 128];
public:
	CglAscii();
	~CglAscii();
	void DrawString(double x, double y, double z, const CString& text);
};

class CglPicture {
private:
	bool active;
	double aspect_ratio;
	//	unsigned char buffer[256*256*3];
	bool bTextureLoaded;
	int mytexid;

public:
	CglPicture();
	~CglPicture();
	unsigned short hist[256];
	double x, y, z;
	double tgt_x, tgt_y, tgt_z;
	double deg_y;
	//	double deg_z, deg_z_tgt;
	void Draw(void);
	void Action(void);
	void SetImage(void* buf, int width, int height, double aspect_ratio_in, int mytexid_in);
	void SetActive(bool active_in) { active = active_in; }
	bool GetActive(void) { return active; }
	void Init(void);
	//	void SetTexid(int id) { mytexid = id; }
};

class CglCamera {
private:
	CglVector3 v_cam_local;		// カメラ位置
	CglVector3 v_lookat_local;	// カメラ位置
	CglVector3 v_cam;			// カメラ位置
	CglVector3 v_lookat;		// カメラ位置
	double aspect_ratio, fovy, near_z, far_z;
	double d_cam, d_lookat;
public:
	CglCamera();
	~CglCamera();
	void Set(const CglVector3& v_cam_in, const CglVector3& v_lookat_in);
	void SetLocal(const CglVector3& v_cam_in, const CglVector3& v_lookat_in);
	inline double GetX(void) { return v_cam_local.x; }
	inline double GetY(void) { return v_cam_local.y; }
	inline double GetZ(void) { return v_cam_local.z; }
	inline void SetAspectRatio(double aspect_ratio_in) { aspect_ratio = aspect_ratio_in; }
	inline void SetFovy(double fovy_in) { fovy = fovy_in; }
	inline void SetZ(double near_z_in, double far_z_in) { near_z = near_z_in; far_z = far_z_in; }
	inline void SetTheDLookat(double d_in) { d_lookat = d_in; }
	inline void SetTheDCam(double d_in) { d_cam = d_in; }
	void Action(void);
	void Project(void);
};

class CglMediaElement {
private:
public:
	double x, y, z;	// 現在位置
	double tgt_x, tgt_y, tgt_z; // 目標位置
	double rx, ry, rz;
	CString megabyte_text;
	int drive_letter;
	double access;
	int mode;
	CglMediaElement();
	~CglMediaElement();
	// 移動とかやる
	void Action(void);
	// 描画
	void Draw(void);
	// 出現
	void Insert(double x_in, double y_in, double z_in,
		double tgt_x_in, double tgt_y_in, double tgt_z_in,
		CString& megabyte_text_in, int drive_letter_in);
	// 消滅
	void Eject(double tgt_x_in, double tgt_y_in, double tgt_z_in);
};

#define PIC_BUFFERS 2



class CglType {
private:
	bool fInit;
	HGLRC m_GLRC;
	CDC* m_GLDC;
	HWND hWnd;
	//	double aspect_ratio;
	int SetDCPixelFormat(HDC hdc);
	CString old_type_string;
	CglMediaElement media[26];
	CglCamera camera;
	CglPicture picture[PIC_BUFFERS];
	int mode;
	unsigned short histgram[PIC_BUFFERS][256];
	unsigned char tmp_buf[PIC_BUFFERS][G_PICTURE_SIZE * G_PICTURE_SIZE * 3];
	bool request_update_buf[PIC_BUFFERS];
	double request_aspect_ratio[PIC_BUFFERS];
	time_t request_time[PIC_BUFFERS];
	int m_glPicture;
	GLuint texid[32];
	int counter_picidx;
	double aspect_ratio;
	time_t now_time, now_time_local;
	bool realTimeClock;
	double clock_local_y, clock_y;
	//	HDC hdcGL_save;
		// メッセージ用カメラに切り替え
	int programSwitch;
	// メッセージ用カメラ
	CglCamera cameraMessage;
	// メッセージ記憶用
	CString messageText;
	// メッセージ用タイマー
	int messageTimer;
public:
	CglType();
	virtual ~CglType();
	bool InitGL(CWnd* cwnd);
	inline void SetGlPicture(int sw) { m_glPicture = sw; }
	void DrawGL();
	//	void DrawGL_NormalProgram();
	//	void DrawGL_MessageProgram();
	void DestroyGL();
	void Type(CString& type_string);
	void Access(int drive, double access_in);
	void Touch(CString& type_string);
	void SetMode(int mode_in) { mode = mode_in; }
	void SetPicture(CxImage* image_in, int width, int height, double aspect_ratio_in, int pic_idx, time_t t);
	// メッセージを投入する
	void SetMessage(const CString& message_in);
	inline void SetRealTimeClock(bool sw) { realTimeClock = sw; }
	inline void SetHwnd(HWND hwnd_in) { hWnd = hwnd_in; }
};

extern void DrawCube(void);
extern void DrawChar(int c);


#endif // !defined(AFX_GLTYPE_H__4989CEB4_AD96_4CC9_AF71_9A77F3AFF400__INCLUDED_)
