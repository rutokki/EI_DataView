// DataViewExDoc.h : interface of the CDataViewExDoc class
//

#pragma once


class CDataViewExDoc : public CDocument
{
protected: // create from serialization only
	CDataViewExDoc();
	DECLARE_DYNCREATE(CDataViewExDoc)
	BOOL m_bMainDataLoaded;
	BOOL m_bIOCardLoaded;
	BOOL m_bLogicLoaded;
	BOOL m_bLDatLoaded;
	// Attributes
	CString StationName;
public:

	// Operations
public:

	// Overrides
public:
	virtual BOOL OnNewDocument();
	CString GetFileStationName(CString& fileName);
	//BOOL ReadLogicContsFile(CString pszPathNm);
	BOOL ReadMainBinData(CString filePath);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);

	BOOL ReadIOCardData(CString& filePath);
	BOOL ReadLogicVariableData(CString& filePath);
	// Implementation
public:
	virtual ~CDataViewExDoc();
#ifdef _DEBUG

	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:


	//void GetFileStationName(CString& fileName);
	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


