// File Name:	CSysFinSttStandby.c
// Description:	Implimentation File of Finisher State "Standby"
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSysFinSttStandby.h"

#include "CShtManager.h"
#include "CCdiAdapter.h"
#include "CCvrChecker.h"
#include "CJamChecker.h"
#include "CSeqDevInitHandler.h"
#include "CDvstInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Standby State ---
CSysFinisherState const gSys_FinSttStandbyMethod = {
	CSysFinSttStandby_Constructor,							// Constructor
	CSysFinisherState_StdIsTransitionState,					// IsTransitionState
	CSysFinSttStandby_CheckTransitionPossibility,			// CheckTransitionPossibility
	CSysFinisherState_StdCheckExitCondition,				// CheckExitCondition
	CSysFinisherState_StdGetNextState,						// GetNextState
	CSysFinSttStandby_Entry,								// Entry
	CSysFinSttStandby_Exit,									// Exit
	CSysFinisherState_StdSendDeviceActionRequest,			// SendDeviceActionRequest
	CSysFinisherState_StdSendDeviceEventNotify,				// SendDeviceEventNotify
	CSysFinisherState_StdReceivePowerUpResponse,			// ReceivePowerUpResponse
	CSysFinisherState_StdReceivePowerDownResponse,			// ReceivePowerDownResponse
	CSysFinisherState_StdReceiveChangeDiagResponse,			// ReceiveChangeDiagResponse
	CSysFinisherState_StdReceiveChangeNormalResponse,		// ReceiveChangeNormalResponse
	CSysFinisherState_StdReceiveCycleUpResponse,			// ReceiveCycleUpResponse
	CSysFinisherState_StdReceiveProcessDownResponse,		// ReceiveProcessDownResponse
	CSysFinSttStandby_ReceiveDeviceActionResponse,			// ReceiveDeviceActionResponse
	CSysFinisherState_StdReceiveDeviceEventNotify,			// ReceiveDeviceEventNotify
	CSysFinisherState_StdReceiveDeviceStatusNotify,			// ReceiveDeviceStatusNotify
	CSysFinisherState_StdSystemConfigurationSet,			// SystemConfigurationSet
	CSysFinisherState_StdPowerUpModeSet,					// PowerUpModeSet
	CSysFinisherState_StdEntranceAreaTampered,				// EntranceAreaTampered
	CSysFinisherState_StdLoadNvmComplete,					// LoadNvmComplete
	CSysFinisherState_StdSaveNvmComplete,					// SaveNvmComplete
	CSysFinisherState_StdSheetManagerReady,					// SheetManagerReady
	CSysFinSttStandby_CoverStatusChange,					// CoverStatusChange
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
// Function Name : CSysFinSttStandby_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttStandby_Constructor()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttStandby_CheckTransitionPossibility
// Description   : 状態遷移可否確認処理
// Parameter     : nextState (遷移先状態)
// Return        : 状態遷移可否
//               :  true = 遷移可能
//               :  false = 遷移付加
///////////////////////////////////////////////////////////////////
bool CSysFinSttStandby_CheckTransitionPossibility(EFinisherState nextState)
{
	static US const cTransPsbl = {
		FIN_STT_CNV_BIT(FIN_STT_POWER_DOWN)|
		FIN_STT_CNV_BIT(FIN_STT_CHANGE_DIAG)|
		FIN_STT_CNV_BIT(FIN_STT_CHANGE_NORMAL)|
		FIN_STT_CNV_BIT(FIN_STT_CYCLE_UP)
	};

	if ( cTransPsbl & FIN_STT_CNV_BIT(nextState) ) return true;
	else										   return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttStandby_Entry
// Description   : 現状態への遷移時処理
// Parameter     : prevState (遷移元フィニッシャー状態)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttStandby_Entry(EFinisherState prevState)
{
	MSP_CreateLocationSchedule();												// ロケーション･イベント･スケジュールの生成処理
	MSP_AbortLocationScheduling();												// ロケーション･イベント発生処理を中止

	MSP_SendMessage(MID_SYS, EVT_STT_STANDBY_NOTIFY, NULL, 0, LOG_OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttStandby_Exit
// Description   : 他状態への遷移時処理
// Parameter     : nextState (遷移先フィニッシャー状態)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttStandby_Exit(EFinisherState nextState)
{
	MSP_SendMessage(MID_SYS, EVT_STT_STANDBY_EXIT_NOTIFY, NULL, 0, LOG_OFF);
	CSeqDevInitHandler_Stop();
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttStandby_ReceiveDeviceActionResponse
// Description   : Device Action Responseイベント受信時処理
// Parameter     : moduleID (Device Module ID)
//               : eventID (MSP Event ID)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinSttStandby_ReceiveDeviceActionResponse(UC moduleID, US eventID)
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

	initState = CSeqDevInitHandler_DeviceActionResponse(eventID, moduleID);

	if ( initState == SEQ_ACT_STT_PAUSE ){
		CJamChecker_DeviceInitializeComplete();									// Jam Check

		initState = CSeqDevInitHandler_Restart();
	}
	else{
		// No Action
	}

	if ( initState == SEQ_ACT_STT_IDLE ){
		CJamChecker_DeviceInitializeComplete();									// Jam Check
		CShtManager_StandbyEject();
		CDvstDeviceStatus_SetStatusValue(CDvstInformer_GetDeviceStatus(DSNO_INIT_STATE), 0);
		CSysFinisherState_DeviceStatusChangeProcess(DSNO_INIT_STATE, 0);
		MSP_SendEvent(MID_SYS, EVT_DEV_EVT_DEV_INIT_COMP, NULL, 0, LOG_OFF);
	}
	else{
		// No Action
	}

	CCdiAdapter_SendDeviceActionResponse(eventID);

	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttStandby_CoverStatusChange
// Description   : カバー状態変化通知受信時処理
// Parameter     : coverID (Cover ID)
//               : status (Cover Status)
// Return        : フィニッシングプロセス停止種別
///////////////////////////////////////////////////////////////////
ESysProcStopType CSysFinSttStandby_CoverStatusChange(ECvrCoverID coverID, ECvrCoverStatus status)
{
	if ( status == CVR_STTS_OPEN ){
		if ( gSys_FinisherStateCommonMember.mAllCoverStatus == CVR_STTS_CLOSE ){
			CSeqDevInitHandler_Stop();

			MSP_SendEvent(MID_SYS, EVT_DEV_EVT_ANY_COVER_OPEN, NULL, 0, LOG_OFF);

			gSys_FinisherStateCommonMember.mAllCoverStatus = CVR_STTS_OPEN;
		}
		else{
			// No Action
		}
	}
	else{																		// Close
		CShtManager_AbortCompileSheetCoverClose(coverID);
		CJamChecker_CoverClose(coverID);

		if ( CCvrChecker_GetCoverStatus(CVR_ID_NUM) == CVR_STTS_CLOSE ){		// All Cover Close
			MSP_SendEvent(MID_SYS, EVT_DEV_EVT_ALL_COVER_CLOSE, NULL, 0, LOG_OFF);

			gSys_FinisherStateCommonMember.mAllCoverStatus = CVR_STTS_CLOSE;

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

	CDvstDeviceStatus_SetStatusValue(CDvstInformer_GetDeviceStatus(CCvrChecker_GetDeviceStatusNo(coverID)), status);
	return CSysFinisherState_DeviceStatusChangeProcess(CCvrChecker_GetDeviceStatusNo(coverID), status);
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
