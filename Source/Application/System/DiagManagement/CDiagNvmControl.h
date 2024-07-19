// File Name:	CDiagNvmControl.h
// Description:	Header File of Diag NVM Control
// Engineer:	Yuichi Hoshino
// Last Edit:	14.07.14
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CDiagNvmControl_h
#define CDiagNvmControl_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

//#include "ENvmID.h"
#include "CDiagNVM.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CDiagNvmControl_Constructor();

void CDiagNvmControl_Initialize();
void CDiagNvmControl_InitializeAll();
void CDiagNvmControl_AutoInitialize(US major, US minor);
void CDiagNvmControl_ReadArea(US offset, UC* data, UC size);
void CDiagNvmControl_WriteArea(US offset, UC* data, UC size);
//CDiagNVM* CDiagNvmControl_Get(ENvmID id);
CDiagNVM* CDiagNvmControl_GetDiag(US chain, US link);
US CDiagNvmControl_GetAreaSize();


#endif // #ifndef CDiagNvmControl_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
