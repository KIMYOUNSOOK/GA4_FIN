// File Name:	CCdiFinCapaAttrOutDest04.h
// Description:	Header File of Output Destination Capabilities Attributes "Output Destination Unit Type(04H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCapaAttrOutDest04_h)
#define CCdiFinCapaAttrOutDest04_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCapaAttrKeyOutDest.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CAPA_ATTR_OUT_DEST_04_SIZE					2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCapaAttrOutDest04_GetAttributeKey() ((UC)eCdiFinCapaKeyOutDest_UnitType)
																				// UC (void)
#define CCdiFinCapaAttrOutDest04_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CAPA_ATTR_OUT_DEST_04_SIZE)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest04_GetDeviceUnitType(apAttrPair) (((apAttrPair)[1] >> 0) & 0x1F)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest04_SetDeviceUnitType(apAttrPair, aType) ((apAttrPair)[1] &= (UC)(~(0x1F << 0))); (apAttrPair)[1] |= (((aType) & 0x1F) << 0)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest04_GetPurge(apAttrPair) (((apAttrPair)[1] >> 5) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest04_SetPurge(apAttrPair, aPurge) ((apAttrPair)[1] &= (UC)(~(0x01 << 5))); (apAttrPair)[1] |= (((aPurge) & 0x01) << 5)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest04_GetVirtual(apAttrPair) (((apAttrPair)[1] >> 7) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest04_SetVirtual(apAttrPair, aVirtual) ((apAttrPair)[1] &= (UC)(~(0x01 << 7))); (apAttrPair)[1] |= (((aVirtual) & 0x01) << 7)
																				// void (UC*, UC)


#endif // !defined(CCdiFinCapaAttrOutDest04_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
