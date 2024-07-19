// File Name:	CJamChecker.h
// Description:	Header File of Jam Checker
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CJamChecker_h
#define CJamChecker_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "DD_DIO.h"

#include "EJamDynamicJamID.h"
#include "EJamZoneStatus.h"
#include "EJamZoneID.h"
#include "ECvrCoverID.h"
#include "EShtPathSensorID.h"
#include "EShtSheetEdge.h"
#include "EShtSheetLocationID.h"
#include "EShtSheetEventID.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define JAM_ZONE_ID_CNV_BIT(id)		(UL)(0x00000001 << id)
#define JAM_PATH_SNR_ID_CNV_BIT(id) (UC)(0x01 << id)
#define JAM_CVR_ID_CNV_BIT(id)		(UC)(0x01 << id)

// --- Spec data structure of set Jam Zone from path sensor conditionn ---
typedef struct /*JamZoneSetSpecPathSensor*/
{
	UC zoneID;							// Jam Zone ID
	UC pathSensorBit;					// Checked Path Sensor ID(Bit)
	UC compileCheckPathBit;				// Compile Sheet Exist Check Path Sensor ID(Bit)
	UC compileCheckOutDest;				// Compile Sheet Exist Check Output Destination ID
} JamZoneSetSpecPathSensor;

// --- Spec data structure of set Jam Zone from sheet location ---
typedef struct /*JamZoneSetSpecSheetLocation*/
{
	UC zoneID;							// Jam Zone ID
	UC locationID;						// Sheet Location ID
} JamZoneSetSpecSheetLocation;

// --- Spec data structure of clear Jam Zone from cover close ---
typedef struct /*JamZoneClearSpecCover*/
{
	UC zoneID;							// Jam Zone ID
	UC pathSensorBit;					// Checked Path Sensor ID(Bit)
	UC compileCheckPathBit;				// Compile Sheet Exist Check Path Sensor ID(Bit)
	UC compileCheckOutDest;				// Compile Sheet Exist Check Output Destination ID
	UC coverIdBit;						// Cover ID(Bit)
} JamZoneClearSpecCover;

// --- Spec data structure of clear Jam Zone from entrance path enviroment change ---
typedef struct /*JamZoneClearSpecEntrance*/
{
	UC zoneID;							// Jam Zone ID
	UC pathSensorBit;					// Checked Path Sensor ID
} JamZoneClearSpecEntrance;

// --- Spec data structure of clear Jam Zone from cyclic check ---
typedef struct /*JamZoneClearSpecCyclic*/
{
	UC zoneID;							// Jam Zone ID
	UC pathSensorBit;					// Checked Path Sensor Input ID(Bit)
	UC compileCheckPathBit;				// Compile Sheet Exist Check Path Sensor ID(Bit)
	UC compileCheckOutDest;				// Compile Sheet Exist Check Output Destination ID
	UC clearCount;						// Clear Count Threshold
} JamZoneClearSpecCyclic;

// --- Spec data structure of select Dyanamic Jam ----
typedef struct /*DJamSelectSpec*/
{
	UC jamID;							// Dynamic Jam ID
	UC nextLocID;						// Next Location ID
	UC planLocID;						// Plan Location ID
} DJamSelectSpec;

// --- Spec data structure of detect Dyanamic Jam ---
typedef struct /*DJamDetectSpec*/
{
	UC locationID;						// Sheet Location ID
	UC edge;							// Sheet Edge
	US punchAddTime;					// Add Time for Punch Action
	UL jamZoneBit;						// Set Jam Zone(Bit)
} DJamDetectSpec;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CJamChecker_Constructor();

EFaultStatus CJamChecker_GetDynamicJamStatus(EJamDynamicJamID jamID);
EJamZoneStatus CJamChecker_GetJamZoneStatus(EJamZoneID zoneID);
void CJamChecker_EndCheck();
void CJamChecker_DiscardOption(EOptionID optionID);
void CJamChecker_PowerOn();
void CJamChecker_CoverClose(ECvrCoverID coverID);
void CJamChecker_DeviceInitializeComplete();
void CJamChecker_FinishingProcessEnd();
void CJamChecker_AllSheetFinishComplete();
void CJamChecker_ChangeNormal();
void CJamChecker_DropEjectSheet();
void CJamChecker_EntranceEnvironmentChange();
void CJamChecker_CyclicCheck();
void CJamChecker_SheetLocation(UC sheetNo, EShtSheetEdge edge, EShtSheetLocationID location);
void CJamChecker_SheetEvent(UC sheetNo, EShtSheetEventID event);
void CJamChecker_JamOccurrence(US statusNo, UC sheetNo);
void CJamChecker_JamTimerTimeUp(EJamDynamicJamID jamID, UC sheetNo);

// extern CJamChecker_GetJamStatus(void); 
#endif // #ifndef CJamChecker_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.04.09	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ Power Down後ステータス誤通知防止 ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
