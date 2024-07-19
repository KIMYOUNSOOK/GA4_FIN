// File Name:	ShtInitTimeInfo.h
// Description:	Implementation of the Enumeration ShtInitTimeInfo
// Engineer:	Xxxxxx Xxxxxx
// Last Edit:	XX.XX.XX
// Revision:	XX
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef ShtInitTimeInfo_h
#define ShtInitTimeInfo_h

#include "EShtInitID.h"

// --- Sheet Initialize Execute Detection Information ---
typedef struct /*ShtInitTimeInfo*/
{
	US sheetInitTime[SHT_INIT_ID_NUM];		// Sheet Initialize Action Time
	US waitTime;							// Request Wait Time
} ShtInitTimeInfo;

#endif // #ifndef ShtInitTimeInfo_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
