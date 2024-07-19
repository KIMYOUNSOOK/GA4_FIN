// File Name:	CCdiFinCapaAttrOutDestC6.h
// Description:	Header File of Output Destination Capabilities Attributes "Staple Type(C6H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCapaAttrOutDestC6_h)
#define CCdiFinCapaAttrOutDestC6_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCapaAttrKeyOutDest.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CAPA_ATTR_OUT_DEST_C6_SIZE					2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCapaAttrOutDestC6_GetAttributeKey() ((UC)eCdiFinCapaKeyOutDest_StapleType)
																				// UC (void)
#define CCdiFinCapaAttrOutDestC6_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CAPA_ATTR_OUT_DEST_C6_SIZE)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDestC6_GetStapleType(apAttrPair) ((apAttrPair)[1])
																				// UC (UC*)
#define CCdiFinCapaAttrOutDestC6_SetStapleType(apAttrPair, aType) ((apAttrPair)[1] = aType)
																				// void (UC*, UC)


#endif // !defined(CCdiFinCapaAttrOutDestC6_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
