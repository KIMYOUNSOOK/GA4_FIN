// File Name:	CMPL_STPL_Boundary.h
// Description:	Header File of Compile Staple Boundary
// Engineer:	Takahiko Minaga
// Last Edit:	14.09.01
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


#if !defined(CMPL_STPL_Boundary_h)
#define CMPL_STPL_Boundary_h


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "ECMPL_STPL_HEAD_Action.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CMPL_STPL_Reset();
void CMPL_STPL_SetUp();
void CMPL_STPL_PowerOff();
void CMPL_STPL_Normal();
void CMPL_STPL_Diag();
void CMPL_STPL_ProcStart();
void CMPL_STPL_StopSteadyFinishing();
void CMPL_STPL_AnyCoverOpen();
void CMPL_STPL_AllCoverClose();
void CMPL_STPL_StopModuleAction();
void CMPL_STPL_StopSheet(UC aSheetNo);
bool CMPL_STPL_IsExecutingModuleAction();
void CMPL_STPL_StapleHeadHome(UC aSheetNo);
void CMPL_STPL_Staple(UC aSheetNo);
void CMPL_STPL_StapleHeadHariout(UC aSheetNo);

void CMPL_STPL_ComponentControl(US aChainNo, US aLinkNo, EAction aAction);
void CMPL_STPL_InformedCompoComplete(US aLinkNo);
void CMPL_STPL_StandbyEjectStartNotify();
void CMPL_STPL_StandbyEjectCompNotify();

void CMPL_STPL_InformedStapleHeadComp();
void CMPL_STPL_InformedStapleSupplyLevel(UC aStapleSupply);
void CMPL_STPL_InformedStapleNG(EFaultStatus faultStatus);
void CMPL_STPL_InformedStapleFail(EFaultStatus faultStatus);
void CMPL_STPL_InformedStapleReadySensorFail(EFaultStatus faultStatus);
void CMPL_STPL_PrintLog(UC para1, UC para2, UC para3, US para4);


#endif //!defined(CMPL_STPL_Boundary_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
