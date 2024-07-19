// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_Service.c
/// @author  Mitsuyuki Nishizawa
/// @date    14.11.04
/// @version 001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define MSP_Service_c

#include "Common.h"
#include "LIB_Service.h"
#include "DD_Event.h"
#include "DD_DIO.h"
#include "DD_COM_Service.h"

#include "MSP_Common.h"
#include "MSP_Config.h"
#include "MSP_EventID.h"
#include "MSP_ProductSpec.h"
#include "MSP_EventMain.h"
#include "MSP_Location.h"
#include "MSP_Sensor.h"
#include "MSP_SubsystemGateway.h"
#include "MSP_EventTimer.h"


// ===========================  <<< 関数プロトタイプ宣言 >>>  ===========================
void MSP_TickEventTimer() ;
void MSP_ReceiveCommand(UC*, US) ;


// ===========================  <<< 外部参照変数 >>>  ===========================
extern CEventTimer	msp_EventTimer ;														// [外部参照] イベントタイマー
extern CSubsystemGateway  msp_IotGateway ;													// [外部参照] サブシステム･ゲートウェイ（IOT）


// *************************************************************  <<<<< Program Code Area >>>>>  **************************************************************

//###### [[[[[ Module Software Platform Frame Service ]]]]] ###################################################################################################

//=====================================================================================											// <<< SYSTEM EVENT >>>
// Function Name : MSP_StartUp()                                                      											// ┌───────────┐
// Description   : MSP Frameの起動を行う.                                            											// ├ "START UP"           ┤
// Parameter     : なし                                                                											// └───────────┘
// Return        : なし
//=====================================================================================
void MSP_StartUp()
{
	UC      testReg[2];
// ----- コンストラクタ実行 -----
	CEventTimer_CEventTimer(&msp_EventTimer) ;												// イベントタイマーのコンストラクタ実行
	CSubsystemGateway_CSubsystemGateway(&msp_IotGateway, &msp_IotGwSpec) ;					// サブシステム･ゲートウェイのコンストラクタ実行
	CTimingController_CTimingController(msp_LocationSpec) ;									// ロケーション･タイミング･コントローラーのコンストラクタ実行

// ----- タイマーの登録 ------
	DD_EVT_EntryCallback(MSP_TickEventTimer) ;

// ----- センサー変化通知関数の登録 ------
	DD_DIO_EntryCallback(CSensorEvent_NotifyEdge) ;

// ----- 受信関数の登録 ------
	DD_COM_EntryReceiver(MSP_ReceiveCommand) ;

// ----- アプリケーション起動用メッセージ送信 -----
	MSP_SendEvent(MID_FRAME, FRM_SYS_START_UP, 0, 0, OFF) ;									// "START_UP"メッセージ送信

	// test Reset register..
	testReg[0] = RESF;
	testReg[1] = 0xFF;
		
	DD_LOG_Put(LOGID_TRNS, 'S', testReg, sizeof(testReg), LOG_CATEGORY_DEBUG);

}


//=====================================================================================
// Function Name : MSP_SendMessage()
// Description   : メッセージ･イベントを生成する.
// Parameter     : senderID		"メッセージ送信元モジュールID"
//               : eventID      "イベント ID"
//               : pMessage		"送信メッセージ格納メモリの先頭アドレス"
//               : length		"送信メッセージのサイズ [単位:Byte]"
//               : log			"送信メッセージのログ情報保持の有無"
// Return        : なし
//=====================================================================================
void MSP_SendMessage(UC senderID, US eventID, const UC *pMessage, US length, UC log)
{
	MSP_SendEvent(senderID, eventID, pMessage, length, log) ;								// メッセージ･イベントを通知する
}


