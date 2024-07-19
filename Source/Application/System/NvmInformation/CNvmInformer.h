// File Name:	CNvmInformer.h
// Description:	Header File of NVM Informer
// Engineer:	Xxxx Xxxx
// Last Edit:	XX.XX.XX
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CNvmInformer_h
#define CNvmInformer_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ENvmID.h"
#include "CNvmNVM.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CNvmInformer_Constructor();
void CNvmInformer_LoadNvm();
bool CNvmInformer_SaveNvm();
void CNvmInformer_FirstPowerOnInitialize();
void CNvmInformer_VersionUpInitialize(US previousMajor, US previousMinor);
void CNvmInformer_OutRangeInitialize();
void CNvmInformer_DiagInitialize();
CNvmNVM* CNvmInformer_Get(ENvmID id);
CNvmNVM* CNvmInformer_DiagGet(US chain, US link);
void CNvmInformer_ReadArea(US offset, UC* pData, UC size);
void CNvmInformer_WriteArea(US offset, UC* pData, UC size);
US CNvmInformer_GetAreaSize();
UC* CNvmInformer_GetAreaAddress();
void CNvmInformer_SetValueChange();

void CNvmInformer_SetMajorVersion(UC version);
void CNvmInformer_SetMinorVersion(UC version);
void CNvmInformer_SetPatchVersion(UC version);
 void CNvmInformer_SetIplVersion(UC version);

#endif // #ifndef CNvmInformer_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
