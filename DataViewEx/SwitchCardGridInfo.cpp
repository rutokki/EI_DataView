#include "stdafx.h"
#include "SwitchCardGridInfo.h"
#include "StructMainData.h"
#include "CommonUtils.h"
using namespace CommonUtil;

namespace
{
	// SWH_CARDTABLE.NoseAB: MJ81(더블선로전환기) 주 선로전환기의 A호/B호 구분
	// ('A'/'B', 단동인 경우 0/미사용) - EI_IP_IOCard_Typedef.h 주석 참고
	CString GetNoseABText(Byte_t noseAB)
	{
		switch (noseAB)
		{
		case 'A': return _T("A호");
		case 'B': return _T("B호");
		default:  return _T("");
		}
	}

	// [추가] NoseAB("A호"/"B호")만으로는 이 카드가 NS-AM인지 MJ81(노스가동)인지 알 수 없어서
	// (그건 카드의 CardKind일 뿐, "어느 호가 NS-AM인가"는 스위치 자체의 설정이라 별개임),
	// CommonUtil::GetSwitchIsNose()로 판정한 EI_DBStruct 기준 A/B 역할(nNoseType)과 결합해
	// "A호(NS-AM)" / "B호(MJ81/노스가동)" 처럼 역할까지 함께 표시한다.
	CString GetNoseABRoleText(Byte_t noseAB, int nNoseType)
	{
		CString strAB = GetNoseABText(noseAB);
		if (strAB.IsEmpty()) return strAB;

		bool bIsA = (noseAB == 'A');
		bool bIsB = (noseAB == 'B');

		CString strRole;
		switch (nNoseType)
		{
		case CommonUtil::ANSAMBMJ81: // A호=NS-AM, B호=MJ81(노스가동)
			if (bIsA) strRole = _T("NS-AM");
			else if (bIsB) strRole = _T("MJ81/노스가동");
			break;
		case CommonUtil::AMJ81BNSAM: // A호=MJ81(노스가동), B호=NS-AM
			if (bIsA) strRole = _T("MJ81/노스가동");
			else if (bIsB) strRole = _T("NS-AM");
			break;
		case CommonUtil::ABMJ81: // A호/B호 모두 MJ81(노스가동)
			strRole = _T("MJ81/노스가동");
			break;
		default:
			break; // 단동(Nose) 등은 A/B 역할 구분이 없음
		}

		if (strRole.IsEmpty()) return strAB;
		return strAB + _T("(") + strRole + _T(")");
	}

	// SWH_CARDTABLE::_CardData_t.NoseKind: MJ81(더블선로전환기) 촉단 구분
	// ('p' : 첨담, 'f' : 크로싱, 단동인 경우 미사용) - EI_IP_IOCard_Typedef.h 주석 참고
	CString GetNoseKindText(Byte_t noseKind)
	{
		switch (noseKind)
		{
		case 'p': return _T("첨담");
		case 'f': return _T("크로싱");
		default:  return _T("");
		}
	}
}

