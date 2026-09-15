// workspace.cpp : implementation of the CWorkSpaceBar class
//

#include "stdafx.h"
#include "DataViewEx.h"
#include "WorkSpaceBar.h"
#include "DataComparison.h"
#include "DiffCompareFrame.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar

BEGIN_MESSAGE_MAP(CWorkSpaceBar, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_NOTIFY(TVN_SELCHANGED, 1, &CWorkSpaceBar::OnSelChangedTree)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar construction/destruction

CWorkSpaceBar::CWorkSpaceBar()
{
	SetEmbedded(&m_wndTree);

	// TODO: add one-time construction code here

}

CWorkSpaceBar::~CWorkSpaceBar()
{
	//ClearAllData();
}

bool CWorkSpaceBar::SetTreeViewData()
{
	// ★ 추가: 지우고 다시 채우는 동안 TVN_SELCHANGED 무시
	m_bSuppressSelChanged = true;

	ClearTreeViewData();

	auto item = DataComparison::GetInstance().GetOriginalLDAT();
	//if (item.empty())
	//{
	//	return false;
	//}
	if (!item.empty()) {
		for (const auto& filePath : item)
		{
			// 1. 전체 경로에서 파일 이름만 추출 (ReverseFind 최적화 및 예외 처리)
			int idx = filePath.ReverseFind(_T('\\'));
			CString fileName = (idx != -1) ? filePath.Mid(idx + 1) : filePath;

			// 2. 트리 노드에 파일 이름 삽입
			HTREEITEM fileItem = m_wndTree.InsertItem(fileName, m_hLDATNode);

			// 3. [핵심] 클릭 시 파일을 읽어오기 위해 전체 경로를 동적 할당하여 ItemData에 저장
			CString* pSavedPath = new CString(filePath);
			m_wndTree.SetItemData(fileItem, (DWORD_PTR)pSavedPath);
		}
	}
	m_wndTree.InsertItem(_T("역"), m_hBinDiffNode);
	m_wndTree.InsertItem(_T("연동도표(진로 정보)"), m_hBinDiffNode);
	m_wndTree.InsertItem(_T("궤도"), m_hBinDiffNode);
	m_wndTree.InsertItem(_T("신호기"), m_hBinDiffNode);
	m_wndTree.InsertItem(_T("선로전환기"), m_hBinDiffNode);
	m_wndTree.InsertItem(_T("폐색"), m_hBinDiffNode);
	m_wndTree.InsertItem(_T("기타 고장 정보 및 출발 반응등"), m_hBinDiffNode);
	m_wndTree.InsertItem(_T("CPT"), m_hBinDiffNode);
	m_wndTree.InsertItem(_T("DWELL"), m_hBinDiffNode);
	m_wndTree.InsertItem(_T("임시속도"), m_hBinDiffNode);
	m_wndTree.InsertItem(_T("끌림 감시장치"), m_hBinDiffNode);
	m_wndTree.InsertItem(_T("INCardList"), m_hBinDiffNode);
	m_wndTree.InsertItem(_T("OutCardList"), m_hBinDiffNode);
	m_wndTree.InsertItem(_T("신호기 및 선로전환기 Card"), m_hBinDiffNode);

	m_wndTree.InsertItem(_T("VRD "), m_hLogicDiffNode);
	m_wndTree.InsertItem(_T("궤도 "), m_hLogicDiffNode);
	m_wndTree.InsertItem(_T("선로전환기 "), m_hLogicDiffNode);
	m_wndTree.InsertItem(_T("신호기 "), m_hLogicDiffNode);
	m_wndTree.InsertItem(_T("LMR "), m_hLogicDiffNode);
	m_wndTree.InsertItem(_T("진로 "), m_hLogicDiffNode);
	m_wndTree.InsertItem(_T("역공통 "), m_hLogicDiffNode);
	m_wndTree.InsertItem(_T("폐색 "), m_hLogicDiffNode);
	m_wndTree.InsertItem(_T("건널목 - 고장검지 "), m_hLogicDiffNode);
	m_wndTree.InsertItem(_T("건널목 - 제어건널목"), m_hLogicDiffNode);
	m_wndTree.InsertItem(_T("타역 궤도 "), m_hLogicDiffNode);
	m_wndTree.InsertItem(_T("타역 신호기 "), m_hLogicDiffNode);
	m_wndTree.InsertItem(_T("타역 선로전환기 "), m_hLogicDiffNode);
	m_wndTree.InsertItem(_T("전차선 절연구간 "), m_hLogicDiffNode);
	m_wndTree.InsertItem(_T("지장물 "), m_hLogicDiffNode);
	m_wndTree.InsertItem(_T("출발반응등 "), m_hLogicDiffNode);
	m_wndTree.InsertItem(_T("기타 고장 "), m_hLogicDiffNode);
	m_wndTree.InsertItem(_T("소속역 정보 "), m_hLogicDiffNode);
	m_wndTree.InsertItem(_T("CPT "), m_hLogicDiffNode);
	m_wndTree.InsertItem(_T("히터 "), m_hLogicDiffNode);
	m_wndTree.InsertItem(_T("연동장치 정보"), m_hLogicDiffNode);

	m_wndTree.SetRedraw(TRUE);

	// ★ 추가: 재구성 끝났으니 다시 정상적으로 선택변경 처리
	m_bSuppressSelChanged = false;

	return true;
}
void CWorkSpaceBar::ClearTreeViewData()
{
	if (m_wndTree.GetSafeHwnd() == nullptr)
		return;

	// [수정] 상위 노드(m_hLDATNode / m_hBinDiffNode / m_hLogicDiffNode)는
	// 그대로 두고, 그 "자식 아이템들만" 삭제한다.
	// LDAT 자식이 들고 있는 ItemData(CString*)는 DeleteItem 시 발생하는
	// TVN_DELETEITEM 알림을 DiffCompareFrame::OnTvnDeleteItem 이 받아서
	// 자동으로 delete 해주므로 여기서 따로 delete 하지 않는다.
	auto ClearChildrenOf = [this](HTREEITEM hParent)
		{
			if (hParent == NULL) return;

			HTREEITEM hChild = m_wndTree.GetChildItem(hParent);
			while (hChild != NULL)
			{
				// DeleteItem 이후에는 hChild 핸들이 무효화되므로, 다음 형제는
				// 미리 구해둔다.
				HTREEITEM hNext = m_wndTree.GetNextSiblingItem(hChild);
				m_wndTree.DeleteItem(hChild);
				hChild = hNext;
			}
		};

	ClearChildrenOf(m_hLDATNode);
	ClearChildrenOf(m_hBinDiffNode);
	ClearChildrenOf(m_hLogicDiffNode);
}
void CWorkSpaceBar::DeleteAllItems()
{
	m_wndTree.DeleteAllItems();
}

void CWorkSpaceBar::OnSelChangedTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	// ★ 추가: 트리를 지우고/다시 채우는 중에 컨트롤이 자동으로 쏘는
	// 선택변경 알림은 무시한다 (재오픈 시 엉뚱한 데이터가 표시되던 원인)
	if (m_bSuppressSelChanged)
	{
		*pResult = 0;
		return;
	}

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if (pNMTreeView == nullptr)
		return;

	HTREEITEM hSelected = pNMTreeView->itemNew.hItem;
	if (hSelected != NULL)
	{
		HTREEITEM hParent = m_wndTree.GetParentItem(hSelected);
		DiffCompareFrame* pParentFrame = DYNAMIC_DOWNCAST(DiffCompareFrame, GetParentFrame());

		if (pParentFrame != nullptr)
		{
			// 1. LDAT 파일 선택 시
			if (hParent == m_hLDATNode)
			{
				CString* pFilePath = (CString*)m_wndTree.GetItemData(hSelected);
				if (pFilePath != nullptr && !pFilePath->IsEmpty())
				{
					int idx = pFilePath->ReverseFind(_T('\\'));
					CString fileName = (idx != -1) ? pFilePath->Mid(idx + 1) : *pFilePath;

					pParentFrame->ProcessLdatSelection(fileName, *pFilePath);
				}
			}
			//바이너리 구조체 항목(역, 신호기 등) 선택
			else if (hParent == m_hBinDiffNode)
			{
				CString categoryType = m_wndTree.GetItemText(hSelected);

				// 프레임에 데이터 처리를 위임
				pParentFrame->ProcessBinDiffSelection(categoryType);
			}
			else if (hParent == m_hLogicDiffNode)
			{
				CString logicType = m_wndTree.GetItemText(hSelected);
				pParentFrame->ProcessLogicDiffSelection(logicType);
			}
		}
	}

	*pResult = 0;
}


int CWorkSpaceBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tree control:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;

	m_wndTree.m_bVisualManagerStyle = TRUE;

	if (!m_wndTree.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create workspace view\n");
		return -1;      // fail to create
	}

	// Setup tree content:
	m_hRootNode = m_wndTree.InsertItem(_T("Diff"));
	m_hBinDiffNode = m_wndTree.InsertItem(_T("bin Diff"), m_hRootNode);
	m_hLogicDiffNode = m_wndTree.InsertItem(_T("logic Diff"), m_hRootNode);
	m_hLDATNode = m_wndTree.InsertItem(_T("LDAT"), m_hRootNode);


	m_wndTree.Expand(m_hRootNode, TVE_EXPAND);
	m_wndTree.Expand(m_hBinDiffNode, TVE_EXPAND);
	m_wndTree.Expand(m_hLogicDiffNode, TVE_EXPAND);
	m_wndTree.Expand(m_hLDATNode, TVE_EXPAND);
	return 0;
}

