// File Name:	CNvmNVM.h
// Description:	Header File of NVM
// Engineer:	Xxxx Xxxx
// Last Edit:	XX.XX.XX
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CNvmNVM_h
#define CNvmNVM_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinDataType.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- NVM Result ---
#define NVM_RESULT_OK			0			// NVM OK
#define NVM_RESULT_NG			(-1)		// NVM NG

// --- NVM Spec ---
typedef struct /*CNvmNvmSpec*/
{
	UC* address;
	US chain;
	US link;	
	UL minValue;
	UL maxValue;
	UL initialValueFx;
	UL initialValueXc;
	UL initialValueAp;
	UL initialValueXe;
	UL initialValueSa;
	UC initAllowed:1;
	UC writeAllowed:1;
	UC readAllowed:1;
	UC dataType;
	// 必要に応じて追加削除
} CNvmNvmSpec;

// --- NVM Member ---
typedef struct /*CNvmNVM*/
{
	const CNvmNvmSpec* mpSpec;
	// 必要に応じて追加削除
} CNvmNVM;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CNvmNVM_Constructor(CNvmNVM* pThis);

UL CNvmNVM_GetValue(CNvmNVM* pThis);
SL CNvmNVM_GetSignedValue(CNvmNVM* pThis);
SC CNvmNVM_SetValue(CNvmNVM* pThis, UL value);
SC CNvmNVM_Increment(CNvmNVM* pThis);
SC CNvmNVM_Initialize(CNvmNVM* pThis);
enum ECdiFinDataType CNvmNVM_GetDataType(CNvmNVM* pThis);
UL CNvmNVM_GetMinValue(CNvmNVM* pThis);
UL CNvmNVM_GetMaxValue(CNvmNVM* pThis);
UC CNvmNVM_IsReadAllowed(CNvmNVM* pThis);
UC CNvmNVM_IsWriteAllowed(CNvmNVM* pThis);
UC CNvmNVM_IsFactoryInitAllowed(CNvmNVM* pThis);
bool CNvmNVM_IsOutRange(CNvmNVM* pThis);

#endif // #ifndef CNvmNVM_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
