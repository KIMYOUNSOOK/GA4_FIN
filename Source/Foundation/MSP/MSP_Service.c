// ������ Module Software Platform Frame ������
/// @file    MSP_Service.c
/// @author  Mitsuyuki Nishizawa
/// @date    14.11.04
/// @version 001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define MSP_Service_c

#include "Common.h"
#include "LIB_Service.h"
#include "DD_Event.h"
#include "DD_DIO.h"
#include "DD_COM_Service.h"

#include "MSP_Common.h"
#include "MSP_Config.h"
#include "MSP_EventID.h"
#include "MSP_ProductSpec.h"
#include "MSP_EventMain.h"
#include "MSP_Location.h"
#include "MSP_Sensor.h"
#include "MSP_SubsystemGateway.h"
#include "MSP_EventTimer.h"


// ===========================  <<< �֐��v���g�^�C�v�錾 >>>  ===========================
void MSP_TickEventTimer() ;
void MSP_ReceiveCommand(UC*, US) ;


// ===========================  <<< �O���Q�ƕϐ� >>>  ===========================
extern CEventTimer	msp_EventTimer ;														// [�O���Q��] �C�x���g�^�C�}�[
extern CSubsystemGateway  msp_IotGateway ;													// [�O���Q��] �T�u�V�X�e����Q�[�g�E�F�C�iIOT�j


// *************************************************************  <<<<< Program Code Area >>>>>  **************************************************************

//###### [[[[[ Module Software Platform Frame Service ]]]]] ###################################################################################################

//=====================================================================================											// <<< SYSTEM EVENT >>>
// Function Name : MSP_StartUp()                                                      											// ��������������������������
// Description   : MSP Frame�̋N�����s��.                                            											// �� "START UP"           ��
// Parameter     : �Ȃ�                                                                											// ��������������������������
// Return        : �Ȃ�
//=====================================================================================
void MSP_StartUp()
{
	UC      testReg[2];
// ----- �R���X�g���N�^���s -----
	CEventTimer_CEventTimer(&msp_EventTimer) ;												// �C�x���g�^�C�}�[�̃R���X�g���N�^���s
	CSubsystemGateway_CSubsystemGateway(&msp_IotGateway, &msp_IotGwSpec) ;					// �T�u�V�X�e����Q�[�g�E�F�C�̃R���X�g���N�^���s
	CTimingController_CTimingController(msp_LocationSpec) ;									// ���P�[�V������^�C�~���O��R���g���[���[�̃R���X�g���N�^���s

// ----- �^�C�}�[�̓o�^ ------
	DD_EVT_EntryCallback(MSP_TickEventTimer) ;

// ----- �Z���T�[�ω��ʒm�֐��̓o�^ ------
	DD_DIO_EntryCallback(CSensorEvent_NotifyEdge) ;

// ----- ��M�֐��̓o�^ ------
	DD_COM_EntryReceiver(MSP_ReceiveCommand) ;

// ----- �A�v���P�[�V�����N���p���b�Z�[�W���M -----
	MSP_SendEvent(MID_FRAME, FRM_SYS_START_UP, 0, 0, OFF) ;									// "START_UP"���b�Z�[�W���M

	// test Reset register..
	testReg[0] = RESF;
	testReg[1] = 0xFF;
		
	DD_LOG_Put(LOGID_TRNS, 'S', testReg, sizeof(testReg), LOG_CATEGORY_DEBUG);

}


//=====================================================================================
// Function Name : MSP_SendMessage()
// Description   : ���b�Z�[�W��C�x���g�𐶐�����.
// Parameter     : senderID		"���b�Z�[�W���M�����W���[��ID"
//               : eventID      "�C�x���g ID"
//               : pMessage		"���M���b�Z�[�W�i�[�������̐擪�A�h���X"
//               : length		"���M���b�Z�[�W�̃T�C�Y [�P��:Byte]"
//               : log			"���M���b�Z�[�W�̃��O���ێ��̗L��"
// Return        : �Ȃ�
//=====================================================================================
void MSP_SendMessage(UC senderID, US eventID, const UC *pMessage, US length, UC log)
{
	MSP_SendEvent(senderID, eventID, pMessage, length, log) ;								// ���b�Z�[�W��C�x���g��ʒm����
}


