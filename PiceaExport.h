// PiceaExport.h: CPiceaExport クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICEAEXPORT_H__DC110D9B_C50B_44F8_8E72_89BCDF5C0A08__INCLUDED_)
#define AFX_PICEAEXPORT_H__DC110D9B_C50B_44F8_8E72_89BCDF5C0A08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PICEA_EXPORT_MAGIC "PICEAEXPORT_001"
#define PICEA_EXPORT_FILENAME "picea_exp_"

class CPiceaExport {
private:
	vector<CString> Data;
	CString base_dir_name;
	CString exp_file_name;
public:
	CPiceaExport(const CString &dir_in, const CString &mode);
	virtual ~CPiceaExport();
	
	void Clear(void);
	void Add(const CString &fullpath_in);
	bool Get(int idx, CString &fullpath_out);
	int GetNumber(void);
	bool Save(void);
	bool Load(void);
	bool DeleteExpFile(void);
	bool Check(void);

};

#endif // !defined(AFX_PICEAEXPORT_H__DC110D9B_C50B_44F8_8E72_89BCDF5C0A08__INCLUDED_)
