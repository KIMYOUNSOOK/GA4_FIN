// File Name: DD_COM_ParPacketReceiver.cpp
// Description:
// Engineer:  Yuichi Hoshino
// Last Edit: 16.02.09
// Revision:  002
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 13.10.15 | Mitsuyuki Nishizawa | Created this file.(CH-Finisher SubCPU Rev002 Base)
// Revision 000
// 15.02.12 | Yuichi Hoshino      | メッセージログ出力処理追加
// Revision 001
// 16.02.09	| Yuichi Hoshino      | Download対応のためのシーケンスビット取得関数追加
// Revision 002


#include "Common.h"
#include "LIB_Lock.h"
#include "DD_Event.h"
#include "DD_LOG.h"

#include "DD_COM_Packet.h"
#include "DD_COM_Commondef.h"

extern UC SynCode[];
extern UC Ack0Code[];
extern UC Ack1Code[];

typedef enum {
	stInit_ID = 1,
	stReset_ID,
	stIdle_ID,
	stCheck1stMessage_ID,
	stReceiveAck_ID,
	stMessage1st_ID,
	stMessage2nd_ID,
	stReceiveMessage_ID,
	stMessageCheck_ID,
	stCancelMessage_ID,
	stSyn1st_ID,
	stSyn2nd_ID,
	stSyn3rd_ID,
	stReceiveSyn_ID
}DD_COM_ParPacketReceiverStateID;

/**
 * メッセージヘッダーの格納場所
 */
UC m_chSum;
/**	 * 受信するメッセージ長	 */
UC m_nMesLength;
/**	 * ComDataReciveにデータを転送した時のシーケンスNo.	 */
UC m_nRecvSequenceNo;
/**	 * バイト間の最大待ち時間	 */
US m_nInterByteTime;
UC m_achMsgHeader[2];
static DD_COM_ParPacketReceiverStateID gState;
DD_COM_RECV_PACKET_TYPE *m_ComPacket;
extern UC comm_dummy;


//==============================================================================
// Function Name : DD_COM_InitParPacketReceiver
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
void DD_COM_InitParPacketReceiver( )
{
	m_nInterByteTime = DD_COM_INTER_BYTE_TIME;
	m_nRecvSequenceNo =0x80;
	m_ComPacket=0;
    // 状態の初期化
    gState = stIdle_ID;
}

//==============================================================================
// Function Name : DD_COM_SetParReceivePacket
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void DD_COM_SetParReceivePacket(DD_COM_RECV_PACKET_TYPE* a_pcPacket)
{
	m_ComPacket = a_pcPacket;
}

//==============================================================================
// Function Name : DD_COM_GetMsgHeader
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static UC DD_COM_GetMsgHeader(int a_i1) 
{
    return(m_achMsgHeader[a_i1]);
}

//==============================================================================
// Function Name : DD_COM_SetMsgHeader
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void DD_COM_SetMsgHeader(US a_i1, UC a_chMsgHeader)
{
    m_achMsgHeader[a_i1] = a_chMsgHeader;
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
	DD_EVT_CancelTimer(tmDD_COMParSciPacketReceiver_ID);
}

//==============================================================================
// Function Name : stReceiveAck
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stReceiveAck(UC a_chByteData)
{
	gState = stReceiveAck_ID;

	//PacketSenderにevAcknowledge_ID通知
	//evGoIdle_ID　生成
	DD_EVT_Put(evAcknowledge_ID, evuDD_COM_ParPacketSender_ID, (UC*)a_chByteData, 1);
	// Next Status
	DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParPacketReceiver_ID, 0, 0);

}

//==============================================================================
// Function Name : stCheck1stMessage
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stCheck1stMessage(UC a_chByteData)
{
	gState = stCheck1stMessage_ID;

	//ACK受信：
	//　　evGoReceiveAck_ID　生成
	//メッセージHeader受信：
	//　　evGoMessage1st_ID　生成
	//Syn受信：
	//　　evGoSyn1st_ID　生成
	 // Check Byte Data
	if ( ( a_chByteData == Ack0Code[ 0 ] ) || ( a_chByteData == Ack1Code[ 0 ] ) ) { // ACK
		DD_EVT_Put(evGoComplete_ID, evuDD_COM_ParPacketReceiver_ID,  (UC*)a_chByteData, 1);
		return;
	}
	if ( ( ( a_chByteData & 0xF0 ) == 0xF0 ) || ( a_chByteData == 0xE0 )) { // Msg
  		DD_EVT_Put(evGoNext_ID, evuDD_COM_ParPacketReceiver_ID,  (UC*)a_chByteData, 1);
		return;
	}
	if ( a_chByteData == SynCode[ 0 ] ) { // Syn
		DD_EVT_Put(evGoStart_ID, evuDD_COM_ParPacketReceiver_ID, 0, 0);
		return;
	}
	// other
  	DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParPacketReceiver_ID, 0, 0);
}

