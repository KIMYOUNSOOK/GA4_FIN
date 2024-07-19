// File Name:	CMPL_TMP_LEFT_Device.h
// Description:	Header File of Compile Left Tamper Device Controller
// Engineer:	FXK
// Last Edit:	15.03.13
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


#if !defined(CMPL_TMP_LEFT_Device_h)
#define CMPL_TMP_LEFT_Device_h


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "ECMPL_TMP_Action.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_InformedEjectSheetThrough();
void CMPL_TMP_LEFT_InformedEjectStart();

bool CMPL_TMP_LEFT_IsExecutiongDeviceAction();
bool CMPL_TMP_LEFT_IsOccurFault();
UC CMPL_TMP_LEFT_GetControlSheetNo();
US CMPL_TMP_LEFT_GetTamperPosition(UC aSheetNo);
void CMPL_TMP_LEFT_Reset();
void CMPL_TMP_LEFT_DeviceStop();
//US CMPL_TMP_LEFT_GetTampingPushTime(UC aSheetNo);
//US CMPL_TMP_LEFT_GetTampingReturnTime(UC aSheetNo);
US   CMPL_TMP_LEFT_GetNormalTampingTime(UC  aSheetNo);  //V3.0.0 (PreDVT)
US   CMPL_TMP_LEFT_GetEjectTampingTime(UC  aSheetNo);

void CMPL_TMP_LEFT_StartAct(ECMPL_TMP_Action aAction, UC aSheetNo);
void CMPL_TMP_LEFT_PreStartAct(ECMPL_TMP_Action aAction, UC aSheetNo);






#endif //!defined(CMPL_TMP_LEFT_Device_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
