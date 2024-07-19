// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_Common.h
/// @author  Mitsuyuki Nishizawa
/// @date    13.11.05
/// @version 001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef MSP_Common_h
#define MSP_Common_h


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

// ========================  <<< コンフィギュレーション関連共通定義 >>>  ========================				☆☆☆ プロダクト･ユニーク ☆☆☆

// ----- オプション・コンフィグ -----
#define CONFIG_BASE			0x00		// オプションなし
#define CONFIG_BOOK			0x01		// オプションなし (w/ ブックレット)
#define CONFIG_PFIM			0x02		// オプションなし (for PFIM)

#define CONFIG_PUNCH		0x04		// パンチ･ユニット (w/ トランスポート)
#define CONFIG_2IP_1		0x08		// 2段インターポーザー (prev. Finisher)
#define CONFIG_FOLDER		0x10		// フォルダー･ユニット
#define CONFIG_SQ_FLD		0x20		// トリマー&スクエア･フォールド
#define CONFIG_TCBM			0x40		// トリマー･クリース･バッファ･モジュール
#define CONFIG_2IP_2		0x80		// 2段インターポーザー (prev. TCBM)

// ----- バッファ制御 -----
#define BUFF_NOT_EXEC		0			// バッファ非対象用紙
#define BUFF_AT_FIN			1			// バッファ対象用紙＠フィニッシャー
#define BUFF_AT_TCBM		3			// バッファ対象用紙＠TCBM

// ----- 給紙トレイ -----
#define IOT					0			// IOT (or Other Device)
#define INSERTER1			1			// Inserter Tray1 (Upper)
#define INSERTER2			2			// Inserter Tray2 (Lower)

// ----- 用紙排出先 -----
#define TOP					0			// Top Tray
#define STACKER				1			// Stacker Tray
#define BOOKLET				2			// Booklet
#define FOLDER 				3			// Folder (Envelope)
#define MAILBOX 			4			// Mailbox Bin
#define PFIM				5			// PFIM


// ========================  <<< MSP/IOTアプリケーション共通定義 >>>  ========================

// ----- 汎用定義 -----
#define UNKNOWN		0

#define CLEAR		0
#define SET			1

#define INACTIVE	0
#define ACTIVE		1

// ----- I/Oエッジ定義 -----
//#define OFF			0
//#define ON			1

//#define FALL		0
//#define RISE		1

// ----- デバッグ･ログ出力切り替え -----
#define LOG_OFF		0
#define LOG_ON		1

// ----- 通信プライオリティー -----
#define	PRI_NORM	0
#define PRI_HIGH	1

// ===========================  <<< マクロ定義 >>>  ===========================

// ----- 処理結果 (汎用) -----
#define	MSP_OK				1																// 処理成功(OK)
#define	MSP_NG				0																// 処理失敗(NG)

// ----- タイマー処理結果 -----
#define	MSP_TMR_OK			0
#define	MSP_TMR_ERR			(-1)

// ----- ロケーション･タイマー関連 -----
#define	MSP_LOC_TMR_VOID	0x7FFF															// タイマーID：無効タイマーID
#define	MSP_LOC_TMR_FULL	0xFFFF															// タイマー･リソースに空きなし

// ----- 無効なイベント･ナンバー -----
#define	MSP_EVT_NUM_VOID	0x03FF															// イベント･ナンバー＝"1023"は使用不可！！！


#endif			// MSP_Common_h


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 13.05.05 | Mitsuyuki Nishizawa | Module Software Platform Frameのプロトタイプとして本ファイルを作成.
// 13.05.05 | Mitsuyuki Nishizawa | タイマー処理結果を追加.（Event Timerヘッダーから移動）
//
//


// *****************************************************************  [ MSP_Common.h : EOF ]  *****************************************************************
