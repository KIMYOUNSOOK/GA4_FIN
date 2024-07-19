// File Name:	CSysFinSttCycleUp.c
// Description:	Implimentation File of Finisher State "Cycle Up"
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSysFinSttCycleUp.h"

#include "CShtManager.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Cycle Up State ---
CSysFinisherState const gSys_FinSttCycleUpMethod = {
	CSysFinSttCycleUp_Constructor,							// Constructor
	CSysFinSttCycleUp_IsTransitionState,					// IsTransitionState
	CSysFinSttCycleUp_CheckTransitionPossibility,			// CheckTransitionPossibility
	CSysFinSttCycleUp_CheckExitCondition,					// CheckExitCondition
	CSysFinSttCycleUp_GetNextState,							// GetNextState
	CSysFinSttCycleUp_Entry,								// Entry
	CSysFinisherState_StdExit,								// Exit
	CSysFinisherState_StdSendDeviceActionRequest,			// SendDeviceActionRequest
	CSysFinisherState_StdSendDeviceEventNotify,				// SendDeviceEventNotify
	CSysFinisherState_StdReceivePowerUpResponse,			// ReceivePowerUpResponse
	CSysFinisherState_StdReceivePowerDownResponse,			// ReceivePowerDownResponse
	CSysFinisherState_StdReceiveChangeDiagResponse,			// ReceiveChangeDiagResponse
	CSysFinisherState_StdReceiveChangeNormalResponse,		// ReceiveChangeNormalResponse
	CSysFinSttCycleUp_ReceiveCycleUpResponse,				// ReceiveCycleUpResponse
	CSysFinisherState_StdReceiveProcessDownResponse,		// ReceiveProcessDownResponse
	CSysFinisherState_StdReceiveDeviceActionResponse,		// ReceiveDeviceActionResponse
	CSysFinisherState_StdReceiveDeviceEventNotify,			// ReceiveDeviceEventNotify
	CSysFinisherState_StdReceiveDeviceStatusNotify,			// ReceiveDeviceStatusNotify
	CSysFinisherState_StdSystemConfigurationSet,			// SystemConfigurationSet
	CSysFinisherState_StdPowerUpModeSet,					// PowerUpModeSet
	CSysFinisherState_StdEntranceAreaTampered,				// EntranceAreaTampered
	CSysFinisherState_StdLoadNvmComplete,					// LoadNvmComplete
	CSysFinisherState_StdSaveNvmComplete,					// SaveNvmComplete
	CSysFinSttCycleUp_SheetManagerReady,					// SheetManagerReady
	CSysFinisherState_StdCoverStatusChange,					// CoverStatusChange
	CSysFinisherState_StdFaultStatusChange,					// FaultStatusChange
	CSysFinisherState_StdJamZoneStatusChange				// JamZoneStatusChange
};

/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of Change Mode State ---
typedef struct /*CSysFinSttCycleUpMember*/ {
	UC mShtMgrReady;
} CSysFinSttCycleUpMember;

static CSysFinSttCycleUpMember gSys_FinSttCycleUpMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleUp_Constructor
// Description   : �R���X�g���N�^
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysFinSttCycleUp_Constructor()
{
	gSys_FinSttCycleUpMember.mShtMgrReady = false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleUp_IsTransitionState
// Description   : 
// Parameter     : �]
// Return        : 
///////////////////////////////////////////////////////////////////
bool CSysFinSttCycleUp_IsTransitionState()
{
	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleUp_CheckTransitionPossibility
// Description   : ��ԑJ�ډۊm�F����
// Parameter     : nextState (�J�ڐ���)
// Return        : ��ԑJ�ډ�
//               :  true = �J�ډ\
//               :  false = �J�ڕt��
///////////////////////////////////////////////////////////////////
bool CSysFinSttCycleUp_CheckTransitionPossibility(EFinisherState nextState)
{
	static US const cTransPsbl = {
		FIN_STT_CNV_BIT(FIN_STT_READY)|
		FIN_STT_CNV_BIT(FIN_STT_CYCLE_DOWN)|
		FIN_STT_CNV_BIT(FIN_STT_HARD_DOWN)
	};

	if ( cTransPsbl & FIN_STT_CNV_BIT(nextState) ) return true;
	else										   return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleUp_CheckExitCondition
// Description   : ����Ԃ���̑J�ڏ��������m�F����
// Parameter     : �]
// Return        : ��ԑJ�ڏ���������
//               :  true = ��ԑJ�ڏ��������ς�
//               :  false = ��ԑJ�ڏ���������
///////////////////////////////////////////////////////////////////
bool CSysFinSttCycleUp_CheckExitCondition()
{
	if ( (gSys_FinisherStateCommonMember.mWaitingModuleBit == 0x0000) &&
		 (gSys_FinSttCycleUpMember.mShtMgrReady == true) ){
		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleUp_GetNextState
// Description   : �ߓn��Ԃɂ�����J�ڐ��Ԋm�F����
// Parameter     : �]
// Return        : �J�ڐ�t�B�j�b�V���[���
///////////////////////////////////////////////////////////////////
EFinisherState CSysFinSttCycleUp_GetNextState()
{
	return FIN_STT_READY;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleUp_Entry
// Description   : ����Ԃւ̑J�ڎ�����
// Parameter     : prevState (�J�ڌ��t�B�j�b�V���[���)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysFinSttCycleUp_Entry(EFinisherState prevState)
{
//	gSys_FinSttCycleUpMember.mShtMgrReady = false;  //V3.4.0 Merge  No57 
//	CShtManager_ProcessStart();

	// --- Device Module�֏�ԑJ�ڈ˗� ---
	gSys_FinisherStateCommonMember.mWaitingModuleBit = MID_DEV_MDL_BIT_ALL;

	MSP_SendMessage(MID_SYS, EVT_STT_CYCLE_UP_REQ, NULL, 0, LOG_OFF);

	gSys_FinSttCycleUpMember.mShtMgrReady = false;  

	CShtManager_ProcessStart();
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleUp_ReceiveCycleUpResponse
// Description   : Cycle Up Response�C�x���g��M������
// Parameter     : moduleID (Device Module ID)
// Return        : �t�B�j�b�V���[��ԑJ�ڗL��
//               :  true = ��Ԃ��J�ڂ���\������
//               :  false = ��Ԃ͑J�ڂ��Ȃ�
///////////////////////////////////////////////////////////////////
bool CSysFinSttCycleUp_ReceiveCycleUpResponse(UC moduleID)
{
	gSys_FinisherStateCommonMember.mWaitingModuleBit &= ~MID_CNV_BIT(moduleID);

	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttCycleUp_SheetManagerReady
// Description   : �V�[�g�}�l�[�W���[�̃��f�B�[�J�ڒʒm��M������
// Parameter     : �]
// Return        : �t�B�j�b�V���[��ԑJ�ڗL��
//               :  true = ��Ԃ��J�ڂ���\������
//               :  false = ��Ԃ͑J�ڂ��Ȃ�
///////////////////////////////////////////////////////////////////
bool CSysFinSttCycleUp_SheetManagerReady()
{
	gSys_FinSttCycleUpMember.mShtMgrReady = true;

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
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVM ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
