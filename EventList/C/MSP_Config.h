// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_Config.h
/// @author  Tsuyoshi Seto
/// @date    16/03/11
/// @time    16:55:06
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
#define	MSP_NUM_OF_AREA		1																	// ポート･エリアの数

// ----- コマンド/ステータス最大パケットサイズ -----
#define	MSP_MAX_CMD_SIZE	256																	// 受信コマンド（ステータス）の最大パケットサイズ（最大:1024）

// ----- イベント最大パケットサイズ -----
#define	MSP_MAX_MSG_SIZE	258																	// 送受信イベントの最大パケットサイズ（最大:1024）

// ----- ロケーション・イベント数 -----
#define	MSP_NUM_OF_LOC		4																	// ロケーション・イベント数(スケジュール・テーブルの要素数として使用)

// ----- ロケーション・タイマー数 -----
#define	MSP_NUM_OF_TMR		64																	// ロケーション･イベントを生成するために使用する管理タイマー数（最大:255）

// ----- パフォーマンス・ログ履歴数 -----
#define	ISP_NUM_OF_LOG		64																	// パフォーマンス測定用のログ領域確保数（最大:128）

// ----- イベント･インデックス定義 -----
typedef enum /*EEventIndex*/ {																	// <<< Event Index ID >>>	8～62はプロダクトで割り付けを実施
	EVT_FRM_SYS  =  0,																			//  0: メッセージ 【Frame System】				（共通定義）･･･ < 予約番号 >
	EVT_SNR_EDGE =  1,																			//  1: センサーエッジ							（共通定義）･･･ < 予約番号 >
	EVT_LOC_TMNG =  2,																			//  2: ロケーションタイミング					（共通定義）･･･ < 予約番号 >
	EVT_CDI_CMD  =  3,																			//  3: コマンド＆ステータス【IOT】				（共通定義）･･･ < 予約番号 >
	EIDX_STT     =  4,																			//  4: メッセージ 【Finisher State】			（共通定義）･･･ < 予約番号 >
	EIDX_DEV_REQ =  5,																			//  5: メッセージ 【Device Action Request】		（共通定義）･･･ < 予約番号 >
	EIDX_DEV_RES =  6,																			//  6: メッセージ 【Device Action Response】	（共通定義）･･･ < 予約番号 >
	EIDX_DEV_EVT =  7,																			//  7: メッセージ 【Device Event】				（共通定義）･･･ < 予約番号 >
	EIDX_COND    =  8,																			//  8: メッセージ 【Condition】					（共通定義）･･･ < 予約番号 >
	EIDX_SHT     =  9,																			//  9: メッセージ 【Sheet Information】			（共通定義）･･･ < 予約番号 >
	EIDX_SHT_REQ = 10,																			// 10: メッセージ 【Sheet Action Request】		（共通定義）･･･ < 予約番号 >
	EIDX_SHT_RES = 11,																			// 11: メッセージ 【Sheet Action Response】		（共通定義）･･･ < 予約番号 >
	EIDX_SHT_EVT = 12,																			// 12: メッセージ 【Sheet Event Notify】		（共通定義）･･･ < 予約番号 >
	EIDX_DIAG    = 13,																			// 13: メッセージ 【Diagnostics】				（共通定義）･･･ < 予約番号 >
	EVT_TIME_OUT = 63																			// 63: イベントタイマー							（共通定義）･･･ < 予約番号 >
} EEventIndex ;

// ----- イベント･ハンドラー定義 -----															// イベント･ハンドラーとして定義された関数を全て記述する
extern void SYS_StartUpEvent(const UC*, UC) ;
extern void CMPL_StartUpEvent(const UC*, UC) ;
extern void TRNS_StartUpEvent(const UC*, UC) ;
extern void TRAY_StartUpEvent(const UC*, UC) ;
extern void CDI_ComSyncNotify(const UC*, UC) ;
extern void CDI_ComFailNotify(const UC*, UC) ;


