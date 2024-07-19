// File Name:	CCdiFinCfgAttrDevContFD.h
// Description:	Header File of Finisher Device Controller Configuration Attributes "Speed Exit Layout(FDH)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCfgAttrDevContFD_h)
#define CCdiFinCfgAttrDevContFD_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyDevCont.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CFG_ATTR_DEV_CONT_FD_BASE_SIZE				2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCfgAttrDevContFD_GetAttributeKey() ((UC)eCdiFinCfgKeyDevCont_ExitLayout)
																				// UC (void)
#define CCdiFinCfgAttrDevContFD_GetAttributeSize(apAttrPair) ( apAttrPair == NULL ? CDIFIN_CFG_ATTR_DEV_CONT_FD_BASE_SIZE : ((UC)CDIFIN_CFG_ATTR_DEV_CONT_FD_BASE_SIZE + (apAttrPair)[1]) )
																				// UC (UC*)
#define CCdiFinCfgAttrDevContFD_GetAttributeSizeByDataLength(aLength) ((UC)CDIFIN_CFG_ATTR_DEV_CONT_FD_BASE_SIZE + aLength)
																				// UC (UC)
#define CCdiFinCfgAttrDevContFD_GetDataLength(apAttrPair) (apAttrPair[1])		// UC (UC*)
#define CCdiFinCfgAttrDevContFD_SetDataLength(apAttrPair, aLength) (apAttrPair[1] = aLength)
																				// void (UC*, UC)
#define CCdiFinCfgAttrDevContFD_GetExitHotLineDistance(apAttrPair) ( ((US)((apAttrPair)[2]) << 8) | (US)((apAttrPair)[3]) )
																				// US (UC*)
#define CCdiFinCfgAttrDevContFD_SetExitHotLineDistance(apAttrPair, aDistance) (apAttrPair)[2] = (UC)((US)(aDistance) >> 8); (apAttrPair)[3] = (UC)((US)(aDistance))
																				// void (UC*, US)
#define CCdiFinCfgAttrDevContFD_GetReleasePointDistance(apAttrPair) ( ((US)((apAttrPair)[4]) << 8) | (US)((apAttrPair)[5]) )
																				// US (UC*)
#define CCdiFinCfgAttrDevContFD_SetReleasePointDistance(apAttrPair, aDistance) (apAttrPair)[4] = (UC)((US)(aDistance) >> 8); (apAttrPair)[5] = (UC)((US)(aDistance))
																				// void (UC*, US)


#endif // !defined(CCdiFinCfgAttrDevContFD_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
