// File Name:	ECMPL_EJCT_BELT_Action.h
// Description:	Ejector Mot Device �̓�����`
// Engineer:	FXK  KimYS
// Last Edit:	15.02.26
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(ECMPL_EJCT_BELT_Action_h)
#define ECMPL_EJCT_BELT_Action_h

typedef enum /* ECMPL_EJCT_BELT_Action */
{
	//Eject Mot ��~
	CMPL_EJCT_st0,   //IDLE

	CMPL_EJCT_COMPO_FWD_EJECT = 20,	// Eject Compile Motor�𐳓]�����ɋ쓮����B
	CMPL_EJCT_COMPO_REV_PADDLE = 21,	// Eject Compile Motor���t�]�����ɋ쓮����B

	//Component ����(Eject Mot - CW)
	CMPL_EJCT_COMPO_FWD = 0x0100,  
	CMPL_EJCT_COMPO_FWD_st1,		// ���[�^�쓮�ݒ�
	CMPL_EJCT_COMPO_FWD_st2,		// ���[�^�쓮�J�n
	CMPL_EJCT_COMPO_FWD_st3_ONWait,//EjectHomeSNR�@ON�҂��� 
	CMPL_EJCT_COMPO_FWD_st3_OFFWait,// EjectHomeSNR�@OFF�҂���
	CMPL_EJCT_COMPO_FWD_st4,		// OFF��M(T2�J�n)
	CMPL_EJCT_COMPO_FWD_st5,		//�@T2��ASpeedDown�ݒ�
	CMPL_EJCT_COMPO_FWD_st6_ONWait,// EjectHomeSNR�@ON�҂���
	CMPL_EJCT_COMPO_FWD_st7,		// ON��M(T3�J�n)
	CMPL_EJCT_COMPO_FWD_st8,		// T3��AStepDown�ݒ�
	CMPL_EJCT_COMPO_FWD_st9,		// ��㎥
	CMPL_EJCT_COMPO_FWD_st10,		//����

	//Component ����(Eject Mot - CCW )
	CMPL_EJCT_COMPO_REV = 0x0200,  
	CMPL_EJCT_COMPO_REV_st1,		// ���[�^�쓮�ݒ�
	CMPL_EJCT_COMPO_REV_st2,		// ���[�^�쓮�J�n
	CMPL_EJCT_COMPO_REV_st3_ONWait,		//�@SubPaddleHomeSNR�@ON�҂�
	CMPL_EJCT_COMPO_REV_st4,		//�@T1 Timer
	CMPL_EJCT_COMPO_REV_st5,	//StepDown
	CMPL_EJCT_COMPO_REV_st6,//��㎥
	CMPL_EJCT_COMPO_REV_st7,	//����

	//SubPaddle Home����
	CMPL_EJCT_PADHOME  = 0x0300,
	CMPL_EJCT_PADHOME_st1,		 //���[�^�쓮 �ݒ�
	CMPL_EJCT_PADHOME_st2,		//���[�^�쓮�J�n
	CMPL_EJCT_PADHOME_st3_ONWait,		//SNR ON�҂�
	CMPL_EJCT_PADHOME_st4,		//ON�����ݎ�M(T1�ݒ�)
	CMPL_EJCT_PADHOME_st5,		//StepDown�ݒ�
	CMPL_EJCT_PADHOME_st6,		//��㎥
	CMPL_EJCT_PADHOME_st7,		//SubPaddleHome���슮��
	CMPL_EJCT_PADHOME_st11_1,		//DummyPulse�ݒ� (V3.5.91)
	CMPL_EJCT_PADHOME_st11_2,		//�PClock��Timer�ݒ� (V3.5.91)

	// EjectHome����
	CMPL_EJCT_EJTHOME  = 0x0400,
	CMPL_EJCT_EJTHOME_st1,		 //Mot�쓮�ݒ�
	CMPL_EJCT_EJTHOME_st2,		//Mot�쓮�J�n
//	CMPL_EJCT_EJTHOME_st3_ONWait,//EjectHome SNR�@ON�҂���
	CMPL_EJCT_EJTHOME_st3_OFFWait,//EjectHomeSNR�@OFF�҂���
	CMPL_EJCT_EJTHOME_st4,		//SNR OFF��M�iT2�J�n�j
	CMPL_EJCT_EJTHOME_st5,		//T2��ASpeedDown�ݒ�
	CMPL_EJCT_EJTHOME_st6_ONWait,//EjectHomeSNR�@On�҂���
	CMPL_EJCT_EJTHOME_st7,		//ON��M(T3�J�n)
	CMPL_EJCT_EJTHOME_st8,		// T3��AStepDown�ݒ�
	CMPL_EJCT_EJTHOME_st9,		//����
	CMPL_EJCT_EJTHOME_st11_1,	//DummyPulse�ݒ� (V3.5.91)
	CMPL_EJCT_EJTHOME_st11_2,	//�PClock��Timer�ݒ� (V3.5.91)

	//Compile����
	CMPL_EJCT_COMPILE = 0x0500,
	CMPL_EJCT_COMPILE_st1,	//Mot �쓮�ݒ�
	CMPL_EJCT_COMPILE_st2,	//Compile����J�n
	CMPL_EJCT_COMPILE_st3,	//P�񕪏���
	CMPL_EJCT_COMPILE_st4,	//StepDown�J�n
	CMPL_EJCT_COMPILE_st5,	//��㎥
	CMPL_EJCT_COMPILE_st6,	//MotOFF


	//Eject����
	CMPL_EJCT_EJECT = 0x0600,
	CMPL_EJCT_EJECT_st1,	//Mot �쓮�ݒ�
	CMPL_EJCT_EJECT_st2,	//Eject����J�n
	CMPL_EJCT_EJECT_st3_OFFWait,//EjectHomeSNR�@OFF�҂�
	CMPL_EJCT_EJECT_st4,	//OFF��M(T2�ݒ�j
	CMPL_EJCT_EJECT_st5,	//T2��ASpeedDown�ݒ�
	CMPL_EJCT_EJECT_st6_ONWait,//EjectHomeSNR�@On�҂�
	CMPL_EJCT_EJECT_st7,	//ON��M(T3�J�n)
	CMPL_EJCT_EJECT_st8,	//T3��AStepDown�ݒ�
	CMPL_EJCT_EJECT_st9,	//��㎥�ݒ�
	CMPL_EJCT_EJECT_st10,	//����
	CMPL_EJCT_EJECT_st11_1,	//DummyPulse�ݒ� (V3.5.91)
	CMPL_EJCT_EJECT_st11_2,	//�PClock��Timer�ݒ� (V3.5.91)


	CMPL_EJCT_ACT_NUM,

} ECMPL_EJCT_BELT_Action;

#define CMPL_EJCT_BELT_ACTION_MASK 0xFF00

//#define CMPL_EJCT_PAD_ACTION_MASK 0xFF00



#endif //#if !defined(ECMPL_EJCT_PAD_Action_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

