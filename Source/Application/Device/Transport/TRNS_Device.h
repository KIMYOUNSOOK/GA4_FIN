// File Name:	TRNS_Device.h
// Description:	Header File of Entrance Controller
// Engineer:	Yuichi Hoshino
// Last Edit:	14.09.22
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef TRNS_Device_h
#define TRNS_Device_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "TRNS_Manager.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define TRNS_CONT_SHT_NUM 	4
#define TRNS_CONT_SHT_MASK	(TRNS_CONT_SHT_NUM - 1)
#define TRNS_CONT_EXT_MASK   0xFF00

#define TRNS_COMPO_CHAIN	12

#define TRNS_MEDIA_WEIGHT_HEAVY	106

#define TRNS_SETTIME_1500		1500
#define TRNS_SETTIME_1100		1100
#define TRNS_SETTIME_850		850
#define TRNS_SETTIME_200		200
#define TRNS_SETTIME_150		150
#define TRNS_GSOL_WAITTIME_100 100
#define TRNS_SETTIME_50		50
#define TRNS_MINUSTIME_20		20
#define TRNS_SETTIME_15		15

#define TRNS_MOTSPEED_600		6000
#define TRNS_MOTSPEED_800		8000

#define TRNS_SPEEDCHANGE_CPM65		65
#define TRNS_SPEEDCHANGE_CPM55		55


#define TRNS_PAPER_LENGTH297 		2970 		//�p����?�l(?0.1[mm])

/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Spec data of Entrance Control ---
typedef struct /*TraEntranceControllerSpec*/
{
	UC dummy;
} TraEntranceControllerSpec;

// --- Control step of Entrance Controller ---
typedef enum /*ETraEntranceControlStep*/
{
	TRNS_CONT_STEP_INACTIVE,
	TRNS_CONT_STEP_IDLE,

	TRNS_COMPO_FORWARD_LOW_SPEED = 10,
	TRNS_COMPO_FORWARD_MID_SPEED = 11,
	TRNS_COMPO_FORWARD_HI_SPEED = 12,
	TRNS_COMPO_GATE_SOLENOID_PULL = 13,
	TRNS_COMPO_GATE_SOLENOID_PUSH = 14,
	
	// Transport Diga ����
	TRNS_DIAG = 0x0100,
	TRNS_DIAG_st1, //
	TRNS_DIAG_st2, //
	TRNS_DIAG_st3, //
	TRNS_DIAG_st4, //

	// Static JAM ?�F�b�N����
	TRNS_SJAM_CHK = 0x0200,
	TRNS_SJAM_CHK_st1, // TransMotClock�J�n
	TRNS_SJAM_CHK_st2, // TransMot�쓮�J�n
	TRNS_SJAM_CHK_st3, // TimeOut�҂�
	TRNS_SJAM_CHK_st4, // Clock��?

	// Static JAM ?�F�b�N����
	TRNS_EXIT = 0x0300,
	TRNS_EXIT_st1, // ��??Clock�J�n
	TRNS_EXIT_st2, // TransMot�쓮�J�n T3TimeUp �҂�
	TRNS_EXIT_st2_T3, // ��??�p��IOT���x�܂Ō���		
	TRNS_EXIT_st3, // No Action ComExitSnrOn�҂� 
	TRNS_EXIT_st4, // ComExitSnrOn��� :ComExitSnrOff �҂�
//	TRNS_EXIT_st4_T4, // ComExitSnrOff��� �҂� 		
	TRNS_EXIT_st5, // ComExitSnrOff���	
	TRNS_EXIT_st6, // ��??����
	TRNS_EXIT_st7, // ��??�p���������x�܂ő���
	TRNS_EXIT_st8, // Ӱ��E~���f
	TRNS_EXIT_st9, // �I�� 

} ETrnsControlStep;
typedef enum /*ETrns_DistanseID*/
{
	// !!! Product Specific !!!
	TRNS_DISTANSEID_R,	//  IOT?�̗̂p�����o���t��??��IOT Fusing-Roll�ԃp�X��
	TRNS_DISTANSEID_E,	// �FIOT?�̗̂p�����o���t��??����ENTRANCE-ROLL�܂ł̋���
	TRNS_DISTANSEID_K,	// ENTRANCE-ROLL����Compile-Exit Sensor�܂ł̋���
	TRNS_DISTANSEID_S,	// IOT?�̗̂p�����o���t��??��IOT Exit Sensor(Hot Line)�ԃp�X��
	// JAM_DJAM_DISTANSEID_I,	// ��s����IOT����̗p���������x[mm/s]
	TRNS_DISTANSEID_NUM
} ETrns_DistanseID;

typedef enum /*ETraEntranceControlStep*/
{
	TRNS_IOTGATE_PUSH,
	TRNS_IOTGATE_PULL_STEP100MS,
	TRNS_IOTGATE_PULL_STEP_A,
	TRNS_IOTGATE_PULL_STEP_B,
	TRNS_IOTGATE_PULL_STEP_C_ON,
	TRNS_IOTGATE_PULL_STEP_C_OFF,	
}ETrnsIotGateStep;

typedef enum /*ETraEntranceControlStep*/
{
	TRNS_SPEED_ID_IOT,
	TRNS_SPEED_ID_V3,
	TRNS_SPEED_ID_V4,
	TRNS_SPEED_ID_V5,
}ETrnsSpeedID;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void TRNS_Device_Constructor();
ETraControllerState TRNS_Device_GetState();
void TRNS_Device_Reset();
void TRNS_Device_SetUp();
void TRNS_Device_StartSheet();
void TRNS_Device_StopSheet();
void TRNS_Device_AbortSheet();
void TRNS_Device_StartJamCheck();
void TRNS_Device_StopJamCheck();
void TRNS_Device_StartJamClearAssist();
void TRNS_Device_StopJamClearAssist();
void TRNS_Device_IotExitLocationLead(UC sheetNo);
void TRNS_Device_EntranceLocationTail(UC sheetNo);
void TRNS_Device_EntranceSensorON();
void TRNS_Device_CompileExitSnrLocationLead();
void TRNS_Device_CompileExitSnrLocationTail();
void TRNS_Device_ComponentControl(US aChainNo, US aLinkNo, EAction aAction);
void TRNS_StartAct(ETrnsControlStep aStep, UC aSheetNo);
void TRNS_Device_IOTGATE_SignalON(void);
void TRNS_Device_IOTGATE_SignalOFF(void);
void TRNS_Device_IOTGateSol_Pull( void);
void TRNS_Device_IOTGateSol_Push(void);
void TRNS_Device_IOTGateSol_AllOff(void);

void TRNS_Device_EmgStop(void);


//void TRNS_Device_Log_Short(UC para1, US para2, US para3, US para4);
void PrintTransLog(UC para1, UC para2, US para3, US para4);


#endif // #ifndef TRNS_Device_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