//=====================================================================================
// Function Name : MSP_SetEventTimerHigh()
// Description   : タイムアウト･イベント（高優先度）の生成をリクエストする.
// Parameter     : delayTime	"タイムアウト･イベント発生の遅延時間"
//               : pHandler		"イベント発生時のコールバック･ハンドラ"
//               : param		"ハンドラをコールバックする際のパラメータ"
// Return        : タイマーセットの結果（成功:タイマーID / 失敗:エラーコード）
// Note          : タイマー値=0でタイマーセットした際に戻り値は"MSP_TMR_TERM"となる.
//=====================================================================================
SS MSP_SetEventTimerHigh(US delayTime, void (*pHandler)(SS, UL), UL param)
{
	Handler  info ;

	info.mpHandler = pHandler ;
	info.mParam    = param ;

	return CEventTimer_Set(&msp_EventTimer, MSP_TMR_HIGH, delayTime, &info) ;				// イベントタイマー（高優先度）をセットする
}


//=====================================================================================
// Function Name : MSP_SetEventTimer()
// Description   : タイムアウト･イベント（低優先度）の生成をリクエストする.
// Parameter     : delayTime	"タイムアウト･イベント発生の遅延時間"
//               : pHandler		"イベント発生時のコールバック･ハンドラ"
//               : param		"ハンドラをコールバックする際のパラメータ"
// Return        : タイマーセットの結果（成功:タイマーID / 失敗:エラーコード）
//=====================================================================================
SS MSP_SetEventTimer(US delayTime, void (*pHandler)(SS, UL), UL param)
{
	Handler  info ;

	info.mpHandler = pHandler ;
	info.mParam    = param ;

	return CEventTimer_Set(&msp_EventTimer, MSP_TMR_LOW, delayTime, &info) ;				// イベント･タイマー（低優先度）をセットする
}


//=====================================================================================
// Function Name : MSP_CancelEventTimer()
// Description   : タイムアウト･イベントの生成をキャンセルする.
// Parameter     : timerID		"タイムアウト･イベント生成用タイマーID"
// Return        : タイマーセットの結果（キャンセル成功 / キャンセル失敗）
//=====================================================================================
SS MSP_CancelEventTimer(SS timerID)
{
	return	CEventTimer_Cancel(&msp_EventTimer, timerID) ;									// イベント･タイマーをキャンセルする
}


//=====================================================================================
// Function Name : MSP_TickEventTimer()
// Description   : カウントダウン処理を実行する.
// Parameter     : なし
// Return        : なし
// Note          : デバイスドライバーの1ms割り込み処理からコールする.
//               : 通常ユーザーには開示しない⇒ヘッダーにプロトタイプ宣言なし！！！
//=====================================================================================
void MSP_TickEventTimer()
{
	CEventTimer_Tick(&msp_EventTimer) ;														// イベント･タイマーのカウントダウン処理を実行する
}


//=====================================================================================
// Function Name : MSP_ActivateCommunication()
// Description   : 他のサブシステムへのコマンド(ステータス)を送信する.
// Parameter     : subsysID		"コマンド(ステータス)送信先サブシステムID"
//               : logID		"ログに表示するID"
// Return        : なし
// Note          : 本サービス実行により通信確立処理を開始する
//               : 処理実施結果はSystem Event "FRM_SYS_COMM_SYNC"にて通知される
//=====================================================================================
void MSP_ActivateCommunication(UC subsysID, UC logID)
{
	// ☆★☆ MSPではDDが自発的に通信を開始 ☆★☆
}


//=====================================================================================
// Function Name : MSP_ReceiveCommand()
// Description   : コマンド（ステータス）の受信およびコマンド･イベントの生成をする.
// Parameter     : pPacket		"サブシステムからの受信パケット格納アドレス（参照先）"
//               : length		"受信パケットサイズ"
// Return        : なし
//=====================================================================================
void MSP_ReceiveCommand(UC *pPacket, US length)
{
	CSubsystemGateway_ReceiveCommand(&msp_IotGateway, pPacket, length) ;					// 他サブシステムからのコマンドを受信する.
}


