// File Name: DD_STM.c
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 13.10.31
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 13.10.31 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev015 Base)
// Revision 000

#define DD_STM_C

#include "Common.h"
#include "DD_Common.h"
#include "DD_TAU.h"
#include "DD_DMA.h"
#include "DD_Event.h"
#include "LIB_Lock.h"
#include "DD_STM.h"
#include "DD_ResourceAssign.h"

#if 0
#ifndef STARTER_KIT
#define DD_STM_PROF_STEP_MAX			128
#define DD_STM_PROF_NUM_MAX				8
#else
#define DD_STM_PROF_STEP_MAX			32
#define DD_STM_PROF_NUM_MAX				4
#endif
#else
#define DD_STM_PROF_STEP_MAX			512


#endif

#ifndef STARTER_KIT
#define DD_STM_PROF_CONST_STEP_SIZE		16
#else
#define DD_STM_PROF_CONST_STEP_SIZE		8
#endif

#define DD_STM_NOT_COUNT_CLOCK			0
#define DD_STM_COUNT_CLOCK				1
#define DD_STM_PULSE_TIMING				2

#define DD_STM_BASE_CLOCK_FREQ			1000000				// [×1Hz]

typedef void (*DD_VOID_FUNC_SS_TYPE)(SS);

// Motor State
typedef enum{
	stIdle_ID,
	stChangePulseWait_ID,
	stChangeEntry_ID,
	stChangeExecute_ID,
	stChangeAnswer_ID,
	stConstant_ID,
	stPTPulseWait_ID,
	stPTEntry_ID,
	stPTExec_ID,
	stPTExecPulseWait_ID,
	stPTJudge_ID,
	stPTAnswer_ID,
	stStopPulseWait_ID,
	stStopEntry_ID,
	stStopExecute_ID,
	stStopExecPulseWait_ID,
	stStopAnswer_ID,
	stSPPulseWait_ID,
	stSPChangeEntry_ID,
	stSPChange_ID,
	stSPChangePulseWait_ID,
	stSPConstEntry_ID,
	stSPConstant_ID,
	stSPConstPulseWait_ID,
	stSPConstJudge_ID,
	stSPStopEntry_ID,
	stSPStop_ID,
	stSPStopPulseWait_ID,
	stSPAnswer_ID,
	stShutDown_ID,
} DD_STM_StateID;

#define DD_STM_RECEIVE_DATA_NUM	4

typedef struct{
	US 						targetIndex;
	US 						totalPulseCnt;
	DD_VOID_FUNC_SS_TYPE	func;
} DD_STM_ReceiveData;

typedef struct{
	UC						controlType;
	US						countStartIndex;
	US						clockCount;
	US						remain;
	DD_VOID_FUNC_SS_TYPE	func;
} DD_STM_CountClockData;

typedef struct{
	DD_STM_StateID			state;

	DD_STM_PROF_ID			profileID;
	UC						lastReceiveNo;
	DD_STM_ReceiveData		receiveData;
	DD_STM_ReceiveData		entryData;

	US						nowIndex;
	US						targetIndex;
	US 						spChangePulseCnt;
	US						spConstPulseCnt;
	US						constTable[DD_STM_PROF_CONST_STEP_SIZE];
	US						spSlowDownPulseCnt;
	SS						spResult;
	DD_VOID_FUNC_SS_TYPE	func;
	DD_STM_CountClockData	count;
} DD_STM_MotorManager;

DD_STM_MotorManager gDD_STM_MotorManager[DD_STM_ID_NUM];

typedef struct {
	DD_STM_PROF_ID	profileID;
	US				idleIndex;
	US				slowUpDownThresh;											// Slow-Up,Slow-Downインデックスの閾値(閾値未満：Slow-Upインデックス、閾値以上：Slow-Downインデックス)
	US				profEndIndex;
} DD_STM_Profile;
DD_STM_Profile gDD_STM_Profile[DD_STM_PROF_ID_NUM];

US gDD_STM_ProfileTable[DD_STM_PROF_STEP_MAX * 2];								// High区間、Low区間を持つため×2している
US gDD_STM_ConvertIndexTable[DD_STM_PROF_STEP_MAX];


// ◆ Prototype(Used Only in DD_STM.c)
// 状態遷移イベント処理
static void stIdle(DD_STM_ID motorID);
static void stChangePulseWait(DD_STM_ID motorID);
static void stChangeEntry(DD_STM_ID motorID);
static void stChangeExecute(DD_STM_ID motorID);
static void stChangeAnswer(DD_STM_ID motorID);
static void stConstant(DD_STM_ID motorID);
static void stStopPulseWait(DD_STM_ID motorID);
static void stStopEntry(DD_STM_ID motorID);
static void stStopExecute(DD_STM_ID motorID);
static void stStopExecPulseWait(DD_STM_ID motorID);
static void stStopAnswer(DD_STM_ID motorID);
static void stSPPulseWait(DD_STM_ID motorID);
static void stSPChangeEntry(DD_STM_ID motorID);
static void stSPChange(DD_STM_ID motorID);
static void stSPChangePulseWait(DD_STM_ID motorID);
static void stSPConstEntry(DD_STM_ID motorID);
static void stSPConstant(DD_STM_ID motorID);
static void stSPConstPulseWait(DD_STM_ID motorID);
static void stSPConstJudge(DD_STM_ID motorID);
static void stSPStopEntry(DD_STM_ID motorID);
static void stSPStop(DD_STM_ID motorID);
static void stSPStopPulseWait(DD_STM_ID motorID);
static void stSPAnswer(DD_STM_ID motorID);
static void stShutDown(DD_STM_ID motorID);
static void stPTPulseWait(DD_STM_ID motorID);
static void stPTEntry(DD_STM_ID motorID);
static void stPTExec(DD_STM_ID motorID);
static void stPTExecPulseWait(DD_STM_ID motorID);
static void stPTJudge(DD_STM_ID motorID);
static void stPTAnswer(DD_STM_ID motorID);

// プロファイルIDセット
static void SetProfileIDExec(DD_STM_ID motorID, DD_STM_PROF_ID profileID);
// カウントクロックデータ
static void SetCountStartIndex(DD_STM_ID motorID);

// クロック制御
static bool SetChangeClockData(DD_STM_ID motorID);
static bool SetStopClockData(DD_STM_ID motorID);

// 割り込みコールバック
static void TransferEndCallback(void* arg);
static void TimeoutCallback(void* arg);

//============================================================================= 
// Function Name : DD_STM_Init
// Description   : ステッピングモータの初期化
// Parameter     : -
// Return        : -
// Note          : 内部パラメータ、使用するTAU、DMAのレジスタを初期化する
//=============================================================================
void DD_STM_Init(void)
{
	UC motorID, profID;
	US loopCnt;

	// モータ関連データの初期化
	for( motorID = 0; motorID < DD_STM_ID_NUM; motorID++ ){
		// TAU、DMAのレジスタ設定、コールバック設定
		DD_TAU_EntryCallback(gDD_STM_CtrlTable[motorID].timerCH, TimeoutCallback, (void*)motorID);
		DD_TAU_InitByChannel(gDD_STM_CtrlTable[motorID].timerCH, DD_TAU_MODE_SQUARE_PULSE_ID, DD_TAU_ACTIVE_HIGH_ID);

		DD_DMA_EntryCallback(gDD_STM_CtrlTable[motorID].dmacCH, TransferEndCallback, (void*)motorID);
		DD_DMA_InitByChannel(gDD_STM_CtrlTable[motorID].dmacCH, DD_DMA_TRIGGER_TYPE_TM, gDD_STM_CtrlTable[motorID].timerCH);

		// gDD_STM_MotorManagerの初期化
		gDD_STM_MotorManager[motorID].state = stIdle_ID;
		gDD_STM_MotorManager[motorID].profileID = DD_STM_PROF_ID_NUM;
		gDD_STM_MotorManager[motorID].receiveData.targetIndex = 0;
		gDD_STM_MotorManager[motorID].receiveData.totalPulseCnt = 0;
		gDD_STM_MotorManager[motorID].receiveData.func = NULL;
		gDD_STM_MotorManager[motorID].entryData.targetIndex = 0;
		gDD_STM_MotorManager[motorID].entryData.totalPulseCnt = 0;
		gDD_STM_MotorManager[motorID].entryData.func = NULL;
		gDD_STM_MotorManager[motorID].nowIndex = 0;
		gDD_STM_MotorManager[motorID].targetIndex = 0;
		gDD_STM_MotorManager[motorID].spChangePulseCnt = 0;
		gDD_STM_MotorManager[motorID].spConstPulseCnt = 0;
		for( loopCnt = 0; loopCnt < DD_STM_PROF_CONST_STEP_SIZE; loopCnt++ ){
			gDD_STM_MotorManager[motorID].constTable[loopCnt] = 0;
		}
		gDD_STM_MotorManager[motorID].spSlowDownPulseCnt = 0;
		gDD_STM_MotorManager[motorID].spResult = DD_STM_RESULT_OK;
		gDD_STM_MotorManager[motorID].func = NULL;
	}

	// プロファイルデータの初期化
	for( profID = 0; profID < DD_STM_PROF_ID_NUM; profID++ ){
		gDD_STM_Profile[profID].profileID = DD_STM_PROF_ID_NUM;
		gDD_STM_Profile[profID].idleIndex = 0;
		gDD_STM_Profile[profID].slowUpDownThresh = 0;
		gDD_STM_Profile[profID].profEndIndex = 0;
	}

	// プロファイルテーブルの初期化
	for( loopCnt = 0; loopCnt < (DD_STM_PROF_STEP_MAX * 2); loopCnt++ ){
		gDD_STM_ProfileTable[loopCnt] = 0;
	}

	// インデックス変換テーブルの初期化
	for( loopCnt = 0; loopCnt < DD_STM_PROF_STEP_MAX; loopCnt++ ){
		gDD_STM_ConvertIndexTable[loopCnt] = 0;
	}
}

