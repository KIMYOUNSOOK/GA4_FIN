// File Name:	TRAY_STK_Device.c
// Description:	Stacker Device
// Engineer:	KimYS
// Last Edit:	15.03.12
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"
#include "DD_DIO.h"
#include "DD_STM.h"
#include "DD_INT.h"
#include "DD_ResourceAssign.h"
#include "MSP_EventTimer.h"
#include "TRAY_STK_Device.h"
#include "TRAY_Manager.h"
#include "NvmService.h"

#include "CNvmInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////
#define STK_DIR_UP					OFF //ON
#define STK_DIR_DOWN				ON  //OFF

#define STK_MOT_ENABLE_TIME_20	20	//
#define STK_MOT_ENABLE_TIME_30	30	//
#define STK_MOT_STOP_TIME_20  	20	//
#define STK_MOT_SWITCH_TIME_50	50
#define STK_FAIL_TIME_500 			500
#define STK_FAIL_TIME_1000 		1000
#define STK_FAIL_TIME_5000 		5000

//MotStepUp���ASNR�C�x���g���������̂���
#define  START_CLK_UNKNOWN   		0
#define  START_CLK_WAIT    			0x80
#define  START_CLK_RECV     			0xFF

#define PLS_SCOPE    					11  // Pulse���͈�
#define PROF_PULSE_STEP_45			44


// Profile table of "Stacker Profile "
/*const US cTRAY_STK_Profile_1[] = { //Profile�ǉ��Ŗ��O�ύX
	4000,	4000,	4020,	4030,	4060,	4090,	4140,	4180,	4240,	4300,	// 0-9 (SlowUp),
	4370,	4440,	4520,	4600,	4690,	4780,	4880,	4980,	5080,	5180,	// 10-19 (SlowUp),
	5290,	5390,	5500,	5610,	5710,	5820,	5920,	6020,	6120,	6220,	// 20-29 (SlowUp),
	6310,	6400,	6480,	6560,	6630,	6700,	6760,	6820,	6860,	6910,	// 30-39 (SlowUp),
	6940,	6970,	6980,	7000,	7000,	// 40-44 (SlowUp),
	7000,	6980,      6970,      6940,      6910,	// 45-49(SlowDown)
	6860,      6820,	6760,	6700,	6630,	6560,	6480,	6400,	6310,	6220,	// 50-59 (SlowDown),
	6120,	6020,	5920,	5820,	5710,	5610,	5500,	5390,	5290,	5180,	// 60-69 (SlowDown),
	5080,	4980,	4880,	4780,	4690,	4600,	4520,	4440,	4370,	4300,	// 70-79 (SlowDown),
	4240,	4180,	4140,	4090,	4060,	4030,	4020,	4000,	4000,	0,		// 80-88 (SlowDown),  89(Stop)
};*/

//V3.3.92 Stacker Profile Added  & Modified. (V3.3.94)
const US cTRAY_STK_Profile_2[] = {
	4000,	4010,	4040,	4080,	4140,	4220,	4320,	4430,	4560,	4700,	// 0-9 (SlowUp),
	4850,	5030,	5210,	5400,	5610,	5820,	6050,	6280,	6510,	6760,	// 10-19 (SlowUp),
	7000,       7250,       7500,      7750,	8000,	8240,	8490,	8720,	8950,	9180,	// 20-29 (SlowUp),
	9390,	9600,	9790,	9970,	10150,	10300,	10440,	10570,	10680,	10780,	// 30-39 (SlowUp),
	10860,	10920,	10960,	10990,	11000,	// 40-44 (SlowUp),
	10990,	10960,     10920,     10860,     10780,	// 45-49(SlowDown)
	10680,     10570,	10440,	10300,	10150,	9970,	9790,	9600,	9390,	9180,	// 50-59 (SlowDown),
	8950,	8720,	8490,	8240,	8000,	7750,	7500,	7250,	7000,	6760,	// 60-69 (SlowDown),
	6510,	6280,	6050,	5820,	5610,	5400,	5210,	5030,	4850,	4700,	// 70-79 (SlowDown),
	4560,	4430,	4320,	4220,	4140,	4080,	4040,	4010,	4000,	0,		// 80-88 (SlowDown),  89(Stop)
};


typedef enum{
	eSTK_NORMAL = 0,
	eSTK_LOWER_SAFETY, 		// Lower Safety Warning : Height SNR1 1000ms On���Ȃ� -> Waring Count  >= 3
	eSTK_TRAY_FAIL_1, 		// Stacker Tray Fail (1)  : NoPaerSnr  1000ms
	eSTK_TRAY_FAIL_2, 		// Stacker Tray Fail(2) : Height SNR1  5000ms Off ���Ȃ�
	eSTK_UPPER_LIMIT_FAIL, //PulseCount=0-> Height SNR1 500ms Off���Ȃ�
	eSTK_PITCHSKIP_CHK,    // (V3.6.92) 1000ms �ȓ���HSNR1��OFF�AHSNR2��ON�ɂȂ�Ȃ��ꍇ - 
	eSTK_NUM,
} ESTK_TimeoutType;

typedef enum{	
	eSTK_JOB_NML_INIT, // Normal Initialize
	eSTK_JOB_SHT_INIT, //Job Initialize
	eSTK_JOB_EJECT,  //�r�o���AStackerJob
	eSTK_JOB_NUM,
} ESTK_JobType;

typedef struct
{
	ETRAY_STK_Action action;
	ETRAY_STK_Action compoAction;

	UL   stackerPulseCnt;  	// Stacker Tray  Pulse Count
	US   beforePulse;	// Pulse Count Restart���A�ȑOPulse�i�[�p
	UC   lowerSafetyCnt; 	// Count== 3 -> Stacker Lower Safety Warning ���m
	UC 	sheetNo;
	UC   preSheetNo;  //��Job��Sheet�i�[�p(ProcessEnd)
	UC   startClock;  // MotStepUp���ASNR�C�x���g���������̂���
	UC   ejectRecvFlag;  //Eject Job ���� ����  Flag ...
	UC   processEndFlag;  //For  Process End  control...
	DD_STM_PROF_ID profile;
	SS   timerIndex[eSTK_NUM];
	UC  jobType;  //Compile�ʒu�ֈړ�Job�͏d������邽�߁AJob���Ƃɋ�ʕK�v����
	UC  standbyFailCheckFlag;  //V3.4.0 FullStack/LowerSafety������AStandby����SNR�ω�����ꍇ,Fail�`�F�b�NFlag
} STK_Control;

typedef struct
{
	void (*mpHandler)(void) ;

}STK_CallFunc ;


static STK_Control 		gSTK;
static EFaultStatus 	gSTK_FaultStat;

/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
static void TRAY_STK_ControlThread();
static void TRAY_STK_ReceiveEventTimeUp(SS aTimerID, UL aType);
static void TRAY_STK_ReceiveFailEventTimeUp(SS aTimerID, UL aType);
static void TRAY_STK_ReceiveSteppingMotorEvent(SS aResult);
static void  TRAY_STK_ReceiveStartClock(SS aResult);

static void TRAY_STK_SetEventTimer(US aTime, UL aType);
static void TRAY_STK_CancelEventTimer(UL Type);
static void TRAY_STK_ClearTimerIndex(UL  eType);  //TimerIndex initialize ..

static void TRAY_STK_GetProfilePulse(US aTotalPulse,  UC  *pIndex);
static US   TRAY_STK_Get_HPulse();
static US   TRAY_STK_Get_GPulse();
static UC	  TRAY_STK_ResetPulse(UL  aMovePls);
static void TRAY_STK_UpdatePulse(UC   aDirection,  UL  aMovePls);
static void  TRAY_STK_MotorUpDown_NVMUpdate(UC   updown);
static void  TRAY_STK_ProcessEndCheck(void); //V3.3.92
static void TRAY_Device_StartCompoLiftUpDown(US aLinkNo);
static void TRAY_Device_StopCompoLiftUpDown(US aLinkNo);


// Stacker Home�ʒu�ֈړ�()
// TRAY_STK_HOME = 0x0300,
static void Home_st1_1();  			// �J�n�^�C�~���O�ݒ�
static void Home_st1_2();  			// HeightSNR1�C2�ɂ��ʒu���f
static void Home_st2_UP();			// ���[�^�쓮�ݒ�
static void Home_st3_UP(); 			// ���[�^�쓮�J�n(�����)
static void Home_st4_UP_ONWait(); 	// NoPaperSNR�@ON�҂�
static void Home_st4_UP_OFFWait();	// NoPaperSNR�@OFF�҂�
static void Home_st5_UP(); 			// Reset�J�n
static void Home_st6_UP();  		// Reset����
static void Home_st7_UP();  //�@SNR1�@OFF��M(10Pulse�J�n)
static void Home_st8_UP(); //�@StepDown�J�n
static void Home_st9_UP();  //�@�^�C�}�ݒ�

static void Home_st2_DN();  //���[�^�쓮�ݒ�
static void Home_st3_DN();  //���[�^�쓮�J�n(������)
static void Home_st4_DN();  //SNR1�@ON��M(H3Pulse�J�n)
static void Home_st5_DN();  //StepDown�J�n�@
static void Home_st6_DN();  //Stacker�ҋ@�ʒu�ֈړ�����J�nTrigger�҂�
static void Home_st7_DN();  //���[�^�쓮�J�n(�����)
static void Home_st8_DN();  //SNR1�@OFF��M(10Pulse�J�n)
static void Home_st9_DN();  //StepDown�J�n
static void Home_st10_DN();	//��㎥
static void Home_st11_DN();	//StackerHome���슮��

//Stacker�ҋ@�ʒu�ֈړ�����J�nTrigger����Reset����J�n(0519)
static void Home_st20_ONWait();	//NoPaperSNR�@ON�҂�
static void Home_st20_OFFWait();  //NoPaperSNR�@OFF�҂�	
static void Home_st21(); //�@Reset�J�n
static void Home_st22();  //�@Reset����


// Compile�ʒu�ֈړ�
// TRAY_STK_COMPILE = 0x0400,
static void Compile_st1();  			// StackerHeightSNR�ɂ�镪�򔻒f
static void Compile_st2_UP();  			// ���[�^�쓮�ݒ�
static void Compile_st3_UP();  			// ���[�^�쓮�J�n
static void Compile_st4_UP_ONWait();  	// NoPaperSNR�@ON�҂�
static void Compile_st4_UP_OFFWait();  	// NoPaperSNR�@OFF�҂�
static void Compile_st5_UP();  			// Reset�J�n
static void Compile_st6_UP();  			// Reset����
static void Compile_st7_UP();   //SNR1�@OFF��M(10Pulse�J�n)
static void Compile_st8_UP();   //StepDown�J�n
static void Compile_st9_UP();   //�^�C�}(�����]���p)�ݒ�
static void Compile_st10_UP();  //���[�^�쓮�J�n(������)
static void Compile_st11_UP();   //SNR1�@ON��M(G-Pulse�J�n)
static void Compile_st12_UP();   //StepDown�J�n

static void Compile_st2_DN();  //���[�^�쓮�ݒ�
static void Compile_st3_DN();   //���[�^�쓮�J�n
static void Compile_st4_DN();  //SNR1 ON��M(G-Pulse�J�n)
static void Compile_st5_DN();   //StepDown�J�n

static void Compile_st13(); 	//Compile�ʒu�ֈړ�����

// Eject�ʒu�ֈړ�(ADJUST)
// TRAY_STK_EJECT = 0x0500,
static void Eject_st1();  	// T1�^�C�}�҂�
static void Eject_st2();  	// ���[�^�쓮�ݒ�
static void Eject_st3();  	// ���[�^�쓮�J�n
static void Eject_st4();   	// Eject�ʒu�ֈړ�����

// �ҋ@�ʒu�ֈړ�(ProcessEnd)
// TRAY_STK_END = 0x0600,
static void End_st1();   		// ���[�^�쓮�J�n
static void End_st2_ONWait();  	// NoPaperSNR�@ON�҂�
static void End_st2_OFFWait();  // NoPaperSNR�@OFF�҂�
static void End_st3();   		// Reset�J�n
static void End_st4();  //Reset����
static void End_st5();  //SNR1�@OFF��M(10Pulse�J�n)
static void End_st6();  //StepDown�J�n
static void End_st7(); //��㎥
static void End_st8();  //�ҋ@�ʒu�ֈړ�����


static const STK_CallFunc  ControlThread_HOME_POSITION[ ] = {
	{NULL},
	{Home_st1_1},	//�J�n�^�C�~���O�ݒ�

	{Home_st1_2},	//HeightSNR1�C2�ɂ��ʒu���f
	{Home_st2_UP},  //���[�^�쓮�ݒ�
	{Home_st3_UP},  //���[�^�쓮�J�n(�����)
	{Home_st4_UP_ONWait}, //NoPaperSNR�@ON�҂�
	{Home_st4_UP_OFFWait}, //NoPaperSNR�@OFF�҂�
	{Home_st5_UP},  //�@Reset�J�n
	{Home_st6_UP},  //�@Reset����
	{Home_st7_UP},  //�@SNR1�@OFF��M(10Pulse�J�n)
	{Home_st8_UP}, //�@StepDown�J�n
	{Home_st9_UP},  //�@�^�C�}�ݒ�

	{Home_st2_DN},  //���[�^�쓮�ݒ�
	{Home_st3_DN},  //���[�^�쓮�J�n(������)
	{Home_st4_DN},  //SNR1�@ON��M(H3Pulse�J�n)
	{Home_st5_DN},  //StepDown�J�n�@
	{Home_st6_DN},  //Stacker�ҋ@�ʒu�ֈړ�����J�nTrigger�҂�
	{Home_st7_DN},  //���[�^�쓮�J�n(�����)
	{Home_st8_DN},  //SNR1�@OFF��M(10Pulse�J�n)
	{Home_st9_DN},  //StepDown�J�n
	{Home_st10_DN}, //��㎥
	{Home_st11_DN}, //StackerHome���슮��
	
	{ Home_st20_ONWait}, //NoPaperSNR�@ON�҂�
	{ Home_st20_OFFWait},  //NoPaperSNR�@OFF�҂� 
	{ Home_st21}, //�@Reset�J�n
	{ Home_st22},  //�@Reset����
	
};


