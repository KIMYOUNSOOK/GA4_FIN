// File Name:	CDiagHFSI.h
// Description:	Header File of Diag HFSI
// Engineer:	Yuichi Hoshino
// Last Edit:	14.07.14
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CDiagHFSI_h
#define CDiagHFSI_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinDataType.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- HFSI Instance ---
typedef struct /*CDiagHFSI*/
{
	UC* address;
	UL min;
	UL max;
	//@@@
} CDiagHFSI;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CDiagHFSI_Constructor(CDiagHFSI* pThis);

US CDiagHFSI_GetChainNo(CDiagHFSI* pThis);
US CDiagHFSI_GetLinkNo(CDiagHFSI* pThis);
UL CDiagHFSI_GetValue(CDiagHFSI* pThis);
SL CDiagHFSI_GetSignedValue(CDiagHFSI* pThis);
SC CDiagHFSI_SetValue(CDiagHFSI* pThis, UL value);
SC CDiagHFSI_Increament(CDiagHFSI* pThis);
enum ECdiFinDataType CDiagHFSI_GetDataType(CDiagHFSI* pThis);
UL CDiagHFSI_GetMinValue(CDiagHFSI* pThis);
UL CDiagHFSI_GetMaxValue(CDiagHFSI* pThis);

void CDiagHFSI_Initialize(CDiagHFSI* pThis);
void CDiagHFSI_Reset(CDiagHFSI* pThis, UC exchangeReason);
UL CDiagHFSI_GetLastCount(CDiagHFSI* pThis, UC lastCount);
UC CDiagHFSI_GetExchangeReason(CDiagHFSI* pThis, UC lastCount);
void CDiagHFSI_SetLifeSpec(CDiagHFSI* pThis, UL spec);
UL CDiagHFSI_GetLifeSpec(CDiagHFSI* pThis);
UC const * CDiagHFSI_GetPartsName(CDiagHFSI* pThis);
UC CDiagHFSI_GetPartsNameSize(CDiagHFSI* pThis);


#endif // #ifndef CDiagHFSI_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
