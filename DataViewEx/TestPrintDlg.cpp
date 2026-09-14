#include "stdafx.h"
#include "TestPrintDlg.h"
#include "StructMainData.h"
#include "CommonUtils.h"
#include "BitUtility.h"
#include "resource.h"
using namespace CommonUtil;

IMPLEMENT_DYNAMIC(TestPrintDlg, CBCGPDialog)

TestPrintDlg::TestPrintDlg(CWnd* pParent /*=nullptr*/)
	: CBCGPDialog(IDD_TEST_PRINT_DLG, pParent)
	, m_nCurPage(1) // 현재 미리보기 페이지 초기화
	, m_strStationName(_T("")) // 역 이름 초기화
{

}

TestPrintDlg::~TestPrintDlg()
{
}

void TestPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(TestPrintDlg, CBCGPDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_TEST_PRINT_BTN, &TestPrintDlg::OnBnClickedTestPrintBtn)
	ON_BN_CLICKED(IDC_TEST_LEFT_BTN, &TestPrintDlg::OnBnClickedTestLeftBtn)
	ON_BN_CLICKED(IDC_TEST_RIGHT_BTN, &TestPrintDlg::OnBnClickedTestRightButton)
END_MESSAGE_MAP()

BOOL TestPrintDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// 1. 화면 전체 해상도 기준 80% 크기 계산
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int dlgWidth = (int)(screenWidth * 0.8);
	int dlgHeight = (int)(screenHeight * 0.8);

	// 2. 화면 중앙에 위치하도록 좌표 계산 및 다이얼로그 크기 변경
	int newX = (screenWidth - dlgWidth) / 2;
	int newY = (screenHeight - dlgHeight) / 2;

	MoveWindow(newX, newY, dlgWidth, dlgHeight);

	// 3. 변경된 다이얼로그의 클라이언트 영역 기준 버튼 재배치
	CRect clientRect;
	GetClientRect(&clientRect);
	int btnWidth = 100;
	int btnHeight = 30;
	int margin = 20;



	int startX = clientRect.right - (btnWidth * 2 + margin + margin);
	int startY = clientRect.bottom - btnHeight - margin;


	//printBtn.MoveWindow(startX, startY, btnWidth, btnHeight);

	return TRUE;
}

void TestPrintDlg::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);

	CWnd* pBtnPrint = GetDlgItem(IDC_TEST_PRINT_BTN);
	CWnd* pBtnLeft = GetDlgItem(IDC_TEST_LEFT_BTN);
	CWnd* pBtnRight = GetDlgItem(IDC_TEST_RIGHT_BTN);

	int margin = 15;
	int spacing = 10;

	if (pBtnPrint != nullptr && pBtnPrint->GetSafeHwnd() != NULL)
	{
		CRect rectBtn;
		pBtnPrint->GetWindowRect(&rectBtn);
		int btnW = rectBtn.Width();
		int btnH = rectBtn.Height();

		int y = cy - btnH - margin;
		int x = cx - btnW - margin;

		pBtnPrint->MoveWindow(x, y, btnW, btnH);

		if (pBtnRight != nullptr && pBtnRight->GetSafeHwnd() != NULL)
		{
			x -= (btnW + spacing);
			pBtnRight->MoveWindow(x, y, btnW, btnH);
		}

		if (pBtnLeft != nullptr && pBtnLeft->GetSafeHwnd() != NULL)
		{
			x -= (btnW + spacing);
			pBtnLeft->MoveWindow(x, y, btnW, btnH);
		}
	}

	Invalidate();
}

