// File Name:	CCdiFinCapaAttrInSrc04.h
// Description:	Header File of Input Source Capabilities Attributes "Input Source Unit Type(04H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCapaAttrInSrc04_h)
#define CCdiFinCapaAttrInSrc04_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCapaAttrKeyInSrc.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CAPA_ATTR_IN_SRC_04_SIZE						2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCapaAttrInSrc04_GetAttributeKey() ((UC)eCdiFinCapaKeyInSrc_UnitType)
																				// UC (void)
#define CCdiFinCapaAttrInSrc04_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CAPA_ATTR_IN_SRC_04_SIZE)
																				// UC (UC*)
#define CCdiFinCapaAttrInSrc04_GetDeviceUnitType(apAttrPair) (((apAttrPair)[1] >> 0) & 0x1F)
																				// UC (UC*)
#define CCdiFinCapaAttrInSrc04_SetDeviceUnitType(apAttrPair, aType) ((apAttrPair)[1] &= ~(0x1F << 0)); (apAttrPair)[1] |= (((aType) & 0x1F) << 0)
																				// void (UC*, UC)


#endif // !defined(CCdiFinCapaAttrInSrc04_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
