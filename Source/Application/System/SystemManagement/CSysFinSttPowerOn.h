// File Name:	CSysFinSttPowerOn.h
// Description:	Header File of Finisher State "Power On"
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CSysFinSttPowerOn_h
#define CSysFinSttPowerOn_h

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

void CSysFinSttPowerOn_Constructor();

bool CSysFinSttPowerOn_CheckTransitionPossibility(EFinisherState nextState);
void CSysFinSttPowerOn_Entry(EFinisherState prevState);
bool CSysFinSttPowerOn_ReceiveDeviceActionResponse(UC moduleID, US eventID);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Table Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Power On State ---
extern CSysFinisherState const gSys_FinSttPowerOnMethod;


#endif // #ifndef CSysFinSttPowerOn_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.02.08	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.03.30	| Yuichi Hoshino	| Trouble Action [ カバークローズイニシャライズ動作実施せず ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
