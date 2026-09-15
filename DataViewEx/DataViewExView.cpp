// DataViewExView.cpp : implementation of the CDataViewExView class
//

#include "stdafx.h"
#include "DataViewEx.h"
#include "DataViewExDoc.h"
#include "DataViewExView.h"
#include "StructMainData.h"

#include "FontSetting.h"
#include "GridColumnDefine.h"
#include "TagNamePrintDlg.h"
#include "DiffCompareFrame.h"
#include "TestPrintDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 그리드 이름 검색바 컨트롤 ID (이 뷰 안에서만 쓰는 로컬 ID, 다른 자식 컨트롤과 안 겹치게 1090번대 사용)
#define IDC_GRID_SEARCH_EDIT     1090
#define IDC_GRID_SEARCH_NEXT_BTN 1091

// CDataViewExView

IMPLEMENT_DYNCREATE(CDataViewExView, CView)

BEGIN_MESSAGE_MAP(CDataViewExView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CDataViewExView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDataViewExView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CDataViewExView::OnFilePrintPreview)
	ON_MESSAGE(WM_PRINTCLIENT, &CDataViewExView::OnPrintClient)
	ON_COMMAND(ID_FILE_OPEN, &CDataViewExView::OnDataOpen)
	ON_WM_SIZE()
	ON_MESSAGE(WM_CONTAINER_CMD, &CDataViewExView::OnContainerCommand)
	ON_COMMAND(ID_TAG_INCARD, &CDataViewExView::OnTagIncard)
	ON_COMMAND(ID_TAG_OUTCARD, &CDataViewExView::OnTagOutcard)
	ON_COMMAND(ID_DIFF_TOOL, &CDataViewExView::OnDiffTool)
	ON_COMMAND(ID_FILE_SAVE, &CDataViewExView::OnFileSave)
	ON_COMMAND(ID_APP_DIFF, &CDataViewExView::OnAppDiff)
	ON_BN_CLICKED(IDC_GRID_SEARCH_NEXT_BTN, &CDataViewExView::OnSearchNext)
	//	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CDataViewExView construction/destruction

CDataViewExView::CDataViewExView()
{
	m_pDiffFrame = nullptr; // 초기화

}

CDataViewExView::~CDataViewExView()
{
	//if (m_pDiffFrame != nullptr)
	//{
	//	if (::IsWindow(m_pDiffFrame->GetSafeHwnd()))
	//	{
	//		// DestroyWindow() 대신 SendMessage(WM_CLOSE)로 정상 종료 유도
	//		m_pDiffFrame->SendMessage(WM_CLOSE);
	//	}
	//	m_pDiffFrame = nullptr;
	//}
	//CView::OnDestroy();
}

BOOL CDataViewExView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// 그리드 이름 검색 Edit박스에서 Enter를 누르면 "찾기" 버튼을 누른 것과 동일하게 동작시킴.
// 일반 CEdit는 CView 안에서는 Enter 키를 자동으로 처리해주지 않아서 직접 가로채야 함.
BOOL CDataViewExView::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN &&
		m_wndSearchEdit.GetSafeHwnd() != NULL && pMsg->hwnd == m_wndSearchEdit.GetSafeHwnd())
	{
		OnSearchNext();
		return TRUE; // 메시지 소비 (Enter로 인한 경고음/기본 처리 방지)
	}

	return CView::PreTranslateMessage(pMsg);
}

// CDataViewExView drawing

void CDataViewExView::OnDraw(CDC* pDC)
{
	CDataViewExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

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
CBCGPGridCtrl* CDataViewExView::GetActiveGrid()
{
	if (m_wndTabCtrl.GetSafeHwnd() == NULL)
		return nullptr;

	// 현재 선택된 탭의 인덱스 가져오기
	int nActiveTab = m_wndTabCtrl.GetActiveTab();
	if (nActiveTab < 0)
		return nullptr;

	// 해당 탭의 윈도우 포인터 가져오기
	CWnd* pActiveWnd = m_wndTabCtrl.GetTabWnd(nActiveTab);

	// 가져온 윈도우가 CBCGPGridCtrl 또는 그 파생 클래스인지 검사
	// (LDAT 전체 보기 탭 같은 CEdit 컨트롤에서 인쇄 방지)
	if (pActiveWnd != nullptr && pActiveWnd->IsKindOf(RUNTIME_CLASS(CBCGPGridCtrl)))
	{
		return DYNAMIC_DOWNCAST(CBCGPGridCtrl, pActiveWnd);
	}
	return nullptr;
}

// 탭 인덱스를 EGridType으로 매핑. OnPrintGridTable/검색·필터 기능이 공유하는 단일 소스
// (예전엔 이 매핑이 GridPrintDlg.cpp에도 따로 있었고 그리드가 추가될 때 어긋난 적이 있었음).
EGridType CDataViewExView::TabIndexToGridType(int nActiveTab)
{
	switch (nActiveTab)
	{
	case 0: return EGridType::Track;
	case 1: return EGridType::Signal;
	case 2: return EGridType::Switch;
	case 3: return EGridType::Device;
	case 4: return EGridType::INCard;
	case 5: return EGridType::OUTCard;
	case 6: return EGridType::SignalCard;
	case 7: return EGridType::SwitchCard;
	case 8: return EGridType::LogicVariable;
	case 9: return EGridType::InterLockingData;
	case 10: return EGridType::Test;
	default: return EGridType::Track;
	}
}

void CDataViewExView::OnSearchNext()
{
	CString strSearch;
	m_wndSearchEdit.GetWindowText(strSearch);
	strSearch.Trim();

	CBCGPGridCtrl* pGrid = GetActiveGrid();
	if (pGrid == nullptr)
	{
		BCGPMessageBox(_T("검색할 수 있는 그리드 탭이 아닙니다."));
		return;
	}

	// 검색/필터 로직 자체는 CustomBCGGridCtrl::SearchByName()에 구현돼 있음(필터 적용,
	// 다음 찾기 이어하기, 네이티브 필터바 UI 숨김까지 그리드 스스로 처리). IN Card 탭
	// (INCardGridInfo)만 예외적으로 CBCGPGridCtrl을 직접 상속해서 CustomBCGGridCtrl이 아니므로
	// 이 탭에서는 검색을 지원하지 않음을 안내.
	CustomBCGGridCtrl* pCustomGrid = DYNAMIC_DOWNCAST(CustomBCGGridCtrl, pGrid);
	if (pCustomGrid == nullptr)
	{
		BCGPMessageBox(_T("이 탭은 이름 검색을 지원하지 않습니다."));
		return;
	}

	// [수정] 그리드마다 이름 컬럼 위치가 다르므로(Rack No 등이 먼저 오는 경우가 있음)
	// 탭 → EGridType → 실제 이름 컬럼 인덱스 순으로 계산해서 사용 (더 이상 0번 고정 아님).
	int nActiveTab = m_wndTabCtrl.GetActiveTab();
	EGridType gridType = TabIndexToGridType(nActiveTab);
	int nNameColumn = GridColumnDefine::GetNameColumnIndex(gridType);

	bool bFound = pCustomGrid->SearchByName(strSearch, nNameColumn);

	if (!strSearch.IsEmpty() && !bFound)
	{
		BCGPMessageBox(_T("일치하는 항목을 찾을 수 없습니다."));
	}
}

void CDataViewExView::OnFilePrintPreview()
{
	auto Data = StructMainData::GetInstance().GetEIDBStruct();

	// [TEMP-DEBUG] GridPrintDlg UI 확인용으로 이 가드를 잠시 꺼둡니다. 실제 로드된 파일이
	// 없으면 여기서 바로 막혀서 OnPrintGridTable()의 더미데이터 코드까지 도달하지 못했습니다.
	// UI 확인 끝나면 아래 if 블록의 주석을 풀어 원래대로 되돌리세요.
	// 2. 데이터가 비어있는지 확인 (역 정보 ID 등이 0이거나 초기화 상태인 경우)
	//if (Data == nullptr)	 // 혹은 앞서 만든 IsEmpty() 함수 활용
	//{
	//	BCGPMessageBox(_T("출력할 데이터가 없습니다. 먼저 파일을 열어 데이터를 로드해주세요."), MB_ICONWARNING | MB_OK);
	//	return; // 인쇄 로직으로 진입하지 않고 안전하게 차단
	//}

	OnPrintGridTable();
}
BOOL CDataViewExView::OnPreparePrinting(CPrintInfo* pInfo)
{

	// 부모 클래스(CView)의 표준 인쇄 준비 루틴 호출[cite: 1]
	return CView::OnPreparePrinting(pInfo);
}

void CDataViewExView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CView::OnBeginPrinting(pDC, pInfo); //[cite: 1]
}

