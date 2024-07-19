// File Name:	CCdiFinShtFinAttr0B.h
// Description:	Header File of Sheet Finish Attributes "Sheet Curl(0BH)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.13
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinShtFinAttr0B_h)
#define CCdiFinShtFinAttr0B_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinShtFinAttrKey.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_SHT_FIN_ATTR_0B_SIZE	3


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinShtFinAttr0B_GetAttributeKey() ((UC)eCdiFinShtFinAttr_SheetCurl)	// UC (void)
#define CCdiFinShtFinAttr0B_GetAttributeSize(apAttrPair) ((UC)CDIFIN_SHT_FIN_ATTR_0B_SIZE)
																				// UC (UC*)
#define CCdiFinShtFinAttr0B_GetSheetCurl(apAttrPair) (apAttrPair[1])			// UC (UC*)
#define CCdiFinShtFinAttr0B_SetSheetCurl(apAttrPair, aCurl) (apAttrPair[1] = aCurl)
																				// void (UC*, UC)
#define CCdiFinShtFinAttr0B_GetCurlQuantity(apAttrPair) (apAttrPair[2])			// UC (UC*)
#define CCdiFinShtFinAttr0B_SetCurlQuantity(apAttrPair, aQuantity) (apAttrPair[2] = aQuantity)
																				// void (UC*, UC)


#endif // !defined(CCdiFinShtFinAttr0B_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
