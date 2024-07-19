// File Name:	CMPL_TMP_Boundary.h
// Description:	Header File of Compile Tamper Boundary
// Engineer:	Takahiko Minaga
// Last Edit:	14.09.01
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


#if !defined(CMPL_TMP_Boundary_h)
#define CMPL_TMP_Boundary_h


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "ECMPL_TMP_Action.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
// Compiler ModuleÇ≈åƒÇ—èoÇµ
void CMPL_TMP_Reset();
void CMPL_TMP_SetUp();
void CMPL_TMP_PowerOff();
void CMPL_TMP_Normal();
void CMPL_TMP_Diag();
void CMPL_TMP_ProcStart();
void CMPL_TMP_StopSteadyFinishing();
void CMPL_TMP_AnyCoverOpen();
void CMPL_TMP_AllCoverClose();
void CMPL_TMP_StopModuleAction();
void CMPL_TMP_StopSheet(UC aSheetNo);
bool CMPL_TMP_IsExecutingModuleAction();
void CMPL_TMP_ComponentControl(US aChainNo, US aLinkNo, EAction aAction);
void CMPL_TMP_SizePositioning(UC aSheetNo);
void CMPL_TMP_NormalTamping(UC aSheetNo);
void CMPL_TMP_EjectTamping(UC aSheetNo);
void CMPL_TMP_MissRegi(UC aSheetNo);
void CMPL_TMP_Offset(UC aSheetNo);
void CMPL_TMP_Return(UC aSheetNo);
void CMPL_TMP_Open(UC aSheetNo);

US CMPL_TMP_GetSizePositioningTime(UC aSheetNo, UC aLastShtNo);
US CMPL_TMP_GetNormalTampingDelayTime(UC aSheetNo);
US CMPL_TMP_GetEjectTampingDelayTime(UC aSheetNo);
bool CMPL_TMP_IsEjectStart();
void CMPL_TMP_EjectStart(UC aSheetNo);

US CMPL_TMP_GetNormalTampingActionTime(UC aSheetNo);
US CMPL_TMP_GetEjectTampingActionTime(UC aSheetNo);

bool CMPL_TMP_IsOpenExec(UC sheetNo);


void CMPL_TMP_EjectSheetThrough(UC aSheetNo);
void CMPL_TMP_StandbyEjectStartNotify();
void CMPL_TMP_StandbyEjectCompNotify();


// Device Ç©ÇÁÇÃí ím
bool CMPL_TMP_IsCanTampingSheetSize(UC aSheetNo);

void CMPL_TMP_InformedLeftTamperComplete(ECMPL_TMP_Action aAction, UC aSheetNo);
void CMPL_TMP_InformedRightTamperComplete(ECMPL_TMP_Action aAction, UC aSheetNo);
void CMPL_TMP_InformedCompoComplete(US aLinkNo);
bool CMPL_TMP_IsEjectSheetThrough();
void CMPL_TMP_InformedLeftTamperHomeSensorOnFail(UC aSheetNo);
void CMPL_TMP_InformedLeftTamperHomeSensorOffFail(UC aSheetNo);
void CMPL_TMP_InformedRightTamperHomeSensorOnFail(UC aSheetNo);
void CMPL_TMP_InformedRightTamperHomeSensorOffFail(UC aSheetNo);


#endif //!defined(CMPL_TMP_Boundary_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
