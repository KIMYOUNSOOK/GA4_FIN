// File Name:	CCdiFinCfgAttrDevContFF.h
// Description:	Header File of Finisher Device Controller Configuration Attributes "No Paper Run(FFH)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCfgAttrDevContFF_h)
#define CCdiFinCfgAttrDevContFF_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyDevCont.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CFG_ATTR_DEV_CONT_FF_SIZE				2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCfgAttrDevContFF_GetAttributeKey() ((UC)eCdiFinCfgKeyDevCont_NoPprRun)
																				// UC (void)
#define CCdiFinCfgAttrDevContFF_GetAttributeSize(apAttrPair) (CDIFIN_CFG_ATTR_DEV_CONT_FF_SIZE)
																				// UC (UC*)
#define CCdiFinCfgAttrDevContFF_GetNoPaperRun(apAttrPair) (apAttrPair[1])		// UC (UC*)
#define CCdiFinCfgAttrDevContFF_SetNoPaperRun(apAttrPair, aNoPaperRun) (apAttrPair[1] = aNoPaperRun)
																				// void (UC*, UC)


#endif // !defined(CCdiFinCfgAttrDevContFF_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
