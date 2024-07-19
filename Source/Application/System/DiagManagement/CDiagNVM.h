// File Name:	CDiagNVM.h
// Description:	Header File of Diag NVM
// Engineer:	Yuichi Hoshino
// Last Edit:	14.07.14
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CDiagNVM_h
#define CDiagNVM_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinDataType.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- NVM Instance ---
typedef struct /*CDiagNVM*/
{
	UC* address;
	UL min;
	UL max;
	//@@@
} CDiagNVM;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CDiagNVM_Constructor(CDiagNVM* pThis);

UL CDiagNVM_GetValue(CDiagNVM* pThis);
SL CDiagNVM_GetSignedValue(CDiagNVM* pThis);
SC CDiagNVM_SetValue(CDiagNVM* pThis, UL value);
SC CDiagNVM_Increament(CDiagNVM* pThis);
SC CDiagNVM_Initialize(CDiagNVM* pThis);
enum ECdiFinDataType CDiagNVM_GetDataType(CDiagNVM* pThis);
UL CDiagNVM_GetMinValue(CDiagNVM* pThis);
UL CDiagNVM_GetMaxValue(CDiagNVM* pThis);
UC CDiagNVM_IsReadAllowed(CDiagNVM* pThis);
UC CDiagNVM_IsWriteAllowed(CDiagNVM* pThis);
UC CDiagNVM_IsFactoryInitAllowed(CDiagNVM* pThis);


#endif // #ifndef CDiagNVM_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
