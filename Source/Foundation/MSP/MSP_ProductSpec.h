// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_ProductSpec.h
/// @author  Mitsuyuki Nishizawa
/// @date    13.05.05
/// @version 000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef MSP_ProductSpec_h
#define MSP_ProductSpec_h


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

// ===========================  <<< データ型定義 >>>  ===========================

// ----- イベントID -----																	// <<< イベントIDの型 >>>
typedef US		EventID ;																	// 上位6bit:Event Index / 下位10bit:Event Number

US MSP_GetEventID(US idx, US num) ;															// Event変換マクロ： Index,Number ⇒ ID変換
US MSP_GetEventIndex(US id) ;																// Event変換マクロ： ID ⇒ Index変換
US MSP_GetEventNumber(US id) ;																// Event変換マクロ： ID ⇒ Number変換

// ----- イベント･ハンドラ･テーブル -----
typedef struct EventHandler {																// <<< イベント･ハンドラー･テーブルの型 >>>
	void (*mpHandler)(const UC*, UC) ;														// ハンドラーのアドレス
} EventHandler ;

// ----- イベント･コントロール･テーブル -----
typedef struct {																			// <<< ハンドラー･テーブルへのアクセス情報 >>>
	US	mStart : 10 ;																		// コールバック開始位置
	US	mCount : 6 ;																		// コールバック関数登録数
} EventControl ;

// ----- イベント･インデックス･テーブル -----
typedef struct {																			// Indexに対応する･･･
	const EventControl *mpControlTable ;													// コントロール･テーブルの先頭アドレス
	const EventHandler *mpHandlerTable ;													// ハンドラー･テーブルの先頭アドレス
} EventIndexTable;

extern const EventIndexTable msp_EvtIndexTable[] ;

// ----- ポート･エリア情報テーブル -----
typedef struct {																			// <<< センサー･イベント･テーブルへのアクセス情報 >>>
	US	mStart : 10 ;																		// ポートチェック開始位置
	US	mCount : 6 ;																		// 有効ポート登録数
} PortAreaInfo ;

extern const PortAreaInfo msp_PortAreaInfo[][2] ;

// ----- センサー･イベント情報テーブル -----												// <<< センサー･エッジ⇒イベント･ナンバー変換情報 >>>
typedef UC SensorEventInfo ;																// ポートのビット位置（0～15）

extern const SensorEventInfo msp_SensorEdgeEvent[] ;

// ----- センサー･エッジ情報テーブル -----
typedef struct {																			// <<< センサー･エッジ･テーブルの型 >>>
	UC	mIndex ;																			// ポート･インデックス番号（DD提供の番号）
	UC	mAreaID ;																			// ポート･エリアID
	UL	mMask[2] ;																			// ポート･エッジ･マスク[RISE/FALL]
} PortEdgeInfo ;

extern const PortEdgeInfo msp_PortEdgeInfo[] ;

// ----- サブシステム･ゲートウェイ･スペック -----
typedef struct {
	UC	mSubsystemID ;																		// 通信対象となるサブシステムの通信用ID
	UC	mEvtIndex ;																			// コマンド･イベント生成用のイベント･インデックス
	UC	mListSize ;																			// "コマンド⇒イベント変換リスト"のサイズ
	const UC  *mpCommandList ;																// コマンド⇒イベント変換リスト
//	ComDataSender  *mpSender ;																// コマンド送信対象サブシステムのポインタ
//	ComDataReceiver  *mpReceiver ;															// コマンド受信対象サブシステムのポインタ
} GatewaySpec ;

extern const GatewaySpec msp_IotGwSpec ;													// 対IOTの仕様記述

// ----- ロケーション･イベント･スペック -----
typedef struct {
	US	mTrigger ;																			// トリガーとなるロケーションID（=イベントID）
	US	mDelay[MSP_NUM_OF_SCH] ;															// トリガーからの遅延時間（スケジュール数分）
	UL	mFilter ;																			// ロケーションのフィルタリング･データ
	UL	mParameter ;																		// ロケーション･イベント発生時のコールバック･パラメータ
} LocationSpec ;

extern const LocationSpec msp_LocationSpec[] ;												// ロケーション･イベントの仕様設定テーブル


#endif			// MSP_ProductSpec_h


// Change History
// Date:    | Engineer:          | Note:
// ---------+--------------------+-----------------------------------------------------------------------------------------------------------------------------
// 13.05.05 | Mitsuyuki Nishizawa| Module Software Platform Frameのプロトタイプとして本ファイルを作成.
//


// **************************************************************  [ MSP_ProductSpec.h : EOF ]  ***************************************************************
