// File Name:	CSysFinSttHardDown.c
// Description:	Implimentation File of Finisher State "Hard Down"
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSysFinSttHardDown.h"

#include "CShtManager.h"
#include "CCdiAdapter.h"
#include "CJamChecker.h"
#include "CSeqDevInitHandler.h"
#include "CSeqProcEndHandler.h"
#include "CDvstInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Hard Down State ---
CSysFinisherState const gSys_FinSttHardDownMethod = {
	CSysFinSttHardDown_Constructor,							// Constructor
	CSysFinSttHardDown_IsTransitionState,					// IsTransitionState
	CSysFinSttHardDown_CheckTransitionPossibility,			// CheckTransitionPossibility
	CSysFinSttHardDown_CheckExitCondition,					// CheckExitCondition
	CSysFinSttHardDown_GetNextState,						// GetNextState
	CSysFinSttHardDown_Entry,								// Entry
	CSysFinSttHardDown_Exit,								// Exit
	CSysFinisherState_StdSendDeviceActionRequest,			// SendDeviceActionRequest
	CSysFinisherState_StdSendDeviceEventNotify,				// SendDeviceEventNotify
	CSysFinisherState_StdReceivePowerUpResponse,			// ReceivePowerUpResponse
	CSysFinisherState_StdReceivePowerDownResponse,			// ReceivePowerDownResponse
	CSysFinisherState_StdReceiveChangeDiagResponse,			// ReceiveChangeDiagResponse
	CSysFinisherState_StdReceiveChangeNormalResponse,		// ReceiveChangeNormalResponse
	CSysFinisherState_StdReceiveCycleUpResponse,			// ReceiveCycleUpResponse
	CSysFinSttHardDown_ReceiveProcessDownResponse,			// ReceiveProcessDownResponse
	CSysFinSttHardDown_ReceiveDeviceActionResponse,			// ReceiveDeviceActionResponse
	CSysFinisherState_StdReceiveDeviceEventNotify,			// ReceiveDeviceEventNotify
	CSysFinisherState_StdReceiveDeviceStatusNotify,			// ReceiveDeviceStatusNotify
	CSysFinisherState_StdSystemConfigurationSet,			// SystemConfigurationSet
	CSysFinisherState_StdPowerUpModeSet,					// PowerUpModeSet
	CSysFinisherState_StdEntranceAreaTampered,				// EntranceAreaTampered
	CSysFinisherState_StdLoadNvmComplete,					// LoadNvmComplete
	CSysFinisherState_StdSaveNvmComplete,					// SaveNvmComplete
	CSysFinisherState_StdSheetManagerReady,					// SheetManagerReady
	CSysFinisherState_StdCoverStatusChange,					// CoverStatusChange
	CSysFinisherState_StdFaultStatusChange,					// FaultStatusChange
	CSysFinisherState_StdJamZoneStatusChange				// JamZoneStatusChange
};

