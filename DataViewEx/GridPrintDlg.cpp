#include "stdafx.h"
#include "resource.h"
#include "GridPrintDlg.h"
#include <vector>
#include <utility>
#include <algorithm>

IMPLEMENT_DYNAMIC(GridPrintDlg, CBCGPDialog)

GridPrintDlg::GridPrintDlg(CWnd* pParent /*=nullptr*/)
	: CBCGPDialog(IDD_GRID_PRINT_DLG, pParent)
	, m_nCurrentPage(0)
	, m_rowsPerPage(30)
	, m_gridType(0)
	, m_isMultiLine(false)
{
}

GridPrintDlg::~GridPrintDlg()
{
	if (m_btnFont.GetSafeHandle() != nullptr)
	{
		m_btnFont.DeleteObject();
	}
}

void GridPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_PREV_PAGE, prevBtn);
	DDX_Control(pDX, IDC_BTN_NEXT_PAGE, nextgBtn);
	DDX_Control(pDX, IDC_BTN_PRINT2, printBtn);
	DDX_Control(pDX, IDCANCEL, cancelBtn);
}

BEGIN_MESSAGE_MAP(GridPrintDlg, CBCGPDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_PRINT2, &GridPrintDlg::OnBnClickedBtnPrint)
	ON_BN_CLICKED(IDC_BTN_PREV_PAGE, &GridPrintDlg::OnBnClickedBtnPrevPage)
	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, &GridPrintDlg::OnBnClickedBtnNextPage)
END_MESSAGE_MAP()

// 텍스트 내 줄 바꿈(\n) 개수를 세어 줄 수 반환 (캐리지 리턴 \r 처리 포함)
static int GetCellLineCount(CString text)
{
	text.Replace(_T("\r\n"), _T("\n"));
	text.Replace(_T("\r"), _T(""));

	if (text.IsEmpty()) return 1;
	int count = 1;
	int nPos = text.Find(_T('\n'));
	while (nPos != -1)
	{
		count++;
		nPos = text.Find(_T('\n'), nPos + 1);
	}
	return count;
}

// 멀티라인 그리드(3번, 7번) 행 가중치 반환 함수 (최소 가중치 2를 부여하여 공간 부족 방지)
static int GetMultiLineGridRowWeight(const std::vector<CString>& rowData)
{
	int maxLines = 1;
	for (const auto& cell : rowData)
	{
		int lines = GetCellLineCount(cell);
		if (lines > maxLines) maxLines = lines;
	}
	// 멀티라인 텍스트의 실제 높이 여유를 위해 최소 2 이상의 가중치를 확보합니다.
	return (std::max)(2, maxLines);
}

// 페이지별 행 범위 계산 함수 (마지막 데이터 유실 방지형 알고리즘 적용)
// 페이지별 행 범위 계산 함수 (안전한 누적 가중치 분할 방식)
static std::vector<std::pair<int, int>> GetPageRanges(const std::vector<std::vector<CString>>& rows, int gridType)
{
	std::vector<std::pair<int, int>> ranges;
	if (rows.empty()) return ranges;

	if (gridType == (int)EGridType::Device || gridType == (int)EGridType::InterLockingData)
	{
		int maxPageWeight = 40; // 한 페이지 최대 허용 가중치
		int curStart = 0;
		int curWeightSum = 0;

		for (int i = 0; i < (int)rows.size(); ++i)
		{
			int w = GetMultiLineGridRowWeight(rows[i]);
			if (w > maxPageWeight) w = maxPageWeight;

			// 현재 페이지에 담을 무게를 초과하는 경우, 이전까지의 행들을 하나의 페이지로 확정
			if (curWeightSum + w > maxPageWeight)
			{
				if (curStart < i)
				{
					ranges.push_back({ curStart, i });
					curStart = i;       // 현재 행부터 새 페이지 시작
					curWeightSum = w;   // 현재 행의 무게로 초기화
				}
				else
				{
					// 행 하나 자체가 너무 무거운 경우 강제로 한 페이지 할당
					ranges.push_back({ i, i + 1 });
					curStart = i + 1;
					curWeightSum = 0;
					continue;
				}
			}
			else
			{
				curWeightSum += w;
			}
		}

		// 마지막 페이지에 남은 잔여 행들이 있다면 반드시 추가 (E2 및 히터 누락 방지)
		if (curStart < (int)rows.size())
		{
			ranges.push_back({ curStart, (int)rows.size() });
		}
	}
	else
	{
		// 일반 그리드는 1페이지당 30개 데이터 고정 분할
		int effectiveRows = 30;
		int totalRows = (int)rows.size();
		for (int start = 0; start < totalRows; start += effectiveRows)
		{
			int end = (std::min)(start + effectiveRows, totalRows);
			ranges.push_back({ start, end });
		}
	}

	if (ranges.empty()) ranges.push_back({ 0, (int)rows.size() });
	return ranges;
}

