// File Name:	CCdiFinShtFinAttr04.h
// Description:	Header File of Sheet Finish Attributes "Booklet Maker Mode(04H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.13
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinShtFinAttr04_h)
#define CCdiFinShtFinAttr04_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinShtFinAttrKey.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_SHT_FIN_ATTR_04_SIZE	2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinShtFinAttr04_GetAttributeKey() ((UC)eCdiFinShtFinAttr_BkltMode)	// UC (void)
#define CCdiFinShtFinAttr04_GetAttributeSize(apAttrPair) ((UC)CDIFIN_SHT_FIN_ATTR_04_SIZE)
																				// UC (UC*)
#define CCdiFinShtFinAttr04_GetBookletMode(apAttrPair) (apAttrPair[1])			// UC (UC*)
#define CCdiFinShtFinAttr04_SetBookletMode(apAttrPair, aBookMode) (apAttrPair[1] = aBookMode)
																				// void (UC*)


#endif // !defined(CCdiFinShtFinAttr04_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
