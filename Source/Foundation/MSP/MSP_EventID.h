// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_EventID.h
/// @author  Tsuyoshi Seto
/// @date    16/03/11
/// @time    16:54:48
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆
//  本ヘッダーファイルはイベント管理リストから自動生成されます。（プロダクト･ユニーク）
// ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆

#ifndef MSP_EventID_h
#define MSP_EventID_h


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

// ========================  <<< イベント定義 >>>  ========================

// ----- メッセージ 【Frame System】 -----														// インデックス番号：0 ⇒ 0x0000-0x03FE
#define	FRM_SYS_START_UP						0x0000											//    0: Start Up
#define	FRM_SYS_COMM_SYNC						0x0001											//    1: Comm Sync
#define	FRM_SYS_COMM_FAIL						0x0002											//    2: Comm Fail

// ----- センサーエッジ -----																	// インデックス番号：1 ⇒ 0x0400-0x07FE
#define	SNR_STAPLE_DOOR_INTERLOCK_SW_ON			0x0400											//    0: ON  @ STAPLE DOOR INTERLOCK SW
#define	SNR_IOT_EXIT_SNR_ON						0x0401											//    1: ON  @ IOT EXIT SNR
#define	SNR_COMPILE_EXIT_SNR_ON					0x0402											//    2: ON  @ COMPILE EXIT SNR
#define	SNR_STAPLE_HOME_SNR_ON					0x0403											//    3: ON  @ STAPLE HOME SNR
#define	SNR_SELF_PRIMING_SNR_ON					0x0404											//    4: ON  @ SELF PRIMING SNR
#define	SNR_LOW_STAPLE_SNR_ON					0x0405											//    5: ON  @ LOW STAPLE SNR
#define	SNR_STACKER_TRAY_HEIGHT2_SNR_ON			0x0406											//    6: ON  @ STACKER TRAY HEIGHT2 SNR
#define	SNR_STACKER_TRAY_HEIGHT1_SNR_ON			0x0407											//    7: ON  @ STACKER TRAY HEIGHT1 SNR
#define	SNR_RIGHT_TAMPER_HOME_SNR_ON			0x0408											//    8: ON  @ RIGHT TAMPER HOME SNR
#define	SNR_LEFT_TAMPER_HOME_SNR_ON				0x0409											//    9: ON  @ LEFT TAMPER HOME SNR
#define	SNR_SET_CLAMP_HOME_SNR_ON				0x040A											//   10: ON  @ SET CLAMP HOME SNR
#define	SNR_REAR_COVER_OPEN_SNR_ON				0x040B											//   11: ON  @ REAR COVER OPEN SNR
#define	SNR_UNIT0_SNR_ON						0x040C											//   12: ON  @ UNIT0 SNR
#define	SNR_STACKER_NO_PAPER_FULL_SNR_ON		0x040D											//   13: ON  @ STACKER NO PAPER FULL SNR
#define	SNR_SUB_PADDLE_HOME_SNR_ON				0x040E											//   14: ON  @ SUB PADDLE HOME SNR
#define	SNR_EJECT_HOME_SNR_ON					0x040F											//   15: ON  @ EJECT HOME SNR
#define	SNR_REG_CL_ON_ON						0x0410											//   16: ON  @ REG CL ON
#define	SNR_STAPLE_DOOR_INTERLOCK_SW_OFF		0x0411											//   17: OFF @ STAPLE DOOR INTERLOCK SW
#define	SNR_IOT_EXIT_SNR_OFF					0x0412											//   18: OFF @ IOT EXIT SNR
#define	SNR_COMPILE_EXIT_SNR_OFF				0x0413											//   19: OFF @ COMPILE EXIT SNR
#define	SNR_STAPLE_HOME_SNR_OFF					0x0414											//   20: OFF @ STAPLE HOME SNR
#define	SNR_SELF_PRIMING_SNR_OFF				0x0415											//   21: OFF @ SELF PRIMING SNR
#define	SNR_LOW_STAPLE_SNR_OFF					0x0416											//   22: OFF @ LOW STAPLE SNR
#define	SNR_STACKER_TRAY_HEIGHT2_SNR_OFF		0x0417											//   23: OFF @ STACKER TRAY HEIGHT2 SNR
#define	SNR_STACKER_TRAY_HEIGHT1_SNR_OFF		0x0418											//   24: OFF @ STACKER TRAY HEIGHT1 SNR
#define	SNR_RIGHT_TAMPER_HOME_SNR_OFF			0x0419											//   25: OFF @ RIGHT TAMPER HOME SNR
#define	SNR_LEFT_TAMPER_HOME_SNR_OFF			0x041A											//   26: OFF @ LEFT TAMPER HOME SNR
#define	SNR_SET_CLAMP_HOME_SNR_OFF				0x041B											//   27: OFF @ SET CLAMP HOME SNR
#define	SNR_REAR_COVER_OPEN_SNR_OFF				0x041C											//   28: OFF @ REAR COVER OPEN SNR
#define	SNR_UNIT0_SNR_OFF						0x041D											//   29: OFF @ UNIT0 SNR
#define	SNR_STACKER_NO_PAPER_FULL_SNR_OFF		0x041E											//   30: OFF @ STACKER NO PAPER FULL SNR
#define	SNR_SUB_PADDLE_HOME_SNR_OFF				0x041F											//   31: OFF @ SUB PADDLE HOME SNR
#define	SNR_EJECT_HOME_SNR_OFF					0x0420											//   32: OFF @ EJECT HOME SNR
#define	SNR_REG_CL_ON_OFF						0x0421											//   33: OFF @ REG CL ON

