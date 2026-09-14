#include "stdafx.h"
#include "DataLoader.h"
#include "DataComparison.h"

// ==========================================
// DB 파일 로딩
// ==========================================
bool DataLoader::LoadDatabase(const CString& filePath, bool IsOriginal)
{
	CFile file;
	CFileException ex;

	if (!file.Open(filePath, CFile::modeRead | CFile::typeBinary, &ex))
	{
		TRACE1("Failed to open database file: %s\n", (LPCTSTR)filePath);
		return false;
	}

	try
	{
		std::unique_ptr<DBstruct> dbStruct(new DBstruct());

		// ReadMainBinData처럼 정확한 크기 검증 수행
		if (file.Read(dbStruct.get(), sizeof(DBstruct)) == sizeof(DBstruct))
		{
			file.Close();

			if (IsOriginal) {
				// [핵심] 외부로 전달할 outDbStruct에 확실하게 이동 대입
				DataComparison::GetInstance().SetOriginal(std::move(dbStruct));
				return true;
			}
			else if (!IsOriginal)
			{
				DataComparison::GetInstance().SetDiff(std::move(dbStruct));
				return true;
			}
		}
		else
		{
			TRACE0("Database file size mismatch.\n");
			file.Close();
		}
	}
	catch (CFileException* pEx)
	{
		TRACE1("Exception occurred while reading database: %d\n", pEx->m_cause);
		pEx->Delete();
		file.Close();
	}

	return false;
}
// ==========================================
// IO Card 파일 로딩
// ==========================================

bool DataLoader::LoadIOCard(const CString& filePath, bool isOriginal)
{
	CFile file;
	CFileException ex;

	if (!OpenFileForReading(filePath, file))
	{
		TRACE1("Failed to open IOCard file: %s\n", (LPCTSTR)filePath);
		return false;
	}

	try
	{
		auto cardInfo = std::make_unique<CARD_INFO>();

		DWORD dwBytesRead = file.Read(cardInfo.get(), sizeof(CARD_INFO));
		if (isOriginal) {
			DataComparison::GetInstance().SetOriginalCardInfo(std::move(cardInfo));
		}
		if (!isOriginal) {
			DataComparison::GetInstance().SetDiffCardInfo(std::move(cardInfo));
		}
		file.Close();

		if (dwBytesRead != sizeof(CARD_INFO))
		{
			TRACE2("IOCard file size mismatch. Expected: %u, Got: %u\n", sizeof(CARD_INFO), dwBytesRead);
			return false;
		}

		//outCardInfo = cardInfo;
		return true;
	}
	catch (CFileException* pEx)
	{
		TRACE1("Exception occurred while reading IOCard: %d\n", pEx->m_cause);
		pEx->Delete();
		file.Close();
		return false;
	}
}

// ==========================================
// Logic Variable 파일 로딩
// ==========================================

bool DataLoader::LoadLogicVariable(const CString& filePath, std::vector<std::shared_ptr<LOGIC_VARIABLE>>& outLogicList)
{
	CFile file;
	CFileException ex;

	if (!OpenFileForReading(filePath, file))
	{
		TRACE1("Failed to open LogicVariable file: %s\n", (LPCTSTR)filePath);
		return false;
	}

	try
	{
		outLogicList.clear();

		// 파일 전체 크기
		ULONGLONG dwFileSize = file.GetLength();

		// LOGIC_VARIABLE 구조체 크기로 나누어 개수 계산
		int nLogicVarCount = static_cast<int>(dwFileSize / sizeof(LOGIC_VARIABLE));

		for (int i = 0; i < nLogicVarCount; ++i)
		{
			auto logicVar = std::make_shared<LOGIC_VARIABLE>();
			DWORD dwBytesRead = file.Read(logicVar.get(), sizeof(LOGIC_VARIABLE));

			if (dwBytesRead != sizeof(LOGIC_VARIABLE))
			{
				//TRACE2("LogicVariable read mismatch at index %d. Expected: %u bytes, Got: %u bytes\n",
				//	i, sizeof(LOGIC_VARIABLE), dwBytesRead);
				file.Close();
				return false;
			}

			outLogicList.push_back(logicVar);
		}

		file.Close();
		return true;
	}
	catch (CFileException* pEx)
	{
		//TRACE1("Exception occurred while reading LogicVariable: %d\n", pEx->m_cause);
		pEx->Delete();
		file.Close();
		return false;
	}
}

// ==========================================
// 통합 로딩 함수
// ==========================================

