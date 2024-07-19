// File Name:	CCdiFinCfgAttrInSrc03.h
// Description:	Header File of Input Source Configuration Attributes "Selectability(03H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCfgAttrInSrc03_h)
#define CCdiFinCfgAttrInSrc03_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyInSrc.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CFG_ATTR_IN_SRC_03_SIZE				2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCfgAttrInSrc03_GetAttributeKey() ((UC)eCdiFinCfgKeyInSrc_Select)
																				// UC (void)
#define CCdiFinCfgAttrInSrc03_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CFG_ATTR_IN_SRC_03_SIZE)
																				// UC (UC*)
#define CCdiFinCfgAttrInSrc03_GetSelectability(apAttrPair) (apAttrPair[1])	// UC (UC*)
#define CCdiFinCfgAttrInSrc03_SetSelectability(apAttrPair, aSelect) (apAttrPair[1] = aSelect)
																				// void (UC*)


#endif // !defined(CCdiFinCfgAttrInSrc03_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
