// File Name:	SystemManagement.c
// Description:	Implimentation File of System Management
// Engineer:	Yuichi Hoshino
// Last Edit:	14.02.08
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSysManager.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Device Condition Notify Event ID Convert Table ---
US const cSys_DeviceConditionEventID[DEVID_NUM] = {
	// !!! Product Specific !!!
	EVT_COND_DEV_COND_FINISHER,
	EVT_COND_DEV_COND_OUT_TRAY,
	EVT_COND_DEV_COND_STAPLER,
	// EVT_COND_DEV_COND_PUNCHER
};


/////////////////////////////////////////////////////////////////////////////////////////
// Frame Event Call Back Function
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : SYS_StartUpEvent
// Description   : Start Upイベントコールバック
// Parameter     : evt (イベントパラメータの先頭アドレス)
//               : from (イベント発行モジュールID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_StartUpEvent(const UC* evt, UC from)
{
	CSysManager_Constructor();

	CSysManager_PowerUp();
}


/////////////////////////////////////////////////////////////////////////////////////////
// Finisher State Management Event Call Back Function
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : SYS_PowerUpResponseEvent
// Description   : Power Up Responseイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント発行モジュールID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_PowerUpResponseEvent(const UC* evt, UC from)
{
	CSysManager_ReceivePowerUpResponse((UC)from);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_PowerDownResponseEvent
// Description   : Power Down Responseイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント発行モジュールID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_PowerDownResponseEvent(const UC* evt, UC from)
{
	CSysManager_ReceivePowerDownResponse((UC)from);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_ChangeDiagResponseEvent
// Description   : Change Diag Responseイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント発行モジュールID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_ChangeDiagResponseEvent(const UC* evt, UC from)
{
	CSysManager_ReceiveChangeDiagResponse((UC)from);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_ChangeNormalResponseEvent
// Description   : Change Normal Responseイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント発行モジュールID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_ChangeNormalResponseEvent(const UC* evt, UC from)
{
	CSysManager_ReceiveChangeNormalResponse((UC)from);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_CycleUpResponseEvent
// Description   : Cycle Up Responseイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント発行モジュールID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_CycleUpResponseEvent(const UC* evt, UC from)
{
	CSysManager_ReceiveCycleUpResponse((UC)from);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_ProcessDownResponseEvent
// Description   : Process Down Responseイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント発行モジュールID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_ProcessDownResponseEvent(const UC* evt, UC from)
{
	CSysManager_ReceiveProcessDownResponse((UC)from);
}


/////////////////////////////////////////////////////////////////////////////////////////
// Device Management Event Call Back Function
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : SYS_DeviceActionResponseEventPowerOnInitializeCMPL
// Description   : Device Action Response(Power On Initialize)イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント発行モジュールID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_DeviceActionResponseEventPowerOnInitializeCMPL(const UC* evt, UC from)
{
	CSysManager_ReceiveDeviceActionResponse((UC)from, EVT_DEV_RES_POWER_ON_INIT_CMPL);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_DeviceActionResponseEventPowerResumeInitializeCMPL
// Description   : Device Action Response()イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント発行モジュールID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_DeviceActionResponseEventPowerResumeInitializeCMPL(const UC* evt, UC from)
{
	CSysManager_ReceiveDeviceActionResponse((UC)from, EVT_DEV_RES_RESUME_INIT_CMPL);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_DeviceActionResponseEventNormalInitializeCMPL
// Description   : Device Action Response()イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント発行モジュールID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_DeviceActionResponseEventNormalInitializeCMPL(const UC* evt, UC from)
{
	CSysManager_ReceiveDeviceActionResponse((UC)from, EVT_DEV_RES_NORMAL_INIT_CMPL);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_DeviceActionResponseEventJamCheckTRNS
// Description   : Device Action Response()イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント発行モジュールID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_DeviceActionResponseEventJamCheckTRNS(const UC* evt, UC from)
{
	CSysManager_ReceiveDeviceActionResponse((UC)from, EVT_DEV_RES_JAM_CHECK_TRA);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_DeviceActionResponseEventProcessEndActionTRNS
// Description   : Device Action Response()イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント発行モジュールID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_DeviceActionResponseEventProcessEndActionTRNS(const UC* evt, UC from)
{
	CSysManager_ReceiveDeviceActionResponse((UC)from, EVT_DEV_RES_PROCESS_END_TRA);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_DeviceActionResponseEventProcessEndActionCMPL
// Description   : Device Action Response()イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント発行モジュールID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_DeviceActionResponseEventProcessEndActionCMPL(const UC* evt, UC from)
{
	CSysManager_ReceiveDeviceActionResponse((UC)from, EVT_DEV_RES_PROCESS_END_CMPL);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_DeviceActionResponseEventJamClearAssistInitializeTRNS
// Description   : Device Action Response()イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント発行モジュールID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_DeviceActionResponseEventJamClearAssistInitializeTRNS(const UC* evt, UC from)
{
	CSysManager_ReceiveDeviceActionResponse((UC)from, EVT_DEV_RES_JAM_CLEAR_ASSIST_TRA);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_DeviceActionResponseEventJamClearAssistInitializeCMPL
// Description   : Device Action Response()イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント発行モジュールID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_DeviceActionResponseEventJamClearAssistInitializeCMPL(const UC* evt, UC from)
{
	CSysManager_ReceiveDeviceActionResponse((UC)from, EVT_DEV_RES_JAM_CLEAR_ASSIST_CMPL);
}
/////////////////////////////////////////////////////////////////// 
// Function Name : SYS_DeviceActionResponseEventProcessEndActionTRAY  [GLOBAL_A4]
// Description   : Device Action Response()イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント発行モジュールID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_DeviceActionResponseEventProcessEndActionTRAY(const UC* evt, UC from)
{
	CSysManager_ReceiveDeviceActionResponse((UC)from, EVT_DEV_RES_PROCESS_END_TRAY);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_DeviceEventNotifyEventXXX
// Description   : Device Event Notifyイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント発行モジュールID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
//void SYS_DeviceEventNotifyEventXXX(const UC* evt, UC from)
//{
//	CSysManager_ReceiveDeviceEventNotify((UC)from, DEV_EVT_XXX);
//}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_DeviceStatusNotifyEvent
// Description   : Device Status Notifyイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント発行モジュールID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_DeviceStatusNotifyEvent(const UC* evt, UC from)
{
	CSysManager_ReceiveDeviceStatusNotify((UC)from, ((EvtParamDeviceStatusNotify*)evt)->statusNo, ((EvtParamDeviceStatusNotify*)evt)->statusValue);
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
