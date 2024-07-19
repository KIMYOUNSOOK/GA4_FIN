// File Name:	CMPL_EJCT_Boundary.h
// Description:	Header File of Compile Staple Boundary
// Engineer:	Takahiko Minaga
// Last Edit:	14.09.01
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


#if !defined(CMPL_EJCT_Boundary_h)
#define CMPL_EJCT_Boundary_h


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "ECMPL_EJCT_BELT_Action.h"
#include "ECMPL_EJCT_SCLMP_Action.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
// Compiler ModuleÇ≈åƒÇ—èoÇµ
//US CMPL_EJCT_EjectorHomeTime(UC aSheetNo, UC aLastShtNo);
void CMPL_EJCT_Reset();
void CMPL_EJCT_SetUp();
void CMPL_EJCT_PowerOff();
void CMPL_EJCT_Normal();
void CMPL_EJCT_Diag();
void CMPL_EJCT_ProcStart();
void CMPL_EJCT_StopSteadyFinishing();
void CMPL_EJCT_AnyCoverOpen();
void CMPL_EJCT_AllCoverClose();
void CMPL_EJCT_StopModuleAction();
void CMPL_EJCT_StopSheet(UC aSheetNo);
void CMPL_EJCT_SheetAbort();
bool CMPL_EJCT_IsExecutingModuleAction();
void CMPL_EJCT_ComponentControl(US aChainNo, US aLinkNo, EAction aAction);
void CMPL_EJCT_EjectorHome(UC aSheetNo);
void CMPL_EJCT_PaddleHome(UC aSheetNo);
UC CMPL_EJCT_GetSubPaddleActionCount(UC aSheetNo);  //PreDVT.(V3.0.0)

void CMPL_EJCT_PaddleCompile(UC aSheetNo);
void CMPL_EJCT_Eject(UC aSheetNo);

void CMPL_EJCT_StandbyEjectStartNotify();
void CMPL_EJCT_StandbyEjectCompNotify();
void CMPL_EJCT_CurrentOFF();


void CMPL_EJCT_InformedEjectMotorComp(US aAction, UC aSheetNo);

void CMPL_EJCT_InformedEjectStart(UC aSheetNo);
void CMPL_EJCT_InformedEjectComp(UC aSheetNo);
void CMPL_EJCT_InformedEjectSheetThrough(UC aSheetNo);

void CMPL_EJCT_InformedEjectHomeSensorOnFail();
void CMPL_EJCT_InformedEjectHomeSensorOffFail();
void CMPL_EJCT_InformedPaddleHomeSensorOffFail();
void CMPL_EJCT_InformedPaddleHomeSensorOnFail();
void CMPL_EJCT_InformedSetClampHomeSensorOffFail();
void CMPL_EJCT_InformedSetClampHomeSensorOnFail();

void CMPL_EJCT_InformedSetEjectJam(UC aSheetNo);
void CMPL_EJCT_CompleteCompoEjcterMotor(US aLinkNo);

void PrintEjectLog_Short(US para1, US para2, US para3);

#endif //!defined(CMPL_EJCT_Boundary_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