// ----- ロケーションタイミング -----															// インデックス番号：2 ⇒ 0x0800-0x0BFE
#define	LOC_IOT_EXIT_SENSOR_LEAD				0x0800											//    0: IOT Exit Sensor Lead
#define	LOC_IOT_EXIT_SENSOR_TAIL				0x0801											//    1: IOT Exit Sensor Tail
#define	LOC_COMPILE_EXIT_SENSOR_LEAD			0x0802											//    2: Compile Exit Sensor Lead
#define	LOC_COMPILE_EXIT_SENSOR_TAIL			0x0803											//    3: Compile Exit Sensor Tail

// ----- コマンド＆ステータス【IOT】 -----														// インデックス番号：3 ⇒ 0x0C00-0x0FFE
#define	CDI_SHEET_EXIT								0x0C00										//    0: Sheet Exit
#define	CDI_SHEET_FINISH_REQUEST					0x0C01										//    1: Sheet Finish Request
#define	CDI_CHANGE_SHEET_FINISH_REQUEST				0x0C02										//    2: Change Sheet Finish Request
#define	CDI_CANCEL_SHEET_FINISH_REQUEST				0x0C03										//    3: Cancel Sheet Finish Request
#define	CDI_SET_FINISH_REQUEST						0x0C04										//    4: Set Finish Request
#define	CDI_SELECT_FINISHER_ATTRIBUTE				0x0C05										//    5: Select Finisher Attribute
#define	CDI_STOP_FINISHER							0x0C06										//    6: Stop Finisher
#define	CDI_DEVICE_STATUS_REQUEST					0x0C07										//    7: Device Status Request
#define	CDI_DEVICE_PAUSE_REQUEST					0x0C08										//    8: Device Pause Request
#define	CDI_REQUEST_JAM_STATUS_CHECK				0x0C09										//    9: Request Jam Status Check
#define	CDI_DEVICE_CAPABILITIES_REQUEST				0x0C0A										//   10: Device Capabilities Request
#define	CDI_DEVICE_CONFIGURATION_REQUEST			0x0C0B										//   11: Device Configuration Request
#define	CDI_SET_DEVICE_CONFIGURATION				0x0C0C										//   12: Set Device Configuration
#define	CDI_CHANGE_DEVICE_CONTROLLER_MODE			0x0C0D										//   13: Change Device Controller Mode
#define	CDI_INITIALIZATION_COMPLETE					0x0C0E										//   14: Initialization Complete
#define	CDI_INITIALIZATION_REQUEST					0x0C0F										//   15: Initialization Request
#define	CDI_INITIALIZATION_ACKNOWLEDGE				0x0C10										//   16: Initialization Acknowledge
#define	CDI_POLL_REQUEST							0x0C11										//   17: Poll Request
#define	CDI_POWER_OFF_ANNOUNCEMENT					0x0C12										//   18: Power Off Announcement
#define	CDI_EXECUTE_DIAGNOSTICS						0x0C13										//   19: Execute Diagnostics
#define	CDI_DEVICE_CONTROLLER_ACTION_REQUEST		0x0C14										//   20: Device Controller Action Request
#define	CDI_SYSTEM_CONTROLLER_INFORMATION_NOTIFY	0x0C15										//   21: System Controller Information Notify

