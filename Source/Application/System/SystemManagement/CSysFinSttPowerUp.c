// File Name:	CSysFinSttPowerUp.c
// Description:	Implimentation File of Finisher State "Power Up"
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSysFinSttPowerUp.h"

#include "CDiagManager.h"
#include "CCdiAdapter.h"
#include "CCvrChecker.h"
#include "CJamChecker.h"
#include "CSeqDevInitHandler.h"
#include "COputInformer.h"
#include "CCapaInformer.h"
#include "CCfgInformer.h"
#include "CDvstInformer.h"
#include "CNmaHandler.h"
#include "CKeyInformer.h"
#include "CNvmInformer.h"
#include "CHfsiInformer.h"
// #include "CTempInformer.h"	�yROM�팸�z


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Power Up State ---
CSysFinisherState const gSys_FinSttPowerUpMethod = {
	CSysFinSttPowerUp_Constructor,							// Constructor
	CSysFinSttPowerUp_IsTransitionState,					// IsTransitionState
	CSysFinSttPowerUp_CheckTransitionPossibility,			// CheckTransitionPossibility
	CSysFinSttPowerUp_CheckExitCondition,					// CheckExitCondition
	CSysFinSttPowerUp_GetNextState,							// GetNextState
	CSysFinSttPowerUp_Entry,								// Entry
	CSysFinSttPowerUp_Exit,									// Exit
	CSysFinisherState_StdSendDeviceActionRequest,			// SendDeviceActionRequest
	CSysFinisherState_StdSendDeviceEventNotify,				// SendDeviceEventNotify
	CSysFinSttPowerUp_ReceivePowerUpResponse,				// ReceivePowerUpResponse
	CSysFinisherState_StdReceivePowerDownResponse,			// ReceivePowerDownResponse
	CSysFinisherState_StdReceiveChangeDiagResponse,			// ReceiveChangeDiagResponse
	CSysFinisherState_StdReceiveChangeNormalResponse,		// ReceiveChangeNormalResponse
	CSysFinisherState_StdReceiveCycleUpResponse,			// ReceiveCycleUpResponse
	CSysFinisherState_StdReceiveProcessDownResponse,		// ReceiveProcessDownResponse
	CSysFinisherState_StdReceiveDeviceActionResponse,		// ReceiveDeviceActionResponse
	CSysFinisherState_StdReceiveDeviceEventNotify,			// ReceiveDeviceEventNotify
	CSysFinisherState_StdReceiveDeviceStatusNotify,			// ReceiveDeviceStatusNotify
	CSysFinSttPowerUp_SystemConfigurationSet,				// SystemConfigurationSet
	CSysFinSttPowerUp_PowerUpModeSet,						// PowerUpModeSet
	CSysFinisherState_StdEntranceAreaTampered,				// EntranceAreaTampered
	CSysFinSttPowerUp_LoadNvmComplete,						// LoadNvmComplete
	CSysFinisherState_StdSaveNvmComplete,					// SaveNvmComplete
	CSysFinisherState_StdSheetManagerReady,					// SheetManagerReady
	CSysFinisherState_StdCoverStatusChange,					// CoverStatusChange
	CSysFinisherState_StdFaultStatusChange,					// FaultStatusChange
	CSysFinisherState_StdJamZoneStatusChange				// JamZoneStatusChange
};


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of Power Up State ---
typedef struct /*CSysFinSttPowerUpMember*/ {
	UC mPowerUpMode;
} CSysFinSttPowerUpMember;

