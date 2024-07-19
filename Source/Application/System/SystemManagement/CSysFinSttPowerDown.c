// File Name:	CSysFinSttPowerDown.c
// Description:	Implimentation File of Finisher State "Power Down"
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSysFinSttPowerDown.h"

#include "CCdiAdapter.h"
#include "CCvrChecker.h"
#include "CJamChecker.h"
#include "CSeqDevInitHandler.h"
#include "CDvstInformer.h"
#include "CNmaHandler.h"
#include "CKeyInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Poweer Down State ---
CSysFinisherState const gSys_FinSttPowerDownMethod = {
	CSysFinSttPowerDown_Constructor,						// Constructor
	CSysFinSttPowerDown_IsTransitionState,					// IsTransitionState
	CSysFinSttPowerDown_CheckTransitionPossibility,			// CheckTransitionPossibility
	CSysFinSttPowerDown_CheckExitCondition,					// CheckExitCondition
	CSysFinSttPowerDown_GetNextState,						// GetNextState
	CSysFinSttPowerDown_Entry,								// Entry
	CSysFinisherState_StdExit,								// Exit
	CSysFinisherState_StdSendDeviceActionRequest,			// SendDeviceActionRequest
	CSysFinisherState_StdSendDeviceEventNotify,				// SendDeviceEventNotify
	CSysFinisherState_StdReceivePowerUpResponse,			// ReceivePowerUpResponse
	CSysFinSttPowerDown_ReceivePowerDownResponse,			// ReceivePowerDownResponse
	CSysFinisherState_StdReceiveChangeDiagResponse,			// ReceiveChangeDiagResponse
	CSysFinisherState_StdReceiveChangeNormalResponse,		// ReceiveChangeNormalResponse
	CSysFinisherState_StdReceiveCycleUpResponse,			// ReceiveCycleUpResponse
	CSysFinisherState_StdReceiveProcessDownResponse,		// ReceiveProcessDownResponse
//V3.4.0 �@T/B�΍�@Merge TB.59 
//	CSysFinisherState_StdReceiveDeviceActionResponse,		// ReceiveDeviceActionResponse
	CSysFinSttPowerDown_ReceiveDeviceActionResponse,		// ReceiveDeviceActionResponse
	CSysFinisherState_StdReceiveDeviceEventNotify,			// ReceiveDeviceEventNotify
	CSysFinisherState_StdReceiveDeviceStatusNotify,			// ReceiveDeviceStatusNotify
	CSysFinisherState_StdSystemConfigurationSet,			// SystemConfigurationSet
	CSysFinisherState_StdPowerUpModeSet,					// PowerUpModeSet
	CSysFinisherState_StdEntranceAreaTampered,				// EntranceAreaTampered
	CSysFinisherState_StdLoadNvmComplete,					// LoadNvmComplete
	CSysFinSttPowerDown_SaveNvmComplete,					// SaveNvmComplete
	CSysFinisherState_StdSheetManagerReady,					// SheetManagerReady
	CSysFinisherState_StdCoverStatusChange,					// CoverStatusChange
	CSysFinisherState_StdFaultStatusChange,					// FaultStatusChange
	CSysFinisherState_StdJamZoneStatusChange				// JamZoneStatusChange
};


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of Power Up State ---
typedef struct /*CSysFinSttPowerDownMember*/ {
	UC mSaveNvmComplete;
} CSysFinSttPowerDownMember;

