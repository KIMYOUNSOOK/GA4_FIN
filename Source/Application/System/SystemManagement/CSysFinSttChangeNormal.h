// File Name:	CSysFinSttChangeNormal.h
// Description:	Header File of Finisher State "Change Normal"
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CSysFinSttChangeNormal_h
#define CSysFinSttChangeNormal_h

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

void CSysFinSttChangeNormal_Constructor();

bool CSysFinSttChangeNormal_IsTransitionState();
bool CSysFinSttChangeNormal_CheckTransitionPossibility(EFinisherState nextState);
bool CSysFinSttChangeNormal_CheckExitCondition();
EFinisherState CSysFinSttChangeNormal_GetNextState();
void CSysFinSttChangeNormal_Entry(EFinisherState prevState);
void CSysFinSttChangeNormal_Exit(EFinisherState nextState);
bool CSysFinSttChangeNormal_ReceiveChangeNormalResponse(UC moduleID);
bool CSysFinSttChangeNormal_ReceiveDeviceActionResponse(UC moduleID, US eventID);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Table Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Change Normal State ---
extern CSysFinisherState const gSys_FinSttChangeNormalMethod;


#endif // #ifndef CSysFinSttChangeNormal_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.11.06	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.03.30	| Yuichi Hoshino	| Trouble Action [ カバークローズイニシャライズ動作実施せず ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
