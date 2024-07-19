// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_EventID.h
/// @author  Tsuyoshi Seto
/// @date    14/04/02
/// @time    16:36:50
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

// ----- センサーエッジ -----																	// インデックス番号：1 ⇒ 0x0400-0x07FE

// ----- ロケーションタイミング -----															// インデックス番号：2 ⇒ 0x0800-0x0BFE
#define	LOC_IOT_EXIT_SENSOR_ON						0x0800										//    0: IOT Exit Sensor ON
#define	LOC_IOT_EXIT_SENSOR_OFF						0x0801										//    1: IOT Exit Sensor OFF
#define	LOC_H_TRA_EXIT_SENSOR_ON					0x0802										//    2: H Tra Exit Sensor ON
#define	LOC_H_TRA_EXIT_SENSOR_OFF					0x0803										//    3: H Tra Exit Sensor OFF
#define	LOC_V_TRA_EXIT_SENSOR_ON					0x0804										//    4: V Tra Exit Sensor ON
#define	LOC_V_TRA_EXIT_SENSOR_OFF					0x0805										//    5: V Tra Exit Sensor OFF
#define	LOC_FOLDER_ENTRANCE_SENSOR_ON				0x0806										//    6: Folder Entrance Sensor ON
#define	LOC_FOLDER_ENTRANCE_SENSOR_OFF				0x0807										//    7: Folder Entrance Sensor OFF
#define	LOC_FOLDER_PATH_SENSOR_1_ON					0x0808										//    8: Folder Path Sensor 1 ON
#define	LOC_FOLDER_PATH_SENSOR_1_OFF				0x0809										//    9: Folder Path Sensor 1 OFF
#define	LOC_FOLDER_PATH_SENSOR_2_ON					0x080A										//   10: Folder Path Sensor 2 ON
#define	LOC_FOLDER_PATH_SENSOR_2_OFF				0x080B										//   11: Folder Path Sensor 2 OFF
#define	LOC_FOLDER_PATH_SENSOR_3_ON					0x080C										//   12: Folder Path Sensor 3 ON
#define	LOC_FOLDER_PATH_SENSOR_3_OFF				0x080D										//   13: Folder Path Sensor 3 OFF
#define	LOC_FOLDER_PATH_SENSOR_4_ON					0x080E										//   14: Folder Path Sensor 4 ON
#define	LOC_FOLDER_PATH_SENSOR_4_OFF				0x080F										//   15: Folder Path Sensor 4 OFF
#define	LOC_FOLDER_EXIT_SENSOR_ON					0x0810										//   16: Folder Exit Sensor ON
#define	LOC_FOLDER_EXIT_SENSOR_OFF					0x0811										//   17: Folder Exit Sensor OFF
#define	LOC_S_TRA_EXIT_SENSOR_ON					0x0812										//   18: S Tra Exit Sensor ON
#define	LOC_S_TRA_EXIT_SENSOR_OFF					0x0813										//   19: S Tra Exit Sensor OFF
#define	LOC_PUNCH_IN_SENSOR_ON						0x0814										//   20: Punch In Sensor ON
#define	LOC_PUNCH_IN_SENSOR_OFF						0x0815										//   21: Punch In Sensor OFF
#define	LOC_FINISHER_IN_SENSOR_ON					0x0816										//   22: Finisher In Sensor ON
#define	LOC_XXX_HOME_POSITION_START					0x0817										//   23: XXX Home Position Start
#define	LOC_FINISHER_IN_SENSOR_OFF					0x0818										//   24: Finisher In Sensor OFF
#define	LOC_TOP_TRAY_EXIT_SENSOR_ON					0x0819										//   25: Top Tray Exit Sensor ON
#define	LOC_TOP_TRAY_EXIT_SENSOR_OFF				0x081A										//   26: Top Tray Exit Sensor OFF
#define	LOC_COMPILE_EXIT_SENSOR_ON					0x081B										//   27: Compile Exit Sensor ON
#define	LOC_COMPILE_EXIT_SENSOR_OFF					0x081C										//   28: Compile Exit Sensor OFF
#define	LOC_COMPILE_NO_PAPER_SENSOR_ON				0x081D										//   29: Compile No Paper Sensor ON
#define	LOC_COMPILE_NO_PAPER_SENSOR_OFF				0x081E										//   30: Compile No Paper Sensor OFF
#define	LOC_EJECT_SENSOR_ON							0x081F										//   31: Eject Sensor ON
#define	LOC_EJECT_SENSOR_OFF						0x0820										//   32: Eject Sensor OFF
#define	LOC_BOOKLET_IN_SENSOR_ON					0x0821										//   33: Booklet In Sensor ON
#define	LOC_BOOKLET_IN_SENSOR_OFF					0x0822										//   34: Booklet In Sensor OFF
#define	LOC_BOOKLET_COMPILE_NO_PAPER_SENSOR_ON		0x0823										//   35: Booklet Compile No Paper Sensor ON
#define	LOC_BOOKLET_COMPILE_NO_PAPER_SENSOR_OFF		0x0824										//   36: Booklet Compile No Paper Sensor OFF
#define	LOC_BOOKLET_COMPILE_NO_PAPER_SENSOR2_ON		0x0825										//   37: Booklet Compile No Paper Sensor2 ON
#define	LOC_BOOKLET_COMPILE_NO_PAPER_SENSOR2_OFF	0x0826										//   38: Booklet Compile No Paper Sensor2 OFF
#define	LOC_BOOKLET_EXIT_SENSOR_ON					0x0827										//   39: Booklet Exit Sensor ON
#define	LOC_BOOKLET_EXIT_SENSOR_OFF					0x0828										//   40: Booklet Exit Sensor OFF
#define	LOC_TRIMMER_IN_SENSOR_ON					0x0829										//   41: Trimmer In Sensor ON
#define	LOC_TRIMMER_IN_SENSOR_OFF					0x082A										//   42: Trimmer In Sensor OFF
#define	LOC_TRIMMER_PATH_SENSOR1_ON					0x082B										//   43: Trimmer Path Sensor1 ON
#define	LOC_TRIMMER_PATH_SENSOR1_OFF				0x082C										//   44: Trimmer Path Sensor1 OFF
#define	LOC_TRIMMER_PATH_SENSOR2_ON					0x082D										//   45: Trimmer Path Sensor2 ON
#define	LOC_TRIMMER_PATH_SENSOR2_OFF				0x082E										//   46: Trimmer Path Sensor2 OFF
#define	LOC_TRIMMER_PATH_SENSOR3_ON					0x082F										//   47: Trimmer Path Sensor3 ON
#define	LOC_TRIMMER_PATH_SENSOR3_OFF				0x0830										//   48: Trimmer Path Sensor3 OFF
#define	LOC_TRIMMER_PATH_SENSOR3_1_ON				0x0831										//   49: Trimmer Path Sensor3_1 ON
#define	LOC_TRIMMER_PATH_SENSOR3_1_OFF				0x0832										//   50: Trimmer Path Sensor3_1 OFF
#define	LOC_PFIM_TP_IN_SENSOR_ON					0x0833										//   51: PFIM Tp In Sensor ON
#define	LOC_PFIM_TP_IN_SENSOR_OFF					0x0834										//   52: PFIM Tp In Sensor OFF

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

