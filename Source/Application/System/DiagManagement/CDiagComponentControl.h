// File Name:	CDiagComponentControl.h
// Description:	Header File of Diag Component Control
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.08
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CDiagComponentControl_h
#define CDiagComponentControl_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinDiagResult.h"
#include "ECdiFinComponentResp.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Component Control Spec ---
// åªèÛïsóv
//typedef struct /*DiagComponentControlSpec*/
//{
//
//} DiagComponentControlSpec;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CDiagComponentControl_Constructor();

void CDiagComponentControl_Start(US chain, US link);
void CDiagComponentControl_Stop(US chain, US link);
void CDiagComponentControl_AllStopInput();
void CDiagComponentControl_On(US chain, US link, UC cyclic);
void CDiagComponentControl_Off(US chain, US link);
void CDiagComponentControl_AllOffOutput();
void CDiagComponentControl_OutputStartNotify(US chain, US link);
void CDiagComponentControl_OutputStopNotify(US chain, US link);
void CDiagComponentControl_InputMonitor();


#endif // #ifndef CDiagComponentControl_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
