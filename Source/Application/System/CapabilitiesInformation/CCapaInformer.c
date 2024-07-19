// File Name:	CCapaInformer.c
// Description:	Implimentation File of Capabilities Informer
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.14
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCapaInformer.h"

#include "COputInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of Capabilities Informer ---
typedef struct /*CCapaInformerMember*/
{
	CCapaDevContCapabilities mDevContCapa[DEV_CONT_NUM];
#if OUT_DEST_NUM != 0
	CCapaOutDestCapabilities mOutDestCapa[OUT_DEST_NUM];
#endif
#if IN_SRC_NUM != 0
	CCapaInSrcCapabilities mInSrcCapa[IN_SRC_NUM];
#endif
} CCapaInformerMember;

static CCapaInformerMember gCapa_InformerMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CCapaInformer_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCapaInformer_Constructor()
{
	UC instance;

	for ( instance = 0; instance < DEV_CONT_NUM; instance++ ){
		CCapaDevContCapabilities_Constructor(&(gCapa_InformerMember.mDevContCapa[instance]), cCapa_InformerSpec.pDevContSpec[instance]);
	}

#if OUT_DEST_NUM != 0
	for ( instance = 0; instance < OUT_DEST_NUM; instance++ ){
		CCapaOutDestCapabilities_Constructor(&(gCapa_InformerMember.mOutDestCapa[instance]), cCapa_InformerSpec.pOutDestSpec[instance]);
	}
#endif

#if IN_SRC_NUM != 0
	for ( instance = 0; instance < IN_SRC_NUM; instance++ ){
		CCapaInSrcCapabilities_Constructor(&(gCapa_InformerMember.mInSrcCapa[instance]), cCapa_InformerSpec.pInSrcSpec[instance]);
	}
#endif
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaInformer_MakeInformation
// Description   : 情報構築処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCapaInformer_MakeInformation()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaInformer_GetDeviceControllerCapabilities
// Description   : Device Controller Capabilities情報取得処理
// Parameter     : devContID (Device Controller ID)
// Return        : Device Controller Capabilitiesインスタンスポイン
//               : タ
///////////////////////////////////////////////////////////////////
CCapaDevContCapabilities* CCapaInformer_GetDeviceControllerCapabilities(UC devContID)
{
	UC instance;

	for ( instance = 0; instance < DEV_CONT_NUM; instance++ ){
		if ( CCapaDevContCapabilities_GetInstanceID((&gCapa_InformerMember.mDevContCapa[instance])) == devContID ){
			break;
		}
		else{
			// Go For Loop
		}
	}

	if ( instance < DEV_CONT_NUM ){
		return &(gCapa_InformerMember.mDevContCapa[instance]);
	}
	else{
		return NULL;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaInformer_GetOutputDestinationCapabilities
// Description   : Output Destination Capabilities情報取得処理
// Parameter     : outDestID (Output Destination ID)
// Return        : Output Destination Capabilitiesインスタンスポイ
//               : ンタ
///////////////////////////////////////////////////////////////////
CCapaOutDestCapabilities* CCapaInformer_GetOutputDestinationCapabilities(UC outDestID)
{
#if OUT_DEST_NUM != 0
	UC instance;

	for ( instance = 0; instance < OUT_DEST_NUM; instance++ ){
		if ( CCapaOutDestCapabilities_GetInstanceID((&gCapa_InformerMember.mOutDestCapa[instance])) == outDestID ){
			break;
		}
		else{
			// Go For Loop
		}
	}

	if ( (instance < OUT_DEST_NUM) &&
		 (COputInformer_GetOptionDetect((EOptionID)CCapaOutDestCapabilities_GetRelativeOptionID((&gCapa_InformerMember.mOutDestCapa[instance]))) == true) ){
		return &(gCapa_InformerMember.mOutDestCapa[instance]);
	}
	else{
		return NULL;
	}
#else
	return NULL;
#endif
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaInformer_GetInputSourceCapabilities
// Description   : Input Source Capabilities情報取得処理
// Parameter     : inSrcID (Input Source ID)
// Return        : Input Source Capabilitiesインスタンスポインタ
///////////////////////////////////////////////////////////////////
CCapaInSrcCapabilities* CCapaInformer_GetInputSourceCapabilities(UC inSrcID)
{
#if IN_SRC_NUM != 0
	UC instance;

	for ( instance = 0; instance < IN_SRC_NUM; instance++ ){
		if ( CCapaInSrcCapabilities_GetInstanceID(&(gCapa_InformerMember.mInSrcCapa[instance])) == inSrcID ){
			break;
		}
		else{
			// Go For Loop
		}
	}

	if ( (instance < IN_SRC_NUM) &&
		 (COputInformer_GetOptionDetect(CCapaInSrcCapabilities_GetRelativeOptionID(&(gCapa_InformerMember.mInSrcCapa[instance]))) == true) ){
		return &(gCapa_InformerMember.mInSrcCapa[instance]);
	}
	else{
		return NULL;
	}
#else
	return NULL;
#endif
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
