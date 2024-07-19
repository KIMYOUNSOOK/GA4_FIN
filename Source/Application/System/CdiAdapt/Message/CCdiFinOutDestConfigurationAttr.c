// File Name:	CCdiFinOutDestConfigurationAttr.c
// Description:	Implimentation File of Output Destination Configuration
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.30
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCdiFinOutDestConfigurationAttr.h"


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
// Function Name : CCdiFinOutDestConfigurationAttr_AnalyzeAttrPairs
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void CCdiFinOutDestConfigurationAttr_AnalyzeAttrPairs(CCdiFinOutDestConfigurationAttr* pThis, UC numAttr)
{
	UC *pAttrPtr;
	UC index, size;

	pThis->mSuper.mNumAttr = numAttr;

	index = 0;
	while ( (index < pThis->mSuper.mDataSize) && (numAttr != 0) ){
		pAttrPtr = &(pThis->mSuper.mpAttrData[index]);

		switch ( *pAttrPtr ){
		case eCdiFinCfgKeyOutDest_Select:	size = CCdiFinCfgAttrOutDest03_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCfgKeyOutDest_Offset:	size = CCdiFinCfgAttrOutDest45_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCfgKeyOutDest_Fold:		size = CCdiFinCfgAttrOutDest46_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCfgKeyOutDest_Staple:	size = CCdiFinCfgAttrOutDest47_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCfgKeyOutDest_Punch:	size = CCdiFinCfgAttrOutDest48_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCfgKeyOutDest_Book:		size = CCdiFinCfgAttrOutDest49_GetAttributeSize(pAttrPtr);	break;
		default:								return;							// Unknown Attribute Key(Fail Safe)
		}

		pThis->mSuper.mSetIndex[*pAttrPtr] = index;
		index += size;

		numAttr--;
	}
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.04.30	| Yuichi Hoshino	| Created this file
//
