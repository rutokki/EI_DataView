#include "stdafx.h"
#include "TestRightView.h"
#include "StructMainData.h" // 싱글톤 데이터 헤더
#include "CommonUtils.h"
#include "BitUtility.h"
#include "DataViewExDoc.h"
#include "resource.h"
using namespace CommonUtil;
IMPLEMENT_DYNCREATE(TestRightView, CView)

TestRightView::TestRightView() {

}
TestRightView::~TestRightView() {
}

BEGIN_MESSAGE_MAP(TestRightView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_SAVE, &TestRightView::OnFileSave)
	ON_COMMAND(ID_FILE_PRINT, &TestRightView::OnFilePrint)
	ON_COMMAND(ID_DIFF_TOOL, &TestRightView::OnFileDiff)
	ON_COMMAND(ID_FILE_OPEN, &TestRightView::OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, &TestRightView::OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_DIFF_TOOL, &TestRightView::OnUpdateFileDiff)
	//ON_WM_SETFOCUS()
END_MESSAGE_MAP()

void TestRightView::OnDraw(CDC* pDC) {

	// TODO: add draw code for native data here
	COLORREF clrText = globalData.clrWindowText;
	if (!pDC->IsPrinting())
	{
		CRect rectClient;
		GetClientRect(rectClient);

		pDC->FillSolidRect(rectClient, CBCGPVisualManager::GetInstance()->GetControlFillColor());
		clrText = CBCGPVisualManager::GetInstance()->GetControlTextColor();
	}
}


int TestRightView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 0. 우측 상세 영역 그룹 박스
	m_grpBox.Create(_T(" 선택 진로 상세 쇄정 정보 "), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CRect(0, 0, 0, 0), this, 2000);
	m_grpBox.m_bVisualManagerStyle = TRUE;
	m_grpBox.SetFont(&FontSetting::FontDungGeunMoCombo);
	// 1. 상호쇄정
	m_lblInterlock.Create(_T("상호쇄정"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 2001);
	m_edtInterlock.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_MULTILINE | WS_VSCROLL, CRect(0, 0, 0, 0), this, 2002);

	m_lblInterlock.m_bVisualManagerStyle = TRUE;
	m_lblInterlock.SetFont(&FontSetting::FontDungGeunMoStatic);
	m_edtInterlock.SetReadOnly(TRUE);
	m_edtInterlock.EnableWindow(FALSE);
	//m_lblInterlock.m_clrBkgnd = RGB(255, 255, 255);
	// 2. 선로전환기쇄정
	m_lblSwitch.Create(_T("선로전환기쇄정"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 2003);
	m_edtSwitch.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_MULTILINE | WS_VSCROLL, CRect(0, 0, 0, 0), this, 2004);

	m_lblSwitch.m_bVisualManagerStyle = TRUE;
	m_lblSwitch.SetFont(&FontSetting::FontDungGeunMoStatic);
	m_edtSwitch.SetReadOnly(TRUE);
	m_edtSwitch.EnableWindow(FALSE);
	//m_lblSwitch.m_clrBkgnd = RGB(255, 255, 255);
	// 3. 신호제어조건
	m_lblSigCond.Create(_T("신호제어조건"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 2005);
	m_edtSigCond.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_MULTILINE | WS_VSCROLL, CRect(0, 0, 0, 0), this, 2006);

	m_lblSigCond.m_bVisualManagerStyle = TRUE;
	m_lblSigCond.SetFont(&FontSetting::FontDungGeunMoStatic);
	m_edtSigCond.SetReadOnly(TRUE);
	m_edtSigCond.EnableWindow(FALSE);

	//m_lblSigCond.m_clrBkgnd = RGB(255, 255, 255);
	// 4. 진로구분쇄정
	m_lblRouteDiv.Create(_T("진로구분쇄정"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 2007);
	m_edtRouteDiv.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_MULTILINE | WS_VSCROLL, CRect(0, 0, 0, 0), this, 2008);

	m_lblRouteDiv.m_bVisualManagerStyle = TRUE;
	m_lblRouteDiv.SetFont(&FontSetting::FontDungGeunMoStatic);
	m_edtRouteDiv.SetReadOnly(TRUE);
	m_edtRouteDiv.EnableWindow(FALSE);
	//m_lblRouteDiv.m_clrBkgnd = RGB(255, 255, 255);

	// 5. 접근쇄정
	m_lblAppLock.Create(_T("접근쇄정"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 2009);
	m_edtAppLock.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_MULTILINE | WS_VSCROLL, CRect(0, 0, 0, 0), this, 2010);

	m_lblAppLock.m_bVisualManagerStyle = TRUE;
	m_lblAppLock.SetFont(&FontSetting::FontDungGeunMoStatic);
	m_edtAppLock.SetReadOnly(TRUE);
	m_edtAppLock.EnableWindow(FALSE);
	//m_lblAppLock.m_clrBkgnd = RGB(255, 255, 255);
	return 0;
}

