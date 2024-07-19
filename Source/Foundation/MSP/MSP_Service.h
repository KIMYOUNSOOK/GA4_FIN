// ������ Module Software Platform Frame ������
/// @file    MSP_Service.h
/// @author  Mitsuyuki Nishizawa
/// @date    14.11.04
/// @version 001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef MSP_Service_h
#define MSP_Service_h

#include "MSP_Common.h"
#include "MSP_Config.h"
#include "MSP_EventID.h"
#include "MSP_ProductSpec.h"
#include "MSP_MessageType.h"
#include "MSP_EventMain.h"


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

//=====================================================================================											// <<< SYSTEM EVENT >>>
// Function Name : MSP_StartUp()                                                      											// ��������������������������
// Description   : ��iSP Frame�̋N�����s��.                                            											// �� "START UP"           ��
// Parameter     : �Ȃ�                                                                											// ��������������������������
// Return        : �Ȃ�
//=====================================================================================
void MSP_StartUp() ;

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
void MSP_SendMessage(UC senderID, US eventID, const UC *pMessage, US length, UC log) ;

//=====================================================================================
// Function Name : MSP_SetEventTimerHigh()
// Description   : �^�C���A�E�g��C�x���g�i���D��x�j�̐��������N�G�X�g����.
// Parameter     : delayTime	"�^�C���A�E�g��C�x���g�����̒x������"
//               : pHandler		"�C�x���g�������̃R�[���o�b�N��n���h��"
//               : param		"�n���h�����R�[���o�b�N����ۂ̃p�����[�^"
// Return        : �^�C�}�[�Z�b�g�̌��ʁi����:�^�C�}�[ID / ���s:�G���[�R�[�h�j
//=====================================================================================
SS MSP_SetEventTimerHigh(US delayTime, void (*pHandler)(SS, UL), UL param) ;

//=====================================================================================
// Function Name : MSP_SetEventTimer()
// Description   : �^�C���A�E�g��C�x���g�i��D��x�j�̐��������N�G�X�g����.
// Parameter     : delayTime	"�^�C���A�E�g��C�x���g�����̒x������"
//               : pHandler		"�C�x���g�������̃R�[���o�b�N��n���h��"
//               : param		"�n���h�����R�[���o�b�N����ۂ̃p�����[�^"
// Return        : �^�C�}�[�Z�b�g�̌��ʁi����:�^�C�}�[ID / ���s:�G���[�R�[�h�j
//=====================================================================================
SS MSP_SetEventTimer(US delayTime, void (*pHandler)(SS, UL), UL param) ;

//=====================================================================================
// Function Name : MSP_CancelEventTimer()
// Description   : �^�C���A�E�g��C�x���g�̐������L�����Z������.
// Parameter     : timerID		"�^�C���A�E�g��C�x���g�����p�^�C�}�[ID"
// Return        : �^�C�}�[�Z�b�g�̌��ʁi�L�����Z������ / �L�����Z�����s�j
//=====================================================================================
SS MSP_CancelEventTimer(SS timerID) ;

//=====================================================================================
// Function Name : MSP_ActivateCommunication()
// Description   : ���̃T�u�V�X�e���ւ̃R�}���h(�X�e�[�^�X)�𑗐M����.
// Parameter     : subsysID		"�R�}���h(�X�e�[�^�X)���M��T�u�V�X�e��ID"
//               : logID		"���O�ɕ\������ID"
//               : failFunc		"�ʐM�t�F�C�����ɃR�[���o�b�N�����֐�"
// Return        : �Ȃ�
// Note          : �{�T�[�r�X���s�ɂ��ʐM�m���������J�n����
//               : �������{���ʂ�System Event "FRM_SYS_COMM_SYNC"�ɂĒʒm�����
//=====================================================================================
void MSP_ActivateCommunication(UC subsysID, UC logID) ;

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
void MSP_SendCommand(UC subsysID, UC senderID, const UC *pPacket, US length, UC log) ;

//=====================================================================================
// Function Name : MSP_UpdateLocationTiming()
// Description   : ���P�[�V������C�x���g�����^�C�~���O��␳����.
// Parameter     : locationID	"�^�C�~���O�␳�Ώۃ��P�[�V����ID�i=�C�x���gID�j"
//               : pTime		"�␳�f�[�^�i�[�������̐擪�A�h���X"
// Return        : �Ȃ�
//=====================================================================================
void MSP_UpdateLocationTiming(US locationID, const SS *pTime) ;

//=====================================================================================
// Function Name : MSP_NotifyTriggerLocation()
// Description   : ���P�[�V������C�x���g�����g���K�[��ʒm����.
// Parameter     : triggerID	"�g���K�[�ƂȂ郍�P�[�V����ID�i=�C�x���gID�j"
//               : sheetNo		"���P�[�V�����̑ΏۂƂȂ�V�[�g��i���o�["
//               : pattern		"�ΏۃV�[�g�̃��P�[�V������p�^�[��"
//               : index		"�ΏۃV�[�g�̃X�P�W���[����C���f�b�N�X�ԍ�"
// Return        : �Ȃ�
//=====================================================================================
void MSP_NotifyTriggerLocation(US triggerID, US sheetNo, UL pattern, UC index) ;

//=====================================================================================
// Function Name : MSP_AbortLocationScheduling()
// Description   : ���P�[�V�����̃X�P�W���[�����O�𒆎~����.�i�S�Ẵ^�C�}�[��~�j
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Caution       : �{�֐��̓A�v���P�[�V�����ɂ����s�֎~�I�I�I
//=====================================================================================
void MSP_AbortLocationScheduling() ;

//=====================================================================================
// Function Name : MSP_ResetLocationSchedule()
// Description   : ���P�[�V�����̃X�P�W���[��������������.
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Caution       : �{�֐��̓A�v���P�[�V�����ɂ����s�֎~�I�I�I
//=====================================================================================
void MSP_ResetLocationSchedule() ;

//=====================================================================================
// Function Name : MSP_CreateLocationSchedule()
// Description   : ���P�[�V�����̃X�P�W���[�����\�z����.
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Caution       : �{�֐��̓A�v���P�[�V�����ɂ����s�֎~�I�I�I
//=====================================================================================
void MSP_CreateLocationSchedule() ;


#endif			// MSP_Service_h


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 14.05.05 | Mitsuyuki Nishizawa | Module Software Platform Frame�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬.
// 14.11.04 | Mitsuyuki Nishizawa | MSP�񋟊֐����̕ύX�ɔ����C��.
//
//


// ****************************************************************  [ MSP_Service.h : EOF ]  *****************************************************************
