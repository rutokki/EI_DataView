#include "stdafx.h"
#include "resource.h"
#include "DiffCompareFrame.h"
#include "DataLoader.h"
#include <vector>
#include <algorithm>
#include "CommonUtils.h"
#include "BitUtility.h"
using namespace CommonUtil;
using namespace RouteInfo;

#define WM_LOGIC_DIFF_FINISHED (WM_USER + 101)
#define WM_RENDER_NEXT_CHUNK (WM_USER +102)
// ==========================================
// 공통 구조체 및 열거형 정의
// ==========================================
		// 4. 메인 UI로 전달할 패키지 구조체 생성
struct ResultPackage {
	std::vector<CString> left;
	std::vector<CString> right;
	std::vector<DiffLine> diffs;
};

// 2. 텍스트를 분석하여 키와 순서 번호를 부여하는 함수
//static std::vector<LineToken> GetLineTokens(const std::vector<CString>& lines)
//{
//	std::vector<LineToken> tokens;
//	std::map<CString, int> keyCounts; // 키별 등장 횟수 카운트 맵
//
//	for (const auto& line : lines)
//	{
//		LineToken tok;
//		tok.fullLine = line;
//
//		int nEqual = line.Find(_T('='));
//		if (nEqual != -1)
//		{
//			tok.key = line.Left(nEqual);
//			tok.key.Trim();
//		}
//		else
//		{
//			tok.key = line;
//			tok.key.Trim();
//		}
//
//		// 동일한 키가 몇 번째 나왔는지 기록 후 카운트 증가
//		tok.occurrence = keyCounts[tok.key]++;
//		tokens.push_back(tok);
//	}
//	return tokens;
//}
struct BinBlockUnit {
	CString key;                  // 최상위 블록의 고유 명칭 (예: "[폐색 명칭] 2A", "[ Station Information ]")
	std::vector<CString> lines;   // 해당 블록에 속한 모든 하위 라인들
};
// 파일 아래쪽에 정의된 줄 단위 LCS diff. emitAlignedLines(블록 내부 줄 비교)에서
// 재사용하기 위해 앞당겨 선언.
static std::vector<DiffLine> ComputeDiff(
	const std::vector<CString>& leftLines,
	const std::vector<CString>& rightLines);

// ★ 하위 섹션 대괄호(`[`)에 흔들리지 않고, 최상위 구분선(`========`) 기준으로만 블록을 쪼개는 파서
std::vector<BinBlockUnit> ParseIntoBinBlocks(const std::vector<CString>& srcLines)
{
	std::vector<BinBlockUnit> blocks;
	BinBlockUnit currentBlock;
	bool inBlock = false;

	for (const auto& line : srcLines)
	{
		// 최상위 구분선(대략 8개 이상의 '='로 시작하는 라인)을 만났을 때만 새로운 블록의 시작으로 인정
		bool isMajorSeparator = (line.GetLength() >= 8 && line.Left(8) == _T("========"));

		if (isMajorSeparator)
		{
			// 이전 블록이 있으면 저장
			if (inBlock && !currentBlock.lines.empty())
			{
				blocks.push_back(currentBlock);
				currentBlock = BinBlockUnit();
			}
			inBlock = true;
			currentBlock.lines.push_back(line);
		}
		else if (inBlock)
		{
			// 구분선 바로 다음 줄에 나오는 메인 명칭(예: "[폐색 명칭] 2A", "[ Station Information ]")을 블록의 고유 키로 지정
			if (currentBlock.key.IsEmpty() && line.GetLength() > 0)
			{
				currentBlock.key = line;
			}
			currentBlock.lines.push_back(line);
		}
		else
		{
			// 맨 상단 헤더 영역
			BinBlockUnit headerBlock;
			headerBlock.key = line;
			headerBlock.lines.push_back(line);
			blocks.push_back(headerBlock);
		}
	}

	if (inBlock && !currentBlock.lines.empty())
	{
		blocks.push_back(currentBlock);
	}

	return blocks;
}

// ★ 블록(Key) 단위 LCS를 수행하는 안전한 바이너리 비교 함수
static std::vector<DiffLine> ComputeBinDiff(
	const std::vector<CString>& leftLines,
	const std::vector<CString>& rightLines)
{
	std::vector<BinBlockUnit> leftBlocks = ParseIntoBinBlocks(leftLines);
	std::vector<BinBlockUnit> rightBlocks = ParseIntoBinBlocks(rightLines);

	const int m = static_cast<int>(leftBlocks.size());
	const int n = static_cast<int>(rightBlocks.size());

	// 1. 블록 키(Key)를 기준으로 LCS DP 테이블 생성
	std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
	for (int i = 1; i <= m; ++i)
	{
		for (int j = 1; j <= n; ++j)
		{
			if (leftBlocks[i - 1].key == rightBlocks[j - 1].key)
				dp[i][j] = dp[i - 1][j - 1] + 1;
			else
				dp[i][j] = (std::max)(dp[i - 1][j], dp[i][j - 1]);
		}
	}

	// 2. 블록 단위 백트래킹 (LCS 오퍼레이션 수집)
	enum BlockOpType {
		OP_MATCH, OP_DELETE, OP_INSERT
	};
	struct BlockOperation {
		BlockOpType type; int lIdx; int rIdx;
	};

	std::vector<BlockOperation> blockOps;
	int i = m, j = n;
	while (i > 0 || j > 0)
	{
		if (i > 0 && j > 0 && leftBlocks[i - 1].key == rightBlocks[j - 1].key)
		{
			blockOps.push_back({ OP_MATCH, i - 1, j - 1 });
			--i; --j;
		}
		else if (j > 0 && (i == 0 || dp[i][j - 1] >= dp[i - 1][j]))
		{
			blockOps.push_back({ OP_INSERT, -1, j - 1 });
			--j;
		}
		else if (i > 0)
		{
			blockOps.push_back({ OP_DELETE, i - 1, -1 });
			--i;
		}
	}
	std::reverse(blockOps.begin(), blockOps.end());

	// 2-a. key 로 delete/insert 후보 재매칭
	std::map<CString, std::vector<int>> delIdxByKey; // key -> blockOps 인덱스
	std::map<CString, std::vector<int>> insIdxByKey;

	for (int idx = 0; idx < static_cast<int>(blockOps.size()); ++idx)
	{
		const auto& op = blockOps[idx];
		if (op.type == OP_DELETE)
			delIdxByKey[leftBlocks[op.lIdx].key].push_back(idx);
		else if (op.type == OP_INSERT)
			insIdxByKey[rightBlocks[op.rIdx].key].push_back(idx);
	}

	// blockOps 인덱스 -> "진짜 이동(내용까지 동일)" 여부
	// -1: 짝 없음(그냥 삭제/추가), 0: 짝은 있으나 내용 다름(수정), 1: 짝 있고 내용 동일(이동)
	std::vector<int> pairStateOf(blockOps.size(), -1);

	for (auto& kv : delIdxByKey)
	{
		const CString& key = kv.first;
		std::vector<int>& delIdxs = kv.second;

		auto it = insIdxByKey.find(key);
		if (it == insIdxByKey.end())
			continue; // 같은 key 의 INSERT 가 없으면 그냥 삭제/추가로 남김

		std::vector<int>& insIdxs = it->second;
		const size_t pairCount = (std::min)(delIdxs.size(), insIdxs.size());

		for (size_t k = 0; k < pairCount; ++k)
		{
			const int delOpIdx = delIdxs[k];
			const int insOpIdx = insIdxs[k];

			const auto& lLines = leftBlocks[blockOps[delOpIdx].lIdx].lines;
			const auto& rLines = rightBlocks[blockOps[insOpIdx].rIdx].lines;

			// 블록 내용 전체가 완전히 동일한지 검사 (라인 개수 + 각 라인 내용)
			const bool bIdentical =
				(lLines.size() == rLines.size()) &&
				std::equal(lLines.begin(), lLines.end(), rLines.begin());

			const int state = bIdentical ? 1 : 0;
			pairStateOf[delOpIdx] = state;
			pairStateOf[insOpIdx] = state;
		}
	}

	// 3. 최종 DiffLine 결과 조립
	std::vector<DiffLine> result;
	int nextMoveGroupId = 0;
	std::map<int, int> moveGroupIdByOpIdx;   // 이동(state==1)으로 확정된 쌍에만 group id 부여
	std::map<int, int> changePartnerOpIdx;   // 수정(state==0)으로 확정된 쌍의 상대 opIdx

	// 이동/수정으로 확정된 쌍들을 미리 매핑해 둔다.
	for (auto& kv : delIdxByKey)
	{
		auto it = insIdxByKey.find(kv.first);
		if (it == insIdxByKey.end()) continue;
		auto& delIdxs = kv.second;
		auto& insIdxs = it->second;
		const size_t pairCount = (std::min)(delIdxs.size(), insIdxs.size());
		for (size_t k = 0; k < pairCount; ++k)
		{
			if (pairStateOf[delIdxs[k]] == 1)
			{
				const int gid = nextMoveGroupId++;
				moveGroupIdByOpIdx[delIdxs[k]] = gid;
				moveGroupIdByOpIdx[insIdxs[k]] = gid;
			}
			else if (pairStateOf[delIdxs[k]] == 0)
			{
				// 같은 key, 내용 다름(수정) 쌍의 상대방 opIdx를 서로 기록
				changePartnerOpIdx[delIdxs[k]] = insIdxs[k];
				changePartnerOpIdx[insIdxs[k]] = delIdxs[k];
			}
		}
	}

	// ★ 블록 두 개(좌/우)를 줄 단위로 비교해서 result에 밀어 넣는 공통 로직.
	// key가 같아서 짝지어진 두 블록이라도, 그 안의 각 줄까지 전부 다르다는
	// 보장은 없으므로 (구분선/키 줄처럼 완전히 같은 줄도 섞여 있을 수 있음)
	// 반드시 줄 단위로 다시 비교해서 진짜 다른 줄만 CHANGE로 표시한다.
	// OP_MATCH(원래부터 key가 같아서 매칭된 블록)와 state==0(key 재매칭으로
	// 짝지어진 "수정" 블록) 양쪽에서 공통으로 사용.
	//
	// [수정] 예전엔 lLines[k] vs rLines[k]처럼 같은 인덱스끼리만 비교했음.
	// 비트 조건에 따라 한쪽에만 줄이 하나 더/덜 들어가는 경우
	// (예: ConvertBlockInfoText의 DepRedKind bit0/bit1처럼 조건부로 줄을
	// 개별 push하는 곳) 그 지점 이후 모든 줄이 한 칸씩 밀려서 실제로는
	// 안 바뀐 줄까지 전부 CHANGE로 잘못 표시되는 문제가 있었음.
	// 파일 상단의 줄 단위 LCS(ComputeDiff)를 재사용해서, 블록 내부도 실제
	// 정렬을 찾아 비교하도록 함 (줄이 밀려도 정렬이 깨지지 않음).
	auto emitAlignedLines = [&result](const std::vector<CString>& lLines, const std::vector<CString>& rLines)
		{
			std::vector<DiffLine> aligned = ComputeDiff(lLines, rLines);
			result.insert(result.end(), aligned.begin(), aligned.end());
		};

	// 수정(state==0) 쌍은 DELETE/INSERT 어느 쪽을 먼저 만나든
	// 그 자리에서 한 번만 "좌우 나란히" 출력하고, 나머지 한쪽은 건너뛴다.
	std::vector<bool> bOpAlreadyEmitted(blockOps.size(), false);

	for (int idx = 0; idx < static_cast<int>(blockOps.size()); ++idx)
	{
		if (bOpAlreadyEmitted[idx])
			continue;

		const auto& op = blockOps[idx];

		if (op.type == OP_MATCH)
		{
			emitAlignedLines(leftBlocks[op.lIdx].lines, rightBlocks[op.rIdx].lines);
		}
		else if (op.type == OP_DELETE)
		{
			const int state = pairStateOf[idx]; // -1: 짝없음, 0: 수정, 1: 이동

			if (state == 1)
			{
				// 완전히 동일한 내용이 다른 자리로 이동
				const int gid = moveGroupIdByOpIdx[idx];
				for (const auto& line : leftBlocks[op.lIdx].lines)
				{
					DiffLine dl{ DIFF_MOVE_DELETE, line, CString() };
					dl.moveGroupId = gid;
					result.push_back(dl);
				}
			}
			else if (state == 0)
			{
				// 같은 key, 내용 다름 -> 좌/우 블록을 줄 단위로 다시 비교해서
				// 실제로 다른 줄만 CHANGE, 같은 줄(구분선/키 등)은 MATCH로 출력
				const int partnerIdx = changePartnerOpIdx[idx];
				emitAlignedLines(
					leftBlocks[op.lIdx].lines,
					rightBlocks[blockOps[partnerIdx].rIdx].lines);

				bOpAlreadyEmitted[partnerIdx] = true; // 상대(INSERT) 쪽은 이미 같이 출력했으므로 건너뜀
			}
			else
			{
				// 짝이 아예 없음 -> 순수 삭제
				for (const auto& line : leftBlocks[op.lIdx].lines)
					result.push_back({ DIFF_DELETE, line, CString() });
			}
		}
		else if (op.type == OP_INSERT)
		{
			const int state = pairStateOf[idx];

			if (state == 1)
			{
				const int gid = moveGroupIdByOpIdx[idx];
				for (const auto& line : rightBlocks[op.rIdx].lines)
				{
					DiffLine dl{ DIFF_MOVE_INSERT, CString(), line };
					dl.moveGroupId = gid;
					result.push_back(dl);
				}
			}
			else if (state == 0)
			{
				// DELETE 쪽보다 INSERT 쪽을 먼저 만난 경우에도 동일하게
				// 줄 단위 비교 후 좌/우 나란히 출력
				const int partnerIdx = changePartnerOpIdx[idx];
				emitAlignedLines(
					leftBlocks[blockOps[partnerIdx].lIdx].lines,
					rightBlocks[op.rIdx].lines);

				bOpAlreadyEmitted[partnerIdx] = true; // 상대(DELETE) 쪽은 이미 같이 출력했으므로 건너뜀
			}
			else
			{
				for (const auto& line : rightBlocks[op.rIdx].lines)
					result.push_back({ DIFF_INSERT, CString(), line });
			}
		}
	}

	{
		std::map<int, std::vector<int>> moveDeleteRowsByGid;
		std::map<int, std::vector<int>> moveInsertRowsByGid;

		for (int row = 0; row < static_cast<int>(result.size()); ++row)
		{
			const DiffLine& dl = result[row];
			if (dl.type == DIFF_MOVE_DELETE)
				moveDeleteRowsByGid[dl.moveGroupId].push_back(row);
			else if (dl.type == DIFF_MOVE_INSERT)
				moveInsertRowsByGid[dl.moveGroupId].push_back(row);
		}

		for (auto& kv : moveDeleteRowsByGid)
		{
			const int gid = kv.first;
			std::vector<int>& delRows = kv.second;

			auto it = moveInsertRowsByGid.find(gid);
			if (it == moveInsertRowsByGid.end())
				continue;

			std::vector<int>& insRows = it->second;
			const size_t n = (std::min)(delRows.size(), insRows.size());

			for (size_t k = 0; k < n; ++k)
			{
				result[delRows[k]].movePartnerRow = insRows[k];
				result[insRows[k]].movePartnerRow = delRows[k];
			}
		}
	}

	return result;
}
static std::vector<DiffLine> ComputeDiff(
	const std::vector<CString>& leftLines,
	const std::vector<CString>& rightLines)
{
	const int m = static_cast<int>(leftLines.size());
	const int n = static_cast<int>(rightLines.size());

	// ============================================================
	// LCS DP 테이블 생성
	// ============================================================

	std::vector<std::vector<int>> dp(
		m + 1,
		std::vector<int>(n + 1, 0)
	);

	for (int i = 1; i <= m; ++i)
	{
		for (int j = 1; j <= n; ++j)
		{
			if (leftLines[i - 1] == rightLines[j - 1])
			{
				dp[i][j] = dp[i - 1][j - 1] + 1;
			}
			else
			{
				dp[i][j] = (std::max)(
					dp[i - 1][j],
					dp[i][j - 1]
					);
			}
		}
	}


	// ============================================================
	// LCS Backtracking
	//
	// 결과는 역순으로 만들어짐
	// ============================================================

	std::vector<DiffLine> reverseResult;

	int i = m;
	int j = n;

	while (i > 0 || j > 0)
	{
		// --------------------------------------------------------
		// MATCH
		// --------------------------------------------------------
		if (i > 0 &&
			j > 0 &&
			leftLines[i - 1] == rightLines[j - 1])
		{
			reverseResult.push_back(
				{
					DIFF_MATCH,
					leftLines[i - 1],
					rightLines[j - 1]
				}
			);

			--i;
			--j;
		}

		// --------------------------------------------------------
		// INSERT
		//
		// 오른쪽 파일에만 존재
		// --------------------------------------------------------
		else if (j > 0 &&
			(i == 0 ||
				dp[i][j - 1] >= dp[i - 1][j]))
		{
			reverseResult.push_back(
				{
					DIFF_INSERT,
					CString(),
					rightLines[j - 1]
				}
			);

			--j;
		}

		// --------------------------------------------------------
		// DELETE
		//
		// 왼쪽 파일에만 존재
		// --------------------------------------------------------
		else if (i > 0)
		{
			reverseResult.push_back(
				{
					DIFF_DELETE,
					leftLines[i - 1],
					CString()
				}
			);

			--i;
		}
	}


	// ============================================================
	//  정방향으로 변환
	// ============================================================

	std::vector<DiffLine> rawResult;

	rawResult.reserve(reverseResult.size());

	for (auto it = reverseResult.rbegin();
		it != reverseResult.rend();
		++it)
	{
		rawResult.push_back(*it);
	}


	// ============================================================
	// 변경 영역 그룹화
	//
	// MATCH를 기준으로 구간을 나눔
	//
	// 예:
	//
	// MATCH
	// DELETE
	// DELETE
	// INSERT
	// INSERT
	// MATCH
	//
	// 위의 DELETE ~ INSERT 전체가 하나의 변경 블록
	// ============================================================

	std::vector<DiffLine> result;

	result.reserve(rawResult.size());

	size_t index = 0;

	while (index < rawResult.size())
	{
		// --------------------------------------------------------
		// MATCH
		// --------------------------------------------------------
		if (rawResult[index].type == DIFF_MATCH)
		{
			result.push_back(rawResult[index]);

			++index;

			continue;
		}


		// ========================================================
		// 변경 영역 시작
		// ========================================================

		std::vector<CString> deletedLines;
		std::vector<CString> insertedLines;


		// --------------------------------------------------------
		// MATCH가 나올 때까지 모든 DELETE / INSERT 수집
		// --------------------------------------------------------

		while (index < rawResult.size() &&
			rawResult[index].type != DIFF_MATCH)
		{
			if (rawResult[index].type == DIFF_DELETE)
			{
				deletedLines.push_back(
					rawResult[index].leftStr
				);
			}
			else if (rawResult[index].type == DIFF_INSERT)
			{
				insertedLines.push_back(
					rawResult[index].rightStr
				);
			}

			++index;
		}


		// ========================================================
		// 변경 영역을 행 단위로 정렬
		//
		// N:M
		// 1:N
		// N:1
		//
		// 모두 여기서 처리
		// ========================================================

		const size_t deleteCount = deletedLines.size();
		const size_t insertCount = insertedLines.size();

		const size_t maxCount =
			(std::max)(deleteCount, insertCount);


		for (size_t k = 0; k < maxCount; ++k)
		{
			const bool hasLeft =
				k < deleteCount;

			const bool hasRight =
				k < insertCount;


			// ----------------------------------------------------
			// 양쪽 모두 존재
			//
			// DELETE + INSERT
			// => CHANGE
			// ----------------------------------------------------
			if (hasLeft && hasRight)
			{
				result.push_back(
					{
						DIFF_CHANGE,
						deletedLines[k],
						insertedLines[k]
					}
				);
			}

			// ----------------------------------------------------
			// 왼쪽만 존재
			//
			// => DELETE
			// ----------------------------------------------------
			else if (hasLeft)
			{
				result.push_back(
					{
						DIFF_DELETE,
						deletedLines[k],
						CString()
					}
				);
			}

			// ----------------------------------------------------
			// 오른쪽만 존재
			//
			// => INSERT
			// ----------------------------------------------------
			else
			{
				result.push_back(
					{
						DIFF_INSERT,
						CString(),
						insertedLines[k]
					}
				);
			}
		}
	}


	return result;
}
#define ID_CAPTION_BAR 1600

// ==========================================
// 1. CDiffGridPaneView (왼쪽 뷰) 구현
// ==========================================
IMPLEMENT_DYNCREATE(CDiffGridPaneView, CView)

CDiffGridPaneView::CDiffGridPaneView()
{

}

CDiffGridPaneView::~CDiffGridPaneView()
{
	m_subFrameToolBar.RemoveAllButtons();
}

BEGIN_MESSAGE_MAP(CDiffGridPaneView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_SUB_FILE_OPEN, &CDiffGridPaneView::OnToolbarFileOpen)
	ON_COMMAND(ID_SUB_FIX, &CDiffGridPaneView::OnNextChange)
	ON_COMMAND(ID_SUB_ADD, &CDiffGridPaneView::OnNextInsert)
	ON_COMMAND(ID_SUB_DELETE, &CDiffGridPaneView::OnNextDelete)
	ON_COMMAND(ID_SUB_MOVE, &CDiffGridPaneView::OnNextMove)
END_MESSAGE_MAP()

void CDiffGridPaneView::OnDraw(CDC* pDC)
{
	COLORREF clrText = globalData.clrWindowText;
	CRect rectClient;
	GetClientRect(rectClient);
	pDC->FillSolidRect(rectClient, CBCGPVisualManager::GetInstance()->GetControlFillColor());
	clrText = CBCGPVisualManager::GetInstance()->GetControlTextColor();
}

int CDiffGridPaneView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_subFrameToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	if (!m_subFrameToolBar.LoadToolBar(IDR_SUBFRAME, 0, 0, FALSE, 0, 0, IDR_SVG1))
	{
		TRACE0("Failed to create view toolbar\n");
		return -1;
	}

	// 뷰 내부에 그리드 컨트롤(에디트) 생성
	m_wndLeftEditLDat.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(0, 0, 0, 0), this, 1100);
	m_wndLeftEditLDat.SetModified(FALSE);
	m_wndLeftEditLDat.SetReadOnly(TRUE);
	m_wndLeftEditLDat.ModifyStyle(WS_HSCROLL, 0);
	return 0;
}

void CDiffGridPaneView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_subFrameToolBar.GetSafeHwnd())
	{
		CSize sizeTB = m_subFrameToolBar.CalcFixedLayout(FALSE, TRUE);
		m_subFrameToolBar.SetWindowPos(NULL, 0, 0, cx, sizeTB.cy, SWP_NOZORDER | SWP_NOACTIVATE);
		m_subFrameToolBar.RedrawWindow();
	}

	if (m_wndLeftEditLDat.GetSafeHwnd())
	{
		CSize sizeTB = m_subFrameToolBar.GetSafeHwnd() ? m_subFrameToolBar.CalcFixedLayout(FALSE, TRUE) : CSize(0, 0);
		m_wndLeftEditLDat.SetWindowPos(NULL, 0, sizeTB.cy, cx, cy - sizeTB.cy, SWP_NOZORDER | SWP_NOACTIVATE);
	}
	UpdateWindow();
}

CString CDiffGridPaneView::GetFileStationName(const CString& fileName)
{
	int nLastBackslash = fileName.ReverseFind(_T('\\'));
	CString strStationName = fileName.Mid(nLastBackslash + 1);
	int nUnderscore = strStationName.Find(_T("_"));
	if (nUnderscore != -1)
	{
		strStationName = strStationName.Left(nUnderscore);
	}
	else {
		BCGPMessageBox(_T("파일 명칭 형식 오류 발생"));
	}
	return strStationName;
}

void CDiffGridPaneView::OnToolbarFileOpen()
{
	CFileDialog dlg(
		TRUE,
		_T("bin"),
		NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("연동 데이터 (*.bin)|*_연동_데이터.bin||"),
		this
	);
	if (dlg.DoModal() == IDOK)
	{
		CString outPutMsg;
		CString strFilePath = dlg.GetPathName();
		CString strFileName = dlg.GetFileTitle();
		bool result = DataLoader::LoadAllOriginalDataFromFolder(strFilePath, outPutMsg);
		if (!outPutMsg.IsEmpty())
		{
			CString resultMsg;
			resultMsg.Format(_T("%s 파일 로딩 실패"), (LPCTSTR)outPutMsg);
			BCGPMessageBox(resultMsg, MB_ICONWARNING);
		}
		CWnd* pParent = GetParentFrame();
		if (pParent && pParent->IsKindOf(RUNTIME_CLASS(DiffCompareFrame)))
		{
			if (((DiffCompareFrame*)pParent)->m_pWorkSpaceBar.SetTreeViewData())
			{
				((DiffCompareFrame*)pParent)->SetCaptionText(strFileName, true);
			}
			else
			{
				BCGPMessageBox(_T("LDAT 파일 갱신 실패"), MB_ICONWARNING);
			}
		}
	}
}
//
//void CDiffGridPaneView::SetLogicData(const std::vector<CString>& lines, const std::vector<DiffLine>& diffs)
//{
//	CString strAll;
//	for (const auto& line : lines) {
//		strAll += line + _T("\r\n");
//	}
//	m_wndLeftEditLDat.SetRedraw(FALSE);
//	m_wndLeftEditLDat.InsertText(strAll);
//	m_wndLeftEditLDat.SetRedraw(TRUE);
//
//	m_wndLeftEditLDat.InitDiffData(diffs, true);
//}


bool CDiffGridPaneView::ReadTextFileLines(const CString& filePath, std::vector<CString>& outLines)
{
	CFile file;
	CFileException ex;
	if (!file.Open(filePath, CFile::modeRead | CFile::shareDenyWrite, &ex))
	{
		return false;
	}

	ULONGLONG fileLen = file.GetLength();
	std::vector<char> buffer(static_cast<size_t>(fileLen) + 1, 0);
	file.Read(buffer.data(), static_cast<UINT>(fileLen));
	file.Close();

	CString strText(buffer.data());
	int nCurPos = 0;
	CString strLine;

	outLines.clear();
	while (!(strLine = strText.Tokenize(_T("\r\n"), nCurPos)).IsEmpty())
	{
		outLines.push_back(strLine);
	}

	return true;
}


