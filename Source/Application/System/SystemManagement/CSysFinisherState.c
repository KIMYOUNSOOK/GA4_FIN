// File Name:	CSysFinisherState.c
// Description:	Implimentation File of Finisher State
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSysFinisherState.h"

#include "CShtManager.h"
#include "CCdiAdapter.h"
#include "CCvrChecker.h"
#include "CJamChecker.h"
#include "CSeqDevInitHandler.h"
#include "CDvstInformer.h"
#include "CCfgInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Common Member of Finisher State ---
SysFinisherStateCommonMember gSys_FinisherStateCommonMember = {
	0x0000,
	false,
	CVR_STTS_CLOSE
};


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

static void ExecuteDeviceStatusChangeProcess(EDeviceStatusNo statusNo, US statusValue);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdConstructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinisherState_StdConstructor()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : ExecuteDeviceStatusChangeProcess
// Description   : 各デバイスステータスの変化時処理
// Parameter     : statusNo (Device Status No.)
//               : statusValue (Status Value)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExecuteDeviceStatusChangeProcess(EDeviceStatusNo statusNo, US statusValue)
{
	switch( statusNo ){
	// !!! Product Specific !!!
//	case DSNO_XXX:
//		
// 		break;
	default:
		break;
	}
	
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_DeviceStatusChangeProcess
// Description   : デバイスステータス変化時処理
// Parameter     : statusNo (Device Status No.)
//               : statusValue (Status Value)
// Return        : フィニッシングプロセス停止種別
///////////////////////////////////////////////////////////////////
ESysProcStopType CSysFinisherState_DeviceStatusChangeProcess(US statusNo, US value)
{
	CDvstDeviceStatus* pDevStts;
	EvtParamDeviceConditionNotify message;
	UC stopType, deviceID, devCond[DEVID_NUM], chgCond;

	stopType = SYS_PROC_STOP_NO_ACT;

	pDevStts = CDvstInformer_GetDeviceStatus((EDeviceStatusNo)statusNo);

	if ( pDevStts != NULL ){
		// --- 現コンディション取得 ---
		for ( deviceID = DEVID_FINISHER; deviceID < DEVID_NUM; deviceID++ ){
			devCond[deviceID] = CDvstInformer_GetDeviceCondtion(deviceID);
		}

		// --- Device Status通知 ---
		CCdiAdapter_SendDeviceStatus((EDeviceStatusNo)statusNo, value);			//変化のあったDeviceStatusをCdiAdapterへ通知

		// --- Condition更新 ---
		CDvstInformer_UpdateDeviceCondition((EDeviceStatusNo)statusNo);

		// --- Sheet Management 停止指示 ---
		if ( CDvstDeviceStatus_CheckDeviceCondition(pDevStts, value) == false ){// Condition NG
			stopType = CDvstDeviceStatus_GetFinProcessStopType(pDevStts);

			switch( stopType ){
			case SYS_PROC_STOP_CYCLE:
				CShtManager_CycleDownFaultStop(statusNo);
				break;
			case SYS_PROC_STOP_HARD:
				CShtManager_HardDownFaultStop(statusNo);
				break;
			default:
				// No Action
				break;
			}
		}
		else{ /* No Action */ }

		// --- Selectability(Configuration)更新 ---
		for ( deviceID = DEVID_FINISHER; deviceID < DEVID_NUM; deviceID++ ){
			chgCond = CDvstInformer_GetDeviceCondtion(deviceID);

			if ( devCond[deviceID] != chgCond ){
				CCfgInformer_UpdateSelectability((EDeviceID)deviceID, (EDeviceCondition)chgCond);

//				if ( (deviceID == DEVID_FINISHER) &&
//					 (chgCond != DEV_COND_OK)){
	// --- フィニッシャージョブ禁止のフェイル発生時のイニシャライズ動作停止 ---V3.4.0 Merge No.54
				if ( (deviceID == DEVID_FINISHER) && (chgCond == DEV_COND_PERM_NG)){
					CSeqDevInitHandler_Stop();
				}
				else{
					// No Action
				}
				// --- Device Condition Notifyイベント発行 ---
				message.condition = chgCond;
				MSP_SendEvent(MID_SYS, cSys_DeviceConditionEventID[deviceID], (const UC *)(&message), sizeof(message), LOG_OFF);
			}
			else{
				// No Action
			}
		}

		// --- Special process for specific device status's private use ---
		if ( CDvstDeviceStatus_GetSpecialProcessExecution(pDevStts) == true ){
			ExecuteDeviceStatusChangeProcess((EDeviceStatusNo)statusNo, value);
		}
		else{ /* No Action */ }
	}
	else{
		// No Action
	}

	return (ESysProcStopType)stopType;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdIsTransitionState
// Description   : 過渡状態確認処理
// Parameter     : ‐
// Return        : 過渡状態識別情報
//               :  true = 過渡状態である
//               :  false = 過渡状態でない
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdIsTransitionState()
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdCheckTransitionPossibility
// Description   : 状態遷移可否確認処理
// Parameter     : nextState (遷移先状態)
// Return        : 状態遷移可否
//               :  true = 遷移可能
//               :  false = 遷移付加
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdCheckTransitionPossibility(EFinisherState nextState)
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdCheckExitCondition
// Description   : 現状態からの遷移条件成立確認処理
// Parameter     : ‐
// Return        : 状態遷移条件成立状況
//               :  true = 状態遷移条件成立済み
//               :  false = 状態遷移条件未成立
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdCheckExitCondition()
{
	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdGetNextState
// Description   : 過渡状態における遷移先状態確認処理
// Parameter     : ‐
// Return        : 遷移先フィニッシャー状態
///////////////////////////////////////////////////////////////////
EFinisherState CSysFinisherState_StdGetNextState()
{
	return FIN_STT_NUM;															// 過渡状態でない
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdEntry
// Description   : 現状態への遷移時処理
// Parameter     : prevState (遷移元フィニッシャー状態)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinisherState_StdEntry(EFinisherState prevState)
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdExit
// Description   : 他状態への遷移時処理
// Parameter     : nextState (遷移先フィニッシャー状態)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinisherState_StdExit(EFinisherState nextState)
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdSendDeviceActionRequest
// Description   : Device Action Requestイベント送信処理
// Parameter     : eventID (MSP Event ID (Device Action Request))
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinisherState_StdSendDeviceActionRequest(US eventID)
{
	MSP_SendEvent(MID_SYS, eventID, NULL, 0, LOG_OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdSendDeviceEventNotify
// Description   : Device Event Notifyイベント送信処理
// Parameter     : event (MSP Event ID (Device Event Notify))
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinisherState_StdSendDeviceEventNotify(US eventID)
{
	MSP_SendEvent(MID_SYS, eventID, NULL, 0, LOG_OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdReceivePowerUpResponse
// Description   : Power Up Responseイベント受信時処理
// Parameter     : moduleID (Device Module ID)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdReceivePowerUpResponse(UC moduleID)
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdReceivePowerDownResponse
// Description   : Power Down Responseイベント受信時処理
// Parameter     : moduleID (Device Module ID)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdReceivePowerDownResponse(UC moduleID)
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdReceiveChangeDiagResponse
// Description   : Change Diag Responseイベント受信時処理
// Parameter     : moduleID (Device Module ID)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdReceiveChangeDiagResponse(UC moduleID)
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdReceiveChangeNormalResponse
// Description   : Change Normal Responseイベント受信時処理
// Parameter     : moduleID (Device Module ID)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdReceiveChangeNormalResponse(UC moduleID)
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdReceiveCycleUpResponse
// Description   : Cycle Up Responseイベント受信時処理
// Parameter     : moduleID (Device Module ID)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdReceiveCycleUpResponse(UC moduleID)
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdReceiveProcessDownResponse
// Description   : Process Down Responseイベント受信時処理
// Parameter     : moduleID (Device Module ID)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdReceiveProcessDownResponse(UC moduleID)
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdReceiveDeviceActionResponse
// Description   : Device Action Responseイベント受信時処理
// Parameter     : moduleID (Device Module ID)
//               : eventID (MSP Event ID)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdReceiveDeviceActionResponse(UC moduleID, US eventID)
{
	switch( eventID ){
	// !!! Product Specific !!!
//	case EVT_DEV_RES_XXXX:
//
//		break;
	default:
		break;
	}

	CCdiAdapter_SendDeviceActionResponse(eventID);

	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdReceiveDeviceEventNotify
// Description   : Device Event Notifyイベント受信時処理
// Parameter     : moduleID (Device Module ID)
//               : eventID (MSP Event ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinisherState_StdReceiveDeviceEventNotify(UC moduleID, US eventID)
{
	switch( eventID ){
	// !!! Product Specific !!!
//	case EVT_DEV_EVT_XXXX:
//
//		break;
	default:
		break;
	}

	CCdiAdapter_SendDeviceEventNotify(eventID);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdReceiveDeviceStatusNotify
// Description   : Device Status Notifyイベント受信時処理
// Parameter     : moduleID (Device Module ID)
//               : statusNo (Status No.)
//               : value (Status Value)
// Return        : フィニッシングプロセス停止種別
///////////////////////////////////////////////////////////////////
ESysProcStopType CSysFinisherState_StdReceiveDeviceStatusNotify(UC moduleID, US statusNo, US value)
{
	return CSysFinisherState_DeviceStatusChangeProcess(statusNo, value);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdSystemConfigurationSet
// Description   : System Configuration通知受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinisherState_StdSystemConfigurationSet()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdPowerUpModeSet
// Description   : パワーアップ種別通知受信時処理
// Parameter     : pwrUpMode (パワーアップ種別)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdPowerUpModeSet(ESysPowerUpMode pwrUpMode)
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdEntranceAreaTampered
// Description   : エントランス部(ジャムクリア)操作通知受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinisherState_StdEntranceAreaTampered()
{
	CJamChecker_EntranceEnvironmentChange();
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdLoadNvmComplete
// Description   : NVMデータロード完了通知受信時処理
// Parameter     : informerID (NVM Control Informer ID)
//               : result (NVMデータロード処理結果)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdLoadNvmComplete(SS result)
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdSaveNvmComplete
// Description   : NVMデータセーブ完了通知受信時処理
// Parameter     : informerID (NVM Control Informer ID)
//               : result (NVMデータセーブ処理結果)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdSaveNvmComplete(SS result)
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdSheetManagerReady
// Description   : シートマネージャーのレディー遷移通知受信時処理
// Parameter     : ‐
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdSheetManagerReady()
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdCoverStatusChange
// Description   : カバー状態変化通知受信時処理
// Parameter     : coverID (Cover ID)
//               : status (Cover Status)
// Return        : フィニッシングプロセス停止種別
///////////////////////////////////////////////////////////////////
ESysProcStopType CSysFinisherState_StdCoverStatusChange(ECvrCoverID coverID, ECvrCoverStatus status)
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
		CJamChecker_CoverClose(coverID);

		if ( CCvrChecker_GetCoverStatus(CVR_ID_NUM) == CVR_STTS_CLOSE ){		// All Cover Close
			MSP_SendEvent(MID_SYS, EVT_DEV_EVT_ALL_COVER_CLOSE, NULL, 0, LOG_OFF);

			gSys_FinisherStateCommonMember.mAllCoverStatus = CVR_STTS_CLOSE;

			gSys_FinisherStateCommonMember.mInitializeReserve = true;
		}
		else{
			// No Action
		}
	}

	CDvstDeviceStatus_SetStatusValue(CDvstInformer_GetDeviceStatus(CCvrChecker_GetDeviceStatusNo(coverID)), status);

	return CSysFinisherState_DeviceStatusChangeProcess(CCvrChecker_GetDeviceStatusNo(coverID), status);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdFaultStatusChange
// Description   : フォルトステータス変化通知受信時処理
// Parameter     : statusNo (Device Status No.)
//               : value (Status Value)
// Return        : フィニッシングプロセス停止種別
///////////////////////////////////////////////////////////////////
ESysProcStopType CSysFinisherState_StdFaultStatusChange(US statusNo, US value)
{
	CDvstDeviceStatus *pDeviceStatus;

	pDeviceStatus = CDvstInformer_GetDeviceStatus(statusNo);

	switch( CDvstDeviceStatus_GetStatusCategory(pDeviceStatus) ){
	case DVST_CTGRY_JAM:
		if ( value == FAULT_NOT_OCCUR ){
			if ( CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_CLEAR ){
				if ( CSeqDevInitHandler_Start(SYS_PWR_UP_MODE_NORMAL) != SEQ_ACT_STT_NUM ){
																				// 複数ジャム同時クリア時は複数回実行(Initialize Handlerではじく設計)
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
		else{
			// No Action
		}

		break;
	default:
		// No Action
		break;
	}

	CDvstDeviceStatus_SetStatusValue(CDvstInformer_GetDeviceStatus(statusNo), value);

	return CSysFinisherState_DeviceStatusChangeProcess(statusNo, value);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdJamZoneStatusChange
// Description   : ジャムゾーンステータス変化通知受信時処理
// Parameter     : devContID (Device Controller ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinisherState_StdJamZoneStatusChange(UC devContID)
{
	CCdiAdapter_SendJamZoneStatus(devContID);
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.02.08	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 14.12.12	| Yuichi Hoshino	| Trouble Action [ 電源ON時イニシャライズ動作実施せず ]
// 14.12.29	| Yuichi Hoshino	| Design Improvement [ Power Upサブシステム設計見直し ]
// 15.03.30	| Yuichi Hoshino	| Trouble Action [ カバークローズイニシャライズ動作実施せず ]
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVM ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
