// File Name:	CMPL_Boundary.c
// Description:	Compiler Module Boundary
// Engineer:	Takahiko Minaga
// Last Edit:	14.08.12
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"
#include "CMPL_Boundary.h"
#include "CMPL_Manager.h"



/////////////////////////////////////////////////////////////////////////////////////////
// Frame Event Call Back Function
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_StartUpEvent
// Description   : Start Up�C�x���g�R�[���o�b�N
// Parameter     : evt (�C�x���g�p�����[�^�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_StartUpEvent(const UC* evt, UC from)
{
	CMPL_ReceiveStartUp();
}

/////////////////////////////////////////////////////////////////////////////////////////
// Finisher State Management Event Call Back Function
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_PowerUpReqEvent
// Description   : Power Up Req�C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_PowerUpReqEvent(const UC* evt, UC from)
{
	CMPL_ReceivePowerUpReq();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_PowerDownReqEvent
// Description   : Power Down Req�C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_PowerDownReqEvent(const UC* evt, UC from)
{
	CMPL_ReceivePowerDownReq();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ChangeNormalReqEvent
// Description   : Change Normal Req�C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ChangeNormalReqEvent(const UC* evt, UC from)
{
	CMPL_ReceiveChangeNormalReq();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ChangeDiagReqEvent
// Description   : Change Diag Req�C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ChangeDiagReqEvent(const UC* evt, UC from)
{
	CMPL_ReceiveChangeDiagReq();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_HardDownReqEvent
// Description   : Hard Down Req�C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_HardDownReqEvent(const UC* evt, UC from)
{
	CMPL_ReceiveHardDownReq();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_CycleDownReqEvent
// Description   : Cycle Down Req�C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_CycleDownReqEvent(const UC* evt, UC from)
{
	CMPL_ReceiveCycleDownReq();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_DevActReqPowerOnInitEvent
// Description   : Device Action Request(Power On Init)�C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_DevActReqPowerOnInitEvent(const UC* evt, UC from)
{
	CMPL_ReceiveDevActReqPowerOnInit();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_DevActReqResumeInitEvent
// Description   : Device Action Request(Resume Init)�C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_DevActReqResumeInitEvent(const UC* evt, UC from)
{
	CMPL_ReceiveDevActReqResumeInit();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_DevActReqNormalInitEvent
// Description   : Device Action Request(Normal Init)�C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_DevActReqNormalInitEvent(const UC* evt, UC from)
{
	CMPL_ReceiveDevActReqNormalInit();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_DevActReqProcessEndEvent
// Description   : Device Action Request(Process End)�C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_DevActReqProcessEndEvent(const UC* evt, UC from)
{
	CMPL_ReceiveDevActReqProcessEnd();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_DevActReqJamClearAssistEvent
// Description   : Device Action Request(Jam Clear Assist)�C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_DevActReqJamClearAssistEvent(const UC* evt, UC from)
{
	CMPL_ReceiveDevActReqJamClearAssist();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_DevEvtAnyCoverOpenEvent
// Description   : Device Event Notify(Any Cover Open)�C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_DevEvtAnyCoverOpenEvent(const UC* evt, UC from)
{
	CMPL_ReceiveDevEvtAnyCoverOpen();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_DevEvtAllCoverCloseEvent
// Description   : Device Event Notify(All Cover Close)�C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_DevEvtAllCoverCloseEvent(const UC* evt, UC from)
{
	CMPL_ReceiveDevEvtAllCoverClose();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_DevEvtDevInitCompEvent
// Description   : Device Event Notify(Device Initialize Complete)�C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_DevEvtDevInitCompEvent(const UC* evt, UC from)
{
	CMPL_ReceiveDevEvtDevInitComp();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_DevEvt24VCutEvent
// Description   : Device Event Notify(24V Cut)�C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_DevEvt24VCutEvent(const UC* evt, UC from)								// Aries Finisher �ł͕s�g�p
{
	CMPL_ReceiveDevEvt24VCut();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_DevEvt24VConnectEvent
// Description   : Device Event Notify(24V Cunnect)�C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_DevEvt24VCunnectEvent(const UC* evt, UC from)							// Aries Finisher �ł͕s�g�p
{
	CMPL_ReceiveDevEvt24VCunnect();
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_ComponentControlStartReq
// Description   : Component Control Start Request�C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ComponentControlStartReq(const UC* evt, UC from)
{
	CMPL_ComponentControl(evt, ACT_START);
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_ComponentControlStopReq
// Description   : Component Control Stop Request�C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ComponentControlStopReq(const UC* evt, UC from)
{
	CMPL_ComponentControl(evt, ACT_STOP);
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_CompileExitSensorLeadEvent
// Description   : Compile Exit Senosr ON �C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_CompileExitSensorLeadEvent(const UC* evt, UC from)
{
	CMPL_ReceiveCompileExitSensorLead(evt);
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_CompileExitSensorTailEvent
// Description   : Compile Exit Senosr ON �C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_CompileExitSensorTailEvent(const UC* evt, UC from)
{
	CMPL_ReceiveCompileExitSensorTail(evt);
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_SheetActionRequestEventCompilerShtInitCMPL
// Description   : Sheet Action Request(Compiler Sheet Initialize Request)
//				 : �C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_SheetActionRequestEventCompilerShtInitCMPL(const UC* evt, UC from)
{
	CMPL_ReceiveSheetInitializeRequest(evt);
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_CycleUpReqEvent
// Description   : Cycle Up Req �C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_CycleUpReqEvent(const UC* evt, UC from)
{
	CMPL_ReceiveCycleUpReq(evt);
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_StandbyNotifyEvent
// Description   : Finisher��Ԃ�Standby��Ԃɍs�����̂�ʒm����C�x���g
//				 : �C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_StandbyNotifyEvent(const UC* evt, UC from)
{
	CMPL_ReceiveStandbyNotify(evt);
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_StandbyExitNotifyEvent
// Description   : Finisher��Ԃ�Standby��Exit�ɂȂ�̂�ʒm����C�x���g
//				 : �C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_StandbyExitNotifyEvent(const UC* evt, UC from)
{
//	CMPL_ReceiveStandbyExitNotify(evt);
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_SheetInformationEventUpdate
// Description   : Sheet Information (Update)
//				 : �C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_SheetInformationEventUpdate(const UC* evt, UC from)
{
	CMPL_ReceiveSheetInformationUpdate(evt);
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_SheetInformationEventAbort
// Description   : Sheet Information (Abort)
//				 : �C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_SheetInformationEventAbort(const UC* evt, UC from)
{
	CMPL_ReceiveSheetInformationAbort(evt);
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_SheetInfomationEventDelivered
// Description   : Sheet Information (Delivered)
//				 : �C�x���g�R�[���o�b�N
// Parameter     : evt (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���s���W���[��ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_SheetInfomationEventDelivered(const UC* evt, UC from)
{
	CMPL_ReceiveSheetInfomationDelivered(evt);
}

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
