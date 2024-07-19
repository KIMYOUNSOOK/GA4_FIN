// File Name:	CSysManager.c
// Description:	Implimentation File of System Manager
// Engineer:	Yuichi Hoshino
// Last Edit:	16.02.26
// Revision:	03
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSysManager.h"

#include "CDiagManager.h"
#include "CShtManager.h"
#include "CCdiAdapter.h"
#include "CCvrChecker.h"
#include "CJamChecker.h"
#include "CSeqDevInitHandler.h"
#include "CSeqProcEndHandler.h"
#include "COputInformer.h"
#include "CCapaInformer.h"
#include "CCfgInformer.h"
#include "CDvstInformer.h"
#include "CNmaHandler.h"
#include "CKeyInformer.h"
#include "CNvmInformer.h"
#include "CHfsiInformer.h"
// #include "CTempInformer.h"	�yROM�팸�z
#include "CDltManager.h"

#include "CSysFinisherState.h"
#include "CSysFinSttUnknown.h"
#include "CSysFinSttPowerUp.h"
#include "CSysFinSttPowerOn.h"
#include "CSysFinSttPowerDown.h"
#include "CSysFinSttPowerOff.h"
#include "CSysFinSttChangeDiag.h"
#include "CSysFinSttDiagnostic.h"
#include "CSysFinSttChangeNormal.h"
#include "CSysFinSttStandby.h"
#include "CSysFinSttCycleUp.h"
#include "CSysFinSttReady.h"
#include "CSysFinSttCycleDown.h"
#include "CSysFinSttHardDown.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of System Manager ---
typedef struct /*CSysManagerMember*/
{
	UC mCurrentState;
	UC mNextState;
	UC mRequestState;
	UC mPowerUpMode;
	UC mPowerDownReserve;
} CSysManagerMember;

static CSysManagerMember gSys_ManagerMember;

// --- Public method table of Finisher State ---
static CSysFinisherState* const cSys_FinSttPtrArray[FIN_STT_NUM] ={
	&gSys_FinSttUnknownMethod,			// FIN_STT_UNKNOWN
	&gSys_FinSttPowerUpMethod,			// FIN_STT_POWER_UP
	&gSys_FinSttPowerOnMethod,			// FIN_STT_POWER_ON
	&gSys_FinSttPowerDownMethod,		// FIN_STT_POWER_DOWN
	&gSys_FinSttPowerOffMethod,			// FIN_STT_POWER_OFF
	&gSys_FinSttChangeDiagMethod,		// FIN_STT_CHANGE_DIAG
	&gSys_FinSttDiagnosticMethod,		// FIN_STT_DIAGNOSTIC
	&gSys_FinSttChangeNormalMethod,		// FIN_STT_CHANGE_NORMAL
	&gSys_FinSttStandbyMethod,			// FIN_STT_STANDBY
	&gSys_FinSttCycleUpMethod,			// FIN_STT_CYCLE_UP
	&gSys_FinSttReadyMethod,			// FIN_STT_READY
	&gSys_FinSttCycleDownMethod,		// FIN_STT_CYCLE_DOWN
	&gSys_FinSttHardDownMethod,			// FIN_STT_HARD_DOWN
};


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

static void DoStateMachine();
static void ChangeDownState(ESysProcStopType stopType);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_Constructor
// Description   : �R���X�g���N�^
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_Constructor()
{
	UC index;

	// --- Initialize member ---
	gSys_ManagerMember.mCurrentState		= FIN_STT_UNKNOWN;
	gSys_ManagerMember.mNextState			= FIN_STT_NUM;
	gSys_ManagerMember.mRequestState		= FIN_STT_NUM;
	gSys_ManagerMember.mPowerUpMode			= SYS_PWR_UP_MODE_NORMAL;
	gSys_ManagerMember.mPowerDownReserve	= false;

	// --- Execute state class constructor ---
	for ( index = 0; index < FIN_STT_NUM; index++ ){
		CSysFinisherState_Constructor(cSys_FinSttPtrArray[index]);
	}

	// --- Execute sub module's constructor ---
	CDiagManager_Constructor();
	CShtManager_Constructor();
	CCdiAdapter_Constructor();
	CCvrChecker_Constructor();
	CJamChecker_Constructor();
	COputInformer_Constructor();
	CCapaInformer_Constructor();
	CCfgInformer_Constructor();
	CDvstInformer_Constructor();
	CNmaHandler_Constructor();
	CKeyInformer_Constructor();
	CNvmInformer_Constructor();
	CHfsiInformer_Constructor();
	// CTempInformer_Constructor();	�yROM�팸�z
	CDltManager_Constructor();

	// --- Execute System Manager's SW componenet constructor ---
	CSeqDevInitHandler_Constructor();
	CSeqProcEndHandler_Constructor();
}

