// File Name:	CSysFinSttCycleDown.c
// Description:	Implimentation File of Finisher State "Cycle Down"
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSysFinSttCycleDown.h"

#include "CShtManager.h"
#include "CCdiAdapter.h"
#include "CJamChecker.h"
#include "CSeqDevInitHandler.h"
#include "CSeqProcEndHandler.h"
#include "CDvstInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Finisher State ---
CSysFinisherState const gSys_FinSttCycleDownMethod = {
	CSysFinSttCycleDown_Constructor,						// Constructor
	CSysFinSttCycleDown_IsTransitionState,					// IsTransitionState
	CSysFinSttCycleDown_CheckTransitionPossibility,			// CheckTransitionPossibility
	CSysFinSttCycleDown_CheckExitCondition,					// CheckExitCondition
	CSysFinSttCycleDown_GetNextState,						// GetNextState
	CSysFinSttCycleDown_Entry,								// Entry
	CSysFinSttCycleDown_Exit,								// Exit
	CSysFinisherState_StdSendDeviceActionRequest,			// SendDeviceActionRequest
	CSysFinisherState_StdSendDeviceEventNotify,				// SendDeviceEventNotify
	CSysFinisherState_StdReceivePowerUpResponse,			// ReceivePowerUpResponse
	CSysFinisherState_StdReceivePowerDownResponse,			// ReceivePowerDownResponse
	CSysFinisherState_StdReceiveChangeDiagResponse,			// ReceiveChangeDiagResponse
	CSysFinisherState_StdReceiveChangeNormalResponse,		// ReceiveChangeNormalResponse
	CSysFinisherState_StdReceiveCycleUpResponse,			// ReceiveCycleUpResponse
	CSysFinSttCycleDown_ReceiveProcessDownResponse,			// ReceiveProcessDownResponse
	CSysFinSttCycleDown_ReceiveDeviceActionResponse,		// ReceiveDeviceActionResponse
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
// Function Name : CSysFinSttCycleDown_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttCycleDown_Constructor()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleDown_IsTransitionState
// Description   : 
// Parameter     : ‐
// Return        : 
///////////////////////////////////////////////////////////////////
bool CSysFinSttCycleDown_IsTransitionState()
{
	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleDown_CheckTransitionPossibility
// Description   : 状態遷移可否確認処理
// Parameter     : nextState (遷移先状態)
// Return        : 状態遷移可否
//               :  true = 遷移可能
//               :  false = 遷移付加
///////////////////////////////////////////////////////////////////
bool CSysFinSttCycleDown_CheckTransitionPossibility(EFinisherState nextState)
{
	static US const cTransPsbl = {
		FIN_STT_CNV_BIT(FIN_STT_HARD_DOWN)|
		FIN_STT_CNV_BIT(FIN_STT_STANDBY)
	};

	if ( cTransPsbl & FIN_STT_CNV_BIT(nextState) ) return true;
	else										   return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleDown_CheckExitCondition
// Description   : 現状態からの遷移条件成立確認処理
// Parameter     : ‐
// Return        : 状態遷移条件成立状況
//               :  true = 状態遷移条件成立済み
//               :  false = 状態遷移条件未成立
///////////////////////////////////////////////////////////////////
bool CSysFinSttCycleDown_CheckExitCondition()
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
// Function Name : CSysFinSttCycleDown_GetNextState
// Description   : 過渡状態における遷移先状態確認処理
// Parameter     : ‐
// Return        : 遷移先フィニッシャー状態
///////////////////////////////////////////////////////////////////
EFinisherState CSysFinSttCycleDown_GetNextState()
{
	return FIN_STT_STANDBY;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleDown_Entry
// Description   : 現状態への遷移時処理
// Parameter     : prevState (遷移元フィニッシャー状態)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttCycleDown_Entry(EFinisherState prevState)
{
	CShtManager_StopProcess(SYS_PROC_STOP_CYCLE);								// スケジューリングを停止

	// --- Device Moduleへ状態遷移依頼 ---
	gSys_FinisherStateCommonMember.mWaitingModuleBit = MID_DEV_MDL_BIT_ALL;

	MSP_SendMessage(MID_SYS, EVT_STT_CYCLE_DOWN_REQ, NULL, 0, LOG_OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleDown_Exit
// Description   : 他状態への遷移時処理
// Parameter     : nextState (遷移先フィニッシャー状態)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttCycleDown_Exit(EFinisherState nextState)
{
	if ( nextState == FIN_STT_STANDBY ){
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
	else{																		// Hard Down
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleDown_ReceiveProcessDownResponse
// Description   : Process Down Responseイベント受信時処理
// Parameter     : moduleID (Device Module ID)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinSttCycleDown_ReceiveProcessDownResponse(UC moduleID)
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
// Function Name : CSysFinSttCycleDown_ReceiveDeviceActionResponse
// Description   : Device Action Responseイベント受信時処理
// Parameter     : moduleID (Device Module ID)
//               : eventID (MSP Event ID)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinSttCycleDown_ReceiveDeviceActionResponse(UC moduleID, US eventID)
{
	UC returnVal;

	returnVal = false; //V3.4.0 Merge No.56
	
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
// 14.03.30	| Yuichi Hoshino	| Trouble Action [ カバークローズイニシャライズ動作実施せず ]
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVM ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
