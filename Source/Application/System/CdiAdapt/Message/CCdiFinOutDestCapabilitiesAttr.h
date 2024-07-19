// File Name:	CCdiFinOutDestCapabilitiesAttr.h
// Description:	Header File of Output Destination Capabilities
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.25

// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinOutDestCapabilitiesAttr_h)
#define CCdiFinOutDestCapabilitiesAttr_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "ECdiFinCapaAttrKeyOutDest.h"

#include "CCdiFinCapaAttrOutDest02.h"
#include "CCdiFinCapaAttrOutDest04.h"
#include "CCdiFinCapaAttrOutDest47.h"
#include "CCdiFinCapaAttrOutDest4B.h"
#include "CCdiFinCapaAttrOutDest50.h"
#include "CCdiFinCapaAttrOutDest55.h"
#include "CCdiFinCapaAttrOutDest5B.h"
#include "CCdiFinCapaAttrOutDest5E.h"
#include "CCdiFinCapaAttrOutDestC2.h"
#include "CCdiFinCapaAttrOutDestC6.h"
#include "CCdiFinCapaAttrOutDestC7.h"
#include "CCdiFinCapaAttrOutDestC8.h"


#include "CCdiFinMessage.h"
#include "CCdiFinAttrPairs.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Output Destination Configuration ---
typedef struct /* CCdiFinOutDestCapabilitiesAttr */
{
	CCdiFinAttrPairs mSuper;
} CCdiFinOutDestCapabilitiesAttr;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
#define CCdiFinOutDestCapabilitiesAttr_Constructor(pThis) CCdiFinAttrPairs_Constructor(&((pThis)->mSuper))
																				// void (CCdiFinOutDestCapabilitiesAttr*)

#define CCdiFinOutDestCapabilitiesAttr_Reset(pThis); CCdiFinAttrPairs_Reset(&((pThis)->mSuper))
																				// void (CCdiFinOutDestCapabilitiesAttr*)
#define CCdiFinOutDestCapabilitiesAttr_GetAttrPairsPointer(pThis) ((UC*)((pThis)->mpAttrData))
																				// UC* (CCdiFinOutDestCapabilitiesAttr*)
#define CCdiFinOutDestCapabilitiesAttr_SetAttrPairsPointer(pThis, pAttr, size) CCdiFinAttrPairs_SetAttrPairsPointer(&((pThis)->mSuper), pAttr, size)
																				// void (CCdiFinOutDestCapabilitiesAttr*, UC*, UC)
void CCdiFinOutDestCapabilitiesAttr_AnalyzeAttrPairs(CCdiFinOutDestCapabilitiesAttr* pThis, UC numAttr);
#define CCdiFinOutDestCapabilitiesAttr_CheckAttrSet(pThis, key) CCdiFinAttrPairs_CheckAttrSet(&((pThis)->mSuper), key)
																				// bool (CCdiFinOutDestCapabilitiesAttr*, UC)
#define CCdiFinOutDestCapabilitiesAttr_AllocateAttrPair(pThis, key, size) CCdiFinAttrPairs_AllocateAttrPair(&((pThis)->mSuper), key, size)
																				// bool (CCdiFinOutDestCapabilitiesAttr*, UC, UC)
#define CCdiFinOutDestCapabilitiesAttr_GetAttributePointer(pThis, key) CCdiFinAttrPairs_GetAttributePointer(&((pThis)->mSuper), key)
																				// UC* (CCdiFinOutDestCapabilitiesAttr*, UC)
#define CCdiFinOutDestCapabilitiesAttr_GetAttrPairsSize(pThis) CCdiFinAttrPairs_GetAttrPairsSize(&((pThis)->mSuper))
																				// UC (CCdiFinOutDestCapabilitiesAttr*)
#define CCdiFinOutDestCapabilitiesAttr_GetNumAttr(pThis) CCdiFinAttrPairs_GetNumAttr(&((pThis)->mSuper))
																				// UC (CCdiFinOutDestCapabilitiesAttr*)


#endif // !defined(CCdiFinOutDestCapabilitiesAttr_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
