// File Name:	ConditionService.h
// Description:	Header File of Condition Service
// Engineer:	Yuichi Hoshino
// Last Edit:	14.02.04
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef ConditionService_h
#define ConditionService_h

////////////////////////////////////////////////////////////
// Include File
////////////////////////////////////////////////////////////

#include "FinisherCommon.h"
#include "ECvrCoverStatus.h"
#include "EJamZoneID.h"
#include "EJamZoneStatus.h"
#include "ECvrCoverID.h"

////////////////////////////////////////////////////////////
// Function Prototype Definition
////////////////////////////////////////////////////////////

EFinisherState SRVC_GetFinisherState(void);
EDeviceCondition SRVC_GetDeviceCondition(EDeviceID deviceID);
ECvrCoverStatus SRVC_GetCoverStatus(void);
ECvrCoverStatus SRVC_GetCoverStatus_ID(ECvrCoverID coverID); //V3.1.7 
EJamZoneStatus SRVC_GetJamZoneStatus(EJamZoneID zoneID);
US SRVC_GetDeviceStatus(EDeviceStatusNo statusNo);
bool SRVC_UpdateDeviceStatus(US statusValue, EDeviceStatusNo statusNo);


#endif // #ifndef ConditionService_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.12.04	| Yuichi Hoshino	| Created this file
// 14.12.05	| Yuichi Hoshino	| Release V0 (Revision 00)
//
