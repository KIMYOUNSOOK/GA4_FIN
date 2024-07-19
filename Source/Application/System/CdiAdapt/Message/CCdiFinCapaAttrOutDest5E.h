// File Name:	CCdiFinCapaAttrOutDest5E.h
// Description:	Header File of Output Destination Capabilities Attributes "Booklet Maker Tray Capacity(5EH)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCapaAttrOutDest5E_h)
#define CCdiFinCapaAttrOutDest5E_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCapaAttrKeyOutDest.h"

#include "ECdiFinTypeBookletReport.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CAPA_ATTR_OUT_DEST_5E_BASE_SIZE					2
#define CDIFIN_CAPA_ATTR_OUT_DEST_5E_SET_SIZE					5


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCapaAttrOutDest5E_GetAttributeKey() ((UC)eCdiFinCapaKeyOutDest_BookTrayCapa)
																				// UC (void)
#define CCdiFinCapaAttrOutDest5E_GetAttributeSize(apAttrPair) ( apAttrPair == NULL ? CDIFIN_CAPA_ATTR_OUT_DEST_5E_BASE_SIZE : ((UC)(CDIFIN_CAPA_ATTR_OUT_DEST_5E_BASE_SIZE + CDIFIN_CAPA_ATTR_OUT_DEST_5E_SET_SIZE * ((apAttrPair)[1]))) )
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest5E_GetAttributeSizeByNumberOfReported(aNumber) ((UC)(CDIFIN_CAPA_ATTR_OUT_DEST_5E_BASE_SIZE + CDIFIN_CAPA_ATTR_OUT_DEST_5E_SET_SIZE * aNumber))
																				// UC (UC)
#define CCdiFinCapaAttrOutDest5E_GetNumberOfReported(apAttrPair) ((apAttrPair)[1])
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest5E_SetNumberOfReported(apAttrPair, aNumber) ((apAttrPair)[1] = aNumber)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest5E_GetTypeOfReported(apAttrPair, aNo) ((apAttrPair)[CDIFIN_CAPA_ATTR_OUT_DEST_5E_BASE_SIZE + CDIFIN_CAPA_ATTR_OUT_DEST_5E_SET_SIZE * (aNo - 1) + 0])
																				// UC (UC*, UC)
#define CCdiFinCapaAttrOutDest5E_SetTypeOfReported(apAttrPair, aType, aNo) ((apAttrPair)[CDIFIN_CAPA_ATTR_OUT_DEST_5E_BASE_SIZE + CDIFIN_CAPA_ATTR_OUT_DEST_5E_SET_SIZE * (aNo - 1) + 0] = aType)
																				// void (UC*, UC, UC)
#define CCdiFinCapaAttrOutDest5E_GetMinNumOfShtPerBooklet(apAttrPair, aNo) ((apAttrPair)[CDIFIN_CAPA_ATTR_OUT_DEST_5E_BASE_SIZE + CDIFIN_CAPA_ATTR_OUT_DEST_5E_SET_SIZE * (aNo - 1) + 1])
																				// UC (UC*, UC)
#define CCdiFinCapaAttrOutDest5E_SetMinNumOfShtPerBooklet(apAttrPair, aNumber, aNo) ((apAttrPair)[CDIFIN_CAPA_ATTR_OUT_DEST_5E_BASE_SIZE + CDIFIN_CAPA_ATTR_OUT_DEST_5E_SET_SIZE * (aNo - 1) + 1] = aNumber)
																				// void (UC*, UC, UC)
#define CCdiFinCapaAttrOutDest5E_GetMaxNumOfShtPerBooklet(apAttrPair, aNo) ((apAttrPair)[CDIFIN_CAPA_ATTR_OUT_DEST_5E_BASE_SIZE + CDIFIN_CAPA_ATTR_OUT_DEST_5E_SET_SIZE * (aNo - 1) + 2])
																				// UC (UC*, UC)
#define CCdiFinCapaAttrOutDest5E_SetMaxNumOfShtPerBooklet(apAttrPair, aNumber, aNo) ((apAttrPair)[CDIFIN_CAPA_ATTR_OUT_DEST_5E_BASE_SIZE + CDIFIN_CAPA_ATTR_OUT_DEST_5E_SET_SIZE * (aNo - 1) + 2] = aNumber)
																				// void (UC*, UC, UC)
#define CCdiFinCapaAttrOutDest5E_GetBookletTrayCapacity(apAttrPair, aNo) ( ((US)((apAttrPair)[CDIFIN_CAPA_ATTR_OUT_DEST_5E_BASE_SIZE + CDIFIN_CAPA_ATTR_OUT_DEST_5E_SET_SIZE * (aNo - 1) + 3]) << 8) | (US)((apAttrPair)[CDIFIN_CAPA_ATTR_OUT_DEST_5E_BASE_SIZE + CDIFIN_CAPA_ATTR_OUT_DEST_5E_SET_SIZE * (aNo - 1) + 4]) )
																				// US (UC*, UC)
#define CCdiFinCapaAttrOutDest5E_SetBookletTrayCapacity(apAttrPair, aCapacity, aNo) (apAttrPair)[CDIFIN_CAPA_ATTR_OUT_DEST_5E_BASE_SIZE + CDIFIN_CAPA_ATTR_OUT_DEST_5E_SET_SIZE * (aNo - 1) + 3] = (UC)((US)(aCapacity) >> 8); (apAttrPair)[CDIFIN_CAPA_ATTR_OUT_DEST_5E_BASE_SIZE + CDIFIN_CAPA_ATTR_OUT_DEST_5E_SET_SIZE * (aNo - 1) + 4] = (UC)((US)(aCapacity))
																				// void (UC*, US, UC)


#endif // !defined(CCdiFinCapaAttrOutDest5E_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
