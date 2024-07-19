// File Name:	CCdiFinCfgAttrDevContFA.h
// Description:	Header File of Finisher Device Controller Configuration Attributes "Sheet Exit Height(FAH)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCfgAttrDevContFA_h)
#define CCdiFinCfgAttrDevContFA_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyDevCont.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CFG_ATTR_DEV_CONT_FA_SIZE				3


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCfgAttrDevContFA_GetAttributeKey() ((UC)eCdiFinCfgKeyDevCont_ShtExtHeight)
																				// UC (void)
#define CCdiFinCfgAttrDevContFA_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CFG_ATTR_DEV_CONT_FA_SIZE)
																				// UC (UC*)
#define CCdiFinCfgAttrDevContFA_GetSheetExitHeight(apAttrPair) ( ((US)((apAttrPair)[1]) << 8) | (US)((apAttrPair)[2]) )
																				// US (UC*)
#define CCdiFinCfgAttrDevContFA_SetSheetExitHeight(apAttrPair, aHeight) (apAttrPair)[1] = (UC)((US)(aHeight) >> 8); (apAttrPair)[2] = (UC)((US)(aHeight))
																				// void (UC*, US)


#endif // !defined(CCdiFinCfgAttrDevContFA_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
