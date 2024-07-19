// File Name:	CSysManager.c
// Description:	Implimentation File of System Manager
// Engineer:	Yuichi Hoshino
// Last Edit:	16.02.26
// Revision:	03
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSysManager.h"

#include "CDiagManager.h"
#include "CShtManager.h"
#include "CCdiAdapter.h"
#include "CCvrChecker.h"
#include "CJamChecker.h"
#include "CSeqDevInitHandler.h"
#include "CSeqProcEndHandler.h"
#include "COputInformer.h"
#include "CCapaInformer.h"
#include "CCfgInformer.h"
#include "CDvstInformer.h"
#include "CNmaHandler.h"
#include "CKeyInformer.h"
#include "CNvmInformer.h"
#include "CHfsiInformer.h"
// #include "CTempInformer.h"	【ROM削減】
#include "CDltManager.h"

#include "CSysFinisherState.h"
#include "CSysFinSttUnknown.h"
#include "CSysFinSttPowerUp.h"
#include "CSysFinSttPowerOn.h"
#include "CSysFinSttPowerDown.h"
#include "CSysFinSttPowerOff.h"
#include "CSysFinSttChangeDiag.h"
#include "CSysFinSttDiagnostic.h"
#include "CSysFinSttChangeNormal.h"
#include "CSysFinSttStandby.h"
#include "CSysFinSttCycleUp.h"
#include "CSysFinSttReady.h"
#include "CSysFinSttCycleDown.h"
#include "CSysFinSttHardDown.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of System Manager ---
typedef struct /*CSysManagerMember*/
{
	UC mCurrentState;
	UC mNextState;
	UC mRequestState;
	UC mPowerUpMode;
	UC mPowerDownReserve;
} CSysManagerMember;

static CSysManagerMember gSys_ManagerMember;

// --- Public method table of Finisher State ---
static CSysFinisherState* const cSys_FinSttPtrArray[FIN_STT_NUM] ={
	&gSys_FinSttUnknownMethod,			// FIN_STT_UNKNOWN
	&gSys_FinSttPowerUpMethod,			// FIN_STT_POWER_UP
	&gSys_FinSttPowerOnMethod,			// FIN_STT_POWER_ON
	&gSys_FinSttPowerDownMethod,		// FIN_STT_POWER_DOWN
	&gSys_FinSttPowerOffMethod,			// FIN_STT_POWER_OFF
	&gSys_FinSttChangeDiagMethod,		// FIN_STT_CHANGE_DIAG
	&gSys_FinSttDiagnosticMethod,		// FIN_STT_DIAGNOSTIC
	&gSys_FinSttChangeNormalMethod,		// FIN_STT_CHANGE_NORMAL
	&gSys_FinSttStandbyMethod,			// FIN_STT_STANDBY
	&gSys_FinSttCycleUpMethod,			// FIN_STT_CYCLE_UP
	&gSys_FinSttReadyMethod,			// FIN_STT_READY
	&gSys_FinSttCycleDownMethod,		// FIN_STT_CYCLE_DOWN
	&gSys_FinSttHardDownMethod,			// FIN_STT_HARD_DOWN
};


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

static void DoStateMachine();
static void ChangeDownState(ESysProcStopType stopType);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_Constructor()
{
	UC index;

	// --- Initialize member ---
	gSys_ManagerMember.mCurrentState		= FIN_STT_UNKNOWN;
	gSys_ManagerMember.mNextState			= FIN_STT_NUM;
	gSys_ManagerMember.mRequestState		= FIN_STT_NUM;
	gSys_ManagerMember.mPowerUpMode			= SYS_PWR_UP_MODE_NORMAL;
	gSys_ManagerMember.mPowerDownReserve	= false;

	// --- Execute state class constructor ---
	for ( index = 0; index < FIN_STT_NUM; index++ ){
		CSysFinisherState_Constructor(cSys_FinSttPtrArray[index]);
	}

	// --- Execute sub module's constructor ---
	CDiagManager_Constructor();
	CShtManager_Constructor();
	CCdiAdapter_Constructor();
	CCvrChecker_Constructor();
	CJamChecker_Constructor();
	COputInformer_Constructor();
	CCapaInformer_Constructor();
	CCfgInformer_Constructor();
	CDvstInformer_Constructor();
	CNmaHandler_Constructor();
	CKeyInformer_Constructor();
	CNvmInformer_Constructor();
	CHfsiInformer_Constructor();
	// CTempInformer_Constructor();	【ROM削減】
	CDltManager_Constructor();

	// --- Execute System Manager's SW componenet constructor ---
	CSeqDevInitHandler_Constructor();
	CSeqProcEndHandler_Constructor();
}