//void TestPrintDlg::OnPaint()
//{
//	CPaintDC dc(this);
//
//	CRect rectClient;
//	GetClientRect(&rectClient);
//
//	rectClient.bottom -= 50;
//	rectClient.DeflateRect(15, 15);
//
//	dc.FillSolidRect(&rectClient, RGB(200, 200, 200));
//
//	CRect rectInner = rectClient;
//	rectInner.DeflateRect(20, 20);
//
//	CRect rectA4 = rectInner;
//	double a4Ratio = 297.0 / 210.0;
//
//	if ((double)rectInner.Width() / rectInner.Height() > a4Ratio)
//	{
//		int targetW = (int)(rectInner.Height() * a4Ratio);
//		int offsetX = (rectInner.Width() - targetW) / 2;
//		rectA4 = CRect(rectInner.left + offsetX, rectInner.top, rectInner.left + offsetX + targetW, rectInner.bottom);
//	}
//	else
//	{
//		int targetH = (int)(rectInner.Width() / a4Ratio);
//		int offsetY = (rectInner.Height() - targetH) / 2;
//		rectA4 = CRect(rectInner.left, rectInner.top + offsetY, rectInner.right, rectInner.top + offsetY + targetH);
//	}
//
//	dc.FillSolidRect(&rectA4, RGB(255, 255, 255));
//	dc.Draw3dRect(&rectA4, RGB(100, 100, 100), RGB(50, 50, 50));
//
//	int nSavedDC = dc.SaveDC();
//	dc.IntersectClipRect(&rectA4);
//
//	double scaleX = (double)rectA4.Width() / 297.0;
//	double scaleY = (double)rectA4.Height() / 210.0;
//
//	CFont titleFont, headerFont, smallFont;
//	titleFont.CreatePointFont(130, _T("맑은 고딕"));
//	headerFont.CreatePointFont(90, _T("맑은 고딕"));
//	smallFont.CreatePointFont(70, _T("맑은 고딕"));
//
//	CFont* pOldFont = dc.SelectObject(&titleFont);
//	dc.SetBkMode(TRANSPARENT);
//	COLORREF oldTextColor = dc.SetTextColor(RGB(0, 0, 0));
//
//	int startX = rectA4.left + (int)(10.0 * scaleX);
//	int startY = rectA4.top + (int)(10.0 * scaleY);
//	int totalWidth = (int)(277.0 * scaleX);
//
//	CRect rectTopBox(startX, startY, startX + totalWidth, startY + (int)(16.0 * scaleY));
//	dc.Draw3dRect(&rectTopBox, RGB(0, 0, 0), RGB(0, 0, 0));
//
//	CRect rectCompany(rectTopBox.left, rectTopBox.top, rectTopBox.left + (int)(55.0 * scaleX), rectTopBox.bottom);
//	CRect rectDocTitle(rectCompany.right, rectTopBox.top, rectTopBox.right, rectTopBox.bottom);
//	dc.Draw3dRect(&rectCompany, RGB(0, 0, 0), RGB(0, 0, 0));
//
//	dc.DrawText(_T("유 경 제 어 (주)"), &rectCompany, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//	dc.DrawText(_T("속 도 코 드 제 어 장 치 체 크 리 스 트"), &rectDocTitle, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//
//	startY = rectTopBox.bottom;
//	int metaHeight = (int)(9.0 * scaleY);
//	CRect rectMeta(startX, startY, startX + totalWidth, startY + metaHeight);
//	dc.Draw3dRect(&rectMeta, RGB(0, 0, 0), RGB(0, 0, 0));
//
//	dc.SelectObject(&headerFont);
//	int w1 = (int)(15.0 * scaleX);
//	int w2 = (int)(30.0 * scaleX);
//	int w3 = (int)(22.0 * scaleX);
//	int w4 = (int)(45.0 * scaleX);
//	int w5 = (int)(22.0 * scaleX);
//	int w6 = (int)(45.0 * scaleX);
//
//	CRect rSNameLbl(rectMeta.left, rectMeta.top, rectMeta.left + w1, rectMeta.bottom);
//	CRect rSNameVal(rSNameLbl.right, rectMeta.top, rSNameLbl.right + w2, rectMeta.bottom);
//	CRect rDateLbl(rSNameVal.right, rectMeta.top, rSNameVal.right + w3, rectMeta.bottom);
//	CRect rDateVal(rDateLbl.right, rectMeta.top, rDateLbl.right + w4, rectMeta.bottom);
//	CRect rTimeLbl(rDateVal.right, rectMeta.top, rDateVal.right + w5, rectMeta.bottom);
//	CRect rTimeVal(rTimeLbl.right, rectMeta.top, rTimeLbl.right + w6, rectMeta.bottom);
//	CRect rPageVal(rTimeVal.right, rectMeta.top, rectMeta.right, rectMeta.bottom);
//
//	dc.Draw3dRect(&rSNameLbl, RGB(0, 0, 0), RGB(0, 0, 0)); dc.DrawText(_T("역 명"), &rSNameLbl, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//	dc.Draw3dRect(&rSNameVal, RGB(0, 0, 0), RGB(0, 0, 0)); dc.DrawText(m_strStationName, &rSNameVal, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//	dc.Draw3dRect(&rDateLbl, RGB(0, 0, 0), RGB(0, 0, 0)); dc.DrawText(_T("시 험 일 시"), &rDateLbl, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//	dc.Draw3dRect(&rDateVal, RGB(0, 0, 0), RGB(0, 0, 0)); dc.DrawText(_T(""), &rDateVal, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//	dc.Draw3dRect(&rTimeLbl, RGB(0, 0, 0), RGB(0, 0, 0)); dc.DrawText(_T("시 험 시 간"), &rTimeLbl, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//	dc.Draw3dRect(&rTimeVal, RGB(0, 0, 0), RGB(0, 0, 0)); dc.DrawText(_T(""), &rTimeVal, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//
//	startY = rectMeta.bottom;
//	int headerHeight = (int)(8.0 * scaleY);
//	CRect rectTblHead(startX, startY, startX + totalWidth, startY + headerHeight);
//	dc.Draw3dRect(&rectTblHead, RGB(0, 0, 0), RGB(0, 0, 0));
//
//	int colW_Signal = (int)(20.0 * scaleX);
//	int colW_Route = (int)(30.0 * scaleX);
//	int colW_Cond = (int)(145.0 * scaleX);
//	int colW_Status = (int)(72.0 * 0.6 * scaleX);
//
//	CRect rH1(rectTblHead.left, rectTblHead.top, rectTblHead.left + colW_Signal, rectTblHead.bottom);
//	CRect rH2(rH1.right, rectTblHead.top, rH1.right + colW_Route, rectTblHead.bottom);
//	CRect rH3(rH2.right, rectTblHead.top, rH2.right + colW_Cond + colW_Status, rectTblHead.bottom);
//	CRect rH4(rH3.right, rectTblHead.top, rectTblHead.right, rectTblHead.bottom);
//
//	dc.Draw3dRect(&rH1, RGB(0, 0, 0), RGB(0, 0, 0)); dc.DrawText(_T("신호기"), &rH1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//	dc.Draw3dRect(&rH2, RGB(0, 0, 0), RGB(0, 0, 0)); dc.DrawText(_T("진로"), &rH2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//	dc.Draw3dRect(&rH3, RGB(0, 0, 0), RGB(0, 0, 0)); dc.DrawText(_T("체 크 항 목"), &rH3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//	dc.Draw3dRect(&rH4, RGB(0, 0, 0), RGB(0, 0, 0)); dc.DrawText(_T("기 사(서명)"), &rH4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//
//	dc.SelectObject(&smallFont);
//	int currentY = rectTblHead.bottom;
//	int rowHeight = (int)(50.0 * scaleY);
//
//	int bottomMargin = (int)(15.0 * scaleY);
//	int maxDisplayRows = (rectA4.bottom - bottomMargin - currentY) / rowHeight;
//	if (maxDisplayRows < 1) maxDisplayRows = 1;
//
//	auto routeSpan = StructMainData::GetInstance().GetRouteInfo();
//	auto ilkSpan = StructMainData::GetInstance().GetInterLockInfo();
//	auto sigSpan = StructMainData::GetInstance().GetSignalInfo();
//	std::vector<size_t> validIndices;
//	for (size_t i = 0; i < routeSpan.size(); ++i)
//	{
//		WORD nRteNo = static_cast<WORD>(i);
//		if (nRteNo >= ilkSpan.size()) continue;
//		CString strRouteName(ilkSpan[nRteNo].Name);
//		if (strRouteName.IsEmpty()) continue;
//		// ==============================================================
//		// [핵심 수정] 여기서 애초에 '!'가 포함된 진로를 인쇄 목록에서 빼버립니다.
//		if (strRouteName.Find(_T('!')) != -1)
//		{
//			continue; // validIndices 에 들어가지 않음
//		}
//		// ==============================================================
//		validIndices.push_back(i);
//	}
//
//	int totalPages = (int)((validIndices.size() + maxDisplayRows - 1) / maxDisplayRows);
//	if (totalPages < 1) totalPages = 1;
//	if (m_nCurPage > totalPages) m_nCurPage = totalPages;
//	if (m_nCurPage < 1) m_nCurPage = 1;
//
//	CString strPageText;
//	strPageText.Format(_T("%d / %d 페이지"), m_nCurPage, totalPages);
//	dc.Draw3dRect(&rPageVal, RGB(0, 0, 0), RGB(0, 0, 0));
//	dc.DrawText(strPageText, &rPageVal, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//
//	size_t startIndex = (size_t)(m_nCurPage - 1) * maxDisplayRows;
//	size_t endIndex = min(startIndex + maxDisplayRows, validIndices.size());
//
//	for (size_t idx = startIndex; idx < endIndex; ++idx)
//	{
//		size_t i = validIndices[idx];
//		WORD nRteNo = static_cast<WORD>(i);
//		CString strRouteName(ilkSpan[nRteNo].Name);
//		if (strRouteName.Find(_T('!')) != -1)
//		{
//			continue;
//		}
//		int nRouteIdx = (int)i;
//		const auto& routeItem = routeSpan[nRouteIdx];
//
//		WORD nDepTrk = ilkSpan[nRteNo].RouteInfo.DepartureT;
//		WORD nArrTrk = ilkSpan[nRteNo].RouteInfo.ArrivalT;
//		WORD nSigNo = ilkSpan[nRteNo].RouteInfo.SignalNo;
//
//		CString strTrackDep;
//		if (nDepTrk > 0)
//			strTrackDep = CommonUtil::GetDBNameByNumber(nDepTrk, GetDBNameByNum::TrackIdx);
//		else
//			strTrackDep = CommonUtil::GetDBNameByNumber(nSigNo, GetDBNameByNum::SignalIdx);
//
//		CString strTrackArr;
//		if (nArrTrk > 0)
//			strTrackArr = CommonUtil::GetDBNameByNumber(nArrTrk, GetDBNameByNum::TrackIdx);
//		else
//		{
//			int nIdxFind = strRouteName.Find(_T('-'));
//			if (nIdxFind > -1 && nIdxFind + 1 < strRouteName.GetLength())
//				strTrackArr = strRouteName.Mid(nIdxFind + 1);
//			else
//				strTrackArr = strRouteName;
//		}
//
//		CString strRouteInfoText;
//		if (!strTrackDep.IsEmpty() && !strTrackArr.IsEmpty())
//			strRouteInfoText.Format(_T("%s -> %s"), (LPCTSTR)strTrackDep, (LPCTSTR)strTrackArr);
//		else
//			strRouteInfoText = strRouteName;
//
//		CString strInterlock = GetSignalLock(nRteNo);
//		CString strSwitchLock = GetSwitchLock(nRteNo, true);
//		CString strSigCond = GetTrackLockAll(nRteNo, 0);
//		CString strRouteDiv = GetTrackLockRoute(nRteNo);
//		CString strAppLock = GetAppLock(nRteNo);
//
//		CRect rectRow(startX, currentY, startX + totalWidth, currentY + rowHeight);
//		dc.Draw3dRect(&rectRow, RGB(0, 0, 0), RGB(0, 0, 0));
//
//		CRect rC1(rectRow.left, rectRow.top, rectRow.left + colW_Signal, rectRow.bottom);
//		CRect rC2(rC1.right, rectRow.top, rC1.right + colW_Route, rectRow.bottom);
//		dc.Draw3dRect(&rC1, RGB(0, 0, 0), RGB(0, 0, 0));
//		dc.Draw3dRect(&rC2, RGB(0, 0, 0), RGB(0, 0, 0));
//
//		CString strSigName = CommonUtil::GetDBNameByNumber(nSigNo, GetDBNameByNum::SignalIdx);
//
//		CString strText;
//		bool bIsArrive = IsBitSet(routeItem.Kind.Value.Arrive, 0);
//		bool bIsDepart = IsBitSet(routeItem.Kind.Value.Depart, 0);
//		bool bIsShunt = IsBitSet(routeItem.Kind.Value.Shunt, 0);
//		bool bIsCallOn = IsBitSet(routeItem.RouteFlag.Value.UdoSig, 0);
//		bool bIsHomeBlock = IsBitSet(routeItem.Kind.Value.IsBlock, 0);
//
//		bool bIsShuntSig = (nSigNo > 0);
//		bool bIsHomeSigBlock = bIsHomeBlock;
//
//		if (bIsArrive)
//		{
//			strText = _T("\n\n장\n\n내\n\n신\n\n호");
//		}
//		else if (bIsDepart)
//		{
//			strText = _T("\n\n출\n\n발\n\n신\n\n호");
//		}
//		else if (bIsShunt)
//		{
//			if (bIsShuntSig)
//			{
//				if (!bIsCallOn)
//				{
//					strText = _T("\n\n입\n\n환\n\n표\n\n지");
//				}
//				else
//				{
//					strText = _T("\n\n입\n\n환\n\n신\n\n호");
//				}
//			}
//			else
//			{
//				strText = _T("\n\n입\n\n환\n\n표\n\n지");
//			}
//		}
//		else
//		{
//			if (bIsHomeSigBlock)
//			{
//				strText = _T("\n\n구\n\n내\n\n폐\n\n색");
//			}
//			else
//			{
//				strText = _T("\n\n신\n\n호\n\n진\n\n로");
//			}
//		}
//
//		CRect rC1_text = rC1; rC1_text.DeflateRect(2, 2);
//		dc.DrawText(strText, &rC1_text, DT_CENTER | DT_VCENTER);
//
//		CRect rcRouteText = rC2;
//		rcRouteText.bottom = rcRouteText.top + (rowHeight / 4);
//		dc.DrawText(strRouteName, &rcRouteText, DT_CENTER | DT_SINGLELINE | DT_BOTTOM);
//
//		CRect rcTrackText = rC2;
//		rcTrackText.top = rectRow.top + (rowHeight / 2);
//		rcTrackText.bottom = rcTrackText.top + (rowHeight / 2);
//		dc.DrawText(strRouteInfoText, &rcTrackText, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
//
//		// 체크 항목 (5개 행)
//		int subItemsCount = 5;
//		int subHeight = rowHeight / subItemsCount;
//		int subX = rC2.right;
//
//		CString subLabels[5] = { _T("상호쇄정"), _T("진로쇄정"), _T("신호제어"), _T("진로구분쇄정"), _T("접근 및 보류쇄정") };
//		CString subValues[5] = { strInterlock, strSwitchLock, strSigCond, strRouteDiv, strAppLock };
//
//		int colW_Label = (int)(35.0 * scaleX);
//
//		for (int s = 0; s < 5; ++s)
//		{
//			int topY = rectRow.top + (s * subHeight);
//			int bottomY = (s == subItemsCount - 1) ? rectRow.bottom : (rectRow.top + ((s + 1) * subHeight));
//
//			CRect rLabel(subX, topY, subX + colW_Label, bottomY);
//			CRect rData(rLabel.right, topY, subX + colW_Cond, bottomY);
//
//			dc.Draw3dRect(&rLabel, RGB(0, 0, 0), RGB(0, 0, 0));
//			dc.Draw3dRect(&rData, RGB(0, 0, 0), RGB(0, 0, 0));
//
//			dc.DrawText(subLabels[s], &rLabel, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//
//			CRect rDataText = rData;
//			rDataText.DeflateRect(4, 2);
//			dc.DrawText(subValues[s], &rDataText, DT_LEFT | DT_WORDBREAK);
//		}
//
//		// 조작반표시상태, 진로선별등(또는 출발반응등), ATS 항목 (데이터 조건에 따라 동적 생성)
//		int statusX = subX + colW_Cond;
//
//		std::vector<CString> statusList;
//		statusList.push_back(_T("조작반표시상태\n(신호 및 진로) ( )"));
//		int nSigIdx = (int)nSigNo - 1;
//
//		// [수정] 원본 조건 반영: 신호기 기준 구내폐색 비트가 0일 때 (즉, 구내폐색이 아닐 때)
//		bool bIsHomeBlockSig = false;
//		if (nSigIdx > 0 && nSigIdx < sigSpan.size())
//		{
//			bIsHomeBlockSig = ((sigSpan[nSigIdx].Kind.Value.HomeBlockS & 0x01) == 0x01);
//		}
//
//		// 구내폐색이 아닌 경우 (!bIsHomeBlockSig)에만 출발반응등 또는 진로선별등 추가
//		if (!bIsHomeBlockSig)
//		{
//			if (bIsDepart)
//				statusList.push_back(_T("출발반응등\n( )"));
//			else
//				statusList.push_back(_T("진로선별등\n( )"));
//		}
//
//		// 입환진로가 아닌 경우에만 ATS 추가
//		if (!bIsShunt)
//		{
//			statusList.push_back(_T("ATS\n( )"));
//		}
//
//		size_t statusItemsCount = statusList.size();
//		if (statusItemsCount < 1) statusItemsCount = 1;
//		int statusHeight = rowHeight / statusItemsCount;
//
//		for (size_t st = 0; st < statusItemsCount; ++st)
//		{
//			int topY = rectRow.top + (int)(st * statusHeight);
//			int bottomY = (st == statusItemsCount - 1) ? rectRow.bottom : (rectRow.top + (int)((st + 1) * statusHeight));
//
//			CRect rSp(statusX, topY, statusX + colW_Status, bottomY);
//			dc.Draw3dRect(&rSp, RGB(0, 0, 0), RGB(0, 0, 0));
//			CRect rSp_t = rSp;
//			rSp_t.DeflateRect(2, 2);
//			CRect rcCalc = rSp_t;
//			dc.DrawText(statusList[st], &rcCalc, DT_CENTER | DT_WORDBREAK | DT_CALCRECT);
//
//			// 2. 셀 높이와 텍스트 높이의 차이를 계산하여 top 위치를 수직 중앙으로 이동
//			int textHeight = rcCalc.Height();
//			int cellHeight = rSp_t.Height();
//
//			if (cellHeight > textHeight)
//			{
//				rSp_t.top += (cellHeight - textHeight) / 2;
//			}
//
//			// 3. 중앙으로 조정된 영역에 텍스트 출력
//			dc.DrawText(statusList[st], &rSp_t, DT_CENTER | DT_WORDBREAK);
//		}
//
//		// 기사 칸 (우측 잔여 영역)
//		CRect rCheckCol(statusX + colW_Status, rectRow.top, rectRow.right, rectRow.bottom);
//		dc.Draw3dRect(&rCheckCol, RGB(0, 0, 0), RGB(0, 0, 0));
//		dc.DrawText(_T(""), &rCheckCol, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//
//		currentY += rowHeight;
//	}
//
//	CRect rectFooter(rectA4.left, rectA4.bottom - (int)(12.0 * scaleY), rectA4.right, rectA4.bottom - (int)(2.0 * scaleY));
//	CString strFooterText;
//	strFooterText.Format(_T("- %d / %d -"), m_nCurPage, totalPages);
//	dc.SelectObject(&headerFont);
//	dc.DrawText(strFooterText, &rectFooter, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//
//	dc.SetTextColor(oldTextColor);
//	dc.SelectObject(pOldFont);
//	titleFont.DeleteObject();
//	headerFont.DeleteObject();
//	smallFont.DeleteObject();
//
//	dc.RestoreDC(nSavedDC);
//}
void TestPrintDlg::OnPaint()
{
	CPaintDC dc(this);

	CRect rectClient;
	GetClientRect(&rectClient);

	rectClient.bottom -= 50;
	rectClient.DeflateRect(15, 15);

	dc.FillSolidRect(&rectClient, RGB(200, 200, 200));

	CRect rectInner = rectClient;
	rectInner.DeflateRect(20, 20);

	CRect rectA4 = rectInner;
	double a4Ratio = 297.0 / 210.0;

	if ((double)rectInner.Width() / rectInner.Height() > a4Ratio)
	{
		int targetW = (int)(rectInner.Height() * a4Ratio);
		int offsetX = (rectInner.Width() - targetW) / 2;
		rectA4 = CRect(rectInner.left + offsetX, rectInner.top, rectInner.left + offsetX + targetW, rectInner.bottom);
	}
	else
	{
		int targetH = (int)(rectInner.Width() / a4Ratio);
		int offsetY = (rectInner.Height() - targetH) / 2;
		rectA4 = CRect(rectInner.left, rectInner.top + offsetY, rectInner.right, rectInner.top + offsetY + targetH);
	}

	dc.FillSolidRect(&rectA4, RGB(255, 255, 255));
	dc.Draw3dRect(&rectA4, RGB(100, 100, 100), RGB(50, 50, 50));

	int nSavedDC = dc.SaveDC();
	dc.IntersectClipRect(&rectA4);

	double scaleX = (double)rectA4.Width() / 297.0;
	double scaleY = (double)rectA4.Height() / 210.0;

	CFont titleFont, headerFont, smallFont;
	titleFont.CreatePointFont(130, _T("맑은 고딕"));
	headerFont.CreatePointFont(90, _T("맑은 고딕"));
	smallFont.CreatePointFont(70, _T("맑은 고딕"));

	CFont* pOldFont = dc.SelectObject(&titleFont);
	dc.SetBkMode(TRANSPARENT);
	COLORREF oldTextColor = dc.SetTextColor(RGB(0, 0, 0));
	int startX = rectA4.left + (int)(10.0 * scaleX);
	int totalWidth = (int)(277.0 * scaleX);

	int dummyStartY = rectA4.top + (int)(10.0 * scaleY) + (int)(25.0 * scaleY);

	std::vector<size_t> validIndices;
	int maxDisplayRows = 0;
	int rowHeight = (int)(50.0 * scaleY);
	int switchPages = 1;
	int indicatorPages = 1; // [추가] 표시등 페이지 수 변수 선언

	// [수정] GetTotalPageCount 인자 변경 반영
	int totalPages = GetTotalPageCount(validIndices, maxDisplayRows, rowHeight, rectA4, dummyStartY, switchPages, indicatorPages);

	if (m_nCurPage > totalPages) m_nCurPage = totalPages;
	if (m_nCurPage < 1) m_nCurPage = 1;

	// [수정] 현재 페이지 구간에 따른 동적 문서 제목 설정
	CString strTitle;
	if (m_nCurPage <= indicatorPages)
		strTitle = _T("각 종 표 시 등 동 작 상 태");
	else if (m_nCurPage <= indicatorPages + switchPages)
		strTitle = _T("선 로 전 환 기 단 동 시 험");
	else
		strTitle = _T("속 도 코 드 제 어 장 치 체 크 리 스 트");

	int currentY = DrawPageHeader(&dc, rectA4, scaleX, scaleY, &titleFont, &headerFont, strTitle, m_nCurPage, totalPages);

	// [수정] 페이지별 분기 렌더링 (표시등 페이지 개수 가변 대응)
	if (m_nCurPage <= indicatorPages)
	{
		int nIndicatorSubPage = m_nCurPage; // 1페이지부터 시작
		DrawIndicatorTestPage(&dc, rectA4, scaleX, scaleY, startX, currentY, totalWidth, &headerFont, &smallFont, nIndicatorSubPage);
	}
	else if (m_nCurPage <= indicatorPages + switchPages)
	{
		int nSwitchSubPage = m_nCurPage - indicatorPages;
		DrawSwitchTestPage(&dc, rectA4, scaleX, scaleY, startX, currentY, totalWidth, &headerFont, &smallFont, nSwitchSubPage);
	}
	else
	{
		int nChecklistSubPage = m_nCurPage - (indicatorPages + switchPages);
		DrawChecklistPage(&dc, rectA4, scaleX, scaleY, startX, currentY, totalWidth, &headerFont, &smallFont, validIndices, maxDisplayRows, nChecklistSubPage);
	}

	DrawPageFooter(&dc, rectA4, scaleX, scaleY, &headerFont, m_nCurPage, totalPages);

	dc.SetTextColor(oldTextColor);
	titleFont.DeleteObject();
	headerFont.DeleteObject();
	smallFont.DeleteObject();
	dc.RestoreDC(nSavedDC);
}
void TestPrintDlg::OnBnClickedTestLeftBtn()
{
	if (m_nCurPage > 1)
	{
		m_nCurPage--;
		Invalidate();
	}
}

