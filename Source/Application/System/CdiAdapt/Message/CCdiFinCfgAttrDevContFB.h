// File Name:	CCdiFinCfgAttrDevContFB.h
// Description:	Header File of Finisher Device Controller Configuration Attributes "Productivity(FBH)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCfgAttrDevContFB_h)
#define CCdiFinCfgAttrDevContFB_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyDevCont.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CFG_ATTR_DEV_CONT_FB_BASE_SIZE				2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCfgAttrDevContFB_GetAttributeKey() ((UC)eCdiFinCfgKeyDevCont_Productivity)
																				// UC (void)
#define CCdiFinCfgAttrDevContFB_GetAttributeSize(apAttrPair) ( apAttrPair == NULL ? CDIFIN_CFG_ATTR_DEV_CONT_FB_BASE_SIZE : ((UC)CDIFIN_CFG_ATTR_DEV_CONT_FB_BASE_SIZE + (apAttrPair)[1]) )
																				// UC (UC*)
#define CCdiFinCfgAttrDevContFB_GetAttributeSizeByNumberOfReported(aNumber) ((UC)CDIFIN_CFG_ATTR_DEV_CONT_FB_BASE_SIZE + aNumber)
																				// UC (UC)
#define CCdiFinCfgAttrDevContFB_GetNumberOfReported(apAttrPair) ((apAttrPair)[1])
																				// UC (UC*)
#define CCdiFinCfgAttrDevContFB_SetNumberOfReported(apAttrPair, aNumber) ((apAttrPair)[1] = aNumber)
																				// void (UC*, UC)
#define CCdiFinCfgAttrDevContFB_GetProductivity(apAttrPair) ((apAttrPair)[2])	// UC (UC*)
#define CCdiFinCfgAttrDevContFB_SetProductivity(apAttrPair, aProductivity) ((apAttrPair)[2] = aProductivity)
																				// void (UC*, UC)


#endif // !defined(CCdiFinCfgAttrDevContFB_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
