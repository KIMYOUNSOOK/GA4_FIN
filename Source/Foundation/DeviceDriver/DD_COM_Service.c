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
// 15.02.12 | Yuichi Hoshino		| メッセージログ出力処理追加
// 15.03.27 | Yuichi Hoshino		| DD_COM_SendMsg()の戻り値見直し
// Revision 001
// 16.02.09	| Yuichi Hoshino		| Download対応のためのシーケンスビット取得関数追加
// 16.02.09	| Yuichi Hoshino		| 通信フェイル発生イベントの2重発生防止処理追加
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

void (*RECVFUNC)(UC* RcvDataAddress, US datalength) = NULL;      /* 受信関数へのポインタ     */

//==============================================================================
// Function Name : DD_COM_Init
// Description   : 通信を初期化する
// Parameter     : なし
// Return        : なし
//==============================================================================
void DD_COM_Init(void)
{
	gCom_State = DD_COM_STT_UNSYNC;

	DD_SCI_Init();
	DD_COM_InitParPacketSender();
	DD_COM_InitParPacketReceiver();
	DD_COM_InitParDataSender();
	//DD_COM_InitParDataReceiver(DD_COM_MAX_RECV_DATA_SIZE);	
	DD_COM_InitParDataReceiver();						// 引数なし
}

//==============================================================================
// Function Name : DD_COM_EntryReceiver
// Description   : 受信モジュールを登録する
// Parameter     : func　：受信モジュールアドレス
// Return        : なし
//==============================================================================
void DD_COM_EntryReceiver (void (*func)(UC * RcvDataAddress, US datalength ))
{
	RECVFUNC = func;
}

//==============================================================================
// Function Name : DD_COM_User_ComSync
// Description   : 通信確立通知処理
// Parameter     : なし
// Return        : なし
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
// Description   : 通信エラー通知処理
// Parameter     : なし
// Return        : なし
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
// Description   : 送信要求処理
// Parameter     : a_pchDataAddress　：送信データへのポインタ
//                 a_nDataLength　：送信データ長
// Return        : 0 ：正常終了
//                 -1：失敗
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
			pComPacket = DD_COM_GetSendPacket(&gComSendQueue);		/*　領域を確保　*/

			if ( pComPacket != 0 ) {								/* 領域確保に成功　*/
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
			else {													/* 領域確保に失敗 */
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

	// 送信指示に失敗
	DD_LOG_Put(LOGID_FINISHER, '0', cNgLog, sizeof(cNgLog), LOG_CATEGORY_DEF);
	DD_LOG_Put(LOGID_FINISHER, 'S', a_pchDataAddress, a_nDataLength, LOG_CATEGORY_DEF);

	return E_NG;
}

//==============================================================================
// Function Name : DD_COM_GetRxSequenceBit  
// Description   : RxSeq Bit取得処理
// Parameter     : ‐
// Return        : シーケンスビット値 [0/1] 
//==============================================================================
UC DD_COM_GetRxSequenceBit(void)
{
	return DD_COM_GetReceiverSequenceBit();
}

//==============================================================================
// Function Name : DD_COM_GetTxSequenceBit
// Description   : TxSeq Bit取得処理
// Parameter     : ‐
// Return        : シーケンスビット値 [0/1] 
//==============================================================================
UC DD_COM_GetTxSequenceBit(void)
{
	return DD_COM_GetSenderSequenceBit();
}
