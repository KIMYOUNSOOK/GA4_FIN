// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_ProductSpec.cpp
/// @author  Tsuyoshi Seto
/// @date    14/04/02
/// @time    16:37:27
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆
//  本ヘッダーファイルはイベント管理リストから自動生成されます。（プロダクト･ユニーク）
// ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆

#define MSP_ProductSpec_cpp

#include "MSP_Frame.h"

// ----- イベント･ハンドラー･テーブル & イベント･コントロール･テーブル -----
// === Event Index Number = 0 ===
static const EventControl msp_FrmSysTbl[] = {				// メッセージ 【Frame System】::コントロール・テーブル
	{ 0, 0 }
} ;

static const EventHandler msp_FrmHdlrTbl[] = {				// メッセージ 【Frame System】::ハンドラー・テーブル
	NULL
} ;

// === Event Index Number = 1 ===
static const EventControl msp_SnrEdgeTbl[] = {				// センサーエッジ::コントロール・テーブル
	{ 0, 0 }
} ;

static const EventHandler msp_SnrHdlrTbl[] = {				// センサーエッジ::ハンドラー・テーブル
	NULL
} ;

// === Event Index Number = 2 ===
static const EventControl msp_LocEvtTbl[] = {				// ロケーションタイミング::コントロール・テーブル
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 }
} ;

static const EventHandler msp_LocHdlrTbl[] = {				// ロケーションタイミング::ハンドラー・テーブル
	NULL
} ;

// === Event Index Number = 3 ===
static const EventControl msp_CdiCmdTbl[] = {				// コマンド＆ステータス【IOT】::コントロール・テーブル
	{  0, 1 },
	{  1, 1 },
	{  2, 1 },
	{  3, 1 },
	{  4, 1 },
	{  5, 1 },
	{  6, 1 },
	{  7, 1 },
	{  8, 1 },
	{  9, 1 },
	{ 10, 1 },
	{ 11, 1 },
	{ 12, 1 },
	{ 13, 1 },
	{ 14, 1 },
	{ 15, 1 },
	{ 16, 1 },
	{ 17, 1 },
	{ 18, 1 },
	{ 19, 1 },
	{ 20, 1 },
	{ 21, 1 }
} ;

static const EventHandler msp_CdiHdlrTbl[] = {				// コマンド＆ステータス【IOT】::ハンドラー・テーブル
	{ CDI_RcvSheetExit               },
	{ CDI_RcvSheetFinRequest         },
	{ CDI_RcvChangeShtFinRequest     },
	{ CDI_RcvCancelShtFinRequest     },
	{ CDI_RcvSetFinRequest           },
	{ CDI_RcvSelectFinAttribute      },
	{ CDI_RcvStopFinisher            },
	{ CDI_RcvDevSttsRequest          },
	{ CDI_RcvDevicePauseReq          },
	{ CDI_RcvReqJamSttsCheck         },
	{ CDI_RcvDevCapabilitiesRequest  },
	{ CDI_RcvDevConfigurationRequest },
	{ CDI_RcvSetDevConfiguration     },
	{ CDI_RcvChangeDevContMode       },
	{ CDI_RcvInitComplete            },
	{ CDI_RcvInitRequest             },
	{ CDI_RcvInitAcknowledge         },
	{ CDI_RcvPollRequest             },
	{ CDI_RcvPwrOffAnnouncement      },
	{ CDI_RcvExecuteDiag             },
	{ CDI_RcvDevContActionReq        },
	{ CDI_RcvSysContInfo             }
} ;

// === Event Index Number = 4 ===
static const EventControl msp_TcbmCmdTbl[] = {				// コマンド＆ステータス【TCBM】::コントロール・テーブル
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 }
} ;

static const EventHandler msp_TcbmHdlrTbl[] = {				// コマンド＆ステータス【TCBM】::ハンドラー・テーブル
	NULL
} ;

// === Event Index Number = 5 ===
static const EventControl msp_SubsysSttMsgTbl[] = {			// メッセージ 【Subsystem State】::コントロール・テーブル
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 }
} ;

static const EventHandler msp_SubSysSttHdlrTbl[] = {		// メッセージ 【Subsystem State】::ハンドラー・テーブル
	NULL
} ;

// === Event Index Number = 6 ===
static const EventControl msp_ActReqTbl[] = {				// メッセージ 【Device Action Request】::コントロール・テーブル
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 }
} ;

static const EventHandler msp_ActReqHdlrTbl[] = {			// メッセージ 【Device Action Request】::ハンドラー・テーブル
	NULL
} ;

// === Event Index Number = 7 ===
static const EventControl msp_ActResTbl[] = {				// メッセージ 【Device Action Response】::コントロール・テーブル
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 }
} ;

static const EventHandler msp_ActResHdlrTbl[] = {			// メッセージ 【Device Action Response】::ハンドラー・テーブル
	NULL
} ;