static const STK_CallFunc  ControlThread_COMPILE_POSITION[ ] = {
	{NULL},
	{Compile_st1},	//StackerHeightSNR�ɂ�镪�򔻒f

	{Compile_st2_UP},  //���[�^�쓮�ݒ�
	{Compile_st3_UP},	//���[�^�쓮�J�n
	{Compile_st4_UP_ONWait},   //NoPaperSNR�@ON�҂�
	{Compile_st4_UP_OFFWait},  //NoPaperSNR�@OFF�҂�
	{Compile_st5_UP},	//Reset�J�n
	{Compile_st6_UP},	//Reset����
	{Compile_st7_UP},	//SNR1�@OFF��M(10Pulse�J�n)
	{Compile_st8_UP},	//StepDown�J�n
	{Compile_st9_UP},	//�^�C�}(�����]���p)�ݒ�
	{Compile_st10_UP},	//���[�^�쓮�J�n(������)
	{Compile_st11_UP},	 //SNR1�@ON��M(G-Pulse�J�n)
	{Compile_st12_UP},	 //StepDown�J�n

	{Compile_st2_DN},  //���[�^�쓮�ݒ�
	{Compile_st3_DN},	//���[�^�쓮�J�n
	{Compile_st4_DN},  //SNR1 ON��M(G-Pulse�J�n)
	{Compile_st5_DN},	//StepDown�J�n

	{Compile_st13}, 	//Compile�ʒu�ֈړ�����
};

static const  STK_CallFunc  ControlThread_EJECT_POSITION[ ] = {
	{NULL},
	{Eject_st1},//T1�^�C�}�҂�
	{Eject_st2},  //���[�^�쓮�ݒ�
	{Eject_st3},  //���[�^�쓮�J�n
	{Eject_st4},  //Eject�ʒu�ֈړ�����
};


