// File Name:	CSysFinSttPowerDown.c
// Description:	Implimentation File of Finisher State "Power Down"
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSysFinSttPowerDown.h"

#include "CCdiAdapter.h"
#include "CCvrChecker.h"
#include "CJamChecker.h"
#include "CSeqDevInitHandler.h"
#include "CDvstInformer.h"
#include "CNmaHandler.h"
#include "CKeyInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Poweer Down State ---
CSysFinisherState const gSys_FinSttPowerDownMethod = {
	CSysFinSttPowerDown_Constructor,						// Constructor
	CSysFinSttPowerDown_IsTransitionState,					// IsTransitionState
	CSysFinSttPowerDown_CheckTransitionPossibility,			// CheckTransitionPossibility
	CSysFinSttPowerDown_CheckExitCondition,					// CheckExitCondition
	CSysFinSttPowerDown_GetNextState,						// GetNextState
	CSysFinSttPowerDown_Entry,								// Entry
	CSysFinisherState_StdExit,								// Exit
	CSysFinisherState_StdSendDeviceActionRequest,			// SendDeviceActionRequest
	CSysFinisherState_StdSendDeviceEventNotify,				// SendDeviceEventNotify
	CSysFinisherState_StdReceivePowerUpResponse,			// ReceivePowerUpResponse
	CSysFinSttPowerDown_ReceivePowerDownResponse,			// ReceivePowerDownResponse
	CSysFinisherState_StdReceiveChangeDiagResponse,			// ReceiveChangeDiagResponse
	CSysFinisherState_StdReceiveChangeNormalResponse,		// ReceiveChangeNormalResponse
	CSysFinisherState_StdReceiveCycleUpResponse,			// ReceiveCycleUpResponse
	CSysFinisherState_StdReceiveProcessDownResponse,		// ReceiveProcessDownResponse
//V3.4.0 　T/B対策　Merge TB.59 
//	CSysFinisherState_StdReceiveDeviceActionResponse,		// ReceiveDeviceActionResponse
	CSysFinSttPowerDown_ReceiveDeviceActionResponse,		// ReceiveDeviceActionResponse
	CSysFinisherState_StdReceiveDeviceEventNotify,			// ReceiveDeviceEventNotify
	CSysFinisherState_StdReceiveDeviceStatusNotify,			// ReceiveDeviceStatusNotify
	CSysFinisherState_StdSystemConfigurationSet,			// SystemConfigurationSet
	CSysFinisherState_StdPowerUpModeSet,					// PowerUpModeSet
	CSysFinisherState_StdEntranceAreaTampered,				// EntranceAreaTampered
	CSysFinisherState_StdLoadNvmComplete,					// LoadNvmComplete
	CSysFinSttPowerDown_SaveNvmComplete,					// SaveNvmComplete
	CSysFinisherState_StdSheetManagerReady,					// SheetManagerReady
	CSysFinisherState_StdCoverStatusChange,					// CoverStatusChange
	CSysFinisherState_StdFaultStatusChange,					// FaultStatusChange
	CSysFinisherState_StdJamZoneStatusChange				// JamZoneStatusChange
};


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of Power Up State ---
typedef struct /*CSysFinSttPowerDownMember*/ {
	UC mSaveNvmComplete;
} CSysFinSttPowerDownMember;

