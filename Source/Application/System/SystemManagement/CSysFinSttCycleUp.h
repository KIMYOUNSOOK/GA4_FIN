// File Name:	CSysFinSttCycleUp.h
// Description:	Header File of Finisher State "Cycle Up"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.02.08
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CSysFinSttCycleUp_h
#define CSysFinSttCycleUp_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"
#include "CSysFinisherState.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CSysFinSttCycleUp_Constructor();

bool CSysFinSttCycleUp_IsTransitionState();
bool CSysFinSttCycleUp_CheckTransitionPossibility(EFinisherState nextState);
bool CSysFinSttCycleUp_CheckExitCondition();
EFinisherState CSysFinSttCycleUp_GetNextState();
void CSysFinSttCycleUp_Entry(EFinisherState prevState);
bool CSysFinSttCycleUp_ReceiveCycleUpResponse(UC moduleID);
bool CSysFinSttCycleUp_SheetManagerReady();


/////////////////////////////////////////////////////////////////////////////////////////
// Function Table Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Cycle Up State ---
extern CSysFinisherState const gSys_FinSttCycleUpMethod;


#endif // #ifndef CSysFinSttCycleUp_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