static const STK_CallFunc  ControlThread_END_POSITION[ ] = {
	 {NULL},
	 {End_st1},   //���[�^�쓮�J�n
	 {End_st2_ONWait},	//NoPaperSNR�@ON�҂�
	 {End_st2_OFFWait},  //NoPaperSNR�@OFF�҂�
	 {End_st3},  //Reset�J�n
	 {End_st4},  //Reset����
	 {End_st5},  //SNR1�@OFF��M(10Pulse�J�n)
	 {End_st6},  //StepDown�J�n
	 {End_st7}, //��㎥
	 {End_st8},  //�ҋ@�ʒu�ֈړ�����
};


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_PreStartAct
// Description   : Stacker_����J�n�O�@���삪�K�v�ȏꍇ
// Parameter     : aAction (���䂷�铮��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_STK_PreStartAct(ETRAY_STK_Action aAction, UC aSheetNo)
{
	switch (aAction) {
		case TRAY_STK_END : 
			gSTK.preSheetNo = aSheetNo;			
			gSTK.processEndFlag = true; //0502
			break;
		default : break;	
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_StartAct
// Description   : Stacker_����J�n
// Parameter     : aAction (���䂷�铮��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_STK_StartAct(ETRAY_STK_Action aAction, UC aSheetNo)
{
	switch( aAction ) {

	case TRAY_STK_COMPO_COMPILE : //= 0x0100,  STACKER MOT (Compile�ʒu�ֈړ�)
		gSTK.action = TRAY_STK_COMPILE_st1;
		gSTK.jobType=eSTK_JOB_NUM; 
		gSTK.compoAction = aAction;
		gSTK.profile = DD_STM_PROF_ID_STACKER_PRF_2; //DD_STM_PROF_ID_STACKER_PRF_1;
		break;

	case TRAY_STK_COMPO_EJECT :  // = 0x0200, 	 STACKER MOT (Eject�ʒu�ֈړ�)
		gSTK.action = TRAY_STK_EJECT_st1;
		gSTK.jobType=eSTK_JOB_NUM; 
		gSTK.compoAction = aAction;
		gSTK.profile =DD_STM_PROF_ID_STACKER_PRF_2;  //DD_STM_PROF_ID_STACKER_PRF_1;
		break;

	case TRAY_STK_HOME :  //= 0x0300,  Stacker Home�ʒu�ֈړ�
		gSTK.action = TRAY_STK_HOME_st1_1;
		gSTK.jobType=eSTK_JOB_SHT_INIT;  //Job Init
		//TB(0528)
		gSTK.stackerPulseCnt = SRVC_GetNvmValueUL(NVMID_ELEVATOR_MOT_PULSE_COUNT);  
		gSTK.profile = DD_STM_PROF_ID_STACKER_PRF_2;   //V3.3.95
		break;

	case TRAY_STK_COMPILE : // = 0x0400,  Compile�ʒu�ֈړ�
		gSTK.action = TRAY_STK_COMPILE_st1;
		gSTK.profile = DD_STM_PROF_ID_STACKER_PRF_2;
		break;

	case TRAY_STK_EJECT :  // = 0x0500,	 Eject�ʒu�ֈړ�(ADJUST)
		gSTK.action = TRAY_STK_EJECT_st1;
		gSTK.jobType=eSTK_JOB_EJECT;  // Eject
		gSTK.profile = DD_STM_PROF_ID_STACKER_PRF_2;
		break;

	case TRAY_STK_END : // = 0x0600,  �ҋ@�ʒu�ֈړ�(ProcessEnd)
		gSTK.action = TRAY_STK_END_st1;		
		gSTK.jobType=eSTK_JOB_NUM; 
		gSTK.profile = DD_STM_PROF_ID_STACKER_PRF_2;
		break;

	default :  return;
	}
	gSTK.sheetNo = aSheetNo;
	gSTK.startClock = START_CLK_UNKNOWN;
	gSTK.beforePulse = 0;
	gSTK.ejectRecvFlag = false;

	TRAY_STK_ClearTimerIndex(eSTK_NUM);
	TRAY_STK_ControlThread();

}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_ControlThread
// Description   :  Stacker����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_STK_ControlThread()
{
	ETRAY_STK_Action aAction = gSTK.action & TRAY_STK_ACTION_MASK;
	US  				 aStatus = gSTK.action & 0x00FF;

	if ( aStatus == 0 )  {
		TRAY_STK_DeviceStop();
		return;
	}

	switch ( aAction)	{

	case TRAY_STK_HOME :
		ControlThread_HOME_POSITION[aStatus].mpHandler(); 			break;

	case TRAY_STK_COMPO_COMPILE :
	case TRAY_STK_COMPILE :
		ControlThread_COMPILE_POSITION[aStatus].mpHandler(); 		break;

	case TRAY_STK_COMPO_EJECT :
	case TRAY_STK_EJECT :
		ControlThread_EJECT_POSITION[aStatus].mpHandler(); 			break;

	case TRAY_STK_END :
		ControlThread_END_POSITION[aStatus].mpHandler(); 			break;

	default : 									break;
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_ReceiveEventTimeUp
// Description   : �C�x���g�^�C�}�̃^�C���A�b�v
// Parameter     : timerID (Timer ID)
//               :    aType
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_STK_ReceiveEventTimeUp(SS aTimerID, UL aType)
{
	if (  aTimerID != gSTK.timerIndex[aType] ) {
		return;
	}

	gSTK.timerIndex[aType] = MSP_MAX_TMR_NUM;

	switch( gSTK.action ) {

	// STACKER HOME����
	case TRAY_STK_HOME_st1_1 : //T1 Timeout
	case TRAY_STK_HOME_st2_UP :
	case TRAY_STK_HOME_st2_DN :
	case TRAY_STK_HOME_st10_DN :
		gSTK.action++;
		break;

	case TRAY_STK_HOME_st9_UP : // Timer �ݒ�(50ms)
		gSTK.action = TRAY_STK_HOME_st2_DN; //������
		break;

	//Compile�ʒu�ֈړ�
	case TRAY_STK_COMPILE_st2_UP :
	case TRAY_STK_COMPILE_st2_DN :
	case TRAY_STK_COMPILE_st9_UP :
	//Eject�ʒu�ֈړ�
	case TRAY_STK_EJECT_st1 :
	case TRAY_STK_EJECT_st2 :
	//�ҋ@�ʒu�ֈړ�(ProcessEnd)
	case TRAY_STK_END_st7 :
		gSTK.action++;
		break;

 	default: return;

	}
	TRAY_STK_ControlThread();
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_ReceiveFailEventTimeUp
// Description   : �C�x���g�^�C�}�̃^�C���A�b�v
// Parameter     : timerID (Timer ID)
//               	: aType
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_STK_ReceiveFailEventTimeUp(SS aTimerID, UL aType)
{
	if ( aTimerID != gSTK.timerIndex[aType] ) {
		return;
	}

	gSTK.timerIndex[aType] = MSP_MAX_TMR_NUM;

	switch( aType ) {

	case  eSTK_LOWER_SAFETY : //Lower Safety Warning : Height SNR1 1000ms On���Ȃ� -> Waring Count  >= 3
		if ( ++gSTK.lowerSafetyCnt >= 3 ) {
		// Lower Safety Warning �@��������
			//gSTK_FaultStat = FAULT_OCCUR;
			SRVC_UpdateDeviceStatus(FAULT_OCCUR, DSNO_STACKER_LOW_SAFETY) ; //Low Safety
			TRAY_STK_DeviceStop();
				
		}else {  //T/B�΍� (LiftDown ���ALowerSafetyFail�����������삳��Ȃ�) V3.4.0
			// Fail Timer (Stacker LowerSafety Warning Counter)
			TRAY_STK_SetEventTimer(STK_FAIL_TIME_1000, eSTK_LOWER_SAFETY);
			PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	0x8888, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 
		}
		break;
/* (CR-011)  Stacker TrayFail 1 Delete..
	case  eSTK_TRAY_FAIL_1 :	// Stacker Tray Fail (1)  : NoPaerSnr  1000ms
		// Stacker Tray Fail �@��������
		gSTK_FaultStat = FAULT_OCCUR;
		SRVC_UpdateDeviceStatus(FAULT_OCCUR, DSNO_STACKER_TRAY_FAIL) ; //Low Safety
		TRAY_STK_DeviceStop();
		break;
*/
	case  eSTK_TRAY_FAIL_2 : //Stacker Tray Fail(2) : Height SNR1	5000ms Off ���Ȃ�
		// Stacker Tray Fail �@��������
		gSTK_FaultStat = FAULT_OCCUR;
		SRVC_UpdateDeviceStatus(FAULT_OCCUR, DSNO_STACKER_TRAY_FAIL) ; //StackerFail
		TRAY_STK_DeviceStop();
		break;

	case  eSTK_UPPER_LIMIT_FAIL : //PulseCount=0-> Height SNR1 500ms Off���Ȃ�
		// Upper Limit Fail ��������
		gSTK_FaultStat = FAULT_OCCUR;
		SRVC_UpdateDeviceStatus(FAULT_OCCUR, DSNO_STACKER_UPLIMIT_FAIL) ; //UpperLimitFail
		TRAY_STK_DeviceStop();
		break;

	case  eSTK_PITCHSKIP_CHK :  //PitchSkip Timeout .. (V3.6.92)
		SRVC_SetPitchSkip(ON); 
		PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	0xCCCC, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 
		break;

	default:
		return;
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_ReceiveSteppingMotorEvent
// Description   :
// Parameter     :aResult
//
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_STK_ReceiveSteppingMotorEvent(SS aResult)
{
	switch( gSTK.action ) {

	//Stacker Home�ʒu�ֈړ�()
	case  TRAY_STK_HOME_st7_UP ://�@10Pulse
	case  TRAY_STK_HOME_st8_UP ://�@StepDown
	case  TRAY_STK_HOME_st4_DN : // H3 Pulse
	case  TRAY_STK_HOME_st5_DN : // StepDown
	case  TRAY_STK_HOME_st8_DN : // 10 Pulse
	case  TRAY_STK_HOME_st9_DN : // StepDown
		gSTK.action++;
		break;


	//Compile�ʒu�ֈړ�
	case  TRAY_STK_COMPILE_st7_UP:  // 10Pulse
	case  TRAY_STK_COMPILE_st8_UP:  // StepDown
	case  TRAY_STK_COMPILE_st11_UP:	// G-Pulse
	case  TRAY_STK_COMPILE_st4_DN:	// G-Pulse
		gSTK.action++;
		break;

	case  TRAY_STK_COMPILE_st12_UP: // StepDown
	case  TRAY_STK_COMPILE_st5_DN:  //StepDown
		gSTK.action = TRAY_STK_COMPILE_st13;
		break;



	//Eject�ʒu�ֈړ�(ADJUST)
	case  TRAY_STK_EJECT_st3:   //���[�^�쓮�J�n
	//�ҋ@�ʒu�ֈړ�(ProcessEnd)
	case  TRAY_STK_END_st5 :  //10pulse
	case  TRAY_STK_END_st6 :  //StepDown
		gSTK.action++;
		break;

	default :  return;
	}

	TRAY_STK_ControlThread();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_ReceiveStartClock
// Description   : DD_STM_StartClock()  Callback
//
///////////////////////////////////////////////////////////////////
void  TRAY_STK_ReceiveStartClock(SS aResult)
{

	gSTK.startClock =  START_CLK_RECV;  //Mot StepUp ����Flag�ݒ�

	switch ( gSTK.action ) {

	/*case  TRAY_STK_HOME_st7_DN : 	// Home
		if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == OFF ) {

			gSTK.action++;
			TRAY_STK_ControlThread();
		}
		break;*/

	case  TRAY_STK_HOME_st3_DN :
	case  TRAY_STK_COMPILE_st3_DN :
	case  TRAY_STK_COMPILE_st10_UP:
		if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == ON ) {
// T/B V3.3.94  LowerSafetyWarning��  Cancel���ꂸ
			TRAY_STK_CancelEventTimer(eSTK_LOWER_SAFETY);
			gSTK.lowerSafetyCnt = 0;
// 
			gSTK.action++;
			TRAY_STK_ControlThread();
		}
		break;

	case  TRAY_STK_HOME_st4_UP_ONWait :
	case  TRAY_STK_HOME_st4_UP_OFFWait :
	case  TRAY_STK_HOME_st5_UP :
	case  TRAY_STK_HOME_st6_UP :
		if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == OFF ) {
			//V3.3.94  TimerCancel .. 
			TRAY_STK_CancelEventTimer(eSTK_TRAY_FAIL_2);
			TRAY_STK_CancelEventTimer(eSTK_UPPER_LIMIT_FAIL);

			gSTK.action = TRAY_STK_HOME_st7_UP;
			TRAY_STK_ControlThread();
		}
		break;
//// 150519 
	case  TRAY_STK_HOME_st20_ONWait :
	case  TRAY_STK_HOME_st20_OFFWait :
	case  TRAY_STK_HOME_st21 :
	case  TRAY_STK_HOME_st22 :
		if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == OFF ) {
			//V3.3.94  TimerCancel ..
			TRAY_STK_CancelEventTimer(eSTK_TRAY_FAIL_2);
			TRAY_STK_CancelEventTimer(eSTK_UPPER_LIMIT_FAIL);

			gSTK.action = TRAY_STK_HOME_st8_DN;
			TRAY_STK_ControlThread();
		}
		break;
		

	case  TRAY_STK_COMPILE_st4_UP_ONWait :
	case  TRAY_STK_COMPILE_st4_UP_OFFWait :
	case  TRAY_STK_COMPILE_st5_UP :
	case  TRAY_STK_COMPILE_st6_UP :
		if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == OFF ) {
			//V3.3.94  TimerCancel ..
			TRAY_STK_CancelEventTimer(eSTK_TRAY_FAIL_2);
			TRAY_STK_CancelEventTimer(eSTK_UPPER_LIMIT_FAIL);

			gSTK.action = TRAY_STK_COMPILE_st7_UP;
			TRAY_STK_ControlThread();
		}
		break;
//150526
	case  TRAY_STK_END_st2_ONWait :
	case  TRAY_STK_END_st2_OFFWait :
	case  TRAY_STK_END_st3 :
	case  TRAY_STK_END_st4 :
		if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == OFF ) {
			//V3.3.94  TimerCancel ..
			TRAY_STK_CancelEventTimer(eSTK_TRAY_FAIL_2);
			TRAY_STK_CancelEventTimer(eSTK_UPPER_LIMIT_FAIL);

			gSTK.action = TRAY_STK_END_st5;
			TRAY_STK_ControlThread();
		}
		break;		

	default :  break;
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_Height1SensorON
// Description   : Height1SensorON Event ��M
//
///////////////////////////////////////////////////////////////////
void TRAY_STK_Height1SensorON ()
{
	if ( gSTK.action == TRAY_STK_st0 )
		return;

	//Fail Timer Clear
	TRAY_STK_CancelEventTimer(eSTK_LOWER_SAFETY);
	gSTK.lowerSafetyCnt = 0;

	switch ( gSTK.action ) {

	case  TRAY_STK_HOME_st3_DN :
	case  TRAY_STK_COMPILE_st3_DN:
	case  TRAY_STK_COMPILE_st10_UP:

		//StepUp����&SNR1=ON�̏����`�F�b�N
		if(gSTK.startClock == START_CLK_RECV) {
			gSTK.action++;
			TRAY_STK_ControlThread();
		}
		else if(gSTK.startClock == START_CLK_WAIT) { //MotStepUp���ASNR1 ON�C�x���g
			//->�ړ��ʕ␳�̂���
			gSTK.beforePulse = DD_STM_StopCountClock(DD_STM_ID_STACKER);
			DD_STM_StartCountClock(DD_STM_ID_STACKER);
		}
		break;

	default :  return;
	}

}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_Height1SensorOFF
// Description   : Height1SensorOFF Event ��M
//
///////////////////////////////////////////////////////////////////
void TRAY_STK_Height1SensorOFF()
{
	if ( gSTK.action == TRAY_STK_st0 )
		return;

	//Fail Timer Clear
	TRAY_STK_CancelEventTimer(eSTK_TRAY_FAIL_2);
	TRAY_STK_CancelEventTimer(eSTK_UPPER_LIMIT_FAIL);

      // ( V3.6.92)  PitchSkip Check .. 
	TRAY_STK_CancelEventTimer(eSTK_PITCHSKIP_CHK); 
	
	if ( ON == SRVC_GetPitchSkip() ) {

		SRVC_SetPitchSkip(OFF);
		PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	0xCCDD, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 
	}

	switch ( gSTK.action ) {

	/*case  TRAY_STK_HOME_st7_DN :
		// StepUp����&SNR1=OFF�̏����`�F�b�N
		if ( gSTK.startClock == START_CLK_RECV ) {
			gSTK.action++;
			TRAY_STK_ControlThread();
		}
		break;*/

	//Reset ���쒆�AStacker Height1 SNR�@OFF��M���A����
	case  TRAY_STK_HOME_st4_UP_ONWait :
	case  TRAY_STK_HOME_st4_UP_OFFWait :
	case  TRAY_STK_HOME_st5_UP :
	case  TRAY_STK_HOME_st6_UP :
	// StepUp����&SNR1=OFF�̏����`�F�b�N
	if ( gSTK.startClock == START_CLK_RECV ) {
		gSTK.action = TRAY_STK_HOME_st7_UP;
		TRAY_STK_ControlThread();
	}
	break;

	//Reset ���쒆�AStacker Height1 SNR�@OFF��M���A����(0519)
	case  TRAY_STK_HOME_st20_ONWait :
	case  TRAY_STK_HOME_st20_OFFWait :
	case  TRAY_STK_HOME_st21 :
	case  TRAY_STK_HOME_st22 :
	// StepUp����&SNR1=OFF�̏����`�F�b�N
	if ( gSTK.startClock == START_CLK_RECV ) {
		gSTK.action = TRAY_STK_HOME_st8_DN;
		TRAY_STK_ControlThread();
	}
	break;

	case  TRAY_STK_COMPILE_st4_UP_ONWait :
	case  TRAY_STK_COMPILE_st4_UP_OFFWait :
	case  TRAY_STK_COMPILE_st5_UP :
	case  TRAY_STK_COMPILE_st6_UP:
	// StepUp����&SNR1=OFF�̏����`�F�b�N
	if ( gSTK.startClock == START_CLK_RECV ) {
		gSTK.action = TRAY_STK_COMPILE_st7_UP;
		TRAY_STK_ControlThread();
	}
	break;

	case  TRAY_STK_END_st2_ONWait :
	case  TRAY_STK_END_st2_OFFWait :
	case  TRAY_STK_END_st3 :
	case  TRAY_STK_END_st4 :
	// StepUp����&SNR1=OFF�̏����`�F�b�N
	if ( gSTK.startClock == START_CLK_RECV ) {
			
		gSTK.action = TRAY_STK_END_st5;
		TRAY_STK_ControlThread();
	}
	break;

	default :  return;
	}

}

//V3.4.0 �������d�l:�@FullStack/LowerSafety������AStandby����SNR�ω�����ꍇ�AStacker����
///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_Height2SensorOFF
// Description   : Stacker Height2 Sensor Event ��M
///////////////////////////////////////////////////////////////////
void TRAY_STK_Height2SensorOFF()
{
	if ( gSTK.action != TRAY_STK_st0  )  // Standby��Ԃ̂݃`�F�b�N
		return;

	//Standby && FlagSet��� 
	if ( true == gSTK.standbyFailCheckFlag ) {
	
		//�iSNR1=ON�ASNR2=OFF�@�Ⴂ���) -> ����Start 
		if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == HIGH) { 
			TRAY_STK_StartAct(TRAY_STK_COMPILE, SHEETNO_NUM); //0);
			PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	0xBBBB, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 
		}
		
	}else { //V3.6.92

		//StackerStatus��Running��� & CompileTray�ɗp���Ȃ� & HSNR2�ω��iON��OFF�j &&HSNR1=ON && Fault NotOccur 
		if ( (TRAY_Manager_GetStatus() == FIN_STT_READY) && 
			(SRVC_CheckUnfinishedSheetExist(eCdiFinOutDest_All) == false) &&
			(DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == HIGH) &&
			(FAULT_NOT_OCCUR ==TRAY_STK_IsOccurFault()) ){	
			
				TRAY_STK_StartAct(TRAY_STK_COMPILE, SHEETNO_NUM);	
				PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	0xBBCC, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 
		}	
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_Height2SensorON
// Description   : Stacker Height2 Sensor Event ��M
///////////////////////////////////////////////////////////////////
void TRAY_STK_Height2SensorON()
{
	if ( gSTK.action == TRAY_STK_st0 )
		return;

	//V3.6.92 
	TRAY_STK_CancelEventTimer(eSTK_PITCHSKIP_CHK);	

	if ( ON == SRVC_GetPitchSkip() ) {
		
		SRVC_SetPitchSkip(OFF);
		PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	0xCCEE, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_NoPaperFullSensorON
// Description   : NoPaperFullSensorON Event ��M
//
///////////////////////////////////////////////////////////////////
void TRAY_STK_NoPaperFullSensorON()
{
	if ( (gSTK.compoAction != TRAY_STK_st0) || (gSTK.action == TRAY_STK_st0))  //Diag Mode || IDLE status ..
		return;

	//Fail Timer Clear
//	TRAY_STK_CancelEventTimer(eSTK_TRAY_FAIL_1);

	switch ( gSTK.action) {

	case  TRAY_STK_HOME_st4_UP_ONWait :
	case  TRAY_STK_HOME_st5_UP :
		
	case  TRAY_STK_HOME_st20_ONWait : //150519 Reset added
	case  TRAY_STK_HOME_st21 :

	case  TRAY_STK_COMPILE_st4_UP_ONWait :
	case  TRAY_STK_COMPILE_st5_UP :

	case  TRAY_STK_END_st2_ONWait :
	case  TRAY_STK_END_st3 :
		  gSTK.action++;
		  TRAY_STK_ControlThread();
		  break;
	default :  return;
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_NoPaperFullSensorOFF
// Description   : NoPaperFullSensorOFF Event ��M
//
///////////////////////////////////////////////////////////////////
void TRAY_STK_NoPaperFullSensorOFF()
{
	if ( (gSTK.compoAction != TRAY_STK_st0) || (gSTK.action == TRAY_STK_st0))  //Diag Mode || IDLE status ..
		return;

	//Fail Timer Clear
//	TRAY_STK_CancelEventTimer(eSTK_TRAY_FAIL_1);

	switch ( gSTK.action ) {

	case  TRAY_STK_HOME_st4_UP_OFFWait :

	case  TRAY_STK_HOME_st20_OFFWait :	

	case  TRAY_STK_COMPILE_st4_UP_OFFWait :

	case  TRAY_STK_END_st2_OFFWait :
		  gSTK.action++;
		  TRAY_STK_ControlThread();
		  break;

	default :  return;
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_InformedSetClampTrigger
// Description :  SetClamp�ˏo��Stacker�̑ҋ@�ʒu�ړ��J�nTrigger
// Parameter     :
///////////////////////////////////////////////////////////////////
void TRAY_STK_InformedStackerTrigger()
{
	gSTK.ejectRecvFlag = true;  // FLAG  Set ..

	//Stacker�ҋ@�ʒu�ֈړ�����J�nTrigger�҂����
	if (  gSTK.action == TRAY_STK_HOME_st6_DN) {
		gSTK.action++;
		TRAY_STK_ControlThread();
		gSTK.ejectRecvFlag = false;
	}
	else  {
	 //SetClamp���삪��葬�������ꍇ?
	 PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,  0xFFEE, (US)gSTK.stackerPulseCnt, 0); 

	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_SetEventTimer
// Description   : �C�x���g�^�C�}�[�Z�b�g
// Parameter     : aTime (�^�C�}�[�l (�~1[ms]))
//			  aType
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_STK_SetEventTimer(US aTime, UL aType)
{

	if (  aType >= eSTK_NUM)
		return;

	// Fail�C�x���g�ƕ����ď���
	if (  aType == eSTK_NORMAL)  {
		gSTK.timerIndex[aType] = MSP_SetEventTimer(aTime,
											TRAY_STK_ReceiveEventTimeUp, aType);

	} else {
		gSTK.timerIndex[aType] = MSP_SetEventTimer(aTime,
											TRAY_STK_ReceiveFailEventTimeUp, aType);
	}
	return;
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_CancelEventTimer
// Description   : �C�x���g�^�C�}�[�Z�b�g
// Parameter     : aType
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_STK_CancelEventTimer(UL  aType)
{
	UC   cnt;

	// ALL CLEAR TIMER
	if (  aType == eSTK_NUM ) {
		for(cnt=0; cnt < eSTK_NUM ; cnt++) {

			if ( gSTK.timerIndex[cnt] != MSP_MAX_TMR_NUM ) {

				MSP_CancelEventTimer(gSTK.timerIndex[cnt]);
				gSTK.timerIndex[cnt] = MSP_MAX_TMR_NUM;
			}
		}
	}

	else if ( gSTK.timerIndex[aType] != MSP_MAX_TMR_NUM ) {

		MSP_CancelEventTimer(gSTK.timerIndex[aType]);
		gSTK.timerIndex[aType] = MSP_MAX_TMR_NUM;
	}
	return;
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_ClearTimerIndex
// Description   : TimerIndex�폜
// Parameter     : ESTK_TimeoutType
// Return        : �]
///////////////////////////////////////////////////////////////////
void  TRAY_STK_ClearTimerIndex(UL  eType)
{
	UC   cnt;

	if ( eType > eSTK_NUM) // Error ..
		return;

	if ( eType == eSTK_NUM) {  // All Clear ����

		for(cnt=0; cnt < eSTK_NUM ; cnt++) {
			gSTK.timerIndex[cnt]=MSP_MAX_TMR_NUM;
		}

	} else {  // ����Index�̂�Clear����

		gSTK.timerIndex[eType] = MSP_MAX_TMR_NUM;
	}
	return;
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_IsExecutiongDeviceAction
// Description   : Stacker�̓����Ԃ��擾
// Parameter     : �]
// Return        : true�F���쒆�Afalse�F��~��
///////////////////////////////////////////////////////////////////
bool TRAY_STK_IsExecutionDeviceAction()
{
	if (  gSTK.action == TRAY_STK_st0 ) {
		return false;
	} else {
		return true;
	}	
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_IsOccurFault
// Description   : Stacker��Fail��Ԃ��擾
// Parameter     : �]
// Return        : true�FFail �Afalse: Not Fail
///////////////////////////////////////////////////////////////////
bool TRAY_STK_IsOccurFault()
{
	if (  gSTK_FaultStat == FAULT_OCCUR ) {
		PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,  0xFFFF, (US)gSTK.stackerPulseCnt, 0); 
		return true;
	}else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_Reset
// Description   : Stacker���Z�b�g
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_STK_Reset()
{
	gSTK.action = TRAY_STK_st0;
	gSTK.compoAction = TRAY_STK_st0;

	gSTK.stackerPulseCnt = SRVC_GetNvmValueUL(NVMID_ELEVATOR_MOT_PULSE_COUNT);  //  Stacker Tray  Pulse Count -> NVM  ??
	gSTK.lowerSafetyCnt = 0; // Count== 3 -> Stacker Lower Safety Warning ���m
	gSTK.sheetNo = SHEETNO_NUM;
	gSTK.preSheetNo=SHEETNO_NUM;
	gSTK.processEndFlag = false;
	gSTK.jobType=eSTK_JOB_NUM;
	gSTK.standbyFailCheckFlag = false;
	gSTK_FaultStat = FAULT_NOT_OCCUR;//0502

	TRAY_STK_ClearTimerIndex(eSTK_NUM);

	DD_DIO_SetOutput(DD_DO_STACKER_MOT_ENB,  OFF);
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR,  LOW);

// V3.3.92  Profile Added .. 
//	DD_STM_SetProfile(DD_STM_PROF_ID_STACKER_UPDOWN, cTRAY_STK_Profile);
//	DD_STM_SetProfile(DD_STM_PROF_ID_STACKER_PRF_1, cTRAY_STK_Profile_1);
	DD_STM_SetProfile(DD_STM_PROF_ID_STACKER_PRF_2, cTRAY_STK_Profile_2);


}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_DeviceStop
// Description   : Stacker��~
// Parameter     : �]  // Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_STK_DeviceStop()
{
	ETRAY_STK_Action aAction = gSTK.action & TRAY_STK_ACTION_MASK;
	UC aSheetNo = gSTK.sheetNo;

	gSTK.action = TRAY_STK_st0;
	gSTK.sheetNo = SHEETNO_NUM;
	gSTK.processEndFlag = false;
	gSTK.jobType=eSTK_JOB_NUM;

	TRAY_STK_CancelEventTimer(eSTK_NUM);

	DD_DIO_SetOutput(DD_DO_STACKER_MOT_ENB,  OFF);
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR,  LOW);

	DD_STM_ShutDownStop(DD_STM_ID_STACKER);
	SRVC_SetPitchSkip(OFF);

	if(gSTK.compoAction != TRAY_STK_st0)	{

		 gSTK.compoAction = TRAY_STK_st0;

	} else {
//		TRAY_Manager_InformedStackerActionComplete((US)aAction , aSheetNo);
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_GetProfilePulse
// Description   : Eject�ʒu�ֈړ����AStep�� , ConstPulse�Z�o
// Parameter      : aTotalPulse (���ړ��� (�~1[Pulse]))
//			   *aIndex(Step),
// Return        : �]
///////////////////////////////////////////////////////////////////
void  TRAY_STK_GetProfilePulse(US aTotalPulse,  UC  *pIndex)
{
	//�@��Pulse >�@(StepUpDownPulse*2 )
	if ( aTotalPulse > (SS)(45 * 2 )) {

		*pIndex = PROF_PULSE_STEP_45;

	} else  { // �@��Pulse�@<=�@(StepUpDownPulse*2 )

		*pIndex = (UC)(aTotalPulse /2) ;

	}
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_Get_HPulse
// Description; //H-Pulse �ݒ� (Eject )
// Parameter     : �]    // Return        : H-Pulse
///////////////////////////////////////////////////////////////////
US    TRAY_STK_Get_HPulse()
{

	US	  H_pulse = 0;
	
 //CR-008	
	UC     compileCount = SRVC_GetCompileCount(gSTK.sheetNo);
	UC     nvmCount = SRVC_GetNvmValueUC(NVMID_PAPER_SET_COUNT_N);


	switch ( SRVC_GetStapleMode(gSTK.sheetNo) ) {

	case eCdiFinStaple_SingleFront ://STAPLE
		// N2 >= N1  --> H1
		if ( SRVC_GetNvmValueUC(NVMID_CORNER_STAPLE_MODE_COUNT_N2) >=
			SRVC_GetNvmValueUC(NVMID_CORNER_STAPLE_COUNT_N1)) {

			if ( compileCount <= nvmCount ) { // �p���Z�b�g�����@N���ȉ�

				H_pulse = SRVC_GetNvmValueUS(NVMID_STACKER_MOT_LIFT_DOWN_COUNT_H1_1);

			} else {

				H_pulse = SRVC_GetNvmValueUS(NVMID_STACKER_MOT_LIFT_DOWN_COUNT_H1_2);

			}
		} else {  // --> H2

			if ( compileCount <= nvmCount ) { // �p���Z�b�g�����@N���ȉ�

				H_pulse = SRVC_GetNvmValueUS(NVMID_STACKER_MOT_LIFT_DOWN_COUNT_H2_1);

			} else {

				H_pulse = SRVC_GetNvmValueUS(NVMID_STACKER_MOT_LIFT_DOWN_COUNT_H2_2);

			}
		}
		break;	

	case eCdiFinStaple_Off : //UNSTAPLE
		// N2 >= N1 --> H1
		if ( SRVC_GetNvmValueUC(NVMID_CORNER_STAPLE_MODE_COUNT_N2) >=
			SRVC_GetNvmValueUC(NVMID_CORNER_STAPLE_COUNT_N1)) {

			if ( compileCount <= nvmCount ) { // �p���Z�b�g�����@N���ȉ�

				H_pulse = SRVC_GetNvmValueUS(NVMID_STACKER_MOT_LIFT_DOWN_COUNT_H1_3);

			} else {

				H_pulse = SRVC_GetNvmValueUS(NVMID_STACKER_MOT_LIFT_DOWN_COUNT_H1_4);

			}
		} else { // --> H2

			if ( compileCount <= nvmCount ) { // �p���Z�b�g�����@N���ȉ�

				H_pulse = SRVC_GetNvmValueUS(NVMID_STACKER_MOT_LIFT_DOWN_COUNT_H2_3);

			} else {

				H_pulse = SRVC_GetNvmValueUS(NVMID_STACKER_MOT_LIFT_DOWN_COUNT_H2_4);

			}
		}
		break;

	  default : break;
	}

	return	H_pulse;
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_Get_GPulse
// Description; //H-Pulse �ݒ� (Eject )
// Parameter     : �]    // Return        : H-Pulse
///////////////////////////////////////////////////////////////////
US    TRAY_STK_Get_GPulse()
{
	US	  G_pulse = 0;

	// N2 >= N1
	if ( SRVC_GetNvmValueUC(NVMID_CORNER_STAPLE_MODE_COUNT_N2) >=
				SRVC_GetNvmValueUC(NVMID_CORNER_STAPLE_COUNT_N1)) {

		G_pulse = SRVC_GetNvmValueUS(NVMID_STACKER_MOT_LIFT_UP_COUNT_G1);

	} else {

		G_pulse = SRVC_GetNvmValueUS(NVMID_STACKER_MOT_LIFT_UP_COUNT_G2);

	}
	//G-Pulse Test  V3.5.94
	PrintStackerTrayLog_Short(0xC0,SRVC_GetNvmValueUC(NVMID_CORNER_STAPLE_MODE_COUNT_N2)  , 
							SRVC_GetNvmValueUC(NVMID_CORNER_STAPLE_COUNT_N1), 
							G_pulse); //Log

	return  G_pulse;
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_ResetPulse
// Description   : Pulse Reset
// Parameter     : �]
// Return        :  NG/OK
///////////////////////////////////////////////////////////////////
UC	TRAY_STK_ResetPulse(UL  aMovePls)
{

	//Reset�p���X�l����
	// CNvmInformer_SaveNvm();
	if ( (aMovePls >= 59-PLS_SCOPE) && (aMovePls <= 59+PLS_SCOPE) ) { // 2mm

		gSTK.stackerPulseCnt = 0;

		// Pulse=0�̏ꍇ�AUpperLimitFail���m�J�n
		TRAY_STK_SetEventTimer(STK_FAIL_TIME_500,	eSTK_UPPER_LIMIT_FAIL);

	} else  if (( aMovePls >= 82-PLS_SCOPE) && (aMovePls <= 82+PLS_SCOPE)) { // 3mm

		gSTK.stackerPulseCnt = 127; 	//59+0+68

	} else  if (( aMovePls >= 105-PLS_SCOPE) && (aMovePls <= 105+PLS_SCOPE)) { // 4mm

		gSTK.stackerPulseCnt = 277; 	//127+82+68

	} else  if (( aMovePls >= 127-PLS_SCOPE) && (aMovePls <= 127+PLS_SCOPE)) { //5 mm

		gSTK.stackerPulseCnt = 450; 	//277+105+68

	} else  if (( aMovePls >= 150-PLS_SCOPE) && (aMovePls <= 150+PLS_SCOPE)) { //6 mm

		gSTK.stackerPulseCnt = 645; //450+127+68

	} else  if (( aMovePls >= 172-PLS_SCOPE) && (aMovePls <= 172+PLS_SCOPE)) { //7 mm

		gSTK.stackerPulseCnt = 863; //645+150+68

	} else  if (( aMovePls >= 195-PLS_SCOPE) && (aMovePls <= 195+PLS_SCOPE)) { //8 mm

		gSTK.stackerPulseCnt = 1103; //863+172+68

	} else  if (( aMovePls >= 218-PLS_SCOPE) && (aMovePls <= 218+PLS_SCOPE)) { //9 mm

		gSTK.stackerPulseCnt = 1366; //1103+195+68

	} else {
		PrintStackerTrayLog_Short(eSTK_LOG_PLS_RESET, (US)gSTK.stackerPulseCnt, 0xFF, 0xFF); //Log
		return	NG;
	}		
//test
	PrintStackerTrayLog_Short(eSTK_LOG_PLS_RESET, (US)gSTK.stackerPulseCnt, 0, 0); //Log

	return  OK;
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_UpdatePulse
// Description   : Pulse Update
// Parameter     : MOT DIRECTION,  MOVEPLS
// Return        : -
///////////////////////////////////////////////////////////////////
void    TRAY_STK_UpdatePulse(UC   aDirection,  UL  aMovePls)
{

	if ( aDirection == STK_DIR_DOWN ) { 	//DOWN -> ����

		gSTK.stackerPulseCnt  = gSTK.stackerPulseCnt + aMovePls;

	} else {  //UP -> ����

		if ( aMovePls >= gSTK.stackerPulseCnt ) {

			gSTK.stackerPulseCnt = 0;

		} else {

			gSTK.stackerPulseCnt = gSTK.stackerPulseCnt  - aMovePls;
		}
	}
	//(CR-013)
	SRVC_SetNvmValueUL(gSTK.stackerPulseCnt, NVMID_ELEVATOR_MOT_PULSE_COUNT);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_SetPulse_FullStackRelease
// Description   : Pulse Update
// Parameter     : aPulse
// Return        : -
///////////////////////////////////////////////////////////////////
void  TRAY_STK_SetPulse_FullStackRelease(UL   aPulse)
{
	gSTK.stackerPulseCnt = aPulse;
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_GetPulse
// Description   : Read pulse
// Parameter     :  
// Return        :  pulse 
///////////////////////////////////////////////////////////////////
UL    TRAY_STK_GetPulse()
{
	return  gSTK.stackerPulseCnt;
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_MotorUpDown_NVMUpdate	// V3.0  NVM �X�V	
// Description   : Stacker Motor Up/Down ����񐔍X�V
// Parameter     :  STK_DIR_UP,  STK_DIR_DOWN
// Return        :  void
///////////////////////////////////////////////////////////////////
void  TRAY_STK_MotorUpDown_NVMUpdate(UC   updown)
{
//STK_DIR_UP=0,  STK_DIR_DOWN=1
	UL	 count=0;

	if ( updown == STK_DIR_UP ) {
		//StackerMotorUP�����
		count =  SRVC_GetNvmValueUL(NVMID_STACKER_MOT_UP_COUNT) ;
		SRVC_SetNvmValueUL(count+1,  NVMID_STACKER_MOT_UP_COUNT) ;

	} else {  //DOWN
		//StackerMotorDOWN�����
		count = SRVC_GetNvmValueUL(NVMID_STACKER_MOT_DOWN_COUNT);
		SRVC_SetNvmValueUL(count+1,  NVMID_STACKER_MOT_DOWN_COUNT) ;

	}
	return;
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_ProcessEndCheck
// Description   :  ProcessEnd���삷�邩���`�F�b�N����֐��iCompile_st13����ړ�)
// Parameter     :  
// Return        : 
///////////////////////////////////////////////////////////////////
void   TRAY_STK_ProcessEndCheck(void)
{
	if ((gSTK.jobType == eSTK_JOB_EJECT) && ( gSTK.processEndFlag == true ) ) {

		PrintStackerTrayLog_Short((UC)eSTK_LOG_PROCESSEND_CHECK,  (US)0x00,  (US)gSTK.jobType , (US)gSTK.stackerPulseCnt); 

		gSTK.processEndFlag = false;
		TRAY_STK_StartAct(TRAY_STK_END , gSTK.preSheetNo);
		
	} else if (gSTK.processEndFlag == true )  { //Abnormal case .. 

		PrintStackerTrayLog_Short((UC)eSTK_LOG_PROCESSEND_CHECK,	  (US)0xFF,   (US)gSTK.jobType , (US)gSTK.stackerPulseCnt); 

	}

}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_StandbyFailCheckStart
// Description   :  FullStack/LowerSafetyWarning������AStandby����SNR�ω�����ꍇ�AStacker����Start 
// Parameter     :  
// Return        : 
///////////////////////////////////////////////////////////////////
void   TRAY_STK_StandbyFailCheckStart(void)
{
	gSTK.standbyFailCheckFlag = true;
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_StandbyFailCheckStop
// Description   :  FullStack/LowerSafetyWarning������AStandby����SNR�ω�����ꍇ�AStacker����Stop
// Parameter     :  
// Return        : 
///////////////////////////////////////////////////////////////////
void   TRAY_STK_StandbyFailCheckStop(void)
{
	gSTK.standbyFailCheckFlag = false;
}


///////////////////////////////////////////////////////////////////
// Function Name : TRNS_InformedCompoControlComplete
// Description   : Module Action Complete(Transport Internal I/F)�ʒm��M����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_InformedCompoControlComplete(US aChainNo, US aLinkNo, EAction aAction)
{
	EvtParamComponentControlStopNotify aEvtParameter;

	aEvtParameter.chainNo = aChainNo;
	aEvtParameter.linkNo = aLinkNo;

	if ( aAction == ACT_START ) {
		MSP_SendMessage(MID_TRAY, EVT_DIAG_COMPO_CONT_START, (const UC*)&aEvtParameter, sizeof(aEvtParameter), LOG_OFF);
	} else {																		// ACT_STOP
		MSP_SendMessage(MID_TRAY, EVT_DIAG_COMPO_CONT_STOP, (const UC*)&aEvtParameter, sizeof(aEvtParameter), LOG_OFF);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_InformedCompoControlStart
// Description   : TRNS Module��Component Control Action�J�n��ʒm����
// Parameter     : aChainLink(0-15bit:ChainNo�A16-31bit:LinkNo)
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_InformedCompoControlStart(SS aTimerID, UL aChainLink)
{
	US aChainNo = (US)(aChainLink & 0x0000FFFF);
	US aLinkNo = (US)((aChainLink >> 16) & 0x0000FFFF);
	TRAY_InformedCompoControlComplete(aChainNo, aLinkNo, ACT_START);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_Device_ComponentControl
// Description   :
// Parameter     :
// Return        :
///////////////////////////////////////////////////////////////////
void TRAY_Device_ComponentControl(US aChainNo, US aLinkNo, EAction aAction)
{
	if ( aAction == ACT_START ) {
		TRAY_Device_StartCompoLiftUpDown(aLinkNo);
	} else { // ACT_STOP
		TRAY_Device_StopCompoLiftUpDown(aLinkNo);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Device_StartCompoLiftUpDown
// Description   :
// Parameter     :
// Return        :
///////////////////////////////////////////////////////////////////
void TRAY_Device_StartCompoLiftUpDown(US aLinkNo)
{
	ETRAY_STK_Action aAction;
	UL aChainLink = (UL)(TRAY_COMPO_CHAIN) | ((UL)(aLinkNo) << 16);
	MSP_SetEventTimer(0, TRAY_InformedCompoControlStart, aChainLink);

	aAction = (ETRAY_STK_Action)aLinkNo;
	switch ( aAction ) {
	case TRAY_STK_COMPO_COMPILE :
	case TRAY_STK_COMPO_EJECT :
		TRAY_STK_StartAct(aAction , 0);	// Link No =
		break;

	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Device_StopCompoLiftUpDown
// Description   :
// Parameter     :
// Return        :
///////////////////////////////////////////////////////////////////
void TRAY_Device_StopCompoLiftUpDown(US aLinkNo)
{
	ETRAY_STK_Action aAction;

	aAction = (ETRAY_STK_Action)aLinkNo;
	switch ( aAction ) {
	case TRAY_STK_COMPO_COMPILE :
	case TRAY_STK_COMPO_EJECT :
		TRAY_InformedCompoControlComplete((US)TRAY_COMPO_CHAIN, aLinkNo, ACT_STOP);
		TRAY_STK_DeviceStop();
		break;

	default:
		break;
	}
}


//Stacker Home�ʒu�ֈړ�()
//TRAY_STK_HOME = 0x0300,
///////////////////////////////////////////////////////////////////
// Function Name : Home_st1_1
// Description  ;  //�J�n�^�C�~���O�ݒ�
// Parameter     : �]    // Return        : �]
// History : V3.6.2 �d�l�ύX�FStackerHome����J�nTrigger��NVM��EjectHome���쎞��+30ms�o��
///////////////////////////////////////////////////////////////////
void Home_st1_1()
{
	US    aEjectHomeTime = CMPL_GetEjectHomeNeedTime(gSTK.sheetNo) + STK_MOT_ENABLE_TIME_30;

	TRAY_STK_SetEventTimer( aEjectHomeTime,	 eSTK_NORMAL);

	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log
}

///////////////////////////////////////////////////////////////////
// Function Name : Home_st1_2
// Description  ;  //HeightSNR1�C2�ɂ��ʒu���f
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st1_2()
{
	ETRAY_STK_Action   action = gSTK.action;
	UC  	sheetNo = gSTK.sheetNo;


	if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == HIGH) { // Lift UP

	     gSTK.action = TRAY_STK_HOME_st2_UP;
	     PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log

	} else  {

		if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT2_SNR) == HIGH) {//Lift Down

			gSTK.action = TRAY_STK_HOME_st2_DN;
			PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log

		} else {  //SetClamp �ޔ����

			gSTK.action = TRAY_STK_st0;
			gSTK.sheetNo = SHEETNO_NUM;
			//0502
			//TRAY_Manager_InformedStackerActionComplete(TRAY_STK_HOME,  sheetNo); //0502
			TRAY_Manager_InformedStackerActionComplete(TRAY_STK_COMPILE,  sheetNo);
			
			PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log
			return;
		}
	}
	TRAY_STK_ControlThread();

}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st2_UP
// Description  ;	//���[�^�쓮�ݒ�
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st2_UP()
{
	//�@�����, ��d��
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_ENB,  ON);
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_DIR,  STK_DIR_UP);
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, LOW);

	TRAY_STK_SetEventTimer(STK_MOT_ENABLE_TIME_30,	eSTK_NORMAL);

}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st3_UP
// Description ;  //���[�^�쓮�J�n(�����)
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st3_UP()
{
	//���d��
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, HIGH);

	// Stacker Mot Start
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_StartClock(DD_STM_ID_STACKER, PROF_PULSE_STEP_45, TRAY_STK_ReceiveStartClock);

	gSTK.startClock = START_CLK_WAIT;  //Set Wait Flag
	gSTK.beforePulse = 0;

	//Clock Count Start ..
	DD_STM_StartCountClock(DD_STM_ID_STACKER);

	//Fail Timer
//	TRAY_STK_SetEventTimer(STK_FAIL_TIME_1000,	  eSTK_TRAY_FAIL_1);
	TRAY_STK_SetEventTimer(STK_FAIL_TIME_5000,	  eSTK_TRAY_FAIL_2);

	// Reset Start ...
	if (  DD_DIO_GetInput(DD_DI_STACKER_NO_PAPER_FULL_SNR) == LOW) {

		gSTK.action = TRAY_STK_HOME_st4_UP_ONWait;

	} else {

		gSTK.action = TRAY_STK_HOME_st4_UP_OFFWait;
	}

}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st4_UP_ONWait
// Description ; //NoPaperSNR�@ON�҂�
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st4_UP_ONWait()
{ //NO ACTION ..
}

///////////////////////////////////////////////////////////////////
// Function Name : Home_st4_UP_OFFWait
// Description ; //NoPaperSNR�@OFF�҂�
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st4_UP_OFFWait()
{ //NO ACTION ..
}

///////////////////////////////////////////////////////////////////
// Function Name : Home_st5_UP
// Description;  //�@Reset�J�n
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st5_UP()
{
	// Reset���쒆�ASNR1=OFF�ƂȂ����ꍇ��Pulse�v�Z�̂���, Save���Ă���
	gSTK.beforePulse = DD_STM_StopCountClock(DD_STM_ID_STACKER);

	//Restart Pulse Count
	DD_STM_StartCountClock(DD_STM_ID_STACKER);
}

///////////////////////////////////////////////////////////////////
// Function Name : Home_st6_UP
// Description ;  //�@Reset����
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st6_UP()
{
	UL	  movePls;

	//Reset�J�n�`Reset������ԃp���X�v�Z
	movePls = DD_STM_StopCountClock(DD_STM_ID_STACKER);

	//Restart  Pulse Count
	DD_STM_StartCountClock(DD_STM_ID_STACKER);

	TRAY_STK_ResetPulse(movePls);

	gSTK.beforePulse = 0;  //Pulse�X�V���I������̂�Clear����

}

///////////////////////////////////////////////////////////////////
// Function Name : Home_st7_UP
// Description ;  //�@SNR1�@OFF��M(10Pulse�J�n)
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st7_UP()
{
	UL     movePls;

	// ResetPulse�J�E���g�̂��߁A�r���Œ��f����Pulse���v�Z����
	movePls = DD_STM_StopCountClock(DD_STM_ID_STACKER) +  gSTK.beforePulse;
	gSTK.beforePulse = 0;
	TRAY_STK_UpdatePulse( STK_DIR_UP, movePls);

	//10Pulse start ..
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_AppointPulseTiming(DD_STM_ID_STACKER, 10, TRAY_STK_ReceiveSteppingMotorEvent);
	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log

}

///////////////////////////////////////////////////////////////////
// Function Name : Home_st8_UP
// Description; //�@StepDown�J�n
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st8_UP()
{
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_SlowDownStop(DD_STM_ID_STACKER, TRAY_STK_ReceiveSteppingMotorEvent);
	
	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log
}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st9_UP
// Description;  //�@�^�C�}�ݒ�
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st9_UP()
{
	// 10Pulse + StepDown
	TRAY_STK_UpdatePulse( STK_DIR_UP, (UL)(10L+PROF_PULSE_STEP_45));

	TRAY_STK_MotorUpDown_NVMUpdate(STK_DIR_UP);  //V3.0

	//50ms Timer set
	TRAY_STK_SetEventTimer(STK_MOT_SWITCH_TIME_50, eSTK_NORMAL);
}

///////////////////////////////////////////////////////////////////
// Function Name : Home_st2_DN
// Description  ;  //���[�^�쓮�ݒ�
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st2_DN()
{
	//Mot Enable,�@�������A ��d��
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_ENB,  ON);
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_DIR,  STK_DIR_DOWN);
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, LOW);

	TRAY_STK_SetEventTimer(STK_MOT_ENABLE_TIME_30,  eSTK_NORMAL);
}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st3_DN
// Description ;  //���[�^�쓮�J�n(������)
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st3_DN()
{
	//���d��
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, HIGH);

	// Stacker Height1 SNR ON����H3Pulse��AStepDown
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_StartClock(DD_STM_ID_STACKER, PROF_PULSE_STEP_45, TRAY_STK_ReceiveStartClock);

	gSTK.startClock = START_CLK_WAIT;  //Set Wait Flag
	gSTK.beforePulse = 0;

	//Clock Count Start ..
	DD_STM_StartCountClock(DD_STM_ID_STACKER);
	
	// Fail Timer (Stacker LowerSafety Warning Counter)
	TRAY_STK_SetEventTimer(STK_FAIL_TIME_1000, eSTK_LOWER_SAFETY);
	//  (V3.6.92)1000ms Caution .. PitchSkip �v��
	TRAY_STK_SetEventTimer(STK_FAIL_TIME_1000, eSTK_PITCHSKIP_CHK);
	
}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st4_DN
// Description ;  //SNR1�@ON��M(H3Pulse�J�n)
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st4_DN()
{
	US	 h_Pulse;
	US   afterPls;
	US   movePls;

	 //CR-008
	h_Pulse = (US)( SRVC_GetNvmValueUS(NVMID_COMPILE_POSITION_H3) * 10) ;//�P�ʕύX : 10count 

	afterPls = DD_STM_StopCountClock(DD_STM_ID_STACKER);

	movePls = gSTK.beforePulse + afterPls;

	TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)movePls);
	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log

	if ( gSTK.beforePulse > 0) {  // StepUp���ASNR1 ON

		if ( h_Pulse > afterPls) {

			movePls = h_Pulse - afterPls;  // StepUp���ASNR ON�����Ƃ�����̈ړ��p���X��������

			//Stacker Height1 SNR ON����H3Pulse��AStepDown
			DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
			DD_STM_AppointPulseTiming(DD_STM_ID_STACKER, movePls, TRAY_STK_ReceiveSteppingMotorEvent);

			TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)movePls);

		} else {  // H3-Pulse ����-> StepDown

			gSTK.action = TRAY_STK_HOME_st5_DN;
			TRAY_STK_ControlThread();
		}

		gSTK.beforePulse=0;

	} else { // gSTK.beforePulse==0

		if ( h_Pulse > 0)  {

			//Stacker Height1 SNR ON����H3Pulse��AStepDown
			DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
			DD_STM_AppointPulseTiming(DD_STM_ID_STACKER, h_Pulse, TRAY_STK_ReceiveSteppingMotorEvent);

			TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)h_Pulse);

		} else {	// H3-Pulse ����-> StepDown

			gSTK.action = TRAY_STK_HOME_st5_DN;
			TRAY_STK_ControlThread();
		}
	}

}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st5_DN
// Description;  //StepDown�J�n�@
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st5_DN()
{
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_SlowDownStop(DD_STM_ID_STACKER, TRAY_STK_ReceiveSteppingMotorEvent);
	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log

}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st6_DN
// Description;  //Stacker�ҋ@�ʒu�ֈړ�����J�nTrigger�҂�
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st6_DN()
{
	//��d��
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, LOW);

	TRAY_STK_UpdatePulse( STK_DIR_DOWN, PROF_PULSE_STEP_45); //44

	TRAY_STK_MotorUpDown_NVMUpdate(STK_DIR_DOWN);  //V3.0

	if ( gSTK.ejectRecvFlag == true ) {  //Eject Job  finish..
	
		PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log

		gSTK.ejectRecvFlag = false;
		gSTK.action++;
		TRAY_STK_ControlThread();

	}

}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st7_DN
// Description;  //���[�^�쓮�J�n(�����)
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st7_DN()
{
	//�@�����, ���d��
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_DIR,  STK_DIR_UP);
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, HIGH);


	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_StartClock(DD_STM_ID_STACKER, PROF_PULSE_STEP_45, TRAY_STK_ReceiveStartClock);

	gSTK.startClock = START_CLK_WAIT;  //Set Wait Flag
	gSTK.beforePulse = 0;

	//Clock Count Start ..
	DD_STM_StartCountClock(DD_STM_ID_STACKER);

// 0519  Reset function added...
	//Fail Timer
//	TRAY_STK_SetEventTimer(STK_FAIL_TIME_1000,	  eSTK_TRAY_FAIL_1);
	TRAY_STK_SetEventTimer(STK_FAIL_TIME_5000,	  eSTK_TRAY_FAIL_2);

	// Reset Start ...
	if (  DD_DIO_GetInput(DD_DI_STACKER_NO_PAPER_FULL_SNR) == LOW) {

		gSTK.action = TRAY_STK_HOME_st20_ONWait;

	} else {

		gSTK.action = TRAY_STK_HOME_st20_OFFWait;
	}

}

///��������������������������������(0519)
//////////////////////////////////////////////////////////////////////////
// Stacker�ҋ@�ʒu�ֈړ�����J�nTrigger����Reset����J�n
///////////////////////////////////////////////////////////////////
// Function Name : Home_st20_ONWait
// Description ; //NoPaperSNR�@ON�҂�
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st20_ONWait()
{ //NO ACTION ..
}

///////////////////////////////////////////////////////////////////
// Function Name : Home_st20_OFFWait
// Description ; //NoPaperSNR�@OFF�҂�
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st20_OFFWait()
{ //NO ACTION ..
}

///////////////////////////////////////////////////////////////////
// Function Name : Home_st21
// Description;  //�@Reset�J�n
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st21()
{
	// Reset���쒆�ASNR1=OFF�ƂȂ����ꍇ��Pulse�v�Z�̂���, Save���Ă���
	gSTK.beforePulse = DD_STM_StopCountClock(DD_STM_ID_STACKER);

	//Restart Pulse Count
	DD_STM_StartCountClock(DD_STM_ID_STACKER);
}

///////////////////////////////////////////////////////////////////
// Function Name : Home_st6_UP
// Description ;  //�@Reset����
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st22()
{
	UL	  movePls;

	//Reset�J�n�`Reset������ԃp���X�v�Z
	movePls = DD_STM_StopCountClock(DD_STM_ID_STACKER);

	//Restart  Pulse Count
	DD_STM_StartCountClock(DD_STM_ID_STACKER);

	TRAY_STK_ResetPulse(movePls);

	gSTK.beforePulse = 0;  //Pulse�X�V���I������̂�Clear����

}
//��������������������������������
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : Home_st8_DN
// Description ;  //SNR1�@OFF��M(10Pulse�J�n)
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st8_DN()
{
	US  movePls;

	// ResetPulse�J�E���g�̂��߁A�r���Œ��f����Pulse���v�Z����
	movePls = DD_STM_StopCountClock(DD_STM_ID_STACKER) +  gSTK.beforePulse; //movePls = DD_STM_StopCountClock(DD_STM_ID_STACKER);
	gSTK.beforePulse = 0;

	TRAY_STK_UpdatePulse( STK_DIR_UP, (UL)movePls);
	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log

	// Stacker Height1 SNR OFF���� 10Pulse��AStepDown
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_AppointPulseTiming(DD_STM_ID_STACKER, 10, TRAY_STK_ReceiveSteppingMotorEvent);

}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st9_DN
// Description;  //StepDown�J�n
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st9_DN()
{
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_SlowDownStop(DD_STM_ID_STACKER, TRAY_STK_ReceiveSteppingMotorEvent);
}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st10_DN
// Description ;	//��㎥
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st10_DN()
{
	TRAY_STK_UpdatePulse( STK_DIR_UP, (UL)(10L + PROF_PULSE_STEP_45) );
	
	TRAY_STK_MotorUpDown_NVMUpdate(STK_DIR_UP);  //V3.0

	TRAY_STK_SetEventTimer(STK_MOT_SWITCH_TIME_50,  eSTK_NORMAL);
}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st11_DN
// Description ;	//StackerHome���슮��
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Home_st11_DN()
{
	ETRAY_STK_Action   action = gSTK.action;
	UC  	sheetNo = gSTK.sheetNo;

	//��d��
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, LOW);
	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log

	// (CR-013) FullStack ��������Check
	 if ( SRVC_GetDeviceStatus(DSNO_STACKER_FULL_STACK) == DVST_TRAY_STK_STTS_NOT_FULL) {
		if( true == TRAY_Manager_ClearCheck()){
			//Lower Safety Warning Count Clear 
			//V3.4.0  T/B �������d�l(FullStack/LowerSafety������AStandby����SNR�ω�����ꍇ�AStacker����
			if (SRVC_GetDeviceStatus(DSNO_STACKER_LOW_SAFETY) == FAULT_OCCUR) {
				gSTK_FaultStat = FAULT_NOT_OCCUR;
				SRVC_UpdateDeviceStatus(FAULT_NOT_OCCUR, DSNO_STACKER_LOW_SAFETY) ; //Low Safety
			}
			gSTK.lowerSafetyCnt = 0;
		}
	 } else  {//FullStack 
	 	if ( true == TRAY_Manager_FullStackReleaseCondition() ) { //����
			//Lower Safety Warning Count Clear 
			//V3.4.0  T/B �������d�l(FullStack/LowerSafety������AStandby����SNR�ω�����ꍇ�AStacker����
			if (SRVC_GetDeviceStatus(DSNO_STACKER_LOW_SAFETY) == FAULT_OCCUR) {
				gSTK_FaultStat = FAULT_NOT_OCCUR;
				SRVC_UpdateDeviceStatus(FAULT_NOT_OCCUR, DSNO_STACKER_LOW_SAFETY) ; //Low Safety
			}
			gSTK.lowerSafetyCnt = 0;
			TRAY_STK_SetPulse_FullStackRelease(0); // Pulse = 0 Clear
	 	}	
	}

	gSTK.action = TRAY_STK_st0;
	gSTK.sheetNo = SHEETNO_NUM;
	gSTK.startClock = START_CLK_UNKNOWN;
	gSTK.beforePulse = 0;
	gSTK.ejectRecvFlag = false;

	// NVM Save ..
	SRVC_SetNvmValueUL(gSTK.stackerPulseCnt, NVMID_ELEVATOR_MOT_PULSE_COUNT);
	TRAY_Manager_InformedStackerActionComplete(TRAY_STK_HOME,	sheetNo);
}

//Compile�ʒu�ֈړ�
//TRAY_STK_COMPILE = 0x0400,
///////////////////////////////////////////////////////////////////
// Function Name : Compile_st1
// Description ;  //StackerHeightSNR�ɂ�镪�򔻒f
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Compile_st1()
{
	ETRAY_STK_Action   action = gSTK.action;
	UC  	sheetNo = gSTK.sheetNo;
	

	if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == HIGH) { // Lift UP

		if ( true == gSTK.standbyFailCheckFlag ) {
		    //FullStack/LowerSafety������AStandby����SNR�ω�����ꍇ�AStacker����Start ...
		    PrintStackerTrayLog_Short((UC)eSTK_LOG_DO_ORNOT,	0x00, (US)gSTK.action, sheetNo); 

		}//V3.3.92  TestROM :	�r�o���쎞�A �����ԁFH1,H2���O�łȂ��ꍇ�FLiftUp���{
		else if ( ( gSTK.jobType != eSTK_JOB_SHT_INIT) &&
			( gSTK.compoAction != TRAY_STK_COMPO_COMPILE) &&
			((DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT2_SNR) == HIGH) )) { //

			if (TRAY_STK_Get_HPulse() == 0)  { // H1,H2���O�̏ꍇ�F���삵�Ȃ�

				gSTK.action = TRAY_STK_st0;
				gSTK.sheetNo = SHEETNO_NUM;
				PrintStackerTrayLog_Short((UC)eSTK_LOG_DO_ORNOT,	0xFF, (US)gSTK.action, sheetNo); 
				//Stacker�����̍X�V�͗v��Ȃ�: ->PF-DVT T/B.. V3.5.1 SetOVer
				TRAY_Manager_InformedStackerActionComplete(TRAY_STK_COMPILE,  sheetNo);
				TRAY_STK_ProcessEndCheck();
				return;
			}

		} else { // H1SNR=ON, H2SNR=OFF ,  or JobInit  or Component 

		}
		
	     gSTK.action = TRAY_STK_COMPILE_st2_UP;
	    PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,  (US)gSTK.action, (US)gSTK.stackerPulseCnt, sheetNo); 

	} else  {

		if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT2_SNR) == HIGH) {//Lift Down

			gSTK.action = TRAY_STK_COMPILE_st2_DN;
			PrintStackerTrayLog_Short((UC)eSTK_LOG_UPDOWN, 0, (US)gSTK.action , sheetNo); //Log

		} else {  //SetClamp �ޔ����

			gSTK.action = TRAY_STK_st0;
			gSTK.sheetNo = SHEETNO_NUM;
			PrintStackerTrayLog_Short((UC)eSTK_LOG_UPDOWN, 0xFF,gSTK.action, sheetNo); //Log

			TRAY_Manager_InformedStackerActionComplete(TRAY_STK_COMPILE,  sheetNo);

			//Diag Component
			if ( gSTK.compoAction == TRAY_STK_COMPO_COMPILE) {
				TRAY_Manager_InformedStackerActionComplete((US) gSTK.compoAction ,  sheetNo);
				gSTK.compoAction = TRAY_STK_st0;
			}
			return;
		}
	}
	TRAY_STK_ControlThread();

}

///////////////////////////////////////////////////////////////////
// Function Name : Compile_st1
// Description;  //���[�^�쓮�ݒ�
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Compile_st2_UP()
{
	//�@�����, ��d��
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_ENB,  ON);
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_DIR,  STK_DIR_UP);
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, LOW);

	TRAY_STK_SetEventTimer(STK_MOT_ENABLE_TIME_30,	eSTK_NORMAL);

}

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st3_UP
 // Description;   //���[�^�쓮�J�n
 // Parameter	  : �]	  // Return 	   : �]
 ///////////////////////////////////////////////////////////////////
void Compile_st3_UP()
{
	 //���d��
	 DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, HIGH);

	 // Stacker Height1 SNR OFF����10Pulse��AStepDown
	 DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	 DD_STM_StartClock(DD_STM_ID_STACKER, PROF_PULSE_STEP_45, TRAY_STK_ReceiveStartClock);

	 gSTK.startClock = START_CLK_WAIT;	//Set Wait Flag
	 gSTK.beforePulse = 0;

	 //Clock Count Start ..
	 DD_STM_StartCountClock(DD_STM_ID_STACKER);

	 //Fail Timer
//	 TRAY_STK_SetEventTimer(STK_FAIL_TIME_1000,    eSTK_TRAY_FAIL_1);
	 TRAY_STK_SetEventTimer(STK_FAIL_TIME_5000,    eSTK_TRAY_FAIL_2);
	 // (V3.6.92)1000ms Caution .. PitchSkip �v��
	 TRAY_STK_SetEventTimer(STK_FAIL_TIME_1000, 	eSTK_PITCHSKIP_CHK);
	
	 // Reset Start ...
	 if (  DD_DIO_GetInput(DD_DI_STACKER_NO_PAPER_FULL_SNR) == LOW) {

		 gSTK.action = TRAY_STK_COMPILE_st4_UP_ONWait;

	 } else {

		 gSTK.action = TRAY_STK_COMPILE_st4_UP_OFFWait;
	 }
	  PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,  (US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 

}

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st4_UP_ONWait
 // Description;   //NoPaperSNR�@ON�҂�
 // Parameter	  : �]	  // Return 	   : �]
 ///////////////////////////////////////////////////////////////////
void Compile_st4_UP_ONWait()
{//NO ACTION
}

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st4_UP_OFFWait
 // Description;  //NoPaperSNR�@OFF�҂�
 // Parameter	  : �]	  // Return 	   : �]
 ///////////////////////////////////////////////////////////////////
void Compile_st4_UP_OFFWait()
{//NO ACTION
 }

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st5_UP
 // Description ; ;   //Reset�J�n
 // Parameter	  : �]	  // Return 	   : �]
 ///////////////////////////////////////////////////////////////////
void Compile_st5_UP()
{
	 // Reset���쒆�ASNR1=OFF�ƂȂ����ꍇ��Pulse�v�Z�̂���, Save���Ă���
	 gSTK.beforePulse = DD_STM_StopCountClock(DD_STM_ID_STACKER);

	 // Restart Pulse Count
	 DD_STM_StartCountClock(DD_STM_ID_STACKER);
	 PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,  (US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 

}

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st6_UP
 // Description;   //Reset����
 // Parameter	  : �]	  // Return 	   : �]
 ///////////////////////////////////////////////////////////////////
void Compile_st6_UP()
{
	 UL    movePls;

	 //Reset�J�n�`Reset������ԃp���X�v�Z
	 movePls = DD_STM_StopCountClock(DD_STM_ID_STACKER);

	 //Restart	Pulse Count
	 DD_STM_StartCountClock(DD_STM_ID_STACKER);

	 TRAY_STK_ResetPulse(movePls);

	 gSTK.beforePulse = 0;  //Pulse�X�V���I������̂�Clear����

}

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st7_UP
 // Description;   //SNR1�@OFF��M(10Pulse�J�n)
 // Parameter	  : �]	  // Return 	   : �]
 ///////////////////////////////////////////////////////////////////
void Compile_st7_UP()
{
	 UL 	movePls;

	 // ResetPulse�J�E���g�̂��߁A�r���Œ��f����Pulse���v�Z����
	 movePls = DD_STM_StopCountClock(DD_STM_ID_STACKER) +  gSTK.beforePulse;

	 gSTK.beforePulse = 0;
	 TRAY_STK_UpdatePulse( STK_DIR_UP, movePls);
	 PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log

	 //10Pulse start ..
	 DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	 DD_STM_AppointPulseTiming(DD_STM_ID_STACKER, 10, TRAY_STK_ReceiveSteppingMotorEvent);

 }

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st8_UP
 // Description ;   //StepDown�J�n
 // Parameter	  : �]	  // Return 	   : �]
 ///////////////////////////////////////////////////////////////////
void Compile_st8_UP()
{
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	 DD_STM_SlowDownStop(DD_STM_ID_STACKER, TRAY_STK_ReceiveSteppingMotorEvent);
	 PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,  (US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 

 }

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st9_UP
 // Description;   //�^�C�}(�����]���p)�ݒ�
 // Parameter	  : �]	  // Return 	   : �]
 ///////////////////////////////////////////////////////////////////
void Compile_st9_UP()
{
	 // 10Pulse + StepDown
	 TRAY_STK_UpdatePulse( STK_DIR_UP, (UL)(10L+PROF_PULSE_STEP_45));

	 TRAY_STK_MotorUpDown_NVMUpdate(STK_DIR_UP);  //V3.0
	 PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,  (US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 

	 //50ms Timer set
	 TRAY_STK_SetEventTimer(STK_MOT_SWITCH_TIME_50, eSTK_NORMAL);

}

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st10_UP
 // Description;  //���[�^�쓮�J�n(������)
 // Parameter	  : �]	  // Return 	   : �]
 ///////////////////////////////////////////////////////////////////
void Compile_st10_UP()
{
		
	// (CR-013) FullStack ��������Check
	 if ( SRVC_GetDeviceStatus(DSNO_STACKER_FULL_STACK) == DVST_TRAY_STK_STTS_NOT_FULL) {
		if( true == TRAY_Manager_ClearCheck()){
			//Lower Safety Warning Count Clear 
			//V3.4.0  T/B �������d�l(FullStack/LowerSafety������AStandby����SNR�ω�����ꍇ�AStacker����
			if (SRVC_GetDeviceStatus(DSNO_STACKER_LOW_SAFETY) == FAULT_OCCUR) {
				gSTK_FaultStat = FAULT_NOT_OCCUR;
				SRVC_UpdateDeviceStatus(FAULT_NOT_OCCUR, DSNO_STACKER_LOW_SAFETY) ; //Low Safety
			}
			gSTK.lowerSafetyCnt = 0;
		}
	 } else  {//FullStack 
	 	if ( true == TRAY_Manager_FullStackReleaseCondition() ) { //����
			//Lower Safety Warning Count Clear 
			//V3.4.0  T/B �������d�l(FullStack/LowerSafety������AStandby����SNR�ω�����ꍇ�AStacker����
			if (SRVC_GetDeviceStatus(DSNO_STACKER_LOW_SAFETY) == FAULT_OCCUR) {
				gSTK_FaultStat = FAULT_NOT_OCCUR;
				SRVC_UpdateDeviceStatus(FAULT_NOT_OCCUR, DSNO_STACKER_LOW_SAFETY) ; //Low Safety
			}
			gSTK.lowerSafetyCnt = 0;
			TRAY_STK_SetPulse_FullStackRelease(0); // Pulse = 0 Clear
	 	}	
	}
	
	//�@�������A
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_DIR,  STK_DIR_DOWN);
	// ���d��
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, HIGH);

	// Stacker Height1 SNR ON����H3Pulse��AStepDown
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_StartClock(DD_STM_ID_STACKER, PROF_PULSE_STEP_45, TRAY_STK_ReceiveStartClock);

	gSTK.startClock = START_CLK_WAIT;  //Set Wait Flag
	gSTK.beforePulse = 0;

	//Clock Count Start ..
	DD_STM_StartCountClock(DD_STM_ID_STACKER);
	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	(US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 

 }


 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st11_UP
 // Description;   //SNR1�@ON��M(G-Pulse�J�n)
 // Parameter	  : �]	  // Return 	   : �]
 ///////////////////////////////////////////////////////////////////
void Compile_st11_UP()
 {

	US   G_Pulse;
	US	 afterPls,  movePls;

	  // Stacker Height1 SNR ON���� G-Pulse��AStepDown
	G_Pulse =TRAY_STK_Get_GPulse();

	afterPls = DD_STM_StopCountClock(DD_STM_ID_STACKER);

	movePls = gSTK.beforePulse + afterPls;

	TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)movePls);
	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	(US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 


	if ( gSTK.beforePulse > 0) {  // StepUp���ASNR1 ON

		if ( G_Pulse > afterPls) {

			movePls = G_Pulse - afterPls;  // StepUp���ASNR ON�����Ƃ�����̈ړ��p���X��������

			//Stacker Height1 SNR ON���� G-Pulse��AStepDown
			DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
			DD_STM_AppointPulseTiming(DD_STM_ID_STACKER, movePls, TRAY_STK_ReceiveSteppingMotorEvent);

			TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)movePls);

		} else {  // G-Pulse ����-> StepDown

			gSTK.action = TRAY_STK_COMPILE_st12_UP;
			TRAY_STK_ControlThread();
		}

		gSTK.beforePulse=0;

	} else { // gSTK.beforePulse==0

		if ( G_Pulse > 0)  {

			//Stacker Height1 SNR ON����G-Pulse��AStepDown
			DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
			DD_STM_AppointPulseTiming(DD_STM_ID_STACKER, G_Pulse, TRAY_STK_ReceiveSteppingMotorEvent);

			TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)G_Pulse);

		} else {	// G-Pulse ����-> StepDown

			gSTK.action = TRAY_STK_COMPILE_st12_UP;
			TRAY_STK_ControlThread();
		}
	}

 }

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st12_UP
 // Description ;   //StepDown�J�n
 // Parameter	  : �]	  // Return 	   : �]
 ///////////////////////////////////////////////////////////////////
 void Compile_st12_UP()
{
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	 DD_STM_SlowDownStop(DD_STM_ID_STACKER, TRAY_STK_ReceiveSteppingMotorEvent);

	 TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)PROF_PULSE_STEP_45);
 	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	(US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 
 }

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st2_DN
 // Description;  //���[�^�쓮�ݒ�
 // Parameter	  : �]	  // Return 	   : �]
 ///////////////////////////////////////////////////////////////////