//============================================================================= 
// Function Name : DD_STM_SetProfile
// Description   : プロファイル設定
// Parameter     : profileID	"プロファイルテーブルのID"
//               : pProfile		"プロファイルテーブルのポインタ"
// Return        : -
// Note          : -
//=============================================================================
void DD_STM_SetProfile(DD_STM_PROF_ID profileID, US* pProfile)
{
	static US profIndex = 0;
	US freqData;
	US lastProfData = 0;
	US loopIndex, searchIndex;
	US* pProfAdd = pProfile;

	if( profileID >= DD_STM_PROF_ID_NUM ){
		return;																	// 定義されていないprofileIDが指定されている(Irregular Case)
	}

	if( gDD_STM_Profile[profileID].profileID == profileID ){
		return;																	// 1度プロファイル設定がされている(Irregular Case)
	}

	for( loopIndex = 0; *pProfAdd != 0; loopIndex++, pProfAdd++ ){
	}

	if( (profIndex + loopIndex * 2)  > (DD_STM_PROF_STEP_MAX * 2) ){
		return;																	// ProfileTableのRAM容量を超える(Irregular Case)
	}

	// プロファイルテーブルにタイマ値データをセット
	// Idleをセット
	gDD_STM_Profile[profileID].profileID = profileID;
	gDD_STM_Profile[profileID].idleIndex = profIndex;

	gDD_STM_ProfileTable[profIndex] = 0;										// Low区間の設定
	profIndex++;

	gDD_STM_ProfileTable[profIndex] = 0;										// High区間の設定
	profIndex++;

	// Slow-Upインデックス分をセット
	for( ; *pProfile >= lastProfData; pProfile++ ){
		freqData = (US)(((UL)(DD_STM_BASE_CLOCK_FREQ * 10) / (UL)(*pProfile)) / 2) - 1 ;

		gDD_STM_ProfileTable[profIndex] = freqData;								// Low区間の設定
		profIndex++;

		gDD_STM_ProfileTable[profIndex] = freqData;								// High区間の設定
		profIndex++;

		lastProfData = *pProfile;
	}

	gDD_STM_Profile[profileID].slowUpDownThresh = profIndex;

	// Slow-Downインデックス分をセット
	for( ; *pProfile != 0; pProfile++ ){
		freqData = (US)(((UL)(DD_STM_BASE_CLOCK_FREQ * 10) / (UL)(*pProfile)) / 2) - 1 ;

		gDD_STM_ProfileTable[profIndex] = freqData;								// Low区間の設定
		profIndex++;

		gDD_STM_ProfileTable[profIndex] = freqData;								// High区間の設定
		profIndex++;
	}

	gDD_STM_Profile[profileID].profEndIndex = profIndex - 2;

	// インデックス変換テーブルにインデックスデータをセット
	// Idleインデックスの変換インデックスをセット
	gDD_STM_ConvertIndexTable[gDD_STM_Profile[profileID].idleIndex / 2] = gDD_STM_Profile[profileID].idleIndex;

	// Slow-Up→Slow-Downの変換インデックスをセット
	for( loopIndex = gDD_STM_Profile[profileID].idleIndex + 2; loopIndex <= gDD_STM_Profile[profileID].slowUpDownThresh; loopIndex += 2 ){
		for( searchIndex = gDD_STM_Profile[profileID].slowUpDownThresh; searchIndex <= gDD_STM_Profile[profileID].profEndIndex; searchIndex += 2 ){
			if( gDD_STM_ProfileTable[searchIndex] > gDD_STM_ProfileTable[loopIndex] ){
				gDD_STM_ConvertIndexTable[loopIndex / 2] = searchIndex;
				break;
			}
			else{
				/* No Action */
			}
		}

		if( searchIndex > gDD_STM_Profile[profileID].profEndIndex ){			// 変換対象のSlow-Downインデックスが見つからないとき
			gDD_STM_ConvertIndexTable[loopIndex / 2] = gDD_STM_Profile[profileID].idleIndex;	//idleIndexを設定する
		}
	}

	// Slow-Down→Slow-Upの変換インデックスをセット
	for( loopIndex = gDD_STM_Profile[profileID].slowUpDownThresh; loopIndex <= gDD_STM_Profile[profileID].profEndIndex; loopIndex += 2 ){
		for( searchIndex = gDD_STM_Profile[profileID].idleIndex + 2; searchIndex <= (gDD_STM_Profile[profileID].slowUpDownThresh - 2); searchIndex += 2 ){
			if( gDD_STM_ProfileTable[searchIndex] < gDD_STM_ProfileTable[loopIndex] ){		// 最高周波数はSlow-Upインデックスに設定されているので、必ず探索は成功する
				gDD_STM_ConvertIndexTable[loopIndex / 2] = searchIndex;
				break;
			}
			else{
				/* No Action */
			}
		}
	}
}

//============================================================================= 
// Function Name : DD_STM_SetProfileID
// Description   : プロファイル設定
// Parameter     : motorID		"モーターID"
// 				 : profileID	"プロファイルテーブルのID"
// Return        : -
// Note          : -
//=============================================================================
void DD_STM_SetProfileID(DD_STM_ID motorID, DD_STM_PROF_ID profileID)
{
	UL eventData = (UL)motorID | ((UL)(profileID) << 8);

	DD_EVT_Put(evReceive_ID, evuDD_STM_ID, (UC*)(eventData), 0);
}

//============================================================================= 
// Function Name : DD_STM_StartClock
// Description   : クロック出力開始
// Parameter     : motorID		"モータのID"
//               : targetIndex	"目標インデックス"
//               : *func		"コールバック関数ポインタ"
// Return        : -
// Note          : -
//=============================================================================
void DD_STM_StartClock(DD_STM_ID motorID, US targetIndex, void (*func)(SS result))
{
	UL eventData;

	gDD_STM_MotorManager[motorID].receiveData.targetIndex	 = targetIndex;
	gDD_STM_MotorManager[motorID].receiveData.func			 = func;

	gDD_STM_MotorManager[motorID].lastReceiveNo++;

	eventData = (UL)motorID | ((UL)(gDD_STM_MotorManager[motorID].lastReceiveNo) << 8);

	DD_EVT_Put(evWrite_ID, evuDD_STM_ID, (UC*)(eventData), 0);
}

//============================================================================= 
// Function Name : DD_STM_ChangeClock
// Description   : クロック出力変更
// Parameter     : motorID		"モータのID"
//               : targetIndex	"目標インデックス"
//               : *func		"コールバック関数ポインタ"
// Return        : -
// Note          : -
//=============================================================================
void DD_STM_ChangeClock(DD_STM_ID motorID, US targetIndex, void (*func)(SS result))
{
	UL eventData;

	gDD_STM_MotorManager[motorID].receiveData.targetIndex	 = targetIndex;
	gDD_STM_MotorManager[motorID].receiveData.func			 = func;

	gDD_STM_MotorManager[motorID].lastReceiveNo++;

	eventData = (UL)motorID | ((UL)(gDD_STM_MotorManager[motorID].lastReceiveNo) << 8);

	DD_EVT_Put(evWrite_ID, evuDD_STM_ID, (UC*)(eventData), 0);
}

//============================================================================= 
// Function Name : DD_STM_MoveBySetPulse
// Description   : 指定パルス数クロック出力
// Parameter     : motorID		"モータのID"
//               : targetIndex	"目標インデックス"
//               : *func		"コールバック関数ポインタ"
// Return        : -
// Note          : -
//=============================================================================
void DD_STM_MoveBySetPulse(DD_STM_ID motorID, US targetIndex, US totalPulseCnt, void (*func)(SS result))
{
	UL eventData;

	gDD_STM_MotorManager[motorID].receiveData.targetIndex	 = targetIndex;
	gDD_STM_MotorManager[motorID].receiveData.totalPulseCnt	 = totalPulseCnt;
	gDD_STM_MotorManager[motorID].receiveData.func			 = func;

	gDD_STM_MotorManager[motorID].lastReceiveNo++;

	eventData = (UL)motorID | ((UL)(gDD_STM_MotorManager[motorID].lastReceiveNo) << 8);

	DD_EVT_Put(evStart_ID, evuDD_STM_ID, (UC*)(eventData), 0);
}

