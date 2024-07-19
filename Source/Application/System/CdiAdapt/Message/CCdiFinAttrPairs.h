// File Name:	CCdiFinAttrPairs.h
// Description:	Header File of Attribute Pairs
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.30
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinAttrPairs_h)
#define CCdiFinAttrPairs_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////
#define CDIFIN_ATTRKEYMAXNUM	256							// Disable Change Number
#define CDIFIN_ATTRNOTSET		0xFF						// Not set Attr Pair


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

typedef struct /* CCdiFinAttrPairs */
{
	UC* mpAttrData;											// data pointer of Attr Pairs Data
	UC mSetIndex[CDIFIN_ATTRKEYMAXNUM];						// Index of array set each Attr Pair
	UC mAttrSize;											// Used size of Attr Pair data
	UC mDataSize;											// Absolute size of Attr Pair data
	UC mNumAttr;											// Num Attr Pairs
} CCdiFinAttrPairs;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinAttrPairs_Constructor(CCdiFinAttrPairs* pThis);

void CCdiFinAttrPairs_Reset(CCdiFinAttrPairs* pThis);
#define CCdiFinAttrPairs_GetAttrPairsPointer(pThis) ((UC*)((pThis)->mpAttrData))// UC* (CCdiFinAttrPairs*)
void CCdiFinAttrPairs_SetAttrPairsPointer(CCdiFinAttrPairs* pThis, UC* pAttr, UC size);
void CCdiFinAttrPairs_AnalyzeAttrPairs(CCdiFinAttrPairs* pThis, UC numAttr);
bool CCdiFinAttrPairs_CheckAttrSet(CCdiFinAttrPairs* pThis, UC key);
bool CCdiFinAttrPairs_AllocateAttrPair(CCdiFinAttrPairs* pThis, UC key, UC size);
UC* CCdiFinAttrPairs_GetAttributePointer(CCdiFinAttrPairs* pThis, UC key);
#define CCdiFinAttrPairs_GetAttrPairsSize(pThis) ((UC)((pThis)->mAttrSize))		// UC (CCdiFinAttrPairs*)
#define CCdiFinAttrPairs_GetNumAttr(pThis) ((UC)((pThis)->mNumAttr))			// UC (CCdiFinAttrPairs*)


#endif // !defined(CCdiFinAttrPairs_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
