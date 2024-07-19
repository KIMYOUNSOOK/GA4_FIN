// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_ProductSpec.c
/// @author  Tsuyoshi Seto
/// @date    16/03/11
/// @time    16:55:14
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆
//  本ヘッダーファイルはイベント管理リストから自動生成されます。（プロダクト･ユニーク）
// ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆

#define MSP_ProductSpec_c

#include "Common.h"
#include "LIB_Service.h"
#include <string.h>
#include "MSP_Common.h"
#include "MSP_Config.h"
#include "MSP_EventID.h"
#include "MSP_ProductSpec.h"

// ----- イベント･ハンドラー･テーブル & イベント･コントロール･テーブル -----
// === Event Index Number = 0 ===
static const EventControl msp_FrmSysTbl[] = {				// メッセージ 【Frame System】::コントロール・テーブル
	{ 0, 4 },
	{ 4, 1 },
	{ 5, 1 }
} ;

static const EventHandler msp_FrmHdlrTbl[] = {				// メッセージ 【Frame System】::ハンドラー・テーブル
	{ SYS_StartUpEvent  },
	{ CMPL_StartUpEvent },
	{ TRNS_StartUpEvent },
	{ TRAY_StartUpEvent },
	{ CDI_ComSyncNotify },
	{ CDI_ComFailNotify }
} ;

// === Event Index Number = 1 ===
static const EventControl msp_SnrEdgeTbl[] = {				// センサーエッジ::コントロール・テーブル
	{  0, 1 },
	{  1, 1 },
	{  2, 1 },
	{  3, 1 },
	{  4, 0 },
	{  4, 0 },
	{  4, 1 },
	{  5, 1 },
	{  6, 1 },
	{  7, 1 },
	{  8, 1 },
	{  9, 1 },
	{ 10, 0 },
	{ 10, 1 },
	{ 11, 1 },
	{ 12, 2 },
	{ 14, 1 },
	{ 15, 1 },
	{ 16, 1 },
	{ 17, 1 },
	{ 18, 1 },
	{ 19, 0 },
	{ 19, 0 },
	{ 19, 1 },
	{ 20, 1 },
	{ 21, 1 },
	{ 22, 1 },
	{ 23, 1 },
	{ 24, 1 },
	{ 25, 0 },
	{ 25, 1 },
	{ 26, 1 },
	{ 27, 2 },
	{ 29, 1 }
} ;

static const EventHandler msp_SnrHdlrTbl[] = {				// センサーエッジ::ハンドラー・テーブル
	{ CVR_StapleDoorILKSwitchON             },
	{ SHT_IotExitSensorON                   },
	{ SHT_CompileExitSensorON               },
	{ CMPL_STPL_HomeSensorON                },
	{ TRAY_STK_Height2SensorON              },
	{ TRAY_STK_Height1SensorON              },
	{ CMPL_TMP_RIGHT_RTHomeSensorON         },
	{ CMPL_TMP_LEFT_LTHomeSensorON          },
	{ CMPL_EJCT_SCLMP_SetClampHomeSensorON  },
	{ CVR_RearCoverSwitchON                 },
	{ TRAY_STK_NoPaperFullSensorON          },
	{ CMPL_EJCT_SubPaddleHomeSensorON       },
	{ CMPL_EJCT_BELT_EjecterHomeSensorON    },
	{ CMPL_EJCT_SCLMP_EjecterHomeSensorON   },
	{ TRNS_IOTGATE_SignalON                 },
	{ CVR_StapleDoorILKSwitchOFF            },
	{ SHT_IotExitSensorOFF                  },
	{ SHT_CompileExitSensorOFF              },
	{ CMPL_STPL_HomeSensorOff               },
	{ TRAY_STK_Height2SensorOFF             },
	{ TRAY_STK_Height1SensorOFF             },
	{ CMPL_TMP_RIGHT_RTHomeSensorOFF        },
	{ CMPL_TMP_LEFT_LTHomeSensorOFF         },
	{ CMPL_EJCT_SCLMP_SetClampHomeSensorOFF },
	{ CVR_RearCoverSwitchOFF                },
	{ TRAY_STK_NoPaperFullSensorOFF         },
	{ CMPL_EJCT_SubPaddleHomeSensorOFF      },
	{ CMPL_EJCT_BELT_EjecterHomeSensorOFF   },
	{ CMPL_EJCT_SCLMP_EjecterHomeSensorOFF  },
	{ TRNS_IOTGATE_SignalOFF                }
} ;

