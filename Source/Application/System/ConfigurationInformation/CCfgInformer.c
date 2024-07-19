// File Name:	CCfgInformer.c
// Description:	Implimentation File of Configuration Informer
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.14
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCfgInformer.h"

#include "CCdiAdapter.h"
#include "COputInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of Configuration Informer ---
typedef struct /*CCfgInformerMember*/
{
	CCfgDevContConfiguration mDevContConfig[DEV_CONT_NUM];
#if OUT_DEST_NUM != 0
	CCfgOutDestConfiguration mOutDestConfig[OUT_DEST_NUM];
#endif
#if IN_SRC_NUM != 0
	CCfgInSrcConfiguration mInSrcConfig[IN_SRC_NUM];
#endif
} CCfgInformerMember;

static CCfgInformerMember gCfg_InformerMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CCfgInformer_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCfgInformer_Constructor()
{
	UC instance;

	for ( instance = 0; instance < DEV_CONT_NUM; instance++ ){
		CCfgDevContConfiguration_Constructor(&(gCfg_InformerMember.mDevContConfig[instance]), cCfg_InformerSpec.pDevContSpec[instance]);
	}

#if OUT_DEST_NUM != 0
	for ( instance = 0; instance < OUT_DEST_NUM; instance++ ){
		CCfgOutDestConfiguration_Constructor(&(gCfg_InformerMember.mOutDestConfig[instance]), cCfg_InformerSpec.pOutDestSpec[instance]);
	}
#endif

#if IN_SRC_NUM != 0
	for ( instance = 0; instance < IN_SRC_NUM; instance++ ){
		CCfgInSrcConfiguration_Constructor(&(gCfg_InformerMember.mInSrcConfig[instance]), cCfg_InformerSpec.pInSrcSpec[instance]);
	}
#endif
}

///////////////////////////////////////////////////////////////////
// Function Name : CCfgInformer_MakeInformation
// Description   : 情報構築処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCfgInformer_MakeInformation()
{
	UC instance;

	for ( instance = 0; instance < DEV_CONT_NUM; instance++ ){
		CCfgDevContConfiguration_MakeInformation(&(gCfg_InformerMember.mDevContConfig[instance]));
	}

#if OUT_DEST_NUM != 0
	for ( instance = 0; instance < OUT_DEST_NUM; instance++ ){
		CCfgOutDestConfiguration_MakeInformation(&(gCfg_InformerMember.mOutDestConfig[instance]));
	}
#endif

#if IN_SRC_NUM != 0
	for ( instance = 0; instance < IN_SRC_NUM; instance++ ){
		CCfgInSrcConfiguration_MakeInformation(&(gCfg_InformerMember.mInSrcConfig[instance]));
	}
#endif
}

///////////////////////////////////////////////////////////////////
// Function Name : CCfgInformer_UpdateSelectability
// Description   : Device Conditionに応じたSelectability更新処理
// Parameter     : deviceID (Device ID)
//               : condition (Device Condition)
// Return        : ‐
//Ver 3.4.0 Merge Trouble Action [ T0066：トレイ使用不可フェイル発生時にSelectability情報を送信していない ]
///////////////////////////////////////////////////////////////////
void CCfgInformer_UpdateSelectability(EDeviceID deviceID, EDeviceCondition condition)
{
	UC instance, chgKey;

	// --- Finisher Device Controller ---
	for ( instance = 0; instance < DEV_CONT_NUM; instance++ ){
		chgKey = CCfgDevContConfiguration_UpdateSelectability(&(gCfg_InformerMember.mDevContConfig[instance]), deviceID, condition);

		if ( chgKey != eCdiFinCfgKeyDevCont_All ){
			CCdiAdapter_SendDeviceConfiguration(eCdiFinSubsys_DevCont, gCfg_InformerMember.mDevContConfig[instance].mpSpec->instanceID, chgKey);
		}
		else{ /* No Action */ }
	}

	// --- Output Destination ---
#if OUT_DEST_NUM != 0
	for ( instance = 0; instance < OUT_DEST_NUM; instance++ ){
		chgKey = CCfgOutDestConfiguration_UpdateSelectability(&(gCfg_InformerMember.mOutDestConfig[instance]), deviceID, condition);

		if ( chgKey != eCdiFinCfgKeyOutDest_All ){
			CCdiAdapter_SendDeviceConfiguration(eCdiFinSubsys_OutDest, gCfg_InformerMember.mOutDestConfig[instance].mpSpec->instanceID, chgKey);
		}
		else{ /* No Action */ }
	}
#endif

	// --- Input Source ---
#if IN_SRC_NUM != 0
	for ( instance = 0; instance < IN_SRC_NUM; instance++ ){
		chgKey = CCfgInSrcConfiguration_UpdateSelectability(&(gCfg_InformerMember.mInSrcConfig[instance]), deviceID, condition);

		if ( chgKey != eCdiFinCfgKeyInSrc_All ){
			CCdiAdapter_SendDeviceConfiguration(eCdiFinSubsys_InSrc, gCfg_InformerMember.mInSrcConfig[instance].mpSpec->instanceID, chgKey);
		}
		else{ /* No Action */ }
	}
#endif
}

