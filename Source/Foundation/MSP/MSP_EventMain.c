// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_EventMain.c
/// @author  Mitsuyuki Nishizawa
/// @date    15.02.12
/// @version 0002
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define MSP_EventMain_c

#include "Common.h"
#include "LIB_Service.h"

#include "MSP_Common.h"
#include "MSP_Config.h"
#include "MSP_EventID.h"
#include "MSP_ProductSpec.h"
#include "MSP_EventMain.h"
#include "MSP_EventTimer.h"


// ===========================  <<< 外部参照変数 >>>  ===========================
extern CEventTimer		msp_EventTimer ;													// [外部参照] イベントタイマー


//=====================================================================================
// Method Name   : MSP_ReceiveEvent()
// Description   : MSPイベントを受信して対応するコールバック関数を呼び出す.
// Parameter     : なし	
// Return        : なし
//=====================================================================================
void MSP_ReceiveEvent()
{
	const EventControl *pControl ;
	const EventHandler *pHandler ;
	US	index, number, count ;
	UC	from ;
	UC	*pMsg ;
	SS	timerID ;
	
	if ( LIB_rcv_mbf(eMbfFrame, &pMsg) > 0 ) {
		if ( MSP_GetEventIndex(*(US*)(pMsg + 2)) == EVT_TIME_OUT ) {						// < タイムアウト･イベント >（イベントタイマーのタイムアウト処理実施）
			timerID = *((SS*)(pMsg + 4)) ;
			CEventTimer_Timeout(&msp_EventTimer, timerID) ;
		}
		else {																				// < タイムアウト･イベント以外 >
			from   = *pMsg ;
			index  = MSP_GetEventIndex(*(EventID*)(pMsg + 2)) ;								// イベント･インデックス取得
			number = MSP_GetEventNumber(*(EventID*)(pMsg + 2)) ;							// イベント･ナンバー取得

			pControl = msp_EvtIndexTable[index].mpControlTable ;							// イベント･コントロール･テーブルの先頭アドレス取得
			pHandler = msp_EvtIndexTable[index].mpHandlerTable ;							// イベント･ハンドラ･テーブルの先頭アドレス取得
			
			for ( count = 0 ; count < pControl[number].mCount ; count++ ) {						 // [[[ LOOP ]]] 登録数分のハンドラーをコールバック（ループ）
				(pHandler[pControl[number].mStart + count]).mpHandler(((UC*)(pMsg + 4)), from) ; // +++++ イベント･ハンドラのコールバックを実施する +++++
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
	SS	result ;
	UC	key ;

	if ( length <= MSP_MAX_MSG_SIZE ) {														// パケット･サイズのチェック

	// ----- メッセージ送信処理 -----
		key = LIB_GetKey() ; LIB_Lock() ;													// 注意!!! 割り込み禁止 ここから!!!
		buffer[0] = 0x00000000 ;															// 最初の4バイトを"0"クリアする
		pWrite = (UC*)buffer ;																// 書き込み先アドレスをセットする
		*pWrite = senderID ;																// 先頭の1バイトに送信モジュールIDをセットする
		memcpy((pWrite + 2), &eventID, 2);													// 3, 4バイト目はEvent IDを詰める
		if ( length != 0 ) memcpy((pWrite + 4), pMessage, length) ;							// 5バイト目から送信データを詰める（書き込みサイズが0でないとき）
		result = LIB_snd_mbf(eMbfFrame, pWrite, length + 4) ;								// メッセージ送信
		LIB_Unlock(key) ;																	// 注意!!! 割り込み禁止 ここまで!!!

	// ----- エラー対応＆ログ出力 -----
		if ( result == LIB_MBF_ERR ) {														// 送信エラー発生！！！
			Printf(LOGID_FRAME, 0, (UC*)eMsg1, (US)sizeof(eMsg1)) ;
		}
		else if ( log == LOG_ON ) {															// デバッグ･ログ出力
			Printf(LOGID_FRAME, 'S', (UC*)buffer, (US)(length + 4)) ;
		}
	}
	else {																					// パケットサイズがバッファーを超える
		Printf(LOGID_FRAME, 0, (UC*)eMsg2, (US)sizeof(eMsg2)) ;
	}
}



// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 14.04.01 | Mitsuyuki Nishizawa | Module Software Platform Frameのプロトタイプとして本ファイルを作成.
// 14.09.24 | Mitsuyuki Nishizawa | イベントデータ書き込み先アドレスを4バイト境界に配置.
// 15.02.12 | Yuichi Hoshino      | Printf()に渡すIDをLog IDに変更
// 15.03.30 | Yuichi Hoshino      | Release version 0002
//
//


// **************************************************************  [ MSP_EventMain.c : EOF ]  *****************************************************************
