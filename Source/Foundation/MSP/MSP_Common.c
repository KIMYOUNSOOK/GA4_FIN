// ������ Module Software Platform Frame ������
/// @file    MSP_Common.c
/// @author  Mitsuyuki Nishizawa
/// @date    14.04.01
/// @version 000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#include "Common.h"
#include "LIB_Service.h"
#include "MSP_Common.h"
#include "MSP_Config.h"
#include "MSP_EventID.h"
#include "MSP_ProductSpec.h"
#include "MSP_EventTimer.h"

// ===========================  <<< �O���Q�ƕϐ� >>>  ===========================
extern CEventTimer		msp_EventTimer ;													// [�O���Q��] �C�x���g�^�C�}�[


static US EVENT_ID(const UC *ptr) { return  *((US*)ptr) ; }										// �C�x���g�ʒm�p�P�b�g����C�x���gID���擾����}�N��

//=====================================================================================
// Method Name   : MSP_ReceiveEvent()
// Description   : MSP�C�x���g����M���đΉ�����R�[���o�b�N�֐����Ăяo��
// Parameter     : �Ȃ�	
// Return        : �Ȃ�
//=====================================================================================
void MSP_ReceiveEvent(void)
{
	const EventControl *pControl ;
	const EventHandler *pHandler ;
	US	index, number, count ;
	UC	from ;
	UC* pMsg ;
	Handler* pTmHandler ;
	SS timerID ;
	
	if ( LIB_rcv_mbf(eMbfFrame, pMsg) > 0 ) {
		if ( EVENT_ID(pMsg) == EVT_TIME_OUT ) {												// �^�C���A�E�g�C�x���g																
			timerID = *((SS*)(pMsg + 6)) ;
			CEventTimer_Timeout(&msp_EventTimer, timerID) ;
		}
		else {
			from   	= *pMsg ;
			index  	= MSP_GetEventIndex(*(EventID*)(pMsg + 4)) ;
			number 	= MSP_GetEventNumber(*(EventID*)(pMsg + 4)) ;

			pControl = msp_EvtIndexTable[index].mpControlTable ;							// �C�x���g��R���g���[����e�[�u���̐擪�A�h���X�擾
			pHandler = msp_EvtIndexTable[index].mpHandlerTable ;							// �C�x���g��n���h����e�[�u���̐擪�A�h���X�擾
			
			for ( count = 0 ; count < pControl[number].mCount ; count++ ) {							// [[[ LOOP ]]] �o�^�����̃n���h���[���R�[���o�b�N�i���[�v�j
				(pHandler[pControl[number].mStart + count]).mpHandler(((UC*)(pMsg + 6)), from) ;	// +++++ �C�x���g��n���h���̃R�[���o�b�N�����{���� +++++
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
//	ER	result ;
	UC	key ;

	if ( length <= MSP_MAX_MSG_SIZE ) {														// �p�P�b�g��T�C�Y�̃`�F�b�N

	// ----- ���b�Z�[�W���M���� -----
		key = LIB_GetKey() ; LIB_Lock() ;													// ����!!! ���荞�݋֎~ ��������!!!
		buffer[0] = 0x00000000 ;															// �ŏ���4�o�C�g��"0"�N���A����
		pWrite = (UC*)buffer ;																// �������ݐ�A�h���X���Z�b�g����
		*pWrite = senderID ;																// �擪��1�o�C�g�ɑ��M���W���[��ID���Z�b�g����
		memcpy((pWrite + 4), &eventID, 2);													// 5, 6�o�C�g�ڂ�Event ID���l�߂�
		memcpy((pWrite + 6), pMessage, length) ;											// 7�o�C�g�ڂ��瑗�M�f�[�^���l�߂�
		LIB_snd_mbf(eMbfFrame, pWrite, length + 6) ;										// ���b�Z�[�W���M
		LIB_Unlock(key) ;																	// ����!!! ���荞�݋֎~ �����܂�!!!

//	// ----- �G���[�Ή������O�o�� -----
//		if ( result != E_OK ) {																// ���M�G���[�����I�I�I
//    		Printf(MID_FRAME, 0, (UC*)eMsg1, (US)sizeof(eMsg1)) ;
//		}
//		else if ( log == LOG_ON ) {															// �f�o�b�O����O�o��
//			Printf(MID_FRAME, 'S', (UC*)buffer, (US)(length + 4)) ;
//		}
	}
	else {																					// �p�P�b�g�T�C�Y���o�b�t�@�[�𒴂���
   		Printf(MID_FRAME, 0, (UC*)eMsg2, (US)sizeof(eMsg2)) ;
	}
}



// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+-----------------------------------------------------------------------------------------------------------------------------
// 14.04.01 | Mitsuyuki Nishizawa | Module Software Platform Frame�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬.
//


// **************************************************************  [ MSP_Frame.cpp : EOF ]  **************************************************************
