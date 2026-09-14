// TestDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "DataViewEx.h"
#include "afxdialogex.h"
#include "TestDlg.h"


// TestDlg 대화 상자

IMPLEMENT_DYNAMIC(TestDlg, CBCGPDialog)

TestDlg::TestDlg(CWnd* pParent /*=nullptr*/)
	: CBCGPDialog(IDD_TEST_DLG, pParent)
{

}

TestDlg::~TestDlg()
{
}

void TestDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TestDlg, CBCGPDialog)
END_MESSAGE_MAP()


// TestDlg 메시지 처리기

BOOL TestDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	CRect rect;
	GetClientRect(&rect);
	// 1. 스플리터 생성 (다이얼로그를 부모로)
	m_wndSplitter.CreateStatic(this, 1, 2, WS_CHILD | WS_VISIBLE, 1001);

	// 2. 패널 생성 (Context 없이 직접 Create)
	// 다이얼로그는 뷰가 아니므로 CreateView 대신 Create를 직접 호출합니다.
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(TestLeftPane), CSize(rect.Width() / 2, rect.Height()), NULL))
	{
		return FALSE;
	}

	// 3. 우측 패널(0행, 1열)에 그리드 뷰 생성
	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(TestRightPane), CSize(rect.Width() / 2, rect.Height()), NULL))
	{
		return FALSE;
	}

	return TRUE;
}
