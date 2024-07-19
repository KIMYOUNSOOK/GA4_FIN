// File Name:	CCdiFinCapaAttrOutDest47.h
// Description:	Header File of Output Destination Capabilities Attributes "Offset Ability(47H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCapaAttrOutDest47_h)
#define CCdiFinCapaAttrOutDest47_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCapaAttrKeyOutDest.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CAPA_ATTR_OUT_DEST_47_SIZE					3


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCapaAttrOutDest47_GetAttributeKey() ((UC)eCdiFinCapaKeyOutDest_Offset)
																				// UC (void)
#define CCdiFinCapaAttrOutDest47_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CAPA_ATTR_OUT_DEST_47_SIZE)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest47_GetSelectability(apAttrPair) ((apAttrPair)[1])	// UC (UC*)
#define CCdiFinCapaAttrOutDest47_SetSelectability(apAttrPair, aSelectability) ((apAttrPair)[1] = aSelectability)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest47_Get3PositionOffset(apAttrPair) (((apAttrPair)[2] >> 1) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest47_Set3PositionOffset(apAttrPair, aOffset) ((apAttrPair)[2] &= ~(0x01 << 1)); (apAttrPair)[2] |= (((aOffset) & 0x01) << 1)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest47_GetOffsetMethod(apAttrPair) (((apAttrPair)[2] >> 4) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest47_SetOffsetMethod(apAttrPair, aMethod) ((apAttrPair)[2] &= ~(0x01 << 4)); (apAttrPair)[2] |= (((aMethod) & 0x01) << 4)
																				// void (UC*, UC)


#endif // !defined(CCdiFinCapaAttrOutDest47_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
