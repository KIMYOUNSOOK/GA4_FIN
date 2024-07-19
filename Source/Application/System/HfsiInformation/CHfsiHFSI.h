// File Name:	CHfsiHFSI.h
// Description:	Header File of HFSI
// Engineer:	Xxxxxx Xxxxxx
// Last Edit:	XX.XX.XX
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CHfsiHFSI_h
#define CHfsiHFSI_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"
#include "ECdiFinDataType.h"
#include "HfsiRam.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- HFSI Result ---
#define HFSI_RESULT_OK			0			// HFSI OK
#define HFSI_RESULT_NG			(-1)		// HFSI NG

// --- HFSI Spec ---
typedef struct /*CHfsiHfsiSpec*/
{
	HFSIRamData* address;
	US chain;
	US link;	
	UC partsName[21];
	UC partsNameSize;
	UL threshold;
	// 必要に応じて追加削除
} CHfsiHfsiSpec;

// --- HFSI Member ---
typedef struct /*CHfsiHFSI*/
{
	const CHfsiHfsiSpec* mpSpec;
	// 必要に応じて追加削除
} CHfsiHFSI;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CHfsiHFSI_Constructor(CHfsiHFSI* pThis);

US CHfsiHFSI_GetChainNo(CHfsiHFSI* pThis);
US CHfsiHFSI_GetLinkNo(CHfsiHFSI* pThis);
UL CHfsiHFSI_GetValue(CHfsiHFSI* pThis);
SL CHfsiHFSI_GetSignedValue(CHfsiHFSI* pThis);
SC CHfsiHFSI_SetValue(CHfsiHFSI* pThis, UL value);
SC CHfsiHFSI_Increment(CHfsiHFSI* pThis);
enum ECdiFinDataType CHfsiHFSI_GetDataType(CHfsiHFSI* pThis);
UL CHfsiHFSI_GetMinValue(CHfsiHFSI* pThis);
UL CHfsiHFSI_GetMaxValue(CHfsiHFSI* pThis);
void CHfsiHFSI_Initialize(CHfsiHFSI* pThis);
void CHfsiHFSI_InitializeLifeSpec(CHfsiHFSI* pThis);
void CHfsiHFSI_Reset(CHfsiHFSI* pThis, UC exchangeReason);
UL CHfsiHFSI_GetLastCount(CHfsiHFSI* pThis, UC lastCount);
UC CHfsiHFSI_GetExchangeReason(CHfsiHFSI* pThis, UC lastCount);
void CHfsiHFSI_SetLifeSpec(CHfsiHFSI* pThis, UL spec);
UL CHfsiHFSI_GetLifeSpec(CHfsiHFSI* pThis);
UC const* CHfsiHFSI_GetPartsName(CHfsiHFSI* pThis);
UC CHfsiHFSI_GetPartsNameSize(CHfsiHFSI* pThis);


#endif // #ifndef CHfsiHFSI_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 15.03.30	| Makoto Tanabe		| Created this file
// 15.03.30	| Makoto Tanabe		| Release V1 (Revision 00)
// 15.04.02	| Makoto Tanabe		| Design Improvement [ スペル間違い(CHfsiHFSI_Increment)修正 ]
// 15.XX.XX	| Makoto Tanabe		| Release V2 (Revision 01)
//