// ----- コマンド＆ステータス【TCBM】 -----														// インデックス番号：4 ⇒ 0x1000-0x13FE
#define	TCBM_READY_FOR_NEXT_SHEET					0x1000										//    0: Ready For Next Sheet
#define	TCBM_SHEET_DELIVERED_TO_OUTPUT				0x1001										//    1: Sheet Delivered To Output
#define	TCBM_SET_FINISHED_AT_OUTPUT					0x1002										//    2: Set Finished At Output
#define	TCBM_SHEET_FED								0x1003										//    3: Sheet Fed
#define	TCBM_ABNORMAL_FINISHING						0x1004										//    4: Abnormal Finishing
#define	TCBM_REPORT_PAUSE_STATUS					0x1005										//    5: Report Pause Status
#define	TCBM_DEVICE_STATUS_UPDATE					0x1006										//    6: Device Status Update
#define	TCBM_JAM_ZONE_UPDATE						0x1007										//    7: Jam Zone Update
#define	TCBM_FEED_ABORTED							0x1008										//    8: Feed Aborted
#define	TCBM_SET_RECOVERY_REQUEST					0x1009										//    9: Set Recovery Request
#define	TCBM_DEVICE_CAPABILITIES_UPDATE				0x100A										//   10: Device Capabilities Update
#define	TCBM_DEVICE_CONFIGURATION_UPDATE			0x100B										//   11: Device Configuration Update
#define	TCBM_DEVICE_CONTROLLER_STATE_UPDATE			0x100C										//   12: Device Controller State Update
#define	TCBM_DEVICE_CONTROLLER_MODE_UPDATE			0x100D										//   13: Device Controller Mode Update
#define	TCBM_INITIALIZATION_REQUEST					0x100E										//   14: Initialization Request
#define	TCBM_INITIALIZATION_ACKNOWLEDGE				0x100F										//   15: Initialization Acknowledge
#define	TCBM_POWER_OFF_ACKNOWLEDGE					0x1010										//   16: Power Off Acknowledge
#define	TCBM_DIAGNOSTICS_RESPONSE					0x1011										//   17: Diagnostics Response
#define	TCBM_DEVICE_CONTROLLER_INFORMATION_NOTIFY	0x1012										//   18: Device Controller Information Notify