void Compile_st2_DN()
{
	 //Mot Enable,�@�������A ��d��
	 DD_DIO_SetOutput(DD_DO_STACKER_MOT_ENB,  ON);
	 DD_DIO_SetOutput(DD_DO_STACKER_MOT_DIR,  STK_DIR_DOWN);
	 DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, LOW);

	 TRAY_STK_SetEventTimer(STK_MOT_ENABLE_TIME_30,  eSTK_NORMAL);

 }

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st3_DN
 // Description ; ;   //���[�^�쓮�J�n
 // Parameter	  : �]	  // Return 	   : �]
 ///////////////////////////////////////////////////////////////////
void Compile_st3_DN()
{
	 //���d��
	 DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, HIGH);

	// Stacker Height1 SNR ON����H3Pulse��AStepDown
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_StartClock(DD_STM_ID_STACKER, PROF_PULSE_STEP_45, TRAY_STK_ReceiveStartClock);

	gSTK.startClock = START_CLK_WAIT;  //Set Wait Flag
	gSTK.beforePulse = 0;

	//Clock Count Start ..
	DD_STM_StartCountClock(DD_STM_ID_STACKER);

	 // Fail Timer (Stacker LowerSafety Warning Counter)
	 TRAY_STK_SetEventTimer(STK_FAIL_TIME_1000, eSTK_LOWER_SAFETY);
	 PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,  (US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 

 }

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st4_DN()
 // Description;  //SNR1 ON��M(G-Pulse�J�n)
 // Parameter	  : �]	  // Return 	   : �]
 ///////////////////////////////////////////////////////////////////
