// NykDrive.cpp: CNykDrive クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "picmove.h"
#include "NykDrive.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

driveElement::driveElement() {
	driveLetter = 0;
	fMount = false;
	fDcim = false;
	type = 0;
	//	mediaSize = 0;
	//	remainSize = 0;
}

driveElement::~driveElement() {}


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CNykDrive::CNykDrive() {
	drives_local.clear();
}

CNykDrive::~CNykDrive() {

}

int CNykDrive::getNumberDrive(void) {
	return (int) drives_local.size();
}

bool CNykDrive::getDriveInfo(int index_in, driveElement* element_in) {
	if(index_in < 0) return false;
	if(index_in >= drives_local.size()) return false;
	*element_in = drives_local[index_in];
	return true;
}


// cDriveLetter = A-Z
static bool isMountVolume(TCHAR cDriveLetter) {
	LPTSTR szVolumeFormat = TEXT("\\\\.\\%c:");
	TCHAR szVolumeName[8];
	HANDLE hVolume;

	wsprintf(szVolumeName, szVolumeFormat, cDriveLetter);

	TRACE("isMount %s\n", szVolumeName);
	hVolume = CreateFile(szVolumeName,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if(hVolume == INVALID_HANDLE_VALUE) {
		return false;
	}
	CloseHandle(hVolume);
	return true;
}

static bool existDcim(char cDriveLetter) {
	CString name;

	name.Format("%c:\\DCIM\\", cDriveLetter);
	DWORD atr = ::GetFileAttributes(name);
	TRACE("%s attr=%d\n", name, atr);
	if(atr == -1) return false;
	if(atr & FILE_ATTRIBUTE_DIRECTORY) return true;
	return false;
}



bool CNykDrive::setDriveInfoRemovableOnly(void) {
	drives_local.clear();
	DWORD fDrive = ::GetLogicalDrives();
	bool driveArray[32];

	DWORD i;
	for(i = 0; i < 32; i++) {
		if((1 << i) & fDrive) {
			driveArray[i] = true;
			//printf("%c:\n", 'A'+i);
		}
		else {
			driveArray[i] = false;
		}
	}

	unsigned char path[] = " :\\";
	for(i = 'D' - 'A'; i <= 'Z' - 'A'; i++) {
		if(driveArray[i]) {
			path[0] = 'A' + i;
			DWORD type = GetDriveType((const char*) path);
			if(type == DRIVE_REMOVABLE) {
				//				TRACE("drive %c\n", i + 'A');
				//				continue;

				driveElement tmp;

				tmp.driveLetter = 'A' + i;
				tmp.type = type;

				// ディスクがマウントされているか調べる
				tmp.fMount = isMountVolume(tmp.driveLetter);

				// ディスクのルートに DCIM フォルダがあるか調べる
				if(tmp.fMount) {
					tmp.fDcim = existDcim(tmp.driveLetter);
				}
				else {
					tmp.fDcim = false;
				}
				drives_local.push_back(tmp);

			}
		}
	}

	return true;
}

/*
UINT GetDriveType(
  LPCTSTR lpRootPathName   // pointer to root path
);

void DispDriveType()
{
	DWORD t;

	t = GetDriveType("c:\\");
	switch(t) {
	case 0:
		printf("判定不能\n");
		break;
	case 1:
		printf("存在しない\n");
		break;
	case DRIVE_REMOVABLE:
		printf("リムーバブル\n");
		break;
	case DRIVE_FIXED:
		printf("固定\n");
		break;
	case DRIVE_REMOTE:
		printf("リモート\n");
		break;
	case DRIVE_CDROM:
		printf("CD-ROM\n");
		break;
	case DRIVE_RAMDISK:
		printf("RAMディスク\n");
		break;
	}
}


*/