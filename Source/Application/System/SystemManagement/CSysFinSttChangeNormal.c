// File Name:	CSysFinSttChangeNormal.c
// Description:	Implimentation File of Finisher State "Change Normal"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.11.06
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSysFinSttChangeNormal.h"

#include "CShtManager.h"
#include "CCdiAdapter.h"
#include "CCvrChecker.h"
#include "CJamChecker.h"
#include "CSeqDevInitHandler.h"
#include "CDvstInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Finisher State ---
CSysFinisherState const gSys_FinSttChangeNormalMethod = {
	CSysFinSttChangeNormal_Constructor,						// Constructor
	CSysFinSttChangeNormal_IsTransitionState,				// IsTransitionState
	CSysFinSttChangeNormal_CheckTransitionPossibility,		// CheckTransitionPossibility
	CSysFinSttChangeNormal_CheckExitCondition,				// CheckExitCondition
	CSysFinSttChangeNormal_GetNextState,					// GetNextState
	CSysFinSttChangeNormal_Entry,							// Entry
	CSysFinSttChangeNormal_Exit,							// Exit
	CSysFinisherState_StdSendDeviceActionRequest,			// SendDeviceActionRequest
	CSysFinisherState_StdSendDeviceEventNotify,				// SendDeviceEventNotify
	CSysFinisherState_StdReceivePowerUpResponse,			// ReceivePowerUpResponse
	CSysFinisherState_StdReceivePowerDownResponse,			// ReceivePowerDownResponse
	CSysFinisherState_StdReceiveChangeDiagResponse,			// ReceiveChangeDiagResponse
	CSysFinSttChangeNormal_ReceiveChangeNormalResponse,		// ReceiveChangeNormalResponse
	CSysFinisherState_StdReceiveCycleUpResponse,			// ReceiveCycleUpResponse
	CSysFinisherState_StdReceiveProcessDownResponse,		// ReceiveProcessDownResponse
	CSysFinSttChangeNormal_ReceiveDeviceActionResponse,		// ReceiveDeviceActionResponse
	CSysFinisherState_StdReceiveDeviceEventNotify,			// ReceiveDeviceEventNotify
	CSysFinisherState_StdReceiveDeviceStatusNotify,			// ReceiveDeviceStatusNotify
	CSysFinisherState_StdSystemConfigurationSet,			// SystemConfigurationSet
	CSysFinisherState_StdPowerUpModeSet,					// PowerUpModeSet
	CSysFinisherState_StdEntranceAreaTampered,				// EntranceAreaTampered
	CSysFinisherState_StdLoadNvmComplete,					// LoadNvmComplete
	CSysFinisherState_StdSaveNvmComplete,					// SaveNvmComplete
	CSysFinisherState_StdSheetManagerReady,					// SheetManagerReady
	CSysFinisherState_StdCoverStatusChange,					// CoverStatusChange
	CSysFinisherState_StdFaultStatusChange,					// FaultStatusChange
	CSysFinisherState_StdJamZoneStatusChange				// JamZoneStatusChange
};


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of Change Mode State ---
typedef struct /*CSysFinSttChangeNormalMember*/ {
	UC mPrevState;
} CSysFinSttChangeNormalMember;

