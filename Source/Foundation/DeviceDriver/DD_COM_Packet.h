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
	UC m_nLength;			/*** パケットのバイトサイズ	 */
	UC m_nPacketNo;			/*** 分割した時のパケット番号(1～ｎ) */
	UC m_nPacketCount;		/*** データを分割した時のパケット数(1～n)	 */
	UC m_chPacket[(DD_COM_MAX_PACKET_SIZE+10)];
}DD_COM_RECV_PACKET_TYPE;

typedef struct {
	DD_COM_RECV_PACKET_TYPE comPacket[DD_COM_NUMBER_S_COM_PACKET];
	//UC  ms_nFreeComPacket = DD_COM_NUMBER_S_COM_PACKET;/** 未使用のComPacketインスタンスの数 */
	US  ms_nMaxPacketSize;
	//パケット使用量確認用
	UC  UsedPacketMax;
	UC  rpoint ;
	UC  wpoint ;
	
}DD_COM_RECV_PACKET_LIST_TYPE;

typedef struct  {
	ReservedFlag reservedFlag;
	UC m_nLength;			/*** パケットのバイトサイズ	 */
	UC m_nPacketNo;			/*** 分割した時のパケット番号(1～ｎ) */
	UC m_nPacketCount;		/*** データを分割した時のパケット数(1～n)	 */
	UC m_chPacket[(DD_COM_MAX_S_PACKET_SIZE+2)];
}DD_COM_SEND_PACKET_TYPE;

typedef struct {
	DD_COM_SEND_PACKET_TYPE comPacket[DD_COM_NUMBER_S_SEND_COM_PACKET];
	//UC  ms_nFreeComPacket = DD_COM_NUMBER_S_COM_PACKET;/** 未使用のComPacketインスタンスの数 */
	US  ms_nMaxPacketSize;
	//パケット使用量確認用
	UC  UsedPacketMax;
	UC  rpoint ;
	UC  wpoint ;
	
}DD_COM_SEND_PACKET_LIST_TYPE;

/* Packetを確保する*/
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
