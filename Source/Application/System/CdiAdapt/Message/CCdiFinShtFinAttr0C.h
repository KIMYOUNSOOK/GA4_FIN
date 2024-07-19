// File Name:	CCdiFinShtFinAttr0C.h
// Description:	Header File of Sheet Finish Attributes "Exit Speed(0CH)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.13
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinShtFinAttr0C_h)
#define CCdiFinShtFinAttr0C_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinShtFinAttrKey.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_SHT_FIN_ATTR_0C_BASE_SIZE					4
#define CDIFIN_SHT_FIN_ATTR_0C_TIMES_SET_SIZE				4


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinShtFinAttr0C_GetAttributeKey() ((UC)eCdiFinShtFinAttr_ExitSpd)	// UC (void)
#define CCdiFinShtFinAttr0C_GetAttributeSize(apAttrPair) ((UC)CDIFIN_SHT_FIN_ATTR_0C_BASE_SIZE + CDIFIN_SHT_FIN_ATTR_0C_TIMES_SET_SIZE * apAttrPair[3])
																				// UC (UC*)
#define CCdiFinShtFinAttr0C_GetInitialSpeed(apAttrPair) ( ((US)((apAttrPair)[1]) <<  8) | (US)((apAttrPair)[2]) )
																				// US (UC*)
#define CCdiFinShtFinAttr0C_SetInitialSpeed(apAttrPair, aSpeed) (apAttrPair)[1] = (UC)((US)aSpeed >> 8); (apAttrPair)[2] = (UC)((US)aSpeed)
																				// void (UC*, US)
#define CCdiFinShtFinAttr0C_GetSpeedChangeTimes(apAttrPair) ((apAttrPair)[3])	// UC (UC*)
#define CCdiFinShtFinAttr0C_SetSpeedChangeTimes(apAttrPair, aTimes) ((apAttrPair)[3] = (UC)(aTimes))
																				// void (UC*, UC)
#define CCdiFinShtFinAttr0C_GetChangeSpeedTiming(apAttrPair, aTime) ( ((US)((apAttrPair)[CDIFIN_SHT_FIN_ATTR_0C_BASE_SIZE + CDIFIN_SHT_FIN_ATTR_0C_TIMES_SET_SIZE * (aTime - 1) + 0]) << 8) | (US)((apAttrPair)[CDIFIN_SHT_FIN_ATTR_0C_BASE_SIZE + CDIFIN_SHT_FIN_ATTR_0C_TIMES_SET_SIZE * (aTime - 1) + 1]) )
																				// US (UC*, UC)
#define CCdiFinShtFinAttr0C_SetChangeSpeedTiming(apAttrPair, aTiming, aTime) (apAttrPair)[CDIFIN_SHT_FIN_ATTR_0C_BASE_SIZE + CDIFIN_SHT_FIN_ATTR_0C_TIMES_SET_SIZE * (aTime - 1) + 0] = (UC)((US)(aTiming) >> 8); (apAttrPair)[CDIFIN_SHT_FIN_ATTR_0C_BASE_SIZE + CDIFIN_SHT_FIN_ATTR_0C_TIMES_SET_SIZE * (aTime - 1) + 1] = (UC)((US)(aTiming))
																				// void (UC*, US, UC)
#define CCdiFinShtFinAttr0C_GetChangeSpeed(apAttrPair, aTime) ( ((US)((apAttrPair)[CDIFIN_SHT_FIN_ATTR_0C_BASE_SIZE + CDIFIN_SHT_FIN_ATTR_0C_TIMES_SET_SIZE * (aTime - 1) + 2]) << 8) | (US)((apAttrPair)[CDIFIN_SHT_FIN_ATTR_0C_BASE_SIZE + CDIFIN_SHT_FIN_ATTR_0C_TIMES_SET_SIZE * (aTime - 1) + 3]) )
																				// US (UC*, UC)
#define CCdiFinShtFinAttr0C_SetChangeSpeed(apAttrPair, aSpeed, aTime) (apAttrPair)[CDIFIN_SHT_FIN_ATTR_0C_BASE_SIZE + CDIFIN_SHT_FIN_ATTR_0C_TIMES_SET_SIZE * (aTime - 1) + 2] = (UC)((US)(aSpeed) >> 8); (apAttrPair)[CDIFIN_SHT_FIN_ATTR_0C_BASE_SIZE + CDIFIN_SHT_FIN_ATTR_0C_TIMES_SET_SIZE * (aTime - 1) + 3] = (UC)((US)(aSpeed))
																				// void (UC*, US, UC)


#endif // !defined(CCdiFinShtFinAttr0C_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
