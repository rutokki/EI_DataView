// DataViewExDoc.cpp : implementation of the CDataViewExDoc class
//

#include "stdafx.h"
#include "DataViewEx.h"

#include "DataViewExDoc.h"
#include "StructMainData.h"
#include <fstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDataViewExDoc

IMPLEMENT_DYNCREATE(CDataViewExDoc, CDocument)

BEGIN_MESSAGE_MAP(CDataViewExDoc, CDocument)
END_MESSAGE_MAP()


// CDataViewExDoc construction/destruction

CDataViewExDoc::CDataViewExDoc()
{
	m_bMainDataLoaded = FALSE;
	m_bIOCardLoaded = FALSE;
	m_bLogicLoaded = FALSE;
	m_bLDatLoaded = FALSE; // [추가] 초기화

}

CDataViewExDoc::~CDataViewExDoc()
{
}

BOOL CDataViewExDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}
CString CDataViewExDoc::GetFileStationName(CString& fileName)
{
	int nLastBackslash = fileName.ReverseFind(_T('\\'));
	CString strStationName = fileName.Mid(nLastBackslash + 1);
	int nUnderscore = strStationName.Find(_T("_"));
	if (nUnderscore != -1)
	{
		strStationName = strStationName.Left(nUnderscore);
	}
	else {
		BCGPMessageBox(_T("정의되지 않은 파일 형식으로 역이름을 찾지 못했습니다."));
	}
	return strStationName;
}
//BOOL CDataViewExDoc::ReadLogicContsFile(CString pszPathNm)
//{
//	// 1. 경로 파싱: 전달받은 파일 경로에서 상위 폴더의 data_logic 경로 추출
//	int nLastBackslash = pszPathNm.ReverseFind(_T('\\'));
//	if (nLastBackslash == -1) return FALSE;
//	CString folderPath = pszPathNm.Left(nLastBackslash);
//
//	int nParentBackslash = folderPath.ReverseFind(_T('\\'));
//	if (nParentBackslash == -1) return FALSE;
//	CString parentFolderPath = folderPath.Left(nParentBackslash);
//
//	// 검색 경로 구성 (예: 역경로/../data_logic/*.LDAT)
//	CString strSearchPath = parentFolderPath + _T("\\data_logic\\*.LDAT");
//
//	CFileFind fileFinder;
//	BOOL bWorking = fileFinder.FindFile(strSearchPath);
//
//	// 매칭되는 LDAT 파일이 아예 없거나 경로를 찾지 못한 경우 실패 처리
//	if (!bWorking)
//	{
//		return FALSE;
//	}
//
//	while (TRUE == bWorking)
//	{
//		bWorking = fileFinder.FindNextFile();
//
//		if (fileFinder.IsDots() || fileFinder.IsDirectory())
//			continue;
//
//		CString fileName = fileFinder.GetFileName();
//		CString filePath = fileFinder.GetFilePath();
//
//		// 2. 스마트 포인터(`std::make_shared`)를 이용한 동적 객체 생성
//		auto ldatFileContent = std::make_shared<LDAT_FILE_CONTENT>();
//		ldatFileContent->fileName = fileName;
//
//		// 3. 모던 C++ 파일 스트림(std::ifstream)을 이용한 텍스트 읽기
//		std::ifstream file(filePath);
//		if (file.is_open())
//		{
//			std::string line;
//			// 버퍼 오버플로우 위험이 있는 fread 대신 std::getline으로 안전하게 한 줄씩 독출
//			while (std::getline(file, line))
//			{
//				ldatFileContent->contents.push_back(CString(line.c_str()));
//			}
//			file.close();
//		}
//		else
//		{
//			// 파일 열기 실패 시 탐색기 닫고 FALSE 반환
//			fileFinder.Close();
//			return FALSE;
//		}
//
//		// 4. 싱글톤 객체에 결과 컨테이너 주입
//		StructMainData::GetInstance().AddLDatFileContent(ldatFileContent);
//	}
//	fileFinder.Close();
//
//	return TRUE; // 모든 과정을 성공적으로 마치면 TRUE 반환
//}
BOOL CDataViewExDoc::ReadMainBinData(CString filePath)
{
	CFile file;
	CFileException ex;

	if (file.Open(filePath, CFile::modeRead | CFile::typeBinary, &ex))
	{
		std::unique_ptr<DBstruct> TSCDBStruct(new DBstruct());

		// 파일 읽기 (성공적으로 읽었는지 확인)
		if (file.Read(TSCDBStruct.get(), sizeof(DBstruct)) == sizeof(DBstruct))
		{
			StructMainData::GetInstance().AddEIDBStruct(std::move(TSCDBStruct));
			file.Close();
			return TRUE;
		}
		else
		{
			BCGPMessageBox(_T("bin 데이터 파일의 크기나 형식이 맞지 않습니다."));
			file.Close();
		}
	}
	else
	{
	}

	return FALSE;
}
// CDataViewExDoc serialization
BOOL CDataViewExDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	// 새 파일을 열기 전에 플래그 초기화
	m_bMainDataLoaded = FALSE;
	m_bIOCardLoaded = FALSE;
	m_bLogicLoaded = FALSE;
	m_bLDatLoaded = FALSE; // [추가] 초기화
	CString fullPath = lpszPathName;
	StructMainData::GetInstance().ClearAllData();
	// 1. 메인 파일 읽기
	if (!ReadMainBinData(fullPath))
	{
		return FALSE; // 메인파일 열기 실패 시 리턴
	}
	m_bMainDataLoaded = TRUE; // 메인 파일 성공 기록

	// 2. 형제 파일 경로 계산
	int nLastBackslash = fullPath.ReverseFind(_T('\\'));
	CString folderPath = fullPath.Left(nLastBackslash);
	CString fileName = fullPath.Mid(nLastBackslash + 1);

	CString parentFolderPath = folderPath.Left(nLastBackslash);
	CString dataLogicFolderPath = parentFolderPath + _T("\\data_logic");

	if (fileName.Find(L"_연동_데이터.bin") != -1)
	{
		StationName = GetFileStationName(fileName);
		CString strIOCardPath = folderPath + _T("\\") + StationName + _T("_ioCard.bin");
		CString strLogicPath = folderPath + _T("\\") + StationName + _T("_LogicVariable.Dat");

		// 3. IO카드 Logic변수 결과를 플래그에 저장
		m_bIOCardLoaded = ReadIOCardData(strIOCardPath);
		m_bLogicLoaded = ReadLogicVariableData(strLogicPath);

		//// [추가] data_logic 폴더 안의 모든 LDAT 파일 전체 읽어서 StructMainData에 저장
		//m_bLDatLoaded = ReadLogicContsFile(dataLogicFolderPath);
	}

	return TRUE;
}

