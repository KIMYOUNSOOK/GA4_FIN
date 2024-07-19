// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_Service.h
/// @author  Mitsuyuki Nishizawa
/// @date    14.11.04
/// @version 001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef MSP_Service_h
#define MSP_Service_h

#include "MSP_Common.h"
#include "MSP_Config.h"
#include "MSP_EventID.h"
#include "MSP_ProductSpec.h"
#include "MSP_MessageType.h"
#include "MSP_EventMain.h"


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

//=====================================================================================											// <<< SYSTEM EVENT >>>
// Function Name : MSP_StartUp()                                                      											// ┌───────────┐
// Description   : μiSP Frameの起動を行う.                                            											// ├ "START UP"           ┤
// Parameter     : なし                                                                											// └───────────┘
// Return        : なし
//=====================================================================================
void MSP_StartUp() ;

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
void MSP_SendMessage(UC senderID, US eventID, const UC *pMessage, US length, UC log) ;

//=====================================================================================
// Function Name : MSP_SetEventTimerHigh()
// Description   : タイムアウト･イベント（高優先度）の生成をリクエストする.
// Parameter     : delayTime	"タイムアウト･イベント発生の遅延時間"
//               : pHandler		"イベント発生時のコールバック･ハンドラ"
//               : param		"ハンドラをコールバックする際のパラメータ"
// Return        : タイマーセットの結果（成功:タイマーID / 失敗:エラーコード）
//=====================================================================================
SS MSP_SetEventTimerHigh(US delayTime, void (*pHandler)(SS, UL), UL param) ;

//=====================================================================================
// Function Name : MSP_SetEventTimer()
// Description   : タイムアウト･イベント（低優先度）の生成をリクエストする.
// Parameter     : delayTime	"タイムアウト･イベント発生の遅延時間"
//               : pHandler		"イベント発生時のコールバック･ハンドラ"
//               : param		"ハンドラをコールバックする際のパラメータ"
// Return        : タイマーセットの結果（成功:タイマーID / 失敗:エラーコード）
//=====================================================================================
SS MSP_SetEventTimer(US delayTime, void (*pHandler)(SS, UL), UL param) ;

//=====================================================================================
// Function Name : MSP_CancelEventTimer()
// Description   : タイムアウト･イベントの生成をキャンセルする.
// Parameter     : timerID		"タイムアウト･イベント生成用タイマーID"
// Return        : タイマーセットの結果（キャンセル成功 / キャンセル失敗）
//=====================================================================================
SS MSP_CancelEventTimer(SS timerID) ;

//=====================================================================================
// Function Name : MSP_ActivateCommunication()
// Description   : 他のサブシステムへのコマンド(ステータス)を送信する.
// Parameter     : subsysID		"コマンド(ステータス)送信先サブシステムID"
//               : logID		"ログに表示するID"
//               : failFunc		"通信フェイル時にコールバックされる関数"
// Return        : なし
// Note          : 本サービス実行により通信確立処理を開始する
//               : 処理実施結果はSystem Event "FRM_SYS_COMM_SYNC"にて通知される
//=====================================================================================
void MSP_ActivateCommunication(UC subsysID, UC logID) ;

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
void MSP_SendCommand(UC subsysID, UC senderID, const UC *pPacket, US length, UC log) ;

//=====================================================================================
// Function Name : MSP_UpdateLocationTiming()
// Description   : ロケーション･イベント発生タイミングを補正する.
// Parameter     : locationID	"タイミング補正対象ロケーションID（=イベントID）"
//               : pTime		"補正データ格納メモリの先頭アドレス"
// Return        : なし
//=====================================================================================
void MSP_UpdateLocationTiming(US locationID, const SS *pTime) ;

//=====================================================================================
// Function Name : MSP_NotifyTriggerLocation()
// Description   : ロケーション･イベント発生トリガーを通知する.
// Parameter     : triggerID	"トリガーとなるロケーションID（=イベントID）"
//               : sheetNo		"ロケーションの対象となるシート･ナンバー"
//               : pattern		"対象シートのロケーション･パターン"
//               : index		"対象シートのスケジュール･インデックス番号"
// Return        : なし
//=====================================================================================
void MSP_NotifyTriggerLocation(US triggerID, US sheetNo, UL pattern, UC index) ;

//=====================================================================================
// Function Name : MSP_AbortLocationScheduling()
// Description   : ロケーションのスケジューリングを中止する.（全てのタイマー停止）
// Parameter     : なし
// Return        : なし
// Caution       : 本関数はアプリケーションによる実行禁止！！！
//=====================================================================================
void MSP_AbortLocationScheduling() ;

//=====================================================================================
// Function Name : MSP_ResetLocationSchedule()
// Description   : ロケーションのスケジュールを初期化する.
// Parameter     : なし
// Return        : なし
// Caution       : 本関数はアプリケーションによる実行禁止！！！
//=====================================================================================
void MSP_ResetLocationSchedule() ;

//=====================================================================================
// Function Name : MSP_CreateLocationSchedule()
// Description   : ロケーションのスケジュールを構築する.
// Parameter     : なし
// Return        : なし
// Caution       : 本関数はアプリケーションによる実行禁止！！！
//=====================================================================================
void MSP_CreateLocationSchedule() ;


#endif			// MSP_Service_h


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 14.05.05 | Mitsuyuki Nishizawa | Module Software Platform Frameのプロトタイプとして本ファイルを作成.
// 14.11.04 | Mitsuyuki Nishizawa | MSP提供関数名称変更に伴う修正.
//
//


// ****************************************************************  [ MSP_Service.h : EOF ]  *****************************************************************
