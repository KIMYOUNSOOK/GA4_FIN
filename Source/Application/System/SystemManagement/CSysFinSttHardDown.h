// File Name:	CSysFinSttHardDown.h
// Description:	Header File of Finisher State "Hard Down"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.02.08
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CSysFinSttHardDown_h
#define CSysFinSttHardDown_h

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

void CSysFinSttHardDown_Constructor();

bool CSysFinSttHardDown_IsTransitionState();
bool CSysFinSttHardDown_CheckTransitionPossibility(EFinisherState nextState);
bool CSysFinSttHardDown_CheckExitCondition();
EFinisherState CSysFinSttHardDown_GetNextState();
void CSysFinSttHardDown_Entry(EFinisherState prevState);
void CSysFinSttHardDown_Exit(EFinisherState nextState);
bool CSysFinSttHardDown_ReceiveProcessDownResponse(UC moduleID);
bool CSysFinSttHardDown_ReceiveDeviceActionResponse(UC moduleID, US eventID);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Table Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Hard Down State ---
extern CSysFinisherState const gSys_FinSttHardDownMethod;


#endif // #ifndef CSysFinSttHardDown_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
