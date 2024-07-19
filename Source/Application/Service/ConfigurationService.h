// File Name:	ConfigurationService.h
// Description:	Header File of Configuration Service
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.07
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef ConfigurationService_h
#define ConfigurationService_h

////////////////////////////////////////////////////////////
// Include File
////////////////////////////////////////////////////////////

#include "FinisherCommon.h"


////////////////////////////////////////////////////////////
// Function Prototype Definition
////////////////////////////////////////////////////////////

bool SRVC_CheckOptionDetection(EOptionID optionID);
UC SRVC_GetOptionType(EOptionID optionID);
US SRVC_GetPowerUpYear(void);
UC SRVC_GetPowerUpMonth(void);
UC SRVC_GetPowerUpDay(void);
UC SRVC_GetPowerUpHour(void);
UC SRVC_GetPowerUpMinute(void);
UC SRVC_GetPowerUpSecond(void);
US SRVC_GetHotLineDistance(void);
US SRVC_GetReleasePointDistance(void);
UC SRVC_GetProductivity(void);


#endif // #ifndef ConfigurationService_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.12.05	| Yuichi Hoshino	| Created this file
// 14.12.05	| Yuichi Hoshino	| Release V0 (Revision 00)
// 14.12.29	| Yuichi Hoshino	| Introduction Spec [ 電源ON時刻取得サービス追加 ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