CString DataLoader::ExtractStationName(const CString& fileName, CString& outErrorMsg)
{
	int nLastBackslash = fileName.ReverseFind(_T('\\'));
	CString strStationName = fileName.Mid(nLastBackslash + 1);
	int nUnderscore = strStationName.Find(_T("_"));
	if (nUnderscore != -1)
	{
		strStationName = strStationName.Left(nUnderscore);
	}
	else {
		outErrorMsg = _T("파일 명칭 형식 오류 발생");
		BCGPMessageBox(outErrorMsg);

	}
	return strStationName;
}

bool DataLoader::LoadAllOriginalDataFromFolder(CString& folderPath, CString& outErrorMsg)
{
	outErrorMsg.Empty();

	try
	{
		// 1. 기존 데이터 초기화	
		DataComparison::GetInstance().ClearOriginal();

		CString basePath = folderPath;

		// 경로 유효성 검사 (역슬래시 존재 여부 확인)
		int nLastBackslash = basePath.ReverseFind(_T('\\'));
		if (nLastBackslash == -1)
		{
			outErrorMsg = _T("잘못된 파일 경로입니다.");
			return false;
		}

		// 역이름 추출
		CString stationName = ExtractStationName(basePath, outErrorMsg);

		// [수정] 매개변수 folderPath와의 섀도잉 방지를 위해 currentFolderPath로 변경
		CString currentFolderPath = basePath.Left(nLastBackslash);
		CString fileName = basePath.Mid(nLastBackslash + 1);

		int nParentBackslash = currentFolderPath.ReverseFind(_T('\\'));
		CString parentFolderPath = (nParentBackslash != -1) ? currentFolderPath.Left(nParentBackslash) : currentFolderPath;

		CString dataLogicFolderPath = parentFolderPath + _T("\\data_logic");

		// 2. 개별 파일 경로 조합
		CString dbFilePath = basePath;
		CString ioCardPath = currentFolderPath + _T("\\") + stationName + _T("_ioCard.bin");
		CString logicPath = currentFolderPath + _T("\\") + stationName + _T("_LogicVariable.Dat");
		CString LDATPath = dataLogicFolderPath;

		// 3. 데이터를 담을 객체 준비

		std::unique_ptr<CARD_INFO> cardInfo;
		std::vector<std::shared_ptr<LOGIC_VARIABLE>> logicList;
		bool bHasAnySuccess = false;

		// --- A. DB 파일 로드 ---
		if (LoadDatabase(dbFilePath, true)) {

			bHasAnySuccess = true;
		}
		else {
			outErrorMsg += _T("- 연동 데이터 파일 (DB) 읽기 실패\n");
		}

		// --- B. IO Card 로드 ---
		if (LoadIOCard(ioCardPath, true)) {
			//DataComparison::GetInstance().SetOriginalCardInfo(cardInfo);
			bHasAnySuccess = true;
		}
		else {
			outErrorMsg += _T("- IO Card 파일 읽기 실패\n");
		}

		// --- C. Logic & LDAT 로드 ---
		bool bLogicSuccess = LoadLogicVariable(logicPath, logicList);
		bool bLDATSuccess = LogicLDATFileLoad(LDATPath, true);

		if (bLogicSuccess && !logicList.empty()) {
			DataComparison::GetInstance().SetOriginalLogicVariable(logicList);
			bHasAnySuccess = true;
		}
		else {
			outErrorMsg += _T("- Logic Variable 파일 읽기 실패\n");
		}

		if (bLDATSuccess) {
			bHasAnySuccess = true;
		}
		else {
			outErrorMsg += _T("- LDAT 파일 읽기 실패\n");
		}

		return bHasAnySuccess;
	}
	catch (...)
	{
		outErrorMsg += _T("\n[치명적 오류] 원본 파일 읽기 중 예외가 발생했습니다.");
		return false;
	}
}

