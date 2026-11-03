#include "stdafx.h" // Or pch.h depending on your project setup
#include "resource.h"
#include "TagNamePrintDlg.h"
#include <algorithm>

// Note: Ensure IDD_TAGNAME_DLG, IDC_STATIC_PREVIEW, and IDC_BTN_PRINT 
// are defined in your Resource.h with correct resource IDs.


IMPLEMENT_DYNAMIC(TagNamePrintDlg, CBCGPDialog)

TagNamePrintDlg::TagNamePrintDlg(CWnd* pParent /*=nullptr*/)
	: CBCGPDialog(IDD_TAGNAME_DLG, pParent)
	, m_cellWidthMm(8)  // Default width (1.0cm ~ 3.0cm)
	, m_cellHeightMm(4) // Default height (0.5cm ~ 2.0cm)
	, m_nCurrentPage(0)
	, m_cellSpacingYMm(0.5)
	, m_isOutCard(false)
	, m_nPortsPerCard(16)
	, m_strCardLabel(_T("In"))
	, m_strListTitle(_T("IN Card List"))
{
	EnableVisualManagerStyle(TRUE, TRUE);
}

TagNamePrintDlg::~TagNamePrintDlg()
{
}

void TagNamePrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(TagNamePrintDlg, CBCGPDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_PRINT, &TagNamePrintDlg::OnBnClickedBtnPrint)
	ON_BN_CLICKED(IDC_CELL_CHANGE_BUTTON, &TagNamePrintDlg::OnBnClickedCellChangeButton)
	ON_BN_CLICKED(IDC_BTN_PREV_PAGE, &TagNamePrintDlg::OnBnClickedBtnPrevPage)
	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, &TagNamePrintDlg::OnBnClickedBtnNextPage)
END_MESSAGE_MAP()

