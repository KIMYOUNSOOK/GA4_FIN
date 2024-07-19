// File Name: DD_ResourceAssign.h
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

#ifndef DD_RESOURCE_ASSIGN_H
#define DD_RESOURCE_ASSIGN_H

//  ================================ SWT ================================
#define DD_SOFTWARE_TIMER_CH			0x03
#define DD_SOFTWARE_TIMER_INTERVAL_MS	1
#define DD_SOFTWARE_TIMER_INTERVAL		((US)((DD_TAU_CPU_CLOCK_FREQ_MHZ * 1000) / DD_SOFTWARE_TIMER_INTERVAL_MS ) - 1)

//  ================================ STM ================================
#ifdef DD_STM_C

#include "DD_STM.h"

typedef struct{
	UC		timerCH;
	UC		dmacCH;
	UC		cntTimerCH;				// DMA代替のパルスカウンタ用タイマ(DMAとは排他使用）
}DD_STM_CTRL_TYPE;

const DD_STM_CTRL_TYPE gDD_STM_CtrlTable[DD_STM_ID_NUM] = {
//		TAU CH 		DMA CH				2nd TAU CH
 	{	0x00,		0,					DD_STM_NO_TIMER		},	// Stacker Motor Clock
	{	0x01,		1,					DD_STM_NO_TIMER		},	// Eject Belt Motor Clock
	{	0x10,		2,					DD_STM_NO_TIMER		},	// Right Tamper Motor Clock
	{	0x11,		3,					DD_STM_NO_TIMER		},	// Left Tamper Motor Clock
};

#endif // #ifdef DD_STM_C



//  ================================ PWM ================================
#ifdef DD_PWM_C

#include "DD_PWM.h"

typedef struct{
	UC		masterTAU_CH;
	UC		slaveTAU_CH;
}DD_PWM_CTRL_TYPE;

const DD_PWM_CTRL_TYPE gDD_PWM_CtrlTable[DD_PWM_ID_NUM] = {
//		Master TAU CH 			Slave TAU CH
	{	0x04,					0x05						},	// Stapler Motor
	{	0x06,					0x07						},	// Eject Compile Motor(Current)
};

#endif	// #ifdef DD_PWM_C



//  ================================ SQP ================================
#ifdef DD_SQP_C

#include "DD_SQP.h"

typedef struct{
	UC		TAU_CH;
}DD_SQP_CTRL_TYPE;

const DD_SQP_CTRL_TYPE gDD_SQP_CtrlTable[DD_SQP_ID_NUM] = {
//		TAU CH
	{	0x02	},									// Transport Motor Clock
};

#endif	// #ifdef DD_SQP_C



//  ================================ DIO ================================
#define DD_INTP0_PORT 	(P13.7)			//
#define DD_INTP1_PORT 	(P5.0)			// Compile Exit Sensor
#define DD_INTP2_PORT 	(P5.1)			// Staple Home Sensor
#define DD_INTP3_PORT 	(P3.0)			// IOT Exit Sensor in
#define DD_INTP6_PORT 	(P14.0)			// Stacker No Paper Sensor
#define DD_INTP7_PORT 	(P14.1)			// SubPaddle Home Sensor
#define DD_INTP8_PORT 	(P7.4)			// Right Tamper Home Sensor
#define DD_INTP9_PORT 	(P7.5)			// Left Tamper Home Sensor
#define DD_INTP10_PORT 	(P7.6)			// Set Clamp Home Sensor
#define DD_INTP11_PORT 	(P7.7)			//
#define DD_INTP12_PORT   (P14.7)			// Eject Home Sensor(Not INT)



typedef enum {
	DD_INT_xxxx1_SENSOR,				// DD_INTP0 = 0
	DD_INT_COMPILE_EXIT_SENSOR,			// DD_INTP1 = 1
	DD_INT_STAPLE_HOME_SENSOR,			// DD_INTP2 = 2
	DD_INT_IOT_EXIT_SENSOR,				// DD_INTP3 = 3
	DD_INT_xxxx2_SENSOR,				// DD_INTP4 = 4, not used
	DD_INT_xxxx3_SENSOR,				// DD_INTP5 = 5, not used
	DD_INT_STACKER_NO_PAPER_SENSOR,		// DD_INTP6 = 6
	DD_INT_SUB_PADDLE_HOME_SENSOR,		// DD_INTP7 = 7
	DD_INT_RIGHT_TAMPER_HOME_SENSOR,	// DD_INTP8 = 8
	DD_INT_LEFT_TAMPER_HOME_SENSOR,		// DD_INTP9 = 9
	DD_INT_SET_CLAMP_HOME_SENSOR,		// DD_INT10 = 10
	DD_INT_xxxx4_SENSOR,				// DD_INT11 = 11
	DD_INT_EJECT_HOME_SENSOR,
	DD_INT_NUM,
} DD_INT_ID;
#endif
