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
// Description   : Start Up�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_StartUpEvent(const UC* evt, UC from)
{
	TRAY_Manager_Constructor();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_PowerUpRequestEvent
// Description   : Power Up Request�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_PowerUpRequestEvent(const UC* evt, UC from)
{
	TRAY_Manager_PowerUp();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_PowerDownRequestEvent
// Description   : Power Down Request�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_PowerDownRequestEvent(const UC* evt, UC from)
{
	TRAY_Manager_PowerDown();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_ChangeDiagRequestEvent
// Description   : Change Diag Request�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_ChangeDiagRequestEvent(const UC* evt, UC from)
{
	TRAY_Manager_ChangeDiag();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_ChangeNormalRequestEvent
// Description   : Change Normal Request�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_ChangeNormalRequestEvent(const UC* evt, UC from)
{
	TRAY_Manager_ChangeNormal();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_CycleUpRequestEvent
// Description   : Cycle Up Request�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_CycleUpRequestEvent(const UC* evt, UC from)
{
	TRAY_Manager_CycleUp();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_DeviceActionRequestHardDown
// Description   : Hard Down Request�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_HardDownRequestEvent(const UC* evt, UC from)
{
	TRAY_Manager_HardDown();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_DeviceActionRequestCycleDown
// Description   : Cycle Down Request�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_CycleDownRequestEvent(const UC* evt, UC from)
{
	TRAY_Manager_CycleDown();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_DeviceEventAnyCoverOpen
// Description   : Device Action Request(Jam Clear Assist)�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_DeviceEventAnyCoverOpen(const UC* evt, UC from)
{
	TRAY_Manager_AnyCoverOpenNotify();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_DeviceEventAllCoverClose
// Description   : Device Event�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_DeviceEventAllCoverClose(const UC* evt, UC from)
{
	TRAY_Manager_AllCoverCloseNotify();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_ComponentControlStartReq
// Description   : Component Control Start Request�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_ComponentControlStartReq(const UC* evt, UC from)
{
	TRAY_Manager_ComponentControl(evt, ACT_START);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_ComponentControlStopReq
// Description   : Component Control Stop Request�C�x���g�R?���o�b�N
// Parameter     : evt (���b�Z?�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W��?��ID)
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_ComponentControlStopReq(const UC* evt, UC from)
{
	TRAY_Manager_ComponentControl(evt, ACT_STOP);
}

//////////////////////////////////////////////////////////////////
// Function Name : TRAY_SheetActionRequestEventStackerShtInitTRAY
// Description   : Sheet Action Request(Stacker Sheet Initialize Request)
//			 : �C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_SheetActionRequestEventStackerShtInitTRAY(const UC* evt, UC from)
{
	TRAY_Manager_SheetInitializeRequest(evt);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_DeviceActionRequestProcessEnd
// Description   : Device Action Request(Process End)�C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_DeviceActionRequestProcessEnd(const UC* evt, UC from)
{
	//TRAY_Manager_ProcessEnd(ACT_START);
	TRAY_Manager_ProcessEnd(evt);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_SheetEventNotifyEventOutTrayEjectStart : Eject �ʒu�ֈړ�
// Description   : Sheet Event Notify�C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_SheetEventNotifyEventOutTrayEjectStart(const UC* evt, UC from)
{
	EvtParamSheetEventNotify *pMessage;

	pMessage = (EvtParamSheetEventNotify *)evt;

	TRAY_Manager_SheetEventNotify(EVT_SHT_EVT_OUT_TRAY_EJECT_START,  pMessage->sheetNo);
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_SheetEventNotifyShtInitStackerJobStart
// Description   : Sheet Event Notify�C�x���g�R�[���o�b�N(Sheet Initialize ->  Stacker Job Init 
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_SheetEventNotifyShtInitStackerJobStart(const UC* evt, UC from)
{
	EvtParamSheetEventNotify *pMessage;

	pMessage = (EvtParamSheetEventNotify *)evt;

	TRAY_Manager_SheetEventNotify(EVT_SHT_EVT_SHT_INIT_STACKER_JOBSTART, pMessage->sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_SheetEventNotifyShtInitStackerWaitingPositionStart
// Description   : Sheet Event Notify�C�x���g�R�[���o�b�N( SetClamp ���슮��-> �ҋ@�ʒu�֏グ����J�nTrigger)
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
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
// Description   : Finisher��Ԃ�Standby��Exit�ɂȂ�̂�ʒm����C�x���g
//				 : �C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
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
