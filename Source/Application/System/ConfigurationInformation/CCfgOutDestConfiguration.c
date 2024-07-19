// File Name:	CCfgOutDestConfiguration.c
// Description:	Implimentation File of Output Destination Configuration
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCfgOutDestConfiguration.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Standard Override Method Table of Output Destination Capabilities ---
struct CfgOutDestConfigurationOverrideMethod const cCfg_OutDestConfigurationMethodStandard = {
	CCfgOutDestConfiguration_MakeInformation_Super,
	CCfgOutDestConfiguration_UpdateSelectability_Super
};


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
// Function Name : CCfgOutDestConfiguration_Constructor
// Description   : コンストラクタ
// Parameter     : pThis (インスタンスポインタ)
//               : pSpec (スペックデータインスタンス)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCfgOutDestConfiguration_Constructor(CCfgOutDestConfiguration* pThis, const CfgOutDestConfigurationSpec* pSpec)
{
	pThis->mpSpec = pSpec;

	// --- Selectability ---
	pThis->mSelectability = eCdi_NotSelectable;

	// --- Current Offset Configuration ---
	pThis->mOffset = eCdi_NotSelectable;

	// --- Current Sheet Folding Configuration ---
	pThis->mZFold	   = eCdi_NotSelectable ;
	pThis->mTriFoldZ   = eCdi_NotSelectable;
	pThis->mTriFoldC   = eCdi_NotSelectable;
	pThis->mSaddleFold = eCdi_NotSelectable;

	// --- Current Staple Configuration ---
	pThis->mSingleStitch = eCdi_NotSelectable;
	pThis->mDoubleStitch = eCdi_NotSelectable;
	pThis->mTripleStitch = eCdi_NotSelectable;
	pThis->mQuadStitch	 = eCdi_NotSelectable;
	pThis->mSingleRear	 = eCdi_NotSelectable;
	pThis->mSingleFront	 = eCdi_NotSelectable;

	// --- Current Punch Configuration ---
	pThis->mDoublePunch	= eCdi_NotSelectable;
	pThis->mTriplePunch	= eCdi_NotSelectable;
	pThis->mQuadPunch	= eCdi_NotSelectable;
	pThis->mMultiPunch	= eCdi_NotSelectable;

	// --- Current Booklet Maker Configuration ---
	pThis->mSaddleFoldAndStaple	  = eCdi_NotSelectable;
	pThis->mSaddleFoldAndNoStaple = eCdi_NotSelectable;
	pThis->mSquareFolding		  = eCdi_NotSelectable;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCfgOutDestConfiguration_MakeInformation_Super
// Description   : 情報構築処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCfgOutDestConfiguration_MakeInformation_Super(CCfgOutDestConfiguration* pThis)
{
	// !!! Product Specific !!!
	// オプションの接続状況から必要に応じてSelectability情報をSelectableに変更する。
}

///////////////////////////////////////////////////////////////////
// Function Name : CCfgOutDestConfiguration_UpdateSelectability_Super
// Description   : Device Conditionに応じたSelectability更新処理
// Parameter     : pThis (インスタンスポインタ)
//               : deviceID (Device ID)
//               : condition (Device Condition)
// Return        : 更新したAttributeのAttr Key
///////////////////////////////////////////////////////////////////
enum ECdiFinCfgAttrKeyOutDest CCfgOutDestConfiguration_UpdateSelectability_Super(CCfgOutDestConfiguration* pThis, EDeviceID deviceID, EDeviceCondition condition)
{
	// !!! Product Specific !!!
	// Deviceの故障通知に応じてSelectability情報をNot Selectableに変更する。
	// なお、変更した情報に対応するAttribute Keyを返す。

	return eCdiFinCfgKeyOutDest_All;
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
