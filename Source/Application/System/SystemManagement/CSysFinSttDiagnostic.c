// File Name:	CSysFinSttDiagnostic.c
// Description:	Implimentation File of Finisher State "Diagnostics"
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSysFinSttDiagnostic.h"

#include "CDiagManager.h"
#include "CSeqDevInitHandler.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Diagnostics State ---
CSysFinisherState const gSys_FinSttDiagnosticMethod = {
	CSysFinSttDiagnostic_Constructor,						// Constructor
	CSysFinisherState_StdIsTransitionState,					// IsTransitionState
	CSysFinSttDiagnostic_CheckTransitionPossibility,		// CheckTransitionPossibility
	CSysFinisherState_StdCheckExitCondition,				// CheckExitCondition
	CSysFinisherState_StdGetNextState,						// GetNextState
	CSysFinSttDiagnostic_Entry,								// Entry
	CSysFinSttDiagnostic_Exit,								// Exit
	CSysFinisherState_StdSendDeviceActionRequest,			// SendDeviceActionRequest
	CSysFinisherState_StdSendDeviceEventNotify,				// SendDeviceEventNotify
	CSysFinisherState_StdReceivePowerUpResponse,			// ReceivePowerUpResponse
	CSysFinisherState_StdReceivePowerDownResponse,			// ReceivePowerDownResponse
	CSysFinisherState_StdReceiveChangeDiagResponse,			// ReceiveChangeDiagResponse
	CSysFinisherState_StdReceiveChangeNormalResponse,		// ReceiveChangeNormalResponse
	CSysFinisherState_StdReceiveCycleUpResponse,			// ReceiveCycleUpResponse
	CSysFinisherState_StdReceiveProcessDownResponse,		// ReceiveProcessDownResponse
	CSysFinisherState_StdReceiveDeviceActionResponse,		// ReceiveDeviceActionResponse
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
// Function Name : CSysFinSttDiagnostic_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttDiagnostic_Constructor()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttDiagnostic_CheckTransitionPossibility
// Description   : 状態遷移可否確認処理
// Parameter     : nextState (遷移先状態)
// Return        : 状態遷移可否
//               :  true = 遷移可能
//               :  false = 遷移付加
///////////////////////////////////////////////////////////////////
bool CSysFinSttDiagnostic_CheckTransitionPossibility(EFinisherState nextState)
{
	static US const cTransPsbl = {
		FIN_STT_CNV_BIT(FIN_STT_POWER_DOWN)|
		FIN_STT_CNV_BIT(FIN_STT_CHANGE_NORMAL)
	};

	if ( cTransPsbl & FIN_STT_CNV_BIT(nextState) ) return true;
	else										   return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttDiagnostic_Entry
// Description   : 現状態への遷移時処理
// Parameter     : prevState (遷移元フィニッシャー状態)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttDiagnostic_Entry(EFinisherState prevState)
{
	MSP_SendMessage(MID_SYS, EVT_STT_DIAGNOSTIC_NOTIFY, NULL, 0, LOG_OFF);

	CDiagManager_DiagnosticsModeStart();
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttDiagnostic_Exit
// Description   : 他状態への遷移時処理
// Parameter     : nextState (遷移先フィニッシャー状態)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttDiagnostic_Exit(EFinisherState nextState)
{
	CDiagManager_DiagnosticsModeEnd();

	CSeqDevInitHandler_Reset();
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.04.08	| Yuichi Hoshino	| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.04.08	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVM ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
