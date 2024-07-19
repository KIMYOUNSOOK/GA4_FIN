// File Name:	CSysFinSttStandby.h
// Description:	Header File of Finisher State "Standby"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.02.08
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CSysFinSttStandby_h
#define CSysFinSttStandby_h

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

void CSysFinSttStandby_Constructor();

bool CSysFinSttStandby_CheckTransitionPossibility(EFinisherState nextState);
void CSysFinSttStandby_Entry(EFinisherState prevState);
void CSysFinSttStandby_Exit(EFinisherState nextState);
bool CSysFinSttStandby_ReceiveDeviceActionResponse(UC moduleID, US eventID);
ESysProcStopType CSysFinSttStandby_CoverStatusChange(ECvrCoverID coverID, ECvrCoverStatus status);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Table Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Standby State ---
extern CSysFinisherState const gSys_FinSttStandbyMethod;


#endif // #ifndef CSysFinSttStandby_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
