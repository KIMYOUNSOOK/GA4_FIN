// File Name:	CCdiFinCapaAttrOutDest5B.h
// Description:	Header File of Output Destination Capabilities Attributes "Folding Booklet Maker Ability(5BH)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCapaAttrOutDest5B_h)
#define CCdiFinCapaAttrOutDest5B_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCapaAttrKeyOutDest.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CAPA_ATTR_OUT_DEST_5B_SIZE					4


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCapaAttrOutDest5B_GetAttributeKey() ((UC)eCdiFinCapaKeyOutDest_Book)
																				// UC (void)
#define CCdiFinCapaAttrOutDest5B_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CAPA_ATTR_OUT_DEST_5B_SIZE)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest5B_GetSelectability(apAttrPair) ((apAttrPair)[1])	// UC (UC*)
#define CCdiFinCapaAttrOutDest5B_SetSelectability(apAttrPair, aSelectability) ((apAttrPair)[1] = aSelectability)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest5B_GetSaddleFoldAndStaple(apAttrPair) (((apAttrPair)[2] >> 0) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest5B_SetSaddleFoldAndStaple(apAttrPair, aSupport) ((apAttrPair)[2] &= ~(0x01 << 0)); (apAttrPair)[2] |= (((aSupport) & 0x01) << 0)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest5B_GetSaddleFoldAndNoStaple(apAttrPair) (((apAttrPair)[2] >> 1) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest5B_SetSaddleFoldAndNoStaple(apAttrPair, aSupport) ((apAttrPair)[2] &= ~(0x01 << 1)); (apAttrPair)[2] |= (((aSupport) & 0x01) << 1)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest5B_GetCoverOrder(apAttrPair) (((apAttrPair)[2] >> 2) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest5B_SetCoverOrder(apAttrPair, aSupport) ((apAttrPair)[2] &= ~(0x01 << 2)); (apAttrPair)[2] |= (((aSupport) & 0x01) << 2)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest5B_GetSquareFolding(apAttrPair) (((apAttrPair)[3] >> 0) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest5B_SetSquareFolding(apAttrPair, aSupport) ((apAttrPair)[3] &= ~(0x01 << 0)); (apAttrPair)[3] |= (((aSupport) & 0x01) << 0)
																				// void (UC*, UC)


#endif // !defined(CCdiFinCapaAttrOutDest5B_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