void TestRightView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (cx < 450 || cy < 300) return;

	// 창 크기(cx, cy)에 비례하여 전체 여백 및 그룹 박스 크기 설정
	int nMarginX = 15;
	int nMarginY = 15;
	int nGroupWidth = cx - (nMarginX * 2);
	int nGroupHeight = cy - (nMarginY * 2);

	if (m_grpBox.GetSafeHwnd())
		m_grpBox.MoveWindow(nMarginX, nMarginY, nGroupWidth, nGroupHeight);

	// 💡 라벨(이름) 영역의 가로 폭을 200으로 넓혀서 글자가 아래로 내려가지 않도록 설정
	int nLabelX = nMarginX + 25;
	int nLabelW = 200;      // 기존 140 -> 200으로 확장
	int nLabelEditGap = 20; // 이름과 박스 사이의 간격

	int nEditX = nLabelX + nLabelW + nLabelEditGap;
	int nEditW = nGroupWidth - (nEditX - nMarginX) - 25;

	// 5개 항목이 그룹 박스 내부 높이에 균등하게 배율 적용되도록 설정
	int nAvailableH = nGroupHeight - 45;
	int nItemH = (nAvailableH / 5) - 10;
	if (nItemH < 40) nItemH = 40;

	int nY = nMarginY + 30;
	int nGap = nItemH + 10;

	// 1. 상호쇄정
	m_lblInterlock.MoveWindow(nLabelX, nY + 6, nLabelW, 24);
	m_edtInterlock.MoveWindow(nEditX, nY, nEditW, nItemH);
	nY += nGap;

	// 2. 선로전환기쇄정
	m_lblSwitch.MoveWindow(nLabelX, nY + 6, nLabelW, 24);
	m_edtSwitch.MoveWindow(nEditX, nY, nEditW, nItemH);
	nY += nGap;

	// 3. 신호제어조건
	m_lblSigCond.MoveWindow(nLabelX, nY + 6, nLabelW, 24);
	m_edtSigCond.MoveWindow(nEditX, nY, nEditW, nItemH);
	nY += nGap;

	// 4. 진로구분쇄정
	m_lblRouteDiv.MoveWindow(nLabelX, nY + 6, nLabelW, 24);
	m_edtRouteDiv.MoveWindow(nEditX, nY, nEditW, nItemH);
	nY += nGap;

	// 5. 접근쇄정
	m_lblAppLock.MoveWindow(nLabelX, nY + 6, nLabelW, 24);
	m_edtAppLock.MoveWindow(nEditX, nY, nEditW, nItemH);
}

void TestRightView::UpdateRouteData(WORD nRteNo)
{
	if (nRteNo == 0)
		return;

	CString strSignalLock = GetSignalLock(nRteNo, false);
	CString strSwitchLock = GetSwitchLock(nRteNo, true, false);
	CString strTrackAll = GetTrackLockAll(nRteNo, 0, false);
	CString strTrackRoute = GetTrackLockRoute(nRteNo, false);
	CString strAppLock = GetAppLock(nRteNo, false);

	m_edtInterlock.SetWindowText(strSignalLock);
	m_edtSwitch.SetWindowText(strSwitchLock);
	m_edtSigCond.SetWindowText(strTrackAll);
	m_edtRouteDiv.SetWindowText(strTrackRoute);
	m_edtAppLock.SetWindowText(strAppLock);
}