static CSysFinSttPowerDownMember gSys_FinSttPowerDownMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerDown_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttPowerDown_Constructor()
{
	gSys_FinSttPowerDownMember.mSaveNvmComplete = false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerDown_IsTransitionState
// Description   : 
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerDown_IsTransitionState()
{
	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerDown_CheckTransitionPossibility
// Description   : 状態遷移可否確認処理
// Parameter     : nextState (遷移先状態)
// Return        : 状態遷移可否
//               :  true = 遷移可能
//               :  false = 遷移付加
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerDown_CheckTransitionPossibility(EFinisherState nextState)
{
	static US const cTransPsbl = {
		FIN_STT_CNV_BIT(FIN_STT_POWER_OFF)
	};

	if ( cTransPsbl & FIN_STT_CNV_BIT(nextState) ) return true;
	else										   return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerDown_CheckExitCondition
// Description   : 現状態からの遷移条件成立確認処理
// Parameter     : ‐
// Return        : 状態遷移条件成立状況
//               :  true = 状態遷移条件成立済み
//               :  false = 状態遷移条件未成立
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerDown_CheckExitCondition()
{
	if ( (gSys_FinisherStateCommonMember.mWaitingModuleBit == 0x0000) &&
		 (gSys_FinSttPowerDownMember.mSaveNvmComplete == true) ){
		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerDown_GetNextState
// Description   : 過渡状態における遷移先状態確認処理
// Parameter     : ‐
// Return        : 遷移先フィニッシャー状態
///////////////////////////////////////////////////////////////////
EFinisherState CSysFinSttPowerDown_GetNextState()
{
	return FIN_STT_POWER_OFF;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerDown_Entry
// Description   : 現状態への遷移時処理
// Parameter     : prevState (遷移元フィニッシャー状態)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttPowerDown_Entry(EFinisherState prevState)
{
	// --- カバー状態チェック停止 ---
	CCvrChecker_EndCheck();

	// --- ジャム状態チェック停止 ---
	CJamChecker_EndCheck();

	// --- イニシャライズ動作停止 ---
	CSeqDevInitHandler_Stop();

	// --- Device Moduleへ状態遷移依頼 ---
	gSys_FinisherStateCommonMember.mWaitingModuleBit = MID_DEV_MDL_BIT_ALL;

	MSP_SendMessage(MID_SYS, EVT_STT_POWER_DOWN_REQ, NULL, 0, LOG_OFF);
}

/////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerDown_ReceivePowerDownResponse
// Description   : Power Down Responseイベント受信時処理
// Parameter     : moduleID (Device Module ID)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerDown_ReceivePowerDownResponse(UC moduleID)
{
	gSys_FinisherStateCommonMember.mWaitingModuleBit &= ~MID_CNV_BIT(moduleID);

	if ( gSys_FinisherStateCommonMember.mWaitingModuleBit == 0x0000 ){			// 全デバイスモジュールからの応答受信
		// --- Save NVM ---
		if ( CKeyInformer_CheckKeyCode() == true ){								// NVM制御異常未発生
			if ( CNmaHandler_Save() == false ){									// セーブ処理不要(書き換えなし)
				// --- NVMセーブ処理終了 ---
				gSys_FinSttPowerDownMember.mSaveNvmComplete = true;
			}
			else{ /* No Action */ }
		}
		else{																	// NVM制御異常発生
			// --- NVMセーブ処理終了 ---
			gSys_FinSttPowerDownMember.mSaveNvmComplete = true;
		}
	}
	else{ /* No Action */ }

	return true;
}

////V3.4.0 　T/B対策　Merge TB.59 ///////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerDown_ReceiveDeviceActionResponse
// Description   : Device Action Responseイベント受信時処理
// Parameter     : moduleID (Device Module ID)
//               : eventID (MSP Event ID)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerDown_ReceiveDeviceActionResponse(UC moduleID, US eventID)
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

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerDown_SaveNvmComplete
// Description   : NVMデータセーブ完了通知受信時処理
// Parameter     : informerID (NVM Control Informer ID)
//               : result (NVMデータセーブ処理結果)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerDown_SaveNvmComplete(SS result)
{
	UC logData[3];

	if ( result == DD_NVM_OK ){
		// --- NVMセーブ処理終了 ---
		gSys_FinSttPowerDownMember.mSaveNvmComplete = true;
	}
	else{
		// --- NVM Fail発生処理 ---
		CDvstDeviceStatus_SetStatusValue(CDvstInformer_GetDeviceStatus(DSNO_NVM_FAIL), FAULT_OCCUR);
		CSysFinisherState_DeviceStatusChangeProcess(DSNO_NVM_FAIL, FAULT_OCCUR);

		// --- Log Data送信 ---
		// logData[0] = eCdiFinLogData_NvmFail;
		// logData[1] = (UC)(result >> 8);
		// logData[2] = (UC)result;

		// CCdiAdapter_SendLogData(logData, sizeof(logData));

		// --- NVMセーブ処理終了 ---
		gSys_FinSttPowerDownMember.mSaveNvmComplete = true;
	}

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
// 15.01.27 | Yuichi Hoshino	| Trouble Action [ Power Off待ち合わせしていない ]
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVM ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
// 15.06.19	| Yuichi Hoshino	| Introduction Spec [ NVM Fail原因ログ送信 ]
// 15.07.15	| Yuichi Hoshino	| Trouble Action [ イニシャライズ動作実行中にパワーダウンに遷移すると遷移が完了しない ]
// 15.08.27	| Yuichi Hoshino	| Trouble Action [ NVMのKey Data書き込み完了を待たずにPower Off Announcementを送信 ]
// 15.12.14	| Yuichi Hoshino	| Design Improvement [ ROM削減 ]
// XX.XX.XX	| Yuichi Hoshino	| Release V2 (Revision 02)
//
