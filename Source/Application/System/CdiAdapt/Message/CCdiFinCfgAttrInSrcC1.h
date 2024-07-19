// File Name:	CCdiFinCfgAttrInSrcC1.h
// Description:	Header File of Input Source Configuration Attributes "Selectability(C1H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCfgAttrInSrcC1_h)
#define CCdiFinCfgAttrInSrcC1_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyInSrc.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CFG_ATTR_IN_SRC_C1_SIZE				5


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCfgAttrInSrcC1_GetAttributeKey() ((UC)eCdiFinCfgKeyInSrc_IregSize)// UC (void)
#define CCdiFinCfgAttrInSrcC1_GetAttributeSize(apAttrPair) (CDIFIN_CFG_ATTR_IN_SRC_C1_SIZE)
																				// UC (UC*)
#define CCdiFinCfgAttrInSrcC1_GetProcessSize(apAttrPair) ( ((US)((apAttrPair)[1]) << 8) | (US)((apAttrPair)[2]) )
																				// US (UC*)
#define CCdiFinCfgAttrInSrcC1_SetProcessSize(apAttrPair, aSize) (apAttrPair)[1] = (UC)((US)(aSize) >> 8); (apAttrPair)[2] = (UC)((US)(aSize))
																				// void (UC*, US)
#define CCdiFinCfgAttrInSrcC1_GetCrossProcessSize(apAttrPair) ( ((US)((apAttrPair)[3]) << 8) | (US)((apAttrPair)[4]) )
																				// US (UC*)
#define CCdiFinCfgAttrInSrcC1_SetCrossProcessSize(apAttrPair, aSize) (apAttrPair)[3] = (UC)((US)(aSize) >> 8); (apAttrPair)[4] = (UC)((US)(aSize))
																				// void (UC*, US)


#endif // !defined(CCdiFinCfgAttrInSrcC1_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
