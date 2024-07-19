// File Name:	CShtManager.c
// Description:	Implimentation File of Sheet Manager
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CShtManager.h"

#include "CShtManagerState.h"
#include "CShtPathSensor.h"
#include "CShtPreparationTime.h"

#include "CShtInitializeManager.h"

#include "CSysManager.h"
#include "CCdiAdapter.h"
#include "CCvrChecker.h"
#include "CJamChecker.h"
#include "CCfgInformer.h"
#include "CDvstInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Structure Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Judge Abort Compile Sheet Result ---
typedef struct /*JudgeAbortCompileResult*/
{
	UC abortExec;
	UC setRecoverySheetNo;
} JudgeAbortCompileResult;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Path Sensor Spec ---
extern ShtPathSensorSpec const cSht_PathSensorSpec[SHT_PATH_SNR_ID_NUM];

// --- Output Destination Information ---
extern ShtOutDestInfo const cSht_OutDestInfoTable[];

// --- Function Pointer of Get Eject Execution Sheet Request ---
extern EShtEject const (*cSht_GetEjectExecutionSheetReqFuncTable[])(UC sheetNo);

// --- Function Pointer of Get Eject Execution Next Sheet Request ---
extern EShtEject const (*cSht_GetEjectExecutionNextSheetReqFuncTable[])(UC sheetNo, UC nextShtNo);

// --- Function Pointer of Get Eject Execution Standby ---
extern EShtEject const (*cSht_GetEjectExecutionStandbyFuncTable[])(UC sheetNo);

// --- Function Pointer of Get Weighted Count ---
extern US const (*cSht_GetWeightedCompileCountFuncTable[])(UC sheetNo);

// --- Function Pointer of Get Offset Finish Position ---
extern EShtOffsetFinish const (*cSht_GetOffsetFinishPositionFuncTable[])(UC sheetNo);

// --- Table of Path Sensor ID to Sheet Location ---
extern UC const cSht_PathSensorIDLocation[SHT_LOC_ID_NUM];

// --- Table of Finisher Speed[×0.1mm/s] ---
extern US const cSht_FinisherSpeed[SHT_FIN_SPD_ID_NUM];

// --- Table of Offset Position ---
UC const cSht_OfstPosFromFinPos[SHT_OFST_FIN_NUM] = {
	eCdiFinOffsetPos_Current,								// Not Offset
	eCdiFinOffsetPos_Left,									// Front Offset
	eCdiFinOffsetPos_Right,									// Rear Offset
	eCdiFinOffsetPos_CenterFront,							// Center Offset(from Front)
	eCdiFinOffsetPos_CenterRear,							// Center Offset(from Rear)
};

// --- Table of Location Trigger ID ---
extern US const cSht_LocationEventID[SHT_LOC_ID_NUM][SHT_EDGE_NUM];

// --- Stop Sheet Specification of Device Status ---
extern ShtStopSheetSpec const cSht_StopSheetSpec[DSNO_NUM];

// --- Sheet Manager Spec ---
extern ShtManagerSpec const cSht_ManagerSpec;

// --- Member of Sheet Manager ---
typedef struct /*CShtManagerMember*/
{
	CShtManagerState mState;								// Sheet Manager state
	CShtPathSensor mPathSensor[SHT_PATH_SNR_ID_NUM];		// Path Sensor
	UC mLastPurgePsbl;										// Last inform purge possiility
	UC mLastAbortShtNo;										// Last inform abort sheet No.
	CShtSheet mSheet[SHEETNO_NUM];							// Request Sheet Instance Queue(Ring)
	UC mProcStaShtNo;										// Process Start Sheet No.
	UC mReqEntryShtNo;										// Next entry sheet No. of sheet finish requet
	UC mReqTopShtNo;										// Prime candidate sheet No. of Next exit from IOT
	UC mShtExtQue[SHEETNO_NUM];								// Exit Sheet No. Queue(Ring)
	UC mExitEntryShtIdx;									// Next entry queue index of sheet exit
	UC mNextDelvShtIdx;										// Next delivered queue index
	UC mNextEjectShtIdx;									// Next eject queue index
	UC mNextFinishShtIdx;									// Next set finish queue index
	UL mLocPlanCfgMask;										// Mask data of sheet path plan
	UC mSetRecoverySetID;									// Last send set recovery requst Set ID
	UC mPitchSkip;    										//V3.6.92   Device都合により、PitchSkip要求をする時の設定Value .. 
} CShtManagerMember;

static CShtManagerMember gSht_ManagerMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define IncrSheetIndex(aIndex) (UC)(((aIndex) + 1) & (SHEETNO_NUM - 1))			// UC (UC)
#define DecrSheetIndex(aIndex) (UC)(((aIndex) - 1) & (SHEETNO_NUM - 1))			// UC (UC)

static bool CheckConditionChangeReady();
static void ProcessReadyInitialize();
static EDeviceID ConvertOutDestIdToDeviceID(UC outDest);
static UC GetExitIndex(UC sheetNo);
static UL CreateLocationPlanConfigMask();
static UL CreateSheetLocationPlan(UC sheetNo);
static UL CreatePurgeLocationPlan(UC sheetNo);
static EShtSheetLocationID GetDeliveredLocation(UC sheetNo);
static EShtSheetLocationID GetSetFinishedLocation(UC sheetNo);
static EShtSheetEventID GetSetFinishedEvent(UC sheetNo);
static bool CheckSheetExist(UC sheetNo);
static EShtFinisherSpeedID DetectFinisherSpeed(UC sheetNo);
static EShtBuffer JudgeBuffer(UC sheetNo);
static US GetBufferFeedWaitTime(UC sheetNo, UC prevShtNo, US shortenTime);
static EShtEject JudgeEjectSheetReq(UC sheetNo);
static EShtEject JudgeEjectNextSheetReq(UC sheetNo, UC nextShtNo);
static EShtEject JudgeEjectStandby(UC sheetNo);
static US DetectWeightedCompileCount(UC sheetNo);
static EShtOffsetFinish DetectOffsetFinish(UC sheetNo);
static EShtDevidedSet DetectDevidedSet(UC sheetNo, UC prevShtNo);
static void CreateSheetInformation(UC sheetNo, UC prevShtNo);
static EDeviceID CheckDeviceCondition(UC sheetNo);
static JudgeAbortCompileResult JudgeAbortCompileSheetProcessEnd();
static JudgeAbortCompileResult JudgeAbortCompileSheetCoverClose(ECvrCoverID coverID);
static JudgeAbortCompileResult JudgeAbortCompileSheetDeviceModuleNG(UC moduleID);
static void SendSheetAttributeEntry(UC sheetNo);
static void SendSheetAttributeUpdate(UC sheetNo);
static void SendSheetAbortedNotify();
static void SendSheetLocationNotify(UC sheetNo, EShtSheetEdge edge, EShtSheetLocationID location);
static void UpdateAbnormalFinishing();
static bool CheckPurgeSheetPossible(UC sheetNo);
static bool CheckPurgePathBlocking(UC sheetNo);
static bool StopSheet(UC shtIdx);
static void StopSheetCycleDown(US devSttsNo, UC jamShtNo);
static void StopSheetHardDown();
static UC AbortEjectWaitSheet();
static void SheetEventEjectFinishComp(UC modulelID, UC sheetNo);
static void SheetEventEjectStart(UC modulelID, UC sheetNo);
static void SheetEventCompileFinishComp(UC modulelID, UC sheetNo);
static void SheetEventPunchBackStart(UC modulelID, UC sheetNo);
static void SheetEventPunchBackComp(UC modulelID, UC sheetNo);
static void PutSheetLocationLog(UC para1, UC para2, UC para3, UC para4);
static void SheetEventEjectComp(UC modulelID, UC sheetNo);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtManager_Constructor()
{
	UC index;

	CShtManagerState_Constructor(&(gSht_ManagerMember.mState));

	gSht_ManagerMember.mLastPurgePsbl	 = eCdiFinPurgePsbl_Possible;
	gSht_ManagerMember.mLastAbortShtNo	= SHEETNO_NUM;
	gSht_ManagerMember.mProcStaShtNo	    = SHEETNO_NUM;
	gSht_ManagerMember.mReqEntryShtNo	= 0;
	gSht_ManagerMember.mReqTopShtNo		= 0;
	gSht_ManagerMember.mExitEntryShtIdx	= 0;
	gSht_ManagerMember.mNextDelvShtIdx	= 0;
	gSht_ManagerMember.mNextEjectShtIdx	= 0;
	gSht_ManagerMember.mNextFinishShtIdx = 0;

	gSht_ManagerMember.mLocPlanCfgMask = 0xFFFFFFFF;

	gSht_ManagerMember.mSetRecoverySetID = 0;
	gSht_ManagerMember.mPitchSkip = OFF;  

	for ( index = 0; index < SHT_PATH_SNR_ID_NUM; index++ ){
		CShtPathSensor_Constructor(&(gSht_ManagerMember.mPathSensor[index]), &(cSht_PathSensorSpec[index]));
	}

	for ( index = 0; index < SHEETNO_NUM; index++ ){
		CShtSheet_Constructor(&(gSht_ManagerMember.mSheet[index]));

		gSht_ManagerMember.mShtExtQue[index] = SHEETNO_NUM;
	}

	CShtInitializeManager_Constructor();
}

///////////////////////////////////////////////////////////////////
// Function Name : CheckConditionChangeReady
// Description   : レディー状態遷移条件判断処理
// Parameter     : ‐
// Return        : レディー状態遷移条件判断結果
//               :  true = 遷移条件成立
//               :  false = 遷移条件非成立
///////////////////////////////////////////////////////////////////
bool CheckConditionChangeReady()
{
	if ( (CShtManagerState_GetState(&(gSht_ManagerMember.mState)) == SHT_MGR_STT_CYCLE_UP) &&
		 ( (CShtManager_CheckUnejectedSheetExist(eCdiFinOutDest_All) == false) ||
		   (CShtSheet_GetEject(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[gSht_ManagerMember.mNextEjectShtIdx]])) == SHT_EJECT_NOT) ) &&
		 (CShtInitializeManager_CheckComplete(gSht_ManagerMember.mProcStaShtNo, SHT_INIT_ID_NUM) == true) ){
		return true;
	}
	else{
		return false;
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : ProcessReadyInitialize
// Description   : レディー状態遷移時初期化処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ProcessReadyInitialize()
{
	UC index, delvPrevShtIdx;

	// --- データリセット ---
	gSht_ManagerMember.mSetRecoverySetID = 0;

	// --- 全パスセンサーのリセット ---
	for ( index = 0; index < SHT_PATH_SNR_ID_NUM; index++ ){
		CShtPathSensor_Reset(&(gSht_ManagerMember.mPathSensor[index]));
	}

	// --- Update sheet management index ---
	if ( gSht_ManagerMember.mNextDelvShtIdx != gSht_ManagerMember.mNextFinishShtIdx ){
																				// セット処理残りあり
		delvPrevShtIdx = DecrSheetIndex(gSht_ManagerMember.mNextDelvShtIdx);

		if ( CDvstInformer_GetDeviceCondtion(ConvertOutDestIdToDeviceID(CShtSheet_GetOutputDestFinish(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[delvPrevShtIdx]])))) != DEV_COND_PERM_NG ){
			if ( (CShtSheet_CheckConditionWaitEject(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[delvPrevShtIdx]])) == false) ||
				 (CShtSheet_GetEject(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[delvPrevShtIdx]])) == SHT_EJECT_EXEC) ){
				gSht_ManagerMember.mNextEjectShtIdx = gSht_ManagerMember.mNextDelvShtIdx;

				if ( CShtSheet_GetSetEnd(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[delvPrevShtIdx]])) == SHT_SET_END ){
					gSht_ManagerMember.mNextFinishShtIdx = gSht_ManagerMember.mNextDelvShtIdx;
				}
				else{
					gSht_ManagerMember.mNextFinishShtIdx = delvPrevShtIdx;
				}
			}
			else{
				gSht_ManagerMember.mNextEjectShtIdx  = delvPrevShtIdx;
				gSht_ManagerMember.mNextFinishShtIdx = delvPrevShtIdx;
			}
		}
		else{																	// 排紙先故障
			gSht_ManagerMember.mNextEjectShtIdx  = gSht_ManagerMember.mNextDelvShtIdx;
			gSht_ManagerMember.mNextFinishShtIdx = gSht_ManagerMember.mNextDelvShtIdx;
		}
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : ConvertOutDestIdToDeviceID
// Description   : Output Destionation IDからDevice IDへの変換処理
// Parameter     : outDest (Output Destination ID)
// Return        : Device ID
///////////////////////////////////////////////////////////////////
EDeviceID ConvertOutDestIdToDeviceID(UC outDest)
{
	UC index;

	index = 0;

	while ( cSht_OutDestInfoTable[index].outDestID != eCdiFinOutDest_All ){
		if ( cSht_OutDestInfoTable[index].outDestID == outDest ){
			return cSht_OutDestInfoTable[index].deviceID;
		}
		else{
			// Go while loop
		}

		index++;
	}

	return cSht_OutDestInfoTable[0].deviceID;
}

