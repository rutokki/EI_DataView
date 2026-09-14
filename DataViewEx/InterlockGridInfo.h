#pragma once
#include "CustomBCGGridCtrl.h"

#define MAX_BLOCK_KIND 20
class InterlockGridInfo : public CustomBCGGridCtrl
{
public:
	InterlockGridInfo();
	virtual ~InterlockGridInfo();
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	virtual void OnDrawGroupRow(CDC* pDC, CBCGPGridRow* pRow, const CRect& rect);
	CString GetRouteInfoStr(const InterLockInfoType& item);
	CString GetCountStr(const InterLockInfoType& item);
	CString GetSwitchLockStr(const InterLockInfoType& item);
	CString GetRouteLockStr(const InterLockInfoType& item);
	CString GetEquipLockStr(const InterLockInfoType& item);
	CString GetExceptionStr(const InterLockInfoType& item);
	CString GetMiscArrayStr(const InterLockInfoType& item);
	CString GetUhoInfoStr(const InterLockInfoType& item);
	CString GetSpecialSwitchStr(const InterLockInfoType& item);
	CString GetSpcStateStr(const InterLockInfoType& item);

	CString GetOppositeRouteStr(UINT nRteNo);

	CString GetRouteKindStr(UINT nRteNo);

	CString GetRouteTrackLockStr(const InterLockInfoType& item, UINT nRteNo);

	CString GetRouteInfo_SignalTrack(UINT nRteNo);

	CString GetRouteInfo_SigStop(const InterLockInfoType& item);



public:
	void loadInterLockData();
};

