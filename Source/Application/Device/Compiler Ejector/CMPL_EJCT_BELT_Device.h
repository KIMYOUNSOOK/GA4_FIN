// File Name:	CMPL_EJCT_BELT_Device.h
// Description:	Header File of Eject Mot Device Controller
// Engineer:	FXK KimYS
// Last Edit:	15.02.26
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


#if !defined(CMPL_EJCT_BELT_Device_h)
#define CMPL_EJCT_BELT_Device_h


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "ECMPL_EJCT_BELT_Action.h"


#define  BELT_RISE       (0)
#define  BELT_FALL       (1)

// Motor Direction
#define  BELT_FORWARD       (1)  
#define  BELT_REVERSE         (0)

/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
UC CMPL_EJCT_BELT_GetEjectorHomeSensor();

bool CMPL_EJCT_BELT_IsExecutiongDeviceAction();
bool CMPL_EJCT_BELT_IsOccurFault();


void CMPL_EJCT_BELT_Reset();
void CMPL_EJCT_BELT_DeviceStop();
void CMPL_EJCT_BELT_DeviceStopEnd(SS aResult);
void CMPL_EJCT_BELT_StopSheet(UC aSheetNo);
void CMPL_EJCT_BELT_StartAct(ECMPL_EJCT_BELT_Action aAction, UC aSheetNo);

extern void Callback(UC intCH, UC edge);

#endif //!defined(CMPL_EJCT_BELT_Device_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
