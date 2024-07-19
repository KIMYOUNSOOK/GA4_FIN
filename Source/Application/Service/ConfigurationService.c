// File Name:	ConfigurationService.c
// Description:	Implimentation File of Configuration Service
// Engineer:	Yuichi Hoshino
// Last Edit:	14.12.29
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "ConfigurationService.h"

#include "COputInformer.h"
#include "CCfgInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckOptionDetection
// Description   : �I�v�V�����̐ڑ��L�������J����B
// Parameter     : optionID (Option ID)
// Return        : �I�v�V�����ڑ��L��
//               :  true = �ڑ�����
//               :  false = �ڑ��Ȃ�
///////////////////////////////////////////////////////////////////
bool SRVC_CheckOptionDetection(EOptionID optionID)
{
	return COputInformer_GetOptionDetect(optionID);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetOptionType
// Description   : �I�v�V�����̎�ʂ����J����B
// Parameter     : optionID (Option ID)
// Return        : �I�v�V�������(�I�v�V�������ɒl�̈Ӗ��̒�`����
//               : �Ȃ�)
///////////////////////////////////////////////////////////////////
UC SRVC_GetOptionType(EOptionID optionID)
{
	return COputInformer_GetOptionType(optionID);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetPowerUpYear
// Description   : �d��ON���ɃV�X�e���R���g���[���[���ʒm���ꂽ��
//               : �t���̔N�������J����B
// Parameter     : �]
// Return        : 0�`65534 = �N��[�~1�N]
//               : 0xFFFF = �s��(�V�X�e�����N�����Ǘ����Ă��Ȃ�)
///////////////////////////////////////////////////////////////////
US SRVC_GetPowerUpYear(void)
{
	CCfgDevContConfiguration* pCfg;

	pCfg = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);

	return CCfgDevContConfiguration_GetSetDateYear(pCfg);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetPowerUpMonth
// Description   : �d��ON���ɃV�X�e���R���g���[���[���ʒm���ꂽ��
//               : �t���̌��������J����B
// Parameter     : �]
// Return        : 1�`12 = ��[�~1��]
//               : 0xFF = �s��(�V�X�e�����������Ǘ����Ă��Ȃ�)
///////////////////////////////////////////////////////////////////
UC SRVC_GetPowerUpMonth(void)
{
	CCfgDevContConfiguration* pCfg;

	pCfg = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);

	return CCfgDevContConfiguration_GetSetDateMonth(pCfg);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetPowerUpDay
// Description   : �d��ON���ɃV�X�e���R���g���[���[���ʒm���ꂽ��
//               : �t���̓��������J����B
// Parameter     : �]
// Return        : 1�`31 = ��[�~1��]
//               : 0xFF = �s��(�V�X�e�����������Ǘ����Ă��Ȃ�)
///////////////////////////////////////////////////////////////////
UC SRVC_GetPowerUpDay(void)
{
	CCfgDevContConfiguration* pCfg;

	pCfg = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);

	return CCfgDevContConfiguration_GetSetDateDay(pCfg);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetPowerUpHour
// Description   : �d��ON���ɃV�X�e���R���g���[���[���ʒm���ꂽ��
//               : �����̎������J����B
// Parameter     : �]
// Return        : 0�`23 = ��[�~1��]
//               : 0xFF = �s��(�V�X�e���������Ǘ����Ă��Ȃ�)
///////////////////////////////////////////////////////////////////
UC SRVC_GetPowerUpHour(void)
{
	CCfgDevContConfiguration* pCfg;

	pCfg = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);

	return CCfgDevContConfiguration_GetSetDateHour(pCfg);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetPowerUpMinute
// Description   : �d��ON���ɃV�X�e���R���g���[���[���ʒm���ꂽ��
//               : �����̕������J����B
// Parameter     : �]
// Return        : 0�`59 = ��[�~1��]
//               : 0xFF = �s��(�V�X�e���������Ǘ����Ă��Ȃ�)
///////////////////////////////////////////////////////////////////
UC SRVC_GetPowerUpMinute(void)
{
	CCfgDevContConfiguration* pCfg;

	pCfg = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);

	return CCfgDevContConfiguration_GetSetDateMinute(pCfg);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetPowerUpSecond
// Description   : �d��ON���ɃV�X�e���R���g���[���[���ʒm���ꂽ��
//               : �����̕b�����J����B
// Parameter     : �]
// Return        : 0�`59 = �b[�~1�b]
//               : 0xFF = �s��(�V�X�e�����b���Ǘ����Ă��Ȃ�)
///////////////////////////////////////////////////////////////////
UC SRVC_GetPowerUpSecond(void)
{
	CCfgDevContConfiguration* pCfg;

	pCfg = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);

	return CCfgDevContConfiguration_GetSetDateSecond(pCfg);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetHotLineDistance
// Description   : �z�b�g���C���M���̕ω������������o�H��̃|�C���g
//               : �܂ł̋����������J����B
// Parameter     : �]
// Return        : 1�`4000[�~0.1mm]
///////////////////////////////////////////////////////////////////
US SRVC_GetHotLineDistance(void)
{
	CCfgDevContConfiguration* pCfg;

	pCfg = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);

//	if ( pCfg != NULL ){
		return CCfgDevContConfiguration_GetHotLineDistance(pCfg);
//	}
//	else{
//		return 1;
//	}
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetReleasePointDistance
// Description   : �V�[�g�����[�X�\�|�C���g�܂ł̋����������J����B
// Parameter     : �]
// Return        : 1�`4000[�~0.1mm]
///////////////////////////////////////////////////////////////////
US SRVC_GetReleasePointDistance(void)
{
	CCfgDevContConfiguration* pCfg;

	pCfg = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);

//	if ( pCfg != NULL ){
		return CCfgDevContConfiguration_GetReleasePointDistance(pCfg);
//	}
//	else{
//		return 1;
//	}
}
///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetProductivity
// Description   : 
// Parameter     : �]
// Return        : 1�`200[�~1PPM]
///////////////////////////////////////////////////////////////////
UC SRVC_GetProductivity(void)
{
	CCfgDevContConfiguration* pCfg;

	pCfg = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);

		return CCfgDevContConfiguration_GetProductivity(pCfg);
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.12.05	| Yuichi Hoshino	| Created this file
// 14.12.05	| Yuichi Hoshino	| Release V0 (Revision 00)
// 14.12.29	| Yuichi Hoshino	| Introduction Spec [ �d��ON�����擾�T�[�r�X�ǉ� ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