// ==========================================
// 2. CDiffGridRightView (오른쪽 뷰) 구현
// ==========================================
IMPLEMENT_DYNCREATE(CDiffGridRightView, CView)

CDiffGridRightView::CDiffGridRightView()
{
	// [수정됨] 생성자에서 CView::OnDestroy() 호출 시 윈도우 핸들이 없어서 크래시 발생 가능성이 있으므로 제거합니다.

}

CDiffGridRightView::~CDiffGridRightView()
{
}

BEGIN_MESSAGE_MAP(CDiffGridRightView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_SUB_FILE_OPEN, &CDiffGridRightView::OnToolbarFileOpen)
	ON_COMMAND(ID_SUB_FIX, &CDiffGridRightView::OnNextChange)
	ON_COMMAND(ID_SUB_ADD, &CDiffGridRightView::OnNextInsert)
	ON_COMMAND(ID_SUB_DELETE, &CDiffGridRightView::OnNextDelete)
	ON_COMMAND(ID_SUB_MOVE, &CDiffGridRightView::OnNextMove)
END_MESSAGE_MAP()

void CDiffGridRightView::OnDraw(CDC* pDC)
{
	COLORREF clrText = globalData.clrWindowText;
	CRect rectClient;
	GetClientRect(rectClient);
	pDC->FillSolidRect(rectClient, CBCGPVisualManager::GetInstance()->GetControlFillColor());
	clrText = CBCGPVisualManager::GetInstance()->GetControlTextColor();
}

int CDiffGridRightView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_subFrameToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	if (!m_subFrameToolBar.LoadToolBar(IDR_SUBFRAME, 0, 0, FALSE, 0, 0, IDR_SVG1))
	{
		TRACE0("Failed to create view toolbar\n");
		return -1;
	}


	m_wndRightEditLDat.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(0, 0, 0, 0), this, 1100);
	//m_wndRightEditLDat.ModifyStyle(WS_HSCROLL, 0);
	//m_wndRightEditLDat.ShowScrollBar(SB_HORZ, FALSE);
	m_wndRightEditLDat.SetModified(FALSE);
	m_wndRightEditLDat.SetReadOnly(TRUE);
	return 0;
}

void CDiffGridRightView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_subFrameToolBar.GetSafeHwnd())
	{
		CSize sizeTB = m_subFrameToolBar.CalcFixedLayout(FALSE, TRUE);
		m_subFrameToolBar.SetWindowPos(NULL, 0, 0, cx, sizeTB.cy, SWP_NOZORDER | SWP_NOACTIVATE);
		m_subFrameToolBar.RedrawWindow();
	}

	if (m_wndRightEditLDat.GetSafeHwnd())
	{
		CSize sizeTB = m_subFrameToolBar.GetSafeHwnd() ? m_subFrameToolBar.CalcFixedLayout(FALSE, TRUE) : CSize(0, 0);
		m_wndRightEditLDat.SetWindowPos(NULL, 0, sizeTB.cy, cx, cy - sizeTB.cy, SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void CDiffGridRightView::OnToolbarFileOpen()
{
	CFileDialog dlg(
		TRUE,
		_T("bin"),
		NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("연동 데이터 (*.bin)|*_연동_데이터.bin||"),
		this
	);
	if (dlg.DoModal() == IDOK)
	{
		CString OutputErrMsg;
		CString strFilePath = dlg.GetPathName();
		DataLoader::LoadAllDifflDataFromFolder(strFilePath, OutputErrMsg);

		if (!OutputErrMsg.IsEmpty())
		{
			CString resultMsg;
			resultMsg.Format(_T("%s 파일 로딩 실패"), (LPCTSTR)OutputErrMsg);
			BCGPMessageBox(resultMsg, MB_ICONWARNING);
		}

		CWnd* pParent = GetParent()->GetParentFrame();
		if (pParent != nullptr)
		{
			DiffCompareFrame* pDiffFrame = DYNAMIC_DOWNCAST(DiffCompareFrame, pParent);
			if (pDiffFrame != nullptr)
			{
				pDiffFrame->SetCaptionText(dlg.GetFileTitle(), false);
			}
		}
	}
}

//void CDiffGridRightView::SetLogicData(const std::vector<CString>& lines, const std::vector<DiffLine>& diffs)
//{
//	CString strAll;
//	for (const auto& line : lines) {
//		strAll += line + _T("\r\n");
//	}
//	m_wndRightEditLDat.SetRedraw(FALSE);
//	m_wndRightEditLDat.InsertText(strAll);
//	m_wndRightEditLDat.SetRedraw(TRUE);
//
//	m_wndRightEditLDat.InitDiffData(diffs, false);
//}

// ==========================================
// 3. DiffCompareFrame 구현
// ==========================================
IMPLEMENT_DYNCREATE(DiffCompareFrame, CBCGPFrameWnd)

DiffCompareFrame::DiffCompareFrame()
	: m_ppParentPtr(nullptr)
	, m_CurrentChange(0)
	, m_CurrentDelete(0)
	, m_CurrnetAdd(0)
	, m_CurrentDiffRow(0)
{
	m_logicThread.Start();

}

DiffCompareFrame::~DiffCompareFrame()
{
	m_logicThread.Stop();
	m_pWorkSpaceBar.DestroyWindow();

}

BEGIN_MESSAGE_MAP(DiffCompareFrame, CBCGPFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_DELETEITEM, 1, &DiffCompareFrame::OnTvnDeleteItem)
	ON_WM_CLOSE()
	//ON_MESSAGE(WM_LOGIC_DIFF_FINISHED, &DiffCompareFrame::OnLogicFinished)
	//ON_MESSAGE(WM_RENDER_NEXT_CHUNK, &DiffCompareFrame::OnRenderNextChunk)
END_MESSAGE_MAP()

int DiffCompareFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFrameWnd::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	EnableDocking(CBRS_ALIGN_ANY);
	const int nPaneSize = globalUtils.ScaleByDPI(200, this);
	if (!m_pWorkSpaceBar.Create(_T("파일 비교"), this, CRect(0, 0, nPaneSize, nPaneSize), TRUE, ID_VIEW_WORKSPACE, WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create WorkSpaceBar\n");
		return -1;
	}
	if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | CBRS_TOP, this, ID_CAPTION_BAR))
	{
		TRACE0("Failed to create captionBar\n");
		return -1;
	}

	m_wndCaptionBar.SetMessageBarMode(TRUE);

	m_pWorkSpaceBar.SetIconIndex(1);
	m_pWorkSpaceBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_pWorkSpaceBar);

	// 워크스페이스 바 도킹까지 전부 끝난 "지금"이 스플리터가 실제로 차지할
	// 수 있는 최종 폭을 알 수 있는 시점. 여기서 정확히 50:50으로 맞추고,
	// 그제서야 OnSize()의 비율 유지 로직이 안전하게 동작하도록 함.
	InitSplitterHalfAndReady();

	return 0;
}

void DiffCompareFrame::InitSplitterHalfAndReady()
{
	if (m_wndSplitter.GetSafeHwnd() == nullptr)
	{
		return;
	}

	// 도킹 배치(워크스페이스 바 등)가 실제 창 배치에 반영되도록 강제 레이아웃
	RecalcLayout();

	CRect rectSplitter;
	m_wndSplitter.GetClientRect(&rectSplitter);
	const int cxTotal = rectSplitter.Width();
	const int nCols = m_wndSplitter.GetColumnCount();

	if (cxTotal > 0 && nCols > 0)
	{
		const int cxEach = cxTotal / nCols;
		for (int col = 0; col < nCols; ++col)
		{
			const int cxThisCol = (col == nCols - 1) ? (cxTotal - cxEach * (nCols - 1)) : cxEach;
			m_wndSplitter.SetColumnInfo(col, cxThisCol, 0);
		}
		m_wndSplitter.RecalcLayout();
	}

	// 이제부터 OnSize()가 GetColumnInfo()로 읽는 "이전 비율"이 워크스페이스
	// 바까지 반영된 진짜 50:50이 됨.
	m_bSplitterReady = true;
}

void DiffCompareFrame::OnSize(UINT nType, int cx, int cy)
{
	// 1) base가 스플리터를 건드리기 "전" 현재 컬럼 비율을 저장
	std::vector<int> cxPrevRatio;
	int cxPrevTotal = 0;
	bool bHavePrevRatio = false;

	if (m_bSplitterReady && m_wndSplitter.GetSafeHwnd() != nullptr)
	{
		const int nColsPrev = m_wndSplitter.GetColumnCount();
		if (nColsPrev > 0)
		{
			cxPrevRatio.resize(nColsPrev, 0);
			bHavePrevRatio = true;

			for (int col = 0; col < nColsPrev; ++col)
			{
				int cxColCur = 0, cxColMin = 0;
				m_wndSplitter.GetColumnInfo(col, cxColCur, cxColMin);
				if (cxColCur <= 0)
				{
					bHavePrevRatio = false;
					break;
				}
				cxPrevRatio[col] = cxColCur;
				cxPrevTotal += cxColCur;
			}

			if (cxPrevTotal <= 0)
			{
				bHavePrevRatio = false;
			}
		}
	}

	// 2) 기본 처리. 이 안에서 CSplitterWnd 기본 알고리즘이 스큐된 재배치를
	//    할 수 있지만, 아래에서 우리가 다시 덮어씀.
	CBCGPFrameWnd::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0)
	{
		return;
	}

	if (!m_bSplitterReady || m_wndSplitter.GetSafeHwnd() == nullptr)
	{
		return;
	}

	const int nCols = m_wndSplitter.GetColumnCount();
	if (nCols <= 0)
	{
		return;
	}

	if (bHavePrevRatio && (int)cxPrevRatio.size() != nCols)
	{
		bHavePrevRatio = false;
	}

	// 3) base 처리가 끝난 뒤 스플리터 자신의 실제 클라이언트 폭
	//    (워크스페이스 바 등 도킹바가 이미 반영된 값)을 기준으로 재분배
	CRect rectSplitter;
	m_wndSplitter.GetClientRect(&rectSplitter);
	const int cxTotal = rectSplitter.Width();

	if (cxTotal <= 0)
	{
		return;
	}

	if (!bHavePrevRatio)
	{
		// 이전 비율 정보가 없는 예외 상황(최초 호출 등) -> 균등 분배로 폴백
		const int cxEach = cxTotal / nCols;
		for (int col = 0; col < nCols; ++col)
		{
			const int cxThisCol = (col == nCols - 1) ? (cxTotal - cxEach * (nCols - 1)) : cxEach;
			m_wndSplitter.SetColumnInfo(col, cxThisCol, 0);
		}
		m_wndSplitter.RecalcLayout();
		return;
	}

	// 이전 비율(cxPrevRatio[col] / cxPrevTotal)을 유지하며 새 총 폭에 맞게
	// 재분배. 마지막 컬럼에 나머지를 몰아서 합이 cxTotal과 정확히 일치.
	int cxAssigned = 0;
	for (int col = 0; col < nCols; ++col)
	{
		int cxThisCol;
		if (col == nCols - 1)
		{
			cxThisCol = cxTotal - cxAssigned;
		}
		else
		{
			cxThisCol = MulDiv(cxPrevRatio[col], cxTotal, cxPrevTotal);
			cxAssigned += cxThisCol;
		}
		m_wndSplitter.SetColumnInfo(col, cxThisCol, 0);
	}

	m_wndSplitter.RecalcLayout();
}

BOOL DiffCompareFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	CRect rect;
	GetClientRect(&rect);


	int nSplitterWidth = rect.Width();

	if (!m_wndSplitter.CreateStatic(this, 1, 2, WS_CHILD | WS_VISIBLE, AFX_IDW_PANE_FIRST))
	{
		return FALSE;
	}

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CDiffGridPaneView), CSize(nSplitterWidth / 2, rect.Height()), pContext))
	{
		return FALSE;
	}

	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CDiffGridRightView), CSize(nSplitterWidth / 2, rect.Height()), pContext))
	{
		return FALSE;
	}

	m_wndSplitter.ShowWindow(SW_SHOW);
	m_wndSplitter.RedrawWindow();

	CWnd* pLeftView = m_wndSplitter.GetPane(0, 0);
	CWnd* pRightView = m_wndSplitter.GetPane(0, 1);

	if (pLeftView)
	{
		pLeftView->ShowWindow(SW_SHOW);
		pLeftView->RedrawWindow();
	}

	if (pRightView)
	{
		pRightView->ShowWindow(SW_SHOW);
		pRightView->RedrawWindow();
	}

	return TRUE;
}
CustomBCGPEditCtrl* DiffCompareFrame::GetLeftEdit()
{
	CDiffGridPaneView* pView = (CDiffGridPaneView*)m_wndSplitter.GetPane(0, 0);
	if (pView) return &pView->m_wndLeftEditLDat;
	return nullptr;
}

CustomBCGPEditCtrl* DiffCompareFrame::GetRightEdit()
{
	CDiffGridRightView* pView = (CDiffGridRightView*)m_wndSplitter.GetPane(0, 1);
	if (pView) return &pView->m_wndRightEditLDat;
	return nullptr;
}
// 텍스트에서 '명칭=' 부분의 '명칭(Key)'만 추출하는 도우미 함수

void DiffCompareFrame::SetCaptionText(CString strCaption, bool isOrigin)
{
	CString temp;
	if (isOrigin) {
		m_strOriginFile = strCaption;   // 원본 파일명을 기억할 변수

	}
	else if (!isOrigin) {
		m_strCompareFile = strCaption;  // 비교 파일명을 기억할 변수
	}

	temp.Format(_T("원본 파일 명 : %s   |   비교 파일 명 : %s"), m_strOriginFile.GetString(), m_strCompareFile.GetString());

	m_wndCaptionBar.SetText(temp, CBCGPCaptionBar::ALIGN_CENTER);
}

void DiffCompareFrame::PostNcDestroy()
{
	if (m_ppParentPtr != nullptr)
	{
		*m_ppParentPtr = nullptr;
		m_ppParentPtr = nullptr;
	}
	CBCGPFrameWnd::PostNcDestroy();
}


void DiffCompareFrame::OnClose()
{
	CBCGPFrameWnd::OnClose();
}
void DiffCompareFrame::OnTvnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if (pNMTreeView != nullptr)
	{
		// 트리에 저장해 둔 동적 할당 CString 포인터 안전하게 해제
		CString* pSavedPath = reinterpret_cast<CString*>(pNMTreeView->itemOld.lParam);
		if (pSavedPath != nullptr)
		{
			delete pSavedPath;
			pSavedPath = nullptr;
		}
	}
	*pResult = 0;
}

void DiffCompareFrame::OnDestroy()
{
	CFrameWnd::OnDestroy(); // 상속받으신 프레임 기본 클래스에 맞게 호출

	// 프레임이 파괴될 때 트리의 모든 아이템을 비워주어 TVN_DELETEITEM을 유발하고 메모리 누수를 방지합니다.
	m_pWorkSpaceBar.DeleteAllItems();
}

void DiffCompareFrame::CompareLDATFiles(
	const CString& originalPath,
	const CString& diffPath)
{
	// ============================================================
	// 1. View 가져오기
	// ============================================================
	CDiffGridPaneView* pLeftView =
		DYNAMIC_DOWNCAST(CDiffGridPaneView, m_wndSplitter.GetPane(0, 0));

	CDiffGridRightView* pRightView =
		DYNAMIC_DOWNCAST(CDiffGridRightView, m_wndSplitter.GetPane(0, 1));

	if (pLeftView == nullptr || pRightView == nullptr)
	{
		TRACE(_T("[Diff] View NULL\n"));
		return;
	}

	// ============================================================
	// 2. Edit Control
	// ============================================================
	CustomBCGPEditCtrl* pLeftEdit = &pLeftView->m_wndLeftEditLDat;
	CustomBCGPEditCtrl* pRightEdit = &pRightView->m_wndRightEditLDat;

	if (pLeftEdit == nullptr || pRightEdit == nullptr)
	{
		return;
	}

	// ============================================================
	// 3. 비교 작업 중 Peer 연결 해제 (이벤트 상호 간섭 방지)
	// ============================================================
	pLeftEdit->m_pPeerEdit = nullptr;
	pRightEdit->m_pPeerEdit = nullptr;

	// ============================================================
	// 4. 파일 읽기
	// ============================================================
	std::vector<CString> leftLines;
	std::vector<CString> rightLines;

	pLeftView->ReadTextFileLines(originalPath, leftLines);
	pLeftView->ReadTextFileLines(diffPath, rightLines);

	// ============================================================
	// 5. Diff 계산
	// ============================================================
	m_diffResults = ComputeBinDiff(leftLines, rightLines);

	// ============================================================
	// 6. Diff 개수 초기화
	// ============================================================
	m_CurrentChange = 0;
	m_CurrentDelete = 0;
	m_CurrnetAdd = 0;
	m_CurrentMove = 0;
	m_CurrentDiffRow = -1;

	for (const auto& diff : m_diffResults)
	{
		switch (diff.type)
		{
		case DIFF_CHANGE:
			++m_CurrentChange;
			break;

		case DIFF_DELETE:
			++m_CurrentDelete;
			break;

		case DIFF_INSERT:
			++m_CurrnetAdd;
			break;

		case DIFF_MOVE_DELETE:
		case DIFF_MOVE_INSERT:
			++m_CurrentMove;
			break;

		default:
			break;
		}
	}

	//TRACE(
	//	_T("[Diff] Change=%d Delete=%d Insert=%d TotalRows=%d\n"),
	//	m_CurrentChange,
	//	m_CurrentDelete,
	//	m_CurrnetAdd,
	//	static_cast<int>(m_diffResults.size()));

	// ============================================================
	// 8. 좌우 정렬된 텍스트 생성 (diffResults 1개 = 1행)
	// ============================================================
	CString alignedLeftText;
	CString alignedRightText;

	const size_t totalDiffCount = m_diffResults.size();
	for (size_t i = 0; i < totalDiffCount; ++i)
	{
		const DiffLine& diff = m_diffResults[i];

		CString leftText = diff.leftStr;
		CString rightText = diff.rightStr;

		// [추가] 이동된 라인이면 몇 번째 행으로/에서 이동했는지 덧붙여 표시
		if (diff.movePartnerRow != -1)
		{
			CString suffix;
			if (diff.type == DIFF_MOVE_DELETE)
			{
				suffix.Format(_T("   [%d행으로 이동]"), diff.movePartnerRow + 1); // 1-based 표시
				leftText += suffix;
			}
			else if (diff.type == DIFF_MOVE_INSERT)
			{
				suffix.Format(_T("   [%d행에서 이동]"), diff.movePartnerRow + 1);
				rightText += suffix;
			}
		}

		// LEFT
		alignedLeftText += leftText;
		if (i + 1 < totalDiffCount)
		{
			alignedLeftText += _T("\r\n");
		}

		// RIGHT
		alignedRightText += rightText;
		if (i + 1 < totalDiffCount)
		{
			alignedRightText += _T("\r\n");
		}
	}
	pLeftEdit->SetWindowText(_T(""));
	pRightEdit->SetWindowText(_T(""));

	pLeftEdit->ResetScrollPosition();
	pRightEdit->ResetScrollPosition();
	// ============================================================
	// 9. Edit 내용 설정
	// ============================================================
	pLeftEdit->SetWindowText(alignedLeftText);
	pRightEdit->SetWindowText(alignedRightText);

	// ============================================================
	// 10. 스크롤 위치 초기화
	// ============================================================
	pLeftEdit->ResetScrollPosition();
	pRightEdit->ResetScrollPosition();

	// ============================================================
	// 11. 줄 색상 설정
	// ============================================================
	const COLORREF clrChange = RGB(255, 255, 200);
	const COLORREF clrDelete = RGB(255, 200, 200);
	const COLORREF clrInsert = RGB(200, 255, 200);
	const COLORREF clrMove = RGB(80, 140, 255);
	const COLORREF clrEmpty = RGB(240, 240, 240);
	const COLORREF clrNone = (COLORREF)-1;

	std::vector<COLORREF> leftColors(totalDiffCount, clrNone);
	std::vector<COLORREF> rightColors(totalDiffCount, clrNone);

	for (int row = 0; row < static_cast<int>(totalDiffCount); ++row)
	{
		const DiffLine& diff = m_diffResults[row];

		switch (diff.type)
		{
		case DIFF_MATCH:
			break;

		case DIFF_CHANGE:
			leftColors[row] = clrChange;
			rightColors[row] = clrChange;
			break;

		case DIFF_DELETE:
			leftColors[row] = clrDelete;
			rightColors[row] = clrEmpty;
			break;

		case DIFF_INSERT:
			leftColors[row] = clrEmpty;
			rightColors[row] = clrInsert;
			break;
		case DIFF_MOVE_DELETE:
			leftColors[row] = clrMove;
			rightColors[row] = clrEmpty;
			break;
		case DIFF_MOVE_INSERT:
			leftColors[row] = clrEmpty;
			rightColors[row] = clrMove;
			break;
		}
	}

	pLeftEdit->SetDiffLineBkColors(leftColors);
	pRightEdit->SetDiffLineBkColors(rightColors);
	pLeftEdit->SetDiffTrailingBkColor(clrEmpty);
	pRightEdit->SetDiffTrailingBkColor(clrEmpty);
	// ============================================================
	// 12. 스크롤 맨 위 / 맨 왼쪽으로 이동
	// ============================================================
	pLeftEdit->SendMessage(WM_VSCROLL, SB_TOP, 0);
	pRightEdit->SendMessage(WM_VSCROLL, SB_TOP, 0);

	//pLeftEdit->SendMessage(WM_HSCROLL, SB_LEFT, 0);
	//pRightEdit->SendMessage(WM_HSCROLL, SB_LEFT, 0);

	// ============================================================
	// 13. Diff Scrollbar Marker 전달
	// ============================================================
	std::vector<DiffScrollMarker> scrollMarkers;
	const int nTotalRows = static_cast<int>(totalDiffCount);

	for (int row = 0; row < nTotalRows; ++row)
	{
		COLORREF color;
		bool bIsDiff = true;

		switch (m_diffResults[row].type)
		{
		case DIFF_CHANGE:
			color = RGB(255, 255, 0);   // 노랑
			break;

		case DIFF_DELETE:
			color = RGB(255, 80, 80);   // 빨강
			break;

		case DIFF_INSERT:
			color = RGB(80, 220, 80);   // 초록
			break;
		case DIFF_MOVE_DELETE:
			color = RGB(80, 140, 255);   // 파랑
			break;
		case DIFF_MOVE_INSERT:
			color = RGB(80, 140, 255);   // 파랑
			break;
		default:
			bIsDiff = false;
			break;
		}

		if (bIsDiff)
		{
			scrollMarkers.push_back(DiffScrollMarker(row, color));
		}
	}

	pLeftEdit->SetDiffScrollMarkers(scrollMarkers, nTotalRows);
	pRightEdit->SetDiffScrollMarkers(scrollMarkers, nTotalRows);

	CString caption;
	caption.Format(
		_T("수정: % d    삭제 : % d    추가 : % d    이동 : % d"),
		m_CurrentChange,
		m_CurrentDelete,
		m_CurrnetAdd,
		m_CurrentMove);

	UpdateCaptionBar(caption);
	// ============================================================
	// 15. 스크롤바 영역 재계산 (추가됨)
	// ============================================================
	pLeftEdit->UpdateScrollBars();
	pRightEdit->UpdateScrollBars();

	// ============================================================
	// 16. Edit 화면 갱신
	// ============================================================
	pLeftEdit->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
	pRightEdit->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);

	// ============================================================
	// 17. Peer 연결 (양방향 스크롤 동기화 복구)
	// ============================================================
	pLeftEdit->m_pPeerEdit = pRightEdit;
	pRightEdit->m_pPeerEdit = pLeftEdit;

	//TRACE(_T("[Diff] CompareLDATFiles END\n"));
}
void DiffCompareFrame::LeftViewLDATFiles(CString originalPath, bool isSingleMode)
{
	CDiffGridPaneView* pLeftView =
		DYNAMIC_DOWNCAST(CDiffGridPaneView, m_wndSplitter.GetPane(0, 0));
	CDiffGridRightView* pRightView =
		DYNAMIC_DOWNCAST(CDiffGridRightView, m_wndSplitter.GetPane(0, 1));
	if (!pLeftView || !pRightView) return;

	CustomBCGPEditCtrl* pLeftEdit = &pLeftView->m_wndLeftEditLDat;
	CustomBCGPEditCtrl* pRightEdit = &pRightView->m_wndRightEditLDat;

	if (isSingleMode)
	{
		pLeftEdit->m_pPeerEdit = nullptr;
		pRightEdit->m_pPeerEdit = nullptr;
	}
	else
	{
		pLeftEdit->m_pPeerEdit = pRightEdit;
		pRightEdit->m_pPeerEdit = pLeftEdit;
	}

	std::vector<CString> leftLines;
	pLeftView->ReadTextFileLines(originalPath, leftLines);

	CString alignedLeftText;
	const size_t totalCount = leftLines.size();
	for (size_t i = 0; i < totalCount; ++i)
	{
		alignedLeftText += leftLines[i];
		if (i + 1 < totalCount)
		{
			alignedLeftText += _T("\r\n");
		}
	}

	// 1. 텍스트 설정
	pLeftEdit->SetWindowText(_T(""));
	pLeftEdit->SetWindowText(alignedLeftText);

	// ============================================================
	// [핵심 추가] 이전 비교 하이라이트 색상 및 스크롤 마커 초기화
	// ============================================================
	std::vector<COLORREF> leftDefaultColors(totalCount, (COLORREF)-1);
	pLeftEdit->SetDiffLineBkColors(leftDefaultColors);
	pLeftEdit->SetDiffScrollMarkers(std::vector<DiffScrollMarker>(), 0);

	// 우측(비교 파일 없음 문구 영역)도 색상/마커 초기화
	std::vector<COLORREF> rightDefaultColors(1, (COLORREF)-1);
	pRightEdit->SetDiffLineBkColors(rightDefaultColors);
	pRightEdit->SetDiffScrollMarkers(std::vector<DiffScrollMarker>(), 0);
	// ============================================================

	// 2. 스크롤 위치 초기화 및 맨 위로 이동
	pLeftEdit->ResetScrollPosition();
	pRightEdit->ResetScrollPosition();
	pLeftEdit->SendMessage(WM_VSCROLL, SB_TOP, 0);

	// 3. 스크롤바 영역 재계산
	pLeftEdit->UpdateScrollBars();
	pRightEdit->UpdateScrollBars();

	// 4. 화면 갱신
	pLeftEdit->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
	pRightEdit->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}
