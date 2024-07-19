// File Name:	CSysFinSttPowerDown.h
// Description:	Header File of Finisher State "Power Down"
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CSysFinSttPowerDown_h
#define CSysFinSttPowerDown_h

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

void CSysFinSttPowerDown_Constructor();

bool CSysFinSttPowerDown_IsTransitionState();
bool CSysFinSttPowerDown_CheckTransitionPossibility(EFinisherState nextState);
bool CSysFinSttPowerDown_CheckExitCondition();
EFinisherState CSysFinSttPowerDown_GetNextState();
void CSysFinSttPowerDown_Entry(EFinisherState prevState);
bool CSysFinSttPowerDown_ReceivePowerDownResponse(UC moduleID);
bool CSysFinSttPowerDown_SaveNvmComplete(SS result);
bool CSysFinSttPowerDown_ReceiveDeviceActionResponse(UC moduleID, US eventID);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Table Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Power Down State ---
extern CSysFinisherState const gSys_FinSttPowerDownMethod;


#endif // #ifndef CSysFinSttPowerDown_h

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
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVM ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
