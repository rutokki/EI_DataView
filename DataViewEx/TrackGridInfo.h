#pragma once
#include "CustomBCGGridCtrl.h"

#define MAX_TRACK_TYPE 18

class TrackGridInfo : public CustomBCGGridCtrl
{
public:
	TrackGridInfo();
	CString GetSafeString(const unsigned char* pRawData, int nMaxLength);
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual CBCGPGridColumnsInfo& GetColumnsInfo() override {
		return m_Column;
	}
	virtual const CBCGPGridColumnsInfo& GetColumnsInfo() const override {
		return m_Column;
	}
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void LoadAllTrackData();
public:
	void UpdateTrackData();
	CString GetBlockName(Byte_t blockNo);
	CString GetAppDelayTime(Byte_t& time);
	void SetItemType(CBCGPGridRow* pRow, TrackInfoType& a);
	CString LoadAllSwitches(const _TrackInfoType_t::_Switch_t* pSwitchArray, int nCount);
	CString GetSignalName(Byte_t* pSignalArray, int nSize);
	CRect OnGetHeaderRect(CDC* pDC, const CRect& rectDraw) override;
	void OnDrawHeader(CDC* pDC) override;
	void OnPrintHeader(CDC* pDC, CPrintInfo* pInfo) override;
	void OnPosSizeChanged() override;

protected:

};

