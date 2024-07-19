// File Name:	CShtPathSensor.h
// Description:	Header File of Path Sensor
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.20
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CShtPathSensor_h
#define CShtPathSensor_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "EShtSheetEdge.h"
#include "EShtReverseKind.h"

#include "LIB_StopWatch.h"

#include "DD_DIO.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Path Sensor Level ---
typedef enum /*EShtPathSnrLevel*/
{
	SHT_PATH_SNR_LVL_OFF = 0,			// Path sensor level is "OFF".
	SHT_PATH_SNR_LVL_ON = 1				// Path sensor level is "ON".
} EShtPathSnrLevel;


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Spec Data of Path Sensor ---
typedef struct /*ShtPathSensorSpec*/
{
	UC inputID;
	UC filterTime[SHT_EDGE_NUM];
	UC reverseFilterBit;
	UC lateDetectWaitTime[SHT_EDGE_NUM];
	UC lateDetectErrorCheckTimes[SHT_EDGE_NUM];
	UC bufferPath:1;
	UC compilePath:1;
	UC pileTransPath:1;
} ShtPathSensorSpec;

// --- Path Sensor ---
typedef struct /*CShtPathSensor*/
{
	const ShtPathSensorSpec* mpSpec;
	UC mNextEdgeSheetNo[SHT_EDGE_NUM];
	UC mWaitEdge;
	UC mReverseFilterBit;
	UC mLateDetectStep;
	UC mLateDetectErrorCount;
	SS mLateDetectTimerID;
	CStopWatch mStopWatch;
} CShtPathSensor;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CShtPathSensor_Constructor(CShtPathSensor* pThis, const ShtPathSensorSpec* pSpec);

bool CShtPathSensor_IsRevereseFilterPath(CShtPathSensor* pThis, EShtReverseKind reverseKind);
bool CShtPathSensor_IsLateDetectFilterPath(CShtPathSensor* pThis, EShtSheetEdge edge);
#define CShtPathSensor_IsBufferPath(apThis) ((apThis)->mpSpec->bufferPath)		// bool (CShtPathSensor*)
#define CShtPathSensor_IsCompilePath(apThis) ((apThis)->mpSpec->compilePath)	// bool (CShtPathSensor*)
#define CShtPathSensor_IsCompileTransPath(apThis) ((apThis)->mpSpec->pileTransPath)
																				// bool (CShtPathSensor*)
EShtPathSnrLevel CShtPathSensor_GetLevel(CShtPathSensor* pThis);
EShtPathSnrLevel CShtPathSensor_GetActualLevel(CShtPathSensor* pThis);
#define CShtPathSensor_GetEntrySheet(apThis, aEdge) ((apThis)->mNextEdgeSheetNo[aEdge])
																				// UC (CShtPathSensor*, EShtSheetEdge)
UC CShtPathSensor_GetLateDetectFilterTime(CShtPathSensor* pThis, UC edge);
void CShtPathSensor_Reset(CShtPathSensor* pThis);
void CShtPathSensor_EntrySheet(CShtPathSensor* pThis, UC sheetNo, EShtSheetEdge edge);
void CShtPathSensor_FilterReverse(CShtPathSensor* pThis, EAction action, EShtReverseKind reverseKind);
UC CShtPathSensor_InformedEdge(CShtPathSensor* pThis, EShtSheetEdge edge);
void CShtPathSensor_InformedLateDetectTimeUp(CShtPathSensor* pThis, SS timerID);


#endif // #ifndef CShtPathSensor_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