void DiffCompareFrame::ApplyDiffToUI(const std::vector<CString>& leftLines, const std::vector<CString>& rightLines)
{
	CDiffGridPaneView* pLeftView = DYNAMIC_DOWNCAST(CDiffGridPaneView, m_wndSplitter.GetPane(0, 0));
	CDiffGridRightView* pRightView = DYNAMIC_DOWNCAST(CDiffGridRightView, m_wndSplitter.GetPane(0, 1));
	if (!pLeftView || !pRightView) return;

	CustomBCGPEditCtrl* pLeftEdit = &pLeftView->m_wndLeftEditLDat;
	CustomBCGPEditCtrl* pRightEdit = &pRightView->m_wndRightEditLDat;

	pLeftEdit->m_pPeerEdit = nullptr;
	pRightEdit->m_pPeerEdit = nullptr;


	m_diffResults = ComputeBinDiff(leftLines, rightLines);

	m_CurrentChange = 0;
	m_CurrentDelete = 0;
	m_CurrnetAdd = 0;
	m_CurrentMove = 0;
	m_CurrentDiffRow = -1;

	for (auto& diff : m_diffResults) {
		switch (diff.type)
		{
		case DIFF_CHANGE:
			++m_CurrentChange;
			break;
		case DIFF_DELETE:
			++m_CurrentDelete;
			break;
		case DIFF_INSERT:
			++m_CurrnetAdd;
			break;
		case DIFF_MOVE_DELETE:
		case DIFF_MOVE_INSERT:
			++m_CurrentMove;
			break;
		default:
			break;
		}
	}

	CString alignedLeftText, alignedRightText;
	const size_t totalDiffCount = m_diffResults.size();
	for (size_t i = 0; i < totalDiffCount; ++i)
	{
		const DiffLine& diff = m_diffResults[i];

		CString leftText = diff.leftStr;
		CString rightText = diff.rightStr;

		// 이동된 라인이면 "몇 번째 행으로/에서 이동했는지" 덧붙여 표시
		if (diff.movePartnerRow != -1)
		{
			CString suffix;
			if (diff.type == DIFF_MOVE_DELETE)
			{
				suffix.Format(_T("   [%d행으로 이동]"), diff.movePartnerRow + 1); // 1-based 표시
				leftText += suffix;
			}
			else if (diff.type == DIFF_MOVE_INSERT)
			{
				suffix.Format(_T("   [%d행에서 이동]"), diff.movePartnerRow + 1);
				rightText += suffix;
			}
		}

		alignedLeftText += leftText;
		alignedRightText += rightText;

		if (i + 1 < totalDiffCount)
		{
			alignedLeftText += _T("\r\n");
			alignedRightText += _T("\r\n");
		}
	}

	pLeftEdit->SetWindowText(_T(""));
	pRightEdit->SetWindowText(_T(""));
	pLeftEdit->SetWindowText(alignedLeftText);
	pRightEdit->SetWindowText(alignedRightText);
	pLeftEdit->ResetScrollPosition();
	pRightEdit->ResetScrollPosition();

	// 색상 및 마커 처리 로직은 기존과 동일하게 유지
	const COLORREF clrChange = RGB(255, 255, 200), clrDelete = RGB(255, 200, 200);
	const COLORREF clrInsert = RGB(200, 255, 200), clrEmpty = RGB(240, 240, 240);
	// 이동만 됨 (내용 동일) - 연한 색
	const COLORREF clrMove = RGB(80, 140, 255);
	const COLORREF clrNone = (COLORREF)-1;

	std::vector<COLORREF> leftColors(totalDiffCount, clrNone);
	std::vector<COLORREF> rightColors(totalDiffCount, clrNone);
	std::vector<DiffScrollMarker> scrollMarkers;

	for (int row = 0; row < static_cast<int>(totalDiffCount); ++row)
	{
		COLORREF scrollColor = 0;
		bool bHasDiff = true;
		const DiffLine& diff = m_diffResults[row];

		switch (diff.type)
		{
		case DIFF_CHANGE:
			leftColors[row] = clrChange;
			rightColors[row] = clrChange;
			scrollColor = RGB(255, 255, 0); break;
		case DIFF_DELETE:
			leftColors[row] = clrDelete;
			rightColors[row] = clrEmpty;
			scrollColor = RGB(255, 80, 80); break;
		case DIFF_INSERT:
			leftColors[row] = clrEmpty;
			rightColors[row] = clrInsert;
			scrollColor = RGB(80, 220, 80); break;
		case DIFF_MOVE_DELETE:
			leftColors[row] = clrMove;
			rightColors[row] = clrEmpty;
			scrollColor = RGB(80, 140, 255);
			break;
		case DIFF_MOVE_INSERT:
			leftColors[row] = clrEmpty;
			rightColors[row] = clrMove;
			scrollColor = RGB(80, 140, 255);
			break;

		default:
			bHasDiff = false; break;
		}

		if (bHasDiff) scrollMarkers.push_back(DiffScrollMarker(row, scrollColor));
	}

	CString caption;
	caption.Format(
		_T("수정: % d    삭제 : % d    추가 : % d    이동 : % d"),
		m_CurrentChange,
		m_CurrentDelete,
		m_CurrnetAdd,
		m_CurrentMove);

	UpdateCaptionBar(caption);

	pLeftEdit->SetDiffLineBkColors(leftColors);
	pRightEdit->SetDiffLineBkColors(rightColors);
	pLeftEdit->SetDiffTrailingBkColor(RGB(240, 240, 240));
	pRightEdit->SetDiffTrailingBkColor(RGB(240, 240, 240));
	pLeftEdit->SetDiffScrollMarkers(scrollMarkers, static_cast<int>(totalDiffCount));
	pRightEdit->SetDiffScrollMarkers(scrollMarkers, static_cast<int>(totalDiffCount));

	pLeftEdit->SendMessage(WM_VSCROLL, SB_TOP, 0);
	pRightEdit->SendMessage(WM_VSCROLL, SB_TOP, 0);
	//pLeftEdit->SendMessage(WM_HSCROLL, SB_LEFT, 0);
	//pRightEdit->SendMessage(WM_HSCROLL, SB_LEFT, 0);

	pLeftEdit->UpdateScrollBars();
	pRightEdit->UpdateScrollBars();

	pLeftEdit->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
	pRightEdit->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);

	pLeftEdit->m_pPeerEdit = pRightEdit;
	pRightEdit->m_pPeerEdit = pLeftEdit;
}
// 트리뷰 등에서 파일 선택 시 불려지는 메인 진입점
void DiffCompareFrame::ProcessLdatSelection(const CString& fileName, const CString& clickedOriginalPath)
{
	CWaitCursor wait;

	auto& dataComp = DataComparison::GetInstance();

	const auto& originalList = dataComp.GetOriginalLDAT();
	const auto& diffList = dataComp.GetDiffLDAT();

	CString diffPath = _T("");
	bool bFoundDiff = false;

	// ============================================================
	// 1. 비교 대상 파일 찾기
	// ============================================================
	for (const auto& path : diffList)
	{
		int idx = path.ReverseFind(_T('\\'));
		CString diffFileName =
			(idx != -1) ? path.Mid(idx + 1) : path;

		if (diffFileName.CompareNoCase(fileName) == 0)
		{
			diffPath = path;
			bFoundDiff = true;
			break;
		}
	}

	CDiffGridPaneView* pLeftView =
		DYNAMIC_DOWNCAST(
			CDiffGridPaneView,
			m_wndSplitter.GetPane(0, 0));

	CDiffGridRightView* pRightView =
		DYNAMIC_DOWNCAST(
			CDiffGridRightView,
			m_wndSplitter.GetPane(0, 1));

	// ============================================================
	// 2. 비교 대상 파일이 없는 경우
	// ============================================================
	if (!bFoundDiff)
	{
		//m_wndCaptionBar.SetText(fileName);

		if (pLeftView != nullptr)
		{
			pLeftView->m_wndLeftEditLDat.SetWindowText(_T(""));
			pRightView->m_wndRightEditLDat.SetWindowText(_T(""));
			pRightView->m_wndRightEditLDat.InsertText(
				_T("비교 대상 파일이 없습니다."));

			LeftViewLDATFiles(clickedOriginalPath, true);
			UpdateNoDataCaptionBar();
			pLeftView->m_wndLeftEditLDat.Invalidate();
		}

		return;
	}

	// ============================================================
	// 3. 비교 실행
	// ============================================================
	if (pLeftView != nullptr && pRightView != nullptr)
	{
		CompareLDATFiles(
			clickedOriginalPath,
			diffPath);
	}

	// ============================================================
	// 4. Caption에 전체 Diff 개수 표시
	// ============================================================
	CString caption;
	caption.Format(
		_T("수정: % d    삭제 : % d    추가 : % d"),
		m_CurrentChange,
		m_CurrentDelete,
		m_CurrnetAdd);

	UpdateCaptionBar(caption);
}
void DiffCompareFrame::ProcessLogicDiffSelection(CString& logicType) {
	logicType.Trim(); // 좌우 공백 제거 (트리 텍스트의 여백 대비)

	UCHAR logicKind = 0;

	if (logicType == _T("속도코드제어장치 VRD"))      logicKind = _T('V');
	else if (logicType == _T("궤도"))                 logicKind = _T('T');
	else if (logicType == _T("선로전환기"))           logicKind = _T('P');
	else if (logicType == _T("신호기"))               logicKind = _T('S');
	else if (logicType == _T("LMR"))                  logicKind = _T('L');
	else if (logicType == _T("진로"))                 logicKind = _T('R');
	else if (logicType == _T("역공통"))               logicKind = _T('N');
	else if (logicType == _T("폐색"))                 logicKind = _T('B');
	else if (logicType == _T("건널목 - 고장검지"))    logicKind = _T('C');
	else if (logicType == _T("건널목 - 제어건널목"))  logicKind = _T('c');
	else if (logicType == _T("타역 궤도"))            logicKind = _T('t');
	else if (logicType == _T("타역 신호기"))          logicKind = _T('s');
	else if (logicType == _T("타역 선로전환기"))      logicKind = _T('p');
	else if (logicType == _T("전차선 절연구간"))      logicKind = _T('D');
	else if (logicType == _T("지장물"))               logicKind = _T('J');
	else if (logicType == _T("출발반응등"))           logicKind = _T('K');
	else if (logicType == _T("기타 고장"))            logicKind = _T('F');
	else if (logicType == _T("소속역 정보"))          logicKind = _T('W');
	else if (logicType == _T("CPT"))                  logicKind = _T('H');
	else if (logicType == _T("히터"))                 logicKind = _T('h');
	else if (logicType == _T("연동장치 정보"))        logicKind = _T('E');
	else {
		return; // 매칭되는 타입이 없을 경우 종료
	}

	SetLogicInfo(logicKind);
}
void DiffCompareFrame::ProcessBinDiffSelection(const CString& type)
{
	CWaitCursor wait;

	std::vector<CString> OriginalLine;
	std::vector<CString> DiffLine;

	if (type == _T("역") || type == _T("연동도표(진로 정보)") || type == _T("궤도") ||
		type == _T("신호기") || type == _T("선로전환기") || type == _T("폐색") ||
		type == _T("기타 고장 정보 및 출발 반응등") ||
		type == _T("입력카드 및 카드 Addr") || type == _T("") || type == _T("CPT") ||
		type == _T("DWELL") || type == _T("제어 건널목") || type == _T("INCardList") ||
		type == _T("OutCardList"))
	{
		ResetEditContent();
	}

	if (type == _T("역"))
	{
		SetStationInfo();
	}
	else if (type == _T("연동도표(진로 정보)")) { //연동도표
		SetInterLock();
	}
	else if (type == _T("궤도")) { // 궤도
		SetTrackInfo();
	}
	else if (type == _T("신호기")) { // 신호기
		SetSignalInfo();
	}
	else if (type == _T("선로전환기")) { //선로전환기
		SetSwitchInfo();
	}
	else if (type == _T("폐색")) { //폐색
		SetBlockInfo();
	}
	else if (type == _T("기타 고장 정보 및 출발 반응등")) {
		SetAllFaultInfo();
	}
	else if (type == _T("입력카드 및 카드 Addr")) {
		SetAllIOCardInfo();
	}
	else if (type == _T("CPT")) {
		SetCPTInfo();
	}
	else if (type == _T("DWELL")) {
		SetDWELLInfo();
	}
	else if (type == _T("INCardList")) {
		SetIncardInfo();
	}
	else if (type == _T("OutCardList")) {
		SetOutCardInfo();
	}


}

// ============================================================
// 현재 실제 ScrollBar 위치를 기준으로 다음 Diff 검색
// ============================================================
void DiffCompareFrame::FindNextDiff(DiffType type)
{
	if (m_diffResults.empty())
	{
		BCGPMessageBox(
			_T("비교 결과가 없습니다."),
			MB_OK | MB_ICONINFORMATION);
		return;
	}

	CDiffGridPaneView* pLeftView =
		DYNAMIC_DOWNCAST(
			CDiffGridPaneView,
			m_wndSplitter.GetPane(0, 0));

	if (pLeftView == nullptr)
		return;

	CustomBCGPEditCtrl* pEdit =
		&pLeftView->m_wndLeftEditLDat;

	if (pEdit == nullptr || !::IsWindow(pEdit->GetSafeHwnd()))
		return;

	// ★ 핵심:
	// 마지막으로 이동했던 Diff 행을 기준으로 하지 않는다.
	// 실제 현재 ScrollBar의 첫 표시 행을 기준으로 검색한다.
	const int nCurrentScroll =
		pEdit->GetFirstVisibleLine();

	// 현재 화면의 첫 행보다 뒤에 있는 항목을 찾는다.
	for (int i = nCurrentScroll + 1;
		i < static_cast<int>(m_diffResults.size());
		++i)
	{
		if (m_diffResults[i].type == type)
		{
			m_CurrentDiffRow = i;
			MoveToDiffRow(i);
			return;
		}
	}

	// ★ 순환하지 않는다.
	// 마지막 항목에 도착했으면 알림만 표시한다.
	ShowDiffEndMessage(type, false);
}

// ============================================================
// 현재 실제 ScrollBar 위치를 기준으로 이전 Diff 검색
// ============================================================
void DiffCompareFrame::FindPrevDiff(DiffType type)
{
	if (m_diffResults.empty())
	{
		BCGPMessageBox(
			_T("비교 결과가 없습니다."),
			MB_OK | MB_ICONINFORMATION);
		return;
	}

	CDiffGridPaneView* pLeftView =
		DYNAMIC_DOWNCAST(
			CDiffGridPaneView,
			m_wndSplitter.GetPane(0, 0));

	if (pLeftView == nullptr)
		return;

	CustomBCGPEditCtrl* pEdit =
		&pLeftView->m_wndLeftEditLDat;

	if (pEdit == nullptr || !::IsWindow(pEdit->GetSafeHwnd()))
		return;

	// 실제 ScrollBar의 첫 표시 행을 기준으로 이전 항목 검색
	const int nCurrentScroll =
		pEdit->GetFirstVisibleLine();

	for (int i = nCurrentScroll - 1;
		i >= 0;
		--i)
	{
		if (m_diffResults[i].type == type)
		{
			m_CurrentDiffRow = i;
			MoveToDiffRow(i);
			return;
		}
	}

	// ★ 순환하지 않는다.
	ShowDiffEndMessage(type, true);
}

void DiffCompareFrame::UpdateCaptionBar(CString str)
{
	CString strEx = m_wndCaptionBar.GetText();

	// 1. 이미 ' - ' 상태 텍스트가 붙어있다면, 그 이전(기본 파일명 영역)까지만 잘라냅니다.
	int nIndex = strEx.Find(_T(" - "));
	if (nIndex != -1)
	{
		strEx = strEx.Left(nIndex);
	}

	// 2. 새로운 변경 갯수 텍스트를 깔끔하게 이어붙입니다.
	if (!str.IsEmpty())
	{
		strEx.AppendFormat(_T(" - %s"), str.GetString());
	}

	// 3. 캡션바에 최종 적용
	m_wndCaptionBar.SetText(strEx, CBCGPCaptionBar::ALIGN_CENTER);
}

void DiffCompareFrame::UpdateNoDataCaptionBar()
{
	CString caption;
	caption.Format(
		_T("수정: % d    삭제 : % d    추가 : % d    이동 : % d"),
		0,
		0,
		0,
		0);

	UpdateCaptionBar(caption);
}

// ============================================================
// 다음/이전 검색 종료 메시지
// ============================================================
void DiffCompareFrame::ShowDiffEndMessage(
	DiffType type,
	bool bPrevious)
{
	CString typeName;

	switch (type)
	{
	case DIFF_CHANGE:
		typeName = _T("수정");
		break;

	case DIFF_DELETE:
		typeName = _T("삭제");
		break;

	case DIFF_INSERT:
		typeName = _T("추가");
		break;
	case DIFF_MOVE_DELETE:
	case DIFF_MOVE_INSERT:
		typeName = _T("이동");
		break;
	default:
		return;
	}

	CString message;

	if (bPrevious)
	{
		message.Format(
			_T("첫 번째 %s 항목입니다.\n\n")
			_T("전체 수정 : %d\n")
			_T("전체 삭제 : %d\n")
			_T("전체 추가 : %d\n")
			_T("전체 이동 : %d"),
			(LPCTSTR)typeName,
			m_CurrentChange,
			m_CurrentDelete,
			m_CurrnetAdd,
			m_CurrentMove);
	}
	else
	{
		message.Format(
			_T("마지막 %s 항목입니다.\n\n")
			_T("전체 수정 : %d\n")
			_T("전체 삭제 : %d\n")
			_T("전체 추가 : %d\n")
			_T("전체 이동 : %d"),
			(LPCTSTR)typeName,
			m_CurrentChange,
			m_CurrentDelete,
			m_CurrnetAdd,
			m_CurrentMove);
	}

	BCGPMessageBox(
		message,
		MB_OK | MB_ICONINFORMATION);
}

void DiffCompareFrame::DisplaySingle(const std::vector<CString>& lines, bool isOrigin)
{
	CDiffGridPaneView* pLeftView = DYNAMIC_DOWNCAST(CDiffGridPaneView, m_wndSplitter.GetPane(0, 0));
	CDiffGridRightView* pRightView = DYNAMIC_DOWNCAST(CDiffGridRightView, m_wndSplitter.GetPane(0, 1));
	if (!pLeftView || !pRightView) return;

	// 원본이면 왼쪽, 비교 파일이면 오른쪽 에디트 컨트롤 선택
	CustomBCGPEditCtrl* pTargetEdit = isOrigin ? &pLeftView->m_wndLeftEditLDat : &pRightView->m_wndRightEditLDat;

	pTargetEdit->m_pPeerEdit = nullptr;

	// 1. 전달받은 라인들을 \r\n으로 결합
	CString targetText;
	for (size_t i = 0; i < lines.size(); ++i)
	{
		targetText += lines[i];
		if (i + 1 < lines.size())
		{
			targetText += _T("\r\n");
		}
	}

	// 2. 텍스트 입력 및 스크롤 위치 초기화
	pTargetEdit->SetWindowText(_T(""));
	pTargetEdit->SetWindowText(targetText);
	pTargetEdit->ResetScrollPosition();

	// 3. 단독 출력이므로 하이라이트 색상 및 스크롤 마커 초기화
	std::vector<COLORREF> defaultColors(lines.size(), (COLORREF)-1);
	pTargetEdit->SetDiffLineBkColors(defaultColors);
	pTargetEdit->SetDiffScrollMarkers(std::vector<DiffScrollMarker>(), 0);

	pTargetEdit->UpdateScrollBars();
	pTargetEdit->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}

// ============================================================
// 찾은 Diff 행으로 화면 이동
//
//  검색 기준은 ScrollBar
//  이동 대상은 찾은 Diff 행
//  좌우 Edit는 한 번의 이동으로 동기화
// ============================================================
void DiffCompareFrame::MoveToDiffRow(int nRow)
{
	if (nRow < 0 ||
		nRow >= static_cast<int>(m_diffResults.size()))
	{
		return;
	}

	CDiffGridPaneView* pLeftView =
		DYNAMIC_DOWNCAST(
			CDiffGridPaneView,
			m_wndSplitter.GetPane(0, 0));

	CDiffGridRightView* pRightView =
		DYNAMIC_DOWNCAST(
			CDiffGridRightView,
			m_wndSplitter.GetPane(0, 1));

	if (pLeftView == nullptr ||
		pRightView == nullptr)
	{
		return;
	}

	CustomBCGPEditCtrl* pLeftEdit =
		&pLeftView->m_wndLeftEditLDat;

	CustomBCGPEditCtrl* pRightEdit =
		&pRightView->m_wndRightEditLDat;

	if (!::IsWindow(pLeftEdit->GetSafeHwnd()) ||
		!::IsWindow(pRightEdit->GetSafeHwnd()))
	{
		return;
	}

	const int nLeftFirst =
		pLeftEdit->GetFirstVisibleLine();

	const int nRightFirst =
		pRightEdit->GetFirstVisibleLine();

	// 찾은 행을 각 Edit의 첫 번째 표시 행으로 이동한다.
	const int nLeftDelta =
		nRow - nLeftFirst;

	const int nRightDelta =
		nRow - nRightFirst;

	// ------------------------------------------------------------
	// 기존 OnVScroll의 Peer 동기화가 다시 실행되지 않도록
	// 이동 중에는 m_bIsScrolling을 TRUE로 설정
	// ------------------------------------------------------------
	pLeftEdit->m_bIsScrolling = true;
	pRightEdit->m_bIsScrolling = true;

	if (nLeftDelta != 0)
	{
		pLeftEdit->LineScroll(nLeftDelta);
	}

	if (nRightDelta != 0)
	{
		pRightEdit->LineScroll(nRightDelta);
	}

	pLeftEdit->m_bIsScrolling = false;
	pRightEdit->m_bIsScrolling = false;

	// 현재 선택된 Diff 행은 UI 상태 표시용으로만 저장한다.
	// 다음 검색의 기준으로 사용하지 않는다.
	m_CurrentDiffRow = nRow;

	pLeftEdit->RedrawWindow(
		NULL,
		NULL,
		RDW_INVALIDATE | RDW_UPDATENOW);

	pRightEdit->RedrawWindow(
		NULL,
		NULL,
		RDW_INVALIDATE | RDW_UPDATENOW);
}

void DiffCompareFrame::ShowNoDataMessage(bool isLeft, const std::vector<CString>& lines)
{
	CDiffGridPaneView* pLeftView = DYNAMIC_DOWNCAST(CDiffGridPaneView, m_wndSplitter.GetPane(0, 0));
	CDiffGridRightView* pRightView = DYNAMIC_DOWNCAST(CDiffGridRightView, m_wndSplitter.GetPane(0, 1));

	CString text;
	for (size_t i = 0; i < lines.size(); ++i)
	{
		text += lines[i];
		if (i + 1 < lines.size())
			text += _T("\r\n");
	}

	if (isLeft)
	{
		if (pLeftView == nullptr) return;
		pLeftView->m_wndLeftEditLDat.Clear();
		pLeftView->m_wndLeftEditLDat.InsertText(text);
		pLeftView->m_wndLeftEditLDat.Invalidate();
	}
	else
	{
		if (pRightView == nullptr) return;
		pRightView->m_wndRightEditLDat.Clear();
		pRightView->m_wndRightEditLDat.InsertText(text);
		pRightView->m_wndRightEditLDat.Invalidate();
	}
}

void DiffCompareFrame::ApplyCompareOrSingle(
	bool hasOrigin, bool hasDiff,
	const std::vector<CString>& leftLines,
	const std::vector<CString>& rightLines)
{
	if (hasOrigin && hasDiff)
	{
		// 원본과 비교 파일 둘 다 있음 -> 정상적인 Diff 비교 실행
		ApplyDiffToUI(leftLines, rightLines);
		return;
	}

	// 둘 중 하나라도 없음 -> 이전 diff 렌더링에서 남은 텍스트/색상/스크롤
	// 마커를 먼저 지운다 (여기가 "이전 색이 남는 문제"의 핵심 수정 지점)
	ResetEditContent();

	if (hasOrigin)
		DisplaySingle(leftLines, true);
	else
		ShowNoDataMessage(true, leftLines);

	if (hasDiff)
		DisplaySingle(rightLines, false);
	else
		ShowNoDataMessage(false, rightLines);

	UpdateNoDataCaptionBar();
}

void DiffCompareFrame::ResetEditContent()
{
	auto resetEditColors = [](CustomBCGPEditCtrl* pEdit)
		{
			std::vector<COLORREF> defaultColors(1, (COLORREF)-1);
			pEdit->SetDiffLineBkColors(defaultColors);
			pEdit->SetDiffScrollMarkers(std::vector<DiffScrollMarker>(), 0);
		};
	CustomBCGPEditCtrl* pLeftEdit = GetLeftEdit();
	CustomBCGPEditCtrl* pRightEdit = GetRightEdit();
	pLeftEdit->SetWindowText(_T(""));
	pRightEdit->SetWindowText(_T(""));
	resetEditColors(pLeftEdit);
	resetEditColors(pRightEdit);
}

// ============================================================
// LEFT VIEW TOOLBAR
// ============================================================
void CDiffGridPaneView::OnNextChange()
{
	DiffCompareFrame* pFrame =
		DYNAMIC_DOWNCAST(
			DiffCompareFrame,
			GetParentFrame());

	if (pFrame == nullptr)
		return;

	pFrame->FindNextDiff(DIFF_CHANGE);
}

void CDiffGridPaneView::OnNextDelete()
{
	DiffCompareFrame* pFrame =
		DYNAMIC_DOWNCAST(
			DiffCompareFrame,
			GetParentFrame());

	if (pFrame == nullptr)
		return;

	pFrame->FindNextDiff(DIFF_DELETE);
}

