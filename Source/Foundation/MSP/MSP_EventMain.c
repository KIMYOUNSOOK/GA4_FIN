// ������ Module Software Platform Frame ������
/// @file    MSP_EventMain.c
/// @author  Mitsuyuki Nishizawa
/// @date    15.02.12
/// @version 0002
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define MSP_EventMain_c

#include "Common.h"
#include "LIB_Service.h"

#include "MSP_Common.h"
#include "MSP_Config.h"
#include "MSP_EventID.h"
#include "MSP_ProductSpec.h"
#include "MSP_EventMain.h"
#include "MSP_EventTimer.h"


// ===========================  <<< �O���Q�ƕϐ� >>>  ===========================
extern CEventTimer		msp_EventTimer ;													// [�O���Q��] �C�x���g�^�C�}�[


//=====================================================================================
// Method Name   : MSP_ReceiveEvent()
// Description   : MSP�C�x���g����M���đΉ�����R�[���o�b�N�֐����Ăяo��.
// Parameter     : �Ȃ�	
// Return        : �Ȃ�
//=====================================================================================
void MSP_ReceiveEvent()
{
	const EventControl *pControl ;
	const EventHandler *pHandler ;
	US	index, number, count ;
	UC	from ;
	UC	*pMsg ;
	SS	timerID ;
	
	if ( LIB_rcv_mbf(eMbfFrame, &pMsg) > 0 ) {
		if ( MSP_GetEventIndex(*(US*)(pMsg + 2)) == EVT_TIME_OUT ) {						// < �^�C���A�E�g��C�x���g >�i�C�x���g�^�C�}�[�̃^�C���A�E�g�������{�j
			timerID = *((SS*)(pMsg + 4)) ;
			CEventTimer_Timeout(&msp_EventTimer, timerID) ;
		}
		else {																				// < �^�C���A�E�g��C�x���g�ȊO >
			from   = *pMsg ;
			index  = MSP_GetEventIndex(*(EventID*)(pMsg + 2)) ;								// �C�x���g��C���f�b�N�X�擾
			number = MSP_GetEventNumber(*(EventID*)(pMsg + 2)) ;							// �C�x���g��i���o�[�擾

			pControl = msp_EvtIndexTable[index].mpControlTable ;							// �C�x���g��R���g���[����e�[�u���̐擪�A�h���X�擾
			pHandler = msp_EvtIndexTable[index].mpHandlerTable ;							// �C�x���g��n���h����e�[�u���̐擪�A�h���X�擾
			
			for ( count = 0 ; count < pControl[number].mCount ; count++ ) {						 // [[[ LOOP ]]] �o�^�����̃n���h���[���R�[���o�b�N�i���[�v�j
				(pHandler[pControl[number].mStart + count]).mpHandler(((UC*)(pMsg + 4)), from) ; // +++++ �C�x���g��n���h���̃R�[���o�b�N�����{���� +++++
			}
		}
	}
}

//=====================================================================================
// Function Name : MSP_SendEvent()
// Description   : �C�x���g��ʒm����.
// Parameter     : senderID		"���b�Z�[�W���M�����W���[��ID"
//               : eventID      "�C�x���gID"
//               : pMessage		"���M���b�Z�[�W�i�[�������̐擪�A�h���X"
//               : length		"���M���b�Z�[�W�̃T�C�Y [�P��:Byte]"
//               : log			"���O�o�͉�"
// Return        : �Ȃ�
// Caution       : �{�֐��Ńf�[�^����K�[��Error()���R�[������ƃ��[�v����\����I�I�I
//=====================================================================================
void MSP_SendEvent(UC senderID, US eventID, const UC *pMessage, US length, UC log)
{
	static const char  eMsg1[] = {"FRAME ERROR: Event Tx Error"} ;
	static const char  eMsg2[] = {"FRAME ERROR: Event Tx Buffer Overflow"} ;

	static UL  buffer[(MSP_MAX_MSG_SIZE + 3) / 4 + 1] ;										// ���M�o�b�t�@ [�Œ�z�u�f�[�^]
	UC	*pWrite ;
	SS	result ;
	UC	key ;

	if ( length <= MSP_MAX_MSG_SIZE ) {														// �p�P�b�g��T�C�Y�̃`�F�b�N

	// ----- ���b�Z�[�W���M���� -----
		key = LIB_GetKey() ; LIB_Lock() ;													// ����!!! ���荞�݋֎~ ��������!!!
		buffer[0] = 0x00000000 ;															// �ŏ���4�o�C�g��"0"�N���A����
		pWrite = (UC*)buffer ;																// �������ݐ�A�h���X���Z�b�g����
		*pWrite = senderID ;																// �擪��1�o�C�g�ɑ��M���W���[��ID���Z�b�g����
		memcpy((pWrite + 2), &eventID, 2);													// 3, 4�o�C�g�ڂ�Event ID���l�߂�
		if ( length != 0 ) memcpy((pWrite + 4), pMessage, length) ;							// 5�o�C�g�ڂ��瑗�M�f�[�^���l�߂�i�������݃T�C�Y��0�łȂ��Ƃ��j
		result = LIB_snd_mbf(eMbfFrame, pWrite, length + 4) ;								// ���b�Z�[�W���M
		LIB_Unlock(key) ;																	// ����!!! ���荞�݋֎~ �����܂�!!!

	// ----- �G���[�Ή������O�o�� -----
		if ( result == LIB_MBF_ERR ) {														// ���M�G���[�����I�I�I
			Printf(LOGID_FRAME, 0, (UC*)eMsg1, (US)sizeof(eMsg1)) ;
		}
		else if ( log == LOG_ON ) {															// �f�o�b�O����O�o��
			Printf(LOGID_FRAME, 'S', (UC*)buffer, (US)(length + 4)) ;
		}
	}
	else {																					// �p�P�b�g�T�C�Y���o�b�t�@�[�𒴂���
		Printf(LOGID_FRAME, 0, (UC*)eMsg2, (US)sizeof(eMsg2)) ;
	}
}



// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 14.04.01 | Mitsuyuki Nishizawa | Module Software Platform Frame�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬.
// 14.09.24 | Mitsuyuki Nishizawa | �C�x���g�f�[�^�������ݐ�A�h���X��4�o�C�g���E�ɔz�u.
// 15.02.12 | Yuichi Hoshino      | Printf()�ɓn��ID��Log ID�ɕύX
// 15.03.30 | Yuichi Hoshino      | Release version 0002
//
//


// **************************************************************  [ MSP_EventMain.c : EOF ]  *****************************************************************