///////////////////////////////////////////////////////////////////
// Function Name : DoStateMachine
// Description   : ��ԑJ�ڏ���
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
static void DoStateMachine()
{
	UC loop, postState, nextState;

	loop = true;

	while ( loop ){
		loop = false;

		if ( (gSys_ManagerMember.mNextState != FIN_STT_NUM) &&
			 (gSys_ManagerMember.mCurrentState != gSys_ManagerMember.mNextState) ){
			if ( (CSysFinisherState_IsTransitionState(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState]) == false) ||	// �ߓn��Ԃł͂Ȃ�
				 (CSysFinisherState_IsTransitionState(cSys_FinSttPtrArray[gSys_ManagerMember.mNextState]) == true) ||	// �ߓn��Ԃ���ߓn��Ԃւ̑J��(Cycle Up��Process Down) V3.1.7 
				 ( (CSysFinisherState_CheckExitCondition(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState]) == true) &&	// ��ԑJ�ڏ����͖������Ă���
				   (gSys_ManagerMember.mRequestState == gSys_ManagerMember.mNextState) ) ){									// System Controller����̎w���͎󂯂Ă���
				postState = gSys_ManagerMember.mCurrentState;
				nextState = gSys_ManagerMember.mNextState;

				// --- ��ԑJ�ڑO�̌㏈�� ---
				CSysFinisherState_Exit(cSys_FinSttPtrArray[postState], (EFinisherState)gSys_ManagerMember.mNextState);

				SRVC_UserDebugLog_Wide(0x10,0x10,postState,nextState); //log
				
				// --- ��ԕύX ---
				gSys_ManagerMember.mCurrentState = nextState;					// ��ԑJ��
				CCdiAdapter_SendFinisherState((EFinisherState)nextState);		// CDI-IF�ɏ�ԑJ�ڂ�ʒm

				// --- ��ԑJ�ڌ�̑O���� ---
				CSysFinisherState_Entry(cSys_FinSttPtrArray[nextState], (EFinisherState)postState);

				// --- �ߓn��ԏ��� ---
				if ( CSysFinisherState_IsTransitionState(cSys_FinSttPtrArray[nextState]) == true ){
																				// �ߓn��Ԃł���
					gSys_ManagerMember.mNextState = CSysFinisherState_GetNextState(cSys_FinSttPtrArray[nextState]);

					loop = true;												// ���ɑJ�ڏ����������Ă���ꍇ�����邽�ߍĎ��s
				}
				else{															// �ߓn��ԂłȂ�
					gSys_ManagerMember.mNextState = FIN_STT_NUM;
					gSys_ManagerMember.mRequestState = FIN_STT_NUM;

					if( gSys_ManagerMember.mPowerDownReserve == true ){			// Power Down�v����M�ς�
						gSys_ManagerMember.mNextState = FIN_STT_POWER_DOWN;		// �J�ڐ��PowerDown
						gSys_ManagerMember.mRequestState = FIN_STT_POWER_OFF;
						gSys_ManagerMember.mPowerDownReserve = false;

						loop = true;
					}
					else{
						// No Action
					}
				}
			}
			else{
				// No Action
			}
		}
		else{
			// No Action
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : ChangeDownState
// Description   : �t�B�j�b�V���[�����_�E���X�e�[�g�ڍs����
// Parameter     : stopType (�t�B�j�b�V���O�v���Z�X��~���)
// Return        : �]
///////////////////////////////////////////////////////////////////
static void ChangeDownState(ESysProcStopType stopType)
{
	switch ( stopType ){
	case SYS_PROC_STOP_HARD:
		if ( CSysFinisherState_CheckTransitionPossibility(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], FIN_STT_HARD_DOWN) == true ){
			gSys_ManagerMember.mNextState = FIN_STT_HARD_DOWN;
			DoStateMachine();
		}
		else{
			// No Action
		}

		break;
	case SYS_PROC_STOP_CYCLE:
		if ( CSysFinisherState_CheckTransitionPossibility(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], FIN_STT_CYCLE_DOWN) == true ){
			gSys_ManagerMember.mNextState = FIN_STT_CYCLE_DOWN;
			DoStateMachine();
		}
		else{
			// No Action
		}

		break;
	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_GetFinisherState
// Description   : �t�B�j�b�V���[��Ԏ擾����
// Parameter     : �]
// Return        : ���݂�Finisher State
///////////////////////////////////////////////////////////////////
EFinisherState CSysManager_GetFinisherState()
{
	return (EFinisherState)gSys_ManagerMember.mCurrentState;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_CheckInformationDetect
// Description   : �t�B�j�b�V���[���m��󋵊m�F����
// Parameter     : �]
// Return        : true = �m��ς݁^false = ���m��
///////////////////////////////////////////////////////////////////
bool CSysManager_CheckInformationDetect()
{
	if ( (gSys_ManagerMember.mCurrentState != FIN_STT_UNKNOWN) &&
		 (gSys_ManagerMember.mCurrentState != FIN_STT_POWER_UP) ){
		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_PowerUp
// Description   : Power Up�w����M������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_PowerUp()
{
	UC state = gSys_ManagerMember.mCurrentState;

	if ( CSysFinisherState_CheckTransitionPossibility(cSys_FinSttPtrArray[state], FIN_STT_POWER_UP) == true ){
		gSys_ManagerMember.mNextState = FIN_STT_POWER_UP;
		gSys_ManagerMember.mRequestState = FIN_STT_POWER_ON;
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_PowerDown
// Description   : Power Down�w����M������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_PowerDown()
{
	UC state = gSys_ManagerMember.mCurrentState;
	
	if ( CSysFinisherState_CheckTransitionPossibility(cSys_FinSttPtrArray[state], FIN_STT_POWER_DOWN) == true ){
		gSys_ManagerMember.mNextState = FIN_STT_POWER_DOWN;
		gSys_ManagerMember.mRequestState = FIN_STT_POWER_OFF;
		DoStateMachine();
	}
	else{
		gSys_ManagerMember.mPowerDownReserve = true;

		// --- Finishing Process������ ---
		if ( CSysFinisherState_CheckTransitionPossibility(cSys_FinSttPtrArray[state], FIN_STT_HARD_DOWN) == true ){
			gSys_ManagerMember.mNextState = FIN_STT_HARD_DOWN;
			gSys_ManagerMember.mRequestState = FIN_STT_STANDBY;
			DoStateMachine();
		}
		else{
			if ( gSys_ManagerMember.mNextState == FIN_STT_STANDBY ){
				gSys_ManagerMember.mRequestState = FIN_STT_STANDBY;
				DoStateMachine();
			}
			else{
				// No Action
			}
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ChangeNormal
// Description   : Normal Mode�J�ڎw����M������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_ChangeNormal()
{
	UC state = gSys_ManagerMember.mCurrentState;
	
	if ( CSysFinisherState_CheckTransitionPossibility(cSys_FinSttPtrArray[state], FIN_STT_CHANGE_NORMAL) == true ){
		gSys_ManagerMember.mNextState = FIN_STT_CHANGE_NORMAL;
		gSys_ManagerMember.mRequestState = FIN_STT_STANDBY;
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ChangeDiag
// Description   : Diag Mode�J�ڎw����M������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_ChangeDiag()
{
	UC state = gSys_ManagerMember.mCurrentState;
	
	if ( CSysFinisherState_CheckTransitionPossibility(cSys_FinSttPtrArray[state], FIN_STT_CHANGE_DIAG) == true ){
		gSys_ManagerMember.mNextState = FIN_STT_CHANGE_DIAG;
		gSys_ManagerMember.mRequestState = FIN_STT_DIAGNOSTIC;
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_CycleUp
// Description   : Cycle Up�w����M������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_CycleUp()
{
	UC state = gSys_ManagerMember.mCurrentState;
	
	if ( CSysFinisherState_CheckTransitionPossibility(cSys_FinSttPtrArray[state], FIN_STT_CYCLE_UP) == true ){
		gSys_ManagerMember.mNextState = FIN_STT_CYCLE_UP;
		gSys_ManagerMember.mRequestState = FIN_STT_READY;
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_CycleDown
// Description   : Cycle Down�w����M������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_CycleDown()
{
	UC state = gSys_ManagerMember.mCurrentState;
	
	if ( CSysFinisherState_CheckTransitionPossibility(cSys_FinSttPtrArray[state], FIN_STT_CYCLE_DOWN) == true ){
		gSys_ManagerMember.mNextState = FIN_STT_CYCLE_DOWN;
		gSys_ManagerMember.mRequestState = FIN_STT_STANDBY;
		DoStateMachine();
	}
	else{
		if ( gSys_ManagerMember.mNextState == FIN_STT_STANDBY ){
			gSys_ManagerMember.mRequestState = FIN_STT_STANDBY;
			DoStateMachine();
		}
		else{
			// No Action
		}
	}	
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_HardDown
// Description   : Hard Down�w����M������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_HardDown()
{
	UC state = gSys_ManagerMember.mCurrentState;
	
	if ( CSysFinisherState_CheckTransitionPossibility(cSys_FinSttPtrArray[state], FIN_STT_HARD_DOWN) == true ){
		gSys_ManagerMember.mNextState = FIN_STT_HARD_DOWN;
		gSys_ManagerMember.mRequestState = FIN_STT_STANDBY;
		DoStateMachine();
	}
	else{
		if ( gSys_ManagerMember.mNextState == FIN_STT_STANDBY ){
			gSys_ManagerMember.mRequestState = FIN_STT_STANDBY;
			DoStateMachine();
		}
		else{
			// No Action
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_SendDeviceActionRequest
// Description   : Device Action Request�C�x���g���M����
// Parameter     : eventID (MSP Event ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_SendDeviceActionRequest(US eventID)
{
	CSysFinisherState_SendDeviceActionRequest(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], eventID);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_SendDeviceEventNotify
// Description   : Device Event Notify�C�x���g���M����
// Parameter     : eventID (MSP Event ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_SendDeviceEventNotify(US eventID)
{
	CSysFinisherState_SendDeviceEventNotify(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], eventID);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ReceivePowerUpResponse
// Description   : Power Up Response�C�x���g��M������
// Parameter     : moduleID (�C�x���g���sModule ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_ReceivePowerUpResponse(UC moduleID)
{
	if ( CSysFinisherState_ReceivePowerUpResponse(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], moduleID) == true ){
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ReceivePowerDownResponse
// Description   : Power Down Response�C�x���g��M������
// Parameter     : moduleID (�C�x���g���sModule ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_ReceivePowerDownResponse(UC moduleID)
{
	if ( CSysFinisherState_ReceivePowerDownResponse(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], moduleID) == true ){
		DoStateMachine();
	}
	else{
		// No Action
	}
}

//////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ReceiveChangeDiagResponse
// Description   : Change Diag Response�C�x���g��M������
// Parameter     : moduleID (�C�x���g���sModule ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_ReceiveChangeDiagResponse(UC moduleID)
{
	if ( CSysFinisherState_ReceiveChangeDiagResponse(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], moduleID) == true ){
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ReceiveChangeNormalResponse
// Description   : Change Normal Response�C�x���g��M������
// Parameter     : moduleID (�C�x���g���sModule ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_ReceiveChangeNormalResponse(UC moduleID)
{
	if ( CSysFinisherState_ReceiveChangeNormalResponse(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], moduleID) == true ){
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ReceiveCycleUpResponse
// Description   : Cycle Up Response�C�x���g��M������
// Parameter     : moduleID (�C�x���g���sModule ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_ReceiveCycleUpResponse(UC moduleID)
{
	if ( CSysFinisherState_ReceiveCycleUpResponse(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], moduleID) == true ){
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ReceiveProcessDownResponse
// Description   : Process Down Response�C�x���g��M������
// Parameter     : moduleID (�C�x���g���sModule ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_ReceiveProcessDownResponse(UC moduleID)
{
	if ( CSysFinisherState_ReceiveProcessDownResponse(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], moduleID) == true ){
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ReceiveDeviceActionResponse
// Description   : Device Action Response�C�x���g��M������
// Parameter     : moduleID (�C�x���g���sModule ID)
//               : eventID (MSP Event ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_ReceiveDeviceActionResponse(UC moduleID, US eventID)
{
	if ( CSysFinisherState_ReceiveDeviceActionResponse(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], moduleID, eventID) == true ){
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ReceiveDeviceEventNotify
// Description   : Device Event Notify�C�x���g��M������
// Parameter     : moduleID (�C�x���g���sModule ID)
//               : eventID (MSP Event ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_ReceiveDeviceEventNotify(UC moduleID, US eventID)
{
	CSysFinisherState_ReceiveDeviceEventNotify(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], moduleID, eventID);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_ReceiveDeviceStatusNotify
// Description   : Device Status Notify�C�x���g��M������
// Parameter     : moduleID (�C�x���g���sModule ID)
//               : statusNo (Status No.)
//               : value (Status Value)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_ReceiveDeviceStatusNotify(UC moduleID, US statusNo, US value)
{
	ChangeDownState(CSysFinisherState_ReceiveDeviceStatusNotify(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], moduleID, statusNo, value));
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_SystemConfigurationSet
// Description   : System Configuration�ʒm��M������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_SystemConfigurationSet()
{
	CSysFinisherState_SystemConfigurationSet(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState]);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_PowerUpModeSet
// Description   : �p���[�A�b�v��ʒʒm��M������
// Parameter     : pwrUpMode (�p���[�A�b�v���)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_PowerUpModeSet(ESysPowerUpMode pwrUpMode)
{
	CSysFinisherState* pState;

	pState = cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState];

	if ( CSysFinisherState_PowerUpModeSet(pState, pwrUpMode) == true ){
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_EntranceAreaTampered
// Description   : �G���g�����X��(�W�����N���A)����ʒm��M������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_EntranceAreaTampered()
{
	CSysFinisherState_EntranceAreaTampered(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState]);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_LoadNvmComplete
// Description   : NVM�f�[�^���[�h���������ʒm��M����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_LoadNvmComplete(SS result)
{
	if ( CSysFinisherState_LoadNvmComplete(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], result) == true ){
		DoStateMachine();
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_SaveNvmComplete
// Description   : NVM�f�[�^�Z�[�u���������ʒm��M����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_SaveNvmComplete(SS result)
{
	if ( CSysFinisherState_SaveNvmComplete(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], result) == true ){
		DoStateMachine();
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_SheetManagerReady
// Description   : �V�[�g�}�l�[�W���[�̃��f�B�[�J�ڒʒm��M������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_SheetManagerReady()
{
	CSysFinisherState* pState;

	pState = cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState];

	if ( CSysFinisherState_SheetManagerReady(pState) == true ){
		DoStateMachine();
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_CoverStatusChange
// Description   : �J�o�[��ԕω��ʒm��M������
// Parameter     : coverID (Cover ID)
//               : status (Cover Status)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_CoverStatusChange(ECvrCoverID coverID, ECvrCoverStatus status)
{
	ChangeDownState(CSysFinisherState_CoverStatusChange(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], coverID, status));
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_FaultStatusChange
// Description   : �t�H���g�X�e�[�^�X�ω��ʒm��M������
// Parameter     : statusNo (Device Status No.)
//               : value (Status Value)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_FaultStatusChange(US statusNo, US value)
{
	ChangeDownState(CSysFinisherState_FaultStatusChange(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], statusNo, value));
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysManager_JamZoneStatusChange
// Description   : �W�����]�[���X�e�[�^�X�ω��ʒm��M������
// Parameter     : devContID (Device Controller ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSysManager_JamZoneStatusChange(UC devContID)
{
	CSysFinisherState_JamZoneStatusChange(cSys_FinSttPtrArray[gSys_ManagerMember.mCurrentState], devContID);
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
// 14.12.12	| Yuichi Hoshino	| Trouble Action [�d��ON���C�j�V�����C�Y������{����]
// 14.12.29	| Yuichi Hoshino	| Design Improvement [ Power Up�T�u�V�X�e���݌v������ ]
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVM ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