//==============================================================================
// Function Name : stMessage1st
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stMessage1st(UC a_chByteData)
{
	DD_COM_RECV_PACKET_TYPE * pPacket;
	gState = stMessage1st_ID;

	//パケットを確保する
	pPacket = DD_COM_GetRecvPacket(&gComRecvQueue);
	//空きパケット領域が無い：
	if ( pPacket == 0 ) {
		//Create(evGoIdle_ID);
		DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParPacketReceiver_ID, 0, 0);
		return;
	}
	DD_COM_SetParReceivePacket( pPacket );
	 
	//受信データをセット
	 // Keep Header
	DD_COM_SetMsgHeader( 0, a_chByteData );
	//パケット番号をセット
	m_ComPacket->m_nPacketCount= -1 ;
	if ( a_chByteData == 0xE0 ) { // Multi Packet
	        m_ComPacket->m_nPacketNo= 1 ;
	}
	else {
		m_ComPacket->m_nPacketNo= -1 ;
	}
	m_ComPacket->m_nLength= 0;
	// SetCheckSum
	m_chSum = a_chByteData;
	//Timerをセットする
	DD_EVT_SetTimer( m_nInterByteTime ,evuDD_COM_ParPacketReceiver_ID,tmDD_COMParSciPacketReceiver_ID, 0);
}

//==============================================================================
// Function Name : stMessage2nd
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stMessage2nd(UC a_chByteData)
{
	gState = stMessage2nd_ID;

	//Timer停止
	DD_EVT_CancelTimer(tmDD_COMParSciPacketReceiver_ID);
	if( ( DD_COM_GetMsgHeader( 0 ) & 0x0F ) == 0 )
	{ // Long Message
		m_nMesLength= a_chByteData ;
		DD_COM_SetMsgHeader( 1, a_chByteData );
	}
	else { // Normal Message
	//　　パケット長算出
		m_nMesLength= DD_COM_GetMsgHeader( 0 ) & 0x0F ;
		m_ComPacket->m_chPacket[m_ComPacket->m_nLength] = a_chByteData;
		m_ComPacket->m_nLength=( m_ComPacket->m_nLength + 1 );
		DD_COM_SetMsgHeader( 1, 0 );
	}
	//CheckSumセット
	m_chSum += a_chByteData;
	//1byte受信Timerセット
	DD_EVT_SetTimer(m_nInterByteTime, evuDD_COM_ParPacketReceiver_ID, tmDD_COMParSciPacketReceiver_ID, 0);
}

//==============================================================================
// Function Name : stReceiveMessage
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stReceiveMessage(UC a_chByteData)
{
	gState = stReceiveMessage_ID;

	//Timer停止
	DD_EVT_CancelTimer(tmDD_COMParSciPacketReceiver_ID);
	// Packet End Check
	//1パケット受信完了：
	if( m_ComPacket->m_nLength == m_nMesLength ) { // Sequence and CheckSum
		DD_EVT_Put(evGoComplete_ID, evuDD_COM_ParPacketReceiver_ID,  (void*)a_chByteData, 1);
		return;
	}
	// Message
	// Set Message
	m_ComPacket->m_chPacket[m_ComPacket->m_nLength] = a_chByteData;
	m_ComPacket->m_nLength = ( m_ComPacket->m_nLength + 1 );
	// Sum
	m_chSum += a_chByteData;
	//タイムアウト設定
	DD_EVT_SetTimer( m_nInterByteTime, evuDD_COM_ParPacketReceiver_ID, tmDD_COMParSciPacketReceiver_ID, 0 );
}

