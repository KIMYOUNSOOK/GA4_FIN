// File Name:	CCdiFinCfgAttrDevCont01.h
// Description:	Header File of Finisher Device Controller Configuration Attributes "Market Region(01H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCfgAttrDevCont01_h)
#define CCdiFinCfgAttrDevCont01_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyDevCont.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CFG_ATTR_DEV_CONT_01_SIZE					2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCfgAttrDevCont01_GetAttributeKey() ((UC)eCdiFinCfgKeyDevCont_Market)
																				// UC (void)
#define CCdiFinCfgAttrDevCont01_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CFG_ATTR_DEV_CONT_01_SIZE)
																				// UC (UC*)
#define CCdiFinCfgAttrDevCont01_GetMarketRegion(apAttrPair) (apAttrPair[1])		// UC (UC*)
#define CCdiFinCfgAttrDevCont01_SetMarketRegion(apAttrPair, aRegion) (apAttrPair[1] = aRegion)
																				// void (UC*)


#endif // !defined(CCdiFinCfgAttrDevCont01_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