//void TagNamePrintDlg::OnPaint()
//{
//	CPaintDC dc(this);
//
//	CWnd* pPreviewStatic = GetDlgItem(IDC_STATIC_PREVIEW);
//	if (pPreviewStatic != nullptr)
//	{
//		CRect rectPreview;
//		pPreviewStatic->GetWindowRect(&rectPreview);
//		ScreenToClient(&rectPreview);
//
//		// 1. 회색 배경 채우기
//		dc.FillSolidRect(&rectPreview, RGB(200, 200, 200));
//
//		// 회색 창 안쪽에 여유 있는 여백(Padding) 공간 확보
//		CRect rectInner = rectPreview;
//		rectInner.DeflateRect(25, 25);
//
//		// 2. A4 가로(Landscape) 방향 흰색 용지 배경 그리기 (정확한 비율: 297mm / 210mm)
//		CRect rectA4 = rectInner;
//		double a4Ratio = 297.0 / 210.0;
//
//		if ((double)rectInner.Width() / rectInner.Height() > a4Ratio)
//		{
//			int targetW = (int)(rectInner.Height() * a4Ratio);
//			int offsetX = (rectInner.Width() - targetW) / 2;
//			rectA4 = CRect(rectInner.left + offsetX, rectInner.top, rectInner.left + offsetX + targetW, rectInner.bottom);
//		}
//		else
//		{
//			int targetH = (int)(rectInner.Width() / a4Ratio);
//			int offsetY = (rectInner.Height() - targetH) / 2;
//			rectA4 = CRect(rectInner.left, rectInner.top + offsetY, rectInner.right, rectInner.top + offsetY + targetH);
//		}
//
//		dc.FillSolidRect(&rectA4, RGB(255, 255, 255));
//		dc.Draw3dRect(&rectA4, RGB(100, 100, 100), RGB(50, 50, 50));
//
//		// 3. A4 용지 영역 안쪽으로 클리핑 영역 설정
//		int nSavedDC = dc.SaveDC();
//		dc.IntersectClipRect(&rectA4);
//
//		if (!tagName.empty())
//		{
//			// 중복 선언 방지를 위해 상단에서 카드 및 포트 규격 통합 선언
//			int maxCardsPerPage = 20; // 5번 조건: 20장마다 새로운 페이지
//			int maxPortsPerCard = 16; // 7번 조건: 카드 1장당 16개의 포트 (MAX_IO_CARD_PORT)
//
//			int totalPorts = (int)tagName.size();
//			int totalCards = (totalPorts + maxPortsPerCard - 1) / maxPortsPerCard;
//			int totalPages = (totalCards + maxCardsPerPage - 1) / maxCardsPerPage;
//			if (totalPages == 0) totalPages = 1;
//
//			CFont titleFont;
//			titleFont.CreatePointFont(65, _T("맑은 고딕"));
//			CFont* pOldFont = dc.SelectObject(&titleFont);
//			dc.SetBkMode(TRANSPARENT);
//			COLORREF oldTextColor = dc.SetTextColor(RGB(0, 0, 0));
//
//			CString titlePrefix = m_isOutCard ? _T("[ [측면입구역] * OUT Card List * ]") : _T("[ [측면입구역] * IN Card List * ]");
//			// 제목 영역 여백 축소
//			CRect rectTitle(rectA4.left + 15, rectA4.top + 5, rectA4.right - 15, rectA4.top + 20);
//			dc.DrawText(titlePrefix, &rectTitle, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
//
//			// 우측 상단에 현재 페이지 정보 출력
//			CString pageStr;
//			pageStr.Format(_T("미리보기 페이지 : %d / %d"), m_nCurrentPage + 1, totalPages);
//			CRect rectPage(rectA4.right - 250, rectA4.top + 5, rectA4.right - 15, rectA4.top + 20);
//			dc.DrawText(pageStr, &rectPage, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
//
//			dc.SelectObject(pOldFont);
//			titleFont.DeleteObject();
//
//			// A4 가로 규격(297mm x 210mm) 기준 스케일 계산
//			double scaleX = (double)rectA4.Width() / 297.0;
//			double scaleY = (double)rectA4.Height() / 210.0;
//
//			// mm 단위를 스케일에 직접 반영 (사용자 입력 세로 간격 m_cellSpacingYMm 적용)
//			int nLabelWidth = (int)(m_cellWidthMm * scaleX);
//			int nLabelHeight = (int)(m_cellHeightMm * scaleY);
//			int nSpacingX = (int)(0.5 * scaleX);
//			int nSpacingY = (int)(m_cellSpacingYMm * scaleY);
//
//			int contentLeft = rectA4.left + (int)(15.0 * scaleX);
//
//			CFont font;
//			font.CreatePointFont(35, _T("맑은 고딕"));
//			pOldFont = dc.SelectObject(&font);
//			oldTextColor = dc.SetTextColor(RGB(255, 255, 255));
//
//			// 상단 블록 시작 위치를 위로 당김 (18mm)
//			int topSectionY = rectA4.top + (int)(18.0 * scaleY);
//
//			// 미리보기 화면 페이징 적용 (현재 페이지에 해당하는 카드 인덱스 도출)
//			int startCardIndex = m_nCurrentPage * maxCardsPerPage;
//			int cardsToDraw = min(maxCardsPerPage, totalCards - startCardIndex);
//
//			for (int c = 0; c < cardsToDraw; ++c)
//			{
//				int cardNum = startCardIndex + c; // 실제 데이터 인덱스 번호
//				int currentX = contentLeft + c * (nLabelWidth + nSpacingX); // 화면에 그려지는 X좌표 위치
//
//				int r = 0;
//				for (; r < maxPortsPerCard; ++r)
//				{
//					int currentY = topSectionY + r * (nLabelHeight + nSpacingY);
//					CRect rectLabel(currentX, currentY, currentX + nLabelWidth, currentY + nLabelHeight);
//
//					dc.FillSolidRect(&rectLabel, RGB(0, 0, 0));
//					dc.Draw3dRect(&rectLabel, RGB(50, 50, 50), RGB(200, 200, 200));
//
//					// 포트(태그) 이름 출력
//					int portIndex = cardNum * maxPortsPerCard + r;
//					if (portIndex < totalPorts)
//					{
//						dc.DrawText(tagName[portIndex], &rectLabel, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//					}
//				}
//				int currentY = topSectionY + r * (nLabelHeight + nSpacingY);
//
//				// 하단 레이블 영역을 정확히 반으로 나누기
//				int nHalfWidth = nLabelWidth / 2;
//				int nHeaderHeight = (int)(nLabelHeight * 0.6); //높이를 0.6배로 조절
//
//				// 2. nHeaderHeight를 적용하여 사각형 영역 설정
//				CRect rectHeader1(currentX, currentY, currentX + nHalfWidth, currentY + nHeaderHeight);
//				CRect rectHeader2(currentX + nHalfWidth, currentY, currentX + nLabelWidth, currentY + nHeaderHeight);
//
//				// 1. 일반 카드색과 동일하게 검은색 배경 및 테두리 설정
//				dc.FillSolidRect(&rectHeader1, RGB(0, 0, 0));
//				dc.Draw3dRect(&rectHeader1, RGB(50, 50, 50), RGB(200, 200, 200));
//
//				dc.FillSolidRect(&rectHeader2, RGB(0, 0, 0));
//				dc.Draw3dRect(&rectHeader2, RGB(50, 50, 50), RGB(200, 200, 200));
//
//				// 2. 글자색은 위(포트 출력부)에서 이미 흰색으로 지정되어 있으므로 
//				// SetTextColor를 다시 호출하지 않습니다. (색상 꼬임 및 1줄만 그려지는 버그 방지)
//
//				// 3. 텍스트 분리 출력 (첫 번째 칸: Out/In, 두 번째 칸: 카드 번호)
//				CString headerStr;
//				headerStr.Format(_T("%s %d"), m_isOutCard ? _T("Out") : _T("In"), cardNum + 1);
//				dc.DrawText(headerStr, &rectHeader1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//				dc.DrawText(headerStr, &rectHeader2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//
//			}
//
//			dc.SetTextColor(oldTextColor);
//			dc.SelectObject(pOldFont);
//			font.DeleteObject();
//		}
//
//		dc.RestoreDC(nSavedDC);
//	}
//}
//
//void TagNamePrintDlg::OnBnClickedBtnPrint()
//{
//	if (tagName.empty())
//	{
//		AfxMessageBox(_T("인쇄할 태그 이름 데이터가 없습니다."));
//		return;
//	}
//
//	CPrintDialog dlg(FALSE);
//
//	// 프린터 설정 기본값을 강제로 '가로(Landscape)' 방향으로 지정
//	if (AfxGetApp()->GetPrinterDeviceDefaults(&dlg.m_pd))
//	{
//		DEVMODE* pDevMode = (DEVMODE*)::GlobalLock(dlg.m_pd.hDevMode);
//		if (pDevMode != nullptr)
//		{
//			pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
//			::GlobalUnlock(dlg.m_pd.hDevMode);
//		}
//	}
//
//	if (dlg.DoModal() == IDOK)
//	{
//		HDC hDC = dlg.GetPrinterDC();
//		if (hDC == nullptr) return;
//
//		CDC dc;
//		dc.Attach(hDC);
//
//		DOCINFO di;
//		::ZeroMemory(&di, sizeof(DOCINFO));
//		di.cbSize = sizeof(DOCINFO);
//		di.lpszDocName = _T("표찰 이름 인쇄");
//
//		if (dc.StartDoc(&di) > 0)
//		{
//			int dpiX = dc.GetDeviceCaps(LOGPIXELSX);
//			int dpiY = dc.GetDeviceCaps(LOGPIXELSY);
//
//			// mm 단위를 프린터 픽셀로 변환 (m_cellSpacingYMm 적용)
//			int nLabelWidth = (int)((m_cellWidthMm / 25.4) * dpiX);
//			int nLabelHeight = (int)((m_cellHeightMm / 25.4) * dpiY);
//			int nSpacingX = (int)((0.5 / 25.4) * dpiX);
//			int nSpacingY = (int)((m_cellSpacingYMm / 25.4) * dpiY);
//			int nMarginX = (int)((10.0 / 25.4) * dpiX);
//			int nMarginY = (int)((10.0 / 25.4) * dpiY);
//
//			int maxCardsPerPage = 20; // 5번 조건: 20장마다 새로운 페이지
//			int maxPortsPerCard = 16; // 7번 조건: 카드 1장당 16개의 포트
//
//			int totalPorts = (int)tagName.size();
//			int totalCards = (totalPorts + maxPortsPerCard - 1) / maxPortsPerCard;
//			int currentCardIndex = 0; // 현재 인쇄할 카드의 시작 인덱스
//
//			CFont font;
//			font.CreatePointFont(42, _T("맑은 고딕"), &dc);
//			CFont titleFont;
//			titleFont.CreatePointFont(80, _T("맑은 고딕"), &dc);
//
//			// 카드 인덱스가 전체 카드를 모두 인쇄할 때까지 반복 (페이지 단위)
//			while (currentCardIndex < totalCards)
//			{
//				dc.StartPage();
//				dc.SetBkMode(TRANSPARENT);
//
//				dc.SelectObject(&titleFont);
//				COLORREF oldTextColor = dc.SetTextColor(RGB(0, 0, 0));
//				CString titlePrefix = m_isOutCard ? _T("[ * OUT Card List * ]") : _T("[ * IN Card List * ]");
//				CRect rectTitle(nMarginX, nMarginY, nMarginX + (int)(150.0 * dpiX / 25.4), nMarginY + (int)(8.0 * dpiY / 25.4));
//				dc.DrawText(titlePrefix, &rectTitle, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
//
//				dc.SelectObject(&font);
//
//				// 블록 시작 위치 조정
//				int topSectionY = nMarginY + (int)(12.0 * dpiY / 25.4);
//
//				// 이번 페이지에 인쇄할 카드 개수 계산 (남은 카드와 최대 허용치 비교)
//				int cardsInThisPage = min(maxCardsPerPage, totalCards - currentCardIndex);
//
//				for (int c = 0; c < cardsInThisPage; ++c)
//				{
//					int cardNum = currentCardIndex + c;
//					int currentX = nMarginX + c * (nLabelWidth + nSpacingX);
//
//					int r = 0;
//					dc.SetTextColor(RGB(255, 255, 255));
//
//					// 1장당 16개의 포트 순회 출력
//					for (; r < maxPortsPerCard; ++r)
//					{
//						int currentY = topSectionY + r * (nLabelHeight + nSpacingY);
//						CRect rectLabel(currentX, currentY, currentX + nLabelWidth, currentY + nLabelHeight);
//
//						dc.FillSolidRect(&rectLabel, RGB(0, 0, 0));
//						dc.Draw3dRect(&rectLabel, RGB(50, 50, 50), RGB(200, 200, 200));
//
//						int portIndex = cardNum * maxPortsPerCard + r;
//						if (portIndex < totalPorts)
//						{
//							dc.DrawText(tagName[portIndex], &rectLabel, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//						}
//					}
//
//					// 하단 레이블 (Out 21, In 22 등 다음 페이지에서도 번호가 이어서 출력되게 적용)
//					int currentY = topSectionY + r * (nLabelHeight + nSpacingY);
//					int nHalfWidth = nLabelWidth / 2;
//					int nHeaderHeight = (int)(nLabelHeight * 0.6); //높이를 0.6배로 조절
//
//					// 2. nHeaderHeight를 적용하여 사각형 영역 설정
//					CRect rectHeader1(currentX, currentY, currentX + nHalfWidth, currentY + nHeaderHeight);
//					CRect rectHeader2(currentX + nHalfWidth, currentY, currentX + nLabelWidth, currentY + nHeaderHeight);
//
//					// 1. 일반 카드색과 동일하게 검은색 배경 및 테두리 설정
//					dc.FillSolidRect(&rectHeader1, RGB(0, 0, 0));
//					dc.Draw3dRect(&rectHeader1, RGB(50, 50, 50), RGB(200, 200, 200));
//
//					dc.FillSolidRect(&rectHeader2, RGB(0, 0, 0));
//					dc.Draw3dRect(&rectHeader2, RGB(50, 50, 50), RGB(200, 200, 200));
//
//					// 2. 글자색은 위(포트 출력부)에서 이미 흰색으로 지정되어 있으므로 
//					// SetTextColor를 다시 호출하지 않습니다. (색상 꼬임 및 1줄만 그려지는 버그 방지)
//
//					// 3. 텍스트 분리 출력 (첫 번째 칸: Out/In, 두 번째 칸: 카드 번호)
//					CString headerStr;
//					headerStr.Format(_T("%s %d"), m_isOutCard ? _T("Out") : _T("In"), cardNum + 1);
//					dc.DrawText(headerStr, &rectHeader1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//					dc.DrawText(headerStr, &rectHeader2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//				}
//
//				dc.SetTextColor(oldTextColor);
//				dc.EndPage();
//
//				// 다음 페이지 인쇄를 위해 인덱스 증가
//				currentCardIndex += cardsInThisPage;
//			}
//
//			titleFont.DeleteObject();
//			font.DeleteObject();
//			dc.EndDoc();
//		}
//		dc.Detach();
//		::DeleteDC(hDC);
//
//		AfxMessageBox(_T("표찰 인쇄 작업이 완료되었습니다."));
//	}
//}
// [추가] 카드(모듈) 수 / 시작 위치 / 이름 칸 수
//  - IN/OUT 카드 : 카드당 m_nPortsPerCard 칸 고정
//  - 신호기 / 선로전환기 모듈 : 모듈 종류(m_kindLayouts)별 이름 칸 수
int TagNamePrintDlg::GetCardCount() const
{
	if (IsNameOnly()) return (int)m_moduleKinds.size();
	int totalPorts = (int)tagName.size();
	return (totalPorts + m_nPortsPerCard - 1) / m_nPortsPerCard;
}

