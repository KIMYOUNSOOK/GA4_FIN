// File Name: Main.c
// Description: ���C�����[�`��
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 15.03.26
// Revision:  001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 14.04.01 | Nishizawa Mitsuyuki | Created this file.
// Revision 000
// 15.03.26 | Hoshino Yuichi      | NVM�Ή�
// 15.03.26 | Hoshino Yuichi      | Log�Ή�
// Revision 001
// 15.03.26 | Shin.KS			  | Global A4-Fin�Ή�

#include "Common.h"
#include "LIB_Service.h"
#include "DD_ResourceAssign.h"
#include "DD_TAU.h"
#include "DD_WDT.h"
#include "DD_DMA.h"
#include "DD_INT.h"
#include "DD_DIO.h"
#include "DD_NVM.h"
#include "DD_STM.h"
#include "DD_PWM.h"
#include "DD_SQP.h"
#include "DD_COM_Service.h"
#include "DD_LOG.h"
#include "MSP_Service.h"

void CpuInit(void);

//============================================================================= 
// Function Name : main
// Description   : �S�Ă̎n�܂�
// Parameter     : -
// Return        : -
// Note          : -
//=============================================================================
void main(void)
{
	static const UC cLogMsg[] = {"Start_Main"};

	CpuInit();

	LIB_StartUp();

	DD_TAU_Init();
	DD_TAU_SetTDR(DD_SOFTWARE_TIMER_CH, DD_SOFTWARE_TIMER_INTERVAL);
	DD_TAU_IntEnable(DD_SOFTWARE_TIMER_CH);
	TS0 |= 0x0008;								// Timer�X�^�[�g

	DD_EVT_Init();

	DD_NVM_Init();
	DD_STM_Init();
	DD_PWM_Init();
	DD_SQP_Init();
	DD_DIO_Init();
	DD_COM_Init();
	DD_INT_Init();
	DD_LOG_Init();

	// ���S�@�\(�s���������E�A�N�Z�X���o����)
	// �ERAM�̈�ւ̕s���A�N�Z�X�K�[�h����
	// �E�|�[�g�@�\�̐��䃌�W�X�^�̃��C�g�����B���[�h�\�B
	// �E���荞�݋@�\�̐��䃌�W�X�^�̃��[�h�^���C�g�\�B
	// �E�N���b�N����@�\�C�d�����o��H�CRAM�p���e�B��G���[���o�@�\�̐��䃌�W�X�^�̃��C�g�����B���[�h�\�B
	IAWCTL = 0x85;
	EI();

	DD_WDT_Start();

	MSP_StartUp();

	DD_LOG_Put(LOGID_FINISHER, '0', cLogMsg, sizeof(cLogMsg), LOG_CATEGORY_DEF);
	while (1U) {
		DD_EVT_Receive();						// EventLoop
		MSP_ReceiveEvent();						// MSP
	}
}

//============================================================================= 
// Function Name : CpuInit
// Description   : CPU���W�X�^�̏����ݒ�
// Parameter     : -
// Return        : -
// Note          : -
//=============================================================================
void CpuInit(void)
{
	// Clock�ݒ�
	CMC = 0U;
	CKC = 0U;	// CSS=0, MCM0=0
	CSC = 0xC0;
	PER0 = 0x07;

	// �d�����o
	LVIM = 0x00;
	LVIS = 0x00;

	// �f�[�^�E�t���b�V��
	// DFLCTL = 0x01;

#ifndef STARTER_KIT
	// �v���A�b�v�񋟃I�v�V�����ݒ�
	PU0 = 0x00;
	PU1 = 0x00;
	PU3 = 0x00;
	PU4 = 0x00;
	PU5 = 0x00;

	PU6 = 0x00;
	PU7 = 0x00;
	PU10 = 0x00;
	PU11 = 0x00;
	PU12 = 0x00;
	PU14 = 0x00;

	// ���̓��[�h�ݒ�
	PIM0 = 0x08;
	PIM1 = 0x02;
	PIM4 = 0x00;
	PIM5 = 0x00;
	PIM14 = 0x00;

	// �o�̓��[�h�ݒ�
	POM0 = 0x00;
	POM1 = 0x00;
	POM4 = 0x00;
	POM5 = 0x00;
	POM7 = 0x00;
	POM14 = 0x00;

	// �|�[�g�E���[�h�E�R���g���[���ݒ�
	PMC0 = 0xF0;
	PMC10 = 0xFE;
	PMC12 = 0xFE;
	PMC14 = 0x7F;

	// A/D�|�[�g�ݒ�
	ADPC = 0x01;

	// ����IO���_�C���N�V�����ݒ�
	PIOR = 0x00;

	// �O���[�o���E�f�W�^���E�C���v�b�g�E�f�B�X�G�[�u���ݒ�
	GDIDIS = 0x00;

	// Digital Output���l�Q�[�g�i��Ƀ|�[�g���x���𓮍삵�Ȃ������ɌŒ肷��j
	P0  = 0x44;
	P1  = 0x14;		// TxD0��HIGH
	P2  = 0x00;
	P3  = 0x00;
	P4  = 0x04;		// TO04(���g�p)��HIGH�o��
	P5  = 0x00;
	P6  = 0x40;
	P7  = 0x00;
	P10 = 0x01;		// 0x00 -> 0x01 Tranport motor brake.
	P11 = 0x00;	
	P12 = 0x01;		
	P13 = 0x01;		// RESET_OUT�M���̓��Z�b�g����(=HIGH)
	P14 = 0x00;
	P15 = 0x00;


	// �|�[�g���[�h
	PM0 = 0x88;
	PM1 = 0x02;
	PM2 = 0x01;
	PM3 = 0xFD;
	PM4 = 0xC1;
	PM5 = 0xCF;
	PM6 = 0x00;
	PM7 = 0xFC;
	PM10 = 0xFE;
	PM11 = 0xFC;
	PM12 = 0xFF;
	PM14 = 0xE3;
	PM15 = 0xF0;
#endif
}


