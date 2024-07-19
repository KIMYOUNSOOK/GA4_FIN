// File Name:	CCdiFinShtFinAttr01.h
// Description:	Header File of Sheet Finish Attributes "Offset Mode(01H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.13
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinShtFinAttr01_h)
#define CCdiFinShtFinAttr01_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinShtFinAttrKey.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_SHT_FIN_ATTR_01_SIZE	2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinShtFinAttr01_GetAttributeKey() ((UC)eCdiFinShtFinAttr_OfstMode)	// UC (void)
#define CCdiFinShtFinAttr01_GetAttributeSize(apAttrPair) ((UC)CDIFIN_SHT_FIN_ATTR_01_SIZE)
																				// UC (UC*)
#define CCdiFinShtFinAttr01_GetOffsetMode(apAttrPair) (apAttrPair[1])			// UC (UC*)
#define CCdiFinShtFinAttr01_SetOffsetMode(apAttrPair, aOffsetMode) (apAttrPair[1] = aOffsetMode)
																				// void (UC*)


#endif // !defined(CCdiFinShtFinAttr01_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
