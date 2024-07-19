// File Name:	CSysFinSttCycleUp.c
// Description:	Implimentation File of Finisher State "Cycle Up"
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSysFinSttCycleUp.h"

#include "CShtManager.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Cycle Up State ---
CSysFinisherState const gSys_FinSttCycleUpMethod = {
	CSysFinSttCycleUp_Constructor,							// Constructor
	CSysFinSttCycleUp_IsTransitionState,					// IsTransitionState
	CSysFinSttCycleUp_CheckTransitionPossibility,			// CheckTransitionPossibility
	CSysFinSttCycleUp_CheckExitCondition,					// CheckExitCondition
	CSysFinSttCycleUp_GetNextState,							// GetNextState
	CSysFinSttCycleUp_Entry,								// Entry
	CSysFinisherState_StdExit,								// Exit
	CSysFinisherState_StdSendDeviceActionRequest,			// SendDeviceActionRequest
	CSysFinisherState_StdSendDeviceEventNotify,				// SendDeviceEventNotify
	CSysFinisherState_StdReceivePowerUpResponse,			// ReceivePowerUpResponse
	CSysFinisherState_StdReceivePowerDownResponse,			// ReceivePowerDownResponse
	CSysFinisherState_StdReceiveChangeDiagResponse,			// ReceiveChangeDiagResponse
	CSysFinisherState_StdReceiveChangeNormalResponse,		// ReceiveChangeNormalResponse
	CSysFinSttCycleUp_ReceiveCycleUpResponse,				// ReceiveCycleUpResponse
	CSysFinisherState_StdReceiveProcessDownResponse,		// ReceiveProcessDownResponse
	CSysFinisherState_StdReceiveDeviceActionResponse,		// ReceiveDeviceActionResponse
	CSysFinisherState_StdReceiveDeviceEventNotify,			// ReceiveDeviceEventNotify
	CSysFinisherState_StdReceiveDeviceStatusNotify,			// ReceiveDeviceStatusNotify
	CSysFinisherState_StdSystemConfigurationSet,			// SystemConfigurationSet
	CSysFinisherState_StdPowerUpModeSet,					// PowerUpModeSet
	CSysFinisherState_StdEntranceAreaTampered,				// EntranceAreaTampered
	CSysFinisherState_StdLoadNvmComplete,					// LoadNvmComplete
	CSysFinisherState_StdSaveNvmComplete,					// SaveNvmComplete
	CSysFinSttCycleUp_SheetManagerReady,					// SheetManagerReady
	CSysFinisherState_StdCoverStatusChange,					// CoverStatusChange
	CSysFinisherState_StdFaultStatusChange,					// FaultStatusChange
	CSysFinisherState_StdJamZoneStatusChange				// JamZoneStatusChange
};

/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of Change Mode State ---
typedef struct /*CSysFinSttCycleUpMember*/ {
	UC mShtMgrReady;
} CSysFinSttCycleUpMember;

static CSysFinSttCycleUpMember gSys_FinSttCycleUpMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleUp_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttCycleUp_Constructor()
{
	gSys_FinSttCycleUpMember.mShtMgrReady = false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleUp_IsTransitionState
// Description   : 
// Parameter     : ‐
// Return        : 
///////////////////////////////////////////////////////////////////
bool CSysFinSttCycleUp_IsTransitionState()
{
	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleUp_CheckTransitionPossibility
// Description   : 状態遷移可否確認処理
// Parameter     : nextState (遷移先状態)
// Return        : 状態遷移可否
//               :  true = 遷移可能
//               :  false = 遷移付加
///////////////////////////////////////////////////////////////////
bool CSysFinSttCycleUp_CheckTransitionPossibility(EFinisherState nextState)
{
	static US const cTransPsbl = {
		FIN_STT_CNV_BIT(FIN_STT_READY)|
		FIN_STT_CNV_BIT(FIN_STT_CYCLE_DOWN)|
		FIN_STT_CNV_BIT(FIN_STT_HARD_DOWN)
	};

	if ( cTransPsbl & FIN_STT_CNV_BIT(nextState) ) return true;
	else										   return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleUp_CheckExitCondition
// Description   : 現状態からの遷移条件成立確認処理
// Parameter     : ‐
// Return        : 状態遷移条件成立状況
//               :  true = 状態遷移条件成立済み
//               :  false = 状態遷移条件未成立
///////////////////////////////////////////////////////////////////
bool CSysFinSttCycleUp_CheckExitCondition()
{
	if ( (gSys_FinisherStateCommonMember.mWaitingModuleBit == 0x0000) &&
		 (gSys_FinSttCycleUpMember.mShtMgrReady == true) ){
		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleUp_GetNextState
// Description   : 過渡状態における遷移先状態確認処理
// Parameter     : ‐
// Return        : 遷移先フィニッシャー状態
///////////////////////////////////////////////////////////////////
EFinisherState CSysFinSttCycleUp_GetNextState()
{
	return FIN_STT_READY;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleUp_Entry
// Description   : 現状態への遷移時処理
// Parameter     : prevState (遷移元フィニッシャー状態)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttCycleUp_Entry(EFinisherState prevState)
{
//	gSys_FinSttCycleUpMember.mShtMgrReady = false;  //V3.4.0 Merge  No57 
//	CShtManager_ProcessStart();

	// --- Device Moduleへ状態遷移依頼 ---
	gSys_FinisherStateCommonMember.mWaitingModuleBit = MID_DEV_MDL_BIT_ALL;

	MSP_SendMessage(MID_SYS, EVT_STT_CYCLE_UP_REQ, NULL, 0, LOG_OFF);

	gSys_FinSttCycleUpMember.mShtMgrReady = false;  

	CShtManager_ProcessStart();
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleUp_ReceiveCycleUpResponse
// Description   : Cycle Up Responseイベント受信時処理
// Parameter     : moduleID (Device Module ID)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinSttCycleUp_ReceiveCycleUpResponse(UC moduleID)
{
	gSys_FinisherStateCommonMember.mWaitingModuleBit &= ~MID_CNV_BIT(moduleID);

	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleUp_SheetManagerReady
// Description   : シートマネージャーのレディー遷移通知受信時処理
// Parameter     : ‐
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinSttCycleUp_SheetManagerReady()
{
	gSys_FinSttCycleUpMember.mShtMgrReady = true;

	return true;
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.04.08	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVM ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