//============================================================================= 
// Function Name : DD_STM_SlowDownStop
// Description   : クロック停止
// Parameter     : motorID		"モータのID"
//               : targetIndex	"目標インデックス"
//               : *func		"コールバック関数ポインタ"
// Return        : -
// Note          : -
//=============================================================================
void DD_STM_SlowDownStop(DD_STM_ID motorID, void (*func)(SS result))
{
	UL eventData;

	gDD_STM_MotorManager[motorID].receiveData.func = func;

	gDD_STM_MotorManager[motorID].lastReceiveNo++;

	eventData = (UL)motorID | ((UL)(gDD_STM_MotorManager[motorID].lastReceiveNo) << 8);

	DD_EVT_Put(evStop_ID, evuDD_STM_ID, (UC*)(eventData), 0);
}

//============================================================================= 
// Function Name : DD_STM_ShutDownStop
// Description   : クロック緊急停止
// Parameter     : motorID		"モータのID"
// Return        : -
// Note          : コールタイミングでShutDown処理を実施する
//=============================================================================
void DD_STM_ShutDownStop(DD_STM_ID motorID)
{
	DD_EVT_Put(evCancel_ID, evuDD_STM_ID, (UC*)(motorID), 0);
}

//============================================================================= 
// Function Name : DD_STM_StartCountClock
// Description   : 出力クロック数カウント(Start)
// Parameter     : motorID		"モータのID"
// Return        : -
// Note          : -
//=============================================================================
void DD_STM_StartCountClock(DD_STM_ID motorID)
{
	if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_NOT_COUNT_CLOCK ){
		gDD_STM_MotorManager[motorID].count.controlType = DD_STM_COUNT_CLOCK;
		gDD_STM_MotorManager[motorID].count.countStartIndex = 0xFFFF;

		DD_EVT_Put(evRead_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
	else{																		// すでにカウントクロック機能が使われている場合
		/* No Action */
	}
}

//============================================================================= 
// Function Name : DD_STM_StopCountClock
// Description   : 出力クロック数カウント(Stop)
// Parameter     : motorID		"モータのID"
// Return        : -
// Note          : -
//=============================================================================
US DD_STM_StopCountClock(DD_STM_ID motorID)
{
	US count, remainCnt, nowIndex;

	switch( gDD_STM_MotorManager[motorID].state ){
	case stChangePulseWait_ID:
	case stChangeEntry_ID:
	case stChangeExecute_ID:
	case stChangeAnswer_ID:
	case stConstant_ID:
		remainCnt = DD_DMA_GetRemainTransferCnt(gDD_STM_CtrlTable[motorID].dmacCH);
		nowIndex = gDD_STM_MotorManager[motorID].targetIndex - ((remainCnt / 2) * 2);
		count = (nowIndex - gDD_STM_MotorManager[motorID].count.countStartIndex) / 2;
		break;

	case stPTPulseWait_ID:
		count = gDD_STM_MotorManager[motorID].count.clockCount;
		break;

	case stPTEntry_ID:
		count = gDD_STM_MotorManager[motorID].count.clockCount / 2;
		break;

	case stPTExec_ID:
	case stPTExecPulseWait_ID:
		remainCnt = DD_DMA_GetRemainTransferCnt(gDD_STM_CtrlTable[motorID].dmacCH);
		count = gDD_STM_MotorManager[motorID].count.clockCount / 2 + (DD_STM_PROF_CONST_STEP_SIZE - 1 - remainCnt) / 2;
		break;

	case stPTJudge_ID:
		count = gDD_STM_MotorManager[motorID].count.clockCount / 2;
		break;

	case stStopPulseWait_ID:
		count = gDD_STM_MotorManager[motorID].count.clockCount / 2;
		break;

	case stStopEntry_ID:
		count = gDD_STM_MotorManager[motorID].count.clockCount / 2;
		break;

	case stStopExecute_ID:
	case stStopExecPulseWait_ID:
		remainCnt = DD_DMA_GetRemainTransferCnt(gDD_STM_CtrlTable[motorID].dmacCH);
		nowIndex = gDD_STM_MotorManager[motorID].targetIndex - ((remainCnt / 2) * 2);
		count = (nowIndex - gDD_STM_MotorManager[motorID].nowIndex) / 2;
		break;

	case stStopAnswer_ID:
	case stIdle_ID:
		count = gDD_STM_MotorManager[motorID].count.clockCount / 2;
		break;

	default:
		count = 0;
		break;
	}

	gDD_STM_MotorManager[motorID].count.controlType = DD_STM_NOT_COUNT_CLOCK;
	DD_EVT_Put(evReset_ID, evuDD_STM_ID, (UC*)(motorID), 0);

	return count;
}

//============================================================================= 
// Function Name : DD_STM_AppointPulseTiming
// Description   : 指定パルスタイミング
// Parameter     : motorID		"モータのID"
// Return        : -
// Note          : -
//=============================================================================
void DD_STM_AppointPulseTiming(DD_STM_ID motorID, US pulseCnt, void (*func)(SS result))
{
	if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_NOT_COUNT_CLOCK ){
		gDD_STM_MotorManager[motorID].count.controlType = DD_STM_PULSE_TIMING;
		gDD_STM_MotorManager[motorID].count.func = func;
		gDD_STM_MotorManager[motorID].count.remain = pulseCnt * 2;

		DD_EVT_Put(evRead_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
	else{																		// すでにカウントクロック機能が使われている場合
		/* No Action */
	}
}

//============================================================================= 
// Function Name : DD_STM_Notify
// Description   : Event通知関数
// Parameter     : eventID 	"イベントID"
//               : pMsg		"データポインタ"
//				 : (0-7bit:morotID、8-15bit:para1、16-23bit:para2、24-31bit:para3)
// Return        : -
// Note          : EventLoopからコールされる
//=============================================================================
void DD_STM_Notify(DD_EventID eventID, UC* pMsg)
{
	UC key;
	DD_STM_ID motorID = (DD_STM_ID)((UL)(pMsg) & 0x000000FF);
	UC para1 = (UC)(((UL)(pMsg) & 0x0000FF00) >> 8);

	switch( eventID ){
	case evReceive_ID:
		switch( gDD_STM_MotorManager[motorID].state ){
		case stIdle_ID:															// クロック停止中のstateの場合
		case stSPAnswer_ID:
		case stStopAnswer_ID:
		case stShutDown_ID:
			SetProfileIDExec(motorID, (DD_STM_PROF_ID)para1 );
			break;

		default:																// クロック出力中のstateの場合
			/* No Action */
			break;
		}
		break;

	case evWrite_ID:
		key = LIB_GetKey();
		LIB_Lock();																// 割り込み禁止

		if( para1 == gDD_STM_MotorManager[motorID].lastReceiveNo ){				// イベント管理No.と受信データ管理No.が一致するとき
			gDD_STM_MotorManager[motorID].entryData = gDD_STM_MotorManager[motorID].receiveData;
			LIB_Unlock(key);													// 割り込み許可
		}
		else{																	// イベント管理No.と受信データ管理No.が一致しないとき
			LIB_Unlock(key);													// 割り込み許可
			break;																// イベント処理を終了する
		}

		if( gDD_STM_MotorManager[motorID].profileID == DD_STM_PROF_ID_NUM ){	// プロファイルID設定が未実施のとき
			break;																// イベント処理を終了する
		}

		switch( gDD_STM_MotorManager[motorID].state ){
		case stIdle_ID:															// クロック停止中のstateの場合
		case stStopAnswer_ID:
		case stSPAnswer_ID:
		case stShutDown_ID:
			stChangeEntry(motorID);
			break;

		case stChangePulseWait_ID:												// クロック出力中のstateの場合
		case stChangeEntry_ID:
		case stChangeExecute_ID:
		case stChangeAnswer_ID:
		case stConstant_ID:
		case stPTPulseWait_ID:
		case stPTEntry_ID:
		case stPTExec_ID:
		case stPTExecPulseWait_ID:
		case stPTJudge_ID:
		case stPTAnswer_ID:
		case stStopPulseWait_ID:
		case stStopEntry_ID:
		case stStopExecute_ID:
		case stStopExecPulseWait_ID:
		case stSPPulseWait_ID:
		case stSPChangeEntry_ID:
		case stSPChange_ID:
		case stSPChangePulseWait_ID:
		case stSPConstEntry_ID:
		case stSPConstant_ID:
		case stSPConstPulseWait_ID:
		case stSPConstJudge_ID:
		case stSPStopEntry_ID:
		case stSPStop_ID:
		case stSPStopPulseWait_ID:
			stChangePulseWait(motorID);
			break;

		default:																// バカよけ
			/* No Action */
			break;
		}
		break;

	case evStart_ID:
		key = LIB_GetKey();
		LIB_Lock();																// 割り込み禁止

		if( para1 == gDD_STM_MotorManager[motorID].lastReceiveNo ){				// イベント管理No.と受信データ管理No.が一致するとき
			gDD_STM_MotorManager[motorID].entryData = gDD_STM_MotorManager[motorID].receiveData;
			LIB_Unlock(key);													// 割り込み許可
		}
		else{																	// イベント管理No.と受信データ管理No.が一致しないとき
			LIB_Unlock(key);													// 割り込み許可
			break;																// イベント処理を終了する
		}

		if( gDD_STM_MotorManager[motorID].profileID == DD_STM_PROF_ID_NUM ){	// プロファイルID設定が未実施のとき
			break;																// イベント処理を終了する
		}

		switch( gDD_STM_MotorManager[motorID].state ){
		case stIdle_ID:															// クロック停止中のstateの場合
		case stStopAnswer_ID:
		case stSPAnswer_ID:
		case stShutDown_ID:
			stSPChangeEntry(motorID);
			break;

		case stChangePulseWait_ID:												// クロック出力中のstateの場合
		case stChangeEntry_ID:
		case stChangeExecute_ID:
		case stChangeAnswer_ID:
		case stConstant_ID:
		case stPTPulseWait_ID:
		case stPTEntry_ID:
		case stPTExec_ID:
		case stPTExecPulseWait_ID:
		case stPTJudge_ID:
		case stPTAnswer_ID:
		case stStopPulseWait_ID:
		case stStopEntry_ID:
		case stStopExecute_ID:
		case stStopExecPulseWait_ID:
		case stSPPulseWait_ID:
		case stSPChangeEntry_ID:
		case stSPChange_ID:
		case stSPChangePulseWait_ID:
		case stSPConstEntry_ID:
		case stSPConstant_ID:
		case stSPConstPulseWait_ID:
		case stSPConstJudge_ID:
		case stSPStopEntry_ID:
		case stSPStop_ID:
		case stSPStopPulseWait_ID:
			stSPPulseWait(motorID);
			break;

		default:																// バカよけ
			/* No Action */
			break;
		}
		break;

	case evRead_ID:
		if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_COUNT_CLOCK ){
			switch( gDD_STM_MotorManager[motorID].state ){
			case stChangePulseWait_ID:
			case stChangeEntry_ID:
			case stChangeExecute_ID:
				SetCountStartIndex(motorID);
				break;

			case stChangeAnswer_ID:
			case stConstant_ID:
				SetCountStartIndex(motorID);
				stPTPulseWait(motorID);
				break;

			default:
				/* No Action */
				break;
			}
		}
		else if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_PULSE_TIMING ){
			switch( gDD_STM_MotorManager[motorID].state ){
			case stChangeAnswer_ID:
			case stConstant_ID:
				stPTPulseWait(motorID);
				break;

			default:
				/* No Action */
				break;
			}
		}
		break;

	case evGoComplete_ID:
		switch( gDD_STM_MotorManager[motorID].state ){
		case stChangePulseWait_ID:
			stChangeEntry(motorID);
			break;

		case stStopPulseWait_ID:
			stStopEntry(motorID);
			break;

		case stStopExecPulseWait_ID:
			stStopAnswer(motorID);
			break;

		case stSPPulseWait_ID:
			stSPChangeEntry(motorID);
			break;

		case stSPChangePulseWait_ID:
			stSPConstEntry(motorID);
			break;

		case stSPConstPulseWait_ID:
			stSPConstJudge(motorID);
			break;

		case stSPStopPulseWait_ID:
			stSPAnswer(motorID);
			break;

		case stPTPulseWait_ID:
			stPTEntry(motorID);
			break;

		case stPTExecPulseWait_ID:
			stPTJudge(motorID);
			break;

		default:
			/* No Action */
			break;
		}
		break;

	case evGoCancel_ID:
		switch( gDD_STM_MotorManager[motorID].state ){
		case stChangeEntry_ID:
			stChangeAnswer(motorID);
			break;

		case stStopEntry_ID:
			stStopAnswer(motorID);
			break;

		case stSPConstEntry_ID:
			stSPStopEntry(motorID);
			break;

		case stSPStopEntry_ID:
			stSPAnswer(motorID);
			break;

		case stPTEntry_ID:
			stPTAnswer(motorID);
			break;

		default:
			/* No Action */
			break;
		}
		break;

	case evGoNext_ID:
		switch( gDD_STM_MotorManager[motorID].state ){
		case stChangeEntry_ID:
			stChangeExecute(motorID);
			break;

		case stStopEntry_ID:
			stStopExecute(motorID);
			break;

		case stSPChangeEntry_ID:
			stSPChange(motorID);
			break;

		case stSPConstEntry_ID:
			stSPConstant(motorID);
			break;

		case stSPConstJudge_ID:
			stSPStopEntry(motorID);
			break;

		case stSPStopEntry_ID:
			stSPStop(motorID);
			break;

		case stPTEntry_ID:
			stPTExec(motorID);
			break;

		case stPTJudge_ID:
			stPTAnswer(motorID);
			break;

		default:
			/* No Action */
			break;
		}
		break;

	case evSend_ID:
		switch( gDD_STM_MotorManager[motorID].state ){
		case stChangeExecute_ID:
			stChangeAnswer(motorID);
			break;

		case stStopExecute_ID:
			stStopExecPulseWait(motorID);
			break;

		case stSPChange_ID:
			stSPChangePulseWait(motorID);
			break;

		case stSPConstant_ID:
			stSPConstPulseWait(motorID);
			break;

		case stSPStop_ID:
			stSPStopPulseWait(motorID);
			break;

		case stPTExec_ID:
			stPTExecPulseWait(motorID);
			break;

		default:
			/* No Action */
			break;
		}
		break;

	case evComplete_ID:
		switch( gDD_STM_MotorManager[motorID].state ){
		case stChangeAnswer_ID:
			stConstant(motorID);
			break;

		case stStopAnswer_ID:
		case stSPAnswer_ID:
		case stShutDown_ID:
			stIdle(motorID);
			break;

		default:
			/* No Action */
			break;
		}
		break;

	case evStop_ID:
		key = LIB_GetKey();
		LIB_Lock();																// 割り込み禁止

		if( para1 == gDD_STM_MotorManager[motorID].lastReceiveNo ){				// イベント管理No.と受信データ管理No.が一致するとき
			gDD_STM_MotorManager[motorID].entryData = gDD_STM_MotorManager[motorID].receiveData;
			LIB_Unlock(key);													// 割り込み許可
		}
		else{																	// イベント管理No.と受信データ管理No.が一致しないとき
			LIB_Unlock(key);													// 割り込み許可
			break;																// イベント処理を終了する
		}

		switch( gDD_STM_MotorManager[motorID].state ){
		case stChangePulseWait_ID:
		case stChangeEntry_ID:
		case stChangeExecute_ID:
		case stChangeAnswer_ID:
		case stConstant_ID:
		case stPTPulseWait_ID:
		case stPTEntry_ID:
		case stPTExec_ID:
		case stPTExecPulseWait_ID:
		case stPTJudge_ID:
		case stPTAnswer_ID:
		case stSPPulseWait_ID:
		case stSPChangeEntry_ID:
		case stSPChange_ID:
		case stSPChangePulseWait_ID:
		case stSPConstEntry_ID:
		case stSPConstant_ID:
		case stSPConstPulseWait_ID:
		case stSPConstJudge_ID:
		case stSPStopEntry_ID:
		case stSPStop_ID:
		case stSPStopPulseWait_ID:
			stStopPulseWait(motorID);
			break;

		default:																// stIdle_ID, stStopPulseWait_ID, stStopEntry_ID, stStopExecute_ID, stStopExecPulseWait_ID, stStopAnswer_ID, stSPAnswer_ID, stShutDown_ID
			/* No Action */
			break;
		}
		break;

	case evCancel_ID:
		switch( gDD_STM_MotorManager[motorID].state ){
		case stChangePulseWait_ID:
		case stChangeEntry_ID:
		case stChangeExecute_ID:
		case stChangeAnswer_ID:
		case stConstant_ID:
		case stPTPulseWait_ID:
		case stPTEntry_ID:
		case stPTExec_ID:
		case stPTExecPulseWait_ID:
		case stPTJudge_ID:
		case stPTAnswer_ID:
		case stStopPulseWait_ID:
		case stStopEntry_ID:
		case stStopExecute_ID:
		case stStopExecPulseWait_ID:
		case stStopAnswer_ID:
		case stSPPulseWait_ID:
		case stSPChangeEntry_ID:
		case stSPChange_ID:
		case stSPChangePulseWait_ID:
		case stSPConstEntry_ID:
		case stSPConstant_ID:
		case stSPConstPulseWait_ID:
		case stSPConstJudge_ID:
		case stSPStopEntry_ID:
		case stSPStop_ID:
		case stSPStopPulseWait_ID:
		case stSPAnswer_ID:
			stShutDown(motorID);
			break;

		default:																// stIdle_ID,stShutDown_ID
			break;
		}
		break;

	case evGoStart_ID:
		switch( gDD_STM_MotorManager[motorID].state ){
		case stSPChangeEntry_ID:
 		case stSPConstJudge_ID:
			stSPConstEntry(motorID);
			break;

		case stPTJudge_ID:
			stPTEntry(motorID);
			break;

		default:
			/* No Action */
			break;
		}
		break;

	case evReset_ID:
		switch( gDD_STM_MotorManager[motorID].state ){
		case stPTPulseWait_ID:
		case stPTEntry_ID:
		case stPTExec_ID:
		case stPTExecPulseWait_ID:
		case stPTJudge_ID:
			DD_DMA_Stop(gDD_STM_CtrlTable[motorID].dmacCH);						// DMA転送を停止
			stConstant(motorID);
			break;

		default:
			break;
		}

	default:
		/* No Action */
		break;
	}
}

