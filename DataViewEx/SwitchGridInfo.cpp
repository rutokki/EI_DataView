#include "stdafx.h"
#include "SwitchGridInfo.h"
#include "GridColumnDefine.h"
#include "StructMainData.h"
#include"CommonUtils.h"


using namespace CommonUtil;

namespace
{
	// [수정] "A호"/"B호" 칼럼용 판정 결과. 첨담/크로싱(NoseKind) 구분은 표시하지 않기로 하고
	// (사용자 확인: 첨담 제거), 이 스위치가 카드상 A호/B호 중 어느 쪽인지(SWH_CARDTABLE::NoseAB)와
	// 그 포트의 명칭(사용자 확인: 명칭 추가)만 판정한다. SwitchInfoType(선로전환기 자체)에는
	// A호/B호 구분이 없고 카드 쪽에만 있으므로, 스위치의 Table Index(nIdx)와 일치하는 카드 포트를
	// 찾아서 판정해야 한다.
	struct SwitchNoseSide
	{
		bool bIsA = false;  // A호
		bool bIsB = false;  // B호
		CString strNameA;   // A호 포트 명칭 (SWH_CARDTABLE::_CardData_t.Name)
		CString strNameB;   // B호 포트 명칭
	};

	SwitchNoseSide GetSwitchNoseSide(Byte_t nIdx)
	{
		SwitchNoseSide side;

		auto SwitchCardInfo = StructMainData::GetInstance().GetSwitchCardInfo();

		for (auto& cardItem : SwitchCardInfo)
		{
			if (cardItem.RackNo == 0) continue;

			for (UINT nPort = 1; nPort < _countof(cardItem.CardData); nPort++)
			{
				auto& portItem = cardItem.CardData[nPort];
				if (portItem.PortNo == 0) continue;

				// 이 포트가 조회 중인 스위치(nIdx)를 가리키는 포트가 아니면 건너뜀
				if (portItem.Idx != nIdx) continue;

				if (cardItem.NoseAB == 'A') { side.bIsA = true; side.strNameA = GetName(portItem); }
				if (cardItem.NoseAB == 'B') { side.bIsB = true; side.strNameB = GetName(portItem); }
			}
		}

		return side;
	}