//=====================================================================================
// Function Name : MSP_SetEventTimerHigh()
// Description   : �^�C���A�E�g��C�x���g�i���D��x�j�̐��������N�G�X�g����.
// Parameter     : delayTime	"�^�C���A�E�g��C�x���g�����̒x������"
//               : pHandler		"�C�x���g�������̃R�[���o�b�N��n���h��"
//               : param		"�n���h�����R�[���o�b�N����ۂ̃p�����[�^"
// Return        : �^�C�}�[�Z�b�g�̌��ʁi����:�^�C�}�[ID / ���s:�G���[�R�[�h�j
// Note          : �^�C�}�[�l=0�Ń^�C�}�[�Z�b�g�����ۂɖ߂�l��"MSP_TMR_TERM"�ƂȂ�.
//=====================================================================================
SS MSP_SetEventTimerHigh(US delayTime, void (*pHandler)(SS, UL), UL param)
{
	Handler  info ;

	info.mpHandler = pHandler ;
	info.mParam    = param ;

	return CEventTimer_Set(&msp_EventTimer, MSP_TMR_HIGH, delayTime, &info) ;				// �C�x���g�^�C�}�[�i���D��x�j���Z�b�g����
}


//=====================================================================================
// Function Name : MSP_SetEventTimer()
// Description   : �^�C���A�E�g��C�x���g�i��D��x�j�̐��������N�G�X�g����.
// Parameter     : delayTime	"�^�C���A�E�g��C�x���g�����̒x������"
//               : pHandler		"�C�x���g�������̃R�[���o�b�N��n���h��"
//               : param		"�n���h�����R�[���o�b�N����ۂ̃p�����[�^"
// Return        : �^�C�}�[�Z�b�g�̌��ʁi����:�^�C�}�[ID / ���s:�G���[�R�[�h�j
//=====================================================================================
SS MSP_SetEventTimer(US delayTime, void (*pHandler)(SS, UL), UL param)
{
	Handler  info ;

	info.mpHandler = pHandler ;
	info.mParam    = param ;

	return CEventTimer_Set(&msp_EventTimer, MSP_TMR_LOW, delayTime, &info) ;				// �C�x���g��^�C�}�[�i��D��x�j���Z�b�g����
}


//=====================================================================================
// Function Name : MSP_CancelEventTimer()
// Description   : �^�C���A�E�g��C�x���g�̐������L�����Z������.
// Parameter     : timerID		"�^�C���A�E�g��C�x���g�����p�^�C�}�[ID"
// Return        : �^�C�}�[�Z�b�g�̌��ʁi�L�����Z������ / �L�����Z�����s�j
//=====================================================================================
SS MSP_CancelEventTimer(SS timerID)
{
	return	CEventTimer_Cancel(&msp_EventTimer, timerID) ;									// �C�x���g��^�C�}�[���L�����Z������
}