//==============================================================================
// Function Name : stMessageCheck
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stMessageCheck(UC a_chByteData)
{
	int logIndex, packetIndex;
	UC logPacket[1 + sizeof(m_ComPacket->m_chPacket) + 1];

	gState = stMessageCheck_ID;

	// Put Log
	logIndex = 0;

	logPacket[logIndex++] = DD_COM_GetMsgHeader( 0 ); // Msg Header

	if ( DD_COM_GetMsgHeader( 1 ) != 0 ) {
		logPacket[logIndex++] = DD_COM_GetMsgHeader( 1 ); // Data Length for Long & Multi Packet Message
	}

	for ( packetIndex = 0 ; packetIndex < m_ComPacket->m_nLength ; packetIndex++ ) {
		logPacket[logIndex++] = m_ComPacket->m_chPacket[packetIndex]; // Client Data
	}

	logPacket[logIndex++] = a_chByteData; // Sequence Bit & Checksum

	DD_LOG_Put(LOGID_IOT, 'R', logPacket, logIndex, LOG_CATEGORY_DEF);

	// Check Sum
	if ( ( m_chSum & 0x7F ) != ( a_chByteData & 0x7F ) ) { // Check Sum Error
		// Next Status
		DD_EVT_Put(evGoCancel_ID, evuDD_COM_ParPacketReceiver_ID, 0, 0);
		return;
	}

	// Check Ack
	if ( ( a_chByteData & 0x80 ) == m_nRecvSequenceNo ) { // Sequence No Error
		// Packet Delete
		if ( m_nRecvSequenceNo == 0 ) {
			DD_EVT_Put(evReceive_ID, evuDD_COM_ParPacketSender_ID,(void*)Ack0Code[ 0 ], 1 );
		}
		else {
			DD_EVT_Put(evReceive_ID, evuDD_COM_ParPacketSender_ID, (void*)Ack1Code[ 0 ], 1 );
		}
		// Next Status
		DD_EVT_Put(evGoCancel_ID, evuDD_COM_ParPacketReceiver_ID, 0, 0);
		return;
	}

	// Set Sequence No.
	m_nRecvSequenceNo = ( a_chByteData & 0x80 );

	// Request Send Ack
	if ( m_nRecvSequenceNo == 0 ) {
		DD_EVT_Put(evReceive_ID, evuDD_COM_ParPacketSender_ID, (void*)&Ack0Code[ 0 ], 1 );
	}
	else {
		DD_EVT_Put(evReceive_ID, evuDD_COM_ParPacketSender_ID, (void*)&Ack1Code[ 0 ], 1 );
	}

	// Set Packet list
	DD_COM_SetParReceivePacket( 0 );
	gComRecvQueue.wpoint++;
	if (DD_COM_NUMBER_S_COM_PACKET <= (gComRecvQueue.wpoint)) {
		gComRecvQueue.wpoint=0;
	}

	// Notice
	DD_EVT_Put(evReceive_ID, evuDD_COM_ParDataReceiver_ID, 0, 0);

	// Next Status
	DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParPacketReceiver_ID, 0, 0);
}

//==============================================================================
// Function Name : stSyn1st
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stSyn1st(void)
{
	gState = stSyn1st_ID;

	//受信タイマセット
	DD_EVT_SetTimer( m_nInterByteTime, evuDD_COM_ParPacketReceiver_ID, tmDD_COMParSciPacketReceiver_ID, 0);
}

//==============================================================================
// Function Name : stSyn2nd
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stSyn2nd(UC a_chByteData)
{
	gState = stSyn2nd_ID;

	//タイマ停止
	DD_EVT_CancelTimer(tmDD_COMParSciPacketReceiver_ID);
	//SYNCではなかったら⇒
	if( a_chByteData != SynCode[ 1 ] ) { // No Syn
		DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParPacketReceiver_ID, 0, 0);
		return;
	}
	//受信タイマStart
	 DD_EVT_SetTimer(m_nInterByteTime, evuDD_COM_ParPacketReceiver_ID, tmDD_COMParSciPacketReceiver_ID, 0);
}


//==============================================================================
// Function Name : stSyn3rd
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stSyn3rd(UC a_chByteData)
{
	gState = stSyn3rd_ID;

	// Stop Timer
	DD_EVT_CancelTimer(tmDD_COMParSciPacketReceiver_ID);
	// Check DataByte
	if( a_chByteData != SynCode[ 2 ] ) { // No Syn
		DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParPacketReceiver_ID, 0, 0);
		return;
	}
	// Notice Syn
  	DD_EVT_Put(evGoComplete_ID, evuDD_COM_ParPacketReceiver_ID, 0, 0);
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

	// Initialize Sequence No.
	m_nRecvSequenceNo =0x80;

	// Notice Syn
  	DD_EVT_Put(evSync_ID, evuDD_COM_ParPacketSender_ID, 0, 0);
	// Next Status
  	DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParPacketReceiver_ID, 0, 0);
}

//==============================================================================
// Function Name : stReset
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stReset(void)
{
	gState = stReset_ID;
}

