#include "stdafx.h"
#include "StructMainData.h"
#include "DeviceGridInfo.h"
#include "CommonUtils.h"
using namespace CommonUtil;
BEGIN_MESSAGE_MAP(DeviceGridInfo, CBCGPGridCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()
DeviceGridInfo::DeviceGridInfo()
{
	EnableColumnsResizeInClientArea(FALSE);
	EnableDragHeaderItems(FALSE);
	//EnableHeader(FALSE);
	EnableHeader(TRUE, 0);
	EnableRowHeader(FALSE);
	SetRowMarker(FALSE);
	//EnableLineNumbers(TRUE);
	SetReadOnly(TRUE);
	EnableMarkSortedColumn(FALSE);
	EnableTextOverflowing(FALSE);
	m_bTextOverflowing = FALSE;
}
DeviceGridInfo::~DeviceGridInfo()
{
	CBCGPGridCtrl::OnDestroy();
}
// BlkKind 바이트 검사 함수
CString DeviceGridInfo::GetBlockKindFromByte(Byte_t byte)
{
	int value = static_cast<int>(byte);
	if (value < 1 || value > 16) {
		return _T("잘못된 범위의 데이터");
	}
	BlockKind kind = static_cast<BlockKind>(value);
	switch (kind)
	{

	case BlockKind::DoubleAuto_5Aspect:        return _T("복선자동");
	case BlockKind::SingleAuto_3Aspect:        return _T("단선자동");
	case BlockKind::DoubleInterlocking:        return _T("복선연동");
	case BlockKind::SingleInterlocking:        return _T("단선연동");
	case BlockKind::TokenBlock:                return _T("통표 폐색");
	case BlockKind::SubwayBlock:               return _T("지하철 폐색");
	case BlockKind::DaeyaBlock:                return _T("대야 폐색");
	case BlockKind::UiwangBlock:               return _T("의왕 폐색");
	case BlockKind::TriangleBlock:             return _T("삼각선 폐색");
	case BlockKind::Cheongnyangni_Mangu:       return _T("청량리 폐색");
	case BlockKind::BiDirectional:             return _T("양방향 폐색\r\n(정방향출발 && 역방향장내)");
	case BlockKind::BiDirectionalRev:          return _T("양방향 폐색\r\n(정방향장내 && 역방향출발)");
	case BlockKind::HighSpeedBlock:            return _T("고속선 폐색");
	case BlockKind::BiDirectionalEtc:          return _T("양방향 폐색 (14 - 세부 종류 미확인)");
	default:                                   return _T("미정의 폐색");
	}
}
// [수정] item.BlockAspect는 enum이 아니라 "현시 개수"(2/3/4/5)를 담은 순수 숫자값이고,
// 실제 신호 입력 이름(Y / BR,DR / YY,Y,YG 등)은 BlkKind와 조합해야 정해짐 - 이 매핑은
// EI_IP_DBStruct_Typedef.h의 BlockAspect 필드 원본 주석("현시 수에 따른 폐색 입력") 표를
// 그대로 반영한 것. 구조체에 현시별 입력 이름이 정의되지 않은 폐색 종류(복선연동/단선연동/
// 통표/지하철/대야/삼각선/청량리/양방향(11)/고속선/14 등)는 "n현시"만 표시.
CString DeviceGridInfo::GetBlcokAspectFromByte(Byte_t blkKind, Byte_t aspectCount)
{
	switch (static_cast<BlockKind>(blkKind))
	{
	case BlockKind::DoubleAuto_5Aspect: // 복선자동
		if (aspectCount == 3) return _T("복선자동 3현시 (Y)");
		if (aspectCount == 5) return _T("복선자동 5현시 (YY, Y, YG)");
		break;
	case BlockKind::SingleAuto_3Aspect: // 단선자동
		if (aspectCount == 3) return _T("단선자동 3현시 (BR, DR)");
		if (aspectCount == 5) return _T("단선자동 5현시 (BR, YY, Y, YG)"); // DR 없음
		break;
	case BlockKind::UiwangBlock: // 의왕 폐색
		if (aspectCount == 3) return _T("의왕 폐색 3현시 (HR, BHR, TR, TPSR, eHR)");
		if (aspectCount == 4 || aspectCount == 5) return _T("의왕 폐색 4,5현시 (HR, BHR, TR, TPSR)");
		break;
	case BlockKind::BiDirectionalRev: // 양방향 폐색 (정방향장내 && 역방향출발)
		if (aspectCount == 3) return _T("양방향 폐색 3현시 (BR, DR)");
		if (aspectCount == 5) return _T("양방향 폐색 5현시 (BR)"); // DR 없음
		break;
	default:
		break;
	}
	// 위에서 처리되지 않은 조합(구조체에 현시별 입력 이름이 정의되지 않은 폐색 종류이거나,
	// 정의된 폐색 종류라도 문서화된 현시 개수와 다른 값)은 현시 개수만이라도 표시
	if (aspectCount >= 2 && aspectCount <= 5)
	{
		CString str;
		str.Format(_T("%d현시"), aspectCount);
		return str;
	}
	return _T("잘못된 현시 데이터");
}
int DeviceGridInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	std::vector<ColumnInfo> deviceColumnInfo = GridColumnDefine::GetDeviceColumnInfo();
	for (int i = 0; i < deviceColumnInfo.size(); i++) {
		InsertColumn(i, deviceColumnInfo[i].columnName, deviceColumnInfo[i].columnWidth);
		if (!(i == deviceColumnInfo.size() - 1)) {
			SetHeaderAlign(i, HDF_CENTER);
			SetColumnAlign(i, HDF_CENTER);
		}
	}
	SetHeaderAlign(deviceColumnInfo.size(), HDF_CENTER);
	//InsertGroupColumn(0, 0);
	//LoadAllData();
	AdjustLayout();
	return 0;
}
// ---------------------------------------------------------
// 데이터 로드 (모든 데이터)
// ---------------------------------------------------------
void DeviceGridInfo::LoadAllData()
{
	// ==========================================
	// 1. 폐색 데이터 로딩
	// ==========================================
	auto blockList = StructMainData::GetInstance().GetBlockInfo();
	for (const auto& item : blockList)
	{
		if (item.Name[0] == 0 || item.Name[0] == 0xFF || item.BlkKind == 0 || item.BlkKind > 16)
		{
			continue; // 데이터가 비어있거나 이상하면 이 행은 건너뜁니다.
		}
		CBCGPGridRow* pRow = CreateRow(GetColumnCount());
		pRow->SetLinesNumber(13);
		//그룹화 기준 명칭  폐색 트리 그룹 생성
		pRow->GetItem(0)->SetValue(_T("폐색"));
		pRow->GetItem(1)->SetValue((LPCTSTR)GetSafeString(item.Name, 20));
		CString KindASpect = (_T(""));
		KindASpect.AppendFormat(GetBlockKindFromByte(item.BlkKind) + _T(" / ") + GetBlcokAspectFromByte(item.BlkKind, item.BlockAspect)); //GetBlockAspectFromByte 폐색 종류및 현시수반환
		// [2번 컬럼] 구분 (BlkKind)
		pRow->GetItem(2)->SetValue((LPCTSTR)KindASpect);
		pRow->GetItem(2)->SetMultiline(TRUE);
		CString strKind = _T("");

		pRow->GetItem(3)->SetValue((LPCTSTR)strKind);
		// [3번 컬럼] 종합 상세 데이터
		// 새롭게 작성한 분해 함수를 통해 궤도, 타이머, 포트 등 모든 정보를 문자열로 받음
		CString strDetails = GetBlockInfoString(const_cast<BlockTagInfoType*>(&item));
		pRow->GetItem(4)->SetValue((LPCTSTR)strDetails);
		pRow->GetItem(4)->SetMultiline(TRUE); // 멀티라인 텍스트 활성화
		AddRow(pRow, FALSE);
	}
	//==========================================
	//2. 건널목 데이터 로딩
	//==========================================
	auto crossingList = StructMainData::GetInstance().GetCossInfo();
	for (const auto& item : crossingList)
	{
		if (item.Name[0] == 0 || item.Name[0] == 0xFF) {
			continue;
		}
		CBCGPGridRow* pRow = CreateRow(GetColumnCount());
		pRow->GetItem(0)->SetValue(_T("건널목")); // "건널목" 그룹으로 묶임
		pRow->GetItem(1)->SetValue((LPCTSTR)GetSafeString(item.Name, 20));
		AddRow(pRow, FALSE);
	}
	//==========================================
	//3.제어 건널목 데이터 로딩
	//==========================================
	auto LClist = StructMainData::GetInstance().GetLCINFO();
	for (const auto& item : LClist) {
		if (item.Name1[0] == 0 || item.Name1[0] == 0xFF) {
			continue;
		}
		CBCGPGridRow* pRow = CreateRow(GetColumnCount());
		pRow->SetLinesNumber(3);
		pRow->GetItem(0)->SetValue(_T("제어 건널목"));
		CString NameCombine = (GetSafeString(item.Name1, 10) + GetSafeString(item.Name2, 4));
		pRow->GetItem(1)->SetValue((LPCTSTR)NameCombine);
		CString strDetails = GetLCListInfoString(const_cast<LC_CTRL_INFO_TYPE*>(&item));
		pRow->GetItem(4)->SetValue((LPCTSTR)strDetails);
		pRow->GetItem(4)->SetMultiline(TRUE); // 멀티라인 텍스트 활성화
		AddRow(pRow, FALSE);
	}
	// ==========================================
	// 4. 히터 데이터 로딩
	// ==========================================
	auto heaterList = StructMainData::GetInstance().GetHeatInfo();
	CString dbgTotalHeater;
	dbgTotalHeater.Format(_T("[LoadAllData - Heater] 총 히터 데이터 개수: %d\n"), (int)heaterList.size());
	OutputDebugString(dbgTotalHeater);
	int heaterIndex = 0;
	for (const auto& item : heaterList) {
		if (item.szHeatName[0] == 0 || item.szHeatName[0] == 0xFF)
		{
			CString dbgSkip;
			dbgSkip.Format(_T("[LoadAllData - Heater] %d번째 히터: 이름이 비어있거나 0xFF이므로 스킵합니다.\n"), heaterIndex);
			OutputDebugString(dbgSkip);
			heaterIndex++;
			continue;
		}
		CString heatName = GetSafeString(item.szHeatName, 10);
		CString dbgStart;
		dbgStart.Format(_T("[LoadAllData - Heater] 처리 중인 히터 이름: [%s] (인덱스: %d)\n"), (LPCTSTR)heatName, heaterIndex);
		OutputDebugString(dbgStart);
		CBCGPGridRow* pRow = CreateRow(GetColumnCount());
		pRow->SetLinesNumber(5);
		pRow->GetItem(0)->SetValue(_T("히터"));
		pRow->GetItem(1)->SetValue((LPCTSTR)heatName);
		CString strResult;
		int validAlmCount = 0;
		for (int i = 0; i < MAX_CNT_HEATER_ALM; i++)
		{
			// 알람 메시지가 비어있지 않은 경우에만 표시
			CString strMsg = GetSafeString(item.AlmData[i].szAlmMsg, 20);
			if (!strMsg.IsEmpty())
			{
				strResult.AppendFormat(_T("알람[%d] : %s\r\n"), i + 1, (LPCTSTR)strMsg);
				validAlmCount++;
			}
		}
		// 알람 개수 및 최종 문자열 상태 로그 출력
		CString dbgAlm;
		dbgAlm.Format(_T("[LoadAllData - Heater] -> [%s] 유효 알람 개수: %d, 생성된 알람 문자열 길이: %d\n"),
			(LPCTSTR)heatName, validAlmCount, strResult.GetLength());
		OutputDebugString(dbgAlm);
		if (!strResult.IsEmpty())
		{
			CString dbgAlmContents;
			dbgAlmContents.Format(_T("[LoadAllData - Heater] -> 알람 내용:\n%s\n"), (LPCTSTR)strResult);
			OutputDebugString(dbgAlmContents);
		}
		pRow->GetItem(4)->SetValue((LPCTSTR)strResult);
		pRow->GetItem(4)->SetMultiline(TRUE); // 멀티라인 텍스트 활성화
		AddRow(pRow, FALSE);
		heaterIndex++;
	}
	//==========================================
	//5. 지장물 데이터 로딩
	//==========================================
	auto fallLockList = StructMainData::GetInstance().GetFallLockInfo();
	for (const auto& item : fallLockList) {
		if (item.Name[0] == 0 || item.Name[0] == 0xFF) {
			continue;
		}
		CBCGPGridRow* pRow = CreateRow(GetColumnCount());
		pRow->SetLinesNumber(2);
		pRow->GetItem(0)->SetValue(_T("지장물"));
		pRow->GetItem(1)->SetValue((LPCTSTR)GetSafeString(item.Name, 20));
		CString strResult = _T("");
		strResult.AppendFormat(_T("낙석 : ") + GetSafeString(item.FallLock.Name, 20) + _T("\r\n"));
		strResult.AppendFormat(_T("보호 : ") + GetSafeString(item.Proc.Name1, 15) + _T("\r\n"));
		strResult.AppendFormat(GetSafeString(item.Proc.Name2, 15));
		//switch (item.type)
		//{
		//case 1:
		//	strResult.AppendFormat(_T("[타입] : 낙석\r\n"));
		//	break;
		//case 2:
		//	strResult.AppendFormat(_T("[타입] : 보호\r\n"));
		//	break;
		//default:
		//	break;
		//}
		pRow->GetItem(4)->SetValue((LPCTSTR)strResult);
		pRow->GetItem(4)->SetMultiline(TRUE); // 멀티라인 텍스트 활성화
		AddRow(pRow, FALSE);
	}
	//==========================================
	//6. 절연구간 데이터 로딩
	//==========================================
	auto deadSecList = StructMainData::GetInstance().GetDeadSectionInfo();
	for (const auto& item : deadSecList) {
		if (item.Name[0] == 0 || item.Name[0] == 0xFF) {
			continue;
		}
		CBCGPGridRow* pRow = CreateRow(GetColumnCount());
		pRow->SetLinesNumber(2);
		pRow->GetItem(0)->SetValue(_T("절연구간"));
		pRow->GetItem(1)->SetValue((LPCTSTR)GetSafeString(item.Name, 20));
		CString strResult = _T("");
		strResult.AppendFormat(_T("[주계] 1계(%s)"), GetSafeString(item.Unit1.Name, 10));
		strResult.AppendFormat(_T("[부계] 2계(%s)"), GetSafeString(item.Unit2.Name, 10));
		pRow->GetItem(4)->SetValue((LPCTSTR)strResult);
		pRow->GetItem(4)->SetMultiline(TRUE); // 멀티라인 텍스트 활성화
		AddRow(pRow, FALSE);
	}
	//==========================================
	//7. 기타 고장 데이터 로딩
	//==========================================
	auto faultList = StructMainData::GetInstance().GetFaultInfo();
	for (const auto& item : faultList) {
		if (item.Name[0] == 0 || item.Name[0] == 0xFF) {
			continue;
		}
		CBCGPGridRow* pRow = CreateRow(GetColumnCount());
		pRow->SetLinesNumber(2);
		pRow->GetItem(0)->SetValue(_T("기타 고장"));
		pRow->GetItem(1)->SetValue((LPCTSTR)GetSafeString(item.Name, 20));
		CString strResult = _T("");
		pRow->GetItem(4)->SetValue((LPCTSTR)strResult);
		AddRow(pRow, FALSE);
	}
	//==========================================
	//8. 임시속도 데이터 로딩
	//==========================================
	auto slowOrderList = StructMainData::GetInstance().GetSlowOrderInfo();
	for (const auto& item : slowOrderList) {
		if (item.Name[0] == 0 || item.Name[0] == 0xFF) {
			continue;
		}
		CBCGPGridRow* pRow = CreateRow(GetColumnCount());
		pRow->SetLinesNumber(3);
		pRow->GetItem(0)->SetValue(_T("임시속도"));
		pRow->GetItem(1)->SetValue((LPCTSTR)GetSafeString(item.Name, 20));

		// [2번 컬럼] 구분 - Type bit0=1 : 인접역 임시속도
		CString strKind = IsBitSet(item.Type, 0) ? _T("인접역 임시속도") : _T("자체 임시속도");
		pRow->GetItem(2)->SetValue((LPCTSTR)strKind);

		CString soTracks;
		for (int i = 0; i < MAX_SLOW_ORDER_TRACK; i++)
		{
			Byte_t trkNo = item.TrackNo[i];
			if (trkNo == 0 || trkNo == 0xFF) continue;
			if (!soTracks.IsEmpty()) soTracks += _T(", ");
			soTracks += GetDBNameByNumber(trkNo, TrackIdx);
		}
		CString soSignals;
		for (int j = 0; j < MAX_SLOW_ORDER_SIGNAL; j++)
		{
			Byte_t sigNo = item.SIgnalNo[j];
			if (sigNo == 0 || sigNo == 0xFF) continue;
			if (!soSignals.IsEmpty()) soSignals += _T(", ");
			soSignals += GetDBNameByNumber(sigNo, SignalIdx);
		}
		CString strResult;
		if (!soTracks.IsEmpty()) strResult.AppendFormat(_T("관련 궤도 : %s\r\n"), (LPCTSTR)soTracks);
		if (!soSignals.IsEmpty()) strResult.AppendFormat(_T("관련 신호기 : %s"), (LPCTSTR)soSignals);
		pRow->GetItem(4)->SetValue((LPCTSTR)strResult);
		pRow->GetItem(4)->SetMultiline(TRUE); // 멀티라인 텍스트 활성화
		AddRow(pRow, FALSE);
	}
	//==========================================
	//9. 끌림 감시장치 데이터 로딩
	//==========================================
	// [주의] AttractionInfoType에는 Name 외 실제 데이터 필드가 없음(Spare만 존재)
	auto attractionList = StructMainData::GetInstance().GetAttractionInfo();
	for (const auto& item : attractionList) {
		if (item.Name[0] == 0 || item.Name[0] == 0xFF) {
			continue;
		}
		CBCGPGridRow* pRow = CreateRow(GetColumnCount());
		pRow->GetItem(0)->SetValue(_T("끌림 감시장치"));
		pRow->GetItem(1)->SetValue((LPCTSTR)GetSafeString(item.Name, 20));
		AddRow(pRow, FALSE);
	}
}
// ---------------------------------------------------------
// 3. 유틸리티 함수들 (데이터 문자열 변환용)
// 널(Null) 문자가 없는 바이트 배열을 안전하게 CString으로 변환해주는 헬퍼 함수
//CString DeviceGridInfo::GetSafeString(const unsigned char* pRawData, int nMaxLength)
//{
//	if (pRawData == nullptr || nMaxLength <= 0)
//		return _T("");
//
//	// unsigned char*를 const char*로 명시적 변환
//	const char* pCharData = reinterpret_cast<const char*>(pRawData);
//
//	// CStringA를 사용하여 nMaxLength만큼만 읽어들임 (바이너리 데이터 안전 처리)
//	CStringA strAnsi(pCharData, nMaxLength);
//
//	// 유니코드 프로젝트라면 자동으로 유니코드로 변환됨
//	return CString(strAnsi);
//}
CString DeviceGridInfo::FormatIOPosition(IO_Position pos)
{
	CString str;
	str.Format(_T("Rack-%02d, Slot-%02d, Point-%02d"), pos.Chassis, pos.CardNo, pos.PortNo);
	return str;
}
CString DeviceGridInfo::GetLCListInfoString(LC_CTRL_INFO_TYPE* pData)
{
	if (pData == nullptr) return _T("");
	CString strTotal, strTemp;
	strTemp.Format(_T("[입력]%s\n"),
		FormatIOPosition(pData->LC_Inp)
	);
	strTotal += strTemp;
	strTemp.Format(_T("[출력]%s"),
		FormatIOPosition(pData->LC_Out)
	);
	strTotal += strTemp;
	return strTotal;
}
void DeviceGridInfo::UpdateDeviceData()
{
	RemoveAll();
	LoadAllData();
	AdjustLayout();
}
void DeviceGridInfo::PrintGrid(LPCTSTR lpszDocName)
{
}
void DeviceGridInfo::PrintPreviewGrid()
{
}
CString DeviceGridInfo::GetBlockInfoString(BlockTagInfoType* pData)
{
	CString strTotal, strTemp;
	// ==========================================
	// 1. 궤도 및 기본 정보 배열 분해
	// ==========================================
	strTotal.Format(_T("[궤도 정보]\r\n장내궤도: %s \r\n출발궤도: %s \r\n출발적색궤도: %s \r\n"),
		(LPCTSTR)GetDBNameByArray(pData->ArrTrack, 10, TrackIdx),       // 10개짜리 배열 전체 스캔
		(LPCTSTR)GetDBNameByArray(pData->DepTrack, 10, TrackIdx),       // 10개짜리 배열 전체 스캔
		(LPCTSTR)GetDBNameByArray(pData->BlockDepRed.DepBlkRedTrk, 10, TrackIdx));
	// 상대/후방 폐색 및 신호기 정보
	strTemp.Format(_T("상대폐색:%d, 후방폐색:%d, 엄호신호:%d, 장내신호:%d\r\n"),
		pData->OppositeBlock, pData->RearBlock, pData->UmhoSignal, pData->ArrivalSignal);
	strTotal += strTemp;
	// ==========================================
	// 2. 타이머 정보
	// ==========================================
	strTemp.Format(_T("[타이머]\r\n폐쇄 출력 최대시간 : %dms [ms] | 입력처리 대기시간 : %dms [ms] | 폐색 취소출력 최대 시간 : %dms [ms]\r\n"),
		pData->OutputTm, pData->DelayTm, pData->CancelTm);
	strTotal += strTemp;
	// ==========================================
	// 3. 비트 플래그 (옵션) 해석
	// ==========================================
	CString strFlags = _T("[동작 옵션] ");
	// [수정] RevKind의 Bit0(서울교통공사 3,4호선 역방향 폐색 여부)과 Bit1(전라선/경부선 구분)은
	// 같은 바이트 안에 있지만 서로 독립된 별개의 플래그임(원본 구조체 주석 기준) - 하나의 코드값처럼
	// else if로 묶여있던 것을 분리. 또한 "& 0x00"은 항상 0이라 절대 참이 될 수 없는 죽은 코드였고
	// (경부선 케이스), 게다가 Bit1 자체는 "양방향 폐색인 경우(BlkKind=11/12/14)에만" 의미가 있음.
	if (pData->KindInfo.RevKind & 0x01)
		strFlags += _T("서울교통 3,4호선 역방향 폐색, ");
	if (pData->BlkKind == 11 || pData->BlkKind == 12 || pData->BlkKind == 14)
	{
		strFlags += (pData->KindInfo.RevKind & 0x02) ? _T("전라선 양방향 폐색, ") : _T("경부선 양방향 폐색, ");
	}
	// 역방향 출발 적색 옵션 (BlockDepRed.DepRedKind, Bit0/Bit1 구분)
	// [수정] 이전 코드의 세 줄은 실제로는 DepRedKind가 아니라 BlockBOthInfo.RevStartRed의
	// 원본 주석 문구가 그대로 잘못 붙어 있었음(문구 대조 결과 완전히 일치). 그래서:
	//  1) DepRedKind 자신의 실제 의미로 텍스트를 고치고,
	//  2) 지금까지 어디에도 표시되지 않던 RevStartRed를 올바른 필드에서 읽어 별도로 추가함.
	//  3) "& 0x00"은 항상 0이라 절대 실행되지 않는 죽은 코드였던 것도 제거.
	if (pData->BlockDepRed.DepRedKind == 0x00)
		strFlags += _T("[출발적색] 출발진로의 착점궤도 낙하일 때 적색표시(default), ");
	if (pData->BlockDepRed.DepRedKind & 0x01)
		strFlags += _T("[출발적색] 궤도 상관없이 폐색 BLTR 낙하일 때 적색표시, ");
	if (pData->BlockDepRed.DepRedKind & 0x02)
		strFlags += _T("[출발적색] 지정 궤도(DepBlkRedTrk) 낙하일 때 적색표시, ");
	if (pData->BlockBOthInfo.RevStartRed & 0x01)
		strFlags += _T("[역방향출발적색] 역방향 출발시(출발신호기 내방궤도 점유시) 폐색 적색 표시 및 출력, ");
	if (pData->BlockBOthInfo.RevStartRed & 0x02)
		strFlags += _T("[역방향출발적색] 역방향 출발 폐색 적색 표시(점멸없음) BLTR 낙하, BR 낙하일 때 출발폐색 적색점등, ");
	// 5. 출발신호 현시 조건 옵션 (Bit0, Bit1 구분)
	// [수정] BlockAspect(다른 값 체계의 잘못된 enum)와 비교하던 부분을 제거.
	// 원본 구조체 주석상 KindInfo.OutKind는 "단선자동 3현시"에 대해서만 정의되어 있어 BlkKind만으로 판단.
	if (pData->BlkKind == static_cast<Byte_t>(BlockKind::SingleAuto_3Aspect))
	{
		// Bit0=1 : 출발신호 진행
		if (pData->KindInfo.OutKind & 0x01)
		{
			strFlags += _T("BR, DR 모두 여자시 출발신호 진행 출력, ");
		}
		// Bit1=1 : 출발신호 주의
		if (pData->KindInfo.OutKind & 0x02)
		{
			strFlags += _T("BR, DR 모두 여자시 출발신호 주의, ");
		}
		// Bit0=0 && Bit1=0 : 출발신호 최소 현시
		if ((pData->KindInfo.OutKind & 0x01) == 0 && (pData->KindInfo.OutKind & 0x02) == 0)
		{
			strFlags += _T("BR, DR 모두 여자시 출발신호 최소 현시, ");
		}
	}
	//  DepSig Bit0 검사 : 양방향 폐색(정방향출발 && 역방향장내) 관련
	// [수정] 주석이 "Bit2=1"로 잘못 적혀 있었음 - 원본 구조체엔 DepSig에 Bit0만 정의돼 있고
	// 마스크도 0x01(Bit0)이라 코드 동작 자체는 맞았지만 주석이 틀렸었음. Bit2는 존재하지 않음.
	// (원본 주석은 "4현시출발 신호기일 때"라는 조건도 추가로 요구하는데, 이 조건은 아직 미반영)
	if (pData->BlkKind == static_cast<Byte_t>(BlockKind::BiDirectional)) {
		if (pData->BlockBOthInfo.DepSig & 0x01) // Bit0=1
		{
			strFlags += _T("폐색 YY 또는 착점궤도 여자시 출발신호 현시, ");
		}
	}
	// 양방향 폐색 연동 제어 옵션 (RevArrSig, Bit0/Bit1 구분)
	// [수정] Bit0은 원본 구조체 주석상 BlkKind=12(정방향장내 && 역방향출발) 전용인데,
	// 기존 코드는 11(BiDirectional)까지 포함해서 적용하고 있었음. BlkKind=11 전용인 Bit1과
	// 범위가 겹치지 않도록 분리.
	if (pData->BlkKind == static_cast<Byte_t>(BlockKind::BiDirectionalRev)) {
		if (pData->BlockBOthInfo.RevArrSig & 0x01)
			strFlags += _T(
				" 역방향출발 폐색 현시이면 장내신호 취급불가"
				"          장내신호 현시상태에서 역방향출발 폐색취급 불가"
				"역방향 출발 후 개통궤도까지 진출후에는(출발폐색 적색) 장내신호 취급가능, ");
	}
	if (pData->BlkKind == static_cast<Byte_t>(BlockKind::BiDirectional))
		if (pData->BlockBOthInfo.RevArrSig & 0x02) // Bit1 검사
			strFlags += _T(
				"역방향장내 폐색 Y 또는 R 상태에서만 장내신호 취급 및 현시 가능 ");
	// 뒤처리 및 기본값 예외 처리
	strFlags.TrimRight(_T(", "));
	if (strFlags == _T("[동작 옵션] "))
		strFlags += _T("기본값");
	strTotal += strFlags + _T("\r\n");

	strTotal += _T("[출력 포트]\r\n");
	CString strPorts;
	for (int i = 0; i < MAX_BLOCK_COND; i++)
	{
		const auto& cond = pData->BlockCond[i];
		if (cond.Gubun == 0) continue; // 미사용 조건은 건너뜀

		CString strLabel;
		switch (cond.Gubun)
		{
		case 'C': strLabel = _T("CNR");    break; // 'C' : 출력 CNR
		case 'R': strLabel = _T("RR(ZR)"); break; // 'R' : 출력 RR(ZR)
		case 'E': strLabel = _T("기타");    break; // 'E' : 기타 출력
		default:  strLabel.Format(_T("%c"), cond.Gubun); break;
		}

		if (!strPorts.IsEmpty()) strPorts += _T(" / ");
		strPorts.AppendFormat(_T("%s: %s"), (LPCTSTR)strLabel, (LPCTSTR)FormatIOPosition(cond.OutPort));
	}
	strTemp = strPorts.IsEmpty() ? CString(_T("해당 폐색 종류에 정의된 포트 출력 없음")) : (CString)strPorts;
	strTotal += strTemp;
	return strTotal;
}
CString DeviceGridInfo::FormatByteArray(const Byte_t* pArr, int nSize)
{
	CString strResult, strTemp;
	for (int i = 0; i < nSize; i++)
	{
		if (pArr[i] != 0) // 0이 아닌 유효한 데이터만 추출
		{
			strTemp.Format(_T("%d, "), pArr[i]);
			strResult += strTemp;
		}
	}
	// 마지막 쉼표 제거
	strResult.TrimRight(_T(", "));
	if (strResult.IsEmpty()) strResult = _T("없음");
	return strResult;
}
// ---------------------------------------------------------
// 4. 기타 메시지 맵 함수들
// ---------------------------------------------------------
void DeviceGridInfo::OnSize(UINT nType, int cx, int cy)
{
	CBCGPGridCtrl::OnSize(nType, cx, cy);
}
void DeviceGridInfo::OnDestroy()
{
	CBCGPGridCtrl::OnDestroy();
}
void DeviceGridInfo::OnDrawGroupRow(CDC* pDC, CBCGPGridRow* pRow, const CRect& rect)
{
	// 그룹 행의 배경색 커스텀
	pDC->FillSolidRect(rect, RGB(240, 240, 240));
}