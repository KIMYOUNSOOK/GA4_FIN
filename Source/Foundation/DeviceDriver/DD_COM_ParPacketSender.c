// File Name: DD_COM_ParPacketSender.cpp
// Description:
// Engineer:  Yuichi Hoshino
// Last Edit: 16.02.09
// Revision:  002
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:				| Note:
// ---------+-----------------------+------------------------------------------------------------------------------------
// 13.10.15 | Mitsuyuki Nishizawa	| Created this file.(CH-Finisher SubCPU Rev005 Base)
// Revision 000
// 15.02.12 | Yuichi Hoshino		| メッセージログ出力処理追加
// 15.03.16 | Yuichi Hoshino		| ロングメッセージのみで送信を実施するよう変更
// 15.03.27 | Yuichi Hoshino		| PARポーリング処理削除
// 15.03.27 | Yuichi Hoshino		| IOT⇔フィニッシャー間通信規定反映
// Revidion 001
// 16.02.09	| Yuichi Hoshino		| Download対応のためのシーケンスビット取得関数追加
// Revision 002

#ifndef DD_COM_PARPACKET_SENDER_C
#define DD_COM_PARPACKET_SENDER_C

#include "Common.h"
#include "LIB_Service.h"
#include "DD_SCI.h"
#include "DD_LOG.h"
#include "DD_COM_Packet.h"
#include "DD_COM_CommonDef.h"
#include "DD_COM_ParPacketSender.h"
#include "DD_COM_pardatasender.h"
//#include "DD_COM_User.h"

typedef enum {
	// OMNonState=0,
	stInit_ID = 1,
	stIdle_ID,			//2
	stMakeSendPacket_ID,	//3
	stWaitAck_ID,		//4
	stACKCheck_ID,		//5
	stSendingComplete_ID,	//6
	stIdleAckSending_ID,	//7
	stIdleAckSendingCheck_ID,	//8
	stSending_ID,		//9
	stWaitPacketAckSending_ID,	//10
	stSendingWaitAckSend_ID,	//11
	stWaitACKSending_ID,	//12
	stSendingWaitAckSending_ID,//13
	stWaitACKCheck_ID,		//14
	stIdleAckSendingWait_ID,	//15
	stSynSend_ID,		//16
	stWaitSynAck_ID,		//17
	stPreExec_ID,		//18
	stWaitSynSnd_ID,		//19
	stSynSendWait_ID,		//20
	stSynSendWaitACKsend_ID,
	stWaitSendSynAck_ID,
	stReSendCheck_ID,
	stFail_ID,
	stReSendCheckAckSending_ID,
	stReceiveSyn_ID,
	stReceiveSynWait_ID,
	stWaitSynAckCheck_ID,
	stWaitSynAckTcheck_ID,
	stReset_ID,
	stNoConnect_ID
}DD_COM_ParPacketSenderID;
	
static DD_COM_ParPacketSenderID gState;

extern UC  comm_dummy;

UC SynCode[] = { 0x16, 0x7F, 0x16 };
UC Ack0Code[] = { 0x06 };
UC Ack1Code[] = { 0x86 };

/**	 * Synの再送回数 (0で無限) 転送用のパケット作成から終了までのシーケンスNo. 転送用のパケット作成時に変更する。	 */
static UC m_nSequenceNo;
//static US m_nReSendTime;			/**	 * 再送開始時間	 */
//static US m_nSendStartTime;		/**	 * SYNC送信開始までの時間	 */
static UC m_achReceiveACK[1];		/**	 * 受信したACKの情報	 */
static UC m_achSendACK[1];			/**	 * 送信するACKメッセージ	 */
//static US m_nReSendSynTime;			/**	 * Synの再送時間	 */
//static UC m_nMaxReSynCount;
//static UC m_achMessage[270];
static UC m_achMessage[DD_COM_MAX_SEND_PACKET_SIZE + 3];
static US m_nMessageLength;
static UC m_nReSendCount;
static UC IsNoramPacketSupport ;	//0:NormalPacket使う、1:NormalPacket使わない
//static UC   m_nMaxReSendCount;

void DD_COM_User_ComSync(void);		// DD_COM_Service.cに関数実体を定義
void DD_COM_User_ComFail(void);		// DD_COM_Service.cに関数実体を定義

