// File Name:	CCdiFinCfgAttrOutDest47.h
// Description:	Header File of Output Destination Configuration Attributes "Current Staple Configuration(47H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCfgAttrOutDest47_h)
#define CCdiFinCfgAttrOutDest47_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyOutDest.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CFG_ATTR_OUT_DEST_47_SIZE				2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCfgAttrOutDest47_GetAttributeKey() ((UC)eCdiFinCfgKeyOutDest_Staple)
																				// UC (void)
#define CCdiFinCfgAttrOutDest47_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CFG_ATTR_OUT_DEST_47_SIZE)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest47_GetSingleStitch(apAttrPair) (((apAttrPair)[1] >> 0) & 0x01)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest47_SetSingleStitch(apAttrPair, aSelect) (apAttrPair)[1] &= ~(0x01 << 0); (apAttrPair)[1] |= (((aSelect) & 0x01) << 0)
																				// void (UC*, UC)
#define CCdiFinCfgAttrOutDest47_GetDoubleStitch(apAttrPair) (((apAttrPair)[1] >> 1) & 0x01)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest47_SetDoubleStitch(apAttrPair, aSelect) (apAttrPair)[1] &= ~(0x01 << 1); (apAttrPair)[1] |= (((aSelect) & 0x01) << 1)
																				// void (UC*, UC)
#define CCdiFinCfgAttrOutDest47_GetTripleStitch(apAttrPair) (((apAttrPair)[1] >> 2) & 0x01)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest47_SetTripleStitch(apAttrPair, aSelect) (apAttrPair)[1] &= ~(0x01 << 2); (apAttrPair)[1] |= (((aSelect) & 0x01) << 2)
																				// void (UC*, UC)
#define CCdiFinCfgAttrOutDest47_GetQuadStitch(apAttrPair) (((apAttrPair)[1] >> 3) & 0x01)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest47_SetQuadStitch(apAttrPair, aSelect) (apAttrPair)[1] &= ~(0x01 << 3); (apAttrPair)[1] |= (((aSelect) & 0x01) << 3)
																				// void (UC*, UC)
#define CCdiFinCfgAttrOutDest47_GetSingleRear(apAttrPair) (((apAttrPair)[1] >> 5) & 0x01)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest47_SetSingleRear(apAttrPair, aSelect) (apAttrPair)[1] &= ~(0x01 << 5); (apAttrPair)[1] |= (((aSelect) & 0x01) << 5)
																				// void (UC*, UC)
#define CCdiFinCfgAttrOutDest47_GetSingleFront(apAttrPair) (((apAttrPair)[1] >> 6) & 0x01)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest47_SetSingleFront(apAttrPair, aSelect) (apAttrPair)[1] &= ~(0x01 << 6); (apAttrPair)[1] |= (((aSelect) & 0x01) << 6)
																				// void (UC*, UC)


#endif // !defined(CCdiFinCfgAttrOutDest47_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