///////////////////////////////////////////////////////////////////
// Function Name : CCfgInformer_GetDeviceControllerConfiguration
// Description   : Device Controller Configuration情報取得処理
// Parameter     : devContID (Device Controller ID)
// Return        : Device Controller Configurationインスタンスポイ
//               : ンタ
///////////////////////////////////////////////////////////////////
CCfgDevContConfiguration* CCfgInformer_GetDeviceControllerConfiguration(UC devContID)
{
	UC instance;

	for ( instance = 0; instance < DEV_CONT_NUM; instance++ ){
		if ( CCfgDevContConfiguration_GetInstanceID(&(gCfg_InformerMember.mDevContConfig[instance])) == devContID ){
			break;
		}
		else{
			// Go For Loop
		}
	}

	if ( instance < DEV_CONT_NUM ){
		return &(gCfg_InformerMember.mDevContConfig[instance]);
	}
	else{
		return NULL;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CCfgInformer_GetOutputDestinationConfiguration
// Description   : Output Destination Configuration情報取得処理
// Parameter     : outDestID (Output Destination ID)
// Return        : Output Destination Configurationインスタンスポイ
//               : ンタ
///////////////////////////////////////////////////////////////////
CCfgOutDestConfiguration* CCfgInformer_GetOutputDestinationConfiguration(UC outDestID)
{
#if OUT_DEST_NUM != 0
	UC instance;

	for ( instance = 0; instance < OUT_DEST_NUM; instance++ ){
		if ( CCfgOutDestConfiguration_GetInstanceID(&(gCfg_InformerMember.mOutDestConfig[instance])) == outDestID ){
			break;
		}
		else{
			// Go For Loop
		}
	}

	if ( (instance < OUT_DEST_NUM) &&
		 (COputInformer_GetOptionDetect(CCfgOutDestConfiguration_GetRelativeOptionID(&(gCfg_InformerMember.mOutDestConfig[instance]))) == true) ){
		return &(gCfg_InformerMember.mOutDestConfig[instance]);
	}
	else{
		return NULL;
	}
#else
	return NULL;
#endif
}

///////////////////////////////////////////////////////////////////
// Function Name : CCfgInformer_GetInputSourceConfiguration
// Description   : Input Source Configuration情報取得処理
// Parameter     : inSrcID (Input Source ID)
// Return        : Input Source Configurationインスタンスポインタ
///////////////////////////////////////////////////////////////////
CCfgInSrcConfiguration* CCfgInformer_GetInputSourceConfiguration(UC inSrcID)
{
#if IN_SRC_NUM != 0
	UC instance;

	for ( instance = 0; instance < IN_SRC_NUM; instance++ ){
		if ( CCfgInSrcConfiguration_GetInstanceID(&(gCfg_InformerMember.mInSrcConfig[instance])) == inSrcID ){
			break;
		}
		else{
			// Go For Loop
		}
	}

	if ( (instance < IN_SRC_NUM) &&
		 (COputInformer_GetOptionDetect(CCfgInSrcConfiguration_GetRelativeOptionID(&(gCfg_InformerMember.mInSrcConfig[instance]))) == true) ){
		return &(gCfg_InformerMember.mInSrcConfig[instance]);
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
// 15.11.02	| Yuichi Hoshino	| Trouble Action [ T0066：トレイ使用不可フェイル発生時にSelectability情報を送信していない ]
//