//==============================================================================
// Function Name :DD_COM_InitParPacketSender 
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================		
void DD_COM_InitParPacketSender(void)
{

	//m_nMaxReSendCount=  DD_COM_MAX_SYN_RESEND ;
	//m_nReSendTime = DD_COM_RESEND_TIME;
	//m_nSendStartTime =  DD_COM_SYN_SEND_START_TIME ;
	//m_nMaxReSynCount = DD_COM_MAX_SYN_RESEND;
	//m_nReSendSynTime =  DD_COM_RESEND_TIME ;

	gState = stInit_ID;
	m_nReSendCount = 0; 
	IsNoramPacketSupport = 1;		//1:NormalPacket使わない
}

//==============================================================================
// Function Name : DD_COM_GetParPacketSendMessage
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static UC DD_COM_GetParPacketSendMessage(US a_i1)
{
    return	(m_achMessage[a_i1]);
}

//==============================================================================
// Function Name : DD_COM_SetParPacketSendMessage
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void DD_COM_SetParPacketSendMessage(US a_i1, UC a_pchMessage)
{
    m_achMessage[a_i1] = a_pchMessage;
}

//==============================================================================
// Function Name : ParPacketSender_MakeSendPacket
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void ParPacketSender_MakeSendPacket(void)
{
    UC CheckSum ;
    US MessageLength;
    US MessageOffset;
    US Index;
    
   // Check DD_COM_RECV_PACKET_TYPE
    if( eReserved != gComSendQueue.comPacket[(gComSendQueue.rpoint)].reservedFlag) {
        DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParPacketSender_ID, 0, 0);
        return;
    }
    // Make Packet
    //   Get Message Length, Set Message Offset, Set Header, Check Sum
    CheckSum = 0;
    MessageLength = gComSendQueue.comPacket[(gComSendQueue.rpoint)].m_nLength;

    //   Init Header
    if( gComSendQueue.comPacket[(gComSendQueue.rpoint)].m_nPacketNo
      != gComSendQueue.comPacket[(gComSendQueue.rpoint)].m_nPacketCount ) { // Multi Packet
        DD_COM_SetParPacketSendMessage( 0, 0xE0 );
    }
    else { // Single Packet or End Packet
        DD_COM_SetParPacketSendMessage( 0, 0xF0 );
    }

	//NormalPacketを使うかどうかで処理を分ける。
	//NormalPacketを使わない場合は、メッセージ長に関係なくすべてLongPacketを使用する。
    if( IsNoramPacketSupport == 0 ){
		if( MessageLength > 15 || (0xE0 == DD_COM_GetParPacketSendMessage( 0 ))) { // Long Message
		    DD_COM_SetParPacketSendMessage( 1, (UC)MessageLength );
	    	CheckSum += DD_COM_GetParPacketSendMessage( 0 );
		    CheckSum += DD_COM_GetParPacketSendMessage( 1 );
		    MessageOffset = 2;
		}
		else { // Normal Message
		    DD_COM_SetParPacketSendMessage( 0, ( DD_COM_GetParPacketSendMessage( 0 ) | (UC)MessageLength ) );
	    	CheckSum += DD_COM_GetParPacketSendMessage( 0 );
		    MessageOffset = 1;
		}
    }
    else {
        DD_COM_SetParPacketSendMessage( 1, (UC)MessageLength );
        CheckSum += DD_COM_GetParPacketSendMessage( 0 );
        CheckSum += DD_COM_GetParPacketSendMessage( 1 );
        MessageOffset = 2;
    }
    //   Set Message
    for( Index = 0 ; Index < MessageLength ; Index++ ) {
        DD_COM_SetParPacketSendMessage( MessageOffset, gComSendQueue.comPacket[(gComSendQueue.rpoint)].m_chPacket[Index] );
        CheckSum += DD_COM_GetParPacketSendMessage( MessageOffset );
        MessageOffset++;
    }
    //   Set Check Sum and Sequence No.
    if( m_nSequenceNo == 0x00 ) {
        m_nSequenceNo =( 0x80 );
    }
    else {
        m_nSequenceNo =( 0x00 );
    }
    CheckSum &= 0x7F;
    DD_COM_SetParPacketSendMessage( MessageOffset, ( m_nSequenceNo | CheckSum ) );
    MessageOffset++;
    m_nMessageLength = MessageOffset;
    //   Initialize Retry counter
    m_nReSendCount= 0 ;

    // Start Sending status
    DD_EVT_Put(evGoNext_ID, evuDD_COM_ParPacketSender_ID, 0, 0);

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
}

