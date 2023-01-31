// PicglExport.h: CPicglExport クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICGLEXPORT_H__DC110D9B_C50B_44F8_8E72_89BCDF5C0A08__INCLUDED_)
#define AFX_PICGLEXPORT_H__DC110D9B_C50B_44F8_8E72_89BCDF5C0A08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PICGL_EXPORT_MAGIC "PICGLEXPORT_001"
#define PICGL_EXPORT_FILENAME "picgl_exp_"

class CPicglExport {
private:
	vector<CString> Data;
	CString base_dir_name;
	CString exp_file_name;
public:
	CPicglExport(const CString& dir_in, const CString& mode);
	virtual ~CPicglExport();

	void Clear(void);
	void Add(const CString& fullpath_in);
	bool Get(int idx, CString& fullpath_out);
	int GetNumber(void);
	bool Save(void);
	bool Load(void);
	bool DeleteExpFile(void);
	bool Check(void);

};

#endif // !defined(AFX_PICGLEXPORT_H__DC110D9B_C50B_44F8_8E72_89BCDF5C0A08__INCLUDED_)
