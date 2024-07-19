// File Name:	CCfgInSrcConfiguration.c
// Description:	Implimentation File of Input Source Configuration
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCfgInSrcConfiguration.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Standard Override Method Table of Output Destination Capabilities ---
struct CfgInSrcConfigurationOverrideMethod const cCfg_InSrcConfigurationMethodStandard = {
	CCfgInSrcConfiguration_Super_MakeInformation,
	CCfgInSrcConfiguration_Super_UpdateSelectability
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
// Function Name : CCfgInSrcConfiguration_Constructor
// Description   : コンストラクタ
// Parameter     : pThis (インスタンスポインタ)
//               : pSpec (スペックデータインスタンス)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCfgInSrcConfiguration_Constructor(CCfgInSrcConfiguration* pThis, const CfgInSrcConfigurationSpec* pSpec)
{
	pThis->mpSpec = pSpec;

	// --- Selectability ---
	pThis->selectability = eCdi_NotSelectable;
	
	// --- Media Size Alternate ---
	pThis->mediaSizeAlt1 = 0;
	pThis->mediaSizeAlt2 = 0;
	pThis->mediaSizeAlt3 = 0;
	pThis->mediaSizeAlt4 = 0;
	pThis->mediaSizeAlt5 = 0;

	// --- Irregular Media Size ---
	pThis->IrregularProcSize = 0;
	pThis->IrregularCrossProcSize = 0;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCfgInSrcConfiguration_Super_MakeInformation
// Description   : 情報構築処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCfgInSrcConfiguration_Super_MakeInformation(CCfgInSrcConfiguration* pThis)
{
	// !!! Product Specific !!!
	// オプションの接続状況から必要に応じてSelectability情報をSelectableに変更する。
}

///////////////////////////////////////////////////////////////////
// Function Name : CCfgInSrcConfiguration_Super_UpdateSelectability
// Description   : Device Conditionに応じたSelectability更新処理
// Parameter     : pThis (インスタンスポインタ)
//               : deviceID (Device ID)
//               : condition (Device Condition)
// Return        : 更新したAttributeのAttr Key
///////////////////////////////////////////////////////////////////
enum ECdiFinCfgAttrKeyInSrc CCfgInSrcConfiguration_Super_UpdateSelectability(CCfgInSrcConfiguration* pThis, EDeviceID deviceID, EDeviceCondition condition)
{
	// !!! Product Specific !!!
	// Deviceの故障通知に応じてSelectability情報をNot Selectableに変更する。
	// なお、変更した情報に対応するAttribute Keyを返す。

	return eCdiFinCfgKeyInSrc_All;
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
