// File Name:	CCdiFinShtFinAttr07.h
// Description:	Header File of Sheet Finish Attributes "Fold Mode(07H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.13
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinShtFinAttr07_h)
#define CCdiFinShtFinAttr07_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinShtFinAttrKey.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_SHT_FIN_ATTR_07_SIZE	2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinShtFinAttr07_GetAttributeKey() ((UC)eCdiFinShtFinAttr_FoldMode)	// UC (void)
#define CCdiFinShtFinAttr07_GetAttributeSize(apAttrPair) ((UC)CDIFIN_SHT_FIN_ATTR_07_SIZE)
																				// UC (UC*)
#define CCdiFinShtFinAttr07_GetFoldMode(apAttrPair) (apAttrPair[1])				// UC (UC*)
#define CCdiFinShtFinAttr07_SetFoldMode(apAttrPair, aFoldMode) (apAttrPair[1] = aFoldMode)
																				// void (UC*)


#endif // !defined(CCdiFinShtFinAttr07_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