SwitchCardGridInfo::SwitchCardGridInfo()
{
	EnableRowHeader(TRUE);
	EnableLineNumbers(TRUE);
	CustomBCGGridCtrl::InitGridControl();
}
BEGIN_MESSAGE_MAP(SwitchCardGridInfo, CustomBCGGridCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void SwitchCardGridInfo::LoadAllSwitchCardData()
{

	auto SwitchCardInfo = StructMainData::GetInstance().GetSwitchCardInfo();

	if (SwitchCardInfo.size() == 0) return;

	auto stationInfo = StructMainData::GetInstance().GetStationInfo();

	UINT nCardNo = 0;          // 논리적 카드 번호 (등록된 카드 순서)
	UINT nRow = 0;              // 그리드 행 번호
	CString strCardKind = _T("");
	// 전체 OutCard(카드 단위) 순회
	for (auto& cardItem : SwitchCardInfo) {
		// 카드 없음 체크
		if (cardItem.RackNo == 0) continue;

		nCardNo++;

		// 카드 번호 제한 (증가 직후 체크 - 넘으면 종료)
		if (nCardNo > stationInfo.NoOfModuleSwh) break;

		// 카드가 바뀔 때(첫 카드는 제외) 빈 줄 추가
		if (nRow != 0) {
			CBCGPGridRow* pEmptyRow = CreateRow(GetColumnCount());
			AddRow(pEmptyRow, FALSE);
			nRow++;
		}
		if (cardItem.CardKind == 5) {
			strCardKind = _T("NS-AM"); // 모듈 1개
		}
		else if (cardItem.CardKind == 6) {
			strCardKind = _T("MJ81"); // 모듈 2개
		}
		else {
			strCardKind = _T("");
		}
		// 카드 내 포트 순회 (Index : 1부터 사용, 0번째는 미사용)
		for (UINT nPort = 1; nPort < _countof(cardItem.CardData); nPort++) {
			auto& portItem = cardItem.CardData[nPort];

			// 미사용 포트는 건너뜀
			if (portItem.PortNo == 0) continue;

			// 그리드 행 증가
			nRow++;

			CBCGPGridRow* pRow = CreateRow(GetColumnCount());

			// [수정] NoseAB/NoseKind/Idx는 Byte_t(정수/문자코드)인데 (LPCTSTR)로 캐스팅해
			// 정수값을 문자열 포인터로 취급하고 있었음 -> 그리드가 그 "주소"를 문자열로
			// 읽으려다 접근 위반(크래시)이 나거나 쓰레기 값이 표시될 수 있는 상태였음.
			// NoseAB/NoseKind는 문자 코드를 사람이 읽을 수 있는 텍스트로 변환하고,
			// Idx는 숫자 그대로 SetValue 하도록 수정.
			//
			// [추가] 카드 원본(NoseAB/NoseKind)만으로는 이 선로전환기가 실제로 "노스가동"인지
			// 알 수 없으므로(노스가동이 아닌 카드에도 값이 남아있을 수 있음), Table Index(Idx)로
			// EI_DBStruct의 SwitchInfoType(Kind.Nose/Kind.Double)을 조회해 실제 노스가동 여부를
			// 판정한 뒤, 노스가동인 경우에만 A호/B호 및 노스 종류 텍스트를 표시한다.
			int nNoseType = GetSwitchIsNose(portItem.Idx);
			// [수정] 단순히 "A호"/"B호"만 보여주면 어느 쪽이 NS-AM이고 어느 쪽이 MJ81(노스가동)인지
			// 알 수 없어서, GetSwitchIsNose() 판정 결과(nNoseType)와 결합해 역할까지 함께 표시.
			CString strNoseAB = (nNoseType != 0) ? GetNoseABRoleText(cardItem.NoseAB, nNoseType) : (CString)_T("");
			CString strNoseKind = (nNoseType != 0) ? GetNoseKindText(portItem.NoseKind) : (CString)_T("");

			pRow->GetItem(0)->SetValue(cardItem.RackNo);              // 랙번호
			pRow->GetItem(1)->SetValue(cardItem.SlotNo);               // 슬롯번호
			pRow->GetItem(2)->SetValue((LONG)nCardNo);              // 카드번호
			pRow->GetItem(3)->SetValue(portItem.PortNo);                // 포트번호
			pRow->GetItem(4)->SetValue((LPCTSTR)strCardKind);           // 선로전환기 카드 종류
			pRow->GetItem(5)->SetValue((LPCTSTR)strNoseAB);             // 노스가동 (A호/B호) - DB 기준 노스가동인 경우만 표시
			pRow->GetItem(6)->SetValue((LPCTSTR)GetName(portItem));     // 이름
			pRow->GetItem(7)->SetValue((LPCTSTR)GetTagName(portItem));  // 표찰이름
			pRow->GetItem(8)->SetValue((LPCTSTR)strNoseKind);           // 노스 종류 (첨담/크로싱) - DB 기준 노스가동인 경우만 표시
			pRow->GetItem(9)->SetValue(portItem.Idx);                   // Table Index (숫자 그대로)

			AddRow(pRow, FALSE);
		}
	}
}

void SwitchCardGridInfo::UpdateSwitchCardData()
{
	RemoveAll();
	LoadAllSwitchCardData();
	AdjustLayout();
}

int SwitchCardGridInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	auto SwitchCardGrid = GridColumnDefine::GetSwitchCardColumnInfo();
	for (int i = 0; i < SwitchCardGrid.size(); i++) {
		InsertColumn(i, SwitchCardGrid[i].columnName, SwitchCardGrid[i].columnWidth);

		SetHeaderAlign(i, HDF_CENTER);
		SetColumnAlign(i, HDF_CENTER);
	}

	return 0;
}

void SwitchCardGridInfo::OnSize(UINT nType, int cx, int cy)
{
	CustomBCGGridCtrl::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
