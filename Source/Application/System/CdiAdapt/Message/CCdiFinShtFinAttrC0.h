// File Name:	CCdiFinShtFinAttrC0.h
// Description:	Header File of Sheet Finish Attributes "Offset Position(C0H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinShtFinAttrC0_h)
#define CCdiFinShtFinAttrC0_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinShtFinAttrKey.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_SHT_FIN_ATTR_C0_SIZE	2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinShtFinAttrC0_GetAttributeKey() ((UC)eCdiFinShtFinAttr_OfstPos)
																				// UC (void)
#define CCdiFinShtFinAttrC0_GetAttributeSize(apAttrPair) ((UC)CDIFIN_SHT_FIN_ATTR_C0_SIZE)
																				// UC (UC*)
#define CCdiFinShtFinAttrC0_GetOffsetPosition(apAttrPair) (apAttrPair[1])		// UC (UC*)
#define CCdiFinShtFinAttrC0_SetOffsetPosition(apAttrPair, aPosition) (apAttrPair[1] = aPosition)
																				// void (UC*, UC)


#endif // !defined(CCdiFinShtFinAttrC0_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
