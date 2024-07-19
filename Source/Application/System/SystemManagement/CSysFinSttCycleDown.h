// File Name:	CSysFinSttCycleDown.h
// Description:	Header File of Finisher State "Cycle Down"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.02.08
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CSysFinSttCycleDown_h
#define CSysFinSttCycleDown_h

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

void CSysFinSttCycleDown_Constructor();

bool CSysFinSttCycleDown_IsTransitionState();
bool CSysFinSttCycleDown_CheckTransitionPossibility(EFinisherState nextState);
bool CSysFinSttCycleDown_CheckExitCondition();
EFinisherState CSysFinSttCycleDown_GetNextState();
void CSysFinSttCycleDown_Entry(EFinisherState prevState);
void CSysFinSttCycleDown_Exit(EFinisherState nextState);
bool CSysFinSttCycleDown_ReceiveProcessDownResponse(UC moduleID);
bool CSysFinSttCycleDown_ReceiveDeviceActionResponse(UC moduleID, US eventID);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Table Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Cycle Down State ---
extern CSysFinisherState const gSys_FinSttCycleDownMethod;


#endif // #ifndef CSysFinSttCycleDown_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
