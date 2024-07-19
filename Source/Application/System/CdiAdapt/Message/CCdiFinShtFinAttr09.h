// File Name:	CCdiFinShtFinAttr09.h
// Description:	Header File of Sheet Finish Attributes "Media Size(09H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.13
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinShtFinAttr09_h)
#define CCdiFinShtFinAttr09_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinShtFinAttrKey.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_SHT_FIN_ATTR_09_SIZE	5


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinShtFinAttr09_GetAttributeKey() ((UC)eCdiFinShtFinAttr_MediaSize)	// UC (void)
#define CCdiFinShtFinAttr09_GetAttributeSize(apAttrPair) ((UC)CDIFIN_SHT_FIN_ATTR_09_SIZE)
																				// UC (UC*)
#define CCdiFinShtFinAttr09_GetProcessSize(apAttrPair) (CF2_US(&(apAttrPair[1])))	// US (UC*)
#define CCdiFinShtFinAttr09_SetProcessSize(apAttrPair, aSize) (US_CF2(aSize, &(apAttrPair[1])))
																				// void (UC*, US)
#define CCdiFinShtFinAttr09_GetCrossProcSize(apAttrPair) (CF2_US(&(apAttrPair[3])))	// US (UC*)
#define CCdiFinShtFinAttr09_SetCrossProcSize(apAttrPair, aSize) (US_CF2(aSize, &(apAttrPair[3])))
																				// void (UC*, US)


#endif // !defined(CCdiFinShtFinAttr09_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
