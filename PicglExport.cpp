// PicglExport.cpp: CPicglExport クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "picmove.h"
#include "PicglExport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CPicglExport::CPicglExport(const CString &dir_in, const CString &mode)
{
	base_dir_name = dir_in;
	exp_file_name = CString(PICGL_EXPORT_FILENAME) + mode + ".dat";
}

CPicglExport::~CPicglExport()
{
}

void CPicglExport::Clear(void)
{
	Data.clear();
}

void CPicglExport::Add(const CString &fp)
{
	Data.push_back(fp);
}

bool CPicglExport::Get(int idx, CString &fp)
{
	if (idx < 0 || idx >= Data.size()) return false;
	fp = Data.at(idx);
	return true;
}

int CPicglExport::GetNumber(void)
{
	return (int)Data.size();
}

bool CPicglExport::Save(void)
{
	CString fn = base_dir_name + exp_file_name;
//	if (::GetFileAttributes(fn) != -1) return false;

	FILE *fp;
	CString dummy = fn + ".tmp";
	fp = fopen(dummy, "wt");
	if (fp == NULL) return false;

	fprintf(fp, PICGL_EXPORT_MAGIC "\n");
	for (int i = 0; i < Data.size(); i++) {
		fprintf(fp, "%s\n", Data.at(i));
	}
	fclose(fp);

	::DeleteFile(fn);
	if (::rename(dummy, fn) != 0) return false;
	return true;
}

bool CPicglExport::Load(void)
{
	FILE *fp;
	fp = fopen(base_dir_name + exp_file_name, "rt");
	if (fp == NULL) return false;
	char line_[8192];
	// 最初の行読み飛ばし
	fgets(line_, 8192, fp);
	if (strcmp(line_, PICGL_EXPORT_MAGIC "\n") != 0) {
//		AfxMessageBox("header error");
		return false;
	}

	Data.clear();
	while(!feof(fp)) {
		if (fgets(line_, 8192, fp) == NULL) break;
		char *p;
		p = strrchr(line_, 0x0d);
		if (p != 0) *p = 0;
		p = strrchr(line_, 0x0a);
		if (p != 0) *p = 0;
		CString line((char*)line_);
		if (line.GetLength() != 0) {
			Data.push_back(line);
		}
	}
	fclose(fp);

	return true;
}

bool CPicglExport::DeleteExpFile(void)
{
	CString fn = base_dir_name + exp_file_name;
	int ret = ::DeleteFile(fn);
	return ret ? true : false;
}

bool CPicglExport::Check(void)
{
	if (::GetFileAttributes(base_dir_name + exp_file_name) == -1) {
//		TRACE("Check %s ng\n", base_dir_name + exp_file_name);
		return false;
	}
//	TRACE("Check %s OK\n", base_dir_name + exp_file_name);
	return true;
}