void CDataViewExView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CView::OnPrint(pDC, pInfo); //[cite: 1]
}

void CDataViewExView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CView::OnEndPrinting(pDC, pInfo); //[cite: 1]
}


// CDataViewExView diagnostics

#ifdef _DEBUG
void CDataViewExView::AssertValid() const
{
	CView::AssertValid();
}

void CDataViewExView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDataViewExDoc* CDataViewExView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDataViewExDoc)));
	return (CDataViewExDoc*)m_pDocument;
}
#endif //_DEBUG


// CDataViewExView message handlers

BOOL CDataViewExView::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

LRESULT CDataViewExView::OnPrintClient(WPARAM wp, LPARAM lp)
{
	if ((lp & PRF_CLIENT) == PRF_CLIENT)
	{
		CDC* pDC = CDC::FromHandle((HDC)wp);
		ASSERT_VALID(pDC);

		OnDraw(pDC);
	}

	return 0;
}

void CDataViewExView::OnNcPaint()
{
	if (!CBCGPVisualManager::GetInstance()->OnDrawViewBorder(this))
	{
		Default();
	}
}

LRESULT CDataViewExView::OnContainerCommand(WPARAM wParam, LPARAM lParam)
{
	UINT nID = (UINT)wParam;

	if (nID == ID_FILE_OPEN)
	{
		OnDataOpen(); // 메인 뷰에 구현된 열기 함수 실행
		return 1;     // 처리 완료 신호(1) 반환
	}
	else if (nID == ID_FILE_SAVE || nID == ID_FILE_SAVE_AS)
	{
		OnFileSave(); // 메인 뷰에 구현된 저장 함수 실행
		return 1;     // 처리 완료 신호(1) 반환
	}
	else if (nID == ID_DIFF_TOOL)
	{
		OnDiffTool(); // 메인 뷰에 구현된 Diff Tool 함수 실행
		return 1;     // 처리 완료 신호(1) 반환
	}
	else if (nID == ID_FILE_PRINT)
	{
		OnFilePrintPreview(); // 메인 뷰에 구현된 인쇄 미리보기 함수 실행
		return 1;             // 처리 완료 신호(1) 반환
	}
	return 0; // 처리 안 함
}


void CDataViewExView::DeleteAttributeTab()
{
	m_wndTabCtrl.EnableToolTips(FALSE);
	m_wndTabCtrl.HideDisabledScrollButtons(TRUE);
	m_wndTabCtrl.EnableTabSwap(FALSE);
}


//void CDataViewExView::ReadIOCardData(CString& filePath) // IOCard.bin 파일 읽기
//{
//	CFile file;
//	CFileException ex;
//	//if (!file.Open(filePath, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary, &ex))
//	//{
//	//	TCHAR szError[1024];
//	//	ex.GetErrorMessage(szError, 1024);
//	//	TRACE(_T("파일 열기 실패: %s, 원인 코드: %d\n"), szError, ex.m_cause);
//	//	BCGPMessageBox(_T("Failed to open IOCard.bin file."));
//	//	return;
//	//}
//	if (file.Open(filePath, CFile::modeRead | CFile::typeBinary))
//	{
//		std::unique_ptr<CARD_INFO>m_TSCCardInfo(new CARD_INFO());
//		file.Read(m_TSCCardInfo.get(), sizeof(CARD_INFO));
//
//		StructMainData::GetInstance().AddTSCCardStruct(std::move(m_TSCCardInfo));
//		//file.Close();
//	}
//	else {
//		BCGPMessageBox(_T("Failed to open IOCard.bin file."));
//	}
//}
//
//void CDataViewExView::ReadLogicVariableData(CString& filePath)
//{
//	CFile file;
//	CFileException ex;
//
//	// 파일 열기 (공유 위반 방지 및 이진 읽기 모드)
//	if (file.Open(filePath, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary, &ex))
//	{
//		// 필요 시 StructMainData의 기존 데이터 초기화 함수 호출
//		// StructMainData::GetInstance().ClearLogicVariables(); 
//
//		ULONGLONG fileSize = file.GetLength();
//		const size_t LOGIC_VARIABLESize = sizeof(LOGIC_VARIABLE);
//
//		// 파일 내용을 블록 단위로 읽기
//		while (file.GetPosition() + LOGIC_VARIABLESize <= fileSize)
//		{
//			// shared_ptr로 동적 할당
//			auto pLogicVar = std::make_shared<LOGIC_VARIABLE>();
//
//			// 실제 메모리 주소(pLogicVar.get())에 직접 읽기
//			if (file.Read(pLogicVar.get(), LOGIC_VARIABLESize) == LOGIC_VARIABLESize)
//			{
//				// StructMainData로 즉시 추가 (Add 함수가 shared_ptr을 받는다고 가정)
//				StructMainData::GetInstance().AddLogicVariable(pLogicVar);
//			}
//			else
//			{
//				break;
//			}
//		}
//		file.Close();
//	}
//	else
//	{
//		TCHAR szError[1024];
//		ex.GetErrorMessage(szError, 1024);
//		BCGPMessageBox(szError);
//	}
//}

