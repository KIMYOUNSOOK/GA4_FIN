// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_SubsystemGateway.c
/// @author  Mitsuyuki Nishizawa
/// @date    15.02.12
/// @version 002
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define MSP_SubsystemGateway_c

#include "Common.h"
#include "LIB_Service.h"
#include "DD_COM_Service.h"

#include "MSP_Common.h"
#include "MSP_Config.h"
#include "MSP_EventID.h"
#include "MSP_ProductSpec.h"
#include "MSP_EventMain.h"
#include "MSP_SubsystemGateway.h"


// ========================  <<< インスタンス生成 >>>  ========================

// ----- サブシステム･ゲートウェイ -----
CSubsystemGateway msp_IotGateway ;																// IOTサブシステム･ゲートウェイ


//$$$$$$ [[[[[ Class: CSubsystemGateway ]]]]] $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// Class Name    : CSubsystemGateway（サブシステムの代理人）
// Description   : サブシステムへのコマンド（ステータス）送信およびコマンド･イベントの生成･通知を実施する.
//               :
// Note          : 通信対象となるサブシステム（Controller, Finisher, etc.）に対して個別のインスタンス生成が必要です.
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//=[[[ Constructor ]]]=================================================================
// Function Name : CSubsystemGateway::CSubsystemGateway()
// Description   : このクラスのコンストラクタ.
// Parameter     : *self		"対象ゲートウェイ･インスタンスのアドレス"
//               : *spec		"スペック･テーブルへのポインタ"
// Return        : -
//=====================================================================================
void CSubsystemGateway_CSubsystemGateway(CSubsystemGateway *self, const GatewaySpec *spec)
{
	self->mpSpec = spec ;																	// スペック情報を登録する
}


//=====================================================================================
// Method Name   : CSubsystemGateway::ReceiveCommand()
// Description   : コマンド（ステータス）の受信およびコマンド･イベントの生成をする.
// Parameter     : *self		"対象ゲートウェイ･インスタンスのアドレス"
//               : *packet		"サブシステムからの受信パケット格納アドレス（参照先）"
//               : length		"受信パケットサイズ"
// Return        : なし
//=====================================================================================
void CSubsystemGateway_ReceiveCommand(CSubsystemGateway *self, const UC *packet, US length)
{
	static const char  eMsg1[] = {"FRAME ERROR: Invalid Command ID"} ;
	static const char  eMsg2[] = {"FRAME ERROR: Command Rx Buffer Overflow"} ;

	const GatewaySpec *pSpec = self->mpSpec ;
	US	number ;
	US	eventID = MSP_GetEventID(pSpec->mEvtIndex, MSP_EVT_NUM_VOID) ;						// 受信データ･バッファのイベントIDを初期化する

//	DDにてデフォルトでLog出力する
//	Printf(LOGID_FRAME, 'R', packet, (US)length) ;

	if ( length <= MSP_MAX_CMD_SIZE ) {														// パケット･サイズのチェック

	// ----- コマンドIDから対応するイベントIDを検索 ⇒ イベント生成･送信 -----
		for ( number = 0 ; number < pSpec->mListSize ; number++ ) {							// [[[ LOOP ]]] コマンド･リストをチェックする（ループ）
			if ( *packet == pSpec->mpCommandList[number] ) {
				eventID = MSP_GetEventID(pSpec->mEvtIndex, number) ;						// コマンド･イベントを生成する
				self->mSendBuffer.mLength = length ;
				memcpy((self->mSendBuffer.mCommand), packet, length) ;
				MSP_SendEvent(MID_FRAME, eventID, (const UC*)&(self->mSendBuffer), (US)(sizeof(US)+length), LOG_OFF) ;	// コマンド･イベントを送信する
				break ;
			}
		}																					// [[[ LOOP ]]] （ここまでループ）

	// ----- 該当するコマンドIDがなかったときの処理 -----
		if ( MSP_GetEventNumber(eventID) == MSP_EVT_NUM_VOID ) {							// 該当するコマンドが検索できなかった
			Printf(LOGID_FRAME, 0, (UC*)eMsg1, (US)sizeof(eMsg1)) ;
		}
	}
	else {																					// パケットサイズがバッファーを超える
		Printf(LOGID_FRAME, 0, (UC*)eMsg2, (US)sizeof(eMsg2)) ;
	}
}


//=====================================================================================
// Method Name   : CSubsystemGateway::SendCommand()
// Description   : コマンド（ステータス）を送信する.
// Parameter     : *self		"対象ゲートウェイ･インスタンスのアドレス"
//               : senderID		"送信元モジュールID"
//               : *packet		"送信パケット格納先アドレス"
//               : length		"送信パケットサイズ"
//               : log			"送信パケットのログ情報保持の有無"
// Return        : コマンド送信の結果（0:送信成功 / 1:送信失敗）
//=====================================================================================
UC CSubsystemGateway_SendCommand(CSubsystemGateway *self, UC senderID, const UC *packet, US length, UC log)
{
	static const char  eMsg[] = {"FRAME ERROR: Command Tx Error"} ;

	SL	result ;

// ----- コマンド（ステータス）送信処理 -----
	result = DD_COM_SendMsg(packet, length) ;

// ----- エラー対応＆ログ出力 -----
	if ( result != E_OK ) {																	// 送信エラー発生！！！
		Printf(LOGID_FRAME, 0, (UC*)eMsg, (US)sizeof(eMsg)) ;
	}

//	DDにてデフォルトでLog出力する
//	if ( log == LOG_ON ) {																	// デバッグ･ログ出力
//		Printf(LOGID_FRAME, 'S', (UC*)packet, (US)length) ;
//	}

	return	(UC)result ;
}


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 13.04.01 | Mitsuyuki Nishizawa | Module Software Platform Frameのプロトタイプとして本ファイルを作成.
// 14.11.05 | Mitsuyuki Nishizawa | 送受信関数名称および引数を変更&初期化処理を追加.
// 14.12.04 | Yuichi Hoshino      | Release version 001
// 15.02.12 | Yuichi Hoshino      | Printf()に渡すIDをLog IDに変更
// 15.02.12 | Yuichi Hoshino      | DDで実施するため、メッセージログ出力処理をコメントアウト
// 15.03.30 | Yuichi Hoshino      | Release version 002
//
//


// **********************************************************  [ MSP_SubsystemGateway.c : EOF ]  **************************************************************