void Compile_st4_DN()
{
	 US   G_Pulse;
	 US   afterPls,  movePls;


	  // Stacker Height1 SNR ON���� G-Pulse��AStepDown
	G_Pulse = TRAY_STK_Get_GPulse();

	 afterPls = DD_STM_StopCountClock(DD_STM_ID_STACKER);

	 movePls = gSTK.beforePulse + afterPls;

	 TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)movePls);
	 PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,  gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 


	 if ( gSTK.beforePulse > 0) {  // StepUp���ASNR1 ON

		 if ( G_Pulse > afterPls) {

			 movePls = G_Pulse - afterPls;	// StepUp���ASNR ON�����Ƃ�����̈ړ��p���X��������

			 //Stacker Height1 SNR ON���� G-Pulse��AStepDown
			 DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
			 DD_STM_AppointPulseTiming(DD_STM_ID_STACKER, movePls, TRAY_STK_ReceiveSteppingMotorEvent);

			 TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)movePls);

		 } else {  // G-Pulse ����-> StepDown

			 gSTK.action = TRAY_STK_COMPILE_st5_DN;
			 TRAY_STK_ControlThread();
		 }

		 gSTK.beforePulse=0;

	 } else { // gSTK.beforePulse==0

		 if ( G_Pulse > 0)	{

			 //Stacker Height1 SNR ON����G-Pulse��AStepDown
			 DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
			 DD_STM_AppointPulseTiming(DD_STM_ID_STACKER, G_Pulse, TRAY_STK_ReceiveSteppingMotorEvent);

			 TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)G_Pulse);

		 } else {	 // G-Pulse ����-> StepDown

			 gSTK.action = TRAY_STK_COMPILE_st5_DN;
			 TRAY_STK_ControlThread();
		 }
	 }

 }

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st5_DN
 // Description;   //StepDown�J�n
 // Parameter	  : �]	  // Return 	   : �]
 ///////////////////////////////////////////////////////////////////
