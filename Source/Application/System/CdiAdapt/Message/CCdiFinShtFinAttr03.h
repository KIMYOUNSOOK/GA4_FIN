// File Name:	CCdiFinShtFinAttr03.h
// Description:	Header File of Sheet Finish Attributes "Punch Mode(03H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.13
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinShtFinAttr03_h)
#define CCdiFinShtFinAttr03_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinShtFinAttrKey.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_SHT_FIN_ATTR_03_SIZE	2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinShtFinAttr03_GetAttributeKey() ((UC)eCdiFinShtFinAttr_PnchMode)	// UC (void)
#define CCdiFinShtFinAttr03_GetAttributeSize(apAttrPair) ((UC)CDIFIN_SHT_FIN_ATTR_03_SIZE)
																				// UC (UC*)
#define CCdiFinShtFinAttr03_GetPunchMode(apAttrPair) (apAttrPair[1])			// UC (UC*)
#define CCdiFinShtFinAttr03_SetPunchMode(apAttrPair, aPunchMode) (apAttrPair[1] = aPunchMode)
																				// void (UC*)


#endif // !defined(CCdiFinShtFinAttr02_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
