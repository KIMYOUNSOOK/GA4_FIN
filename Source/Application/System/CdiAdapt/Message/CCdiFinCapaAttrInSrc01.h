// File Name:	CCdiFinCapaAttrInSrc01.h
// Description:	Header File of Input Source Capabilities Attributes "Input Max Capacity(01H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCapaAttrInSrc01_h)
#define CCdiFinCapaAttrInSrc01_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCapaAttrKeyInSrc.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CAPA_ATTR_IN_SRC_01_SIZE						3


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCapaAttrInSrc01_GetAttributeKey() ((UC)eCdiFinCapaKeyInSrc_MaxCapa)
																				// UC (void)
#define CCdiFinCapaAttrInSrc01_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CAPA_ATTR_IN_SRC_01_SIZE)
																				// UC (UC*)
#define CCdiFinCapaAttrInSrc01_GetInputMaxCapacity(apAttrPair) ( ((US)((apAttrPair)[1]) << 8) | (US)((apAttrPair)[2]) )
																				// US (UC*)
#define CCdiFinCapaAttrInSrc01_SetInputMaxCapacity(apAttrPair, aCapacity) (apAttrPair)[1] = (UC)((US)(aCapacity) >> 8); (apAttrPair)[2] = (UC)((US)(aCapacity))
																				// void (UC*, US)


#endif // !defined(CCdiFinCapaAttrInSrc01_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