// === Event Index Number = 2 ===
static const EventControl msp_LocEvtTbl[] = {				// ロケーションタイミング::コントロール・テーブル
	{ 0, 1 },
	{ 1, 0 },
	{ 1, 2 },
	{ 3, 2 }
} ;

static const EventHandler msp_LocHdlrTbl[] = {				// ロケーションタイミング::ハンドラー・テーブル
	{ TRNS_IotExitLocationLead        },
	{ CMPL_CompileExitSensorLeadEvent },
	{ TRNS_CompileExitLocationLead    },
	{ CMPL_CompileExitSensorTailEvent },
	{ TRNS_CompileExitLocationTail    }
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
static const EventControl msp_FinSttMsgTbl[] = {			// メッセージ 【Finisher State】::コントロール・テーブル
	{  0, 3 },
	{  3, 1 },
	{  4, 0 },
	{  4, 3 },
	{  7, 1 },
	{  8, 0 },
	{  8, 3 },
	{ 11, 1 },
	{ 12, 0 },
	{ 12, 3 },
	{ 15, 1 },
	{ 16, 1 },
	{ 17, 3 },
	{ 20, 1 },
	{ 21, 0 },
	{ 21, 3 },
	{ 24, 3 },
	{ 27, 1 },
	{ 28, 2 }
} ;

static const EventHandler msp_FinSttHdlrTbl[] = {			// メッセージ 【Finisher State】::ハンドラー・テーブル
	{ TRNS_PowerUpRequestEvent      },
	{ TRAY_PowerUpRequestEvent      },
	{ CMPL_PowerUpReqEvent          },
	{ SYS_PowerUpResponseEvent      },
	{ TRNS_PowerDownRequestEvent    },
	{ TRAY_PowerDownRequestEvent    },
	{ CMPL_PowerDownReqEvent        },
	{ SYS_PowerDownResponseEvent    },
	{ TRNS_ChangeDiagRequestEvent   },
	{ TRAY_ChangeDiagRequestEvent   },
	{ CMPL_ChangeDiagReqEvent       },
	{ SYS_ChangeDiagResponseEvent   },
	{ TRNS_ChangeNormalRequestEvent },
	{ TRAY_ChangeNormalRequestEvent },
	{ CMPL_ChangeNormalReqEvent     },
	{ SYS_ChangeNormalResponseEvent },
	{ CMPL_StandbyNotifyEvent       },
	{ TRNS_CycleUpRequestEvent      },
	{ TRAY_CycleUpRequestEvent      },
	{ CMPL_CycleUpReqEvent          },
	{ SYS_CycleUpResponseEvent      },
	{ TRNS_CycleDownRequestEvent    },
	{ TRAY_CycleDownRequestEvent    },
	{ CMPL_CycleDownReqEvent        },
	{ TRNS_HardDownRequestEvent     },
	{ TRAY_HardDownRequestEvent     },
	{ CMPL_HardDownReqEvent         },
	{ SYS_ProcessDownResponseEvent  },
	{ CMPL_StandbyExitNotifyEvent   },
	{ TRAY_StandbyExitNotifyEvent   }
} ;

// === Event Index Number = 5 ===
static const EventControl msp_DevActReqTbl[] = {			// メッセージ 【Device Action Request】::コントロール・テーブル
	{ 0, 1 },
	{ 1, 1 },
	{ 2, 1 },
	{ 3, 1 },
	{ 4, 1 },
	{ 5, 1 },
	{ 6, 1 },
	{ 7, 1 },
	{ 8, 1 }
} ;

static const EventHandler msp_DevActReqHdlrTbl[] = {		// メッセージ 【Device Action Request】::ハンドラー・テーブル
	{ CMPL_DevActReqPowerOnInitEvent         },
	{ CMPL_DevActReqResumeInitEvent          },
	{ CMPL_DevActReqNormalInitEvent          },
	{ TRNS_DeviceActionRequestJamCheck       },
	{ TRNS_DeviceActionRequestProcessEnd     },
	{ CMPL_DevActReqProcessEndEvent          },
	{ TRAY_DeviceActionRequestProcessEnd     },
	{ TRNS_DeviceActionRequestJamClearAssist },
	{ CMPL_DevActReqJamClearAssistEvent      }
} ;

// === Event Index Number = 6 ===
static const EventControl msp_DevActResTbl[] = {			// メッセージ 【Device Action Response】::コントロール・テーブル
	{ 0, 1 },
	{ 1, 1 },
	{ 2, 1 },
	{ 3, 1 },
	{ 4, 1 },
	{ 5, 1 },
	{ 6, 1 },
	{ 7, 1 },
	{ 8, 1 }
} ;

static const EventHandler msp_DevActResHdlrTbl[] = {		// メッセージ 【Device Action Response】::ハンドラー・テーブル
	{ SYS_DeviceActionResponseEventPowerOnInitializeCMPL        },
	{ SYS_DeviceActionResponseEventPowerResumeInitializeCMPL    },
	{ SYS_DeviceActionResponseEventNormalInitializeCMPL         },
	{ SYS_DeviceActionResponseEventJamCheckTRNS                 },
	{ SYS_DeviceActionResponseEventProcessEndActionTRNS         },
	{ SYS_DeviceActionResponseEventProcessEndActionCMPL         },
	{ SYS_DeviceActionResponseEventProcessEndActionTRAY         },
	{ SYS_DeviceActionResponseEventJamClearAssistInitializeTRNS },
	{ SYS_DeviceActionResponseEventJamClearAssistInitializeCMPL }
} ;

// === Event Index Number = 7 ===
static const EventControl msp_DevEvtTbl[] = {				// メッセージ 【Device Event】::コントロール・テーブル
	{ 0, 3 },
	{ 3, 3 },
	{ 6, 1 }
} ;

static const EventHandler msp_DevEvtHdlrTbl[] = {			// メッセージ 【Device Event】::ハンドラー・テーブル
	{ TRNS_DeviceEventAnyCoverOpen  },
	{ TRAY_DeviceEventAnyCoverOpen  },
	{ CMPL_DevEvtAnyCoverOpenEvent  },
	{ TRNS_DeviceEventAllCoverClose },
	{ TRAY_DeviceEventAllCoverClose },
	{ CMPL_DevEvtAllCoverCloseEvent },
	{ CMPL_DevEvtDevInitCompEvent   }
} ;

// === Event Index Number = 8 ===
static const EventControl msp_CondTbl[] = {					// メッセージ 【Condition】::コントロール・テーブル
	{ 0, 1 },
	{ 1, 0 },
	{ 1, 0 },
	{ 1, 0 }
} ;

static const EventHandler msp_CondHdlrTbl[] = {				// メッセージ 【Condition】::ハンドラー・テーブル
	{ SYS_DeviceStatusNotifyEvent }
} ;

// === Event Index Number = 9 ===
static const EventControl msp_ShtInfoTbl[] = {				// メッセージ 【Sheet Information】::コントロール・テーブル
	{ 0, 0 },
	{ 0, 1 },
	{ 1, 1 },
	{ 2, 2 }
} ;

static const EventHandler msp_ShtInfoHdlrTbl[] = {			// メッセージ 【Sheet Information】::ハンドラー・テーブル
	{ CMPL_SheetInformationEventUpdate   },
	{ CMPL_SheetInfomationEventDelivered },
	{ TRNS_SheetAbortedNotify            },
	{ CMPL_SheetInformationEventAbort    }
} ;

// === Event Index Number = 10 ===
static const EventControl msp_ShtActReqTbl[] = {			// メッセージ 【Sheet Action Request】::コントロール・テーブル
	{ 0, 1 },
	{ 1, 1 }
} ;

static const EventHandler msp_ShtActReqHdlrTbl[] = {		// メッセージ 【Sheet Action Request】::ハンドラー・テーブル
	{ CMPL_SheetActionRequestEventCompilerShtInitCMPL },
	{ TRAY_SheetActionRequestEventStackerShtInitTRAY  }
} ;

// === Event Index Number = 11 ===
static const EventControl msp_ShtActResTbl[] = {			// メッセージ 【Sheet Action Response】::コントロール・テーブル
	{ 0, 1 },
	{ 1, 1 }
} ;

static const EventHandler msp_ShtActResHdlrTbl[] = {		// メッセージ 【Sheet Action Response】::ハンドラー・テーブル
	{ SHT_SheetActionResponseEventCompilerShtInitCMPL },
	{ SHT_SheetActionResponseEventStackerShtInitTRAY  }
} ;

// === Event Index Number = 12 ===
static const EventControl msp_ShtEvtTbl[] = {				// メッセージ 【Sheet Event Notify】::コントロール・テーブル
	{  0, 1 },
	{  1, 1 },
	{  2, 2 },
	{  4, 1 },
	{  5, 1 },
	{  6, 1 },
	{  7, 1 },
	{  8, 1 },
	{  9, 1 },
	{ 10, 1 }
} ;

static const EventHandler msp_ShtEvtHdlrTbl[] = {			// メッセージ 【Sheet Event Notify】::ハンドラー・テーブル
	{ SHT_SheetEventNotifyEventOutTrayEjectFinishStart        },
	{ SHT_SheetEventNotifyEventOutTrayEjectFinishComp         },
	{ SHT_SheetEventNotifyEventOutTrayEjectStart              },
	{ TRAY_SheetEventNotifyEventOutTrayEjectStart             },
	{ SHT_SheetEventNotifyEventOutTrayCompileFinishComp       },
	{ SHT_SheetEventNotifyEventOutTrayEjectComp               },
	{ JAM_SheetEventNotifyEventOutTrayJamOccurrence           },
	{ TRAY_SheetEventNotifyShtInitStackerJobStart             },
	{ TRAY_SheetEventNotifyShtInitStackerWaitingPositionStart },
	{ TRAY_SheetEventNotifyTrayJobTrigger_1                   },
	{ TRAY_SheetEventNotifyTrayJobTrigger_2                   }
} ;

// === Event Index Number = 13 ===
static const EventControl msp_DiagEvtTbl[] = {				// メッセージ 【Diagnostics】::コントロール・テーブル
	{ 0, 3 },
	{ 3, 1 },
	{ 4, 3 },
	{ 7, 1 }
} ;

static const EventHandler msp_DiagHdlrTbl[] = {				// メッセージ 【Diagnostics】::ハンドラー・テーブル
	{ TRNS_ComponentControlStartReq         },
	{ TRAY_ComponentControlStartReq         },
	{ CMPL_ComponentControlStartReq         },
	{ DIAG_ComponentControlStartNotifyEvent },
	{ TRNS_ComponentControlStopReq          },
	{ TRAY_ComponentControlStopReq          },
	{ CMPL_ComponentControlStopReq          },
	{ DIAG_ComponentControlStopNotifyEvent  }
} ;

// ----- イベント･インデックス･テーブル -----
extern const EventIndexTable msp_EvtIndexTable[] = {		// Event Index ---> Event Control / Event Handler
	{ msp_FrmSysTbl   , msp_FrmHdlrTbl       },				// Index No.= 0: メッセージ 【Frame System】
	{ msp_SnrEdgeTbl  , msp_SnrHdlrTbl       },				// Index No.= 1: センサーエッジ
	{ msp_LocEvtTbl   , msp_LocHdlrTbl       },				// Index No.= 2: ロケーションタイミング
	{ msp_CdiCmdTbl   , msp_CdiHdlrTbl       },				// Index No.= 3: コマンド＆ステータス【IOT】
	{ msp_FinSttMsgTbl, msp_FinSttHdlrTbl    },				// Index No.= 4: メッセージ 【Finisher State】
	{ msp_DevActReqTbl, msp_DevActReqHdlrTbl },				// Index No.= 5: メッセージ 【Device Action Request】
	{ msp_DevActResTbl, msp_DevActResHdlrTbl },				// Index No.= 6: メッセージ 【Device Action Response】
	{ msp_DevEvtTbl   , msp_DevEvtHdlrTbl    },				// Index No.= 7: メッセージ 【Device Event】
	{ msp_CondTbl     , msp_CondHdlrTbl      },				// Index No.= 8: メッセージ 【Condition】
	{ msp_ShtInfoTbl  , msp_ShtInfoHdlrTbl   },				// Index No.= 9: メッセージ 【Sheet Information】
	{ msp_ShtActReqTbl, msp_ShtActReqHdlrTbl },				// Index No.=10: メッセージ 【Sheet Action Request】
	{ msp_ShtActResTbl, msp_ShtActResHdlrTbl },				// Index No.=11: メッセージ 【Sheet Action Response】
	{ msp_ShtEvtTbl   , msp_ShtEvtHdlrTbl    },				// Index No.=12: メッセージ 【Sheet Event Notify】
	{ msp_DiagEvtTbl  , msp_DiagHdlrTbl      }				// Index No.=13: メッセージ 【Diagnostics】
} ;

// --- ポート・エリア情報テーブル ---
extern const PortAreaInfo msp_PortAreaInfo[][2] = {
	{ { 0, 17 }, { 17, 17 } }								// Port Area ID = 0: ON×17 / OFF×17
} ;

// --- センサーエッジ⇒イベント変換表 ---
extern const SensorEventInfo msp_SensorEdgeEvent[] = {
	 0 ,													// Port Area ID = 0 / Bit 0 ON  ⇒ Event No. = 0x0400
	 1 ,													// Port Area ID = 0 / Bit 1 ON  ⇒ Event No. = 0x0401
	 2 ,													// Port Area ID = 0 / Bit 2 ON  ⇒ Event No. = 0x0402
	 3 ,													// Port Area ID = 0 / Bit 3 ON  ⇒ Event No. = 0x0403
	 4 ,													// Port Area ID = 0 / Bit 4 ON  ⇒ Event No. = 0x0404
	 5 ,													// Port Area ID = 0 / Bit 5 ON  ⇒ Event No. = 0x0405
	 6 ,													// Port Area ID = 0 / Bit 6 ON  ⇒ Event No. = 0x0406
	 7 ,													// Port Area ID = 0 / Bit 7 ON  ⇒ Event No. = 0x0407
	 8 ,													// Port Area ID = 0 / Bit 8 ON  ⇒ Event No. = 0x0408
	 9 ,													// Port Area ID = 0 / Bit 9 ON  ⇒ Event No. = 0x0409
	10 ,													// Port Area ID = 0 / Bit10 ON  ⇒ Event No. = 0x040A
	11 ,													// Port Area ID = 0 / Bit11 ON  ⇒ Event No. = 0x040B
	12 ,													// Port Area ID = 0 / Bit12 ON  ⇒ Event No. = 0x040C
	13 ,													// Port Area ID = 0 / Bit13 ON  ⇒ Event No. = 0x040D
	14 ,													// Port Area ID = 0 / Bit14 ON  ⇒ Event No. = 0x040E
	15 ,													// Port Area ID = 0 / Bit15 ON  ⇒ Event No. = 0x040F
	16 ,													// Port Area ID = 0 / Bit16 ON  ⇒ Event No. = 0x0410
	 0 ,													// Port Area ID = 0 / Bit 0 OFF ⇒ Event No. = 0x0411
	 1 ,													// Port Area ID = 0 / Bit 1 OFF ⇒ Event No. = 0x0412
	 2 ,													// Port Area ID = 0 / Bit 2 OFF ⇒ Event No. = 0x0413
	 3 ,													// Port Area ID = 0 / Bit 3 OFF ⇒ Event No. = 0x0414
	 4 ,													// Port Area ID = 0 / Bit 4 OFF ⇒ Event No. = 0x0415
	 5 ,													// Port Area ID = 0 / Bit 5 OFF ⇒ Event No. = 0x0416
	 6 ,													// Port Area ID = 0 / Bit 6 OFF ⇒ Event No. = 0x0417
	 7 ,													// Port Area ID = 0 / Bit 7 OFF ⇒ Event No. = 0x0418
	 8 ,													// Port Area ID = 0 / Bit 8 OFF ⇒ Event No. = 0x0419
	 9 ,													// Port Area ID = 0 / Bit 9 OFF ⇒ Event No. = 0x041A
	10 ,													// Port Area ID = 0 / Bit10 OFF ⇒ Event No. = 0x041B
	11 ,													// Port Area ID = 0 / Bit11 OFF ⇒ Event No. = 0x041C
	12 ,													// Port Area ID = 0 / Bit12 OFF ⇒ Event No. = 0x041D
	13 ,													// Port Area ID = 0 / Bit13 OFF ⇒ Event No. = 0x041E
	14 ,													// Port Area ID = 0 / Bit14 OFF ⇒ Event No. = 0x041F
	15 ,													// Port Area ID = 0 / Bit15 OFF ⇒ Event No. = 0x0420
	16														// Port Area ID = 0 / Bit16 OFF ⇒ Event No. = 0x0421
} ;

// --- センサー･エッジ情報テーブル ---
extern const PortEdgeInfo msp_PortEdgeInfo[] = {
	{ 0, 0, { 0x0001FFFF, 0x0001FFFF } }					// Port Index = 0 : Port Area ID = 0 : Mask [ON] 0x0001FFFF / [OFF] 0x0001FFFF
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
	0x03,													// Device Configuration Request         ⇒ Event No.11
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

// --- サブシステム・ゲートウェイ・スペック ---

extern const GatewaySpec msp_IotGwSpec = {					// 対IOTゲートウェイの仕様記述
	SID_IOT                   ,
	EVT_CDI_CMD               ,
	sizeof(msp_ContCmd2Event) ,
	msp_ContCmd2Event
} ;

// ----- ロケーション･イベント仕様設定 -----
extern const LocationSpec msp_LocationSpec[MSP_NUM_OF_LOC] = {
//	+-----------+---------------------+------------+------+------------------------------+
//	| Triger ID | Timing (x Schedule) | Pattern    | Parm |     Location Event           |
//	+-----------+---------------------+------------+------+------------------------------+
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },     // IOT Exit Sensor Lead
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 },     // IOT Exit Sensor Tail
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 }, // Compile Exit Sensor Lead
	{    0x03FF ,                   0 , 0xFFFFFFFF ,    0 }   // Compile Exit Sensor Tail
} ;

US MSP_GetEventID(US idx, US num) { return  ((idx << 10)|(num & 0x03FF)) ; }                // Event変換マクロ： Index,Number ⇒ ID変換
US MSP_GetEventIndex(US id) { return  (id >> 10) ; }                                        // Event変換マクロ： ID ⇒ Index変換
US MSP_GetEventNumber(US id) { return  (id & 0x03FF) ; }                                    // Event変換マクロ： ID ⇒ Number変換

// **************************************************************  [ MSP_ProductSpec.c : EOF ]  ***************************************************************