// 1. 신호기 쇄정
CString TestRightView::GetSignalLock(WORD nRteNo, bool bIsPrtData)
{
	auto ilkSpan = StructMainData::GetInstance().GetInterLockInfo();
	if (nRteNo >= ilkSpan.size())
		return _T("");

	const auto& ilkInfo = ilkSpan[nRteNo];
	CString strLockList, strTemp;

	for (int i = 0; i < ilkInfo.Count.NoOfSigLock; ++i)
	{
		Byte_t nSigNo = ilkInfo.SignalLock[i];
		if (nSigNo == 0)
			break;

		CString strSigName = CommonUtil::GetDBNameByNumber(nSigNo, GetDBNameByNum::SignalIdx);

		if (bIsPrtData)
			strTemp.Format(_T("%s( ) "), (LPCTSTR)strSigName);
		else
			strTemp.Format(_T("%s "), (LPCTSTR)strSigName);

		strLockList += strTemp;
	}

	return strLockList;
}

// 2. 진로쇄정 : 선로전환기 (실제 구조체 SwitchLock 및 Exception 활용)
CString TestRightView::GetSwitchLock(WORD nRteNo, bool bIsAddTotSig, bool bIsPrtData)
{
	auto ilkSpan = StructMainData::GetInstance().GetInterLockInfo();
	auto routeSpan = StructMainData::GetInstance().GetRouteInfo();
	int nRouteIdx = (int)nRteNo - 1;

	if (nRteNo >= ilkSpan.size() || nRouteIdx < 0 || nRouteIdx >= routeSpan.size())
		return _T("");

	const auto& ilkInfo = ilkSpan[nRteNo];
	const auto& rteItem = routeSpan[nRouteIdx];
	CString strLockList, strTemp;

	for (int i = 0; i < ilkInfo.Count.NoOfSwhLock; ++i)
	{
		Byte_t nSwhNo = static_cast<Byte_t>(ilkInfo.SwitchLock[i].SwitchNo);
		if (nSwhNo == 0)
			break;

		CString strSwhName = CommonUtil::GetDBNameByNumber(nSwhNo, GetDBNameByNum::SwitchIdx);

		// InterLockingInfo::SwitchLock::Direction 상수 활용 (1:정위, 2:반위)
		bool bIsNormal = (InterLockingInfo::SwitchLock::Direction::NORMAL == ilkInfo.SwitchLock[i].Direction);
		CString strDirSuffix = bIsNormal ? _T("N") : _T("R");

		if (bIsPrtData)
			strTemp.Format(_T("%s%s( ) "), (LPCTSTR)strSwhName, (LPCTSTR)strDirSuffix);
		else
			strTemp.Format(_T("%s%s "), (LPCTSTR)strSwhName, (LPCTSTR)strDirSuffix);

		strLockList += strTemp;
	}

	// 단조건 (NoOfExcept > 0 이고 Exception 배열 존재 시)
	if (ilkInfo.Count.NoOfExcept > 0)
	{
		for (int i = 0; i < ilkInfo.Count.NoOfExcept; ++i)
		{
			if (ilkInfo.Exception[i].TblIdx == 0)
				break;
			if ('S' != ilkInfo.Exception[i].Kind) // Kind = 'S' (신호기 인덱스)
				continue;

			Byte_t nTblIdx = static_cast<Byte_t>(ilkInfo.Exception[i].TblIdx);
			Byte_t nSwhNo = static_cast<Byte_t>(ilkInfo.Exception[i].SwitchNo);

			CString strEquipName = CommonUtil::GetDBNameByNumber(nTblIdx, GetDBNameByNum::SignalIdx);
			CString strSwhName = CommonUtil::GetDBNameByNumber(nSwhNo, GetDBNameByNum::SwitchIdx);

			bool bIsExceptNormal = (InterLockingInfo::SwitchLock::Direction::NORMAL == ilkInfo.Exception[i].SwitchDir);
			CString strDirSuffix = bIsExceptNormal ? _T("N") : _T("R");

			if (bIsPrtData)
				strTemp.Format(_T("(%s%s 단 %s)( ) "), (LPCTSTR)strSwhName, (LPCTSTR)strDirSuffix, (LPCTSTR)strEquipName);
			else
				strTemp.Format(_T("(%s%s 단 %s) "), (LPCTSTR)strSwhName, (LPCTSTR)strDirSuffix, (LPCTSTR)strEquipName);

			strLockList += strTemp;
		}
	}

	// 일괄제어 신호기
	if (bIsAddTotSig && rteItem.TotRteSigNo[0] > 0)
	{
		for (int i = 0; i < 4; ++i)
		{
			Byte_t nTotSigNo = static_cast<Byte_t>(rteItem.TotRteSigNo[i]);
			if (nTotSigNo == 0)
				break;

			CString strSigName = CommonUtil::GetDBNameByNumber(nTotSigNo, GetDBNameByNum::SignalIdx);
			if (bIsPrtData)
				strTemp.Format(_T("전방입환 %s( ) "), (LPCTSTR)strSigName);
			else
				strTemp.Format(_T("전방입환 %s "), (LPCTSTR)strSigName);

			strLockList += strTemp;
		}
	}

	return strLockList;
}

