// File Name:	CCdiFinCfgAttrDevCont41.h
// Description:	Header File of Finisher Device Controller Configuration Attributes "System Configuration(41H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCfgAttrDevCont41_h)
#define CCdiFinCfgAttrDevCont41_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyDevCont.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CFG_ATTR_DEV_CONT_41_SIZE				2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCfgAttrDevCont41_GetAttributeKey() ((UC)eCdiFinCfgKeyDevCont_SysCfg)
																				// UC (void)
#define CCdiFinCfgAttrDevCont41_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CFG_ATTR_DEV_CONT_41_SIZE)
																				// UC (UC*)
#define CCdiFinCfgAttrDevCont41_GetSystemConfiguration(apAttrPair) (apAttrPair[1])
																				// UC (UC*)
#define CCdiFinCfgAttrDevCont41_SetSystemConfiguration(apAttrPair, aCfg) (apAttrPair[1] = aCfg)
																				// void (UC*)


#endif // !defined(CCdiFinCfgAttrDevCont41_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
