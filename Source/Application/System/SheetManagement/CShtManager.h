// File Name:	CShtManager.h
// Description:	Header File of Sheet Manager
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.20
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CShtManager_h
#define CShtManager_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinOutDest.h"
#include "EShtSheetLocationID.h"
#include "ECdiFinPurgePossibility.h"
#include "EShtPathSensorID.h"
#include "EShtSheetEdge.h"
#include "EShtFinisherSpeedID.h"
#include "ECdiFinOffsetPos.h"
#include "EShtOffsetFinish.h"
#include "ESysProcStopType.h"
#include "ECdiFinSetEndAct.h"
#include "ECvrCoverID.h"
#include "EShtSheetEventID.h"

#include "CShtSheet.h"
#include "CShtSheetAttribute.h"
#include "CShtPathSensor.h"

#include "CSeqShtInitHandler.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define SHT_LOC_ID_CNV_BIT(locationID)	(0x00000001 << locationID)


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Output Destination Information ---
typedef struct /*ShtOutDestInfo*/
{
	UC outDestID;						// Output Destination ID
	UC deviceID;						// Finisher Device ID
	UC deliveredLocation;				// Delivered Point(Sheet Location ID)
//	UC setFinishedLocation;				// Set Finished Point(Sheet Location ID) //V3.4.0 Merge
//	UC setFinishedEvent;				// Set Finished Point(Sheet Event ID)
} ShtOutDestInfo;

// --- Stop Sheet Specification of Device Status ---
typedef struct /*ShtStopSheetSpec*/
{
	UC outDest;							// Output Destination ID
	UC pathSnr;							// Path Sensor ID
	UC purge;							// Purge Possibility
} ShtStopSheetSpec;

// --- Sheet Manager Spec ---
typedef struct /*ShtManagerSpec*/
{
	UC purgeOutDestID;					// Output Destination ID of Purge Tray
} ShtManagerSpec;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CShtManager_Constructor();

UC CShtManager_GetProcessStartSheet();
enum ECdiFinPurgePossibility CShtManager_GetPurgePossibility();
UC CShtManager_GetTopRequestSheet();
UC CShtManager_GetBottomRequestSheet();
UC CShtManager_GetPrevRequestSheet(UC sheetNo);
UC CShtManager_GetNextRequestSheet(UC sheetNo);
UC CShtManager_GetTopExitSheet();
UC CShtManager_GetBottomExitSheet();
UC CShtManager_GetPrevExitSheet(UC sheetNo);
UC CShtManager_GetNextExitSheet(UC sheetNo);
UC CShtManager_GetBufferPairSheet(UC sheetNo);
bool CShtManager_CheckUndeliveredSheetExist(UC outDest);
bool CShtManager_CheckUnejectedSheetExist(UC outDest);
bool CShtManager_CheckUnfinishedSetExist(UC outDest);
bool CShtManager_CheckUnfinishedSetExist_2(UC outDest);  //V3.1.4 

UC CShtManager_GetNextArriveSheet(EShtPathSensorID pathSnrID, EShtSheetEdge edge);
bool CShtManager_CheckPathSensorPlan(EShtPathSensorID pathSnrID, UC sheetNo);
bool CShtManager_CheckPathSensorArrival(EShtPathSensorID pathSnrID, UC sheetNo, EShtSheetEdge edge);
UC CShtManager_GetSheetNo(UC sheetID);
const CShtSheet* CShtManager_GetSheet(UC sheetNo);
US CShtManager_ConvFinisherSpeed(EShtFinisherSpeedID speedID);
enum ECdiFinOffsetPos CShtManager_ConvOffsetPosition(EShtOffsetFinish ofstFin);
void CShtManager_ProcessStart();
void CShtManager_ProcessEnd();
void CShtManager_StopProcess(ESysProcStopType stopType);
void CShtManager_CycleDownFaultStop(US devSttsNo);
void CShtManager_HardDownFaultStop(US devSttsNo);
void CShtManager_DynamicJamStop(US devSttsNo, UC jamShtNo);
UC CShtManager_CreateProcessStartSheet(UC outDest, UC binSlct, CShtSheetAttribute* pShtAttr);
UC CShtManager_CreateSheet(UC sheetID, UC setID, UC outDest, UC binSlct, CShtSheetAttribute* pShtAttr);
void CShtManager_UpdateSheetAttribute(UC sheetID, CShtSheetAttribute* pShtAttr);
bool CShtManager_CancelSheet(UC sheetID);
void CShtManager_SetEnd(enum ECdiFinSetEndAct setEndAct);
void CShtManager_ExecuteSheet(UC sheetID);
void CShtManager_SheetEnd();
bool CShtManager_AbortCompileSheetProcessEnd();
bool CShtManager_AbortCompileSheetCoverClose(ECvrCoverID coverID);
bool CShtManager_AbortCompileSheetDeviceModuleNG(UC moduleID);
void CShtManager_StandbyEject();
void CShtManager_SendSheetActionRequest(US eventID, UC sheetNo);
void CShtManager_ReceiveSheetActionResponse(UC moduleID, US eventID, UC sheetNo);
void CShtManager_ReceiveSheetEventNotify(UC moduleID, US eventID, EShtSheetEventID event, UC sheetNo);
void CShtManager_PathSensorEdgeChange(EShtPathSensorID pathSnrID, EShtSheetEdge edge);
void CShtManager_PathSensorLateDetect(CShtPathSensor *pPathSnr, EShtSheetEdge edge);
void PutSheetManagerLog(UC para1, UC para2, US para3, US para4);
void CShtManager_SetPitchSkip(UC  bOnOff);
UC CShtManager_GetPitchSkip(void);


#endif // #ifndef CShtManager_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
