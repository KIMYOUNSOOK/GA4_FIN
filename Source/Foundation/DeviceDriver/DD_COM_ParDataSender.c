// File Name: DD_COM_ParDataSender.c
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
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev000 Base)
// Revision 000

#ifndef PARDATASENDER_C
#define PARDATASENDER_C

#include "Common.h"
#include "LIB_Lock.h"
#include "DD_Event.h"

#include "DD_COM_pardatasender.h"
#include "DD_COM_Commondef.h"
#include "DD_COM_Packet.h"
#include "DD_COM_Service.h"

typedef enum{
	// NonState = 0,
	stReStart_ID = 1,
	stIdle_ID,
	stReset_ID,
	stPreExec_ID,
	stInit_ID,
	stFail_ID,
	stSending_ID,
	stRelease_ID,
}DD_COM_ParDataSenderStatusID;
	
DD_COM_ParDataSenderStatusID state;
extern UC comm_dummy;
static DD_COM_SEND_PACKET_TYPE *m_pcItsComPacket;

void DD_COM_User_ComFail(void);		// DD_COM_Service.cに関数実体を定義

//==============================================================================
// Function Name : DD_COM_InitParDataSender
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
void DD_COM_InitParDataSender() 
{
	gComPacketMaxLength = DD_COM_MAX_SEND_PACKET_SIZE;
	state = stInit_ID;

	//送信QUEUE初期化
	gComSendQueue.ms_nMaxPacketSize = DD_COM_MAX_PACKET_SIZE;
	//パケット使用量確認用
	gComSendQueue.UsedPacketMax=DD_COM_NUMBER_S_SEND_COM_PACKET;
	gComSendQueue.rpoint = 0;
	gComSendQueue.wpoint = 0;
	DD_EVT_Put(evComplete_ID, evuDD_COM_ParPacketSender_ID, 0, 0);
}


//==============================================================================
// Function Name : DD_COM_StopParDataSender
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
US  DD_COM_StopParDataSender()
{
	return(0);
}

//==============================================================================
// Function Name : stInit
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stInit(void) 
{
	state = stInit_ID;
	// No Action
}

//==============================================================================
// Function Name : stIdle
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stIdle(void) 
{
	state = stIdle_ID;
	// No Action
}

//==============================================================================
// Function Name : stSending
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stSending(void) 
{
	// Check Packet List
	DD_COM_SEND_PACKET_TYPE* compacket;
	
	state = stSending_ID;
	
	//listにパケットがある
	if(eReleased != (gComSendQueue.comPacket[gComSendQueue.rpoint].reservedFlag)){
		 
		compacket = &(gComSendQueue.comPacket[gComSendQueue.rpoint]);
		if( compacket == DD_COM_NULL )
		{
			m_pcItsComPacket = DD_COM_NULL ;	/*リストから取得したパケットを記憶しておく*/
			
			DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParDataSender_ID, 0, 0);
			return;
		}
	m_pcItsComPacket = compacket;		/*リストから取得したパケットを記憶しておく*/
	DD_EVT_Put(evSend_ID, evuDD_COM_ParPacketSender_ID, 0, 0);

	return;

	}
	m_pcItsComPacket = DD_COM_NULL ;		/*リストから取得したパケットを記憶しておく*/
	DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParDataSender_ID, 0, 0);
}

//==============================================================================
// Function Name : stPreExec
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stPreExec(void) 
{
	state = stPreExec_ID;

	// Next Status
	DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParDataSender_ID, 0, 0);
	
	//次のステートに遷移後、すぐにデータ送信開始に遷移

	if(eReleased != (gComSendQueue.comPacket[gComSendQueue.rpoint].reservedFlag)){
		DD_EVT_Put(evStart_ID, evuDD_COM_ParDataSender_ID, 0, 0);
	}
}

//==============================================================================
// Function Name : stRelease
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stRelease(void) 
{
	state = stRelease_ID;

	// 1Packet Deleteする
	DD_COM_DestroySendPacket(&gComSendQueue,&gComSendQueue.comPacket[(gComSendQueue.rpoint)]);
	gComSendQueue.rpoint++;
	if(gComSendQueue.rpoint >= DD_COM_NUMBER_S_SEND_COM_PACKET){
		gComSendQueue.rpoint = 0;
	}
	DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParDataSender_ID, 0, 0);
}

//==============================================================================
// Function Name : stFail
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stFail(void) 
{
	DD_COM_SEND_PACKET_TYPE* compacket;
	state = stFail_ID;
	//Fail通知
	DD_COM_User_ComFail();
	//送信中のパケットを削除する
	if( m_pcItsComPacket != DD_COM_NULL){
		 DD_COM_DestroySendPacket(&gComSendQueue,m_pcItsComPacket);
	}
	gComSendQueue.rpoint++;
	if(gComSendQueue.rpoint >= DD_COM_NUMBER_S_SEND_COM_PACKET){
		gComSendQueue.rpoint = 0;
	}
	//リストにあるデータを削除する
}

//==============================================================================
// Function Name : stReStart
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stReStart(void) 
{
	state = stReStart_ID;

	// Start ComPacketSender
	DD_EVT_Put(evReset_ID, evuDD_COM_ParPacketSender_ID, 0, 0);
	// ReStart
	DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParDataSender_ID, 0, 0);

}

//==============================================================================
// Function Name : stReset
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stReset(void) 
{
	state = stReset_ID;

	//リセット受信時の動作があれば、ここに記述

	DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParDataSender_ID, 0, 0);

}

//==============================================================================
// Function Name : DD_COM_ParDataSenderNotify
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
void DD_COM_ParDataSenderNotify(DD_EventID eventId,void *arg)
{
	switch (eventId) {
	case evGoIdle_ID:
		switch (state) {
		case stPreExec_ID:
		case stSending_ID:
			stIdle();
			break;
		case stRelease_ID:
			stSending();
			break;
		case stReStart_ID:
		case stReset_ID:
			stInit();
			break;
		default:
			break;
		}
		break;
	case evComplete_ID:
		switch (state) {
		case stSending_ID:
			stRelease();
			break;
		default:
			break;
		}
		break;
	case evFail_ID:
		switch (state) {
		case stIdle_ID:
		case stSending_ID:
			stFail();
			break;
		default:
			break;
		}
		break;
	case evSync_ID:
		switch (state) {
		case stIdle_ID:
		case stSending_ID:
			stReset();
			break;
		default:
			break;
		}
		break;
	case evReset_ID:
		switch (state) {
		case stIdle_ID:
		case stSending_ID:
		case stFail_ID:
			stReStart();
			break;
		default:
			break;
		}
		break;
	case evSend_ID:
		switch (state) {
		case stIdle_ID:
			stSending();
			break;
		default:
			break;
		}
		break;
	case evStart_ID:
		switch (state) {
		case stInit_ID:
		case stFail_ID:
			stPreExec();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

#endif