extern void CVR_StapleDoorILKSwitchON(const UC*, UC) ;
extern void SHT_IotExitSensorON(const UC*, UC) ;
extern void SHT_CompileExitSensorON(const UC*, UC) ;
extern void CMPL_STPL_HomeSensorON(const UC*, UC) ;
extern void TRAY_STK_Height2SensorON(const UC*, UC) ;
extern void TRAY_STK_Height1SensorON(const UC*, UC) ;
extern void CMPL_TMP_RIGHT_RTHomeSensorON(const UC*, UC) ;
extern void CMPL_TMP_LEFT_LTHomeSensorON(const UC*, UC) ;
extern void CMPL_EJCT_SCLMP_SetClampHomeSensorON(const UC*, UC) ;
extern void CVR_RearCoverSwitchON(const UC*, UC) ;
extern void TRAY_STK_NoPaperFullSensorON(const UC*, UC) ;
extern void CMPL_EJCT_SubPaddleHomeSensorON(const UC*, UC) ;
extern void CMPL_EJCT_BELT_EjecterHomeSensorON(const UC*, UC) ;
extern void CMPL_EJCT_SCLMP_EjecterHomeSensorON(const UC*, UC) ;
extern void TRNS_IOTGATE_SignalON(const UC*, UC) ;
extern void CVR_StapleDoorILKSwitchOFF(const UC*, UC) ;
extern void SHT_IotExitSensorOFF(const UC*, UC) ;
extern void SHT_CompileExitSensorOFF(const UC*, UC) ;
extern void CMPL_STPL_HomeSensorOff(const UC*, UC) ;
extern void TRAY_STK_Height2SensorOFF(const UC*, UC) ;
extern void TRAY_STK_Height1SensorOFF(const UC*, UC) ;
extern void CMPL_TMP_RIGHT_RTHomeSensorOFF(const UC*, UC) ;
extern void CMPL_TMP_LEFT_LTHomeSensorOFF(const UC*, UC) ;
extern void CMPL_EJCT_SCLMP_SetClampHomeSensorOFF(const UC*, UC) ;
extern void CVR_RearCoverSwitchOFF(const UC*, UC) ;
extern void TRAY_STK_NoPaperFullSensorOFF(const UC*, UC) ;
extern void CMPL_EJCT_SubPaddleHomeSensorOFF(const UC*, UC) ;
extern void CMPL_EJCT_BELT_EjecterHomeSensorOFF(const UC*, UC) ;
extern void CMPL_EJCT_SCLMP_EjecterHomeSensorOFF(const UC*, UC) ;
extern void TRNS_IOTGATE_SignalOFF(const UC*, UC) ;


extern void TRNS_IotExitLocationLead(const UC*, UC) ;
extern void CMPL_CompileExitSensorLeadEvent(const UC*, UC) ;
extern void TRNS_CompileExitLocationLead(const UC*, UC) ;
extern void CMPL_CompileExitSensorTailEvent(const UC*, UC) ;
extern void TRNS_CompileExitLocationTail(const UC*, UC) ;


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


extern void TRNS_PowerUpRequestEvent(const UC*, UC) ;
extern void TRAY_PowerUpRequestEvent(const UC*, UC) ;
extern void CMPL_PowerUpReqEvent(const UC*, UC) ;
extern void SYS_PowerUpResponseEvent(const UC*, UC) ;
extern void TRNS_PowerDownRequestEvent(const UC*, UC) ;
extern void TRAY_PowerDownRequestEvent(const UC*, UC) ;
extern void CMPL_PowerDownReqEvent(const UC*, UC) ;
extern void SYS_PowerDownResponseEvent(const UC*, UC) ;
extern void TRNS_ChangeDiagRequestEvent(const UC*, UC) ;
extern void TRAY_ChangeDiagRequestEvent(const UC*, UC) ;
extern void CMPL_ChangeDiagReqEvent(const UC*, UC) ;
extern void SYS_ChangeDiagResponseEvent(const UC*, UC) ;
extern void TRNS_ChangeNormalRequestEvent(const UC*, UC) ;
extern void TRAY_ChangeNormalRequestEvent(const UC*, UC) ;
extern void CMPL_ChangeNormalReqEvent(const UC*, UC) ;
extern void SYS_ChangeNormalResponseEvent(const UC*, UC) ;
extern void CMPL_StandbyNotifyEvent(const UC*, UC) ;
extern void TRNS_CycleUpRequestEvent(const UC*, UC) ;
extern void TRAY_CycleUpRequestEvent(const UC*, UC) ;
extern void CMPL_CycleUpReqEvent(const UC*, UC) ;
extern void SYS_CycleUpResponseEvent(const UC*, UC) ;
extern void TRNS_CycleDownRequestEvent(const UC*, UC) ;
extern void TRAY_CycleDownRequestEvent(const UC*, UC) ;
extern void CMPL_CycleDownReqEvent(const UC*, UC) ;
extern void TRNS_HardDownRequestEvent(const UC*, UC) ;
extern void TRAY_HardDownRequestEvent(const UC*, UC) ;
extern void CMPL_HardDownReqEvent(const UC*, UC) ;
extern void SYS_ProcessDownResponseEvent(const UC*, UC) ;
extern void CMPL_StandbyExitNotifyEvent(const UC*, UC) ;
extern void TRAY_StandbyExitNotifyEvent(const UC*, UC) ;


