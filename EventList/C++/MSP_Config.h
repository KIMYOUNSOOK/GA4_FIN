// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_Config.h
/// @author  Tsuyoshi Seto
/// @date    14/04/02
/// @time    16:37:23
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆
//  本ヘッダーファイルはイベント管理リストから自動生成されます。（プロダクト･ユニーク）
// ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆

#ifndef MSP_Config_h
#define MSP_Config_h


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

// ----- ポート･エリア数 -----
#define	MSP_NUM_OF_AREA		0																	// ポート･エリアの数

// ----- コマンド/ステータス最大パケットサイズ -----
#define	MSP_MAX_CMD_SIZE	256																	// 受信コマンド（ステータス）の最大パケットサイズ（最大:1024）

// ----- イベント最大パケットサイズ -----
#define	MSP_MAX_MSG_SIZE	258																	// 送受信イベントの最大パケットサイズ（最大:1024）

// ----- ロケーション・イベント数 -----
#define	MSP_NUM_OF_LOC		53																	// ロケーション・イベント数(スケジュール・テーブルの要素数として使用)

// ----- ロケーション・タイマー数 -----
#define	MSP_NUM_OF_TMR		64																	// ロケーション･イベントを生成するために使用する管理タイマー数（最大:255）

// ----- パフォーマンス・ログ履歴数 -----
#define	ISP_NUM_OF_LOG		64																	// パフォーマンス測定用のログ領域確保数（最大:128）

// ----- イベント･インデックス定義 -----
enum EEventIndex {																				// <<< Event Index ID >>>	8～62はプロダクトで割り付けを実施
	EVT_FRM_SYS    =  0,																		//  0: メッセージ 【Frame System】				（共通定義）･･･ < 予約番号 >
	EVT_SNR_EDGE   =  1,																		//  1: センサーエッジ							（共通定義）･･･ < 予約番号 >
	EVT_LOC_TMNG   =  2,																		//  2: ロケーションタイミング					（共通定義）･･･ < 予約番号 >
	EVT_CDI_CMD    =  3,																		//  3: コマンド＆ステータス【IOT】				（共通定義）･･･ < 予約番号 >
	EVT_TCBM_CMD   =  4,																		//  4: コマンド＆ステータス【TCBM】				（共通定義）･･･ < 予約番号 >
	EVT_SUBSYS_STT =  5,																		//  5: メッセージ 【Subsystem State】			（共通定義）･･･ < 予約番号 >
	EVT_ACT_REQ    =  6,																		//  6: メッセージ 【Device Action Request】		（共通定義）･･･ < 予約番号 >
	EVT_ACT_RES    =  7,																		//  7: メッセージ 【Device Action Response】	（共通定義）･･･ < 予約番号 >
	EVT_DEV_EVT    =  8,																		//  8: メッセージ 【Device Event】				（共通定義）･･･ < 予約番号 >
	EVT_SHT_EVT    =  9,																		//  9: メッセージ 【Sheet Event】				（共通定義）･･･ < 予約番号 >
	EVT_TIME_OUT   = 63																			// 63: イベントタイマー							（共通定義）･･･ < 予約番号 >
} ;

// ----- イベント･ハンドラー定義 -----															// イベント･ハンドラーとして定義された関数を全て記述する
extern void CDI_RcvSheetExit(const UC*, UC) ;
extern void CDI_RcvSheetFinRequest(const UC*, UC) ;
extern void CDI_RcvChangeShtFinRequest(const UC*, UC) ;
extern void CDI_RcvCancelShtFinRequest(const UC*, UC) ;
extern void CDI_RcvSetFinRequest(const UC*, UC) ;
extern void CDI_RcvSelectFinAttribute(const UC*, UC) ;
extern void CDI_RcvStopFinisher(const UC*, UC) ;
extern void CDI_RcvDevSttsRequest(const UC*, UC) ;
extern void CDI_RcvDevicePauseReq(const UC*, UC) ;
extern void CDI_RcvReqJamSttsCheck(const UC*, UC) ;
extern void CDI_RcvDevCapabilitiesRequest(const UC*, UC) ;
extern void CDI_RcvDevConfigurationRequest(const UC*, UC) ;
extern void CDI_RcvSetDevConfiguration(const UC*, UC) ;
extern void CDI_RcvChangeDevContMode(const UC*, UC) ;
extern void CDI_RcvInitComplete(const UC*, UC) ;
extern void CDI_RcvInitRequest(const UC*, UC) ;
extern void CDI_RcvInitAcknowledge(const UC*, UC) ;
extern void CDI_RcvPollRequest(const UC*, UC) ;
extern void CDI_RcvPwrOffAnnouncement(const UC*, UC) ;
extern void CDI_RcvExecuteDiag(const UC*, UC) ;
extern void CDI_RcvDevContActionReq(const UC*, UC) ;
extern void CDI_RcvSysContInfo(const UC*, UC) ;


#endif          // MSP_Config_h

// ****************************************************************  [ MSP_Config.h : EOF ]  *****************************************************************
