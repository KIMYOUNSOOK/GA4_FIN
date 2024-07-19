// ☆☆☆ Module Software Platform Frame ☆☆☆
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

// ===========================  <<< 外部参照変数 >>>  ===========================
extern CEventTimer		msp_EventTimer ;													// [外部参照] イベントタイマー


static US EVENT_ID(const UC *ptr) { return  *((US*)ptr) ; }										// イベント通知パケットからイベントIDを取得するマクロ

//=====================================================================================
// Method Name   : MSP_ReceiveEvent()
// Description   : MSPイベントを受信して対応するコールバック関数を呼び出す
// Parameter     : なし	
// Return        : なし
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
		if ( EVENT_ID(pMsg) == EVT_TIME_OUT ) {												// タイムアウトイベント																
			timerID = *((SS*)(pMsg + 6)) ;
			CEventTimer_Timeout(&msp_EventTimer, timerID) ;
		}
		else {
			from   	= *pMsg ;
			index  	= MSP_GetEventIndex(*(EventID*)(pMsg + 4)) ;
			number 	= MSP_GetEventNumber(*(EventID*)(pMsg + 4)) ;

			pControl = msp_EvtIndexTable[index].mpControlTable ;							// イベント･コントロール･テーブルの先頭アドレス取得
			pHandler = msp_EvtIndexTable[index].mpHandlerTable ;							// イベント･ハンドラ･テーブルの先頭アドレス取得
			
			for ( count = 0 ; count < pControl[number].mCount ; count++ ) {							// [[[ LOOP ]]] 登録数分のハンドラーをコールバック（ループ）
				(pHandler[pControl[number].mStart + count]).mpHandler(((UC*)(pMsg + 6)), from) ;	// +++++ イベント･ハンドラのコールバックを実施する +++++
			}
		}
	}
}

//=====================================================================================
// Function Name : MSP_SendEvent()
// Description   : イベントを通知する.
// Parameter     : senderID		"メッセージ送信元モジュールID"
//               : eventID      "イベントID"
//               : pMessage		"送信メッセージ格納メモリの先頭アドレス"
//               : length		"送信メッセージのサイズ [単位:Byte]"
//               : log			"ログ出力可否"
// Return        : なし
// Caution       : 本関数でデータ･ロガーのError()をコールするとループする可能性大！！！
//=====================================================================================
void MSP_SendEvent(UC senderID, US eventID, const UC *pMessage, US length, UC log)
{
	static const char  eMsg1[] = {"FRAME ERROR: Event Tx Error"} ;
	static const char  eMsg2[] = {"FRAME ERROR: Event Tx Buffer Overflow"} ;

	static UL  buffer[(MSP_MAX_MSG_SIZE + 3) / 4 + 1] ;										// 送信バッファ [固定配置データ]
	UC	*pWrite ;
//	ER	result ;
	UC	key ;

	if ( length <= MSP_MAX_MSG_SIZE ) {														// パケット･サイズのチェック

	// ----- メッセージ送信処理 -----
		key = LIB_GetKey() ; LIB_Lock() ;													// 注意!!! 割り込み禁止 ここから!!!
		buffer[0] = 0x00000000 ;															// 最初の4バイトを"0"クリアする
		pWrite = (UC*)buffer ;																// 書き込み先アドレスをセットする
		*pWrite = senderID ;																// 先頭の1バイトに送信モジュールIDをセットする
		memcpy((pWrite + 4), &eventID, 2);													// 5, 6バイト目はEvent IDを詰める
		memcpy((pWrite + 6), pMessage, length) ;											// 7バイト目から送信データを詰める
		LIB_snd_mbf(eMbfFrame, pWrite, length + 6) ;										// メッセージ送信
		LIB_Unlock(key) ;																	// 注意!!! 割り込み禁止 ここまで!!!

//	// ----- エラー対応＆ログ出力 -----
//		if ( result != E_OK ) {																// 送信エラー発生！！！
//    		Printf(MID_FRAME, 0, (UC*)eMsg1, (US)sizeof(eMsg1)) ;
//		}
//		else if ( log == LOG_ON ) {															// デバッグ･ログ出力
//			Printf(MID_FRAME, 'S', (UC*)buffer, (US)(length + 4)) ;
//		}
	}
	else {																					// パケットサイズがバッファーを超える
   		Printf(MID_FRAME, 0, (UC*)eMsg2, (US)sizeof(eMsg2)) ;
	}
}



// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+-----------------------------------------------------------------------------------------------------------------------------
// 14.04.01 | Mitsuyuki Nishizawa | Module Software Platform Frameのプロトタイプとして本ファイルを作成.
//


// **************************************************************  [ MSP_Frame.cpp : EOF ]  **************************************************************
