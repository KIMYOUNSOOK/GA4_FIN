// File Name:	CCdiFinCapaAttrOutDestC7.h
// Description:	Header File of Output Destination Capabilities Attributes "Booklet Maker Staple Type(C7H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCapaAttrOutDestC7_h)
#define CCdiFinCapaAttrOutDestC7_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCapaAttrKeyOutDest.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CAPA_ATTR_OUT_DEST_C7_SIZE					2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCapaAttrOutDestC7_GetAttributeKey() ((UC)eCdiFinCapaKeyOutDest_BookStapleType)
																				// UC (void)
#define CCdiFinCapaAttrOutDestC7_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CAPA_ATTR_OUT_DEST_C7_SIZE)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDestC7_GetStapleType(apAttrPair) ((apAttrPair)[1])	// UC (UC*)
#define CCdiFinCapaAttrOutDestC7_SetStapleType(apAttrPair, aType) ((apAttrPair)[1] = aType)
																				// void (UC*, UC)


#endif // !defined(CCdiFinCapaAttrOutDestC7_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
