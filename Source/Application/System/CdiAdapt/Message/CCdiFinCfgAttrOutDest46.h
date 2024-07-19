// File Name:	CCdiFinCfgAttrOutDest46.h
// Description:	Header File of Output Destination Configuration Attributes "Current Sheet Folding Configuration(46H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCfgAttrOutDest46_h)
#define CCdiFinCfgAttrOutDest46_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyOutDest.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CFG_ATTR_OUT_DEST_46_SIZE				2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCfgAttrOutDest46_GetAttributeKey() ((UC)eCdiFinCfgKeyOutDest_Fold)
																				// UC (void)
#define CCdiFinCfgAttrOutDest46_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CFG_ATTR_OUT_DEST_46_SIZE)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest46_GetZFold(apAttrPair) (((apAttrPair)[1] >> 0) & 0x01)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest46_SetZFold(apAttrPair, aSelect) ((apAttrPair)[1] &= ~(0x01 << 0)); (apAttrPair)[1] |= (((aSelect) & 0x01) << 0)
																				// void (UC*, UC)
#define CCdiFinCfgAttrOutDest46_GetTriFoldZ(apAttrPair) (((apAttrPair)[1] >> 1) & 0x01)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest46_SetTriFoldZ(apAttrPair, aSelect) (apAttrPair)[1] &= ~(0x01 << 1); (apAttrPair)[1] |= (((aSelect) & 0x01) << 1)
																				// void (UC*, UC)
#define CCdiFinCfgAttrOutDest46_GetTriFoldC(apAttrPair) (((apAttrPair)[1] >> 2) & 0x01)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest46_SetTriFoldC(apAttrPair, aSelect) (apAttrPair)[1] &= ~(0x01 << 2); (apAttrPair)[1] |= (((aSelect) & 0x01) << 2)
																				// void (UC*, UC)
#define CCdiFinCfgAttrOutDest46_GetSaddleFold(apAttrPair) (((apAttrPair)[1] >> 3) & 0x01)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest46_SetSaddleFold(apAttrPair, aSelect) (apAttrPair)[1] &= ~(0x01 << 3); (apAttrPair)[1] |= (((aSelect) & 0x01) << 3)
																				// void (UC*, UC)


#endif // !defined(CCdiFinCfgAttrOutDest46_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