// ----- メッセージ 【Finisher State】 -----													// インデックス番号：4 ⇒ 0x1000-0x13FE
#define	EVT_STT_POWER_UP_REQ						0x1000										//    0: Power Up Req
#define	EVT_STT_POWER_UP_RES						0x1001										//    1: Power Up Res
#define	EVT_STT_POWER_ON_NOTIFY						0x1002										//    2: Power On Notify
#define	EVT_STT_POWER_DOWN_REQ						0x1003										//    3: Power Down Req
#define	EVT_STT_POWER_DOWN_RES						0x1004										//    4: Power Down Res
#define	EVT_STT_POWER_OFF_NOTIFY					0x1005										//    5: Power Off Notify
#define	EVT_STT_CHANGE_DIAG_REQ						0x1006										//    6: Change Diag Req
#define	EVT_STT_CHANGE_DIAG_RES						0x1007										//    7: Change Diag Res
#define	EVT_STT_DIAGNOSTIC_NOTIFY					0x1008										//    8: Diagnostic Notify
#define	EVT_STT_CHANGE_NORMAL_REQ					0x1009										//    9: Change Normal Req
#define	EVT_STT_CHANGE_NORMAL_RES					0x100A										//   10: Change Normal Res
#define	EVT_STT_STANDBY_NOTIFY						0x100B										//   11: Standby Notify
#define	EVT_STT_CYCLE_UP_REQ						0x100C										//   12: Cycle Up Req
#define	EVT_STT_CYCLE_UP_RES						0x100D										//   13: Cycle Up Res
#define	EVT_STT_READY_NOTIFY						0x100E										//   14: Ready Notify
#define	EVT_STT_CYCLE_DOWN_REQ						0x100F										//   15: Cycle Down Req
#define	EVT_STT_HARD_DOWN_REQ						0x1010										//   16: Hard Down Req
#define	EVT_STT_PROCESS_DOWN_RES					0x1011										//   17: Process Down Res
#define	EVT_STT_STANDBY_EXIT_NOTIFY					0x1012										//   18: Standby Exit Notify

// ----- メッセージ 【Device Action Request】 -----												// インデックス番号：5 ⇒ 0x1400-0x17FE
#define	EVT_DEV_REQ_POWER_ON_INIT_CMPL				0x1400										//    0: Power On Init CMPL
#define	EVT_DEV_REQ_RESUME_INIT_CMPL				0x1401										//    1: Resume Init CMPL
#define	EVT_DEV_REQ_NORMAL_INIT_CMPL				0x1402										//    2: Normal Init CMPL
#define	EVT_DEV_REQ_JAM_CHECK_TRA					0x1403										//    3: Jam Check TRA
#define	EVT_DEV_REQ_PROCESS_END_TRA					0x1404										//    4: Process End TRA
#define	EVT_DEV_REQ_PROCESS_END_CMPL				0x1405										//    5: Process End CMPL
#define	EVT_DEV_REQ_PROCESS_END_TRAY				0x1406										//    6: Process End TRAY
#define	EVT_DEV_REQ_JAM_CLEAR_ASSIST_TRA			0x1407										//    7: Jam Clear Assist TRA
#define	EVT_DEV_REQ_JAM_CLEAR_ASSIST_CMPL			0x1408										//    8: Jam Clear Assist CMPL

