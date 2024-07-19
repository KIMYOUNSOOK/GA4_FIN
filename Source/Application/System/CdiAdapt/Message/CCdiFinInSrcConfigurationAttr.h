// File Name:	CCdiFinInSrcConfigurationAttr.h
// Description:	Header File of Input Source Configuration
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.30
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinInSrcConfigurationAttr_h)
#define CCdiFinInSrcConfigurationAttr_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyInSrc.h"

#include "CCdiFinCfgAttrInSrc03.h"
#include "CCdiFinCfgAttrInSrcC0.h"
#include "CCdiFinCfgAttrInSrcC1.h"

#include "CCdiFinMessage.h"
#include "CCdiFinAttrPairs.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Finisher Device Controller Configuration ---
typedef struct /* CCdiFinInSrcConfigurationAttr */
{
	CCdiFinAttrPairs mSuper;
} CCdiFinInSrcConfigurationAttr;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
#define CCdiFinInSrcConfigurationAttr_Constructor(pThis) CCdiFinAttrPairs_Constructor(&((pThis)->mSuper))
																				// void (CCdiFinInSrcConfigurationAttr*)

#define CCdiFinInSrcConfigurationAttr_Reset(pThis); CCdiFinAttrPairs_Reset(&((pThis)->mSuper))
																				// void (CCdiFinInSrcConfigurationAttr*)
#define CCdiFinInSrcConfigurationAttr_GetAttrPairsPointer(pThis) ((UC*)((pThis)->mpAttrData))
																				// UC* (CCdiFinInSrcConfigurationAttr*)
#define CCdiFinInSrcConfigurationAttr_SetAttrPairsPointer(pThis, pAttr, size) CCdiFinAttrPairs_SetAttrPairsPointer(&((pThis)->mSuper), pAttr, size)
																				// void (CCdiFinInSrcConfigurationAttr*, UC*, UC)
void CCdiFinInSrcConfigurationAttr_AnalyzeAttrPairs(CCdiFinInSrcConfigurationAttr* pThis, UC numAttr);
#define CCdiFinInSrcConfigurationAttr_CheckAttrSet(pThis, key) CCdiFinAttrPairs_CheckAttrSet(&((pThis)->mSuper), key)
																				// bool (CCdiFinInSrcConfigurationAttr*, UC)
#define CCdiFinInSrcConfigurationAttr_AllocateAttrPair(pThis, key, size) CCdiFinAttrPairs_AllocateAttrPair(&((pThis)->mSuper), key, size)
																				// bool (CCdiFinInSrcConfigurationAttr*, UC, UC)
#define CCdiFinInSrcConfigurationAttr_GetAttributePointer(pThis, key) CCdiFinAttrPairs_GetAttributePointer(&((pThis)->mSuper), key)
																				// UC* (CCdiFinInSrcConfigurationAttr*, UC)
#define CCdiFinInSrcConfigurationAttr_GetAttrPairsSize(pThis) CCdiFinAttrPairs_GetAttrPairsSize(&((pThis)->mSuper))
																				// UC (CCdiFinInSrcConfigurationAttr*)
#define CCdiFinInSrcConfigurationAttr_GetNumAttr(pThis) CCdiFinAttrPairs_GetNumAttr(&((pThis)->mSuper))
																				// UC (CCdiFinInSrcConfigurationAttr*)


#endif // !defined(CCdiFinInSrcConfigurationAttr_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