///////////////////////////////////////////////////////////////////
// Function Name : GetExitIndex
// Description   : Sheet No.からSheet Exit Indexへの変換処理
// Parameter     : sheetNo (Sheet No.)
// Return        : Sheet Exit Index
//               : 指定のSheet No.のシートが管理上にない場合は、
//               : SHEETNO_NUMを返す。
///////////////////////////////////////////////////////////////////
UC GetExitIndex(UC sheetNo)
{
	UC shtIndex;

	if ( (sheetNo < SHEETNO_NUM) && (gSht_ManagerMember.mNextFinishShtIdx != gSht_ManagerMember.mExitEntryShtIdx) ){
		shtIndex = gSht_ManagerMember.mNextFinishShtIdx;

		while ( shtIndex != gSht_ManagerMember.mExitEntryShtIdx ){
			if ( gSht_ManagerMember.mShtExtQue[shtIndex] == sheetNo ) return shtIndex;
			else													 shtIndex = IncrSheetIndex(shtIndex);
		}
	}
	else{ /* Sheet don't exist. */ }

	return SHEETNO_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CreateLocationPlanConfigMask
// Description   : マシン構成によるロケーション計画のマスクデータ(
//               : 電源ON時に計画に組み込まれないことが決まるロケー
//               : ションのマスクデータ)作成処理
// Parameter     : ‐
// Return        : ロケーション計画のマスクデータ(Bit管理)
///////////////////////////////////////////////////////////////////
UL CreateLocationPlanConfigMask()
{
	UL maskBit = 0xFFFFFFFF;

	// !!! Product Specific !!!
	maskBit = 0x00000003;

	return maskBit;
}

///////////////////////////////////////////////////////////////////
// Function Name : CreateSheetLocationPlan
// Description   : シートロケーション計画作成処理。
// Parameter     : sheetNo (Sheet No.)
// Return        : シートロケーション計画(Bit管理)
///////////////////////////////////////////////////////////////////
UL CreateSheetLocationPlan(UC sheetNo)
{
	UL planBit;

	// !!! Product Specific !!!
	planBit = SHT_LOC_ID_CNV_BIT(SHT_LOC_ID_IOT_EXIT_SNR)|
			  SHT_LOC_ID_CNV_BIT(SHT_LOC_ID_COMPILE_EXIT_SNR);

	return (planBit & gSht_ManagerMember.mLocPlanCfgMask);
}

///////////////////////////////////////////////////////////////////
// Function Name : CreatePurgeLocationPlan
// Description   : パージシートロケーション計画作成処理。
// Parameter     : sheetNo (Sheet No.)
// Return        : パージシートロケーション計画(Bit管理)
///////////////////////////////////////////////////////////////////
UL CreatePurgeLocationPlan(UC sheetNo)
{
	UL planBit;

	// !!! Product Specific !!!
	planBit = SHT_LOC_ID_CNV_BIT(SHT_LOC_ID_IOT_EXIT_SNR)|
			  SHT_LOC_ID_CNV_BIT(SHT_LOC_ID_COMPILE_EXIT_SNR);

	return (planBit & gSht_ManagerMember.mLocPlanCfgMask);
}

///////////////////////////////////////////////////////////////////
// Function Name : GetDeliveredLocation
// Description   : 指定シートのSheet Delivered To Output送信ロケーシ
//               : ョン取得処理。
// Parameter     : sheetNo (Sheet No.)
// Return        : デリバード位置のSheet Location ID
///////////////////////////////////////////////////////////////////
EShtSheetLocationID GetDeliveredLocation(UC sheetNo)
{
	UC index, outDest;

	index = 0;

	outDest = CShtSheet_GetOutputDestination(&(gSht_ManagerMember.mSheet[sheetNo]));

	while ( cSht_OutDestInfoTable[index].outDestID != eCdiFinOutDest_All ){
		if ( cSht_OutDestInfoTable[index].outDestID == outDest ){
			return cSht_OutDestInfoTable[index].deliveredLocation;
		}
		else{
			// Go while loop
		}

		index++;
	}

	return cSht_OutDestInfoTable[0].deliveredLocation;
}

///////////////////////////////////////////////////////////////////
// Function Name : GetSetFinishedLocation
// Description   : 
// Parameter     : 
// Return        : 
///////////////////////////////////////////////////////////////////
EShtSheetLocationID GetSetFinishedLocation(UC sheetNo)
{
	return SHT_LOC_ID_NUM;  //V3.4.0
}

///////////////////////////////////////////////////////////////////
// Function Name : GetSetFinishedEvent
// Description   : 
// Parameter     : 
// Return        : 
///////////////////////////////////////////////////////////////////
EShtSheetEventID  GetSetFinishedEvent(UC sheetNo)
{
	return  SHT_EVT_ID_EJECT_COMP;  //V3.4.0  
}

///////////////////////////////////////////////////////////////////
// Function Name : CheckSheetExist
// Description   : シート搬送管理上シート有無確認処理。
// Parameter     : sheetNo (Sheet No.) [0 ～ SHEETNO_NUM]
// Return        : true = シート搬送管理上にシート有り
//               : false = シート搬送管理上にシート無し
///////////////////////////////////////////////////////////////////
bool CheckSheetExist(UC sheetNo)
{
	UC checkShtNo;

	if ( (gSht_ManagerMember.mReqEntryShtNo != gSht_ManagerMember.mReqTopShtNo) ||
		 (gSht_ManagerMember.mNextFinishShtIdx != gSht_ManagerMember.mExitEntryShtIdx) ){
		if ( sheetNo < SHEETNO_NUM ){
			if ( gSht_ManagerMember.mNextFinishShtIdx != gSht_ManagerMember.mExitEntryShtIdx ){
				checkShtNo = gSht_ManagerMember.mShtExtQue[gSht_ManagerMember.mNextFinishShtIdx];
			}
			else{
				checkShtNo = gSht_ManagerMember.mReqTopShtNo;
			}

			while ( checkShtNo != gSht_ManagerMember.mReqEntryShtNo ){
				if ( checkShtNo == sheetNo ) return true;
				else						 checkShtNo = IncrSheetIndex(checkShtNo);
			}
		}
		else{
			return true;
		}
	}
	else{ /* No Action */ }

	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : DetectFinisherSpeed
// Description   : フィニッシャー搬送速度取得処理。
// Parameter     : sheetNo (Sheet No.)
// Return        : フィニッシャー搬送速度識別子
///////////////////////////////////////////////////////////////////
EShtFinisherSpeedID DetectFinisherSpeed(UC sheetNo)
{
	UC spdID;
	US usLength;
	
	// !!! Product Specific !!!
	spdID = SHT_FIN_SPD_ID_NVMV5_800;
	usLength = SRVC_GetProcessSize(sheetNo);

	if ( SRVC_GetNvmValueUS(NVMID_FINISHER_SPEED) == 0 ) {
	
		if ( SRVC_GetProductivity() > PITCH_SPEEDCHANGE_CPM65 ) { 
			spdID = SHT_FIN_SPD_ID_NVMV5_800;  
		} else if  ( SRVC_GetProductivity() > PITCH_SPEEDCHANGE_CPM55 ) {
			spdID = SHT_FIN_SPD_ID_NVMV4_700;
		} else {
			spdID = SHT_FIN_SPD_ID_NVMV3_600;		
		}

	} else if ( SRVC_GetNvmValueUS(NVMID_FINISHER_SPEED) == 1 )  {
	
		spdID = SHT_FIN_SPD_ID_NVMV3_600; //TRNS_MOTSPEED_600; 
	} else if ( SRVC_GetNvmValueUS(NVMID_FINISHER_SPEED) == 2 )  {

		spdID = SHT_FIN_SPD_ID_NVMV4_700; //TRNS_MOTSPEED_700; 
	} else if ( SRVC_GetNvmValueUS(NVMID_FINISHER_SPEED) == 3 )  {
		
		spdID = SHT_FIN_SPD_ID_NVMV5_800; //TRNS_MOTSPEED_800; 
	}	
//	PutSheetManagerLog(0x22,sheetNo,spdID,usLength);

	return spdID;
}

///////////////////////////////////////////////////////////////////
// Function Name : JudgeBuffer
// Description   : バッファ制御実施判断処理。
// Parameter     : sheetNo (Sheet No.)
// Return        : バッファ制御実施判断結果
///////////////////////////////////////////////////////////////////
EShtBuffer JudgeBuffer(UC sheetNo)
{
	UC buffer;

	buffer = SHT_BUFFER_NOT;

	// !!! Product Specific !!!
//	buffer = SHT_BUFFER_XXX;

	return buffer;
}

///////////////////////////////////////////////////////////////////
// Function Name : GetBufferFeedWaitTime
// Description   : バッファフィード待ち時間取得処理
// Parameter     : sheetNo (対象シートのSheet No.)
//               : prevShtNo (先行シートのSheet No.)
//               : shortenTime (バッファ制御による減算時間)
// Return        : バッファフィード待ち時間[×1ms]
///////////////////////////////////////////////////////////////////
US GetBufferFeedWaitTime(UC sheetNo, UC prevShtNo, US shortenTime)
{
// ROM削減のためにコメントアウト
//	US bufferInTime;
//
//	// !!! Product Specific !!!
//	Buffer 引き込み時間に必要な時間との比較が必要!!!
//	bufferInTime = XXXXX;
//
//	if ( shortenTime < bufferInTime ){
//		return bufferInTime;
//	}
//	else{
//		return shortenTime;
//	}
	return 0;
}

///////////////////////////////////////////////////////////////////
// Function Name : JudgeEjectSheetReq
// Description   : 対象シートのシート処理依頼受信時の排紙判断処理
// Parameter     : sheetNo (Sheet No.)
// Return        : 排紙判断判断結果
///////////////////////////////////////////////////////////////////
EShtEject JudgeEjectSheetReq(UC sheetNo)
{
	UC index, eject;

	index = 0;

	while ( cSht_GetEjectExecutionSheetReqFuncTable[index] != NULL ){
		eject = (UC)(cSht_GetEjectExecutionSheetReqFuncTable[index](sheetNo));

		if ( eject == SHT_EJECT_NOT ) return SHT_EJECT_NOT;

		index++;
	}

	return SHT_EJECT_EXEC;
}

///////////////////////////////////////////////////////////////////
// Function Name : JudgeEjectNextSheetReq
// Description   : 次用紙のシート処理依頼受信時の排紙判断処理
// Parameter     : sheetNo (判断対象シートのSheet No.)
//               : nextShtNo (次用紙のSheet No.)
// Return        : 排紙判断判断結果
///////////////////////////////////////////////////////////////////
EShtEject JudgeEjectNextSheetReq(UC sheetNo, UC nextShtNo)
{
	UC index, eject;

	index = 0;

	while ( cSht_GetEjectExecutionNextSheetReqFuncTable[index] != NULL ){
		eject = (UC)(cSht_GetEjectExecutionNextSheetReqFuncTable[index](sheetNo, nextShtNo));

		if ( eject == SHT_EJECT_NOT ) return SHT_EJECT_NOT;

		index++;
	}

	return SHT_EJECT_EXEC;
}

///////////////////////////////////////////////////////////////////
// Function Name : JudgeEjectStandby
// Description   : スタンバイ時(フィニッシングプロセス終了時やイニ
//               : シャライズ動作終了時)の排紙判断処理
// Parameter     : sheetNo (Sheet No.)
// Return        : 排紙判断判断結果
///////////////////////////////////////////////////////////////////
EShtEject JudgeEjectStandby(UC sheetNo)
{
	UC index, eject;
     //V3.4.0 Merge (DevStatus判断追加)
	if ( (CCvrChecker_GetCoverStatus(CVR_ID_NUM) == CVR_STTS_CLOSE) &&
		 (CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_CLEAR) ){//&&
	//	 (CJamChecker_GetScratchSheetCompileStatus() == FAULT_NOT_OCCUR) ){
		if ( CShtSheet_GetSetEnd(&(gSht_ManagerMember.mSheet[sheetNo])) == SHT_SET_END ){
			return SHT_EJECT_EXEC;
		}
		else{
			index = 0;

			while ( cSht_GetEjectExecutionStandbyFuncTable[index] != NULL ){
				eject = (UC)(cSht_GetEjectExecutionStandbyFuncTable[index](sheetNo));

				if ( eject == SHT_EJECT_NOT ) return SHT_EJECT_NOT;

				index++;
			}

			return SHT_EJECT_EXEC;
		}
	}
	else{
		return SHT_EJECT_NOT;
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : DetectWeightedCompileCount
// Description   : 重みづけコンパイル枚数決定処理
// Parameter     : sheetNo (Sheet No.)
// Return        : 重みづけコンパイル枚数[×0.1枚]
///////////////////////////////////////////////////////////////////
US DetectWeightedCompileCount(UC sheetNo)
{
	UC index, count;

	index = 0;

	while ( cSht_GetWeightedCompileCountFuncTable[index] != NULL ){
		count = (UC)(cSht_GetWeightedCompileCountFuncTable[index](sheetNo));

		if ( count != 0 ) return count;

		index++;
	}

	return 10;
}

///////////////////////////////////////////////////////////////////
// Function Name : DetectOffsetFinish
// Description   : オフセット位置判断処理
// Parameter     : sheetNo (Sheet No.)
// Return        : オフセット位置判断結果
///////////////////////////////////////////////////////////////////
EShtOffsetFinish DetectOffsetFinish(UC sheetNo)
{
	UC index, offset;

	index = 0;

	while ( cSht_GetOffsetFinishPositionFuncTable[index] != NULL ){
		offset = (UC)(cSht_GetOffsetFinishPositionFuncTable[index](sheetNo));

		if ( offset != SHT_OFST_FIN_NOT ) return offset;

		index++;
	}

	return SHT_OFST_FIN_NOT;
}

///////////////////////////////////////////////////////////////////
// Function Name : DetectDevidedSet
// Description   : ディバイディッドセット決定処理
// Parameter     : sheetNo (Sheet No.)
//               : prevShtNo (先行シートのSheet No.)
// Return        : ディバイディッドセット判断結果
///////////////////////////////////////////////////////////////////
EShtDevidedSet DetectDevidedSet(UC sheetNo, UC prevShtNo)
{
	if ( CShtSheet_GetSetTop(&(gSht_ManagerMember.mSheet[sheetNo])) == SHT_SET_TOP_NOT ){
		if( (CShtSheet_GetDevidedSet(&(gSht_ManagerMember.mSheet[prevShtNo])) == SHT_SET_DEVIDED) ||
			( (CShtSheet_GetSetEnd(&(gSht_ManagerMember.mSheet[sheetNo])) == SHT_SET_END_NOT) &&
			  (CShtSheet_GetEject(&(gSht_ManagerMember.mSheet[sheetNo])) == SHT_EJECT_EXEC) ) ){
			return SHT_SET_DEVIDED;
		}
		else{ /* Undevided */ }
	}
	else{ /* Undevided */ }

	return SHT_SET_UNDEVIDED;
}

///////////////////////////////////////////////////////////////////
// Function Name : CreateSheetInformation
// Description   : シート情報構築処理
// Parameter     : sheetNo (対象シートのSheet No.)
//               : prevShtNo (対象シートの先行シートのSheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CreateSheetInformation(UC sheetNo, UC prevShtNo)
{
	CShtSheetAttribute *pShtAttr, *pPrevShtAttr;
	US weightedCount;
	UC count;

	pShtAttr = CShtSheet_GetAttribute(&(gSht_ManagerMember.mSheet[sheetNo]));

	// --- フィニッシャー搬送速度[x0.1mm/s]セット ---
	CShtSheet_SetFinisherSpeedID(&(gSht_ManagerMember.mSheet[sheetNo]), DetectFinisherSpeed(sheetNo));

	// --- シート一枚当たりの重み付けスタック枚数セット ---
	count = 1;
	weightedCount = DetectWeightedCompileCount(sheetNo);

	// --- コンパイルシート束内最大・最小坪量セット(一次セット) ---V3.4.0 T/B対策Merge No.41
	CShtSheet_SetMaxMediaWeightOfCompile(&(gSht_ManagerMember.mSheet[sheetNo]), CShtSheetAttribute_GetMediaWeight(pShtAttr));
	CShtSheet_SetMinMediaWeightOfCompile(&(gSht_ManagerMember.mSheet[sheetNo]), CShtSheetAttribute_GetMediaWeight(pShtAttr));
	// --- コンパイルシート束内最大・最小サイズセット(一次セット) ---
	CShtSheet_SetMaxProcSizeOfCompile(&(gSht_ManagerMember.mSheet[sheetNo]), CShtSheetAttribute_GetProcessSize(pShtAttr));
	CShtSheet_SetMaxCrossProcSizeOfCompile(&(gSht_ManagerMember.mSheet[sheetNo]), CShtSheetAttribute_GetCrossProcSize(pShtAttr));
	CShtSheet_SetMinProcSizeOfCompile(&(gSht_ManagerMember.mSheet[sheetNo]), CShtSheetAttribute_GetProcessSize(pShtAttr));
	CShtSheet_SetMinCrossProcSizeOfCompile(&(gSht_ManagerMember.mSheet[sheetNo]), CShtSheetAttribute_GetCrossProcSize(pShtAttr));

	if ( prevShtNo != SHEETNO_NUM ){											// Previous sheet exist.
		pPrevShtAttr = CShtSheet_GetAttribute(&(gSht_ManagerMember.mSheet[prevShtNo]));

		// --- 同セット情報チェック ---
		if ( CShtSheet_GetSetEnd(&(gSht_ManagerMember.mSheet[prevShtNo])) == SHT_SET_END_NOT ){
																				// Previous sheet is same set.
			if ( (CShtSheet_GetProcSizeMix(&(gSht_ManagerMember.mSheet[prevShtNo])) == SHT_SIZE_MIX) ||
				 (CShtSheetAttribute_GetProcessSize(pPrevShtAttr) != CShtSheetAttribute_GetProcessSize(pShtAttr)) ){
				CShtSheet_SetProcSizeMix(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_SIZE_MIX);
			}
			else{ /* No Action */ }

			if ( (CShtSheet_GetCrossProcSizeMix(&(gSht_ManagerMember.mSheet[prevShtNo])) == SHT_SIZE_MIX) ||
				 (CShtSheetAttribute_GetCrossProcSize(pPrevShtAttr) != CShtSheetAttribute_GetCrossProcSize(pShtAttr)) ){
				CShtSheet_SetCrossProcSizeMix(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_SIZE_MIX);
			}
			else{ /* No Action */ }

		}
		else{ /* No Action */ }

		// --- 同コンパイルシート束情報チェック ---
		if ( CShtSheet_GetEject(&(gSht_ManagerMember.mSheet[prevShtNo])) == SHT_EJECT_NOT ){
			// --- スタック複数枚目の場合のスタック枚数の決定 ---
			count += CShtSheet_GetCompileCount(&(gSht_ManagerMember.mSheet[prevShtNo]));
			weightedCount += CShtSheet_GetWeightedCompileCount(&(gSht_ManagerMember.mSheet[prevShtNo]));

			// --- コンパイルシート束内最大・最小坪量セット ---V3.4.0 T/B対策Merge No.41
			if ( CShtSheet_GetMaxMediaWeightOfCompile(&(gSht_ManagerMember.mSheet[prevShtNo])) > CShtSheetAttribute_GetMediaWeight(pShtAttr) ){
				CShtSheet_SetMaxMediaWeightOfCompile(&(gSht_ManagerMember.mSheet[sheetNo]), CShtSheet_GetMaxMediaWeightOfCompile(&(gSht_ManagerMember.mSheet[prevShtNo])));
			}
			else{ /* No Action */ }

			if ( CShtSheet_GetMinMediaWeightOfCompile(&(gSht_ManagerMember.mSheet[prevShtNo])) < CShtSheetAttribute_GetMediaWeight(pShtAttr) ){
				CShtSheet_SetMinMediaWeightOfCompile(&(gSht_ManagerMember.mSheet[sheetNo]), CShtSheet_GetMinMediaWeightOfCompile(&(gSht_ManagerMember.mSheet[prevShtNo])));
			}
			else{ /* No Action */ }

			// --- コンパイルシート束内最大・最小サイズセット ---
			if ( CShtSheet_GetMaxProcSizeOfCompile(&(gSht_ManagerMember.mSheet[prevShtNo])) > CShtSheetAttribute_GetProcessSize(pShtAttr) ){
				CShtSheet_SetMaxProcSizeOfCompile(&(gSht_ManagerMember.mSheet[sheetNo]), CShtSheet_GetMaxProcSizeOfCompile(&(gSht_ManagerMember.mSheet[prevShtNo])));
			}
			else{ /* No Action */ }

			if ( CShtSheet_GetMaxCrossProcSizeOfCompile(&(gSht_ManagerMember.mSheet[prevShtNo])) > CShtSheetAttribute_GetCrossProcSize(pShtAttr) ){
				CShtSheet_SetMaxCrossProcSizeOfCompile(&(gSht_ManagerMember.mSheet[sheetNo]), CShtSheet_GetMaxCrossProcSizeOfCompile(&(gSht_ManagerMember.mSheet[prevShtNo])));
			}
			else{ /* No Action */ }

			if ( CShtSheet_GetMinProcSizeOfCompile(&(gSht_ManagerMember.mSheet[prevShtNo])) < CShtSheetAttribute_GetProcessSize(pShtAttr) ){
				CShtSheet_SetMinProcSizeOfCompile(&(gSht_ManagerMember.mSheet[sheetNo]), CShtSheet_GetMinProcSizeOfCompile(&(gSht_ManagerMember.mSheet[prevShtNo])));
			}
			else{ /* No Action */ }

			if ( CShtSheet_GetMinCrossProcSizeOfCompile(&(gSht_ManagerMember.mSheet[prevShtNo])) < CShtSheetAttribute_GetCrossProcSize(pShtAttr) ){
				CShtSheet_SetMinCrossProcSizeOfCompile(&(gSht_ManagerMember.mSheet[sheetNo]), CShtSheet_GetMinCrossProcSizeOfCompile(&(gSht_ManagerMember.mSheet[prevShtNo])));
			}
			else{ /* No Action */ }
		}
		else{ /* No Action */ }

		// ディバイディッドセット情報セット
		CShtSheet_SetDevidedSet(&(gSht_ManagerMember.mSheet[sheetNo]), DetectDevidedSet(sheetNo, prevShtNo));
	}
	else{ /* No Action */ }														// Previous sheet don't exit.

	// コンパイル枚数セット
	CShtSheet_SetCompileCount(&(gSht_ManagerMember.mSheet[sheetNo]), count);
	CShtSheet_SetWeightedCompileCount(&(gSht_ManagerMember.mSheet[sheetNo]), weightedCount);

	// 後処理最終形状セット
	CShtSheet_SetOffsetFinish(&(gSht_ManagerMember.mSheet[sheetNo]), DetectOffsetFinish(sheetNo));

	// 排紙判断
	CShtSheet_SetEject(&(gSht_ManagerMember.mSheet[sheetNo]), JudgeEjectSheetReq(sheetNo));
//	PutSheetManagerLog(0x55,prevShtNo,(US)DetectDevidedSet(sheetNo, prevShtNo),weightedCount);
}

///////////////////////////////////////////////////////////////////
// Function Name : CheckDeviceCondition
// Description   : シート処理に関連するデバイスのコンディション確認
//               : 処理。
// Parameter     : sheetNo (Sheet No.)
// Return        : NG Device ID
///////////////////////////////////////////////////////////////////
EDeviceID CheckDeviceCondition(UC sheetNo)
{
	CShtSheetAttribute *pShtAttr;
	EDeviceID outDev;
	EDeviceID ngDev;

	ngDev = DEVID_NUM;

	if ( (CDvstInformer_GetDeviceCondtion(DEVID_FINISHER) == DEV_COND_OK) &&
		 (CCvrChecker_GetCoverStatus(CVR_ID_NUM) == CVR_STTS_CLOSE) &&
		 (CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_CLEAR) ){
		outDev = ConvertOutDestIdToDeviceID(CShtSheet_GetOutputDestFinish(&(gSht_ManagerMember.mSheet[sheetNo])));

		if ( CDvstInformer_GetDeviceCondtion(outDev) == DEV_COND_OK ){			// Condition OK
			pShtAttr = CShtSheet_GetAttribute(&(gSht_ManagerMember.mSheet[sheetNo]));

			// !!! Product Specific !!!
//			指定処理内容に応じたDevice Conditionをチェックする。
		}
		else{																	// Output destination NG
			ngDev = outDev;
		}
	}
	else{																		// Finisher NG
		ngDev = DEVID_FINISHER;
	}

	return ngDev;
}


///////////////////////////////////////////////////////////////////
// Function Name : JudgeAbortCompileSheetProcessEnd
// Description   : フィニッシングプロセス終了時のフォルト発生状況や
//               : 、機内残留紙の状況によって、デリバード済みシート
//               : のアボート処理を実施するか、また、それに伴いSet
//               : Recovery Requestを送信するかどうかと、その対象紙
//               : をどのシート(のSet ID)にするかを判断する処理
// Parameter     : ‐
// Return        : アボート判断処理結果
//               :  - アボート実施／未実施
//               :  - Set Recovery送信対象シート識別子
///////////////////////////////////////////////////////////////////
JudgeAbortCompileResult JudgeAbortCompileSheetProcessEnd()
{
	JudgeAbortCompileResult result;

	// !!! Product Specific !!!
	// アボート処理を実施しない
	result.abortExec = false;
	result.setRecoverySheetNo = SHEETNO_NUM;

	return result;
}

///////////////////////////////////////////////////////////////////
// Function Name : JudgeAbortCompileSheetCoverClose
// Description   : カバー開閉によって、コンパイルトレイ内のシートが
//               : 抜かれるなどして、セット処理が継続できなくなるケ
//               : ースを考慮し、シートのアボート処理を実施するか、
//               : また、それに伴いSet Recovery Requestを送信するか
//               : どうかとその対象紙をどのシート(のSet ID)にするか
//               : を判断する処理
// Parameter     : coverID (Cover ID)
// Return        : アボート判断処理結果
//               :  - アボート実施／未実施
//               :  - Set Recovery送信対象シート識別子
///////////////////////////////////////////////////////////////////
JudgeAbortCompileResult JudgeAbortCompileSheetCoverClose(ECvrCoverID coverID)
{
	JudgeAbortCompileResult result;

	// !!! Product Specific !!!
	// アボート処理を実施しない
	result.abortExec = false;
	result.setRecoverySheetNo = SHEETNO_NUM;

	return result;
}

///////////////////////////////////////////////////////////////////
// Function Name : JudgeAbortCompileSheetDeviceModuleNG
// Description   : デバイスモジュールの制御上の問題でシートのアボー
//               : ト処理を実施するか、また、それに伴いSet Recovery
//               :  Requestを送信するかどうかとその対象紙をどのシー
//               : ト(のSet ID)にするかを判断する処理
// Parameter     : outDest (Output Destination ID)
// Return        : アボート判断処理結果
//               :  - アボート実施／未実施
//               :  - Set Recovery送信対象シート識別子
///////////////////////////////////////////////////////////////////
JudgeAbortCompileResult JudgeAbortCompileSheetDeviceModuleNG(UC outDest)
{

	JudgeAbortCompileResult result;

	// !!! Product Specific !!!
	result.abortExec = false;
	result.setRecoverySheetNo = SHEETNO_NUM;

	return result;
}

///////////////////////////////////////////////////////////////////
// Function Name : SendSheetAttributeEntry
// Description   : Sheet Attribute Entryイベント送信処理
// Parameter     : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SendSheetAttributeEntry(UC sheetNo)
{
	EvtParamSheetAttributeEntry evtParameter;

	evtParameter.sheetNo = sheetNo;

	MSP_SendMessage(MID_SYS, EVT_SHT_ENTRY, (const UC *)(&evtParameter), sizeof(evtParameter), LOG_OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : SendSheetAttributeUpdate
// Description   : Sheet Attribute Updateイベント送信処理
// Parameter     : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SendSheetAttributeUpdate(UC sheetNo)
{
	EvtParamSheetAttributeUpdate evtParameter;

	evtParameter.sheetNo = sheetNo;

	MSP_SendMessage(MID_SYS, EVT_SHT_UPDATE, (const UC *)(&evtParameter), sizeof(evtParameter), LOG_OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : SendSheetAbortedNotify
// Description   : Sheet Aborted Notifyイベント送信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SendSheetAbortedNotify()
{
	MSP_SendMessage(MID_SYS, EVT_SHT_ABORTED, NULL, 0, LOG_OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : SendSheetLocationNotify
// Description   : Sheet Location Notifyイベント送信処理
// Parameter     : sheetNo (Sheet No.)
//               : edge (シートエッジ識別子)
//               : location (Sheet Location ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SendSheetLocationNotify(UC sheetNo, EShtSheetEdge edge, EShtSheetLocationID location)
{
	MSP_NotifyTriggerLocation(cSht_LocationEventID[location][edge], sheetNo, 0xFFFFFFFF, 0);
}

///////////////////////////////////////////////////////////////////
// Function Name : UpdateAbnormalFinishing
// Description   : Abnormal Finishing情報更新処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void UpdateAbnormalFinishing()
{
	enum ECdiFinPurgePossibility purgePossibility;

	UC abortShtNo, shtIndex;

	// Search new abort sheet
	abortShtNo = SHEETNO_NUM;

	shtIndex = gSht_ManagerMember.mNextDelvShtIdx;
	while ( shtIndex != gSht_ManagerMember.mExitEntryShtIdx ){
		if ( gSht_ManagerMember.mLastAbortShtNo == gSht_ManagerMember.mShtExtQue[shtIndex] ){
																				// Already sheet is aborted.
			break;																// New abort sheet don't exist.
		}
		else{ /* No Action */ }

		if ( CShtSheet_CheckConditionNormal(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[shtIndex]])) == false ){
																				// Sheet condition is abnormal.
			abortShtNo = gSht_ManagerMember.mShtExtQue[shtIndex];
			//V3.4.0 T/B対策Merge No.38																
			gSht_ManagerMember.mLastAbortShtNo = abortShtNo;					// アボート対象シートを記憶
			break;
		}
		else{ /* No Action */ }

		shtIndex = IncrSheetIndex(shtIndex);
	}

	// Judge update abnormal finishing information (V3.4.0 T/B対策Merge No.38)
	purgePossibility = CShtManager_GetPurgePossibility();
	if ( (CShtManagerState_GetState(&(gSht_ManagerMember.mState)) != SHT_MGR_STT_STANDBY) &&
		 ( (gSht_ManagerMember.mLastPurgePsbl != purgePossibility) || (abortShtNo != SHEETNO_NUM) ) ){
		gSht_ManagerMember.mLastPurgePsbl  = purgePossibility;
		CCdiAdapter_SendAbnormalFinishing(abortShtNo);
	}	
/*	if ( (CShtManagerState_GetState(&(gSht_ManagerMember.mState)) != SHT_MGR_STT_STANDBY) &&
		 ( (gSht_ManagerMember.mLastPurgePsbl != CShtManager_GetPurgePossibility()) || (abortShtNo != SHEETNO_NUM) ) ){
		gSht_ManagerMember.mLastPurgePsbl  = CShtManager_GetPurgePossibility();
		gSht_ManagerMember.mLastAbortShtNo = abortShtNo;
		CCdiAdapter_SendAbnormalFinishing(abortShtNo);
	}*/
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CheckPurgeSheetPossible
// Description   : パージ可否判断処理
// Parameter     : sheetNo (Sheet No.)
// Return        : true = パージ可能
//               : false = パージ不可
///////////////////////////////////////////////////////////////////
bool CheckPurgeSheetPossible(UC sheetNo)
{
	// !!! Product Specific !!!
//	パージ可否判断を実施する。
	if (CShtManager_CheckPathSensorArrival(SHT_PATH_SNR_ID_COMPILE_EXIT, sheetNo, SHT_EDGE_TAIL) == true)
		return false;
	else 
		return true;

//	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CheckPurgePathBlocking
// Description   : パージ搬送経路封鎖確認処理
// Parameter     : sheetNo (Sheet No.)
// Return        : パージ搬送経路封鎖確認結果
//               :  true = パージパスを封鎖している(パージ不可)
//               :  false = パージパスを封鎖していない(パージ可能)
///////////////////////////////////////////////////////////////////
bool CheckPurgePathBlocking(UC sheetNo)
{
	// !!! Product Specific !!!
//	パージパスの封鎖チェックを実施する。
	if (CShtManager_CheckPathSensorArrival(SHT_PATH_SNR_ID_COMPILE_EXIT, sheetNo, SHT_EDGE_TAIL) == true)
		return false;
	else 
		return true;

//	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : StopSheet
// Description   : シート制御停止処理
// Parameter     : shtIdx (Sheet Exit Index)
// Return        : アボート実施結果
//               :  true = シート制御停止シートあり
//               :  false = シート制御停止シートなし(アボートなし)
///////////////////////////////////////////////////////////////////
bool StopSheet(UC shtIdx)
{
	UC abortExec, purgeTrayCond, nextLocLead, nextLocTail, sheetNo;

	abortExec = false;

	// --- Stop transport sheet ---
	if ( gSht_ManagerMember.mExitEntryShtIdx != gSht_ManagerMember.mNextEjectShtIdx ){
//		if ( cSht_ManagerSpec.purgeOutDestID != eCdiFinOutDest_All ) purgeTrayCond = CDvstInformer_GetDeviceCondtion(ConvertOutDestIdToDeviceID(cSht_ManagerSpec.purgeOutDestID));
//		else														 purgeTrayCond = DEV_COND_PERM_NG;
		purgeTrayCond = DEV_COND_OK;											// 故障トレイを使用してパージするため無条件でOKとする

		while ( shtIdx != gSht_ManagerMember.mExitEntryShtIdx ){
			sheetNo = gSht_ManagerMember.mShtExtQue[shtIdx];

			// --- Check sheet purge condition ---
			if ( (CShtManagerState_GetState(&(gSht_ManagerMember.mState)) == SHT_MGR_STT_PURGE_DOWN) &&
				 (purgeTrayCond != DEV_COND_PERM_NG) &&
				 (CheckPurgeSheetPossible(sheetNo) == true) ){
				if ( CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_COND_CHG_EVT_PURGE) == SHT_COND_PURGE ){
					// --- Set data for purge ---
					CShtSheet_SetLocationPlan(&(gSht_ManagerMember.mSheet[sheetNo]), CreatePurgeLocationPlan(sheetNo));
					CShtSheet_SetOutputDestFinish(&(gSht_ManagerMember.mSheet[sheetNo]), cSht_ManagerSpec.purgeOutDestID);

					nextLocLead = CShtSheet_GetNextLocation(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_EDGE_LEAD);
					if ( nextLocLead < SHT_LOC_ID_NUM ){
						CShtPathSensor_EntrySheet(&(gSht_ManagerMember.mPathSensor[cSht_PathSensorIDLocation[nextLocLead]]), sheetNo, SHT_EDGE_LEAD);
					}
					else{ /* No Action */ }

					nextLocTail = CShtSheet_GetNextLocation(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_EDGE_TAIL);
					if ( nextLocTail < SHT_LOC_ID_NUM ){
						CShtPathSensor_EntrySheet(&(gSht_ManagerMember.mPathSensor[cSht_PathSensorIDLocation[nextLocTail]]), sheetNo, SHT_EDGE_TAIL);
					}
					else{ /* No Action */ }

					abortExec = true;
				}
				else{ /* No Action */ }
			}
			else{																// This sheet is impossible purge.
				if ( CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_COND_CHG_EVT_STOP) != SHT_COND_NUM ){
					abortExec = true;
				}
				else{ /* No Action */ }
			}

			// --- Check sheet stop condition by remain sheet ---
			if ( CShtSheet_CheckConditionStop(&(gSht_ManagerMember.mSheet[sheetNo])) == true ){
																				// Sheet is stop sheet.
				if ( CShtManagerState_GetState(&(gSht_ManagerMember.mState)) == SHT_MGR_STT_PURGE_DOWN ){
					if ( CheckPurgePathBlocking(sheetNo) == true ){
																				// Blocking buffer path.
						CShtManagerState_ChangeBlockDown(&(gSht_ManagerMember.mState));
					}
					else{ /* No Action */ }
				}
				else{ /* No Action */ }
			}
			else{ /* No Action */ }

			shtIdx = IncrSheetIndex(shtIdx);
		}
	}
	else{ /* No Action */ }

	// --- Cancel candidate sheet ---
	if ( CShtManagerState_GetState(&(gSht_ManagerMember.mState)) == SHT_MGR_STT_BLOCK_DOWN ){
		sheetNo = gSht_ManagerMember.mReqTopShtNo;

		while ( sheetNo != gSht_ManagerMember.mReqEntryShtNo ){
		//  V3.4.0 T/B対策Merge	
		//	if ( CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[shtIdx]]), SHT_COND_CHG_EVT_CANCEL) != SHT_COND_NUM ){
			if ( CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_COND_CHG_EVT_CANCEL) != SHT_COND_NUM ){

				abortExec = true;
			}
			else{ /* No Action */ }

			sheetNo = IncrSheetIndex(sheetNo);
		}
	}
	else{ /* No Action */ }

	return abortExec;
}

///////////////////////////////////////////////////////////////////
// Function Name : StopSheetCycleDown
// Description   : サイクルダウンでのシート制御停止処理
// Parameter     : devSttsNo (Device Status No.)
//               : jamShtNo (ジャム原因紙のSheet No.)
//               :  原因紙がない場合はSHEETNO_NUM
// Return        : ‐
///////////////////////////////////////////////////////////////////
void StopSheetCycleDown(US devSttsNo, UC jamShtNo)
{
	UC abortExec;
	UC ngOutDest, ngPathSnr, shtIdx;

	if ( devSttsNo < DSNO_NUM ){
		if ( cSht_StopSheetSpec[devSttsNo].purge == eCdiFinPurgePsbl_Possible ){
			CShtManagerState_ChangePurgeDown(&(gSht_ManagerMember.mState));
		}
		else{																	// Purge impossible
			CShtManagerState_ChangeBlockDown(&(gSht_ManagerMember.mState));
		}

		if ( gSht_ManagerMember.mExitEntryShtIdx != gSht_ManagerMember.mNextEjectShtIdx ){
																				// Managed sheet is exist.
			abortExec = false;

			// --- Update jam sheet condition ---
		// V3.4.0  T/B対策Merge No.62
		//	if ( CheckSheetExist(jamShtNo) == true ){
			if ( (jamShtNo < SHEETNO_NUM) && (CheckSheetExist(jamShtNo) == true) ){		
				if ( CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[jamShtNo]), SHT_COND_CHG_EVT_JAM) != SHT_COND_NUM ){
					abortExec = true;
				}
				else{ /* No Action */ }
			}
			else{ /* No Action */ }

			// --- Search for abort top sheet ---
			ngOutDest = cSht_StopSheetSpec[devSttsNo].outDest;
			ngPathSnr = cSht_StopSheetSpec[devSttsNo].pathSnr;

			shtIdx = gSht_ManagerMember.mNextEjectShtIdx;

			while ( shtIdx != gSht_ManagerMember.mExitEntryShtIdx ){
				// --- ジャム原因紙停止 ---
				if ( (gSht_ManagerMember.mShtExtQue[shtIdx] == jamShtNo) &&		// Jam Sheet
					 (CShtSheet_GetCondition(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[shtIdx]])) == SHT_COND_JAM) ){
					break;
				}
				else{ /* No Action */ }

				// --- NG排紙先未排紙用紙停止 ---
				if ( (ngOutDest != eCdiFinOutDest_All) &&
					 (CShtSheet_GetOutputDestFinish(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[shtIdx]])) == ngOutDest) ){
					break;
				}
				else{ /* No Action */ }

				// --- NG搬送パス未通過用紙停止 ---
				if ( (ngPathSnr != SHT_PATH_SNR_ID_NUM) &&
					 (CShtManager_CheckPathSensorPlan(ngPathSnr, gSht_ManagerMember.mShtExtQue[shtIdx]) == true) &&
					 (CShtManager_CheckPathSensorArrival(ngPathSnr, gSht_ManagerMember.mShtExtQue[shtIdx], SHT_EDGE_TAIL) == false) ){
					break;
				}
				else{ /* No Action */ }

				shtIdx = IncrSheetIndex(shtIdx);
			}

			// --- Stop sheet ---
			if ( shtIdx != gSht_ManagerMember.mExitEntryShtIdx ){
				if ( StopSheet(shtIdx) == true ){
					abortExec = true;
				}
				else{ /* No Action */ }
			}
			else{ /* No Action */ }

			// --- Notify abort sheet ---
			if ( abortExec == true ){
				SendSheetAbortedNotify();
			}
			else{ /* No Action */ }
		}
		else{ /* No Action */ }

		if ( jamShtNo == SHEETNO_NUM ){
			UpdateAbnormalFinishing();
		}
		else{ /* No Action */ }
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : StopSheetHardDown
// Description   : ハードダウンでのシート制御停止処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void StopSheetHardDown()
{
	UC shtIdx;

	CShtManagerState_ChangeBlockDown(&(gSht_ManagerMember.mState));

	if ( gSht_ManagerMember.mExitEntryShtIdx != gSht_ManagerMember.mNextEjectShtIdx ){
																				// Managed sheet is exist.
		if ( CShtManagerState_GetState(&(gSht_ManagerMember.mState)) == SHT_MGR_STT_STANDBY ){
																				// Not Finishing Process
			shtIdx = gSht_ManagerMember.mNextEjectShtIdx;

			while ( shtIdx != gSht_ManagerMember.mExitEntryShtIdx ){
				if ( CShtSheet_CheckConditionDelivery(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[shtIdx]])) == true ){
					if ( StopSheet(shtIdx) == true ){
						SendSheetAbortedNotify();
					}
					else{ /* No Action */ }

					break;
				}
				else{ /* Go while loop */ }										// Not Delivery sheet

				shtIdx = IncrSheetIndex(shtIdx);
			}
		}
		else{																	// Finishing Process
			if ( StopSheet(gSht_ManagerMember.mNextEjectShtIdx) == true ){
				SendSheetAbortedNotify();
			}
			else{ /* No Action */ }
		}
	}
	else{ /* No Action */ }

	UpdateAbnormalFinishing();
}

///////////////////////////////////////////////////////////////////
// Function Name : AbortEjectWaitSheet
// Description   : 排紙待ちシート(コンパイルシート)のアボート処理
// Parameter     : ‐
// Return        : アボート処理実施結果
//               :  true = アボート対象シートあり(アボート処理実施)
//               :  false = アボート対象シートなし
///////////////////////////////////////////////////////////////////
UC AbortEjectWaitSheet()
{
	UC abortTop, shtIdx;

	abortTop = SHEETNO_NUM;

	if ( gSht_ManagerMember.mNextFinishShtIdx != gSht_ManagerMember.mNextDelvShtIdx ){
		// --- Abort sheet ---
		shtIdx = gSht_ManagerMember.mNextFinishShtIdx;

		while ( shtIdx != gSht_ManagerMember.mExitEntryShtIdx ){
			if ( CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[shtIdx]]), SHT_COND_CHG_EVT_ABORT) != SHT_COND_CHG_EVT_NUM ){
				if ( abortTop == SHEETNO_NUM ){
					abortTop = gSht_ManagerMember.mShtExtQue[shtIdx];
				}
				else{ /* No Action */ }
			}
			else{ /* No Action */ }

			shtIdx = IncrSheetIndex(shtIdx);
		}
	}
	else{ /* No Action */ }

	return abortTop;
}