static CSysFinSttPowerDownMember gSys_FinSttPowerDownMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerDown_Constructor
// Description   : �R���X�g���N�^
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysFinSttPowerDown_Constructor()
{
	gSys_FinSttPowerDownMember.mSaveNvmComplete = false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerDown_IsTransitionState
// Description   : 
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerDown_IsTransitionState()
{
	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerDown_CheckTransitionPossibility
// Description   : ��ԑJ�ډۊm�F����
// Parameter     : nextState (�J�ڐ���)
// Return        : ��ԑJ�ډ�
//               :  true = �J�ډ\
//               :  false = �J�ڕt��
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerDown_CheckTransitionPossibility(EFinisherState nextState)
{
	static US const cTransPsbl = {
		FIN_STT_CNV_BIT(FIN_STT_POWER_OFF)
	};

	if ( cTransPsbl & FIN_STT_CNV_BIT(nextState) ) return true;
	else										   return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerDown_CheckExitCondition
// Description   : ����Ԃ���̑J�ڏ��������m�F����
// Parameter     : �]
// Return        : ��ԑJ�ڏ���������
//               :  true = ��ԑJ�ڏ��������ς�
//               :  false = ��ԑJ�ڏ���������
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerDown_CheckExitCondition()
{
	if ( (gSys_FinisherStateCommonMember.mWaitingModuleBit == 0x0000) &&
		 (gSys_FinSttPowerDownMember.mSaveNvmComplete == true) ){
		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerDown_GetNextState
// Description   : �ߓn��Ԃɂ�����J�ڐ��Ԋm�F����
// Parameter     : �]
// Return        : �J�ڐ�t�B�j�b�V���[���
///////////////////////////////////////////////////////////////////
EFinisherState CSysFinSttPowerDown_GetNextState()
{
	return FIN_STT_POWER_OFF;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerDown_Entry
// Description   : ����Ԃւ̑J�ڎ�����
// Parameter     : prevState (�J�ڌ��t�B�j�b�V���[���)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysFinSttPowerDown_Entry(EFinisherState prevState)
{
	// --- �J�o�[��ԃ`�F�b�N��~ ---
	CCvrChecker_EndCheck();

	// --- �W������ԃ`�F�b�N��~ ---
	CJamChecker_EndCheck();

	// --- �C�j�V�����C�Y�����~ ---
	CSeqDevInitHandler_Stop();

	// --- Device Module�֏�ԑJ�ڈ˗� ---
	gSys_FinisherStateCommonMember.mWaitingModuleBit = MID_DEV_MDL_BIT_ALL;

	MSP_SendMessage(MID_SYS, EVT_STT_POWER_DOWN_REQ, NULL, 0, LOG_OFF);
}

/////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerDown_ReceivePowerDownResponse
// Description   : Power Down Response�C�x���g��M������
// Parameter     : moduleID (Device Module ID)
// Return        : �t�B�j�b�V���[��ԑJ�ڗL��
//               :  true = ��Ԃ��J�ڂ���\������
//               :  false = ��Ԃ͑J�ڂ��Ȃ�
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerDown_ReceivePowerDownResponse(UC moduleID)
{
	gSys_FinisherStateCommonMember.mWaitingModuleBit &= ~MID_CNV_BIT(moduleID);

	if ( gSys_FinisherStateCommonMember.mWaitingModuleBit == 0x0000 ){			// �S�f�o�C�X���W���[������̉�����M
		// --- Save NVM ---
		if ( CKeyInformer_CheckKeyCode() == true ){								// NVM����ُ햢����
			if ( CNmaHandler_Save() == false ){									// �Z�[�u�����s�v(���������Ȃ�)
				// --- NVM�Z�[�u�����I�� ---
				gSys_FinSttPowerDownMember.mSaveNvmComplete = true;
			}
			else{ /* No Action */ }
		}
		else{																	// NVM����ُ픭��
			// --- NVM�Z�[�u�����I�� ---
			gSys_FinSttPowerDownMember.mSaveNvmComplete = true;
		}
	}
	else{ /* No Action */ }

	return true;
}

////V3.4.0 �@T/B�΍�@Merge TB.59 ///////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerDown_ReceiveDeviceActionResponse
// Description   : Device Action Response�C�x���g��M������
// Parameter     : moduleID (Device Module ID)
//               : eventID (MSP Event ID)
// Return        : �t�B�j�b�V���[��ԑJ�ڗL��
//               :  true = ��Ԃ��J�ڂ���\������
//               :  false = ��Ԃ͑J�ڂ��Ȃ�
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerDown_ReceiveDeviceActionResponse(UC moduleID, US eventID)
{
	UC initState;

	switch( eventID ){
	// !!! Product Specific !!!
//	case EVT_DEV_RES_XXXX:
//
//		break;
	default:
		break;
	}

	// --- �C�j�V�����C�Y�����~�҂� ---
	initState = CSeqDevInitHandler_DeviceActionResponse(eventID, moduleID);

	if ( initState == SEQ_ACT_STT_IDLE ){
		CDvstDeviceStatus_SetStatusValue(CDvstInformer_GetDeviceStatus(DSNO_INIT_STATE), 0);
		CSysFinisherState_DeviceStatusChangeProcess(DSNO_INIT_STATE, 0);
		MSP_SendEvent(MID_SYS, EVT_DEV_EVT_DEV_INIT_COMP, NULL, 0, LOG_OFF);
	}
	else{ /* No Action */ }

	// --- CDI���b�Z�[�W���M ---
	CCdiAdapter_SendDeviceActionResponse(eventID);

	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerDown_SaveNvmComplete
// Description   : NVM�f�[�^�Z�[�u�����ʒm��M������
// Parameter     : informerID (NVM Control Informer ID)
//               : result (NVM�f�[�^�Z�[�u��������)
// Return        : �t�B�j�b�V���[��ԑJ�ڗL��
//               :  true = ��Ԃ��J�ڂ���\������
//               :  false = ��Ԃ͑J�ڂ��Ȃ�
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerDown_SaveNvmComplete(SS result)
{
	UC logData[3];

	if ( result == DD_NVM_OK ){
		// --- NVM�Z�[�u�����I�� ---
		gSys_FinSttPowerDownMember.mSaveNvmComplete = true;
	}
	else{
		// --- NVM Fail�������� ---
		CDvstDeviceStatus_SetStatusValue(CDvstInformer_GetDeviceStatus(DSNO_NVM_FAIL), FAULT_OCCUR);
		CSysFinisherState_DeviceStatusChangeProcess(DSNO_NVM_FAIL, FAULT_OCCUR);

		// --- Log Data���M ---
		// logData[0] = eCdiFinLogData_NvmFail;
		// logData[1] = (UC)(result >> 8);
		// logData[2] = (UC)result;

		// CCdiAdapter_SendLogData(logData, sizeof(logData));

		// --- NVM�Z�[�u�����I�� ---
		gSys_FinSttPowerDownMember.mSaveNvmComplete = true;
	}

	return true;
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.04.08	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.01.27 | Yuichi Hoshino	| Trouble Action [ Power Off�҂����킹���Ă��Ȃ� ]
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVM ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
// 15.06.19	| Yuichi Hoshino	| Introduction Spec [ NVM Fail�������O���M ]
// 15.07.15	| Yuichi Hoshino	| Trouble Action [ �C�j�V�����C�Y������s���Ƀp���[�_�E���ɑJ�ڂ���ƑJ�ڂ��������Ȃ� ]
// 15.08.27	| Yuichi Hoshino	| Trouble Action [ NVM��Key Data�������݊�����҂�����Power Off Announcement�𑗐M ]
// 15.12.14	| Yuichi Hoshino	| Design Improvement [ ROM�팸 ]
// XX.XX.XX	| Yuichi Hoshino	| Release V2 (Revision 02)
//
