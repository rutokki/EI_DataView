#include "stdafx.h"
#include "LogicVariableGridInfo.h"
#include "StructMainData.h"
#include"CommonUtils.h"

using namespace CommonUtil;
BEGIN_MESSAGE_MAP(LogicVariableGridInfo, CustomBCGGridCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()
LogicVariableGridInfo::LogicVariableGridInfo()
{
	CustomBCGGridCtrl::InitGridControl();
	EnableRowHeader(TRUE);
	EnableLineNumbers(TRUE);
	SetRowHeaderWidth(40, TRUE);
}
int LogicVariableGridInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	auto LogicVariableGrid = GridColumnDefine::GetLogicVariableColumnInfo();
	for (int i = 0; i < LogicVariableGrid.size(); i++) {
		InsertColumn(i, LogicVariableGrid[i].columnName, LogicVariableGrid[i].columnWidth);

		SetHeaderAlign(i, HDF_CENTER);
		SetColumnAlign(i, HDF_CENTER);
	}

	return 0;
}

void LogicVariableGridInfo::OnSize(UINT nType, int cx, int cy)
{
	CBCGPGridCtrl::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void LogicVariableGridInfo::LoadAllLogicData()
{
	auto logicInfo = StructMainData::GetInstance().GetLogicVariableVec();
	if (logicInfo.size() == 0) return;
	for (auto& item : logicInfo) {
		if (!item) continue;
		CBCGPGridRow* pRow = CreateRow(GetColumnCount());
		pRow->GetItem(0)->SetValue(item->nLogicIdx); // 로직IDX
		pRow->GetItem(1)->SetValue((LPCTSTR)GetszName(item)); // 명칭
		//연동구분 Kind
		pRow->GetItem(2)->SetValue((LPCTSTR)(GetKindName(item->Kind)));
		//연동 Idx 연동DB의 인덱스 TableIdx
		pRow->GetItem(3)->SetValue(item->TableIdx);
		//Card 타입	 CARDINFO.CardType

		pRow->GetItem(4)->SetValue((LPCTSTR)GetCardType(item->CARDINFO.CardType));
		//Rack No nRackNo
		pRow->GetItem(5)->SetValue(item->CARDINFO.nRackNo);
		pRow->GetItem(6)->SetValue(item->CARDINFO.nSlotNo);
		pRow->GetItem(7)->SetValue(item->CARDINFO.nPortNo);
		//Slot No nSlotNo

		//Port No nPortNo
		//로직 구분 LogicKind
		pRow->GetItem(8)->SetValue((LPCTSTR)GetKindName(item->LogicKind));
		//로직 종류 LogicType 
		pRow->GetItem(9)->SetValue(item->LogicType);
		//Timer TimerKind
		pRow->GetItem(10)->SetValue((LPCTSTR)GetKindLogicTime(item->TimerKind));
		//Time(ms) TimeValue
		pRow->GetItem(11)->SetValue(item->TimeValue);
		//DB_Time TimeValueDB
		pRow->GetItem(12)->SetValue(item->TimeValueDB);
		AddRow(pRow, FALSE);
	}
}
CString LogicVariableGridInfo::GetszName(std::shared_ptr<LOGIC_VARIABLE> item)
{
	if (!item) return _T("");

	// 1. 46바이트 크기의 안전한 임시 버퍼를 만들고 전체를 0으로 초기화 (마지막 바이트 무조건 \0 보장)
	char szTempName[47] = { 0, };

	// 2. 최대 46바이트까지만 안전하게 복사 (메모리 오버런 및 뒤쪽 데이터 침범 원천 차단)
	memcpy(szTempName, item->szVarName, 46);

	// 3. 멀티바이트 문자열을 유니코드 CString으로 안전하게 변환
	CString strName(szTempName);
	return strName;
}
void LogicVariableGridInfo::UpdateLogicData()
{
	RemoveAll();
	LoadAllLogicData();
	AdjustLayout();
}