//==============================================================================
// Function Name : stMakeSendPacket
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stMakeSendPacket(void)
{
	gState = stMakeSendPacket_ID;

	ParPacketSender_MakeSendPacket();
}


//==============================================================================
// Function Name : stWaitAck
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stWaitAck(void)
{
	 gState = stWaitAck_ID;
}

//==============================================================================
// Function Name : ParPacketSender_ACKCheck
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void ParPacketSender_ACKCheck(UC a_chAckCode)
{
    // Check ACK Code
    UC ExpectCode;
    
    if( m_nSequenceNo == 0x00 ) {
        ExpectCode = Ack0Code[ 0 ];
    }
    else {
        ExpectCode = Ack1Code[ 0 ];
    }
    
    if( a_chAckCode == ExpectCode ) {
 		// Stop Timer
		 DD_EVT_CancelTimer(tmDD_COM_ParPacketSender_ID);

			 // Notice Packet Send End
		 DD_EVT_Put(evComplete_ID, evuDD_COM_ParDataSender_ID, 0, 0);
		 DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParPacketSender_ID, 0, 0);
    }
}

//==============================================================================
// Function Name : stACKCheck
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stACKCheck(UC a_chAckCode)
{
	gState = stACKCheck_ID ;
	ParPacketSender_ACKCheck(a_chAckCode);
}

//==============================================================================
// Function Name : stWaitACKCheck
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stWaitACKCheck(UC a_chAckCode)
{
	gState = stWaitACKCheck_ID;
	ParPacketSender_ACKCheck(a_chAckCode);
}

//==============================================================================
// Function Name : stSendingComplete
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stSendingComplete(void)
{
	gState = stSendingComplete_ID;

	DD_EVT_SetTimer( DD_COM_RESEND_TIME, evuDD_COM_ParPacketSender_ID, tmDD_COM_ParPacketSender_ID, 0);
	DD_EVT_Put(evGoNext_ID, evuDD_COM_ParPacketSender_ID, 0, 0);
}

//==============================================================================
// Function Name : stIdleAckSending
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stIdleAckSending(UC a_chAckCode)
{
	gState = stIdleAckSending_ID;

    // Set Ack
    m_achSendACK[0]= a_chAckCode ;
    DD_SCI_OpPacketSend( &m_achSendACK[0], 1 );
}

//==============================================================================
// Function Name : stIdleAckSendingCheck
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stIdleAckSendingCheck(void)
{
	gState = stIdleAckSendingCheck_ID;

	ParPacketSender_MakeSendPacket();
}

//==============================================================================
// Function Name : stSending
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stSending(void)
{
	gState = stSending_ID;

	DD_SCI_OpPacketSend( &m_achMessage[0], m_nMessageLength );

	DD_LOG_Put(LOGID_IOT, 'S', &m_achMessage[0], m_nMessageLength, LOG_CATEGORY_DEF);
}

//==============================================================================
// Function Name : stWaitPacketAckSending
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stWaitPacketAckSending(void)
{
	gState = stWaitPacketAckSending_ID;
}

//==============================================================================
// Function Name : stSendingWaitAckSend
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stSendingWaitAckSend(UC a_chAckCode)
{
	gState = stSendingWaitAckSend_ID;

  	m_achSendACK[0] = a_chAckCode ;
}

//==============================================================================
// Function Name : stWaitACKSending
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stWaitACKSending(UC a_chAckCode)
{
	gState = stWaitACKSending_ID;

    // Set Ack
   	m_achSendACK[0]= a_chAckCode ;
    DD_SCI_OpPacketSend(&m_achSendACK[0], 1 );
}

