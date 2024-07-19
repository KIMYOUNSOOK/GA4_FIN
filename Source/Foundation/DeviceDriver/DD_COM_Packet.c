// File Name: DD_COM_Packet.c
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


#include "Common.h"
#include "LIB_Lock.h"
#include "DD_COM_Commondef.h"
#include "DD_COM_Packet.h"


DD_COM_SEND_PACKET_LIST_TYPE	gComSendQueue;
DD_COM_RECV_PACKET_LIST_TYPE 	gComRecvQueue;
US	gComPacketMaxLength; 

//==============================================================================
// Function Name : DD_COM_GetRecvPacket
// Description   : 新しいパケットを確保する
// Parameter     : なし
// Return        : なし
//==============================================================================
DD_COM_RECV_PACKET_TYPE* DD_COM_GetRecvPacket(DD_COM_RECV_PACKET_LIST_TYPE* p_comPacketList)
{
	UC packetIndex;
	UC key = LIB_GetKey();
	
    LIB_Lock();
    packetIndex = p_comPacketList->wpoint;
    
 	if ( eReleased == p_comPacketList->comPacket[packetIndex].reservedFlag ) {
		p_comPacketList->comPacket[packetIndex].reservedFlag = eReserved;
		LIB_Unlock(key);
		return (&(p_comPacketList->comPacket[packetIndex]));
	}
	
	LIB_Unlock(key);

	return 0;	//Error QUEUE FULL

}

//==============================================================================
// Function Name : DD_COM_GetSendPacket
// Description   : 新しいパケットを確保する
// Parameter     : なし
// Return        : なし
//==============================================================================
DD_COM_SEND_PACKET_TYPE* DD_COM_GetSendPacket(DD_COM_SEND_PACKET_LIST_TYPE* p_comPacketList)
{
	UC packetIndex;
	UC key = LIB_GetKey();
	
    LIB_Lock();
    packetIndex = p_comPacketList->wpoint;
 	
 	if ( eReleased == p_comPacketList->comPacket[packetIndex].reservedFlag ) {
		p_comPacketList->comPacket[packetIndex].reservedFlag = eReserved;
		p_comPacketList->wpoint++;
		if (p_comPacketList->wpoint >= DD_COM_NUMBER_S_SEND_COM_PACKET ){
			p_comPacketList->wpoint = 0;
		}
		LIB_Unlock(key);
		return (&(p_comPacketList->comPacket[packetIndex]));
	}
	
	LIB_Unlock(key);

	return 0;	//Error QUEUE FULL
}

//==============================================================================
// Function Name : DD_COM_DestroyRecvPacket
// Description   : パケットを解放する
// Parameter     : なし
// Return        : なし
//==============================================================================
void DD_COM_DestroyRecvPacket(DD_COM_RECV_PACKET_LIST_TYPE* p_comPacketList,DD_COM_RECV_PACKET_TYPE* p_comPacket)
{
	UC key = LIB_GetKey();
	
	LIB_Lock();
	p_comPacket->reservedFlag = eReleased;
    LIB_Unlock(key);
}

//==============================================================================
// Function Name : DD_COM_DestroyRecvPacket
// Description   : パケットを解放する
// Parameter     : なし
// Return        : なし
//==============================================================================
void DD_COM_DestroySendPacket(DD_COM_SEND_PACKET_LIST_TYPE* p_comPacketList,DD_COM_SEND_PACKET_TYPE* p_comPacket)
{
	UC key = LIB_GetKey();
	
	LIB_Lock();
	p_comPacket->reservedFlag = eReleased;
    LIB_Unlock(key);
}

//==============================================================================
// Function Name : DD_COM_SetSendPacketData
// Description   : 送信データをセットする
// Parameter     : なし
// Return        : なし
//==============================================================================
UC DD_COM_SetSendPacketData(const UC* a_pchSetPacketData, UC a_nSetPacketLength, UC a_nSetPacketNo, UC a_nSetPacketCount, DD_COM_SEND_PACKET_TYPE* p_comPacket)
{
	UC index;

	if ( a_nSetPacketLength > DD_COM_MAX_S_PACKET_SIZE ) {
		p_comPacket->m_nLength = 0;
		return DD_COM_FALSE;
	}
       
	for ( index = 0 ; index < a_nSetPacketLength ; index++ ) {
        p_comPacket->m_chPacket[index] = a_pchSetPacketData[index];
    }
    
    p_comPacket->m_nLength= a_nSetPacketLength ;
    p_comPacket->m_nPacketNo= a_nSetPacketNo ;
    p_comPacket->m_nPacketCount =  a_nSetPacketCount;
    
	return DD_COM_TRUE;
}