//============================================================================= 
// Function Name : stIdle
// Description   : stIdle_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stIdle(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stIdle_ID;
}

//============================================================================= 
// Function Name : stChangePulseWait
// Description   : stChangePulseWait_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stChangePulseWait(DD_STM_ID motorID)
{
	US remainCnt;

	gDD_STM_MotorManager[motorID].state = stChangePulseWait_ID;

	DD_DMA_Stop(gDD_STM_CtrlTable[motorID].dmacCH);								// 動作中のDMA転送を停止
	remainCnt = DD_DMA_GetRemainTransferCnt(gDD_STM_CtrlTable[motorID].dmacCH);// 残り転送回数を取得

	if( gDD_STM_MotorManager[motorID].nowIndex != gDD_STM_MotorManager[motorID].targetIndex ){	// SPConstのDMA転送時は更新しない
		gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_MotorManager[motorID].targetIndex - ((remainCnt / 2) * 2);
	}

	DD_TAU_IntEnable(gDD_STM_CtrlTable[motorID].timerCH);						// 指定モータのタイマ割り込み許可
}

//============================================================================= 
// Function Name : stChangeEntry
// Description   : stChangeEntry_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stChangeEntry(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stChangeEntry_ID;

	gDD_STM_MotorManager[motorID].targetIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex + 2 +
												gDD_STM_MotorManager[motorID].entryData.targetIndex * 2;
	gDD_STM_MotorManager[motorID].func = gDD_STM_MotorManager[motorID].entryData.func;

	if( SetChangeClockData(motorID)  == true ){								// クロック変更あり
		DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
	else{																	// クロック変更なし
		DD_EVT_Put(evGoCancel_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
}

//============================================================================= 
// Function Name : stChangeExecute
// Description   : stChangeExecute_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stChangeExecute(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stChangeExecute_ID;
}

//============================================================================= 
// Function Name : stChangeAnswer
// Description   : stChangeAnswer_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stChangeAnswer(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stChangeAnswer_ID;

	gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_MotorManager[motorID].targetIndex;

	if( gDD_STM_MotorManager[motorID].func != NULL ){
		gDD_STM_MotorManager[motorID].func(DD_STM_RESULT_OK);
	}

	if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_NOT_COUNT_CLOCK ){	// 定速時に出力クロック数をカウントしない場合
		DD_EVT_Put(evComplete_ID, evuDD_STM_ID, (UC*)motorID, 0);
	}
	else{																		// 定速時に出力クロック数をカウントする場合
		DD_EVT_Put(evRead_ID, evuDD_STM_ID, (UC*)motorID, 0);
	}
}

