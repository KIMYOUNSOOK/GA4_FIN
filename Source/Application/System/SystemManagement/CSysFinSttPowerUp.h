// File Name:	CSysFinSttPowerUp.h
// Description:	Header File of Finisher State "Power Up"
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CSysFinSttPowerUp_h
#define CSysFinSttPowerUp_h

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

void CSysFinSttPowerUp_Constructor();

bool CSysFinSttPowerUp_IsTransitionState();
bool CSysFinSttPowerUp_CheckTransitionPossibility(EFinisherState nextState);
bool CSysFinSttPowerUp_CheckExitCondition();
EFinisherState CSysFinSttPowerUp_GetNextState();
void CSysFinSttPowerUp_Entry(EFinisherState prevState);
void CSysFinSttPowerUp_Exit(EFinisherState nextState);
bool CSysFinSttPowerUp_ReceivePowerUpResponse(UC moduleID);
void CSysFinSttPowerUp_SystemConfigurationSet();
bool CSysFinSttPowerUp_PowerUpModeSet(ESysPowerUpMode pwrUpMode);
bool CSysFinSttPowerUp_LoadNvmComplete(SS result);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Table Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Power Up State ---
extern CSysFinisherState const gSys_FinSttPowerUpMethod;


#endif // #ifndef CSysFinSttPowerUp_h

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
// 14.12.29	| Yuichi Hoshino	| Design Improvement [ Power Upサブシステム設計見直し ]
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVM ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