//==============================================================================
// Function Name : stSendingWaitAckSending
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stSendingWaitAckSending(void)
{
	gState = stSendingWaitAckSending_ID;

    DD_EVT_SetTimer( DD_COM_RESEND_TIME, evuDD_COM_ParPacketSender_ID, tmDD_COM_ParPacketSender_ID, 0);

 	//   // Set Ack
    DD_SCI_OpPacketSend(&m_achSendACK[0], 1 );
}


//==============================================================================
// Function Name : stIdleAckSendingWait
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stIdleAckSendingWait(void)
{
	gState = stIdleAckSendingWait_ID;
}

//==============================================================================
// Function Name : stSynSend
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
//
static void stSynSend(void)
{
	gState = stSynSend_ID;

	DD_SCI_OpPacketSend( &SynCode[0], sizeof(SynCode) );
}


//==============================================================================
// Function Name : stWaitSynAck
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
void stWaitSynAck(void)
{
	gState = stWaitSynAck_ID;

    DD_EVT_SetTimer((US)DD_COM_SYN_RESEND_TIME, evuDD_COM_ParPacketSender_ID, tmDD_COM_ParPacketSender_ID, 0);
}

//==============================================================================
// Function Name : stPreExec
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stPreExec(void)
{
	gState = stPreExec_ID;

	DD_EVT_Put(evStart_ID, evuDD_COM_ParDataSender_ID, 0, 0);

	m_nSequenceNo =0xff;
	m_nReSendCount= 0 ;

	DD_EVT_CancelTimer(tmDD_COM_ParPacketSender_ID);
	DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParPacketSender_ID, 0, 0);

	DD_COM_User_ComSync();
}

//==============================================================================
// Function Name : stWaitSynSnd
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stWaitSynSnd(void)
{
	gState = stWaitSynSnd_ID;
}

//==============================================================================
// Function Name : stSynSendWait
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stSynSendWait(void)
{
	gState = stSynSendWait_ID;

	 /*   SubCPUからはSync送信しないので、ここでは何もしない
	if(0 != m_nSendStartTime){
		DD_EVT_SetTimer( m_nSendStartTime,evuDD_COM_ParPacketSender_ID,tmDD_COM_ParPacketSender_ID );

	}else{
		DD_EVT_Put(evGoNext_ID, evuDD_COM_ParPacketSender_ID, 0);

	}
	*/
}

//==============================================================================
// Function Name : stSynSendWaitACKsend
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stSynSendWaitACKsend(void)
{
	gState = stSynSendWaitACKsend_ID;

	DD_SCI_OpPacketSend( &Ack0Code[0], sizeof(Ack0Code) );
}

//==============================================================================
// Function Name : stReSendCheck
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stReSendCheck(void)
{
	gState = stReSendCheck_ID;

    m_nReSendCount=( m_nReSendCount + 1 );
    if( m_nReSendCount > DD_COM_RESEND_CNT ) {
       	DD_EVT_Put(evGoFail_ID, evuDD_COM_ParPacketSender_ID, 0, 0);
		return;
    }
	DD_EVT_Put(evGoNext_ID, evuDD_COM_ParPacketSender_ID, 0, 0);
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

	DD_COM_User_ComFail();		//通信Fail
	DD_EVT_Put(evFail_ID, evuDD_COM_ParDataReceiver_ID, 0, 0); //DataSenderとDataReceiverに通知する順を逆にしないこと！
  	DD_EVT_Put(evFail_ID, evuDD_COM_ParDataSender_ID, 0, 0);
}

//==============================================================================
// Function Name : stReSendCheckAckSending
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stReSendCheckAckSending(void)
{
	gState = stReSendCheckAckSending_ID;

	m_nReSendCount =(m_nReSendCount + 1 );
	// Repeat Check
	if( m_nReSendCount > DD_COM_MAX_SYN_RESEND ) {
	   	DD_EVT_Put(evGoFail_ID, evuDD_COM_ParPacketSender_ID, 0, 0);
	}
	else {	
		DD_EVT_Put(evGoNext_ID, evuDD_COM_ParPacketSender_ID, 0, 0);
	}
}

//==============================================================================
// Function Name : stReceiveSyn
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stReceiveSyn(void)
{
	gState = stReceiveSyn_ID;

	DD_EVT_Put(evSync_ID, evuDD_COM_ParDataSender_ID, 0, 0);

	DD_EVT_Put(evGoNext_ID, evuDD_COM_ParPacketSender_ID, 0, 0);
}

