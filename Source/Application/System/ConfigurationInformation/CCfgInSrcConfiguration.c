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
// Description   : �R���X�g���N�^
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : pSpec (�X�y�b�N�f�[�^�C���X�^���X)
// Return        : �]
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
// Description   : ���\�z����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CCfgInSrcConfiguration_Super_MakeInformation(CCfgInSrcConfiguration* pThis)
{
	// !!! Product Specific !!!
	// �I�v�V�����̐ڑ��󋵂���K�v�ɉ�����Selectability����Selectable�ɕύX����B
}

///////////////////////////////////////////////////////////////////
// Function Name : CCfgInSrcConfiguration_Super_UpdateSelectability
// Description   : Device Condition�ɉ�����Selectability�X�V����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : deviceID (Device ID)
//               : condition (Device Condition)
// Return        : �X�V����Attribute��Attr Key
///////////////////////////////////////////////////////////////////
enum ECdiFinCfgAttrKeyInSrc CCfgInSrcConfiguration_Super_UpdateSelectability(CCfgInSrcConfiguration* pThis, EDeviceID deviceID, EDeviceCondition condition)
{
	// !!! Product Specific !!!
	// Device�̌̏�ʒm�ɉ�����Selectability����Not Selectable�ɕύX����B
	// �Ȃ��A�ύX�������ɑΉ�����Attribute Key��Ԃ��B

	return eCdiFinCfgKeyInSrc_All;
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
