// File Name:	TRNS_MGR.c
// Description:	Source File of Transport Module Manager
// Engineer:	Yuichi Hoshino
// Last Edit:	14.09.03
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "TRNS_Manager.h"

#include "DD_DIO.h"

#include "TRNS_Device.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define TRNS_MGR_STT_CNV_BIT(state)	(0x0001 << state)

// --- Module Controller collaborate action ---
typedef enum /*ETraContCollaboAction*/
{
	TRNS_COLLABO_ACT_JAM_CHECK,
	TRNS_COLLABO_ACT_JAM_CLEAR_ASSIST,
	TRNS_COLLABO_ACT_NUM
} ETraControllerCollaboAction;

// --- Module Manager Member ---
typedef struct /*TRNS_MGRMember*/
{
	UC mState;							// Module State
	UC mCondition;						// Module Condition
	UC mCollaboAct;
	US mStartDiagLinkNo;				// Link No. of requested start component control
	US mStopDiagLinkNo;					// Link No. of requested stop component control
} TRNS_MGRMember;

static TRNS_MGRMember gTrnsManagerMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void TRNS_MGR_PowerUpEvent();
void TRNS_MGR_PowerOnEvent();
void TRNS_MGR_PowerDownEvent();
void TRNS_MGR_PowerOffEvent();
void TRNS_MGR_ChangeDiagEvent();
void TRNS_MGR_DiagEvent();
void TRNS_MGR_ChangeNormalEvent();
void TRNS_MGR_StandbyEvent();
void TRNS_MGR_CycleUpEvent();
void TRNS_MGR_ReadyEvent();
void TRNS_MGR_CycleDownEvent();
void TRNS_MGR_HardDownEvent();


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_Constructor
// Description   : コンストラク?
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_Constructor()
{
	gTrnsManagerMember.mState = TRNS_MGR_STT_UNKNOWN;
	gTrnsManagerMember.mCondition = DEV_COND_OK;
	gTrnsManagerMember.mCollaboAct = TRNS_COLLABO_ACT_NUM;

	gTrnsManagerMember.mStartDiagLinkNo = 0;
	gTrnsManagerMember.mStopDiagLinkNo = 0;

	// --- Device Controller's construct  ---
	TRNS_Device_Constructor();
	// TrnsExitController_Constructor();
}

