// File Name: DD_PWM.h
// Description:
// Engineer:  Suzuki Yuhei
// Last Edit: 12.10.04
// Revision:  006
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 12.7.5   | Suzuki Yuhei        | Created this file.
// Revision 000

#ifndef DD_PWM_H
#define DD_PWM_H

typedef enum{
	DD_PWM_ID_STAPLER_MOT,
	DD_PWM_ID_EJECT_BELT_CUR,
	DD_PWM_ID_NUM
} DD_PWM_ID;

// ƒAƒvƒŠ’ñ‹ŸAPI
void DD_PWM_Init(void);
void DD_PWM_SetFrequency(UC motorID, UL frequency, UC duty);
void DD_PWM_Start(UC motorID);
void DD_PWM_Stop(UC motorID);

#endif