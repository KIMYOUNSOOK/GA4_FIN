// File Name:	CTempData.h
// Description:	Header File of TempData
// Engineer:	Xxxx Xxxx
// Last Edit:	XX.XX.XX
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CTempData_h
#define CTempData_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Result ---
#define TEMP_RESULT_OK		0
#define TEMP_RESULT_NG		(-1)

// --- Temp Data Spec ---
typedef struct
{
	UL* address;
	UL initialValue;
	UC dataType;
} CTempDataSpec;

// --- TempData Member ---
typedef struct /*CTempData*/
{
	const CTempDataSpec* mpSpec;
} CTempData;

/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CTempData_Constructor(CTempData* pThis);

SC CTempData_Initialize(CTempData* pThis);

#endif // #ifndef CTempData_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