//============================================================================= 
// Function Name : stConstant
// Description   : stConstant_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stConstant(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stConstant_ID;
}

//============================================================================= 
// Function Name : stStopPulseWait
// Description   : stStopPulseWait_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stStopPulseWait(DD_STM_ID motorID)
{
	US remainCnt, lastState;


	gDD_STM_MotorManager[motorID].state = stStopPulseWait_ID;

	DD_DMA_Stop(gDD_STM_CtrlTable[motorID].dmacCH);								// 動作中のDMA転送を停止
	remainCnt = DD_DMA_GetRemainTransferCnt(gDD_STM_CtrlTable[motorID].dmacCH);// 残り転送回数を取得

	if( gDD_STM_MotorManager[motorID].nowIndex != gDD_STM_MotorManager[motorID].targetIndex ){	// SPConstのDMA転送時は更新しない
		gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_MotorManager[motorID].targetIndex - ((remainCnt / 2) * 2);
	}

	if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_COUNT_CLOCK ){
		if( (lastState == stPTExec_ID) || (lastState == stPTExecPulseWait_ID) ){
			gDD_STM_MotorManager[motorID].count.clockCount += ((DD_STM_PROF_CONST_STEP_SIZE - 1 - remainCnt) / 2) * 2;	// DMA転送残り
		}
	}

	DD_TAU_IntEnable(gDD_STM_CtrlTable[motorID].timerCH);						// 指定モータのタイマ割り込み許可
}

//============================================================================= 
// Function Name : stStopEntry
// Description   : stStopEntry_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stStopEntry(DD_STM_ID motorID)
{
	UL eventData;

	gDD_STM_MotorManager[motorID].state = stStopEntry_ID;

	gDD_STM_MotorManager[motorID].targetIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex;
	gDD_STM_MotorManager[motorID].func = gDD_STM_MotorManager[motorID].entryData.func;

	if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_COUNT_CLOCK ){
		gDD_STM_MotorManager[motorID].count.clockCount += 2;
	}

	if( SetStopClockData(motorID)  == true ){								// クロック変更なし
		DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
	else{																	// クロック変更なし
		DD_EVT_Put(evGoCancel_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
}

//============================================================================= 
// Function Name : stStopExecute
// Description   : stStopExecute_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stStopExecute(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stStopExecute_ID;
}

//============================================================================= 
// Function Name : stStopExecPulseWait
// Description   : stStopExecPulseWait_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stStopExecPulseWait(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stStopExecPulseWait_ID;

	gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_MotorManager[motorID].targetIndex;

	DD_TAU_IntEnable(gDD_STM_CtrlTable[motorID].timerCH);						// 指定モータのタイマ割り込み許可
}

//============================================================================= 
// Function Name : stStopAnswer
// Description   : stStopAnswer_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stStopAnswer(DD_STM_ID motorID)
{
	US nowIndex = gDD_STM_MotorManager[motorID].nowIndex;
	
	gDD_STM_MotorManager[motorID].state = stStopAnswer_ID;

	DD_TAU_Stop(gDD_STM_CtrlTable[motorID].timerCH);

	gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex;

	if( gDD_STM_MotorManager[motorID].func != NULL ){
		gDD_STM_MotorManager[motorID].func(DD_STM_RESULT_OK);
	}

	if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_COUNT_CLOCK ){
		gDD_STM_MotorManager[motorID].count.clockCount += (gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex - nowIndex);
	}

	DD_EVT_Put(evComplete_ID, evuDD_STM_ID, (UC*)motorID, 0);
}

//============================================================================= 
// Function Name : stSPPulseWait
// Description   : stSPPulseWait_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stSPPulseWait(DD_STM_ID motorID)
{
	US remainCnt;

	gDD_STM_MotorManager[motorID].state = stSPPulseWait_ID;

	DD_DMA_Stop(gDD_STM_CtrlTable[motorID].dmacCH);								// 動作中のDMA転送を停止
	remainCnt = DD_DMA_GetRemainTransferCnt(gDD_STM_CtrlTable[motorID].dmacCH);// 残り転送回数を取得

	if( gDD_STM_MotorManager[motorID].nowIndex != gDD_STM_MotorManager[motorID].targetIndex ){	// SPConstのDMA転送時は更新しない
		gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_MotorManager[motorID].targetIndex - ((remainCnt / 2) * 2);
	}

	DD_TAU_IntEnable(gDD_STM_CtrlTable[motorID].timerCH);						// 指定モータのタイマ割り込み許可
}

