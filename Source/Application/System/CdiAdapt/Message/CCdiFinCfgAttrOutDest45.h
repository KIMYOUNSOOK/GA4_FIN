// File Name:	CCdiFinCfgAttrOutDest45.h
// Description:	Header File of Output Destination Configuration Attributes "Current Offset Configuration(45H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCfgAttrOutDest45_h)
#define CCdiFinCfgAttrOutDest45_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyOutDest.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CFG_ATTR_OUT_DEST_45_SIZE				2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCfgAttrOutDest45_GetAttributeKey() ((UC)eCdiFinCfgKeyOutDest_Offset)
																				// UC (void)
#define CCdiFinCfgAttrOutDest45_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CFG_ATTR_OUT_DEST_45_SIZE)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest45_GetOffset(apAttrPair) (apAttrPair[1])			// UC (UC*)
#define CCdiFinCfgAttrOutDest45_SetOffset(apAttrPair, aSelect) (apAttrPair[1] = aSelect)
																				// void (UC*, UC)


#endif // !defined(CCdiFinCfgAttrOutDest45_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
