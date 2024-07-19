// File Name:	CCapaInformer.h
// Description:	Header File of Capabilities Informer
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.14
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CCapaInformer_h
#define CCapaInformer_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "CCapaDevContCapabilities.h"
#include "CCapaOutDestCapabilities.h"
#include "CCapaInSrcCapabilities.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Capabilities Informer Spec ---
typedef struct /*CapaInformerSpec*/
{
	const CapaDevContCapabilitiesSpec *pDevContSpec[DEV_CONT_NUM];
#if OUT_DEST_NUM != 0
	const CapaOutDestCapabilitiesSpec *pOutDestSpec[OUT_DEST_NUM];
#endif
#if IN_SRC_NUM != 0
	const CapaInSrcCapabilitiesSpec *pInSrcSpec[IN_SRC_NUM];
#endif
} CapaInformerSpec;

extern CapaInformerSpec const cCapa_InformerSpec;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCapaInformer_Constructor();

void CCapaInformer_MakeInformation();
CCapaDevContCapabilities* CCapaInformer_GetDeviceControllerCapabilities(UC devContID);
CCapaOutDestCapabilities* CCapaInformer_GetOutputDestinationCapabilities(UC outDestID);
CCapaInSrcCapabilities* CCapaInformer_GetInputSourceCapabilities(UC inSrcID);


#endif // #ifndef CCapaInformer_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
