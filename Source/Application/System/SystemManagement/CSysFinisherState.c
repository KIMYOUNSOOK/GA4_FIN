// File Name:	CSysFinisherState.c
// Description:	Implimentation File of Finisher State
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSysFinisherState.h"

#include "CShtManager.h"
#include "CCdiAdapter.h"
#include "CCvrChecker.h"
#include "CJamChecker.h"
#include "CSeqDevInitHandler.h"
#include "CDvstInformer.h"
#include "CCfgInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Common Member of Finisher State ---
SysFinisherStateCommonMember gSys_FinisherStateCommonMember = {
	0x0000,
	false,
	CVR_STTS_CLOSE
};


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

static void ExecuteDeviceStatusChangeProcess(EDeviceStatusNo statusNo, US statusValue);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdConstructor
// Description   : �R���X�g���N�^
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysFinisherState_StdConstructor()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : ExecuteDeviceStatusChangeProcess
// Description   : �e�f�o�C�X�X�e�[�^�X�̕ω�������
// Parameter     : statusNo (Device Status No.)
//               : statusValue (Status Value)
// Return        : �]
///////////////////////////////////////////////////////////////////
void ExecuteDeviceStatusChangeProcess(EDeviceStatusNo statusNo, US statusValue)
{
	switch( statusNo ){
	// !!! Product Specific !!!
//	case DSNO_XXX:
//		
// 		break;
	default:
		break;
	}
	
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_DeviceStatusChangeProcess
// Description   : �f�o�C�X�X�e�[�^�X�ω�������
// Parameter     : statusNo (Device Status No.)
//               : statusValue (Status Value)
// Return        : �t�B�j�b�V���O�v���Z�X��~���
///////////////////////////////////////////////////////////////////
ESysProcStopType CSysFinisherState_DeviceStatusChangeProcess(US statusNo, US value)
{
	CDvstDeviceStatus* pDevStts;
	EvtParamDeviceConditionNotify message;
	UC stopType, deviceID, devCond[DEVID_NUM], chgCond;

	stopType = SYS_PROC_STOP_NO_ACT;

	pDevStts = CDvstInformer_GetDeviceStatus((EDeviceStatusNo)statusNo);

	if ( pDevStts != NULL ){
		// --- ���R���f�B�V�����擾 ---
		for ( deviceID = DEVID_FINISHER; deviceID < DEVID_NUM; deviceID++ ){
			devCond[deviceID] = CDvstInformer_GetDeviceCondtion(deviceID);
		}

		// --- Device Status�ʒm ---
		CCdiAdapter_SendDeviceStatus((EDeviceStatusNo)statusNo, value);			//�ω��̂�����DeviceStatus��CdiAdapter�֒ʒm

		// --- Condition�X�V ---
		CDvstInformer_UpdateDeviceCondition((EDeviceStatusNo)statusNo);

		// --- Sheet Management ��~�w�� ---
		if ( CDvstDeviceStatus_CheckDeviceCondition(pDevStts, value) == false ){// Condition NG
			stopType = CDvstDeviceStatus_GetFinProcessStopType(pDevStts);

			switch( stopType ){
			case SYS_PROC_STOP_CYCLE:
				CShtManager_CycleDownFaultStop(statusNo);
				break;
			case SYS_PROC_STOP_HARD:
				CShtManager_HardDownFaultStop(statusNo);
				break;
			default:
				// No Action
				break;
			}
		}
		else{ /* No Action */ }

		// --- Selectability(Configuration)�X�V ---
		for ( deviceID = DEVID_FINISHER; deviceID < DEVID_NUM; deviceID++ ){
			chgCond = CDvstInformer_GetDeviceCondtion(deviceID);

			if ( devCond[deviceID] != chgCond ){
				CCfgInformer_UpdateSelectability((EDeviceID)deviceID, (EDeviceCondition)chgCond);

//				if ( (deviceID == DEVID_FINISHER) &&
//					 (chgCond != DEV_COND_OK)){
	// --- �t�B�j�b�V���[�W���u�֎~�̃t�F�C���������̃C�j�V�����C�Y�����~ ---V3.4.0 Merge No.54
				if ( (deviceID == DEVID_FINISHER) && (chgCond == DEV_COND_PERM_NG)){
					CSeqDevInitHandler_Stop();
				}
				else{
					// No Action
				}
				// --- Device Condition Notify�C�x���g���s ---
				message.condition = chgCond;
				MSP_SendEvent(MID_SYS, cSys_DeviceConditionEventID[deviceID], (const UC *)(&message), sizeof(message), LOG_OFF);
			}
			else{
				// No Action
			}
		}

		// --- Special process for specific device status's private use ---
		if ( CDvstDeviceStatus_GetSpecialProcessExecution(pDevStts) == true ){
			ExecuteDeviceStatusChangeProcess((EDeviceStatusNo)statusNo, value);
		}
		else{ /* No Action */ }
	}
	else{
		// No Action
	}

	return (ESysProcStopType)stopType;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdIsTransitionState
// Description   : �ߓn��Ԋm�F����
// Parameter     : �]
// Return        : �ߓn��Ԏ��ʏ��
//               :  true = �ߓn��Ԃł���
//               :  false = �ߓn��ԂłȂ�
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdIsTransitionState()
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdCheckTransitionPossibility
// Description   : ��ԑJ�ډۊm�F����
// Parameter     : nextState (�J�ڐ���)
// Return        : ��ԑJ�ډ�
//               :  true = �J�ډ\
//               :  false = �J�ڕt��
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdCheckTransitionPossibility(EFinisherState nextState)
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdCheckExitCondition
// Description   : ����Ԃ���̑J�ڏ��������m�F����
// Parameter     : �]
// Return        : ��ԑJ�ڏ���������
//               :  true = ��ԑJ�ڏ��������ς�
//               :  false = ��ԑJ�ڏ���������
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdCheckExitCondition()
{
	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdGetNextState
// Description   : �ߓn��Ԃɂ�����J�ڐ��Ԋm�F����
// Parameter     : �]
// Return        : �J�ڐ�t�B�j�b�V���[���
///////////////////////////////////////////////////////////////////
EFinisherState CSysFinisherState_StdGetNextState()
{
	return FIN_STT_NUM;															// �ߓn��ԂłȂ�
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdEntry
// Description   : ����Ԃւ̑J�ڎ�����
// Parameter     : prevState (�J�ڌ��t�B�j�b�V���[���)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysFinisherState_StdEntry(EFinisherState prevState)
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdExit
// Description   : ����Ԃւ̑J�ڎ�����
// Parameter     : nextState (�J�ڐ�t�B�j�b�V���[���)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysFinisherState_StdExit(EFinisherState nextState)
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdSendDeviceActionRequest
// Description   : Device Action Request�C�x���g���M����
// Parameter     : eventID (MSP Event ID (Device Action Request))
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysFinisherState_StdSendDeviceActionRequest(US eventID)
{
	MSP_SendEvent(MID_SYS, eventID, NULL, 0, LOG_OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdSendDeviceEventNotify
// Description   : Device Event Notify�C�x���g���M����
// Parameter     : event (MSP Event ID (Device Event Notify))
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysFinisherState_StdSendDeviceEventNotify(US eventID)
{
	MSP_SendEvent(MID_SYS, eventID, NULL, 0, LOG_OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdReceivePowerUpResponse
// Description   : Power Up Response�C�x���g��M������
// Parameter     : moduleID (Device Module ID)
// Return        : �t�B�j�b�V���[��ԑJ�ڗL��
//               :  true = ��Ԃ��J�ڂ���\������
//               :  false = ��Ԃ͑J�ڂ��Ȃ�
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdReceivePowerUpResponse(UC moduleID)
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdReceivePowerDownResponse
// Description   : Power Down Response�C�x���g��M������
// Parameter     : moduleID (Device Module ID)
// Return        : �t�B�j�b�V���[��ԑJ�ڗL��
//               :  true = ��Ԃ��J�ڂ���\������
//               :  false = ��Ԃ͑J�ڂ��Ȃ�
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdReceivePowerDownResponse(UC moduleID)
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdReceiveChangeDiagResponse
// Description   : Change Diag Response�C�x���g��M������
// Parameter     : moduleID (Device Module ID)
// Return        : �t�B�j�b�V���[��ԑJ�ڗL��
//               :  true = ��Ԃ��J�ڂ���\������
//               :  false = ��Ԃ͑J�ڂ��Ȃ�
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdReceiveChangeDiagResponse(UC moduleID)
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdReceiveChangeNormalResponse
// Description   : Change Normal Response�C�x���g��M������
// Parameter     : moduleID (Device Module ID)
// Return        : �t�B�j�b�V���[��ԑJ�ڗL��
//               :  true = ��Ԃ��J�ڂ���\������
//               :  false = ��Ԃ͑J�ڂ��Ȃ�
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdReceiveChangeNormalResponse(UC moduleID)
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdReceiveCycleUpResponse
// Description   : Cycle Up Response�C�x���g��M������
// Parameter     : moduleID (Device Module ID)
// Return        : �t�B�j�b�V���[��ԑJ�ڗL��
//               :  true = ��Ԃ��J�ڂ���\������
//               :  false = ��Ԃ͑J�ڂ��Ȃ�
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdReceiveCycleUpResponse(UC moduleID)
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdReceiveProcessDownResponse
// Description   : Process Down Response�C�x���g��M������
// Parameter     : moduleID (Device Module ID)
// Return        : �t�B�j�b�V���[��ԑJ�ڗL��
//               :  true = ��Ԃ��J�ڂ���\������
//               :  false = ��Ԃ͑J�ڂ��Ȃ�
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdReceiveProcessDownResponse(UC moduleID)
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdReceiveDeviceActionResponse
// Description   : Device Action Response�C�x���g��M������
// Parameter     : moduleID (Device Module ID)
//               : eventID (MSP Event ID)
// Return        : �t�B�j�b�V���[��ԑJ�ڗL��
//               :  true = ��Ԃ��J�ڂ���\������
//               :  false = ��Ԃ͑J�ڂ��Ȃ�
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdReceiveDeviceActionResponse(UC moduleID, US eventID)
{
	switch( eventID ){
	// !!! Product Specific !!!
//	case EVT_DEV_RES_XXXX:
//
//		break;
	default:
		break;
	}

	CCdiAdapter_SendDeviceActionResponse(eventID);

	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdReceiveDeviceEventNotify
// Description   : Device Event Notify�C�x���g��M������
// Parameter     : moduleID (Device Module ID)
//               : eventID (MSP Event ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysFinisherState_StdReceiveDeviceEventNotify(UC moduleID, US eventID)
{
	switch( eventID ){
	// !!! Product Specific !!!
//	case EVT_DEV_EVT_XXXX:
//
//		break;
	default:
		break;
	}

	CCdiAdapter_SendDeviceEventNotify(eventID);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdReceiveDeviceStatusNotify
// Description   : Device Status Notify�C�x���g��M������
// Parameter     : moduleID (Device Module ID)
//               : statusNo (Status No.)
//               : value (Status Value)
// Return        : �t�B�j�b�V���O�v���Z�X��~���
///////////////////////////////////////////////////////////////////
ESysProcStopType CSysFinisherState_StdReceiveDeviceStatusNotify(UC moduleID, US statusNo, US value)
{
	return CSysFinisherState_DeviceStatusChangeProcess(statusNo, value);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdSystemConfigurationSet
// Description   : System Configuration�ʒm��M������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysFinisherState_StdSystemConfigurationSet()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdPowerUpModeSet
// Description   : �p���[�A�b�v��ʒʒm��M������
// Parameter     : pwrUpMode (�p���[�A�b�v���)
// Return        : �t�B�j�b�V���[��ԑJ�ڗL��
//               :  true = ��Ԃ��J�ڂ���\������
//               :  false = ��Ԃ͑J�ڂ��Ȃ�
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdPowerUpModeSet(ESysPowerUpMode pwrUpMode)
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdEntranceAreaTampered
// Description   : �G���g�����X��(�W�����N���A)����ʒm��M������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysFinisherState_StdEntranceAreaTampered()
{
	CJamChecker_EntranceEnvironmentChange();
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdLoadNvmComplete
// Description   : NVM�f�[�^���[�h�����ʒm��M������
// Parameter     : informerID (NVM Control Informer ID)
//               : result (NVM�f�[�^���[�h��������)
// Return        : �t�B�j�b�V���[��ԑJ�ڗL��
//               :  true = ��Ԃ��J�ڂ���\������
//               :  false = ��Ԃ͑J�ڂ��Ȃ�
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdLoadNvmComplete(SS result)
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdSaveNvmComplete
// Description   : NVM�f�[�^�Z�[�u�����ʒm��M������
// Parameter     : informerID (NVM Control Informer ID)
//               : result (NVM�f�[�^�Z�[�u��������)
// Return        : �t�B�j�b�V���[��ԑJ�ڗL��
//               :  true = ��Ԃ��J�ڂ���\������
//               :  false = ��Ԃ͑J�ڂ��Ȃ�
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdSaveNvmComplete(SS result)
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdSheetManagerReady
// Description   : �V�[�g�}�l�[�W���[�̃��f�B�[�J�ڒʒm��M������
// Parameter     : �]
// Return        : �t�B�j�b�V���[��ԑJ�ڗL��
//               :  true = ��Ԃ��J�ڂ���\������
//               :  false = ��Ԃ͑J�ڂ��Ȃ�
///////////////////////////////////////////////////////////////////
bool CSysFinisherState_StdSheetManagerReady()
{
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdCoverStatusChange
// Description   : �J�o�[��ԕω��ʒm��M������
// Parameter     : coverID (Cover ID)
//               : status (Cover Status)
// Return        : �t�B�j�b�V���O�v���Z�X��~���
///////////////////////////////////////////////////////////////////
ESysProcStopType CSysFinisherState_StdCoverStatusChange(ECvrCoverID coverID, ECvrCoverStatus status)
{
	if ( status == CVR_STTS_OPEN ){
		if ( gSys_FinisherStateCommonMember.mAllCoverStatus == CVR_STTS_CLOSE ){
			CSeqDevInitHandler_Stop();

			MSP_SendEvent(MID_SYS, EVT_DEV_EVT_ANY_COVER_OPEN, NULL, 0, LOG_OFF);

			gSys_FinisherStateCommonMember.mAllCoverStatus = CVR_STTS_OPEN;
		}
		else{
			// No Action
		}
	}
	else{																		// Close
		CJamChecker_CoverClose(coverID);

		if ( CCvrChecker_GetCoverStatus(CVR_ID_NUM) == CVR_STTS_CLOSE ){		// All Cover Close
			MSP_SendEvent(MID_SYS, EVT_DEV_EVT_ALL_COVER_CLOSE, NULL, 0, LOG_OFF);

			gSys_FinisherStateCommonMember.mAllCoverStatus = CVR_STTS_CLOSE;

			gSys_FinisherStateCommonMember.mInitializeReserve = true;
		}
		else{
			// No Action
		}
	}

	CDvstDeviceStatus_SetStatusValue(CDvstInformer_GetDeviceStatus(CCvrChecker_GetDeviceStatusNo(coverID)), status);

	return CSysFinisherState_DeviceStatusChangeProcess(CCvrChecker_GetDeviceStatusNo(coverID), status);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdFaultStatusChange
// Description   : �t�H���g�X�e�[�^�X�ω��ʒm��M������
// Parameter     : statusNo (Device Status No.)
//               : value (Status Value)
// Return        : �t�B�j�b�V���O�v���Z�X��~���
///////////////////////////////////////////////////////////////////
ESysProcStopType CSysFinisherState_StdFaultStatusChange(US statusNo, US value)
{
	CDvstDeviceStatus *pDeviceStatus;

	pDeviceStatus = CDvstInformer_GetDeviceStatus(statusNo);

	switch( CDvstDeviceStatus_GetStatusCategory(pDeviceStatus) ){
	case DVST_CTGRY_JAM:
		if ( value == FAULT_NOT_OCCUR ){
			if ( CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_CLEAR ){
				if ( CSeqDevInitHandler_Start(SYS_PWR_UP_MODE_NORMAL) != SEQ_ACT_STT_NUM ){
																				// �����W���������N���A���͕�������s(Initialize Handler�ł͂����݌v)
					CDvstDeviceStatus_SetStatusValue(CDvstInformer_GetDeviceStatus(DSNO_INIT_STATE), 1);
					CSysFinisherState_DeviceStatusChangeProcess(DSNO_INIT_STATE, 1);
				}
				else{
					// No Action
				}
			}
			else{
				// No Action
			}
		}
		else{
			// No Action
		}

		break;
	default:
		// No Action
		break;
	}

	CDvstDeviceStatus_SetStatusValue(CDvstInformer_GetDeviceStatus(statusNo), value);

	return CSysFinisherState_DeviceStatusChangeProcess(statusNo, value);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinisherState_StdJamZoneStatusChange
// Description   : �W�����]�[���X�e�[�^�X�ω��ʒm��M������
// Parameter     : devContID (Device Controller ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysFinisherState_StdJamZoneStatusChange(UC devContID)
{
	CCdiAdapter_SendJamZoneStatus(devContID);
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.02.08	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 14.12.12	| Yuichi Hoshino	| Trouble Action [ �d��ON���C�j�V�����C�Y������{���� ]
// 14.12.29	| Yuichi Hoshino	| Design Improvement [ Power Up�T�u�V�X�e���݌v������ ]
// 15.03.30	| Yuichi Hoshino	| Trouble Action [ �J�o�[�N���[�Y�C�j�V�����C�Y������{���� ]
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVM ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
