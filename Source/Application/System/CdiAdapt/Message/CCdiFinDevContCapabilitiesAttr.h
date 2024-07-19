// File Name:	CCdiFinDevContCapabilitiesAttr.h
// Description:	Header File of Finisher Device Controller Capabilities
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDevContCapabilitiesAttr_h)
#define CCdiFinDevContCapabilitiesAttr_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "ECdiFinCapaAttrKeyDevCont.h"

#include "CCdiFinCapaAttrDevCont03.h"

#include "CCdiFinMessage.h"
#include "CCdiFinAttrPairs.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Finisher Device Controller Configuration ---
typedef struct /* CCdiFinDevContCapabilitiesAttr */
{
	CCdiFinAttrPairs mSuper;
} CCdiFinDevContCapabilitiesAttr;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
#define CCdiFinDevContCapabilitiesAttr_Constructor(pThis) CCdiFinAttrPairs_Constructor(&((pThis)->mSuper))
																				// void (CCdiFinDevContCapabilitiesAttr*)

#define CCdiFinDevContCapabilitiesAttr_Reset(pThis) CCdiFinAttrPairs_Reset(&((pThis)->mSuper))
																				// void (CCdiFinDevContCapabilitiesAttr*)
#define CCdiFinDevContCapabilitiesAttr_GetAttrPairsPointer(pThis) ((UC*)((pThis)->mpAttrData))
																				// UC* (CCdiFinDevContCapabilitiesAttr*)
#define CCdiFinDevContCapabilitiesAttr_SetAttrPairsPointer(pThis, pAttr, size) CCdiFinAttrPairs_SetAttrPairsPointer(&((pThis)->mSuper), pAttr, size)
																				// void (CCdiFinDevContCapabilitiesAttr*, UC*, UC)
void CCdiFinDevContCapabilitiesAttr_AnalyzeAttrPairs(CCdiFinDevContCapabilitiesAttr* pThis, UC numAttr);
#define CCdiFinDevContCapabilitiesAttr_CheckAttrSet(pThis, key) CCdiFinAttrPairs_CheckAttrSet(&((pThis)->mSuper), key)
																				// bool (CCdiFinDevContCapabilitiesAttr*, UC)
#define CCdiFinDevContCapabilitiesAttr_AllocateAttrPair(pThis, key, size) CCdiFinAttrPairs_AllocateAttrPair(&((pThis)->mSuper), key, size)
																				// bool (CCdiFinDevContCapabilitiesAttr*, UC, UC)
#define CCdiFinDevContCapabilitiesAttr_GetAttributePointer(pThis, key) CCdiFinAttrPairs_GetAttributePointer(&((pThis)->mSuper), key)
																				// UC* (CCdiFinDevContCapabilitiesAttr*, UC)
#define CCdiFinDevContCapabilitiesAttr_GetAttrPairsSize(pThis) CCdiFinAttrPairs_GetAttrPairsSize(&((pThis)->mSuper))
																				// UC (CCdiFinDevContCapabilitiesAttr*)
#define CCdiFinDevContCapabilitiesAttr_GetNumAttr(pThis) CCdiFinAttrPairs_GetNumAttr(&((pThis)->mSuper))
																				// UC (CCdiFinDevContCapabilitiesAttr*)


#endif // !defined(CCdiFinDevContCapabilitiesAttr_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
