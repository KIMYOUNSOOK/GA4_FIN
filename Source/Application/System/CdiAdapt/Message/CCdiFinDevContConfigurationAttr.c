// File Name:	CCdiFinDevContConfigurationAttr.c
// Description:	Implimentation File of Finisher Device Controller Configuration
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.30
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "CCdiFinDevContConfigurationAttr.h"


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
// Function Name : CCdiFinDevContConfigurationAttr_AnalyzeAttrPairs
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void CCdiFinDevContConfigurationAttr_AnalyzeAttrPairs(CCdiFinDevContConfigurationAttr* pThis, UC numAttr)
{
	UC *pAttrPtr;
	UC index, size;

	pThis->mSuper.mNumAttr = numAttr;

	index = 0;
	while ( (index < pThis->mSuper.mDataSize) && (numAttr != 0) ){
		pAttrPtr = &(pThis->mSuper.mpAttrData[index]);

		switch ( *pAttrPtr ){
		case eCdiFinCfgKeyDevCont_Market:		size = CCdiFinCfgAttrDevCont01_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCfgKeyDevCont_DateTime:		size = CCdiFinCfgAttrDevCont40_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCfgKeyDevCont_SysCfg:		size = CCdiFinCfgAttrDevCont41_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCfgKeyDevCont_MediaSizeGrp:	size = CCdiFinCfgAttrDevContC0_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCfgKeyDevCont_PrevDevCont:	size = 2;													break;
		case eCdiFinCfgKeyDevCont_ProductCode:	size = 2;													break;
		case eCdiFinCfgKeyDevCont_ShtExtHeight:	size = CCdiFinCfgAttrDevContFA_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCfgKeyDevCont_Productivity:	size = CCdiFinCfgAttrDevContFB_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCfgKeyDevCont_ExitSpdRange:	size = CCdiFinCfgAttrDevContFC_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCfgKeyDevCont_ExitLayout:	size = CCdiFinCfgAttrDevContFD_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCfgKeyDevCont_JamBypass:	size = CCdiFinCfgAttrDevContFE_GetAttributeSize(pAttrPtr);	break;
		case eCdiFinCfgKeyDevCont_NoPprRun:		size = CCdiFinCfgAttrDevContFF_GetAttributeSize(pAttrPtr);	break;
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