bool DataLoader::LoadAllDifflDataFromFolder(CString& filePath, CString& outErrorMsg)
{
	outErrorMsg.Empty(); // 에러 메시지 초기화

	try
	{
		DataComparison::GetInstance().ClearDiff();


		int nLastBackslash = filePath.ReverseFind(_T('\\'));
		if (nLastBackslash == -1)
		{
			outErrorMsg = _T("잘못된 파일 경로입니다.");
			return false;
		}

		CString stationName = ExtractStationName(filePath, outErrorMsg);

		CString currentFolderPath = filePath.Left(nLastBackslash);
		CString fileName = filePath.Mid(nLastBackslash + 1);

		int nParentBackslash = currentFolderPath.ReverseFind(_T('\\'));
		CString parentFolderPath = (nParentBackslash != -1) ? currentFolderPath.Left(nParentBackslash) : currentFolderPath;

		CString dataLogicFolderPath = parentFolderPath + _T("\\data_logic");

		// 경로 조합
		CString dbFilePath = filePath;
		CString ioCardPath = currentFolderPath + _T("\\") + stationName + _T("_ioCard.bin");
		CString logicPath = currentFolderPath + _T("\\") + stationName + _T("_LogicVariable.Dat");


		std::shared_ptr<CARD_INFO> cardInfo;
		std::vector<std::shared_ptr<LOGIC_VARIABLE>> logicList;

		bool bHasAnySuccess = false;

		// --- A. DB 파일 로드 ---
		if (LoadDatabase(dbFilePath, false)) {

			bHasAnySuccess = true;
		}
		else {
			outErrorMsg += _T("- 비교 연동 데이터 파일 (DB) 읽기 실패\n");
		}

		// --- B. IO Card 로드 ---
		if (LoadIOCard(ioCardPath, false)) {
			//DataComparison::GetInstance().SetDiffCardInfo(cardInfo);
			bHasAnySuccess = true;
		}
		else {
			outErrorMsg += _T("- 비교 IO Card 파일 읽기 실패\n");
		}

		// --- C. Logic & LDAT 로드 ---
		bool bLogicSuccess = LoadLogicVariable(logicPath, logicList);
		bool bLDATSuccess = LogicLDATFileLoad(dataLogicFolderPath, false);

		if (bLogicSuccess && !logicList.empty()) {
			DataComparison::GetInstance().SetDiffLogicVariable(logicList);
			bHasAnySuccess = true;
		}
		else {
			// [수정] 중복 누적되던 에러 메시지 정리
			outErrorMsg += _T("- 비교 Logic Variable 파일 읽기 실패\n");
		}

		if (bLDATSuccess) {
			bHasAnySuccess = true;
		}
		else {
			outErrorMsg += _T("- 비교 LDAT 파일 읽기 실패\n");
		}

		return bHasAnySuccess;
	}
	catch (...)
	{
		outErrorMsg += _T("\n비교 파일 읽기 중 예외가 발생했습니다.");
		return false;
	}
}


bool DataLoader::LogicLDATFileLoad(const CString& filePath, bool isOriginal)
{
	// filePath는 data_logic 폴더 경로
	// 끝에 역슬래시(\)가 없으면 붙여준다
	CString basePath = filePath;
	if (basePath.Right(1) != _T("\\"))
	{
		basePath += _T("\\");
	}

	//  검색 경로 구성 
	CString strSearchPath = basePath + _T("*.*");

	CFileFind fileFinder;
	BOOL bWorking = fileFinder.FindFile(strSearchPath);

	// 경로를 찾지 못했거나 파일이 아예 없는 경우
	if (!bWorking)
	{
		return false;
	}

	bool bFileFound = false; // 조건에 맞는 파일을 하나라도 찾았는지 확인하는 플래그

	//  파일 순회
	while (bWorking)
	{
		bWorking = fileFinder.FindNextFile();

		// 디렉토리는 무시
		if (fileFinder.IsDirectory()) continue;

		CString fileName = fileFinder.GetFileName();

		// 파일의 전체 경로
		CString strFullPath = fileFinder.GetFilePath();

		// 파일명에 "LDAT"가 포함
		// CompareNoCase -> 대소문자 구별 x 
		if (fileName.GetLength() >= 5 && fileName.Right(5).CompareNoCase(_T(".LDAT")) == 0)
		{
			// titleName 대신 '전체 경로(strFullPath)'를 싱글톤 벡터에 저장합니다.
			if (isOriginal)
			{
				DataComparison::GetInstance().AddOriginalLogicVariable(strFullPath);
			}
			else if (!isOriginal)
			{
				DataComparison::GetInstance().AddDiffLogicVariable(strFullPath);
			}

			bFileFound = true; // 유효한 파일을 찾았음을 체크
		}
	}

	// 파일 핸들 닫기
	fileFinder.Close();

	// 조건에 맞는 파일을 1개라도 찾아서 저장했다면 true, 아니면 false 반환
	return bFileFound;
}

//std::shared_ptr<DBstruct> DataLoader::LoadDataFromFile(const CString& filePath)
//{
//	std::shared_ptr<DBstruct> result;
//
//	if (LoadDatabase(filePath, result))
//	{
//		return result;
//	}
//
//	return nullptr;
//}

// ==========================================
// 헬퍼 함수
// ==========================================

bool DataLoader::FileExists(const CString& filePath)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile(filePath, &wfd);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	FindClose(hFind);
	return true;
}

bool DataLoader::OpenFileForReading(const CString& filePath, CFile& outFile)
{
	CFileException ex;

	if (!outFile.Open(filePath, CFile::modeRead | CFile::typeBinary, &ex))
	{
		TRACE1("Failed to open file: %s\n", (LPCTSTR)filePath);
		return false;
	}

	return true;
}
