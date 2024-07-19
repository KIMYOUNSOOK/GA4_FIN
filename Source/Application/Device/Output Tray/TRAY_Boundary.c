// File Name:	TRAY_Boundary.c
// Description:	Source File of StackerTray Boundary
// Engineer:	KimYS
// Last Edit:	15.3.31
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "TRAY_Manager.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_StartUpEvent
// Description   : Start Upイベントコ?ルバック
// Parameter     : evt (メッセ?ジの先頭アドレス)
//               : from (イベント発行モジュ?ルID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_StartUpEvent(const UC* evt, UC from)
{
	TRAY_Manager_Constructor();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_PowerUpRequestEvent
// Description   : Power Up Requestイベントコ?ルバック
// Parameter     : evt (メッセ?ジの先頭アドレス)
//               : from (イベント発行モジュ?ルID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_PowerUpRequestEvent(const UC* evt, UC from)
{
	TRAY_Manager_PowerUp();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_PowerDownRequestEvent
// Description   : Power Down Requestイベントコ?ルバック
// Parameter     : evt (メッセ?ジの先頭アドレス)
//               : from (イベント発行モジュ?ルID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_PowerDownRequestEvent(const UC* evt, UC from)
{
	TRAY_Manager_PowerDown();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_ChangeDiagRequestEvent
// Description   : Change Diag Requestイベントコ?ルバック
// Parameter     : evt (メッセ?ジの先頭アドレス)
//               : from (イベント発行モジュ?ルID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_ChangeDiagRequestEvent(const UC* evt, UC from)
{
	TRAY_Manager_ChangeDiag();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_ChangeNormalRequestEvent
// Description   : Change Normal Requestイベントコ?ルバック
// Parameter     : evt (メッセ?ジの先頭アドレス)
//               : from (イベント発行モジュ?ルID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_ChangeNormalRequestEvent(const UC* evt, UC from)
{
	TRAY_Manager_ChangeNormal();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_CycleUpRequestEvent
// Description   : Cycle Up Requestイベントコ?ルバック
// Parameter     : evt (メッセ?ジの先頭アドレス)
//               : from (イベント発行モジュ?ルID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_CycleUpRequestEvent(const UC* evt, UC from)
{
	TRAY_Manager_CycleUp();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_DeviceActionRequestHardDown
// Description   : Hard Down Requestイベントコ?ルバック
// Parameter     : evt (メッセ?ジの先頭アドレス)
//               : from (イベント発行モジュ?ルID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_HardDownRequestEvent(const UC* evt, UC from)
{
	TRAY_Manager_HardDown();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_DeviceActionRequestCycleDown
// Description   : Cycle Down Requestイベントコ?ルバック
// Parameter     : evt (メッセ?ジの先頭アドレス)
//               : from (イベント発行モジュ?ルID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_CycleDownRequestEvent(const UC* evt, UC from)
{
	TRAY_Manager_CycleDown();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_DeviceEventAnyCoverOpen
// Description   : Device Action Request(Jam Clear Assist)イベントコ?ルバック
// Parameter     : evt (メッセ?ジの先頭アドレス)
//               : from (イベント発行モジュ?ルID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_DeviceEventAnyCoverOpen(const UC* evt, UC from)
{
	TRAY_Manager_AnyCoverOpenNotify();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_DeviceEventAllCoverClose
// Description   : Device Eventイベントコ?ルバック
// Parameter     : evt (メッセ?ジの先頭アドレス)
//               : from (イベント発行モジュ?ルID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_DeviceEventAllCoverClose(const UC* evt, UC from)
{
	TRAY_Manager_AllCoverCloseNotify();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_ComponentControlStartReq
// Description   : Component Control Start Requestイベントコ?ルバック
// Parameter     : evt (メッセ?ジの先頭アドレス)
//               : from (イベント発行モジュ?ルID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_ComponentControlStartReq(const UC* evt, UC from)
{
	TRAY_Manager_ComponentControl(evt, ACT_START);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_ComponentControlStopReq
// Description   : Component Control Stop Requestイベントコ?ルバック
// Parameter     : evt (メッセ?ジの先頭アドレス)
//               : from (イベント発行モジュ?ルID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_ComponentControlStopReq(const UC* evt, UC from)
{
	TRAY_Manager_ComponentControl(evt, ACT_STOP);
}

//////////////////////////////////////////////////////////////////
// Function Name : TRAY_SheetActionRequestEventStackerShtInitTRAY
// Description   : Sheet Action Request(Stacker Sheet Initialize Request)
//			 : イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント発行モジュールID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_SheetActionRequestEventStackerShtInitTRAY(const UC* evt, UC from)
{
	TRAY_Manager_SheetInitializeRequest(evt);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_DeviceActionRequestProcessEnd
// Description   : Device Action Request(Process End)イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント発行モジュールID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_DeviceActionRequestProcessEnd(const UC* evt, UC from)
{
	//TRAY_Manager_ProcessEnd(ACT_START);
	TRAY_Manager_ProcessEnd(evt);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_SheetEventNotifyEventOutTrayEjectStart : Eject 位置へ移動
// Description   : Sheet Event Notifyイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_SheetEventNotifyEventOutTrayEjectStart(const UC* evt, UC from)
{
	EvtParamSheetEventNotify *pMessage;

	pMessage = (EvtParamSheetEventNotify *)evt;

	TRAY_Manager_SheetEventNotify(EVT_SHT_EVT_OUT_TRAY_EJECT_START,  pMessage->sheetNo);
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_SheetEventNotifyShtInitStackerJobStart
// Description   : Sheet Event Notifyイベントコールバック(Sheet Initialize ->  Stacker Job Init 
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_SheetEventNotifyShtInitStackerJobStart(const UC* evt, UC from)
{
	EvtParamSheetEventNotify *pMessage;

	pMessage = (EvtParamSheetEventNotify *)evt;

	TRAY_Manager_SheetEventNotify(EVT_SHT_EVT_SHT_INIT_STACKER_JOBSTART, pMessage->sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_SheetEventNotifyShtInitStackerWaitingPositionStart
// Description   : Sheet Event Notifyイベントコールバック( SetClamp 動作完了-> 待機位置へ上げ動作開始Trigger)
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_SheetEventNotifyShtInitStackerWaitingPositionStart(const UC* evt, UC from)
{
	EvtParamSheetEventNotify *pMessage;

	pMessage = (EvtParamSheetEventNotify *)evt;

	TRAY_Manager_SheetEventNotify(EVT_SHT_EVT_SHT_INIT_STACKER_WAITING_POS_START, pMessage->sheetNo);
}

void TRAY_SheetEventNotifyTrayJobTrigger_1(const UC* evt, UC from)
{
	EvtParamSheetEventNotify *pMessage;

	pMessage = (EvtParamSheetEventNotify *)evt;

	TRAY_Manager_SheetEventNotify(EVT_SHT_EVT_TRAYJOB_TRIGGER_1, pMessage->sheetNo);

}

void TRAY_SheetEventNotifyTrayJobTrigger_2(const UC* evt, UC from)
{

}

//////////////////////////////////////////////////////////////////
// Function Name : TRAY_StandbyExitNotifyEvent
// Description   : Finisher状態がStandby→Exitになるのを通知するイベント
//				 : イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント発行モジュールID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_StandbyExitNotifyEvent(const UC* evt, UC from)
{
	TRAY_Manager_ReceiveStandbyExitNotify();
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