void CDataViewExView::updateLayOut()
{
	CRect rectClient;
	GetClientRect(rectClient);

	// 0. 상단에 그리드 이름 검색바를 위한 공간 확보
	int nSearchBarHeight = 0;
	if (m_wndSearchEdit.GetSafeHwnd() != NULL)
	{
		const int nMargin = 4;
		const int nBarHeight = 24;
		const int nBtnWidth = 60;

		int nEditWidth = rectClient.Width() - (nMargin * 3) - nBtnWidth;
		if (nEditWidth < 60) nEditWidth = 60;

		m_wndSearchEdit.SetWindowPos(NULL, rectClient.left + nMargin, rectClient.top + nMargin,
			nEditWidth, nBarHeight, SWP_NOZORDER | SWP_NOACTIVATE);
		m_wndSearchNextBtn.SetWindowPos(NULL, rectClient.left + nMargin + nEditWidth + nMargin, rectClient.top + nMargin,
			nBtnWidth, nBarHeight, SWP_NOZORDER | SWP_NOACTIVATE);

		nSearchBarHeight = nBarHeight + (nMargin * 2);
	}

	// 1. 탭 컨트롤 크기 조절 (검색바 아래부터 채움)
	m_wndTabCtrl.SetWindowPos(NULL, rectClient.left, rectClient.top + nSearchBarHeight,
		rectClient.Width(), rectClient.Height() - nSearchBarHeight, SWP_NOZORDER | SWP_NOACTIVATE);
	m_wndTabCtrl.RedrawWindow();


	UpdateWindow();
}

void CDataViewExView::OnContextMenu(CWnd*, CPoint point)
{
	if (CBCGPPopupMenu::GetSafeActivePopupMenu() != NULL)
	{
		return;
	}

	theApp.ShowPopupMenu(IDR_CONTEXT_MENU, point, this);
}

//void CDataViewExView::OnDataOpen()
//{
//	CFile file;
//	CFileDialog dlg(TRUE, _T("bin"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("연동 데이터 (*.bin)|*_연동_데이터.bin||"));
//	CString fileName = _T("");
//	CString folderPath = _T("");
//	CString strStationName = _T("");
//	CString strIOCardPath = _T("");
//	CString strLogicPath = _T("");
//	if (dlg.DoModal() == IDOK)
//	{
//		fileName = dlg.GetFileName();
//		folderPath = dlg.GetFolderPath();
//		strStationName = GetFileStationName(fileName);
//		strIOCardPath = folderPath + _T("\\") + strStationName + _T("_ioCard.bin");
//		strLogicPath = folderPath + _T("\\") + strStationName + _T("_LogicVariable.Dat");
//		CDataViewExDoc* pDoc = (CDataViewExDoc*)GetDocument();
//		if (fileName.Find(L"_연동_데이터.bin") != -1)
//		{
//			CString fullPath = dlg.GetPathName();
//			DWORD dwAttrs = GetFileAttributes(fullPath);
//			if (dwAttrs == INVALID_FILE_ATTRIBUTES || (dwAttrs & FILE_ATTRIBUTE_DIRECTORY))
//			{
//
//				AfxMessageBox(_T("Invalid file path or file does not exist."));
//				return;
//			}
//			if (GetDocument()->OnOpenDocument(dlg.GetPathName()))
//			{
//				pFrame = (CMainFrame*)AfxGetMainWnd();
//				pFrame->AddRecentFile(dlg.GetPathName());
//				//if (pFrame) pFrame->m_wndWorkSpace.AddRecentFile(strMainFilePath);
//				if (!m_wndTrackInfoGrid.GetSafeHwnd()) {
//					m_wndTrackInfoGrid.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), &m_wndTabCtrl, 1002);
//					m_wndTabCtrl.AddTab(&m_wndTrackInfoGrid, _T("궤도"), 0);
//				}
//				else {
//					m_wndTrackInfoGrid.UpdateTrackData();
//				}
//				//if (!m_wndSignalInfoGrid.GetSafeHwnd()) {
//				//	m_wndSignalInfoGrid.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), &m_wndTabCtrl, 1003);
//				//}
//				//else {
//				//	m_wndSignalInfoGrid.UpdateSignalData();
//				//}
//				//if (!m_wndSwitchInfoGrid.GetSafeHwnd()) {
//				//	m_wndSwitchInfoGrid.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), &m_wndTabCtrl, 1003);
//				//}
//				//else {
//				//	m_wndSwitchInfoGrid.UpdateSignalData();
//				//}
//				//if (!m_wndDeviceInfoGrid.GetSafeHwnd()) {
//				//	m_wndDeviceInfoGrid.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), &m_wndTabCtrl, 1003);
//				//}
//				//else {
//				//	m_wndDeviceInfoGrid.UpdateSignalData();
//				//}
//
//				// 메인 bin 파일만 읽고 나머지 데이터(IOCard, LogicVariable) 읽기 수행
//				//if (pDoc->ReadIOCardData(strIOCardPath))
//				//{
//				//	m_wndInCardInfoGrid.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), &m_wndTabCtrl, 1006);
//				//	m_wndOutCardInfoGrid.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), &m_wndTabCtrl, 1007);
//				//}
//				//ReadLogicVariableData(strLogicPath)
//				pDoc->ReadLogicVariableData(strLogicPath);
//				AfxMessageBox(_T("File opened successfully."));
//
//				m_wndTrackInfoGrid.EnableColumnAutoSize(TRUE);
//				//m_wndSignalInfoGrid.EnableColumnAutoSize(TRUE);
//				//m_wndSwitchInfoGrid.EnableColumnAutoSize(TRUE);
//				//m_wndDeviceInfoGrid.EnableColumnAutoSize(TRUE);
//				//m_wndInCardInfoGrid.EnableColumnAutoSize(TRUE);
//				//m_wndOutCardInfoGrid.EnableColumnAutoSize(TRUE);
//
//				//윈탭 추가
//				//m_wndTabCtrl.AddTab(&m_wndTrackInfoGrid, _T("궤도"), 0);
//				//m_wndTabCtrl.AddTab(&m_wndSignalInfoGrid, _T("신호기"), 1);
//				//m_wndTabCtrl.AddTab(&m_wndSwitchInfoGrid, _T("선로전환기"), 2);
//				//m_wndTabCtrl.AddTab(&m_wndDeviceInfoGrid, _T("폐색,건널목,히터,지장물"), 3);
//				//m_wndTabCtrl.AddTab(&m_wndInCardInfoGrid, _T("IN Card"), 4);
//				//m_wndTabCtrl.AddTab(&m_wndOutCardInfoGrid, _T("OUT Card"), 5);
//			}
//			else
//			{
//				AfxMessageBox(_T("Failed to open the file."));
//			}
//		}
//	}
//}
void CDataViewExView::OnDataOpen()
{
	CFileDialog dlg(TRUE, _T("bin"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("연동 데이터 (*.bin)|*_연동_데이터.bin||"));

	if (dlg.DoModal() == IDOK)
	{
		CString strFullPath = dlg.GetPathName();

		// Document의 OnOpenDocument를 직접 호출하거나 AfxGetApp()->OpenDocumentFile을 호출합니다.
		// 여기서는 현재 문서를 여는 표준 방식을 사용합니다.
		//if (GetDocument()->OnOpenDocument(strFullPath))
		//{
			// 1. 화면(그리드) 데이터 갱신
			//UpdateGridsData(); // 그리드에 데이터 채우고 Invalidate 하는 함수
			//if (!m_wndTrackInfoGrid.GetSafeHwnd()) {
			//	m_wndTrackInfoGrid.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), &m_wndTabCtrl, 1002);
			//	m_wndTabCtrl.AddTab(&m_wndTrackInfoGrid, _T("궤도"), 0);
			//}
			//else {
			//	m_wndTrackInfoGrid.UpdateTrackData();
			//}
		AfxGetApp()->OpenDocumentFile(dlg.GetPathName());
		// 2. 최근 파일 목록에 추가 (전체 경로 전달)
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) {
			pFrame->AddRecentFile(strFullPath); // 본인의 workspacebar 객체명
		}



	}
	//else
	//{
	//	AfxMessageBox(_T("파일을 여는데 실패했습니다."));
	//}
}
//void CDataViewExView::OnInitialUpdate()
//{
//	CView::OnInitialUpdate();
//
//	// 1. 최초 1회 그리드 및 탭 껍데기 생성 (중복 생성 방지 포함)
//	SetupGrids();
//
//	// 2. Document에 저장된 데이터 로드 상태(플래그)를 확인하고 그리드 데이터 채우기
//	UpdateGridsData();
//}

