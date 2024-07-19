// File Name:	CCdiFinCapaAttrDevCont03.h
// Description:	Header File of Finisher Device Controller Capabilities Attributes "Device Controller Software Version(03H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCapaAttrDevCont03_h)
#define CCdiFinCapaAttrDevCont03_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCapaAttrKeyDevCont.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CAPA_ATTR_DEV_CONT_03_SIZE					4


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCapaAttrDevCont03_GetAttributeKey() ((UC)eCdiFinCapaKeyDevCont_SoftVersion)
																				// UC (void)
#define CCdiFinCapaAttrDevCont03_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CAPA_ATTR_DEV_CONT_03_SIZE)
																				// UC (UC*)
#define CCdiFinCapaAttrDevCont03_GetMajorVersion(apAttrPair) ((apAttrPair)[1])	// UC (UC*)
#define CCdiFinCapaAttrDevCont03_SetMajorVersion(apAttrPair, aVersion) ((apAttrPair)[1] = (aVersion))
																				// void (UC*)
#define CCdiFinCapaAttrDevCont03_GetMinorVersion(apAttrPair) ((apAttrPair)[2])	// UC (UC*)
#define CCdiFinCapaAttrDevCont03_SetMinorVersion(apAttrPair, aVersion) ((apAttrPair)[2] = (aVersion))
																				// void (UC*)
#define CCdiFinCapaAttrDevCont03_GetPatchVersion(apAttrPair) ((apAttrPair)[3])	// UC (UC*)
#define CCdiFinCapaAttrDevCont03_SetPatchVersion(apAttrPair, aVersion) ((apAttrPair)[3] = (aVersion))
																				// void (UC*)


#endif // !defined(CCdiFinCapaAttrDevCont03_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