// ----- メッセージ 【Subsystem State】 -----													// インデックス番号：5 ⇒ 0x1400-0x17FE
#define	SUBSYS_STT_POWER_UP							0x1400										//    0: Power Up
#define	SUBSYS_STT_POWER_ON							0x1401										//    1: Power On
#define	SUBSYS_STT_CHANGE_MODE						0x1402										//    2: Change Mode
#define	SUBSYS_STT_STANDBY							0x1403										//    3: Standby
#define	SUBSYS_STT_DIAGNOSTICS						0x1404										//    4: Diagnostics
#define	SUBSYS_STT_POWER_DOWN						0x1405										//    5: Power Down
#define	SUBSYS_STT_POWER_OFF						0x1406										//    6: Power Off

// ----- メッセージ 【Device Action Request】 -----												// インデックス番号：6 ⇒ 0x1800-0x1BFE
#define	ACT_REQ_CYCLE_DOWN							0x1800										//    0: Cycle Down
#define	ACT_REQ_HARD_DOWN							0x1801										//    1: Hard Down
#define	ACT_REQ_POWER_ON_INIT_BOOKLET				0x1802										//    2: Power On Init Booklet
#define	ACT_REQ_POWER_ON_INIT_COMPILER				0x1803										//    3: Power On Init Compiler
#define	ACT_REQ_POWER_ON_INIT_FOLDER				0x1804										//    4: Power On Init Folder
#define	ACT_REQ_POWER_ON_INIT_PUNCH					0x1805										//    5: Power On Init Punch
#define	ACT_REQ_POWER_ON_INIT_STACKER				0x1806										//    6: Power On Init Stacker
#define	ACT_REQ_POWER_ON_INIT_STATIC_JAM			0x1807										//    7: Power On Init Static JAM
#define	ACT_REQ_POWER_ON_INIT_TSQF					0x1808										//    8: Power On Init TSQF
#define	ACT_REQ_RESUME_INIT_COMPILER				0x1809										//    9: Resume Init Compiler
#define	ACT_REQ_RESUME_INIT_PUNCH					0x180A										//   10: Resume Init Punch
#define	ACT_REQ_RESUME_INIT_STATIC_JAM				0x180B										//   11: Resume Init Static JAM
#define	ACT_REQ_CLOSE_INIT_BOOKLET					0x180C										//   12: Close Init Booklet
#define	ACT_REQ_CLOSE_INIT_COMPILER					0x180D										//   13: Close Init Compiler
#define	ACT_REQ_CLOSE_INIT_PUNCH					0x180E										//   14: Close Init Punch
#define	ACT_REQ_CLOSE_INIT_STACKER					0x180F										//   15: Close Init Stacker
#define	ACT_REQ_CLOSE_INIT_STATIC_JAM				0x1810										//   16: Close Init Static JAM
#define	ACT_REQ_CLOSE_INIT_TSQF						0x1811										//   17: Close Init TSQF
#define	ACT_REQ_SHEET_INIT_BOOKLET					0x1812										//   18: Sheet Init Booklet
#define	ACT_REQ_SHEET_INIT_COMPILER_SET_EJECT		0x1813										//   19: Sheet Init Compiler Set Eject
#define	ACT_REQ_SHEET_INIT_COMPILER_SHEET_EJECT		0x1814										//   20: Sheet Init Compiler Sheet Eject
#define	ACT_REQ_SHEET_INIT_FOLDER					0x1815										//   21: Sheet Init Folder
#define	ACT_REQ_SHEET_INIT_PUNCH					0x1816										//   22: Sheet Init Punch
#define	ACT_REQ_SHEET_INIT_STACKER					0x1817										//   23: Sheet Init Stacker
#define	ACT_REQ_SHEET_INIT_TOP_TRAY					0x1818										//   24: Sheet Init Top Tray
#define	ACT_REQ_SHEET_INIT_TSQF						0x1819										//   25: Sheet Init TSQF
#define	ACT_REQ_PROCESS_END_BOOKLET					0x181A										//   26: Process End Booklet
#define	ACT_REQ_PROCESS_END_BOOKLET_TRAY			0x181B										//   27: Process End Booklet Tray
#define	ACT_REQ_PROCESS_END_COMPILER				0x181C										//   28: Process End Compiler
#define	ACT_REQ_PROCESS_END_FOLDER					0x181D										//   29: Process End Folder
#define	ACT_REQ_PROCESS_END_MAIN_TRANSPORT			0x181E										//   30: Process End Main Transport
#define	ACT_REQ_PROCESS_END_PUNCH					0x181F										//   31: Process End Punch
#define	ACT_REQ_PROCESS_END_STACKER					0x1820										//   32: Process End Stacker
#define	ACT_REQ_COMPONENT_CONTROL_START				0x1821										//   33: Component Control Start
#define	ACT_REQ_COMPONENT_CONTROL_STOP				0x1822										//   34: Component Control Stop
#define	ACT_REQ_ENVELOPE_TRAY_OPEN					0x1823										//   35: Envelope Tray Open
#define	ACT_REQ_COMPILE_SHELF_PUSH					0x1824										//   36: Compile Shelf Push
#define	ACT_REQ_SHEET_INIT_SHELF_PUSH				0x1825										//   37: Sheet Init Shelf Push
#define	ACT_REQ_POWER_ON_INIT_SHELF_PULL			0x1826										//   38: Power On Init Shelf Pull
#define	ACT_REQ_CLOSE_INIT_SHELF_PULL				0x1827										//   39: Close Init Shelf Pull
#define	ACT_REQ_COMPILER_FINISHING_SHELF_PULL		0x1828										//   40: Compiler Finishing Shelf Pull
#define	ACT_REQ_POWER_ON_INIT_TAMPER_HOME			0x1829										//   41: Power On Init Tamper Home
#define	ACT_REQ_SHEET_INIT_TAMPER_HOME				0x182A										//   42: Sheet Init Tamper Home
#define	ACT_REQ_PROCESS_END_TAMPER_HOME				0x182B										//   43: Process End Tamper Home
#define	ACT_REQ_SHEET_INIT_TAMPER_SIZE_POS			0x182C										//   44: Sheet Init Tamper Size Pos
#define	ACT_REQ_TAMPER_NORMAL_TAMPING				0x182D										//   45: Tamper Normal Tamping
#define	ACT_REQ_TAMPER_SET_LAST_TAMPING				0x182E										//   46: Tamper Set Last Tamping
#define	ACT_REQ_TAMPER_PREP_OFFSET					0x182F										//   47: Tamper Prep Offset
#define	ACT_REQ_TAMPER_EXEC_OFFSET					0x1830										//   48: Tamper Exec Offset
#define	ACT_REQ_TAMPER_RETURN_1						0x1831										//   49: Tamper Return 1
#define	ACT_REQ_TAMPER_RETURN_2						0x1832										//   50: Tamper Return 2
#define	ACT_REQ_TAMPER_ESCAPE						0x1833										//   51: Tamper Escape
#define	ACT_REQ_POWER_ON_INIT_STAPLE_HEAD_HOME		0x1834										//   52: Power On Init Staple Head Home
#define	ACT_REQ_CLOSE_INIT_STAPLE_HEAD_HOME			0x1835										//   53: Close Init Staple Head Home
#define	ACT_REQ_POWER_ON_INIT_STAPLE_ENDWALL_INIT	0x1836										//   54: Power On Init Staple Endwall Init
#define	ACT_REQ_RESUME_INIT_STAPLE_ENDWALL_INIT		0x1837										//   55: Resume Init Staple Endwall Init
#define	ACT_REQ_CLOSE_INIT_STAPLE_ENDWALL_INIT		0x1838										//   56: Close Init Staple Endwall Init
#define	ACT_REQ_POWER_ON_INIT_STAPLE_FRONT_HOME		0x1839										//   57: Power On Init Staple Front Home
#define	ACT_REQ_RESUME_INIT_STAPLE_FRONT_HOME		0x183A										//   58: Resume Init Staple Front Home
#define	ACT_REQ_CLOSE_INIT_STAPLE_FRONT_HOME		0x183B										//   59: Close Init Staple Front Home
#define	ACT_REQ_PROCESS_END_STAPLE_FRONT_HOME		0x183C										//   60: Process End Staple Front Home
#define	ACT_REQ_POWER_ON_INIT_STAPLE_LEAD			0x183D										//   61: Power On Init Staple Lead
#define	ACT_REQ_CLOSE_INIT_STAPLE_LEAD				0x183E										//   62: Close Init Staple Lead
#define	ACT_REQ_STAPLE_POSITIONING					0x183F										//   63: Staple Positioning
#define	ACT_REQ_STAPLE_ENDWALL_OPEN					0x1840										//   64: Staple Endwall Open
#define	ACT_REQ_STAPLE_EXEC							0x1841										//   65: Staple Exec
#define	ACT_REQ_STAPLE_ENDWALL_CLOSE				0x1842										//   66: Staple Endwall Close
#define	ACT_REQ_STAPLE_RETURN						0x1843										//   67: Staple Return
#define	ACT_REQ_POWER_ON_INIT_SUB_PADDLE			0x1844										//   68: Power On Init Sub Paddle
#define	ACT_REQ_SHEET_INIT_SUB_PADDLE				0x1845										//   69: Sheet Init Sub Paddle
#define	ACT_REQ_COVER_CLOSE_INIT_SUB_PADDLE			0x1846										//   70: Cover Close Init Sub Paddle
#define	ACT_REQ_SUB_PADDLE_UP_DOWN					0x1847										//   71: Sub Paddle Up Down
#define	ACT_REQ_SUB_PADDLE_RECEIVE_SHEET			0x1848										//   72: Sub Paddle Receive Sheet
#define	ACT_REQ_PADDLE_MOTOR_START					0x1849										//   73: Paddle Motor Start
#define	ACT_REQ_PADDLE_MOTOR_STOP					0x184A										//   74: Paddle Motor Stop
#define	ACT_REQ_EJECT_CLAMP_DOWN					0x184B										//   75: Eject Clamp Down
#define	ACT_REQ_EJECT_CLAMP_HALF_DOWN				0x184C										//   76: Eject Clamp Half Down
#define	ACT_REQ_EJECT_CLAMP_UP						0x184D										//   77: Eject Clamp Up
#define	ACT_REQ_EJECT_MOTOR_CARRY_IN				0x184E										//   78: Eject Motor Carry In
#define	ACT_REQ_EJECT_MOTOR_SET_EJECT				0x184F										//   79: Eject Motor Set Eject
#define	ACT_REQ_EJECT_MOTOR_SHEET_EJECT				0x1850										//   80: Eject Motor Sheet Eject
#define	ACT_REQ_EJECT_MOTOR_ESCAPE					0x1851										//   81: Eject Motor Escape
#define	ACT_REQ_SET_CLAMP_STICK						0x1852										//   82: Set Clamp Stick
#define	ACT_REQ_TAMPER_HOLD_SHEET					0x1853										//   83: Tamper Hold Sheet