void CDataViewExView::OnInitialUpdate()
{
	//FontSetting::Initialize();
	//VLDEnable();
	CRect rectClient;
	GetClientRect(rectClient);
	if (!m_wndTabCtrl.GetSafeHwnd()) {
		m_wndTabCtrl.Create(CBCGPTabWnd::STYLE_3D_ONENOTE, rectClient, this, 1001, CBCGPTabWnd::LOCATION_TOP);
		m_wndTabCtrl.SetFont(&FontSetting::FontDungGeunMo);
		m_wndTabCtrl.Invalidate();
		DeleteAttributeTab();
	}
	if (!m_wndSearchEdit.GetSafeHwnd())
	{
		// 그리드 이름 검색("찾기") 바. 실제 위치/크기는 updateLayOut()에서 잡음.
		CRect rectDummy(0, 0, 0, 0);
		m_wndSearchEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, rectDummy, this, IDC_GRID_SEARCH_EDIT);
		m_wndSearchEdit.SetFont(&FontSetting::FontDungGeunMo);
		// 빈 칸일 때 회색으로 안내 문구 표시 (입력값 자체에는 영향 없음, 이름만 검색됨을 안내)
		m_wndSearchEdit.SendMessage(EM_SETCUEBANNER, 0, (LPARAM)(LPCWSTR)_T("이름으로 검색"));

		m_wndSearchNextBtn.Create(_T("찾기"), WS_CHILD | WS_VISIBLE, rectDummy, this, IDC_GRID_SEARCH_NEXT_BTN);
		m_wndSearchNextBtn.SetFont(&FontSetting::FontDungGeunMo);
	}
	SetupGrids();

	// 방금 만든 검색바/탭 컨트롤 크기를 첫 화면에서부터 바로 맞춤 (OnSize가 아직 안 불렸을 수 있음)
	updateLayOut();

	UpdateGridsData();
	//그리드 생성 및 초기화
	//CBCGPGridCtrl* pGrid;
	//pGrid = new TrackGridInfo();
	//pGrid->GetColumnsInfo().SetOwnerList(pGrid);


	//m_wndTabCtrl.AddTab(&m_wndInCardInfoGrid, _T("이상x"), 3);
	//m_wndTabCtrl.AddTab(&m_wndOutCardInfoGrid, _T("타역"), 4);
	//m_wndTabCtrl.AddTab(&m_wndPDOCardInfoGrid, _T("PDO카드"), 5);
	//m_wndTabCtrl.AddTab(&m_wndLogicVariableGrid, _T("로직변수"), 6);


	CView::OnInitialUpdate();


}

void CDataViewExView::OnSize(UINT nType, int cx, int cy)
{
	//CView::OnSize(nType, cx, cy);

	if (m_wndTabCtrl.GetSafeHwnd() != NULL)
	{
		updateLayOut();

	}

	UpdateWindow();
}