extern void CMPL_DevActReqPowerOnInitEvent(const UC*, UC) ;
extern void CMPL_DevActReqResumeInitEvent(const UC*, UC) ;
extern void CMPL_DevActReqNormalInitEvent(const UC*, UC) ;
extern void TRNS_DeviceActionRequestJamCheck(const UC*, UC) ;
extern void TRNS_DeviceActionRequestProcessEnd(const UC*, UC) ;
extern void CMPL_DevActReqProcessEndEvent(const UC*, UC) ;
extern void TRAY_DeviceActionRequestProcessEnd(const UC*, UC) ;
extern void TRNS_DeviceActionRequestJamClearAssist(const UC*, UC) ;
extern void CMPL_DevActReqJamClearAssistEvent(const UC*, UC) ;


extern void SYS_DeviceActionResponseEventPowerOnInitializeCMPL(const UC*, UC) ;
extern void SYS_DeviceActionResponseEventPowerResumeInitializeCMPL(const UC*, UC) ;
extern void SYS_DeviceActionResponseEventNormalInitializeCMPL(const UC*, UC) ;
extern void SYS_DeviceActionResponseEventJamCheckTRNS(const UC*, UC) ;
extern void SYS_DeviceActionResponseEventProcessEndActionTRNS(const UC*, UC) ;
extern void SYS_DeviceActionResponseEventProcessEndActionCMPL(const UC*, UC) ;
extern void SYS_DeviceActionResponseEventProcessEndActionTRAY(const UC*, UC) ;
extern void SYS_DeviceActionResponseEventJamClearAssistInitializeTRNS(const UC*, UC) ;
extern void SYS_DeviceActionResponseEventJamClearAssistInitializeCMPL(const UC*, UC) ;


extern void TRNS_DeviceEventAnyCoverOpen(const UC*, UC) ;
extern void TRAY_DeviceEventAnyCoverOpen(const UC*, UC) ;
extern void CMPL_DevEvtAnyCoverOpenEvent(const UC*, UC) ;
extern void TRNS_DeviceEventAllCoverClose(const UC*, UC) ;
extern void TRAY_DeviceEventAllCoverClose(const UC*, UC) ;
extern void CMPL_DevEvtAllCoverCloseEvent(const UC*, UC) ;
extern void CMPL_DevEvtDevInitCompEvent(const UC*, UC) ;


extern void SYS_DeviceStatusNotifyEvent(const UC*, UC) ;


extern void CMPL_SheetInformationEventUpdate(const UC*, UC) ;
extern void CMPL_SheetInfomationEventDelivered(const UC*, UC) ;
extern void TRNS_SheetAbortedNotify(const UC*, UC) ;
extern void CMPL_SheetInformationEventAbort(const UC*, UC) ;


extern void CMPL_SheetActionRequestEventCompilerShtInitCMPL(const UC*, UC) ;
extern void TRAY_SheetActionRequestEventStackerShtInitTRAY(const UC*, UC) ;


extern void SHT_SheetActionResponseEventCompilerShtInitCMPL(const UC*, UC) ;
extern void SHT_SheetActionResponseEventStackerShtInitTRAY(const UC*, UC) ;


extern void SHT_SheetEventNotifyEventOutTrayEjectFinishStart(const UC*, UC) ;
extern void SHT_SheetEventNotifyEventOutTrayEjectFinishComp(const UC*, UC) ;
extern void SHT_SheetEventNotifyEventOutTrayEjectStart(const UC*, UC) ;
extern void TRAY_SheetEventNotifyEventOutTrayEjectStart(const UC*, UC) ;
extern void SHT_SheetEventNotifyEventOutTrayCompileFinishComp(const UC*, UC) ;
extern void SHT_SheetEventNotifyEventOutTrayEjectComp(const UC*, UC) ;
extern void JAM_SheetEventNotifyEventOutTrayJamOccurrence(const UC*, UC) ;
extern void TRAY_SheetEventNotifyShtInitStackerJobStart(const UC*, UC) ;
extern void TRAY_SheetEventNotifyShtInitStackerWaitingPositionStart(const UC*, UC) ;
extern void TRAY_SheetEventNotifyTrayJobTrigger_1(const UC*, UC) ;
extern void TRAY_SheetEventNotifyTrayJobTrigger_2(const UC*, UC) ;


extern void TRNS_ComponentControlStartReq(const UC*, UC) ;
extern void TRAY_ComponentControlStartReq(const UC*, UC) ;
extern void CMPL_ComponentControlStartReq(const UC*, UC) ;
extern void DIAG_ComponentControlStartNotifyEvent(const UC*, UC) ;
extern void TRNS_ComponentControlStopReq(const UC*, UC) ;
extern void TRAY_ComponentControlStopReq(const UC*, UC) ;
extern void CMPL_ComponentControlStopReq(const UC*, UC) ;
extern void DIAG_ComponentControlStopNotifyEvent(const UC*, UC) ;


#endif          // MSP_Config_h

// ****************************************************************  [ MSP_Config.h : EOF ]  *****************************************************************
