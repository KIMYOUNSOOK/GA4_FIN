// File Name:	CNmaHandler.h
// Description:	Header File of NVM Access Handler
// Engineer:	Yuichi Hoshino
// Last Edit:	16.02.26
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CNmaHandler_h
#define CNmaHandler_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- NVM Data Informer ID ---
typedef enum /*ENmaInformerID*/
{
	NMA_INFORMER_KEY,					// Key Data Informer
	NMA_INFORMER_NVM,					// NVM Informer
	NMA_INFORMER_HFSI,					// HFSI Informer
	// NMA_INFORMER_TEMP,					// Temporary Data Informer	ÅyROMçÌå∏Åz
	NMA_INFORMER_NUM					// Number of NVM Data Informer
} ENmaInformerID;


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CNmaHandler_Constructor();

void CNmaHandler_Load();
bool CNmaHandler_Save();
void CNmaHandler_LoadNvmComplete(ENmaInformerID informerID, SS result);
void CNmaHandler_SaveNvmComplete(ENmaInformerID informerID, SS result);


#endif // #ifndef CNmaHandler_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 16.02.26	| Yuichi Hoshino	| Created this file
// 16.02.26	| Yuichi Hoshino	| Introduction Spec [ Download ]
// 16.03.01	| Yuichi Hoshino	| Release Revision 00 to GA-Finisher V1.20.0 Make.
//
