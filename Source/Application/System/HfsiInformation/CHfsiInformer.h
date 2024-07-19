// File Name:	CHfsiInformer.h
// Description:	Header File of HFSI Informer
// Engineer:	Xxxx Xxxx
// Last Edit:	XX.XX.XX
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CHfsiInformer_h
#define CHfsiInformer_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "EHfsiItemID.h"
#include "CHfsiHFSI.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CHfsiInformer_Constructor();
void CHfsiInformer_LoadNvm();
bool CHfsiInformer_SaveNvm();
void CHfsiInformer_FirstPowerOnInitialize();
void CHfsiInformer_VersionUpInitialize(US previousMajor, US previousMinor);
void CHfsiInformer_DiagInitialize();
CHfsiHFSI* CHfsiInformer_Get(EHfsiItemID id);
CHfsiHFSI* CHfsiInformer_DiagGet(US chain, US link);
CHfsiHFSI* CHfsiInformer_ResetIterate();
CHfsiHFSI* CHfsiInformer_NextIterate();

void CHfsiInformer_SetValueChange();
US CHfsiInformer_GetAreaSize();
UC* CHfsiInformer_GetAreaAddress();
void CHfsiInformer_AutoInitialize(US previousMajor, US previousMinor);

#endif // #ifndef CHfsiInformer_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