static CSysFinSttPowerUpMember gSys_FinSttPowerUpMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_Constructor
// Description   : �R���X�g���N�^
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysFinSttPowerUp_Constructor()
{
	gSys_FinSttPowerUpMember.mPowerUpMode = SYS_PWR_UP_MODE_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_IsTransitionState
// Description   : 
// Parameter     : �]
// Return        : 
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerUp_IsTransitionState()
{
	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_CheckTransitionPossibility
// Description   : ��ԑJ�ډۊm�F����
// Parameter     : nextState (�J�ڐ���)
// Return        : ��ԑJ�ډ�
//               :  true = �J�ډ\
//               :  false = �J�ڕt��
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerUp_CheckTransitionPossibility(EFinisherState nextState)
{
	static US const cTransPsbl = {
		FIN_STT_CNV_BIT(FIN_STT_POWER_ON)
	};

	if ( cTransPsbl & FIN_STT_CNV_BIT(nextState) ) return true;
	else										   return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_CheckExitCondition
// Description   : ����Ԃ���̑J�ڏ��������m�F����
// Parameter     : �]
// Return        : ��ԑJ�ڏ���������
//               :  true = ��ԑJ�ڏ��������ς�
//               :  false = ��ԑJ�ڏ���������
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerUp_CheckExitCondition()
{
	if ( (gSys_FinisherStateCommonMember.mWaitingModuleBit == 0x0000) &&
		 (gSys_FinSttPowerUpMember.mPowerUpMode != SYS_PWR_UP_MODE_NUM) ){
		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_GetNextState
// Description   : �ߓn��Ԃɂ�����J�ڐ��Ԋm�F����
// Parameter     : �]
// Return        : �J�ڐ�t�B�j�b�V���[���
///////////////////////////////////////////////////////////////////
EFinisherState CSysFinSttPowerUp_GetNextState()
{
	return FIN_STT_POWER_ON;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_Entry
// Description   : ����Ԃւ̑J�ڎ�����
// Parameter     : prevState (�J�ڌ��t�B�j�b�V���[���)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysFinSttPowerUp_Entry(EFinisherState prevState)
{
	// --- Load NVM ---
	CNmaHandler_Load();
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_Exit
// Description   : ����Ԃւ̑J�ڎ�����
// Parameter     : nextState (�J�ڐ�t�B�j�b�V���[���)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysFinSttPowerUp_Exit(EFinisherState nextState)
{
	// --- �C�j�V�����C�Y������{ ---
	gSys_FinisherStateCommonMember.mInitializeReserve = false;

	if ( gSys_FinSttPowerUpMember.mPowerUpMode != SYS_PWR_UP_MODE_ABORT ){
		if ( CSeqDevInitHandler_Start((ESysPowerUpMode)gSys_FinSttPowerUpMember.mPowerUpMode) != SEQ_ACT_STT_NUM ){
			CDvstDeviceStatus_SetStatusValue(CDvstInformer_GetDeviceStatus(DSNO_INIT_STATE), 1);
			CSysFinisherState_DeviceStatusChangeProcess(DSNO_INIT_STATE, 1);
		}
		else{
			// No Action
		}
	} else {
		// No Action 
	}
	
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_ReceivePowerUpResponse
// Description   : Power Up Response�C�x���g��M������
// Parameter     : moduleID (Device Module ID)
// Return        : �t�B�j�b�V���[��ԑJ�ڗL��
//               :  true = ��Ԃ��J�ڂ���\������
//               :  false = ��Ԃ͑J�ڂ��Ȃ�
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerUp_ReceivePowerUpResponse(UC moduleID)
{
	gSys_FinisherStateCommonMember.mWaitingModuleBit &= ~MID_CNV_BIT(moduleID);

	if ( gSys_FinisherStateCommonMember.mWaitingModuleBit == 0x0000 ){
		// --- �t�B�j�b�V���[��񃁃b�Z�[�W���M�w�� ---
		CCdiAdapter_SendInitialInformation();
	}
	else{
		// No Action
	}

	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_SystemConfigurationSet
// Description   : �p���[�A�b�v��ʒʒm��M������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysFinSttPowerUp_SystemConfigurationSet()
{
	// --- NVM �o�[�W�����A�b�v������ ---
	CNvmInformer_VersionUpInitialize(CKeyInformer_GetMajorVersion(), CKeyInformer_GetMinorVersion());
	CHfsiInformer_VersionUpInitialize(CKeyInformer_GetMajorVersion(), CKeyInformer_GetMinorVersion());
	// CTempInformer_VersionInitialize(CKeyInformer_GetMajorVersion(), CKeyInformer_GetMinorVersion(), CKeyInformer_GetPatchVersion());

	CKeyInformer_SetMajorVersion(MAJOR_VERSION);
	CKeyInformer_SetMinorVersion(MINOR_VERSION);
	CKeyInformer_SetPatchVersion(PATCH_VERSION);

	// NVM Area��Version�p��NVM�����邽�߁B
	CNvmInformer_SetMajorVersion(MAJOR_VERSION);
	CNvmInformer_SetMinorVersion(MINOR_VERSION);
	CNvmInformer_SetPatchVersion(PATCH_VERSION);
	// CNvmInformer_SetIplVersion(CKeyInformer_GetIplVersion());

	// --- NVM �͈͊O�`�F�b�N ---
	CNvmInformer_OutRangeInitialize();

	// --- �t�B�j�b�V���[���\�z ---
	COputInformer_MakeInformation();
	CCapaInformer_MakeInformation();
	CCfgInformer_MakeInformation();

	// --- Device Module�֏�ԑJ�ڈ˗� ---
	gSys_FinisherStateCommonMember.mWaitingModuleBit = MID_DEV_MDL_BIT_ALL;

	MSP_SendMessage(MID_SYS, EVT_STT_POWER_UP_REQ, NULL, 0, LOG_OFF);

	// --- Diag Manager�d��ON�������˗� ---
	CDiagManager_PowerUp();

	// --- Interlock Check �J�n ---
	CCvrChecker_DetectInitialStatus();

	// --- �d��ON�� Jam�`�F�b�N ---
	CJamChecker_PowerOn();

	// --- Moview���Z�b�g ---
	SRVC_Moview_SetVersion();
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_PowerUpModeSet
// Description   : �p���[�A�b�v��ʒʒm��M������
// Parameter     : pwrUpMode (�p���[�A�b�v���)
// Return        : �t�B�j�b�V���[��ԑJ�ڗL��
//               :  true = ��Ԃ��J�ڂ���\������
//               :  false = ��Ԃ͑J�ڂ��Ȃ�
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerUp_PowerUpModeSet(ESysPowerUpMode pwrUpMode)
{
	gSys_FinSttPowerUpMember.mPowerUpMode = pwrUpMode;

	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttPowerUp_LoadNvmComplete
// Description   : NVM�f�[�^���[�h�����ʒm��M������
// Parameter     : informerID (NVM Control Informer ID)
//               : result (NVM�f�[�^���[�h��������)
// Return        : �t�B�j�b�V���[��ԑJ�ڗL��
//               :  true = ��Ԃ��J�ڂ���\������
//               :  false = ��Ԃ͑J�ڂ��Ȃ�
///////////////////////////////////////////////////////////////////
bool CSysFinSttPowerUp_LoadNvmComplete(SS result)
{
	UL logSelect;
	UC logData[3];

	if ( result == DD_NVM_OK ){
		// --- 1st Power ON NVM������ ---
		if ( CKeyInformer_CheckKeyCode() == false ){							// 1st Power ON
			CKeyInformer_SetKeyCode();
			CKeyInformer_SetMajorVersion(0);									// Version Up�����������{���邽�߂�Ver.0.0.0���Z�b�g
			CKeyInformer_SetMinorVersion(0);
			CKeyInformer_SetPatchVersion(0);

			CNvmInformer_FirstPowerOnInitialize();
			CHfsiInformer_FirstPowerOnInitialize();
		}
		else{ /* No Action */ }

		// --- IPL Version�Z�b�g ---
		// CNvmNVM_SetValue(CNvmInformer_Get(NVMID_IPL_SW_VERSION), CKeyInformer_GetIplVersion());
	}
	else{
		// --- Key Data���� ---
		CKeyInformer_ClearKeyCode();											// NVM�������Load�o���Ȃ��������Ƃ��Ӗ������邽�߂�Key Code�łȂ��l���Z�b�g
		CKeyInformer_ClearDownloadMode();										// Download Mode�Ɉڍs���Ȃ��悤�ɃN���A
		CKeyInformer_SetMajorVersion(MAJOR_VERSION);							// Version Up���������{�����Ȃ����߂Ɍ�Version���Z�b�g
		CKeyInformer_SetMinorVersion(MINOR_VERSION);
		CKeyInformer_SetPatchVersion(PATCH_VERSION);

		// --- NVM RAM�̈拭��������(�듮��h�~�̂��߂ɏ����l��������������) ---
		CNvmInformer_FirstPowerOnInitialize();
		CHfsiInformer_FirstPowerOnInitialize();
		// CTempInformer_FirstPowerOnInitialize();	�yROM�팸�z

		// --- Device Status�X�V ---
		CDvstDeviceStatus_SetStatusValue(CDvstInformer_GetDeviceStatus(DSNO_NVM_FAIL), FAULT_OCCUR);
		CSysFinisherState_DeviceStatusChangeProcess(DSNO_NVM_FAIL, FAULT_OCCUR);

		// --- Log Data���M ---
		// logData[0] = eCdiFinLogData_NvmFail;
		// logData[1] = (UC)(result >> 8);
		// logData[2] = (UC)result;

		// CCdiAdapter_SendLogData(logData, sizeof(logData));
	}

	// --- ���O�o�̓��W���[�����Z�b�g ---
	// logSelect = CNvmNVM_GetValue(CNvmInformer_Get(NVMID_DD_LOG_MODULE_SELECT_0_15));
	// logSelect |= (CNvmNVM_GetValue(CNvmInformer_Get(NVMID_DD_LOG_MODULE_SELECT_16_31)) << 16);
	// DD_LOG_SetModuleSelect(logSelect);

	// DD_LOG_SetCategory((UC)CNvmNVM_GetValue(CNvmInformer_Get(NVMID_DD_LOG_CATEGORY)));

	// --- System Controller�Ƃ̒ʐM�J�n ---
	CCdiAdapter_Sync();

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
// 14.12.12	| Yuichi Hoshino	| Trouble Action [ �d��ON���C�j�V�����C�Y������{���� ]
// 14.12.29	| Yuichi Hoshino	| Design Improvement [ Power Up�T�u�V�X�e���݌v������ ]
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVM ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
// 16.04.22	| Yuichi Hoshino	| Trouble Action [ T0151�FPower Up Sequence���̓d��OFF����Power Off Acknowledge���������Ȃ� ]

