// File Name:	CCdiFinCfgAttrOutDest49.h
// Description:	Header File of Output Destination Configuration Attributes "Current Booklet Maker Configuration(49H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCfgAttrOutDest49_h)
#define CCdiFinCfgAttrOutDest49_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyOutDest.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CFG_ATTR_OUT_DEST_49_SIZE				3


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCfgAttrOutDest49_GetAttributeKey() ((UC)eCdiFinCfgKeyOutDest_Book)
																				// UC (void)
#define CCdiFinCfgAttrOutDest49_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CFG_ATTR_OUT_DEST_49_SIZE)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest49_GetSaddleFoldAndStaple(apAttrPair) (((apAttrPair)[1] >> 0) & 0x01)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest49_SetSaddleFoldAndStaple(apAttrPair, aSelect) (apAttrPair)[1] &= ~(0x01 << 0); (apAttrPair)[1] |= (((aSelect) & 0x01) << 0)
																				// void (UC*, UC)
#define CCdiFinCfgAttrOutDest49_GetSaddleFoldAndNoStaple(apAttrPair) (((apAttrPair)[1] >> 1) & 0x01)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest49_SetSaddleFoldAndNoStaple(apAttrPair, aSelect) (apAttrPair)[1] &= ~(0x01 << 1); (apAttrPair)[1] |= (((aSelect) & 0x01) << 1)
																				// void (UC*, UC)
#define CCdiFinCfgAttrOutDest49_GetSquareFolding(apAttrPair) (((apAttrPair)[2] >> 0) & 0x01)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest49_SetSquareFolding(apAttrPair, aSelect) (apAttrPair)[2] &= ~(0x01 << 0); (apAttrPair)[2] |= (((aSelect) & 0x01) << 0)
																				// void (UC*, UC)


#endif // !defined(CCdiFinCfgAttrOutDest49_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