//==============================================================================
// Function Name : stCancelMessage
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stCancelMessage(void)
{
	gState = stCancelMessage_ID;
	if(m_ComPacket != DD_COM_NULL){
		//パケットを削除する
		DD_COM_DestroyRecvPacket(&gComRecvQueue,m_ComPacket);
//		gComRecvQueue.wpoint++;
//		if(gComRecvQueue.wpoint >= DD_COM_NUMBER_S_COM_PACKET){
//			gComRecvQueue.wpoint = 0;
//		}
		DD_COM_SetParReceivePacket( DD_COM_NULL );
	}
    // Next Status
  	DD_EVT_Put(evGoIdle_ID, evuDD_COM_ParPacketReceiver_ID, 0, 0);
}

//==============================================================================
// Function Name : stInit
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stInit(void)
{
	gState =stInit_ID;
}

//==============================================================================
// Function Name : DD_COM_RecvErr
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
void DD_COM_RecvErr(void)
{	
	//受信エラー処理
	DD_EVT_Put(evErr_ID, evuDD_COM_ParPacketReceiver_ID, 0, 0);
}

//==============================================================================
// Function Name : DD_COM_GetReceiverSequenceBit
// Description   : 受信シーケンスビット値取得処理
// Parameter     : なし
// Return        : シーケンスビット値 [0/1]
//==============================================================================
UC DD_COM_GetReceiverSequenceBit()
{
	if ( m_nRecvSequenceNo == 0x80 ) return 1;
	else							 return 0;
}

//==============================================================================
// Function Name : DD_COM_ParPacketReceiverNotify
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
void DD_COM_ParPacketReceiverNotify(DD_EventID a_pcEvent, UC* pMsg)
{
	switch (a_pcEvent) {
	case evGoComplete_ID:
		switch (gState) {
		case stReceiveMessage_ID:
			stMessageCheck( (UC)pMsg );
			break;
		case stSyn3rd_ID:
			stReceiveSyn();
			break;
		case stCheck1stMessage_ID:
			stReceiveAck( (UC)pMsg );
			break;
		default:
			break;
		}
		break;
	case evGoIdle_ID:
		switch (gState) {
		case stInit_ID:
			stReset();
			break;
		case stReset_ID:
		case stCheck1stMessage_ID:
		case stReceiveAck_ID:
		case stReceiveMessage_ID:
		case stCancelMessage_ID:
		case stSyn2nd_ID:
		case stSyn3rd_ID:
		case stReceiveSyn_ID:
		case stMessageCheck_ID:
			stIdle();
			break;
		default:
			break;
		}
		break;
	case evGoNext_ID:
		switch (gState) {
		case stCheck1stMessage_ID:
			stMessage1st( (UC)pMsg );
			break;
		default:
			break;
		}
		break;
	case evGoStart_ID:
		switch (gState) {
		case stCheck1stMessage_ID:
			stSyn1st();
			break;
		default:
			break;
		}
		break;
	case evStart_ID:
		switch (gState) {
		case stIdle_ID:
			stCheck1stMessage( (UC)pMsg );
			break;
		case stMessage1st_ID:
			stMessage2nd( (UC)pMsg );
			break;
		case stMessage2nd_ID:
		case stReceiveMessage_ID:
			stReceiveMessage( (UC)pMsg );
			break;
		case stSyn1st_ID:
			stSyn2nd( (UC)pMsg );
			break;
		case stSyn2nd_ID:
			stSyn3rd( (UC)pMsg );
			break;
		default:
			break;
		}
		break;
	case evReset_ID:
		switch (gState) {
		case stInit_ID:
		case stIdle_ID:
		case stMessage1st_ID:
		case stMessage2nd_ID:
		case stReceiveMessage_ID:
		case stSyn1st_ID:
		case stSyn2nd_ID:
			stReset();
			break;
		default:
			break;
		}
		break;
	case evTimeout_ID:
		switch (gState) {
		case stMessage1st_ID:
		case stMessage2nd_ID:
		case stReceiveMessage_ID:
			stCancelMessage();
			break;
		case stSyn1st_ID:
		case stSyn2nd_ID:
			stIdle();
			break;
		default:
			break;
		}
		break;
	case evErr_ID:
		switch (gState) {
		case stMessage1st_ID:
		case stMessage2nd_ID:
		case stReceiveMessage_ID:
			stCancelMessage();
			break;
		case stSyn1st_ID:
		case stSyn2nd_ID:
			stIdle();
			break;
		}
		break;
	 case evGoCancel_ID:
	 	switch (gState) {
		case stMessageCheck_ID:
			stCancelMessage();
			break;
		}
	 	break;
	default:
		break;
	}
}