// 3. 신호제어 : 궤도 (RouteLock Kind 및 LeverLockTrack 활용)
CString TestRightView::GetTrackLockAll(WORD nRteNo, UCHAR ShuntCommon, bool bIsPrtData)
{
	auto ilkSpan = StructMainData::GetInstance().GetInterLockInfo();
	auto routeSpan = StructMainData::GetInstance().GetRouteInfo();
	int nRouteIdx = (int)nRteNo;
	if (nRteNo >= ilkSpan.size())
		return _T("");
	WORD nSignNo = ilkSpan[nRteNo].RouteInfo.SignalNo;
	const auto& ilkInfo = ilkSpan[nRteNo];
	const auto& routeItem = routeSpan[nRouteIdx];
	CString strLockList, strTemp;

	for (int i = 0; i < ilkInfo.Count.NoOfCtlLock; ++i)
	{
		Byte_t nTrackNo = static_cast<Byte_t>(ilkInfo.RouteLock[i].TrackNo);
		if (nTrackNo == 0)
			break;

		BYTE kind = ilkInfo.RouteLock[i].Kind;

		// 진로쇄정(1) 또는 접촉관계쇄정(2) 포함 여부 판단
		if ((InterLockingInfo::RouteLock::Kind::ROUTE_LOCK_TRK != kind) &&
			(InterLockingInfo::RouteLock::Kind::TRACK_LOCK_TRK != kind) &&
			(InterLockingInfo::RouteLock::Kind::TEMP_LOCK_TRK != kind))
			continue;

		bool bIsLeverTrk = false;
		for (int j = 0; j < 5; ++j)
		{
			if (ilkInfo.LeverLockTrack[j] == 0)
				break;
			if (nTrackNo == ilkInfo.LeverLockTrack[j])
			{
				bIsLeverTrk = true;
				break;
			}
		}

		CString strTrkName = CommonUtil::GetDBNameByNumber(nTrackNo, GetDBNameByNum::TrackIdx);
		if (bIsLeverTrk)
		{
			if (bIsPrtData)
				strTemp.Format(_T("폐로쇄정 %s( ) "), (LPCTSTR)strTrkName);
			else
				strTemp.Format(_T("폐로쇄정 %s "), (LPCTSTR)strTrkName);
		}
		else
		{
			if (bIsPrtData)
				strTemp.Format(_T("%s( ) "), (LPCTSTR)strTrkName);
			else
				strTemp.Format(_T("%s "), (LPCTSTR)strTrkName);
		}

		strLockList += strTemp;
	}
	for (int nIdxLock = 0; nIdxLock < NO_OF_LOCK_EQUIP; ++nIdxLock)
	{
		Byte_t nEquipTblNo = ilkInfo.EquipLock[nIdxLock].TblNo;
		if (nEquipTblNo == 0)
			break;

		Byte_t kind = ilkInfo.EquipLock[nIdxLock].Kind;
		Byte_t gubun = ilkInfo.EquipLock[nIdxLock].Gubun;

		CString strEquipName;
		if (kind == 'P') // 선로전환기
		{
			strEquipName = CommonUtil::GetDBNameByNumber(nEquipTblNo, GetDBNameByNum::SwitchIdx);
		}
		else
		{

			strEquipName = CommonUtil::GetDBNameByNumber(nEquipTblNo, GetDBNameByNum::TrackIdx);

		}

		if (strEquipName.IsEmpty())
			continue;

		if (bIsPrtData)
			strTemp.Format(_T("%s(  ) "), (LPCTSTR)strEquipName);
		else
			strTemp.Format(_T("%s "), (LPCTSTR)strEquipName);

		strLockList += strTemp;
	}

	// 3. 신호 최고현시 (MaxSignal) 처리 (전역변수 RTE_INFO 제거, routeItem 활용)
	{
		CString strAspect;
		switch (routeItem.MaxSignal)
		{
		case SIG_ASPECT_VALUE_G:  strAspect = _T("최고현시G"); break;
		case SIG_ASPECT_VALUE_YG: strAspect = _T("최고현시YG"); break;
		case SIG_ASPECT_VALUE_Y:  strAspect = _T("최고현시Y"); break;
		case SIG_ASPECT_VALUE_YY: strAspect = _T("최고현시YY"); break;
		default:                  strAspect = _T("최고현시R"); break;
		}

		if (bIsPrtData)
			strTemp.Format(_T("%s(  ) "), (LPCTSTR)strAspect);
		else
			strTemp.Format(_T("%s "), (LPCTSTR)strAspect);

		strLockList += strTemp;
	}


	if (((routeItem.Kind.Depart & 0x01) == 0x01) && (routeItem.DepBlockNo > 0))
	{
		// 인자로 받은 nSigNo를 바로 사용
		CString strSigName = CommonUtil::GetDBNameByNumber(nSignNo, CommonUtil::SignalIdx);

		auto blkSpan = StructMainData::GetInstance().GetBlockInfo();
		if (routeItem.DepBlockNo < blkSpan.size())
		{
			BYTE blkKind = blkSpan[routeItem.DepBlockNo].BlkKind;
			CString strBlockTmp = CommonUtil::GetBlockTextSBR(strSigName, blkKind, true);

			if (!strBlockTmp.IsEmpty())
			{
				strLockList += strBlockTmp;
			}
		}
	}
	return strLockList;
}
// 4. 진로구분쇄정 : 궤도 (실제 RouteLock 구조 반영)
CString TestRightView::GetTrackLockRoute(WORD nRteNo, bool bIsPrtData)
{
	auto ilkSpan = StructMainData::GetInstance().GetInterLockInfo();
	if (nRteNo >= ilkSpan.size())
		return _T("");

	const auto& ilkInfo = ilkSpan[nRteNo];
	CString strLockList, strTemp;

	for (int i = 0; i < ilkInfo.Count.NoOfCtlLock; ++i)
	{
		Byte_t nTrackNo = static_cast<Byte_t>(ilkInfo.RouteLock[i].TrackNo);
		if (nTrackNo == 0)
			break;

		// 진로쇄정 궤도(1)만 추출
		if (InterLockingInfo::RouteLock::Kind::ROUTE_LOCK_TRK != ilkInfo.RouteLock[i].Kind)
			continue;

		// 도착궤도(ArrivalT)와 같은 경우 제외
		if (nTrackNo == ilkInfo.RouteInfo.ArrivalT)
			continue;

		CString strTrkName = CommonUtil::GetDBNameByNumber(nTrackNo, GetDBNameByNum::TrackIdx);
		if (bIsPrtData)
			strTemp.Format(_T("%s( ) "), (LPCTSTR)strTrkName);
		else
			strTemp.Format(_T("%s "), (LPCTSTR)strTrkName);

		strLockList += strTemp;
	}

	return strLockList;
}

