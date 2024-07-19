// File Name:	CCdiFinDevContConfigurationAttr.h
// Description:	Header File of Finisher Device Controller Configuration
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.30
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDevContConfigurationAttr_h)
#define CCdiFinDevContConfigurationAttr_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyDevCont.h"

#include "CCdiFinCfgAttrDevCont01.h"
#include "CCdiFinCfgAttrDevCont40.h"
#include "CCdiFinCfgAttrDevCont41.h"
#include "CCdiFinCfgAttrDevContC0.h"
#include "CCdiFinCfgAttrDevContFA.h"
#include "CCdiFinCfgAttrDevContFB.h"
#include "CCdiFinCfgAttrDevContFC.h"
#include "CCdiFinCfgAttrDevContFD.h"
#include "CCdiFinCfgAttrDevContFE.h"
#include "CCdiFinCfgAttrDevContFF.h"

#include "CCdiFinMessage.h"
#include "CCdiFinAttrPairs.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Finisher Device Controller Configuration ---
typedef struct /* CCdiFinDevContConfigurationAttr */
{
	CCdiFinAttrPairs mSuper;
} CCdiFinDevContConfigurationAttr;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
#define CCdiFinDevContConfigurationAttr_Constructor(pThis) CCdiFinAttrPairs_Constructor(&((pThis)->mSuper))
																				// void (CCdiFinDevContConfigurationAttr*)

#define CCdiFinDevContConfigurationAttr_Reset(pThis) CCdiFinAttrPairs_Reset(&((pThis)->mSuper))
																				// void (CCdiFinDevContConfigurationAttr*)
#define CCdiFinDevContConfigurationAttr_GetAttrPairsPointer(pThis) ((UC*)((pThis)->mpAttrData))
																				// UC* (CCdiFinDevContConfigurationAttr*)
#define CCdiFinDevContConfigurationAttr_SetAttrPairsPointer(pThis, pAttr, size) CCdiFinAttrPairs_SetAttrPairsPointer(&((pThis)->mSuper), pAttr, size)
																				// void (CCdiFinDevContConfigurationAttr*, UC*, UC)
void CCdiFinDevContConfigurationAttr_AnalyzeAttrPairs(CCdiFinDevContConfigurationAttr* pThis, UC numAttr);
#define CCdiFinDevContConfigurationAttr_CheckAttrSet(pThis, key) CCdiFinAttrPairs_CheckAttrSet(&((pThis)->mSuper), key)
																				// bool (CCdiFinDevContConfigurationAttr*, UC)
#define CCdiFinDevContConfigurationAttr_AllocateAttrPair(pThis, key, size) CCdiFinAttrPairs_AllocateAttrPair(&((pThis)->mSuper), key, size)
																				// bool (CCdiFinDevContConfigurationAttr*, UC, UC)
#define CCdiFinDevContConfigurationAttr_GetAttributePointer(pThis, key) CCdiFinAttrPairs_GetAttributePointer(&((pThis)->mSuper), key)
																				// UC* (CCdiFinDevContConfigurationAttr*, UC)
#define CCdiFinDevContConfigurationAttr_GetAttrPairsSize(pThis) CCdiFinAttrPairs_GetAttrPairsSize(&((pThis)->mSuper))
																				// UC (CCdiFinDevContConfigurationAttr*)
#define CCdiFinDevContConfigurationAttr_GetNumAttr(pThis) CCdiFinAttrPairs_GetNumAttr(&((pThis)->mSuper))
																				// UC (CCdiFinDevContConfigurationAttr*)


#endif // !defined(CCdiFinDevContConfigurationAttr_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
