// File Name:	CCdiFinCfgAttrInSrcC0.h
// Description:	Header File of Input Source Configuration Attributes "Selectability(C0H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCfgAttrInSrcC0_h)
#define CCdiFinCfgAttrInSrcC0_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyInSrc.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CFG_ATTR_IN_SRC_C0_BASE_SIZE				2
#define CDIFIN_CFG_ATTR_IN_SRC_C0_SET_SIZE					1


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCfgAttrInSrcC0_GetAttributeKey() ((UC)eCdiFinCfgKeyInSrc_SizeAlt)// UC (void)
#define CCdiFinCfgAttrInSrcC0_GetAttributeSize(apAttrPair) ( apAttrPair == NULL ? CDIFIN_CFG_ATTR_IN_SRC_C0_BASE_SIZE : ((UC)(CDIFIN_CFG_ATTR_IN_SRC_C0_BASE_SIZE + CDIFIN_CFG_ATTR_IN_SRC_C0_SET_SIZE * (apAttrPair)[1])) )
																				// UC (UC*)
#define CCdiFinCfgAttrInSrcC0_GetAttributeSizeByNumberOfAlternate(aNumber) ((UC)(CDIFIN_CFG_ATTR_IN_SRC_C0_BASE_SIZE + CDIFIN_CFG_ATTR_IN_SRC_C0_SET_SIZE * aNumber))
																				// UC (UC)
#define CCdiFinCfgAttrInSrcC0_GetNumberOfAlternate(apAttrPair) (apAttrPair[1])	// UC (UC*)
#define CCdiFinCfgAttrInSrcC0_SetNumberOfAlternate(apAttrPair, aNumber) (apAttrPair[1] = aNumber)
																				// void (UC*, UC)
#define CCdiFinCfgAttrInSrcC0_GetSizeAlternate1(apAttrPair) (apAttrPair[2])	// UC (UC*)
#define CCdiFinCfgAttrInSrcC0_SetSizeAlternate1(apAttrPair, aAlternate) (apAttrPair[2] = aAlternate)
																				// void (UC*, UC)
#define CCdiFinCfgAttrInSrcC0_GetSizeAlternate2(apAttrPair) (apAttrPair[3])	// UC (UC*)
#define CCdiFinCfgAttrInSrcC0_SetSizeAlternate2(apAttrPair, aAlternate) (apAttrPair[3] = aAlternate)
																				// void (UC*, UC)
#define CCdiFinCfgAttrInSrcC0_GetSizeAlternate3(apAttrPair) (apAttrPair[4])	// UC (UC*)
#define CCdiFinCfgAttrInSrcC0_SetSizeAlternate3(apAttrPair, aAlternate) (apAttrPair[4] = aAlternate)
																				// void (UC*, UC)
#define CCdiFinCfgAttrInSrcC0_GetSizeAlternate4(apAttrPair) (apAttrPair[5])	// UC (UC*)
#define CCdiFinCfgAttrInSrcC0_SetSizeAlternate4(apAttrPair, aAlternate) (apAttrPair[5] = aAlternate)
																				// void (UC*, UC)
#define CCdiFinCfgAttrInSrcC0_GetSizeAlternate5(apAttrPair) (apAttrPair[6])	// UC (UC*)
#define CCdiFinCfgAttrInSrcC0_SetSizeAlternate5(apAttrPair, aAlternate) (apAttrPair[6] = aAlternate)
																				// void (UC*, UC)


#endif // !defined(CCdiFinCfgAttrInSrcC0_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