//=====================================================================================
// Function Name : MSP_SendCommand()
// Description   : 他のサブシステムへのコマンド(ステータス)を送信する.
// Parameter     : subsysID		"コマンド(ステータス)送信先サブシステムID"
//               : senderID		"コマンド(ステータス)送信元モジュールID"
//               : pPacket		"送信コマンド(ステータス)格納メモリの先頭アドレス"
//               : length		"送信コマンド(ステータス)のサイズ [単位:Byte]"
//               : log			"送信コマンド(ステータス)のログ情報保持の有無"
// Return        : なし
//=====================================================================================
void MSP_SendCommand(UC subsysID, UC senderID, const UC *pPacket, US length, UC log)
{
	CSubsystemGateway_SendCommand(&msp_IotGateway, senderID, pPacket, length, log) ;		// 他サブシステムへコマンドを送信する.
}


//=====================================================================================
// Function Name : MSP_UpdateLocationTiming()
// Description   : ロケーション･イベント発生タイミングを補正する.
// Parameter     : locationID	"タイミング補正対象ロケーションID（=イベントID）"
//               : pTime		"補正データ格納メモリの先頭アドレス"
// Return        : なし
//=====================================================================================
void MSP_UpdateLocationTiming(US locationID, const SS *pTime)
{
	CTimingController_UpdateTiming(MSP_GetEventNumber(locationID), pTime) ;					// ロケーション生成スケジュールのタイミング情報を更新する
}


//=====================================================================================
// Function Name : MSP_NotifyTriggerLocation()
// Description   : ロケーション･イベント発生トリガーを通知する.
// Parameter     : triggerID	"トリガーとなるロケーションID（=イベントID）"
//               : sheetNo		"ロケーションの対象となるシート･ナンバー"
//               : pattern		"対象シートのロケーション･パターン"
//               : index		"対象シートのスケジュール･インデックス番号"
// Return        : なし
//=====================================================================================
void MSP_NotifyTriggerLocation(US triggerID, US sheetNo, UL pattern, UC index)
{
	CTimingController_NotifyTrigger(MSP_GetEventNumber(triggerID), sheetNo, pattern, index) ;	// ロケーション生成スケジューリングを開始するトリガー発生を通知する
}


//=====================================================================================
// Function Name : MSP_AbortLocationScheduling()
// Description   : ロケーションのスケジューリングを中止する.（全てのタイマー停止）
// Parameter     : なし
// Return        : なし
// Caution       : 本関数はアプリケーションによる実行禁止！！！
//=====================================================================================
void MSP_AbortLocationScheduling()
{
	CTimingController_AbortScheduling() ;													// ロケーション生成のスケジューリングを中止する
}


//=====================================================================================
// Function Name : MSP_ResetLocationSchedule()
// Description   : ロケーションのスケジュールを初期化する.
// Parameter     : なし
// Return        : なし
// Caution       : 本関数はアプリケーションによる実行禁止！！！
//=====================================================================================
void MSP_ResetLocationSchedule()
{
	CTimingController_ResetSchedule() ;														// ロケーション生成スケジュール･テーブルを初期化する
}


//=====================================================================================
// Function Name : MSP_CreateLocationSchedule()
// Description   : ロケーションのスケジュールを構築する.
// Parameter     : なし
// Return        : なし
// Caution       : 本関数はアプリケーションによる実行禁止！！！
//=====================================================================================
void MSP_CreateLocationSchedule()
{
	CTimingController_CreateSchedule() ;													// ロケーション生成スケジュール･テーブルを構築する
}


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 14.04.01 | Mitsuyuki Nishizawa | Module Software Platform Frameのプロトタイプとして本ファイルを作成.
// 14.11.04 | Mitsuyuki Nishizawa | MSP提供関数名称変更に伴う修正.
//
//


// **************************************************************  [ MSP_Service.c : EOF ]  *******************************************************************