void Compile_st5_DN()
{
	 DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	 DD_STM_SlowDownStop(DD_STM_ID_STACKER, TRAY_STK_ReceiveSteppingMotorEvent);

	 TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)PROF_PULSE_STEP_45);
	  //V03.00.90  PreDVT T/B �΍�
	 if ( gSTK.jobType != eSTK_JOB_SHT_INIT) 
  	 TRAY_Manager_PreviousPulseUpdate(gSTK.stackerPulseCnt); 
	  //������������
 }

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st13
 // Description; 	//Compile�ʒu�ֈړ�����
 // Parameter	  : �]	  // Return 	   : �]
 ///////////////////////////////////////////////////////////////////
void Compile_st13()
{
	 ETRAY_STK_Action	 action = gSTK.action;
	 ETRAY_STK_Action compoAction = gSTK.compoAction;
	 UC 			 sheetNO = gSTK.sheetNo;

	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	(US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 

	gSTK.action = TRAY_STK_st0;
	gSTK.compoAction = TRAY_STK_st0;
	gSTK.sheetNo = SHEETNO_NUM;
	gSTK.startClock = START_CLK_UNKNOWN;
	gSTK.beforePulse = 0;

	
	//��d��
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, LOW);

	TRAY_STK_MotorUpDown_NVMUpdate(STK_DIR_DOWN);  //V3.0
	
   	if ( (compoAction == TRAY_STK_COMPO_COMPILE) || (compoAction == TRAY_STK_COMPO_EJECT) ) {
		   TRAY_Manager_InformedStackerActionComplete((US)compoAction,	sheetNO);
	 } else {
		   SRVC_SetNvmValueUL(gSTK.stackerPulseCnt, NVMID_ELEVATOR_MOT_PULSE_COUNT); 
		   CNvmInformer_SaveNvm();

		   if ( false == gSTK.standbyFailCheckFlag ) { //StandbyFailCheck����łȂ��ꍇ
		   	
			   TRAY_Manager_InformedStackerActionComplete(TRAY_STK_COMPILE, sheetNO);

		   } else {
				// V3.4.0 Fault�����ׂăN���A���ꂽ��AFlag���N���A����
				//�������d�l�@FullStack/LowerSafety������AStandby����SNR�ω�����ꍇ�AStacker����
				//FullStackCheck .->. No FullStack && No LowerSafety
				if (( SRVC_GetDeviceStatus( DSNO_STACKER_FULL_STACK ) == DVST_TRAY_STK_STTS_NOT_FULL ) &&
					 (SRVC_GetDeviceStatus(DSNO_STACKER_LOW_SAFETY) == FAULT_NOT_OCCUR) ){
			 
					TRAY_STK_StandbyFailCheckStop();  
					return;
				}
		   }
	}
	
	// ProcessEnd Job Start Check ...-> �֐���
	if ( ( gSTK.jobType != eSTK_JOB_SHT_INIT) &&
		( gSTK.compoAction != TRAY_STK_COMPO_COMPILE) ) {
		TRAY_STK_ProcessEndCheck();
	}
}

