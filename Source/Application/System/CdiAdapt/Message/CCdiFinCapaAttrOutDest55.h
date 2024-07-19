// File Name:	CCdiFinCapaAttrOutDest55.h
// Description:	Header File of Output Destination Capabilities Attributes "Punching Ability(55H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCapaAttrOutDest55_h)
#define CCdiFinCapaAttrOutDest55_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCapaAttrKeyOutDest.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CAPA_ATTR_OUT_DEST_55_SIZE					4


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCapaAttrOutDest55_GetAttributeKey() ((UC)eCdiFinCapaKeyOutDest_Punch)
																				// UC (void)
#define CCdiFinCapaAttrOutDest55_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CAPA_ATTR_OUT_DEST_55_SIZE)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest55_GetSelectability(apAttrPair) ((apAttrPair)[1])	// UC (UC*)
#define CCdiFinCapaAttrOutDest55_SetSelectability(apAttrPair, aSelectability) ((apAttrPair)[1] = aSelectability)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest55_GetDoublePunch(apAttrPair) (((apAttrPair)[2] >> 0) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest55_SetDoublePunch(apAttrPair, aSupport) ((apAttrPair)[2] &= ~(0x01 << 0)); (apAttrPair)[2] |= (((aSupport) & 0x01) << 0)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest55_GetTriplePunch(apAttrPair) (((apAttrPair)[2] >> 1) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest55_SetTriplePunch(apAttrPair, aSupport) ((apAttrPair)[2] &= ~(0x01 << 1)); (apAttrPair)[2] |= (((aSupport) & 0x01) << 1)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest55_GetQuadPunch(apAttrPair) (((apAttrPair)[2] >> 2) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest55_SetQuadPunch(apAttrPair, aSupport) ((apAttrPair)[2] &= ~(0x01 << 2)); (apAttrPair)[2] |= (((aSupport) & 0x01) << 2)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest55_GetMultiPunch(apAttrPair) (((apAttrPair)[2] >> 3) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest55_SetMultiPunch(apAttrPair, aSupport) ((apAttrPair)[2] &= ~(0x01 << 3)); (apAttrPair)[2] |= (((aSupport) & 0x01) << 3)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest55_GetDoublePunchType(apAttrPair) (((apAttrPair)[3] >> 0) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest55_SetDoublePunchType(apAttrPair, aType) ((apAttrPair)[3] &= ~(0x01 << 0)); (apAttrPair)[3] |= (((aType) & 0x01) << 0)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest55_GetQuadPunchType(apAttrPair) (((apAttrPair)[3] >> 4) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest55_SetQuadPunchType(apAttrPair, aType) ((apAttrPair)[3] &= ~(0x01 << 4)); (apAttrPair)[3] |= (((aType) & 0x01) << 4)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest55_GetMultiPunchType(apAttrPair) (((apAttrPair)[3] >> 6) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest55_SetMultiPunchType(apAttrPair, aType) ((apAttrPair)[3] &= ~(0x01 << 6)); (apAttrPair)[3] |= (((aType) & 0x01) << 6)
																				// void (UC*, UC)


#endif // !defined(CCdiFinCapaAttrOutDest55_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
