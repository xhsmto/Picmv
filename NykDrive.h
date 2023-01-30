// NykDrive.h: CNykDrive �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NYKDRIVE_H__6178AB81_28D4_4932_A9E1_85BD605CBF99__INCLUDED_)
#define AFX_NYKDRIVE_H__6178AB81_28D4_4932_A9E1_85BD605CBF99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace std;

class driveElement {
public:
	driveElement();
	~driveElement();
	char driveLetter;
	bool fMount;
	bool fDcim;
	int type;
//	long mediaSize, remainSize;

};

class CNykDrive  
{
private:
	vector<driveElement> drives_local;
public:
	CNykDrive();
	virtual ~CNykDrive();
	
// �����[�o���h���C�u�݂̂��I�u�W�F�N�g�Ɋi�[����
	bool setDriveInfoRemovableOnly(void);
// �h���C�u�����擾����
	int getNumberDrive(void);
// �h���C�u�����擾����
	bool getDriveInfo(int index, driveElement *element_in);
};

#endif // !defined(AFX_NYKDRIVE_H__6178AB81_28D4_4932_A9E1_85BD605CBF99__INCLUDED_)
