// File Name: ComDataReceiver.cpp
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
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev005 Base)
// Revision 000

#ifndef DD_COM_PAR_DATA_RECEIVER_C
#define DD_COM_PAR_DATA_RECEIVER_C

#include "Common.h"
#include "LIB_Lock.h"
#include "DD_Event.h"
#include "DD_COM_ParDataReceiver.h"
#include "DD_COM_ParPacketReceiver.h"
#include "DD_COM_CommonDef.h"
#include "DD_COM_Packet.h"
#include "DD_COM_Service.h"

typedef enum  {
//	stNonState=0,
	stIdle_ID = 1,
	stPacketReceive_ID,
	stReceiveData_ID,
	stFail_ID,
	stComFail_ID,
	stCancel_ID		//state追加[12.10.25]
}DD_COM_ParDataReceiverStateID;

static DD_COM_ParDataReceiverStateID gState;
static US m_nDataLength;
static UC m_chData[256];

extern void   (*RECVFUNC)(UC* RcvDataAddress, US datalength);  	// DD_COM_Service.cに実体を定義

//==============================================================================
// Function Name : DD_COM_InitParDataReceiver
// Description   : 初期化する
// Parameter     : なし
// Return        : なし
//==============================================================================
void DD_COM_InitParDataReceiver( )
{
    m_nDataLength= 0 ;
    gState = stIdle_ID;   		// 状態の初期化
 // 受信開始処理はDD_COM_Initで行う
}

//==============================================================================
// Function Name : DD_COM_GetParRecvData
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static UC DD_COM_GetParRecvData(US  a_i1)
{
    return	m_chData[a_i1];
}

//==============================================================================
// Function Name : DD_COM_SetParRecvData
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void DD_COM_SetParRecvData(US  a_i1, UC  a_chData) 
{
    m_chData[a_i1] = a_chData;
}

//==============================================================================
// Function Name : DD_COM_ReceiveUser_ReceiveData
// Description   : 受信通知処理
// Parameter     : prm1　：受信データへのポインタ
//		　　a_nDataLength　：受信データ長
// Return        : なし
//==============================================================================
void DD_COM_ReceiveUser_ReceiveData(UC* prm1, US a_nDataLength)
{
	if (RECVFUNC != 0) {
		RECVFUNC(prm1, a_nDataLength);
	}
	//test	Comm_SendMsg(prm1, a_nDataLength);
}

//==============================================================================
// Function Name : stIdle
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stIdle(void)
{
	gState = stIdle_ID;
    m_nDataLength =  0 ;
}


//==============================================================================
// Function Name : stPacketReceive
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stPacketReceive(void)
{
	DD_COM_RECV_PACKET_TYPE * pPacket;
	int index;
	
	gState = stPacketReceive_ID;

	// List Check
	pPacket = &(gComRecvQueue.comPacket[gComRecvQueue.rpoint]);
	gComRecvQueue.rpoint++;
	
	if(gComRecvQueue.rpoint >= DD_COM_NUMBER_S_COM_PACKET) {
		gComRecvQueue.rpoint = 0;
	}
	
	if( pPacket == 0 ) {
		 // Create(evGoIdle_ID);
		DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParDataReceiver_ID, 0, 0);
		return;
	}
	
	if( pPacket->reservedFlag == eReleased ) {	//無効データ
		DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParDataReceiver_ID, 0, 0);
		return;
	}
	
	if( pPacket->m_nLength == 0 ) {	//Length=0のデータは破棄する
		DD_COM_DestroyRecvPacket(&gComRecvQueue,pPacket);
		DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParDataReceiver_ID, 0, 0);
		return;
	}

	if( (m_nDataLength + pPacket->m_nLength ) > DD_COM_MAX_RECV_DATA_SIZE ) {
		DD_COM_DestroyRecvPacket(&gComRecvQueue, pPacket);
    	DD_EVT_Put(evGoFail_ID, evuDD_COM_ParDataReceiver_ID, 0, 0);
       	return;
	}
		// Message to Data
	for( index = 0 ; index < pPacket->m_nLength ; index++ ) {
		DD_COM_SetParRecvData( m_nDataLength, pPacket->m_chPacket[index] );
		m_nDataLength=( m_nDataLength + 1 );
	}
	
	// End Check
	if( pPacket->m_nPacketNo != pPacket->m_nPacketCount ) { // Multi Packet
	// Delete Packet
		DD_COM_DestroyRecvPacket(&gComRecvQueue,pPacket);
		return;
	}
	else {
		// Delete Packet
		DD_COM_DestroyRecvPacket(&gComRecvQueue,pPacket);
		// Send Packet
		DD_EVT_Put(evGoComplete_ID, evuDD_COM_ParDataReceiver_ID, 0, 0);
		return;
	}
}

//==============================================================================
// Function Name : ReceiveData
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stReceiveData(void)
{
	gState = stReceiveData_ID;

	DD_COM_ReceiveUser_ReceiveData( &m_chData[0], m_nDataLength );
	DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParDataReceiver_ID, 0, 0);
}

//==============================================================================
// Function Name : stFail
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stFail(void)
{
	gState = stFail_ID;
	// Notice Fail Status
	DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParDataReceiver_ID, 0, 0);

}
//==============================================================================
// Function Name : stComFail
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stComFail(void)
{
 	gState = stComFail_ID;
}

//==============================================================================
// Function Name : stCancel
// Description   : 受信パケットが有る場合(受信パケットを解放する)
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stCancel(void)
{
	DD_COM_RECV_PACKET_TYPE * pPacket;
	
	gState = stCancel_ID;
	pPacket = &(gComRecvQueue.comPacket[gComRecvQueue.rpoint]);
	gComRecvQueue.rpoint++;
	
	if(gComRecvQueue.rpoint >= DD_COM_NUMBER_S_COM_PACKET) {
		gComRecvQueue.rpoint = 0;
	}
	
	if(pPacket != 0){ 
		// Delete Packet
		DD_COM_DestroyRecvPacket(&gComRecvQueue,pPacket);
	}
	
 	DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParDataReceiver_ID, 0, 0);
}

//==============================================================================
// Function Name : DD_COM_ParDataReceiverNotify
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
void DD_COM_ParDataReceiverNotify(DD_EventID eventId,void *arg)
{
	switch (eventId) {
	case evGoComplete_ID:
		switch (gState)
		{
		case stPacketReceive_ID:
			stReceiveData();
			break;
			default:
			break;
		}
		break;
	case evGoFail_ID:
		switch (gState)
		{
		case stPacketReceive_ID:
			stFail();
			break;
			default:
			break;
		}
		break;
	case evGoIdle_ID:
		switch (gState)
		{
		case stPacketReceive_ID:
		case stReceiveData_ID:
		case stFail_ID:
			stIdle();
			break;
		case stCancel_ID:
			stComFail();
			break;
		default:
			break;
		}
		break;
	case evReceive_ID:
		switch (gState)
		{
		case stIdle_ID:
		case stPacketReceive_ID:
			stPacketReceive();
			break;
		case stComFail_ID:
			stCancel();
			break;
		default:
			break;
		}
		break;
	case evReset_ID:
		switch (gState)
		{
		case stIdle_ID:
		case stPacketReceive_ID:
			stIdle();
			break;
		default:
			break;
		}
		break;
	case evFail_ID:
			stComFail();
		break;

	default:
		break;
	}
}

#endif