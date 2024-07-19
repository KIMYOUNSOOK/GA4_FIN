// File Name:	DiagManagement.c
// Description:	Implimentation File of Diag Management
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.11
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CDiagManager.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Diag Management Event Call Back Function
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : DIAG_ComponentControlStartNotifyEvent
// Description   : Component Control Start Notifyイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void DIAG_ComponentControlStartNotifyEvent(const UC* evt, UC from)
{
	EvtParamComponentControlStartNotify *pMessage;

	pMessage = (EvtParamComponentControlStartNotify *)evt;

	CDiagManager_ReceiveComponentControlStartNotify(from, pMessage->chainNo, pMessage->linkNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : DIAG_ComponentControlStopNotifyEvent
// Description   : Component Control Stop Notifyイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void DIAG_ComponentControlStopNotifyEvent(const UC* evt, UC from)
{
	EvtParamComponentControlStopNotify *pMessage;

	pMessage = (EvtParamComponentControlStopNotify *)evt;

	CDiagManager_ReceiveComponentControlStopNotify(from, pMessage->chainNo, pMessage->linkNo);
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