// ----- メッセージ 【Device Action Response】 -----											// インデックス番号：7 ⇒ 0x1C00-0x1FFE
#define	ACT_RES_CYCLE_DOWN							0x1C00										//    0: Cycle Down
#define	ACT_RES_HARD_DOWN							0x1C01										//    1: Hard Down
#define	ACT_RES_POWER_ON_INIT_BOOKLET				0x1C02										//    2: Power On Init Booklet
#define	ACT_RES_POWER_ON_INIT_COMPILER				0x1C03										//    3: Power On Init Compiler
#define	ACT_RES_POWER_ON_INIT_FOLDER				0x1C04										//    4: Power On Init Folder
#define	ACT_RES_POWER_ON_INIT_PUNCH					0x1C05										//    5: Power On Init Punch
#define	ACT_RES_POWER_ON_INIT_STACKER				0x1C06										//    6: Power On Init Stacker
#define	ACT_RES_POWER_ON_INIT_STATIC_JAM			0x1C07										//    7: Power On Init Static JAM
#define	ACT_RES_POWER_ON_INIT_TSQF					0x1C08										//    8: Power On Init TSQF
#define	ACT_RES_RESUME_INIT_COMPILER				0x1C09										//    9: Resume Init Compiler
#define	ACT_RES_RESUME_INIT_PUNCH					0x1C0A										//   10: Resume Init Punch
#define	ACT_RES_RESUME_INIT_STATIC_JAM				0x1C0B										//   11: Resume Init Static JAM
#define	ACT_RES_CLOSE_INIT_BOOKLET					0x1C0C										//   12: Close Init Booklet
#define	ACT_RES_CLOSE_INIT_COMPILER					0x1C0D										//   13: Close Init Compiler
#define	ACT_RES_CLOSE_INIT_PUNCH					0x1C0E										//   14: Close Init Punch
#define	ACT_RES_CLOSE_INIT_STACKER					0x1C0F										//   15: Close Init Stacker
#define	ACT_RES_CLOSE_INIT_STATIC_JAM				0x1C10										//   16: Close Init Static JAM
#define	ACT_RES_CLOSE_INIT_TSQF						0x1C11										//   17: Close Init TSQF
#define	ACT_RES_SHEET_INIT_BOOKLET					0x1C12										//   18: Sheet Init Booklet
#define	ACT_RES_SHEET_INIT_COMPILER_SET_EJECT		0x1C13										//   19: Sheet Init Compiler Set Eject
#define	ACT_RES_SHEET_INIT_COMPILER_SHEET_EJECT		0x1C14										//   20: Sheet Init Compiler Sheet Eject
#define	ACT_RES_SHEET_INIT_FOLDER					0x1C15										//   21: Sheet Init Folder
#define	ACT_RES_SHEET_INIT_PUNCH					0x1C16										//   22: Sheet Init Punch
#define	ACT_RES_SHEET_INIT_STACKER					0x1C17										//   23: Sheet Init Stacker
#define	ACT_RES_SHEET_INIT_TOP_TRAY					0x1C18										//   24: Sheet Init Top Tray
#define	ACT_RES_SHEET_INIT_TSQF						0x1C19										//   25: Sheet Init TSQF
#define	ACT_RES_PROCESS_END_BOOKLET					0x1C1A										//   26: Process End Booklet
#define	ACT_RES_PROCESS_END_BOOKLET_TRAY			0x1C1B										//   27: Process End Booklet Tray
#define	ACT_RES_PROCESS_END_COMPILER				0x1C1C										//   28: Process End Compiler
#define	ACT_RES_PROCESS_END_FOLDER					0x1C1D										//   29: Process End Folder
#define	ACT_RES_PROCESS_END_MAIN_TRANSPORT			0x1C1E										//   30: Process End Main Transport
#define	ACT_RES_PROCESS_END_PUNCH					0x1C1F										//   31: Process End Punch
#define	ACT_RES_PROCESS_END_STACKER					0x1C20										//   32: Process End Stacker
#define	ACT_RES_COMPONENT_CONTROL_START				0x1C21										//   33: Component Control Start
#define	ACT_RES_COMPONENT_CONTROL_STOP				0x1C22										//   34: Component Control Stop
#define	ACT_RES_ENVELOPE_TRAY_OPEN					0x1C23										//   35: Envelope Tray Open
#define	ACT_RES_SHELF_PUSH							0x1C24										//   36: Shelf Push
#define	ACT_RES_SHELF_PULL							0x1C25										//   37: Shelf Pull
#define	ACT_RES_TAMPER_HOME							0x1C26										//   38: Tamper Home
#define	ACT_RES_TAMPER_SIZE_POS						0x1C27										//   39: Tamper Size Pos
#define	ACT_RES_TAMPER_NORMAL_TAMPING				0x1C28										//   40: Tamper Normal Tamping
#define	ACT_RES_TAMPER_SET_LAST_TAMPING				0x1C29										//   41: Tamper Set Last Tamping
#define	ACT_RES_TAMPER_PREP_OFFSET					0x1C2A										//   42: Tamper Prep Offset
#define	ACT_RES_TAMPER_EXEC_OFFSET					0x1C2B										//   43: Tamper Exec Offset
#define	ACT_RES_TAMPER_RETURN_1						0x1C2C										//   44: Tamper Return 1
#define	ACT_RES_TAMPER_RETURN_2						0x1C2D										//   45: Tamper Return 2
#define	ACT_RES_TAMPER_ESCAPE						0x1C2E										//   46: Tamper Escape
#define	ACT_RES_STAPLE_HEAD_HOME					0x1C2F										//   47: Staple Head Home
#define	ACT_RES_STAPLE_ENDWALL_INIT					0x1C30										//   48: Staple Endwall Init
#define	ACT_RES_STAPLE_FRONT_HOME					0x1C31										//   49: Staple Front Home
#define	ACT_RES_STAPLE_LEAD							0x1C32										//   50: Staple Lead
#define	ACT_RES_STAPLE_POSITIONING					0x1C33										//   51: Staple Positioning
#define	ACT_RES_STAPLE_ENDWALL_OPEN					0x1C34										//   52: Staple Endwall Open
#define	ACT_RES_STAPLE_EXEC							0x1C35										//   53: Staple Exec
#define	ACT_RES_STAPLE_ENDWALL_CLOSE				0x1C36										//   54: Staple Endwall Close
#define	ACT_RES_STAPLE_RETURN						0x1C37										//   55: Staple Return
#define	ACT_RES_POWER_ON_INIT_SUB_PADDLE			0x1C38										//   56: Power On Init Sub Paddle
#define	ACT_RES_SHEET_INIT_SUB_PADDLE				0x1C39										//   57: Sheet Init Sub Paddle
#define	ACT_RES_COVER_CLOSE_INIT_SUB_PADDLE			0x1C3A										//   58: Cover Close Init Sub Paddle
#define	ACT_RES_SUB_PADDLE_UP_DOWN					0x1C3B										//   59: Sub Paddle Up Down
#define	ACT_RES_SUB_PADDLE_RECEIVE_SHEET			0x1C3C										//   60: Sub Paddle Receive Sheet
#define	ACT_RES_PADDLE_MOTOR_START					0x1C3D										//   61: Paddle Motor Start
#define	ACT_RES_PADDLE_MOTOR_STOP					0x1C3E										//   62: Paddle Motor Stop
#define	ACT_RES_EJECT_CLAMP_DOWN					0x1C3F										//   63: Eject Clamp Down
#define	ACT_RES_EJECT_CLAMP_HALF_DOWN				0x1C40										//   64: Eject Clamp Half Down
#define	ACT_RES_EJECT_CLAMP_UP						0x1C41										//   65: Eject Clamp Up
#define	ACT_RES_EJECT_MOTOR_CARRY_IN				0x1C42										//   66: Eject Motor Carry In
#define	ACT_RES_EJECT_MOTOR_SET_EJECT				0x1C43										//   67: Eject Motor Set Eject
#define	ACT_RES_EJECT_MOTOR_SHEET_EJECT				0x1C44										//   68: Eject Motor Sheet Eject
#define	ACT_RES_EJECT_MOTOR_ESCAPE					0x1C45										//   69: Eject Motor Escape
#define	ACT_RES_SET_CLAMP_STICK						0x1C46										//   70: Set Clamp Stick
#define	ACT_RES_TAMPER_HOLD_SHEET					0x1C47										//   71: Tamper Hold Sheet