void CDiffGridPaneView::OnNextInsert()
{
	DiffCompareFrame* pFrame =
		DYNAMIC_DOWNCAST(
			DiffCompareFrame,
			GetParentFrame());

	if (pFrame == nullptr)
		return;

	pFrame->FindNextDiff(DIFF_INSERT);
}
void CDiffGridPaneView::OnNextMove()
{
	DiffCompareFrame* pFrame =
		DYNAMIC_DOWNCAST(
			DiffCompareFrame,
			GetParentFrame());
	if (pFrame == nullptr)
		return;
	pFrame->FindNextDiff(DIFF_MOVE_DELETE);
}
// ============================================================
// RIGHT VIEW TOOLBAR
// ============================================================
void CDiffGridRightView::OnNextChange()
{
	DiffCompareFrame* pFrame =
		DYNAMIC_DOWNCAST(
			DiffCompareFrame,
			GetParentFrame());

	if (pFrame == nullptr)
		return;

	pFrame->FindNextDiff(DIFF_CHANGE);
}

void CDiffGridRightView::OnNextDelete()
{
	DiffCompareFrame* pFrame =
		DYNAMIC_DOWNCAST(
			DiffCompareFrame,
			GetParentFrame());

	if (pFrame == nullptr)
		return;

	pFrame->FindNextDiff(DIFF_DELETE);
}

void CDiffGridRightView::OnNextInsert()
{
	DiffCompareFrame* pFrame =
		DYNAMIC_DOWNCAST(
			DiffCompareFrame,
			GetParentFrame());

	if (pFrame == nullptr)
		return;

	pFrame->FindNextDiff(DIFF_INSERT);
}

void CDiffGridRightView::OnNextMove()
{
	DiffCompareFrame* pFrame =
		DYNAMIC_DOWNCAST(
			DiffCompareFrame,
			GetParentFrame());

	if (pFrame == nullptr)
		return;

	pFrame->FindNextDiff(DIFF_MOVE_INSERT);
}
void DiffCompareFrame::SetStationInfo()
{
	auto& dc = DataComparison::GetInstance();

	std::vector<CString> leftLines;
	std::vector<CString> rightLines;

	const bool hasOrigin = dc.HasOriginal();
	const bool hasDiff = dc.HasDiff();
	bool hasLeftData = true;
	bool hasRightData = true;
	// 1. 둘 다 데이터가 없으면 중단
	if (!hasOrigin && !hasDiff)
	{
		return;
	}

	// 2. 원본(Left) 데이터 수집 및 예외 처리
	if (hasOrigin)
	{
		auto stationInfo = dc.GetOriginalStation();
		leftLines = ConvertStationInfoText(stationInfo);
		if (leftLines.empty()) {
			leftLines.push_back(_T("원본 데이터가 없습니다."));
			hasLeftData = false;
		}
	}
	else
	{
		leftLines.push_back(_T("선택한 파일이 없습니다."));
	}

	// 3. 비교군(Right) 데이터 수집 및 예외 처리
	if (hasDiff)
	{
		auto diffStationInfo = dc.GetDiffStation();
		rightLines = ConvertStationInfoText(diffStationInfo);
		if (rightLines.empty()) {
			rightLines.push_back(_T("비교할 데이터가 없습니다."));
			hasRightData = false;
		}
	}
	else
	{
		rightLines.push_back(_T("비교할 파일이 없습니다."));
	}

	// 4. 수집된 라인을 전달하여 [Diff 정렬 -> 화면 출력 -> 색상 하이라이트] 일괄 처리
	ApplyCompareOrSingle(hasLeftData, hasRightData, leftLines, rightLines);
}
std::vector<CString> DiffCompareFrame::ConvertStationInfoText(StationInfoType stn) {

	std::vector<CString> lines;
	CString temp;

	lines.push_back(_T("========================================"));
	lines.push_back(_T("              [역 정보]                 "));
	lines.push_back(_T("----------------------------------------"));

	// 1. 기본 식별 번호 (모든 항목 고정 출력)
	temp.Format(_T("StationNo  : %d"), stn.StationNo); lines.push_back(temp);
	temp.Format(_T("LdtsNo     : %d"), stn.LdtsNo); lines.push_back(temp);
	temp.Format(_T("LctcStnNo  : %d"), stn.LctcStnNo); lines.push_back(temp);
	temp.Format(_T("RbcStnNo   : %d"), stn.RbcStnNo); lines.push_back(temp);

	// 2. StationInfo 플래그 (모든 플래그를 ON/OFF로 고정 출력)
	// [구조체 변경] .Value 제거 (StationInfo 는 필드가 바로 노출되는 구조체임)
	// [수정] IsCtcStd/IsCtcStd2 는 별도 필드가 아니라 IsCtc 의 bit1/bit2 이고,
	//        LocalCTC/LCtcMain 도 별도 필드가 아니라 IsRcCtrl 의 bit1/bit2 임
	lines.push_back(_T("")); // 여백 줄 분리
	lines.push_back(_T("--- [역 상태 정보] ---"));
	auto printFlag = [&](bool bOn, LPCTSTR desc) {
		temp.Format(_T(" - [%s] %s"), bOn ? _T("ON ") : _T("OFF"), desc);
		lines.push_back(temp);
		};

	printFlag(IsBitSet(stn.StationInfo.IsCtc, 0), _T("CTC 역 여부"));
	printFlag(IsBitSet(stn.StationInfo.IsCtc, 1), _T("CTC 표준프로토콜(표준1)"));
	printFlag(IsBitSet(stn.StationInfo.IsCtc, 2), _T("CTC 개정프로토콜(표준2)"));
	printFlag(IsBitSet(stn.StationInfo.IsRcCtrl, 0), _T("원격제어"));
	printFlag(IsBitSet(stn.StationInfo.IsRcCtrl, 1), _T("집중화장치 거점역"));
	printFlag(IsBitSet(stn.StationInfo.IsRcCtrl, 2), _T("집중화장치 소속역"));
	printFlag(IsBitSet(stn.StationInfo.TniUse, 0), _T("열번인식기"));
	printFlag(IsBitSet(stn.StationInfo.LmcUse, 0), _T("체류보수"));
	printFlag(IsBitSet(stn.StationInfo.SupportUse, 0), _T("전기설비 기술지원 시스템"));
	printFlag(IsBitSet(stn.StationInfo.RbcUse, 0), _T("RBC통신"));
	printFlag(IsBitSet(stn.StationInfo.CptStn, 0), _T("열차 진입방지 장치(CPT)"));
	printFlag(IsBitSet(stn.StationInfo.IsControlLC, 0), _T("건널목 출력 제어"));
	printFlag(IsBitSet(stn.StationInfo.StnKind, 0), _T("연동역"));
	// [추가] TrkOption (구조체엔 있으나 기존 코드에 없던 필드)
	printFlag(IsBitSet(stn.StationInfo.TrkOption, 0), _T("착점궤도 등 모든궤도에 비상해정 취급 후 해정"));
	printFlag(IsBitSet(stn.StationInfo.TrkOption, 1), _T("궤도별 보호설정 및 해정(버튼) 표시 추가"));

	// 3. StationEquip 장비 설정 플래그 (모두 고정 출력)
	// [구조체 변경] .Value 제거
	// [수정] IsUpsAc 는 bit0/bit1 두 개의 독립 비트가 아니라, bit0 하나로 두 상태
	//        (여자시 정상/여자시 장애)를 구분하는 필드임
	lines.push_back(_T("")); // 여백 줄 분리
	lines.push_back(_T("--- [역 장비 정보] ---"));
	printFlag(IsBitSet(stn.StationEquip.IsEIsFuse, 0), _T("연동논리부 FUSE 있음"));
	printFlag(!IsBitSet(stn.StationEquip.IsUpsAc, 0), _T("UPS AC전원 정상판정 (여자시 정상, default)"));
	printFlag(IsBitSet(stn.StationEquip.IsUpsAc, 0), _T("UPS AC전원 정상판정 논리반전 (여자시 장애)"));
	printFlag(IsBitSet(stn.StationEquip.SigLmrKind, 0), _T("신호기 고장검지 1회선"));
	printFlag(IsBitSet(stn.StationEquip.EtcEquip, 0), _T("히터 개정 전 동작설정"));

	// 4. 통신 및 설비 수량 정보 (0이어도 생략하지 않고 고정 출력)
	// [구조체 변경] CntLdtsDIM / CntLdtsDOM 은 현재 StationInfoType 에 없는 필드라 제외함
	// [구조체 변경] NoOfSdCard(PDO Card 수) 삭제, NoOfModuleSig(신호기 카드 수)/NoOfModuleSwh(선로전환기 카드 수)로 분리됨
	// [추가] NoOfRack / NoOfSO / NoOfSTL / AcrCntRelay / AcrCntAto / AcrCntTs (구조체엔 있으나 기존 코드에 없던 필드)
	lines.push_back(_T("")); // 여백 줄 분리
	lines.push_back(_T("--- [장비 수] ---"));
	temp.Format(_T(" - 진로 수 : %d"), stn.NoOfRouteExt); lines.push_back(temp);
	temp.Format(_T(" - 궤도 수 : %d"), stn.NoOfTrack); lines.push_back(temp);
	temp.Format(_T(" - 신호기 수 : %d"), stn.NoOfSignal); lines.push_back(temp);
	temp.Format(_T(" - 선로전환기 수 : %d"), stn.NoOfSwitch); lines.push_back(temp);
	temp.Format(_T(" - 건널목 수 : %d"), stn.NoOfLevelCross); lines.push_back(temp);
	temp.Format(_T(" - Rack 수 : %d"), stn.NoOfRack); lines.push_back(temp);
	temp.Format(_T(" - 입력 카드 수 : %d"), stn.NoOfInCard); lines.push_back(temp);
	temp.Format(_T(" - 출력 카드 수 : %d"), stn.NoOfOutCard); lines.push_back(temp);
	temp.Format(_T(" - 신호기 카드 수 : %d"), stn.NoOfModuleSig); lines.push_back(temp);
	temp.Format(_T(" - 선로전환기 카드 수 : %d"), stn.NoOfModuleSwh); lines.push_back(temp);
	temp.Format(_T(" - 폐색 제어 수 : %d"), stn.NoOfBlock); lines.push_back(temp);
	temp.Format(_T(" - 전차선 절연구간 수 : %d"), stn.NoOfDeadSection); lines.push_back(temp);
	temp.Format(_T(" - 지장물 수 : %d"), stn.NoOfFallLock); lines.push_back(temp);
	temp.Format(_T(" - 임시속도(Slow Order) 수 : %d"), stn.NoOfSO); lines.push_back(temp);
	temp.Format(_T(" - 출발반응등 수 : %d"), stn.NoOfSTL); lines.push_back(temp);
	temp.Format(_T(" - CPT 수 : %d"), stn.NoOfCpt); lines.push_back(temp);
	temp.Format(_T(" - 히터 수 : %d"), stn.NoOfHeat); lines.push_back(temp);
	temp.Format(_T(" - 연동논리부 정류기 수 : %d"), stn.AcrCntEis); lines.push_back(temp);
	temp.Format(_T(" - AF 정류기 수 : %d"), stn.AcrCntAF); lines.push_back(temp);
	temp.Format(_T(" - 계전기랙 정류기 수 : %d"), stn.AcrCntRelay); lines.push_back(temp);
	temp.Format(_T(" - ATO/TWC 정류기 수 : %d"), stn.AcrCntAto); lines.push_back(temp);
	temp.Format(_T(" - 속도코드 정류기 수 : %d"), stn.AcrCntTs); lines.push_back(temp);

	lines.push_back(_T("")); // 여백 줄 분리
	lines.push_back(_T("--- [비상정지 정보] ---"));
	for (int i = 0; i < MAX_EMG_STOP; ++i)
	{
		for (int dir = 0; dir < 2; ++dir)
		{
			CString dirStr = (dir == 0) ? _T("상선") : _T("하선");
			CString nameStr = GetSafeString(stn.EmgStop[i][dir].Name);
			nameStr.Trim();
			if (nameStr.IsEmpty()) nameStr = _T("(없음)");

			temp.Format(_T(" - 비상정지[%d][%s] 명칭 : %s"), i, (LPCTSTR)dirStr, (LPCTSTR)nameStr);
			lines.push_back(temp);

			CString arrSigs = GetDBNameByArray(stn.EmgStop[i][dir].ArrSignal, SignalIdx);
			if (arrSigs.IsEmpty()) arrSigs = _T("---");
			temp.Format(_T("   > 장내 신호기 : %s"), (LPCTSTR)arrSigs);
			lines.push_back(temp);

			CString depSigs = GetDBNameByArray(stn.EmgStop[i][dir].DepSignal, SignalIdx);
			if (depSigs.IsEmpty()) depSigs = _T("---");
			temp.Format(_T("   > 출발 신호기 : %s"), (LPCTSTR)depSigs);
			lines.push_back(temp);

			// [추가] TrackNo
			CString trkNames = GetDBNameByArray(stn.EmgStop[i][dir].TrackNo, TrackIdx);
			if (trkNames.IsEmpty()) trkNames = _T("---");
			temp.Format(_T("   > 관련 궤도 : %s"), (LPCTSTR)trkNames);
			lines.push_back(temp);

			CString outStr = IsBitSet(stn.EmgStop[i][dir].EmgOut, 0) ? _T("있음") : _T("없음");
			if (IsBitSet(stn.EmgStop[i][dir].EmgOut, 1)) outStr += _T(" (상하선 겸용)");
			temp.Format(_T("   > 출력 : %s"), (LPCTSTR)outStr);
			lines.push_back(temp);
		}
	}

	// 6. 설비 출력 정보 (EquipOut)
	// [구조체 변경] SpeedCtrl / SpeedCtrlNear 는 현재 StationInfoType 에 없는 필드라 제외함
	// [구조체 변경] OutHeater/OutEL/OutCtc/OutLocal/OutMain 은 stn 최상위가 아니라 EquipOut 하위 필드이고,
	//              IO_Position 이 아니라 bit0=1 여부를 나타내는 플래그(Byte_t) 임.
	//              OutMain 도 배열이 아니라 단일 필드임
	lines.push_back(_T("")); // 여백 줄 분리
	lines.push_back(_T("--- [설비 출력 정보] ---"));
	for (int i = 0; i < MAX_OUT_HEATER; i++) {
		CString label; label.Format(_T("Switch Heater 출력[%d]"), i + 1);
		printFlag(IsBitSet(stn.EquipOut.OutHeater[i], 0), label);
	}
	for (int i = 0; i < MAX_OUT_EL; i++) {
		CString label; label.Format(_T("전차선 출력[%d]"), i + 1);
		printFlag(IsBitSet(stn.EquipOut.OutEL[i], 0), label);
	}
	printFlag(IsBitSet(stn.EquipOut.OutCtc, 0), _T("CTC 모드시 출력"));
	printFlag(IsBitSet(stn.EquipOut.OutLocal, 0), _T("Local 모드시 출력"));
	printFlag(IsBitSet(stn.EquipOut.OutMain, 0), _T("주계출력"));

	return lines;
}

// bIsOrigin이 true이면 원본(Origin), false이면 비교군(Diff) 데이터에서 명칭을 조회합니다.
std::vector<CString> DiffCompareFrame::ConvertTrackInfoText(const std::span<TrackInfoType>& trackList, bool bIsOrigin)
{
	std::vector<CString> lines;
	CString temp;

	// [명칭 조회 람다] bIsOrigin 플래그에 맞춰 올바른 CommonUtil 함수를 호출합니다.
	auto GetDBName = [&](BYTE nNum, GetDBNameByNum num) -> CString {
		if (bIsOrigin) {
			return CommonUtil::GetOriginNameByNumber(nNum, num);
		}
		else {
			return CommonUtil::GetDiffNameByNumber(nNum, num);
		}
		};

	// [안전한 궤도 명칭 조회 람다] (현재 순회 중인 궤도 리스트 기준)
	auto GetSafeTrackName = [&](BYTE trackIdx) -> CString {
		if (trackIdx == 0 || trackIdx == 0xFF || trackIdx >= trackList.size()) {
			return _T("-");
		}
		return GetSafeString(trackList[trackIdx].Name, 20);
		};

	// 전달받은 궤도 배열 전체를 순회합니다.
	for (const auto& track : trackList)
	{
		// 유효하지 않은(비어있는) 궤도는 건너뜁니다.
		if (track.Name[0] == 0 || track.Name[0] == 0xFF) continue;

		// 1. 기본 정보 (궤도 명칭)
		lines.push_back(_T("========================================"));
		CString trackName = GetSafeString(track.Name, 20);
		temp.Format(_T("[궤도 명칭] %s"), (LPCTSTR)trackName);
		lines.push_back(temp);
		lines.push_back(_T("----------------------------------------"));

		// 2. Kind (궤도 종류 플래그)
		// [구조체 변경] .Value 제거 (Kind 는 필드가 바로 노출되는 구조체임. 필드명 자체는 모두 일치)
		lines.push_back(_T("[궤도 종류]"));
		CString trackKindOpts = _T("");
		auto appendTrackKind = [&](bool bSet, LPCTSTR desc) {
			if (bSet) {
				if (!trackKindOpts.IsEmpty()) trackKindOpts += _T(", ");
				trackKindOpts += desc;
			}
			};

		appendTrackKind(IsBitSet(track.Kind.ApproachT, 0), _T("접근쇄정 궤도 (ApproachT)"));
		appendTrackKind(IsBitSet(track.Kind.DetectorT, 0), _T("보류쇄정 궤도 (DetectorT)"));
		appendTrackKind(IsBitSet(track.Kind.MainT, 0), _T("본선 궤도 (MainT)"));
		appendTrackKind(IsBitSet(track.Kind.BlockT, 0), _T("폐색 궤도 (BlockT)"));
		appendTrackKind(IsBitSet(track.Kind.IncSignalT, 0), _T("신호기 포함 궤도 (IncSignalT)"));
		appendTrackKind(IsBitSet(track.Kind.IncSwitchT, 0), _T("선로전환기 포함 궤도 (IncSwitchT)"));
		appendTrackKind(IsBitSet(track.Kind.FirstT, 0), _T("구내 첫번째 궤도 / 접근벨 설정 (FirstT)"));
		appendTrackKind(IsBitSet(track.Kind.LockT, 0), _T("폐로쇄정 구간 궤도 (LockT)"));
		appendTrackKind(IsBitSet(track.Kind.CptTrack, 0), _T("CPT 궤도 (CptTrack)"));
		appendTrackKind(IsBitSet(track.Kind.SideTrack, 0), _T("안전측선 또는 출구 (SideTrack bit0)"));
		appendTrackKind(IsBitSet(track.Kind.SideTrack, 1), _T("궤도장애 없음 (SideTrack bit1)"));
		appendTrackKind(IsBitSet(track.Kind.SideTrack, 2), _T("궤도 이상복구 없음 (SideTrack bit2)"));
		appendTrackKind(IsBitSet(track.Kind.SpcTrack, 0), _T("타역 궤도 (SpcTrack)"));
		appendTrackKind(IsBitSet(track.Kind.VirtualTrk, 0), _T("가상궤도 (VirtualTrk bit0)"));
		appendTrackKind(IsBitSet(track.Kind.VirtualTrk, 1), _T("궤도점유로 진로 제어불가 설정 없음 (VirtualTrk bit1)"));

		if (trackKindOpts.IsEmpty()) {
			trackKindOpts = _T("없음");
		}

		CString finalTrackKindLine;
		finalTrackKindLine.Format(_T("  - %s"), (LPCTSTR)trackKindOpts);
		lines.push_back(finalTrackKindLine);

		// 3. TrkDelayTime (복구 시소시간)
		// [구조체 변경] track.AppTime.Value.AppTime 필드는 현재 TrackInfoType 에 없음.
		//              동일한 의미(복구 시소시간)를 갖는 실제 필드는 TrkDelayTime.RecvTime 임
		lines.push_back(_T("[궤도복구 시소시간]"));
		BYTE appTimeVal = track.TrkDelayTime.RecvTime;
		CString timeStr = _T("2000 ms (default)");
		if (IsBitSet(appTimeVal, 0)) timeStr = _T("0 초 - 사용하지 않음");
		else if (IsBitSet(appTimeVal, 1)) timeStr = _T("500 ms");
		else if (IsBitSet(appTimeVal, 2)) timeStr = _T("1000 ms");
		else if (IsBitSet(appTimeVal, 3)) timeStr = _T("1200 ms");
		else if (IsBitSet(appTimeVal, 4)) timeStr = _T("1500 ms");
		else if (IsBitSet(appTimeVal, 5)) timeStr = _T("1800 ms");
		else if (IsBitSet(appTimeVal, 6)) timeStr = _T("2500 ms");
		else if (IsBitSet(appTimeVal, 7)) timeStr = _T("3000 ms");
		temp.Format(_T("  - 시소시간: %s"), (LPCTSTR)timeStr);
		lines.push_back(temp);

		// 4. Equipment (장비 수)
		lines.push_back(_T("[설비 수량]"));
		temp.Format(_T("  - 선로전환기 수: %d, 신호기 수: %d"), track.Equipment.NoOfSwitch, track.Equipment.NoOfSignal);
		lines.push_back(temp);

		// 5. Switch 배열
		bool bHasSwitch = false;
		CString switchListStr = _T("");
		for (int i = 0; i < _countof(track.Switch); ++i)
		{
			BYTE swNo = track.Switch[i].SwitchNo;
			if (swNo != 0 && swNo != 0xFF)
			{
				CString swName = GetDBName(swNo, SwitchIdx); // 원본/비교군 자동 분기
				if (bHasSwitch) switchListStr += _T(", ");
				temp.Format(_T("%d(%s)"), swNo, (LPCTSTR)swName);
				switchListStr += temp;
				bHasSwitch = true;
			}
		}
		if (bHasSwitch)
		{
			lines.push_back(_T("[포함된 선로전환기]"));
			lines.push_back(_T("  - ") + switchListStr);
		}
		else {
			lines.push_back(_T("[포함된 선로전환기]"));
			lines.push_back(_T("  - 없음"));
		}

		// 6. 인접 궤도 및 연계 번호
		lines.push_back(_T("[인접 및 연계 번호]"));
		CString leftName = GetSafeTrackName(track.LeftTrack);
		CString rightName = GetSafeTrackName(track.RightTrack);
		temp.Format(_T("  - Left 궤도: %d (%s), Right 궤도: %d (%s)"), track.LeftTrack, (LPCTSTR)leftName, track.RightTrack, (LPCTSTR)rightName);
		lines.push_back(temp);

		CString incSignals = _T("");
		bool bHasIncSig = false;
		for (int i = 0; i < _countof(track.IncSignal); ++i)
		{
			BYTE sigNo = track.IncSignal[i];
			if (sigNo != 0 && sigNo != 0xFF)
			{
				CString sigName = GetDBName(sigNo, SignalIdx);
				if (bHasIncSig) incSignals += _T(", ");
				CString tmp;
				tmp.Format(_T("%s"), (LPCTSTR)sigName);
				incSignals += tmp;
				bHasIncSig = true;
			}
		}
		if (!incSignals.IsEmpty() && incSignals != _T("-"))
		{
			temp.Format(_T("  - 궤도 포함된 신호기: %s"), (LPCTSTR)incSignals);
			lines.push_back(temp);
		}
		else {
			lines.push_back(_T(" - 궤도 포함된 신호기 X"));
		}

		CString blockName = GetDBName(track.InBlockNo, BlockIdx);
		CString cptName = GetDBName(track.CptNo, CPTIdx);
		temp.Format(_T("  - 관련 폐색표시: %d (%s)"), track.InBlockNo, (LPCTSTR)blockName);
		lines.push_back(temp);
		temp.Format(_T("  - 관련 CPT 번호: %d (%s)"), track.CptNo, (LPCTSTR)cptName);
		lines.push_back(temp);
		temp.Format(_T("  - 열번창 Table Index : %d"), track.TrnIdx);
		lines.push_back(temp);

		// [삭제] SpeedType/AutoRteNo/TrkLockSwitch 는 현재 TrackInfoType 구조체에 존재하지 않는 필드임
		// (구조체 전체를 재검토했으나 동일 의미의 대체 필드도 없어 완전히 제거함)

		// 7. PlatForm
		lines.push_back(_T("[플랫폼 정보]"));
		temp.Format(_T("  - 플랫폼 번호: %d"), track.PlatForm.PlatFormNo);
		lines.push_back(temp);
		// [구조체 변경] .Value 제거 (PlatForm.Kind 는 필드가 바로 노출되는 구조체임)
		if (IsBitSet(track.PlatForm.Kind.PlatForm, 0)) {
			lines.push_back(_T("  - 플랫폼 궤도: Yes"));
		}
		if (IsBitSet(track.PlatForm.Kind.UpTrack, 0)) {
			lines.push_back(_T("  - 선로 구분: 하선궤도"));
		}
		else {
			lines.push_back(_T("  - 선로 구분: 상선궤도"));
		}

		// 궤도 데이터 간의 여백 추가
		lines.push_back(_T(""));
	}

	return lines;
}
void DiffCompareFrame::SetTrackInfo()
{
	auto& dc = DataComparison::GetInstance();

	std::vector<CString> leftLines;
	std::vector<CString> rightLines;

	const bool hasOrigin = dc.HasOriginal();
	const bool hasDiff = dc.HasDiff();
	bool hasLeftData = true;
	bool hasRightData = true;
	// 1. 둘 다 데이터가 없으면 중단
	if (!hasOrigin && !hasDiff)
	{
		return;
	}

	// 2. 원본(Left) 데이터 수집 및 예외 처리
	if (hasOrigin)
	{
		auto originalTracks = dc.GetOriginalTrack();
		leftLines = ConvertTrackInfoText(originalTracks, true);
		if (leftLines.empty()) {
			leftLines.push_back(_T("원본 데이터가 없습니다."));
			hasLeftData = false;
		}
	}
	else
	{
		leftLines.push_back(_T("선택한 파일이 없습니다."));
	}

	// 3. 비교군(Right) 데이터 수집 및 예외 처리
	if (hasDiff)
	{
		auto diffTracks = dc.GetDiffTrack();
		rightLines = ConvertTrackInfoText(diffTracks, false);
		if (rightLines.empty()) {
			rightLines.push_back(_T("비교 데이터가 없습니다."));
			hasRightData = false;
		}
	}
	else
	{
		rightLines.push_back(_T("비교할 파일이 없습니다."));
	}

	ApplyCompareOrSingle(hasLeftData, hasRightData, leftLines, rightLines);
}

