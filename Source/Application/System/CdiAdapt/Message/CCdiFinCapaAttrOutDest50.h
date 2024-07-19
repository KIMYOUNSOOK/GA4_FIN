// File Name:	CCdiFinCapaAttrOutDest50.h
// Description:	Header File of Output Destination Capabilities Attributes "Folding Ability(50H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.12.11
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCapaAttrOutDest50_h)
#define CCdiFinCapaAttrOutDest50_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCapaAttrKeyOutDest.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CAPA_ATTR_OUT_DEST_50_BASE_SIZE					7
#define CDIFIN_CAPA_ATTR_OUT_DEST_50_SET_SIZE					3


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCapaAttrOutDest50_GetAttributeKey() ((UC)eCdiFinCapaKeyOutDest_Staple)
																				// UC (void)
#define CCdiFinCapaAttrOutDest50_GetAttributeSize(apAttrPair) ( apAttrPair == NULL ? CDIFIN_CAPA_ATTR_OUT_DEST_50_BASE_SIZE : ((UC)(CDIFIN_CAPA_ATTR_OUT_DEST_50_BASE_SIZE + CDIFIN_CAPA_ATTR_OUT_DEST_50_SET_SIZE * ((apAttrPair)[6]))) )
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest50_GetAttributeSizeByNumberOfReported(aNumber) ((UC)(CDIFIN_CAPA_ATTR_OUT_DEST_50_BASE_SIZE + CDIFIN_CAPA_ATTR_OUT_DEST_50_SET_SIZE * aNumber))
																				// UC (UC)
#define CCdiFinCapaAttrOutDest50_GetSelectability(apAttrPair) ((apAttrPair)[1])	// UC (UC*)
#define CCdiFinCapaAttrOutDest50_SetSelectability(apAttrPair, aSelectability) ((apAttrPair)[1] = aSelectability)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest50_GetSingleStitch(apAttrPair) (((apAttrPair)[4] >> 0) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest50_SetSingleStitch(apAttrPair, aSupport) ((apAttrPair)[4] &= ~(0x01 << 0)); (apAttrPair)[4] |= (((aSupport) & 0x01) << 0)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest50_GetDoubleStitch(apAttrPair) (((apAttrPair)[4] >> 1) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest50_SetDoubleStitch(apAttrPair, aSupport) ((apAttrPair)[4] &= ~(0x01 << 1)); (apAttrPair)[4] |= (((aSupport) & 0x01) << 1)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest50_GetTripleStitch(apAttrPair) (((apAttrPair)[4] >> 2) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest50_SetTripleStitch(apAttrPair, aSupport) ((apAttrPair)[4] &= ~(0x01 << 2)); (apAttrPair)[4] |= (((aSupport) & 0x01) << 2)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest50_GetQuadStitch(apAttrPair) (((apAttrPair)[4] >> 3) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest50_SetQuadStitch(apAttrPair, aSupport) ((apAttrPair)[4] &= ~(0x01 << 3)); (apAttrPair)[4] |= (((aSupport) & 0x01) << 3)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest50_GetSingleRear(apAttrPair) (((apAttrPair)[4] >> 5) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest50_SetSingleRear(apAttrPair, aSupport) ((apAttrPair)[4] &= ~(0x01 << 5)); (apAttrPair)[4] |= (((aSupport) & 0x01) << 5)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest50_GetSingleFront(apAttrPair) (((apAttrPair)[4] >> 6) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest50_SetSingleFront(apAttrPair, aSupport) ((apAttrPair)[4] &= ~(0x01 << 6)); (apAttrPair)[4] |= (((aSupport) & 0x01) << 6)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest50_GetPinCut(apAttrPair) (((apAttrPair)[5] >> 0) & 0x01)
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest50_SetPinCut(apAttrPair, aSupport) ((apAttrPair)[5] &= ~(0x01 << 0)); (apAttrPair)[5] |= (((aSupport) & 0x01) << 0)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest50_GetNumberOfReported(apAttrPair) ((apAttrPair)[6])
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest50_SetNumberOfReported(apAttrPair, aNumber) ((apAttrPair)[6] = aNumber)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest50_GetMinNumOfShtPerSet1Set(apAttrPair) ((apAttrPair)[7])
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest50_SetMinNumOfShtPerSet1Set(apAttrPair, aNumber) ((apAttrPair)[7] = aNumber)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest50_GetMaxNumOfShtPerSet1Set(apAttrPair) ((apAttrPair)[8])
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest50_SetMaxNumOfShtPerSet1Set(apAttrPair, aNumber) ((apAttrPair)[8] = aNumber)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest50_GetTrayCapacity1Set(apAttrPair) ((apAttrPair)[9])
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest50_SetTrayCapacity1Set(apAttrPair, aCapacity) ((apAttrPair)[9] = aCapacity)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest50_GetMinNumOfShtPerSetSmallSize(apAttrPair) ((apAttrPair)[10])
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest50_SetMinNumOfShtPerSetSmallSize(apAttrPair, aNumber) ((apAttrPair)[10] = aNumber)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest50_GetMaxNumOfShtPerSetSmallSize(apAttrPair) ((apAttrPair)[11])
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest50_SetMaxNumOfShtPerSetSmallSize(apAttrPair, aNumber) ((apAttrPair)[11] = aNumber)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest50_GetTrayCapacitySmallSize(apAttrPair) ((apAttrPair)[12])
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest50_SetTrayCapacitySmallSize(apAttrPair, aCapacity) ((apAttrPair)[12] = aCapacity)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest50_GetMinNumOfShtPerSetLargeSize(apAttrPair) ((apAttrPair)[13])
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest50_SetMinNumOfShtPerSetLargeSize(apAttrPair, aNumber) ((apAttrPair)[13] = aNumber)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest50_GetMaxNumOfShtPerSetLargeSize(apAttrPair) ((apAttrPair)[14])
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest50_SetMaxNumOfShtPerSetLargeSize(apAttrPair, aNumber) ((apAttrPair)[14] = aNumber)
																				// void (UC*, UC)
#define CCdiFinCapaAttrOutDest50_GetTrayCapacityLargeSize(apAttrPair) ((apAttrPair)[15])
																				// UC (UC*)
#define CCdiFinCapaAttrOutDest50_SetTrayCapacityLargeSize(apAttrPair, aCapacity) ((apAttrPair)[15] = aCapacity)
																				// void (UC*, UC)


#endif // !defined(CCdiFinCapaAttrOutDest50_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.12.05	| Yuichi Hoshino	| Created this file
// 14.12.05	| Yuichi Hoshino	| Release V0 (Revision 00)
// 14.12.11	| Yuichi Hoshino	| Trouble Action [Single RearASingle Front BitˆÊ’u•s³]
// 15.03.30 | Yuichi Hoshino	| Release V1 (Revision 01)
//