// ----- メッセージ 【Device Event】 -----														// インデックス番号：8 ⇒ 0x2000-0x23FE
#define	DEV_EVT_INTERLOCK_OPEN						0x2000										//    0: Interlock Open
#define	DEV_EVT_INTERLOCK_CLOSE						0x2001										//    1: Interlock Close
#define	DEV_EVT_24V_CUT								0x2002										//    2: 24V Cut
#define	DEV_EVT_24V_CONNECT							0x2003										//    3: 24V Connect
#define	DEV_EVT_ENVELOPE_BUTTON_PUSH				0x2004										//    4: Envelope Button Push
#define	DEV_EVT_POWER_ON_INIT_COMPLETE				0x2005										//    5: Power On Init Complete
#define	DEV_EVT_RESUME_INIT_COMPLETE				0x2006										//    6: Resume Init Complete
#define	DEV_EVT_CLOSE_INIT_COMPLETE					0x2007										//    7: Close Init Complete
#define	DEV_EVT_PROCESS_END_COMPLETE				0x2008										//    8: Process End Complete
#define	DEV_EVT_CONFIG_FIXED						0x2009										//    9: Config Fixed
#define	DEV_EVT_DEVICE_STATUS_UPDATE				0x200A										//   10: Device Status Update
#define	DEV_EVT_STK_POWER_ON_INIT_STEP1_END			0x200B										//   11: STK Power On Init Step1 End
#define	DEV_EVT_STK_POWER_ON_INIT_STEP2_END			0x200C										//   12: STK Power On Init Step2 End
#define	DEV_EVT_STK_POWER_ON_INIT_STEP3_END			0x200D										//   13: STK Power On Init Step3 End
#define	DEV_EVT_STK_POWER_ON_INIT_STEP4_END			0x200E										//   14: STK Power On Init Step4 End
#define	DEV_EVT_STK_COVER_CLOSE_INIT_STEP1_END		0x200F										//   15: STK Cover Close Init Step1 End
#define	DEV_EVT_STK_CMPL_EXIT_SNR_ON_STEP1_END		0x2010										//   16: STK Cmpl Exit Snr On Step1 End
#define	DEV_EVT_STK_CMPL_EXIT_SNR_ON_STEP2_END		0x2011										//   17: STK Cmpl Exit Snr On Step2 End
#define	DEV_EVT_STK_CMPL_EXIT_SNR_ON_STEP3_END		0x2012										//   18: STK Cmpl Exit Snr On Step3 End
#define	DEV_EVT_STK_CMPL_EXIT_SNR_END				0x2013										//   19: STK Cmpl Exit Snr End
#define	DEV_EVT_TSQF_TRANSPORT_ROLL_INIT			0x2014										//   20: TSQF Transport Roll Init
#define	DEV_EVT_TSQF_SQUARE_INIT					0x2015										//   21: TSQF Square Init
#define	DEV_EVT_TSQF_STABILIZE_SHEET				0x2016										//   22: TSQF Stabilize Sheet
#define	DEV_EVT_TSQF_SQUARE_SHEET					0x2017										//   23: TSQF Square Sheet
#define	DEV_EVT_TSQF_TRANSPORT_SHEET				0x2018										//   24: TSQF Transport Sheet
#define	DEV_EVT_TSQF_TRIMMING_PREPARE				0x2019										//   25: TSQF Trimming Prepare
#define	DEV_EVT_TSQF_CUT_SHEET						0x201A										//   26: TSQF Cut Sheet
#define	DEV_EVT_TSQF_EJECT_SHEET					0x201B										//   27: TSQF Eject Sheet
#define	DEV_EVT_TMP_HOLD_SHEET_RESERVE				0x201C										//   28: TMP Hold Sheet Reserve
#define	DEV_EVT_SUBCPU_DATA_READ_END				0x201D										//   29: SubCPU Data Read End
#define	DEV_EVT_PUNCHING_END						0x201E										//   30: Punching End
#define	DEV_EVT_ENVELOPE_BUTTON_CHECK_PERMIT		0x201F										//   31: Envelope Button Check Permit
#define	DEV_EVT_STAPLE_MODE_FIXED					0x2020										//   32: Staple Mode Fixed
#define	DEV_EVT_FOLDER_RELAY_OFF					0x2021										//   33: Folder Relay Off

