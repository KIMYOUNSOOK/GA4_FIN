// File Name:	CCdiFinCapaAttrOutDestC7.h
// Description:	Header File of Output Destination Capabilities Attributes "Booklet Maker Staple Type(C7H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCapaAttrOutDestC8_h)
#define CCdiFinCapaAttrOutDestC8_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCapaAttrKeyOutDest.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CAPA_ATTR_OUT_DEST_C8_SIZE				2 + ( CDIFIN_CAPA_ATTR_OUT_DEST_C8_NUM_MEDIATYPE * 4 )
#define CDIFIN_CAPA_ATTR_OUT_DEST_C8_NUM_MEDIATYPE					11


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition CCdiFinCapaAttrOutDestC8_SetNumberOfSheets
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCapaAttrOutDestC8_GetAttributeKey() ((UC)eCdiFinCapaKeyOutDest_StplSpecMediaType)
																				// UC (void)
#define CCdiFinCapaAttrOutDestC8_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CAPA_ATTR_OUT_DEST_C8_SIZE)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDestC8_GetStplSpecMediaType(apAttrPair) ((apAttrPair)[1])	// UC (UC*)
#define CCdiFinCapaAttrOutDestC8_SetStplSpecMediaType(apAttrPair,aPosition, aValue) ((apAttrPair)[aPosition] = aValue)
#define CCdiFinCapaAttrOutDestC8_SetNumberOfSpecificMediaTypes(apAttrPair, aType) ((apAttrPair)[1] = CDIFIN_CAPA_ATTR_OUT_DEST_C8_NUM_MEDIATYPE)
#define CCdiFinCapaAttrOutDestC8_SetStplSpecMediaWeightUp(apAttrPair,aPosition, aValue) ((apAttrPair)[aPosition] &= ~(0x0F ));  (apAttrPair)[aPosition]  |= ((aValue) & 0x0F) 
#define CCdiFinCapaAttrOutDestC8_SetStplSpecMediaWeightDown(apAttrPair,aPosition, aValue) ((apAttrPair)[aPosition] = aValue)
#define CCdiFinCapaAttrOutDestC8_SetStplSpecMediaSizeCategory(apAttrPair,aPosition, aValue) ((apAttrPair)[aPosition]  &= ~(0x03 << 4)); (apAttrPair)[aPosition]  |= (((aValue) & 0x03) << 4)
// ((apAttrPair)[2] &= ~(0x01 << 0)); (apAttrPair)[2] |= (((aSupport) & 0x01) << 0)
#define CCdiFinCapaAttrOutDestC8_SetNumberOfSheets(apAttrPair,aPosition, aValue) ((apAttrPair)[aPosition] = aValue)

																				// void (UC*, UC)


#endif // !defined(CCdiFinCapaAttrOutDestC8_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
