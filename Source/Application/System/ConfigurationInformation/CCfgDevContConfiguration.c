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
// Description   : �R���X�g���N�^
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : pSpec (�X�y�b�N�f�[�^�C���X�^���X)
// Return        : �]
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
// Description   : ���\�z����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CCfgDevContConfiguration_MakeInformation(CCfgDevContConfiguration* pThis)
{
	// No Action(���I�Ƀt�B�j�b�V���[�����Ȕ��f�ŕύX����K�v��������͂Ȃ�)
}

///////////////////////////////////////////////////////////////////
// Function Name : CCfgDevContConfiguration_UpdateSelectability
// Description   : Device Condition�ɉ�����Selectability�X�V����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : deviceID (Device ID)
//               : condition (Device Condition)
// Return        : �X�V����Attribute��Attr Key
///////////////////////////////////////////////////////////////////
enum ECdiFinCfgAttrKeyDevCont CCfgDevContConfiguration_UpdateSelectability(CCfgDevContConfiguration* pThis, EDeviceID deviceID, EDeviceCondition condition)
{
	// No Action(���I�Ƀt�B�j�b�V���[�����Ȕ��f�ŕύX����K�v��������͂Ȃ�)

	return eCdiFinCfgKeyDevCont_All;
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