// 5. 접근쇄정 (ReleaseTime 및 ApproachLock 배열 활용)
CString TestRightView::GetAppLock(WORD nRteNo, bool bIsPrtData)
{
	auto ilkSpan = StructMainData::GetInstance().GetInterLockInfo();
	if (nRteNo >= ilkSpan.size())
		return _T("");

	const auto& ilkInfo = ilkSpan[nRteNo];
	CString strLockList, strTemp;

	if (ilkInfo.ApproachLock.ReleaseTime > 0)
	{
		if (bIsPrtData)
			strTemp.Format(_T("%d초( )"), ilkInfo.ApproachLock.ReleaseTime);
		else
			strTemp.Format(_T("%d초"), ilkInfo.ApproachLock.ReleaseTime);

		strLockList += strTemp;
	}

	for (int i = 0; i < ilkInfo.Count.NoOfAppLock; ++i)
	{
		Byte_t nTrackNo = static_cast<Byte_t>(ilkInfo.ApproachLock.ApproachTrack[i].AppTrack);
		if (nTrackNo == 0)
			break;

		CString strTrkName = CommonUtil::GetDBNameByNumber(nTrackNo, GetDBNameByNum::TrackIdx);
		if (bIsPrtData)
			strTemp.Format(_T(" %s( )"), (LPCTSTR)strTrkName);
		else
			strTemp.Format(_T(" %s"), (LPCTSTR)strTrkName);

		strLockList += strTemp;
	}

	return strLockList;
}


