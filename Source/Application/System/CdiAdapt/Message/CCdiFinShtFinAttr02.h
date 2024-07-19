// File Name:	CCdiFinShtFinAttr02.h
// Description:	Header File of Sheet Finish Attributes "Staple Mode(02H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.13
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinShtFinAttr02_h)
#define CCdiFinShtFinAttr02_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinShtFinAttrKey.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_SHT_FIN_ATTR_02_SIZE	2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinShtFinAttr02_GetAttributeKey() ((UC)eCdiFinShtFinAttr_StplMode)	// UC (void)
#define CCdiFinShtFinAttr02_GetAttributeSize(apAttrPair) ((UC)CDIFIN_SHT_FIN_ATTR_02_SIZE)
																				// UC (UC*)
#define CCdiFinShtFinAttr02_GetOffsetMode(apAttrPair) (apAttrPair[1])			// UC (UC*)
#define CCdiFinShtFinAttr02_SetOffsetMode(apAttrPair, aStapleMode) (apAttrPair[1] = aStapleMode)
																				// void (UC*)


#endif // !defined(CCdiFinShtFinAttr02_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
