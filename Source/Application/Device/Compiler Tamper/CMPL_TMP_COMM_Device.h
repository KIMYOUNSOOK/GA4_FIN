// File Name:	CMPL_TMP_COMM_Device.h
// Description:	Header File of Compile Tamper Common Device
// Engineer:	Takahiko Minaga
// Last Edit:	14.09.01
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


#if !defined(CMPL_TMP_COMM_Device_h)
#define CMPL_TMP_COMM_Device_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "DD_STM.h"
#include "DD_DIO.h"
#include "ECMPL_TMP_Profile.h"
#include "NvmService.h"

// Motor Direction
// #define CMPL_TMP_COMM_INSIDE		ON    // CCW
// #define CMPL_TMP_COMM_OUTSIDE		OFF  //CW
#define CMPL_TMP_COMM_INSIDE		OFF 	//
#define CMPL_TMP_COMM_OUTSIDE		ON

#define TMP_RISE		(0)			// DDí[Ç∆îΩëŒò_óù
#define TMP_FALL		(1)

#define CMPL_TMP_COMM_MOT_STEP		0.2506 	 //0.2618
#define CMPL_TMP_COMM_TAMPING_PROFILE_BORDER_LENGTH	2970

#define CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20	20			// éñëOó„é• (Å~1[ms])
#define CMPL_TMP_COMM_MOTOR_STOP_TIME_10	10			// éñå„ó„é• (Å~1[ms])
#define CMPL_TMP_COMM_MOTOR_STOP_TIME_20	20			// éñå„ó„é• (Å~1[ms])

#define CMPL_TMP_COMM_START_DELAY_20		20
#define CMPL_TMP_COMM_SWITCH_TIME_10		10
#define CMPL_TMP_COMM_SWITCH_TIME_50		50
#define CMPL_TMP_COMM_FAIL_TIME_500 		500

#define CMPL_TMP_COMM_SNR_ON_FAIL_TIME		480

#define CMPL_TMP_COMM_OFFSET_PLS   			80   //78
#define CMPL_TMP_COMM_POS_UNKNOWN			0xFFFF
#define CMPL_TMP_COMM_POS_OPEN				0

#define CMPL_TMP_COMPO_TOTAL_PULSE			100			// CompoContìÆçÏÇÃèoóÕÉpÉãÉXêî (Å~1[pulse])

// TargetIndex define
#define  CMPL_TMP_PROF_TARGET_STEP_NUM_HI    29  // 30-1
#define  CMPL_TMP_PROF_TARGET_STEP_NUM_MID   12  // 13-1
#define  CMPL_TMP_PROF_TARGET_STEP_NUM_LOW  	12  // 13-1
#define  CMPL_TMP_PROF_TARGET_STEP_NUM_INIT  29  // 30-1

#define CMPL_TMP_SIZE_POSITION_MOVE_PUSE_150	150


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CMPL_TMP_COMM_SetProfile();

ECMPL_TMP_ProfileID CMPL_TMP_COMM_GetTampingProfile(UC aSheetNo);
void PrintTamperLog_Short(US para1, US para2, US para3);


#endif //!defined(CMPL_TMP_COMM_Device_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
