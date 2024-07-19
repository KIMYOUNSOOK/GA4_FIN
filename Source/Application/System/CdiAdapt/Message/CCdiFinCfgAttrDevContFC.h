// File Name:	CCdiFinCfgAttrDevContFC.h
// Description:	Header File of Finisher Device Controller Configuration Attributes "Exit Speed Range(FCH)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCfgAttrDevContFC_h)
#define CCdiFinCfgAttrDevContFC_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyDevCont.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CFG_ATTR_DEV_CONT_FC_BASE_SIZE				2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCfgAttrDevContFC_GetAttributeKey() ((UC)eCdiFinCfgKeyDevCont_ExitSpdRange)
																				// UC (void)
#define CCdiFinCfgAttrDevContFC_GetAttributeSize(apAttrPair) ( apAttrPair == NULL ? CDIFIN_CFG_ATTR_DEV_CONT_FC_BASE_SIZE : ((UC)CDIFIN_CFG_ATTR_DEV_CONT_FC_BASE_SIZE + (apAttrPair)[1]) )
																				// UC (UC*)
#define CCdiFinCfgAttrDevContFC_GetAttributeSizeByDataLength(aLength) ((UC)CDIFIN_CFG_ATTR_DEV_CONT_FC_BASE_SIZE + aLength)
																				// UC (UC*)
#define CCdiFinCfgAttrDevContFC_GetMinimumExitSpeed(apAttrPair) ( ((US)((apAttrPair)[2]) << 8) | (US)((apAttrPair)[3]) )
																				// US (UC*)
#define CCdiFinCfgAttrDevContFC_SetMinimumExitSpeed(apAttrPair, aSpeed) (apAttrPair)[2] = (UC)((US)(aSpeed) >> 8); (apAttrPair)[3] = (UC)((US)(aSpeed))
																				// void (UC*, US)
#define CCdiFinCfgAttrDevContFC_GetMaximumExitSpeed(apAttrPair) ( ((US)((apAttrPair)[4]) << 8) | (US)((apAttrPair)[5]) )
																				// US (UC*)
#define CCdiFinCfgAttrDevContFC_SetMaximumExitSpeed(apAttrPair, aSpeed) (apAttrPair)[4] = (UC)((US)(aSpeed) >> 8); (apAttrPair)[5] = (UC)((US)(aSpeed))
																				// void (UC*, US)


#endif // !defined(CCdiFinCfgAttrDevContFC_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