void DiffCompareFrame::SetSignalInfo()
{
	auto& dc = DataComparison::GetInstance();

	std::vector<CString> leftLines;
	std::vector<CString> rightLines;

	const bool hasOrigin = dc.HasOriginal();
	const bool hasDiff = dc.HasDiff();
	bool HasLeftData = true;
	bool HasRightData = true;
	// 1. 둘 다 데이터가 없으면 중단
	if (!hasOrigin && !hasDiff)
	{
		return;
	}

	// 2. 원본(Left) 데이터 수집 및 예외 처리
	if (hasOrigin)
	{
		auto originalSignals = dc.GetOriginalSignal();
		leftLines = ConvertSignalInfoText(originalSignals, true);
		if (leftLines.empty())
		{
			leftLines.push_back(_T("원본 데이터가 없습니다."));
			HasLeftData = false;
		}
	}
	else
	{
		leftLines.push_back(_T("선택한 파일이 없습니다."));
	}

	// 3. 비교군(Right) 데이터 수집 및 예외 처리
	if (hasDiff)
	{
		auto diffSignals = dc.GetDiffSignal();
		rightLines = ConvertSignalInfoText(diffSignals, false);
		if (rightLines.empty())
		{
			rightLines.push_back(_T("비교군 데이터가 없습니다."));
			HasRightData = false;
		}
	}
	else
	{
		rightLines.push_back(_T("비교할 파일이 없습니다."));
	}

	// 4. 수집된 라인을 전달하여 [Diff 정렬 -> 화면 출력 -> 색상 하이라이트] 일괄 처리
	ApplyCompareOrSingle(HasLeftData, HasRightData, leftLines, rightLines);
}
std::vector<CString> DiffCompareFrame::ConvertSignalInfoText(const std::span<SignalInfoType>& signalList, bool bIsOrigin)
{
	std::vector<CString> lines;
	CString temp;

	// 원본인지 비교군인지에 따라 CommonUtil 함수를 선택하는 람다
	auto GetDBName = [&](BYTE nNum, GetDBNameByNum num) -> CString {
		if (bIsOrigin) {
			return CommonUtil::GetOriginNameByNumber(nNum, num);
		}
		else {
			return CommonUtil::GetDiffNameByNumber(nNum, num);
		}
		};

	// 전달받은 신호기 배열 전체를 순회합니다.
	for (const auto& signal : signalList)
	{
		// 유효하지 않은(비어있는) 신호기는 건너뜁니다.
		if (signal.Name[0] == 0 || signal.Name[0] == 0xFF) continue;

		// 1. 기본 정보 (신호기 명칭)
		lines.push_back(_T("========================================"));
		CString sigName = GetSafeString(signal.Name, 20);
		temp.Format(_T("[신호기 명칭] %s"), (LPCTSTR)sigName);
		lines.push_back(temp);
		lines.push_back(_T("----------------------------------------"));

		// 2. 신호기 종류 (Kind)
		// [구조체 변경] .Value 제거 (Kind 는 필드가 바로 노출되는 구조체임)
		// [구조체 변경] IncRepeatS 는 현재 SignalInfoType.Kind 에 없는 필드라 제외함
		//              (중계신호기 포함 여부는 RepeatSigNo > 0 으로 판단, 아래 4번 항목 참고)
		lines.push_back(_T("[신호기 종류 (Kind)]"));
		CString signalOptions = _T("");
		auto appendSignal = [&](bool bSet, LPCTSTR desc) {
			if (bSet) {
				if (!signalOptions.IsEmpty()) signalOptions += _T(", ");
				signalOptions += desc;
			}
			};

		appendSignal(IsBitSet(signal.Kind.MainS, 0), _T("주신호기 (MainS)"));
		appendSignal(IsBitSet(signal.Kind.ShuntD, 0), _T("입환 표지 (ShuntD)"));
		appendSignal(IsBitSet(signal.Kind.ShuntS, 0), _T("입환 신호기 (ShuntS)"));
		appendSignal(IsBitSet(signal.Kind.BlockS, 0), _T("폐색 신호기 (BlockS)"));
		appendSignal(IsBitSet(signal.Kind.HomeBlockS, 0), _T("구내 폐색 신호기 (HomeBlockS)"));
		appendSignal(IsBitSet(signal.Kind.CallOnS, 0), _T("유도등 포함 (CallOnS)"));
		appendSignal(IsBitSet(signal.Kind.RepeatS, 0), _T("중계 신호기 (RepeatS)"));
		appendSignal(IsBitSet(signal.Kind.UmhoSig, 0), _T("엄호신호기 (UmhoSig)"));
		appendSignal(IsBitSet(signal.Kind.IsTTB, 0), _T("TTB 존재 (IsTTB)"));
		appendSignal(IsBitSet(signal.Kind.CptSignal, 0), _T("CPT 신호기 (CptSignal)"));
		appendSignal(IsBitSet(signal.Kind.SpcSignal, 0), _T("타역 신호기 (SpcSignal)"));

		if (signalOptions.IsEmpty()) {
			signalOptions = _T("없음");
		}

		CString finalSignalLine;
		finalSignalLine.Format(_T("  - %s"), (LPCTSTR)signalOptions);
		lines.push_back(finalSignalLine);
		// 3. 진로 및 현시 수
		lines.push_back(_T("[진로 및 현시 수]"));
		temp.Format(_T("  - 진로 수 (NoOfRoute): %d, 현시 수 (NoOfLight): %d"), signal.NoOfRoute, signal.NoOfLight);
		lines.push_back(temp);

		// 4. 관련 궤도 및 연계 신호기 번호 (CommonUtil 명칭 자동 조회 적용)
		lines.push_back(_T("[관련 설비 및 번호]"));
		CString trackName = GetDBName(signal.TrackNo, TrackIdx);
		CString repeatSigName = GetDBName(signal.RepeatSigNo, SignalIdx);
		CString frontSigName = GetDBName(signal.FrontSignalNo, SignalIdx);

		temp.Format(_T("  - 신호기 궤도: %d (%s)"), signal.TrackNo, (LPCTSTR)trackName);
		lines.push_back(temp);
		temp.Format(_T("  - 중계 신호기 번호: %d (%s)"), signal.RepeatSigNo, (LPCTSTR)repeatSigName);
		lines.push_back(temp);
		temp.Format(_T("  - 전방 신호기 번호: %d (%s)"), signal.FrontSignalNo, (LPCTSTR)frontSigName);
		lines.push_back(temp);

		// 신호기 데이터 간 여백 추가
		lines.push_back(_T(""));
	}

	return lines;
}
void DiffCompareFrame::SetSwitchInfo()
{
	auto& dc = DataComparison::GetInstance();

	std::vector<CString> leftLines;
	std::vector<CString> rightLines;

	const bool hasOrigin = dc.HasOriginal();
	const bool hasDiff = dc.HasDiff();
	bool hasLeftData = true;
	bool hasRightData = true;
	// 1. 둘 다 데이터가 없으면 중단
	if (!hasOrigin && !hasDiff)
	{
		return;
	}

	// 2. 원본(Left) 데이터 수집 및 예외 처리
	if (hasOrigin)
	{
		auto originalSwitches = dc.GetOriginalSwitch();
		leftLines = ConvertSwitchInfoText(originalSwitches, true);
		if (leftLines.empty())
		{
			leftLines.push_back(_T("원본 데이터가 없습니다."));
			hasLeftData = false;
		}
	}
	else
	{
		leftLines.push_back(_T("선택한 파일이 없습니다."));
	}

	// 3. 비교군(Right) 데이터 수집 및 예외 처리
	if (hasDiff)
	{
		auto diffSwitches = dc.GetDiffSwitch();
		rightLines = ConvertSwitchInfoText(diffSwitches, false);
		if (rightLines.empty()) {
			rightLines.push_back(_T("비교 데이터가 없습니다."));
			hasRightData = false;
		}
	}
	else
	{
		rightLines.push_back(_T("비교할 파일이 없습니다."));
	}


	ApplyCompareOrSingle(hasLeftData, hasRightData, leftLines, rightLines);
}
std::vector<CString> DiffCompareFrame::ConvertSwitchInfoText(const std::span<SwitchInfoType>& switchList, bool bIsOrigin)
{
	std::vector<CString> lines;
	CString temp;

	// 원본인지 비교군인지에 따라 CommonUtil 함수를 선택하는 람다
	auto GetDBName = [&](BYTE nNum, GetDBNameByNum num) -> CString {
		if (bIsOrigin) {
			return CommonUtil::GetOriginNameByNumber(nNum, num);
		}
		else {
			return CommonUtil::GetDiffNameByNumber(nNum, num);
		}
		};

	// 전달받은 선로전환기 배열 전체를 순회합니다.
	for (const auto& sw : switchList)
	{
		// 유효하지 않은(비어있는) 선로전환기는 건너뜁니다.
		if (sw.Name[0] == 0 || sw.Name[0] == 0xFF) continue;

		// 1. 기본 정보 (선로전환기 명칭)
		lines.push_back(_T("========================================"));
		CString swName = GetSafeString(sw.Name, 20);
		temp.Format(_T("[선로전환기 명칭] %s"), (LPCTSTR)swName);
		lines.push_back(temp);
		lines.push_back(_T("----------------------------------------"));

		// 2. 종류 (Kind)
		// [구조체 변경] .Value 제거. Twin 은 현재 SwitchInfoType.Kind 에 없는 필드라 제외함
		lines.push_back(_T("[선로전환기 종류 (Kind)]"));
		CString switchOptions = _T("");
		auto appendSwitch = [&](bool bSet, LPCTSTR desc) {
			if (bSet) {
				if (!switchOptions.IsEmpty()) switchOptions += _T(", ");
				switchOptions += desc;
			}
			};

		appendSwitch(IsBitSet(sw.Kind.Single, 0), _T("단동 선로전환기 (Single)"));
		appendSwitch(IsBitSet(sw.Kind.Double, 0), _T("쌍동 선로전환기 (Double)"));
		appendSwitch(IsBitSet(sw.Kind.Triple, 0), _T("삼동 선로전환기 (Triple)"));
		appendSwitch(IsBitSet(sw.Kind.FourTime, 0), _T("사동 선로전환기 (FourTime)"));
		appendSwitch(IsBitSet(sw.Kind.Scissors, 0), _T("시서스 선로전환기 (Scissors)"));
		appendSwitch(IsBitSet(sw.Kind.Nose, 0), _T("노스가동 선로전환기 (Nose)"));
		appendSwitch(IsBitSet(sw.Kind.SpcSwitch, 0), _T("타역 선로전환기 (SpcSwitch)"));

		if (switchOptions.IsEmpty()) {
			switchOptions = _T("없음");
		}

		CString finalSwitchLine;
		finalSwitchLine.Format(_T("  - %s"), (LPCTSTR)switchOptions);
		lines.push_back(finalSwitchLine);
		// 3. 점 궤도 번호 (A, B, C, D 궤도)
		// [구조체 변경] ATrackNo/BTrackNo/CTrackNo/DTrackNo 는 현재 SwitchInfoType 에 없음.
		//              실제 필드는 SwhTrack[4] 배열임 ([0]=A,[1]=B,[2]=C,[3]=D)
		lines.push_back(_T("[관련 궤도 번호]"));
		CString aTrk = GetDBName(sw.SwhTrack[0], TrackIdx);
		CString bTrk = GetDBName(sw.SwhTrack[1], TrackIdx);
		CString cTrk = GetDBName(sw.SwhTrack[2], TrackIdx);
		CString dTrk = GetDBName(sw.SwhTrack[3], TrackIdx);

		if (sw.SwhTrack[0] != 0 && sw.SwhTrack[0] != 0xFF) {
			temp.Format(_T("  - A 점 궤도: %s"), (LPCTSTR)aTrk);
			lines.push_back(temp);
		}
		if (sw.SwhTrack[1] != 0 && sw.SwhTrack[1] != 0xFF) {
			temp.Format(_T("  - B 점 궤도: %s"), (LPCTSTR)bTrk);
			lines.push_back(temp);
		}
		if (sw.SwhTrack[2] != 0 && sw.SwhTrack[2] != 0xFF) {
			temp.Format(_T("  - C 점 궤도: %s"), (LPCTSTR)cTrk);
			lines.push_back(temp);
		}
		if (sw.SwhTrack[3] != 0 && sw.SwhTrack[3] != 0xFF) {
			temp.Format(_T("  - D 점 궤도: %s"), (LPCTSTR)dTrk);
			lines.push_back(temp);
		}

		// 4. [구조체 변경] LockTrack(단순 궤도번호 배열)은 현재 SwitchInfoType 에 없음.
		//    설비상태에 대한 쇄정 정보는 SwitchLockEquip.LockEquipStat[] (구분코드+Index+상태) 로 재구성됨
		// [추가] 구조체엔 있으나 기존 코드에 없던 SwitchLockEquip 정보
		CString lockEquipStr = _T("");
		for (int i = 0; i < MAX_SWITCH_LOCK_EQUIP; ++i)
		{
			const auto& eq = sw.SwitchLockEquip.LockEquipStat[i];
			if (eq.Kind == 0 && eq.TblNo == 0) continue;
			CString tmp;
			tmp.Format(_T("%c%d(0x%02X)"), eq.Kind, eq.TblNo, eq.EquipStat);
			if (!lockEquipStr.IsEmpty()) lockEquipStr += _T(", ");
			lockEquipStr += tmp;
		}
		if (!lockEquipStr.IsEmpty() || sw.SwitchLockEquip.LockTime > 0)
		{
			lines.push_back(_T("[설비상태에 대한 쇄정 (SwitchLockEquip)]"));
			if (!lockEquipStr.IsEmpty())
			{
				temp.Format(_T("  - 대상 설비: %s"), (LPCTSTR)lockEquipStr);
				lines.push_back(temp);
			}
			if (IsBitSet(sw.SwitchLockEquip.SwitchDir, 0)) lines.push_back(_T("  - 정위일 때 쇄정"));
			if (IsBitSet(sw.SwitchLockEquip.SwitchDir, 1)) lines.push_back(_T("  - 반위일 때 쇄정"));
			if (IsBitSet(sw.SwitchLockEquip.SwitchDir, 2)) lines.push_back(_T("  - 정위상태 아니면 정위전환 후 쇄정"));
			if (sw.SwitchLockEquip.LockTime == 255) lines.push_back(_T("  - 쇄정시간: 계속 쇄정"));
			else if (sw.SwitchLockEquip.LockTime > 0) {
				temp.Format(_T("  - 쇄정시간: %d 초"), sw.SwitchLockEquip.LockTime);
				lines.push_back(temp);
			}
		}

		// 5. 자동정위 전환 (AutoSwitch) 정보
		// [구조체 변경] AutoSwitch.Kind 는 단일 Byte_t(설비구분코드 'S'/'T'/'P'/'B') 필드이며
		//              .Value.AutoNormalIn / .AutoNormalRte 같은 하위 비트필드가 아님.
		//              해당 조건은 AutoSwitch.AutoNormalCond 로 재구성됨
		//              (bit0/bit1 의미는 Kind 값에 따라 다르고, bit7=1 이면 설비상태 관계없이 자동전환)
		if (sw.AutoSwitch.AutoSwhTm > 0 || sw.AutoSwitch.AutoNormalCond > 0 || sw.AutoSwitch.Kind != 0)
		{
			lines.push_back(_T("[자동정위 전환 설정 (AutoSwitch)]"));
			if (sw.AutoSwitch.Kind != 0 || sw.AutoSwitch.TblNo != 0) {
				temp.Format(_T("  - 조건 설비: %c-%d"), sw.AutoSwitch.Kind, sw.AutoSwitch.TblNo);
				lines.push_back(temp);
			}
			temp.Format(_T("  - 전환 시간 (AutoSwhTm): %d 초"), sw.AutoSwitch.AutoSwhTm);
			lines.push_back(temp);
			// [추가] Lock (구조체엔 있으나 기존 코드에 없던 필드)
			if (IsBitSet(sw.AutoSwitch.Lock, 0)) lines.push_back(_T("  - 전환 후 쇄정 동작 (Lock)"));
			if (IsBitSet(sw.AutoSwitch.AutoNormalCond, 7)) {
				lines.push_back(_T("  - 설비상태 관계없이 자동정위 전환"));
			}
			else if (sw.AutoSwitch.AutoNormalCond > 0) {
				temp.Format(_T("  - 자동정위전환 조건(AutoNormalCond) : 0x%02X"), sw.AutoSwitch.AutoNormalCond);
				lines.push_back(temp);
			}
		}

		// 선로전환기 데이터 간 여백 추가
		lines.push_back(_T(""));
	}

	return lines;
}
void DiffCompareFrame::SetBlockInfo()
{
	auto& dc = DataComparison::GetInstance();

	std::vector<CString> leftLines;
	std::vector<CString> rightLines;

	const bool hasOrigin = dc.HasOriginal();
	const bool hasDiff = dc.HasDiff();
	bool hasRightData = true;
	bool hasLeftData = true;

	// 1. 원본(Left) 데이터 수집 및 예외 처리
	if (hasOrigin)
	{
		auto originalBlocks = dc.GetOriginalBlock();
		leftLines = ConvertBlockInfoText(originalBlocks, true);
		if (leftLines.empty())
		{
			hasLeftData = false;
			leftLines.push_back(_T("원본 데이터가 없습니다."));
		}
	}
	else
	{
		leftLines.push_back(_T("선택한 파일이 없습니다."));
	}

	// 2. 비교군(Right) 데이터 수집 및 예외 처리
	if (hasDiff)
	{
		auto diffBlocks = dc.GetDiffBlock();
		rightLines = ConvertBlockInfoText(diffBlocks, false);
		if (rightLines.empty())
		{
			hasRightData = false;
			rightLines.push_back(_T("비교 데이터가 없습니다."));
		}
	}
	else
	{
		rightLines.push_back(_T("비교할 파일이 없습니다."));
	}

	// 3. 둘 다 데이터가 없는 경우 처리 중단
	ApplyCompareOrSingle(hasLeftData, hasRightData, leftLines, rightLines);
}
std::vector<CString> DiffCompareFrame::ConvertBlockInfoText(const std::span<BlockTagInfoType>& blockList, bool bIsOrigin)
{
	std::vector<CString> lines;
	CString temp;

	// 원본인지 비교군인지에 따라 CommonUtil 함수를 선택하는 람다
	auto GetDBName = [&](BYTE nNum, GetDBNameByNum num) -> CString {
		if (bIsOrigin) {
			return CommonUtil::GetOriginNameByNumber(nNum, num);
		}
		else {
			return CommonUtil::GetDiffNameByNumber(nNum, num);
		}
		};

	// 전달받은 폐색 배열 전체를 순회합니다.
	for (const auto& blk : blockList)
	{
		// 유효하지 않은(비어있는) 폐색은 건너뜁니다.
		if (blk.Name[0] == 0 || blk.Name[0] == 0xFF) continue;

		// 1. 기본 정보 (폐색 명칭)
		lines.push_back(_T("========================================"));
		CString blkName = GetSafeString(blk.Name, 20);
		temp.Format(_T("[폐색 명칭] %s"), (LPCTSTR)blkName);
		lines.push_back(temp);
		lines.push_back(_T("----------------------------------------"));

		// [추가] BlkKind(폐색 종류)는 BlockAspect(현시 수)와 조합해야 실제 신호 입력 명칭이
		// 정해지므로, 맨 앞에서 먼저 표시 (BitUtility.h의 DescribeBlkKind_Aspect 사용)
		temp.Format(_T("[폐색 종류] %s"), (LPCTSTR)DescribeBlkKind_Aspect(blk.BlkKind, blk.BlockAspect));
		lines.push_back(temp);

		lines.push_back(_T("[폐색 종류 및 옵션]"));

		// 비트가 켜져 있는(설정된) 경우에만 항목을 추가하는 헬퍼 람다
		CString kindOptions = _T("");
		auto appendKind = [&](bool bSet, LPCTSTR desc) {
			if (bSet) {
				if (!kindOptions.IsEmpty()) kindOptions += _T(", ");
				kindOptions += desc;
			}
			};
		// BitUtility.h의 namespace 전용 함수(DescribeRevStartRed 등)가 반환한 문자열을
		// kindOptions에 합쳐주는 헬퍼. 켜진 비트가 없으면 빈 문자열이 오므로 그대로 건너뜀.
		auto appendKindText = [&](const CString& text) {
			if (text.IsEmpty()) return;
			if (!kindOptions.IsEmpty()) kindOptions += _T(", ");
			kindOptions += text;
			};
		// [구조체 변경] .Value 제거. ExpBlk/MetroRev/JeonlaRev 는 KindInfo 에 없는 필드이며,
		//              실제로는 KindInfo.RevKind 의 bit0(서울교통 3,4호선 역방향)/bit1(전라선 양방향)임.
		//              RevStartRed/RevArrSig 는 KindInfo 가 아니라 BlockBOthInfo 하위 필드로 재구성됨.
		//              OutKind bit2 로 쓰던 값은 실제로 BlockBOthInfo.DepSig(bit0)임

		appendKind(IsBitSet(blk.KindInfo.RevKind, 0), _T("서울교통 3,4호선 역방향 폐색"));
		appendKind(IsBitSet(blk.KindInfo.RevKind, 1), _T("전라선 양방향 폐색 (0:경부선 양방향 폐색)"));
		appendKindText(DescribeRevStartRed(blk.BlockBOthInfo.RevStartRed));
		appendKindText(DescribeOutKind(blk.KindInfo.OutKind));
		appendKind(IsBitSet(blk.BlockBOthInfo.DepSig, 0), _T("폐색 YY 여자 또는 진로 착점궤도 여자시 출발신호 현시(4현시)"));
		appendKindText(DescribeRevArrSig(blk.BlockBOthInfo.RevArrSig));
		// [추가] DispKind (구조체엔 있으나 기존 코드에 없던 필드)
		appendKind(IsBitSet(blk.KindInfo.DispKind, 0), _T("출발/정방향 개통표시등 있음(화면표시)"));
		appendKind(IsBitSet(blk.KindInfo.DispKind, 1), _T("양방향폐색 출발 적색점멸 있음(출발RF)"));
		appendKind(IsBitSet(blk.KindInfo.DispKind, 2), _T("양방향폐색 출발 황색점멸 있음(출발YF)"));
		appendKind(IsBitSet(blk.KindInfo.DispKind, 3), _T("양방향폐색 장내 적색점멸 있음(장내RF)"));
		appendKind(IsBitSet(blk.KindInfo.DispKind, 4), _T("양방향폐색 장내 황색점멸 있음(장내YF)"));

		// 설정된 옵션이 하나도 없으면 "없음"을 찍지 않고 줄 자체를 생략, 있는 것만 출력
		if (!kindOptions.IsEmpty()) {
			CString finalLine;
			finalLine.Format(_T("  - %s"), (LPCTSTR)kindOptions);
			lines.push_back(finalLine);
		}

		// 3. 폐색 구분 및 연계 번호
		lines.push_back(_T("[폐색 구분 및 관련 설비]"));


		CString dirStr = _T("");
		if (blk.DirKind == 1) dirStr = _T("장내");
		else if (blk.DirKind == 2) dirStr = _T("출발");
		temp.Format(_T("  - 폐색 구분 (DirKind): %d (%s)"), blk.DirKind, (LPCTSTR)dirStr);
		lines.push_back(temp);

		CString oppBlkName = GetDBName(blk.OppositeBlock, BlockIdx);
		CString rearBlkName = GetDBName(blk.RearBlock, BlockIdx);
		CString umhoSigName = GetDBName(blk.UmhoSignal, SignalIdx);
		CString arrSigName = GetDBName(blk.ArrivalSignal, SignalIdx);

		temp.Format(_T("  - 상대 폐색 (OppositeBlock): %d (%s)"), blk.OppositeBlock, (LPCTSTR)oppBlkName);
		lines.push_back(temp);
		temp.Format(_T("  - 양방향 폐색 번호 (RearBlock): %d (%s)"), blk.RearBlock, (LPCTSTR)rearBlkName);
		lines.push_back(temp);
		temp.Format(_T("  - 엄호신호기 번호 (UmhoSignal): %d (%s)"), blk.UmhoSignal, (LPCTSTR)umhoSigName);
		lines.push_back(temp);
		temp.Format(_T("  - 양방향 폐색 장내 신호기 (ArrivalSignal): %d (%s)"), blk.ArrivalSignal, (LPCTSTR)arrSigName);
		lines.push_back(temp);

		// 4. 장내/출발 관련 궤도 배열
		CString arrTracks = _T("");
		bool bHasArrTrk = false;
		for (int i = 0; i < _countof(blk.ArrTrack); ++i)
		{
			BYTE trkNo = blk.ArrTrack[i];
			if (trkNo != 0 && trkNo != 0xFF)
			{
				CString trkName = GetDBName(trkNo, TrackIdx);
				if (bHasArrTrk) arrTracks += _T(", ");
				CString tmp; tmp.Format(_T("%d(%s)"), trkNo, (LPCTSTR)trkName);
				arrTracks += tmp;
				bHasArrTrk = true;
			}
		}
		if (!arrTracks.IsEmpty())
		{
			temp.Format(_T("  - 장내 관련 궤도 (ArrTrack): %s"), (LPCTSTR)arrTracks);
			lines.push_back(temp);
		}

		CString depTracks = _T("");
		bool bHasDepTrk = false;
		for (int i = 0; i < _countof(blk.DepTrack); ++i)
		{
			BYTE trkNo = blk.DepTrack[i];
			if (trkNo != 0 && trkNo != 0xFF)
			{
				CString trkName = GetDBName(trkNo, TrackIdx);
				if (bHasDepTrk) depTracks += _T(", ");
				CString tmp; tmp.Format(_T("%d(%s)"), trkNo, (LPCTSTR)trkName);
				depTracks += tmp;
				bHasDepTrk = true;
			}
		}
		if (!depTracks.IsEmpty())
		{
			temp.Format(_T("  - 출발/개통 관련 궤도 (DepTrack): %s"), (LPCTSTR)depTracks);
			lines.push_back(temp);
		}

		CString arrFirstTrkName = GetDBName(blk.ArrFirstTrk, TrackIdx);
		CString depLastTrkName = GetDBName(blk.DepLastTrk, TrackIdx);
		temp.Format(_T("  - 역방향 장내진로 첫 궤도 (ArrFirstTrk): %d (%s)"), blk.ArrFirstTrk, (LPCTSTR)arrFirstTrkName);
		lines.push_back(temp);
		temp.Format(_T("  - 역방향 출발진로 착점궤도 (DepLastTrk): %d (%s)"), blk.DepLastTrk, (LPCTSTR)depLastTrkName);
		lines.push_back(temp);

		// 5. 타이머 설정 (OutputTm, DelayTm, CancelTm)
		lines.push_back(_T("[폐색 타이머 설정]"));
		temp.Format(_T("  - 출력 최대 시간 (OutputTm): %d ms"), blk.OutputTm);
		lines.push_back(temp);
		temp.Format(_T("  - 입력처리 대기시간 (DelayTm): %d ms"), blk.DelayTm);
		lines.push_back(temp);
		temp.Format(_T("  - 취소 출력 최대 시간 (CancelTm): %d ms"), blk.CancelTm);
		lines.push_back(temp);

		// 6. BlockDepRed (출발폐색 적색 표시)
		// [구조체 변경] DepBlkRedTrk 는 blk 최상위가 아니라 BlockDepRed 하위 필드임
		// [추가] DepRedKind (구조체엔 있으나 기존 코드에 없던 필드)
		if (IsBitSet(blk.BlockDepRed.DepRedKind, 0)) lines.push_back(_T("  - 궤도 상관없이 폐색 BLTR 낙하시 적색표시 (DepRedKind bit0)"));
		if (IsBitSet(blk.BlockDepRed.DepRedKind, 1)) lines.push_back(_T("  - 적색표시 궤도 낙하시 적색표시 (DepRedKind bit1, DepBlkRedTrk 설정 필요)"));

		CString redTracks = _T("");
		bool bHasRedTrk = false;
		for (int i = 0; i < _countof(blk.BlockDepRed.DepBlkRedTrk); ++i)
		{
			BYTE trkNo = blk.BlockDepRed.DepBlkRedTrk[i];
			if (trkNo != 0 && trkNo != 0xFF)
			{
				CString trkName = GetDBName(trkNo, TrackIdx);
				if (bHasRedTrk) redTracks += _T(", ");
				CString tmp; tmp.Format(_T("%d(%s)"), trkNo, (LPCTSTR)trkName);
				redTracks += tmp;
				bHasRedTrk = true;
			}
		}
		if (!redTracks.IsEmpty())
		{
			temp.Format(_T("  - 출발폐색 적색 표시 궤도 (DepBlkRedTrk): %s"), (LPCTSTR)redTracks);
			lines.push_back(temp);
		}

		// 7. 출력 조건 (BlockCond) - 출력 구분(Gubun)과 포트(OutPort).
		// DeviceGridInfo::GetBlockInfoString의 [출력 포트] 디코딩과 동일한 방식.
		// 켜진 조건이 하나도 없으면 섹션 자체를 생략 (있는 것만 출력).
		std::vector<CString> condLines;
		for (int i = 0; i < MAX_BLOCK_COND; ++i)
		{
			const auto& cond = blk.BlockCond[i];
			if (cond.Gubun == 0) continue; // 미사용 조건은 건너뜀

			CString strLabel;
			switch (cond.Gubun)
			{
			case 'C': strLabel = _T("CNR");    break; // 출력 CNR
			case 'R': strLabel = _T("RR(ZR)"); break; // 출력 RR(ZR)
			case 'E': strLabel = _T("기타");    break; // 기타 출력
			default:  strLabel.Format(_T("%c"), cond.Gubun); break;
			}

			CString condLine;
			condLine.Format(_T("  - [%s] 출력 포트: %s"), (LPCTSTR)strLabel, (LPCTSTR)GetSafeIOPosition(cond.OutPort));
			condLines.push_back(condLine);
		}
		if (!condLines.empty())
		{
			lines.push_back(_T("[폐색 출력 조건]"));
			lines.insert(lines.end(), condLines.begin(), condLines.end());
		}

		// 폐색 데이터 간 여백 추가
		lines.push_back(_T(""));
	}

	return lines;
}
void DiffCompareFrame::SetAllFaultInfo()
{
	auto& dc = DataComparison::GetInstance();

	std::vector<CString> leftLines;
	std::vector<CString> rightLines;

	const bool hasOrigin = dc.HasOriginal();
	const bool hasDiff = dc.HasDiff();
	bool hasLeftData = true;
	bool hasRightData = true;
	// 1. 둘 다 데이터가 없으면 중단
	if (!hasOrigin && !hasDiff)
	{
		return;
	}

	// 텍스트 배열을 순서대로 이어붙여주는 람다 헬퍼
	auto AppendLines = [](std::vector<CString>& dest, const std::vector<CString>& src) {
		if (!src.empty()) {
			dest.insert(dest.end(), src.begin(), src.end());
		}
		};

	// 2. 원본(Left) 데이터 일괄 수집 및 병합
	if (hasOrigin)
	{
		AppendLines(leftLines, ConvertLevelCrossInfoText(dc.GetOriginalLevelCross(), true));
		AppendLines(leftLines, ConvertLCInfoText(dc.GetOriginalLC(), true));
		AppendLines(leftLines, ConvertDeadSectionInfoText(dc.GetOriginalDeadSection(), true));
		AppendLines(leftLines, ConvertFallLockInfoText(dc.GetOriginalFallLock(), true));
		AppendLines(leftLines, ConvertSTLInfoText(dc.GetOriginalSTL(), true));
		AppendLines(leftLines, ConvertFaultInfoText(dc.GetOriginalFault(), true));
		AppendLines(leftLines, ConvertHeatInfoText(dc.GetOriginalHeat(), true));
		if (leftLines.empty())
		{
			hasLeftData = false;
			leftLines.push_back(_T("원본 데이터가 없습니다."));
		}
	}
	else
	{
		leftLines.push_back(_T("선택한 파일이 없습니다."));
	}

	// 3. 비교군(Right) 데이터 일괄 수집 및 병합
	if (hasDiff)
	{
		AppendLines(rightLines, ConvertLevelCrossInfoText(dc.GetDiffLevelCross(), false));
		AppendLines(rightLines, ConvertLCInfoText(dc.GetDiffLC(), false));
		AppendLines(rightLines, ConvertDeadSectionInfoText(dc.GetDiffDeadSection(), false));
		AppendLines(rightLines, ConvertFallLockInfoText(dc.GetDiffFallLock(), false));
		AppendLines(rightLines, ConvertSTLInfoText(dc.GetDiffSTL(), false));
		AppendLines(rightLines, ConvertFaultInfoText(dc.GetDiffFault(), false));
		AppendLines(rightLines, ConvertHeatInfoText(dc.GetDiffHeat(), false));
		if (rightLines.empty())
		{
			hasRightData = false;
			rightLines.push_back(_T("비교 데이터가 없습니다."));
		}
	}

	else
	{
		rightLines.push_back(_T("비교할 파일이 없습니다."));
	}


	ApplyCompareOrSingle(hasLeftData, hasRightData, leftLines, rightLines);
}
std::vector<CString> DiffCompareFrame::ConvertLevelCrossInfoText(const std::span<LevelCrossInfoType>& list, bool bIsOrigin)
{
	std::vector<CString> lines;
	CString temp;
	for (const auto& item : list)
	{
		if (item.Name[0] == 0 || item.Name[0] == 0xFF) continue;
		lines.push_back(_T("========================================"));
		CString name = GetSafeString(item.Name, 20);
		temp.Format(_T("[건널목 정보 명칭] %s"), (LPCTSTR)name);
		lines.push_back(temp);
		lines.push_back(_T("----------------------------------------"));
		lines.push_back(_T(""));
	}
	return lines;
}