//==============================================================================
// Function Name : stReceiveSynWait
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stReceiveSynWait(void)
{
	gState = stReceiveSynWait_ID;

	DD_EVT_Put(evSync_ID, evuDD_COM_ParDataSender_ID, 0, 0);
}

//==============================================================================
// Function Name : stWaitSynAckCheck
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stWaitSynAckCheck(UC a_chAckCode)
{
	 gState = stWaitSynAckCheck_ID;

	if( a_chAckCode == Ack0Code[0] ) {
		DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParPacketSender_ID, 0, 0);
	}
}

//==============================================================================
// Function Name : stWaitSendSynAck
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stWaitSendSynAck(void)
{
	gState = stWaitSendSynAck_ID;

	DD_SCI_OpPacketSend( &Ack0Code[0], sizeof(Ack0Code) );
}

//==============================================================================
// Function Name : stWaitSynAckTcheck
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stWaitSynAckTcheck(void)
{
	gState = stWaitSynAckTcheck_ID;

	m_nReSendCount=( m_nReSendCount + 1 );
	// Repeat Check
	if( m_nReSendCount > DD_COM_MAX_SYN_RESEND ) {
		DD_EVT_Put(evGoFail_ID, evuDD_COM_ParPacketSender_ID, 0, 0);
	}
	else {
		DD_EVT_Put(evGoNext_ID, evuDD_COM_ParPacketSender_ID, 0, 0);
	}
}

//==============================================================================
// Function Name : stReset
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void	stReset(void)
{
	gState = stReset_ID;

	m_nReSendCount= 0 ;
    DD_EVT_Put(evGoNext_ID, evuDD_COM_ParPacketSender_ID, 0, 0);
}

//==============================================================================
// Function Name : stNoConnect
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void	stNoConnect(void)
{
	gState = stNoConnect_ID;
}

//==============================================================================
// Function Name : DD_COM_GetSenderSequenceBit
// Description   : 送信シーケンスビット値取得処理
// Parameter     : なし
// Return        : シーケンスビット値 [0/1]
//==============================================================================
UC DD_COM_GetSenderSequenceBit()
{
	if ( m_nSequenceNo == 0x80 ) return 1;
	else						 return 0;
}

//==============================================================================
// Function Name : DD_COM_OpComplete_INTERRUPT
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
void DD_COM_OpComplete_INTERRUPT(void)
{
    DD_EVT_Put(evComplete_ID, evuDD_COM_ParPacketSender_ID, 0, 0);

}

