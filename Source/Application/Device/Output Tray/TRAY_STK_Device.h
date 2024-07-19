// File Name:	TRAY_STK_Device.h
// Description:	Header File of Stacker Device
// Engineer:	FXK
// Last Edit:	15.03.25
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef TRAY_STK_Device_h
#define TRAY_STK_Device_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "ETRAY_STK_Action.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void TRAY_STK_StartAct(ETRAY_STK_Action aAction, UC aSheetNo);
void TRAY_STK_PreStartAct(ETRAY_STK_Action aAction, UC aSheetNo); 

void TRAY_STK_Reset();
void TRAY_STK_DeviceStop();

bool TRAY_STK_IsOccurFault();
bool TRAY_STK_IsExecutionDeviceAction();

void TRAY_STK_InformedStackerTrigger();
void TRAY_Device_ComponentControl(US aChainNo, US aLinkNo, EAction aAction);
void TRAY_InformedCompoComplete(US aLinkNo);
void TRAY_InformedCompoControlComplete(US aChainNo, US aLinkNo, EAction aAction);

void  TRAY_STK_SetPulse_FullStackRelease(UL   aPulse);
 UL    TRAY_STK_GetPulse();
 void	 TRAY_STK_StandbyFailCheckStart(void);
 void   TRAY_STK_StandbyFailCheckStop(void);


#endif // #ifndef TRAY_STK_Device_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
