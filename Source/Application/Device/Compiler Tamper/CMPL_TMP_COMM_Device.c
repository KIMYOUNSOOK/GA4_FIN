// File Name:	CMPL_TMP_COMM_Device.c
// Description:	Compiler Front/Rear Tamper Device Controller
// Engineer:	Takahiko Minaga
// Last Edit:	14.09.24
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"
#include "DD_STM.h"
#include "ECMPL_TMP_Profile.h"
#include "CMPL_TMP_COMM_Device.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


#define CMPL_TMP_PRF_STEP_NUM_NORMAL_HI		30
#define CMPL_TMP_PRF_STEP_NUM_NORMAL_MID	13
#define CMPL_TMP_PRF_STEP_NUM_NORMAL_LOW	13

#define CMPL_TMP_PRF_STEP_NUM_INITIALIZE	30


// Profile table of Slow-Up/Down高速プロファイル
const US cCMPL_TMP_Hi_Speed_Profile[] = {
	6000,	6030,	6100,	6230,	6400,	 6620,	 6890,	 7190,	 7540,	 7910,	// 0-9
	 8320,	 8740,	 9190,	 9650,	10120,	10590,	11060,	11520,	11970,	12390,	// 10-19
	12800,	13170,	13520,	13820,	14090,	14310,	14480,	14610,	14680,	14710,	// 20-29
	14680,    14610,    14480,     14310,    14090,    13820,     13520,    13170,    12800,    12390,    //  30-39
	11970,    11520,    11060,    10590,      10120,     9650,      9190,      8740,      8320,      7910,   // 40-49
	 7540,       7190,     6890,      6620,        6400,     6230,      6100,      6030,      6000,       0,      // 50-59 
};

// Profile table of Slow-Up/Down中速プロファイル
const US cCMPL_TMP_Mid_Speed_Profile[] = {
	6000,	6070,	6260,	6580,	6980,	7460,	7970,	8470,	8950,	9350,	// 0-9
	9670,	9860,	9930,	9860,      9670,      9350,       8950,     8470,      7970,      7460,	// 10-19
	6980,     6580,       6260,      6070,      6000,         0,   									//20-24  
};

// Profile table of Slow-Up/Down低速プロファイル
const US cCMPL_TMP_Low_Speed_Profile[] = {
	2500,	2580,	2810,	3170,	3640,	4190,	4790,	5380,	5930,	6400,	// 0-9
	6760,	6990,	7070,	6990,	6760,	6400, 	5930, 	5380,	4790,	4190, // 10-19
	3640,	3170, 	2810, 	2580,	2500,	  0,									//20-24  
};


// Profile table of Slow-Up/Downイニシャライズプロファイル
const US cCMPL_TMP_Init_Speed_Profile[] = {
	6000,	6500,	7000,	7500,	8000,	8500,	9000,	9500,	10000,	10070,	// 0-9
	10100,	10150,	10200,	10250,	10310,	10370,	10430,	10500,	10570,	10630,	// 10-19
	10690,	10750,	10800,	10850,	10900,	10930,	10960,	10980,	10990,	11000,	// 20-29
	10990,    10980,     10960,    10930,    10900,    10850,     10800,    10750,    10690,    10630,    // 30-39
	10570,    10500,     10430,    10370,    10310,    10250,     10200,	10150,    10100,    10070,   //40-49
	10000,      9500,      9000,      8500,      8000,      7500,       7000,     6500,      6000,      0,       //50-59
};

#define CMPL_TMP_HI_SPEED_STEP		((sizeof(cCMPL_TMP_Hi_Speed_Profile)/sizeof(US)) - 1)
#define CMPL_TMP_MID_SPEED_STEP		((sizeof(cCMPL_TMP_Mid_Speed_Profile)/sizeof(US)) - 1)
#define CMPL_TMP_LOW_SPEED_STEP		((sizeof(cCMPL_TMP_Low_Speed_Profile)/sizeof(US)) - 1)
#define CMPL_TMP_INIT_SPEED_STEP	((sizeof(cCMPL_TMP_Init_Speed_Profile)/sizeof(US)) - 1)


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_COMM_SetProfile
// Description   : タンパーモーター プロファイルt設定
// Parameter     : aProfile (指定タンパープロファイル ID)
//				 :
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_COMM_SetProfile()
{

	 DD_STM_SetProfile(DD_STM_PROF_ID_TAMPER_NORMAL_HI, cCMPL_TMP_Hi_Speed_Profile);
	 DD_STM_SetProfile(DD_STM_PROF_ID_TAMPER_NORMAL_MID, cCMPL_TMP_Mid_Speed_Profile );
	 DD_STM_SetProfile(DD_STM_PROF_ID_TAMPER_NORMAL_LOW, cCMPL_TMP_Low_Speed_Profile );
	 DD_STM_SetProfile(DD_STM_PROF_ID_TAMPER_INITIALIZE, cCMPL_TMP_Init_Speed_Profile );

}



///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_COMM_GetTampingProfile
// Description   : タンピングProfile取得
// Parameter     : aSheetNo (対象シートナンバー)
// Return        : タンピングProfile
///////////////////////////////////////////////////////////////////
ECMPL_TMP_ProfileID CMPL_TMP_COMM_GetTampingProfile(UC aSheetNo)
{
	ECMPL_TMP_ProfileID aProfile;

	if( SRVC_GetProcessSize(aSheetNo) <= CMPL_TMP_COMM_TAMPING_PROFILE_BORDER_LENGTH )
		aProfile = CMPL_TMP_PRF_ID_NORMAL_HI;
	else
		aProfile = CMPL_TMP_PRF_ID_NORMAL_MID;

	return aProfile;
}



///////////////////////////////////////////////////////////////////
// Function Name : PrintTamperLog_Short
// Description   :  Pulse
// Parameter     : para1 (数字列の最初に付加する値) : Tamper Action
//               : para2 (数字列の２番目に付加する値) : 移動パルス
//               : para3 (数字列の３番目に付加する値) : Sheet No
// Return        : ‐
///////////////////////////////////////////////////////////////////
void PrintTamperLog_Short(US para1, US para2, US para3)
{

	UC 	log[6];

	log[0] =  (UC)(para1 >>8 );
	log[1] =  (UC)(para1 & 0x00FF);
	log[2] =  (UC)(para2 >>8 );	
	log[3] =  (UC)(para2 & 0x00FF);

	log[4] =  (UC)(para3 >>8 );
	log[5] =  (UC)(para3 & 0x00FF);

	
//#define	LOGID_TMP=12, 'S'=Send,   LOG_CATEGORY_DEBUG=2
	DD_LOG_Put(LOGID_TMP, 'S', log, sizeof(log), LOG_CATEGORY_DEBUG);

}