//==============================================================================
// Function Name : DD_COM_ParPacketSenderNotify
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
void DD_COM_ParPacketSenderNotify(DD_EventID eventId, UC* pMsg)
{
	switch (eventId) {
	case evGoIdle_ID:
		switch (gState) {
		case stInit_ID:
			stSynSendWait();
			break;
		case stMakeSendPacket_ID:
		case stACKCheck_ID:
		case stPreExec_ID:
			stIdle();
			break;
		case stIdleAckSendingCheck_ID:
			stIdleAckSending(*pMsg);
			break;
		case stWaitSynAckCheck_ID:
			stPreExec();
			break;
		case stWaitACKCheck_ID:
			stIdleAckSendingWait();
			break;
		default:
			break;
		}
		break;
	case evTimeout_ID:
		switch (gState) {
		case stWaitAck_ID:
		case stACKCheck_ID:
			stReSendCheck();
			break;
		case stWaitACKSending_ID:
		case stSendingWaitAckSending_ID:
		case stWaitACKCheck_ID:
			stReSendCheckAckSending();
			break;
		case stWaitSynAck_ID:
		case stWaitSynAckCheck_ID:
			stWaitSynAckTcheck();
			break;
		case stSynSendWait_ID:
			stSynSend();
			break;
		default:
			break;
		}
		break;
	case evSend_ID:
		switch (gState) {
		case stIdle_ID:
			stMakeSendPacket();
			break;
		case stIdleAckSending_ID:
		case stIdleAckSendingWait_ID:
			stIdleAckSendingCheck();
			break;
		default:
			break;
		}
		break;
	case evReceive_ID:
		switch (gState) {
		case stIdle_ID:
			stIdleAckSending(*pMsg);
			break;
		case stWaitAck_ID:
		case stACKCheck_ID:
			stWaitACKSending(*pMsg);
			break;
		case stSending_ID:
			stSendingWaitAckSend(*pMsg);
			break;
		default:
			break;
		}
		break;
	case evGoNext_ID:
		switch (gState) {
		case stMakeSendPacket_ID:
		case stReSendCheck_ID:
			stSending();
			break;
		case stSendingComplete_ID:
			stWaitAck();
			break;
		case stIdleAckSendingCheck_ID:
		case stReSendCheckAckSending_ID:
			stWaitPacketAckSending();
			break;
		case stReceiveSyn_ID:
			stSynSendWaitACKsend();
			break;
		case stWaitSynAckTcheck_ID:
		case stReset_ID:
			stSynSend();
			break;
		case stSynSendWait_ID:
			stSynSend();
			break;
		default:
			break;
		}
		break;
	case evAcknowledge_ID:
		switch (gState) {
		case stWaitAck_ID:
		case stACKCheck_ID:
			stACKCheck((UC)pMsg);
			break;
		case stWaitACKSending_ID:
		case stSendingWaitAckSending_ID:
		case stWaitACKCheck_ID:
			stWaitACKCheck((UC)pMsg);
			break;
		case stWaitSynAck_ID:
		case stWaitSynAckCheck_ID:
		case stWaitSendSynAck_ID:
			stWaitSynAckCheck((UC)pMsg);
			break;
		default:
			break;
		}
		break;
	case evGoFail_ID:
		switch (gState) {
		case stReSendCheck_ID:
		case stReSendCheckAckSending_ID:
		case stWaitSynAckTcheck_ID:
			stFail();
			break;
		default:
			break;
		}
		break;
	case evComplete_ID:
		switch (gState) {
		case stIdleAckSending_ID:
		case stIdleAckSendingWait_ID:
			stIdle();
			break;
		case stSending_ID:
			stSendingComplete();
			break;
		case stWaitPacketAckSending_ID:
			stSending();
			break;
		case stSendingWaitAckSend_ID:
			stSendingWaitAckSending();
			break;
		case stSendingWaitAckSending_ID:
		case stWaitACKSending_ID:
		case stWaitACKCheck_ID:
			stWaitAck();
			break;
		case stInit_ID:
			stSynSendWait();
			break;
		case stSynSend_ID:
		//case stWaitSynSnd:
			stWaitSynAck();
			break;
		case stWaitSynSnd_ID:
			stWaitSendSynAck();
			break;
		case stSynSendWaitACKsend_ID:
		case stWaitSendSynAck_ID:
			stPreExec();
			break;
		case stReceiveSynWait_ID:
			stSynSendWaitACKsend();
			break;
		default:
			break;
		}
		break;
	case evSync_ID:
		switch (gState) {
		case stInit_ID:
//		case stFail_ID:	通信Fail検知後、Sync応答しないようにする(通信復活しない)[12.10.25]
		case stIdle_ID:
		case stMakeSendPacket_ID:
		case stWaitAck_ID:
		case stACKCheck_ID:
		case stSendingComplete_ID:
			stReceiveSyn();
			break;
		case stIdleAckSending_ID:
		case stIdleAckSendingCheck_ID:
		case stSending_ID:
		case stWaitPacketAckSending_ID:
		case stSendingWaitAckSend_ID:
		case stWaitACKSending_ID:
		case stSendingWaitAckSending_ID:
		case stWaitACKCheck_ID:
		case stIdleAckSendingWait_ID:
			stReceiveSynWait();
			break;
		case stSynSend_ID:
			stWaitSynSnd();
			break;
		case stSynSendWait_ID:
			stSynSendWaitACKsend();
			break;
		case stWaitSynAck_ID:
			 stWaitSendSynAck();
			 break;
		default:
			break;
		}
		break;
	case evReset_ID:
		switch (gState) {
		case stInit_ID:
		case stFail_ID:
		case stIdle_ID:
			stReset();
			break;
		default:
			break;
		}
		break;
	case evStop_ID:
		switch(gState) {
		case stInit_ID:
			stNoConnect();
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