// ----- メッセージ 【Sheet Event】 -----														// インデックス番号：9 ⇒ 0x2400-0x27FE
#define	SHT_EVT_ATTRIBUTE_UPDATE					0x2400										//    0: Attribute Update
#define	SHT_EVT_JOB_CANCEL							0x2401										//    1: Job Cancel
#define	SHT_EVT_ABORT								0x2402										//    2: Abort
#define	SHT_EVT_PUNCH_BACK_START					0x2403										//    3: Punch Back Start
#define	SHT_EVT_PUNCH_BACK_COMPLETE					0x2404										//    4: Punch Back Complete
#define	SHT_EVT_SWITCH_BACK_START					0x2405										//    5: Switch Back Start
#define	SHT_EVT_BUFFER_FEED_START					0x2406										//    6: Buffer Feed Start
#define	SHT_EVT_EJECT_START							0x2407										//    7: Eject Start
#define	SHT_EVT_COMPILE_FINISH_START				0x2408										//    8: Compile Finish Start
#define	SHT_EVT_COMPILE_FINISH_COMPLETE				0x2409										//    9: Compile Finish Complete
#define	SHT_EVT_FOLDER_SECFOLD_START				0x240A										//   10: Folder Secfold Start
#define	SHT_EVT_EJECT_FINISH_COMPLETE				0x240B										//   11: Eject Finish Complete
#define	SHT_EVT_STAPLE_CANCEL						0x240C										//   12: Staple Cancel
#define	SHT_EVT_EXIT_START							0x240D										//   13: Exit Start
#define	SHT_EVT_PROCESS_START_SHEET					0x240E										//   14: Process Start Sheet
#define	SHT_EVT_RCV_SHEET_FINISH_REQ				0x240F										//   15: Rcv_Sheet_Finish_Req
#define	SHT_EVT_RCV_SHEET_EXIT						0x2410										//   16: Rcv_Sheet_Exit
#define	SHT_EVT_RCV_SET_FINISH_REQ					0x2411										//   17: Rcv_Set_Finish_Req