// ----- メッセージ 【Device Action Response】 -----											// インデックス番号：6 ⇒ 0x1800-0x1BFE
#define	EVT_DEV_RES_POWER_ON_INIT_CMPL				0x1800										//    0: Power On Init CMPL
#define	EVT_DEV_RES_RESUME_INIT_CMPL				0x1801										//    1: Resume Init CMPL
#define	EVT_DEV_RES_NORMAL_INIT_CMPL				0x1802										//    2: Normal Init CMPL
#define	EVT_DEV_RES_JAM_CHECK_TRA					0x1803										//    3: Jam Check TRA
#define	EVT_DEV_RES_PROCESS_END_TRA					0x1804										//    4: Process End TRA
#define	EVT_DEV_RES_PROCESS_END_CMPL				0x1805										//    5: Process End CMPL
#define	EVT_DEV_RES_PROCESS_END_TRAY				0x1806										//    6: Process End TRAY
#define	EVT_DEV_RES_JAM_CLEAR_ASSIST_TRA			0x1807										//    7: Jam Clear Assist TRA
#define	EVT_DEV_RES_JAM_CLEAR_ASSIST_CMPL			0x1808										//    8: Jam Clear Assist CMPL

// ----- メッセージ 【Device Event】 -----														// インデックス番号：7 ⇒ 0x1C00-0x1FFE
#define	EVT_DEV_EVT_ANY_COVER_OPEN					0x1C00										//    0: Any Cover Open
#define	EVT_DEV_EVT_ALL_COVER_CLOSE					0x1C01										//    1: All Cover Close
#define	EVT_DEV_EVT_DEV_INIT_COMP					0x1C02										//    2: Dev Init Comp

// ----- メッセージ 【Condition】 -----															// インデックス番号：8 ⇒ 0x2000-0x23FE
#define	EVT_COND_DEV_STTS							0x2000										//    0: Dev Stts
#define	EVT_COND_DEV_COND_FINISHER					0x2001										//    1: Dev Cond Finisher
#define	EVT_COND_DEV_COND_OUT_TRAY					0x2002										//    2: Dev Cond Out Tray
#define	EVT_COND_DEV_COND_STAPLER					0x2003										//    3: Dev Cond Stapler

// ----- メッセージ 【Sheet Information】 -----													// インデックス番号：9 ⇒ 0x2400-0x27FE
#define	EVT_SHT_ENTRY								0x2400										//    0: Entry
#define	EVT_SHT_UPDATE								0x2401										//    1: Update
#define	EVT_SHT_DELIVERED							0x2402										//    2: Delivered
#define	EVT_SHT_ABORTED								0x2403										//    3: Aborted

// ----- メッセージ 【Sheet Action Request】 -----												// インデックス番号：10 ⇒ 0x2800-0x2BFE
#define	EVT_SHT_REQ_COMPILER_SHT_INIT_CMPL			0x2800										//    0: Compiler Sht Init CMPL
#define	EVT_SHT_REQ_STACKER_SHT_INIT_TRAY			0x2801										//    1: Stacker Sht Init TRAY

// ----- メッセージ 【Sheet Action Response】 -----												// インデックス番号：11 ⇒ 0x2C00-0x2FFE
#define	EVT_SHT_RES_COMPILER_SHT_INIT_CMPL			0x2C00										//    0: Compiler Sht Init CMPL
#define	EVT_SHT_RES_STACKER_SHT_INIT_TRAY			0x2C01										//    1: Stacker Sht Init TRAY

