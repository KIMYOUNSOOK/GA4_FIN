// File Name:	CCdiFinAttrPairs.c
// Description:	Implimentation File of Attribute Pairs
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.30
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCdiFinAttrPairs.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CCdiFinAttrPairs_Constructor
// Description   : ƒRƒ“ƒXƒgƒ‰ƒNƒ^
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void CCdiFinAttrPairs_Constructor(CCdiFinAttrPairs* pThis)
{
	CCdiFinAttrPairs_Reset(pThis);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiFinAttrPairs_Reset
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void CCdiFinAttrPairs_Reset(CCdiFinAttrPairs* pThis)
{
	US index;

	pThis->mpAttrData = 0;

	for ( index = 0 ; index < CDIFIN_ATTRKEYMAXNUM ; index++ ){
		pThis->mSetIndex[index] = CDIFIN_ATTRNOTSET;
	}

	pThis->mAttrSize = 0;
	pThis->mDataSize = 0;
	pThis->mNumAttr	 = 0;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiFinAttrPairs_SetAttrPairsPointer
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void CCdiFinAttrPairs_SetAttrPairsPointer(CCdiFinAttrPairs* pThis, UC* pAttr, UC size)
{
	pThis->mpAttrData = pAttr;
	pThis->mDataSize  = size;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiFinAttrPairs_AnalyzeAttrPairs
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void CCdiFinAttrPairs_AnalyzeAttrPairs(CCdiFinAttrPairs* pThis, UC numAttr)
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiFinAttrPairs_CheckAttrSet
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
bool CCdiFinAttrPairs_CheckAttrSet(CCdiFinAttrPairs* pThis, UC key)
{
	if ( pThis->mSetIndex[key] != CDIFIN_ATTRNOTSET ) return true;
	else											  return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiFinAttrPairs_AllocateAttrPair
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
bool CCdiFinAttrPairs_AllocateAttrPair(CCdiFinAttrPairs* pThis, UC key, UC size)
{
	US index, nextSetIdx;

	if ( (pThis->mSetIndex[key] == CDIFIN_ATTRNOTSET) &&
		 ((pThis->mAttrSize + size) < pThis->mDataSize) ){
		pThis->mSetIndex[key] = pThis->mAttrSize;
		pThis->mAttrSize += size;
		pThis->mNumAttr++;

		pThis->mpAttrData[pThis->mSetIndex[key] + 0] = key;					// Octet0:Attr Key

		nextSetIdx = pThis->mSetIndex[key] + size;
		for ( index = (pThis->mSetIndex[key] + 1) ; index < nextSetIdx ; index++ ){
			pThis->mpAttrData[index] = 0x00;
		}

		return true;
	}
	else{ return false; }
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiFinAttrPairs_GetAttibutePointer
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
UC* CCdiFinAttrPairs_GetAttributePointer(CCdiFinAttrPairs* pThis, UC key)
{
		if ( pThis->mSetIndex[key] != CDIFIN_ATTRNOTSET ) {
			return &(pThis->mpAttrData[pThis->mSetIndex[key]]);
		}
		else{
			return NULL;
		}
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.04.25	| Yuichi Hoshino	| Created this file
//