BOOL GridPrintDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	if (m_gridType == (int)EGridType::Device || m_gridType == (int)EGridType::InterLockingData)
	{
		m_isMultiLine = true;
	}
	else
	{
		m_isMultiLine = false;
	}

	if (m_gridType != (int)EGridType::InterLockingData)
	{
		bool bAlreadyHasNo = false;
		if (!m_columnInfos.empty() && m_columnInfos[0].columnName == _T("번호"))
		{
			bAlreadyHasNo = true;
		}

		if (!bAlreadyHasNo && !m_columnInfos.empty())
		{
			ColumnInfo noCol;
			noCol.columnName = _T("번호");
			noCol.columnWidth = 20;
			m_columnInfos.insert(m_columnInfos.begin(), noCol);

			for (size_t i = 0; i < m_rows.size(); ++i)
			{
				CString strNo;
				strNo.Format(_T("%d"), (int)(i + 1));
				m_rows[i].insert(m_rows[i].begin(), strNo);
			}
		}
	}

	SetWindowText(_T("그리드 테이블 인쇄"));

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int dlgWidth = (int)(screenWidth * 0.85);
	int dlgHeight = (int)(screenHeight * 0.85);

	SetWindowPos(NULL, 0, 0, dlgWidth, dlgHeight, SWP_NOMOVE | SWP_NOZORDER);
	CenterWindow();

	CWnd* pOkBtn = GetDlgItem(IDOK);
	if (pOkBtn != nullptr) pOkBtn->ShowWindow(SW_HIDE);

	CWnd* pPreviewStatic = GetDlgItem(IDC_STATIC_PREVIEW);
	if (pPreviewStatic != nullptr) pPreviewStatic->ShowWindow(SW_HIDE);

	// [UI 개선] 예전에는 리소스 템플릿의 작은 버튼 크기(50x14 DLU)를 그대로 둔 채
	// 위치만 옮겨서, 화면의 85%로 확대된 다이얼로그 안에 좁쌀만 한 버튼 4개가
	// 불균일한 간격으로 떠 있는 것처럼 보였음. 버튼을 실제로 키우고, 폰트도 키우고,
	// [이전/다음] - [인쇄/취소] 두 그룹으로 나눠 간격을 통일해 우측 하단에 정렬함.
	prevBtn.SetWindowText(_T("◀ 이전"));
	nextgBtn.SetWindowText(_T("다음 ▶"));

	if (m_btnFont.GetSafeHandle() == nullptr)
	{
		m_btnFont.CreatePointFont(120, _T("맑은 고딕"));
	}
	prevBtn.SetFont(&m_btnFont);
	nextgBtn.SetFont(&m_btnFont);
	printBtn.SetFont(&m_btnFont);
	cancelBtn.SetFont(&m_btnFont);

	const int btnWidth = 130;
	const int btnHeight = 42;
	const int gap = 14;
	const int groupGap = 36;
	const int bottomMargin = 30;

	int btnY = dlgHeight - btnHeight - bottomMargin;
	int cancelX = dlgWidth - btnWidth - bottomMargin;
	int printX = cancelX - gap - btnWidth;
	int nextX = printX - groupGap - btnWidth;
	int prevX = nextX - gap - btnWidth;

	cancelBtn.MoveWindow(cancelX, btnY, btnWidth, btnHeight);
	printBtn.MoveWindow(printX, btnY, btnWidth, btnHeight);
	nextgBtn.MoveWindow(nextX, btnY, btnWidth, btnHeight);
	prevBtn.MoveWindow(prevX, btnY, btnWidth, btnHeight);

	return TRUE;
}

