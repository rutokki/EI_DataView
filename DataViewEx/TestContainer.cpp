#include "stdafx.h"
#include "TestContainer.h"
#include "TestRightView.h"
#include "TestLeftView.h"
#include "resource.h"
//#include "DataViewExView.h"

IMPLEMENT_DYNAMIC(TestContainer, CBCGPWnd)

TestContainer::TestContainer()
	: testLeftView(nullptr)
	, testRightView(nullptr)
{

}

TestContainer::~TestContainer()
{
}

BEGIN_MESSAGE_MAP(TestContainer, CBCGPWnd)
	ON_WM_CREATE()
	//ON_COMMAND(ID_FILE_SAVE, &TestContainer::OnFileSave)
	ON_WM_SIZE()
END_MESSAGE_MAP()

int TestContainer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 1행 2열의 정적 스플리터 생성 (부모 윈도우는 'this')
	if (!m_wndSplitter.CreateStatic(this, 1, 2, WS_CHILD | WS_VISIBLE, AFX_IDW_PANE_FIRST))
	{
		return -1;
	}
	SetControlVisualMode(this);
	// 1. 메인 프레임으로부터 현재 활성 도큐먼트(CDocument) 포인터 얻기
	CDocument* pCurrentDoc = nullptr;
	CFrameWnd* pMainFrame = DYNAMIC_DOWNCAST(CFrameWnd, AfxGetMainWnd());
	if (pMainFrame != nullptr)
	{
		CView* pActiveView = pMainFrame->GetActiveView();
		if (pActiveView != nullptr)
		{
			pCurrentDoc = pActiveView->GetDocument();
		}
	}

	// 2. 뷰 생성을 위한 런타임 컨텍스트 설정 (도큐먼트 연동)
	CCreateContext context;
	context.m_pCurrentDoc = pCurrentDoc; // 도큐먼트 연결!
	context.m_pNewDocTemplate = (pCurrentDoc != nullptr) ? pCurrentDoc->GetDocTemplate() : nullptr;
	context.m_pLastView = nullptr;
	context.m_pCurrentFrame = pMainFrame;

	// 3. 좌측 패인(0행, 0열)에 뷰 생성
	context.m_pNewViewClass = RUNTIME_CLASS(TestLeftView);
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(TestLeftView), CSize(300, 100), &context))
	{
		return -1;
	}

	// 4. 우측 패인(0행, 1열)에 뷰 생성
	context.m_pNewViewClass = RUNTIME_CLASS(TestRightView);
	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(TestRightView), CSize(300, 100), &context))
	{
		return -1;
	}

	// 생성된 뷰 객체 포인터 캐싱
	testLeftView = DYNAMIC_DOWNCAST(TestLeftView, m_wndSplitter.GetPane(0, 0));
	testRightView = DYNAMIC_DOWNCAST(TestRightView, m_wndSplitter.GetPane(0, 1));

	return 0;
}

void TestContainer::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// 컨테이너 크기 변경 시 스플리터 크기 맞춤 조절
	if (m_wndSplitter.GetSafeHwnd() != NULL)
	{
		m_wndSplitter.MoveWindow(0, 0, cx, cy);
	}
}

//void TestContainer::OnFileSave()
//{
//	//계층 구조: TestContainer -> CBCGPTabWnd(GetParent) -> CDataViewExView(GetParent()->GetParent())
//	CWnd* pParentTab = GetParent();
//	if (pParentTab != nullptr)
//	{
//		CWnd* pDataView = pParentTab->GetParent();
//		if (pDataView != nullptr)
//		{
//			// CDataViewExView로 캐스팅하여 앞서 만든 CSV 내보내기/저장 함수 직접 호출
//			CDataViewExView* pExView = DYNAMIC_DOWNCAST(CDataViewExView, pDataView);
//			if (pExView != nullptr)
//			{
//				pExView->OnExportCSV(); // 실제 파일 저장/내보내기 로직 실행
//				return;
//			}
//		}
//	}
//
//}
void TestContainer::LoadRouteData()
{
	// 1. 좌측 뷰에 진로 리스트 데이터 로드
	if (testLeftView != nullptr)
	{
		testLeftView->LoadRouteList();
	}
}

void TestContainer::ChangeRouteSelection(WORD nRteNo)
{
	if (testRightView != nullptr)
	{
		testRightView->UpdateRouteData(nRteNo);
	}
}
BOOL TestContainer::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 파일 열기/저장 명령이 들어오면 도큐먼트로 새어나가는 것을 원천 차단합니다.
	if ((nID == ID_FILE_SAVE || nID == ID_FILE_SAVE_AS || nID == ID_FILE_OPEN || nID == ID_FILE_PRINT || nID == ID_DIFF_TOOL) && nCode == CN_COMMAND)
	{
		// pHandlerInfo가 채워져 있다면 '검사' 요청이므로 실제 실행은 건너뜁니다.
		//if (pHandlerInfo != nullptr)
		//	return TRUE;

		// 부모 창들을 거슬러 올라가며 커스텀 메시지를 받아줄 메인 뷰를 찾습니다.
		CWnd* pParent = GetParent();
		while (pParent != nullptr)
		{
			// SendMessage를 보내고, 메인 뷰가 처리했다면 1을 리턴받습니다.
			if (pParent->SendMessage(WM_CONTAINER_CMD, (WPARAM)nID, 0) == 1)
			{
				return TRUE; // [성공] 도큐먼트로 절대 안 넘어감!
			}
			pParent = pParent->GetParent();
		}
	}

	// 1. 내부에 있는 스플리터나 뷰들이 처리하도록 시도
	if (CBCGPWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	return FALSE;
}