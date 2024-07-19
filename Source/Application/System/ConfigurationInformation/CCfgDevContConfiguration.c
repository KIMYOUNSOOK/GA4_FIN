// File Name:	CCfgDevContConfiguration.c
// Description:	Implimentation File of Device Controller Configuration
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCfgDevContConfiguration.h"


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
// Function Name : CCfgDevContConfiguration_Constructor
// Description   : コンストラクタ
// Parameter     : pThis (インスタンスポインタ)
//               : pSpec (スペックデータインスタンス)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCfgDevContConfiguration_Constructor(CCfgDevContConfiguration* pThis, const CfgDevContConfigurationSpec* pSpec)
{
	pThis->mpSpec = pSpec;

	// --- Market Region ---
	pThis->mMarketRegion = eCdiFinMarket_FX;

	// --- Set Date Time ---
	pThis->mYear   = 0xFFFF;
	pThis->mMonth  = 0xFF;
	pThis->mDay	   = 0xFF;
	pThis->mHour   = 0xFF;
	pThis->mMinute = 0xFF;
	pThis->mSecond = 0xFF;

	// --- System Configuration ---
	pThis->mSysConfig = eCdiFinSysCfg_RealSys;

	// --- Media Size Group ---
	pThis->mMediaSizeGroup = eCdiFinSizeGrp_FX;
	pThis->mSelect8K16K	   = eCdiFinSlct8K16K_TFX;

	// --- Sheet Exit Height ---
	pThis->mSheetExitHeight = 1;

	// --- Productivity ---
	pThis->mProductivity = 1;

	// --- Exit Speed Range ---
	pThis->minExitSpeed = 3500;
	pThis->maxExitSpeed = 3500;

	// --- Sheet Exit Layout ---
	pThis->hotLineDistance		= 1;
	pThis->releasePointDistance	= 1;

	// --- Jam Bypass ---
	pThis->mJamBypass = eCdiFin_NotExec;

	// --- No Paper Run ---
	pThis->mNoPaperRun = eCdiFin_NotExec;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCfgDevContConfiguration_MakeInformation
// Description   : 情報構築処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCfgDevContConfiguration_MakeInformation(CCfgDevContConfiguration* pThis)
{
	// No Action(動的にフィニッシャーが自己判断で変更する必要がある情報はない)
}

///////////////////////////////////////////////////////////////////
// Function Name : CCfgDevContConfiguration_UpdateSelectability
// Description   : Device Conditionに応じたSelectability更新処理
// Parameter     : pThis (インスタンスポインタ)
//               : deviceID (Device ID)
//               : condition (Device Condition)
// Return        : 更新したAttributeのAttr Key
///////////////////////////////////////////////////////////////////
enum ECdiFinCfgAttrKeyDevCont CCfgDevContConfiguration_UpdateSelectability(CCfgDevContConfiguration* pThis, EDeviceID deviceID, EDeviceCondition condition)
{
	// No Action(動的にフィニッシャーが自己判断で変更する必要がある情報はない)

	return eCdiFinCfgKeyDevCont_All;
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