//=====================================================================================
// Function Name : MSP_TickEventTimer()
// Description   : �J�E���g�_�E�����������s����.
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �f�o�C�X�h���C�o�[��1ms���荞�ݏ�������R�[������.
//               : �ʏ탆�[�U�[�ɂ͊J�����Ȃ��˃w�b�_�[�Ƀv���g�^�C�v�錾�Ȃ��I�I�I
//=====================================================================================
void MSP_TickEventTimer()
{
	CEventTimer_Tick(&msp_EventTimer) ;														// �C�x���g��^�C�}�[�̃J�E���g�_�E�����������s����
}


//=====================================================================================
// Function Name : MSP_ActivateCommunication()
// Description   : ���̃T�u�V�X�e���ւ̃R�}���h(�X�e�[�^�X)�𑗐M����.
// Parameter     : subsysID		"�R�}���h(�X�e�[�^�X)���M��T�u�V�X�e��ID"
//               : logID		"���O�ɕ\������ID"
// Return        : �Ȃ�
// Note          : �{�T�[�r�X���s�ɂ��ʐM�m���������J�n����
//               : �������{���ʂ�System Event "FRM_SYS_COMM_SYNC"�ɂĒʒm�����
//=====================================================================================
void MSP_ActivateCommunication(UC subsysID, UC logID)
{
	// ������ MSP�ł�DD�������I�ɒʐM���J�n ������
}


//=====================================================================================
// Function Name : MSP_ReceiveCommand()
// Description   : �R�}���h�i�X�e�[�^�X�j�̎�M����уR�}���h��C�x���g�̐���������.
// Parameter     : pPacket		"�T�u�V�X�e������̎�M�p�P�b�g�i�[�A�h���X�i�Q�Ɛ�j"
//               : length		"��M�p�P�b�g�T�C�Y"
// Return        : �Ȃ�
//=====================================================================================
void MSP_ReceiveCommand(UC *pPacket, US length)
{
	CSubsystemGateway_ReceiveCommand(&msp_IotGateway, pPacket, length) ;					// ���T�u�V�X�e������̃R�}���h����M����.
}


//=====================================================================================
// Function Name : MSP_SendCommand()
// Description   : ���̃T�u�V�X�e���ւ̃R�}���h(�X�e�[�^�X)�𑗐M����.
// Parameter     : subsysID		"�R�}���h(�X�e�[�^�X)���M��T�u�V�X�e��ID"
//               : senderID		"�R�}���h(�X�e�[�^�X)���M�����W���[��ID"
//               : pPacket		"���M�R�}���h(�X�e�[�^�X)�i�[�������̐擪�A�h���X"
//               : length		"���M�R�}���h(�X�e�[�^�X)�̃T�C�Y [�P��:Byte]"
//               : log			"���M�R�}���h(�X�e�[�^�X)�̃��O���ێ��̗L��"
// Return        : �Ȃ�
//=====================================================================================
void MSP_SendCommand(UC subsysID, UC senderID, const UC *pPacket, US length, UC log)
{
	CSubsystemGateway_SendCommand(&msp_IotGateway, senderID, pPacket, length, log) ;		// ���T�u�V�X�e���փR�}���h�𑗐M����.
}


//=====================================================================================
// Function Name : MSP_UpdateLocationTiming()
// Description   : ���P�[�V������C�x���g�����^�C�~���O��␳����.
// Parameter     : locationID	"�^�C�~���O�␳�Ώۃ��P�[�V����ID�i=�C�x���gID�j"
//               : pTime		"�␳�f�[�^�i�[�������̐擪�A�h���X"
// Return        : �Ȃ�
//=====================================================================================
void MSP_UpdateLocationTiming(US locationID, const SS *pTime)
{
	CTimingController_UpdateTiming(MSP_GetEventNumber(locationID), pTime) ;					// ���P�[�V���������X�P�W���[���̃^�C�~���O�����X�V����
}


//=====================================================================================
// Function Name : MSP_NotifyTriggerLocation()
// Description   : ���P�[�V������C�x���g�����g���K�[��ʒm����.
// Parameter     : triggerID	"�g���K�[�ƂȂ郍�P�[�V����ID�i=�C�x���gID�j"
//               : sheetNo		"���P�[�V�����̑ΏۂƂȂ�V�[�g��i���o�["
//               : pattern		"�ΏۃV�[�g�̃��P�[�V������p�^�[��"
//               : index		"�ΏۃV�[�g�̃X�P�W���[����C���f�b�N�X�ԍ�"
// Return        : �Ȃ�
//=====================================================================================
void MSP_NotifyTriggerLocation(US triggerID, US sheetNo, UL pattern, UC index)
{
	CTimingController_NotifyTrigger(MSP_GetEventNumber(triggerID), sheetNo, pattern, index) ;	// ���P�[�V���������X�P�W���[�����O���J�n����g���K�[������ʒm����
}


//=====================================================================================
// Function Name : MSP_AbortLocationScheduling()
// Description   : ���P�[�V�����̃X�P�W���[�����O�𒆎~����.�i�S�Ẵ^�C�}�[��~�j
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Caution       : �{�֐��̓A�v���P�[�V�����ɂ����s�֎~�I�I�I
//=====================================================================================
void MSP_AbortLocationScheduling()
{
	CTimingController_AbortScheduling() ;													// ���P�[�V���������̃X�P�W���[�����O�𒆎~����
}


//=====================================================================================
// Function Name : MSP_ResetLocationSchedule()
// Description   : ���P�[�V�����̃X�P�W���[��������������.
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Caution       : �{�֐��̓A�v���P�[�V�����ɂ����s�֎~�I�I�I
//=====================================================================================
void MSP_ResetLocationSchedule()
{
	CTimingController_ResetSchedule() ;														// ���P�[�V���������X�P�W���[����e�[�u��������������
}


//=====================================================================================
// Function Name : MSP_CreateLocationSchedule()
// Description   : ���P�[�V�����̃X�P�W���[�����\�z����.
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Caution       : �{�֐��̓A�v���P�[�V�����ɂ����s�֎~�I�I�I
//=====================================================================================
void MSP_CreateLocationSchedule()
{
	CTimingController_CreateSchedule() ;													// ���P�[�V���������X�P�W���[����e�[�u�����\�z����
}


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 14.04.01 | Mitsuyuki Nishizawa | Module Software Platform Frame�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬.
// 14.11.04 | Mitsuyuki Nishizawa | MSP�񋟊֐����̕ύX�ɔ����C��.
//
//


// **************************************************************  [ MSP_Service.c : EOF ]  *******************************************************************
