#pragma once
#include "CustomBCGGridCtrl.h"

// [수정] switchList(SetSwitchType, SwitchGridInfo.cpp)에는 SINGLE/DOUBLE/TRIPLE/FOURTIME/
// SCISSORS/NOSE 6개 항목만 있음 (기존엔 9였다가 8로 수정된 적이 있었으나, 그마저도
// SPCSWITCH/DIRKINDBIT1이 이 "종류(O/X)" 배열에 잘못 섞여 있었음):
//  - DirKind는 종류 비트가 아니라 정위/반위 방향(2비트) 인코딩이라 이미 별도로
//    9~10번(정위/반위) 칼럼에 처리되고 있어 이 배열에서 제외.
//  - SpcSwitch(bit0=타역 선로전환기)는 8번 "타역" 칼럼에 직접(배열 밖에서) 표시하도록 분리.
// 따라서 이 배열/루프가 채우는 칼럼은 1~6번(단동~노스가동)뿐이라 6으로 수정.
#define MAX_SWITCH_TYPE 6
struct SCBitCheckInfo {
	UCHAR* pValue; // 변수의 주소
	UINT   bit;    // 체크할 비트 상수
};

class SwitchGridInfo : public CustomBCGGridCtrl
{
public:
	SwitchGridInfo();
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	virtual CBCGPGridColumnsInfo& GetColumnsInfo() override {
		return m_Column;
	}
	virtual const CBCGPGridColumnsInfo& GetColumnsInfo() const override {
		return m_Column;
	}
	CRect OnGetHeaderRect(CDC* pDC, const CRect& rectDraw) override;
	void OnDrawHeader(CDC* pDC) override;
	void OnPrintHeader(CDC* pDC, CPrintInfo* pInfo) override;
	void OnPosSizeChanged() override;
	void LoadAllSwitchData();
	// [수정] "노스가동"(6번 컬럼)/A호·A호구분·B호·B호구분(11~14번 컬럼)을 CommonUtil::GetSwitchIsNose()로
	// 정확히 판정하려면 이 스위치의 Table Index(GetSwitchInfo() 배열 내 위치)가 필요해서 추가.
	void SetSwitchType(CBCGPGridRow* pRow, SwitchInfoType& sigItem, Byte_t nIdx);
public:
	void UpdateSwitchData();
};

