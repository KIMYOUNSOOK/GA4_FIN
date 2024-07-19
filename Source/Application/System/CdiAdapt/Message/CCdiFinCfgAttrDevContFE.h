// File Name:	CCdiFinCfgAttrDevContFE.h
// Description:	Header File of Finisher Device Controller Configuration Attributes "Jam Bypass(FEH)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCfgAttrDevContFE_h)
#define CCdiFinCfgAttrDevContFE_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyDevCont.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CFG_ATTR_DEV_CONT_FE_SIZE				2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCfgAttrDevContFE_GetAttributeKey() ((UC)eCdiFinCfgKeyDevCont_JamBypass)
																				// UC (void)
#define CCdiFinCfgAttrDevContFE_GetAttributeSize(apAttrPair) (CDIFIN_CFG_ATTR_DEV_CONT_FE_SIZE)
																				// UC (UC*)
#define CCdiFinCfgAttrDevContFE_GetJamBypass(apAttrPair) (apAttrPair[1])		// UC (UC*)
#define CCdiFinCfgAttrDevContFE_SetJamBypass(apAttrPair, aBypass) (apAttrPair[1] = aBypass)
																				// void (UC*, UC)


#endif // !defined(CCdiFinCfgAttrDevContFE_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
