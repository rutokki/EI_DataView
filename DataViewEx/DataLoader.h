#pragma once
#include "stdafx.h"

// ==========================================
// DataLoader: 파일 로딩 통합 클래스
// ==========================================
// DB, IOCard, Logic 파일을 읽어 데이터 구조로 변환
class DataLoader
{
public:
	// ==========================================
	// DB 파일 로딩
	// ==========================================


	static bool LoadDatabase(const CString& filePath, bool isOriginal);

	// ==========================================
	// IO Card 파일 로딩
	// ==========================================


	static bool LoadIOCard(const CString& filePath, bool isOriginal);

	// ==========================================
	// Logic Variable 파일 로딩
	// ==========================================


	static bool LoadLogicVariable(const CString& filePath, std::vector<std::shared_ptr<LOGIC_VARIABLE>>& outLogicList);

	// ==========================================
	// 통합 로딩 함수
	// ==========================================

	static CString ExtractStationName(const CString& fileName, CString& outErrorMsg);
	static bool LoadAllOriginalDataFromFolder(CString& folderPath, CString& outErrorMsg);
	static bool LoadAllDifflDataFromFolder(CString& folderPath, CString& outErrorMsg);
	static bool LogicLDATFileLoad(const CString& filePath, bool isOriginal);

	//static std::shared_ptr<DBstruct> LoadDataFromFile(const CString& filePath);

private:
	// 생성자/소멸자 비활성화 (static 함수만 사용)
	DataLoader() = delete;
	~DataLoader() = delete;

	// ==========================================
	// 헬퍼 함수 (파일 검증)
	// ==========================================

	static bool FileExists(const CString& filePath);


	/// 파일 열기 시도
	static bool OpenFileForReading(const CString& filePath, CFile& outFile);
};