// === Event Index Number = 8 ===
static const EventControl msp_DevEvtTbl[] = {				// メッセージ 【Device Event】::コントロール・テーブル
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 }
} ;

static const EventHandler msp_DevEvtHdlrTbl[] = {			// メッセージ 【Device Event】::ハンドラー・テーブル
	NULL
} ;

// === Event Index Number = 9 ===
static const EventControl msp_ShtEvtTbl[] = {				// メッセージ 【Sheet Event】::コントロール・テーブル
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 }
} ;

static const EventHandler msp_ShtEvtHdlrTbl[] = {			// メッセージ 【Sheet Event】::ハンドラー・テーブル
	NULL
} ;

// ----- イベント･インデックス･テーブル -----
extern const EventIndexTable msp_EvtIndexTable[] = {		// Event Index ---> Event Control / Event Handler
	{ msp_FrmSysTbl      , msp_FrmHdlrTbl       },			// Index No.= 0: メッセージ 【Frame System】
	{ msp_SnrEdgeTbl     , msp_SnrHdlrTbl       },			// Index No.= 1: センサーエッジ
	{ msp_LocEvtTbl      , msp_LocHdlrTbl       },			// Index No.= 2: ロケーションタイミング
	{ msp_CdiCmdTbl      , msp_CdiHdlrTbl       },			// Index No.= 3: コマンド＆ステータス【IOT】
	{ msp_TcbmCmdTbl     , msp_TcbmHdlrTbl      },			// Index No.= 4: コマンド＆ステータス【TCBM】
	{ msp_SubsysSttMsgTbl, msp_SubSysSttHdlrTbl },			// Index No.= 5: メッセージ 【Subsystem State】
	{ msp_ActReqTbl      , msp_ActReqHdlrTbl    },			// Index No.= 6: メッセージ 【Device Action Request】
	{ msp_ActResTbl      , msp_ActResHdlrTbl    },			// Index No.= 7: メッセージ 【Device Action Response】
	{ msp_DevEvtTbl      , msp_DevEvtHdlrTbl    },			// Index No.= 8: メッセージ 【Device Event】
	{ msp_ShtEvtTbl      , msp_ShtEvtHdlrTbl    }			// Index No.= 9: メッセージ 【Sheet Event】
} ;

// --- ポート・エリア情報テーブル ---
extern const PortAreaInfo msp_PortAreaInfo[][2] = {
	{ { 0, 0 }, { 0, 0 } }
} ;

// --- センサーエッジ⇒イベント変換表 ---
extern const SensorEventInfo msp_SensorEdgeEvent[] = {
	NULL
} ;

// --- センサー･エッジ情報テーブル ---
extern const PortEdgeInfo msp_PortEdgeInfo[] = {
	{ 0, 0, { 0x0000, 0x0000 } }
} ;

// ----- 受信コマンド⇒イベント変換表 -----
static const UC msp_ContCmd2Event[] = {						// [[[ IOTからの受信コマンド ]]]
	0x23,													// Sheet Exit                           ⇒ Event No. 0
	0x21,													// Sheet Finish Request                 ⇒ Event No. 1
	0xE1,													// Change Sheet Finish Request          ⇒ Event No. 2
	0x28,													// Cancel Sheet Finish Request          ⇒ Event No. 3
	0x24,													// Set Finish Request                   ⇒ Event No. 4
	0x20,													// Select Finisher Attribute            ⇒ Event No. 5
	0x25,													// Stop Finisher                        ⇒ Event No. 6
	0x4E,													// Device Status Request                ⇒ Event No. 7
	0x36,													// Device Pause Request                 ⇒ Event No. 8
	0x37,													// Request Jam Status Check             ⇒ Event No. 9
	0x01,													// Device Capabilities Request          ⇒ Event No.10
	0x02,													// Device Configuration Request         ⇒ Event No.11
	0x07,													// Set Device Configuration             ⇒ Event No.12
	0x48,													// Change Device Controller Mode        ⇒ Event No.13
	0x82,													// Initialization Complete              ⇒ Event No.14
	0x86,													// Initialization Request               ⇒ Event No.15
	0x87,													// Initialization Acknowledge           ⇒ Event No.16
	0x88,													// Poll Request                         ⇒ Event No.17
	0x8A,													// Power Off Announcement               ⇒ Event No.18
	0xA0,													// Execute Diagnostics                  ⇒ Event No.19
	0xEC,													// Device Controller Action Request     ⇒ Event No.20
	0xED													// System Controller Information Notify ⇒ Event No.21
} ;

