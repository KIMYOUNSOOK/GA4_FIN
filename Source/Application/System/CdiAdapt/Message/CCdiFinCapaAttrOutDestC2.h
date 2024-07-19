// File Name:	CCdiFinCapaAttrOutDestC2.h
// Description:	Header File of Output Destination Capabilities Attributes "Count Of Sheet In Stapled Set(C2H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCapaAttrOutDestC2_h)
#define CCdiFinCapaAttrOutDestC2_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCapaAttrKeyOutDest.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CAPA_ATTR_OUT_DEST_C2_SIZE					3


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCapaAttrOutDestC2_GetAttributeKey() ((UC)eCdiFinCapaKeyOutDest_CountInStplSet)
																				// UC (void)
#define CCdiFinCapaAttrOutDestC2_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CAPA_ATTR_OUT_DEST_C2_SIZE)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDestC2_GetCountOfSheetZFoldA3(apAttrPair) ((apAttrPair)[1])
																				// UC (UC*)
#define CCdiFinCapaAttrOutDestC2_SetCountOfSheetZFold(apAttrPair, aCount) ((apAttrPair)[1] = aCount)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDestC2_GetCountOfSheetZFoldExcept(apAttrPair) ((apAttrPair)[2])
																				// UC (UC*)
#define CCdiFinCapaAttrOutDestC2_SetCountOfSheetZFoldExcept(apAttrPair, aCount) ((apAttrPair)[2] = aCount)
																				// void (UC*, UC)


#endif // !defined(CCdiFinCapaAttrOutDestC2_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
