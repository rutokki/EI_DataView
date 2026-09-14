#pragma once
#include "stdafx.h"
enum DiffType {
	DIFF_MATCH = 0,
	DIFF_DELETE,    // 원본(Left)에만 존재 (삭제됨)
	DIFF_INSERT,    // 비교(Right)에만 존재 (추가됨)
	DIFF_CHANGE,   // 동일한 위치지만 내용이 다름
	DIFF_MOVE_DELETE,     // 동일한 내용이지만 위치가 다름 (이동됨)
	DIFF_MOVE_INSERT,     // 동일한 내용이지만 위치가 다름 (이동됨)
};
struct DiffLine {
	DiffType type;
	CString leftStr;
	CString rightStr;
	int movePartnerRow = -1;
	int moveGroupId = -1;
};