static const UC msp_TcbmCmd2Event[] = {						// [[[ TCBMからの受信コマンド ]]]
	0x22,													// Ready For Next Sheet                 ⇒ Event No. 0
	0x30,													// Sheet Delivered To Output            ⇒ Event No. 1
	0x33,													// Set Finished At Output               ⇒ Event No. 2
	0x27,													// Sheet Fed                            ⇒ Event No. 3
	0xE5,													// Abnormal Finishing                   ⇒ Event No. 4
	0x37,													// Report Pause Status                  ⇒ Event No. 5
	0x4F,													// Device Status Update                 ⇒ Event No. 6
	0xE0,													// Jam Zone Update                      ⇒ Event No. 7
	0xE2,													// Feed Aborted                         ⇒ Event No. 8
	0xE3,													// Set Recovery Request                 ⇒ Event No. 9
	0x02,													// Device Capabilities Update           ⇒ Event No.10
	0x04,													// Device Configuration Update          ⇒ Event No.11
	0x41,													// Device Controller State Update       ⇒ Event No.12
	0x45,													// Device Controller Mode Update        ⇒ Event No.13
	0x86,													// Initialization Request               ⇒ Event No.14
	0x87,													// Initialization Acknowledge           ⇒ Event No.15
	0x8B,													// Power Off Acknowledge                ⇒ Event No.16
	0xA1,													// Diagnostics Response                 ⇒ Event No.17
	0xEE													// Device Controller Information Notify ⇒ Event No.18
} ;

// --- サブシステム・ゲートウェイ・スペック ---

extern const GatewaySpec msp_IotGwSpec = {					// 対IOTゲートウェイの仕様記述
	SID_IOT                   ,
	EVT_CDI_CMD               ,
	sizeof(msp_ContCmd2Event) ,
	msp_ContCmd2Event         ,
	&msp_IotSender            ,
	&msp_IotReceiver
} ;

extern const GatewaySpec msp_TcbmGwSpec = {					// 対TCBMゲートウェイの仕様記述
	SID_TCBM                  ,
	EVT_TCBM_CMD              ,
	sizeof(msp_TcbmCmd2Event) ,
	msp_TcbmCmd2Event         ,
	&msp_TcbmSender           ,
	&msp_TcbmReceiver
} ;

// ----- ロケーション･イベント仕様設定 -----
extern const LocationSpec msp_LocationSpec[MSP_NUM_OF_LOC] = {
//	+-----------+---------------------+------------+------+------------------------------------------+
//	| Triger ID | Timing (x Schedule) | Pattern    | Parm |     Location Event                       |
//	+-----------+---------------------+------------+------+------------------------------------------+
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },                   // IOT Exit Sensor ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },                  // IOT Exit Sensor OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },                 // H Tra Exit Sensor ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },                // H Tra Exit Sensor OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },                 // V Tra Exit Sensor ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },                // V Tra Exit Sensor OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },            // Folder Entrance Sensor ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },           // Folder Entrance Sensor OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },              // Folder Path Sensor 1 ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },             // Folder Path Sensor 1 OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },              // Folder Path Sensor 2 ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },             // Folder Path Sensor 2 OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },              // Folder Path Sensor 3 ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },             // Folder Path Sensor 3 OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },              // Folder Path Sensor 4 ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },             // Folder Path Sensor 4 OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },                // Folder Exit Sensor ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },               // Folder Exit Sensor OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },                 // S Tra Exit Sensor ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },                // S Tra Exit Sensor OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },                   // Punch In Sensor ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },                  // Punch In Sensor OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },                // Finisher In Sensor ON
	{    0x0816 ,                  20 , 0x00000003 ,    0 },              // XXX Home Position Start
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },               // Finisher In Sensor OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },              // Top Tray Exit Sensor ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },             // Top Tray Exit Sensor OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },               // Compile Exit Sensor ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },              // Compile Exit Sensor OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },           // Compile No Paper Sensor ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },          // Compile No Paper Sensor OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },                      // Eject Sensor ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },                     // Eject Sensor OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },                 // Booklet In Sensor ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },                // Booklet In Sensor OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },   // Booklet Compile No Paper Sensor ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },  // Booklet Compile No Paper Sensor OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },  // Booklet Compile No Paper Sensor2 ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 }, // Booklet Compile No Paper Sensor2 OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },               // Booklet Exit Sensor ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },              // Booklet Exit Sensor OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },                 // Trimmer In Sensor ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },                // Trimmer In Sensor OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },              // Trimmer Path Sensor1 ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },             // Trimmer Path Sensor1 OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },              // Trimmer Path Sensor2 ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },             // Trimmer Path Sensor2 OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },              // Trimmer Path Sensor3 ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },             // Trimmer Path Sensor3 OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },            // Trimmer Path Sensor3_1 ON
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },           // Trimmer Path Sensor3_1 OFF
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },                 // PFIM Tp In Sensor ON
	{    0x03FF ,                   0 , 0x00000000 ,    0 }                  // PFIM Tp In Sensor OFF
} ;

// *************************************************************  [ MSP_ProductSpec.cpp : EOF ]  **************************************************************