void TestPrintDlg::OnBnClickedTestRightButton()
{
	CRect rectClient;
	GetClientRect(&rectClient);

	CRect rectInner = rectClient;
	rectInner.bottom -= 50;
	rectInner.DeflateRect(15, 15);

	CRect rectA4 = rectInner;
	double a4Ratio = 297.0 / 210.0;
	if ((double)rectInner.Width() / rectInner.Height() > a4Ratio)
	{
		int targetW = (int)(rectInner.Height() * a4Ratio);
		int offsetX = (rectInner.Width() - targetW) / 2;
		rectA4 = CRect(rectInner.left + offsetX, rectInner.top, rectInner.left + offsetX + targetW, rectInner.bottom);
	}
	else
	{
		int targetH = (int)(rectInner.Width() / a4Ratio);
		int offsetY = (rectInner.Height() - targetH) / 2;
		rectA4 = CRect(rectInner.left, rectInner.top + offsetY, rectInner.right, rectInner.top + offsetY + targetH);
	}

	double scaleY = (double)rectA4.Height() / 210.0;

	int dummyStartY = rectA4.top + (int)(10.0 * scaleY) + (int)(25.0 * scaleY);
	int rowHeight = (int)(50.0 * scaleY);

	std::vector<size_t> validIndices;
	int maxDisplayRows = 0;
	int switchPages = 1;
	int indicatorPages = 1; // [추가]

	// [수정] GetTotalPageCount 호출부 인자 완벽 동기화
	int totalPages = GetTotalPageCount(validIndices, maxDisplayRows, rowHeight, rectA4, dummyStartY, switchPages, indicatorPages);

	if (m_nCurPage < totalPages)
	{
		m_nCurPage++;
		Invalidate();
	}
}

