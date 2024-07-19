// File Name:	CCdiFinShtFinAttr20.h
// Description:	Header File of Sheet Finish Attributes "Set Action(20H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinShtFinAttr20_h)
#define CCdiFinShtFinAttr20_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinShtFinAttrKey.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_SHT_FIN_ATTR_20_SIZE		2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinShtFinAttr20_GetAttributeKey() ((UC)eCdiFinShtFinAttr_SetAction)
																				// UC (void)
#define CCdiFinShtFinAttr20_GetAttributeSize(apAttrPair) ((UC)CDIFIN_SHT_FIN_ATTR_20_SIZE)
																				// UC (UC*)
#define CCdiFinShtFinAttr20_GetSetAction(apAttrPair) (apAttrPair[1])			// UC (UC*)
#define CCdiFinShtFinAttr20_SetSetAction(apAttrPair, aSetAction) (apAttrPair[1] = aSetAction)
																				// void (UC*, UC)


#endif // !defined(CCdiFinShtFinAttr20_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
