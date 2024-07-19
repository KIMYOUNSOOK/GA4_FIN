// File Name:	CCdiFinShtFinAttr0D.h
// Description:	Header File of Sheet Finish Attributes "Sheet Integrity(0DH)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinShtFinAttr0D_h)
#define CCdiFinShtFinAttr0D_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinShtFinAttrKey.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_SHT_FIN_ATTR_0D_SIZE	2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinShtFinAttr0D_GetAttributeKey() ((UC)eCdiFinShtFinAttr_ShtIntegrity)
																				// UC (void)
#define CCdiFinShtFinAttr0D_GetAttributeSize(apAttrPair) ((UC)CDIFIN_SHT_FIN_ATTR_0D_SIZE)
																				// UC (UC*)
#define CCdiFinShtFinAttr0D_GetSheetIntegrity(apAttrPair) (apAttrPair[1])		// UC (UC*)
#define CCdiFinShtFinAttr0D_SetSheetIntegrity(apAttrPair, aIntegrity) (apAttrPair[1] = aIntegrity)
																				// void (UC*, UC)


#endif // !defined(CCdiFinShtFinAttr0D_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