void TestRightView::OnFileSave()
{
	CWnd* pParentWnd = GetParent()->GetParent(); // TestContainer 스플리터 윈도우
	if (pParentWnd != nullptr)
	{
		AFX_CMDHANDLERINFO info = { 0 };
		if (pParentWnd->OnCmdMsg(ID_FILE_SAVE, CN_COMMAND, NULL, &info))
		{
			return;
		}
	}
}
void TestRightView::OnFileDiff() {
	CWnd* pParentWnd = GetParent()->GetParent(); // TestContainer 스플리터 윈도우
	if (pParentWnd != nullptr)
	{
		AFX_CMDHANDLERINFO info = { 0 };
		if (pParentWnd->OnCmdMsg(ID_DIFF_TOOL, CN_COMMAND, NULL, &info))
		{
			return;
		}
	}
}
void TestRightView::OnFilePrint() {
	CWnd* pParentWnd = GetParent()->GetParent(); // TestContainer 스플리터 윈도우
	if (pParentWnd != nullptr)
	{
		AFX_CMDHANDLERINFO info = { 0 };
		if (pParentWnd->OnCmdMsg(ID_FILE_PRINT, CN_COMMAND, NULL, &info))
		{
			return;
		}
	}
}
void TestRightView::OnFileOpen()
{
	CWnd* pParentWnd = GetParent()->GetParent();; // TestContainer 스플리터 윈도우
	if (pParentWnd != nullptr)
	{
		AFX_CMDHANDLERINFO info = { 0 };
		if (pParentWnd->OnCmdMsg(ID_FILE_OPEN, CN_COMMAND, NULL, &info))
		{
			return;
		}
	}
}
void TestRightView::OnUpdatePrint(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE); // 포커스가 어디 있든 이 버튼을 항상 활성화 상태로 강제 유지!
}

void TestRightView::OnUpdateFileDiff(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE); // 파일 열기 버튼도 항상 활성화!
}