//============================================================================= 
// Function Name : stSPChangeEntry
// Description   : stSPChangeEntry_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stSPChangeEntry(DD_STM_ID motorID)
{
	US nowIndex, targetIndex, indexConvPlsCnt, changePulseCnt, slowDownStartIndex, slowDownPulseCnt, totalPulseCnt;
	UC loopCnt;

	gDD_STM_MotorManager[motorID].state = stSPChangeEntry_ID;

	targetIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex + 2 +
				  gDD_STM_MotorManager[motorID].entryData.targetIndex * 2;
	totalPulseCnt = gDD_STM_MotorManager[motorID].entryData.totalPulseCnt * 2;
	gDD_STM_MotorManager[motorID].func = gDD_STM_MotorManager[motorID].entryData.func;

	nowIndex = gDD_STM_MotorManager[motorID].nowIndex;

	if( (totalPulseCnt <= 2) &&	(nowIndex == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex) ){	// クロック停止状態からの1Pulse出力
		gDD_STM_MotorManager[motorID].targetIndex = nowIndex + 2;
		gDD_STM_MotorManager[motorID].spChangePulseCnt = 2;
		gDD_STM_MotorManager[motorID].spConstPulseCnt = 0;
		gDD_STM_MotorManager[motorID].spSlowDownPulseCnt = 0;
		gDD_STM_MotorManager[motorID].spResult = DD_STM_RESULT_OK;
	}
	else{
		// インデックスの変換
		if( ((targetIndex < gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh) &&
			 (nowIndex < gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh)) ||
			((targetIndex >= gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh) &&
			 (nowIndex >= gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh)) ){

			if( targetIndex < nowIndex ){
				if( gDD_STM_ConvertIndexTable[nowIndex / 2] == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// 現在インデックスの変換インデックスがidleのとき
					targetIndex = nowIndex;

					indexConvPlsCnt = 0;
				}
				else{														// 現在インデックスの変換インデックスがidleではないとき
					if( gDD_STM_ConvertIndexTable[targetIndex / 2] == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// 目標インデックスの変換インデックスがidleのとき
						nowIndex = gDD_STM_ConvertIndexTable[nowIndex / 2];
						targetIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex;	// profEndIndexをセットする

						indexConvPlsCnt = 2;
					}
					else{													// 目標インデックスの変換インデックスがidleではないとき
						nowIndex = gDD_STM_ConvertIndexTable[nowIndex / 2];
						targetIndex = gDD_STM_ConvertIndexTable[targetIndex / 2];

						indexConvPlsCnt = 2;
					}
				}
			}
			else{
				indexConvPlsCnt = 0;
			}
		}
		else{
			if(	gDD_STM_ProfileTable[nowIndex] == gDD_STM_ProfileTable[targetIndex] ){	// 現在インデックスの周波数と、目標インデックスの周波数が同じとき
				nowIndex = targetIndex;

				indexConvPlsCnt = 0;
			}
			else{
				if( gDD_STM_ConvertIndexTable[nowIndex / 2] == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// 現在インデックスの変換インデックスがidleのとき
					targetIndex = gDD_STM_ConvertIndexTable[targetIndex / 2];

					indexConvPlsCnt = 0;
				}
				else{
					if( targetIndex < gDD_STM_ConvertIndexTable[nowIndex / 2] ){
						targetIndex = gDD_STM_ConvertIndexTable[targetIndex / 2];

						indexConvPlsCnt = 0;
					}
					else{
						nowIndex = gDD_STM_ConvertIndexTable[nowIndex / 2];

						indexConvPlsCnt = 2;
					}
				}
			}
		}

		// 目標インデックスの調整
		for( ; ; ){
			changePulseCnt = targetIndex - nowIndex + indexConvPlsCnt;

			if( targetIndex < gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh ){	// 目標インデックスがSlow-Upインデックスの場合
				if( gDD_STM_ConvertIndexTable[targetIndex / 2] == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){
					slowDownPulseCnt = 0;
				}
				else{
					slowDownStartIndex = gDD_STM_ConvertIndexTable[targetIndex / 2];	// Slow-Downの開始インデックスを取得
					slowDownPulseCnt = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex - slowDownStartIndex + 2;	//インデックス変換のため+2
				}
			}
			else{															// 目標インデックスがSlow-Downインデックスの場合
				slowDownPulseCnt = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex - targetIndex;
			}

			if( totalPulseCnt >= (changePulseCnt + slowDownPulseCnt) ){
				gDD_STM_MotorManager[motorID].nowIndex = nowIndex;
				gDD_STM_MotorManager[motorID].targetIndex = targetIndex;
				gDD_STM_MotorManager[motorID].spChangePulseCnt = changePulseCnt;
				gDD_STM_MotorManager[motorID].spConstPulseCnt = totalPulseCnt - changePulseCnt - slowDownPulseCnt;
				if( gDD_STM_MotorManager[motorID].spConstPulseCnt > 0 ){
					for( loopCnt = 0; loopCnt < DD_STM_PROF_CONST_STEP_SIZE; loopCnt++ ){
						gDD_STM_MotorManager[motorID].constTable[loopCnt] = gDD_STM_ProfileTable[gDD_STM_MotorManager[motorID].targetIndex];
					}
				}
				gDD_STM_MotorManager[motorID].spSlowDownPulseCnt = slowDownPulseCnt;
				gDD_STM_MotorManager[motorID].spResult = DD_STM_RESULT_OK;
				break;
			}
			else{
				if( targetIndex < gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh ){	// 目標インデックスがSlow-Upインデックスの場合
					targetIndex -= 2;
					if( targetIndex < nowIndex ){
						gDD_STM_MotorManager[motorID].nowIndex = nowIndex;
						gDD_STM_MotorManager[motorID].targetIndex = nowIndex;
						gDD_STM_MotorManager[motorID].spChangePulseCnt = 0;
						gDD_STM_MotorManager[motorID].spConstPulseCnt = 0;
						gDD_STM_MotorManager[motorID].spSlowDownPulseCnt = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex - nowIndex;
						gDD_STM_MotorManager[motorID].spResult = DD_STM_RESULT_NG;
						break;
					}
					else{
						/* Continue for loop */
					}
				}
				else{															// 目標インデックスがSlow-Downインデックスの場合
					gDD_STM_MotorManager[motorID].nowIndex = nowIndex;
					gDD_STM_MotorManager[motorID].targetIndex = nowIndex;
					gDD_STM_MotorManager[motorID].spChangePulseCnt = 0;
					gDD_STM_MotorManager[motorID].spConstPulseCnt = 0;
					gDD_STM_MotorManager[motorID].spSlowDownPulseCnt = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex - nowIndex;
					gDD_STM_MotorManager[motorID].spResult = DD_STM_RESULT_NG;
					break;
				}
			}
		}
	}

	if( SetChangeClockData(motorID) == true ){								// クロック変更あり
		DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
	else{																	// クロック変更なし
		DD_EVT_Put(evGoStart_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
}

//============================================================================= 
// Function Name : stSPChange
// Description   : stSPChange_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stSPChange(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stSPChange_ID;
}

//============================================================================= 
// Function Name : stSPChangePulseWait
// Description   : stSPChangePulseWait_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stSPChangePulseWait(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stSPChangePulseWait_ID;

	gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_MotorManager[motorID].targetIndex;

	DD_TAU_IntEnable(gDD_STM_CtrlTable[motorID].timerCH);						// 指定モータのタイマ割り込み許可
}

//============================================================================= 
// Function Name : stSPConstEntry
// Description   : stSPConstEntry_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stSPConstEntry(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stSPConstEntry_ID;

	if( gDD_STM_MotorManager[motorID].spConstPulseCnt > DD_STM_PROF_CONST_STEP_SIZE ){
		DD_DMA_Start(gDD_STM_CtrlTable[motorID].dmacCH,
					 &(gDD_STM_MotorManager[motorID].constTable[1]),
					 (DD_STM_PROF_CONST_STEP_SIZE - 1));
		DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
	else if( gDD_STM_MotorManager[motorID].spConstPulseCnt > 0 ){
		DD_DMA_Start(gDD_STM_CtrlTable[motorID].dmacCH,
					 &(gDD_STM_MotorManager[motorID].constTable[1]),
					 (gDD_STM_MotorManager[motorID].spConstPulseCnt - 1));
		DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
	else{																		// 定速での残り出力パルス数が0のとき
		DD_EVT_Put(evGoCancel_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
}

//============================================================================= 
// Function Name : stSPConstant
// Description   : stSPConstant_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stSPConstant(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stSPConstant_ID;
}

//============================================================================= 
// Function Name : stSPConstPulseWait
// Description   : stSPConstPulseWait_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stSPConstPulseWait(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stSPConstPulseWait_ID;

	if( gDD_STM_MotorManager[motorID].spConstPulseCnt > DD_STM_PROF_CONST_STEP_SIZE ){
		gDD_STM_MotorManager[motorID].spConstPulseCnt -= DD_STM_PROF_CONST_STEP_SIZE;
	}
	else{
		gDD_STM_MotorManager[motorID].spConstPulseCnt = 0;
	}

	DD_TAU_IntEnable(gDD_STM_CtrlTable[motorID].timerCH);						// 指定モータのタイマ割り込み許可
}

//============================================================================= 
// Function Name : stSPConstJudge
// Description   : stSPConstJudge_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stSPConstJudge(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stSPConstJudge_ID;

	if( gDD_STM_MotorManager[motorID].spConstPulseCnt > 0 ){
		DD_EVT_Put(evGoStart_ID, evuDD_STM_ID, (UC*)(motorID), 0);				// ConstPulse出力継続
	}
	else{
		DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);				// ConstPulse出力終了
	}
}

//============================================================================= 
// Function Name : stSPStopEntry
// Description   : stSPStopEntry_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stSPStopEntry(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stSPStopEntry_ID;

	gDD_STM_MotorManager[motorID].targetIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex;

	if( gDD_STM_MotorManager[motorID].spSlowDownPulseCnt > 0 ){					// Slow-Downを実施する
		SetStopClockData(motorID);												// クロック停止実施
		DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
	else{																		// Slow-Downを実施しない
		DD_EVT_Put(evGoCancel_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
}

//============================================================================= 
// Function Name : stSPStop
// Description   : stSPStop_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stSPStop(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stSPStop_ID;
}

//============================================================================= 
// Function Name : stSPStopPulseWait
// Description   : stSPStopPulseWait_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stSPStopPulseWait(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stSPStopPulseWait_ID;

	DD_TAU_IntEnable(gDD_STM_CtrlTable[motorID].timerCH);						// 指定モータのタイマ割り込み許可
}