void CDataViewExView::SetupGrids()
{
	// 이미 첫 번째 그리드의 핸들이 존재한다면(생성되었다면) 즉시 리턴하여 재생성 방지
	if (m_wndTrackInfoGrid.GetSafeHwnd() != NULL)
		return;

	// 탭 컨트롤 초기화 (혹시 모를 찌꺼기 방지)
	m_wndTabCtrl.RemoveAllTabs();


	// 각 그리드 생성 (Create) - 부모는 탭 컨트롤(&m_wndTabCtrl)
	SetRedraw(FALSE);

	CRect rectDummy;
	GetClientRect(rectDummy);

	m_wndTrackInfoGrid.Create(WS_CHILD | WS_VISIBLE, rectDummy, &m_wndTabCtrl, 1002);
	m_wndSignalInfoGrid.Create(WS_CHILD | WS_VISIBLE, rectDummy, &m_wndTabCtrl, 1003);
	m_wndSwitchInfoGrid.Create(WS_CHILD | WS_VISIBLE, rectDummy, &m_wndTabCtrl, 1004);
	m_wndDeviceInfoGrid.Create(WS_CHILD | WS_VISIBLE, rectDummy, &m_wndTabCtrl, 1005);
	m_wndInCardInfoGrid.Create(WS_CHILD | WS_VISIBLE, rectDummy, &m_wndTabCtrl, 1006);
	m_wndOutCardInfoGrid.Create(WS_CHILD | WS_VISIBLE, rectDummy, &m_wndTabCtrl, 1007);
	m_wndLogicVariableGrid.Create(WS_CHILD | WS_VISIBLE, rectDummy, &m_wndTabCtrl, 1008);
	m_wndSignalCardInfoGrid.Create(WS_CHILD | WS_VISIBLE, rectDummy, &m_wndTabCtrl, 1009);
	m_wndSwitchCardInfoGrid.Create(WS_CHILD | WS_VISIBLE, rectDummy, &m_wndTabCtrl, 1010);
	//m_wndEditLDat.Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_VSCROLL | WS_HSCROLL | ES_READONLY,
	//	rectDummy, &m_wndTabCtrl, 1009); // -LDAT 파일 읽기 안씀-
	m_wndInterLockGird.Create(WS_CHILD | WS_VISIBLE, rectDummy, &m_wndTabCtrl, 1010);
	if (m_wndTestContainer.GetSafeHwnd() == NULL)
	{
		m_wndTestContainer.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP, rectDummy, &m_wndTabCtrl, 1011);

	}

	m_wndTrackInfoGrid.SetVisualManagerColorTheme(TRUE, TRUE);
	m_wndTrackInfoGrid.EnableColumnAutoSize(TRUE);
	m_wndSignalInfoGrid.EnableColumnAutoSize(TRUE);
	m_wndSwitchInfoGrid.EnableColumnAutoSize(TRUE);
	m_wndDeviceInfoGrid.EnableColumnAutoSize(TRUE);
	m_wndInCardInfoGrid.EnableColumnAutoSize(TRUE);
	m_wndOutCardInfoGrid.EnableColumnAutoSize(TRUE);
	m_wndLogicVariableGrid.EnableColumnAutoSize(TRUE);
	m_wndInterLockGird.EnableColumnAutoSize(TRUE);
	m_wndDeviceInfoGrid.EnableTextOverflowing(FALSE);
	m_wndSignalCardInfoGrid.EnableColumnAutoSize(TRUE);
	m_wndSwitchCardInfoGrid.EnableColumnAutoSize(TRUE);
	// 탭에 각 그리드 추가
	m_wndTabCtrl.AddTab(&m_wndTrackInfoGrid, _T("궤도"), 0);
	m_wndTabCtrl.AddTab(&m_wndSignalInfoGrid, _T("신호기"), 1);
	m_wndTabCtrl.AddTab(&m_wndSwitchInfoGrid, _T("선로전환기"), 2);
	m_wndTabCtrl.AddTab(&m_wndDeviceInfoGrid, _T("폐색/건널목/히터/지장물"), 3);
	m_wndTabCtrl.AddTab(&m_wndInCardInfoGrid, _T("IN Card"), 4);
	m_wndTabCtrl.AddTab(&m_wndOutCardInfoGrid, _T("OUT Card"), 5);
	m_wndTabCtrl.AddTab(&m_wndSignalCardInfoGrid, _T("Signal Card"), 6);
	m_wndTabCtrl.AddTab(&m_wndSwitchCardInfoGrid, _T("Switch Card"), 7);
	m_wndTabCtrl.AddTab(&m_wndLogicVariableGrid, _T("로직 변수"), 8);
	m_wndTabCtrl.AddTab(&m_wndInterLockGird, _T("연동 도표"), 9);
	m_wndTabCtrl.AddTab(&m_wndTestContainer, _T("합동 시험표"), 10);
	//m_wndTabCtrl.AddTab(&m_wndEditLDat, _T("LDAT 전체 보기"), 9);

	SetRedraw(TRUE);
}

void CDataViewExView::UpdateGridsData()
{
	CDataViewExDoc* pDoc = GetDocument();
	if (!pDoc) return;

	//  모든 그리드의 기존 데이터  지우기
	m_wndTrackInfoGrid.RemoveAll();
	m_wndSignalInfoGrid.RemoveAll();
	m_wndSwitchInfoGrid.RemoveAll();
	m_wndDeviceInfoGrid.RemoveAll();
	m_wndInCardInfoGrid.RemoveAll();
	m_wndOutCardInfoGrid.RemoveAll();
	m_wndLogicVariableGrid.RemoveAll();
	m_wndInterLockGird.RemoveAll();
	m_wndSignalCardInfoGrid.RemoveAll();
	m_wndSwitchCardInfoGrid.RemoveAll();
	// (_연동_데이터.bin) 성공 시 궤도/신호기/전환기 등 업데이트
	if (pDoc->m_bMainDataLoaded)
	{

		// 각 그리드 클래스 내부에 정의된 업데이트 함수 호출
		m_wndTrackInfoGrid.UpdateTrackData(); // 궤도 데이터 업데이트
		m_wndSignalInfoGrid.UpdateSignalData(); // 신호기 데이터 업데이트
		m_wndSwitchInfoGrid.UpdateSwitchData(); // 전환기 데이터 업데이트
		m_wndDeviceInfoGrid.UpdateDeviceData(); // 폐색/건널목/히터/지장물 데이터 업데이트
		m_wndInterLockGird.loadInterLockData(); // 연동 도표 데이터 업데이트
		m_wndTestContainer.LoadRouteData();
	}

	// 3. IOCard 데이터 성공 시 IN/OUT 그리드 업데이트
	if (pDoc->m_bIOCardLoaded)
	{
		m_wndInCardInfoGrid.UpdateInCardData();
		m_wndOutCardInfoGrid.UpdateOutCardData();
		m_wndSignalCardInfoGrid.UpdateSignalCardData();
		m_wndSwitchCardInfoGrid.UpdateSwitchCardData();
	}

	// 4. Logic 데이터 성공 시 로직 그리드 업데이트 (필요한 경우 추가)
	if (pDoc->m_bLogicLoaded)
	{

		m_wndLogicVariableGrid.UpdateLogicData();
	}
	// 5. LDAT 파일 로드 성공 (제거 필요)
/*	if (pDoc->m_bLDatLoaded)
	{
		UpdateLDatEditData();
	}*/

	// 5. 화면 레이아웃 및 탭 컨트롤 강제 갱신 (화면 다시 그리기)
	m_wndTabCtrl.Invalidate();
	m_wndTabCtrl.UpdateWindow();

	// 회원님이 기존에 만들어두신 화면 리사이즈/레이아웃 재배치 함수 호출
	updateLayOut();
}

