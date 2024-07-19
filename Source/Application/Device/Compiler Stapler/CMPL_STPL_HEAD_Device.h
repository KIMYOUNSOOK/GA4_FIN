// File Name:	CMPL_STPL_HEAD_Device.h
// Description:	Header File of Compile Staple Device Controller
// Engineer:	Takahiko Minaga
// Last Edit:	14.09.01
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


#if !defined(CMPL_STPL_HEAD_Device_h)
#define CMPL_STPL_HEAD_Device_h


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "ECMPL_STPL_HEAD_Action.h"


#define STPL_RISE		(1)
#define STPL_FALL		(0)

#define STPL_FORWARD		(1)		// CW
#define STPL_REVERSE		(0)		// CCW

#define DUTY_100		100
#define DUTY_70			70

/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
bool CMPL_STPL_HEAD_IsExecutiongDeviceAction();
bool CMPL_STPL_HEAD_IsOccurFault();
void CMPL_STPL_HEAD_AllCoverClose();
void CMPL_STPL_HEAD_Reset();
void CMPL_STPL_HEAD_DeviceStop();
void CMPL_STPL_HEAD_StopSheet(aSheetNo);
void CMPL_STPL_HEAD_StartAct(ECMPL_STPL_HEAD_Action aAction, UC aSheetNo);









#endif //!defined(CMPL_STPL_HEAD_Device_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
