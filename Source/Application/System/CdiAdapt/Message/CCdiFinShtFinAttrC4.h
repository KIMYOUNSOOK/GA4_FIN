// File Name:	CCdiFinShtFinAttrC4.h
// Description:	Header File of Sheet Finish Attributes "Exit Sheet Interval(C4H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinShtFinAttrC4_h)
#define CCdiFinShtFinAttrC4_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinShtFinAttrKey.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_SHT_FIN_ATTR_C4_SIZE		3


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinShtFinAttrC4_GetAttributeKey() ((UC)eCdiFinShtFinAttr_ExitShtInterval)
																				// UC (void)
#define CCdiFinShtFinAttrC4_GetAttributeSize(apAttrPair) ((UC)CDIFIN_SHT_FIN_ATTR_C4_SIZE)
																				// UC (UC*)
#define CCdiFinShtFinAttrC4_GetExitSheetInterval(apAttrPair) ( ((US)((apAttrPair)[1]) <<  8) | (US)((apAttrPair)[2]) )
																				// US (UC*)
#define CCdiFinShtFinAttrC4_SetExitSheetInterval(apAttrPair, aInterval) (apAttrPair)[1] = (UC)((US)(aInterval) >> 8); (apAttrPair)[2] = (UC)((US)(aInterval))
																				// void (UC*, US)


#endif // !defined(CCdiFinShtFinAttrC4_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