static CSysFinSttChangeNormalMember gSys_FinSttChangeNormalMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttChangeNormal_Constructor
// Description   : �R���X�g���N�^
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysFinSttChangeNormal_Constructor()
{
	gSys_FinSttChangeNormalMember.mPrevState = FIN_STT_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttChangeNormal_IsTransitionState
// Description   : �ߓn��Ԋm�F����
// Parameter     : �]
// Return        : �ߓn��Ԏ��ʏ��
//               :  true = �ߓn��Ԃł���
//               :  false = �ߓn��ԂłȂ�
///////////////////////////////////////////////////////////////////
bool CSysFinSttChangeNormal_IsTransitionState()
{
	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttChangeNormal_CheckTransitionPossibility
// Description   : ��ԑJ�ډۊm�F����
// Parameter     : nextState (�J�ڐ���)
// Return        : ��ԑJ�ډ�
//               :  true = �J�ډ\
//               :  false = �J�ڕt��
///////////////////////////////////////////////////////////////////
bool CSysFinSttChangeNormal_CheckTransitionPossibility(EFinisherState nextState)
{
	static US const cTransPsbl = {
		FIN_STT_CNV_BIT(FIN_STT_STANDBY)
	};

	if ( cTransPsbl & FIN_STT_CNV_BIT(nextState) ) return true;
	else										   return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttChangeNormal_CheckExitCondition
// Description   : ����Ԃ���̑J�ڏ��������m�F����
// Parameter     : �]
// Return        : ��ԑJ�ڏ���������
//               :  true = ��ԑJ�ڏ��������ς�
//               :  false = ��ԑJ�ڏ���������
///////////////////////////////////////////////////////////////////
bool CSysFinSttChangeNormal_CheckExitCondition()
{
	if ( gSys_FinisherStateCommonMember.mWaitingModuleBit == 0x0000 ){
		if ( CSeqDevInitHandler_GetState() == SEQ_ACT_STT_IDLE ){				// �C�j�V�����C�Y���쒆�łȂ����
			return true;
		}
		else{
			// NG
		}
	}
	else{
		// NG
	}

	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttChangeNormal_GetNextState
// Description   : �ߓn��Ԃɂ�����J�ڐ��Ԋm�F����
// Parameter     : �]
// Return        : �J�ڐ�t�B�j�b�V���[���
///////////////////////////////////////////////////////////////////
EFinisherState CSysFinSttChangeNormal_GetNextState()
{
	return FIN_STT_STANDBY;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttChangeNormal_Entry
// Description   : ����Ԃւ̑J�ڎ�����
// Parameter     : prevState (�J�ڌ��t�B�j�b�V���[���)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysFinSttChangeNormal_Entry(EFinisherState prevState)
{
	gSys_FinSttChangeNormalMember.mPrevState = prevState;

	// --- MSP Location���Z�b�g���� ---
	MSP_ResetLocationSchedule();												// ���P�[�V������C�x���g��X�P�W���[���̃��Z�b�g����

	// --- �W�������m ---
	if ( prevState == FIN_STT_DIAGNOSTIC ){
		CJamChecker_ChangeNormal();
	}
	else{
		// No Action
	}

	// --- Device Module�֏�ԑJ�ڈ˗� ---
	gSys_FinisherStateCommonMember.mWaitingModuleBit = MID_DEV_MDL_BIT_ALL;

	MSP_SendMessage(MID_SYS, EVT_STT_CHANGE_NORMAL_REQ, NULL, 0, LOG_OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttChangeNormal_Exit
// Description   : ����Ԃւ̑J�ڎ�����
// Parameter     : nextState (�J�ڐ�t�B�j�b�V���[���)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysFinSttChangeNormal_Exit(EFinisherState nextState)
{
	if ( (gSys_FinSttChangeNormalMember.mPrevState == FIN_STT_DIAGNOSTIC) ||
		 (gSys_FinisherStateCommonMember.mInitializeReserve == true) ){
		gSys_FinisherStateCommonMember.mInitializeReserve = false;

		if ( CSeqDevInitHandler_Start(SYS_PWR_UP_MODE_NORMAL) != SEQ_ACT_STT_NUM ){
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

	gSys_FinSttChangeNormalMember.mPrevState = FIN_STT_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttChangeNormal_ReceiveChangeNormalResponse
// Description   : Change Normal Response�C�x���g��M������
// Parameter     : moduleID (Device Module ID)
// Return        : �t�B�j�b�V���[��ԑJ�ڗL��
//               :  true = ��Ԃ��J�ڂ���\������
//               :  false = ��Ԃ͑J�ڂ��Ȃ�
///////////////////////////////////////////////////////////////////
bool CSysFinSttChangeNormal_ReceiveChangeNormalResponse(UC moduleID)
{
	gSys_FinisherStateCommonMember.mWaitingModuleBit &= ~MID_CNV_BIT(moduleID);

	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttChangeNormal_ReceiveDeviceActionResponse
// Description   : Device Action Response�C�x���g��M������
// Parameter     : moduleID (Device Module ID)
//               : eventID (MSP Event ID)
// Return        : �t�B�j�b�V���[��ԑJ�ڗL��
//               :  true = ��Ԃ��J�ڂ���\������
//               :  false = ��Ԃ͑J�ڂ��Ȃ�
///////////////////////////////////////////////////////////////////
bool CSysFinSttChangeNormal_ReceiveDeviceActionResponse(UC moduleID, US eventID)
{
	UC retValue, initState;

	retValue = false;

	switch( eventID ){
	// !!! Product Specific !!!
//	case EVT_DEV_RES_XXXX:
//
//		break;
	default:
		break;
	}

	initState = CSeqDevInitHandler_DeviceActionResponse(eventID, moduleID);

	if ( initState == SEQ_ACT_STT_PAUSE ){
		CJamChecker_DeviceInitializeComplete();									// Jam Check

		initState = CSeqDevInitHandler_Restart();
	}
	else{
		// No Action
	}

	if ( initState == SEQ_ACT_STT_IDLE ){
		CJamChecker_DeviceInitializeComplete();									// Jam Check
		CShtManager_StandbyEject();
		CDvstDeviceStatus_SetStatusValue(CDvstInformer_GetDeviceStatus(DSNO_INIT_STATE), 0);
		CSysFinisherState_DeviceStatusChangeProcess(DSNO_INIT_STATE, 0);
		MSP_SendEvent(MID_SYS, EVT_DEV_EVT_DEV_INIT_COMP, NULL, 0, LOG_OFF);

		retValue = true;
	}
	else{
		// No Action
	}

	CCdiAdapter_SendDeviceActionResponse(eventID);

	return retValue;
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.11.06	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.03.30	| Yuichi Hoshino	| Trouble Action [ �J�o�[�N���[�Y�C�j�V�����C�Y������{���� ]
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVM ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
