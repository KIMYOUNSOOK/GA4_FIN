// File Name:	CCdiFinInSrcCapabilitiesAttr.h
// Description:	Header File of Input Source Capabilities
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinInSrcCapabilitiesAttr_h)
#define CCdiFinInSrcCapabilitiesAttr_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "ECdiFinCapaAttrKeyInSrc.h"

#include "CCdiFinCapaAttrInSrc01.h"
#include "CCdiFinCapaAttrInSrc04.h"

#include "CCdiFinMessage.h"
#include "CCdiFinAttrPairs.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Finisher Device Controller Configuration ---
typedef struct /* CCdiFinInSrcCapabilitiesAttr */
{
	CCdiFinAttrPairs mSuper;
} CCdiFinInSrcCapabilitiesAttr;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
#define CCdiFinInSrcCapabilitiesAttr_Constructor(pThis) CCdiFinAttrPairs_Constructor(&((pThis)->mSuper))
																				// void (CCdiFinInSrcCapabilitiesAttr*)

#define CCdiFinInSrcCapabilitiesAttr_Reset(pThis); CCdiFinAttrPairs_Reset(&((pThis)->mSuper))
																				// void (CCdiFinInSrcCapabilitiesAttr*)
#define CCdiFinInSrcCapabilitiesAttr_GetAttrPairsPointer(pThis) ((UC*)((pThis)->mpAttrData))
																				// UC* (CCdiFinInSrcCapabilitiesAttr*)
#define CCdiFinInSrcCapabilitiesAttr_SetAttrPairsPointer(pThis, pAttr, size) CCdiFinAttrPairs_SetAttrPairsPointer(&((pThis)->mSuper), pAttr, size)
																				// void (CCdiFinInSrcCapabilitiesAttr*, UC*, UC)
void CCdiFinInSrcCapabilitiesAttr_AnalyzeAttrPairs(CCdiFinInSrcCapabilitiesAttr* pThis, UC numAttr);
#define CCdiFinInSrcCapabilitiesAttr_CheckAttrSet(pThis, key) CCdiFinAttrPairs_CheckAttrSet(&((pThis)->mSuper), key)
																				// bool (CCdiFinInSrcCapabilitiesAttr*, UC)
#define CCdiFinInSrcCapabilitiesAttr_AllocateAttrPair(pThis, key, size) CCdiFinAttrPairs_AllocateAttrPair(&((pThis)->mSuper), key, size)
																				// bool (CCdiFinInSrcCapabilitiesAttr*, UC, UC)
#define CCdiFinInSrcCapabilitiesAttr_GetAttributePointer(pThis, key) CCdiFinAttrPairs_GetAttributePointer(&((pThis)->mSuper), key)
																				// UC* (CCdiFinInSrcCapabilitiesAttr*, UC)
#define CCdiFinInSrcCapabilitiesAttr_GetAttrPairsSize(pThis) CCdiFinAttrPairs_GetAttrPairsSize(&((pThis)->mSuper))
																				// UC (CCdiFinInSrcCapabilitiesAttr*)
#define CCdiFinInSrcCapabilitiesAttr_GetNumAttr(pThis) CCdiFinAttrPairs_GetNumAttr(&((pThis)->mSuper))
																				// UC (CCdiFinInSrcCapabilitiesAttr*)


#endif // !defined(CCdiFinInSrcCapabilitiesAttr_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