//Zvoid CDataViewExView::UpdateLDatEditData()
//{
//	if (m_wndEditLDat.GetSafeHwnd() == NULL)
//		return;
//
//	CString strTotalContent = _T("");
//
//	// StructMainData 싱글톤 인스턴스에서 LDAT 파일 컨텐츠 목록 가져오기
//	const auto& ldatList = StructMainData::GetInstance().GetLDatFileContents();
//
//	for (const auto& ldatFile : ldatList)
//	{
//		strTotalContent += _T("========================================\r\n");
//		strTotalContent += _T("[파일 명]: ") + ldatFile->fileName + _T("\r\n");
//		strTotalContent += _T("========================================\r\n");
//
//		// 파일 내부의 각 라인 스트링 결합
//		for (const CString& line : ldatFile->contents)
//		{
//			strTotalContent += line + _T("\r\n");
//		}
//
//		strTotalContent += _T("\r\n\r\n"); // 파일 구분용 빈 줄
//	}
//
//	// BCGP 에디트 컨트롤에 텍스트 반영
//	m_wndEditLDat.SetWindowText(strTotalContent);
//}

void CDataViewExView::OnTagIncard()
{
	std::vector<CString> names;
	bool bIsOutCard = false;
	CWnd* pTargetGrid = &m_wndInCardInfoGrid;   // 4번 탭: IN Card
	if (pTargetGrid != nullptr)
	{
		CBCGPGridCtrl* pGrid = (CBCGPGridCtrl*)pTargetGrid;

		// CBCGPGridCtrl 행 개수 구하기
		int nRowCount = pGrid->GetRowCount();
		for (int i = 0; i < nRowCount; ++i)
		{
			// 특정 행(i) 객체 얻기
			CBCGPGridRow* pRow = pGrid->GetRow(i);
			if (pRow == nullptr) continue;

			// 카드 구분용 공백 행 판별: Rack No(0번 컬럼)가 비어있거나 0이면
			// LoadAllInCardData()에서 추가한 빈 줄이므로 인쇄 데이터에서 제외
			CBCGPGridItem* pRackItem = pRow->GetItem(0);
			CString strRackNo = (pRackItem != nullptr) ? (CString)pRackItem->GetValue() : (CString)_T("");
			if (strRackNo.IsEmpty() || strRackNo == (CString)_T("0"))
			{
				continue;
			}

			// 5번째 컬럼(인덱스 4)의 셀 가져오기 (0부터 시작하므로 4가 5번째 컬럼)
			CBCGPGridItem* pItem = pRow->GetItem(5);
			if (pItem != nullptr)
			{
				// 셀의 텍스트 값을 CString으로 추출
				CString strName = pItem->GetValue();
				if (strName.IsEmpty())
				{
					names.push_back(_T(""));
				}
				else {
					names.push_back(strName);
				}
			}
		}
	}

	// 4번 또는 5번 탭이 아니거나 추출된 데이터가 없을 경우의 방어 코드
	if (names.empty())
	{
		AfxMessageBox(_T("현재 탭에 인쇄할 IO카드 표찰 데이터가 존재하지 않습니다.\n(IO카드 탭에서만 지원됩니다.)"));
		return;
	}

	// 다이얼로그 객체 생성 및 데이터 전달 후 모달 팝업 실행
	TagNamePrintDlg printDlg;
	printDlg.tagName = names;
	printDlg.m_isOutCard = bIsOutCard;
	printDlg.DoModal();
}

