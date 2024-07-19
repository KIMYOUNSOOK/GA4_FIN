// File Name: DD_DIO.h
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 13.10.15
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev007 Base)
// Revision 000

#ifndef DD_DIO_H
#define DD_DIO_H

#include "DD_Event.h"

#define DD_DIO_BIT_CONV(id) ((UL)0x00000001 << (id))

//DigitalInput
#define	MSG_SNR_ON_EDGE		0x80
#define	MSG_SNR_OFF_EDGE	0x81

typedef enum{	

	DD_DO_TRANSPORT_MOT_DIR,
	DD_DO_TRANSPORT_MOT_BRAKE,
	DD_DO_TRANSPORT_MOT_GATE_SOL_PULL,
	DD_DO_TRANSPORT_MOT_GATE_SOL_PUSH,

	DD_DO_LEFT_TAMPER_MOT_CUR,
	DD_DO_LEFT_TAMPER_MOT_DIR,
	DD_DO_LEFT_TAMPER_MOT_ENB,

	DD_DO_RIGHT_TAMPER_MOT_CUR,
	DD_DO_RIGHT_TAMPER_MOT_DIR,
	DD_DO_RIGHT_TAMPER_MOT_ENB,

	DD_DO_STACKER_MOT_CUR,
	DD_DO_STACKER_MOT_DIR,
	DD_DO_STACKER_MOT_ENB,	

	DD_DO_EJECT_BELT_MOT_DIR,
	DD_DO_EJECT_BELT_MOT_ENB,
	DD_DO_EJECT_BELT_MOT_PHASE1,
	DD_DO_EJECT_BELT_MOT_PHASE2,

	DD_DO_SET_CLAMP_CLUTCH_ENB,	

	DD_DO_STAPLER_MOT_IN1,

	DD_DO_RESET_OUT,
	DD_DO_WDT_CLR,
	DD_DD_INTLK_24_ON,

	DD_DO_ID_MAX
	
} DD_DIO_OutputPortID;

// Port LevelからSoftware管理レベルへの変換(反転)を行うOutputを指定する
static const UL DD_DIO_OutputConvertSwLevelBit = {
//	DD_DIO_BIT_CONV(DD_DO_XXXX)|
	DD_DIO_BIT_CONV(DD_DO_TRANSPORT_MOT_DIR) |
	DD_DIO_BIT_CONV(DD_DO_TRANSPORT_MOT_BRAKE) |
	DD_DIO_BIT_CONV(DD_DO_RIGHT_TAMPER_MOT_CUR) |
	DD_DIO_BIT_CONV(DD_DO_LEFT_TAMPER_MOT_CUR) |
	DD_DIO_BIT_CONV(DD_DO_STACKER_MOT_CUR) |
	
	0x00000000
};

// 入力信号は32まで
typedef enum{

	DD_DI_STAPLE_DOOR_INTERLOCK_SW,
	DD_DI_IOT_EXIT_SNR,
	DD_DI_COMPILE_EXIT_SNR,
	DD_DI_STAPLE_HOME_SNR,
	DD_DI_SELF_PRIMING_SNR,
	DD_DI_LOW_STAPLE_SNR,
	DD_DI_STACKER_TRAY_HEIGHT2_SNR,
	DD_DI_STACKER_TRAY_HEIGHT1_SNR,
	DD_DI_RIGHT_TAMPER_HOME_SNR,
	DD_DI_LEFT_TAMPER_HOME_SNR,
	DD_DI_SET_CLAMP_HOME_SNR,
	DD_DI_REAR_COVER_OPEN_SNR,
	DD_DI_UNIT0_SNR,
	DD_DI_STACKER_NO_PAPER_FULL_SNR,
	DD_DI_SUB_PADDLE_HOME_SNR,
	DD_DI_EJECT_HOME_SNR,
	DD_DI_REGI_CL_SNR,

	DD_DI_ID_MAX
} DD_DIO_InputPortID;

// Port LevelからSoftware管理レベルへの変換(反転)を行うInputを指定する
static const UL DD_DIO_InputConvertSwLevelBit = {
	DD_DIO_BIT_CONV(DD_DI_IOT_EXIT_SNR) |
	DD_DIO_BIT_CONV(DD_DI_COMPILE_EXIT_SNR) |
	DD_DIO_BIT_CONV(DD_DI_EJECT_HOME_SNR) |
	DD_DIO_BIT_CONV(DD_DI_SUB_PADDLE_HOME_SNR) |
	DD_DIO_BIT_CONV(DD_DI_SET_CLAMP_HOME_SNR) |
	DD_DIO_BIT_CONV(DD_DI_RIGHT_TAMPER_HOME_SNR) |
	DD_DIO_BIT_CONV(DD_DI_LEFT_TAMPER_HOME_SNR) |
	DD_DIO_BIT_CONV(DD_DI_SELF_PRIMING_SNR) |
	DD_DIO_BIT_CONV(DD_DI_STACKER_NO_PAPER_FULL_SNR) |
	// DD_DIO_BIT_CONV(DD_DI_STAPLE_DOOR_INTERLOCK_SW) |
	DD_DIO_BIT_CONV(DD_DI_REAR_COVER_OPEN_SNR) |
	0x00000000
};

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/

// EventLoopに登録する関数
void DD_DIO_Notify(DD_EventID eventId, void*arg);

// MSPに提供するAPI(その他は使用不可)
void DD_DIO_EntryCallback(void (*func)(UC, UC, UL));

// アプリに提供するAPI
void DD_DIO_Init(void);
UC DD_DIO_GetInput(DD_DIO_InputPortID inputID);
UC DD_DIO_GetInputPortLevel(DD_DIO_InputPortID inputID);
UC DD_DIO_GetDirectSensorLevel(DD_DIO_InputPortID inputID);
void DD_DIO_SetOutput(DD_DIO_OutputPortID portID, UC value);


#endif
