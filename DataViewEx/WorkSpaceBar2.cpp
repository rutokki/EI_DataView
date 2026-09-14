// workspace2.cpp : implementation of the CWorkSpaceBar2 class
//

#include "stdafx.h"
#include "DataViewEx.h"
#include "WorkSpaceBar2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar2

BEGIN_MESSAGE_MAP(CWorkSpaceBar2, CBCGPDialogBar)
	ON_UPDATE_COMMAND_UI(IDC_CHECK_DLGBAR, &CWorkSpaceBar2::OnUpdateCheckBox)
	ON_MESSAGE(WM_INITDIALOG, &CWorkSpaceBar2::HandleInitDialog)
	ON_LBN_DBLCLK(IDC_LIST_DLGBAR, &CWorkSpaceBar2::OnLbnDblclkListDlgbar)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar2 construction/destruction

CWorkSpaceBar2::CWorkSpaceBar2()
{
	// TODO: add one-time construction code here
	EnableVisualManagerStyle();
	EnableLayout();
}

CWorkSpaceBar2::~CWorkSpaceBar2()
{
}

void CWorkSpaceBar2::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialogBar::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CWorkSpaceBar2)
	DDX_Control(pDX, IDC_LIST_DLGBAR, m_wndList);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar2 message handlers

LRESULT CWorkSpaceBar2::HandleInitDialog(WPARAM wParam, LPARAM lParam)
{
	CBCGPDialogBar::HandleInitDialog(wParam, lParam);
	m_wndList.SetFont(&FontSetting::FontDungGeunMo);



	m_wndList.m_bVisualManagerStyle = TRUE;

	CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout();
	if (pLayout != NULL)
	{
		//pLayout->AddAnchor(IDC_STATIC_DLGBAR, CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeHorz);
		pLayout->AddAnchor(IDC_LIST_DLGBAR, CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth);
		//pLayout->AddAnchor(IDC_CHECK_DLGBAR, CBCGPStaticLayout::e_MoveTypeVert, CBCGPStaticLayout::e_SizeTypeHorz); // 체크박스 레이아웃 설정
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWorkSpaceBar2::OnUpdateCheckBox(CCmdUI* pCmdUI)
{
}

void CWorkSpaceBar2::AddPath(CString path)
{
	// 1. 전체 경로에서 마지막 역슬래시('\') 위치를 찾습니다.
	int nLastBackslash = path.ReverseFind(_T('\\'));

	// 2. 인덱스 다음(+1)부터 끝까지 문자열을 추출합니다. (예: "C:\...\Seoul_연동_데이터.bin" -> "Seoul_연동_데이터.bin")
	CString strKey = path.Mid(nLastBackslash + 1);

	// 3. Map에서 키(파일명)가 존재하는지 찾습니다.
	auto it = m_mapRecentFiles.find(strKey);

	if (it != m_mapRecentFiles.end())
	{
		// 3-1. 찾은 경우: 값(전체 경로)만 최신 경로로 변경
		it->second = path;

		// 리스트박스에서 기존 항목을 찾아 지웁니다. (새로 상단에 올리기 위함)
		int nIndex = m_wndList.FindStringExact(-1, strKey);
		if (nIndex != LB_ERR)
		{
			m_wndList.DeleteString(nIndex);
		}
	}
	else
	{
		// 3-2. 없는 경우: Map에 새로운 키와 값을 추가
		m_mapRecentFiles.insert(std::make_pair(strKey, path));
	}

	// 4. 리스트박스 맨 위(0번째)에 '키(파일명)'를 추가하여 사용자에게 보여줌
	m_wndList.InsertString(0, strKey);
}



//void CWorkSpaceBar2::OnLbnDblclkListDlgbar()
//{
//	int nSel = m_wndList.GetCurSel();
//	if (nSel != LB_ERR) {
//		CString strFile;
//		m_wndList.GetText(nSel, strFile);
//		AfxGetApp()->OpenDocumentFile(strFile);
//	}
//}
void CWorkSpaceBar2::OnLbnDblclkListDlgbar()
{
	int nSel = m_wndList.GetCurSel();
	if (nSel != LB_ERR) {
		CString strKey;

		// 리스트박스에서 클릭한 항목의 텍스트(키 값)를 가져옵니다.
		m_wndList.GetText(nSel, strKey);

		// 맵에서 키 값으로 검색하여 매핑된 '전체 경로(Value)'를 찾습니다.
		auto it = m_mapRecentFiles.find(strKey);
		if (it != m_mapRecentFiles.end())
		{
			CString strFullPath = it->second;

			// 찾은 전체 경로를 통해 문서를 엽니다. (알아서 OnOpenDocument -> OnInitialUpdate로 흐름을 탐)
			AfxGetApp()->OpenDocumentFile(strFullPath);
		}
	}
}