///////////////////////////////////////////////////////////////////
// Function Name : SheetEventEjectFinishComp
// Description   : イジェクトフィニッシング完了通知受信時処理
// Parameter     : modulelID (Module ID)
//               : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SheetEventEjectFinishComp(UC modulelID, UC sheetNo)
{
	UC shtIdx, nextLocLead, nextLocTail, checkIdx;

	shtIdx = GetExitIndex(sheetNo);

	if ( shtIdx < SHEETNO_NUM ){
		nextLocLead = CShtSheet_GetNextLocation(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_EDGE_LEAD);
		nextLocTail = CShtSheet_GetNextLocation(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_EDGE_TAIL);

		if ( (nextLocLead >= SHT_LOC_ID_NUM) &&									// Already path ejected sheets.
			 (nextLocTail >= SHT_LOC_ID_NUM) ){
			if ( gSht_ManagerMember.mNextEjectShtIdx == shtIdx ){
				CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_COND_CHG_EVT_COMPLETE);

				checkIdx = IncrSheetIndex(shtIdx);
				gSht_ManagerMember.mNextEjectShtIdx = checkIdx;					// Next eject change from finished sheet.

				// Search eject sheet in delivered sheet.
				while ( checkIdx != gSht_ManagerMember.mNextDelvShtIdx ){		// Exist delivered sheet in after sheets of finished sheet.
					gSht_ManagerMember.mNextEjectShtIdx = checkIdx;

					if ( CShtSheet_GetEject(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[checkIdx]])) == SHT_EJECT_EXEC ){
						break;
					}
					else{ /* Go while loop */ }

					checkIdx = IncrSheetIndex(checkIdx);
				}

				if ( gSht_ManagerMember.mNextFinishShtIdx == shtIdx ){
					checkIdx = IncrSheetIndex(shtIdx);

					if ( CShtSheet_GetSetEnd(&(gSht_ManagerMember.mSheet[sheetNo])) == SHT_SET_END ){
						while ( checkIdx != gSht_ManagerMember.mNextEjectShtIdx ){
							if ( CShtSheet_GetSetEnd(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[checkIdx]])) == SHT_SET_END ){
								break;
							}
							else{ /* Go while loop */ }

							checkIdx = IncrSheetIndex(checkIdx);
						}

						gSht_ManagerMember.mNextFinishShtIdx = checkIdx;
					}
					else{														// Not Set End
						while ( checkIdx != gSht_ManagerMember.mNextDelvShtIdx ){
							gSht_ManagerMember.mNextFinishShtIdx = checkIdx;

							if ( (checkIdx == gSht_ManagerMember.mNextEjectShtIdx) ||
								 (CShtSheet_GetSetEnd(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[checkIdx]])) == SHT_SET_END) ){
								break;											// Fail Safe(Set End = Eject Sheet)
							}
							else{ /* Go while loop */ }

							checkIdx = IncrSheetIndex(checkIdx);
						}
					}
				}
				else{ /* No Action */ }
			}
			else{
				// No Action
			}
		}
		else{ /* No Action */ }

		if ( CheckConditionChangeReady() == true ){
			ProcessReadyInitialize();
			CShtManagerState_ChangeReady(&(gSht_ManagerMember.mState));
			CSysManager_SheetManagerReady();
		}
		else{
			// No Action
		}
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : SheetEventEjectStart
// Description   : イジェクト開始通知受信時処理
// Parameter     : modulelID (Module ID)
//               : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SheetEventEjectStart(UC modulelID, UC sheetNo)
{
	UC nextLocLead, nextLocTail;

	if ( CShtSheet_GetCondition(&(gSht_ManagerMember.mSheet[sheetNo])) == SHT_COND_EJECT ){
		nextLocLead = CShtSheet_GetNextLocation(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_EDGE_LEAD);
		nextLocTail = CShtSheet_GetNextLocation(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_EDGE_TAIL);

		if ( (nextLocLead < SHT_LOC_ID_NUM) && (nextLocTail < SHT_LOC_ID_NUM) ){
			CShtPathSensor_EntrySheet(&(gSht_ManagerMember.mPathSensor[cSht_PathSensorIDLocation[nextLocLead]]), sheetNo, SHT_EDGE_LEAD);
			CShtPathSensor_EntrySheet(&(gSht_ManagerMember.mPathSensor[cSht_PathSensorIDLocation[nextLocTail]]), sheetNo, SHT_EDGE_TAIL);

			while ( (nextLocLead < SHT_LOC_ID_NUM) && (CShtPathSensor_IsCompilePath(&(gSht_ManagerMember.mPathSensor[cSht_PathSensorIDLocation[nextLocLead]])) == true) ){
				CShtManager_PathSensorEdgeChange(nextLocLead, SHT_EDGE_LEAD);

				nextLocLead = CShtSheet_GetNextLocation(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_EDGE_LEAD);
			}

			while ( (nextLocTail < SHT_LOC_ID_NUM) && (CShtPathSensor_IsCompilePath(&(gSht_ManagerMember.mPathSensor[cSht_PathSensorIDLocation[nextLocTail]])) == true) ){
				if ( CShtPathSensor_GetActualLevel(&(gSht_ManagerMember.mPathSensor[cSht_PathSensorIDLocation[nextLocTail]])) == SHT_PATH_SNR_LVL_OFF ){
					CShtManager_PathSensorEdgeChange(nextLocTail, SHT_EDGE_TAIL);
																				// Paper set sensor of compile tray is Off(sheet's flexure).
				}
				else{															// Sensor level is ON
					break;
				}

				nextLocTail = CShtSheet_GetNextLocation(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_EDGE_TAIL);
			}
		}
		else{ /* No Action */ }
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : SheetEventCompileFinishComp
// Description   : コンパイルフィニッシング完了通知受信時処理
// Parameter     : modulelID (Module ID)
//               : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SheetEventCompileFinishComp(UC modulelID, UC sheetNo)
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : SheetEventPunchBackStart
// Description   : パンチバック開始通知受信時処理
// Parameter     : modulelID (Module ID)
//               : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SheetEventPunchBackStart(UC modulelID, UC sheetNo)
{
//	UC index;
//
//	for ( index = 0; index < SHT_PATH_SNR_ID_NUM; index++ ){
//		CShtPathSensor_FilterReverse(&(gSht_ManagerMember.mPathSensor[index]), ACT_START, SHT_REVERSE_KIND_PUNCH_BACK);
//	}
}

///////////////////////////////////////////////////////////////////
// Function Name : SheetEventPunchBackComp
// Description   : パンチバック完了通知受信時処理
// Parameter     : modulelID (Module ID)
//               : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SheetEventPunchBackComp(UC modulelID, UC sheetNo)
{
//	UC index;
//
//	for ( index = 0; index < SHT_PATH_SNR_ID_NUM; index++ ){
//		CShtPathSensor_FilterReverse(&(gSht_ManagerMember.mPathSensor[index]), ACT_STOP, SHT_REVERSE_KIND_PUNCH_BACK);
//	}
}
///////////////////////////////////////////////////////////////////
// Function Name : SheetEventEjectComp
// Description   : 
// Parameter     : 
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SheetEventEjectComp(UC modulelID, UC sheetNo)
{
	if ( (CShtSheet_GetSetEnd(&(gSht_ManagerMember.mSheet[sheetNo])) == SHT_SET_END) &&
		//V3.4.0
		// (CShtSheet_GetSetFinishedEvent(&(gSht_ManagerMember.mSheet[sheetNo])) == SHT_EVT_ID_EJECT_COMP) ){
		(GetSetFinishedEvent(sheetNo) == SHT_EVT_ID_EJECT_COMP) ){
		CCdiAdapter_SendSetFinished(sheetNo);
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : PutSheetLocationLog
// Description   : シートロケーションログ出力指示処理
// Parameter     : para1 (数字列の最初に付加する値)
//               : para2 (数字列の２番目に付加する値)
//               : para3 (数字列の３番目に付加する値)
//               : para4 (数字列の４番目に付加する値)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void PutSheetLocationLog(UC para1, UC para2, UC para3, UC para4)
{
	UC log[4];

	log[0] = para1;
	log[1] = para2;
	log[2] = para3;
	log[3] = para4;

	DD_LOG_Put(LOGID_SHTLOC, 'S', log, sizeof(log), LOG_CATEGORY_DEF);
}
///////////////////////////////////////////////////////////////////
// Function Name : PutSheetManagerLog
// Description   : シートロケーションログ出力指示処理
// Parameter     : para1 (数字列の最初に付加する値)
//               : para2 (数字列の２番目に付加する値)
//               : para3 (数字列の３番目に付加する値)
//               : para4 (数字列の４番目に付加する値)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void PutSheetManagerLog(UC para1, UC para2, US para3, US para4)
{
	UC log[6];

	log[0] = para1;
	log[1] = para2;
	log[2] = (UC) (para3 >>8 ) ;
	log[3] = (UC) (para3 & 0x00FF);	
	log[4] = (UC) (para4 >>8 ) ;
	log[5] = (UC) (para4 & 0x00FF);	

	DD_LOG_Put(LOGID_SHT_Mgr, 'S', log, sizeof(log), LOG_CATEGORY_DEBUG);
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_GetProcessStartSheet
// Description   : プロセススタートシートのSheet No.取得処理
// Parameter     : ‐
// Return        : プロセススタートシートのSheet No.
///////////////////////////////////////////////////////////////////
UC CShtManager_GetProcessStartSheet()
{
	return gSht_ManagerMember.mProcStaShtNo;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_GetPurgePossibility
// Description   : パージ可否判断処理
// Parameter     : ‐
// Return        : パージ可否判断結果
///////////////////////////////////////////////////////////////////
enum ECdiFinPurgePossibility CShtManager_GetPurgePossibility()
{
	switch ( CShtManagerState_GetState(&(gSht_ManagerMember.mState)) ){
	case SHT_MGR_STT_CYCLE_UP:
	case SHT_MGR_STT_READY:
	case SHT_MGR_STT_CYCLE_DOWN:
	case SHT_MGR_STT_PURGE_DOWN:
		return eCdiFinPurgePsbl_Possible;
	default:
		return eCdiFinPurgePsbl_Impossible;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_GetTopRequestSheet
// Description   : シートリクエストキュー内のトップに当たるシート(
//               : 最も古くに処理依頼を受理したシート)のSheet No.取
//               : 得処理
// Parameter     : ‐
// Return        : シートリクエストキュー内のトップに当たるシートの
//               : Sheet No.
///////////////////////////////////////////////////////////////////
UC CShtManager_GetTopRequestSheet()
{
	if ( gSht_ManagerMember.mNextFinishShtIdx != gSht_ManagerMember.mExitEntryShtIdx ) return gSht_ManagerMember.mShtExtQue[gSht_ManagerMember.mNextFinishShtIdx];
	else if ( gSht_ManagerMember.mReqEntryShtNo != gSht_ManagerMember.mReqTopShtNo )	 return gSht_ManagerMember.mReqTopShtNo;
	else																			 return SHEETNO_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_GetBottomRequestSheet
// Description   : シートリクエストキュー内のボトムに当たるシート(
//               : 最も新しく処理依頼を受理したシート)のSheet No.取
//               : 得処理
// Parameter     : ‐
// Return        : シートリクエストキュー内のボトムに当たるシートの
//               : Sheet No.
///////////////////////////////////////////////////////////////////
UC CShtManager_GetBottomRequestSheet()
{
	if ( gSht_ManagerMember.mReqEntryShtNo != gSht_ManagerMember.mReqTopShtNo )			  return DecrSheetIndex(gSht_ManagerMember.mReqEntryShtNo);
	else if ( gSht_ManagerMember.mNextFinishShtIdx != gSht_ManagerMember.mExitEntryShtIdx ) return gSht_ManagerMember.mShtExtQue[DecrSheetIndex(gSht_ManagerMember.mExitEntryShtIdx)];
	else																				  return SHEETNO_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_GetPrevRequestSheet
// Description   : シートリクエストキューにおける１つ前のシート(古
//               : くに指示されたシート)のSheet No.取得処理
// Parameter     : sheetNo (Sheet No.)
// Return        : シートリクエストキューにおける１つ前のシートの
//               : Sheet No.
///////////////////////////////////////////////////////////////////
UC CShtManager_GetPrevRequestSheet(UC sheetNo)
{
	UC checkShtNo = DecrSheetIndex(sheetNo);

	if ( (sheetNo < SHEETNO_NUM) && (CheckSheetExist(checkShtNo) == true) ) return checkShtNo;
	else																    return SHEETNO_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_GetNextRequestSheet
// Description   : シートリクエストキューにおける１つ後のシート(新
//               : しく指示されたシート)のSheet No.取得処理
// Parameter     : sheetNo (Sheet No.)
// Return        : シートリクエストキューにおける１つ後のシートの
//               : Sheet No.
///////////////////////////////////////////////////////////////////
UC CShtManager_GetNextRequestSheet(UC sheetNo)
{
	UC checkShtNo = IncrSheetIndex(sheetNo);

	if ( (sheetNo < SHEETNO_NUM) && (CheckSheetExist(checkShtNo) == true) ) return checkShtNo;
	else																	return SHEETNO_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_GetTopExitSheet
// Description   : シートイグジットキュー内のトップに当たるシート(
//               : 最も古くにShee Exitしたシート)のSheet No.取得処
//               : 理
// Parameter     : ‐
// Return        : Sheet No.
///////////////////////////////////////////////////////////////////
UC CShtManager_GetTopExitSheet()
{
	if ( gSht_ManagerMember.mNextFinishShtIdx != gSht_ManagerMember.mExitEntryShtIdx ) return gSht_ManagerMember.mShtExtQue[gSht_ManagerMember.mNextFinishShtIdx];
	else																			 return SHEETNO_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_GetBottomExitSheet
// Description   : シートイグジットキュー内のボトムに当たるシート(
//               : 最も新しくShee Exitしたシート)のSheet No.取得処
//               : 理
// Parameter     : ‐
// Return        : Sheet No.
///////////////////////////////////////////////////////////////////
UC CShtManager_GetBottomExitSheet()
{
	if ( gSht_ManagerMember.mNextFinishShtIdx != gSht_ManagerMember.mExitEntryShtIdx ) return gSht_ManagerMember.mShtExtQue[DecrSheetIndex(gSht_ManagerMember.mExitEntryShtIdx)];
	else																			 return SHEETNO_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_GetPrevExitSheet
// Description   : シートイグジットキューにおける１つ前のシート(古
//               : くにSheet Exitしたシート)のSheet No.取得処理
// Parameter     : sheetNo (Sheet No.)
// Return        : Sheet No.
///////////////////////////////////////////////////////////////////
UC CShtManager_GetPrevExitSheet(UC sheetNo)
{
	UC shtIdx;

	if ( (sheetNo < SHEETNO_NUM) && (gSht_ManagerMember.mNextFinishShtIdx != gSht_ManagerMember.mExitEntryShtIdx) ){
		shtIdx = gSht_ManagerMember.mNextFinishShtIdx;

		while ( shtIdx != gSht_ManagerMember.mExitEntryShtIdx ){
			if ( gSht_ManagerMember.mShtExtQue[shtIdx] == sheetNo ) {
				if ( shtIdx != gSht_ManagerMember.mNextFinishShtIdx ) return gSht_ManagerMember.mShtExtQue[DecrSheetIndex(shtIdx)];
				else												 break;
			}
			else{ /* Go while loop. */ }

			shtIdx = IncrSheetIndex(shtIdx);
		}
	}
	else{ /* Sheet don't exist. */ }

	return SHEETNO_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_GetNextExitSheet
// Description   : シートイグジットキューにおける１つ後のシート(新
//               : しくSheet Exitしたシート)のSheet No.取得処理
// Parameter     : sheetNo (Sheet No.)
// Return        : Sheet No.
///////////////////////////////////////////////////////////////////
UC CShtManager_GetNextExitSheet(UC sheetNo)
{
	UC shtIdx;

	if ( (sheetNo < SHEETNO_NUM) && (gSht_ManagerMember.mNextFinishShtIdx != gSht_ManagerMember.mExitEntryShtIdx) ){
		shtIdx = gSht_ManagerMember.mNextFinishShtIdx;

		while ( shtIdx != gSht_ManagerMember.mExitEntryShtIdx ){
			if ( gSht_ManagerMember.mShtExtQue[shtIdx] == sheetNo ) {
				shtIdx = IncrSheetIndex(shtIdx);
				if ( shtIdx != gSht_ManagerMember.mExitEntryShtIdx ) return gSht_ManagerMember.mShtExtQue[shtIdx];
				else												break;
			}
			else{ /* Go while loop. */ }

			shtIdx = IncrSheetIndex(shtIdx);
		}
	}
	else{ /* Sheet don't exist. */ }

	return SHEETNO_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_GetBufferPairSheet
// Description   : バッファ制御において対になるシートのSheet No.取
//               : 得処理
// Parameter     : sheetNo (Sheet No.)
// Return        : Sheet No.
///////////////////////////////////////////////////////////////////
UC CShtManager_GetBufferPairSheet(UC sheetNo)
{
	UC pairShtNo;

	if ( sheetNo < SHEETNO_NUM ){
		switch ( CShtSheet_GetBuffer(&(gSht_ManagerMember.mSheet[sheetNo])) ){
		case SHT_BUFFER_TOP:
			pairShtNo = IncrSheetIndex(sheetNo);

			if ( CheckSheetExist(pairShtNo) == true ){							// 前回使用時情報残り対応
				if ( CShtSheet_GetBuffer(&(gSht_ManagerMember.mSheet[pairShtNo])) == SHT_BUFFER_END ){
					return pairShtNo;
				}
				else{ /* Not exist pair sheet. */ }
			}
			else{ /* Not exist pair sheet. */ }

			break;
		case SHT_BUFFER_END:
			pairShtNo = DecrSheetIndex(sheetNo);
			if ( CShtSheet_GetBuffer(&(gSht_ManagerMember.mSheet[pairShtNo])) == SHT_BUFFER_TOP ){
				return pairShtNo;
			}
			else{ /* NG */ }

			break;
		default:																// eBufferSheet_Not
			break;
		}
	}
	else{ /* Sheet No. NG */ }

	return SHEETNO_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_CheckUndeliveredSheetExist
// Description   : 未デリバードシートの有無確認処理
// Parameter     : outDest (Output Destination ID)
// Return        : 有無確認結果
//               :  true = 未デリバードシートあり
//               :  false = 未デリバードシートなし
///////////////////////////////////////////////////////////////////
bool CShtManager_CheckUndeliveredSheetExist(UC outDest)
{
	UC shtIdx, sheetNo, state;

	// --- Check exit sheet ---
	if ( gSht_ManagerMember.mNextDelvShtIdx != gSht_ManagerMember.mExitEntryShtIdx ){
		shtIdx = gSht_ManagerMember.mNextDelvShtIdx;

		while ( shtIdx != gSht_ManagerMember.mExitEntryShtIdx ){
			if ( CShtSheet_CheckConditionDelivery(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[shtIdx]])) == true ){
				if ( (outDest == eCdiFinOutDest_All) ||
					 (outDest == CShtSheet_GetOutputDestFinish(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[shtIdx]]))) ){
					return true;
				}
				else{ /* Go while loop. */ }
			}
			else{ /* Go while loop. */ }

			shtIdx = IncrSheetIndex(shtIdx);
		}
	}
	else{ /* No Action */ }														// Exit sheet don't exist.

	// --- Check candidate sheet ---
	if ( gSht_ManagerMember.mReqEntryShtNo != gSht_ManagerMember.mReqTopShtNo ){
		state = CShtManagerState_GetState(&(gSht_ManagerMember.mState));
		if ( (state == SHT_MGR_STT_READY) ||
			 (state == SHT_MGR_STT_CYCLE_DOWN) ||
			 (state == SHT_MGR_STT_PURGE_DOWN) ){
			sheetNo = gSht_ManagerMember.mReqTopShtNo;

			while ( sheetNo != gSht_ManagerMember.mReqEntryShtNo ){
				if ( CShtSheet_GetCondition(&(gSht_ManagerMember.mSheet[sheetNo])) == SHT_COND_CANDIDATE ){
					if ( (outDest == eCdiFinOutDest_All) ||
						 (outDest == CShtSheet_GetOutputDestFinish(&(gSht_ManagerMember.mSheet[sheetNo]))) ){
						return true;
					}
					else{ /* Go while loop. */ }
				}
				else{ /* Go while loop. */ }

				sheetNo = IncrSheetIndex(sheetNo);
			}
		}
		else{ /* No Action */ }													// Candidate sheet don't care.
	}
	else{ /* No Action */ }														// Candidate sheet don't exist.

	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_CheckUnejectedSheetExist
// Description   : (コンパイルトレイからの)未排紙シートの有無確認処
//               : 理
// Parameter     : outDest (Output Destination ID)
// Return        : 有無確認結果
//               :  true = 未排紙シートあり
//               :  false = 未排紙シートなし
///////////////////////////////////////////////////////////////////
bool CShtManager_CheckUnejectedSheetExist(UC outDest)
{
	UC shtIdx;

	if ( gSht_ManagerMember.mNextEjectShtIdx != gSht_ManagerMember.mNextDelvShtIdx ){
		shtIdx = gSht_ManagerMember.mNextEjectShtIdx;

		while ( shtIdx != gSht_ManagerMember.mNextDelvShtIdx ){
			if ( CShtSheet_CheckConditionWaitEject(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[shtIdx]])) == true ){
				if ( (outDest == eCdiFinOutDest_All) ||
					 (outDest == CShtSheet_GetOutputDestFinish(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[shtIdx]]))) ){
					return true;
				}
				else{ /* Go while loop. */ }
			}
			else{  /* Go while loop. */ }

			shtIdx = IncrSheetIndex(shtIdx);
		}
	}
	else{ /* Unejected delivered sheet don't exist. */ }

	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_CheckUnfinishedSetExist
// Description   : 未フィニッシュセット有無確認処理
// Parameter     : outDest (Output Destination ID)
// Return        : 有無確認結果
//               :  true = 未フィニッシュセットあり
//               :  false = 未フィニッシュセットなし
///////////////////////////////////////////////////////////////////
bool CShtManager_CheckUnfinishedSetExist(UC outDest)
{
	UC shtIdx;

	if ( gSht_ManagerMember.mNextFinishShtIdx != gSht_ManagerMember.mNextDelvShtIdx ){
		shtIdx = gSht_ManagerMember.mNextFinishShtIdx;

		while ( shtIdx != gSht_ManagerMember.mNextDelvShtIdx ){
			if ( CShtSheet_CheckConditionWaitFinish(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[shtIdx]])) == true ){
				if ( (outDest == eCdiFinOutDest_All) ||
					 (outDest == CShtSheet_GetOutputDestFinish(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[shtIdx]]))) ){
					return true;
				}
				else{ /* Go while loop. */ }
			}
			else{  /* Go while loop. */ }

			shtIdx = IncrSheetIndex(shtIdx);
		}
	}
	else{ /* Unfinished delivered sheet don't exist. */ }

	return false;
}


///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_CheckUnfinishedSetExist_2
// Description   : 未フィニッシュセット有無確認処理
// Parameter     : outDest (Output Destination ID)
// Return        : 有無確認結果
//               :  true = 未フィニッシュセットあり
//               :  false = 未フィニッシュセットなし
///////////////////////////////////////////////////////////////////
bool CShtManager_CheckUnfinishedSetExist_2(UC outDest)
{
	UC shtIdx;

	if ( gSht_ManagerMember.mNextFinishShtIdx != gSht_ManagerMember.mNextDelvShtIdx ){
		shtIdx = gSht_ManagerMember.mNextFinishShtIdx;

		while ( shtIdx != gSht_ManagerMember.mNextDelvShtIdx ){
			if ( CShtSheet_CheckConditionSetFinishReceive(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[shtIdx]])) == true ){
				return true;
			}
			else{  /* Go while loop. */ }

			shtIdx = IncrSheetIndex(shtIdx);
		}
	}
	else{ /* Unfinished delivered sheet don't exist. */ }

	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_GetNextArriveSheet
// Description   : 指定パスセンサに次に到達予定のシート取得処理
// Parameter     : pathSnrID (Path Sensor ID)
//               : edge (シートエッジ識別子)
// Return        : Sheet No. [0 ～ SHEETNO_NUM]
///////////////////////////////////////////////////////////////////
UC CShtManager_GetNextArriveSheet(EShtPathSensorID pathSnrID, EShtSheetEdge edge)
{
	if ( (pathSnrID < SHT_PATH_SNR_ID_NUM) && (edge < SHT_EDGE_NUM) ){
		return CShtPathSensor_GetEntrySheet(&(gSht_ManagerMember.mPathSensor[pathSnrID]), edge);
	}
	else{ /* No Action */ }

	return SHEETNO_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_CheckPathSensorPlan
// Description   : 指定シートのパスセンサ通過予定確認処理
// Parameter     : pathSnrID (Path Sensor ID)
//               : sheetNo (Sheet No.)
// Return        : 通過予定確認結果
//               :  true = 通過予定あり
//               :  false = 通過予定なし
///////////////////////////////////////////////////////////////////
bool CShtManager_CheckPathSensorPlan(EShtPathSensorID pathSnrID, UC sheetNo)
{
	UC index;

	for ( index = 0; index < SHT_LOC_ID_NUM; index++ ){
		if ( pathSnrID == cSht_PathSensorIDLocation[index] ){
			if ( CShtSheet_CheckLocationPlan(&(gSht_ManagerMember.mSheet[sheetNo]), index) == true ){
				return true;
			}
			else{
				// Go for loop
			}
		}
		else{ /* No Action */ }
	}

	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_CheckPathSensorArrival
// Description   : 指定シートのパスセンサ到達状況確認処理
// Parameter     : pathSnrID (Path Sensor ID)
//               : sheetNo (Sheet No.)
//               : edge (シートエッジ識別子)
// Return        : true = 到達済み
//               : false = 未到達
///////////////////////////////////////////////////////////////////
bool CShtManager_CheckPathSensorArrival(EShtPathSensorID pathSnrID, UC sheetNo, EShtSheetEdge edge)
{
	UC location;

	for ( location = 0; location < SHT_LOC_ID_NUM; location++ ){
		if ( pathSnrID == cSht_PathSensorIDLocation[location] ){
			if ( CShtSheet_CheckLocationArrival(&(gSht_ManagerMember.mSheet[sheetNo]), location, edge) == false ){
				return false;
			}
			else{
				// Go for loop
			}
		}
		else{ /* No Action */ }
	}

	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_GetLateDetectLocationFilterTime
// Description   : 指定シートのシート端時間待ち確定処理時のフィルタ
//               : 時間取得処理
// Parameter     : pathSnrID (Path Sensor ID)
//               : sheetNo (Sheet No.)
//               : edge (シートエッジ識別子)
// Return        : ‐
///////////////////////////////////////////////////////////////////
UC CShtManager_GetLateDetectLocationFilterTime(EShtPathSensorID pathSnrID, UC sheetNo, EShtSheetEdge edge)
{
	if ( (pathSnrID < SHT_PATH_SNR_ID_NUM) && (edge < SHT_EDGE_NUM) ){
		return CShtPathSensor_GetLateDetectFilterTime(&(gSht_ManagerMember.mPathSensor[pathSnrID]), edge);
	}
	else{ /* No Action */ }

	return 0;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_GetSheetNo
// Description   : Sheet IDからのSheet No.取得処理
// Parameter     : sheetID (Sheet ID)
// Return        : シート識別子(Sheet No.)
///////////////////////////////////////////////////////////////////
UC CShtManager_GetSheetNo(UC sheetID)
{
	UC sheetNo;
	UC checkShtNo;

	//↓↓↓↓ TB No.71. Delivered済みの用紙のSheetIDが再利用できない	
	// --- 検索対象範囲決定 ---
	if ( gSht_ManagerMember.mNextFinishShtIdx != gSht_ManagerMember.mExitEntryShtIdx ) { sheetNo = gSht_ManagerMember.mShtExtQue[gSht_ManagerMember.mNextFinishShtIdx]; }
	else if ( gSht_ManagerMember.mReqTopShtNo != gSht_ManagerMember.mReqEntryShtNo )   { sheetNo = gSht_ManagerMember.mReqTopShtNo; }
	else																			   { sheetNo = gSht_ManagerMember.mReqEntryShtNo; }

	// --- Sheet ID一致シート検索(Deliveredした用紙のSheet IDは再利用されるため(用紙管理内に同一Sheet ID存在ケースあり)新しく生成されたシートから検索する) ---
	checkShtNo = gSht_ManagerMember.mReqEntryShtNo;

	while ( checkShtNo != sheetNo ){
		checkShtNo = DecrSheetIndex(checkShtNo);

		if ( CShtSheet_GetSheetID(&(gSht_ManagerMember.mSheet[checkShtNo])) == sheetID ){
			return checkShtNo;
		}
		else{ /* Go while loop */ }
	}

/*	while( sheetNo != gSht_ManagerMember.mReqEntryShtNo ){
		if( CShtSheet_GetSheetID(&(gSht_ManagerMember.mSheet[sheetNo])) == sheetID )		return sheetNo;
		else			sheetNo = IncrSheetIndex(sheetNo);		
	}*/

	return SHEETNO_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_GetSheet
// Description   : シートインスタンス取得処理
// Parameter     : sheetNo (Sheet No.)
// Return        : シートインスタンスポインタ
///////////////////////////////////////////////////////////////////
const CShtSheet* CShtManager_GetSheet(UC sheetNo)
{
	if ( sheetNo < SHEETNO_NUM ) return &(gSht_ManagerMember.mSheet[sheetNo]);
	else						 return NULL;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_ConvFinisherSpeed
// Description   : フィニッシャー搬送基本速度のSpeed IDから速度値へ
//               : の変換処理
// Parameter     : speedID (Speed ID)
// Return        : フィニッシャー搬送基本速度[×0.1mm/s]
///////////////////////////////////////////////////////////////////
US CShtManager_ConvFinisherSpeed(EShtFinisherSpeedID speedID)
{
 US FinSpeed;
	if ( speedID == SHT_FIN_SPD_ID_IOT ) {
		FinSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V5)*10;// Dummy
	} else if ( speedID == SHT_FIN_SPD_ID_NVMV3_600 )  {
		FinSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V3)*10;//
	} else if ( speedID == SHT_FIN_SPD_ID_NVMV4_700  )  {
		FinSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V4)*10;//
	} else if ( speedID == SHT_FIN_SPD_ID_NVMV5_800  )  {
		FinSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V5)*10;//
	}
	return FinSpeed;
	// if ( speedID < SHT_FIN_SPD_ID_NUM ) return cSht_FinisherSpeed[speedID];
	// else								return cSht_FinisherSpeed[0];
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_ConvOffsetPosition
// Description   : オフセット位置判断結果情報からOffset Positionへ
//               : の変換処理
// Parameter     : ofstFin (Offset Finish Position)
// Return        : Offset Position
///////////////////////////////////////////////////////////////////
enum ECdiFinOffsetPos CShtManager_ConvOffsetPosition(EShtOffsetFinish ofstFin)
{
	if( ofstFin < SHT_OFST_FIN_NUM ) return (enum ECdiFinOffsetPos)(cSht_OfstPosFromFinPos[ofstFin]);
	else							  return eCdiFinOffsetPos_Left;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_ProcessStart
// Description   : フィニッシングプロセス開始時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtManager_ProcessStart()
{
	UC prevShtNo;

	if ( CShtManagerState_ChangeCycleUp(&(gSht_ManagerMember.mState)) == true ){
		if ( gSht_ManagerMember.mNextDelvShtIdx != gSht_ManagerMember.mNextFinishShtIdx ){
																				// セット処理残りあり
			prevShtNo = gSht_ManagerMember.mShtExtQue[DecrSheetIndex(gSht_ManagerMember.mNextDelvShtIdx)];
		}
		else{																	// セット処理残りなし
			prevShtNo = SHEETNO_NUM;
		}

		CShtInitializeManager_Start();
		//V3.4.0 T/B対策Merge( No.44)
		//CShtInitializeManager_DetectInitExecution(gSht_ManagerMember.mProcStaShtNo, SHEETNO_NUM);
		CShtInitializeManager_DetectInitExecution(gSht_ManagerMember.mProcStaShtNo, prevShtNo);
		CShtInitializeManager_SheetRequest(gSht_ManagerMember.mProcStaShtNo, prevShtNo);

		if ( CheckConditionChangeReady() == true ){
			ProcessReadyInitialize();
			CShtManagerState_ChangeReady(&(gSht_ManagerMember.mState));
			CSysManager_SheetManagerReady();
		}
		else{
			// No Action
		}
	}
	else{
		// NG
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_ProcessEnd
// Description   : フィニッシングプロセス終了時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtManager_ProcessEnd()
{
	CShtManagerState_ChangeStandby(&(gSht_ManagerMember.mState));
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_StopProcess
// Description   : フィニッシングプロセス停止(シート処理の停止)開始
//               : 時の処理
// Parameter     : stopType (フィニッシングプロセス停止種別)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtManager_StopProcess(ESysProcStopType stopType)
{
	if ( stopType == SYS_PROC_STOP_CYCLE ){
		// --- シートイニシャライズ動作穏当停止 --- V3.1.7
		CShtInitializeManager_CycleDown();
	}
	else{																		// Hard Down
		StopSheetHardDown();
		// --- シートイニシャライズ動作緊急停止 ---V3.1.7
		CShtInitializeManager_HardDown();
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_CycleDownFaultStop
// Description   : 停止種別がサイクルダウンのフォルトが発生した際の
//               : シート処理停止処理
// Parameter     : devSttsNo (Device Status No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtManager_CycleDownFaultStop(US devSttsNo)
{
	// --- Stop transport sheet ---
	StopSheetCycleDown(devSttsNo, SHEETNO_NUM);

	// --- シートイニシャライズ動作穏当停止 --- V3.1.7 シートイニシャライズ中 CycleDown 
	CShtInitializeManager_CycleDown();
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_HardDownFaultStop
// Description   : 停止種別がハードダウンのフォルトが発生した際のシ
//               : ート処理停止処理
// Parameter     : devSttsNo (Device Status No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtManager_HardDownFaultStop(US devSttsNo)
{
	// --- Stop transport sheet ---
	StopSheetHardDown();

	// --- シートイニシャライズ動作緊急停止 --- V3.1.7 シートイニシャライズ中 HardDown 
	CShtInitializeManager_HardDown();
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_DynamicJamStop
// Description   : ダイナミックジャム発生時のシート処理停止処理
// Parameter     : devSttsNo (Device Status No.)
//               : jamShtNo (ジャム原因紙のSheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtManager_DynamicJamStop(US devSttsNo, UC jamShtNo)
{
	StopSheetCycleDown(devSttsNo, jamShtNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_CreateProcessStartSheet
// Description   : プロセススタートシート生成指示受信処理
// Parameter     : outDest (Output Destination ID)
//               : binSlct (Bin Selection)
//               : pShtAttr (シートアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
UC CShtManager_CreateProcessStartSheet(UC outDest, UC binSlct, CShtSheetAttribute* pShtAttr)
{
	UC prevShtNo;

	if ( CShtManagerState_GetState(&(gSht_ManagerMember.mState)) == SHT_MGR_STT_STANDBY ){
		// --- プロセス開始前情報構築 ---
		gSht_ManagerMember.mLastPurgePsbl  = true;
		gSht_ManagerMember.mLastAbortShtNo = SHEETNO_NUM;
		gSht_ManagerMember.mLocPlanCfgMask = CreateLocationPlanConfigMask();

		// --- シート管理情報 ---
		if ( gSht_ManagerMember.mNextDelvShtIdx != gSht_ManagerMember.mNextFinishShtIdx ){
																				// セット処理残りあり
			prevShtNo = gSht_ManagerMember.mShtExtQue[DecrSheetIndex(gSht_ManagerMember.mNextDelvShtIdx)];

			gSht_ManagerMember.mReqTopShtNo = IncrSheetIndex(prevShtNo);
		}
		else{																	// セット処理残りなし
			prevShtNo = SHEETNO_NUM;
		}

		gSht_ManagerMember.mReqEntryShtNo = gSht_ManagerMember.mReqTopShtNo;
		gSht_ManagerMember.mExitEntryShtIdx = gSht_ManagerMember.mNextDelvShtIdx;

		gSht_ManagerMember.mProcStaShtNo = gSht_ManagerMember.mReqEntryShtNo;

		CShtSheet_Reset(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mReqEntryShtNo]));

		CShtSheet_SetOutputDestination(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mReqEntryShtNo]), outDest);
		CShtSheet_SetBinSelection(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mReqEntryShtNo]), binSlct);
		CShtSheet_SetAttribute(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mReqEntryShtNo]), pShtAttr);

		CShtSheet_SetOutputDestFinish(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mReqEntryShtNo]), outDest);

		if ( prevShtNo == SHEETNO_NUM ){										// Previous sheet don't exist.
			CShtSheet_SetSetTop(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mReqEntryShtNo]), SHT_SET_TOP);
		}
		else{
			if ( CShtSheet_GetSetEnd(&(gSht_ManagerMember.mSheet[prevShtNo])) == SHT_SET_END ){
				CShtSheet_SetSetTop(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mReqEntryShtNo]), SHT_SET_TOP);
			}
			else{
				// --- 先行紙排紙条件チェック ---
				if ( CShtSheet_GetEject(&(gSht_ManagerMember.mSheet[prevShtNo])) == SHT_EJECT_NOT ){
					if ( JudgeEjectNextSheetReq(prevShtNo, gSht_ManagerMember.mReqEntryShtNo) == SHT_EJECT_EXEC ){
						CShtSheet_SetEject(&(gSht_ManagerMember.mSheet[prevShtNo]), SHT_EJECT_EXEC);

						if ( CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[prevShtNo]), SHT_COND_CHG_EVT_EJECT_DETECT) == SHT_COND_REMAIN ){
							// このタイミングでRemainになることは想定外(ディバイディッド対象となる枚数積まれた状態でLeaveにはならない)
						}
						else{ /* No Action */ }									// Normal Case(No Change or Eject)

						if ( CShtSheet_GetSetEnd(&(gSht_ManagerMember.mSheet[prevShtNo])) == SHT_SET_END_NOT ){
							CShtSheet_SetDevidedSet(&(gSht_ManagerMember.mSheet[prevShtNo]), SHT_SET_DEVIDED);
						}
						else{ /* No Action */ }

						SendSheetAttributeUpdate(prevShtNo);
					}
					else{ /* No Action */ }										// This sheet's eject judgement is "eSheetEject_NotExec".
				}
				else{ /* No Action */ }											// Previous sheet's eject judgement is "eSheetEject_Exec".
			}
		}

		CreateSheetInformation(gSht_ManagerMember.mReqEntryShtNo, prevShtNo);

		return gSht_ManagerMember.mReqEntryShtNo;
	}
	else{																		// Sheet Manager isn't Standby.
		return SHEETNO_NUM;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_CreateSheet
// Description   : シート生成指示受信処理
// Parameter     : sheetID (Sheet ID)
//               : setID (Set ID)
//               : outDest (Output Destination ID)
//               : binSlct (Bin Selection)
//               : pShtAttr (シートアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
UC CShtManager_CreateSheet(UC sheetID, UC setID, UC outDest, UC binSlct, CShtSheetAttribute* pShtAttr)
{
	CShtPreparationTime prepTime;
	ShtInitTimeInfo initTimeInfo;
	US resTime, shortenTime, bufferInTime, bufferFeedWaitTime;
	UC index, state, entShtNo, prevShtNo, ngDevice;

	CShtPreparationTime_Constructor(&prepTime);

	// --- シート管理内先行シート取得 ---
	if ( (gSht_ManagerMember.mReqEntryShtNo == gSht_ManagerMember.mProcStaShtNo) &&
		 (CheckSheetExist(SHEETNO_NUM) == false) ){								// Process start sheet & managed sheet don't exist.
		prevShtNo = SHEETNO_NUM;
	}
	else{																		// Previous sheet exist.
		prevShtNo = DecrSheetIndex(gSht_ManagerMember.mReqEntryShtNo);
	}

	if ( (prevShtNo == SHEETNO_NUM) ||											// 重連対応(受理済みSheet通知対応)
		 (CShtSheet_GetSheetID(&(gSht_ManagerMember.mSheet[prevShtNo])) != sheetID) ||
		 (CShtSheet_GetCondition(&(gSht_ManagerMember.mSheet[prevShtNo])) != SHT_COND_CANDIDATE) ){
			 																	// セットキャンセルリカバリー対応
		entShtNo = gSht_ManagerMember.mReqEntryShtNo;

		CShtSheet_Reset(&(gSht_ManagerMember.mSheet[entShtNo]));

		// --- 用紙情報(IOT指定)セット ---
		CShtSheet_SetSheetID(&(gSht_ManagerMember.mSheet[entShtNo]), sheetID);
		CShtSheet_SetSetID(&(gSht_ManagerMember.mSheet[entShtNo]), setID);
		CShtSheet_SetOutputDestination(&(gSht_ManagerMember.mSheet[entShtNo]), outDest);
		CShtSheet_SetBinSelection(&(gSht_ManagerMember.mSheet[entShtNo]), binSlct);
		CShtSheet_SetAttribute(&(gSht_ManagerMember.mSheet[entShtNo]), pShtAttr);

		// --- 用紙情報(フィニッシャー判断)セット ---
		state = CShtManagerState_GetState(&(gSht_ManagerMember.mState));

		if ( (state == SHT_MGR_STT_CYCLE_UP) || (state == SHT_MGR_STT_READY) ){
			CShtSheet_SetOutputDestFinish(&(gSht_ManagerMember.mSheet[entShtNo]), outDest);

			if ( prevShtNo != SHEETNO_NUM ){									// Previous sheet exist.
				// --- セット残り(異常)チェック ---
				if ( (CShtSheet_GetSetEnd(&(gSht_ManagerMember.mSheet[prevShtNo])) == SHT_SET_END_NOT) &&
					 (CShtSheetAttribute_GetSetAction(pShtAttr) == eCdiFinSetAct_NewSet) ){
	
						CSysManager_FaultStatusChange(DSNO_LOGIC_FAIL, FAULT_OCCUR);
	
				}
				else{
					// No Action
				}

				// --- 先行紙排紙条件チェック ---
				if ( CShtSheet_GetEject(&(gSht_ManagerMember.mSheet[prevShtNo])) == SHT_EJECT_NOT ){
					if ( JudgeEjectNextSheetReq(prevShtNo, entShtNo) == SHT_EJECT_EXEC ){
						CShtSheet_SetEject(&(gSht_ManagerMember.mSheet[prevShtNo]), SHT_EJECT_EXEC);

						if ( CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[prevShtNo]), SHT_COND_CHG_EVT_EJECT_DETECT) == SHT_COND_REMAIN ){
							// このタイミングでRemainになることは想定外(ディバイディッド対象となる枚数積まれた状態でLeaveにはならない)
						}
						else{ /* No Action */ }									// Normal Case(No Change or Eject)

						if ( CShtSheet_GetSetEnd(&(gSht_ManagerMember.mSheet[prevShtNo])) == SHT_SET_END_NOT ){
							CShtSheet_SetDevidedSet(&(gSht_ManagerMember.mSheet[prevShtNo]), SHT_SET_DEVIDED);
						}
						else{ /* No Action */ }

						SendSheetAttributeUpdate(prevShtNo);
					}
					else{ /* No Action */ }										// This sheet's eject judgement is "eSheetEject_NotExec".
				}
				else{ /* No Action */ }											// Previous sheet's eject judgement is "eSheetEject_Exec".
			}
			else{ /* No Action */ }

			if ( CShtSheetAttribute_GetSetAction(pShtAttr) == eCdiFinSetAct_NewSet ){
				CShtSheet_SetSetTop(&(gSht_ManagerMember.mSheet[entShtNo]), SHT_SET_TOP);
			}
			else{ /* No Action */ }

			CreateSheetInformation(entShtNo, prevShtNo);

			// --- シートイニシャライズ判断&実施 ---
			initTimeInfo = CShtInitializeManager_DetectInitExecution(entShtNo, prevShtNo);
			CShtInitializeManager_SheetRequest(entShtNo, prevShtNo);

			// --- バッファ制御判断 ---
			CShtSheet_SetBuffer(&(gSht_ManagerMember.mSheet[entShtNo]), JudgeBuffer(entShtNo));

			// --- 搬送計画セット ---
			CShtSheet_SetLocationPlan(&(gSht_ManagerMember.mSheet[entShtNo]), CreateSheetLocationPlan(entShtNo));
		}
		else{ /* No Action */ }													// Sheet Manager State is "Standby" or "XXX Down".

		// --- Preparation Time計算 ---
		if ( CShtManagerState_GetState(&(gSht_ManagerMember.mState)) == SHT_MGR_STT_READY ){
			ngDevice = CheckDeviceCondition(entShtNo);

			//if ( ngDevice == DEVID_NUM ){										// All device condition is OK.
			if ((ngDevice == DEVID_NUM) && 
				(OFF == CShtManager_GetPitchSkip()) ){										// All device condition is OK.
			
				if ( initTimeInfo.waitTime != 0xFFFF ){
					if ( entShtNo != gSht_ManagerMember.mProcStaShtNo ){			// Not process start sheet.
						CShtPreparationTime_Calculate(&prepTime, entShtNo, prevShtNo, initTimeInfo);
						resTime = CShtPreparationTime_GetPreparationTime(&prepTime);

						if ( CShtSheet_GetBuffer(&(gSht_ManagerMember.mSheet[entShtNo])) == SHT_BUFFER_TOP ){
							shortenTime = CShtPreparationTime_GetShortenTime(&prepTime);

							bufferFeedWaitTime = GetBufferFeedWaitTime(entShtNo, prevShtNo, shortenTime);

							CShtSheet_SetBufferFeedWaitTime(&(gSht_ManagerMember.mSheet[entShtNo]), bufferFeedWaitTime);
						}
						else{ /* No Action */ }
					}
					else{														// Process start sheet.
						resTime = eCdiFinPrepTime_NoWait;
					}

					if ( resTime > 60000 ){// 60秒超え
					
						//	resTime = eCdiFinPrepTime_ConsiTime;
						//↓↓↓↓ TB No.72 Considerable Timeから復帰できない
						if ( (CShtManager_CheckUndeliveredSheetExist(eCdiFinOutDest_All) == true) ||
																			// 未デリバード用紙あり(先行紙がデリバードしていない)
							 ( (CShtManager_CheckUnejectedSheetExist(eCdiFinOutDest_All) == true) &&
				   				(CShtSheet_GetEject(&(gSht_ManagerMember.mSheet[prevShtNo])) == SHT_EJECT_EXEC) ) ||
																			// 未排紙用紙があり(先行紙がコンパイル済み)、先行紙が排紙対象である(排紙中)
				 				(CShtInitializeManager_CheckComplete(entShtNo, SHT_INIT_ID_NUM) == false) ){
																			// 対象シートにおけるイニシャライズ動作が完了していない
								resTime = eCdiFinPrepTime_ConsiTime;
						}
						else{															// 全ての準備が完了(すぐに用紙を受け入れ可能)
							resTime = eCdiFinPrepTime_NoWait;
						}
						// ↑↑↑↑↑
					}
					else{ // 60秒以下
						// No Action
					}

					if ( gSht_ManagerMember.mProcStaShtNo == entShtNo ){
						for ( index = 0; index < SHT_INIT_ID_NUM; index++ ){
							if ( initTimeInfo.sheetInitTime[index] != 0 ){
								resTime = eCdiFinPrepTime_Unknown;

								break;
							}
							else{
								// Go for loop.
							}
						}
					}
					else{ /* No Action */ }
				}
				else{															// Sheet initialization time is unknown.
					resTime = eCdiFinPrepTime_Unknown;
				}
			}
			else{	//DeviceNG or  pitchskip ..(V3.6.92)															// Device condition is "No Good".
				resTime = eCdiFinPrepTime_Unknown;
				PutSheetManagerLog(0xcc,gSht_ManagerMember.mPitchSkip, eCdiFinPrepTime_Unknown, eCdiFinPrepTime_Unknown );
			}	
		}
		else{																	// Sheet manager is reject sheet request state.
			resTime = eCdiFinPrepTime_Unknown;
		}
		
		CShtSheet_SetPrepTime(&(gSht_ManagerMember.mSheet[entShtNo]), resTime);

		if ( (resTime != eCdiFinPrepTime_Unknown) && (resTime != eCdiFinPrepTime_ConsiTime) ){
		//V3.4.0 Merge (ShtOutDestInfoTable内容をCShtSheetに格納するのを削除して直接テーブル参照)
		//	CShtSheet_SetDeliveredLocation(&(gSht_ManagerMember.mSheet[entShtNo]), GetDeliveredLocation(entShtNo));
		//	CShtSheet_SetSetFinishedLocation(&(gSht_ManagerMember.mSheet[entShtNo]), GetSetFinishedLocation(entShtNo));
		//	CShtSheet_SetSetFinishedEvent(&(gSht_ManagerMember.mSheet[entShtNo]), GetSetFinishedEvent(entShtNo));
			CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[entShtNo]), SHT_COND_CHG_EVT_ENTRY);
			SendSheetAttributeEntry(entShtNo);
			gSht_ManagerMember.mReqEntryShtNo = IncrSheetIndex(gSht_ManagerMember.mReqEntryShtNo);
		}
		else{
			// No Action(Turn down sheet entry request.)
		}

		return entShtNo;
	}
	else{
		return prevShtNo;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_UpdateSheetAttribute
// Description   : シートアトリビュート更新指示受信時処理
// Parameter     : sheetID (Sheet ID)
//               : pShtAttr (シートアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtManager_UpdateSheetAttribute(UC sheetID, CShtSheetAttribute* pShtAttr)
{
	CShtSheetAttribute *pPosAttr;
	UC sheetNo, integrity, shtIdx;

	sheetNo = CShtManager_GetSheetNo(sheetID);

	if ( sheetNo != SHEETNO_NUM ){
		pPosAttr = CShtSheet_GetAttribute(&(gSht_ManagerMember.mSheet[sheetNo]));

		integrity = CShtSheetAttribute_GetSheetIntegrity(pPosAttr);

		CShtSheet_SetAttribute(&(gSht_ManagerMember.mSheet[sheetNo]), pShtAttr);

		// Sheet Integrity変更によるPurge判断
		if ( (CShtSheetAttribute_GetSheetIntegrity(pShtAttr) == eCdiFinIntegrity_Abnormal) &&
			 (CShtSheetAttribute_GetSheetIntegrity(pShtAttr) != integrity) ){
			shtIdx = GetExitIndex(sheetNo);

			if ( shtIdx != SHEETNO_NUM ){										// 未Sheet Exit用紙はNo Action
				if ( cSht_ManagerSpec.purgeOutDestID != eCdiFinOutDest_All ) CShtManagerState_ChangePurgeDown(&(gSht_ManagerMember.mState));
				else														 CShtManagerState_ChangeBlockDown(&(gSht_ManagerMember.mState));

				if ( StopSheet(shtIdx) == true ){
					SendSheetAbortedNotify();
				}
				else{ /* No Action */ }

				UpdateAbnormalFinishing();
			}
			else{ /* No Action */ }
		}
		else{ /* No Action */ }

		SendSheetAttributeUpdate(sheetNo);
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_CancelSheet
// Description   : シートキャンセル指示受信処理
// Parameter     : sheetID (キャンセル対象の先頭シートのSheet ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
bool CShtManager_CancelSheet(UC sheetID)
{
	UC result, abortExec, sheetNo, checkShtNo, shtIdx;

	result = true;
	abortExec = false;
	sheetNo = CShtManager_GetSheetNo(sheetID);

	if ( sheetNo != SHEETNO_NUM ){
		if ( CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_COND_CHG_EVT_CANCEL) == SHT_COND_CANCEL ){
			checkShtNo = sheetNo;
			while ( checkShtNo != gSht_ManagerMember.mReqEntryShtNo ){
				CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[checkShtNo]), SHT_COND_CHG_EVT_CANCEL);

				checkShtNo = IncrSheetIndex(checkShtNo);
			}

			gSht_ManagerMember.mReqEntryShtNo = sheetNo;

			CShtInitializeManager_SheetCancel(sheetNo);

			abortExec = true;
		}
		else{																	// Disable cancel condition
			CShtManagerState_ChangeBlockDown(&(gSht_ManagerMember.mState));

			shtIdx = GetExitIndex(sheetNo);

			if ( shtIdx < SHEETNO_NUM ){
				abortExec = StopSheet(shtIdx);
			}
			else{ /* No Action */ }

			result = false;
		}
	}
	else{																		// Not find cancel sheet.
		if ( gSht_ManagerMember.mNextFinishShtIdx != gSht_ManagerMember.mExitEntryShtIdx ){
			CShtManagerState_ChangeBlockDown(&(gSht_ManagerMember.mState));

			abortExec = StopSheet(gSht_ManagerMember.mNextFinishShtIdx);			// Stop all sheet

			result = false;
		}
		else{ /* No Action */ }
	}

	if ( abortExec == true ){
		SendSheetAbortedNotify();
	}
	else{ /* No Action */ }

	UpdateAbnormalFinishing();

	return result;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_SetEnd
// Description   : セットエンド指示受信時処理
// Parameter     : setEndAct (Set End Action)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtManager_SetEnd(enum ECdiFinSetEndAct setEndAct)
{
	UC sheetNo, setEndShtNo, lastDelvShtNo, nextLocLead, nextLocTail, deliverdLoc, finishedLoc, finishedEvt, shtIdx, checkIdx;

	if ( CheckSheetExist(SHEETNO_NUM) == true ){								// Managed sheet exist
		// --- セットエンド対象シート検索 ---
		sheetNo = DecrSheetIndex(gSht_ManagerMember.mReqEntryShtNo);

		if ( CShtSheet_CheckConditionNormal(&(gSht_ManagerMember.mSheet[sheetNo])) == true ){
			setEndShtNo = sheetNo;
		}
		else if ( setEndAct == eCdiFinSetEndAct_AbortSet ){
			// --- 正常シート検索処理 ---
			setEndShtNo = SHEETNO_NUM;

			while ( CheckSheetExist(sheetNo) == true ){
				if ( CShtSheet_CheckConditionNormal(&(gSht_ManagerMember.mSheet[sheetNo])) == true ){
					setEndShtNo = sheetNo;
					break;
				}
				else{ /* Go while loop. */ }

				sheetNo = DecrSheetIndex(sheetNo);
			}

			// --- セット処理途中異常シートのアボート処理 ---
			if ( (setEndShtNo == SHEETNO_NUM) &&
				 (gSht_ManagerMember.mNextFinishShtIdx != gSht_ManagerMember.mNextDelvShtIdx) ){
				lastDelvShtNo = gSht_ManagerMember.mShtExtQue[DecrSheetIndex(gSht_ManagerMember.mNextDelvShtIdx)];

				CShtSheet_SetSetEnd(&(gSht_ManagerMember.mSheet[lastDelvShtNo]), SHT_SET_END);
																				// 次のプロセスに引き継がないために最後にデリバードしたシートをセットエンド&イジェクト対象にする
				CShtSheet_SetEject(&(gSht_ManagerMember.mSheet[lastDelvShtNo]), SHT_EJECT_EXEC);
																				// 制御不要であるため部品には通知しない
			}
			else{ /* No Action */ }
		}
		else{
			setEndShtNo = SHEETNO_NUM;
		}

		if ( setEndShtNo != SHEETNO_NUM ){
			if ( CShtSheet_GetSetEnd(&(gSht_ManagerMember.mSheet[setEndShtNo])) == SHT_SET_END_NOT ){
																				// Fail Safe Check
				// --- Update sheet information ---
				CShtSheet_SetSetEndAction(&(gSht_ManagerMember.mSheet[setEndShtNo]), setEndAct);
				CShtSheet_SetSetEnd(&(gSht_ManagerMember.mSheet[setEndShtNo]), SHT_SET_END);
				//V3.4.0 Merge (No.40)
				if ( (CShtManagerState_GetState(&(gSht_ManagerMember.mState)) == SHT_MGR_STT_READY) ||
					 ( (CShtManagerState_GetState(&(gSht_ManagerMember.mState)) == SHT_MGR_STT_STANDBY) &&
					   (CCvrChecker_GetCoverStatus(CVR_ID_NUM) == CVR_STTS_CLOSE) &&
					   (CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_CLEAR)
					  )){// &&
//					   (CJamChecker_GetScratchSheetCompileStatus() == FAULT_NOT_OCCUR) ) ){
						CShtSheet_SetEject(&(gSht_ManagerMember.mSheet[setEndShtNo]), SHT_EJECT_EXEC);
						CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[setEndShtNo]), SHT_COND_CHG_EVT_EJECT_DETECT);
				}
				else{ /* No Action */ }

				if ( CShtSheet_CheckConditionWaitFinish(&(gSht_ManagerMember.mSheet[setEndShtNo])) == true ){
					// --- セットフィニッシュ処理 ---
					if ( setEndShtNo == gSht_ManagerMember.mShtExtQue[gSht_ManagerMember.mNextFinishShtIdx] ){
						nextLocLead = CShtSheet_GetNextLocation(&(gSht_ManagerMember.mSheet[setEndShtNo]), SHT_EDGE_LEAD);
						nextLocTail = CShtSheet_GetNextLocation(&(gSht_ManagerMember.mSheet[setEndShtNo]), SHT_EDGE_TAIL);
						deliverdLoc	= GetDeliveredLocation(setEndShtNo);
						finishedLoc = GetSetFinishedLocation(setEndShtNo);

						if ( (nextLocLead >= SHT_LOC_ID_NUM) &&					// Already path ejected sheets.
							 (nextLocTail >= SHT_LOC_ID_NUM) ){
							if ( (CShtSheet_CheckEventOccurence(&(gSht_ManagerMember.mSheet[setEndShtNo]), SHT_EVT_ID_EJECT_FINISH_COMP) == true) ||
								 (deliverdLoc == finishedLoc) ){				// Delivered Point is the same as Set Finished Point.
								shtIdx = GetExitIndex(setEndShtNo);
								checkIdx = IncrSheetIndex(shtIdx);

								while ( checkIdx != gSht_ManagerMember.mNextEjectShtIdx ){
									if ( CShtSheet_GetSetEnd(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[checkIdx]])) == SHT_SET_END ){
										break;
									}
									else{ /* Go while loop */ }

									checkIdx = IncrSheetIndex(checkIdx);
								}

								gSht_ManagerMember.mNextFinishShtIdx = checkIdx;
							}
							else{ /* No Action */ }
						}
						else{ /* No Action */ }

						// --- Notify Set Finished ---
						finishedEvt = GetSetFinishedEvent(setEndShtNo);
						if ( ( (finishedEvt != SHT_EVT_ID_NUM) &&
							   (CShtSheet_CheckEventOccurence(&(gSht_ManagerMember.mSheet[setEndShtNo]), finishedEvt) == true) ) ||
							 ( (finishedLoc != SHT_LOC_ID_NUM) &&
							   (CShtSheet_CheckLocationArrival(&(gSht_ManagerMember.mSheet[setEndShtNo]), finishedLoc, SHT_EDGE_TAIL) == true) ) ){
							CCdiAdapter_SendSetFinished(setEndShtNo);
						}
						else{ /* No Action */ }
					}
					else{ /* No Action */ }
				}
				else if ( CShtSheet_GetCondition(&(gSht_ManagerMember.mSheet[setEndShtNo])) == SHT_COND_REMAIN ){
																				// Change sheet condition to "Remain" from "Leave".
					CJamChecker_DropEjectSheet();
					SendSheetAbortedNotify();
				}
				else{ /* No Action */ }

				SendSheetAttributeUpdate(setEndShtNo);
			}
			else{
				// Unexpected Set End(Already end)
			}
		}
		else{
			// Unexpected Set End(Not Available)
		}
	}
	else{
		// Unexpected Set End(No Sheet)
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_ExecuteSheet
// Description   : シート制御開始(Sheet Exit)指示受信時の処理
// Parameter     : sheetID (Sheet ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtManager_ExecuteSheet(UC sheetID)
{
	UC abortExec;
	UC sheetNo, nextLocLead, nextLocTail;

	abortExec = false;
	sheetNo = CShtManager_GetSheetNo(sheetID);

	if ( sheetNo != SHEETNO_NUM ){
		switch ( CShtManagerState_GetState(&(gSht_ManagerMember.mState)) ){
		case SHT_MGR_STT_READY:
		case SHT_MGR_STT_CYCLE_DOWN:
			if ( CShtSheetAttribute_GetSheetIntegrity(CShtSheet_GetAttribute(&(gSht_ManagerMember.mSheet[sheetNo]))) == eCdiFinIntegrity_Normal ){
				CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_COND_CHG_EVT_NORMAL_EXIT);
			
			}
			else{																// Abnormal
				if ( cSht_ManagerSpec.purgeOutDestID != eCdiFinOutDest_All ){
					CShtManagerState_ChangePurgeDown(&(gSht_ManagerMember.mState));

					if ( CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_COND_CHG_EVT_ABNORMAL_EXIT) == SHT_COND_PURGE ){
						CShtSheet_SetLocationPlan(&(gSht_ManagerMember.mSheet[sheetNo]), CreatePurgeLocationPlan(sheetNo));
						CShtSheet_SetOutputDestFinish(&(gSht_ManagerMember.mSheet[sheetNo]), cSht_ManagerSpec.purgeOutDestID);

						abortExec = true;
					}
					else{ /* No Action */ }
				}
				else{
					CShtManagerState_ChangeBlockDown(&(gSht_ManagerMember.mState));

					if ( CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_COND_CHG_EVT_STOP) != SHT_COND_NUM ){
						abortExec = true;
					}
					else{ /* No Action */ }
				}
			}

			break;
		case SHT_MGR_STT_PURGE_DOWN:
			if ( CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_COND_CHG_EVT_ABNORMAL_EXIT) == SHT_COND_PURGE ){
				CShtSheet_SetLocationPlan(&(gSht_ManagerMember.mSheet[sheetNo]), CreatePurgeLocationPlan(sheetNo));
				CShtSheet_SetOutputDestFinish(&(gSht_ManagerMember.mSheet[sheetNo]), cSht_ManagerSpec.purgeOutDestID);

				abortExec = true;
			}
			else{ /* No Action */ }

			break;
		default:																// Standby/CycleUp/BlockDown
			if ( CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_COND_CHG_EVT_STOP) != SHT_COND_NUM ){
				abortExec = true;
			}
			else{ /* No Action */ }
		}

		if ( sheetNo == gSht_ManagerMember.mReqTopShtNo ){						// Sheet Finish Requestの順番通りにSheet Exitを受信したら
			gSht_ManagerMember.mReqTopShtNo = IncrSheetIndex(gSht_ManagerMember.mReqTopShtNo);

			while ( gSht_ManagerMember.mReqTopShtNo != gSht_ManagerMember.mReqEntryShtNo ){
																				// 既に後続Entry SheetのSheet Exitを受信している場合があるため、未Sheet Exit用紙を検索
				if ( CShtSheet_GetCondition(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mReqTopShtNo])) == SHT_COND_CANDIDATE ){
					break;
				}
				else{ /* Go While Loop */ }

				gSht_ManagerMember.mReqTopShtNo = IncrSheetIndex(gSht_ManagerMember.mReqTopShtNo);
			}
		}
		else{ /* No Action */ }													// Sheet Finish Requestの順番通りでない場合はインデックス更新しない

		gSht_ManagerMember.mShtExtQue[gSht_ManagerMember.mExitEntryShtIdx] = sheetNo;
		gSht_ManagerMember.mExitEntryShtIdx = IncrSheetIndex(gSht_ManagerMember.mExitEntryShtIdx);

		if ( CShtSheet_CheckConditionDelivery(&(gSht_ManagerMember.mSheet[sheetNo])) == true ){
																				// 搬送対象となったら
			nextLocLead = CShtSheet_GetNextLocation(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_EDGE_LEAD);
			CShtPathSensor_EntrySheet(&(gSht_ManagerMember.mPathSensor[cSht_PathSensorIDLocation[nextLocLead]]), sheetNo, SHT_EDGE_LEAD);
			nextLocTail = CShtSheet_GetNextLocation(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_EDGE_TAIL);
			CShtPathSensor_EntrySheet(&(gSht_ManagerMember.mPathSensor[cSht_PathSensorIDLocation[nextLocTail]]), sheetNo, SHT_EDGE_TAIL);
		}
		else{ /* No Action */ }													// 搬送対象外

		if ( gSht_ManagerMember.mProcStaShtNo == sheetNo ){
			gSht_ManagerMember.mProcStaShtNo = SHEETNO_NUM;
		}
		else{ /* No Action */ }													// Process Start Sheetではない

		CShtInitializeManager_SheetExecution(sheetNo);

		if ( abortExec == true ){
			SendSheetAbortedNotify();
		}
		else{ /* No Action */ }

		if ( (abortExec == true) || (CShtSheet_GetCondition(&(gSht_ManagerMember.mSheet[sheetNo])) == SHT_COND_CANCEL) ){
			UpdateAbnormalFinishing();
		}
		else{ /* No Action */ }

		PutSheetLocationLog(sheetNo, 0xFF, sheetID, CShtSheet_GetCondition(&(gSht_ManagerMember.mSheet[sheetNo])));
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_SheetEnd
// Description   : 実行中のフィニッシングプロセスにおけるシート指示
//               : 終了通知受信時の処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtManager_SheetEnd()
{
	UC sheetNo, abortExec;

	CShtManagerState_ChangeCycleDown(&(gSht_ManagerMember.mState));

	gSht_ManagerMember.mProcStaShtNo = SHEETNO_NUM;

	abortExec = false;

	sheetNo = gSht_ManagerMember.mReqTopShtNo;
	while ( sheetNo != gSht_ManagerMember.mReqEntryShtNo ){
		if ( CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_COND_CHG_EVT_CANCEL) != SHT_COND_NUM ){
			abortExec = true;
		}
		else{ /* No Action */ }

		sheetNo = IncrSheetIndex(sheetNo);
	}

	if ( abortExec == true ){
		SendSheetAbortedNotify();
	}
	else{ /* No Action */ }

	CShtInitializeManager_End();
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_AbortCompileSheetProcessEnd
// Description   : プロセスエンド時点でのフォルト発生状況による用紙
//               : アボート処理
// Parameter     : ‐
// Return        : true = アボート実施/false = アボート実施せず
///////////////////////////////////////////////////////////////////
bool CShtManager_AbortCompileSheetProcessEnd()
{
	JudgeAbortCompileResult result;
	UC lastDelvShtNo;

	result = JudgeAbortCompileSheetProcessEnd();

	if ( result.abortExec == true ){
		if ( AbortEjectWaitSheet() != SHEETNO_NUM ){							// In case
			SendSheetAbortedNotify();
		}
		else{ /* No Action */ }

		lastDelvShtNo = gSht_ManagerMember.mShtExtQue[DecrSheetIndex(gSht_ManagerMember.mNextDelvShtIdx)];

		CShtSheet_SetEject(&(gSht_ManagerMember.mSheet[lastDelvShtNo]), SHT_EJECT_EXEC);
																				// 制御不要であるためイジェクト対象となったことを部品には通知しない
		if ( result.setRecoverySheetNo != SHEETNO_NUM ){
			CShtSheet_SetSetEnd(&(gSht_ManagerMember.mSheet[lastDelvShtNo]), SHT_SET_END);
																				// 次のプロセスに引き継がないために最後にデリバードしたシートをセットエンド＆イジェクト対象にする
			CCdiAdapter_SendSetRecoveryRequest(result.setRecoverySheetNo);
		}
		else{ /* No Action */ }
	}
	else{ /* No Action */ }

	return result.abortExec;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_AbortCompileSheetCoverClose
// Description   : カバークローズ時の用紙アボート指示受信時処理
// Parameter     : coverID (Cover ID)
// Return        : true = アボート実施/false = アボート実施せず
///////////////////////////////////////////////////////////////////
bool CShtManager_AbortCompileSheetCoverClose(ECvrCoverID coverID)
{
	JudgeAbortCompileResult result;
	UC lastDelvShtNo;

	if ( CShtManager_CheckUndeliveredSheetExist(eCdiFinOutDest_All) == false ){
		result = JudgeAbortCompileSheetCoverClose(coverID);

		if ( result.abortExec == true ){
			if ( AbortEjectWaitSheet() != SHEETNO_NUM ){						// In case
				SendSheetAbortedNotify();
			}
			else{ /* No Action */ }

			lastDelvShtNo = gSht_ManagerMember.mShtExtQue[DecrSheetIndex(gSht_ManagerMember.mNextDelvShtIdx)];

			CShtSheet_SetEject(&(gSht_ManagerMember.mSheet[lastDelvShtNo]), SHT_EJECT_EXEC);
																				// 制御不要であるためイジェクト対象となったことを部品には通知しない
			if ( result.setRecoverySheetNo != SHEETNO_NUM ){
				CShtSheet_SetSetEnd(&(gSht_ManagerMember.mSheet[lastDelvShtNo]), SHT_SET_END);
																				// 次のプロセスに引き継がないために最後にデリバードしたシートをセットエンド＆イジェクト対象にする
				CCdiAdapter_SendSetRecoveryRequest(result.setRecoverySheetNo);
			}
			else{ /* No Action */ }
		}
		else{ /* No Action */ }

		return result.abortExec;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_AbortCompileSheetDeviceModuleNG
// Description   : デバイスモジュール判断の用紙除去が必要な事象発生
//               : 時の用紙アボート指示受信時処理
// Parameter     : moduleID (デバイスモジュールのModule ID)
// Return        : true = アボート実施/false = アボート実施せず
///////////////////////////////////////////////////////////////////
bool CShtManager_AbortCompileSheetDeviceModuleNG(UC moduleID)
{
	JudgeAbortCompileResult result;
	UC lastDelvShtNo;

	if ( CShtManager_CheckUndeliveredSheetExist(eCdiFinOutDest_All) == false ){
		result = JudgeAbortCompileSheetDeviceModuleNG(moduleID);

		if ( result.abortExec == true ){
			if ( AbortEjectWaitSheet() != SHEETNO_NUM ){						// In case
				SendSheetAbortedNotify();
			}
			else{ /* No Action */ }

			lastDelvShtNo = gSht_ManagerMember.mShtExtQue[DecrSheetIndex(gSht_ManagerMember.mNextDelvShtIdx)];

			CShtSheet_SetEject(&(gSht_ManagerMember.mSheet[lastDelvShtNo]), SHT_EJECT_EXEC);
																				// 制御不要であるためイジェクト対象となったことを部品には通知しない
			if ( result.setRecoverySheetNo != SHEETNO_NUM ){
				CShtSheet_SetSetEnd(&(gSht_ManagerMember.mSheet[lastDelvShtNo]), SHT_SET_END);
																				// 次のプロセスに引き継がないために最後にデリバードしたシートをセットエンド＆イジェクト対象にする
				CCdiAdapter_SendSetRecoveryRequest(result.setRecoverySheetNo);
			}
			else{ /* No Action */ }
		}
		else{ /* No Action */ }

		return result.abortExec;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_StandbyEject
// Description   : フィニッシングプロセス停止時の排紙判断指示受信時
//               : の処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtManager_StandbyEject()
{
	
	if ( CShtManager_CheckUnejectedSheetExist(eCdiFinOutDest_All) == true ){
		if ( JudgeEjectStandby(gSht_ManagerMember.mShtExtQue[gSht_ManagerMember.mNextEjectShtIdx]) == SHT_EJECT_EXEC ){
			CShtSheet_SetEject(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[gSht_ManagerMember.mNextEjectShtIdx]]), SHT_EJECT_EXEC);

			SRVC_UserDebugLog_Wide(0x80, 0x80, 
									gSht_ManagerMember.mShtExtQue[gSht_ManagerMember.mNextEjectShtIdx], 
									gSht_ManagerMember.mShtExtQue[gSht_ManagerMember.mNextEjectShtIdx]);
			
			if ( CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[gSht_ManagerMember.mNextEjectShtIdx]]), SHT_COND_CHG_EVT_EJECT_DETECT) == SHT_COND_REMAIN ){
				CJamChecker_DropEjectSheet();
			}
			else{ /* No Action */ }

			SendSheetAttributeUpdate(gSht_ManagerMember.mShtExtQue[gSht_ManagerMember.mNextEjectShtIdx]);
		}
		else{ /* No Action */ }
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_SendSheetActionRequest
// Description   : Sheet Action Requestイベント送信依頼受信時処理
// Parameter     : eventID (MSP Event ID)
//               : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtManager_SendSheetActionRequest(US eventID, UC sheetNo)
{
	EvtParamSheetActionRequest evtParameter;

	if ( sheetNo < SHEETNO_NUM ){
		evtParameter.sheetNo = sheetNo;

		MSP_SendMessage(MID_SYS, eventID, (const UC *)(&evtParameter), sizeof(evtParameter), LOG_OFF);
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_ReceiveSheetActionResponse
// Description   : Sheet Action Responseイベント受信時処理
// Parameter     : moduleID (発行モジュールのModule ID)
//               : eventID (MSP Event ID)
//               : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtManager_ReceiveSheetActionResponse(UC moduleID, US eventID, UC sheetNo)
{

	if ( sheetNo < SHEETNO_NUM ){
		// --- Sheet Action Response受信時処理 ---
		switch ( eventID ){
		// !!! Product Specific !!!
//		case EVT_SHT_RES_XXXX:
//			XXXX;

//			break;
		default:
			break;
		}
		// --- シートイニシャライズ動作処理 ---
		CShtInitializeManager_SheetAction(sheetNo, eventID, moduleID);

		if ( CheckConditionChangeReady() == true ){
			ProcessReadyInitialize();
			CShtManagerState_ChangeReady(&(gSht_ManagerMember.mState));
			CSysManager_SheetManagerReady();
		}
		else{
			// No Action
		}

		// --- CDI Message送信 ---
		CCdiAdapter_SendSheetActionResponse(sheetNo, eventID);
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_ReceiveSheetEventNotify
// Description   : Sheet Event Notifyイベント発行受信時処理。
// Parameter     : moduleID (発行モジュールのModule ID)
//               : eventID (MSP Event ID)
//               : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtManager_ReceiveSheetEventNotify(UC moduleID, US eventID, EShtSheetEventID event, UC sheetNo)
{
	UC shtEvtID;

	if ( sheetNo < SHEETNO_NUM ){
		if( event < SHT_EVT_ID_NUM ){
			CShtSheet_SetEvent(&(gSht_ManagerMember.mSheet[sheetNo]), event);

			switch (event){
			case SHT_EVT_ID_EJECT_FINISH_COMP:	SheetEventEjectFinishComp(moduleID, sheetNo);	break;
			case SHT_EVT_ID_EJECT_START:		SheetEventEjectStart(moduleID, sheetNo);		break;
			case SHT_EVT_ID_COMPILE_FINISH_COMP:SheetEventCompileFinishComp(moduleID, sheetNo);	break;
			// !!! Product Specific !!!
			// case SHT_EVT_ID_PNCH_BACK_START:	SheetEventPunchBackStart(moduleID, sheetNo);	break;
			// case SHT_EVT_ID_PNCH_BACK_COMP:		SheetEventPunchBackComp(moduleID, sheetNo);		break;
			// case SHT_EVT_ID_XXX:
				break;
			case SHT_EVT_ID_EJECT_COMP:			SheetEventEjectComp(moduleID, sheetNo);			break;
			default:
				break;
			}

			CShtInitializeManager_SheetEvent(sheetNo, event);
			CJamChecker_SheetEvent(sheetNo, event);
			SRVC_Moview_SetPaperAction(event, sheetNo);
		}
		else{ /* No Action */ }

		// --- CDI Message送信 ---
		CCdiAdapter_SendSheetEventNotify(sheetNo, eventID);
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_PathSensorEdgeChange
// Description   : パスセンサー変化通知受信時処理
// Parameter     : pathSnrID (Path Sensor ID)
//               : edge (シートエッジ識別子)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtManager_PathSensorEdgeChange(EShtPathSensorID pathSnrID, EShtSheetEdge edge)
{
	EvtParamSheetDeliveredNotify evtParameter;
	UC sheetNo, shtIdx, shtCond;
	UC checkShtNo, checkIdx;
	UC afterShtNo, afterShtIdx, afterShtCond;
	UC location, nextLoc, finishedLoc;

	sheetNo = CShtPathSensor_InformedEdge(&(gSht_ManagerMember.mPathSensor[pathSnrID]), edge);
	shtIdx = GetExitIndex(sheetNo);

	if ( shtIdx < SHEETNO_NUM ){												// Waiting sensor edge sheet exist.
		location = CShtSheet_GetNextLocation(&(gSht_ManagerMember.mSheet[sheetNo]), edge);

		shtCond = CShtSheet_GetCondition(&(gSht_ManagerMember.mSheet[sheetNo]));

		if ( shtCond == SHT_COND_DELIVERY ){
			// --- Update sheet information ---
			checkIdx = shtIdx;
			checkShtNo = gSht_ManagerMember.mShtExtQue[checkIdx];

			while ( (checkShtNo == sheetNo) ||
					( (CShtPathSensor_IsBufferPath(&(gSht_ManagerMember.mPathSensor[pathSnrID])) == true) &&	// Edge sensor is buffer path.
					  (CShtSheet_GetBuffer(&(gSht_ManagerMember.mSheet[sheetNo])) == SHT_BUFFER_TOP) &&		// Current sheet is buffer top.
					  (CShtSheet_GetBuffer(&(gSht_ManagerMember.mSheet[checkShtNo])) == SHT_BUFFER_END) &&
					  (CShtSheet_CheckLocationPlan(&(gSht_ManagerMember.mSheet[checkShtNo]), location) == true) &&
					  (CShtSheet_GetCondition(&(gSht_ManagerMember.mSheet[checkShtNo])) == SHT_COND_DELIVERY) ) ){
				CShtSheet_SetLocation(&(gSht_ManagerMember.mSheet[checkShtNo]), location, edge);

				if ( location == GetDeliveredLocation(checkShtNo) ){
																				// Buffer時のSensor OFF
					if ( edge == SHT_EDGE_TAIL ){								// Tail Edge(Delivered Timing)
						finishedLoc = GetSetFinishedLocation(checkShtNo);
						if ( checkIdx != gSht_ManagerMember.mNextDelvShtIdx ){	// Go out of order
							// 想定順番とは異なるデリバードシート発生
						}
						else{ /* No Action */ }

						// --- Update sheet condition ---
						if ( location != finishedLoc ){							// Delivered Point isn't the same as Set Finished Point.(Compile tay exist)
							CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[checkShtNo]), SHT_COND_CHG_EVT_DELIVERED);

							if ( CShtSheet_GetEject(&(gSht_ManagerMember.mSheet[checkShtNo])) == SHT_EJECT_EXEC ){
								if ( CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[checkShtNo]), SHT_COND_CHG_EVT_EJECT_DETECT) == SHT_COND_REMAIN ){
									// このタイミングでRemainになることは想定外(ディバイディッド対象となる枚数積まれた状態でLeaveにはならない)
								}
								else{ /* No Action */ }							// Normal Case(No Change)
							}
							else{ /* No Action */ }
						}
						else{
							CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[checkShtNo]), SHT_COND_CHG_EVT_COMPLETE);
						}

						// --- Update sheet index ---
						gSht_ManagerMember.mNextDelvShtIdx = IncrSheetIndex(checkIdx);

						// --- Notify sheet delivered ---
						CCdiAdapter_SendSheetDelivered(checkShtNo);

						if ( location != finishedLoc ){							// Not Set Finished Point
							if ( CShtSheet_GetEject(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[gSht_ManagerMember.mNextEjectShtIdx]])) == SHT_EJECT_NOT ){
								gSht_ManagerMember.mNextEjectShtIdx = checkIdx;
							}
							else{ /* No Action */ }

							if ( checkIdx == gSht_ManagerMember.mNextEjectShtIdx ){
																				// Delivered sheet is next eject sheet.
								if ( CShtSheet_GetSetEnd(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[gSht_ManagerMember.mNextFinishShtIdx]])) == SHT_SET_END_NOT ){
									gSht_ManagerMember.mNextFinishShtIdx = checkIdx;
								}
								else{ /* No Action */ }
							}
							else{ /* No Action */ }
						}
						else{													// Set Finished Point
							// --- コンパイルトレイなし搬送時のシート管理情報更新 ---
							gSht_ManagerMember.mNextEjectShtIdx  = gSht_ManagerMember.mNextDelvShtIdx;
																				// Current sheet ejected.
							gSht_ManagerMember.mNextFinishShtIdx = checkIdx;		// Current sheet is next finish.

							if ( CShtSheet_GetSetEnd(&(gSht_ManagerMember.mSheet[checkShtNo])) == SHT_SET_END ){
								gSht_ManagerMember.mNextFinishShtIdx = gSht_ManagerMember.mNextDelvShtIdx;
																				// Next finish sheet change next dekivered sheet.(Not exist remain set.)
								CCdiAdapter_SendSetFinished(checkShtNo);
							}
							else{ /* No Action */ }								// Not Set End
						}

						CShtInitializeManager_SheetDelivered(checkShtNo);
						evtParameter.sheetNo = checkShtNo;
						MSP_SendMessage(MID_SYS, EVT_SHT_DELIVERED, (const UC *)(&evtParameter), sizeof(evtParameter), LOG_OFF);
					}
					else{ /* No Action */ }
				}
				else{															// Not delivered point
					nextLoc = CShtSheet_GetNextLocation(&(gSht_ManagerMember.mSheet[checkShtNo]), edge);
					if ( nextLoc < SHT_LOC_ID_NUM ){							// Next path exist.
						if ( (CShtSheet_GetBuffer(&(gSht_ManagerMember.mSheet[checkShtNo])) != SHT_BUFFER_END) ||
							 (CShtSheet_CheckLocationArrival(&(gSht_ManagerMember.mSheet[DecrSheetIndex(checkShtNo)]), nextLoc, edge) == true ) ){
								 												// Avoid overtaking entry by buffer follow sheet.
																				// If buffer follow sheet is good condition, Previous sheet always exist.
							CShtPathSensor_EntrySheet(&(gSht_ManagerMember.mPathSensor[cSht_PathSensorIDLocation[nextLoc]]), checkShtNo, edge);
						}
						else{ /* No Action */ }
					}
					else{ /* No Action */ }
				}

				CShtInitializeManager_SheetLocation(checkShtNo, edge, location);
				CJamChecker_SheetLocation(checkShtNo, edge, location);
				SendSheetLocationNotify(checkShtNo, edge, location);
				PutSheetLocationLog(checkShtNo, location, edge, SHT_COND_DELIVERY);
				SRVC_Moview_SetPaperLocation(edge, location, checkShtNo);

				checkIdx = IncrSheetIndex(checkIdx);

				if ( checkIdx == gSht_ManagerMember.mExitEntryShtIdx ) break;	// Posterior sheet don't exist.

				checkShtNo = gSht_ManagerMember.mShtExtQue[checkIdx];
			}

			// --- After sheet entry to path sensor ---
			afterShtIdx = shtIdx;

			while ( afterShtIdx != gSht_ManagerMember.mExitEntryShtIdx ){
				afterShtNo = gSht_ManagerMember.mShtExtQue[afterShtIdx];
				afterShtCond = CShtSheet_GetCondition(&(gSht_ManagerMember.mSheet[afterShtNo]));

				if ( (afterShtCond == SHT_COND_DELIVERY) || (afterShtCond == SHT_COND_PURGE) ){
					if ( cSht_PathSensorIDLocation[CShtSheet_GetNextLocation(&(gSht_ManagerMember.mSheet[afterShtNo]), edge)] == pathSnrID ){
						CShtPathSensor_EntrySheet(&(gSht_ManagerMember.mPathSensor[pathSnrID]), afterShtNo, edge);
						break;
					}
					else { /* No Action */ }
				}
				else { /* No Action */ }

				afterShtIdx = IncrSheetIndex(afterShtIdx);
			}
		}
		else if ( shtCond == SHT_COND_PURGE ){
			// --- Update sheet information ---
			CShtSheet_SetLocation(&(gSht_ManagerMember.mSheet[sheetNo]), location, edge);
			nextLoc = CShtSheet_GetNextLocation(&(gSht_ManagerMember.mSheet[sheetNo]), edge);

			if ( nextLoc < SHT_LOC_ID_NUM ){
				CShtPathSensor_EntrySheet(&(gSht_ManagerMember.mPathSensor[cSht_PathSensorIDLocation[nextLoc]]), sheetNo, edge);
			}
			else{																// Delivered Point
				if ( edge == SHT_EDGE_TAIL ){									// Delivered Timing
					CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_COND_CHG_EVT_COMPLETE);
					CCdiAdapter_SendSheetDelivered(sheetNo);
					CShtInitializeManager_SheetDelivered(sheetNo);
					evtParameter.sheetNo = sheetNo;
					MSP_SendMessage(MID_SYS, EVT_SHT_DELIVERED, (const UC *)(&evtParameter), sizeof(evtParameter), LOG_OFF);
				}
				else{ /* No Action */ }
			}

			CShtInitializeManager_SheetLocation(sheetNo, edge, location);
			CJamChecker_SheetLocation(sheetNo, edge, location);
			SendSheetLocationNotify(sheetNo, edge, location);
			PutSheetLocationLog(sheetNo, location, edge, SHT_COND_PURGE);
			SRVC_Moview_SetPaperLocation(edge, location, sheetNo);

			// --- After sheet entry to path sensor ---
			afterShtIdx = IncrSheetIndex(shtIdx);

			while ( afterShtIdx != gSht_ManagerMember.mExitEntryShtIdx ){
				afterShtNo = gSht_ManagerMember.mShtExtQue[afterShtIdx];
				afterShtCond = CShtSheet_GetCondition(&(gSht_ManagerMember.mSheet[afterShtNo]));

				if ( (afterShtCond == SHT_COND_DELIVERY) || (afterShtCond == SHT_COND_PURGE) ){
					if ( cSht_PathSensorIDLocation[CShtSheet_GetNextLocation(&(gSht_ManagerMember.mSheet[afterShtNo]), edge)] == pathSnrID ){
						CShtPathSensor_EntrySheet(&(gSht_ManagerMember.mPathSensor[pathSnrID]), afterShtNo, edge);
						break;
					}
					else { /* No Action */ }
				}
				else { /* No Action */ }

				afterShtIdx = IncrSheetIndex(afterShtIdx);
			}
		}
		else if ( shtCond == SHT_COND_EJECT ){
			// --- Update sheet information ---
			CShtSheet_SetLocation(&(gSht_ManagerMember.mSheet[sheetNo]), location, edge);

			nextLoc = CShtSheet_GetNextLocation(&(gSht_ManagerMember.mSheet[sheetNo]), edge);

			if ( nextLoc < SHT_LOC_ID_NUM ){									// Next path exist.
				CShtPathSensor_EntrySheet(&(gSht_ManagerMember.mPathSensor[cSht_PathSensorIDLocation[nextLoc]]), sheetNo, edge);
			}
			else{																// Next path don't exist.
				if ( edge == SHT_EDGE_TAIL ){
					if ( gSht_ManagerMember.mNextEjectShtIdx == shtIdx ){
						if ( CShtSheet_CheckEventOccurence(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_EVT_ID_EJECT_FINISH_COMP) == true ){
							CShtSheet_ChangeCondition(&(gSht_ManagerMember.mSheet[sheetNo]), SHT_COND_CHG_EVT_COMPLETE);

							checkIdx = IncrSheetIndex(shtIdx);
							gSht_ManagerMember.mNextEjectShtIdx = checkIdx;		// Next eject change from finished sheet.

							// Search eject sheet in delivered sheet.
							while ( checkIdx != gSht_ManagerMember.mNextDelvShtIdx ){
																				// Exist delivered sheet in after sheets of finished sheet.
								gSht_ManagerMember.mNextEjectShtIdx = checkIdx;

								if ( CShtSheet_GetEject(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[checkIdx]])) == SHT_EJECT_EXEC ){
									break;
								}
								else{ /* Go while loop */ }

								checkIdx = IncrSheetIndex(checkIdx);
							}

							if ( gSht_ManagerMember.mNextFinishShtIdx == shtIdx ){
																				// Edge sheet is next finish sheet.
								checkIdx = IncrSheetIndex(shtIdx);

								if ( CShtSheet_GetSetEnd(&(gSht_ManagerMember.mSheet[sheetNo])) == SHT_SET_END ){
									while ( checkIdx != gSht_ManagerMember.mNextEjectShtIdx ){
										if ( CShtSheet_GetSetEnd(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[checkIdx]])) == SHT_SET_END ){
											break;
										}
										else{ /* Go while loop */ }

										checkIdx = IncrSheetIndex(checkIdx);
									}

									gSht_ManagerMember.mNextFinishShtIdx = checkIdx;
								}
								else{											// Not Set End
									while ( checkIdx != gSht_ManagerMember.mNextDelvShtIdx ){
										gSht_ManagerMember.mNextFinishShtIdx = checkIdx;

										if ( (checkIdx == gSht_ManagerMember.mNextEjectShtIdx) ||
											 (CShtSheet_GetSetEnd(&(gSht_ManagerMember.mSheet[gSht_ManagerMember.mShtExtQue[checkIdx]])) == SHT_SET_END) ){
											break;								// Fail Safe(Set End = Eject Sheet)
										}
										else{ /* Go while loop */ }

										checkIdx = IncrSheetIndex(checkIdx);
									}
								}
							}
							else{ /* Abnormal Case */ }
						}
						else{ /* No Action */ }
					}
					else{ /* Abnormal Case */ }

					if ( (CShtSheet_GetSetEnd(&(gSht_ManagerMember.mSheet[sheetNo])) == SHT_SET_END) &&
						 (GetSetFinishedLocation(sheetNo) == location) ){
						CCdiAdapter_SendSetFinished(sheetNo);
					}
					else{ /* No Action */ }										// Not Set End
				}
				else{ /* No Action */ }											// Lead Edge
			}

			CShtInitializeManager_SheetLocation(sheetNo, edge, location);
			CJamChecker_SheetLocation(sheetNo, edge, location);
			SendSheetLocationNotify(sheetNo, edge, location);
			PutSheetLocationLog(sheetNo, location, edge, SHT_COND_EJECT);
			SRVC_Moview_SetPaperLocation(edge, location, sheetNo);

			// --- After sheet entry to path sensor ---
			afterShtIdx = IncrSheetIndex(shtIdx);

			while ( afterShtIdx != gSht_ManagerMember.mExitEntryShtIdx ){
				afterShtNo = gSht_ManagerMember.mShtExtQue[afterShtIdx];
				afterShtCond = CShtSheet_GetCondition(&(gSht_ManagerMember.mSheet[afterShtNo]));

				if ( afterShtCond == SHT_COND_EJECT ){
					if ( (cSht_PathSensorIDLocation[CShtSheet_GetNextLocation(&(gSht_ManagerMember.mSheet[afterShtNo]), edge)] == pathSnrID) &&
						 (CShtSheet_CheckEventOccurence(&(gSht_ManagerMember.mSheet[afterShtNo]), SHT_EVT_ID_EJECT_START) == true) ){
						CShtPathSensor_EntrySheet(&(gSht_ManagerMember.mPathSensor[pathSnrID]), afterShtNo, edge);
						break;
					}
					else { /* No Action */ }
				}
				else { break; }

				afterShtIdx = IncrSheetIndex(afterShtIdx);
			}
		}
		else{
			// Delivery NG Condition
		}
	}
	else{
		// Unexpected sensor edge
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_PathSensorLateDetect
// Description   : シート端時間待ち確定処理の時間経過通知受信処理
// Parameter     : pPathSnr (パスセンサインスタンスポインタ)
//               : edge (シートエッジ識別子)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtManager_PathSensorLateDetect(CShtPathSensor *pPathSnr, EShtSheetEdge edge)
{
	UC pathSnrID;

	for ( pathSnrID = 0; pathSnrID < SHT_PATH_SNR_ID_NUM; pathSnrID++ ){
		if ( &(gSht_ManagerMember.mPathSensor[pathSnrID]) == pPathSnr ){
			CShtManager_PathSensorEdgeChange(pathSnrID, edge);
			break;
		}
		else{ /* Go for loop */ }
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_SetPitchSkip
// Description   : Device都合により、PitchSkip要求をする時の設定関数
// Parameter     : bOnOff :  On(PitchSkip ）/ Off（PitchSkipしない）
///////////////////////////////////////////////////////////////////
void CShtManager_SetPitchSkip(UC  bOnOff)
{
	gSht_ManagerMember.mPitchSkip  =  bOnOff;
	return;	
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManager_GetPitchSkip
// Description   : PitchSkip状態 ： On(PitchSkip中）/ Off（PitchSkipしない）
// Return    :  On(PitchSkip ）/ Off（PitchSkipしない）
///////////////////////////////////////////////////////////////////
UC CShtManager_GetPitchSkip(void)
{
	return  gSht_ManagerMember.mPitchSkip;
}

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.05.20	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 14.12.08	| Yuichi Hoshino	| Trouble Action [ Purge Possibility戻り値不正 ]
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ シートロケーションログ出力 ]
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ Moview ]
// 15.03.30	| Yuichi Hoshino	| Design Improvement [ Sheet Event定義見直し ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
// 16.03.16	| Yuichi Hoshino	| Trouble Action [ Delivered済みの用紙のSheet IDが再利用出来ない ]
// 16.03.22	| Yuichi Hoshino	| Trouble Action [ Considerable Timeから復帰できない ]

