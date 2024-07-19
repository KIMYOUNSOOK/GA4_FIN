// File Name:	CCdiFinCapaAttrOutDest4B.h
// Description:	Header File of Output Destination Capabilities Attributes "Folding Ability(4BH)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCapaAttrOutDest4B_h)
#define CCdiFinCapaAttrOutDest4B_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCapaAttrKeyOutDest.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CAPA_ATTR_OUT_DEST_4B_SIZE					5


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCapaAttrOutDest4B_GetAttributeKey() ((UC)eCdiFinCapaKeyOutDest_Fold)
																				// UC (void)
#define CCdiFinCapaAttrOutDest4B_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CAPA_ATTR_OUT_DEST_4B_SIZE)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest4B_GetSelectability(apAttrPair) ((apAttrPair)[1])	// UC (UC*)
#define CCdiFinCapaAttrOutDest4B_SetSelectability(apAttrPair, aSelectability) ((apAttrPair)[1] = aSelectability)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest4B_GetZFoldSupport(apAttrPair) (((apAttrPair)[2] >> 0) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest4B_SetZFoldSupport(apAttrPair, aSupport) ((apAttrPair)[2] &= ~(0x01 << 0)); (apAttrPair)[2] |= (((aSupport) & 0x01) << 0)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest4B_GetTriFoldCSupport(apAttrPair) (((apAttrPair)[2] >> 4) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest4B_SetTriFoldCSupport(apAttrPair, aSupport) ((apAttrPair)[2] &= ~(0x01 << 4)); (apAttrPair)[2] |= (((aSupport) & 0x01) << 4)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest4B_GetTriFoldZSupport(apAttrPair) (((apAttrPair)[3] >> 2) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest4B_SetTriFoldZSupport(apAttrPair, aSupport) ((apAttrPair)[3] &= ~(0x01 << 2)); (apAttrPair)[3] |= (((aSupport) & 0x01) << 2)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest4B_GetSaddleFoldSupport(apAttrPair) (((apAttrPair)[3] >> 4) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest4B_SetSaddleFoldSupport(apAttrPair, aSupport) ((apAttrPair)[3] &= ~(0x01 << 4)); (apAttrPair)[3] |= (((aSupport) & 0x01) << 4)
																				// void (UC*, UC)


#endif // !defined(CCdiFinCapaAttrOutDest4B_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
