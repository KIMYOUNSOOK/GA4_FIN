// File Name:	CCdiFinCfgAttrOutDest48.h
// Description:	Header File of Output Destination Configuration Attributes "Current Punch Configuration(48H)"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.16
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCfgAttrOutDest48_h)
#define CCdiFinCfgAttrOutDest48_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyOutDest.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_CFG_ATTR_OUT_DEST_48_SIZE				2


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinCfgAttrOutDest48_GetAttributeKey() ((UC)eCdiFinCfgKeyOutDest_Punch)
																				// UC (void)
#define CCdiFinCfgAttrOutDest48_GetAttributeSize(apAttrPair) ((UC)CDIFIN_CFG_ATTR_OUT_DEST_48_SIZE)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest48_GetDoublePunch(apAttrPair) (((apAttrPair)[1] >> 0) & 0x01)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest48_SetDoublePunch(apAttrPair, aSelect) (apAttrPair)[1] &= ~(0x01 << 0); (apAttrPair)[1] |= (((aSelect) & 0x01) << 0)
																				// void (UC*, UC)
#define CCdiFinCfgAttrOutDest48_GetTriplePunch(apAttrPair) (((apAttrPair)[1] >> 1) & 0x01)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest48_SetTriplePunch(apAttrPair, aSelect) (apAttrPair)[1] &= ~(0x01 << 1); (apAttrPair)[1] |= (((aSelect) & 0x01) << 1)
																				// void (UC*, UC)
#define CCdiFinCfgAttrOutDest48_GetQuadPunch(apAttrPair) (((apAttrPair)[1] >> 2) & 0x01)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest48_SetQuadPunch(apAttrPair, aSelect) (apAttrPair)[1] &= ~(0x01 << 2); (apAttrPair)[1] |= (((aSelect) & 0x01) << 2)
																				// void (UC*, UC)
#define CCdiFinCfgAttrOutDest48_GetMultiPunch(apAttrPair) (((apAttrPair)[1] >> 3) & 0x01)
																				// UC (UC*)
#define CCdiFinCfgAttrOutDest48_SetMultiPunch(apAttrPair, aSelect) (apAttrPair)[1] &= ~(0x01 << 3); (apAttrPair)[1] |= (((aSelect) & 0x01) << 3)
																				// void (UC*, UC)


#endif // !defined(CCdiFinCfgAttrOutDest48_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
