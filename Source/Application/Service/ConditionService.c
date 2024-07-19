// File Name:	ConditionService.c
// Description:	Implimentation File of Condition Service
// Engineer:	Yuichi Hoshino
// Last Edit:	14.02.04
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "ConditionService.h"

#include "CSysManager.h"
#include "CCvrChecker.h"
#include "CJamChecker.h"
#include "CDvstInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetFinisherState
// Description   : �t�B�j�b�V���[��ԏ������J����B
// Parameter     : �]
// Return        : FIN_STT_UNKNOWN �` FIN_STT_PROC_END
///////////////////////////////////////////////////////////////////
EFinisherState SRVC_GetFinisherState(void)
{
	return CSysManager_GetFinisherState();
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetDeviceCondition
// Description   : �f�o�C�X�̃R���f�B�V�����������J����B
// Parameter     : deviceID (Device ID)
// Return        : DEV_COND_OK/DEV_COND_TEMP_NG/DEV_COND_PERM_NG
///////////////////////////////////////////////////////////////////
EDeviceCondition SRVC_GetDeviceCondition(EDeviceID deviceID)
{
	return CDvstInformer_GetDeviceCondtion(deviceID);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetCoverStatus
// Description   : PF-Finisher(�T�u�V�X�e��)���̃J�o�[�A�h���A��
//               : �g�[�^���̏�Ԃ����J����B
// Parameter     : �]
// Return        : CVR_STTS_CLOSE/CVR_STTS_OPEN 
///////////////////////////////////////////////////////////////////
ECvrCoverStatus SRVC_GetCoverStatus(void)
{
	return CCvrChecker_GetCoverStatus(CVR_ID_NUM);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetCoverStatus_ID : (V3.1.7 T/B �΍�)
// Description   : PF-Finisher(�T�u�V�X�e��)���̃J�o�[�A�h���A��
//               : �g�[�^���̏�Ԃ����J����B
// Parameter     : �]
// Return        : CVR_STTS_CLOSE/CVR_STTS_OPEN 
///////////////////////////////////////////////////////////////////
ECvrCoverStatus SRVC_GetCoverStatus_ID(ECvrCoverID coverID)
{
	return CCvrChecker_GetCoverStatus(coverID);
}


///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetJamZoneStatus
// Description   : �W�����]�[���̏�ԏ������J����B
// Parameter     : zoneID (Jam Zone ID)
// Return        : JAM_ZONE_STTS_CLEAR/JAM_ZONE_STTS_SET
///////////////////////////////////////////////////////////////////
EJamZoneStatus SRVC_GetJamZoneStatus(EJamZoneID zoneID)
{
	return CJamChecker_GetJamZoneStatus(zoneID);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetDeviceStatus
// Description   : �f�o�C�X�X�e�[�^�X�̏�ԏ������J����B
// Parameter     : statusNo (Device Status No.)
// Return        : 0�`65535(�l�̈Ӗ���Device Status���ɈقȂ�)
///////////////////////////////////////////////////////////////////
US SRVC_GetDeviceStatus(EDeviceStatusNo statusNo){
	CDvstDeviceStatus* pDvStts;

	pDvStts = CDvstInformer_GetDeviceStatus(statusNo);

	if ( pDvStts != NULL ){
		return CDvstDeviceStatus_GetStatusValue(pDvStts);
	}
	else{
		return 0;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_UpdateDeviceStatus
// Description   : �f�o�C�X�X�e�[�^�X�̃X�e�[�^�X�o�����[���X�V����B
// Parameter     : statusValue (Status Value)
//               : statusNo (Device Status No.)
// Return        : true = �l�ɕω���������/false = �l�ɕω��������Ȃ�����
///////////////////////////////////////////////////////////////////
bool SRVC_UpdateDeviceStatus(US statusValue, EDeviceStatusNo statusNo){
	CDvstDeviceStatus* pDvStts;
	EvtParamDeviceStatusNotify evtParam;

	pDvStts = CDvstInformer_GetDeviceStatus(statusNo);

	if ( (pDvStts != NULL) &&
		 (CDvstDeviceStatus_SetStatusValue(pDvStts, statusValue) == true) ){
		evtParam.statusNo	 = statusNo;
		evtParam.statusValue = statusValue;

		MSP_SendMessage(MID_SYS, EVT_COND_DEV_STTS, (const UC *)&evtParam, sizeof(evtParam), 0);

		return true;
	}
	else{
		return false;
	}
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.12.04	| Yuichi Hoshino	| Created this file
// 14.12.05	| Yuichi Hoshino	| Release V0 (Revision 00)
//
