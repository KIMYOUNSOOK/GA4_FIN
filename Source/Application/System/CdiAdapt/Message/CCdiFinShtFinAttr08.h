// File Name:	CCdiFinShtFinAttr08.h
// Description:	Header File of Sheet Finish Attributes "Media Type(08H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.13
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinShtFinAttr08_h)
#define CCdiFinShtFinAttr08_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinShtFinAttrKey.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_SHT_FIN_ATTR_08_SIZE	5


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinShtFinAttr08_GetAttributeKey() ((UC)eCdiFinShtFinAttr_MediaType)	// UC (void)
#define CCdiFinShtFinAttr08_GetAttributeSize(apAttrPair) ((UC)CDIFIN_SHT_FIN_ATTR_08_SIZE)
																				// UC (UC*)
#define CCdiFinShtFinAttr08_GetMediaType(apAttrPair) (0x0F & apAttrPair[1])		// UC (UC*)
#define CCdiFinShtFinAttr08_SetMediaType(apAttrPair, aType) (apAttrPair[1] = (apAttrPair[1] & 0xF0) | (aType & 0x0F))
																				// void (UC*, UC)
#define CCdiFinShtFinAttr08_GetMediaWeight(apAttrPair) (CF2_US(&(apAttrPair[2])) & 0x0FFF)
																				// US (UC*)
#define CCdiFinShtFinAttr08_SetMediaWeight(apAttrPair, aWeight) (apAttrPair)[2] &= 0xF0; (apAttrPair)[2] |= (UC)(weight >> 8) & (0x0F); (apAttrPair)[3] = (UC)weight
																				// void (UC*, US)
#define CCdiFinShtFinAttr08_GetEnvelope(apAttrPair) ((apAttrPair[4] >> 0) & 0x01)
																				// UC (UC*)
#define CCdiFinShtFinAttr08_SetEnvelope(apAttrPair, aEnv) (apAttrPair[4] = (apAttrPair[4] & ~(0x01 << 0)) | ((aEnv & 0x01) << 0))
																				// void (UC*, UC)
#define CCdiFinShtFinAttr08_GetTabbedPaper(apAttrPair) ((apAttrPair[4] >> 1) & 0x01)
																				// UC (UC*)
#define CCdiFinShtFinAttr08_SetTabbedPaper(apAttrPair, aTabbed) (apAttrPair[4] = (apAttrPair[4] & ~(0x01 << 1)) | ((aTabbed & 0x01) << 1))
																				// void (UC*, UC)
#define CCdiFinShtFinAttr08_GetReusedPaper(apAttrPair) ((apAttrPair[4] >> 2) & 0x01)
																				// UC (UC*)
#define CCdiFinShtFinAttr08_SetReusedPaper(apAttrPair, aReused) (apAttrPair[4] = (apAttrPair[4] & ~(0x01 << 2)) | ((aReused & 0x01) << 2))
																				// void (UC*, UC)
#define CCdiFinShtFinAttr08_GetPrepunchedMaterial(apAttrPair) ((apAttrPair[4] >> 3) & 0x01)
																				// UC (UC*)
#define CCdiFinShtFinAttr08_SetPrepunchedMaterial(apAttrPair, aPrePunch) (apAttrPair[4] = (apAttrPair[4] & ~(0x01 << 3)) | ((aPrePunch & 0x01) << 3))
																				// void (UC*, UC)


#endif // !defined(CCdiFinShtFinAttr08_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