	// [수정] "노스가동"(6번) 칼럼은 다른 종류 칼럼처럼 O 색표시만 하기로 하고(사용자 확인:
	// 노스가동 색표시만), A호가 NS-AM인지 MJ81인지 등 세부 구성은 12/14번(A호구분/B호구분) 칼럼에서
	// 각각 따로 표시하도록 뺐다 (사용자 확인: A호 B호를 따로 표시). GetSwitchIsNose()의
	// 반환값(GetNoseType)을 이 스위치의 A호/B호 각각에 대한 역할 텍스트로 변환한다.
	CString GetNoseSideRoleText(int nNoseType, bool bSideIsA)
	{
		switch (nNoseType)
		{
		case CommonUtil::Nose:        return _T("노스가동"); // 단동 - A호/B호 구분 없이 한쪽만 존재
		case CommonUtil::ANSAMBMJ81:  return bSideIsA ? _T("NS-AM") : _T("MJ81");
		case CommonUtil::AMJ81BNSAM:  return bSideIsA ? _T("MJ81") : _T("NS-AM");
		case CommonUtil::ABMJ81:      return _T("MJ81");
		default:                      return _T("");
		}
	}
}
BEGIN_MESSAGE_MAP(SwitchGridInfo, CustomBCGGridCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

CRect SwitchGridInfo::OnGetHeaderRect(CDC* pDC, const CRect& rectDraw)
{
	CRect rect = CBCGPGridCtrl::OnGetHeaderRect(pDC, rectDraw);
	// 2단 헤더를 위해 높이 확장
	rect.bottom = rect.top + (rect.Height() * m_Column.GetHeaderLineCount());
	return rect;
}

void SwitchGridInfo::OnDrawHeader(CDC* pDC)
{
	m_Column.PrepareDrawHeader();
	CBCGPGridCtrl::OnDrawHeader(pDC);
}

void SwitchGridInfo::OnPrintHeader(CDC* pDC, CPrintInfo* pInfo)
{
	m_Column.PreparePrintHeader();
	CBCGPGridCtrl::OnPrintHeader(pDC, pInfo);
}

void SwitchGridInfo::OnPosSizeChanged()
{
	CBCGPGridCtrl::OnPosSizeChanged();
	m_Column.ReposHeaderItems();
}

void SwitchGridInfo::LoadAllSwitchData()
{
	auto SwitchList = StructMainData::GetInstance().GetSwitchInfo();
	// [수정] GetSwitchIsNose()/카드 교차조회는 이 스위치의 Table Index(배열 내 위치)가 필요하므로,
	// range-for 대신 인덱스를 함께 갖는 루프로 변경.
	for (size_t nIdx = 0; nIdx < SwitchList.size(); nIdx++)
	{
		auto& item = SwitchList[nIdx];
		if (item.Name[0] == 0 || item.Name[0] == 0xff) {
			continue;
		}
		CBCGPGridRow* pRow = CreateRow(GetColumnCount());
		pRow->GetItem(0)->SetValue((LPCTSTR)CommonUtil::GetName(item));// 선로전환기 이름
		SetSwitchType(pRow, item, (Byte_t)nIdx); // 선로전환기 타입/방향/A호B호 정의 1~ 14번 칼럼
		// [수정] 사용자 확인(명칭 따로/A호 B호 구분 따로)으로 A호/B호가 4개 칼럼(11~14번)으로
		// 재구성되면서 A점~D점이 15~18번으로 밀림 -> 인덱스 수정.
		pRow->GetItem(15)->SetValue((LPCTSTR)GetDBNameByNumber(item.SwhTrack[0], GetDBNameByNum::TrackIdx)); // A점
		pRow->GetItem(16)->SetValue((LPCTSTR)GetDBNameByNumber(item.SwhTrack[1], GetDBNameByNum::TrackIdx)); // B점
		pRow->GetItem(17)->SetValue((LPCTSTR)GetDBNameByNumber(item.SwhTrack[2], GetDBNameByNum::TrackIdx)); // C점
		pRow->GetItem(18)->SetValue((LPCTSTR)GetDBNameByNumber(item.SwhTrack[3], GetDBNameByNum::TrackIdx)); // D점

		// [정리] 로컬 출력(OUT WLR/PDO WR-N/PDO WR-R, Kind.LocalOut) 관련은 매핑 대상에서
		// 제외하기로 확정함 (사용자 확인: 로컬 출력 제거). item.OutWLR 등은 애초에
		// 현재 EI_IP_DBStruct_Typedef.h의 SwitchInfoType에 없는 필드이기도 함.
		// 첨담(A)/크로싱(A)/첨담(B)/크로싱(B) : SetSwitchType()에서 11~14번(A호/A호구분/B호/B호구분)
		// 칼럼으로 대체함 (GridColumnDefine.h 참고)
		// [제거] 쇄정설비(SwitchLockEquip)/자동정위전환(AutoSwitch) 표시 칼럼은 사용자 확인
		// (17,18번 칼럼제거)에 따라 제거함. 필요해지면 item.SwitchLockEquip / item.AutoSwitch에서
		// 다시 꺼내 쓸 수 있음 (이전 버전 참고).

		AddRow(pRow, FALSE);

	}

}
void SwitchGridInfo::SetSwitchType(CBCGPGridRow* pRow, SwitchInfoType& switchItem, Byte_t nIdx)
{
	// [수정] 이 배열은 1~6번(단동/쌍동/삼동/사동/시서스/노스가동) "종류" O/X 칼럼만 채움.
	// SpcSwitch/DirKind는 원래 여기 섞여 있었으나:
	//  - DirKind(정위/반위 방향, 2비트 인코딩)는 "종류" 비트 플래그가 아니고 이미 바로 아래에서
	//    9~10번(정위/반위) 칼럼으로 별도 처리되고 있어 제외.
	//  - SpcSwitch(bit0=타역 선로전환기)는 실제로는 8번 "타역" 칼럼용인데, 배열 순서(i+1) 때문에
	//    7번 "표시" 칼럼에 잘못 표시되고 있었음 -> 아래에서 8번 칼럼에 직접 표시하도록 분리.
	// (7번 "표시" 칼럼은 대응하는 실제 필드가 없어 비워둠. 로컬 출력(Kind.LocalOut) 관련은
	// 매핑하지 않기로 확정함 - 사용자 확인: 로컬 출력 제거)
	SCBitCheckInfo switchList[] = {
		{&switchItem.Kind.Single, SwitchInfoT::SINGLE},
		{&switchItem.Kind.Double, SwitchInfoT::DOUBLE},
		{&switchItem.Kind.Triple, SwitchInfoT::TRIPLE},
		{&switchItem.Kind.FourTime, SwitchInfoT::FOURTIME},
		{&switchItem.Kind.Scissors, SwitchInfoT::SCISSORS},
		{&switchItem.Kind.Nose, SwitchInfoT::NOSE},
	};
	if (switchItem.Kind.DirKind & SwitchInfoT::DIRKINDBIT2)
	{
		pRow->GetItem(9)->SetValue(_T("R방향"));
		pRow->GetItem(10)->SetValue(_T("L방향"));
	}
	else if (switchItem.Kind.DirKind & SwitchInfoT::DIRKINDBIT1) {
		pRow->GetItem(9)->SetValue(_T("L방향"));
		pRow->GetItem(10)->SetValue(_T("B방향"));
	}
	else {
		pRow->GetItem(9)->SetValue(_T("-"));
		pRow->GetItem(10)->SetValue(_T("-"));
	}
	for (int i = 0; i < MAX_SWITCH_TYPE; i++) {
		if (*switchList[i].pValue & switchList[i].bit) {

			CBCGPGridItem* pItem = pRow->GetItem(i + 1);
			if (pItem != nullptr) {
				pItem->SetValue(_T("O"));
				pItem->SetTextColor((RGB(80, 205, 80)));
				pItem->SetBackgroundColor((RGB(80, 205, 80)));
			}
		}
	}

	// [수정] SpcSwitch(bit0=타역 선로전환기)를 8번 "타역" 칼럼에 직접 표시 (위 switchList 주석 참고)
	if (switchItem.Kind.SpcSwitch & SwitchInfoT::SPCSWITCH)
	{
		CBCGPGridItem* pItem = pRow->GetItem(8);
		if (pItem != nullptr) {
			pItem->SetValue(_T("O"));
			pItem->SetTextColor((RGB(80, 205, 80)));
			pItem->SetBackgroundColor((RGB(80, 205, 80)));
		}
	}

	// [수정] "노스가동"(6번) 칼럼은 다른 종류 칼럼과 동일하게 O 색표시만 한다 (사용자 확인:
	// 노스가동 색표시만). A호가 NS-AM인지 MJ81인지 등 세부 구성은 아래에서 12/14번(A호구분/B호구분)
	// 칼럼에 각각 따로 표시한다.
	int nNoseType = GetSwitchIsNose(nIdx);
	CBCGPGridItem* pNoseItem = pRow->GetItem(6);
	if (pNoseItem != nullptr)
	{
		if (nNoseType != 0)
		{
			pNoseItem->SetValue(_T("O"));
			pNoseItem->SetTextColor(RGB(80, 205, 80));
			pNoseItem->SetBackgroundColor(RGB(80, 205, 80));
		}
		else
		{
			// DB 기준으로는 노스가동이 아닌데 카드 쪽 원시 비트가 켜져 있던 경우까지 포함해
			// 항상 판정 결과를 우선시하도록 명시적으로 비움
			pNoseItem->SetValue(_T(""));
		}
	}

	// [수정] 사용자 확인(명칭 따로 / A호 B호 구분 따로)에 따라 "A호"/"B호"에 합쳐 쓰던 "명칭(역할)"
	// 텍스트를 4개 칼럼으로 분리: 11번 A호(명칭만) / 12번 A호구분(NS-AM/MJ81만) /
	// 13번 B호(명칭만) / 14번 B호구분. 노스가동인 경우에만 SWH_CARDTABLE을 교차 조회해서 채운다.
	if (nNoseType != 0)
	{
		SwitchNoseSide side = GetSwitchNoseSide(nIdx);

		auto SetCell = [&](int nCol, const CString& strText)
		{
			if (strText.IsEmpty()) return;
			CBCGPGridItem* pItem = pRow->GetItem(nCol);
			if (pItem == nullptr) return;
			pItem->SetValue((LPCTSTR)strText);
			pItem->SetTextColor(RGB(80, 205, 80));
			pItem->SetBackgroundColor(RGB(80, 205, 80));
		};
		if (side.bIsA)
		{
			SetCell(11, side.strNameA.IsEmpty() ? (CString)_T("O") : side.strNameA);   // A호 (명칭)
			SetCell(12, GetNoseSideRoleText(nNoseType, true));                          // A호구분
		}
		if (side.bIsB)
		{
			SetCell(13, side.strNameB.IsEmpty() ? (CString)_T("O") : side.strNameB);   // B호 (명칭)
			SetCell(14, GetNoseSideRoleText(nNoseType, false));                         // B호구분
		}
	}
}
void SwitchGridInfo::UpdateSwitchData()
{
	RemoveAll();
	LoadAllSwitchData();
	AdjustLayout();
}

SwitchGridInfo::SwitchGridInfo()
{
	m_Column.SetOwnerList(this);

	CustomBCGGridCtrl::InitGridControl();
	EnableRowHeader(TRUE);
	EnableLineNumbers(TRUE);
}

int SwitchGridInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Column.SetHeaderLineCount(2);

	std::vector<ColumnInfo> switchColumn = GridColumnDefine::GetSwitchColumnInfo();
	CArray<int, int> arrSwitchTypeCols;
	CArray<int, int> arrSwitchDirCols;
	for (int i = 1; i <= 8; i++) arrSwitchTypeCols.Add(i);
	for (int j = 9; j <= 10; j++) arrSwitchDirCols.Add(j);
	CArray<int, int> arrDetailSignalTypeLines;
	CArray<int, int> arrDetailSignalDirLines;
	arrDetailSignalTypeLines.Add(0); // 상단 그룹 헤더는 0번 라인
	arrDetailSignalDirLines.Add(0);
	// 그룹 추가: 궤도 종류 상세정보
	m_Column.AddHeaderItem(&arrSwitchTypeCols, &arrDetailSignalTypeLines, 2, _T("선로전환기 종류"), HDF_CENTER, -1);
	m_Column.AddHeaderItem(&arrSwitchDirCols, &arrDetailSignalDirLines, -1, _T("방향"), HDF_CENTER, -1);

	// 1. 컬럼 추가
	for (int i = 0; i < (int)switchColumn.size(); i++) {
		InsertColumn(i, switchColumn[i].columnName, switchColumn[i].columnWidth);

		SetHeaderAlign(i, HDF_CENTER);
		SetColumnAlign(i, HDF_CENTER);
	}
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	return 0;
}

void SwitchGridInfo::OnSize(UINT nType, int cx, int cy)
{
	CBCGPGridCtrl::OnSize(nType, cx, cy);
	m_Column.ReposHeaderItems(); // 다시 계산된 정보를 바탕으로 위치 재배치

	// 5. 마지막으로 전체 그리드 재조정
	AdjustLayout();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}