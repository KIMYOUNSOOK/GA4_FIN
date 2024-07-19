// File Name:	CCdiFinCfgAttrOutDest03.h
// Description:	Header File of Output Destination Configuration Attributes "Selectability(03H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCfgAttrOutDest03_h)
#define CCdiFinCfgAttrOutDest03_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyOutDest.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CFG_ATTR_OUT_DEST_03_SIZE				2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCfgAttrOutDest03_GetAttributeKey() ((UC)eCdiFinCfgKeyOutDest_Select)
																				// UC (void)
#define CCdiFinCfgAttrOutDest03_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CFG_ATTR_OUT_DEST_03_SIZE)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest03_GetSelectability(apAttrPair) (apAttrPair[1])	// UC (UC*)
#define CCdiFinCfgAttrOutDest03_SetSelectability(apAttrPair, aSelect) (apAttrPair[1] = aSelect)
																				// void (UC*)


#endif // !defined(CCdiFinCfgAttrOutDest03_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