// ----- メッセージ 【Sheet Event Notify】 -----												// インデックス番号：12 ⇒ 0x3000-0x33FE
#define	EVT_SHT_EVT_OUT_TRAY_EJECT_FINISH_START			0x3000									//    0: Out Tray Eject Finish Start
#define	EVT_SHT_EVT_OUT_TRAY_EJECT_FINISH_COMP			0x3001									//    1: Out Tray Eject Finish Comp
#define	EVT_SHT_EVT_OUT_TRAY_EJECT_START				0x3002									//    2: Out Tray Eject Start
#define	EVT_SHT_EVT_OUT_TRAY_COMPILE_FINISH_COMP		0x3003									//    3: Out Tray Compile Finish Comp
#define	EVT_SHT_EVT_OUT_TRAY_EJECT_COMP					0x3004									//    4: Out Tray Eject Comp
#define	EVT_SHT_EVT_OUT_TRAY_JAM_OCCURRENCE				0x3005									//    5: Out Tray Jam Occurrence
#define	EVT_SHT_EVT_SHT_INIT_STACKER_JOBSTART			0x3006									//    6: Sht Init Stacker JobStart
#define	EVT_SHT_EVT_SHT_INIT_STACKER_WAITING_POS_START	0x3007									//    7: Sht Init Stacker Waiting_Pos_Start
#define	EVT_SHT_EVT_TRAYJOB_TRIGGER_1					0x3008									//    8: TrayJob_Trigger_1
#define	EVT_SHT_EVT_TRAYJOB_TRIGGER_2					0x3009									//    9: TrayJob_Trigger_2

// ----- メッセージ 【Diagnostics】 -----														// インデックス番号：13 ⇒ 0x3400-0x37FE
#define	EVT_DIAG_COMPO_CONT_START_REQ					0x3400									//    0: Compo Cont Start Req
#define	EVT_DIAG_COMPO_CONT_START						0x3401									//    1: Compo Cont Start
#define	EVT_DIAG_COMPO_CONT_STOP_REQ					0x3402									//    2: Compo Cont Stop Req
#define	EVT_DIAG_COMPO_CONT_STOP						0x3403									//    3: Compo Cont Stop


// ========================  <<< 共通定義 >>>  ========================

// ----- ロケーション･スケジュール数 -----
#define	MSP_NUM_OF_SCH								1											// ロケーション・イベントのスケジュール数


// ========================  <<< モジュールID定義 >>>  ========================

#define	MID_NON										0x00										// ID: 0 モジュール指定なし（予約番号）
#define	SID_IOT										0x01										// ID: 1 IOT
#define	MID_FRAME									0x02										// ID: 2 Frame
#define	MID_SYS										0x03										// ID: 3 Sys
#define	MID_TRNS									0x04										// ID: 4 Trns
#define	MID_CMPL									0x05										// ID: 5 Cmpl
#define	MID_TRAY									0x06										// ID: 6 Tray
#define	MID_NUM										0x07										// ID: 7 Num


// ========================  <<< イベント型定義 >>>  ========================

// ----- コマンド･イベントのデータ型 -----
typedef struct /*CommandEvent*/ {																// <<< Command/Status Eventの型 >>>
	US	mLength ;																				// Command Length
	UC	mCommand[256] ;																			// Command Packet [MSP_MAX_CMD_SIZE]
} CommandEvent ;

// ----- ロケーション･イベントのデータ型 -----
typedef struct /*LocationEvent*/ {																// <<< Location Timing Eventの型 >>>
	US	mSheetNo ;																				// Location Sheet Number
	UL	mParameter ;																			// Location Event Parameter
} LocationEvent ;


#endif          // MSP_EventID_h

// ****************************************************************  [ MSP_EventID.h : EOF ]  *****************************************************************
