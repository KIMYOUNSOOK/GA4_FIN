// ������ Module Software Platform Frame ������
/// @file    MSP_SubsystemGateway.c
/// @author  Mitsuyuki Nishizawa
/// @date    15.02.12
/// @version 002
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define MSP_SubsystemGateway_c

#include "Common.h"
#include "LIB_Service.h"
#include "DD_COM_Service.h"

#include "MSP_Common.h"
#include "MSP_Config.h"
#include "MSP_EventID.h"
#include "MSP_ProductSpec.h"
#include "MSP_EventMain.h"
#include "MSP_SubsystemGateway.h"


// ========================  <<< �C���X�^���X���� >>>  ========================

// ----- �T�u�V�X�e����Q�[�g�E�F�C -----
CSubsystemGateway msp_IotGateway ;																// IOT�T�u�V�X�e����Q�[�g�E�F�C


//$$$$$$ [[[[[ Class: CSubsystemGateway ]]]]] $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// Class Name    : CSubsystemGateway�i�T�u�V�X�e���̑㗝�l�j
// Description   : �T�u�V�X�e���ւ̃R�}���h�i�X�e�[�^�X�j���M����уR�}���h��C�x���g�̐�����ʒm�����{����.
//               :
// Note          : �ʐM�ΏۂƂȂ�T�u�V�X�e���iController, Finisher, etc.�j�ɑ΂��Čʂ̃C���X�^���X�������K�v�ł�.
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//=[[[ Constructor ]]]=================================================================
// Function Name : CSubsystemGateway::CSubsystemGateway()
// Description   : ���̃N���X�̃R���X�g���N�^.
// Parameter     : *self		"�ΏۃQ�[�g�E�F�C��C���X�^���X�̃A�h���X"
//               : *spec		"�X�y�b�N��e�[�u���ւ̃|�C���^"
// Return        : -
//=====================================================================================
void CSubsystemGateway_CSubsystemGateway(CSubsystemGateway *self, const GatewaySpec *spec)
{
	self->mpSpec = spec ;																	// �X�y�b�N����o�^����
}


//=====================================================================================
// Method Name   : CSubsystemGateway::ReceiveCommand()
// Description   : �R�}���h�i�X�e�[�^�X�j�̎�M����уR�}���h��C�x���g�̐���������.
// Parameter     : *self		"�ΏۃQ�[�g�E�F�C��C���X�^���X�̃A�h���X"
//               : *packet		"�T�u�V�X�e������̎�M�p�P�b�g�i�[�A�h���X�i�Q�Ɛ�j"
//               : length		"��M�p�P�b�g�T�C�Y"
// Return        : �Ȃ�
//=====================================================================================
void CSubsystemGateway_ReceiveCommand(CSubsystemGateway *self, const UC *packet, US length)
{
	static const char  eMsg1[] = {"FRAME ERROR: Invalid Command ID"} ;
	static const char  eMsg2[] = {"FRAME ERROR: Command Rx Buffer Overflow"} ;

	const GatewaySpec *pSpec = self->mpSpec ;
	US	number ;
	US	eventID = MSP_GetEventID(pSpec->mEvtIndex, MSP_EVT_NUM_VOID) ;						// ��M�f�[�^��o�b�t�@�̃C�x���gID������������

//	DD�ɂăf�t�H���g��Log�o�͂���
//	Printf(LOGID_FRAME, 'R', packet, (US)length) ;

	if ( length <= MSP_MAX_CMD_SIZE ) {														// �p�P�b�g��T�C�Y�̃`�F�b�N

	// ----- �R�}���hID����Ή�����C�x���gID������ �� �C�x���g��������M -----
		for ( number = 0 ; number < pSpec->mListSize ; number++ ) {							// [[[ LOOP ]]] �R�}���h����X�g���`�F�b�N����i���[�v�j
			if ( *packet == pSpec->mpCommandList[number] ) {
				eventID = MSP_GetEventID(pSpec->mEvtIndex, number) ;						// �R�}���h��C�x���g�𐶐�����
				self->mSendBuffer.mLength = length ;
				memcpy((self->mSendBuffer.mCommand), packet, length) ;
				MSP_SendEvent(MID_FRAME, eventID, (const UC*)&(self->mSendBuffer), (US)(sizeof(US)+length), LOG_OFF) ;	// �R�}���h��C�x���g�𑗐M����
				break ;
			}
		}																					// [[[ LOOP ]]] �i�����܂Ń��[�v�j

	// ----- �Y������R�}���hID���Ȃ������Ƃ��̏��� -----
		if ( MSP_GetEventNumber(eventID) == MSP_EVT_NUM_VOID ) {							// �Y������R�}���h�������ł��Ȃ�����
			Printf(LOGID_FRAME, 0, (UC*)eMsg1, (US)sizeof(eMsg1)) ;
		}
	}
	else {																					// �p�P�b�g�T�C�Y���o�b�t�@�[�𒴂���
		Printf(LOGID_FRAME, 0, (UC*)eMsg2, (US)sizeof(eMsg2)) ;
	}
}


//=====================================================================================
// Method Name   : CSubsystemGateway::SendCommand()
// Description   : �R�}���h�i�X�e�[�^�X�j�𑗐M����.
// Parameter     : *self		"�ΏۃQ�[�g�E�F�C��C���X�^���X�̃A�h���X"
//               : senderID		"���M�����W���[��ID"
//               : *packet		"���M�p�P�b�g�i�[��A�h���X"
//               : length		"���M�p�P�b�g�T�C�Y"
//               : log			"���M�p�P�b�g�̃��O���ێ��̗L��"
// Return        : �R�}���h���M�̌��ʁi0:���M���� / 1:���M���s�j
//=====================================================================================
UC CSubsystemGateway_SendCommand(CSubsystemGateway *self, UC senderID, const UC *packet, US length, UC log)
{
	static const char  eMsg[] = {"FRAME ERROR: Command Tx Error"} ;

	SL	result ;

// ----- �R�}���h�i�X�e�[�^�X�j���M���� -----
	result = DD_COM_SendMsg(packet, length) ;

// ----- �G���[�Ή������O�o�� -----
	if ( result != E_OK ) {																	// ���M�G���[�����I�I�I
		Printf(LOGID_FRAME, 0, (UC*)eMsg, (US)sizeof(eMsg)) ;
	}

//	DD�ɂăf�t�H���g��Log�o�͂���
//	if ( log == LOG_ON ) {																	// �f�o�b�O����O�o��
//		Printf(LOGID_FRAME, 'S', (UC*)packet, (US)length) ;
//	}

	return	(UC)result ;
}


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 13.04.01 | Mitsuyuki Nishizawa | Module Software Platform Frame�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬.
// 14.11.05 | Mitsuyuki Nishizawa | ����M�֐����̂���ш�����ύX&������������ǉ�.
// 14.12.04 | Yuichi Hoshino      | Release version 001
// 15.02.12 | Yuichi Hoshino      | Printf()�ɓn��ID��Log ID�ɕύX
// 15.02.12 | Yuichi Hoshino      | DD�Ŏ��{���邽�߁A���b�Z�[�W���O�o�͏������R�����g�A�E�g
// 15.03.30 | Yuichi Hoshino      | Release version 002
//
//


// **********************************************************  [ MSP_SubsystemGateway.c : EOF ]  **************************************************************
