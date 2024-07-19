// File Name:	CShtInitializeManager.h
// Description:	Header File of Sheet Initialize
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.20
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CShtInitializeManager_h
#define CShtInitializeManager_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "EShtSheetEdge.h"
#include "EShtInitID.h"

#include "ShtInitTimeInfo.h"

#include "CSeqShtInitHandler.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Start Sheet Initialize Spec ---
typedef struct /*ShtInitializeStartSpec*/
{
	UC startLocation;					// EShtSheetLocationID
	UC startLocationEdge;				// EShtSheetEdge
	UC startEvent;						// EShtSheetEventID
	US startWaitTime;
} ShtInitializeStartSpec;

// --- Sheet Initialize Spec ---
typedef struct /*ShtInitializeSpec*/
{
	UC optionID;						// EOptionID
	UC ejectDepend;
	ShtInitializeStartSpec startSpec[SHT_EJECT_NUM];
} ShtInitializeSpec;

// --- Sheet Initialize Manager Spec ---
typedef struct /*ShtInitializeManagerSpec*/
{
	ShtInitializeSpec shtInitSpec[SHT_INIT_ID_NUM];
} ShtInitializeManagerSpec;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CShtInitializeManager_Constructor();

bool CShtInitializeManager_CheckPlan(UC sheetNo, EShtInitID shtInitID);
bool CShtInitializeManager_CheckComplete(UC sheetNo, EShtInitID shtInitID);
ShtInitTimeInfo CShtInitializeManager_DetectInitExecution(UC sheetNo, UC prevShtNo);
void CShtInitializeManager_Start();
void CShtInitializeManager_End();
void CShtInitializeManager_CycleDown();
void CShtInitializeManager_HardDown();

void CShtInitializeManager_SheetRequest(UC sheetNo, UC prevShtNo);
void CShtInitializeManager_SheetExecution(UC sheetNo);
void CShtInitializeManager_SheetDelivered(UC sheetNo);
void CShtInitializeManager_SheetCancel(UC sheetNo);
void CShtInitializeManager_SheetLocation(UC sheetNo, EShtSheetEdge edge, EShtSheetLocationID location);
void CShtInitializeManager_SheetEvent(UC sheetNo, EShtSheetEventID event);
void CShtInitializeManager_SheetAction(UC sheetNo, US eventID, UC moduleID);


#endif // #ifndef CShtInitializeManager_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