void TestPrintDlg::OnBnClickedTestPrintBtn()
{
	CPrintDialog dlg(FALSE);
	if (AfxGetApp()->GetPrinterDeviceDefaults(&dlg.m_pd))
	{
		DEVMODE* pDevMode = (DEVMODE*)::GlobalLock(dlg.m_pd.hDevMode);
		if (pDevMode != nullptr)
		{
			pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
			::GlobalUnlock(dlg.m_pd.hDevMode);
		}
	}

	if (dlg.DoModal() == IDOK)
	{
		HDC hDC = dlg.GetPrinterDC();
		if (hDC == nullptr)
		{
			AfxMessageBox(_T("프린터 DC를 가져오지 못했습니다."));
			return;
		}

		CDC dc;
		dc.Attach(hDC);

		DOCINFO di;
		::ZeroMemory(&di, sizeof(DOCINFO));
		di.cbSize = sizeof(DOCINFO);
		di.lpszDocName = _T("인쇄 문서");

		if (dc.StartDoc(&di) > 0)
		{
			// 프린터 해상도를 가상 A4 크기로 취급
			int nHorzRes = dc.GetDeviceCaps(HORZRES);
			int nVertRes = dc.GetDeviceCaps(VERTRES);
			CRect rectA4(0, 0, nHorzRes, nVertRes);

			double scaleX = (double)rectA4.Width() / 297.0;
			double scaleY = (double)rectA4.Height() / 210.0;

			CFont titleFont, headerFont, smallFont;
			titleFont.CreatePointFont(130, _T("맑은 고딕"), &dc);
			headerFont.CreatePointFont(100, _T("맑은 고딕"), &dc);
			smallFont.CreatePointFont(70, _T("맑은 고딕"), &dc);

			// 1. 총 페이지 수 계산 (OnPaint와 동일한 로직)
			std::vector<size_t> validIndices;
			int maxDisplayRows = 0;
			int rowHeight = (int)(50.0 * scaleY);
			int switchPages = 1;
			int indicatorPages = 1;
			int totalPages = GetTotalPageCount(validIndices, maxDisplayRows, rowHeight, rectA4,
				rectA4.top + (int)(35.0 * scaleY), switchPages, indicatorPages);

			// 2. 전체 페이지를 순회하며 인쇄
			for (int pageNo = 1; pageNo <= totalPages; ++pageNo)
			{
				dc.StartPage();
				dc.SetBkMode(TRANSPARENT);

				// 제목 결정
				CString strTitle;
				strTitle = _T("속 도 코 드 제 어 장 치 체 크 리 스 트");

				// OnPaint에서 쓰던 그리기 함수들을 그대로 호출!
				int currentY = DrawPageHeader(&dc, rectA4, scaleX, scaleY, &titleFont, &headerFont, strTitle, pageNo, totalPages);

				int startX = rectA4.left + (int)(10.0 * scaleX);
				int totalWidth = (int)(277.0 * scaleX);

				if (pageNo <= indicatorPages)
				{
					DrawIndicatorTestPage(&dc, rectA4, scaleX, scaleY, startX, currentY, totalWidth, &headerFont, &smallFont, pageNo);
				}
				else if (pageNo <= indicatorPages + switchPages)
				{
					DrawSwitchTestPage(&dc, rectA4, scaleX, scaleY, startX, currentY, totalWidth, &headerFont, &smallFont, pageNo - indicatorPages);
				}
				else
				{
					DrawChecklistPage(&dc, rectA4, scaleX, scaleY, startX, currentY, totalWidth, &headerFont, &smallFont, validIndices, maxDisplayRows, pageNo - (indicatorPages + switchPages));
				}

				//DrawPageFooter(&dc, rectA4, scaleX, scaleY, &headerFont, pageNo, totalPages);

				dc.EndPage();
			}

			dc.EndDoc();
		}
		dc.Detach();
		::DeleteDC(hDC);

		AfxMessageBox(_T("인쇄가 완료되었습니다."));
	}
}

CString TestPrintDlg::GetSignalLock(WORD nRteNo)
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
		strTemp.Format(_T("%s( ) "), (LPCTSTR)strSigName);
		strLockList += strTemp;
	}
	return strLockList;
}

CString TestPrintDlg::GetSwitchLock(WORD nRteNo, bool bIsAddTotSig)
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
		bool bIsNormal = (InterLockingInfo::SwitchLock::Direction::NORMAL == ilkInfo.SwitchLock[i].Direction);
		CString strDirSuffix = bIsNormal ? _T("N") : _T("R");

		strTemp.Format(_T("%s%s( ) "), (LPCTSTR)strSwhName, (LPCTSTR)strDirSuffix);
		strLockList += strTemp;
	}

	if (ilkInfo.Count.NoOfExcept > 0)
	{
		for (int i = 0; i < ilkInfo.Count.NoOfExcept; ++i)
		{
			if (ilkInfo.Exception[i].TblIdx == 0)
				break;
			if ('S' != ilkInfo.Exception[i].Kind)
				continue;

			Byte_t nTblIdx = static_cast<Byte_t>(ilkInfo.Exception[i].TblIdx);
			Byte_t nSwhNo = static_cast<Byte_t>(ilkInfo.Exception[i].SwitchNo);

			CString strEquipName = CommonUtil::GetDBNameByNumber(nTblIdx, GetDBNameByNum::SignalIdx);
			CString strSwhName = CommonUtil::GetDBNameByNumber(nSwhNo, GetDBNameByNum::SwitchIdx);
			bool bIsExceptNormal = (InterLockingInfo::SwitchLock::Direction::NORMAL == ilkInfo.Exception[i].SwitchDir);
			CString strDirSuffix = bIsExceptNormal ? _T("N") : _T("R");

			strTemp.Format(_T("(%s%s 단 %s)( ) "), (LPCTSTR)strSwhName, (LPCTSTR)strDirSuffix, (LPCTSTR)strEquipName);
			strLockList += strTemp;
		}
	}

	if (bIsAddTotSig && rteItem.TotRteSigNo[0] > 0)
	{
		for (int i = 0; i < 4; ++i)
		{
			Byte_t nTotSigNo = static_cast<Byte_t>(rteItem.TotRteSigNo[i]);
			if (nTotSigNo == 0)
				break;

			CString strSigName = CommonUtil::GetDBNameByNumber(nTotSigNo, GetDBNameByNum::SignalIdx);
			strTemp.Format(_T("전방입환 %s( ) "), (LPCTSTR)strSigName);
			strLockList += strTemp;
		}
	}

	return strLockList;
}

CString TestPrintDlg::GetTrackLockAll(WORD nRteNo, UCHAR ShuntCommon)
{
	auto ilkSpan = StructMainData::GetInstance().GetInterLockInfo();
	auto routeSpan = StructMainData::GetInstance().GetRouteInfo();
	int nRouteIdx = (int)nRteNo;
	if (nRteNo >= ilkSpan.size() || nRouteIdx < 0 || nRouteIdx >= routeSpan.size())
		return _T("");

	const auto& ilkInfo = ilkSpan[nRteNo];
	const auto& routeItem = routeSpan[nRouteIdx];
	CString strLockList, strTemp;

	for (int i = 0; i < ilkInfo.Count.NoOfCtlLock; ++i)
	{
		Byte_t nTrackNo = static_cast<Byte_t>(ilkInfo.RouteLock[i].TrackNo);
		if (nTrackNo == 0)
			break;

		BYTE kind = ilkInfo.RouteLock[i].Kind;
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
			strTemp.Format(_T("폐로쇄정 %s( ) "), (LPCTSTR)strTrkName);
		}
		else
		{
			strTemp.Format(_T("%s( ) "), (LPCTSTR)strTrkName);
		}
		strLockList += strTemp;
	}

	for (int nIdxLock = 0; nIdxLock < NO_OF_LOCK_EQUIP; ++nIdxLock)
	{
		Byte_t nEquipTblNo = ilkInfo.EquipLock[nIdxLock].TblNo;
		if (nEquipTblNo == 0)
			break;

		Byte_t kind = ilkInfo.EquipLock[nIdxLock].Kind;
		CString strEquipName;
		if (kind == 'P')
			strEquipName = CommonUtil::GetDBNameByNumber(nEquipTblNo, GetDBNameByNum::SwitchIdx);
		else
			strEquipName = CommonUtil::GetDBNameByNumber(nEquipTblNo, GetDBNameByNum::TrackIdx);

		if (strEquipName.IsEmpty())
			continue;

		strTemp.Format(_T("%s( ) "), (LPCTSTR)strEquipName);
		strLockList += strTemp;
	}

	CString strAspect;
	switch (routeItem.MaxSignal)
	{
	case SIG_ASPECT_VALUE_G:    strAspect = _T("최고현시G"); break;
	case SIG_ASPECT_VALUE_YG:   strAspect = _T("최고현시YG"); break;
	case SIG_ASPECT_VALUE_Y:    strAspect = _T("최고현시Y"); break;
	case SIG_ASPECT_VALUE_YY:   strAspect = _T("최고현시YY"); break;
	default:                    strAspect = _T("최고현시R"); break;
	}

	strTemp.Format(_T("%s( ) "), (LPCTSTR)strAspect);
	strLockList += strTemp;

	return strLockList;
}

