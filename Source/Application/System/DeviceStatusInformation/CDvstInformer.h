// File Name:	CDvstInformer.h
// Description:	Header File of Device Status Informer
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.14
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CDvstInformer_h
#define CDvstInformer_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "CDvstDeviceStatus.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CDvstInformer_Constructor();

US CDvstInformer_GetDeviceStatusNo(US statusID);
CDvstDeviceStatus* CDvstInformer_GetDeviceStatus(US statusNo);
CDvstDeviceStatus* CDvstInformer_GetDeviceStatusInOrder(US statusNo);
EDeviceCondition CDvstInformer_GetDeviceCondtion(EDeviceID deviceID);
void CDvstInformer_UpdateDeviceCondition(US statusNo);


#endif // #ifndef CDvstInformer_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
