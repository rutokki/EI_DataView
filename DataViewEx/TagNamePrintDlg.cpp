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
	, m_bottomBlankMm(0.0)
	, m_maxSpacingYMm(3.0)
	, m_maxBottomBlankMm(30.0)
	, m_maxCellHeightMm(8.0)
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
//  - 신호기 / 선로전환기 모듈 : 모듈당 m_nPortsPerCard(MAX_SIG_MODULE_PORT / MAX_SWH_MODULE_PORT) 칸 고정
int TagNamePrintDlg::GetCardCount() const
{
	int totalPorts = (int)tagName.size();
	return (totalPorts + m_nPortsPerCard - 1) / m_nPortsPerCard;
}

int TagNamePrintDlg::GetCardStart(int nCard) const
{
	return nCard * m_nPortsPerCard;
}

int TagNamePrintDlg::GetCardCells(int nCard) const
{
	return m_nPortsPerCard;
}

// [추가] 표찰 이름을 칸 크기에 맞는 글자 크기로 출력
//  글자 높이는 칸 세로의 75%, 이름이 길어 칸 가로를 넘으면 그 비율만큼 줄임
//  (칸 가로 / 세로를 바꾸면 글자 크기도 같이 바뀜, 미리보기 / 인쇄 공통)
static void DrawFitText(CDC& dc, const CString& strText, const CRect& rect)
{
	if (strText.IsEmpty() || rect.Width() <= 0 || rect.Height() <= 0) return;

	LOGFONT lf = {};
	lf.lfHeight = -(std::max)(1, (int)(rect.Height() * 0.75));
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfQuality = CLEARTYPE_QUALITY;
	_tcscpy_s(lf.lfFaceName, _T("맑은 고딕"));

	CFont font;
	font.CreateFontIndirect(&lf);
	CFont* pOldFont = dc.SelectObject(&font);

	int nAvailWidth = (std::max)(1, (int)(rect.Width() * 0.92));
	CSize size = dc.GetTextExtent(strText);
	if (size.cx > nAvailWidth)
	{
		lf.lfHeight = -(std::max)(1, (int)((double)-lf.lfHeight * nAvailWidth / size.cx));
		dc.SelectObject(pOldFont);
		font.DeleteObject();
		font.CreateFontIndirect(&lf);
		dc.SelectObject(&font);
	}

	CRect rectText = rect;
	dc.DrawText(strText, &rectText, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
	dc.SelectObject(pOldFont);
}

// [추가] 페이지당 카드 수 계산 (미리보기 / 인쇄 / 페이지 이동 공통)
//  A4 가로(297 x 210mm) 에서 여백을 뺀 영역(TAG_USABLE_WIDTH_MM x TAG_USABLE_HEIGHT_MM)에
//  한 줄 16장(서브랙 슬롯 수) x 위아래 2줄 = 페이지당 32장
//  칸 크기를 키워 A4 를 넘으면 칸 가로 / 카드 높이 기준으로 들어가는 만큼만
double TagNamePrintDlg::GetCardHeightMm() const
{
	// 머리표 3개(고정 높이) + 머리표 앞뒤 간격 3개 + 포트 칸 n개(칸 세로) + 포트 사이 간격 (n-1)개 + 하단 공백
	double dHeader = TAG_HEADER_HEIGHT_MM;
	double dGap = GetHeaderGapMm();
	return dHeader * 3 + dGap * 3 + m_nPortsPerCard * m_cellHeightMm
		+ (std::max)(0, m_nPortsPerCard - 1) * m_cellSpacingYMm + m_bottomBlankMm;
}

int TagNamePrintDlg::GetCardsPerRow() const
{
	// 한 줄 최대 TAG_CARDS_PER_ROW(16)장, 칸이 넓어 A4 폭을 넘으면 들어가는 만큼만
	int nCount = (int)((TAG_USABLE_WIDTH_MM + TAG_COL_GAP_MM) / (m_cellWidthMm + TAG_COL_GAP_MM));
	return (std::max)(1, (std::min)(TAG_CARDS_PER_ROW, nCount));
}

int TagNamePrintDlg::GetRowsPerPage() const
{
	double dCardHeight = GetCardHeightMm();
	if (dCardHeight <= 0.0) return 1;
	// 위아래 최대 TAG_ROWS_PER_PAGE(2)줄, 카드가 길어 A4 높이를 넘으면 들어가는 만큼만
	int nCount = (int)((TAG_USABLE_HEIGHT_MM + TAG_ROW_GAP_MM) / (dCardHeight + TAG_ROW_GAP_MM));
	return (std::max)(1, (std::min)(TAG_ROWS_PER_PAGE, nCount));
}

int TagNamePrintDlg::GetCardsPerPage() const
{
	return GetCardsPerRow() * GetRowsPerPage();
}

int TagNamePrintDlg::GetTotalPages() const
{
	int nPerPage = GetCardsPerPage();
	int nPages = (GetCardCount() + nPerPage - 1) / nPerPage;
	return (std::max)(1, nPages);
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
			// [수정] 페이지당 카드 수 : 한 줄 16장 x 위아래 2줄 (A4 에 들어가는 만큼)
			int maxCardsPerPage = GetCardsPerPage();
			int nCardsPerRow = GetCardsPerRow();

			int totalPorts = (int)tagName.size();
			int totalCards = GetCardCount(); // [수정] 카드(모듈) 수
			int totalPages = GetTotalPages();
			// 설정 변경으로 페이지 수가 줄어든 경우 현재 페이지 보정
			if (m_nCurrentPage >= totalPages) m_nCurrentPage = totalPages - 1;
			if (m_nCurrentPage < 0) m_nCurrentPage = 0;

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
			int nHeaderGap = (int)(GetHeaderGapMm() * scaleY); // [추가] 머리표 앞뒤 간격

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
			int nRowPitch = (int)((GetCardHeightMm() + TAG_ROW_GAP_MM) * scaleY); // 카드 줄 간격

			for (int c = 0; c < cardsToDraw; ++c)
			{
				int cardNum = startCardIndex + c; // 실제 데이터 인덱스 번호
				// [수정] 한 줄에 nCardsPerRow 장, 넘치면 아래 줄로
				int currentX = contentLeft + (c % nCardsPerRow) * (nLabelWidth + nSpacingX); // 화면에 그려지는 X좌표 위치
				int cardTopY = topSectionY + (c / nCardsPerRow) * nRowPitch;

				// 높이 및 너비 계산
				int nHalfWidth = nLabelWidth / 2;
				// [수정] 머리표(In/Out/SIG/SWI n) 높이는 칸 크기 조절과 무관하게 고정
				int nHeaderHeight = (int)(TAG_HEADER_HEIGHT_MM * scaleY);

				CString headerStr;
				headerStr.Format(_T("%s %d"), (LPCTSTR)m_strCardLabel, cardNum + 1);

				// --- [수정됨] 1. 최상단 행 (가로 절반, 오른쪽 배치) ---
				// 시작 X좌표를 currentX + nHalfWidth 로 변경하여 오른쪽에만 그려지게 함
				CRect rectTop1(currentX + nHalfWidth, cardTopY, currentX + nLabelWidth, cardTopY + nHeaderHeight);
				dc.FillSolidRect(&rectTop1, RGB(120, 120, 120)); // 사진처럼 약간 밝은 회색
				dc.Draw3dRect(&rectTop1, RGB(50, 50, 50), RGB(200, 200, 200));
				dc.DrawText(headerStr, &rectTop1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				// --- 2. 상단 두 번째 행 (2칸, 반반 너비) ---
				int top2Y = cardTopY + nHeaderHeight + nHeaderGap;
				CRect rectTop2_1(currentX, top2Y, currentX + nHalfWidth, top2Y + nHeaderHeight);
				CRect rectTop2_2(currentX + nHalfWidth, top2Y, currentX + nLabelWidth, top2Y + nHeaderHeight);

				dc.FillSolidRect(&rectTop2_1, RGB(80, 80, 80)); // 사진처럼 약간 어두운 회색
				dc.Draw3dRect(&rectTop2_1, RGB(50, 50, 50), RGB(200, 200, 200));
				dc.DrawText(headerStr, &rectTop2_1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				dc.FillSolidRect(&rectTop2_2, RGB(80, 80, 80));
				dc.Draw3dRect(&rectTop2_2, RGB(50, 50, 50), RGB(200, 200, 200));
				dc.DrawText(headerStr, &rectTop2_2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				// --- 3. 16포트 데이터 출력 영역 시작 위치 계산 ---
				int dataStartY = top2Y + nHeaderHeight + nHeaderGap;

				int r = 0;
				int nCells = GetCardCells(cardNum); // [수정] 카드(모듈)별 이름 칸 수
				for (; r < nCells; ++r)
				{
					// [추가] 실제 사용 포트 수만큼만 칸 출력 (4등형 신호기 : 2칸), 카드 길이는 그대로 유지
					if (r >= GetCardUsedCells(cardNum)) continue;

					int currentY = dataStartY + r * (nLabelHeight + nSpacingY);
					CRect rectLabel(currentX, currentY, currentX + nLabelWidth, currentY + nLabelHeight);

					dc.FillSolidRect(&rectLabel, RGB(0, 0, 0)); // 데이터는 기존처럼 검은색
					dc.Draw3dRect(&rectLabel, RGB(50, 50, 50), RGB(200, 200, 200));

					// 포트(태그) 이름 출력
					int portIndex = GetCardStart(cardNum) + r;
					if (portIndex < totalPorts)
					{
						DrawFitText(dc, tagName[portIndex], rectLabel); // [수정] 칸 크기에 맞춰 글자 크기 자동 조절
					}
				}

				// --- 4. 하단 레이블 (2칸, 반반 너비) ---
				// [수정] 하단 공백 : 마지막 포트 칸과 아래 꼬리표(머리표) 사이에 빈 칸 (표찰 홀더 길이 맞춤용)
				int blankY = dataStartY + r * (nLabelHeight + nSpacingY) - nSpacingY + nHeaderGap; // 마지막 칸 아래 + 머리표 간격
				int nBottomBlank = (int)(m_bottomBlankMm * scaleY);
				if (nBottomBlank > 0)
				{
					CRect rectBlank(currentX, blankY, currentX + nLabelWidth, blankY + nBottomBlank);
					dc.FillSolidRect(&rectBlank, RGB(255, 255, 255));
					dc.Draw3dRect(&rectBlank, RGB(50, 50, 50), RGB(50, 50, 50));
				}
				int bottomY = blankY + nBottomBlank;
				CRect rectBottom1(currentX, bottomY, currentX + nHalfWidth, bottomY + nHeaderHeight);
				CRect rectBottom2(currentX + nHalfWidth, bottomY, currentX + nLabelWidth, bottomY + nHeaderHeight);

				dc.FillSolidRect(&rectBottom1, RGB(80, 80, 80));
				dc.Draw3dRect(&rectBottom1, RGB(50, 50, 50), RGB(200, 200, 200));
				dc.DrawText(headerStr, &rectBottom1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				dc.FillSolidRect(&rectBottom2, RGB(80, 80, 80));
				dc.Draw3dRect(&rectBottom2, RGB(50, 50, 50), RGB(200, 200, 200));
				dc.DrawText(headerStr, &rectBottom2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

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
			int nHeaderGap = (int)((GetHeaderGapMm() / 25.4) * dpiY); // [추가] 머리표 앞뒤 간격
			int nMarginX = (int)((10.0 / 25.4) * dpiX);
			int nMarginY = (int)((10.0 / 25.4) * dpiY);

			// [수정] 페이지당 카드 수 : 미리보기와 동일한 계산 사용
			int maxCardsPerPage = GetCardsPerPage();
			int nCardsPerRow = GetCardsPerRow();
			int nRowPitch = (int)(((GetCardHeightMm() + TAG_ROW_GAP_MM) / 25.4) * dpiY); // 카드 줄 간격

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
					// [수정] 한 줄에 nCardsPerRow 장, 넘치면 아래 줄로
					int currentX = nMarginX + (c % nCardsPerRow) * (nLabelWidth + nSpacingX);
					int cardTopY = topSectionY + (c / nCardsPerRow) * nRowPitch;

					dc.SetTextColor(RGB(255, 255, 255));

					// 높이 및 너비 계산
					int nHalfWidth = nLabelWidth / 2;
					// [수정] 머리표(In/Out/SIG/SWI n) 높이는 칸 크기 조절과 무관하게 고정
					int nHeaderHeight = (int)((TAG_HEADER_HEIGHT_MM / 25.4) * dpiY);

					CString headerStr;
					headerStr.Format(_T("%s %d"), (LPCTSTR)m_strCardLabel, cardNum + 1);

					// --- [수정됨] 1. 최상단 행 (가로 절반, 오른쪽 배치) ---
					// 시작 X좌표를 currentX + nHalfWidth 로 변경하여 오른쪽에만 그려지게 함
					CRect rectTop1(currentX + nHalfWidth, cardTopY, currentX + nLabelWidth, cardTopY + nHeaderHeight);
					dc.FillSolidRect(&rectTop1, RGB(120, 120, 120));
					dc.Draw3dRect(&rectTop1, RGB(50, 50, 50), RGB(200, 200, 200));
					dc.DrawText(headerStr, &rectTop1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

					// --- 2. 상단 두 번째 행 (2칸, 반반 너비) ---
					int top2Y = cardTopY + nHeaderHeight + nHeaderGap;
					CRect rectTop2_1(currentX, top2Y, currentX + nHalfWidth, top2Y + nHeaderHeight);
					CRect rectTop2_2(currentX + nHalfWidth, top2Y, currentX + nLabelWidth, top2Y + nHeaderHeight);

					dc.FillSolidRect(&rectTop2_1, RGB(80, 80, 80));
					dc.Draw3dRect(&rectTop2_1, RGB(50, 50, 50), RGB(200, 200, 200));
					dc.DrawText(headerStr, &rectTop2_1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

					dc.FillSolidRect(&rectTop2_2, RGB(80, 80, 80));
					dc.Draw3dRect(&rectTop2_2, RGB(50, 50, 50), RGB(200, 200, 200));
					dc.DrawText(headerStr, &rectTop2_2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

					// --- 3. 16포트 데이터 출력 영역 시작 위치 계산 ---
					int dataStartY = top2Y + nHeaderHeight + nHeaderGap;

					int r = 0;
					int nCells = GetCardCells(cardNum); // [수정] 카드(모듈)별 이름 칸 수
					for (; r < nCells; ++r)
					{
						// [추가] 실제 사용 포트 수만큼만 칸 출력 (4등형 신호기 : 2칸), 카드 길이는 그대로 유지
						if (r >= GetCardUsedCells(cardNum)) continue;

						int currentY = dataStartY + r * (nLabelHeight + nSpacingY);
						CRect rectLabel(currentX, currentY, currentX + nLabelWidth, currentY + nLabelHeight);

						dc.FillSolidRect(&rectLabel, RGB(0, 0, 0));
						dc.Draw3dRect(&rectLabel, RGB(50, 50, 50), RGB(200, 200, 200));

						int portIndex = GetCardStart(cardNum) + r;
						if (portIndex < totalPorts)
						{
							DrawFitText(dc, tagName[portIndex], rectLabel); // [수정] 칸 크기에 맞춰 글자 크기 자동 조절
						}
					}

					// --- 4. 하단 레이블 (2칸, 반반 너비) ---
					// [수정] 하단 공백 : 마지막 포트 칸과 아래 꼬리표(머리표) 사이에 빈 칸 (표찰 홀더 길이 맞춤용)
					int blankY = dataStartY + r * (nLabelHeight + nSpacingY) - nSpacingY + nHeaderGap; // 마지막 칸 아래 + 머리표 간격
					int nBottomBlank = (int)(m_bottomBlankMm / 25.4 * dpiY);
					if (nBottomBlank > 0)
					{
						CRect rectBlank(currentX, blankY, currentX + nLabelWidth, blankY + nBottomBlank);
						dc.FillSolidRect(&rectBlank, RGB(255, 255, 255));
						dc.Draw3dRect(&rectBlank, RGB(50, 50, 50), RGB(50, 50, 50));
					}
					int bottomY = blankY + nBottomBlank;
					CRect rectBottom1(currentX, bottomY, currentX + nHalfWidth, bottomY + nHeaderHeight);
					CRect rectBottom2(currentX + nHalfWidth, bottomY, currentX + nLabelWidth, bottomY + nHeaderHeight);

					dc.FillSolidRect(&rectBottom1, RGB(80, 80, 80));
					dc.Draw3dRect(&rectBottom1, RGB(50, 50, 50), RGB(200, 200, 200));
					dc.DrawText(headerStr, &rectBottom1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

					dc.FillSolidRect(&rectBottom2, RGB(80, 80, 80));
					dc.Draw3dRect(&rectBottom2, RGB(50, 50, 50), RGB(200, 200, 200));
					dc.DrawText(headerStr, &rectBottom2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

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
	if (m_pEditBottom != nullptr && m_pEditBottom->GetSafeHwnd() != nullptr)
	{
		m_pEditBottom->GetWindowText(str);
		if (!str.IsEmpty()) m_bottomBlankMm = _tstof(str);
	}

	// mm 단위 유효 범위 제한 (가로 1.0 ~ 14.0mm)
	// 칸 세로 : IN/OUT 1.0 ~ 8.0mm, 신호기 / 선로전환기 모듈 1.0 ~ 20.0mm (m_maxCellHeightMm)
	// 하단 공백 : IN/OUT 0.0 ~ 30.0mm, 신호기 / 선로전환기 모듈 0.0 ~ 120.0mm (m_maxBottomBlankMm)
	// 세로간격 : IN/OUT(16포트) 0.0 ~ 3.0mm, 신호기 / 선로전환기 모듈(4 / 2포트) 0.0 ~ 40.0mm (m_maxSpacingYMm)
	m_cellWidthMm = (std::min)((std::max)(m_cellWidthMm, 1.0), 14.0);
	m_cellHeightMm = (std::min)((std::max)(m_cellHeightMm, 1.0), m_maxCellHeightMm);
	m_cellSpacingYMm = (std::min)((std::max)(m_cellSpacingYMm, 0.0), m_maxSpacingYMm);
	m_bottomBlankMm = (std::min)((std::max)(m_bottomBlankMm, 0.0), m_maxBottomBlankMm);

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
	m_bottomBlankMm = ReadProfileMm(GetProfileKey(_T("BottomBlank")), m_bottomBlankMm);
}

void TagNamePrintDlg::SaveSettings()
{
	WriteProfileMm(GetProfileKey(_T("Width")), m_cellWidthMm);
	WriteProfileMm(GetProfileKey(_T("Height")), m_cellHeightMm);
	WriteProfileMm(GetProfileKey(_T("SpacingY")), m_cellSpacingYMm);
	WriteProfileMm(GetProfileKey(_T("BottomBlank")), m_bottomBlankMm);
}

// [추가] 미리보기 아래쪽(왼쪽)에 설정 입력란 배치
//  칸 가로 / 칸 세로 / 세로 간격 / 하단 공백 + [적용]
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

	// [추가] 하단 공백 입력란 (리소스에 없으므로 직접 생성)
	m_pEditBottom = std::make_unique<CEdit>();
	m_pEditBottom->CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""), WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,
		CRect(0, 0, editWidth, rowHeight), this, 0);
	m_pEditBottom->SetFont(pFont);

	// --- 1행 : 칸 크기 ---
	int x = 20;
	struct { LPCTSTR text; CWnd* pEdit; bool bShow; } sizeItems[] = {
		{ _T("칸 가로(mm)"), pEditW, true },
		{ _T("칸 세로(mm)"), pEditH, true },
		{ _T("세로 간격(mm)"), pEditS, true },
		{ _T("하단 공백(mm)"), m_pEditBottom.get(), true },
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

	dc.SelectObject(pOldFont);

	// 미리보기 영역이 설정 행과 겹치지 않도록 높이 조정
	CRect rectPreview;
	pPreview->GetWindowRect(&rectPreview);
	ScreenToClient(&rectPreview);
	if (rectPreview.bottom > nRowY - 10)
	{
		rectPreview.bottom = nRowY - 10;
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
	if (m_pEditBottom != nullptr && m_pEditBottom->GetSafeHwnd() != nullptr)
	{
		str.Format(_T("%.1f"), m_bottomBlankMm);
		m_pEditBottom->SetWindowText(str);
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
	int totalPages = GetTotalPages(); // [수정] 미리보기 / 인쇄와 동일한 페이지 계산

	if (m_nCurrentPage < totalPages - 1)
	{
		m_nCurrentPage++; // 다음 페이지로
		Invalidate();
		UpdateWindow();
	}
}