CString TestPrintDlg::GetTrackLockRoute(WORD nRteNo)
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

		if (InterLockingInfo::RouteLock::Kind::ROUTE_LOCK_TRK != ilkInfo.RouteLock[i].Kind)
			continue;

		if (nTrackNo == ilkInfo.RouteInfo.ArrivalT)
			continue;

		CString strTrkName = CommonUtil::GetDBNameByNumber(nTrackNo, GetDBNameByNum::TrackIdx);
		strTemp.Format(_T("%s( ) "), (LPCTSTR)strTrkName);
		strLockList += strTemp;
	}

	return strLockList;
}

CString TestPrintDlg::GetAppLock(WORD nRteNo)
{
	auto ilkSpan = StructMainData::GetInstance().GetInterLockInfo();
	if (nRteNo >= ilkSpan.size())
		return _T("");

	const auto& ilkInfo = ilkSpan[nRteNo];
	CString strLockList, strTemp;

	if (ilkInfo.ApproachLock.ReleaseTime > 0)
	{
		strTemp.Format(_T("%d초( )"), ilkInfo.ApproachLock.ReleaseTime);
		strLockList += strTemp;
	}

	for (int i = 0; i < ilkInfo.Count.NoOfAppLock; ++i)
	{
		Byte_t nTrackNo = static_cast<Byte_t>(ilkInfo.ApproachLock.ApproachTrack[i].AppTrack);
		if (nTrackNo == 0)
			break;

		CString strTrkName = CommonUtil::GetDBNameByNumber(nTrackNo, GetDBNameByNum::TrackIdx);
		strTemp.Format(_T(" %s( )"), (LPCTSTR)strTrkName);
		strLockList += strTemp;
	}

	return strLockList;
}

void TestPrintDlg::SetStationName(CString strStationName)
{
	m_strStationName = strStationName;
}

// [추가] 체크리스트(유효 진로) 또는 선로전환기 시험 항목이 하나라도 있는지 확인.
// 각종표시등 페이지는 데이터 유무와 무관하게 항상 고정 항목을 인쇄하므로 판단 기준에서 제외.
bool TestPrintDlg::HasPrintableData()
{
	auto routeSpan = StructMainData::GetInstance().GetRouteInfo();
	auto ilkSpan = StructMainData::GetInstance().GetInterLockInfo();
	auto stationInfo = StructMainData::GetInstance().GetStationInfo();

	for (size_t i = 0; i < routeSpan.size(); ++i)
	{
		WORD nRteNo = static_cast<WORD>(i);
		if (nRteNo >= ilkSpan.size()) continue;
		CString strRouteName(ilkSpan[nRteNo].Name);
		if (strRouteName.IsEmpty() || strRouteName.Find(_T('!')) != -1) continue;
		return true;
	}

	return stationInfo.NoOfSwitch > 0;
}
int TestPrintDlg::GetTotalPageCount(std::vector<size_t>& validIndices, int& maxDisplayRows, int rowHeight, const CRect& rectA4, int startY, int& outSwitchPages, int& outIndicatorPages)
{
	double scaleY = (double)rectA4.Height() / 210.0;
	int bottomMargin = (int)(15.0 * scaleY);

	// [1] 체크리스트 페이지 계산
	maxDisplayRows = (rectA4.bottom - bottomMargin - startY - (int)(8.0 * scaleY)) / rowHeight;
	if (maxDisplayRows < 1) maxDisplayRows = 1;
	auto stationInfo = StructMainData::GetInstance().GetStationInfo();
	auto routeSpan = StructMainData::GetInstance().GetRouteInfo();
	auto ilkSpan = StructMainData::GetInstance().GetInterLockInfo();

	validIndices.clear();
	for (size_t i = 0; i < routeSpan.size(); ++i)
	{
		WORD nRteNo = static_cast<WORD>(i);
		if (nRteNo >= ilkSpan.size()) continue;
		CString strRouteName(ilkSpan[nRteNo].Name);
		if (strRouteName.IsEmpty() || strRouteName.Find(_T('!')) != -1) continue;
		validIndices.push_back(i);
	}
	int dynamicChecklistPages = (int)((validIndices.size() + maxDisplayRows - 1) / maxDisplayRows);
	if (dynamicChecklistPages < 1) dynamicChecklistPages = 1;

	// [2] 선로전환기 페이지 계산 ()
	int maxSwitchesPerPage = 16;
	int totalSwitches = stationInfo.NoOfSwitch; // 실제 데이터로 변경
	outSwitchPages = (totalSwitches + maxSwitchesPerPage - 1) / maxSwitchesPerPage;
	if (outSwitchPages < 1) outSwitchPages = 1;

	// [3] 각종 표시등 페이지 계산 (조건문 포함된 실제 배열 기준)
	std::vector<CString> arrIndicators;
	arrIndicators.push_back(_T("RUN ( )"));
	if (stationInfo.StationInfo.IsRcCtrl & 0x01 || stationInfo.StationInfo.IsRcCtrl & 0x02) {
		arrIndicators.push_back(_T("RC ( )"));
	}
	arrIndicators.push_back(_T("CTC ( )"));
	arrIndicators.push_back(_T("LOCAL()"));
	arrIndicators.push_back(_T("1계 ( )"));
	arrIndicators.push_back(_T("2계 ( )"));
	arrIndicators.push_back(_T("N1 ( )"));
	arrIndicators.push_back(_T("N2 ( )"));
	arrIndicators.push_back(_T("COM1 ( )"));
	arrIndicators.push_back(_T("COM2 ( )"));
	arrIndicators.push_back(_T("UPS ( )"));
	arrIndicators.push_back(_T("출입문 ( )"));
	arrIndicators.push_back(_T("정류기 ( )"));
	arrIndicators.push_back(_T("입환 ( )"));
	arrIndicators.push_back(_T("FUSE  ( )"));
	arrIndicators.push_back(_T("축전지 ( )"));

	if (stationInfo.NoOfSignal > 0) arrIndicators.push_back(_T("S ( )"));
	if (stationInfo.NoOfSwitch > 0) arrIndicators.push_back(_T("P ( )"));
	arrIndicators.push_back(_T("T ( )"));
	if (stationInfo.NoOfSignal > 0) arrIndicators.push_back(_T("신호기 고장 BUZZER ( )"));
	if (stationInfo.NoOfSwitch > 0) arrIndicators.push_back(_T("선로전환기 고장 BUZZER ( )"));
	arrIndicators.push_back(_T("선로전환 고장 BUZZER ( )"));

	int totalIndicators = (int)arrIndicators.size();
	int indicatorRowHeight = (int)(12.0 * scaleY);
	int headerHeight = (int)(12.0 * scaleY);
	int availableHeight = rectA4.bottom - bottomMargin - startY - headerHeight;
	int maxIndicatorsPerPage = availableHeight / indicatorRowHeight;
	if (maxIndicatorsPerPage < 1) maxIndicatorsPerPage = 1;

	outIndicatorPages = (totalIndicators + maxIndicatorsPerPage - 1) / maxIndicatorsPerPage;
	if (outIndicatorPages < 1) outIndicatorPages = 1;

	// 총 페이지 = 표시등 페이지 수 + 선로전환기 페이지 수 + 체크리스트 페이지 수
	return outIndicatorPages + outSwitchPages + dynamicChecklistPages;
}

