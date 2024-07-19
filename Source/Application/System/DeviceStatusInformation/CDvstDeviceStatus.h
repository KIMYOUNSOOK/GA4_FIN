// File Name:	CDvstDeviceStatus.h
// Description:	Header File of Device Status
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.14
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CDvstDeviceStatus_h
#define CDvstDeviceStatus_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "EDvstCategory.h"
#include "ESysProcStopType.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Status Value Info ---
typedef struct /*DvstStatusValueInfo*/
{
	US chain;
	US link;
} DvstStatusValueInfo;

// --- Device Status Spec ---
typedef struct /*DvstDeviceStatusSpec*/
{
	US id;
	US defaultStts;
	US ngThresh;
	UC category;
	UL deviceBit;
	UC subNo;
	UC specialProcess;
	UC stopType;
	const DvstStatusValueInfo* pValueInfoTbl;
	US valueInfoTblSize;
} DvstDeviceStatusSpec;

// --- Device Status Member ---
typedef struct /*CDvstDeviceStatus*/
{
	const DvstDeviceStatusSpec* mpSpec;
	US *mpStatusValue;
} CDvstDeviceStatus;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CDvstDeviceStatus_Constructor(CDvstDeviceStatus* pThis);

US CDvstDeviceStatus_GetStatusID(CDvstDeviceStatus* pThis);
US CDvstDeviceStatus_GetStatusValue(CDvstDeviceStatus* pThis);
bool CDvstDeviceStatus_SetStatusValue(CDvstDeviceStatus* pThis, US statusValue);
enum EDvstCategory CDvstDeviceStatus_GetStatusCategory(CDvstDeviceStatus* pThis);
bool CDvstDeviceStatus_CheckRelativeDevice(CDvstDeviceStatus* pThis, EDeviceID deviceID);
UC CDvstDeviceStatus_GetSubNo(CDvstDeviceStatus* pThis);
US CDvstDeviceStatus_GetDefaultStatus(CDvstDeviceStatus* pThis);
bool CDvstDeviceStatus_IsDefaultStatus(CDvstDeviceStatus* pThis, US statusValue);
US CDvstDeviceStatus_GetChainNo(CDvstDeviceStatus* pThis, US statusValue);
US CDvstDeviceStatus_GetLinkNo(CDvstDeviceStatus* pThis, US statusValue);
bool CDvstDeviceStatus_CheckDeviceCondition(CDvstDeviceStatus* pThis, US statusValue);
ESysProcStopType CDvstDeviceStatus_GetFinProcessStopType(CDvstDeviceStatus* pThis);
bool CDvstDeviceStatus_GetSpecialProcessExecution(CDvstDeviceStatus* pThis);


#endif // #ifndef CDvstDeviceStatus_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
