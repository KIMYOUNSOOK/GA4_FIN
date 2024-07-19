// File Name: DD_COM_Packet.h
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 13.10.15
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev002 Base)
// Revision 000

#ifndef DD_COM_PACKET_H
#define DD_COM_PACKET_H

#include "DD_COM_Commondef.h"

typedef enum {
	eReleased=0,
	eReserved,
}ReservedFlag;

typedef struct  {
	ReservedFlag reservedFlag;
	UC m_nLength;			/*** �p�P�b�g�̃o�C�g�T�C�Y	 */
	UC m_nPacketNo;			/*** �����������̃p�P�b�g�ԍ�(1�`��) */
	UC m_nPacketCount;		/*** �f�[�^�𕪊��������̃p�P�b�g��(1�`n)	 */
	UC m_chPacket[(DD_COM_MAX_PACKET_SIZE+10)];
}DD_COM_RECV_PACKET_TYPE;

typedef struct {
	DD_COM_RECV_PACKET_TYPE comPacket[DD_COM_NUMBER_S_COM_PACKET];
	//UC  ms_nFreeComPacket = DD_COM_NUMBER_S_COM_PACKET;/** ���g�p��ComPacket�C���X�^���X�̐� */
	US  ms_nMaxPacketSize;
	//�p�P�b�g�g�p�ʊm�F�p
	UC  UsedPacketMax;
	UC  rpoint ;
	UC  wpoint ;
	
}DD_COM_RECV_PACKET_LIST_TYPE;

typedef struct  {
	ReservedFlag reservedFlag;
	UC m_nLength;			/*** �p�P�b�g�̃o�C�g�T�C�Y	 */
	UC m_nPacketNo;			/*** �����������̃p�P�b�g�ԍ�(1�`��) */
	UC m_nPacketCount;		/*** �f�[�^�𕪊��������̃p�P�b�g��(1�`n)	 */
	UC m_chPacket[(DD_COM_MAX_S_PACKET_SIZE+2)];
}DD_COM_SEND_PACKET_TYPE;

typedef struct {
	DD_COM_SEND_PACKET_TYPE comPacket[DD_COM_NUMBER_S_SEND_COM_PACKET];
	//UC  ms_nFreeComPacket = DD_COM_NUMBER_S_COM_PACKET;/** ���g�p��ComPacket�C���X�^���X�̐� */
	US  ms_nMaxPacketSize;
	//�p�P�b�g�g�p�ʊm�F�p
	UC  UsedPacketMax;
	UC  rpoint ;
	UC  wpoint ;
	
}DD_COM_SEND_PACKET_LIST_TYPE;

/* Packet���m�ۂ���*/
DD_COM_RECV_PACKET_TYPE * DD_COM_GetRecvPacket(DD_COM_RECV_PACKET_LIST_TYPE* p_comPacketList);
DD_COM_SEND_PACKET_TYPE * DD_COM_GetSendPacket(DD_COM_SEND_PACKET_LIST_TYPE* p_comPacketList);

UC DD_COM_SetSendPacketData(const UC* a_pchSetPacketData, UC a_nSetPacketLength, 
UC a_nSetPacketNo , UC a_nSetPacketCount ,DD_COM_SEND_PACKET_TYPE* p_comPacket);


void DD_COM_DestroyRecvPacket(DD_COM_RECV_PACKET_LIST_TYPE* p_comPacketList,DD_COM_RECV_PACKET_TYPE* p_comPacket);	
void DD_COM_DestroySendPacket(DD_COM_SEND_PACKET_LIST_TYPE* p_comPacketList,DD_COM_SEND_PACKET_TYPE* p_comPacket);	


#ifndef COMPACKET_C
extern	DD_COM_SEND_PACKET_LIST_TYPE  gComSendQueue;
extern  DD_COM_RECV_PACKET_LIST_TYPE  gComRecvQueue;

extern US gComPacketMaxLength;
extern US ms_nMaxPacketSize;

#endif

#endif // COMPACKET_H
