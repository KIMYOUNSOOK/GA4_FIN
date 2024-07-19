// File Name:	CCdiFinOutDestCapabilitiesAttr.c
// Description:	Implimentation File of Output Destination Capabilities
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCdiFinOutDestCapabilitiesAttr.h"


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
// Function Name : CCdiFinOutDestCapabilitiesAttr_AnalyzeAttrPairs
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void CCdiFinOutDestCapabilitiesAttr_AnalyzeAttrPairs(CCdiFinOutDestCapabilitiesAttr* pThis, UC numAttr)
{
	UC *pAttrPtr;
	UC index, size;

	pThis->mSuper.mNumAttr = numAttr;

	index = 0;
	while ( (index < pThis->mSuper.mDataSize) && (numAttr != 0) ){
		pAttrPtr = &(pThis->mSuper.mpAttrData[index]);

		switch ( *pAttrPtr ){
		case eCdiFinCapaKeyOutDest_MaxCapa:			size = CCdiFinCapaAttrOutDest02_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCapaKeyOutDest_UnitType:		size = CCdiFinCapaAttrOutDest04_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCapaKeyOutDest_Offset:			size = CCdiFinCapaAttrOutDest47_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCapaKeyOutDest_Fold:			size = CCdiFinCapaAttrOutDest4B_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCapaKeyOutDest_Staple:			size = CCdiFinCapaAttrOutDest50_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCapaKeyOutDest_Punch:			size = CCdiFinCapaAttrOutDest55_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCapaKeyOutDest_Book:			size = CCdiFinCapaAttrOutDest5B_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCapaKeyOutDest_BookTrayCapa:	size = CCdiFinCapaAttrOutDest5E_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCapaKeyOutDest_CountInStplSet:	size = CCdiFinCapaAttrOutDestC2_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCapaKeyOutDest_StapleType:		size = CCdiFinCapaAttrOutDestC6_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCapaKeyOutDest_BookStapleType:	size = CCdiFinCapaAttrOutDestC7_GetAttributeSize(pAttrPtr);	break;
		default:									return;						// Unknown Attribute Key(Fail Safe)
		}

		pThis->mSuper.mSetIndex[*pAttrPtr] = index;
		index += size;

		numAttr--;
	}
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.06.17	| Yuichi Hoshino	| Created this file
//
