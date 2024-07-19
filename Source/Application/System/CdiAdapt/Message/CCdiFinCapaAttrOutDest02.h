// File Name:	CCdiFinCapaAttrOutDest02.h
// Description:	Header File of Output Destination Capabilities Attributes "Output Max Capacity(02H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCapaAttrOutDest02_h)
#define CCdiFinCapaAttrOutDest02_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCapaAttrKeyOutDest.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CAPA_ATTR_OUT_DEST_02_SIZE					7


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCapaAttrOutDest02_GetAttributeKey() ((UC)eCdiFinCapaKeyOutDest_MaxCapa)
																				// UC (void)
#define CCdiFinCapaAttrOutDest02_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CAPA_ATTR_OUT_DEST_02_SIZE)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest02_GetMaxSmallSizeCapacity(apAttrPair) ( ((US)((apAttrPair)[1]) << 8) | (US)((apAttrPair)[2]) )
																				// US (UC*)
#define CCdiFinCapaAttrOutDest02_SetMaxSmallSizeCapacity(apAttrPair, aCapacity) (apAttrPair)[1] = (UC)((US)(aCapacity) >> 8); (apAttrPair)[2] = (UC)((US)(aCapacity))
																				// void (UC*, US)
#define CCdiFinCapaAttrOutDest02_GetMaxLargeSizeCapacity(apAttrPair) ( ((US)((apAttrPair)[3]) << 8) | (US)((apAttrPair)[4]) )
																				// US (UC*)
#define CCdiFinCapaAttrOutDest02_SetMaxLargeSizeCapacity(apAttrPair, aCapacity) (apAttrPair)[3] = (UC)((US)(aCapacity) >> 8); (apAttrPair)[4] = (UC)((US)(aCapacity))
																				// void (UC*, US)
#define CCdiFinCapaAttrOutDest02_GetMixedSizeCapacity(apAttrPair) ( ((US)((apAttrPair)[5]) << 8) | (US)((apAttrPair)[6]) )
																				// US (UC*)
#define CCdiFinCapaAttrOutDest02_SetMixedSizeCapacity(apAttrPair, aCapacity) (apAttrPair)[5] = (UC)((US)(aCapacity) >> 8); (apAttrPair)[6] = (UC)((US)(aCapacity))
																				// void (UC*, US)


#endif // !defined(CCdiFinCapaAttrOutDest02_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
