// File Name:	CMPL_Manager.h
// Description:	Compiler Module Manager
// Engineer:	Takahiko Minaga
// Last Edit:	14.08.12
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


#if !defined(CMPL_Manager_h)
#define CMPL_Manager_h


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "ECMPL_State.h"
#include "ECMPL_ModuleAction.h"
#include "ECMPL_ModuleEvent.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
bool CMPL_IsSheetInitExec(UC aSheetNo, ECMPL_ModuleAction aAction); //V3.5.93

void CMPL_InformedModuleActionComp(ECMPL_ModuleAction aAction, UC aSheetNo);
void CMPL_InformedModuleEvent(ECMPL_ModuleEvent aEvent, UC aSheetNo);
void CMPL_InformedDeviceStatusChange(EDeviceStatusNo aStatusNo, US aStatusValue);
void CMPL_InformedCompoControlComplete(US aChainNo, US aLinkNo, EAction aAction);
void CMPL_ReceiveStartUp();
void CMPL_ReceivePowerUpReq();
void CMPL_ReceivePowerDownReq();
void CMPL_ReceiveChangeNormalReq();
void CMPL_ReceiveChangeDiagReq();
void CMPL_ReceiveHardDownReq();
void CMPL_ReceiveCycleDownReq();
void CMPL_ReceiveDevActReqPowerOnInit();
void CMPL_ReceiveDevActReqResumeInit();
void CMPL_ReceiveDevActReqNormalInit();
void CMPL_ReceiveDevActReqProcessEnd();
void CMPL_ReceiveDevActReqJamClearAssist();
void CMPL_ReceiveDevEvtAnyCoverOpen();
void CMPL_ReceiveDevEvtAllCoverClose();
void CMPL_ReceiveDevEvtDevInitComp();
void CMPL_ReceiveDevEvt24VCut();												// Aries Finisher では不使用
void CMPL_ReceiveDevEvt24VCunnect();											// Aries Finisher では不使用
void CMPL_ReceiveCompileExitSensorLead(const UC* evt);
void CMPL_ReceiveCompileExitSensorTail(const UC* evt);
void CMPL_ReceiveSheetInitializeRequest(const UC* evt);
void CMPL_ReceiveCycleUpReq(const UC* evt);
void CMPL_ReceiveStandbyNotify(const UC* evt);
UC CMPL_ReceiveStandbyExitNotify(const UC* evt);

void CMPL_ReceiveSheetInformationEntry(const UC* evt);
void CMPL_ReceiveSheetInformationUpdate(const UC* evt);
void CMPL_ReceiveSheetInformationAbort(const UC* evt);
void CMPL_ReceiveSheetInfomationDelivered(const UC* evt);
void CMPL_ComponentControl(const UC* evt, EAction aAction);
void PutCMPLManagerLog(UC para1, UC para2, US para3, US para4);

#endif //!defined(CMPL_Manager_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//