//============================================================================= 
// Function Name : stSPAnswer
// Description   : stSPAnswer_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stSPAnswer(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stSPAnswer_ID;

	DD_TAU_Stop(gDD_STM_CtrlTable[motorID].timerCH);

	gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex;

	if( gDD_STM_MotorManager[motorID].func != NULL ){
		gDD_STM_MotorManager[motorID].func(gDD_STM_MotorManager[motorID].spResult);
	}

	DD_EVT_Put(evComplete_ID, evuDD_STM_ID, (UC*)motorID, 0);
}

//============================================================================= 
// Function Name : stShutDown
// Description   : stShutDown_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stShutDown(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stShutDown_ID;

	DD_DMA_Stop(gDD_STM_CtrlTable[motorID].dmacCH);								// DMA転送を停止
	DD_TAU_Stop(gDD_STM_CtrlTable[motorID].timerCH);							// タイマ割り込みを停止

	gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex;
	gDD_STM_MotorManager[motorID].count.controlType = DD_STM_NOT_COUNT_CLOCK;

	DD_EVT_Put(evComplete_ID, evuDD_STM_ID, (UC*)motorID, 0);
}

//============================================================================= 
// Function Name : stPTPulseWait
// Description   : stPTPulseWait_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stPTPulseWait(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stPTPulseWait_ID;

	gDD_STM_MotorManager[motorID].count.clockCount = gDD_STM_MotorManager[motorID].nowIndex - gDD_STM_MotorManager[motorID].count.countStartIndex;

	DD_TAU_IntEnable(gDD_STM_CtrlTable[motorID].timerCH);						// 指定モータのタイマ割り込み許可
}

//============================================================================= 
// Function Name : stPTEntry
// Description   : stPTEntry_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stPTEntry(DD_STM_ID motorID)
{
	UC loopCnt;

	if( gDD_STM_MotorManager[motorID].state == stPTPulseWait_ID ){
		if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_PULSE_TIMING ){
			if( gDD_STM_MotorManager[motorID].count.remain <= 2 ){
				gDD_STM_MotorManager[motorID].count.remain = 0;
			}
			else{
				gDD_STM_MotorManager[motorID].count.remain -= 2;
			}
		}
		else{
			gDD_STM_MotorManager[motorID].count.clockCount += 2;
		}
		for( loopCnt = 0; loopCnt < DD_STM_PROF_CONST_STEP_SIZE; loopCnt++ ){	// DMA転送用 RAMデータ作成
			gDD_STM_MotorManager[motorID].constTable[loopCnt] = gDD_STM_ProfileTable[gDD_STM_MotorManager[motorID].nowIndex];
		}
	}

	gDD_STM_MotorManager[motorID].state = stPTEntry_ID;

	if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_PULSE_TIMING ){
		if( gDD_STM_MotorManager[motorID].count.remain <= 0 ){
			DD_EVT_Put(evGoCancel_ID, evuDD_STM_ID, (UC*)(motorID), 0);
		}
		else if( gDD_STM_MotorManager[motorID].count.remain <= DD_STM_PROF_CONST_STEP_SIZE ){
			DD_DMA_Start(gDD_STM_CtrlTable[motorID].dmacCH, &(gDD_STM_MotorManager[motorID].constTable[1]), (gDD_STM_MotorManager[motorID].count.remain - 1));
			DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);
		}
		else{
			DD_DMA_Start(gDD_STM_CtrlTable[motorID].dmacCH, &(gDD_STM_MotorManager[motorID].constTable[1]), (DD_STM_PROF_CONST_STEP_SIZE - 1));
			DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);
		}
	}
	else{
		DD_DMA_Start(gDD_STM_CtrlTable[motorID].dmacCH, &(gDD_STM_MotorManager[motorID].constTable[1]), (DD_STM_PROF_CONST_STEP_SIZE - 1));
		DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
}

//============================================================================= 
// Function Name : stPTExec
// Description   : stPTExec_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stPTExec(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stPTExec_ID;
}

//============================================================================= 
// Function Name : stPTExecPulseWait
// Description   : stPTExecPulseWait_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stPTExecPulseWait(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stPTExecPulseWait_ID;

	DD_TAU_IntEnable(gDD_STM_CtrlTable[motorID].timerCH);						// 指定モータのタイマ割り込み許可
}

//============================================================================= 
// Function Name : stPTJudge
// Description   : stPTJudge_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stPTJudge(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stPTJudge_ID;

	if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_COUNT_CLOCK ){
		gDD_STM_MotorManager[motorID].count.clockCount += DD_STM_PROF_CONST_STEP_SIZE;

		DD_EVT_Put(evGoStart_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
	else{																		// controlType == DD_STM_PULSE_TIMING
		if( gDD_STM_MotorManager[motorID].count.remain > DD_STM_PROF_CONST_STEP_SIZE ){
			gDD_STM_MotorManager[motorID].count.remain -= DD_STM_PROF_CONST_STEP_SIZE;

			DD_EVT_Put(evGoStart_ID, evuDD_STM_ID, (UC*)(motorID), 0);
		}
		else{
			gDD_STM_MotorManager[motorID].count.remain = 0;

			DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);
		}
	}
}

//============================================================================= 
// Function Name : stPTAnswer
// Description   : stPTAnswer_IDへの遷移処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 
//=============================================================================
void stPTAnswer(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stPTAnswer_ID;
	gDD_STM_MotorManager[motorID].count.controlType = DD_STM_NOT_COUNT_CLOCK;

	if( gDD_STM_MotorManager[motorID].count.func != NULL ){
		gDD_STM_MotorManager[motorID].count.func(DD_STM_RESULT_OK);
	}

	DD_EVT_Put(evComplete_ID, evuDD_STM_ID, (UC*)(motorID), 0);
}

//============================================================================= 
// Function Name : SetProfileIDExec
// Description   : Profile ID セット処理
// Parameter     : motorID		"モータのID"
// Return        : 
// Note          : 状態遷移は行わない
//=============================================================================
void SetProfileIDExec(DD_STM_ID motorID, DD_STM_PROF_ID profileID)
{
	if( gDD_STM_Profile[profileID].profileID == profileID ){					// プロファイル設定済みのprofileIDのとき
		gDD_STM_MotorManager[motorID].profileID = profileID;
		gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_Profile[profileID].idleIndex;
	}
	else{																		// プロファイル設定をしていないprofileIDのとき
		/* Irregular Case */
	}
}