// 기존 View에 있던 ReadIOCardData, ReadLogicVariableData, GetFileStationName 함수들을
// 여기로 그대로 옮겨오세요!
void CDataViewExDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		//char strFilePath[MAX_PATH] = { 0, };
		////CFileDialog fileOpen(TRUE, _T("bin"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||"));
		//// 빈파일만 열기
		//std::unique_ptr<DBstruct>TSCDBStruct(new DBstruct());
		//CFile* file = ar.GetFile();
		//file->Read(TSCDBStruct.get(), sizeof(DBstruct)); // TSCDBStruct 읽기
		//StructMainData::GetInstance().AddTSCDBStuct(std::move(TSCDBStruct));

		////file->Close();
		//file->Read(&m_TSCCardInfo, sizeof(CARD_INFO));
		//같은 경로에 있는 _T("%s_IOCard.bin") 파일 m_TSCCardInfo 읽기
		//같은 경로에 있는 로직변수 _T("%s_LogicVariable.Dat") 읽기
//#if defined _DEBUG
		//같은 경로에 있는 로직내용(*.LDAT) 읽기
//#endif
	}
}
BOOL CDataViewExDoc::ReadIOCardData(CString& filePath) // IOCard.bin 파일 읽기
{
	CFile file;
	CFileException ex;
	//if (!file.Open(filePath, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary, &ex))
	//{
	//	TCHAR szError[1024];
	//	ex.GetErrorMessage(szError, 1024);
	//	TRACE(_T("파일 열기 실패: %s, 원인 코드: %d\n"), szError, ex.m_cause);
	//	BCGPMessageBox(_T("Failed to open IOCard.bin file."));
	//	return;
	//}
	if (file.Open(filePath, CFile::modeRead | CFile::typeBinary))
	{
		std::unique_ptr<CARD_INFO>m_TSCCardInfo(new CARD_INFO());
		file.Read(m_TSCCardInfo.get(), sizeof(CARD_INFO));

		StructMainData::GetInstance().AddEICardStruct(std::move(m_TSCCardInfo));
		//file.Close();

		return TRUE;
	}
	else {

		BCGPMessageBox(_T("IO카드.bin 을 읽기 실패했습니다."));
		return FALSE;
	}
	return FALSE;
}

BOOL CDataViewExDoc::ReadLogicVariableData(CString& filePath)
{
	CFile file;
	CFileException ex;

	// 파일 열기 (공유 위반 방지 및 이진 읽기 모드)
	if (file.Open(filePath, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary, &ex))
	{
		// 1. 기존 데이터 초기화 (이전 데이터가 남아 섞이는 현상 방지)
		// 만약 StructMainData에 전체 초기화 함수가 없다면 구현하여 호출해야 합니다.
		//StructMainData::GetInstance().ClearLogicVariables();

		ULONGLONG fileSize = file.GetLength();
		const size_t LOGIC_VARIABLESize = sizeof(LOGIC_VARIABLE);

		// 파일 내용을 블록 단위로 읽기
		while (file.GetPosition() + LOGIC_VARIABLESize <= fileSize)
		{
			// shared_ptr로 동적 할당
			auto pLogicVar = std::make_shared<LOGIC_VARIABLE>();

			// 2. 원본과 동일하게 메모리 전체를 0으로 초기화 (패딩 바이트 문제 방지)
			memset(pLogicVar.get(), 0, LOGIC_VARIABLESize);

			// 실제 메모리 주소(pLogicVar.get())에 직접 읽기
			if (file.Read(pLogicVar.get(), LOGIC_VARIABLESize) == LOGIC_VARIABLESize)
			{
				// StructMainData로 즉시 추가
				StructMainData::GetInstance().AddLogicVariable(pLogicVar);
			}
			else
			{
				file.Close();
				return FALSE; // 3. break 대신 즉시 FALSE 반환하도록 수정
			}
		}

		file.Close();
		return TRUE;
	}
	else
	{
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		BCGPMessageBox(szError);
		return FALSE;
	}
}

// CDataViewExDoc diagnostics

#ifdef _DEBUG
void CDataViewExDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDataViewExDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDataViewExDoc commands
