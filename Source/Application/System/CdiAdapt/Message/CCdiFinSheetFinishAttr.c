// File Name:	CCdiFinSheetFinishAttr.c
// Description:	Implimentation File of Sheet Finish Attributes
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.01
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCdiFinSheetFinishAttr.h"


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
// Function Name : CCdiFinSheetFinishAttr_AnalyzeAttrPairs
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void CCdiFinSheetFinishAttr_AnalyzeAttrPairs(CCdiFinSheetFinishAttr* pThis, UC numAttr)
{
	UC *pAttrPtr;
	UC index, size;

	pThis->mSuper.mNumAttr = numAttr;

	index = 0;
	while ( (index < pThis->mSuper.mDataSize) && (numAttr != 0) ){
		pAttrPtr = &(pThis->mSuper.mpAttrData[index]);

		switch ( *pAttrPtr ){
		case eCdiFinShtFinAttr_OfstMode:		size = CCdiFinShtFinAttr01_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinShtFinAttr_StplMode:		size = CCdiFinShtFinAttr02_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinShtFinAttr_PnchMode:		size = CCdiFinShtFinAttr03_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinShtFinAttr_BkltMode:		size = CCdiFinShtFinAttr04_GetAttributeSize(pAttrPtr);	break;
//		case eCdiFinShtFinAttr_InsertMode:		size = CCdiFinShtFinAttr06_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinShtFinAttr_FoldMode:		size = CCdiFinShtFinAttr07_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinShtFinAttr_MediaType:		size = CCdiFinShtFinAttr08_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinShtFinAttr_MediaSize:		size = CCdiFinShtFinAttr09_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinShtFinAttr_SheetCurl:		size = CCdiFinShtFinAttr0B_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinShtFinAttr_ExitSpd:			size = CCdiFinShtFinAttr0C_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinShtFinAttr_ShtIntegrity:	size = CCdiFinShtFinAttr0D_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinShtFinAttr_SetAction:		size = CCdiFinShtFinAttr20_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinShtFinAttr_OfstPos:			size = CCdiFinShtFinAttrC0_GetAttributeSize(pAttrPtr);	break;
//		case eCdiFinShtFinAttr_BookCustTblNo:	size = CCdiFinShtFinAttrC2_GetAttributeSize(pAttrPtr);	break;
//		case eCdiFinShtFinAttr_FoldCustTblNo:	size = CCdiFinShtFinAttrC3_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinShtFinAttr_ExitShtInterval:	size = CCdiFinShtFinAttrC4_GetAttributeSize(pAttrPtr);	break;
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
// 14.04.25	| Yuichi Hoshino	| Created this file
//