// 2. 절연구간 정보 텍스트 변환
// [구조체 변경] type/Index/Input(IO_Position) 필드는 삭제되고, Unit1/Unit2/Unit_Act 하위 Name 으로 구조 변경됨
std::vector<CString> DiffCompareFrame::ConvertDeadSectionInfoText(const std::span<DeadSectionInfoType>& list, bool bIsOrigin)
{
	std::vector<CString> lines;
	CString temp;
	for (const auto& item : list)
	{
		if (item.Name[0] == 0 || item.Name[0] == 0xFF) continue;
		lines.push_back(_T("========================================"));
		CString name = GetSafeString(item.Name, 20);
		temp.Format(_T("[절연구간 명칭] %s"), (LPCTSTR)name);
		lines.push_back(temp);
		lines.push_back(_T("----------------------------------------"));

		CString unit1Name = GetSafeString(item.Unit1.Name, 10);
		if (!unit1Name.IsEmpty())
		{
			temp.Format(_T("  - 1계 명칭 (Unit1): %s"), (LPCTSTR)unit1Name);
			lines.push_back(temp);
		}
		CString unit2Name = GetSafeString(item.Unit2.Name, 10);
		if (!unit2Name.IsEmpty())
		{
			temp.Format(_T("  - 2계 명칭 (Unit2): %s"), (LPCTSTR)unit2Name);
			lines.push_back(temp);
		}
		CString unitActName = GetSafeString(item.Unit_Act.Name, 10);
		if (!unitActName.IsEmpty())
		{
			temp.Format(_T("  - 운용 ( 여자:1계 주계 / 낙하:2계 주계): %s"), (LPCTSTR)unitActName);
			lines.push_back(temp);
		}
		lines.push_back(_T(""));
	}
	return lines;
}

// 3. 지장물 정보 텍스트 변환

// Release 는 구조체 주석상 "규격변경 이후 삭제(사용하지 않음)" 이므로 표시하지 않음
std::vector<CString> DiffCompareFrame::ConvertFallLockInfoText(const std::span<FallLockInfoType>& list, bool bIsOrigin)
{
	std::vector<CString> lines;
	CString temp;
	for (const auto& item : list)
	{
		if (item.Name[0] == 0 || item.Name[0] == 0xFF) continue;
		lines.push_back(_T("========================================"));
		CString name = GetSafeString(item.Name, 20);
		temp.Format(_T("[지장물 명칭] %s"), (LPCTSTR)name);
		lines.push_back(temp);
		lines.push_back(_T("----------------------------------------"));

		CString fallLockName = GetSafeString(item.FallLock.Name, 20);
		if (!fallLockName.IsEmpty())
		{
			temp.Format(_T("  - 낙석 명칭 (FallLock): %s"), (LPCTSTR)fallLockName);
			lines.push_back(temp);
		}
		CString procName1 = GetSafeString(item.Proc.Name1, 15);
		CString procName2 = GetSafeString(item.Proc.Name2, 15);
		if (!procName1.IsEmpty() || !procName2.IsEmpty())
		{
			temp.Format(_T("  - 보호 명칭 (Proc): %s / %s"), (LPCTSTR)procName1, (LPCTSTR)procName2);
			lines.push_back(temp);
		}
		lines.push_back(_T(""));
	}
	return lines;
}

// 4. 출발반응등 정보 텍스트 변환

std::vector<CString> DiffCompareFrame::ConvertSTLInfoText(const std::span<STLInfoType>& list, bool bIsOrigin)
{
	std::vector<CString> lines;
	CString temp;
	for (const auto& item : list)
	{
		if (item.Name[0] == 0 || item.Name[0] == 0xFF) continue;
		lines.push_back(_T("========================================"));
		CString name = GetSafeString(item.Name, 20);
		temp.Format(_T("[출발반응등 명칭] %s"), (LPCTSTR)name);
		lines.push_back(temp);
		lines.push_back(_T("----------------------------------------"));
		lines.push_back(_T(""));
	}
	return lines;
}

// 5. 기타 고장 정보 텍스트 변환

std::vector<CString> DiffCompareFrame::ConvertFaultInfoText(const std::span<FaultInfoType>& list, bool bIsOrigin)
{
	std::vector<CString> lines;
	CString temp;
	for (const auto& item : list)
	{
		if (item.Name[0] == 0 || item.Name[0] == 0xFF) continue;
		lines.push_back(_T("========================================"));
		CString name = GetSafeString(item.Name, 20);
		temp.Format(_T("[기타 고장 정보 명칭] %s"), (LPCTSTR)name);
		lines.push_back(temp);
		lines.push_back(_T("----------------------------------------"));

		CString typeStr = (item.Type == 1) ? _T("출발대용표시등") : _T("기타");
		CString hideStr = (item.Hide == 1) ? _T("화면 숨김") : _T("화면 표시");
		temp.Format(_T("  - 타입: (%s), 화면 숨김(Hide): %s"), (LPCTSTR)typeStr, item.Hide);
		lines.push_back(temp);
		lines.push_back(_T(""));
	}
	return lines;
}

std::vector<CString> DiffCompareFrame::ConvertSOInfoText(const std::span<SlowOrderInfoType>& list, bool)
{
	return std::vector<CString>();
}

std::vector<CString> DiffCompareFrame::ConvertAttrInfoText(const std::span<AttractionInfoType>& list, bool)
{
	return std::vector<CString>();
}

// 6. 히터 정보 텍스트 변환
// [추가] InputHeat/InputFail/AlmData[i].InputAlarm (구조체엔 있으나 기존 코드에 없던 필드)
std::vector<CString> DiffCompareFrame::ConvertHeatInfoText(const std::span<HeatInfoType>& list, bool bIsOrigin)
{
	std::vector<CString> lines;
	CString temp;
	for (const auto& item : list)
	{
		if (item.szHeatName[0] == 0 || item.szHeatName[0] == 0xFF) continue;
		lines.push_back(_T("========================================"));
		CString heatName = GetSafeString(item.szHeatName, 10);
		temp.Format(_T("[Heater 명칭] %s"), (LPCTSTR)heatName);
		lines.push_back(temp);
		lines.push_back(_T("----------------------------------------"));

		// [추가] 동작상태 Input (여자일 때 동작중)
		temp.Format(_T("  - InputHeat (동작상태, 여자시 동작중): %s"), (LPCTSTR)GetSafeIOPosition(item.InputHeat));
		lines.push_back(temp);
		// [추가] Fail Input (여자일 때 장애, 규격개정 이후에 대해서만 입력 있음)
		temp.Format(_T("  - InputFail (여자시 장애): %s"), (LPCTSTR)GetSafeIOPosition(item.InputFail));
		lines.push_back(temp);

		lines.push_back(_T("  [장애 알람 메시지 목록]"));
		for (int i = 0; i < _countof(item.AlmData); ++i)
		{
			CString almMsg = GetSafeString(item.AlmData[i].szAlmMsg, 20);
			if (!almMsg.IsEmpty() && almMsg != _T("-"))
			{
				// [추가] 해당 알람 메시지의 Input (InputFail 여자 && InputAlarm 낙하 일 때 표시)
				temp.Format(_T("    - [%d] %s (InputAlarm: %s)"), i, (LPCTSTR)almMsg, (LPCTSTR)GetSafeIOPosition(item.AlmData[i].InputAlarm));
				lines.push_back(temp);
			}
		}
		//lines.push_back(_T(""));
	}
	return lines;
}