void CDataViewExView::OnTagOutcard()
{
	std::vector<CString> names;
	bool bIsOutCard = true;
	CWnd* pTargetGrid = &m_wndOutCardInfoGrid;   // 4번 탭: IN Card
	if (pTargetGrid != nullptr)
	{
		CBCGPGridCtrl* pGrid = (CBCGPGridCtrl*)pTargetGrid;

		// CBCGPGridCtrl 행 개수 구하기
		int nRowCount = pGrid->GetRowCount();
		for (int i = 0; i < nRowCount; ++i)
		{
			// 특정 행(i) 객체 얻기
			CBCGPGridRow* pRow = pGrid->GetRow(i);
			if (pRow == nullptr) continue;

			// 카드 구분용 공백 행 판별: Rack No(0번 컬럼)가 비어있거나 0이면
			// (OutCard 쪽 그리드도 동일하게 빈 줄을 추가했다면) 인쇄 데이터에서 제외
			CBCGPGridItem* pRackItem = pRow->GetItem(0);
			CString strRackNo = (pRackItem != nullptr) ? (CString)pRackItem->GetValue() : (CString)_T("");
			if (strRackNo.IsEmpty() || strRackNo == (CString)_T("0"))
			{
				continue;
			}

			// 5번째 컬럼(인덱스 4)의 셀 가져오기 (0부터 시작하므로 4가 5번째 컬럼)
			CBCGPGridItem* pItem = pRow->GetItem(5);
			if (pItem != nullptr)
			{
				// 셀의 텍스트 값을 CString으로 추출
				CString strName = pItem->GetValue();
				if (strName.IsEmpty())
				{
					names.push_back(_T(""));
				}
				else {
					names.push_back(strName);
				}
			}
		}
	}

	// 4번 또는 5번 탭이 아니거나 추출된 데이터가 없을 경우의 방어 코드
	if (names.empty())
	{
		AfxMessageBox(_T("현재 탭에 인쇄할 태그 데이터가 존재하지 않습니다.\n(INCard 또는 OUTCard만 지원됩니다.)"));
		return;
	}

	// 다이얼로그 객체 생성 및 데이터 전달 후 모달 팝업 실행
	TagNamePrintDlg printDlg;
	printDlg.tagName = names;
	printDlg.m_isOutCard = bIsOutCard;
	printDlg.DoModal();
}
void CDataViewExView::OnDiffTool()
{
	// 1. 이미 비교창이 열려있다면 앞으로 가져오기 (중복 생성 방지)
	if (m_pDiffFrame != nullptr && m_pDiffFrame->GetSafeHwnd() != NULL)
	{
		m_pDiffFrame->SetForegroundWindow();
		return;
	}

	// 2. 새로 생성
	m_pDiffFrame = new DiffCompareFrame();
	if (m_pDiffFrame == nullptr)
	{
		AfxMessageBox(_T("Failed to create DiffCompareFrame"));
		return;
	}

	// 프레임 윈도우 포인터 주소를 DiffCompareFrame에 전달
	m_pDiffFrame->m_ppParentPtr = &m_pDiffFrame;

	// 화면 중앙에 띄우기 위한 좌표 계산
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int w = (int)(screenWidth * 0.8);
	int h = (int)(screenHeight * 0.8);
	CRect rect((screenWidth - w) / 2, (screenHeight - h) / 2, (screenWidth + w) / 2, (screenHeight + h) / 2);

	// 3. 독립 팝업 프레임 창 생성
	if (!m_pDiffFrame->Create(NULL, _T("설정 파일 비교 (Diff Compare)"), WS_OVERLAPPEDWINDOW, rect, NULL))
	{
		delete m_pDiffFrame;
		m_pDiffFrame = nullptr;
		AfxMessageBox(_T("비교창 생성에 실패했습니다."));
		return;
	}

	m_pDiffFrame->ShowWindow(SW_SHOW);
	m_pDiffFrame->UpdateWindow();
}
void CDataViewExView::OnExportCSV()
{
	// 1. 활성화된 그리드 가져오기
	CBCGPGridCtrl* pGrid = GetActiveGrid();
	pGrid->GetColumnCount(); // 컬럼 수 확인 (디버깅용)
	pGrid->GetRowCount();    // 행 수 확인 (디버깅용
	if (pGrid == nullptr || pGrid->GetRowCount() == 0 || pGrid->GetColumnCount() == 0)
	{
		AfxMessageBox(_T("저장할 데이터가 없습니다."));
		return;
	}

	// 2. CSV 구분자를 콤마(,)로 설정
	pGrid->SetExportTextSeparator(CBCGPGridCtrl::Format_CSV, _T(","));

	CString strText;
	// [핵심 1] Format_CSV 와 EF_IncludeHeader를 같이 넘겨야 컬럼 헤더가 포함됩니다.
	DWORD dwFlags = CBCGPGridCtrl::Format_CSV | CBCGPGridCtrl::EF_IncludeHeader;
	pGrid->ExportToCSV(strText, dwFlags);

	if (strText.IsEmpty())
	{
		AfxMessageBox(_T("저장할 데이터가 없습니다."));
		return;
	}

	// 3. 파일 저장 다이얼로그 호출
	CFileDialog dlg(FALSE, _T("csv"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("CSV 파일 (*.csv)|*.csv|모든 파일 (*.*)|*.*||"));
	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();
		CFile file; // CStdioFile 대신 CFile 사용 (텍스트 모드 줄바꿈 중복 변환 원천 차단)

		// [핵심 2] 바이트 모드로 파일 생성 및 오픈
		if (file.Open(strPath, CFile::modeCreate | CFile::modeWrite))
		{
			// 엑셀(Excel)에서 한글이 깨지지 않도록 UTF-8 BOM(0xEF, 0xBB, 0xBF) 먼저 기록
			unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
			file.Write(bom, 3);

			// CString(유니코드)을 UTF-8 바이트 스트림으로 안전하게 변환
			CT2CA pszUtf8(strText, CP_UTF8);
			file.Write(pszUtf8, (UINT)strlen(pszUtf8));

			file.Close();
			AfxMessageBox(_T("CSV 파일 저장이 완료되었습니다."));
		}
		else
		{
			AfxMessageBox(_T("파일을 생성하거나 열 수 없습니다."));
		}
	}
}
void CDataViewExView::OnFileSave()
{
	int nActiveTab = m_wndTabCtrl.GetActiveTab();
	CDataViewExDoc* pDoc = GetDocument();
	if (nActiveTab == 10 || pDoc == nullptr)
	{
		BCGPMessageBox(_T("현재 탭에서는 저장 기능이 실행되지 않습니다."));
		return;
	}
	OnExportCSV();
}
void CDataViewExView::OnPrintGridTable()
{
	int nActiveTab = m_wndTabCtrl.GetActiveTab();
	CDataViewExDoc* pDoc = GetDocument();
	if (nActiveTab <= -1 || pDoc == nullptr)
	{
		BCGPMessageBox(_T("활성화된 탭이 없거나 표가 유효하지 않습니다."));
		return;
	}
	if (nActiveTab == 10)
	{
		TestPrintDlg dlg;
		if (!dlg.HasPrintableData())
		{
			AfxMessageBox(_T("인쇄할 시험표 데이터가 없습니다."));
			return;
		}
		dlg.SetStationName(pDoc->StationName);
		dlg.DoModal();
		return;
	}

	CBCGPGridCtrl* pGrid = GetActiveGrid();
	if (pGrid == nullptr)
	{
		BCGPMessageBox(_T("인쇄할 데이터가 없습니다."));
		return;
	}

	// 1. 현재 활성화된 탭의 인덱스 가져오기

	// 2. 탭 인덱스를 EGridType ENUM으로 매핑 (검색/필터 기능과 공유하는 단일 매핑 함수)
	EGridType gridType = TabIndexToGridType(nActiveTab);

	// [추가] Test 탭(인덱스 8)인 경우 TestPrintDlg 실행 후 종료


	// GridColumnDefine으로부터 해당 탭에 맞는 컬럼 정의 가져오기
	std::vector<ColumnInfo> columnInfos = GridColumnDefine::GetColumnInfo(gridType);
	if (columnInfos.empty())
	{
		AfxMessageBox(_T("현재 탭에 정의된 컬럼 정보가 없습니다."));
		return;
	}

	// 5. 헤더 이름 목록 추출 (미리 메모리 예약으로 최적화)
	std::vector<CString> headers;
	headers.reserve(columnInfos.size());
	for (const auto& info : columnInfos)
	{
		headers.push_back(info.columnName);
	}

	// 6. GridPrintDlg 객체를 스택 대신 힙(std::unique_ptr)에 생성
	auto pDlg = std::make_unique<GridPrintDlg>();
	pDlg->m_columnInfos = std::move(columnInfos); // 소유권 이전으로 복사 비용 제거
	pDlg->m_gridType = (int)gridType;

	// 7. 지역 변수 rows 대신 다이얼로그 내부 멤버(pDlg->m_rows)에 직접 데이터 추출
	if (!ExtractGridData(pGrid, headers, pDlg->m_rows))
	{
		// [TEMP-DEBUG] 실제 DB 데이터가 없어도 GridPrintDlg UI(버튼 배치/폰트 등)를 확인할 수 있게
		// 더미 데이터를 채워서 계속 진행합니다. UI 확인이 끝나면 이 블록을 지우고 원래대로
		// (AfxMessageBox + return) 되돌리세요.
		FillDummyRowsForPrintPreview(pDlg->m_columnInfos, pDlg->m_rows);
	}

	// 8. 모달 다이얼로그 실행
	pDlg->DoModal();
}
// [TEMP-DEBUG] GridPrintDlg UI 확인용 더미 데이터 생성 함수. 확인 끝나면 OnPrintGridTable의
// 호출부와 함께 이 함수도 삭제하세요.
void CDataViewExView::FillDummyRowsForPrintPreview(const std::vector<ColumnInfo>& columnInfos, std::vector<std::vector<CString>>& rows)
{
	rows.clear();
	const int nDummyRowCount = 12;
	for (int r = 0; r < nDummyRowCount; ++r)
	{
		std::vector<CString> row;
		row.reserve(columnInfos.size());
		for (size_t c = 0; c < columnInfos.size(); ++c)
		{
			CString cell;
			cell.Format(_T("샘플%d-%d"), r + 1, (int)c + 1);
			row.push_back(cell);
		}
		rows.push_back(row);
	}
}
bool CDataViewExView::ExtractGridData(CBCGPGridCtrl* pGrid, const std::vector<CString>& headers, std::vector<std::vector<CString>>& rows)
{
	if (pGrid == nullptr)
	{
		OutputDebugString(_T("[ExtractGridData] pGrid is nullptr\n"));
		return false;
	}

	int nCols = (int)headers.size();
	int nRows = pGrid->GetRowCount();

	CString dbgInit;
	dbgInit.Format(_T("[ExtractGridData] 그리드 총 행 수(GetRowCount): %d, 정의된 컬럼 수: %d\n"), nRows, nCols);
	OutputDebugString(dbgInit);

	rows.clear();
	rows.reserve(nRows);

	for (int r = 0; r < nRows; ++r)
	{
		CBCGPGridRow* pRow = pGrid->GetRow(r);
		if (pRow == nullptr)
		{
			CString dbgNull;
			dbgNull.Format(_T("[ExtractGridData] 행 %d: pRow가 nullptr입니다.\n"), r);
			OutputDebugString(dbgNull);
			continue;
		}

		int nItemCount = pRow->GetItemCount();
		BOOL bIsGroup = pRow->IsGroup();

		// 1번 컬럼 명칭 확인
		CString firstColStr = _T("");
		if (nItemCount > 1)
		{
			CBCGPGridItem* pFirstItem = pRow->GetItem(0);
			if (pFirstItem != nullptr)
			{
				_variant_t var = pFirstItem->GetValue();
				if (var.vt != VT_EMPTY && var.vt != VT_NULL)
				{
					firstColStr = (LPCTSTR)(_bstr_t)var;
				}
			}
		}

		// 각 행의 상태 상세 로그 출력
		CString dbgRowInfo;
		dbgRowInfo.Format(_T("[ExtractGridData] 행 [%d] - IsGroup: %d, ItemCount: %d, 1번컬럼명칭: [%s]\n"),
			r, bIsGroup, nItemCount, (LPCTSTR)firstColStr);
		OutputDebugString(dbgRowInfo);

		// [필터링 조건 및 스킵 사유 추적]
		if (nItemCount <= 1)
		{
			OutputDebugString(_T("  -> [스킵] 아이템 개수가 1개 이하임 (nItemCount <= 1)\n"));
			continue;
		}

		if (bIsGroup && firstColStr.IsEmpty())
		{
			OutputDebugString(_T("  -> [스킵] 그룹 행이면서 1번 컬럼이 비어있음 (그룹 헤더)\n"));
			continue;
		}

		if (firstColStr.IsEmpty())
		{
			OutputDebugString(_T("  -> [스킵] 1번 컬럼(명칭)이 비어있음\n"));
			continue;
		}

		// 정상 데이터 행 수집
		std::vector<CString> rowData;
		rowData.reserve(nCols);

		for (int c = 0; c < nCols; ++c)
		{
			CString cellStr = _T("");
			if (c < nItemCount)
			{
				CBCGPGridItem* pItem = pRow->GetItem(c);
				if (pItem != nullptr)
				{
					_variant_t var = pItem->GetValue();
					if (var.vt != VT_EMPTY && var.vt != VT_NULL)
					{
						cellStr = (LPCTSTR)(_bstr_t)var;
					}
				}
			}
			rowData.push_back(cellStr);
		}
		rows.push_back(rowData);

		CString dbgSuccess;
		dbgSuccess.Format(_T("  -> [성공] [%s] 행 데이터 수집 완료 (누적 수집된 행: %d개)\n"), (LPCTSTR)firstColStr, (int)rows.size());
		OutputDebugString(dbgSuccess);
	}

	CString dbgResult;
	dbgResult.Format(_T("[ExtractGridData] 최종 수집된 유효 행 데이터 총 개수: %d\n"), (int)rows.size());
	OutputDebugString(dbgResult);

	return !rows.empty();
}
void CDataViewExView::OnAppDiff()
{
	OnDiffTool();
}

//void CDataViewExView::OnClose()
//{
//	CBCGPGridView::OnDestroy();
//	if (m_pDiffFrame != nullptr && ::IsWindow(m_pDiffFrame->GetSafeHwnd())) {
//		m_pDiffFrame->SendMessage(WM_CLOSE);
//
//		m_pDiffFrame = nullptr;
//	}
//	CBCGPGridView::OnClose();
//}

void CDataViewExView::OnDestroy()
{
	CBCGPGridView::OnDestroy();
	if (m_pDiffFrame != nullptr && ::IsWindow(m_pDiffFrame->GetSafeHwnd())) {
		m_pDiffFrame->SendMessage(WM_CLOSE);

		m_pDiffFrame = nullptr;
	}

}
