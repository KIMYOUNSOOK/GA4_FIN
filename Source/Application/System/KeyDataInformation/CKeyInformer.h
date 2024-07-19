// File Name:	CKeyInformer.h
// Description:	Header File of Key Data Informer
// Engineer:	Yuichi Hoshino
// Last Edit:	16.02.09
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CKeyInformer_h
#define CKeyInformer_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "DD_NVM.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- KEY Data Change ---
#define KEY_NO_CHANGE		0			// •Ï‰»‚È‚µ
#define KEY_CHANGE			1			// •Ï‰»‚ ‚è


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CKeyInformer_Constructor();

void CKeyInformer_LoadNvm();
bool CKeyInformer_SaveNvm();
bool CKeyInformer_CheckKeyCode();
void CKeyInformer_SetKeyCode();
void CKeyInformer_ClearKeyCode();
bool CKeyInformer_CheckDownloadMode();
void CKeyInformer_SetDownloadMode();
void CKeyInformer_ClearDownloadMode();
void CKeyInformer_SetMajorVersion(UC version);
UC CKeyInformer_GetMajorVersion();
void CKeyInformer_SetMinorVersion(UC version);
UC CKeyInformer_GetMinorVersion();
void CKeyInformer_SetPatchVersion(UC version);
UC CKeyInformer_GetPatchVersion();
UL CKeyInformer_GetWriteCount();
void CKeyInformer_SetRxSequenceBit(UC seqBit);
UC CKeyInformer_GetRxSequenceBit();
void CKeyInformer_SetTxSequenceBit(UC seqBit);
UC CKeyInformer_GetTxSequenceBit();
void CKeyInformer_SetIplVersion(UC version);
UC CKeyInformer_GetIplVersion();


#endif // #ifndef CKeyInformer_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 15.03.31	| Yuichi Hoshino	| Created this file based on Base SW V1
// 15.06.19	| Yuichi Hoshino	| Release Revision 00 to GA-Finisher V0.1.0 Make.
// 16.02.09	| Yuichi Hoshino	| Introduction Spec [ Download ]
// 16.03.01	| Yuichi Hoshino	| Release Revision 01 to GA-Finisher V1.20.0 Make.
//