void DiffCompareFrame::SetInterLock()
{
	auto& dc = DataComparison::GetInstance();

	std::vector<CString> leftLines;
	std::vector<CString> rightLines;

	const bool hasOrigin = dc.HasOriginal();
	const bool hasDiff = dc.HasDiff();
	bool hasLeftData = true;
	bool hasRightData = true;
	// 1. 둘 다 데이터가 없으면 중단
	if (!hasOrigin && !hasDiff)
	{
		return;
	}

	// 2. 원본(Left) 데이터 수집 및 예외 처리
	if (hasOrigin)
	{
		auto InterLockInfo = dc.GetOriginalInterLock();
		auto RouteInfo = dc.GetOriginalRoute();
		leftLines = ConvertInterlockInfoText(InterLockInfo, RouteInfo, true);
		if (leftLines.empty())
		{
			hasLeftData = false;
			leftLines.push_back(_T("원본 데이터가 없습니다."));
		}
	}
	else
	{
		leftLines.push_back(_T("선택한 파일이 없습니다."));
	}

	// 3. 비교군(Right) 데이터 수집 및 예외 처리
	if (hasDiff)
	{
		auto DiffCPTInfo = dc.GetDiffInterLock();
		auto DiffRouteInfo = dc.GetDiffRoute();
		rightLines = ConvertInterlockInfoText(DiffCPTInfo, DiffRouteInfo, false);
		if (rightLines.empty())
		{
			hasRightData = false;
			rightLines.push_back(_T("비교 데이터가 없습니다."));
		}
	}
	else
	{
		rightLines.push_back(_T("비교할 파일이 없습니다."));
	}

	// 4. 수집된 라인을 전달하여 [Diff 정렬 -> 화면 출력 -> 색상 하이라이트] 일괄 처리
	ApplyCompareOrSingle(hasLeftData, hasRightData, leftLines, rightLines);
}
std::vector<CString> DiffCompareFrame::ConvertInterlockInfoText(const std::span<InterLockInfoType>& interLockList, const std::span<RouteInfoType>& routeList, bool bIsOrigin)
{
	std::vector<CString> lines;
	CString temp;

	// 원본인지 비교군인지에 따라 CommonUtil 함수를 선택하는 람다
	auto GetDBName = [&](BYTE nNum, GetDBNameByNum num) -> CString {
		if (bIsOrigin) {
			return CommonUtil::GetOriginNameByNumber(nNum, num);
		}
		else {
			return CommonUtil::GetDiffNameByNumber(nNum, num);
		}
		};

	for (size_t nIdx = 0; nIdx < interLockList.size(); ++nIdx)
	{
		const auto& item = interLockList[nIdx];
		if (item.Name[0] == 0 || (unsigned char)item.Name[0] == 0xFF) continue;

		UINT nRteNo = static_cast<UINT>(nIdx + 1);
		CString routeName = GetSafeString(item.Name, 20);
		routeName.Trim();

		// 1. 블록 헤더 (LCS 매칭의 고유 키)
		lines.push_back(_T("========================================"));
		temp.Format(_T("[연동진로 명칭] %s"), (LPCTSTR)routeName);
		lines.push_back(temp);
		lines.push_back(_T("------------------------------------------------"));

		// 2. 진로 기본 정보 (신호기, 출발, 도착, 유도등)
		CString strSignalName = GetDBName(static_cast<BYTE>(item.RouteInfo.SignalNo), GetDBNameByNum::SignalIdx);
		CString strDepTrack = GetDBName(static_cast<BYTE>(item.RouteInfo.DepartureT), GetDBNameByNum::TrackIdx);
		CString strArrTrack = GetDBName(static_cast<BYTE>(item.RouteInfo.ArrivalT), GetDBNameByNum::TrackIdx);

		temp.Format(_T("  - 진로 기본: 신호기:%s, 출발:%s, 도착:%s"), (LPCTSTR)strSignalName, (LPCTSTR)strDepTrack, (LPCTSTR)strArrTrack);
		if (IsBitSet(item.RouteInfo.IsCallOnSig, 0)) {
			temp += _T(" [(무)유도등 ON]");
		}
		lines.push_back(temp);

		// 3. 대항진로 정보 (routeList 활용)
		int nRouteIdx = static_cast<int>(nRteNo) - 1;
		if (nRouteIdx >= 0 && nRouteIdx < static_cast<int>(routeList.size()))
		{
			const auto& rteItem = routeList[nRouteIdx];
			CString strOppList = _T("");
			bool bFirstOpp = true;

			for (int nIdxInc = 0; nIdxInc < MAX_INHIBIT_ROUTE; nIdxInc++)
			{
				WORD inhibitRteNo = rteItem.InhibitRteNo[nIdxInc];
				if (0 == inhibitRteNo) break;

				if (inhibitRteNo < interLockList.size()) {
					if (!bFirstOpp) strOppList += _T(", ");
					CString szEquipTmp = GetSafeString(interLockList[inhibitRteNo].Name, 20);
					strOppList += szEquipTmp;
					bFirstOpp = false;
				}
			}
			if (!strOppList.IsEmpty()) {
				temp.Format(_T("  - 대항진로: %s"), (LPCTSTR)strOppList);
				lines.push_back(temp);
			}

			// 4. 진로 구분 및 플래그
			// [버그 수정] Kind/RouteFlag 는 .Value 로 감싸인 중첩 구조체가 아니라 그 자체가 필드 구조체임
			const auto& kindVal = rteItem.Kind;
			const auto& flagVal = rteItem.RouteFlag;
			CString strRouteKind = _T("  - 진로구분:");
			if (IsBitSet(kindVal.Direction, 0)) strRouteKind += _T(" [Left방향]");
			else strRouteKind += _T(" [Right방향]");
			if (IsBitSet(kindVal.Special, 0)) strRouteKind += _T(" [유효장]");
			if (IsBitSet(kindVal.Special, 1)) strRouteKind += _T(" [단조건]");
			if (IsBitSet(kindVal.IsTTB, 0))    strRouteKind += _T(" [TTB]");
			if (IsBitSet(kindVal.IsBlock, 0))  strRouteKind += _T(" [구내폐색]");
			if (IsBitSet(kindVal.Arrive, 0))   strRouteKind += _T(" [장내]");
			if (IsBitSet(kindVal.Depart, 0))   strRouteKind += _T(" [출발]");
			if (IsBitSet(kindVal.Shunt, 0))    strRouteKind += _T(" [입환]");
			if (IsBitSet(kindVal.IsLC, 0))     strRouteKind += _T(" [건널목포함]");
			// [추가] 구조체엔 있으나 기존 코드에 없던 Kind 필드
			if (IsBitSet(kindVal.JeonlaRevBlk, 0)) strRouteKind += _T(" [전라선역방향폐색]");
			if (IsBitSet(kindVal.ShuntSig, 0)) strRouteKind += _T(" [입환표지/신호기공용]");
			if (IsBitSet(flagVal.UdoSig, 0))   strRouteKind += _T(" [(무)유도진로]");
			// [추가] 구조체엔 있으나 기존 코드에 없던 RouteFlag 필드
			if (IsBitSet(flagVal.MainSig, 0))  strRouteKind += _T(" [주신호연결]");
			if (IsBitSet(flagVal.IOCheck, 0))  strRouteKind += _T(" [선별등출력있음(HU)]");
			if (IsBitSet(flagVal.IOCheck, 1))  strRouteKind += _T(" [선별등입력있음(ULMR)]");
			lines.push_back(strRouteKind);

			// 5. 현시 정보 (Max / Min)
			LPCTSTR pszMaxAspect = _T("-------");
			switch (rteItem.MaxSignal & 0x0F) {
			case SIGNAL_PROCEED: pszMaxAspect = _T("진행-G"); break;
			case SIGNAL_CAUTION: pszMaxAspect = _T("주의-Y"); break;
			case SIGNAL_SLOW:    pszMaxAspect = _T("감속-YG"); break;
			case SIGNAL_BOUND:   pszMaxAspect = _T("경계-YY"); break;
			case SIGNAL_STOP:    pszMaxAspect = _T("정지"); break;
			}
			LPCTSTR pszMinAspect = _T("-------");
			switch (rteItem.MinSignal & 0x0F) {
			case SIGNAL_PROCEED: pszMinAspect = _T("진행-G"); break;
			case SIGNAL_CAUTION: pszMinAspect = _T("주의-Y"); break;
			case SIGNAL_SLOW:    pszMinAspect = _T("감속-YG"); break;
			case SIGNAL_BOUND:   pszMinAspect = _T("경계-YY"); break;
			case SIGNAL_STOP:    pszMinAspect = _T("정지"); break;
			}
			temp.Format(_T("  - 현시: Max:%02x(%s), Min:%02x(%s)"), (rteItem.MaxSignal & 0x0F), pszMaxAspect, (rteItem.MinSignal & 0x0F), pszMinAspect);
			lines.push_back(temp);

			// 전방 / 후방 신호기
			if (rteItem.FrontSignalNo > 0 || rteItem.RearSignalNo > 0) {
				CString strFront = rteItem.FrontSignalNo > 0 ? GetDBName(rteItem.FrontSignalNo, SignalIdx) : (CString)_T("-");
				CString strRear = rteItem.RearSignalNo > 0 ? GetDBName(rteItem.RearSignalNo, SignalIdx) : (CString)_T("-");
				temp.Format(_T("  - 전/후방 신호기: 전방[%s], 후방[%s]"), (LPCTSTR)strFront, (LPCTSTR)strRear);
				lines.push_back(temp);
			}

			// [추가] 일괄(총괄)제어 신호기 (TotRteSigNo) - 구조체엔 있으나 기존 코드에 없던 필드
			CString strTotRteSig = _T("");
			for (int i = 0; i < MAX_TOTRTE_SIG; i++) {
				if (rteItem.TotRteSigNo[i] == 0) continue;
				CString sub; sub.Format(_T(" %s"), (LPCTSTR)GetDBName(rteItem.TotRteSigNo[i], SignalIdx));
				strTotRteSig += sub;
			}
			if (!strTotRteSig.IsEmpty()) {
				temp.Format(_T("  - 일괄(총괄)제어 신호기(TotRteSigNo):%s"), (LPCTSTR)strTotRteSig);
				lines.push_back(temp);
			}

			// [추가] 전방연계진로, (무)유도 진로번호, 구내폐색 관련 진로 - 구조체엔 있으나 기존 코드에 없던 필드
			if (rteItem.FrontLinkRteNo != 0) {
				temp.Format(_T("  - 전방연계진로(FrontLinkRteNo): %d"), rteItem.FrontLinkRteNo);
				lines.push_back(temp);
			}
			if (rteItem.UdoRteNo != 0) {
				temp.Format(_T("  - (무)유도 진로번호(UdoRteNo): %d"), rteItem.UdoRteNo);
				lines.push_back(temp);
			}
			CString strBlockSigRte = _T("");
			for (int i = 0; i < MAX_HOME_BLOCK_ROUTE; i++) {
				if (rteItem.BlockSigRteNo[i] == 0) continue;
				CString sub; sub.Format(_T(" %d"), rteItem.BlockSigRteNo[i]);
				strBlockSigRte += sub;
			}
			if (!strBlockSigRte.IsEmpty()) {
				temp.Format(_T("  - 구내폐색 관련 진로(BlockSigRteNo):%s"), (LPCTSTR)strBlockSigRte);
				lines.push_back(temp);
			}

			// [추가] 장내/출발폐색 번호, 폐색출력 없음 플래그 - 구조체엔 있으나 기존 코드에 없던 필드
			if (rteItem.ArrBlockNo != 0 || rteItem.DepBlockNo != 0) {
				temp.Format(_T("  - 폐색번호: 장내(ArrBlockNo):%d, 출발(DepBlockNo):%d"), rteItem.ArrBlockNo, rteItem.DepBlockNo);
				if (IsBitSet(rteItem.NotOutBlock, 0)) temp += _T(" [진로제어시 폐색출력 없음]");
				lines.push_back(temp);
			}

			// [추가] 자동 진로 제어(AutoRteCtrl) - 구조체엔 있으나 기존 코드에 없던 필드
			if (rteItem.AutoRteCtrl.AutoMode != 0 || rteItem.AutoRteCtrl.PreRteNo != 0) {
				temp.Format(_T("  - 자동진로제어(AutoRteCtrl): Mode:%d, 이전진로:%d, 지연시간:%d초"),
					rteItem.AutoRteCtrl.AutoMode, rteItem.AutoRteCtrl.PreRteNo, rteItem.AutoRteCtrl.CtrlTime);
				lines.push_back(temp);
			}
		}

		// 6. 카운트 / 수량 정보
		temp.Format(_T("  - 카운트: 선로전환기:%d, 단조건:%d, 신호기:%d, 궤도:%d, 진로쇄정:%d, 접근쇄정:%d"),
			item.Count.NoOfSwhLock, item.Count.NoOfExcept, item.Count.NoOfSigLock,
			item.Count.NoOfCtlLock, item.Count.NoOfRteLock, item.Count.NoOfAppLock);
		lines.push_back(temp);

		// 7. 쇄정 선로전환기 정보
		if (item.Count.NoOfSwhLock > 0) {
			CString strSwhLock = _T("");
			for (int i = 0; i < item.Count.NoOfSwhLock; i++) {
				if (item.SwitchLock[i].SwitchNo == 0) continue;
				CString strSwhName = GetDBName(static_cast<BYTE>(item.SwitchLock[i].SwitchNo), SwitchIdx);
				CString sub;
				sub.Format(_T(" %s[%d]"), (LPCTSTR)strSwhName, item.SwitchLock[i].Direction);
				// [추가] LockKind/TimeValue - 구조체엔 있으나 기존 코드에 없던 필드
				if (IsBitSet(item.SwitchLock[i].LockKind, 0)) sub += _T("(진로쇄정)");
				if (IsBitSet(item.SwitchLock[i].LockKind, 1)) sub += _T("(Overlap쇄정)");
				strSwhLock += sub;
			}
			if (!strSwhLock.IsEmpty()) {
				temp.Format(_T("  - 쇄정 선로전환기:%s"), (LPCTSTR)strSwhLock);
				lines.push_back(temp);
			}
		}

		// [추가] 진로(구분)쇄정 궤도(RouteLock) - 구조체엔 있으나 기존 코드에 없던 필드
		CString strRouteLock = _T("");
		for (int i = 0; i < NO_OF_LOCK_TRK; i++) {
			if (item.RouteLock[i].TrackNo == 0) break;
			CString sub; sub.Format(_T(" %s"), (LPCTSTR)GetDBName(item.RouteLock[i].TrackNo, TrackIdx));
			if (IsBitSet(item.RouteLock[i].ReleaseTrk, 0)) sub += _T("[도착시해정]");
			strRouteLock += sub;
		}
		if (!strRouteLock.IsEmpty()) {
			temp.Format(_T("  - 진로쇄정 궤도(RouteLock):%s"), (LPCTSTR)strRouteLock);
			lines.push_back(temp);
		}

		// [추가] 조건에 따른 진로의 신호기 정지 설비(EquipLock) - 구조체엔 있으나 기존 코드에 없던 필드
		CString strEquipLock = _T("");
		for (int i = 0; i < NO_OF_LOCK_EQUIP; i++) {
			if (item.EquipLock[i].Kind == 0) continue;
			CString sub; sub.Format(_T(" [%c/%d/cond%d]"), item.EquipLock[i].Kind, item.EquipLock[i].TblNo, item.EquipLock[i].stateCond);
			strEquipLock += sub;
		}
		if (!strEquipLock.IsEmpty()) {
			temp.Format(_T("  - 설비쇄정(EquipLock):%s"), (LPCTSTR)strEquipLock);
			lines.push_back(temp);
		}

		// [추가] 단조건(Exception) - 구조체엔 있으나 기존 코드에 없던 필드
		CString strException = _T("");
		for (int i = 0; i < NO_OF_EXCEPTION; i++) {
			if (item.Exception[i].Kind == 0) continue;
			CString sub; sub.Format(_T(" [%c/%d/전철기%d(%d)]"), item.Exception[i].Kind, item.Exception[i].TblIdx, item.Exception[i].SwitchNo, item.Exception[i].SwitchDir);
			strException += sub;
		}
		if (!strException.IsEmpty()) {
			temp.Format(_T("  - 단조건(Exception):%s"), (LPCTSTR)strException);
			lines.push_back(temp);
		}

		// 8. 진로쇄정 궤도 정보 (ControlTrack: 신호제어 및 철사쇄정 궤도)
		CString strRteTrk = _T("");
		for (int i = 0; i < NO_OF_LOCK_TRK; i++) {
			BYTE trkNo = item.ControlTrack[i];
			if (trkNo == 0) break;
			CString strTrkName = GetDBName(trkNo, TrackIdx);
			CString sub; sub.Format(_T(" %s"), (LPCTSTR)strTrkName);
			strRteTrk += sub;
		}
		if (!strRteTrk.IsEmpty()) {
			temp.Format(_T("  - 쇄정 궤도(ControlTrack):%s"), (LPCTSTR)strRteTrk);
			lines.push_back(temp);
		}

		// [추가] 쇄정 신호기(SignalLock), 폐로쇄정 궤도(LeverLockTrack), TTB상태 단락궤도(TtbDelTrack)
		CString strSigLock = _T("");
		for (int i = 0; i < MAX_SIGNAL_LOCK; i++) {
			if (item.SignalLock[i] == 0) continue;
			CString sub; sub.Format(_T(" %s"), (LPCTSTR)GetDBName(item.SignalLock[i], SignalIdx));
			strSigLock += sub;
		}
		if (!strSigLock.IsEmpty()) {
			temp.Format(_T("  - 쇄정 신호기(SignalLock):%s"), (LPCTSTR)strSigLock);
			lines.push_back(temp);
		}
		CString strLeverLock = _T("");
		for (int i = 0; i < MAX_LEVERLOCK_TRACK; i++) {
			if (item.LeverLockTrack[i] == 0) continue;
			CString sub; sub.Format(_T(" %s"), (LPCTSTR)GetDBName(item.LeverLockTrack[i], TrackIdx));
			strLeverLock += sub;
		}
		if (!strLeverLock.IsEmpty()) {
			temp.Format(_T("  - 폐로쇄정 궤도(LeverLockTrack):%s"), (LPCTSTR)strLeverLock);
			lines.push_back(temp);
		}
		CString strTtbDel = _T("");
		for (int i = 0; i < MAX_TTB_DEL_TRACK; i++) {
			if (item.TtbDelTrack[i] == 0) continue;
			CString sub; sub.Format(_T(" %s"), (LPCTSTR)GetDBName(item.TtbDelTrack[i], TrackIdx));
			strTtbDel += sub;
		}
		if (!strTtbDel.IsEmpty()) {
			temp.Format(_T("  - TTB상태 단락유지 궤도(TtbDelTrack):%s"), (LPCTSTR)strTtbDel);
			lines.push_back(temp);
		}

		// [추가] 접근 및 보류쇄정(ApproachLock) - 구조체엔 있으나 기존 코드에 없던 필드
		CString strAppTrk = _T("");
		for (int i = 0; i < MAX_APP_LOCK; i++) {
			if (item.ApproachLock.ApproachTrack[i].AppTrack == 0) continue;
			CString sub; sub.Format(_T(" %s"), (LPCTSTR)GetDBName(item.ApproachLock.ApproachTrack[i].AppTrack, TrackIdx));
			strAppTrk += sub;
		}
		if (!strAppTrk.IsEmpty() || item.ApproachLock.ReleaseTime != 0) {
			temp.Format(_T("  - 접근/보류쇄정 궤도(ApproachLock, 해정시간:%d초):%s"), item.ApproachLock.ReleaseTime, (LPCTSTR)strAppTrk);
			lines.push_back(temp);
		}

		// [추가] 유효장(UhoInfo) - 구조체엔 있으나 기존 코드에 없던 필드
		if (item.UhoInfo.FirstTrack != 0 || item.UhoInfo.LastTrack != 0) {
			CString strUhoTrk = _T("");
			for (int i = 0; i < MAX_CNT_EF_TRACK; i++) {
				if (item.UhoInfo.TrackNo[i] == 0) continue;
				CString sub; sub.Format(_T(" %s"), (LPCTSTR)GetDBName(item.UhoInfo.TrackNo[i], TrackIdx));
				strUhoTrk += sub;
			}
			temp.Format(_T("  - 유효장(UhoInfo): %s ~ %s, 궤도:%s, 해정시간:%d초"),
				(LPCTSTR)GetDBName(item.UhoInfo.FirstTrack, TrackIdx), (LPCTSTR)GetDBName(item.UhoInfo.LastTrack, TrackIdx),
				(LPCTSTR)strUhoTrk, item.UhoInfo.LockTime);
			if (IsBitSet(item.UhoInfo.UhoRelease, 0)) temp += _T(" [착점도착시 해정]");
			lines.push_back(temp);
		}

		// [추가] 선로전환기 전환 확인(SwitchCheck) - 구조체엔 있으나 기존 코드에 없던 필드
		CString strSwCheck = _T("");
		for (int i = 0; i < MAX_CHECK_SWITCH; i++) {
			if (item.SwitchCheck[i].SwitchNo == 0) continue;
			CString sub; sub.Format(_T(" %s(궤도:%s,방향:%d)"), (LPCTSTR)GetDBName(item.SwitchCheck[i].SwitchNo, SwitchIdx),
				(LPCTSTR)GetDBName(item.SwitchCheck[i].TrackNo, TrackIdx), item.SwitchCheck[i].SwitchDir);
			strSwCheck += sub;
		}
		if (!strSwCheck.IsEmpty()) {
			temp.Format(_T("  - 전환확인(SwitchCheck):%s"), (LPCTSTR)strSwCheck);
			lines.push_back(temp);
		}

		// [추가] 타역설비 조건부 신호정지(SpcState) - 구조체엔 있으나 기존 코드에 없던 필드
		for (int i = 0; i < MAX_CNT_SPC_STATE; i++) {
			const auto& spc = item.SpcState[i];
			if (spc.SpcTrack == 0 && spc.SpcSignal == 0 && spc.SpcSwitch == 0 && spc.SpcFaultIDX == 0) continue;
			temp.Format(_T("  - 타역조건(SpcState[%d]): 궤도:%d"), i, spc.SpcTrack);
			if (spc.SpcSignal != 0) {
				temp.AppendFormat(_T(", 신호기:%d(%s시정지)"), spc.SpcSignal, IsBitSet(spc.SignalStat, 0) ? _T("진행") : _T("정지"));
			}
			if (spc.SpcSwitch != 0) {
				CString swSt = _T("");
				if (IsBitSet(spc.SwitchStat, 0)) swSt += _T("정위시정지");
				if (IsBitSet(spc.SwitchStat, 1)) swSt += _T("반위시정지");
				temp.AppendFormat(_T(", 전철기:%d(%s)"), spc.SpcSwitch, (LPCTSTR)swSt);
			}
			if (spc.SpcFaultIDX != 0) {
				temp.AppendFormat(_T(", 기타고장:%d(%s시정지)"), spc.SpcFaultIDX, IsBitSet(spc.SpcFaultStat, 0) ? _T("여자") : _T("낙하"));
			}
			lines.push_back(temp);
		}

		// 항목 간 여백 추가 (다른 컨버터블록들과 동일한 규격)
		lines.push_back(_T(""));
	}

	return lines;
}
void DiffCompareFrame::SetCPTInfo()
{
	auto& dc = DataComparison::GetInstance();

	std::vector<CString> leftLines;
	std::vector<CString> rightLines;

	const bool hasOrigin = dc.HasOriginal();
	const bool hasDiff = dc.HasDiff();
	bool hasLeftData = true;
	bool hasRightData = true;
	// 1. 둘 다 데이터가 없으면 중단
	if (!hasOrigin && !hasDiff)
	{
		return;
	}

	// 2. 원본(Left) 데이터 수집 및 예외 처리
	if (hasOrigin)
	{
		auto CPTInfo = dc.GetOriginalCPT();
		leftLines = ConvertCPTInfoText(CPTInfo, true);
		if (leftLines.empty())
		{
			hasLeftData = false;
			leftLines.push_back(_T("원본 데이터가 없습니다."));
		}
	}
	else
	{
		leftLines.push_back(_T("선택한 파일이 없습니다."));
	}

	// 3. 비교군(Right) 데이터 수집 및 예외 처리
	if (hasDiff)
	{
		auto DiffCPTInfo = dc.GetDiffCPT();
		rightLines = ConvertCPTInfoText(DiffCPTInfo, false);
		if (rightLines.empty())
		{
			hasRightData = false;
			rightLines.push_back(_T("비교 데이터가 없습니다."));
		}
	}
	else
	{
		rightLines.push_back(_T("비교할 파일이 없습니다."));
	}

	// 4. 수집된 라인을 전달하여 [Diff 정렬 -> 화면 출력 -> 색상 하이라이트] 일괄 처리
	ApplyCompareOrSingle(hasLeftData, hasRightData, leftLines, rightLines);
}

void DiffCompareFrame::SetDWELLInfo()
{
	auto& dc = DataComparison::GetInstance();

	std::vector<CString> leftLines;
	std::vector<CString> rightLines;

	const bool hasOrigin = dc.HasOriginal();
	const bool hasDiff = dc.HasDiff();
	bool hasLeftData = true;
	bool hasRightData = true;
	// 1. 둘 다 데이터가 없으면 중단
	if (!hasOrigin && !hasDiff)
	{
		return;
	}

	// 2. 원본(Left) 데이터 수집 및 예외 처리
	if (hasOrigin)
	{
		auto dwellInfo = dc.GetOriginalDWELL();
		leftLines = ConvertDwellInfoText(dwellInfo, true);
		if (leftLines.empty()) {
			leftLines.push_back(_T("원본 데이터가 없습니다."));
			hasLeftData = false;
		}
	}
	else
	{
		leftLines.push_back(_T("선택한 파일이 없습니다."));
	}

	// 3. 비교군(Right) 데이터 수집 및 예외 처리
	if (hasDiff)
	{
		auto diffDwellInfo = dc.GetDiffDWELL();
		rightLines = ConvertDwellInfoText(diffDwellInfo, false);
		if (rightLines.empty()) {
			rightLines.push_back(_T("비교 데이터가 없습니다."));
			hasRightData = false;
		}
	}
	else
	{
		rightLines.push_back(_T("비교할 파일이 없습니다."));
	}

	// 4. 수집된 라인을 전달하여 [Diff 정렬 -> 화면 출력 -> 색상 하이라이트] 일괄 처리
	ApplyCompareOrSingle(hasLeftData, hasRightData, leftLines, rightLines);
}

std::vector<CString> DiffCompareFrame::ConvertCPTInfoText(const std::span<CPTInfoType>& list, bool bIsOrigin)
{
	std::vector<CString> lines;
	CString temp;

	auto GetDBName = [&](BYTE nNum, GetDBNameByNum num) -> CString {
		if (bIsOrigin) return CommonUtil::GetOriginNameByNumber(nNum, num);
		else return CommonUtil::GetDiffNameByNumber(nNum, num);
		};

	for (const auto& item : list)
	{
		if (item.CptName[0] == 0 || (unsigned char)item.CptName[0] == 0xFF) continue;

		lines.push_back(_T("========================================"));
		// char 배열이므로 명시적으로 CString 변환
		CString cptName = CString(item.CptName, 20).Trim();
		temp.Format(_T("[CPT 정보] 명칭: %s (CPT 번호: %d)"), (LPCTSTR)cptName, item.CptNo);
		lines.push_back(temp);
		lines.push_back(_T("----------------------------------------"));

		// [버그 수정] CptKind 는 bit0(출발용)/bit1(장내용) 비트플래그임
		CString kindStr = _T("");
		if (IsBitSet(item.CptKind, 0)) kindStr = _T("출발용 (신호기 점멸 표시)");
		else if (IsBitSet(item.CptKind, 1)) kindStr = _T("장내용 (궤도 점유 표시, 비장애/비점멸)");

		temp.Format(_T("  - 타입: 0x%02X (%s)"), item.CptKind, (LPCTSTR)kindStr);
		lines.push_back(temp);

		temp.Format(_T("  - 입력 위치 (CptInput): %s"), (LPCTSTR)GetSafeIOPosition(item.CptInput));
		lines.push_back(temp);

		// [구조체 변경] CptInfo -> CptEquip (필드명 변경)
		lines.push_back(_T("  - CPT 연관 정보 목록 (신호기 / 궤도):"));
		bool bHasSub = false;
		for (int i = 0; i < MAX_CPT_EQUIP; ++i)
		{
			const auto& sub = item.CptEquip[i];
			if (sub.SignalNo != 0 || sub.TrackNo != 0)
			{
				CString sigName = (sub.SignalNo != 0) ? GetDBName(sub.SignalNo, SignalIdx) : CString(_T("-"));
				CString trkName = (sub.TrackNo != 0) ? GetDBName(sub.TrackNo, TrackIdx) : CString(_T("-"));
				temp.Format(_T("    * [%d] 신호기: %d (%s) / 궤도: %d (%s)"),
					i, sub.SignalNo, (LPCTSTR)sigName, sub.TrackNo, (LPCTSTR)trkName);
				lines.push_back(temp);
				bHasSub = true;
			}
		}
		if (!bHasSub) {
			lines.push_back(_T("    * 등록된 연관 정보 없음"));
		}

		lines.push_back(_T(""));
	}

	return lines;
}

std::vector<CString> DiffCompareFrame::ConvertDwellInfoText(const std::span<DwellInfoType>& list, bool bIsOrigin)
{
	std::vector<CString> lines;
	CString temp;

	auto GetDBName = [&](BYTE nNum, GetDBNameByNum num) -> CString {
		if (bIsOrigin) return CommonUtil::GetOriginNameByNumber(nNum, num);
		else return CommonUtil::GetDiffNameByNumber(nNum, num);
		};

	for (const auto& item : list)
	{
		if (item.SubStnName[0] == 0 || item.SubStnName[0] == 0xFF) continue;

		lines.push_back(_T("========================================"));
		CString subStnName = GetSafeString(item.SubStnName, 20);
		temp.Format(_T("[소속역(Dwell) 정보] 역 명칭: %s (역 번호: %d, 플랫폼 번호: %d)"),
			(LPCTSTR)subStnName, item.SubStnNo, item.PlatFormNo);
		lines.push_back(temp);
		lines.push_back(_T("------------------------------------------------"));

		// [버그 수정] SubStnInfo 는 .Value 로 감싸인 중첩 구조체가 아니라 그 자체가 필드 구조체임
		CString endStationStr = IsBitSet(item.SubStnInfo.EndStation, 0) ? _T("종착역 (Dwell Lamp 없음)") : _T("일반역");
		CString upKindStr = (item.SubStnInfo.UpKind == 0x01) ? _T("하선") : _T("상선");
		temp.Format(_T("  - 종착역 구분: %s, 선로 방향: %s"), (LPCTSTR)endStationStr, (LPCTSTR)upKindStr);
		lines.push_back(temp);

		// 플랫폼 궤도 인덱스 목록
		CString trkListStr;
		for (int i = 0; i < MAX_DWELL_TRACK_FLATFORM; ++i)
		{
			if (item.PlatFormTrk[i] != 0)
			{
				CString trkName = GetDBName(item.PlatFormTrk[i], TrackIdx);
				CString sub;
				sub.Format(_T("[%d: %s] "), item.PlatFormTrk[i], (LPCTSTR)trkName);
				trkListStr += sub;
			}
		}
		if (trkListStr.IsEmpty()) trkListStr = _T("-");
		temp.Format(_T("  - 플랫폼 궤도: %s"), (LPCTSTR)trkListStr);
		lines.push_back(temp);

		// 플랫폼 정차등 신호기 및 출발 신호기
		CString pfSigName = (item.PlatFormSig != 0) ? GetDBName(item.PlatFormSig, SignalIdx) : CString(_T("-"));
		CString depSigName = (item.DepSignal != 0) ? GetDBName(item.DepSignal, SignalIdx) : CString(_T("-"));
		temp.Format(_T("  - 정차등 현시체크 신호기: %d (%s)"), item.PlatFormSig, (LPCTSTR)pfSigName);
		lines.push_back(temp);
		temp.Format(_T("  - 출발 신호기: %d (%s)"), item.DepSignal, (LPCTSTR)depSigName);
		lines.push_back(temp);
		temp.Format(_T("  - 열차번호 창 인덱스: %d"), item.TrainInfoNo);
		lines.push_back(temp);

		// 비상정지 설정 궤도 목록
		CString emgListStr;
		for (int i = 0; i < MAX_DWELL_TRACK_EMG; ++i)
		{
			if (item.EmgTrack[i] != 0)
			{
				CString trkName = GetDBName(item.EmgTrack[i], TrackIdx);
				CString sub;
				sub.Format(_T("[%s] "), (LPCTSTR)trkName);
				if (!emgListStr.IsEmpty())
				{
					emgListStr += _T(", ");
				}
				emgListStr += sub;
			}
		}
		if (emgListStr.IsEmpty()) emgListStr = _T("-");

		temp.Format(_T("  - 비상정지 설정 궤도: %s"), (LPCTSTR)emgListStr);

		lines.push_back(temp);

		lines.push_back(_T(""));
	}

	return lines;
}

// [구조체 변경] LC_INFO_TYPE -> LC_CTRL_INFO_TYPE, Name2 크기 4 -> 10
std::vector<CString> DiffCompareFrame::ConvertLCInfoText(const std::span<LC_CTRL_INFO_TYPE>& list, bool bIsOrigin)
{
	std::vector<CString> lines;
	CString temp;

	for (const auto& item : list)
	{
		// 유효하지 않은 데이터 건너뛰기
		if (item.Name1[0] == 0 || item.Name1[0] == 0xFF) continue;

		lines.push_back(_T("========================================"));

		CString name1 = GetSafeString(item.Name1, 10);
		CString name2 = GetSafeString(item.Name2, 10);

		// Name1과 Name2 조합 및 LC 번호 출력
		if (!name2.IsEmpty() && name2 != _T("-") && name2 != _T("\0"))
		{
			temp.Format(_T("[LC 정보] %s %s (LC 번호: %d)"), (LPCTSTR)name1, (LPCTSTR)name2, item.LcNo);
		}
		else
		{
			temp.Format(_T("[LC 정보] %s (LC 번호: %d)"), (LPCTSTR)name1, item.LcNo);
		}
		lines.push_back(temp);
		lines.push_back(_T("----------------------------------------"));

		// 입력 및 출력 I/O Position 정보 출력 (앞서 만든 GetSafeIOPosition 활용)
		temp.Format(_T("  - 입력 (LC_Inp): %s"), (LPCTSTR)GetSafeIOPosition(item.LC_Inp));
		lines.push_back(temp);

		temp.Format(_T("  - 출력 (LC_Out): %s"), (LPCTSTR)GetSafeIOPosition(item.LC_Out));
		lines.push_back(temp);

		// Status 비트 상태 분석
		lines.push_back(_T("  - 상태 설정 (Status):"));

		if (IsBitSet(item.Status, 0))
			lines.push_back(_T("    * Bit0=1 : 출력여자일 때 입력낙하"));
		else
			lines.push_back(_T("    * Bit0=0 : 출력여자일 때 입력여자"));

		if (IsBitSet(item.Status, 1))
			lines.push_back(_T("    * Bit1=1 : 부정입력 없음"));

		// [삭제] Bit2 는 구조체 주석상 "부정출력 없음 - 사용하지 않음" 이므로 표시하지 않음
		// [삭제] Bit7 은 현재 구조체 주석에 존재하지 않는 필드로, 기존 코드의 "평상시 OFF/ON" 해석은 근거가 없어 제거함

		if (IsBitSet(item.Status, 3))
			lines.push_back(_T("    * Bit3=1 : 동작불능 없음"));

		// 데이터 간 여백
		lines.push_back(_T(""));
	}

	return lines;
}
// [구조체 변경] InputDataType/IoCardAddressType 은 구조체에서 완전히 삭제됨.
// PDOCARDTABLE 이 신호기용(SIG_CARDTABLE)/선로전환기용(SWH_CARDTABLE)으로 분리되어 대체됨에 따라
// ConvertInputDataInfoText/ConvertIoCardAddressInfoText 함수를 삭제하고 신호기/선로전환기 카드 정보로 교체함
void DiffCompareFrame::SetAllIOCardInfo()
{
	auto& dc = DataComparison::GetInstance();

	std::vector<CString> leftLines;
	std::vector<CString> rightLines;
	bool hasLeftData = true;
	bool hasRightData = true;
	// 여러 데이터를 하나의 배열로 이어 붙여주는 람다 함수 (유용하게 재사용 가능)
	auto AppendLines = [](std::vector<CString>& dest, const std::vector<CString>& src) {
		if (!src.empty()) {
			dest.insert(dest.end(), src.begin(), src.end());
		}
		};

	// 1. 원본(Left) 데이터 모으기
	if (dc.HasOriginalCard())
	{
		AppendLines(leftLines, ConvertSignalCardInfoText(dc.GetOriginalSignalCardInfo(), true));
		AppendLines(leftLines, ConvertSwitchCardInfoText(dc.GetOriginalSwitchCardInfo(), true));
		if (leftLines.empty())
		{
			hasLeftData = false;
			leftLines.push_back(_T("원본 데이터가 없습니다."));
		}

	}
	else {
		leftLines.push_back(_T("선택한 파일이 없습니다."));
	}

	// 2. 비교군(Right) 데이터 모으기
	if (dc.HasDiffCard())
	{
		AppendLines(rightLines, ConvertSignalCardInfoText(dc.GetDiffSignalCardInfo(), false));
		AppendLines(rightLines, ConvertSwitchCardInfoText(dc.GetDiffSwitchCardInfo(), false));
		if (rightLines.empty())
		{
			hasRightData = false;
			rightLines.push_back(_T("비교 데이터가 없습니다."));
		}
	}
	else {
		rightLines.push_back(_T("선택한 파일이 없습니다."));
	}

	// 3. 모은 데이터를 "비교 + 표시 + 하이라이트" 공통 함수로 전달
	ApplyCompareOrSingle(hasLeftData, hasRightData, leftLines, rightLines);
}