// -------------------------------------------------------------------------
// [1페이지] 각종표시등 동작상태
// -------------------------------------------------------------------------
void TestPrintDlg::DrawIndicatorTestPage(CDC* pDC, const CRect& rectA4, double scaleX, double scaleY, int startX, int startY, int totalWidth, CFont* pHeaderFont, CFont* pSmallFont, int nIndicatorSubPage)
{
	CFont* pOldFont = pDC->SelectObject(pHeaderFont);

	int rowHeight = (int)(12.0 * scaleY);
	int colW_Name = (int)(80.0 * scaleX);
	int colW_State = (int)(120.0 * scaleX);
	int colW_Drive = totalWidth - (colW_Name + colW_State);

	// 헤더 좌표 계산 및 그리기
	CRect rTitleName(startX, startY, startX + colW_Name, startY + rowHeight);
	CRect rTitleState(rTitleName.right, startY, rTitleName.right + colW_State, startY + rowHeight);
	CRect rTitleDrive(rTitleState.right, startY, startX + totalWidth, startY + rowHeight);

	pDC->Draw3dRect(&rTitleName, RGB(0, 0, 0), RGB(0, 0, 0));  pDC->DrawText(_T("표 시 등 명 칭"), &rTitleName, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->Draw3dRect(&rTitleState, RGB(0, 0, 0), RGB(0, 0, 0)); pDC->DrawText(_T("동 작 상 태"), &rTitleState, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->Draw3dRect(&rTitleDrive, RGB(0, 0, 0), RGB(0, 0, 0)); pDC->DrawText(_T("기    사"), &rTitleDrive, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	startY += rowHeight;
	pDC->SelectObject(pSmallFont);

	auto stationInfo = StructMainData::GetInstance().GetStationInfo();

	// 표시등 데이터 배열 구성
	std::vector<CString> arrIndicators;
	arrIndicators.push_back(_T("RUN (  )"));
	if (stationInfo.StationInfo.IsRcCtrl & 0x01 || stationInfo.StationInfo.IsRcCtrl & 0x02) {
		arrIndicators.push_back(_T("RC (  )"));
	}
	arrIndicators.push_back(_T("CTC (  )"));
	arrIndicators.push_back(_T("LOCAL(  )"));
	arrIndicators.push_back(_T("1계 (  )"));
	arrIndicators.push_back(_T("2계 (  )"));
	arrIndicators.push_back(_T("N1 (  )"));
	arrIndicators.push_back(_T("N2 (  )"));
	arrIndicators.push_back(_T("COM1 (  )"));
	arrIndicators.push_back(_T("COM2 (  )"));
	arrIndicators.push_back(_T("UPS (  )"));
	arrIndicators.push_back(_T("출입문 (  )"));
	arrIndicators.push_back(_T("정류기 (  )"));
	arrIndicators.push_back(_T("입환 (  )"));
	arrIndicators.push_back(_T("FUSE  (  )"));
	arrIndicators.push_back(_T("축전지 (  )"));

	if (stationInfo.NoOfSignal > 0) arrIndicators.push_back(_T("S (  )"));
	if (stationInfo.NoOfSwitch > 0) arrIndicators.push_back(_T("P (  )"));
	arrIndicators.push_back(_T("T (  )"));
	if (stationInfo.NoOfSignal > 0) arrIndicators.push_back(_T("신호기 고장 BUZZER (  )"));
	if (stationInfo.NoOfSwitch > 0) arrIndicators.push_back(_T("선로전환기 고장 BUZZER (  )"));
	arrIndicators.push_back(_T("선로전환 고장 BUZZER (  )"));

	// 푸터 공간(약 15mm)을 고려하여 한 페이지에 들어갈 수 있는 최대 행 개수 재계산
	int bottomMargin = (int)(20.0 * scaleY);
	int availableHeight = rectA4.bottom - bottomMargin - startY;
	int maxIndicatorsPerPage = availableHeight / rowHeight;
	if (maxIndicatorsPerPage < 1) maxIndicatorsPerPage = 1;

	// ★ 서브 페이지에 따른 데이터 슬라이싱 (1페이지당 개수 제한)
	int startIndex = (nIndicatorSubPage - 1) * maxIndicatorsPerPage;
	int endIndex = min(startIndex + maxIndicatorsPerPage, (int)arrIndicators.size());
	int displayCount = endIndex - startIndex;

	if (displayCount <= 0) return;

	for (int i = 0; i < displayCount; ++i)
	{
		int currentY = startY + (i * rowHeight);
		int dataIndex = startIndex + i; // 실제 데이터 인덱스 매핑

		CRect rNameRow(startX, currentY, startX + colW_Name, currentY + rowHeight);
		CRect rStateRow(rNameRow.right, currentY, rNameRow.right + colW_State, currentY + rowHeight);
		CRect rDriveRow(rStateRow.right, currentY, startX + totalWidth, currentY + rowHeight);

		pDC->Draw3dRect(&rNameRow, RGB(0, 0, 0), RGB(0, 0, 0));
		pDC->Draw3dRect(&rStateRow, RGB(0, 0, 0), RGB(0, 0, 0));
		pDC->Draw3dRect(&rDriveRow, RGB(0, 0, 0), RGB(0, 0, 0));

		pDC->DrawText(arrIndicators[dataIndex], &rNameRow, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		pDC->DrawText(_T("정 상 동 작 (   ) / 기 능 불 량 (   ) / 시 험 보 류 (   )"), &rStateRow, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		pDC->DrawText(_T(""), &rDriveRow, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	pDC->SelectObject(pOldFont);
}

// -------------------------------------------------------------------------
// [2페이지 ~ ] 선로전환기 단동시험 (페이징 적용)
// -------------------------------------------------------------------------
void TestPrintDlg::DrawSwitchTestPage(CDC* pDC, const CRect& rectA4, double scaleX, double scaleY, int startX, int startY, int totalWidth, CFont* pHeaderFont, CFont* pSmallFont, int nSwitchSubPage)
{
	CFont* pOldFont = pDC->SelectObject(pHeaderFont);
	auto stationInfo = StructMainData::GetInstance().GetStationInfo();
	auto switchSpan = StructMainData::GetInstance().GetSwitchInfo();
	auto trackInfo = StructMainData::GetInstance().GetTrackInfo();
	int wKind = (int)(45.0 * scaleX);
	int wChk = (int)(100.0 * scaleX);
	int wLock = (int)(100.0 * scaleX);
	int wState = totalWidth - (wKind + wChk + wLock);
	int rowHeight = (int)(9.0 * scaleY);

	// 테이블 헤더 출력
	CRect rTitleKind(startX, startY, startX + wKind, startY + rowHeight);
	CRect rTitleChk(rTitleKind.right, startY, rTitleKind.right + wChk, startY + rowHeight);
	CRect rTitleLock(rTitleChk.right, startY, rTitleChk.right + wLock, startY + rowHeight);
	CRect rTitleState(rTitleLock.right, startY, startX + totalWidth, startY + rowHeight);

	pDC->Draw3dRect(&rTitleKind, RGB(0, 0, 0), RGB(0, 0, 0));  pDC->DrawText(_T("구     분"), &rTitleKind, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->Draw3dRect(&rTitleChk, RGB(0, 0, 0), RGB(0, 0, 0));   pDC->DrawText(_T("명 칭 별 기 능 상 태"), &rTitleChk, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->Draw3dRect(&rTitleLock, RGB(0, 0, 0), RGB(0, 0, 0));  pDC->DrawText(_T("철 사 쇄 정"), &rTitleLock, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->Draw3dRect(&rTitleState, RGB(0, 0, 0), RGB(0, 0, 0)); pDC->DrawText(_T("기    사"), &rTitleState, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	startY += rowHeight;

	// 페이징 연산 (실제 선로전환기 총 개수 연동)
	int maxSwitchesPerPage = 16;
	int totalSwitches = stationInfo.NoOfSwitch;

	int startIndex = (nSwitchSubPage - 1) * maxSwitchesPerPage;
	int endIndex = min(startIndex + maxSwitchesPerPage, totalSwitches);
	int displayCount = endIndex - startIndex;

	if (displayCount <= 0) return;

	int dataTotalHeight = rowHeight * displayCount;
	CRect rDataKind(startX, startY, startX + wKind, startY + dataTotalHeight);
	pDC->Draw3dRect(&rDataKind, RGB(0, 0, 0), RGB(0, 0, 0));

	//개행(공백) 없이 원본 텍스트 정의
	CString strKindText = _T("선로전환기\n단동시험");

	//DT_CALCRECT를 이용해 두 줄 텍스트가 차지하는 실제 높이 계산
	CRect rMeasure = rDataKind;
	pDC->DrawText(strKindText, &rMeasure, DT_CENTER | DT_WORDBREAK | DT_CALCRECT);

	// 전체 박스 높이에서 텍스트 높이를 뺀 후 2로 나누어 상단 여백(offset) 산출
	int textHeight = rMeasure.Height();
	int offsetY = (rDataKind.Height() - textHeight) / 2;

	//계산된 오프셋을 반영한 위치에 텍스트 출력
	CRect rDrawText = rDataKind;
	rDrawText.top += offsetY;
	pDC->DrawText(strKindText, &rDrawText, DT_CENTER | DT_WORDBREAK);
	pDC->SelectObject(pSmallFont);

	for (int i = 0; i < displayCount; ++i)
	{
		int nSwhNo = startIndex + i + 1; // 1번부터 시작
		if (nSwhNo > stationInfo.NoOfSwitch)
			break;

		int currentY = startY + (i * rowHeight);

		CRect rChkRow(rDataKind.right, currentY, rDataKind.right + wChk, currentY + rowHeight);
		CRect rLockRow(rChkRow.right, currentY, rChkRow.right + wLock, currentY + rowHeight);
		CRect rStateRow(rLockRow.right, currentY, startX + totalWidth, currentY + rowHeight);

		pDC->Draw3dRect(&rChkRow, RGB(0, 0, 0), RGB(0, 0, 0));
		pDC->Draw3dRect(&rLockRow, RGB(0, 0, 0), RGB(0, 0, 0));
		pDC->Draw3dRect(&rStateRow, RGB(0, 0, 0), RGB(0, 0, 0));

		// 1. [명칭별 기능상태 N/R 출력]
		CRect rN = rChkRow; rN.right = rChkRow.left + (rChkRow.Width() / 2);
		CRect rR = rChkRow; rR.left = rN.right;

		CString strSwhName(switchSpan[nSwhNo].Name);
		CString strTextN, strTextR;
		strTextN.Format(_T("%s  N (   )"), strSwhName.GetString());
		strTextR.Format(_T("%s  R (   )"), strSwhName.GetString());

		pDC->DrawText(strTextN, &rN, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		pDC->DrawText(strTextR, &rR, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		// 2. [철사쇄정 궤도 정보 출력 (A궤도 / B궤도)]
		CRect rLockA = rLockRow; rLockA.right = rLockRow.left + (rLockRow.Width() / 2);
		CRect rLockB = rLockRow; rLockB.left = rLockA.right;

		if (switchSpan[nSwhNo].SwhTrack[0] > 0)
		{
			CString strTrkAName(trackInfo[switchSpan[nSwhNo].SwhTrack[0]].Name);
			CString strTextTrkA;
			strTextTrkA.Format(_T("%s (   )"), strTrkAName.GetString());
			pDC->DrawText(strTextTrkA, &rLockA, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}

		if (switchSpan[nSwhNo].SwhTrack[1] > 0)
		{
			CString strTrkBName(trackInfo[switchSpan[nSwhNo].SwhTrack[1]].Name);
			CString strTextTrkB;
			strTextTrkB.Format(_T("%s (   )"), strTrkBName.GetString());
			pDC->DrawText(strTextTrkB, &rLockB, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}

	pDC->SelectObject(pOldFont);
}



int TestPrintDlg::DrawPageHeader(CDC* pDC, const CRect& rectA4, double scaleX, double scaleY, CFont* pTitleFont, CFont* pHeaderFont, const CString& strDocTitle, int nCurPage, int nTotalPages)
{
	int startX = rectA4.left + (int)(10.0 * scaleX);
	int startY = rectA4.top + (int)(10.0 * scaleY);
	int totalWidth = (int)(277.0 * scaleX);

	CFont* pOldFont = pDC->SelectObject(pTitleFont);

	// 상단 박스 (회사명, 문서제목)
	CRect rectTopBox(startX, startY, startX + totalWidth, startY + (int)(16.0 * scaleY));
	pDC->Draw3dRect(&rectTopBox, RGB(0, 0, 0), RGB(0, 0, 0));

	CRect rectCompany(rectTopBox.left, rectTopBox.top, rectTopBox.left + (int)(55.0 * scaleX), rectTopBox.bottom);
	CRect rectDocTitle(rectCompany.right, rectTopBox.top, rectTopBox.right, rectTopBox.bottom);

	pDC->Draw3dRect(&rectCompany, RGB(0, 0, 0), RGB(0, 0, 0));
	pDC->DrawText(_T("유 경 제 어 (주)"), &rectCompany, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->DrawText(strDocTitle, &rectDocTitle, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// 메타 박스 (역명, 일시, 시간, 페이지 등)
	startY = rectTopBox.bottom;
	int metaHeight = (int)(9.0 * scaleY);
	CRect rectMeta(startX, startY, startX + totalWidth, startY + metaHeight);
	pDC->Draw3dRect(&rectMeta, RGB(0, 0, 0), RGB(0, 0, 0));

	pDC->SelectObject(pHeaderFont);
	int w1 = (int)(15.0 * scaleX); int w2 = (int)(30.0 * scaleX); int w7 = (int)(40.0 * scaleX);
	int w3 = (int)(22.0 * scaleX); int w4 = (int)(45.0 * scaleX);
	int w5 = (int)(22.0 * scaleX); int w6 = (int)(45.0 * scaleX);

	CRect rSNameLbl(rectMeta.left, rectMeta.top, rectMeta.left + w1, rectMeta.bottom);
	CRect rSNameVal(rSNameLbl.right, rectMeta.top, rSNameLbl.right + w2, rectMeta.bottom);
	CRect rDateLbl(rSNameVal.right, rectMeta.top, rSNameVal.right + w3, rectMeta.bottom);
	CRect rDateVal(rDateLbl.right, rectMeta.top, rDateLbl.right + w4, rectMeta.bottom);
	CRect rTimeLbl(rDateVal.right, rectMeta.top, rDateVal.right + w5, rectMeta.bottom);
	CRect rTimeVal(rTimeLbl.right, rectMeta.top, rTimeLbl.right + w6, rectMeta.bottom);

	// [추가됨] '시험시간' 값 우측부터 메타박스 끝까지를 페이지 영역으로 할당
	CRect rPageLbl(rTimeVal.right, rectMeta.top, rTimeVal.right + w7, rectMeta.bottom);
	CRect rPageVal(rPageLbl.right, rectMeta.top, rectMeta.right, rectMeta.bottom); // 남은 공간 전체 할당

	pDC->Draw3dRect(&rSNameLbl, RGB(0, 0, 0), RGB(0, 0, 0)); pDC->DrawText(_T("역 명"), &rSNameLbl, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->Draw3dRect(&rSNameVal, RGB(0, 0, 0), RGB(0, 0, 0)); pDC->DrawText(m_strStationName, &rSNameVal, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->Draw3dRect(&rDateLbl, RGB(0, 0, 0), RGB(0, 0, 0)); pDC->DrawText(_T("시 험 일 시"), &rDateLbl, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->Draw3dRect(&rDateVal, RGB(0, 0, 0), RGB(0, 0, 0)); pDC->DrawText(_T(""), &rDateVal, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->Draw3dRect(&rTimeLbl, RGB(0, 0, 0), RGB(0, 0, 0)); pDC->DrawText(_T("시 험 시 간"), &rTimeLbl, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->Draw3dRect(&rTimeVal, RGB(0, 0, 0), RGB(0, 0, 0)); pDC->DrawText(_T(""), &rTimeVal, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// [추가됨] 페이지 문자열 포맷팅 및 출력
	CString strPageText;
	strPageText.Format(_T("%d"), nCurPage);
	pDC->Draw3dRect(&rPageVal, RGB(0, 0, 0), RGB(0, 0, 0));
	pDC->DrawText(strPageText, &rPageVal, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	pDC->Draw3dRect(&rPageLbl, RGB(0, 0, 0), RGB(0, 0, 0)); pDC->DrawText(_T("페이지"), &rPageLbl, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	pDC->SelectObject(pOldFont);
	return rectMeta.bottom;
}
void TestPrintDlg::DrawPageFooter(CDC* pDC, const CRect& rectA4, double scaleX, double scaleY, CFont* pHeaderFont, int nCurPage, int nTotalPages)
{
	CFont* pOldFont = pDC->SelectObject(pHeaderFont);
	CRect rectFooter(rectA4.left, rectA4.bottom - (int)(12.0 * scaleY), rectA4.right, rectA4.bottom - (int)(2.0 * scaleY));

	CString strFooterText;
	strFooterText.Format(_T("- %d / %d 페이지 -"), nCurPage, nTotalPages);
	pDC->DrawText(strFooterText, &rectFooter, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	pDC->SelectObject(pOldFont);
}


// -------------------------------------------------------------------------
// [페이지 2~] 체크리스트 동적 표출
// -------------------------------------------------------------------------
void TestPrintDlg::DrawChecklistPage(CDC* pDC, const CRect& rectA4, double scaleX, double scaleY, int startX, int startY, int totalWidth, CFont* pHeaderFont, CFont* pSmallFont, const std::vector<size_t>& validIndices, int maxDisplayRows, int nChecklistSubPage)
{
	CFont* pOldFont = pDC->SelectObject(pHeaderFont);

	// 1. 체크리스트 헤더
	int headerHeight = (int)(8.0 * scaleY);
	CRect rectTblHead(startX, startY, startX + totalWidth, startY + headerHeight);
	pDC->Draw3dRect(&rectTblHead, RGB(0, 0, 0), RGB(0, 0, 0));

	int colW_Signal = (int)(20.0 * scaleX);
	int colW_Route = (int)(30.0 * scaleX);
	int colW_Cond = (int)(160.0 * scaleX);
	int colW_Status = (int)(72.0 * 0.6 * scaleX);

	CRect rH1(rectTblHead.left, rectTblHead.top, rectTblHead.left + colW_Signal, rectTblHead.bottom);
	CRect rH2(rH1.right, rectTblHead.top, rH1.right + colW_Route, rectTblHead.bottom);
	CRect rH3(rH2.right, rectTblHead.top, rH2.right + colW_Cond + colW_Status, rectTblHead.bottom);
	CRect rH4(rH3.right, rectTblHead.top, rectTblHead.right, rectTblHead.bottom);

	pDC->Draw3dRect(&rH1, RGB(0, 0, 0), RGB(0, 0, 0)); pDC->DrawText(_T("신호기"), &rH1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->Draw3dRect(&rH2, RGB(0, 0, 0), RGB(0, 0, 0)); pDC->DrawText(_T("진로"), &rH2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->Draw3dRect(&rH3, RGB(0, 0, 0), RGB(0, 0, 0)); pDC->DrawText(_T("체 크 항 목"), &rH3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->Draw3dRect(&rH4, RGB(0, 0, 0), RGB(0, 0, 0)); pDC->DrawText(_T("기 사(서명)"), &rH4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	pDC->SelectObject(pSmallFont);

	int currentY = rectTblHead.bottom;

	// [수정] 각 서브 항목별 개별 높이 지정 (신호제어 h3만 높게 설정)
	int h1 = (int)(8.5 * scaleY); // 상호쇄정
	int h2 = (int)(8.5 * scaleY); // 진로쇄정
	int h3 = (int)(14.0 * scaleY); // 신호제어 (다른 항목보다 2배 큼)
	int h4 = (int)(8.5 * scaleY); // 진로구분
	int h5 = (int)(8. * scaleY); // 접근보류
	int rowHeight = h1 + h2 + h3 + h4 + h5; // 총합 높이

	// 그리기 루프에서 사용할 높이 배열 세팅
	int subHeights[5] = { h1, h2, h3, h4, h5 };

	auto routeSpan = StructMainData::GetInstance().GetRouteInfo();
	auto ilkSpan = StructMainData::GetInstance().GetInterLockInfo();
	auto sigSpan = StructMainData::GetInstance().GetSignalInfo();

	// 2. 인덱스 슬라이싱 연산 (nChecklistSubPage 는 1부터 시작)
	size_t startIndex = (size_t)(nChecklistSubPage - 1) * maxDisplayRows;
	size_t endIndex = min(startIndex + maxDisplayRows, validIndices.size());

	for (size_t idx = startIndex; idx < endIndex; ++idx)
	{
		size_t i = validIndices[idx];
		WORD nRteNo = static_cast<WORD>(i);
		const auto& routeItem = routeSpan[nRteNo];
		CString strRouteName(ilkSpan[nRteNo].Name);

		WORD nDepTrk = ilkSpan[nRteNo].RouteInfo.DepartureT;
		WORD nArrTrk = ilkSpan[nRteNo].RouteInfo.ArrivalT;
		WORD nSigNo = ilkSpan[nRteNo].RouteInfo.SignalNo;

		// 경로 파싱
		CString strTrackDep;
		if (nDepTrk > 0) strTrackDep = CommonUtil::GetDBNameByNumber(nDepTrk, GetDBNameByNum::TrackIdx);
		else strTrackDep = CommonUtil::GetDBNameByNumber(nSigNo, GetDBNameByNum::SignalIdx);

		CString strTrackArr;
		if (nArrTrk > 0) strTrackArr = CommonUtil::GetDBNameByNumber(nArrTrk, GetDBNameByNum::TrackIdx);
		else {
			int nIdxFind = strRouteName.Find(_T('-'));
			if (nIdxFind > -1 && nIdxFind + 1 < strRouteName.GetLength()) strTrackArr = strRouteName.Mid(nIdxFind + 1);
			else strTrackArr = strRouteName;
		}

		CString strRouteInfoText;
		if (!strTrackDep.IsEmpty() && !strTrackArr.IsEmpty()) strRouteInfoText.Format(_T("%s -> %s"), (LPCTSTR)strTrackDep, (LPCTSTR)strTrackArr);
		else strRouteInfoText = strRouteName;

		// 항목 문자열 추출
		CString strInterlock = GetSignalLock(nRteNo); // 상호
		CString strSwitchLock = GetSwitchLock(nRteNo, true); // 진로

		// [수정] 신호제어 문자열 결합 처리 방식 개선
		CString strSigCond = GetTrackLockAll(nRteNo, 0); // 신호
		if (!strSigCond.IsEmpty()) {
			strSigCond += _T("\r\n"); // 단순 \r 대신 줄바꿈으로 연결
		}
		strSigCond += GetSwitchLock(nRteNo, true); // 신호 + 전환기

		CString strRouteDiv = GetTrackLockRoute(nRteNo); // 진로 구분
		CString strAppLock = GetAppLock(nRteNo); // 접근 및 보류

		// 외곽선 및 C1, C2
		CRect rectRow(startX, currentY, startX + totalWidth, currentY + rowHeight);
		pDC->Draw3dRect(&rectRow, RGB(0, 0, 0), RGB(0, 0, 0));

		CRect rC1(rectRow.left, rectRow.top, rectRow.left + colW_Signal, rectRow.bottom);
		CRect rC2(rC1.right, rectRow.top, rC1.right + colW_Route, rectRow.bottom);
		pDC->Draw3dRect(&rC1, RGB(0, 0, 0), RGB(0, 0, 0));
		pDC->Draw3dRect(&rC2, RGB(0, 0, 0), RGB(0, 0, 0));

		// 비트 판별 (장내/출발/입환/폐색)
		bool bIsArrive = IsBitSet(routeItem.Kind.Arrive, 0);
		bool bIsDepart = IsBitSet(routeItem.Kind.Depart, 0);
		bool bIsShunt = IsBitSet(routeItem.Kind.Shunt, 0);
		bool bIsCallOn = IsBitSet(routeItem.RouteFlag.UdoSig, 0);
		bool bIsHomeBlock = IsBitSet(routeItem.Kind.IsBlock, 0);
		bool bIsShuntSig = (nSigNo > 0);

		CString strText;
		if (bIsArrive) strText = _T("\n\n장\n\n내\n\n신\n\n호");
		else if (bIsDepart) strText = _T("\n\n출\n\n발\n\n신\n\n호");
		else if (bIsShunt) {
			if (bIsShuntSig) strText = bIsCallOn ? _T("\n\n입\n\n환\n\n신\n\n호") : _T("\n\n입\n\n환\n\n표\n\n지");
			else strText = _T("\n\n입\n\n환\n\n표\n\n지");
		}
		else {
			strText = bIsHomeBlock ? _T("\n\n구\n\n내\n\n폐\n\n색") : _T("\n\n신\n\n호\n\n진\n\n로");
		}

		CRect rC1_text = rC1; rC1_text.DeflateRect(2, 2);
		pDC->DrawText(strText, &rC1_text, DT_CENTER | DT_VCENTER);

		CRect rcRouteText = rC2; rcRouteText.bottom = rcRouteText.top + (rowHeight / 4);
		pDC->DrawText(strRouteName, &rcRouteText, DT_CENTER | DT_SINGLELINE | DT_BOTTOM);

		CRect rcTrackText = rC2; rcTrackText.top = rectRow.top + (rowHeight / 2); rcTrackText.bottom = rcTrackText.top + (rowHeight / 2);
		pDC->DrawText(strRouteInfoText, &rcTrackText, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

		// [수정] 우측 세부 항목 (균등분할 폐기, 배열에 따른 개별 높이 적용)
		int subX = rC2.right;
		CString subLabels[5] = { _T("상호쇄정"), _T("진로쇄정"), _T("신호제어"), _T("진로구분쇄정"), _T("접근 및 보류쇄정") };
		CString subValues[5] = { strInterlock, strSwitchLock, strSigCond, strRouteDiv, strAppLock };
		int colW_Label = (int)(35.0 * scaleX);

		int currentSubY = rectRow.top; // 각 행의 시작 Y좌표

		for (int s = 0; s < 5; ++s)
		{
			int currentH = subHeights[s]; // 배열에서 현재 항목의 높이를 가져옴
			int bottomY = currentSubY + currentH;

			// 만약 마지막 루프라면 부모 사각형의 bottom에 꽉 맞추어 오차 보정
			if (s == 4) {
				bottomY = rectRow.bottom;
			}

			CRect rLabel(subX, currentSubY, subX + colW_Label, bottomY);
			CRect rData(rLabel.right, currentSubY, subX + colW_Cond, bottomY);

			pDC->Draw3dRect(&rLabel, RGB(0, 0, 0), RGB(0, 0, 0));
			pDC->Draw3dRect(&rData, RGB(0, 0, 0), RGB(0, 0, 0));

			// 라벨은 중앙 정렬 
			pDC->DrawText(subLabels[s], &rLabel, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			// 데이터 텍스트 그리기 설정
			CRect rDataText = rData;
			rDataText.DeflateRect(4, 2);

			// [수정] DT_TOP 적용, DT_EDITCONTROL 및 DT_END_ELLIPSIS 추가하여 테두리 이탈 완전 방지
			pDC->DrawText(subValues[s], &rDataText, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_EDITCONTROL | DT_END_ELLIPSIS);

			// 다음 항목을 그리기 위해 Y좌표 이동
			currentSubY = bottomY;
		}

		// 상태 (출발반응등, 진로선별등, ATS 등)
		int statusX = subX + colW_Cond;
		std::vector<CString> statusList;
		statusList.push_back(_T("조작반표시상태\n(신호 및 진로) (  )"));

		int nSigIdx = (int)nSigNo - 1;
		bool bIsHomeBlockSig = false;
		if (nSigIdx >= 0 && nSigIdx < sigSpan.size())
		{
			bIsHomeBlockSig = ((sigSpan[nSigIdx].Kind.HomeBlockS & 0x01) == 0x01);
		}

		if (!bIsHomeBlockSig) statusList.push_back(bIsDepart ? _T("출발반응등\n(  )") : _T("진로선별등\n(  )"));
		if (!bIsShunt) statusList.push_back(_T("ATS\n(  )"));

		size_t statusItemsCount = max(statusList.size(), (size_t)1);
		int statusHeight = rowHeight / (int)statusItemsCount;

		for (size_t st = 0; st < statusList.size(); ++st)
		{
			int topY = rectRow.top + (int)(st * statusHeight);
			int bottomY = (st == statusList.size() - 1) ? rectRow.bottom : (rectRow.top + (int)((st + 1) * statusHeight));

			CRect rSp(statusX, topY, statusX + colW_Status, bottomY);
			pDC->Draw3dRect(&rSp, RGB(0, 0, 0), RGB(0, 0, 0));

			CRect rSp_t = rSp; rSp_t.DeflateRect(2, 2);
			CRect rcCalc = rSp_t;
			pDC->DrawText(statusList[st], &rcCalc, DT_CENTER | DT_WORDBREAK | DT_CALCRECT);
			if (rSp_t.Height() > rcCalc.Height()) rSp_t.top += (rSp_t.Height() - rcCalc.Height()) / 2;
			pDC->DrawText(statusList[st], &rSp_t, DT_CENTER | DT_WORDBREAK);
		}

		// 기사 서명
		CRect rCheckCol(statusX + colW_Status, rectRow.top, rectRow.right, rectRow.bottom);
		pDC->Draw3dRect(&rCheckCol, RGB(0, 0, 0), RGB(0, 0, 0));
		pDC->DrawText(_T(""), &rCheckCol, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		currentY += rowHeight;
	}

	pDC->SelectObject(pOldFont);
}