//Eject�ʒu�ֈړ�(ADJUST)
//TRAY_STK_EJECT = 0x0500,
///////////////////////////////////////////////////////////////////
// Function Name : Eject_st1
// Description ;  //T1�^�C�}�҂�
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Eject_st1()
{
	if( SRVC_GetStapleMode(gSTK.sheetNo) == eCdiFinStaple_Off ) {  //UnStaple

		TRAY_STK_SetEventTimer(SRVC_GetNvmValueUS(NVMID_UN_STAPLE_STACKER_DOWN_TIMING_AT_EJECT)* 10 ,
								  eSTK_NORMAL);

	} else {

		TRAY_STK_SetEventTimer(SRVC_GetNvmValueUS(NVMID_CORNER_STAPLE_STACKER_DOWN_TIMING_AT_EJECT)* 10 ,
								  eSTK_NORMAL);
	}
//	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	(US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 
}

///////////////////////////////////////////////////////////////////
// Function Name : Eject_st2
// Description  ;  //���[�^�쓮�ݒ�
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Eject_st2()
{
	//Mot Enable�A ��d��
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_ENB,  ON);	
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_DIR,  STK_DIR_DOWN);	
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, LOW);	

	TRAY_STK_SetEventTimer(STK_MOT_ENABLE_TIME_20,  eSTK_NORMAL);
