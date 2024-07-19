// File Name:	CSysFinSttPowerUp.c
// Description:	Implimentation File of Finisher State "Power Up"
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSysFinSttPowerUp.h"

#include "CDiagManager.h"
#include "CCdiAdapter.h"
#include "CCvrChecker.h"
#include "CJamChecker.h"
#include "CSeqDevInitHandler.h"
#include "COputInformer.h"
#include "CCapaInformer.h"
#include "CCfgInformer.h"
#include "CDvstInformer.h"
#include "CNmaHandler.h"
#include "CKeyInformer.h"
#include "CNvmInformer.h"
#include "CHfsiInformer.h"
// #include "CTempInformer.h"	【ROM削減】


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Power Up State ---
CSysFinisherState const gSys_FinSttPowerUpMethod = {
	CSysFinSttPowerUp_Constructor,							// Constructor
	CSysFinSttPowerUp_IsTransitionState,					// IsTransitionState
	CSysFinSttPowerUp_CheckTransitionPossibility,			// CheckTransitionPossibility
	CSysFinSttPowerUp_CheckExitCondition,					// CheckExitCondition
	CSysFinSttPowerUp_GetNextState,							// GetNextState
	CSysFinSttPowerUp_Entry,								// Entry
	CSysFinSttPowerUp_Exit,									// Exit
	CSysFinisherState_StdSendDeviceActionRequest,			// SendDeviceActionRequest
	CSysFinisherState_StdSendDeviceEventNotify,				// SendDeviceEventNotify
	CSysFinSttPowerUp_ReceivePowerUpResponse,				// ReceivePowerUpResponse
	CSysFinisherState_StdReceivePowerDownResponse,			// ReceivePowerDownResponse
	CSysFinisherState_StdReceiveChangeDiagResponse,			// ReceiveChangeDiagResponse
	CSysFinisherState_StdReceiveChangeNormalResponse,		// ReceiveChangeNormalResponse
	CSysFinisherState_StdReceiveCycleUpResponse,			// ReceiveCycleUpResponse
	CSysFinisherState_StdReceiveProcessDownResponse,		// ReceiveProcessDownResponse
	CSysFinisherState_StdReceiveDeviceActionResponse,		// ReceiveDeviceActionResponse
	CSysFinisherState_StdReceiveDeviceEventNotify,			// ReceiveDeviceEventNotify
	CSysFinisherState_StdReceiveDeviceStatusNotify,			// ReceiveDeviceStatusNotify
	CSysFinSttPowerUp_SystemConfigurationSet,				// SystemConfigurationSet
	CSysFinSttPowerUp_PowerUpModeSet,						// PowerUpModeSet
	CSysFinisherState_StdEntranceAreaTampered,				// EntranceAreaTampered
	CSysFinSttPowerUp_LoadNvmComplete,						// LoadNvmComplete
	CSysFinisherState_StdSaveNvmComplete,					// SaveNvmComplete
	CSysFinisherState_StdSheetManagerReady,					// SheetManagerReady
	CSysFinisherState_StdCoverStatusChange,					// CoverStatusChange
	CSysFinisherState_StdFaultStatusChange,					// FaultStatusChange
	CSysFinisherState_StdJamZoneStatusChange				// JamZoneStatusChange
};


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of Power Up State ---
typedef struct /*CSysFinSttPowerUpMember*/ {
	UC mPowerUpMode;
} CSysFinSttPowerUpMember;

