// File Name:	CCdiFinCfgAttrDevCont40.h
// Description:	Header File of Finisher Device Controller Configuration Attributes "Set Date Time(40H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCfgAttrDevCont40_h)
#define CCdiFinCfgAttrDevCont40_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyDevCont.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CFG_ATTR_DEV_CONT_40_SIZE				8


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCfgAttrDevCont40_GetAttributeKey() ((UC)eCdiFinCfgKeyDevCont_DateTime)
																				// UC (void)
#define CCdiFinCfgAttrDevCont40_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CFG_ATTR_DEV_CONT_40_SIZE)
																				// UC (UC*)
#define CCdiFinCfgAttrDevCont40_GetYear(apAttrPair) ( ((US)((apAttrPair)[1]) << 8) | (US)((apAttrPair)[2]) )
																				// US (UC*)
#define CCdiFinCfgAttrDevCont40_SetYear(apAttrPair, aYear) (apAttrPair)[1] = (UC)((US)(aYear) >> 8); (apAttrPair)[2] = (UC)((US)(aYear))
																				// void (UC*, US)
#define CCdiFinCfgAttrDevCont40_GetMonth(apAttrPair) (apAttrPair[3])			// UC (UC*)
#define CCdiFinCfgAttrDevCont40_SetMonth(apAttrPair, aMonth) (apAttrPair[3] = aMonth)
																				// void (UC*, UC)
#define CCdiFinCfgAttrDevCont40_GetDay(apAttrPair) (apAttrPair[4])				// UC (UC*)
#define CCdiFinCfgAttrDevCont40_SetDay(apAttrPair, aDay) (apAttrPair[4] = aDay)	// void (UC*, UC)
#define CCdiFinCfgAttrDevCont40_GetHour(apAttrPair) (apAttrPair[5])				// UC (UC*)
#define CCdiFinCfgAttrDevCont40_SetHour(apAttrPair, aHour) (apAttrPair[5] = aHour)
																				// void (UC*, UC)
#define CCdiFinCfgAttrDevCont40_GetMinute(apAttrPair) (apAttrPair[6])			// UC (UC*)
#define CCdiFinCfgAttrDevCont40_SetMinute(apAttrPair, aMinute) (apAttrPair[6] = aMinute)
																				// void (UC*, UC)
#define CCdiFinCfgAttrDevCont40_GetSecond(apAttrPair) (apAttrPair[7])			// UC (UC*)
#define CCdiFinCfgAttrDevCont40_SetSecond(apAttrPair, aSecond) 	(apAttrPair[7] = aSecond)
																				// void (UC*, UC)


#endif // !defined(CCdiFinCfgAttrDevCont40_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