// ========================  <<< 共通定義 >>>  ========================

// ----- ロケーション･スケジュール数 -----
#define	MSP_NUM_OF_SCH								1											// ロケーション・イベントのスケジュール数


// ========================  <<< モジュールID定義 >>>  ========================

#define	MID_NON										0x00										// ID: 0 モジュール指定なし（予約番号）
#define	SID_IOT										0x01										// ID: 1 IOT
#define	SID_TCBM									0x02										// ID: 2 TCBM
#define	MID_FRAME									0x03										// ID: 3 Frame
#define	MID_SYSTEM									0x05										// ID: 4 System
#define	MID_BOOKLET_TRAY							0x05										// ID: 5 Booklet Tray
#define	MID_BOOKLET									0x06										// ID: 6 Booklet
#define	MID_COMPILER								0x07										// ID: 7 Compiler
#define	MID_EJECT									0x08										// ID: 8 Eject
#define	MID_MAIN_TRANSPORT							0x09										// ID: 9 Main Transport
#define	MID_PUNCH									0x0A										// ID:10 Punch
#define	MID_SHELF									0x0B										// ID:11 Shelf
#define	MID_STACKER_TRAY							0x0C										// ID:12 Stacker Tray
#define	MID_STAPLER									0x0D										// ID:13 Stapler
#define	MID_TAMPER									0x0E										// ID:14 Tamper
#define	MID_TOP_TRAY								0x0F										// ID:15 Top Tray
#define	MID_TSQF									0x10										// ID:16 TSQF
#define	MID_FOLDER									0x11										// ID:17 Folder
#define	MID_OPTION_TRANSPORT						0x12										// ID:18 Option Transport
#define	MID_MAX										0x13										// ID:19 Max


// ========================  <<< イベント型定義 >>>  ========================

// ----- コマンド･イベントのデータ型 -----
struct CommandEvent {																			// <<< Command/Status Eventの型 >>>
	UC	mCommand[256] ;																			// Command Packet [MSP_MAX_CMD_SIZE]
} ;

// ----- ロケーション･イベントのデータ型 -----
struct LocationEvent {																			// <<< Location Timing Eventの型 >>>
	US	mSheetNo ;																				// Location Sheet Number
	UL	mParameter ;																			// Location Event Parameter
} ;


#endif          // MSP_EventID_h

// ****************************************************************  [ MSP_EventID.h : EOF ]  *****************************************************************