//	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	(US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 
}

///////////////////////////////////////////////////////////////////
// Function Name : Eject_st3
// Description  ;  //���[�^�쓮�J�n
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Eject_st3()
{	
	US    totalPulse;
	UC    targetIndex;
	
	totalPulse = TRAY_STK_Get_HPulse();

	if(totalPulse  < 3) { //�@�ړ����Ȃ�
	
		gSTK.action =  TRAY_STK_EJECT_st4;
		TRAY_STK_ControlThread();

	} else {

		//���d��
		DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, HIGH);	
	
	
		TRAY_STK_GetProfilePulse(totalPulse, &targetIndex);

		DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
		//STK Motor Start 
		DD_STM_MoveBySetPulse(DD_STM_ID_STACKER, targetIndex, totalPulse, TRAY_STK_ReceiveSteppingMotorEvent);

		TRAY_STK_UpdatePulse( STK_DIR_DOWN, totalPulse);

		TRAY_STK_MotorUpDown_NVMUpdate(STK_DIR_DOWN);  //V3.0
		PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	(US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 
	//Fail Timer ..		
//		TRAY_STK_SetEventTimer(STK_FAIL_TIME_1000,	  eSTK_TRAY_FAIL_1);
	}

}

///////////////////////////////////////////////////////////////////
// Function Name :Eject_st4
// Description ;  //Eject�ʒu�ֈړ�����
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void Eject_st4()
{
	ETRAY_STK_Action   action = gSTK.action;
	UC  	sheetNo = gSTK.sheetNo;

	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	(US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 			

	//��d��
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, LOW);	

	gSTK.action = TRAY_STK_st0;
	gSTK.sheetNo = SHEETNO_NUM;

	
	//Diag Component
	if ( gSTK.compoAction == TRAY_STK_COMPO_EJECT ) {
		TRAY_Manager_InformedStackerActionComplete((US)gSTK.compoAction,  sheetNo);
		gSTK.compoAction = TRAY_STK_st0;
	} else {
	
		SRVC_SetNvmValueUL(gSTK.stackerPulseCnt, NVMID_ELEVATOR_MOT_PULSE_COUNT);
		TRAY_Manager_InformedStackerActionComplete(TRAY_STK_EJECT,  sheetNo);
	}
}


//�ҋ@�ʒu�ֈړ�(ProcessEnd)
//TRAY_STK_END = 0x0600,
///////////////////////////////////////////////////////////////////
// Function Name : End_st1
// Description  ;   //���[�^�쓮�J�n
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void End_st1()
{
	ETRAY_STK_Action   action = gSTK.action;
	UC  	sheetNo = gSTK.sheetNo;

	if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == HIGH) { // Lift UP 
	
		//�@�����,���d��
		DD_DIO_SetOutput(DD_DO_STACKER_MOT_ENB,  ON);
		DD_DIO_SetOutput(DD_DO_STACKER_MOT_DIR,  STK_DIR_UP);
		DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, HIGH);	
		
		DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
		DD_STM_StartClock(DD_STM_ID_STACKER, PROF_PULSE_STEP_45, TRAY_STK_ReceiveStartClock);
		
		gSTK.startClock = START_CLK_WAIT;
		gSTK.beforePulse = 0;
		
	} else  {

		gSTK.action = TRAY_STK_st0;
		gSTK.sheetNo = SHEETNO_NUM;
		gSTK.processEndFlag = false; //V3.3.92

		TRAY_Manager_InformedStackerActionComplete(TRAY_STK_END,  sheetNo);
				
		return;
	}	
		
	//Clock Count Start ..
	DD_STM_StartCountClock(DD_STM_ID_STACKER);

	//Fail Timer
//	TRAY_STK_SetEventTimer(STK_FAIL_TIME_1000,	  eSTK_TRAY_FAIL_1);
	TRAY_STK_SetEventTimer(STK_FAIL_TIME_5000,	  eSTK_TRAY_FAIL_2);

	// Reset Start ...
	if (  DD_DIO_GetInput(DD_DI_STACKER_NO_PAPER_FULL_SNR) == LOW) {

		gSTK.action = TRAY_STK_END_st2_ONWait;

	} else {

		gSTK.action = TRAY_STK_END_st2_OFFWait;
	}
	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	(US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 			

}

///////////////////////////////////////////////////////////////////
// Function Name : End_st2_ONWait
// Description  ;   ;  //NoPaperSNR�@ON�҂�
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void End_st2_ONWait()
{//NO ACTION
}
///////////////////////////////////////////////////////////////////
// Function Name : End_st2_OFFWait
// Description  ; ;  //NoPaperSNR�@OFF�҂�
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void End_st2_OFFWait()
{//NO ACTION
}

///////////////////////////////////////////////////////////////////
// Function Name : End_st3
// Description  ;  ;  //Reset�J�n
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void End_st3()
{
	// Reset���쒆�ASNR1=OFF�ƂȂ����ꍇ��Pulse�v�Z�̂���, Save���Ă���
	gSTK.beforePulse = DD_STM_StopCountClock(DD_STM_ID_STACKER);
	
	// Restart Pulse Count 
	DD_STM_StartCountClock(DD_STM_ID_STACKER);
}

///////////////////////////////////////////////////////////////////
// Function Name : End_st4
// Description ;  //Reset����
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void End_st4()
{
	UL	  movePls;

	//Reset�J�n�`Reset������ԃp���X�v�Z
	movePls = DD_STM_StopCountClock(DD_STM_ID_STACKER);
	
	//Restart  Pulse Count 
	DD_STM_StartCountClock(DD_STM_ID_STACKER);
	
	TRAY_STK_ResetPulse(movePls);	

	gSTK.beforePulse = 0;  //Pulse�X�V���I������̂�Clear����

}

///////////////////////////////////////////////////////////////////
// Function Name : End_st5
// Description;  //SNR1�@OFF��M(10Pulse�J�n)
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void End_st5()
{
	UL	   movePls;
	
	// ResetPulse�J�E���g�̂��߁A�r���Œ��f����Pulse���v�Z����
	movePls = DD_STM_StopCountClock(DD_STM_ID_STACKER) +  gSTK.beforePulse;
	
	gSTK.beforePulse = 0;
	TRAY_STK_UpdatePulse( STK_DIR_UP, movePls);

	TRAY_STK_MotorUpDown_NVMUpdate(STK_DIR_UP);  //V3.0

	//10Pulse start .. 
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_AppointPulseTiming(DD_STM_ID_STACKER, 10, TRAY_STK_ReceiveSteppingMotorEvent);

}

///////////////////////////////////////////////////////////////////
// Function Name : End_st6
// Description ;  //StepDown�J�n
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void End_st6()
{
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_SlowDownStop(DD_STM_ID_STACKER, TRAY_STK_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : End_st7
// Description ; //��㎥
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
void End_st7()
{
	//��d��	
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, LOW); 

	TRAY_STK_UpdatePulse( STK_DIR_UP, (UL)(10L + PROF_PULSE_STEP_45));
	
	//50ms Timer set 
	TRAY_STK_SetEventTimer(STK_MOT_SWITCH_TIME_50, eSTK_NORMAL);

}

///////////////////////////////////////////////////////////////////
// Function Name : End_st8
// Description;  //�ҋ@�ʒu�ֈړ�����
// Parameter     : �]    // Return        : �]
///////////////////////////////////////////////////////////////////
 void End_st8()
{
	ETRAY_STK_Action    action = gSTK.action;
	UC				sheetNO = gSTK.sheetNo;
	
	//���[�^��~
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_ENB,  OFF);
	//Cancel Fail Timer..
//	TRAY_STK_CancelEventTimer(eSTK_TRAY_FAIL_1);
	TRAY_STK_CancelEventTimer(eSTK_TRAY_FAIL_2);
	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	(US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 	

	gSTK.action = TRAY_STK_st0;
	gSTK.sheetNo = SHEETNO_NUM;
	gSTK.startClock = START_CLK_UNKNOWN;
	gSTK.beforePulse = 0; 		
	gSTK.processEndFlag = false; //V3.3.92
	
	SRVC_SetNvmValueUL(gSTK.stackerPulseCnt, NVMID_ELEVATOR_MOT_PULSE_COUNT);
	CNvmInformer_SaveNvm();
	TRAY_Manager_InformedStackerActionComplete(TRAY_STK_END,  sheetNO);
		
}

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
