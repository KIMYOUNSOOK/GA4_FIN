// File Name:	CMPL_TMP_RIGHT_Device.h
// Description:	Header File of Compile Right Tamper Device
// Engineer:	FXK
// Last Edit:	15.03.12
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


#if !defined(CMPL_TMP_RIGHT_Device_h)
#define CMPL_TMP_RIGHT_Device_h


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "ECMPL_TMP_Action.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CMPL_TMP_RIGHT_InformedEjectStart();
void CMPL_TMP_RIGHT_InformedEjectSheetThrough();
bool CMPL_TMP_RIGHT_IsExecutiongDeviceAction();
bool CMPL_TMP_RIGHT_IsOccurFault();
UC CMPL_TMP_RIGHT_GetControlSheetNo();
US CMPL_TMP_RIGHT_GetTamperPosition(UC aSheetNo);

void CMPL_TMP_RIGHT_Reset();
void CMPL_TMP_RIGHT_DeviceStop();
void CMPL_TMP_RIGHT_StartAct(ECMPL_TMP_Action aAction, UC aSheetNo);





#endif //!defined(CMPL_TMP_REAR_Device_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