// [추가] 신호기 카드(SIG_CARDTABLE) 정보 텍스트 변환 - PDOCARDTABLE 분리로 새로 생긴 테이블
std::vector<CString> DiffCompareFrame::ConvertSignalCardInfoText(const std::span<SIG_CARDTABLE>& list, bool bIsOrigin)
{
	std::vector<CString> lines;
	CString temp;

	auto GetDBName = [&](BYTE nNum, GetDBNameByNum num) -> CString {
		if (bIsOrigin) return CommonUtil::GetOriginNameByNumber(nNum, num);
		else return CommonUtil::GetDiffNameByNumber(nNum, num);
		};

	for (const auto& card : list)
	{
		if (card.RackNo == 0 && card.SlotNo == 0) continue;

		CString cardKindStr = (card.CardKind == 3) ? _T("4등형(주신호기/구내폐색)") : (card.CardKind == 4) ? _T("2등형(입환표지/신호기)") : _T("");

		bool bHasPort = false;
		for (int port = 1; port <= MAX_SIG_MODULE_PORT; ++port)
		{
			const auto& item = card.CardData[port];
			if (item.Name[0] == 0 || (unsigned char)item.Name[0] == 0xFF) continue;

			if (!bHasPort)
			{
				lines.push_back(_T("========================================"));
				temp.Format(_T("[신호기 카드] 랙(%d) / 슬롯(%d), 카드구분: %s"), card.RackNo, card.SlotNo, (LPCTSTR)cardKindStr);
				lines.push_back(temp);
				lines.push_back(_T("------------------------------------------------"));
				bHasPort = true;
			}

			CString name = GetSafeString(item.Name, 20);
			name.Trim();
			CString tagName = GetSafeString(item.TagName, 20);
			tagName.Trim();
			CString sigName = GetDBName(item.Idx, SignalIdx);
			temp.Format(_T("  - 포트(%d) 명칭: %s, 표찰: %s, Table Index: %d(%s)"), item.PortNo, (LPCTSTR)name, (LPCTSTR)tagName, item.Idx, (LPCTSTR)sigName);
			lines.push_back(temp);
		}
		if (bHasPort) lines.push_back(_T(""));
	}

	return lines;
}

// [추가] 선로전환기 카드(SWH_CARDTABLE) 정보 텍스트 변환 - PDOCARDTABLE 분리로 새로 생긴 테이블
std::vector<CString> DiffCompareFrame::ConvertSwitchCardInfoText(const std::span<SWH_CARDTABLE>& list, bool bIsOrigin)
{
	std::vector<CString> lines;
	CString temp;

	auto GetDBName = [&](BYTE nNum, GetDBNameByNum num) -> CString {
		if (bIsOrigin) return CommonUtil::GetOriginNameByNumber(nNum, num);
		else return CommonUtil::GetDiffNameByNumber(nNum, num);
		};

	for (const auto& card : list)
	{
		if (card.RackNo == 0 && card.SlotNo == 0) continue;

		CString cardKindStr = (card.CardKind == 5) ? _T("NS-AM") : (card.CardKind == 6) ? _T("MJ81") : _T("");
		CString noseAB = (card.NoseAB == 'A') ? _T("A호") : (card.NoseAB == 'B') ? _T("B호") : _T("");

		bool bHasPort = false;
		for (int port = 1; port <= MAX_SWH_MODULE_PORT; ++port)
		{
			const auto& item = card.CardData[port];
			if (item.Name[0] == 0 || (unsigned char)item.Name[0] == 0xFF) continue;

			if (!bHasPort)
			{
				lines.push_back(_T("========================================"));
				temp.Format(_T("[선로전환기 카드] 랙(%d) / 슬롯(%d), 카드구분: %s"), card.RackNo, card.SlotNo, (LPCTSTR)cardKindStr);
				if (!noseAB.IsEmpty()) temp.AppendFormat(_T(" (%s)"), (LPCTSTR)noseAB);
				lines.push_back(temp);
				lines.push_back(_T("------------------------------------------------"));
				bHasPort = true;
			}

			CString name = GetSafeString(item.Name, 20);
			name.Trim();
			CString tagName = GetSafeString(item.TagName, 20);
			tagName.Trim();
			CString swhName = GetDBName(item.Idx, SwitchIdx);
			CString noseKindStr = (item.NoseKind == 'p') ? _T("첨단") : (item.NoseKind == 'f') ? _T("크로싱") : _T("");
			temp.Format(_T("  - 포트(%d) 명칭: %s, 표찰: %s, Table Index: %d(%s)"), item.PortNo, (LPCTSTR)name, (LPCTSTR)tagName, item.Idx, (LPCTSTR)swhName);
			if (!noseKindStr.IsEmpty()) temp.AppendFormat(_T(", 노스구분: %s"), (LPCTSTR)noseKindStr);
			lines.push_back(temp);
		}
		if (bHasPort) lines.push_back(_T(""));
	}

	return lines;
}
void DiffCompareFrame::SetIncardInfo()
{
	auto& dc = DataComparison::GetInstance();

	std::vector<CString> leftLines;
	std::vector<CString> rightLines;

	const bool hasOrigin = dc.HasOriginalCard();
	const bool hasDiff = dc.HasDiffCard();
	bool hasLeftData = true;
	bool hasRightData = true;
	CDiffGridPaneView* pLeftView = DYNAMIC_DOWNCAST(CDiffGridPaneView, m_wndSplitter.GetPane(0, 0));
	CDiffGridRightView* pRightView = DYNAMIC_DOWNCAST(CDiffGridRightView, m_wndSplitter.GetPane(0, 1));

	// 1. 둘 다 데이터가 없으면 중단
	if (!hasOrigin && !hasDiff)
	{
		return;
	}

	// 2. 원본(Left) 데이터 수집 및 예외 처리
	if (hasOrigin)
	{
		auto inCardInfo = dc.GetOriginalInCardInfo();

		leftLines = ConvertInCardInfoText(inCardInfo, true);
		if (leftLines.empty())
		{
			hasLeftData = false;
			leftLines.push_back(_T("원본 데이터가 없습니다."));
		}
	}
	else
	{
		leftLines.push_back(_T("선택한 파일이 없습니다."));
	}

	// 3. 비교군(Right) 데이터 수집 및 예외 처리
	if (hasDiff)
	{
		auto DiffInCardInfo = dc.GetDiffInCardInfo();
		rightLines = ConvertInCardInfoText(DiffInCardInfo, false);
		if (rightLines.empty())
		{
			hasRightData = false;
			rightLines.push_back(_T("비교 데이터가 없습니다."));
		}
	}
	else
	{
		rightLines.push_back(_T("비교할 파일이 없습니다."));
	}

	// 4. 수집된 라인을 전달하여 [Diff 정렬 -> 화면 출력 -> 색상 하이라이트] 일괄 처리
	ApplyCompareOrSingle(hasLeftData, hasRightData, leftLines, rightLines);
}
// [구조체 변경] INCARDTABLE -> IN_CARDTABLE. RackNo/SlotNo 만 카드 최상위 필드이고,
// Name/BitName/TagName/Kind/Idx/BitNo 는 CardData[MAX_IO_CARD_PORT+1] 하위 포트별 필드로 재구성됨
std::vector<CString> DiffCompareFrame::ConvertInCardInfoText(const std::span<IN_CARDTABLE>& list, bool bIsOrigin)
{
	std::vector<CString> lines;
	CString temp;

	for (const auto& card : list)
	{
		// 유효하지 않은 카드(랙/슬롯 미설정) 건너뛰기
		if (card.RackNo == 0 && card.SlotNo == 0) continue;

		bool bHasPort = false;
		for (int port = 1; port <= MAX_IO_CARD_PORT; ++port)
		{
			const auto& item = card.CardData[port];
			// 유효하지 않은 데이터 건너뛰기
			if (item.Name[0] == 0 || (unsigned char)item.Name[0] == 0xFF) continue;

			if (!bHasPort)
			{
				// 1. 블록 헤더 (LCS 매칭의 고유 키)
				lines.push_back(_T("========================================"));
				temp.Format(_T("[입력 카드] 랙(%d) / 슬롯(%d)"), card.RackNo, card.SlotNo);
				lines.push_back(temp);
				lines.push_back(_T("------------------------------------------------"));
				bHasPort = true;
			}

			// 2. 포트 위치 및 명칭
			CString name = GetSafeString(item.Name, 20);
			name.Trim();
			temp.Format(_T("  - 포트(%d) 명칭: %s"), item.PortNo, (LPCTSTR)name);
			lines.push_back(temp);

			// 3. 비트 명칭 및 표찰 명칭
			CString bitName = GetSafeString(item.BitName, 20);
			bitName.Trim();
			CString tagName = GetSafeString(item.TagName, 20);
			tagName.Trim();
			temp.Format(_T("    비트 명칭: %s, 표찰 명칭(TagName): %s"), (LPCTSTR)bitName, (LPCTSTR)tagName);
			lines.push_back(temp);

			// 4. 종류('T', 'S', 'P', 'L', 'N'), 인덱스, 비트 오프셋
			CString kindStr = _T("");
			switch (item.Kind) {
			case 'V': kindStr = _T("VRD"); break;
			case 'T': kindStr = _T("궤도"); break;
			case 'S': kindStr = _T("신호기"); break;
			case 'P': kindStr = _T("선로전환기"); break;
			case 'L': kindStr = _T("LMR"); break;
			case 'R': kindStr = _T("진로선별둥"); break;
			case 'N': kindStr = _T("역공통"); break;
			case 'B': kindStr = _T("폐색"); break;
			case 'C': kindStr = _T("건널목(고장검지)"); break;
			case 'c': kindStr = _T("건널목(제어건널목)"); break;
			case 't': kindStr = _T("타역궤도"); break;
			case 's': kindStr = _T("타역 신호기"); break;
			case 'p': kindStr = _T("타역 선로전환기"); break;
			case 'D': kindStr = _T("전차선 절연구간"); break;
			case 'J': kindStr = _T("지장물"); break;
			case 'K': kindStr = _T("출발반응등"); break;
			case 'F': kindStr = _T("기타 고장"); break;
			case 'W': kindStr = _T("소속역 정보"); break;
			case 'H': kindStr = _T("열차진입 방지"); break;
			case 'h': kindStr = _T("히터"); break;
			case 'E': kindStr = _T("연동장치"); break;
			}
			temp.Format(_T("    종류: %s, Table Index: %d, Bit Offset: %d"), (LPCTSTR)kindStr, item.Idx, item.BitNo);
			lines.push_back(temp);
		}

		// 항목 간 여백 추가
		if (bHasPort) lines.push_back(_T(""));
	}

	return lines;
}
void DiffCompareFrame::SetOutCardInfo()
{
	auto& dc = DataComparison::GetInstance();

	std::vector<CString> leftLines;
	std::vector<CString> rightLines;

	CDiffGridPaneView* pLeftView = DYNAMIC_DOWNCAST(CDiffGridPaneView, m_wndSplitter.GetPane(0, 0));
	CDiffGridRightView* pRightView = DYNAMIC_DOWNCAST(CDiffGridRightView, m_wndSplitter.GetPane(0, 1));

	const bool hasOrigin = dc.HasOriginal();
	const bool hasDiff = dc.HasDiff();
	bool hasLeftData = true;
	bool hasRightData = true;
	// 1. 둘 다 데이터가 없으면 중단
	// 2. 원본(Left) 데이터 수집 및 예외 처리
	if (hasOrigin)
	{
		auto inCardInfo = dc.GetOriginalOutCardInfo();

		leftLines = ConvertOutCardInfoText(inCardInfo, true);
		if (leftLines.empty()) {
			hasLeftData = false;
			leftLines.push_back(_T("원본 데이터가 없습니다."));
		}
	}
	else
	{
		leftLines.push_back(_T("선택한 파일이 없습니다."));
	}

	// 3. 비교군(Right) 데이터 수집 및 예외 처리
	if (hasDiff)
	{
		auto DiffInCardInfo = dc.GetDiffOutCardInfo();
		rightLines = ConvertOutCardInfoText(DiffInCardInfo, false);
		if (rightLines.empty()) {
			hasRightData = false;
			rightLines.push_back(_T("비교 데이터가 없습니다."));
		}
	}
	else
	{
		rightLines.push_back(_T("비교할 파일이 없습니다."));
	}

	ApplyCompareOrSingle(hasLeftData, hasRightData, leftLines, rightLines);
}

std::vector<CString> DiffCompareFrame::ConvertOutCardInfoText(const std::span<OUT_CARDTABLE>& list, bool bIsOrigin)
{
	std::vector<CString> lines;
	CString temp;

	for (const auto& card : list)
	{
		// 유효하지 않은 카드(랙/슬롯 미설정) 건너뛰기
		if (card.RackNo == 0 && card.SlotNo == 0) continue;

		bool bHasPort = false;
		for (int port = 1; port <= MAX_IO_CARD_PORT; ++port)
		{
			const auto& item = card.CardData[port];
			// 유효하지 않은 데이터 건너뛰기
			if (item.Name[0] == 0 || (unsigned char)item.Name[0] == 0xFF) continue;

			if (!bHasPort)
			{
				// 1. 블록 헤더 (LCS 매칭의 고유 키)
				lines.push_back(_T("========================================"));
				temp.Format(_T("[출력 카드] 랙(%d) / 슬롯(%d)"), card.RackNo, card.SlotNo);
				lines.push_back(temp);
				lines.push_back(_T("------------------------------------------------"));
				bHasPort = true;
			}

			// 2. 포트 위치 및 명칭
			CString name = GetSafeString(item.Name, 20);
			name.Trim();
			temp.Format(_T("  - 포트(%d) 명칭: %s"), item.PortNo, (LPCTSTR)name);
			lines.push_back(temp);

			// 3. 비트 명칭 및 표찰 명칭
			CString bitName = GetSafeString(item.BitName, 20);
			bitName.Trim();
			CString tagName = GetSafeString(item.TagName, 20);
			tagName.Trim();
			temp.Format(_T("    비트 명칭: %s, 표찰 명칭(TagName): %s"), (LPCTSTR)bitName, (LPCTSTR)tagName);
			lines.push_back(temp);

			// 4. 종류, 테이블 인덱스, 비트 오프셋
			CString kindStr = _T("기타('N')");
			switch (item.Kind) {
			case 'V': kindStr = _T("VRD"); break;
			case 'T': kindStr = _T("궤도"); break;
			case 'S': kindStr = _T("신호기"); break;
			case 'P': kindStr = _T("선로전환기"); break;
			case 'L': kindStr = _T("LMR"); break;
			case 'R': kindStr = _T("진로선별둥"); break;
			case 'N': kindStr = _T("역공통"); break;
			case 'B': kindStr = _T("폐색"); break;
			case 'C': kindStr = _T("건널목(고장검지)"); break;
			case 'c': kindStr = _T("건널목(제어건널목)"); break;
			case 't': kindStr = _T("타역궤도"); break;
			case 's': kindStr = _T("타역 신호기"); break;
			case 'p': kindStr = _T("타역 선로전환기"); break;
			case 'D': kindStr = _T("전차선 절연구간"); break;
			case 'J': kindStr = _T("지장물"); break;
			case 'K': kindStr = _T("출발반응등"); break;
			case 'F': kindStr = _T("기타 고장"); break;
			case 'W': kindStr = _T("소속역 정보"); break;
			case 'H': kindStr = _T("열차진입 방지"); break;
			case 'h': kindStr = _T("히터"); break;
			case 'E': kindStr = _T("연동장치"); break;
			}
			temp.Format(_T("    종류: %s, Table Index: %d, Bit Offset: %d"), (LPCTSTR)kindStr, item.Idx, item.BitNo);
			lines.push_back(temp);

			// 5. PORTOUT (구조체엔 있으나 기존 코드에 없던 하위 필드)
			if (item.PORTOUT.OutKind != 0)
			{
				CString outKindStr = _T("");
				switch (item.PORTOUT.OutKind) {
				case 1: outKindStr = _T("신호기 ATS 출력"); break;
				case 2: outKindStr = _T("신호기 후방폐색 제어출력"); break;
				case 3: outKindStr = _T("비상정지 출력"); break;
				case 4: outKindStr = _T("임시속도 출력"); break;
				case 5: outKindStr = _T("주계 출력"); break;
				case 6: outKindStr = _T("운영모드 출력"); break;
				case 7: outKindStr = _T("Switch Heater 출력"); break;
				case 8: outKindStr = _T("전차선 출력"); break;
				}
				temp.Format(_T("    PORTOUT: %s (OutGubun: %d)"), (LPCTSTR)outKindStr, item.PORTOUT.OutGubun);
				lines.push_back(temp);
			}
		}

		// 항목 간 여백 추가
		if (bHasPort) lines.push_back(_T(""));
	}

	return lines;
}

std::vector<CString> DiffCompareFrame::ConvertLogicInfoText(const std::span<std::shared_ptr<LOGIC_VARIABLE>>& logicList, bool bIsOrigin)
{
	std::vector<CString> lines;
	CString temp;

	// 원본/비교군에 따라 CommonUtil을 통해 DB 연결 장비 명칭을 가져오는 람다
	auto GetDBName = [&](UCHAR kind, UINT tableIdx) -> CString {
		GetDBNameByNum numType = GetDBNameByNum::TrackIdx;
		if (kind == 'S' || kind == 's') numType = GetDBNameByNum::SignalIdx;
		else if (kind == 'P' || kind == 'p') numType = GetDBNameByNum::SwitchIdx;
		else if (kind == 'T' || kind == 't') numType = GetDBNameByNum::TrackIdx;
		else return _T("");

		BYTE nNum = static_cast<BYTE>(tableIdx);
		if (bIsOrigin) {
			return CommonUtil::GetOriginNameByNumber(nNum, numType);
		}
		else {
			return CommonUtil::GetDiffNameByNumber(nNum, numType);
		}
		};

	for (const auto& item : logicList)
	{
		// 유효하지 않은 데이터 건너뛰기 (명칭 첫 바이트가 0이거나 0xFF인 경우)
		if (item->szVarName[0] == 0 || (unsigned char)item->szVarName[0] == 0xFF) continue;

		CString varName = GetSafeString(item->szVarName, 46);
		varName.Trim();

		// 1. 블록 헤더 (BinDiff 매칭의 고유 키 역할)
		lines.push_back(_T("========================================"));
		temp.Format(_T("[로직정보] 명칭:%s"), (LPCTSTR)varName);
		lines.push_back(temp);
		lines.push_back(_T("----------------------------------------"));
		temp.Format(_T("인덱스 : %d"), item->nLogicIdx);
		// 2. 로직 구분 (LogicKind) 문자 해석
		CString kindName = _T("기타");
		switch (item->LogicKind) {
		case 'V': kindName = _T("VRD 속도코드제어장치"); break;
		case 'T': kindName = _T("궤도"); break;
		case 'P': kindName = _T("선로전환기"); break;
		case 'S': kindName = _T("신호기"); break;
		case 'L': kindName = _T("LMR"); break;
		case 'R': kindName = _T("진로"); break;
		case 'N': kindName = _T("역공통"); break;
		case 'B': kindName = _T("폐색"); break;
		case 'C': kindName = _T("건널목(고장검지)"); break;
		case 'c': kindName = _T("건널목(제어)"); break;
		case 't': kindName = _T("타역 궤도"); break;
		case 's': kindName = _T("타역 신호기"); break;
		case 'p': kindName = _T("타역 선로전환기"); break;
		case 'D': kindName = _T("전차선 절연구간"); break;
		case 'J': kindName = _T("지장물"); break;
		case 'K': kindName = _T("출발반응등"); break;
		case 'F': kindName = _T("기타 고장"); break;
		case 'W': kindName = _T("소속역 정보"); break;
		case 'H': kindName = _T("CPT"); break;
		case 'h': kindName = _T("히터"); break;
		case 'E': kindName = _T("연동장치 정보"); break;
		}
		temp.Format(_T("  - 로직 구분: %c (%s)"), item->LogicKind, (LPCTSTR)kindName);
		lines.push_back(temp);

		// 3. 연동DB 매핑 정보 및 해석된 장비명
		CString dbTargetName = GetDBName(item->Kind, item->TableIdx);
		if (dbTargetName.IsEmpty()) dbTargetName = _T("-");
		temp.Format(_T("  - DB 매핑: Kind:%c, TableIdx:%d, BitNo:%d (장비명: %s)"),
			item->Kind, item->TableIdx, item->BitNo, (LPCTSTR)dbTargetName);
		lines.push_back(temp);

		// 4. 카드 정보 (CARDINFO)
		CString cardTypeStr = _T("카드 정보 없음");
		if (item->CARDINFO.CardType == 1) cardTypeStr = _T("IN");
		else if (item->CARDINFO.CardType == 2) cardTypeStr = _T("OUT");
		else if (item->CARDINFO.CardType == 3 || item->CARDINFO.CardType == 4) cardTypeStr = _T("PDO");

		temp.Format(_T("  - 카드 정보: 타입(%s), 랙(%d), 슬롯(%d), 포트(%d), attr(0x%02X), param(0x%02X)"),
			(LPCTSTR)cardTypeStr, item->CARDINFO.nRackNo, item->CARDINFO.nSlotNo,
			item->CARDINFO.nPortNo, item->CARDINFO.attr, item->CARDINFO.param);
		lines.push_back(temp);

		// 5. 로직 타입(LogicType) 및 타이머 정보
		CString logicTypeStr = _T("");
		if (item->LogicType == 1) logicTypeStr = _T("입력(I)");
		else if (item->LogicType == 2) logicTypeStr = _T("출력(O)");
		else if (item->LogicType == 4) logicTypeStr = _T("PDO(P)");
		else logicTypeStr.Format(_T("%d"), item->LogicType);

		CString timerKindStr = _T("없음/기타");
		switch (item->TimerKind) {
		case 2: timerKindStr = _T("낙하 후 Delay 여자 (2)"); break;
		case 3: timerKindStr = _T("여자 후 Delay 낙하 (3)"); break;
		case 4: timerKindStr = _T("조건 만족 시 Delay 후 ON-OFF (4)"); break;
		case 5: timerKindStr = _T("OSCI (반복) (5)"); break;
		case 6: timerKindStr = _T("TRIG (6)"); break;
		}

		temp.Format(_T("  - 동작 타입: LogicType(%s), TimerKind(%s), TimeValue:%d ms, TimeValueDB:%d ms"),
			(LPCTSTR)logicTypeStr, (LPCTSTR)timerKindStr, item->TimeValue, item->TimeValueDB);
		lines.push_back(temp);

		// 항목 간 여백 추가
		lines.push_back(_T(""));
	}

	return lines;
}


void DiffCompareFrame::SetLogicInfo(UCHAR logicKind)
{
	// 2만 건 중에서 선택한 로직 종류에 해당하는 데이터만 빠르게 필터링하여 가져옴
	auto originalList = DataComparison::GetInstance().GetOriginalLogicVariable(logicKind);
	auto diffList = DataComparison::GetInstance().GetDiffLogicVariable(logicKind);

	CDiffGridPaneView* pLeftView = DYNAMIC_DOWNCAST(CDiffGridPaneView, m_wndSplitter.GetPane(0, 0));
	CDiffGridRightView* pRightView = DYNAMIC_DOWNCAST(CDiffGridRightView, m_wndSplitter.GetPane(0, 1));

	//bool hasOrigin = DataComparison::GetInstance().HasOriginalLogic();
	//bool hasDiff = DataComparison::GetInstance().HasDiffLogic();

	std::vector<CString> origLines = ConvertLogicInfoText(originalList, true);
	std::vector<CString> diffLines = ConvertLogicInfoText(diffList, false);
	// 4. 수집된 라인을 전달하여 [Diff 정렬 -> 화면 출력 -> 색상 하이라이트] 일괄 처리
	if (!origLines.size() == 0 && !diffLines.size() == 0)
	{
		// 원본과 비교 파일 둘 다 있음 -> 정상적인 Diff 비교 실행
		ApplyDiffToUI(origLines, diffLines);
	}
	else if (origLines.size() == 0 && diffLines.size() == 0)
	{
		ResetEditContent();

		pRightView->m_wndRightEditLDat.Clear();
		pRightView->m_wndRightEditLDat.InsertText(_T("비교 대상 데이터가 없습니다."));
		pRightView->m_wndRightEditLDat.Invalidate();

		pLeftView->m_wndLeftEditLDat.Clear();
		pLeftView->m_wndLeftEditLDat.InsertText(_T("원본 데이터가 없습니다."));
		pLeftView->m_wndLeftEditLDat.Invalidate();

		UpdateNoDataCaptionBar();
	}
	else if (diffLines.size() == 0)
	{
		// 원본만 있고 비교 파일은 없음 -> 원본만 단독 출력 
		ResetEditContent();
		DisplaySingle(origLines, true);
		pRightView->m_wndRightEditLDat.Clear();
		pRightView->m_wndRightEditLDat.InsertText(_T("비교 대상 데이터가 없습니다."));
		pRightView->m_wndRightEditLDat.Invalidate();
		UpdateNoDataCaptionBar();
	}
	else if (origLines.size() == 0) // 해당 경우는 존재하지 않음
	{
		//비교 파일만 있고 원본은 없음 -> 비교 파일만 단독 출력
		ResetEditContent();
		DisplaySingle(diffLines, false);
		pLeftView->m_wndLeftEditLDat.Clear();
		pLeftView->m_wndLeftEditLDat.InsertText(_T("원본 데이터가 없습니다."));
		pLeftView->m_wndLeftEditLDat.Invalidate();
		UpdateNoDataCaptionBar();
	}
}