//============================================================================= 
// Function Name : SetChangeClockData
// Description   : クロック変更に必要なデータをセット
// Parameter     : motorID		"モータのID"
// Return        : true			"クロック変更あり"
//				 : false		"クロック変更なし"
// Note          : 
//=============================================================================
bool SetChangeClockData(DD_STM_ID motorID)
{
	US nowIndex, targetIndex, indexConvPlsCnt, movePulseCnt;

	nowIndex = gDD_STM_MotorManager[motorID].nowIndex;
	targetIndex = gDD_STM_MotorManager[motorID].targetIndex;

	// 現在インデックス、目標インデックスの設定
	if( ((targetIndex < gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh) &&	// 目標インデックス、現在インデックが共に、Slow-Upインデックス
		 (nowIndex < gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh)) ||
		((targetIndex >= gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh) &&	// 目標インデックス、現在インデックが共に、Slow-Downインデックス
		 (nowIndex >= gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh)) ){

		if( targetIndex < nowIndex ){
			if( gDD_STM_ConvertIndexTable[nowIndex / 2] == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// 現在インデックスの変換インデックスがidleのとき
				gDD_STM_MotorManager[motorID].targetIndex = gDD_STM_MotorManager[motorID].nowIndex;	// クロック変更を行わない(両インデックスの変換先がIdleのため)

				indexConvPlsCnt = 0;
			}
			else{																// 現在インデックスの変換インデックスがidleではないとき
				if( gDD_STM_ConvertIndexTable[targetIndex / 2] == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// 目標インデックスの変換インデックスがidleのとき
					gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_ConvertIndexTable[nowIndex / 2];
					gDD_STM_MotorManager[motorID].targetIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex;	// profEndIndexをセットする

					indexConvPlsCnt = 2;
				}
				else{															// 目標インデックスの変換インデックスがidleではないとき
					gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_ConvertIndexTable[nowIndex / 2];
					gDD_STM_MotorManager[motorID].targetIndex = gDD_STM_ConvertIndexTable[targetIndex / 2];

					indexConvPlsCnt = 2;
				}
			}
		}
		else{
			indexConvPlsCnt = 0;
		}
	}
	else{
		if(	gDD_STM_ProfileTable[nowIndex] == gDD_STM_ProfileTable[targetIndex] ){	// 現在インデックスの周波数と、目標インデックスの周波数が同じとき
			gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_MotorManager[motorID].targetIndex;

			indexConvPlsCnt = 0;
		}
		else{																	// 現在インデックスの周波数と、目標インデックスの周波数が違うとき
			if( gDD_STM_ConvertIndexTable[nowIndex / 2] == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// 現在インデックスの変換インデックスがidleのとき
				gDD_STM_MotorManager[motorID].targetIndex = gDD_STM_ConvertIndexTable[targetIndex / 2];

				indexConvPlsCnt = 0;
			}
			else{																// 現在インデックスの変換インデックスがidleではないとき
				if( gDD_STM_ConvertIndexTable[targetIndex / 2] == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// 目標インデックスの変換インデックスがidleのとき
					gDD_STM_MotorManager[motorID].targetIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex;	// profEndIndexをセットする

					indexConvPlsCnt = 0;
				}
				else{
					if( targetIndex < gDD_STM_ConvertIndexTable[nowIndex / 2] ){
						gDD_STM_MotorManager[motorID].targetIndex = gDD_STM_ConvertIndexTable[targetIndex / 2];

						indexConvPlsCnt = 0;
					}
					else{
						gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_ConvertIndexTable[nowIndex / 2];

						indexConvPlsCnt = 2;									// 現在インデックス変換をした場合は、+2
					}
				}
			}
		}
	}

	// クロック出力
	movePulseCnt = gDD_STM_MotorManager[motorID].targetIndex - gDD_STM_MotorManager[motorID].nowIndex + indexConvPlsCnt;

	if( movePulseCnt > 0){
		if( indexConvPlsCnt == 0 ){												// 現在インデックスを変換していない場合
			DD_DMA_Start(gDD_STM_CtrlTable[motorID].dmacCH,					// 次インデックスからクロック出力開始
						 &(gDD_STM_ProfileTable[gDD_STM_MotorManager[motorID].nowIndex + 2 + 1]),
						 (movePulseCnt - 1));
			DD_TAU_SetTDR(gDD_STM_CtrlTable[motorID].timerCH, gDD_STM_ProfileTable[gDD_STM_MotorManager[motorID].nowIndex + 2]);
			if( gDD_STM_MotorManager[motorID].nowIndex == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// Clock出力停止中のときは、クロックスタートする
				DD_TAU_Start(gDD_STM_CtrlTable[motorID].timerCH);
			}
		}
		else{																	// 現在インデックスを変換した場合
			DD_DMA_Start(gDD_STM_CtrlTable[motorID].dmacCH,					// 現在インデックスからクロック出力開始
						 &(gDD_STM_ProfileTable[gDD_STM_MotorManager[motorID].nowIndex + 1]),
						 (movePulseCnt - 1));
			DD_TAU_SetTDR(gDD_STM_CtrlTable[motorID].timerCH, gDD_STM_ProfileTable[gDD_STM_MotorManager[motorID].nowIndex]);
			if( gDD_STM_MotorManager[motorID].nowIndex == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// Clock出力停止中のときは、クロックスタートする
				DD_TAU_Start(gDD_STM_CtrlTable[motorID].timerCH);
			}
		}

		return true;
	}
	else{
		return false;
	}
}

//============================================================================= 
// Function Name : SetStopClockData
// Description   : クロック変更に必要なデータをセット
// Parameter     : motorID		"モータのID"
// Return        : true			"Slow-Downあり"
//				 : false		"Slow-Downなし"
// Note          : 
//=============================================================================
bool SetStopClockData(DD_STM_ID motorID)
{
	US nowIndex, targetIndex, indexConvPlsCnt, movePulseCnt;

	nowIndex = gDD_STM_MotorManager[motorID].nowIndex;
	targetIndex = gDD_STM_MotorManager[motorID].targetIndex;

	// 現在インデックス、目標インデックスの設定
	if( gDD_STM_ConvertIndexTable[nowIndex / 2] == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// 現在インデックスの変換インデックスがidleのとき
		gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_MotorManager[motorID].targetIndex;

		indexConvPlsCnt = 0;
	}
	else{																		// 現在インデックスの変換インデックスがidleではないとき
		if( nowIndex < gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh ){	// 現在インデックスがSlow-Upインデックスのとき
			gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_ConvertIndexTable[nowIndex / 2];
			gDD_STM_MotorManager[motorID].targetIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex;	// profEndIndexをセットする

			indexConvPlsCnt = 2;
		}
		else{																	// 現在インデックスがSlow-Downインデックスのとき
			indexConvPlsCnt = 0;
		}
	}

	// クロック出力
	movePulseCnt = gDD_STM_MotorManager[motorID].targetIndex - gDD_STM_MotorManager[motorID].nowIndex + indexConvPlsCnt;

	if( movePulseCnt > 0){
		if( indexConvPlsCnt == 0 ){												// 現在インデックスを変換していない場合
			DD_DMA_Start(gDD_STM_CtrlTable[motorID].dmacCH,					// 次インデックスからクロック出力開始
						 &(gDD_STM_ProfileTable[gDD_STM_MotorManager[motorID].nowIndex + 2 + 1]),
						 (movePulseCnt - 1));
			DD_TAU_SetTDR(gDD_STM_CtrlTable[motorID].timerCH, gDD_STM_ProfileTable[gDD_STM_MotorManager[motorID].nowIndex + 2]);
			if( gDD_STM_MotorManager[motorID].nowIndex == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// Clock出力停止中のときは、クロックスタートする
				DD_TAU_Start(gDD_STM_CtrlTable[motorID].timerCH);
			}
		}
		else{																	// 現在インデックスを変換した場合
			DD_DMA_Start(gDD_STM_CtrlTable[motorID].dmacCH,					// 現在インデックスからクロック出力開始
						 &(gDD_STM_ProfileTable[gDD_STM_MotorManager[motorID].nowIndex + 1]),
						 (movePulseCnt - 1));
			DD_TAU_SetTDR(gDD_STM_CtrlTable[motorID].timerCH, gDD_STM_ProfileTable[gDD_STM_MotorManager[motorID].nowIndex]);
			if( gDD_STM_MotorManager[motorID].nowIndex == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// Clock出力停止中のときは、クロックスタートする
				DD_TAU_Start(gDD_STM_CtrlTable[motorID].timerCH);
			}
		}

		return true;
	}
	else{
		return false;
	}
}

//============================================================================= 
// Function Name : SetCountStartIndex
// Description   : 出力クロック数カウントのデータセット
// Parameter     : arg  : モータID値									
// Return        : -										
// Note          : -
//=============================================================================
void SetCountStartIndex(DD_STM_ID motorID)
{
	US remainCnt = DD_DMA_GetRemainTransferCnt(gDD_STM_CtrlTable[motorID].dmacCH);

	if( gDD_STM_MotorManager[motorID].count.countStartIndex == 0xFFFF ){
		gDD_STM_MotorManager[motorID].count.countStartIndex = gDD_STM_MotorManager[motorID].targetIndex - ((remainCnt / 2) * 2);
	}
	else{
		/* No Action(すでにデータセットをしているため) */
	}
}


//============================================================================= 
// Function Name : TransferEndCallback							
// Description   : 転送完了時にコールバックされる割込み処理
// Parameter     : arg  : モータID値									
// Return        : -										
// Note          : -
//=============================================================================
void TransferEndCallback(void* arg)
{
	DD_STM_ID motorID = (DD_STM_ID)(arg);

	switch( gDD_STM_MotorManager[motorID].state ){
	case stChangeExecute_ID:
	case stStopExecute_ID:
	case stSPChange_ID:
	case stSPConstant_ID:
	case stSPStop_ID:
	case stPTExec_ID:
	//↓↓(TB-XXX) DMA転送割込み処理ができず、App側にCallbackしない -> 以下の"stOOEntry_ID"追加.
	case stSPChangeEntry_ID :
	case stSPConstEntry_ID :
	case stSPStopEntry_ID :
	case stChangeEntry_ID :
	case stPTEntry_ID :
	case stStopEntry_ID :
		DD_EVT_Put(evSend_ID, evuDD_STM_ID, (UC*)(motorID), 0);
		break;

	default:
		/* No Action */
		break;
	}
}

//============================================================================= 
// Function Name : TimeoutCallback							
// Description   : タイマ(CPU)のタイムアウト時にコールバックされる割込み処理									
// Parameter     : arg : モータのID値											
// Return        : -									
// Note          : -
//=============================================================================
void TimeoutCallback(void* arg)
{
	DD_STM_ID motorID = (DD_STM_ID)(arg);

	if( DD_TAU_GetLevel(gDD_STM_CtrlTable[motorID].timerCH) == 0x00 ){		// TimeOutレベルがLowの場合（Highなら何もしない）
		switch( gDD_STM_MotorManager[motorID].state ){
		case stChangePulseWait_ID:
		case stStopPulseWait_ID:
		case stStopExecPulseWait_ID:
		case stSPPulseWait_ID:
		case stSPChangePulseWait_ID:
		case stSPConstPulseWait_ID:		// ←要検討 T.B.D.
		case stSPStopPulseWait_ID:
		case stPTPulseWait_ID:
		case stPTExecPulseWait_ID:
			DD_EVT_Put(evGoComplete_ID, evuDD_STM_ID, (UC*)(motorID), 0);
			break;

		default:
			/* No Action */
			break;
		}

		DD_TAU_IntDisable(gDD_STM_CtrlTable[motorID].timerCH);					// Timer割込みを無効にする
	}
}

