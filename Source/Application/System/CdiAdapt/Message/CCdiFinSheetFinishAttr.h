// File Name:	CCdiFinSheetFinishAttr.h
// Description:	Header File of Sheet Finish Attributes
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.01
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinSheetFinishAttr_h)
#define CCdiFinSheetFinishAttr_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "ECdiFinShtFinAttrKey.h"

#include "CCdiFinShtFinAttr01.h"
#include "CCdiFinShtFinAttr02.h"
#include "CCdiFinShtFinAttr03.h"
#include "CCdiFinShtFinAttr04.h"
#include "CCdiFinShtFinAttr07.h"
#include "CCdiFinShtFinAttr08.h"
#include "CCdiFinShtFinAttr09.h"
#include "CCdiFinShtFinAttr0B.h"
#include "CCdiFinShtFinAttr0C.h"
#include "CCdiFinShtFinAttr0D.h"
#include "CCdiFinShtFinAttr20.h"
#include "CCdiFinShtFinAttrC0.h"
#include "CCdiFinShtFinAttrC4.h"

#include "CCdiFinMessage.h"
#include "CCdiFinAttrPairs.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Sheet Finish Attributes ---
typedef struct /* CCdiFinSheetFinishAttr */
{
	CCdiFinAttrPairs mSuper;
} CCdiFinSheetFinishAttr;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinSheetFinishAttr_Constructor(pThis) CCdiFinAttrPairs_Constructor(&((pThis)->mSuper))
																				// void (CCdiFinSheetFinishAttr*)

#define CCdiFinSheetFinishAttr_Reset(pThis); CCdiFinAttrPairs_Reset(&((pThis)->mSuper))]
																				// void (CCdiFinSheetFinishAttr*)
#define CCdiFinSheetFinishAttr_GetAttrPairsPointer(pThis) ((UC*)((pThis)->mpAttrData))
																				// UC* (CCdiFinSheetFinishAttr*)
#define CCdiFinSheetFinishAttr_SetAttrPairsPointer(pThis, pAttr, size) CCdiFinAttrPairs_SetAttrPairsPointer(&((pThis)->mSuper), pAttr, size)
																				// void (CCdiFinSheetFinishAttr*, UC*, UC)
void CCdiFinSheetFinishAttr_AnalyzeAttrPairs(CCdiFinSheetFinishAttr* pThis, UC numAttr);
#define CCdiFinSheetFinishAttr_CheckAttrSet(pThis, key) CCdiFinAttrPairs_CheckAttrSet(&((pThis)->mSuper), key)
																				// bool (CCdiFinSheetFinishAttr*, UC)
#define CCdiFinSheetFinishAttr_AllocateAttrPair(pThis, key, size) CCdiFinAttrPairs_AllocateAttrPair(&((pThis)->mSuper), key, size)
																				// bool (CCdiFinSheetFinishAttr*, UC, UC)
#define CCdiFinSheetFinishAttr_GetAttributePointer(pThis, key) CCdiFinAttrPairs_GetAttributePointer(&((pThis)->mSuper), key)
																				// UC* (CCdiFinSheetFinishAttr*, UC)
#define CCdiFinSheetFinishAttr_GetAttrPairsSize(pThis) CCdiFinAttrPairs_GetAttrPairsSize(&((pThis)->mSuper))
																				// UC (CCdiFinSheetFinishAttr*)
#define CCdiFinSheetFinishAttr_GetNumAttr(pThis) CCdiFinAttrPairs_GetNumAttr(&((pThis)->mSuper))
																				// UC (CCdiFinSheetFinishAttr*)


#endif // !defined(CCdiFinSheetFinishAttr_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