int TagNamePrintDlg::GetCardStart(int nCard) const
{
	if (!IsNameOnly()) return nCard * m_nPortsPerCard;
	int nStart = 0;
	for (int i = 0; i < nCard; ++i)
		nStart += GetCardCells(i);
	return nStart;
}

int TagNamePrintDlg::GetCardCells(int nCard) const
{
	if (!IsNameOnly()) return m_nPortsPerCard;
	const auto* pLayout = GetModuleLayout(nCard);
	return (pLayout != nullptr) ? pLayout->nCells : 0;
}

void TagNamePrintDlg::OnPaint()
{
	CPaintDC dc(this);

	CWnd* pPreviewStatic = GetDlgItem(IDC_STATIC_PREVIEW);
	if (pPreviewStatic != nullptr)
	{
		CRect rectPreview;
		pPreviewStatic->GetWindowRect(&rectPreview);
		ScreenToClient(&rectPreview);

		// 1. 회색 배경 채우기
		dc.FillSolidRect(&rectPreview, RGB(200, 200, 200));

		// 회색 창 안쪽에 여유 있는 여백(Padding) 공간 확보
		CRect rectInner = rectPreview;
		rectInner.DeflateRect(25, 25);

		// 2. A4 가로(Landscape) 방향 흰색 용지 배경 그리기 (정확한 비율: 297mm / 210mm)
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

		// 3. A4 용지 영역 안쪽으로 클리핑 영역 설정
		int nSavedDC = dc.SaveDC();
		dc.IntersectClipRect(&rectA4);

		if (!tagName.empty())
		{
			// 중복 선언 방지를 위해 상단에서 카드 및 포트 규격 통합 선언
			int maxCardsPerPage = 20; // 5번 조건: 20장마다 새로운 페이지

			int totalPorts = (int)tagName.size();
			int totalCards = GetCardCount(); // [수정] 카드(모듈) 수
			int totalPages = (totalCards + maxCardsPerPage - 1) / maxCardsPerPage;
			if (totalPages == 0) totalPages = 1;

			CFont titleFont;
			titleFont.CreatePointFont(65, _T("맑은 고딕"));
			CFont* pOldFont = dc.SelectObject(&titleFont);
			dc.SetBkMode(TRANSPARENT);
			COLORREF oldTextColor = dc.SetTextColor(RGB(0, 0, 0));

			CString titlePrefix;
			titlePrefix.Format(_T("[ [역] * %s * ]"), (LPCTSTR)m_strListTitle);
			// 제목 영역 여백 축소
			CRect rectTitle(rectA4.left + 15, rectA4.top + 5, rectA4.right - 15, rectA4.top + 20);
			dc.DrawText(titlePrefix, &rectTitle, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

			// 우측 상단에 현재 페이지 정보 출력
			CString pageStr;
			pageStr.Format(_T("미리보기 페이지 : %d / %d"), m_nCurrentPage + 1, totalPages);
			CRect rectPage(rectA4.right - 250, rectA4.top + 5, rectA4.right - 15, rectA4.top + 20);
			dc.DrawText(pageStr, &rectPage, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

			dc.SelectObject(pOldFont);
			titleFont.DeleteObject();

			// A4 가로 규격(297mm x 210mm) 기준 스케일 계산
			double scaleX = (double)rectA4.Width() / 297.0;
			double scaleY = (double)rectA4.Height() / 210.0;

			// mm 단위를 스케일에 직접 반영 (사용자 입력 세로 간격 m_cellSpacingYMm 적용)
			int nLabelWidth = (int)(m_cellWidthMm * scaleX);
			int nLabelHeight = (int)(m_cellHeightMm * scaleY);
			int nSpacingX = (int)(0.5 * scaleX);
			int nSpacingY = (int)(m_cellSpacingYMm * scaleY);

			int contentLeft = rectA4.left + (int)(15.0 * scaleX);

			CFont font;
			font.CreatePointFont(35, _T("맑은 고딕"));
			pOldFont = dc.SelectObject(&font);
			oldTextColor = dc.SetTextColor(RGB(255, 255, 255));

			// 상단 블록 시작 위치
			int topSectionY = rectA4.top + (int)(18.0 * scaleY);

			// 미리보기 화면 페이징 적용 (현재 페이지에 해당하는 카드 인덱스 도출)
			int startCardIndex = m_nCurrentPage * maxCardsPerPage;
			int cardsToDraw = min(maxCardsPerPage, totalCards - startCardIndex);

			for (int c = 0; c < cardsToDraw; ++c)
			{
				int cardNum = startCardIndex + c; // 실제 데이터 인덱스 번호
				int currentX = contentLeft + c * (nLabelWidth + nSpacingX); // 화면에 그려지는 X좌표 위치

				// 높이 및 너비 계산
				int nHalfWidth = nLabelWidth / 2;
				int nHeaderHeight = (int)(nLabelHeight * 0.6); // 헤더 높이를 0.6배로 고정

				CString headerStr;
				headerStr.Format(_T("%s %d"), (LPCTSTR)m_strCardLabel, cardNum + 1);

				// [추가] 신호기 / 선로전환기 모듈은 머리표 없이 이름 칸만 인쇄
				int dataStartY = topSectionY;
				if (!IsNameOnly())
				{
					// --- [수정됨] 1. 최상단 행 (가로 절반, 오른쪽 배치) ---
					// 시작 X좌표를 currentX + nHalfWidth 로 변경하여 오른쪽에만 그려지게 함
					CRect rectTop1(currentX + nHalfWidth, topSectionY, currentX + nLabelWidth, topSectionY + nHeaderHeight);
					dc.FillSolidRect(&rectTop1, RGB(120, 120, 120)); // 사진처럼 약간 밝은 회색
					dc.Draw3dRect(&rectTop1, RGB(50, 50, 50), RGB(200, 200, 200));
					dc.DrawText(headerStr, &rectTop1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

					// --- 2. 상단 두 번째 행 (2칸, 반반 너비) ---
					int top2Y = topSectionY + nHeaderHeight + nSpacingY;
					CRect rectTop2_1(currentX, top2Y, currentX + nHalfWidth, top2Y + nHeaderHeight);
					CRect rectTop2_2(currentX + nHalfWidth, top2Y, currentX + nLabelWidth, top2Y + nHeaderHeight);

					dc.FillSolidRect(&rectTop2_1, RGB(80, 80, 80)); // 사진처럼 약간 어두운 회색
					dc.Draw3dRect(&rectTop2_1, RGB(50, 50, 50), RGB(200, 200, 200));
					dc.DrawText(headerStr, &rectTop2_1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

					dc.FillSolidRect(&rectTop2_2, RGB(80, 80, 80));
					dc.Draw3dRect(&rectTop2_2, RGB(50, 50, 50), RGB(200, 200, 200));
					dc.DrawText(headerStr, &rectTop2_2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

					// --- 3. 16포트 데이터 출력 영역 시작 위치 계산 ---
					dataStartY = top2Y + nHeaderHeight + nSpacingY;
				}

				// [추가] 모듈 표찰 : 표찰 띠(자르는 선) 테두리 출력
				if (IsNameOnly())
				{
					CRect rectStrip(currentX, dataStartY, currentX + nLabelWidth, dataStartY + (int)(GetStripLenMm(cardNum) * scaleY));
					dc.FillSolidRect(&rectStrip, RGB(255, 255, 255));
					dc.Draw3dRect(&rectStrip, RGB(0, 0, 0), RGB(0, 0, 0));
				}

				int r = 0;
				int nCells = GetCardCells(cardNum); // [수정] 카드(모듈)별 이름 칸 수
				for (; r < nCells; ++r)
				{
					// [수정] 모듈 표찰은 전면판 명칭 옆 위치(첫 칸 위치 + 칸 간격)에 맞춰 출력
					int currentY = IsNameOnly()
						? dataStartY + (int)(GetLabelOffsetMm(cardNum, r) * scaleY)
						: dataStartY + r * (nLabelHeight + nSpacingY);
					CRect rectLabel(currentX, currentY, currentX + nLabelWidth, currentY + nLabelHeight);

					dc.FillSolidRect(&rectLabel, RGB(0, 0, 0)); // 데이터는 기존처럼 검은색
					dc.Draw3dRect(&rectLabel, RGB(50, 50, 50), RGB(200, 200, 200));

					// 포트(태그) 이름 출력
					int portIndex = GetCardStart(cardNum) + r;
					if (portIndex < totalPorts)
					{
						dc.DrawText(tagName[portIndex], &rectLabel, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}
				}

				if (!IsNameOnly())
				{
					// --- 4. 하단 레이블 (2칸, 반반 너비) ---
					int bottomY = dataStartY + r * (nLabelHeight + nSpacingY);
					CRect rectBottom1(currentX, bottomY, currentX + nHalfWidth, bottomY + nHeaderHeight);
					CRect rectBottom2(currentX + nHalfWidth, bottomY, currentX + nLabelWidth, bottomY + nHeaderHeight);

					dc.FillSolidRect(&rectBottom1, RGB(80, 80, 80));
					dc.Draw3dRect(&rectBottom1, RGB(50, 50, 50), RGB(200, 200, 200));
					dc.DrawText(headerStr, &rectBottom1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

					dc.FillSolidRect(&rectBottom2, RGB(80, 80, 80));
					dc.Draw3dRect(&rectBottom2, RGB(50, 50, 50), RGB(200, 200, 200));
					dc.DrawText(headerStr, &rectBottom2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
			}

			dc.SetTextColor(oldTextColor);
			dc.SelectObject(pOldFont);
			font.DeleteObject();
		}

		dc.RestoreDC(nSavedDC);
	}
}

void TagNamePrintDlg::OnBnClickedBtnPrint()
{
	if (tagName.empty())
	{
		AfxMessageBox(_T("인쇄할 태그 이름 데이터가 없습니다."));
		return;
	}

	CPrintDialog dlg(FALSE);

	// 프린터 설정 기본값을 강제로 '가로(Landscape)' 방향으로 지정
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
		di.lpszDocName = _T("표찰 이름 인쇄");

		if (dc.StartDoc(&di) > 0)
		{
			int dpiX = dc.GetDeviceCaps(LOGPIXELSX);
			int dpiY = dc.GetDeviceCaps(LOGPIXELSY);

			// mm 단위를 프린터 픽셀로 변환 (m_cellSpacingYMm 적용)
			int nLabelWidth = (int)((m_cellWidthMm / 25.4) * dpiX);
			int nLabelHeight = (int)((m_cellHeightMm / 25.4) * dpiY);
			int nSpacingX = (int)((0.5 / 25.4) * dpiX);
			int nSpacingY = (int)((m_cellSpacingYMm / 25.4) * dpiY);
			int nMarginX = (int)((10.0 / 25.4) * dpiX);
			int nMarginY = (int)((10.0 / 25.4) * dpiY);

			int maxCardsPerPage = 20; // 5번 조건: 20장마다 새로운 페이지

			int totalPorts = (int)tagName.size();
			int totalCards = GetCardCount(); // [수정] 카드(모듈) 수
			int currentCardIndex = 0; // 현재 인쇄할 카드의 시작 인덱스

			CFont font;
			font.CreatePointFont(42, _T("맑은 고딕"), &dc);
			CFont titleFont;
			titleFont.CreatePointFont(80, _T("맑은 고딕"), &dc);

			// 카드 인덱스가 전체 카드를 모두 인쇄할 때까지 반복 (페이지 단위)
			while (currentCardIndex < totalCards)
			{
				dc.StartPage();
				dc.SetBkMode(TRANSPARENT);

				dc.SelectObject(&titleFont);
				COLORREF oldTextColor = dc.SetTextColor(RGB(0, 0, 0));
				CString titlePrefix;
				titlePrefix.Format(_T("[ * %s * ]"), (LPCTSTR)m_strListTitle);
				CRect rectTitle(nMarginX, nMarginY, nMarginX + (int)(150.0 * dpiX / 25.4), nMarginY + (int)(8.0 * dpiY / 25.4));
				dc.DrawText(titlePrefix, &rectTitle, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

				dc.SelectObject(&font);

				// 블록 시작 위치 조정
				int topSectionY = nMarginY + (int)(12.0 * dpiY / 25.4);

				// 이번 페이지에 인쇄할 카드 개수 계산 (남은 카드와 최대 허용치 비교)
				int cardsInThisPage = min(maxCardsPerPage, totalCards - currentCardIndex);

				for (int c = 0; c < cardsInThisPage; ++c)
				{
					int cardNum = currentCardIndex + c;
					int currentX = nMarginX + c * (nLabelWidth + nSpacingX);

					dc.SetTextColor(RGB(255, 255, 255));

					// 높이 및 너비 계산
					int nHalfWidth = nLabelWidth / 2;
					int nHeaderHeight = (int)(nLabelHeight * 0.6); // 헤더 높이를 0.6배로 고정

					CString headerStr;
					headerStr.Format(_T("%s %d"), (LPCTSTR)m_strCardLabel, cardNum + 1);

					// [추가] 신호기 / 선로전환기 모듈은 머리표 없이 이름 칸만 인쇄
					int dataStartY = topSectionY;
					if (!IsNameOnly())
					{
						// --- [수정됨] 1. 최상단 행 (가로 절반, 오른쪽 배치) ---
						// 시작 X좌표를 currentX + nHalfWidth 로 변경하여 오른쪽에만 그려지게 함
						CRect rectTop1(currentX + nHalfWidth, topSectionY, currentX + nLabelWidth, topSectionY + nHeaderHeight);
						dc.FillSolidRect(&rectTop1, RGB(120, 120, 120));
						dc.Draw3dRect(&rectTop1, RGB(50, 50, 50), RGB(200, 200, 200));
						dc.DrawText(headerStr, &rectTop1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

						// --- 2. 상단 두 번째 행 (2칸, 반반 너비) ---
						int top2Y = topSectionY + nHeaderHeight + nSpacingY;
						CRect rectTop2_1(currentX, top2Y, currentX + nHalfWidth, top2Y + nHeaderHeight);
						CRect rectTop2_2(currentX + nHalfWidth, top2Y, currentX + nLabelWidth, top2Y + nHeaderHeight);

						dc.FillSolidRect(&rectTop2_1, RGB(80, 80, 80));
						dc.Draw3dRect(&rectTop2_1, RGB(50, 50, 50), RGB(200, 200, 200));
						dc.DrawText(headerStr, &rectTop2_1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

						dc.FillSolidRect(&rectTop2_2, RGB(80, 80, 80));
						dc.Draw3dRect(&rectTop2_2, RGB(50, 50, 50), RGB(200, 200, 200));
						dc.DrawText(headerStr, &rectTop2_2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

						// --- 3. 16포트 데이터 출력 영역 시작 위치 계산 ---
						dataStartY = top2Y + nHeaderHeight + nSpacingY;
					}

					// [추가] 모듈 표찰 : 표찰 띠(자르는 선) 테두리 출력
					if (IsNameOnly())
					{
						CRect rectStrip(currentX, dataStartY, currentX + nLabelWidth, dataStartY + (int)(GetStripLenMm(cardNum) / 25.4 * dpiY));
						dc.FillSolidRect(&rectStrip, RGB(255, 255, 255));
						dc.Draw3dRect(&rectStrip, RGB(0, 0, 0), RGB(0, 0, 0));
					}

					int r = 0;
					int nCells = GetCardCells(cardNum); // [수정] 카드(모듈)별 이름 칸 수
					for (; r < nCells; ++r)
					{
						// [수정] 모듈 표찰은 전면판 명칭 옆 위치(첫 칸 위치 + 칸 간격)에 맞춰 출력
						int currentY = IsNameOnly()
							? dataStartY + (int)(GetLabelOffsetMm(cardNum, r) / 25.4 * dpiY)
							: dataStartY + r * (nLabelHeight + nSpacingY);
						CRect rectLabel(currentX, currentY, currentX + nLabelWidth, currentY + nLabelHeight);

						dc.FillSolidRect(&rectLabel, RGB(0, 0, 0));
						dc.Draw3dRect(&rectLabel, RGB(50, 50, 50), RGB(200, 200, 200));

						int portIndex = GetCardStart(cardNum) + r;
						if (portIndex < totalPorts)
						{
							dc.DrawText(tagName[portIndex], &rectLabel, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
						}
					}

					if (!IsNameOnly())
					{
						// --- 4. 하단 레이블 (2칸, 반반 너비) ---
						int bottomY = dataStartY + r * (nLabelHeight + nSpacingY);
						CRect rectBottom1(currentX, bottomY, currentX + nHalfWidth, bottomY + nHeaderHeight);
						CRect rectBottom2(currentX + nHalfWidth, bottomY, currentX + nLabelWidth, bottomY + nHeaderHeight);

						dc.FillSolidRect(&rectBottom1, RGB(80, 80, 80));
						dc.Draw3dRect(&rectBottom1, RGB(50, 50, 50), RGB(200, 200, 200));
						dc.DrawText(headerStr, &rectBottom1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

						dc.FillSolidRect(&rectBottom2, RGB(80, 80, 80));
						dc.Draw3dRect(&rectBottom2, RGB(50, 50, 50), RGB(200, 200, 200));
						dc.DrawText(headerStr, &rectBottom2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}
				}

				dc.SetTextColor(oldTextColor);
				dc.EndPage();

				// 다음 페이지 인쇄를 위해 인덱스 증가
				currentCardIndex += cardsInThisPage;
			}

			titleFont.DeleteObject();
			font.DeleteObject();
			dc.EndDoc();
		}
		dc.Detach();
		::DeleteDC(hDC);

		AfxMessageBox(_T("표찰 인쇄 작업이 완료되었습니다."));
	}
}
BOOL TagNamePrintDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int dlgWidth = (int)(screenWidth * 0.9);
	int dlgHeight = (int)(screenHeight * 0.9);

	SetWindowPos(NULL, 0, 0, dlgWidth, dlgHeight, SWP_NOMOVE | SWP_NOZORDER);
	CenterWindow();

	CWnd* pPreview = GetDlgItem(IDC_STATIC_PREVIEW);
	CWnd* pBtn = GetDlgItem(IDC_BTN_PRINT);
	CWnd* pCBtn = GetDlgItem(IDCANCEL);
	CWnd* pPrevBtn = GetDlgItem(IDC_BTN_PREV_PAGE);
	CWnd* pNextBtn = GetDlgItem(IDC_BTN_NEXT_PAGE);

	if (pPreview != nullptr && pBtn != nullptr)
	{
		CRect rectBtn;
		pBtn->GetWindowRect(&rectBtn);

		int btnWidth = rectBtn.Width();
		int btnHeight = rectBtn.Height();

		// 인쇄 및 취소 버튼 우측 하단 배치
		pBtn->MoveWindow(dlgWidth - btnWidth - 150, dlgHeight - btnHeight - 45, btnWidth, btnHeight);
		pCBtn->MoveWindow(dlgWidth - btnWidth - 40, dlgHeight - btnHeight - 45, btnWidth, btnHeight);

		// 페이지 전환 버튼들을 인쇄 버튼 좌측에 나란히 배치
		if (pPrevBtn != nullptr && pNextBtn != nullptr)
		{
			pPrevBtn->MoveWindow(dlgWidth - btnWidth - 370, dlgHeight - btnHeight - 45, btnWidth, btnHeight);
			pNextBtn->MoveWindow(dlgWidth - btnWidth - 260, dlgHeight - btnHeight - 45, btnWidth, btnHeight);
		}

		pPreview->MoveWindow(20, 20, dlgWidth - 40, dlgHeight - btnHeight - 75);
	}

	// [추가] 표찰 크기 / 위치 조절 UI (저장된 값 불러온 뒤 배치)
	LoadSettings();
	LayoutSettingControls();
	UpdateSettingEdits();

	return TRUE;
}

void TagNamePrintDlg::OnBnClickedCellChangeButton()
{
	CString str;
	GetDlgItemText(IDC_EDIT_W, str);
	if (!str.IsEmpty()) m_cellWidthMm = _tstof(str);
	GetDlgItemText(IDC_EDIT_H, str);
	if (!str.IsEmpty()) m_cellHeightMm = _tstof(str);
	GetDlgItemText(IDC_EDIT_S, str);
	if (!str.IsEmpty()) m_cellSpacingYMm = _tstof(str);

	// mm 단위 유효 범위 제한 (가로 1.0 ~ 14.0mm, 세로 1.0 ~ 8.0mm, 세로간격 0.0 ~ 3.0mm)
	m_cellWidthMm = (std::min)((std::max)(m_cellWidthMm, 1.0), 14.0);
	m_cellHeightMm = (std::min)((std::max)(m_cellHeightMm, 1.0), 8.0);
	m_cellSpacingYMm = (std::min)((std::max)(m_cellSpacingYMm, 0.0), 3.0);

	// [추가] 모듈 종류별 띠 길이 / 첫 칸 위치 / 칸 간격 (띠 길이 10 ~ 180mm, 위치·간격은 띠 길이 이내)
	for (size_t i = 0; i < m_kindRows.size() && i < m_kindLayouts.size(); ++i)
	{
		auto& layout = m_kindLayouts[i];
		double* pValues[3] = { &layout.stripLenMm, &layout.topMm, &layout.pitchMm };
		for (int k = 0; k < 3; ++k)
		{
			if (m_kindRows[i].pEdit[k] == nullptr) continue;
			m_kindRows[i].pEdit[k]->GetWindowText(str);
			if (!str.IsEmpty()) *pValues[k] = _tstof(str);
		}
		layout.stripLenMm = (std::min)((std::max)(layout.stripLenMm, 10.0), 180.0);
		layout.topMm = (std::min)((std::max)(layout.topMm, 0.0), layout.stripLenMm);
		layout.pitchMm = (std::min)((std::max)(layout.pitchMm, 0.0), layout.stripLenMm);
	}

	SaveSettings();
	UpdateSettingEdits();

	Invalidate();
	UpdateWindow();
}

void TagNamePrintDlg::OnOK()
{
	// Enter 키로 대화상자가 닫히지 않도록 설정 적용으로 대체
	OnBnClickedCellChangeButton();
}

// [추가] 레지스트리 저장 키 : 카드 목록(IN/OUT/SIGNAL/SWITCH)별로 칸 크기를 따로 저장
CString TagNamePrintDlg::GetProfileKey(LPCTSTR pszName) const
{
	CString strKey;
	strKey.Format(_T("%s_%s"), (LPCTSTR)m_strListTitle, pszName);
	return strKey;
}

namespace
{
	const LPCTSTR TAG_PRINT_SECTION = _T("TagPrint");

	double ReadProfileMm(LPCTSTR pszKey, double dDefault)
	{
		CString str = AfxGetApp()->GetProfileString(TAG_PRINT_SECTION, pszKey, _T(""));
		return str.IsEmpty() ? dDefault : _tstof(str);
	}

	void WriteProfileMm(LPCTSTR pszKey, double dValue)
	{
		CString str;
		str.Format(_T("%.1f"), dValue);
		AfxGetApp()->WriteProfileString(TAG_PRINT_SECTION, pszKey, str);
	}
}

void TagNamePrintDlg::LoadSettings()
{
	m_cellWidthMm = ReadProfileMm(GetProfileKey(_T("Width")), m_cellWidthMm);
	m_cellHeightMm = ReadProfileMm(GetProfileKey(_T("Height")), m_cellHeightMm);
	m_cellSpacingYMm = ReadProfileMm(GetProfileKey(_T("SpacingY")), m_cellSpacingYMm);

	// 모듈 종류별 배치는 종류 키(Kind3 등)로 저장 -> 신호기 / 선로전환기 목록에서 공유
	for (auto& layout : m_kindLayouts)
	{
		layout.stripLenMm = ReadProfileMm(layout.key + _T("_StripLen"), layout.stripLenMm);
		layout.topMm = ReadProfileMm(layout.key + _T("_Top"), layout.topMm);
		layout.pitchMm = ReadProfileMm(layout.key + _T("_Pitch"), layout.pitchMm);
	}
}

void TagNamePrintDlg::SaveSettings()
{
	WriteProfileMm(GetProfileKey(_T("Width")), m_cellWidthMm);
	WriteProfileMm(GetProfileKey(_T("Height")), m_cellHeightMm);
	WriteProfileMm(GetProfileKey(_T("SpacingY")), m_cellSpacingYMm);

	for (const auto& layout : m_kindLayouts)
	{
		WriteProfileMm(layout.key + _T("_StripLen"), layout.stripLenMm);
		WriteProfileMm(layout.key + _T("_Top"), layout.topMm);
		WriteProfileMm(layout.key + _T("_Pitch"), layout.pitchMm);
	}
}

// [추가] 미리보기 아래쪽(왼쪽)에 설정 입력란 배치
//  1행 : 칸 가로 / 칸 세로 / 세로 간격(IN/OUT 만) + [적용]
//  모듈 표찰이면 그 위에 종류별 1행씩 : 띠 길이 / 첫 칸 위치 / 칸 간격
void TagNamePrintDlg::LayoutSettingControls()
{
	CWnd* pPreview = GetDlgItem(IDC_STATIC_PREVIEW);
	CWnd* pPrintBtn = GetDlgItem(IDC_BTN_PRINT);
	CWnd* pApplyBtn = GetDlgItem(IDC_CELL_CHANGE_BUTTON);
	CWnd* pEditW = GetDlgItem(IDC_EDIT_W);
	CWnd* pEditH = GetDlgItem(IDC_EDIT_H);
	CWnd* pEditS = GetDlgItem(IDC_EDIT_S);
	if (pPreview == nullptr || pPrintBtn == nullptr || pApplyBtn == nullptr
		|| pEditW == nullptr || pEditH == nullptr || pEditS == nullptr)
		return;

	CFont* pFont = GetFont();
	const int rowHeight = 24;
	const int rowGap = 6;
	const int editWidth = 55;
	const int labelGap = 6;
	const int groupGap = 18;

	// 인쇄 버튼과 같은 높이를 1행 기준으로 사용 (클라이언트 좌표)
	CRect rectPrint;
	pPrintBtn->GetWindowRect(&rectPrint);
	ScreenToClient(&rectPrint);
	int nRowY = rectPrint.top + (rectPrint.Height() - rowHeight) / 2;

	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(pFont);

	auto addLabel = [&](std::unique_ptr<CStatic>& pStatic, LPCTSTR pszText, int& x, int y)
	{
		int w = dc.GetTextExtent(pszText).cx + 4;
		pStatic = std::make_unique<CStatic>();
		pStatic->Create(pszText, WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE, CRect(x, y, x + w, y + rowHeight), this);
		pStatic->SetFont(pFont);
		x += w + labelGap;
	};

	// --- 1행 : 칸 크기 ---
	int x = 20;
	struct { LPCTSTR text; CWnd* pEdit; bool bShow; } sizeItems[] = {
		{ _T("칸 가로(mm)"), pEditW, true },
		{ _T("칸 세로(mm)"), pEditH, true },
		{ _T("세로 간격(mm)"), pEditS, !IsNameOnly() }, // 모듈 표찰은 칸 간격을 종류별로 따로 설정
	};
	for (auto& item : sizeItems)
	{
		if (!item.bShow)
		{
			item.pEdit->ShowWindow(SW_HIDE);
			continue;
		}
		m_sizeLabels.emplace_back();
		addLabel(m_sizeLabels.back(), item.text, x, nRowY);
		item.pEdit->MoveWindow(x, nRowY, editWidth, rowHeight);
		item.pEdit->ShowWindow(SW_SHOW);
		x += editWidth + groupGap;
	}
	pApplyBtn->SetWindowText(_T("적용"));
	pApplyBtn->MoveWindow(x, nRowY, 70, rowHeight);
	pApplyBtn->ShowWindow(SW_SHOW);

	// --- 모듈 종류별 행 (1행 위로 쌓음) ---
	m_kindRows.clear();
	m_kindRows.resize(IsNameOnly() ? m_kindLayouts.size() : 0);
	int nTopY = nRowY;
	for (size_t i = 0; i < m_kindRows.size(); ++i)
	{
		auto& row = m_kindRows[i];
		int y = nRowY - (int)(m_kindRows.size() - i) * (rowHeight + rowGap);
		nTopY = (std::min)(nTopY, y);
		x = 20;

		CString strTitle;
		strTitle.Format(_T("[%s]"), (LPCTSTR)m_kindLayouts[i].name);
		row.pTitle = std::make_unique<CStatic>();
		row.pTitle->Create(strTitle, WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE, CRect(x, y, x + 110, y + rowHeight), this);
		row.pTitle->SetFont(pFont);
		x += 110 + labelGap;

		LPCTSTR texts[3] = { _T("띠 길이(mm)"), _T("첫 칸 위치(mm)"), _T("칸 간격(mm)") };
		for (int k = 0; k < 3; ++k)
		{
			addLabel(row.pLabel[k], texts[k], x, y);
			row.pEdit[k] = std::make_unique<CEdit>();
			row.pEdit[k]->CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
				CRect(x, y, x + editWidth, y + rowHeight), this, 0);
			row.pEdit[k]->SetFont(pFont);
			x += editWidth + groupGap;
		}
	}
	dc.SelectObject(pOldFont);

	// 미리보기 영역이 설정 행과 겹치지 않도록 높이 조정
	CRect rectPreview;
	pPreview->GetWindowRect(&rectPreview);
	ScreenToClient(&rectPreview);
	if (rectPreview.bottom > nTopY - 10)
	{
		rectPreview.bottom = nTopY - 10;
		pPreview->MoveWindow(&rectPreview);
	}
}

void TagNamePrintDlg::UpdateSettingEdits()
{
	CString str;
	str.Format(_T("%.1f"), m_cellWidthMm);
	SetDlgItemText(IDC_EDIT_W, str);
	str.Format(_T("%.1f"), m_cellHeightMm);
	SetDlgItemText(IDC_EDIT_H, str);
	str.Format(_T("%.1f"), m_cellSpacingYMm);
	SetDlgItemText(IDC_EDIT_S, str);

	for (size_t i = 0; i < m_kindRows.size() && i < m_kindLayouts.size(); ++i)
	{
		const auto& layout = m_kindLayouts[i];
		double values[3] = { layout.stripLenMm, layout.topMm, layout.pitchMm };
		for (int k = 0; k < 3; ++k)
		{
			if (m_kindRows[i].pEdit[k] == nullptr) continue;
			str.Format(_T("%.1f"), values[k]);
			m_kindRows[i].pEdit[k]->SetWindowText(str);
		}
	}
}

void TagNamePrintDlg::OnBnClickedBtnPrevPage()
{
	if (m_nCurrentPage > 0)
	{
		m_nCurrentPage--; // 이전 페이지로
		Invalidate();
		UpdateWindow();
	}
}

void TagNamePrintDlg::OnBnClickedBtnNextPage()
{
	int maxCardsPerPage = 20;
	int totalCards = GetCardCount(); // [수정] 카드(모듈) 수
	int totalPages = (totalCards + maxCardsPerPage - 1) / maxCardsPerPage;

	if (m_nCurrentPage < totalPages - 1)
	{
		m_nCurrentPage++; // 다음 페이지로
		Invalidate();
		UpdateWindow();
	}
}