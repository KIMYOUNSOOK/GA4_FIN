// File Name: DD_COM_Service.c
// Description:
// Engineer:  Yuichi Hoshino
// Last Edit: 16.02.09
// Revision:  002
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:				| Note:
// ---------+-----------------------+--------------------------------------------------------------------------------------
// 13.10.15 | Mitsuyuki Nishizawa	| Created this file.(CH-Finisher SubCPU Rev003 Base)
// Revision 000
// 15.02.12 | Yuichi Hoshino		| ���b�Z�[�W���O�o�͏����ǉ�
// 15.03.27 | Yuichi Hoshino		| DD_COM_SendMsg()�̖߂�l������
// Revision 001
// 16.02.09	| Yuichi Hoshino		| Download�Ή��̂��߂̃V�[�P���X�r�b�g�擾�֐��ǉ�
// 16.02.09	| Yuichi Hoshino		| �ʐM�t�F�C�������C�x���g��2�d�����h�~�����ǉ�
// Revision 002

#include "Common.h"
#include "LIB_Service.h"
#include "DD_COM_Packet.h"
#include "DD_COM_ParPacketReceiver.h"
#include "DD_COM_ParDataReceiver.h"
#include "DD_COM_Pardatasender.h"
#include "DD_COM_ParPacketSender.h"
#include "DD_SCI.h"
#include "DD_LOG.h"
#include "MSP_Service.h"

// --- Communication State ---
typedef enum{
	DD_COM_STT_UNSYNC,
	DD_COM_STT_SYNC,
	DD_COM_STT_FAIL
} DD_COM_State;

static UC gCom_State;  

void (*RECVFUNC)(UC* RcvDataAddress, US datalength) = NULL;      /* ��M�֐��ւ̃|�C���^     */

//==============================================================================
// Function Name : DD_COM_Init
// Description   : �ʐM������������
// Parameter     : �Ȃ�
// Return        : �Ȃ�
//==============================================================================
void DD_COM_Init(void)
{
	gCom_State = DD_COM_STT_UNSYNC;

	DD_SCI_Init();
	DD_COM_InitParPacketSender();
	DD_COM_InitParPacketReceiver();
	DD_COM_InitParDataSender();
	//DD_COM_InitParDataReceiver(DD_COM_MAX_RECV_DATA_SIZE);	
	DD_COM_InitParDataReceiver();						// �����Ȃ�
}

//==============================================================================
// Function Name : DD_COM_EntryReceiver
// Description   : ��M���W���[����o�^����
// Parameter     : func�@�F��M���W���[���A�h���X
// Return        : �Ȃ�
//==============================================================================
void DD_COM_EntryReceiver (void (*func)(UC * RcvDataAddress, US datalength ))
{
	RECVFUNC = func;
}

//==============================================================================
// Function Name : DD_COM_User_ComSync
// Description   : �ʐM�m���ʒm����
// Parameter     : �Ȃ�
// Return        : �Ȃ�
//==============================================================================
void DD_COM_User_ComSync(void)
{
	if ( gCom_State == DD_COM_STT_UNSYNC ){
		gCom_State = DD_COM_STT_SYNC;

		MSP_SendEvent(MID_FRAME, FRM_SYS_COMM_SYNC, 0, 0, OFF);
	}
	else{ /* No Action */ }
}

//==============================================================================
// Function Name : DD_COM_User_ComFail
// Description   : �ʐM�G���[�ʒm����
// Parameter     : �Ȃ�
// Return        : �Ȃ�
//==============================================================================
void DD_COM_User_ComFail(void)
{
	if ( (gCom_State == DD_COM_STT_UNSYNC) || (gCom_State == DD_COM_STT_SYNC) ){
		gCom_State = DD_COM_STT_FAIL;

		MSP_SendEvent(MID_FRAME, FRM_SYS_COMM_FAIL, 0, 0, OFF);
	}
	else{ /* No Action */ }
}

//==============================================================================
// Function Name : DD_COM_SendMsg
// Description   : ���M�v������
// Parameter     : a_pchDataAddress�@�F���M�f�[�^�ւ̃|�C���^
//                 a_nDataLength�@�F���M�f�[�^��
// Return        : 0 �F����I��
//                 -1�F���s
//==============================================================================
SL DD_COM_SendMsg(const UC* a_pchDataAddress, US a_nDataLength )
{
	static UC const cNgLog[] = {"! DD_COM Send_Message_NG ->"};
	UC key;
	UC packetCount ;
	US USpacketCount;
	UC packetNo;
	US setPos;
	UC setAnswer;
	UC comm_dummy;
	UC pComPacketID;
	DD_COM_SEND_PACKET_TYPE* pComPacket;

	packetCount = 0;

	if ( (gCom_State == DD_COM_STT_SYNC) &&
		 (a_nDataLength != 0) && (a_nDataLength <= DD_COM_MAX_SEND_DATA_SIZE) ) {	// Check data length
		USpacketCount = ( a_nDataLength - 1 ) / (gComPacketMaxLength);
		packetCount = (UC)USpacketCount;
		packetCount++;

		for ( packetNo=1, setPos=0 ; packetNo <= packetCount ; packetNo++, setPos += (gComPacketMaxLength) ) {
			pComPacket = DD_COM_GetSendPacket(&gComSendQueue);		/*�@�̈���m�ہ@*/

			if ( pComPacket != 0 ) {								/* �̈�m�ۂɐ����@*/
				if ( packetNo == packetCount ) {
					setAnswer = DD_COM_SetSendPacketData( &(a_pchDataAddress[ setPos ]),
													(UC)(a_nDataLength - setPos), 
													packetNo, 
													packetCount,
													pComPacket);
				}
				else {
					setAnswer = DD_COM_SetSendPacketData( &(a_pchDataAddress[ setPos ]), 
													(UC)gComPacketMaxLength, 
													packetNo, 
													packetCount,
													pComPacket);
				}

				if ( setAnswer == DD_COM_FALSE ) break;
			}
			else {													/* �̈�m�ۂɎ��s */
				break;
			}
			//tmpList.Put(pComPacket);
		}

		DD_EVT_Put(evSend_ID, evuDD_COM_ParDataSender_ID, 0, 0);

		return E_OK;
	}
	else {
		// NG
	}

	// ���M�w���Ɏ��s
	DD_LOG_Put(LOGID_FINISHER, '0', cNgLog, sizeof(cNgLog), LOG_CATEGORY_DEF);
	DD_LOG_Put(LOGID_FINISHER, 'S', a_pchDataAddress, a_nDataLength, LOG_CATEGORY_DEF);

	return E_NG;
}

//==============================================================================
// Function Name : DD_COM_GetRxSequenceBit  
// Description   : RxSeq Bit�擾����
// Parameter     : �]
// Return        : �V�[�P���X�r�b�g�l [0/1] 
//==============================================================================
UC DD_COM_GetRxSequenceBit(void)
{
	return DD_COM_GetReceiverSequenceBit();
}

//==============================================================================
// Function Name : DD_COM_GetTxSequenceBit
// Description   : TxSeq Bit�擾����
// Parameter     : �]
// Return        : �V�[�P���X�r�b�g�l [0/1] 
//==============================================================================
UC DD_COM_GetTxSequenceBit(void)
{
	return DD_COM_GetSenderSequenceBit();
}