/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttHardDown_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttHardDown_Constructor()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttHardDown_IsTransitionState
// Description   : 
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
bool CSysFinSttHardDown_IsTransitionState()
{
	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttHardDown_CheckTransitionPossibility
// Description   : 状態遷移可否確認処理
// Parameter     : nextState (遷移先状態)
// Return        : 状態遷移可否
//               :  true = 遷移可能
//               :  false = 遷移付加
///////////////////////////////////////////////////////////////////
bool CSysFinSttHardDown_CheckTransitionPossibility(EFinisherState nextState)
{
	static US const cTransPsbl = {
		FIN_STT_CNV_BIT(FIN_STT_STANDBY)// |
	//	FIN_STT_CNV_BIT(FIN_STT_CYCLE_DOWN)	//V3.1.7 
	};

	if ( cTransPsbl & FIN_STT_CNV_BIT(nextState) ) return true;
	else										   return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttHardDown_CheckExitCondition
// Description   : 現状態からの遷移条件成立確認処理
// Parameter     : ‐
// Return        : 状態遷移条件成立状況
//               :  true = 状態遷移条件成立済み
//               :  false = 状態遷移条件未成立
///////////////////////////////////////////////////////////////////
bool CSysFinSttHardDown_CheckExitCondition()
{
	if ( (gSys_FinisherStateCommonMember.mWaitingModuleBit == 0x0000) &&
		 (CSeqProcEndHandler_GetState() == SEQ_ACT_STT_IDLE) ){
		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttHardDown_GetNextState
// Description   : 過渡状態における遷移先状態確認処理
// Parameter     : ‐
// Return        : 遷移先フィニッシャー状態
///////////////////////////////////////////////////////////////////
EFinisherState CSysFinSttHardDown_GetNextState()
{
	return FIN_STT_STANDBY;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttHardDown_Entry
// Description   : 現状態への遷移時処理
// Parameter     : prevState (遷移元フィニッシャー状態)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttHardDown_Entry(EFinisherState prevState)
{
	CShtManager_StopProcess(SYS_PROC_STOP_HARD);								// スケジューリングを停止

	// --- Device Moduleへ状態遷移依頼 ---
	if ( prevState != FIN_STT_CYCLE_DOWN ){
		gSys_FinisherStateCommonMember.mWaitingModuleBit = MID_DEV_MDL_BIT_ALL;
	}
	else{ /* No Action */ }

	MSP_SendMessage(MID_SYS, EVT_STT_HARD_DOWN_REQ, NULL, 0, LOG_OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttHardDown_Exit
// Description   : 他状態への遷移時処理
// Parameter     : nextState (遷移先フィニッシャー状態)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttHardDown_Exit(EFinisherState nextState)
{
	CShtManager_AbortCompileSheetProcessEnd();
	CShtManager_ProcessEnd();
	CJamChecker_FinishingProcessEnd();
	CShtManager_StandbyEject();

	if ( gSys_FinisherStateCommonMember.mInitializeReserve == true ){
		gSys_FinisherStateCommonMember.mInitializeReserve = false;

		if ( CSeqDevInitHandler_Start(SYS_PWR_UP_MODE_NORMAL) != SEQ_ACT_STT_NUM ){
			CDvstDeviceStatus_SetStatusValue(CDvstInformer_GetDeviceStatus(DSNO_INIT_STATE), 1);
			CSysFinisherState_DeviceStatusChangeProcess(DSNO_INIT_STATE, 1);
		}
		else{
			// No Action
		}
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttHardDown_ReceiveProcessDownResponse
// Description   : Process Down Responseイベント受信時処理
// Parameter     : moduleID (Device Module ID)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinSttHardDown_ReceiveProcessDownResponse(UC moduleID)
{
	gSys_FinisherStateCommonMember.mWaitingModuleBit &= ~MID_CNV_BIT(moduleID);

	if ( gSys_FinisherStateCommonMember.mWaitingModuleBit == 0x0000 ){
		CJamChecker_AllSheetFinishComplete();
		CSeqProcEndHandler_Start();
	}
	else{
		// No Action
	}

	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttHardDown_ReceiveDeviceActionResponse
// Description   : Device Action Responseイベント受信時処理
// Parameter     : moduleID (Device Module ID)
//               : eventID (MSP Event ID)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinSttHardDown_ReceiveDeviceActionResponse(UC moduleID, US eventID)
{
	UC returnVal = false;  //V3.1.7 

	switch( eventID ){
	// !!! Product Specific !!!
//	case EVT_DEV_RES_XXXX:
//
//		break;
	default:
		break;
	}

	if ( CSeqProcEndHandler_DeviceActionResponse(eventID, moduleID) == SEQ_ACT_STT_IDLE ){
		returnVal = true;
	}
	else{
		// No Action
	}

	CCdiAdapter_SendDeviceActionResponse(eventID);

	return returnVal;
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
// 15.03.30	| Yuichi Hoshino	| Trouble Action [ カバークローズイニシャライズ動作実施せず ]
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVM ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
