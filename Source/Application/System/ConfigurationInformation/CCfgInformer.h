// File Name:	CCfgInformer.h
// Description:	Header File of Configuration Informer
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.14
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CCfgInformer_h
#define CCfgInformer_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinDevCont.h"
#include "ECdiFinOutDest.h"
#include "ECdiFinInSrc.h"

#include "CCfgDevContConfiguration.h"
#include "CCfgOutDestConfiguration.h"
#include "CCfgInSrcConfiguration.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Configuration Informer Spec ---
typedef struct /*CfgInformerSpec*/
{
	const CfgDevContConfigurationSpec *pDevContSpec[DEV_CONT_NUM];
#if OUT_DEST_NUM != 0
	const CfgOutDestConfigurationSpec *pOutDestSpec[OUT_DEST_NUM];
#endif
#if IN_SRC_NUM != 0
	const CfgInSrcConfigurationSpec *pInSrcSpec[IN_SRC_NUM];
#endif
} CfgInformerSpec;

extern CfgInformerSpec const cCfg_InformerSpec;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCfgInformer_Constructor();

void CCfgInformer_MakeInformation();
void CCfgInformer_UpdateSelectability(EDeviceID deviceID, EDeviceCondition condition);
CCfgDevContConfiguration* CCfgInformer_GetDeviceControllerConfiguration(UC devContID);
CCfgOutDestConfiguration* CCfgInformer_GetOutputDestinationConfiguration(UC outDestID);
CCfgInSrcConfiguration* CCfgInformer_GetInputSourceConfiguration(UC inSrcID);


#endif // #ifndef CCfgInformer_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
