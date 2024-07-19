// File Name:	CSysFinSttChangeDiag.c
// Description:	Implimentation File of Finisher State "Change Diag"
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSysFinSttChangeDiag.h"

#include "CShtManager.h"
#include "CCdiAdapter.h"
#include "CCvrChecker.h"
#include "CJamChecker.h"
#include "CSeqDevInitHandler.h"
#include "CDvstInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Finisher State ---
CSysFinisherState const gSys_FinSttChangeDiagMethod ={
	CSysFinSttChangeDiag_Constructor,						// Constructor
	CSysFinSttChangeDiag_IsTransitionState,					// IsTransitionState
	CSysFinSttChangeDiag_CheckTransitionPossibility,		// CheckTransitionPossibility
	CSysFinSttChangeDiag_CheckExitCondition,				// CheckExitCondition
	CSysFinSttChangeDiag_GetNextState,						// GetNextState
	CSysFinSttChangeDiag_Entry,								// Entry
	CSysFinisherState_StdExit,								// Exit
	CSysFinisherState_StdSendDeviceActionRequest,			// SendDeviceActionRequest
	CSysFinisherState_StdSendDeviceEventNotify,				// SendDeviceEventNotify
	CSysFinisherState_StdReceivePowerUpResponse,			// ReceivePowerUpResponse
	CSysFinisherState_StdReceivePowerDownResponse,			// ReceivePowerDownResponse
	CSysFinSttChangeDiag_ReceiveChangeDiagResponse,			// ReceiveChangeDiagResponse
	CSysFinisherState_StdReceiveChangeNormalResponse,		// ReceiveChangeNormalResponse
	CSysFinisherState_StdReceiveCycleUpResponse,			// ReceiveCycleUpResponse
	CSysFinisherState_StdReceiveProcessDownResponse,		// ReceiveProcessDownResponse
//  V3.4.0 T/B対策　Merge No.55
//	CSysFinisherState_StdReceiveDeviceActionResponse,		// ReceiveDeviceActionResponse
	CSysFinSttChangeDiag_ReceiveDeviceActionResponse, // ReceiveDeviceActionResponse 
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
// Function Name : CSysFinSttChangeDiag_Constructor
// Description   : 
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttChangeDiag_Constructor()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttChangeDiag_IsTransitionState
// Description   : 
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
bool CSysFinSttChangeDiag_IsTransitionState()
{
	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttChangeDiag_CheckTransitionPossibility
// Description   : 状態遷移可否確認処理
// Parameter     : nextState (遷移先状態)
// Return        : 状態遷移可否
//               :  true = 遷移可能
//               :  false = 遷移付加
///////////////////////////////////////////////////////////////////
bool CSysFinSttChangeDiag_CheckTransitionPossibility(EFinisherState nextState)
{
	static US const cTransPsbl = {
		FIN_STT_CNV_BIT(FIN_STT_DIAGNOSTIC)
	};

	if ( cTransPsbl & FIN_STT_CNV_BIT(nextState) ) return true;
	else										   return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttChangeDiag_CheckExitCondition
// Description   : 現状態からの遷移条件成立確認処理
// Parameter     : ‐
// Return        : 状態遷移条件成立状況
//               :  true = 状態遷移条件成立済み
//               :  false = 状態遷移条件未成立
///////////////////////////////////////////////////////////////////
bool CSysFinSttChangeDiag_CheckExitCondition()
{
	if ( gSys_FinisherStateCommonMember.mWaitingModuleBit == 0x0000 ) return true;
	else															  return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdGetNextState
// Description   : 過渡状態における遷移先状態確認処理
// Parameter     : ‐
// Return        : 遷移先フィニッシャー状態
///////////////////////////////////////////////////////////////////
EFinisherState CSysFinSttChangeDiag_GetNextState()
{
	return FIN_STT_DIAGNOSTIC;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttChangeDiag_Entry
// Description   : 現状態への遷移時処理
// Parameter     : prevState (遷移元フィニッシャー状態)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttChangeDiag_Entry(EFinisherState prevState)
{
	CSeqDevInitHandler_Stop();													// イニシャライズ動作を中止する

	// --- Device Moduleへ状態遷移依頼 ---
	gSys_FinisherStateCommonMember.mWaitingModuleBit = MID_DEV_MDL_BIT_ALL;

	MSP_SendMessage(MID_SYS, EVT_STT_CHANGE_DIAG_REQ, NULL, 0, LOG_OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttChangeDiag_ReceiveChangeDiagResponse
// Description   : Change Diag Responseイベント受信時処理
// Parameter     : moduleID (Device Module ID)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinSttChangeDiag_ReceiveChangeDiagResponse(UC moduleID)
{
	gSys_FinisherStateCommonMember.mWaitingModuleBit &= ~MID_CNV_BIT(moduleID);

	return true;
}

///V3.0.4 T/B対策　Merge No.55////////////////////////////////////////////////////////
// Function Name : CSysFinSttChangeDiag_ReceiveDeviceActionResponse
// Description   : Device Action Responseイベント受信時処理
// Parameter     : moduleID (Device Module ID)
//               : eventID (MSP Event ID)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinSttChangeDiag_ReceiveDeviceActionResponse(UC moduleID, US eventID)
{
	UC initState;

	switch( eventID ){
	// !!! Product Specific !!!
//	case EVT_DEV_RES_XXXX:
//
//		break;
	default:
		break;
	}

	// --- イニシャライズ動作停止待ち ---
	initState = CSeqDevInitHandler_DeviceActionResponse(eventID, moduleID);

	if ( initState == SEQ_ACT_STT_IDLE ){
		CDvstDeviceStatus_SetStatusValue(CDvstInformer_GetDeviceStatus(DSNO_INIT_STATE), 0);
		CSysFinisherState_DeviceStatusChangeProcess(DSNO_INIT_STATE, 0);
		MSP_SendEvent(MID_SYS, EVT_DEV_EVT_DEV_INIT_COMP, NULL, 0, LOG_OFF);
	}
	else{ /* No Action */ }

	// --- CDIメッセージ送信 ---
	CCdiAdapter_SendDeviceActionResponse(eventID);

	return false;
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.11.06	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVM ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