void GridPrintDlg::OnPaint()
{
	CPaintDC dc(this);

	CRect rectClient;
	GetClientRect(&rectClient);

	CRect rectBtn;
	printBtn.GetWindowRect(&rectBtn);
	int bottomMargin = rectBtn.Height() + 60;

	CRect rectPreview(20, 20, rectClient.right - 20, rectClient.bottom - bottomMargin);

	dc.FillSolidRect(&rectPreview, RGB(200, 200, 200));

	CRect rectInner = rectPreview;
	rectInner.DeflateRect(10, 10);

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

	if (!m_rows.empty() && !m_columnInfos.empty())
	{
		std::vector<std::pair<int, int>> pageRanges = GetPageRanges(m_rows, m_gridType);
		int totalPages = (int)pageRanges.size();
		if (m_nCurrentPage >= totalPages) m_nCurrentPage = totalPages - 1;
		if (m_nCurrentPage < 0) m_nCurrentPage = 0;

		int startRow = pageRanges[m_nCurrentPage].first;
		int endRow = pageRanges[m_nCurrentPage].second;

		CFont font;
		int fontSize = 70;
		font.CreatePointFont(fontSize, _T("맑은 고딕"));
		CFont* pOldFont = dc.SelectObject(&font);
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(RGB(0, 0, 0));

		CString headerText;
		switch (m_gridType)
		{
		case (int)EGridType::Track:            headerText = _T("궤도"); break;
		case (int)EGridType::Signal:            headerText = _T("신호기"); break;
		case (int)EGridType::Switch:            headerText = _T("선로전환기"); break;
		case (int)EGridType::Device:            headerText = _T("폐색/건널목/히터/지장물"); break;
		case (int)EGridType::INCard:            headerText = _T("IN Card"); break;
		case (int)EGridType::OUTCard:           headerText = _T("OUT Card"); break;
		case (int)EGridType::SignalCard:        headerText = _T("Signal Card"); break;
		case (int)EGridType::SwitchCard:        headerText = _T("Switch Card"); break;
		case (int)EGridType::LogicVariable:     headerText = _T("로직 변수"); break;
		case (int)EGridType::InterLockingData:  headerText = _T("연동 도표"); break;
		default: headerText = _T(""); break;
		}

		CRect rectHeader(rectA4.left + 30, rectA4.top + 15, rectA4.right - 30, rectA4.top + 45);
		CString headerTitle;
		headerTitle.Format(_T(" 테이블 인쇄 (유형: %s)"), headerText);
		dc.DrawText(headerTitle, &rectHeader, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		CRect rectFooter(rectA4.left + 30, rectA4.bottom - 40, rectA4.right - 30, rectA4.bottom - 15);
		CString footerStr;
		footerStr.Format(_T("- %d / %d 페이지 -"), m_nCurrentPage + 1, totalPages);
		dc.DrawText(footerStr, &rectFooter, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		int tableLeft = rectA4.left + 30;
		int tableTop = rectA4.top + 50;
		int tableWidth = rectA4.Width() - 60;
		int tableHeight = rectA4.Height() - 95;

		int numCols = (int)m_columnInfos.size();

		std::vector<int> rowWeights;
		int pageWeightSum = 0;
		for (int r = startRow; r < endRow; ++r)
		{
			int w = (m_gridType == (int)EGridType::Device || m_gridType == (int)EGridType::InterLockingData) ? GetMultiLineGridRowWeight(m_rows[r]) : 1;
			rowWeights.push_back(w);
			pageWeightSum += w;
		}

		int pageMaxWeight = (m_gridType == (int)EGridType::Device || m_gridType == (int)EGridType::InterLockingData) ? (std::max)(40, pageWeightSum) : 30;
		int unitHeight = tableHeight / (pageMaxWeight + 1);
		int headerRowHeight = unitHeight;

		int totalTableHeight = headerRowHeight;
		for (size_t idx = 0; idx < rowWeights.size(); ++idx)
		{
			totalTableHeight += (unitHeight * rowWeights[idx]);
		}

		CRect rectTableBox(tableLeft, tableTop, tableLeft + tableWidth, tableTop + totalTableHeight);
		dc.FillSolidRect(&rectTableBox, RGB(255, 255, 255));

		CRect rectHeaderBox(tableLeft, tableTop, tableLeft + tableWidth, tableTop + headerRowHeight);
		dc.FillSolidRect(&rectHeaderBox, RGB(235, 235, 235));

		int totalWeight = 0;
		for (const auto& info : m_columnInfos) totalWeight += info.columnWidth;
		if (totalWeight <= 0) totalWeight = 1;

		int currentX = tableLeft;
		for (int c = 0; c < numCols; ++c)
		{
			int colWidth = (int)((double)tableWidth * m_columnInfos[c].columnWidth / totalWeight);
			if (c == numCols - 1) colWidth = (tableLeft + tableWidth) - currentX;

			CRect rectCell(currentX, tableTop, currentX + colWidth, tableTop + headerRowHeight);
			dc.DrawText(m_columnInfos[c].columnName, &rectCell, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
			currentX += colWidth;
		}

		int currentY = tableTop + headerRowHeight;
		for (size_t idx = 0; idx < rowWeights.size(); ++idx)
		{
			int r = startRow + (int)idx;
			int rHeight = unitHeight * rowWeights[idx];
			const auto& rowData = m_rows[r];

			currentX = tableLeft;
			for (int c = 0; c < numCols; ++c)
			{
				int colWidth = (int)((double)tableWidth * m_columnInfos[c].columnWidth / totalWeight);
				if (c == numCols - 1) colWidth = (tableLeft + tableWidth) - currentX;

				CRect rectCell(currentX, currentY, currentX + colWidth, currentY + rHeight);
				CString cellText = (c < (int)rowData.size()) ? rowData[c] : CString(_T(""));

				if (m_isMultiLine)
				{
					// 렌더링 전 \r 제거 및 \n 정규화 처리
					cellText.Replace(_T("\r\n"), _T("\n"));
					cellText.Replace(_T("\r"), _T(""));

					CStringArray lines;
					int curPos = 0;
					CString token = cellText.Tokenize(_T("\n"), curPos);
					while (token != _T(""))
					{
						lines.Add(token);
						token = cellText.Tokenize(_T("\n"), curPos);
					}
					if (lines.GetSize() == 0 && !cellText.IsEmpty()) lines.Add(cellText);

					int lineCount = (int)lines.GetSize();
					TEXTMETRIC tm;
					dc.GetTextMetrics(&tm);
					int fontHeight = tm.tmHeight;

					int totalTextHeight = lineCount * fontHeight;
					int startY = rectCell.top + (rectCell.Height() - totalTextHeight) / 2;
					if (startY < rectCell.top + 2) startY = rectCell.top + 2;

					for (int l = 0; l < lineCount; ++l)
					{
						CRect rectLine(rectCell.left + 4, startY + (l * fontHeight), rectCell.right - 4, startY + ((l + 1) * fontHeight));
						dc.DrawText(lines[l], &rectLine, DT_LEFT | DT_TOP | DT_WORD_ELLIPSIS);
					}
				}
				else
				{
					dc.DrawText(cellText, &rectCell, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
				}

				currentX += colWidth;
			}
			currentY += rHeight;
		}

		CPen gridPen(PS_SOLID, 1, RGB(160, 160, 160));
		CPen* pOldPen = dc.SelectObject(&gridPen);

		dc.MoveTo(tableLeft, tableTop);
		dc.LineTo(tableLeft + tableWidth, tableTop);

		int lineY = tableTop + headerRowHeight;
		dc.MoveTo(tableLeft, lineY);
		dc.LineTo(tableLeft + tableWidth, lineY);

		for (size_t idx = 0; idx < rowWeights.size(); ++idx)
		{
			lineY += (unitHeight * rowWeights[idx]);
			dc.MoveTo(tableLeft, lineY);
			dc.LineTo(tableLeft + tableWidth, lineY);
		}

		currentX = tableLeft;
		dc.MoveTo(currentX, tableTop);
		dc.LineTo(currentX, tableTop + totalTableHeight);

		for (int c = 0; c < numCols; ++c)
		{
			int colWidth = (int)((double)tableWidth * m_columnInfos[c].columnWidth / totalWeight);
			if (c == numCols - 1) colWidth = (tableLeft + tableWidth) - currentX;
			currentX += colWidth;
			dc.MoveTo(currentX, tableTop);
			dc.LineTo(currentX, tableTop + totalTableHeight);
		}

		dc.SelectObject(pOldPen);
		gridPen.DeleteObject();
		dc.SelectObject(pOldFont);
		font.DeleteObject();
	}

	dc.RestoreDC(nSavedDC);
}

void GridPrintDlg::OnBnClickedBtnPrint()
{
	if (m_rows.empty() || m_columnInfos.empty())
	{
		AfxMessageBox(_T("인쇄할 데이터가 없습니다."));
		return;
	}

	CPrintDialog dlg(FALSE);

	if (AfxGetApp()->GetPrinterDeviceDefaults(&dlg.m_pd))
	{
		LPDEVMODE pDevMode = (LPDEVMODE)::GlobalLock(dlg.m_pd.hDevMode);
		if (pDevMode != nullptr)
		{
			pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
			::GlobalUnlock(dlg.m_pd.hDevMode);
		}
	}

	if (dlg.DoModal() == IDOK)
	{
		LPDEVMODE pDevMode = dlg.GetDevMode();
		if (pDevMode != nullptr) pDevMode->dmOrientation = DMORIENT_LANDSCAPE;

		HDC hDC = dlg.CreatePrinterDC();
		if (hDC == nullptr) hDC = dlg.m_pd.hDC;
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
		di.lpszDocName = _T("그리드 테이블 인쇄");

		if (dc.StartDoc(&di) > 0)
		{
			int dpiX = dc.GetDeviceCaps(LOGPIXELSX);
			int dpiY = dc.GetDeviceCaps(LOGPIXELSY);

			std::vector<std::pair<int, int>> pageRanges = GetPageRanges(m_rows, m_gridType);
			int totalPages = (int)pageRanges.size();
			int numCols = (int)m_columnInfos.size();

			int totalWeight = 0;
			for (const auto& info : m_columnInfos) totalWeight += info.columnWidth;
			if (totalWeight <= 0) totalWeight = 1;

			CFont font;
			int fontSize = 70;
			font.CreatePointFont(fontSize, _T("맑은 고딕"), &dc);

			CString headerText;
			switch (m_gridType)
			{
			case (int)EGridType::Track:            headerText = _T("궤도"); break;
			case (int)EGridType::Signal:            headerText = _T("신호기"); break;
			case (int)EGridType::Switch:            headerText = _T("선로전환기"); break;
			case (int)EGridType::Device:            headerText = _T("폐색/건널목/히터/지장물"); break;
			case (int)EGridType::INCard:            headerText = _T("IN Card"); break;
			case (int)EGridType::OUTCard:           headerText = _T("OUT Card"); break;
			case (int)EGridType::SignalCard:        headerText = _T("Signal Card"); break;
			case (int)EGridType::SwitchCard:        headerText = _T("Switch Card"); break;
			case (int)EGridType::LogicVariable:     headerText = _T("로직 변수"); break;
			case (int)EGridType::InterLockingData:  headerText = _T("연동 도표"); break;
			default: headerText = _T(""); break;
			}

			for (int page = 0; page < totalPages; ++page)
			{
				dc.StartPage();
				dc.SetBkMode(TRANSPARENT);
				CFont* pOldFont = dc.SelectObject(&font);

				int marginX = (int)((15.0 / 25.4) * dpiX);
				int marginY = (int)((15.0 / 25.4) * dpiY);
				int pageWidth = dc.GetDeviceCaps(HORZRES) - (marginX * 2);
				int pageHeight = dc.GetDeviceCaps(VERTRES) - (marginY * 2);

				int headerHeight = (int)((12.0 / 25.4) * dpiY);
				int footerHeight = (int)((12.0 / 25.4) * dpiY);

				int tableTop = marginY + headerHeight;
				int tableHeight = pageHeight - headerHeight - footerHeight;

				int startRow = pageRanges[page].first;
				int endRow = pageRanges[page].second;

				std::vector<int> rowWeights;
				int pageWeightSum = 0;
				for (int r = startRow; r < endRow; ++r)
				{
					int w = (m_gridType == (int)EGridType::Device || m_gridType == (int)EGridType::InterLockingData) ? GetMultiLineGridRowWeight(m_rows[r]) : 1;
					rowWeights.push_back(w);
					pageWeightSum += w;
				}

				int pageMaxWeight = (m_gridType == (int)EGridType::Device || m_gridType == (int)EGridType::InterLockingData) ? (std::max)(40, pageWeightSum) : 30;
				int unitHeight = tableHeight / (pageMaxWeight + 1);
				int headerRowHeight = unitHeight;

				int totalTableHeight = headerRowHeight;
				for (size_t idx = 0; idx < rowWeights.size(); ++idx)
				{
					totalTableHeight += (unitHeight * rowWeights[idx]);
				}

				CRect rectHeader(marginX, marginY, marginX + pageWidth, marginY + headerHeight);
				CString headerTitle;
				headerTitle.Format((LPCTSTR)_T("■ 그리드 테이블 인쇄 보고서 (유형: %s)"), headerText);
				dc.DrawText(headerTitle, &rectHeader, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

				CRect rectFooter(marginX, marginY + pageHeight - footerHeight, marginX + pageWidth, marginY + pageHeight);
				CString footerStr;
				footerStr.Format((LPCTSTR)_T("- %d / %d 페이지 -"), page + 1, totalPages);
				dc.DrawText(footerStr, &rectFooter, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				CRect rectTableBox(marginX, tableTop, marginX + pageWidth, tableTop + totalTableHeight);
				dc.FillSolidRect(&rectTableBox, RGB(255, 255, 255));

				CRect rectHeaderBox(marginX, tableTop, marginX + pageWidth, tableTop + headerRowHeight);
				dc.FillSolidRect(&rectHeaderBox, RGB(235, 235, 235));

				int currentX = marginX;
				for (int c = 0; c < numCols; ++c)
				{
					int colWidth = (int)((double)pageWidth * m_columnInfos[c].columnWidth / totalWeight);
					if (c == numCols - 1) colWidth = (marginX + pageWidth) - currentX;

					CRect rectCell(currentX, tableTop, currentX + colWidth, tableTop + headerRowHeight);
					dc.DrawText(m_columnInfos[c].columnName, &rectCell, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					currentX += colWidth;
				}

				int currentY = tableTop + headerRowHeight;
				for (size_t idx = 0; idx < rowWeights.size(); ++idx)
				{
					int r = startRow + (int)idx;
					int rHeight = unitHeight * rowWeights[idx];
					const auto& rowData = m_rows[r];

					currentX = marginX;
					for (int c = 0; c < numCols; ++c)
					{
						int colWidth = (int)((double)pageWidth * m_columnInfos[c].columnWidth / totalWeight);
						if (c == numCols - 1) colWidth = (marginX + pageWidth) - currentX;

						CRect rectCell(currentX, currentY, currentX + colWidth, currentY + rHeight);
						CString cellText = (c < (int)rowData.size()) ? rowData[c] : CString(_T(""));

						if (m_isMultiLine)
						{
							cellText.Replace(_T("\r\n"), _T("\n"));
							cellText.Replace(_T("\r"), _T(""));

							CStringArray lines;
							int curPos = 0;
							CString token = cellText.Tokenize(_T("\n"), curPos);
							while (token != _T(""))
							{
								lines.Add(token);
								token = cellText.Tokenize(_T("\n"), curPos);
							}
							if (lines.GetSize() == 0 && !cellText.IsEmpty()) lines.Add(cellText);

							int lineCount = (int)lines.GetSize();
							TEXTMETRIC tm;
							dc.GetTextMetrics(&tm);
							int fontHeight = tm.tmHeight;

							int totalTextHeight = lineCount * fontHeight;
							int startY = rectCell.top + (rectCell.Height() - totalTextHeight) / 2;
							if (startY < rectCell.top + 2) startY = rectCell.top + 2;

							for (int l = 0; l < lineCount; ++l)
							{
								CRect rectLine(rectCell.left + 4, startY + (l * fontHeight), rectCell.right - 4, startY + ((l + 1) * fontHeight));
								dc.DrawText(lines[l], &rectLine, DT_LEFT | DT_TOP | DT_WORD_ELLIPSIS);
							}
						}
						else
						{
							dc.DrawText(cellText, &rectCell, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
						}

						currentX += colWidth;
					}
					currentY += rHeight;
				}

				CPen gridPen(PS_SOLID, 1, RGB(160, 160, 160));
				CPen* pOldPen = dc.SelectObject(&gridPen);

				dc.MoveTo(marginX, tableTop);
				dc.LineTo(marginX + pageWidth, tableTop);

				int lineY = tableTop + headerRowHeight;
				dc.MoveTo(marginX, lineY);
				dc.LineTo(marginX + pageWidth, lineY);

				for (size_t idx = 0; idx < rowWeights.size(); ++idx)
				{
					lineY += (unitHeight * rowWeights[idx]);
					dc.MoveTo(marginX, lineY);
					dc.LineTo(marginX + pageWidth, lineY);
				}

				currentX = marginX;
				dc.MoveTo(currentX, tableTop);
				dc.LineTo(currentX, tableTop + totalTableHeight);

				for (int c = 0; c < numCols; ++c)
				{
					int colWidth = (int)((double)pageWidth * m_columnInfos[c].columnWidth / totalWeight);
					if (c == numCols - 1) colWidth = (marginX + pageWidth) - currentX;
					currentX += colWidth;
					dc.MoveTo(currentX, tableTop);
					dc.LineTo(currentX, tableTop + totalTableHeight);
				}

				dc.SelectObject(pOldPen);
				gridPen.DeleteObject();
				dc.SelectObject(pOldFont);
				dc.EndPage();
			}

			font.DeleteObject();
			dc.EndDoc();
		}
		dc.Detach();

		AfxMessageBox(_T("그리드 인쇄 작업이 완료되었습니다."));
	}
}

void GridPrintDlg::OnBnClickedBtnPrevPage()
{
	if (m_nCurrentPage > 0)
	{
		m_nCurrentPage--;
		Invalidate();
		UpdateWindow();
	}
}

void GridPrintDlg::OnBnClickedBtnNextPage()
{
	std::vector<std::pair<int, int>> pageRanges = GetPageRanges(m_rows, m_gridType);
	int totalPages = (int)pageRanges.size();

	if (m_nCurrentPage < totalPages - 1)
	{
		m_nCurrentPage++;
		Invalidate();
		UpdateWindow();
	}
}