///////////////////////////////////////////////////////////////////
// Function Name : DoStateMachine
// Description   : 状態遷移処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
static void DoStateMachine()
{
	UC loop, postState, nextState;

	loop = true;

	while ( loop ){
		loop = false;

		if ( (gSys_ManagerMember.mNextState != FIN_STT_NUM) &&
			 (gSys_ManagerMember.mCurrentState != gSys_ManagerMember.mNextState) ){
			if ( (CSysFinisherState_IsTransitionState(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState]) == false) ||	// 過渡状態ではない
				 (CSysFinisherState_IsTransitionState(cSys_FinSttPtrArray[gSys_ManagerMember.mNextState]) == true) ||	// 過渡状態から過渡状態への遷移(Cycle Up⇒Process Down) V3.1.7 
				 ( (CSysFinisherState_CheckExitCondition(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState]) == true) &&	// 状態遷移条件は満たしている
				   (gSys_ManagerMember.mRequestState == gSys_ManagerMember.mNextState) ) ){									// System Controllerからの指示は受けている
				postState = gSys_ManagerMember.mCurrentState;
				nextState = gSys_ManagerMember.mNextState;

				// --- 状態遷移前の後処理 ---
				CSysFinisherState_Exit(cSys_FinSttPtrArray[postState], (EFinisherState)gSys_ManagerMember.mNextState);

				SRVC_UserDebugLog_Wide(0x10,0x10,postState,nextState); //log
				
				// --- 状態変更 ---
				gSys_ManagerMember.mCurrentState = nextState;					// 状態遷移
				CCdiAdapter_SendFinisherState((EFinisherState)nextState);		// CDI-IFに状態遷移を通知

				// --- 状態遷移後の前処理 ---
				CSysFinisherState_Entry(cSys_FinSttPtrArray[nextState], (EFinisherState)postState);

				// --- 過渡状態処理 ---
				if ( CSysFinisherState_IsTransitionState(cSys_FinSttPtrArray[nextState]) == true ){
																				// 過渡状態である
					gSys_ManagerMember.mNextState = CSysFinisherState_GetNextState(cSys_FinSttPtrArray[nextState]);

					loop = true;												// 既に遷移条件が整っている場合があるため再実行
				}
				else{															// 過渡状態でない
					gSys_ManagerMember.mNextState = FIN_STT_NUM;
					gSys_ManagerMember.mRequestState = FIN_STT_NUM;

					if( gSys_ManagerMember.mPowerDownReserve == true ){			// Power Down要求受信済み
						gSys_ManagerMember.mNextState = FIN_STT_POWER_DOWN;		// 遷移先はPowerDown
						gSys_ManagerMember.mRequestState = FIN_STT_POWER_OFF;
						gSys_ManagerMember.mPowerDownReserve = false;

						loop = true;
					}
					else{
						// No Action
					}
				}
			}
			else{
				// No Action
			}
		}
		else{
			// No Action
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : ChangeDownState
// Description   : フィニッシャー自発ダウンステート移行処理
// Parameter     : stopType (フィニッシングプロセス停止種別)
// Return        : ‐
///////////////////////////////////////////////////////////////////
static void ChangeDownState(ESysProcStopType stopType)
{
	switch ( stopType ){
	case SYS_PROC_STOP_HARD:
		if ( CSysFinisherState_CheckTransitionPossibility(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], FIN_STT_HARD_DOWN) == true ){
			gSys_ManagerMember.mNextState = FIN_STT_HARD_DOWN;
			DoStateMachine();
		}
		else{
			// No Action
		}

		break;
	case SYS_PROC_STOP_CYCLE:
		if ( CSysFinisherState_CheckTransitionPossibility(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], FIN_STT_CYCLE_DOWN) == true ){
			gSys_ManagerMember.mNextState = FIN_STT_CYCLE_DOWN;
			DoStateMachine();
		}
		else{
			// No Action
		}

		break;
	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_GetFinisherState
// Description   : フィニッシャー状態取得処理
// Parameter     : ‐
// Return        : 現在のFinisher State
///////////////////////////////////////////////////////////////////
EFinisherState CSysManager_GetFinisherState()
{
	return (EFinisherState)gSys_ManagerMember.mCurrentState;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_CheckInformationDetect
// Description   : フィニッシャー情報確定状況確認処理
// Parameter     : ‐
// Return        : true = 確定済み／false = 未確定
///////////////////////////////////////////////////////////////////
bool CSysManager_CheckInformationDetect()
{
	if ( (gSys_ManagerMember.mCurrentState != FIN_STT_UNKNOWN) &&
		 (gSys_ManagerMember.mCurrentState != FIN_STT_POWER_UP) ){
		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_PowerUp
// Description   : Power Up指示受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_PowerUp()
{
	UC state = gSys_ManagerMember.mCurrentState;

	if ( CSysFinisherState_CheckTransitionPossibility(cSys_FinSttPtrArray[state], FIN_STT_POWER_UP) == true ){
		gSys_ManagerMember.mNextState = FIN_STT_POWER_UP;
		gSys_ManagerMember.mRequestState = FIN_STT_POWER_ON;
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_PowerDown
// Description   : Power Down指示受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_PowerDown()
{
	UC state = gSys_ManagerMember.mCurrentState;
	
	if ( CSysFinisherState_CheckTransitionPossibility(cSys_FinSttPtrArray[state], FIN_STT_POWER_DOWN) == true ){
		gSys_ManagerMember.mNextState = FIN_STT_POWER_DOWN;
		gSys_ManagerMember.mRequestState = FIN_STT_POWER_OFF;
		DoStateMachine();
	}
	else{
		gSys_ManagerMember.mPowerDownReserve = true;

		// --- Finishing Process中処理 ---
		if ( CSysFinisherState_CheckTransitionPossibility(cSys_FinSttPtrArray[state], FIN_STT_HARD_DOWN) == true ){
			gSys_ManagerMember.mNextState = FIN_STT_HARD_DOWN;
			gSys_ManagerMember.mRequestState = FIN_STT_STANDBY;
			DoStateMachine();
		}
		else{
			if ( gSys_ManagerMember.mNextState == FIN_STT_STANDBY ){
				gSys_ManagerMember.mRequestState = FIN_STT_STANDBY;
				DoStateMachine();
			}
			else{
				// No Action
			}
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ChangeNormal
// Description   : Normal Mode遷移指示受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_ChangeNormal()
{
	UC state = gSys_ManagerMember.mCurrentState;
	
	if ( CSysFinisherState_CheckTransitionPossibility(cSys_FinSttPtrArray[state], FIN_STT_CHANGE_NORMAL) == true ){
		gSys_ManagerMember.mNextState = FIN_STT_CHANGE_NORMAL;
		gSys_ManagerMember.mRequestState = FIN_STT_STANDBY;
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ChangeDiag
// Description   : Diag Mode遷移指示受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_ChangeDiag()
{
	UC state = gSys_ManagerMember.mCurrentState;
	
	if ( CSysFinisherState_CheckTransitionPossibility(cSys_FinSttPtrArray[state], FIN_STT_CHANGE_DIAG) == true ){
		gSys_ManagerMember.mNextState = FIN_STT_CHANGE_DIAG;
		gSys_ManagerMember.mRequestState = FIN_STT_DIAGNOSTIC;
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_CycleUp
// Description   : Cycle Up指示受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_CycleUp()
{
	UC state = gSys_ManagerMember.mCurrentState;
	
	if ( CSysFinisherState_CheckTransitionPossibility(cSys_FinSttPtrArray[state], FIN_STT_CYCLE_UP) == true ){
		gSys_ManagerMember.mNextState = FIN_STT_CYCLE_UP;
		gSys_ManagerMember.mRequestState = FIN_STT_READY;
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_CycleDown
// Description   : Cycle Down指示受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_CycleDown()
{
	UC state = gSys_ManagerMember.mCurrentState;
	
	if ( CSysFinisherState_CheckTransitionPossibility(cSys_FinSttPtrArray[state], FIN_STT_CYCLE_DOWN) == true ){
		gSys_ManagerMember.mNextState = FIN_STT_CYCLE_DOWN;
		gSys_ManagerMember.mRequestState = FIN_STT_STANDBY;
		DoStateMachine();
	}
	else{
		if ( gSys_ManagerMember.mNextState == FIN_STT_STANDBY ){
			gSys_ManagerMember.mRequestState = FIN_STT_STANDBY;
			DoStateMachine();
		}
		else{
			// No Action
		}
	}	
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_HardDown
// Description   : Hard Down指示受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_HardDown()
{
	UC state = gSys_ManagerMember.mCurrentState;
	
	if ( CSysFinisherState_CheckTransitionPossibility(cSys_FinSttPtrArray[state], FIN_STT_HARD_DOWN) == true ){
		gSys_ManagerMember.mNextState = FIN_STT_HARD_DOWN;
		gSys_ManagerMember.mRequestState = FIN_STT_STANDBY;
		DoStateMachine();
	}
	else{
		if ( gSys_ManagerMember.mNextState == FIN_STT_STANDBY ){
			gSys_ManagerMember.mRequestState = FIN_STT_STANDBY;
			DoStateMachine();
		}
		else{
			// No Action
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_SendDeviceActionRequest
// Description   : Device Action Requestイベント送信処理
// Parameter     : eventID (MSP Event ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_SendDeviceActionRequest(US eventID)
{
	CSysFinisherState_SendDeviceActionRequest(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], eventID);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_SendDeviceEventNotify
// Description   : Device Event Notifyイベント送信処理
// Parameter     : eventID (MSP Event ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_SendDeviceEventNotify(US eventID)
{
	CSysFinisherState_SendDeviceEventNotify(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], eventID);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ReceivePowerUpResponse
// Description   : Power Up Responseイベント受信時処理
// Parameter     : moduleID (イベント発行Module ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_ReceivePowerUpResponse(UC moduleID)
{
	if ( CSysFinisherState_ReceivePowerUpResponse(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], moduleID) == true ){
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ReceivePowerDownResponse
// Description   : Power Down Responseイベント受信時処理
// Parameter     : moduleID (イベント発行Module ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_ReceivePowerDownResponse(UC moduleID)
{
	if ( CSysFinisherState_ReceivePowerDownResponse(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], moduleID) == true ){
		DoStateMachine();
	}
	else{
		// No Action
	}
}

//////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ReceiveChangeDiagResponse
// Description   : Change Diag Responseイベント受信時処理
// Parameter     : moduleID (イベント発行Module ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_ReceiveChangeDiagResponse(UC moduleID)
{
	if ( CSysFinisherState_ReceiveChangeDiagResponse(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], moduleID) == true ){
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ReceiveChangeNormalResponse
// Description   : Change Normal Responseイベント受信時処理
// Parameter     : moduleID (イベント発行Module ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_ReceiveChangeNormalResponse(UC moduleID)
{
	if ( CSysFinisherState_ReceiveChangeNormalResponse(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], moduleID) == true ){
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ReceiveCycleUpResponse
// Description   : Cycle Up Responseイベント受信時処理
// Parameter     : moduleID (イベント発行Module ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_ReceiveCycleUpResponse(UC moduleID)
{
	if ( CSysFinisherState_ReceiveCycleUpResponse(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], moduleID) == true ){
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ReceiveProcessDownResponse
// Description   : Process Down Responseイベント受信時処理
// Parameter     : moduleID (イベント発行Module ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_ReceiveProcessDownResponse(UC moduleID)
{
	if ( CSysFinisherState_ReceiveProcessDownResponse(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], moduleID) == true ){
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ReceiveDeviceActionResponse
// Description   : Device Action Responseイベント受信時処理
// Parameter     : moduleID (イベント発行Module ID)
//               : eventID (MSP Event ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_ReceiveDeviceActionResponse(UC moduleID, US eventID)
{
	if ( CSysFinisherState_ReceiveDeviceActionResponse(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], moduleID, eventID) == true ){
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ReceiveDeviceEventNotify
// Description   : Device Event Notifyイベント受信時処理
// Parameter     : moduleID (イベント発行Module ID)
//               : eventID (MSP Event ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_ReceiveDeviceEventNotify(UC moduleID, US eventID)
{
	CSysFinisherState_ReceiveDeviceEventNotify(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], moduleID, eventID);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ReceiveDeviceStatusNotify
// Description   : Device Status Notifyイベント受信時処理
// Parameter     : moduleID (イベント発行Module ID)
//               : statusNo (Status No.)
//               : value (Status Value)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_ReceiveDeviceStatusNotify(UC moduleID, US statusNo, US value)
{
	ChangeDownState(CSysFinisherState_ReceiveDeviceStatusNotify(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], moduleID, statusNo, value));
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_SystemConfigurationSet
// Description   : System Configuration通知受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_SystemConfigurationSet()
{
	CSysFinisherState_SystemConfigurationSet(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState]);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_PowerUpModeSet
// Description   : パワーアップ種別通知受信時処理
// Parameter     : pwrUpMode (パワーアップ種別)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_PowerUpModeSet(ESysPowerUpMode pwrUpMode)
{
	CSysFinisherState* pState;

	pState = cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState];

	if ( CSysFinisherState_PowerUpModeSet(pState, pwrUpMode) == true ){
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_EntranceAreaTampered
// Description   : エントランス部(ジャムクリア)操作通知受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_EntranceAreaTampered()
{
	CSysFinisherState_EntranceAreaTampered(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState]);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_LoadNvmComplete
// Description   : NVMデータロード処理完了通知受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_LoadNvmComplete(SS result)
{
	if ( CSysFinisherState_LoadNvmComplete(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], result) == true ){
		DoStateMachine();
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_SaveNvmComplete
// Description   : NVMデータセーブ処理完了通知受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_SaveNvmComplete(SS result)
{
	if ( CSysFinisherState_SaveNvmComplete(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], result) == true ){
		DoStateMachine();
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_SheetManagerReady
// Description   : シートマネージャーのレディー遷移通知受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_SheetManagerReady()
{
	CSysFinisherState* pState;

	pState = cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState];

	if ( CSysFinisherState_SheetManagerReady(pState) == true ){
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_CoverStatusChange
// Description   : カバー状態変化通知受信時処理
// Parameter     : coverID (Cover ID)
//               : status (Cover Status)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_CoverStatusChange(ECvrCoverID coverID, ECvrCoverStatus status)
{
	ChangeDownState(CSysFinisherState_CoverStatusChange(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], coverID, status));
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_FaultStatusChange
// Description   : フォルトステータス変化通知受信時処理
// Parameter     : statusNo (Device Status No.)
//               : value (Status Value)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_FaultStatusChange(US statusNo, US value)
{
	ChangeDownState(CSysFinisherState_FaultStatusChange(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], statusNo, value));
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_JamZoneStatusChange
// Description   : ジャムゾーンステータス変化通知受信時処理
// Parameter     : devContID (Device Controller ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSysManager_JamZoneStatusChange(UC devContID)
{
	CSysFinisherState_JamZoneStatusChange(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], devContID);
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
// 14.12.12	| Yuichi Hoshino	| Trouble Action [電源ON時イニシャライズ動作実施せず]
// 14.12.29	| Yuichi Hoshino	| Design Improvement [ Power Upサブシステム設計見直し ]
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVM ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
