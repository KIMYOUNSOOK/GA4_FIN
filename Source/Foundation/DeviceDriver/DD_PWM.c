// File Name: DD_PWM.c
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 13.09.11
// Revision:  010
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 12.08.05 | Nishizawa Mitsuyuki | Created this file.
// Revision 001

#define DD_PWM_C

#include "Common.h"
#include "DD_Common.h"
#include "DD_TAU.h"
#include "DD_Event.h"
#include "LIB_Lock.h"
#include "DD_PWM.h"
#include "DD_ResourceAssign.h"

//============================================================================= 
// Function Name : DD_PWM_Init
// Description   : タイマチャネルの設定初期化
// Parameter     : void
// Return        : void
// Note          : -
//=============================================================================
void DD_PWM_Init(void)
{
	UC motorIndex = 0;
	
	for (motorIndex = 0; motorIndex < DD_PWM_ID_NUM; motorIndex++) {
		DD_TAU_InitByChannel(gDD_PWM_CtrlTable[motorIndex].masterTAU_CH, DD_TAU_MODE_PWM_MASTER_ID, DD_TAU_ACTIVE_HIGH_ID);
		DD_TAU_InitByChannel(gDD_PWM_CtrlTable[motorIndex].slaveTAU_CH, DD_TAU_MODE_PWM_SLAVE_ID, DD_TAU_ACTIVE_NO_CARE_ID);	// スレーブはActiveレベルは無関係
	}
}

//============================================================================= 
// Function Name : DD_PWM_SetFrequency
// Description   : 周波数とDUTYの設定
// Parameter     : motorID		"モータID"
//               : frequency	"周波数"
//				 : duty			"デューティー比"
// Return        : void
// Note          : -
//=============================================================================
void DD_PWM_SetFrequency(UC motorID, UL frequency, UC duty)
{
	US tdrValMaster = (US)(1000000 / frequency) - 1;
	US tdrValSlave = (US)(((1000000 / frequency) * (UL)duty) / 100);
	
	DD_TAU_SetTDR(gDD_PWM_CtrlTable[motorID].masterTAU_CH, tdrValMaster);
	DD_TAU_SetTDR(gDD_PWM_CtrlTable[motorID].slaveTAU_CH, tdrValSlave);
}

//============================================================================= 
// Function Name : DD_PWM_Start
// Description   : Pwm出力の開始	
// Parameter     : 
// Return        : 
// Note          : 
//=============================================================================
void DD_PWM_Start(UC motorID)
{
	DD_TAU_Start2CH(gDD_PWM_CtrlTable[motorID].masterTAU_CH, gDD_PWM_CtrlTable[motorID].slaveTAU_CH);
}

//============================================================================= 
// Function Name : DD_PWM_Stop
// Description   : 	Pwm出力の停止
// Parameter     : 
// Return        : 
// Note          : 
//=============================================================================
void DD_PWM_Stop(UC motorID)
{
	DD_TAU_Stop2CH(gDD_PWM_CtrlTable[motorID].masterTAU_CH, gDD_PWM_CtrlTable[motorID].slaveTAU_CH);
}