///////////////////////////////////////////////////////////////////
// Function Name : PowerUpEvent
// Description   : Power Up遷移イベント発生時処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_PowerUpEvent()
{
	static const US ChangeState = TRNS_MGR_STT_CNV_BIT(TRNS_MGR_STT_UNKNOWN);

	if( ChangeState & TRNS_MGR_STT_CNV_BIT(gTrnsManagerMember.mState) ) {
		// --- Exit Process ---
		// No Action

		// --- Change state ---
		gTrnsManagerMember.mState = TRNS_MGR_STT_POWER_UP;

		// --- Entry Process ---
		// No Action
	}
	else {
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : PowerOnEvent
// Description   : Power On遷移イベント発生時処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_PowerOnEvent()
{
	static const US ChangeState = TRNS_MGR_STT_CNV_BIT(TRNS_MGR_STT_POWER_UP);

	if( ChangeState & TRNS_MGR_STT_CNV_BIT(gTrnsManagerMember.mState) ) {
		// --- Exit Process ---
		TRNS_Device_SetUp();

		// --- Change state ---
		gTrnsManagerMember.mState = TRNS_MGR_STT_POWER_ON;
		MSP_SendMessage(MID_TRNS, EVT_STT_POWER_UP_RES, NULL, 0, LOG_OFF);

		// --- Entry Process ---
		// No Action
	}
	else {
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : PowerDownEvent
// Description   : Power Down遷移イベント発生時処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_PowerDownEvent()
{
	static const US ChangeState = TRNS_MGR_STT_CNV_BIT(TRNS_MGR_STT_POWER_ON) |
								   TRNS_MGR_STT_CNV_BIT(TRNS_MGR_STT_DIAG) |
								   TRNS_MGR_STT_CNV_BIT(TRNS_MGR_STT_STANDBY);

	if( ChangeState & TRNS_MGR_STT_CNV_BIT(gTrnsManagerMember.mState) ) {
		// --- Exit Process ---
		// No Action

		// --- Change state ---
		gTrnsManagerMember.mState = TRNS_MGR_STT_POWER_DOWN;

		// --- Entry Process ---
		// No Action
	}
	else {
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : PowerOffEvent
// Description   : Power Off遷移イベント発生時処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_PowerOffEvent()
{
	static const US ChangeState = TRNS_MGR_STT_CNV_BIT(TRNS_MGR_STT_POWER_DOWN);

	if( ChangeState & TRNS_MGR_STT_CNV_BIT(gTrnsManagerMember.mState) ) {
		// --- Exit Process ---
		// No Action

		// --- Change state ---
		gTrnsManagerMember.mState = TRNS_MGR_STT_POWER_OFF;
		MSP_SendMessage(MID_TRNS, EVT_STT_POWER_DOWN_RES, NULL, 0, LOG_OFF);

		// --- Entry Process ---
		// No Action
	}
	else {
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : ChangeDiagEvent
// Description   : Change Diag遷移イベント発生時処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_ChangeDiagEvent()
{
	static const US ChangeState = TRNS_MGR_STT_CNV_BIT(TRNS_MGR_STT_POWER_ON)|
								   TRNS_MGR_STT_CNV_BIT(TRNS_MGR_STT_STANDBY);

	if( ChangeState & TRNS_MGR_STT_CNV_BIT(gTrnsManagerMember.mState) ) {
		// --- Exit Process ---
		// No Action

		// --- Change state ---
		gTrnsManagerMember.mState = TRNS_MGR_STT_CHANGE_DIAG;

		// --- Entry Process ---
		// No Action
	}
	else {
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : DiagEvent
// Description   : Diag遷移イベント発生時処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_DiagEvent()
{
	static const US ChangeState = TRNS_MGR_STT_CNV_BIT(TRNS_MGR_STT_CHANGE_DIAG);

	if( ChangeState & TRNS_MGR_STT_CNV_BIT(gTrnsManagerMember.mState) ) {
		// --- Exit Process ---
		// No Action

		// --- Change state ---
		gTrnsManagerMember.mState = TRNS_MGR_STT_DIAG;
		MSP_SendMessage(MID_TRNS, EVT_STT_CHANGE_DIAG_RES, NULL, 0, LOG_OFF);

		// --- Entry Process ---
		// No Action
	}
	else {
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : ChangeNormalEvent
// Description   : Change Normal遷移イベント発生時処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_ChangeNormalEvent()
{
	static const US ChangeState = TRNS_MGR_STT_CNV_BIT(TRNS_MGR_STT_POWER_ON)|
								   TRNS_MGR_STT_CNV_BIT(TRNS_MGR_STT_DIAG);

	if( ChangeState & TRNS_MGR_STT_CNV_BIT(gTrnsManagerMember.mState) ) {
		// --- Exit Process ---
		if( gTrnsManagerMember.mState == TRNS_MGR_STT_DIAG ) {
			TRNS_Device_SetUp();
//			TrnsExitController_SetUp();
		}
		else { /* No Action */ }

		// --- Change state ---
		gTrnsManagerMember.mState = TRNS_MGR_STT_CHANGE_NORMAL;

		// --- Entry Process ---
		// No Action
	}
	else {
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : NormalEvent
// Description   : Normal遷移イベント発生時処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_NormalEvent()
{
	static const US ChangeState = TRNS_MGR_STT_CNV_BIT(TRNS_MGR_STT_CHANGE_NORMAL);

	if( ChangeState & TRNS_MGR_STT_CNV_BIT(gTrnsManagerMember.mState) ) {
		// --- Exit Process ---
		// No Action

		// --- Change state ---
		gTrnsManagerMember.mState = TRNS_MGR_STT_STANDBY;
		MSP_SendMessage(MID_TRNS, EVT_STT_CHANGE_NORMAL_RES, NULL, 0, LOG_OFF);

		// --- Entry Process ---
		// No Action
	}
	else {
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : StandbyEvent
// Description   : Standby遷移イベント発生時処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_StandbyEvent()
{
	static const US ChangeState = TRNS_MGR_STT_CNV_BIT(TRNS_MGR_STT_CYCLE_DOWN)|
								   TRNS_MGR_STT_CNV_BIT(TRNS_MGR_STT_HARD_DOWN);
	UC prevState;

	if( (ChangeState & TRNS_MGR_STT_CNV_BIT(gTrnsManagerMember.mState))  &&
		 (SRVC_CheckUndeliveredSheetExist(eCdiFinOutDest_All) == false)  &&
		 (TRNS_Device_GetState() == TRNS_CONT_STT_IDLE )  ){

		prevState = gTrnsManagerMember.mState;

		// --- Exit Process ---NoAction
		SRVC_UserDebugLog(0x91,0x91);

		// --- Change state ---
		gTrnsManagerMember.mState = TRNS_MGR_STT_STANDBY;
		MSP_SendMessage(MID_TRNS, EVT_STT_PROCESS_DOWN_RES, NULL, 0, LOG_OFF);

		// --- Entry Process ---
		TRNS_Device_StopSheet();
	}
	else {
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CycleUpEvent
// Description   : Cycle Up遷移イベント発生時処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_CycleUpEvent()
{
	static const US ChangeState = TRNS_MGR_STT_CNV_BIT(TRNS_MGR_STT_STANDBY);

	if( ChangeState & TRNS_MGR_STT_CNV_BIT(gTrnsManagerMember.mState) ) {
		// --- Exit Process ---
		// No Action

		// --- Change state ---
		gTrnsManagerMember.mState = TRNS_MGR_STT_CYCLEUP;

		// --- Entry Process ---
		TRNS_Device_StartSheet();
	}
	else {
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : ReadyEvent
// Description   : Ready遷移イベント発生時処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_ReadyEvent()
{
	static const US ChangeState = TRNS_MGR_STT_CNV_BIT(TRNS_MGR_STT_CYCLEUP);

	if( ChangeState & TRNS_MGR_STT_CNV_BIT(gTrnsManagerMember.mState) ) {
		// --- Exit Process ---
		// No Action

		// --- Change state ---
		gTrnsManagerMember.mState = TRNS_MGR_STT_READY;
		MSP_SendMessage(MID_TRNS, EVT_STT_CYCLE_UP_RES, NULL, 0, LOG_OFF);

		// --- Entry Process ---
		// No Action
	}
	else {
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CycleDownEvent
// Description   : Cycle Down遷移イベント発生時処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_CycleDownEvent()
{
	static const US ChangeState = TRNS_MGR_STT_CNV_BIT(TRNS_MGR_STT_CYCLEUP)|
								   TRNS_MGR_STT_CNV_BIT(TRNS_MGR_STT_READY);

	if( ChangeState & TRNS_MGR_STT_CNV_BIT(gTrnsManagerMember.mState) ) {
		// --- Exit Process ---
		// No Action

		// --- Change state ---
		gTrnsManagerMember.mState = TRNS_MGR_STT_CYCLE_DOWN;

		// --- Entry Process ---
		// No Action
	}
	else {
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : HardDownEvent
// Description   : Hard Down遷移イベント発生時処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_HardDownEvent()
{
	static const US ChangeState = TRNS_MGR_STT_CNV_BIT(TRNS_MGR_STT_CYCLEUP)|
								   TRNS_MGR_STT_CNV_BIT(TRNS_MGR_STT_READY)|
								   TRNS_MGR_STT_CNV_BIT(TRNS_MGR_STT_CYCLE_DOWN);

	if( ChangeState & TRNS_MGR_STT_CNV_BIT(gTrnsManagerMember.mState) ) {
		// --- Exit Process ---
		// No Action

		// --- Change state ---
		gTrnsManagerMember.mState = TRNS_MGR_STT_HARD_DOWN;

		// --- Entry Process ---
		// TRNS_Device_EmgStop(); //IOTとの結合デバッグ TB.No3 対策		
		TRNS_Device_StopSheet();
		// TrnsExitController_StopSheet();
	}
	else {
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_StartUp
// Description   : Start Up処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_StartUp()
{
	TRNS_Device_Reset();
//	TrnsExitController_Reset();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_PowerUp
// Description   : Power Upメッセ?ジ受信処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_PowerUp()
{
	TRNS_MGR_PowerUpEvent();

	TRNS_MGR_PowerOnEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_PowerDown
// Description   : Power Downメッセ?ジ受信処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_PowerDown()
{
	TRNS_MGR_PowerDownEvent();

	TRNS_MGR_PowerOffEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_ChangeDiag
// Description   : Change Diagメッセ?ジ受信処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_ChangeDiag()
{
	TRNS_MGR_ChangeDiagEvent();

	TRNS_MGR_DiagEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_ChangeNormal
// Description   : Change Normalメッセ?ジ受信処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_ChangeNormal()
{
	TRNS_MGR_ChangeNormalEvent();

	TRNS_MGR_NormalEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_CycleUp
// Description   : Cycle Upメッセ?ジ受信処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_CycleUp()
{
	TRNS_MGR_CycleUpEvent();

	TRNS_MGR_ReadyEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_HardDown
// Description   : Hard Down Requestメッセ?ジ受信処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_HardDown()
{
	PrintTransLog(0x0D, gTrnsManagerMember.mState, gTrnsManagerMember.mCondition, 0);	
	TRNS_MGR_HardDownEvent();

	TRNS_MGR_StandbyEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_CycleDown
// Description   : Cycle Down Requestメッセ?ジ受信処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_CycleDown()
{
	PrintTransLog(0x0C, gTrnsManagerMember.mState, (US)gTrnsManagerMember.mCondition, 0);		
	TRNS_MGR_CycleDownEvent();

	TRNS_MGR_StandbyEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_JamCheckRequest
// Description   : Device Action Request(Jam Check)メッセ?ジ受信処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_JamCheckRequest()
{
	// if ( (DD_DIO_GetInput(DD_DI_IOT_EXIT_SNR) == LOW) &&
	//	 (DD_DIO_GetInput(DD_DI_VERTICAL_SNR) == OFF) ) {
	if ( SRVC_GetDeviceCondition(DEVID_FINISHER) == DEV_COND_OK && CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_CLEAR ) {

		gTrnsManagerMember.mCollaboAct = TRNS_COLLABO_ACT_JAM_CHECK;

		TRNS_Device_StartJamCheck();
		MSP_SendMessage(MID_TRNS, EVT_DEV_RES_JAM_CHECK_TRA, NULL, 0, LOG_OFF);
		// TrnsExitController_StartJamCheck();
	}
	else {
		MSP_SendMessage(MID_TRNS, EVT_DEV_RES_JAM_CHECK_TRA, NULL, 0, LOG_OFF);
	}
	PrintTransLog(0x0E, 1, (US) gTrnsManagerMember.mState, (US) gTrnsManagerMember.mCondition);		
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_ProcessEndRequest
// Description   : Device Action Request(Process End Action)メッセ?ジ受信処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_ProcessEndRequest()
{
	MSP_SendMessage(MID_TRNS, EVT_DEV_RES_PROCESS_END_TRA, NULL, 0, LOG_OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_JamClearAssistRequest
// Description   : Device Action Request(Jam Clear Assist)メッセ?ジ受信処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_JamClearAssistRequest() 
{
	if ( SRVC_GetDeviceCondition(DEVID_FINISHER) == DEV_COND_OK && CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_CLEAR ) {

		gTrnsManagerMember.mCollaboAct = TRNS_COLLABO_ACT_JAM_CLEAR_ASSIST;

		TRNS_Device_StartJamClearAssist();
		MSP_SendMessage(MID_TRNS, EVT_DEV_RES_JAM_CLEAR_ASSIST_TRA, NULL, 0, LOG_OFF);
		PrintTransLog(0x0E, 2, (US) gTrnsManagerMember.mState, (US) gTrnsManagerMember.mCondition);	
	}
	else {
		MSP_SendMessage(MID_TRNS, EVT_DEV_RES_JAM_CLEAR_ASSIST_TRA, NULL, 0, LOG_OFF);		
		PrintTransLog(0x0E, 3, (US) gTrnsManagerMember.mState, (US) gTrnsManagerMember.mCondition);	
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_AnyCoverOpenNotify
// Description   : Device Event(Any Cover Open)メッセ?ジ受信処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_AnyCoverOpenNotify()
{
	switch ( gTrnsManagerMember.mCollaboAct ) {
	case TRNS_COLLABO_ACT_JAM_CHECK:
		TRNS_Device_StopJamCheck();
//		TrnsExitController_StopJamCheck();

		break;
	case TRNS_COLLABO_ACT_JAM_CLEAR_ASSIST:
		TRNS_Device_StopJamClearAssist();
//		TrnsExitController_StopJamClearAssist();

		break;
	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_AllCoverCloseNotify
// Description   : Device Event(All Cover Close)メッセ?ジ受信処理
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_AllCoverCloseNotify()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_FinisherConditionNotify
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_FinisherConditionNotify()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_SheetAbortedNotify
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_SheetAbortedNotify()
{
	TRNS_Device_AbortSheet();
//	TrnsExitController_AbortSheet();

	TRNS_MGR_StandbyEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_ComponentControl
// Description   : Component Control Stop Request指示受信処理
// Parameter     : 
//               : 
// Return        : 
///////////////////////////////////////////////////////////////////
void TRNS_MGR_ComponentControl(const UC* evt, EAction aAction)
{
	EvtParamComponentControlStartRequest *pRcvEvtParameter;
	EvtParamComponentControlStartNotify evtParameter;

	pRcvEvtParameter = (EvtParamComponentControlStartRequest *)evt;
	evtParameter.chainNo = pRcvEvtParameter->chainNo;
	evtParameter.linkNo = pRcvEvtParameter->linkNo;

	if ( evtParameter.chainNo == TRNS_COMPO_CHAIN ) {
		switch ( (ETrnsControlStep)evtParameter.linkNo ) {
		case TRNS_COMPO_FORWARD_LOW_SPEED :
		case TRNS_COMPO_FORWARD_MID_SPEED :
		case TRNS_COMPO_FORWARD_HI_SPEED :
		case TRNS_COMPO_GATE_SOLENOID_PULL :
		case TRNS_COMPO_GATE_SOLENOID_PUSH :
			TRNS_Device_ComponentControl(evtParameter.chainNo, evtParameter.linkNo, aAction);
			break;

		default :	// 未定義のLink No. No Action
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_ComponentControlStartNotify
// Description   : Component Control Stop Notify
// Parameter     : chainNo (Chain No.)
//               : linkNo (Link No.)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_ComponentControlStartNotify()
{
	EvtParamComponentControlStartNotify evtPara;

	evtPara.chainNo = 12;
	evtPara.linkNo  = gTrnsManagerMember.mStartDiagLinkNo;

	gTrnsManagerMember.mStartDiagLinkNo = 0;

	MSP_SendMessage(MID_TRNS, EVT_DIAG_COMPO_CONT_START, (UC *)(&evtPara), sizeof(evtPara), LOG_OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_InformedCompoControlComplete
// Description   : Module Action Complete(Transport Internal I/F)通知受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRNS_InformedCompoControlComplete(US aChainNo, US aLinkNo, EAction aAction)
{
	EvtParamComponentControlStopNotify aEvtParameter;

	aEvtParameter.chainNo = aChainNo;
	aEvtParameter.linkNo = aLinkNo;
	
	if ( aAction == ACT_START ) {
		MSP_SendMessage(MID_TRNS, EVT_DIAG_COMPO_CONT_START, (const UC*)&aEvtParameter, sizeof(aEvtParameter), LOG_OFF);
	} else {																		// ACT_STOP
		MSP_SendMessage(MID_TRNS, EVT_DIAG_COMPO_CONT_STOP, (const UC*)&aEvtParameter, sizeof(aEvtParameter), LOG_OFF);
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_ComponentControlStopNotify
// Description   : Component Control Stop Notify
// Parameter     : chainNo (Chain No.)
//               : linkNo (Link No.)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_ComponentControlStopNotify()
{
	EvtParamComponentControlStopNotify evtPara;

	evtPara.chainNo = 12;
	evtPara.linkNo  = gTrnsManagerMember.mStopDiagLinkNo;

	gTrnsManagerMember.mStopDiagLinkNo = 0;

	MSP_SendMessage(MID_TRNS, EVT_DIAG_COMPO_CONT_STOP, (UC *)(&evtPara), sizeof(evtPara), LOG_OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_JamCheckComplete
// Description   : 
// Parameter     : contID (Controller ID of Transport Module)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_JamCheckComplete(ETraControllerID contID)
{
	if( gTrnsManagerMember.mCollaboAct == TRNS_COLLABO_ACT_JAM_CHECK ) {
		if( TRNS_Device_GetState() == TRNS_CONT_STT_IDLE )   {
			MSP_SendMessage(MID_TRNS, EVT_DEV_RES_JAM_CHECK_TRA, NULL, 0, LOG_OFF);

			gTrnsManagerMember.mCollaboAct = TRNS_COLLABO_ACT_NUM;
		}
		else { /* No Action */ }												// 動作中
	}
	else { /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_JamClearAssistComplete
// Description   : 
// Parameter     : contID (Controller ID of Transport Module)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_JamClearAssistComplete(ETraControllerID contID)
{
	if( gTrnsManagerMember.mCollaboAct == TRNS_COLLABO_ACT_JAM_CLEAR_ASSIST ) {
		if( TRNS_Device_GetState() == TRNS_CONT_STT_IDLE ) {
			MSP_SendMessage(MID_TRNS, EVT_DEV_RES_JAM_CLEAR_ASSIST_TRA, NULL, 0, LOG_OFF);

			gTrnsManagerMember.mCollaboAct = TRNS_COLLABO_ACT_NUM;
		}
		else { /* No Action */ }												// 動作中
	}
	else { /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_SheetControlComplete
// Description   : 対象シ?トの搬送制御が(異常停?を含め)完了したこ
//               : との通知受信処理
// Parameter     : contID (Controller ID of Transport Module)
// Parameter     : sheetNo (Sheet No.)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_MGR_SheetControlComplete(ETraControllerID contID, UC sheetNo)
{
	TRNS_MGR_StandbyEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_MGR_GetState
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
UC TRNS_MGR_GetState(void)
{
	return gTrnsManagerMember.mState;
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
// 15.12.15	| Lee jae eun			| IOTとの結合デバッグ TB.No3 Stop Finisher/Shut Downを送信後にFinisherが回りっぱなしになる件の対策 
// 16.02.12    | Lee Jae Eun           | TB-00XX TRNS_Manager_Stanby遷移前、Abort処理によってTRNSDeviceがIdleに遷移させる。	V3.3.0
// 16.02.12    | Lee Jae Eun           | TB-00XX StopFinisher(HardDown)によってJam発生後、Jam用紙除去してもJamZoneClearされない部分修正。
//
