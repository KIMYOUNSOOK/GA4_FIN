// File Name:	TRNS_Bpundary.c
// Description:	Source File of Transport Module Boundary
// Engineer:	Yuichi Hoshino
// Last Edit:	14.09.25
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "TRAY_Manager.h"
#include "TRNS_Device.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_StartUpEvent
// Description   : Start Up�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_StartUpEvent(const UC* evt, UC from)
{
	TRNS_MGR_Constructor();

	TRNS_MGR_StartUp();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_PowerUpRequestEvent
// Description   : Power Up�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_PowerUpRequestEvent(const UC* evt, UC from)
{
	TRNS_MGR_PowerUp();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_PowerDownRequestEvent
// Description   : Power Down�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_PowerDownRequestEvent(const UC* evt, UC from)
{
	TRNS_MGR_PowerDown();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_ChangeDiagRequestEvent
// Description   : Change Diag Request�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_ChangeDiagRequestEvent(const UC* evt, UC from)
{
	TRNS_MGR_ChangeDiag();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_ChangeNormalRequestEvent
// Description   : Change Normal Request�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_ChangeNormalRequestEvent(const UC* evt, UC from)
{
	TRNS_MGR_ChangeNormal();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_CycleUpRequestEvent
// Description   : Cycle Up Request�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_CycleUpRequestEvent(const UC* evt, UC from)
{
	TRNS_MGR_CycleUp();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_HardDownRequestEvent
// Description   : Hard Down Request�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_HardDownRequestEvent(const UC* evt, UC from)
{
	TRNS_MGR_HardDown();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_CycleDownRequestEvent
// Description   : Cycle Down Request�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_CycleDownRequestEvent(const UC* evt, UC from)
{
	TRNS_MGR_CycleDown();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_DeviceActionRequestJamCheck
// Description   : Device Action Request(Jam Check)�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_DeviceActionRequestJamCheck(const UC* evt, UC from)
{
	TRNS_MGR_JamCheckRequest();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_DeviceActionRequestProcessEnd
// Description   : Device Action Request(Process End Action)�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_DeviceActionRequestProcessEnd(const UC* evt, UC from)
{
	TRNS_MGR_ProcessEndRequest();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_DeviceActionRequestJamClearAssist
// Description   : Device Action Request(Jam Clear Assist)�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_DeviceActionRequestJamClearAssist(const UC* evt, UC from)
{
	TRNS_MGR_JamClearAssistRequest();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_DeviceEventAnyCoverOpen
// Description   : Device Action Request(Jam Clear Assist)�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_DeviceEventAnyCoverOpen(const UC* evt, UC from)
{
    	TRNS_MGR_AnyCoverOpenNotify();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_DeviceEventAllCoverClose
// Description   : Device Event�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_DeviceEventAllCoverClose(const UC* evt, UC from)
{
	TRNS_MGR_AllCoverCloseNotify();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_SheetAbortedNotify
// Description   : Sheet Sheet Aborted Notify�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_SheetAbortedNotify(const UC* evt, UC from)
{
	TRNS_MGR_SheetAbortedNotify();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_ComponentControlStartReq
// Description   : Component Control Start Request�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_ComponentControlStartReq(const UC* evt, UC from)
{
	TRNS_MGR_ComponentControl(evt, ACT_START);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_ComponentControlStopReq
// Description   : Component Control Stop Request�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_ComponentControlStopReq(const UC* evt, UC from)
{
	TRNS_MGR_ComponentControl(evt, ACT_STOP);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_IotExitLocationLead
// Description   : 
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_IotExitLocationLead(const UC* evt, UC from)
{
	LocationEvent *pRcvEvtParameter;

	pRcvEvtParameter = (LocationEvent *)evt;

	TRNS_Device_IotExitLocationLead((UC)pRcvEvtParameter->mSheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_CompileExitLocationTail
// Description   : 
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_CompileExitLocationLead(const UC* evt, UC from)
{
	// LocationEvent *pRcvEvtParameter;
	// pRcvEvtParameter = (LocationEvent *)evt;
	TRNS_Device_CompileExitSnrLocationLead();
}
///////////////////////////////////////////////////////////////////
// Function Name : TRNS_CompileExitLocationTail
// Description   : 
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_CompileExitLocationTail(const UC* evt, UC from)
{
	// LocationEvent *pRcvEvtParameter;
	// pRcvEvtParameter = (LocationEvent *)evt;
	TRNS_Device_CompileExitSnrLocationTail();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_IOTGATE_SignalON
// Description   : 
// Parameter     : notUsed (���b�Z?�W�̐擪�A�h���X(���g�p))
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_IOTGATE_SignalON(const UC* notUsed, UC from)
{
	TRNS_Device_IOTGATE_SignalON();
}
///////////////////////////////////////////////////////////////////
// Function Name : TRNS_IOTGATE_SignalOFF
// Description   : 
// Parameter     : notUsed (���b�Z?�W�̐擪�A�h���X(���g�p))
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_IOTGATE_SignalOFF(const UC* notUsed, UC from)
{
	TRNS_Device_IOTGATE_SignalOFF();
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
