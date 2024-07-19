// File Name:	CCdiFinCfgAttrDevContC0.h
// Description:	Header File of Finisher Device Controller Configuration Attributes "Media Size Group(C0H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCfgAttrDevContC0_h)
#define CCdiFinCfgAttrDevContC0_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyDevCont.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CFG_ATTR_DEV_CONT_C0_SIZE				3


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCfgAttrDevContC0_GetAttributeKey() ((UC)eCdiFinCfgKeyDevCont_MediaSizeGrp)
																				// UC (void)
#define CCdiFinCfgAttrDevContC0_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CFG_ATTR_DEV_CONT_C0_SIZE)
																				// UC (UC*)
#define CCdiFinCfgAttrDevContC0_GetMediaSieGroup(apAttrPair) (apAttrPair[1])	// UC (UC*)
#define CCdiFinCfgAttrDevContC0_SetMediaSieGroup(apAttrPair, aGroup) (apAttrPair[1] = aGroup)
																				// void (UC*)
#define CCdiFinCfgAttrDevContC0_GetSelect8K16K(apAttrPair) (apAttrPair[2])		// UC (UC*)
#define CCdiFinCfgAttrDevContC0_SetSelect8K16K(apAttrPair, aSelect) (apAttrPair[2] = aSelect)
																				// void (UC*)


#endif // !defined(CCdiFinCfgAttrDevContC0_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
