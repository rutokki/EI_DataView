#include "stdafx.h"
#include "InterlockGridInfo.h"
#include "StructMainData.h"
#include "CommonUtils.h"
#include "BitUtility.h"
using namespace CommonUtil;
using namespace RouteInfo;
InterlockGridInfo::InterlockGridInfo()
{
	EnableColumnsResizeInClientArea(FALSE);
	EnableDragHeaderItems(FALSE);
	SetReadOnly(TRUE);
	EnableMarkSortedColumn(FALSE);
	SetClearInplaceEditOnEnter(FALSE);
}
InterlockGridInfo::~InterlockGridInfo()
{
}
BEGIN_MESSAGE_MAP(InterlockGridInfo, CustomBCGGridCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()
int InterlockGridInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CustomBCGGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	auto interLcokInfo = GridColumnDefine::GetInterLockingDataColumnInfo();
	for (int i = 0; i < interLcokInfo.size(); i++) {
		InsertColumn(i, interLcokInfo[i].columnName, interLcokInfo[i].columnWidth);
		if (!(i == interLcokInfo.size() - 1)) {
			SetHeaderAlign(i, HDF_CENTER);
			SetColumnAlign(i, HDF_CENTER);
		}
		SetHeaderAlign(interLcokInfo.size(), HDF_CENTER);
	}
	//loadInterLockData();
	return 0;
}
void InterlockGridInfo::OnDestroy()
{
	CustomBCGGridCtrl::OnDestroy();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
void InterlockGridInfo::OnDrawGroupRow(CDC* pDC, CBCGPGridRow* pRow, const CRect& rect)
{
	// 그룹 행의 배경색 커스텀
	pDC->FillSolidRect(rect, RGB(240, 240, 240));
}
//  진로 기본 정보
CString InterlockGridInfo::GetRouteInfoStr(const InterLockInfoType& item)
{
	CString str, temp;
	// DB 번호들을 각각의 이름(문자열)으로 조회
	CString strSignalName = CommonUtil::GetDBNameByNumber(static_cast<Byte_t>(item.RouteInfo.SignalNo), GetDBNameByNum::SignalIdx);
	CString strDepTrack = CommonUtil::GetDBNameByNumber(static_cast<Byte_t>(item.RouteInfo.DepartureT), GetDBNameByNum::TrackIdx);
	CString strArrTrack = CommonUtil::GetDBNameByNumber(static_cast<Byte_t>(item.RouteInfo.ArrivalT), GetDBNameByNum::TrackIdx);
	// 이름 기반으로 포맷팅 적용
	str.Format(_T("신호기:%s, 출발:%s, 도착:%s"),
		(LPCTSTR)strSignalName,
		(LPCTSTR)strDepTrack,
		(LPCTSTR)strArrTrack);
	// 비트 유틸을 활용한 유도등 여부 판별
	if (IsBitSet(item.RouteInfo.IsCallOnSig, 0))
	{
		str += _T(", [(무)유도등 ON]");
	}
	return str + _T("\r\n");
}
//  카운트 정보
CString InterlockGridInfo::GetCountStr(const InterLockInfoType& item)
{
	CString str;
	str.Format(_T("쇄정 선로전환기 수 :%d, 단조건 수 :%d, 쇄정 신호기 수 :%d, 신호제어 궤도 수:%d, 진로쇄정 궤도 수:%d, 접근쇄정 궤도 수:%d\r\n"),
		item.Count.NoOfSwhLock, item.Count.NoOfExcept, item.Count.NoOfSigLock,
		item.Count.NoOfCtlLock, item.Count.NoOfRteLock, item.Count.NoOfAppLock);
	return str;
}
//  쇄정 선로전환기 정보 (SwitchIdx 활용)
CString InterlockGridInfo::GetSwitchLockStr(const InterLockInfoType& item)
{
	if (item.Count.NoOfSwhLock == 0) return _T("");
	CString str = _T("쇄정 선로전환기 : "), temp;
	for (int i = 0; i < item.Count.NoOfSwhLock; i++)
	{
		if (item.SwitchLock[i].SwitchNo == 0) continue;
		// 선로전환기 번호를 이름으로 변환
		CString strSwitchName = CommonUtil::GetDBNameByNumber(static_cast<Byte_t>(item.SwitchLock[i].SwitchNo), GetDBNameByNum::SwitchIdx);
		temp.Format(_T("%s[%d]"), (LPCTSTR)strSwitchName, item.SwitchLock[i].Direction);
		str += temp;
		Byte_t lk = item.SwitchLock[i].LockKind;
		if (lk > 0) {
			str += _T("(");
			if (lk & 0x01) str += _T("진로");
			if (lk & 0x02) str += _T(" Overlap");
			if (lk & 0x04) str += _T(" 설정시쇄정");
			if (lk & 0x08) {
				// [추가] TimeValue: 시간후해정의 실제 시간값(초) 표시
				CString strTime;
				strTime.Format(_T("착점도착 후  %d초 후 해정, 착점궤도 복구시 해정"), item.SwitchLock[i].TimeValue);
				str += strTime;
			}
			if (lk & 0x10) {
				// [추가] TimeValue: 시간후해정의 실제 시간값(초) 표시
				CString strTime;
				strTime.Format(_T("착점도착 후  %d초 후 해정, 착점궤도 복구시에도 쇄정 유지"), item.SwitchLock[i].TimeValue);
				str += strTime;
			}
			if (lk & 0x01 || lk & 0x02) {
				if (item.SwitchLock[i].ReleaseTrkNo > 0) {
					// 해정 궤도 번호도 궤도 이름으로 변환
					CString strReleaseTrk = CommonUtil::GetDBNameByNumber(static_cast<Byte_t>(item.SwitchLock[i].ReleaseTrkNo), GetDBNameByNum::TrackIdx);
					temp.Format(_T(" 선로전환기 해정 궤도 :%s"), (LPCTSTR)strReleaseTrk);
					str += temp;
				}
			}
		}
		else {
			str += _T(" ");
		}
	}
	return str + _T("\r\n");
}
//  진로쇄정 궤도 정보 (TrackIdx 활용)
CString InterlockGridInfo::GetRouteLockStr(const InterLockInfoType& item)
{
	CString strRouteLock, temp;
	for (int i = 0; i < sizeof(item.RouteLock) / sizeof(item.RouteLock[0]); i++)
	{
		if (item.RouteLock[i].TrackNo == 0) continue;
		// 궤도 번호를 이름으로 변환
		CString strTrackName = CommonUtil::GetDBNameByNumber(static_cast<Byte_t>(item.RouteLock[i].TrackNo), GetDBNameByNum::TrackIdx);
		CString strRouteLockKind;
		if (item.RouteLock[i].Kind == 1) strRouteLockKind = _T("진로쇄정 궤도(TRACK_ROUTELOCK)");
		else if (item.RouteLock[i].Kind == 2) strRouteLockKind = _T("접촉관계 쇄정(TRACK_TRACKLOCK)");
		else if (item.RouteLock[i].Kind == 3) strRouteLockKind = _T("임의 쇄정(TRACK_TEMPLOCK) - DelTrack");
		else if (item.RouteLock[i].Kind == 4) strRouteLockKind = _T("임의 쇄정(TRACK_SPECIALLOCK)");

		temp.Format(_T("%s(%s)"), (LPCTSTR)strTrackName, (LPCTSTR)strRouteLockKind);
		strRouteLock += temp;
		Byte_t udo = item.RouteLock[i].RelGubun;
		if (udo & 0x01) strRouteLock += _T(" 무유도정지");
		else if (udo == 0) strRouteLock += _T(" 모두정지");
		if (udo & 0x02) strRouteLock += _T(" 유도만현시");
		if (udo & 0x04) {
			// [추가] TimeValue: 시간후해정의 실제 시간값(초) 표시
			CString strTime;
			strTime.Format(_T(" 시간후해정(%d초)"), item.RouteLock[i].TimeValue);
			strRouteLock += strTime;
		}

		if (item.RouteLock[i].ReleaseTrk & 0x01) strRouteLock += _T(" 도착시 진로해정");
		strRouteLock += _T(") ");
	}
	if (!strRouteLock.IsEmpty()) return _T("진로쇄정 궤도 : ") + strRouteLock + _T("\r\n");
	return _T("");
}
//  설비조건 쇄정 정보
CString InterlockGridInfo::GetEquipLockStr(const InterLockInfoType& item)
{
	CString strEquip, temp;
	CString equipLockKind;
	CString equipStateCond;
	auto switchData = StructMainData::GetInstance().GetSwitchInfo();
	for (int i = 0; i < sizeof(item.EquipLock) / sizeof(item.EquipLock[0]); i++)
	{
		if (item.EquipLock[i].Kind == 0) continue;
		equipLockKind = GetKindName(item.EquipLock[i].Kind);
		temp.Format(_T("%s"), (LPCTSTR)equipLockKind);
		if (item.EquipLock[i].Kind == 'J' && item.EquipLock[i].Gubun != 0) {
			if (item.EquipLock[i].Gubun == 'F') temp.AppendFormat(_T("%s"), _T("낙석"));
			else if (item.EquipLock[i].Gubun == 'P') temp.AppendFormat(_T("%s"), _T("보호"));
		}

		switch (item.EquipLock[i].stateCond) {
		case 1:
			equipStateCond = _T("설비 낙하시 (무)유도와 신호기 모두 정지");
			break;
		case 2:
			equipStateCond = _T("설비 낙하시 무유도만 정지");
			break;
		case 3:
			equipStateCond = _T("설비 낙하시 유도만 현시");
			break;
		case 4:
			equipStateCond = _T("설비 여자일 때 신호정지");
			break;
		case 5:
			if (item.EquipLock[i].Kind == 'P')
				if (switchData[item.EquipLock[i].TblNo].Kind.DirKind != 1)
					equipStateCond = _T("선로전환기 정위 아닐 시 신호정지");
			break;
		case 6:
			if (item.EquipLock[i].Kind == 'P')
				if (switchData[item.EquipLock[i].TblNo].Kind.DirKind == 2)
					equipStateCond = _T("선로전환기 반위 아닐 시 신호정지");
			break;
		case 7:
			if (item.EquipLock[i].Kind == 'P')
				equipStateCond = _T("선로전환기 쇄정상태일 때 신호정지");
			break;

		}
		temp.AppendFormat(_T("[Cond : %s] "), equipStateCond);
		strEquip += temp;
	}
	if (!strEquip.IsEmpty()) return _T("설비조건 쇄정 : ") + strEquip + _T("\r\n");
	return _T("");
}
//  단조건 정보 (SwitchIdx 활용)
CString InterlockGridInfo::GetExceptionStr(const InterLockInfoType& item)
{
	if (item.Count.NoOfExcept == 0)
		return _T("");
	CString strResult = _T("단조건 : ");
	CString strTmp;
	bool bFirst = true;
	for (int i = 0; i < item.Count.NoOfExcept; i++)
	{
		const auto& exceptItem = item.Exception[i];
		if (exceptItem.Kind == 0)
			continue;
		// 콤마 구분자 처리 (첫 번째 항목이 아닐 경우 앞에 추가)
		if (!bFirst)
		{
			strResult += _T(", ");
		}
		strTmp.Empty();
		// 스위치 번호 이름 조회
		CString strSwhName = CommonUtil::GetDBNameByNumber(static_cast<Byte_t>(exceptItem.SwitchNo), GetDBNameByNum::SwitchIdx);
		switch (exceptItem.Kind)
		{
		case DANCASE_KIND_SIGNAL:
		{
			// 쇄정 란 : 선로전환기 단 신호기 (TblIdx는 신호기 인덱스로 가정)
			CString strSigName = CommonUtil::GetDBNameByNumber(static_cast<Byte_t>(exceptItem.TblIdx), GetDBNameByNum::SignalIdx);
			if (exceptItem.SwitchDir == SWITCH_DIRECTION_KIND_DB_NORMAL) // 정위
			{
				strTmp.Format(_T("%c: %s - %s"), exceptItem.Kind, (LPCTSTR)strSwhName, (LPCTSTR)strSigName);
			}
			else if (exceptItem.SwitchDir == SWITCH_DIRECTION_KIND_DB_REVERSE) // 반위 (!)
			{
				strTmp.Format(_T("%c: !%s - %s"), exceptItem.Kind, (LPCTSTR)strSwhName, (LPCTSTR)strSigName);
			}
		}
		break;
		case DANCASE_KIND_TRACK:
		{
			// 신호제어 란 : 궤도 단 선로전환기 (TblIdx는 궤도 인덱스로 가정)
			CString strTrkName = CommonUtil::GetDBNameByNumber(static_cast<Byte_t>(exceptItem.TblIdx), GetDBNameByNum::TrackIdx);
			if (exceptItem.SwitchDir == SWITCH_DIRECTION_KIND_DB_NORMAL) // 정위
			{
				strTmp.Format(_T("%c: %s - %s"), exceptItem.Kind, (LPCTSTR)strTrkName, (LPCTSTR)strSwhName);
			}
			else if (exceptItem.SwitchDir == SWITCH_DIRECTION_KIND_DB_REVERSE) // 반위 (!)
			{
				strTmp.Format(_T("%c: %s - !%s"), exceptItem.Kind, (LPCTSTR)strTrkName, (LPCTSTR)strSwhName);
			}
		}
		break;
		}
		if (!strTmp.IsEmpty())
		{
			strResult += strTmp;
			bFirst = false;
		}
	}
	// 단조건에 유효한 내용이 채워지지 않았으면 빈 문자열 반환
	if (bFirst)
		return _T("");
	return strResult + _T("\r\n");
}
//  일반 배열 및 기타 정보 (구내폐색, 쇄정신호기, 철사쇄정 등 - SignalIdx, TrackIdx 활용)
CString InterlockGridInfo::GetMiscArrayStr(const InterLockInfoType& item)
{
	CString strTotal, temp;
	// 쇄정 신호기 (SignalIdx 적용)
	CString strSigLock;
	for (int i = 0; i < sizeof(item.SignalLock); i++) {
		if (item.SignalLock[i] != 0) {
			CString strName = CommonUtil::GetDBNameByNumber(item.SignalLock[i], GetDBNameByNum::SignalIdx);
			temp.Format(_T("%s "), (LPCTSTR)strName);
			strSigLock += temp;
		}
	}
	if (!strSigLock.IsEmpty()) strTotal += _T("쇄정 신호기 : ") + strSigLock + _T("\r\n");
	// 폐로쇄정 궤도 (TrackIdx 적용)
	CString strLeverTrk;
	for (int i = 0; i < sizeof(item.LeverLockTrack); i++) {
		if (item.LeverLockTrack[i] != 0) {
			CString strName = CommonUtil::GetDBNameByNumber(item.LeverLockTrack[i], GetDBNameByNum::TrackIdx);
			temp.Format(_T("%s "), (LPCTSTR)strName);
			strLeverTrk += temp;
		}
	}
	if (!strLeverTrk.IsEmpty()) strTotal += _T("폐로쇄정 궤도 : ") + strLeverTrk + _T("\r\n");
	// 신호제어/철사쇄정 (TrackIdx 적용)
	CString strCtrlTrk;
	for (int i = 0; i < sizeof(item.ControlTrack); i++) {
		if (item.ControlTrack[i] != 0) {
			CString strName = CommonUtil::GetDBNameByNumber(item.ControlTrack[i], GetDBNameByNum::TrackIdx);
			temp.Format(_T("%s "), (LPCTSTR)strName);
			strCtrlTrk += temp;
		}
	}
	if (!strCtrlTrk.IsEmpty()) strTotal += _T("신호제어/철사쇄정 : ") + strCtrlTrk + _T("\r\n");
	// TTB 단락 유지궤도 (TrackIdx 적용)
	CString strTtbTrk;
	for (int i = 0; i < sizeof(item.TtbDelTrack); i++) {
		if (item.TtbDelTrack[i] != 0) {
			CString strName = CommonUtil::GetDBNameByNumber(item.TtbDelTrack[i], GetDBNameByNum::TrackIdx);
			temp.Format(_T("%s "), (LPCTSTR)strName);
			strTtbTrk += temp;
		}
	}
	if (!strTtbTrk.IsEmpty()) strTotal += _T("TTB 단락 유지궤도 : ") + strTtbTrk + _T("\r\n");
	// 접근/보류쇄정 (TrackIdx 적용)
	// [구조체 변경] item.ApproachLock.ApproachTrack[] 는 구조체 배열이므로 원소 수는 sizeof가 아닌
	//              MAX_APP_LOCK 기준으로, 유효성 판별은 AppTrack 필드로 함
	CString strAppLock;
	for (int i = 0; i < MAX_APP_LOCK; i++) {
		if (item.ApproachLock.ApproachTrack[i].AppTrack != 0) {
			CString strName = CommonUtil::GetDBNameByNumber(item.ApproachLock.ApproachTrack[i].AppTrack, GetDBNameByNum::TrackIdx);
			temp.Format(_T("%s "), (LPCTSTR)strName);
			strAppLock += temp;
		}
	}
	if (!strAppLock.IsEmpty() || item.ApproachLock.ReleaseTime > 0) {
		temp.Format(_T("접근/보류쇄정 : [%d초] %s\r\n"), item.ApproachLock.ReleaseTime, (LPCTSTR)strAppLock);
		strTotal += temp;
	}
	// [구조체 변경] item.DualArrive / item.ExtArrivalT / item.RouteReleaseT 는 현재
	//              EI_IP_DBStruct_Typedef.h 의 InterLockInfoType 에 없는 필드라 제외함
	return strTotal;
}
//  유효장 정보 (TrackIdx 활용)
CString InterlockGridInfo::GetUhoInfoStr(const InterLockInfoType& item)
{
	if (item.UhoInfo.FirstTrack == 0 && item.UhoInfo.LastTrack == 0) return _T("");
	CString strFirst = CommonUtil::GetDBNameByNumber(static_cast<Byte_t>(item.UhoInfo.FirstTrack), GetDBNameByNum::TrackIdx);
	CString strLast = CommonUtil::GetDBNameByNumber(static_cast<Byte_t>(item.UhoInfo.LastTrack), GetDBNameByNum::TrackIdx);
	CString str, temp;
	str.Format(_T("유효장 : %s ~ %s (해정시간:%d초, 착점해정설정:%d) 궤도:"),
		(LPCTSTR)strFirst, (LPCTSTR)strLast, item.UhoInfo.LockTime, (item.UhoInfo.UhoRelease & 0x01));
	for (int i = 0; i < 8; i++) {
		if (item.UhoInfo.TrackNo[i] != 0) {
			CString strTrkName = CommonUtil::GetDBNameByNumber(static_cast<Byte_t>(item.UhoInfo.TrackNo[i]), GetDBNameByNum::TrackIdx);
			temp.Format(_T("%s "), (LPCTSTR)strTrkName);
			str += temp;
		}
	}
	return str + _T("\r\n");
}
// 접근쇄정 선로조건 및 점유시 전환불가 (SwitchIdx, TrackIdx 활용)
CString InterlockGridInfo::GetSpecialSwitchStr(const InterLockInfoType& item)
{
	CString strTotal, temp;
	// 접근쇄정 선로조건
	// [구조체 변경] item.ApproachSwitch(최상위 2차원 배열) 는 없고, 실제로는
	//              item.ApproachLock.ApproachTrack[i].ApproachSwitch[j] 로 중첩되어 있음
	CString strAppSwh;
	for (int i = 0; i < MAX_APP_LOCK; i++) {
		for (int j = 0; j < MAX_APP_LOCK_SWITCH; j++) {
			const auto& swh = item.ApproachLock.ApproachTrack[i].ApproachSwitch[j];
			if (swh.SwitchNo != 0) {
				CString strSwitchName = CommonUtil::GetDBNameByNumber(static_cast<Byte_t>(swh.SwitchNo), GetDBNameByNum::SwitchIdx);
				CString strRteName = CommonUtil::GetDBNameByNumber(static_cast<Byte_t>(swh.RteNo), GetDBNameByNum::InterLockIdx);
				temp.Format(_T("궤도IDX[%d] 선로:%s[%d] 진로:%s  "), i, (LPCTSTR)strSwitchName, swh.SwitchDir, (LPCTSTR)strRteName);
				strAppSwh += temp;
			}
		}
	}
	if (!strAppSwh.IsEmpty()) strTotal += _T("접근쇄정 선로조건 : ") + strAppSwh + _T("\r\n");
	// 점유시 전환불가
	CString strSwhChk;
	for (int i = 0; i < sizeof(item.SwitchCheck) / sizeof(item.SwitchCheck[0]); i++) {
		if (item.SwitchCheck[i].TrackNo == 0) continue;
		CString strTrackName = CommonUtil::GetDBNameByNumber(static_cast<Byte_t>(item.SwitchCheck[i].TrackNo), GetDBNameByNum::TrackIdx);
		CString strSwitchName = CommonUtil::GetDBNameByNumber(static_cast<Byte_t>(item.SwitchCheck[i].SwitchNo), GetDBNameByNum::SwitchIdx);
		temp.Format(_T("궤도:%s->선로:%s[%d]  "), (LPCTSTR)strTrackName, (LPCTSTR)strSwitchName, item.SwitchCheck[i].SwitchDir);
		strSwhChk += temp;
	}
	if (!strSwhChk.IsEmpty()) strTotal += _T("점유시 전환불가 : ") + strSwhChk + _T("\r\n");
	return strTotal;
}
//  타역설비 조건 (TrackIdx, SignalIdx, SwitchIdx 활용)
CString InterlockGridInfo::GetSpcStateStr(const InterLockInfoType& item)
{
	CString strSpc, temp;
	for (int i = 0; i < 5; i++)
	{
		if (item.SpcState[i].SpcTrack == 0 && item.SpcState[i].SpcSignal == 0 && item.SpcState[i].SpcSwitch == 0 && item.SpcState[i].SpcFaultIDX == 0) continue;
		strSpc += _T("[");
		if (item.SpcState[i].SpcTrack > 0) {
			CString strTrkName = CommonUtil::GetDBNameByNumber(static_cast<Byte_t>(item.SpcState[i].SpcTrack), GetDBNameByNum::TrackIdx);
			temp.Format(_T("궤도:%s "), (LPCTSTR)strTrkName); strSpc += temp;
		}
		if (item.SpcState[i].SpcSignal > 0) {
			CString strSigName = CommonUtil::GetDBNameByNumber(static_cast<Byte_t>(item.SpcState[i].SpcSignal), GetDBNameByNum::SignalIdx);
			temp.Format(_T("신호:%s(%s) "), (LPCTSTR)strSigName, (item.SpcState[i].SignalStat & 0x01) ? _T("진행") : _T("정지")); strSpc += temp;
		}
		if (item.SpcState[i].SpcSwitch > 0) {
			CString strSwhName = CommonUtil::GetDBNameByNumber(static_cast<Byte_t>(item.SpcState[i].SpcSwitch), GetDBNameByNum::SwitchIdx);
			temp.Format(_T("선로:%s(%d) "), (LPCTSTR)strSwhName, item.SpcState[i].SwitchStat); strSpc += temp;
		}
		if (item.SpcState[i].SpcFaultIDX > 0) {
			// [수정] SpcFaultIDX의 상위비트를 조건으로 쓰던 방식 대신, 구조체상 별도 필드인
			//        SpcFaultStat(bit0=0:낙하시 신호정지(default), bit0=1:여자시 신호정지)을 사용하도록 수정
			temp.Format(_T("기타고장IDX:%d(%s) "), item.SpcState[i].SpcFaultIDX,
				IsBitSet(item.SpcState[i].SpcFaultStat, 0) ? _T("여자시") : _T("낙하시"));
			strSpc += temp;
		}
		strSpc += _T("] ");
	}
	if (!strSpc.IsEmpty()) return _T("타역조건 : ") + strSpc + _T("\r\n");
	return _T("");
}
CString InterlockGridInfo::GetOppositeRouteStr(UINT nRteNo)
{
	auto routeSpan = StructMainData::GetInstance().GetRouteInfo();
	auto ilkSpan = StructMainData::GetInstance().GetInterLockInfo();
	// 0기반 인덱스로 변환
	int nRouteIdx = (int)nRteNo - 1;
	if (nRouteIdx < 0 || nRouteIdx >= (int)routeSpan.size())
		return _T("");
	CString strEquipList = _T("");
	bool bFirst = true;
	//---------------------------------------------------------------
	for (int nIdxInc = 0; nIdxInc < MAX_INHIBIT_ROUTE; nIdxInc++)
	{
		WORD inhibitRteNo = routeSpan[nRouteIdx].InhibitRteNo[nIdxInc];
		if (0 == inhibitRteNo)
			break;
		if (bFirst)
		{
			strEquipList = _T("대항진로 :");
			bFirst = false;
		}
		// 대항진로 번호는 원본과 동일하게 ILK_INFO(ilkSpan) 인덱스로 직접 사용
		if (inhibitRteNo < ilkSpan.size())
		{
			CString szEquipTmp;
			szEquipTmp.Format(_T(" %s"), (LPCTSTR)CString(ilkSpan[inhibitRteNo].Name));
			strEquipList += szEquipTmp;
		}
	}
	//---------------------------------------------------------------
	if (!strEquipList.IsEmpty())
	{
		return strEquipList + _T("\r\n");
	}
	return _T("");
}
CString InterlockGridInfo::GetRouteKindStr(UINT nRteNo)
{
	auto routeSpan = StructMainData::GetInstance().GetRouteInfo();
	int nRouteIdx = (int)nRteNo - 1;
	if (nRouteIdx < 0 || nRouteIdx >= (int)routeSpan.size())
		return _T("");
	// RTE(RouteInfoType) 구조체의 실제 멤버 계층 접근
	// [구조체 변경] Kind/RouteFlag 는 .Value 로 감싸인 union 이 아니라 필드가 바로 노출되는 구조체임
	const auto& rteItem = routeSpan[nRouteIdx];
	const auto& kindVal = rteItem.Kind;
	const auto& flagVal = rteItem.RouteFlag;
	CString strEquipList = _T("진로구분 :");
	//  방향 판별 (Left/Right)
	if (IsBitSet(kindVal.Direction, 0))
		strEquipList += _T(" [Left방향]");
	else
		strEquipList += _T(" [Right방향]");
	//  Kind 세부 플래그 판별
	if (IsBitSet(kindVal.Special, 0)) strEquipList += _T(" [유효장 진로]");
	if (IsBitSet(kindVal.Special, 1)) strEquipList += _T(" [단조건 진로]");
	if (IsBitSet(kindVal.IsTTB, 0))    strEquipList += _T(" [TTB]");
	if (IsBitSet(kindVal.IsBlock, 0))  strEquipList += _T(" [구내폐색]");
	if (IsBitSet(kindVal.Arrive, 0))   strEquipList += _T(" [장내진로]");
	if (IsBitSet(kindVal.Depart, 0))   strEquipList += _T(" [출발진로]");
	if (IsBitSet(kindVal.Shunt, 0))    strEquipList += _T(" [입환진로]");
	if (IsBitSet(kindVal.IsLC, 0))     strEquipList += _T(" [구내 건널목 포함]");
	// [추가] Kind 의 나머지 플래그
	if (IsBitSet(kindVal.JeonlaRevBlk, 0)) strEquipList += _T(" [전라선 역방향 폐색]");
	if (IsBitSet(kindVal.ShuntSig, 0))     strEquipList += _T(" [입환 표지·신호기 공용(입환공용)]");
	//  RouteFlag 세부 플래그 판별
	if (IsBitSet(flagVal.UdoSig, 0))
	{
		strEquipList += _T(" [(무)유도진로]");
	}
	// [수정] IOCheck: "출력/입력 금지" 가 아니라 "출력/입력 있음" 여부를 나타내는 필드임
	if (IsBitSet(flagVal.IOCheck, 0)) strEquipList += _T(" [진로선별등 출력있음(HU)]");
	if (IsBitSet(flagVal.IOCheck, 1)) strEquipList += _T(" [진로선별등 입력있음(ULMR)]");
	return strEquipList + _T("\r\n");
}
CString InterlockGridInfo::GetRouteTrackLockStr(const InterLockInfoType& item, UINT nRteNo)
{
	auto routeSpan = StructMainData::GetInstance().GetRouteInfo();
	int nRouteIdx = (int)nRteNo - 1;
	if (nRouteIdx < 0 || nRouteIdx >= (int)routeSpan.size())
		return _T("");
	const auto& rteItem = routeSpan[nRouteIdx];
	CString strEquipList = _T("쇄정 궤도 :");
	CString strTmp;
	//. 쇄정 궤도 (InterLockInfo의 ControlTrack 순회)
	for (int i = 0; i < NO_OF_LOCK_TRK; i++)
	{
		Byte_t trkNo = item.ControlTrack[i];
		if (trkNo == 0)
			break;
		CString strTrkName = CommonUtil::GetDBNameByNumber(trkNo, GetDBNameByNum::TrackIdx);
		strTmp.Format(_T(" %s"), (LPCTSTR)strTrkName);
		strEquipList += strTmp;
	}
	//  출발 폐색 (RTE의 DepBlockNo)
	if (rteItem.DepBlockNo > 0 && rteItem.DepBlockNo < MAX_BLOCK_KIND)
	{
		// 만약 블록 이름 조회가 필요하다면 CommonUtil 또는 원본 배열(BLK_INFO) 사용
		CString strBlkName = CommonUtil::GetDBNameByNumber(rteItem.DepBlockNo, GetDBNameByNum::BlockIdx);
		strTmp.Format(_T(",  폐색: [출발:%s]"), (LPCTSTR)strBlkName);
		strEquipList += strTmp;
	}
	//  장내 폐색 (RTE의 ArrBlockNo)
	if (rteItem.ArrBlockNo > 0 && rteItem.ArrBlockNo < MAX_BLOCK_KIND)
	{
		CString strBlkName = CommonUtil::GetDBNameByNumber(rteItem.ArrBlockNo, GetDBNameByNum::BlockIdx);
		strTmp.Format(_T(",  폐색: [장내:%s]"), (LPCTSTR)strBlkName);
		strEquipList += strTmp;
	}
	// [추가] NotOutBlock: 진로 제어시 폐색 출력이 없는지 여부
	if (IsBitSet(rteItem.NotOutBlock, 0))
	{
		strEquipList += _T(",  [진로제어시 폐색 출력없음]");
	}
	//  폐로쇄정 (InterLockInfo의 LeverLockTrack)
	if (item.LeverLockTrack[0] > 0)
	{
		strEquipList += _T(",  폐로쇄정:");
		for (int i = 0; i < 5; i++)
		{
			Byte_t trkNo = item.LeverLockTrack[i];
			if (trkNo == 0)
				break;
			CString strTrkName = CommonUtil::GetDBNameByNumber(trkNo, GetDBNameByNum::TrackIdx);
			strTmp.Format(_T("<%s>"), (LPCTSTR)strTrkName);
			strEquipList += strTmp;
		}
	}
	return strEquipList + _T("\r\n");
}
CString InterlockGridInfo::GetRouteInfo_SignalTrack(UINT nRteNo)
{
	if ((0 == nRteNo) || (nRteNo >= MAX_ROUTE))
		return _T("");
	int nRouteIdx = (int)nRteNo - 1;
	auto routeSpan = StructMainData::GetInstance().GetRouteInfo();
	auto ilkSpan = StructMainData::GetInstance().GetInterLockInfo();
	if (nRouteIdx < 0 || nRouteIdx >= (int)routeSpan.size() || nRouteIdx >= (int)ilkSpan.size())
		return _T("");
	const auto& rteItem = routeSpan[nRouteIdx];
	const auto& ilkItem = ilkSpan[nRouteIdx];
	CString strEquipList = _T("");
	CString strTmp;
	// 1. Max 현시 판별
	LPCTSTR pszMaxAspectName = _T("-------");
	switch (rteItem.MaxSignal & 0x0F)
	{
	case SIGNAL_PROCEED: pszMaxAspectName = _T("진행-G"); break;
	case SIGNAL_CAUTION: pszMaxAspectName = _T("주의-Y"); break;
	case SIGNAL_SLOW:    pszMaxAspectName = _T("감속-YG"); break;
	case SIGNAL_BOUND:   pszMaxAspectName = _T("경계-YY"); break;
	case SIGNAL_STOP:    pszMaxAspectName = _T("정지"); break;
	}
	strTmp.Format(_T("현시: Max:%02x(%s)"), (rteItem.MaxSignal & 0x0F), pszMaxAspectName);
	strEquipList += strTmp;
	// 2. Min 현시 판별
	LPCTSTR pszMinAspectName = _T("-------");
	switch (rteItem.MinSignal & 0x0F)
	{
	case SIGNAL_PROCEED: pszMinAspectName = _T("진행-G"); break;
	case SIGNAL_CAUTION: pszMinAspectName = _T("주의-Y"); break;
	case SIGNAL_SLOW:    pszMinAspectName = _T("감속-YG"); break;
	case SIGNAL_BOUND:   pszMinAspectName = _T("경계-YY"); break;
	case SIGNAL_STOP:    pszMinAspectName = _T("정지"); break;
	}
	strTmp.Format(_T(", 현시: Min:%02x(%s)"), (rteItem.MinSignal & 0x0F), pszMinAspectName);
	strEquipList += strTmp;
	// [추가] FrontLinkRteNo: 전방신호 연계진로(전방 신호기 현시에 따라 이 진로의 신호현시가 좌우됨)
	if (rteItem.FrontLinkRteNo > 0)
	{
		strTmp.Format(_T(" (전방연계진로:%d)"), rteItem.FrontLinkRteNo);
		strEquipList += strTmp;
	}
	// 3. 전방 신호기
	strEquipList += _T("  , 전방: ");
	if (rteItem.FrontSignalNo > 0)
	{
		CString strFrontSig = CommonUtil::GetDBNameByNumber(rteItem.FrontSignalNo, GetDBNameByNum::SignalIdx);
		strTmp.Format(_T("%s "), (LPCTSTR)strFrontSig);
		strEquipList += strTmp;
	}
	// 4. 후방 신호기
	strEquipList += _T("  , 후방: ");
	if (rteItem.RearSignalNo > 0)
	{
		CString strRearSig = CommonUtil::GetDBNameByNumber(rteItem.RearSignalNo, GetDBNameByNum::SignalIdx);
		strTmp.Format(_T("%s "), (LPCTSTR)strRearSig);
		strEquipList += strTmp;
	}
	// 5. 주신호 연결 여부
	// [구조체 변경] .Value 제거 (RouteFlag 는 필드가 바로 노출됨)
	if (IsBitSet(rteItem.RouteFlag.MainSig, 0))
	{
		strEquipList += _T("[주신호연결] ");
	}
	// 6. 구내폐색 진로 처리
	// [구조체 변경] .Value 제거 (Kind 는 필드가 바로 노출됨)
	if (IsBitSet(rteItem.Kind.IsBlock, 0))
	{
		if (rteItem.BlockSigRteNo[0] > 0)
		{
			strEquipList += _T(", 구내폐색: ");
			for (int i = 0; i < 6; i++)
			{
				if (0 == rteItem.BlockSigRteNo[i])
					continue;
				int targetIdx = rteItem.BlockSigRteNo[i] - 1;
				if (targetIdx >= 0 && targetIdx < (int)ilkSpan.size())
				{
					CString strIlkName(ilkSpan[targetIdx].Name);
					strTmp.Format(_T("%s "), (LPCTSTR)strIlkName);
					strEquipList += strTmp;
				}
			}
		}
	}
	// 7. 유도 진로
	if (rteItem.UdoRteNo > 0)
	{
		int targetIdx = rteItem.UdoRteNo - 1;
		if (targetIdx >= 0 && targetIdx < (int)ilkSpan.size())
		{
			CString strUdoName(ilkSpan[targetIdx].Name);
			strTmp.Format(_T(", 유도 진로: %s"), (LPCTSTR)strUdoName);
			strEquipList += strTmp;
		}
	}
	// 8. 일괄제어 신호기
	if (rteItem.TotRteSigNo[0] > 0)
	{
		strEquipList += _T(", 일괄제어 신호기: ");
		for (int i = 0; i < 4; i++)
		{
			if (0 == rteItem.TotRteSigNo[i])
				continue;
			CString strTotSig = CommonUtil::GetDBNameByNumber(rteItem.TotRteSigNo[i], GetDBNameByNum::SignalIdx);
			strTmp.Format(_T("%s "), (LPCTSTR)strTotSig);
			strEquipList += strTmp;
		}
	}
	// [추가] 9. 자동 진로 제어 (AutoRteCtrl)
	if (rteItem.AutoRteCtrl.AutoMode > 0)
	{
		strTmp.Format(_T(", 자동진로제어[모드:%d"), rteItem.AutoRteCtrl.AutoMode);
		strEquipList += strTmp;
		if (rteItem.AutoRteCtrl.PreRteNo > 0)
		{
			strTmp.Format(_T(",선행진로:%d"), rteItem.AutoRteCtrl.PreRteNo);
			strEquipList += strTmp;
		}
		for (int i = 0; i < 3; i++)
		{
			const auto& cond = rteItem.AutoRteCtrl.CtrlCond[i];
			if (cond.CondTblNo == 0 && cond.CondTblKind == 0) continue;
			strTmp.Format(_T(",조건%d(%c-%d:상태%d)"), i + 1, cond.CondTblKind, cond.CondTblNo, cond.CondTblState);
			strEquipList += strTmp;
		}
		if (rteItem.AutoRteCtrl.CtrlTime > 0)
		{
			// [수정] 조건성립 후 진로제어 동작까지의 지연시간(초)
			strTmp.Format(_T(",지연시간:%d초"), rteItem.AutoRteCtrl.CtrlTime);
			strEquipList += strTmp;
		}
		strEquipList += _T("]");
	}
	return strEquipList + _T("\r\n");
}
CString InterlockGridInfo::GetRouteInfo_SigStop(const InterLockInfoType& item)
{
	CString strEquipOn = _T("");   // 4: 여자일 때 신호 정지
	CString strEquipOff = _T("");  // 1: 낙하일 때 신호 정지
	CString strCallOff = _T("");   // 2: 낙하일 때 무유도만 정지 (입환)
	CString strCallOn = _T("");    // 3: 낙하일 때 유도만 현시 (주신호)
	CString strNotNor = _T("");    // 5: 정위가 아닐 때 신호 정지
	CString strNotRev = _T("");    // 6: 반위가 아닐 때 신호 정지
	CString strLock = _T("");      // 7: 쇄정상태일 때 신호 정지
	for (int i = 0; i < NO_OF_LOCK_EQUIP; i++)
	{
		if (item.EquipLock[i].TblNo == 0) break;
		// 1. Kind를 GetDBNameByNum으로 변환
		CommonUtil::GetDBNameByNum dbNum = GetDBNameNumFromKind(item.EquipLock[i].Kind);
		if ((int)dbNum == 0) continue;
		// 2. 설비 이름 가져오기
		CString strEquipName = CommonUtil::GetDBNameByNumber(static_cast<Byte_t>(item.EquipLock[i].TblNo), dbNum);
		if (strEquipName.IsEmpty() || strEquipName == _T("-")) continue;
		CString temp;
		temp.Format(_T(" %s"), (LPCTSTR)strEquipName);
		// 3. stateCond 값에 따라 정확한 조건별 변수에만 누적
		switch (item.EquipLock[i].stateCond)
		{
		case 1: strEquipOff += temp; break;
		case 2: strCallOff += temp; break;
		case 3: strCallOn += temp; break;
		case 4: strEquipOn += temp; break;
		case 5: strNotNor += temp; break;
		case 6: strNotRev += temp; break;
		case 7: strLock += temp; break;
		}
	}
	// 4. 최종 문자열 조합 (데이터가 존재하는 경우에만 줄바꿈과 함께 추가)
	CString strResult = _T("");
	if (!strEquipOn.IsEmpty())  strResult += _T("여자일 때 신호 정지 :") + strEquipOn + _T("\r\n");
	if (!strEquipOff.IsEmpty()) strResult += _T("낙하일 때 신호 정지 :") + strEquipOff + _T("\r\n");
	if (!strCallOff.IsEmpty())  strResult += _T("낙하일 때 무유도만 정지 (입환) :") + strCallOff + _T("\r\n");
	if (!strCallOn.IsEmpty())   strResult += _T("낙하일 때 유도만 현시 (주신호) :") + strCallOn + _T("\r\n");
	if (!strNotNor.IsEmpty())   strResult += _T("정위가 아닐 때 신호 정지 :") + strNotNor + _T("\r\n");
	if (!strNotRev.IsEmpty())   strResult += _T("반위가 아닐 때 신호 정지 :") + strNotRev + _T("\r\n");
	if (!strLock.IsEmpty())     strResult += _T("쇄정상태일 때 신호 정지 :") + strLock + _T("\r\n");
	return strResult;
}
void InterlockGridInfo::loadInterLockData()
{
	auto interLockData = StructMainData::GetInstance().GetInterLockInfo();
	auto stationData = StructMainData::GetInstance().GetStationInfo();
	CBCGPGridRow* pSTRow = CreateRow(GetColumnCount());
	pSTRow->SetLinesNumber(11);
	pSTRow->GetItem(1)->SetValue(_T("역 정보"));
	pSTRow->GetItem(2)->SetMultiline(TRUE);
	CString stationTxt;
	//  역 기본 번호 및 장치 수
	stationTxt.AppendFormat(_T("역 번호 : [%d], 궤도 수 : %d, 신호기 수 : %d, 선로전환기 수 : %d, 진로 수 : %d, 건널목 수 : %d \r\n"),
		stationData.StationNo, stationData.NoOfTrack, stationData.NoOfSignal, stationData.NoOfSwitch, stationData.NoOfRouteExt, stationData.NoOfLevelCross);
	// [구조체 변경] NoOfSdCard(PDO Card 수)는 삭제됨. PDOCARDTABLE이 신호기 카드(NoOfModuleSig)/
	//              선로전환기 카드(NoOfModuleSwh)로 분리된 것에 맞춰 표시도 분리함
	stationTxt.AppendFormat(_T("입력 카드 수 : [%d], 출력 카드 수 : [%d], 신호기 카드 수 : [%d], 선로전환기 카드 수 : [%d], 폐색 수 : [%d], Rack 수 : [%d], Heat 수 : [%d] \r\n"),
		stationData.NoOfInCard, stationData.NoOfOutCard, stationData.NoOfModuleSig, stationData.NoOfModuleSwh, stationData.NoOfBlock, stationData.NoOfRack, stationData.NoOfHeat);
	stationTxt.AppendFormat(_T("연동논리부 정류기 수 : [%d], AF 정류기 수 : [%d], 계전기랙 정류기 수 : [%d], ATO/TWC 정류기 수 : [%d], 속도코드 정류기 수 : [%d] \r\n"),
		stationData.AcrCntEis, stationData.AcrCntAF, stationData.AcrCntRelay, stationData.AcrCntAto, stationData.AcrCntTs);
	//  신호기 고장검지 회선 (SigLmrKind)
	// [구조체 변경] CntLdtsDIM / CntLdtsDOM 은 현재 StationInfoType 에 없는 필드라 제외함
	stationTxt.AppendFormat(_T("신호기 고장검지 1회선 : [%d] \r\n"), stationData.StationEquip.SigLmrKind);
	//  추가 역 번호 정보 (값이 존재하는 경우)
	if (stationData.LctcStnNo > 0)
	{
		stationTxt.AppendFormat(_T("집중화 역 번호 : [%d] \r\n"), stationData.LctcStnNo);
	}
	if (stationData.RbcStnNo > 0)
	{
		stationTxt.AppendFormat(_T("RBC 역 번호 : [%d] \r\n"), stationData.RbcStnNo);
	}
	if (stationData.LdtsNo > 0)
	{
		stationTxt.AppendFormat(_T("LDTS 번호 : [%d] \r\n"), stationData.LdtsNo);
	}
	// 전차선 절연구간, 지장물, CPT 수
	// [추가] NoOfSO(임시속도 수), NoOfSTL(출발반응등 수)
	if (stationData.NoOfDeadSection > 0 || stationData.NoOfFallLock > 0 || stationData.NoOfCpt > 0
		|| stationData.NoOfSO > 0 || stationData.NoOfSTL > 0)
	{
		stationTxt.AppendFormat(_T("전차선 절연구간 수 : [%d], 지장물 수 : [%d], 열차진입 방호장치(CPT) 수 : [%d], 임시속도(Slow Order) 수 : [%d], 출발반응등 수 : [%d] \r\n"),
			stationData.NoOfDeadSection, stationData.NoOfFallLock, stationData.NoOfCpt, stationData.NoOfSO, stationData.NoOfSTL);
	}
	// 모드 및 설정 상태 (네임스페이스 비트 연산 적용)
	// [구조체 변경] .Value 제거 (StationInfo/StationEquip 는 필드가 바로 노출됨)
	// [구조체 변경] IsCtcStd / IsCtcStd2 / LocalCTC / LCtcMain 은 현재 StationInfoType 에 없는
	//              필드라 제외함
	CString modeTxt = _T("모드 정보: ");
	bool bModeAdded = false;
	if (stationData.StationInfo.IsCtc & StatuonInfoType::StatoinValue::ISCTC) {
		modeTxt += _T("CTC "); bModeAdded = true;
	}
	if (stationData.StationInfo.IsRcCtrl & StatuonInfoType::StatoinValue::ISRCCTRL) {
		modeTxt += _T("RC "); bModeAdded = true;
	}
	if (stationData.StationEquip.EtcEquip & StatuonInfoType::StationEquipValue::HEATER_PRE_REVISION_OP) {
		modeTxt += _T("히터동작(개정이전) "); bModeAdded = true;
	}
	if (stationData.StationInfo.IsControlLC & StatuonInfoType::StatoinValue::ISCONTROLLC) {
		modeTxt += _T("건널목 출력 제어 "); bModeAdded = true;
	}
	// [추가] StationInfo/StationEquip 의 나머지 필드
	// (전용 비트 상수가 확인되지 않아 IsBitSet(bit0) 로 판별)
	if (IsBitSet(stationData.StationInfo.TniUse, 0)) {
		modeTxt += _T("열번인식기 ");     bModeAdded = true;
	}
	if (IsBitSet(stationData.StationInfo.LmcUse, 0)) {
		modeTxt += _T("체류보수 ");       bModeAdded = true;
	}
	if (IsBitSet(stationData.StationInfo.SupportUse, 0)) {
		modeTxt += _T("전기설비 기술지원 시스템 "); bModeAdded = true;
	}
	if (IsBitSet(stationData.StationInfo.RbcUse, 0)) {
		modeTxt += _T("RBC통신 ");        bModeAdded = true;
	}
	if (IsBitSet(stationData.StationInfo.CptStn, 0)) {
		modeTxt += _T("CPT설치 ");        bModeAdded = true;
	}
	if (IsBitSet(stationData.StationInfo.StnKind, 0)) {
		modeTxt += _T("연동역 ");         bModeAdded = true;
	}
	if (IsBitSet(stationData.StationEquip.IsEIsFuse, 0)) {
		modeTxt += _T("연동논리부FUSE사용 "); bModeAdded = true;
	}
	// [수정] IsUpsAc 는 "AC 사용/미사용"이 아니라, 두 값 모두 "UPS AC전원 있음" 상태에서
	//        여자/낙하 정상·장애 판정 논리(계전기 극성)만 반대로 뒤집는 설정 비트임
	//        bit0=0(default) : 여자시 정상, 낙하시 장애 / bit0=1 : 여자시 장애, 낙하시 정상
	if (IsBitSet(stationData.StationEquip.IsUpsAc, 0)) {
		modeTxt += _T("UPS AC전원 정상판정 논리반전(여자시 장애) "); bModeAdded = true;
	}
	if (bModeAdded)
	{
		modeTxt += _T("\r");
		stationTxt.Append(modeTxt);
	}
	// TrkOption 관련 옵션 메시지 (비트 연산 적용)
	// [구조체 변경] .Value 제거
	if (stationData.StationInfo.TrkOption & StatuonInfoType::StatoinValue::TRKOPTION)
	{
		stationTxt.AppendFormat(_T("TrkOption : 착점궤도 등 모든궤도에 대해 비상해정 취급 후 해정 [%d] \r\n"), stationData.StationInfo.TrkOption);
	}

	// [추가] 비상정지 설비 정보 (EmgStop)
	for (int i = 0; i < MAX_EMG_STOP; i++)
	{
		for (int dir = 0; dir < 2; dir++)
		{
			const auto& emg = stationData.EmgStop[i][dir];
			if (emg.Name[0] == 0 || emg.Name[0] == 0xFF) continue;

			CString strEmg;
			strEmg.Format(_T("비상정지[%d-%s] : %s"), i + 1, dir == 0 ? _T("상") : _T("하"), (LPCTSTR)GetSafeString(emg.Name));

			CString strArr;
			for (int k = 0; k < MAX_EMG_STOP_SIGNAL; k++)
			{
				if (emg.ArrSignal[k] == 0) continue;
				strArr.AppendFormat(_T("%s "), (LPCTSTR)CommonUtil::GetDBNameByNumber(emg.ArrSignal[k], GetDBNameByNum::SignalIdx));
			}
			if (!strArr.IsEmpty()) strEmg += _T(", 장내:") + strArr;

			CString strDep;
			for (int k = 0; k < MAX_EMG_STOP_SIGNAL; k++)
			{
				if (emg.DepSignal[k] == 0) continue;
				strDep.AppendFormat(_T("%s "), (LPCTSTR)CommonUtil::GetDBNameByNumber(emg.DepSignal[k], GetDBNameByNum::SignalIdx));
			}
			if (!strDep.IsEmpty()) strEmg += _T(", 출발:") + strDep;

			CString strTrk;
			for (int k = 0; k < MAX_EMG_STOP_TRACK; k++)
			{
				if (emg.TrackNo[k] == 0) continue;
				strTrk.AppendFormat(_T("%s "), (LPCTSTR)CommonUtil::GetDBNameByNumber(emg.TrackNo[k], GetDBNameByNum::TrackIdx));
			}
			if (!strTrk.IsEmpty()) strEmg += _T(", 궤도:") + strTrk;

			if (IsBitSet(emg.EmgOut, 0)) strEmg += _T(" [정상출력]");
			if (IsBitSet(emg.EmgOut, 1)) strEmg += _T(" [상하선 겸용출력]");

			stationTxt += strEmg + _T("\r\n");
		}
	}

	// [추가] 설비 출력 정보 (EquipOut)
	CString strEquipOut;
	for (int i = 0; i < MAX_OUT_HEATER; i++)
	{
		if (IsBitSet(stationData.EquipOut.OutHeater[i], 0))
			strEquipOut.AppendFormat(_T("Heater%d "), i + 1);
	}
	for (int i = 0; i < MAX_OUT_EL; i++)
	{
		if (IsBitSet(stationData.EquipOut.OutEL[i], 0))
			strEquipOut.AppendFormat(_T("EL%d "), i + 1);
	}
	if (IsBitSet(stationData.EquipOut.OutCtc, 0))   strEquipOut += _T("CTC ");
	if (IsBitSet(stationData.EquipOut.OutLocal, 0)) strEquipOut += _T("Local ");
	if (IsBitSet(stationData.EquipOut.OutMain, 0))  strEquipOut += _T("주공급 ");
	if (!strEquipOut.IsEmpty())
	{
		stationTxt.AppendFormat(_T("설비 출력 : %s\r\n"), (LPCTSTR)strEquipOut);
	}

	pSTRow->GetItem(2)->SetValue((LPCTSTR)stationTxt);
	AddRow(pSTRow, FALSE);
	int nRouteCount = (int)interLockData.size();
	long nRow = 0;
	for (int nIdx = 0; nIdx < nRouteCount; nIdx++)
	{
		// 유효하지 않은 데이터 건너뛰기
		if (interLockData[nIdx].Name[0] == 0x00 || (unsigned char)interLockData[nIdx].Name[0] == 0xFF)
		{
			continue;
		}
		// 1부터 시작하는 순번 (원본의 nIdxCnt 대응)
		WORD nIdxCnt = nIdx + 1;
		long nCol = 0;
		nRow++;
		// 행 생성 (3열 구조: 번호, 진로명, 상세내용)
		CBCGPGridRow* pRow = CreateRow(GetColumnCount());
		if (pRow == nullptr) continue;
		pRow->SetLinesNumber(20);
		//===============================================================
		// 0열: 번호
		CString tmpStr;
		tmpStr.Format(_T("%d"), nIdxCnt);
		pRow->GetItem(nCol++)->SetValue((LPCTSTR)tmpStr);
		//===============================================================
		// 1열: 진로 이름
		CString strName(interLockData[nIdx].Name);
		pRow->GetItem(nCol++)->SetValue((LPCTSTR)strName);
		//===============================================================
		// 2열: 상세 내용들
		CString strData = _T("");
		char szIlkInfo[2048];
		// 예: 신호기 및 출발/도착 정보
		memset(szIlkInfo, 0x00, sizeof(szIlkInfo));
		// GetRouteInfo_SignalTrack 에 해당하는 로직 처리
		strData += GetRouteInfoStr(interLockData[nIdx]);
		// 대항진로 정보 추가 (StructMainData의 RouteInfo 활용)
		CString strOpp = GetOppositeRouteStr(nIdxCnt);
		if (!strOpp.IsEmpty()) {
			strData += strOpp;
		}
		CString strRoutKind = GetRouteKindStr(nIdxCnt);
		if (!strRoutKind.IsEmpty()) {
			strData += strRoutKind;
		}
		CString strTrackLock = GetRouteTrackLockStr(interLockData[nIdx], nIdxCnt);
		if (!strTrackLock.IsEmpty()) {
			strData += strTrackLock;
		}
		strData += GetCountStr(interLockData[nIdx])
			+ GetRouteInfo_SignalTrack(nIdxCnt)
			+ GetSwitchLockStr(interLockData[nIdx])
			+ GetRouteLockStr(interLockData[nIdx])
			+ GetEquipLockStr(interLockData[nIdx])
			+ GetExceptionStr(interLockData[nIdx])
			+ GetMiscArrayStr(interLockData[nIdx])
			+ GetUhoInfoStr(interLockData[nIdx])
			+ GetSpecialSwitchStr(interLockData[nIdx])
			+ GetSpcStateStr(interLockData[nIdx])
			+ GetRouteInfo_SigStop(interLockData[nIdx]);
		strData.TrimRight(_T("\r\n"));
		CBCGPGridItem* pDataItem = pRow->GetItem(2);
		pDataItem->SetValue((LPCTSTR)strData);
		pDataItem->SetMultiline(TRUE);
		AddRow(pRow, FALSE);
	}
}