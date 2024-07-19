// File Name:	ECMPL_EJCT_BELT_Action.h
// Description:	Ejector Mot Device �̓�����`
// Engineer:	FXK  KimYS
// Last Edit:	15.02.26
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(ETRAY_STK_Action_h)
#define ETRAY_STK_Action_h

#define  TRAY_COMPO_CHAIN 12

typedef enum /* ETRAY_STK_Action */
{

	// STACKER ��~
	TRAY_STK_st0,	//IDLE 

	TRAY_STK_COMPO_COMPILE = 60,
	TRAY_STK_COMPO_EJECT = 61,

	// STACKER����(Compo) �J�n
	// STACKER MOT (Compile�ʒu�ֈړ�)
	// TRAY_STK_COMPO_COMPILE = 0x0100, 

	// STACKER MOT (Eject�ʒu�ֈړ�)
	// TRAY_STK_COMPO_EJECT = 0x0200, 

	//Stacker Home�ʒu�ֈړ�()
	TRAY_STK_HOME = 0x0300,
	TRAY_STK_HOME_st1_1,  //�J�n�^�C�~���O�ݒ�
	TRAY_STK_HOME_st1_2,  //HeightSNR1�C2�ɂ��ʒu���f
	TRAY_STK_HOME_st2_UP,   //���[�^�쓮�ݒ�
	TRAY_STK_HOME_st3_UP,  //���[�^�쓮�J�n(�����)
	TRAY_STK_HOME_st4_UP_ONWait,  //NoPaperSNR�@ON�҂�
	TRAY_STK_HOME_st4_UP_OFFWait, //NoPaperSNR�@OFF�҂� 
	TRAY_STK_HOME_st5_UP,  //�@Reset�J�n
	TRAY_STK_HOME_st6_UP,  //�@Reset����
	TRAY_STK_HOME_st7_UP,  //�@SNR1�@OFF��M(10Pulse�J�n)
	TRAY_STK_HOME_st8_UP,  //�@StepDown�J�n
	TRAY_STK_HOME_st9_UP,  //�@�^�C�}�ݒ�
	
	TRAY_STK_HOME_st2_DN,  //���[�^�쓮�ݒ�
	TRAY_STK_HOME_st3_DN,  //���[�^�쓮�J�n(������)
	TRAY_STK_HOME_st4_DN,  //SNR1�@ON��M(H3Pulse�J�n)
	TRAY_STK_HOME_st5_DN,  //StepDown�J�n�@
	TRAY_STK_HOME_st6_DN,  //Stacker�ҋ@�ʒu�ֈړ�����J�nTrigger�҂�
	TRAY_STK_HOME_st7_DN,  //���[�^�쓮�J�n(�����)
	TRAY_STK_HOME_st8_DN,  //SNR1�@OFF��M(10Pulse�J�n)
	TRAY_STK_HOME_st9_DN,  //StepDown�J�n
	TRAY_STK_HOME_st10_DN,  //��㎥
	TRAY_STK_HOME_st11_DN,  //StackerHome���슮��
	
      //Stacker�ҋ@�ʒu�ֈړ�����J�nTrigger����Reset����J�n(0519)
	TRAY_STK_HOME_st20_ONWait,  //NoPaperSNR�@ON�҂�
	TRAY_STK_HOME_st20_OFFWait,  //NoPaperSNR�@OFF�҂�	
	TRAY_STK_HOME_st21,  //�@Reset�J�n
	TRAY_STK_HOME_st22,  //�@Reset����

	//Compile�ʒu�ֈړ�
	TRAY_STK_COMPILE = 0x0400,
	TRAY_STK_COMPILE_st1,  //StackerHeightSNR�ɂ�镪�򔻒f
	TRAY_STK_COMPILE_st2_UP,  //���[�^�쓮�ݒ�
	TRAY_STK_COMPILE_st3_UP,  //���[�^�쓮�J�n
	TRAY_STK_COMPILE_st4_UP_ONWait,  //NoPaperSNR�@ON�҂�
	TRAY_STK_COMPILE_st4_UP_OFFWait,  //NoPaperSNR�@OFF�҂�
	TRAY_STK_COMPILE_st5_UP,  //Reset�J�n
	TRAY_STK_COMPILE_st6_UP,  //Reset����
	TRAY_STK_COMPILE_st7_UP,  //SNR1�@OFF��M(10Pulse�J�n)
	TRAY_STK_COMPILE_st8_UP,  //StepDown�J�n
	TRAY_STK_COMPILE_st9_UP,  //�^�C�}(�����]���p)�ݒ�
	TRAY_STK_COMPILE_st10_UP,  //���[�^�쓮�J�n(������)
	TRAY_STK_COMPILE_st11_UP,  //SNR1�@ON��M(G-Pulse�J�n)
	TRAY_STK_COMPILE_st12_UP,  //StepDown�J�n
	
	TRAY_STK_COMPILE_st2_DN,  //���[�^�쓮�ݒ�
	TRAY_STK_COMPILE_st3_DN,  //���[�^�쓮�J�n
	TRAY_STK_COMPILE_st4_DN,  //SNR1 ON��M(G-Pulse�J�n)
	TRAY_STK_COMPILE_st5_DN,  //StepDown�J�n
	
	TRAY_STK_COMPILE_st13,  //Compile�ʒu�ֈړ�����

	
	//Eject�ʒu�ֈړ�(ADJUST)
	TRAY_STK_EJECT = 0x0500,
	TRAY_STK_EJECT_st1,  //T1�^�C�}�҂�
	TRAY_STK_EJECT_st2,  //���[�^�쓮�ݒ�
	TRAY_STK_EJECT_st3,  //���[�^�쓮�J�n(H-Pulse+StepDown���ړ�)
	TRAY_STK_EJECT_st4,  //Eject�ʒu�ֈړ�����


	//�ҋ@�ʒu�ֈړ�(ProcessEnd)
	TRAY_STK_END = 0x0600,
	TRAY_STK_END_st1,  //���[�^�쓮�J�n
	TRAY_STK_END_st2_ONWait,  //NoPaperSNR�@ON�҂�
	TRAY_STK_END_st2_OFFWait,  //NoPaperSNR�@OFF�҂�
	TRAY_STK_END_st3,  //Reset�J�n
	TRAY_STK_END_st4,  //Reset����
	TRAY_STK_END_st5,  //SNR1�@OFF��M(10Pulse�J�n)
	TRAY_STK_END_st6,  //StepDown�J�n
	TRAY_STK_END_st7,  //��㎥
	TRAY_STK_END_st8,  //�ҋ@�ʒu�ֈړ�����

	// Number of Module Action
	TRAY_STK_ACT_NUM,

} ETRAY_STK_Action;

#define  TRAY_STK_ACTION_MASK 0xFF00

#endif //#if !defined(ETRAY_STK_Action_h)

/*#if !defined(Tray_Action_h)
#define Tray_Action_h

#include "DD_STM.h"

typedef enum
{
	//Eject Mot ��~
	TRAY_st0,   //IDLE

	//Component ����(Tray Motor) �J�n(Link No)
	TRAY_COMPO_UP = 60,   	// Stacker Tray���㏸������B
	TRAY_COMPO_DOWN	= 61,	//Stacker Tray�����~������B
	TRAY_COMPO_EJECTMOT,
	TRAY_COMPO_st1,
	TRAY_COMPO_st2,
	TRAY_COMPO_st3,
	TRAY_COMPO_st4, 

} STK_TRAY_Action;


typedef struct {
	STK_TRAY_Action    action;    //Main  State
	STK_TRAY_Action	  compoAction; //Component ActionState

	UC  	 sheetNo;
	SS 	 timerIndex;
	SS    interruptIndex;
	UC 	 direction;
	DD_STM_PROF_ID profile;

} STK_TRAY_Control;

static STK_TRAY_Control     gSTK_TRAY;

#endif //#if !defined(Tray_Action_h) */

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

