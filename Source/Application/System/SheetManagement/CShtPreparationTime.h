// File Name:	CShtPreparationTime.h
// Description:	Header File of Preparation Time
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.20
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CShtPreparationTime_h
#define CShtPreparationTime_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ShtInitTimeInfo.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Preparation Time ---
typedef struct /*CShtPreparationTime*/
{
	US mPrepTime;
	US mShortenTime;
} CShtPreparationTime;

typedef enum /*EJamDynamicJamID*/
{
	// !!! Product Specific !!!
	PATH_ID_R,			//  IOT筺体の用紙搬出部フレームとIOT Fusing-Roll間パス長
	PATH_ID_E,	//  IOT筺体の用紙搬出部フレームからENTRANCE-ROLLまでの距離
	PATH_ID_K,	// ENTRANCE-ROLLからCompile-Exit Sensorまでの距離
	PATH_ID_I,			// 
	PATH_ID_NUM
} EJamDynamicJamID;

#define PATH_LENGTH_REF_2970 2970
#define FIN_PROCESS_SPEED_6000 6000
#define FIN_PROCESS_SPEED_8000 8000



/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CShtPreparationTime_Constructor(CShtPreparationTime* pThis);

#define CShtPreparationTime_GetPreparationTime(apThis) ((apThis)->mPrepTime)	// US (CShtPreparationTime*)
#define CShtPreparationTime_GetShortenTime(apThis) ((apThis)->mShortenTime)		// US (CShtPreparationTime*)
void CShtPreparationTime_Calculate(CShtPreparationTime* pThis, UC sheetNo, UC prevShtNo, ShtInitTimeInfo shtInitTimeInfo);
//US CShtPrepTime_Base_SheetPitch(UC sheetNo, UC prevShtNo);
//US CShtPrepTime_Base_SetPitch(UC sheetNo, UC prevShtNo);
US CShtPrepTime_Base_SheetPitch(UC sheetNo);
US CShtPrepTime_Base_SetPitch(UC sheetNo);


#endif // #ifndef CShtPreparationTime_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
