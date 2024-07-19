// File Name:	CCdiFinOutDestConfigurationAttr.h
// Description:	Header File of Output Destination Configuration
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.25
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinOutDestConfigurationAttr_h)
#define CCdiFinOutDestConfigurationAttr_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyOutDest.h"

#include "CCdiFinCfgAttrOutDest03.h"
#include "CCdiFinCfgAttrOutDest45.h"
#include "CCdiFinCfgAttrOutDest46.h"
#include "CCdiFinCfgAttrOutDest47.h"
#include "CCdiFinCfgAttrOutDest48.h"
#include "CCdiFinCfgAttrOutDest49.h"

#include "CCdiFinMessage.h"
#include "CCdiFinAttrPairs.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Output Destination Configuration ---
typedef struct /* CCdiFinOutDestConfigurationAttr */
{
	CCdiFinAttrPairs mSuper;
} CCdiFinOutDestConfigurationAttr;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
#define CCdiFinOutDestConfigurationAttr_Constructor(pThis) CCdiFinAttrPairs_Constructor(&((pThis)->mSuper))
																				// void (CCdiFinOutDestConfigurationAttr*)

#define CCdiFinOutDestConfigurationAttr_Reset(pThis); CCdiFinAttrPairs_Reset(&((pThis)->mSuper))
																				// void (CCdiFinOutDestConfigurationAttr*)
#define CCdiFinOutDestConfigurationAttr_GetAttrPairsPointer(pThis) ((UC*)((pThis)->mpAttrData))
																				// UC* (CCdiFinOutDestConfigurationAttr*)
#define CCdiFinOutDestConfigurationAttr_SetAttrPairsPointer(pThis, pAttr, size) CCdiFinAttrPairs_SetAttrPairsPointer(&((pThis)->mSuper), pAttr, size)
																				// void (CCdiFinOutDestConfigurationAttr*, UC*, UC)
void CCdiFinOutDestConfigurationAttr_AnalyzeAttrPairs(CCdiFinOutDestConfigurationAttr* pThis, UC numAttr);
#define CCdiFinOutDestConfigurationAttr_CheckAttrSet(pThis, key) CCdiFinAttrPairs_CheckAttrSet(&((pThis)->mSuper), key)
																				// bool (CCdiFinOutDestConfigurationAttr*, UC)
#define CCdiFinOutDestConfigurationAttr_AllocateAttrPair(pThis, key, size) CCdiFinAttrPairs_AllocateAttrPair(&((pThis)->mSuper), key, size)
																				// bool (CCdiFinOutDestConfigurationAttr*, UC, UC)
#define CCdiFinOutDestConfigurationAttr_GetAttributePointer(pThis, key) CCdiFinAttrPairs_GetAttributePointer(&((pThis)->mSuper), key)
																				// UC* (CCdiFinOutDestConfigurationAttr*, UC)
#define CCdiFinOutDestConfigurationAttr_GetAttrPairsSize(pThis) CCdiFinAttrPairs_GetAttrPairsSize(&((pThis)->mSuper))
																				// UC (CCdiFinOutDestConfigurationAttr*)
#define CCdiFinOutDestConfigurationAttr_GetNumAttr(pThis) CCdiFinAttrPairs_GetNumAttr(&((pThis)->mSuper))
																				// UC (CCdiFinOutDestConfigurationAttr*)


#endif // !defined(CCdiFinOutDestConfigurationAttr_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