static CSysFinSttPowerUpMember gSys_FinSttPowerUpMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttPowerUp_Constructor()
{
	gSys_FinSttPowerUpMember.mPowerUpMode = SYS_PWR_UP_MODE_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_IsTransitionState
// Description   : 
// Parameter     : ‐
// Return        : 
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerUp_IsTransitionState()
{
	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_CheckTransitionPossibility
// Description   : 状態遷移可否確認処理
// Parameter     : nextState (遷移先状態)
// Return        : 状態遷移可否
//               :  true = 遷移可能
//               :  false = 遷移付加
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerUp_CheckTransitionPossibility(EFinisherState nextState)
{
	static US const cTransPsbl = {
		FIN_STT_CNV_BIT(FIN_STT_POWER_ON)
	};

	if ( cTransPsbl & FIN_STT_CNV_BIT(nextState) ) return true;
	else										   return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_CheckExitCondition
// Description   : 現状態からの遷移条件成立確認処理
// Parameter     : ‐
// Return        : 状態遷移条件成立状況
//               :  true = 状態遷移条件成立済み
//               :  false = 状態遷移条件未成立
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerUp_CheckExitCondition()
{
	if ( (gSys_FinisherStateCommonMember.mWaitingModuleBit == 0x0000) &&
		 (gSys_FinSttPowerUpMember.mPowerUpMode != SYS_PWR_UP_MODE_NUM) ){
		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_GetNextState
// Description   : 過渡状態における遷移先状態確認処理
// Parameter     : ‐
// Return        : 遷移先フィニッシャー状態
///////////////////////////////////////////////////////////////////
EFinisherState CSysFinSttPowerUp_GetNextState()
{
	return FIN_STT_POWER_ON;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_Entry
// Description   : 現状態への遷移時処理
// Parameter     : prevState (遷移元フィニッシャー状態)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttPowerUp_Entry(EFinisherState prevState)
{
	// --- Load NVM ---
	CNmaHandler_Load();
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_Exit
// Description   : 他状態への遷移時処理
// Parameter     : nextState (遷移先フィニッシャー状態)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttPowerUp_Exit(EFinisherState nextState)
{
	// --- イニシャライズ動作実施 ---
	gSys_FinisherStateCommonMember.mInitializeReserve = false;

	if ( gSys_FinSttPowerUpMember.mPowerUpMode != SYS_PWR_UP_MODE_ABORT ){
		if ( CSeqDevInitHandler_Start((ESysPowerUpMode)gSys_FinSttPowerUpMember.mPowerUpMode) != SEQ_ACT_STT_NUM ){
			CDvstDeviceStatus_SetStatusValue(CDvstInformer_GetDeviceStatus(DSNO_INIT_STATE), 1);
			CSysFinisherState_DeviceStatusChangeProcess(DSNO_INIT_STATE, 1);
		}
		else{
			// No Action
		}
	} else {
		// No Action 
	}
	
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_ReceivePowerUpResponse
// Description   : Power Up Responseイベント受信時処理
// Parameter     : moduleID (Device Module ID)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerUp_ReceivePowerUpResponse(UC moduleID)
{
	gSys_FinisherStateCommonMember.mWaitingModuleBit &= ~MID_CNV_BIT(moduleID);

	if ( gSys_FinisherStateCommonMember.mWaitingModuleBit == 0x0000 ){
		// --- フィニッシャー情報メッセージ送信指示 ---
		CCdiAdapter_SendInitialInformation();
	}
	else{
		// No Action
	}

	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_SystemConfigurationSet
// Description   : パワーアップ種別通知受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysFinSttPowerUp_SystemConfigurationSet()
{
	// --- NVM バージョンアップ初期化 ---
	CNvmInformer_VersionUpInitialize(CKeyInformer_GetMajorVersion(), CKeyInformer_GetMinorVersion());
	CHfsiInformer_VersionUpInitialize(CKeyInformer_GetMajorVersion(), CKeyInformer_GetMinorVersion());
	// CTempInformer_VersionInitialize(CKeyInformer_GetMajorVersion(), CKeyInformer_GetMinorVersion(), CKeyInformer_GetPatchVersion());

	CKeyInformer_SetMajorVersion(MAJOR_VERSION);
	CKeyInformer_SetMinorVersion(MINOR_VERSION);
	CKeyInformer_SetPatchVersion(PATCH_VERSION);

	// NVM AreaもVersion用のNVMがあるため。
	CNvmInformer_SetMajorVersion(MAJOR_VERSION);
	CNvmInformer_SetMinorVersion(MINOR_VERSION);
	CNvmInformer_SetPatchVersion(PATCH_VERSION);
	// CNvmInformer_SetIplVersion(CKeyInformer_GetIplVersion());

	// --- NVM 範囲外チェック ---
	CNvmInformer_OutRangeInitialize();

	// --- フィニッシャー情報構築 ---
	COputInformer_MakeInformation();
	CCapaInformer_MakeInformation();
	CCfgInformer_MakeInformation();

	// --- Device Moduleへ状態遷移依頼 ---
	gSys_FinisherStateCommonMember.mWaitingModuleBit = MID_DEV_MDL_BIT_ALL;

	MSP_SendMessage(MID_SYS, EVT_STT_POWER_UP_REQ, NULL, 0, LOG_OFF);

	// --- Diag Manager電源ON時処理依頼 ---
	CDiagManager_PowerUp();

	// --- Interlock Check 開始 ---
	CCvrChecker_DetectInitialStatus();

	// --- 電源ON時 Jamチェック ---
	CJamChecker_PowerOn();

	// --- Moview情報セット ---
	SRVC_Moview_SetVersion();
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_PowerUpModeSet
// Description   : パワーアップ種別通知受信時処理
// Parameter     : pwrUpMode (パワーアップ種別)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerUp_PowerUpModeSet(ESysPowerUpMode pwrUpMode)
{
	gSys_FinSttPowerUpMember.mPowerUpMode = pwrUpMode;

	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_LoadNvmComplete
// Description   : NVMデータロード完了通知受信時処理
// Parameter     : informerID (NVM Control Informer ID)
//               : result (NVMデータロード処理結果)
// Return        : フィニッシャー状態遷移有無
//               :  true = 状態が遷移する可能性あり
//               :  false = 状態は遷移しない
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerUp_LoadNvmComplete(SS result)
{
	UL logSelect;
	UC logData[3];

	if ( result == DD_NVM_OK ){
		// --- 1st Power ON NVM初期化 ---
		if ( CKeyInformer_CheckKeyCode() == false ){							// 1st Power ON
			CKeyInformer_SetKeyCode();
			CKeyInformer_SetMajorVersion(0);									// Version Up初期化を実施するためにVer.0.0.0をセット
			CKeyInformer_SetMinorVersion(0);
			CKeyInformer_SetPatchVersion(0);

			CNvmInformer_FirstPowerOnInitialize();
			CHfsiInformer_FirstPowerOnInitialize();
		}
		else{ /* No Action */ }

		// --- IPL Versionセット ---
		// CNvmNVM_SetValue(CNvmInformer_Get(NVMID_IPL_SW_VERSION), CKeyInformer_GetIplVersion());
	}
	else{
		// --- Key Data操作 ---
		CKeyInformer_ClearKeyCode();											// NVMが正常にLoad出来なかったことを意味させるためにKey Codeでない値をセット
		CKeyInformer_ClearDownloadMode();										// Download Modeに移行しないようにクリア
		CKeyInformer_SetMajorVersion(MAJOR_VERSION);							// Version Up初期化実施をしないために現Versionをセット
		CKeyInformer_SetMinorVersion(MINOR_VERSION);
		CKeyInformer_SetPatchVersion(PATCH_VERSION);

		// --- NVM RAM領域強制初期化(誤動作防止のために初期値を強制書き込み) ---
		CNvmInformer_FirstPowerOnInitialize();
		CHfsiInformer_FirstPowerOnInitialize();
		// CTempInformer_FirstPowerOnInitialize();	【ROM削減】

		// --- Device Status更新 ---
		CDvstDeviceStatus_SetStatusValue(CDvstInformer_GetDeviceStatus(DSNO_NVM_FAIL), FAULT_OCCUR);
		CSysFinisherState_DeviceStatusChangeProcess(DSNO_NVM_FAIL, FAULT_OCCUR);

		// --- Log Data送信 ---
		// logData[0] = eCdiFinLogData_NvmFail;
		// logData[1] = (UC)(result >> 8);
		// logData[2] = (UC)result;

		// CCdiAdapter_SendLogData(logData, sizeof(logData));
	}

	// --- ログ出力モジュール情報セット ---
	// logSelect = CNvmNVM_GetValue(CNvmInformer_Get(NVMID_DD_LOG_MODULE_SELECT_0_15));
	// logSelect |= (CNvmNVM_GetValue(CNvmInformer_Get(NVMID_DD_LOG_MODULE_SELECT_16_31)) << 16);
	// DD_LOG_SetModuleSelect(logSelect);

	// DD_LOG_SetCategory((UC)CNvmNVM_GetValue(CNvmInformer_Get(NVMID_DD_LOG_CATEGORY)));

	// --- System Controllerとの通信開始 ---
	CCdiAdapter_Sync();

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
// 14.12.12	| Yuichi Hoshino	| Trouble Action [ 電源ON時イニシャライズ動作実施せず ]
// 14.12.29	| Yuichi Hoshino	| Design Improvement [ Power Upサブシステム設計見直し ]
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVM ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
// 16.04.22	| Yuichi Hoshino	| Trouble Action [ T0151：Power Up Sequence中の電源OFF時にPower Off Acknowledgeを応答しない ]

