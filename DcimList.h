// DcimList.h: CDcimList �N���X�̃C���^�[�t�F�C�X
// JPEG �t�@�C����EXIF��񍞂݂ŊǗ�����N���X 050210-
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCIMLIST_H__53F6EBDC_CE94_419D_8C93_84A7C3013227__INCLUDED_)
#define AFX_DCIMLIST_H__53F6EBDC_CE94_419D_8C93_84A7C3013227__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace std;

#define NYK_FILE_INVALID 0
#define NYK_FILE_JPG 1
#define NYK_FILE_OTHER_IMAGE 2
#define NYK_FILE_OTHER_VIDEO 3

#define NEW_DIR_EX_MAX 16

typedef struct Dcim_t {
public:
	//	Dcim();
	CString dir;		// ���f�B���N�g����
	CString orgName;	// ���t�@�C����
	CString newName;	// �V�t�@�C����
	CString newThumbName;
	CString newDir;
	CString newDirEx[NEW_DIR_EX_MAX];
	//	CString newVideoDir;
	//	CString newVideoDirEx[NEW_DIR_EX_MAX];
	CString newThumbDir;
	CString maker, model;
	time_t shotTime;
	time_t fileTime;
	int orientation;	// ����
	int width, height;	// �T�C�Y
	double filesize;
	int errCode;
	bool copyDone;
	int fileFormat;
} Dcim;


class CDcimList {
private:
	vector<Dcim> dcim;
	CString extensions;
	CString extensions_video;
	int m_outDirs;
	int m_outDirsVideo;
public:
	CDcimList();
	virtual ~CDcimList();

	int GetCountOutDirs() { return m_outDirs; }
	int GetCountOutDirsVideo() { return m_outDirsVideo; }
	// �f�B���N�g�� dirList �̉��� JPEG �t�@�C�����ċA�I�Ƀ��X�g�A�b�v����
	void LoadList(CString dirList, void (*callback)(const CString&));

	void LoadListFromDragFiles(vector<CString>& dropFile_in, void (*callback)(const CString&));

	void SortByName();

	// ���X�g�A�b�v����Ă���t�@�C������Ԃ�
	int GetSizeList(void);

	// index �Ԗڂ̃t�@�C�������i�[����
	void GetDcim(Dcim* ret, int index);
	void SetDcim(Dcim* val, int index);

	void SetExifInfomation(const CString& naming_text, const CString& thumb_naming_text,
		const CString& naming_dir, const CString& thumb_naming_dir, const CString& naming_video_dir, void (*callback)(const CString&, int, int), int ex_upper,
		int counter_by_day);

	void Clear(void);

	// �Ώۊg���q�����Z�b�g����
	void SetExtensions(const CString& extensions_in);
	void SetExtensionsVideo(const CString& extensions_in);

	/*	void SetCopyDoneFlagOff(void);
		void Copy(const CString &outdir);
		void DeleteOrginal(void);*/
};

extern void nykRegularExtensions(CString& extensions_in);

extern void makeDcimFileName(CString& outname, time_t t_in, const CString& naming_text, time_t t_now, const CString& ext, const CString& original_name,
	const CString& maker, const CString& model, const CString& ext2, int counter_by_day);


#endif // !defined(AFX_DCIMLIST_H__53F6EBDC_CE94_419D_8C93_84A7C3013227__INCLUDED_)
