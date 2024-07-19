// File Name:	CMPL_EJCT_SCLMP_Device.h
// Description:	Header File of Set Clamp Device Controller
// Engineer:    KimYS
// Last Edit:	15.03.03
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


#if !defined(CMPL_EJCT_SCLMP_Device_h)
#define CMPL_EJCT_SCLMP_Device_h


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "ECMPL_EJCT_SCLMP_Action.h"



/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
UC CMPL_EJCT_SCLMP_GetEjectorHomeSensor();
UC  CMPL_EJCT_SCLMP_GetSetClampHomeSensor();
bool CMPL_EJCT_SCLMP_IsExecutiongDeviceAction();
bool CMPL_EJCT_SCLMP_IsOccurFault();
void CMPL_EJCT_SCLMP_Reset();
void CMPL_EJCT_SCLMP_DeviceStop();
void CMPL_EJCT_SCLMP_StopSheet(UC aSheetNo);
void CMPL_EJCT_SCLMP_StartAct(ECMPL_EJCT_SCLMP_Action aAction, UC aSheetNo);






#endif //!defined(CMPL_EJCT_ROLL_Device_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
