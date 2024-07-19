// File Name: DD_TAU.c
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
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev006 Base)
// Revision 000


#pragma interrupt INTTM00 Timer00_Interrupt
#pragma interrupt INTTM01 Timer01_Interrupt
#pragma interrupt INTTM02 Timer02_Interrupt
#pragma interrupt INTTM03 Timer03_Interrupt
#pragma interrupt INTTM04 Timer04_Interrupt
#pragma interrupt INTTM05 Timer05_Interrupt
#pragma interrupt INTTM06 Timer06_Interrupt
#pragma interrupt INTTM07 Timer07_Interrupt
#ifndef STARTER_KIT
#pragma interrupt INTTM10 Timer10_Interrupt
#pragma interrupt INTTM11 Timer11_Interrupt
#pragma interrupt INTTM12 Timer12_Interrupt
#pragma interrupt INTTM13 Timer13_Interrupt
#endif

#include "Common.h"
#include "DD_Common.h"
#include "LIB_Lock.h"
#include "DD_Event.h"
#include "DD_TAU.h"

#define DD_TAU_CHANNEL_NUM		12
#define DD_TAU_IS_CHANNEL_ENABLE(ch) ((ch <= 0x07) || ((ch >=0x10) && (ch <= 0x13)))

DD_CALLBACK_TYPE timerCallbackTable[DD_TAU_CHANNEL_NUM];
	
// �^�C�}�̃f�[�^���W�X�^
static const volatile US* gDD_TAU_TDR_Table[] = {
	// Timer Unit 0
	(volatile US*)&TDR00, (volatile US*)&TDR01, (volatile US*)&TDR02, (volatile US*)&TDR03,
	(volatile US*)&TDR04, (volatile US*)&TDR05, (volatile US*)&TDR06, (volatile US*)&TDR07,
#ifndef STARTER_KIT	
	// Timer Unit 1
	(volatile US*)&TDR10, (volatile US*)&TDR11, (volatile US*)&TDR12, (volatile US*)&TDR13,
#endif
};

// �^�C�}�̃��[�h���W�X�^
static const volatile US* gDD_TAU_TMR_Table[] = {
	// Timer Unit 0
	(volatile US*)&TMR00, (volatile US*)&TMR01, (volatile US*)&TMR02, (volatile US*)&TMR03,
	(volatile US*)&TMR04, (volatile US*)&TMR05, (volatile US*)&TMR06, (volatile US*)&TMR07,
#ifndef STARTER_KIT	
	// Timer Unit 1
	(volatile US*)&TMR10, (volatile US*)&TMR11, (volatile US*)&TMR12, (volatile US*)&TMR13,
#endif
};

// �^�C�}�o�͂̃��[�h�ݒ背�W�X�^
static const volatile US* gDD_TAU_TOM_Table[] = {
	// Timer Unit 0
	(volatile US*)&TOM0, (volatile US*)&TOM1
};

// �^�C�}�o�͂̃A�N�e�B�u���x���ݒ背�W�X�^
static const volatile US* gDD_TAU_TOL_Table[] = {
	// Timer Unit 0
	(volatile US*)&TOL0, (volatile US*)&TOL1
};

static const US gDD_TAU_MODE_DATA[] = {
	DD_TAU_CLK_SEL_CKM0 | DD_TAU_CLK_MODE_CKS | DD_TAU_16BITS_MODE | DD_TAU_TRIGGER_SOFTWARE | DD_TAU_MODE_INTERVAL_TIMER | DD_TAU_START_INT_UNUSED,		// DD_TAU_MODE_INTERVAL_TIMER_ID
	DD_TAU_CLK_SEL_CKM0 | DD_TAU_CLK_MODE_CKS | DD_TAU_TRIGGER_SOFTWARE | DD_TAU_MODE_INTERVAL_TIMER | DD_TAU_START_INT_USED,								// DD_TAU_MODE_SQUARE_PULSE_ID
	DD_TAU_CLK_SEL_CKM0 | DD_TAU_CLK_MODE_CKS | DD_TAU_COMBINATION_MASTER | DD_TAU_TRIGGER_SOFTWARE | DD_TAU_MODE_INTERVAL_TIMER | DD_TAU_MODE_PWM_MASTER,	// DD_TAU_MODE_PWM_MASTER_ID
	DD_TAU_CLK_SEL_CKM0 | DD_TAU_CLK_MODE_CKS | DD_TAU_COMBINATION_SLAVE | DD_TAU_TRIGGER_MASTER_INT | DD_TAU_MODE_INTERVAL_TIMER | DD_TAU_MODE_PWM_SLAVE,	// DD_TAU_MODE_PWM_SLAVE_ID
	DD_TAU_CLK_SEL_CKM0 | DD_TAU_CLK_MODE_TIMN | DD_TAU_TRIGGER_SOFTWARE | DD_TAU_MODE_EVENT_COUNT															// DD_TAU_MODE_EXTERNAL_EVENT_COUNTER_ID
} ;

//============================================================================= 
// Function Name : DD_TAU_Init
// Description   : �^�C�}�[�̏������i�S�`���l�����ʁj
// Parameter     : -
// Return        : -
// Note          : -
//=============================================================================
void DD_TAU_Init(void)
{
	// Supplies input clock
    TAU0EN = 1U;
    TAU1EN = 1U;
	
														// ��N���b�N32MHz�̕���
    TPS0 = 0x0005 | 0x0060 | 0x0300 | 0x0000;			// CK00 : 1MHz(32����) /  CK01 : 500kHz(64����) /  CK02 : 250kHz(128����) /  CK03 : 125kHz(256����)
    TPS1 = 0x0005 | 0x0060 | 0x0300 | 0x0000;			// CK10 : 1MHz(32����) /  CK11 : 500kHz(64����) /  CK12 : 250kHz(128����) /  CK13 : 125kHz(256����)
	
    // Stop all channels
    TT0 = 0x0FF;										// bit0�`7�܂ł�CH0�`CH7
    TT1 = 0x00F;										// bit0�`4�܂ł�CH8�`CH11
	
	// Disable INTTMXX interrupt
    TMMK00 = TMMK01 = TMMK02 = TMMK03 = TMMK04 = TMMK05 = TMMK06 = TMMK07 = TMMK10 = TMMK11 = TMMK12 = TMMK13 = TMMK13H = 1U;
	
	// Clear INTTMXX interrupt flag
    TMIF00 = TMIF01 = TMIF02 = TMIF03 = TMIF04 = TMIF05 = TMIF06 = TMIF07 = TMIF10 = TMIF11 = TMIF12 = TMIF13 = TMIF13H = 0U;
	
	// Set INTTMXX low priority
	TMPR100 = TMPR101 = TMPR102 = TMPR103 = TMPR104 = TMPR105 = TMPR106 = TMPR107 = TMPR110 = TMPR111 = TMPR112 = TMPR113 
	 =  TMPR000 = TMPR001 = TMPR002 = TMPR003 = TMPR004 = TMPR005 = TMPR006 = TMPR007 = TMPR010 = TMPR011 = TMPR012 = TMPR013 = 1U;
	
	// Clear Data Register
	TDR00 = TDR01 = TDR02 = TDR03 = TDR04 = TDR05 = TDR06 = TDR07 = TDR10 = TDR11= TDR12 = TDR13 = 0U;
	
	// �^�C�}�o�̓��[�h�̓}�X�^�E�`���l���o�̓��[�h�ŏ�����
	TOM0 = TOM1 = 0;
	
	// �^�C�}�o�̓��x�����W�X�^�͑S�ăA�N�e�B�u�E�n�C�j�ŏ�����
	TOL0 = TOL1 = 0;
}

//============================================================================= 
// Function Name : DD_TAU_InitByChannel
// Description   : �`���l�����Ƃ̃^�C�}�[�̏�����
// Parameter     : -
// Return        : -
// Note          : -
//=============================================================================
void DD_TAU_InitByChannel(UC ch, DD_TAU_ModeID mode, DD_TAU_ActiveLevelID activeLvl) 
{
	UC tableIndex = (8 * (ch >> 4)) + (ch & 0x0F);
	UC unitIndex = (ch >> 4) & 0x0F;
	volatile US* tmpAddr;
	
	if (DD_TAU_IS_CHANNEL_ENABLE(ch)) {
		tmpAddr = gDD_TAU_TMR_Table[tableIndex];
		*tmpAddr = gDD_TAU_MODE_DATA[mode];
		
		// PWM�̃X���[�u���[�h�̏ꍇ�ɂ݃X���[�u�o�̓��[�h�ɐݒ肷��
		if ( mode == DD_TAU_MODE_PWM_SLAVE_ID ) {
			tmpAddr = gDD_TAU_TOM_Table[unitIndex];
			*tmpAddr |= (0x0001 << (ch & 0x0F));
		}
		
		// �o�͔��]���K�v�ł����"1"�ɐݒ肷��(���`�g�A�܂���PWM�}�X�^�[���[�h�̂݁j
		if ( (mode == DD_TAU_MODE_SQUARE_PULSE_ID) && (mode == DD_TAU_MODE_PWM_SLAVE_ID) 
			&& (activeLvl == DD_TAU_ACTIVE_LOW_ID) ) {
			tmpAddr = gDD_TAU_TOL_Table[unitIndex];
			*tmpAddr |= (0x0001 << (ch & 0x0F));
		}
	}
}

//============================================================================= 
// Function Name : DD_TAU_Start							
// Description   : Timer�̃J�E���g�J�n									
// Parameter     : 											
// Return        :										
// Note          : 
//=============================================================================
void DD_TAU_Start(UC ch)
{
	UC key = 0;
	
	if (DD_TAU_IS_CHANNEL_ENABLE(ch)) {
		if ((ch & 0x10) == 0) {								// TimerUnit0
			key = LIB_GetKey();
			LIB_Lock();
			if (ch != 0x05) {
				TOE0 |= (0x0001 << ch);						// �N���b�N�o�͐ݒ�ON
			}
	    	TS0 |= (0x0001 << ch);							// �^�C�}�X�^�[�g�ݒ�ON
			LIB_Unlock(key);
		}
		else {												// TimerUnit1
#ifndef STARTER_KIT
			key = LIB_GetKey();
			LIB_Lock();
			TOE1 |= (0x0001 << (ch & 0x0F));
	    	TS1 |= (0x0001 << (ch & 0x0F));
			LIB_Unlock(key);
#endif
		}
	}
}

//============================================================================= 
// Function Name : DD_TAU_Start2CH							
// Description   : Timer�̃J�E���g�J�n									
// Parameter     : 											
// Return        :										
// Note          : 
//=============================================================================
void DD_TAU_Start2CH(UC ch1, UC ch2)
{
	UC key = 0;
	US setData = 0;
	
	if (DD_TAU_IS_CHANNEL_ENABLE(ch1) && DD_TAU_IS_CHANNEL_ENABLE(ch2)) {
		if (((ch1 & 0x10) == 0) && ((ch2 & 0x10) == 0)) {		// TimerUnit0
			setData = (0x0001 << ch1) | (0x0001 << (ch2 & 0x0F));
			key = LIB_GetKey();
			LIB_Lock();
			TOE0 |= setData;									// �N���b�N�o�͐ݒ�ON
	    	TS0 |= setData;										// �^�C�}�X�^�[�g�ݒ�ON
			LIB_Unlock(key);
		}
		else if (((ch1 & 0x10) != 0) && ((ch2 & 0x10) != 0)) {	// TimerUnit1
#ifndef STARTER_KIT
			setData = (0x0001 << ch1) | (0x0001 << (ch2 & 0x0F));
			key = LIB_GetKey();
			LIB_Lock();
			TOE1 |= setData;
	    	TS1 |= setData;
			LIB_Unlock(key);
#endif
		}
	}
}

//============================================================================= 
// Function Name : DD_TAU_Stop							
// Description   : Timer�̃J�E���g��~								
// Parameter     : 											
// Return        :										
// Note          : 
//=============================================================================
void DD_TAU_Stop(UC ch)
{
	UC key = 0;
	if (DD_TAU_IS_CHANNEL_ENABLE(ch)) {
		if ((ch & 0x10) == 0) {									// TimerUnit0
			key = LIB_GetKey();
			LIB_Lock();
	    	TT0 |= (0x0001 << ch);								// �J�E���g��~
			TOE0 &= ~(0x0001 << ch);							// �^�C�}�o�͋֎~
			TO0 &= ~(0x01 << ch);								// �o�̓��x����Low�ɗ��Ƃ�
			LIB_Unlock(key);
		}
		else {													// TimerUnit1
#ifndef STARTER_KIT
			key = LIB_GetKey();
			LIB_Lock();
	    	TT1 |= (0x0001 << (ch & 0x0F));
			TOE1 &= ~(0x0001 << (ch & 0x0F));
			TO1 &= ~(0x01 << (ch & 0x0F));
			LIB_Unlock(key);
#endif
		}
	}
}

//============================================================================= 
// Function Name : DD_TAU_Stop2CH							
// Description   : Timer�̃J�E���g�J�n									
// Parameter     : 											
// Return        :										
// Note          : 
//=============================================================================
void DD_TAU_Stop2CH(UC ch1, UC ch2)
{
	UC key = 0;
	US setData = 0;
	
	if (DD_TAU_IS_CHANNEL_ENABLE(ch1) && DD_TAU_IS_CHANNEL_ENABLE(ch2)) {
		if (((ch1 & 0x10) == 0) && ((ch2 & 0x10) == 0)) {		// TimerUnit0
			setData = (0x0001 << ch1) | (0x0001 << (ch2 & 0x0F));
			key = LIB_GetKey();
			LIB_Lock();
			TOE0 &= ~setData;
			TO0 &= ~setData;
	    	TS0 &= ~setData;
			LIB_Unlock(key);
		}
		else if (((ch1 & 0x10) != 0) && ((ch2 & 0x10) != 0)) {	// TimerUnit1
#ifndef STARTER_KIT
			setData = (0x0001 << ch1) | (0x0001 << (ch2 & 0x0F));
			key = LIB_GetKey();
			LIB_Lock();
			TOE1 &= ~setData;
	    	TS1 &= ~setData;
			LIB_Unlock(key);
#endif
		}
	}
}

//============================================================================= 
// Function Name : DD_TAU_SetTDR							
// Description   : TDR�Ƀf�[�^�Z�b�g								
// Parameter     : 											
// Return        :										
// Note          : 
//=============================================================================
void DD_TAU_SetTDR(UC ch, US cnt)
{
	volatile US* TDRAddr;
	UC timerIndex = ch;
	
	if (DD_TAU_IS_CHANNEL_ENABLE(ch)) {
		// ch = 0x10, 0x11, 0x12, 0x13�́AIndex = 8, 9, 10, 11�ɒu��������
		if ((ch & 0xF0) != 0) {
			timerIndex = 8 + (ch & 0x0F);	
		}
		
		TDRAddr = gDD_TAU_TDR_Table[timerIndex];
		*TDRAddr = cnt;
	}
}

//============================================================================= 
// Function Name : DD_TAU_EntryCallback						
// Description   : SW��Timer�����݂̗L��������؂�ւ���									
// Parameter     : 											
// Return        :										
// Note          : 
//=============================================================================
void DD_TAU_EntryCallback(UC ch, void (*func)(void* arg), void* arg)
{
	UC timerIndex = ch;

	if (DD_TAU_IS_CHANNEL_ENABLE(ch)) {
		// ch = 0x10, 0x11, 0x12, 0x13�́AIndex = 8, 9, 10, 11�ɒu��������
		if ((ch & 0xF0) != 0) {
			timerIndex = 8 + (ch & 0x0F);	
		}
		timerCallbackTable[timerIndex].pFunc = func;
		timerCallbackTable[timerIndex].arg = arg;
	}
}

//============================================================================= 
// Function Name : DD_TAU_IntEnable						
// Description   : Timer�����݂�L���ɂ���								
// Parameter     : 											
// Return        :										
// Note          : bit��SFR�̓e�[�u�����ł��Ȃ��̂�switch
//=============================================================================
void DD_TAU_IntEnable(UC ch)
{
	switch (ch) {
	case 0x00:
	    TMIF00 = 0U;    // clear INTTM00 interrupt flag
		TMMK00 = 0U;    // enable INTTM00 interrupt
		break;
	case 0x01:
	    TMIF01 = 0U;    
		TMMK01 = 0U;    
		break;
	case 0x02:
	    TMIF02 = 0U;    
		TMMK02 = 0U;    
		break;
	case 0x03:
	    TMIF03 = 0U;    
		TMMK03 = 0U;    
		break;
	case 0x04:
	    TMIF04 = 0U;    
		TMMK04 = 0U;    
		break;
	case 0x05:
	    TMIF05 = 0U;    
		TMMK05 = 0U;    
		break;
	case 0x06:
	    TMIF06 = 0U;    
		TMMK06 = 0U;    
		break;
	case 0x07:
	    TMIF07 = 0U;    
		TMMK07 = 0U;    
		break;
#ifndef STARTER_KIT
	case 0x10:
	    TMIF10 = 0U;    
		TMMK10 = 0U;    
		break;
	case 0x11:
	    TMIF11 = 0U;    
		TMMK11 = 0U;    
		break;
	case 0x12:
	    TMIF12 = 0U;    
		TMMK12 = 0U;    
		break;
	case 0x13:
	    TMIF13 = 0U;    
		TMMK13 = 0U;    
		break;
#endif
	default:
		break;
	}
}

//============================================================================= 
// Function Name : DD_TAU_IntDisable(UC timerCH)							
// Description   : Timer�����݂𖳌��ɂ���								
// Parameter     : 											
// Return        :										
// Note          : bit��SFR�̓e�[�u�����ł��Ȃ��̂�switch
//=============================================================================
void DD_TAU_IntDisable(UC ch)
{
	switch (ch) {
	case 0x00:
		TMMK00 = 1U;    // disable INTTM00 interrupt
	    TMIF00 = 0U;    // clear INTTM00 interrupt flag
		break;
	case 0x01:
		TMMK01 = 1U;   
	    TMIF01 = 0U;    
		break;
	case 0x02:
		TMMK02 = 1U;    
	    TMIF02 = 0U;     
		break;
	case 0x03:    
		TMMK03 = 1U;    
	    TMIF03 = 0U;
		break;
	case 0x04:    
		TMMK04 = 1U;
	    TMIF04 = 0U;    
		break;
	case 0x05:    
		TMMK05 = 1U; 
	    TMIF05 = 0U;   
		break;
	case 0x06:    
		TMMK06 = 1U; 
	    TMIF06 = 0U;   
		break;
	case 0x07:    
		TMMK07 = 1U; 
	    TMIF07 = 0U;   
		break;
#ifndef STARTER_KIT
	case 0x10:    
		TMMK10 = 1U;
	    TMIF10 = 0U;    
		break;
	case 0x11:    
		TMMK11 = 1U;  
	    TMIF11 = 0U;  
		break;
	case 0x12:    
		TMMK12 = 1U;
	    TMIF12 = 0U;    
		break;
	case 0x13:    
		TMMK13 = 1U;
	    TMIF13 = 0U;    
		break;
#endif
	default:
		break;
	}
}

//============================================================================= 
// Function Name : DD_TAU_GetLevel			
// Description   : TO�̃��x����ǂݍ���
// Parameter     : Timer�̃`���l��
// Return        : �w�肵��Timer�̃`���l���̃��x��(0 or 1)
// Note          : -
//=============================================================================
UC DD_TAU_GetLevel(UC ch)
{
	UC ret = 0;
	
	if (DD_TAU_IS_CHANNEL_ENABLE(ch)) {
		if (ch < 0x08) {
			if((UC)(TO0 & (0x01 << ch)) != 0) {
				ret = 1;
			}
		}
		else {
#ifndef STARTER_KIT
			if((UC)(TO1 & (0x01 << (ch & 0x0F))) != 0) {
				ret = 1;				
			}
#endif
		}
	}
	return ret;
}

//============================================================================= 
// Function Name : DD_TAU_GetDataRegAddr			
// Description   : Timer�̃f�[�^���W�X�^�̃A�h���X���擾����
// Parameter     : Timer�̃`���l��
// Return        : Timer�̃f�[�^���W�X�^�̉���8bit
// Note          : DMA��DSA���W�X�^�ݒ�p
//=============================================================================
UC DD_TAU_GetDataRegAddr(UC ch)
{
	UC retAddr = 0;
	
	switch (ch) {
	case 0x00:
		retAddr = 0x18;
		break;
	case 0x01:
		retAddr = 0x1A;
		break;
	case 0x02:
		retAddr = 0x44;
		break;
	case 0x10:
		retAddr = 0x70;
		break;
	case 0x11:
		retAddr = 0x72;
		break;
	default:
		break;
	}
	
	return retAddr;
}

//============================================================================= 
// Function Name : Timer00_Interrupt							
// Description   : 									
// Parameter     : 											
// Return        :										
// Note          : 
//=============================================================================
__interrupt static void Timer00_Interrupt(void)
{
	if (timerCallbackTable[0].pFunc != 0) {
		timerCallbackTable[0].pFunc(timerCallbackTable[0].arg);
	}
}

//============================================================================= 
// Function Name : Timer01_Interrupt							
// Description   : 									
// Parameter     : 											
// Return        :										
// Note          : 
//=============================================================================
__interrupt static void Timer01_Interrupt(void)
{
	if (timerCallbackTable[1].pFunc != 0) {
		timerCallbackTable[1].pFunc(timerCallbackTable[1].arg);
	}
}

//============================================================================= 
// Function Name : Timer02_Interrupt							
// Description   : 									
// Parameter     : 											
// Return        :										
// Note          : 
//=============================================================================
__interrupt static void Timer02_Interrupt(void)
{
	if (timerCallbackTable[2].pFunc != 0) {
		timerCallbackTable[2].pFunc(timerCallbackTable[2].arg);
	}
}

//============================================================================= 
// Function Name : Timer03_Interrupt							
// Description   : 									
// Parameter     : 											
// Return        :										
// Note          : 
//=============================================================================
__interrupt static void Timer03_Interrupt(void)
{
	if (timerCallbackTable[3].pFunc != 0) {
		timerCallbackTable[3].pFunc(timerCallbackTable[3].arg);
	}
}


//============================================================================= 
// Function Name : Timer04_Interrupt							
// Description   : 									
// Parameter     : 											
// Return        :										
// Note          : 
//=============================================================================
__interrupt static void Timer04_Interrupt(void)
{
	if (timerCallbackTable[4].pFunc != 0) {
		timerCallbackTable[4].pFunc(timerCallbackTable[4].arg);
	}
}

//============================================================================= 
// Function Name : Timer05_Interrupt							
// Description   : 									
// Parameter     : 											
// Return        :										
// Note          : 
//=============================================================================
__interrupt static void Timer05_Interrupt(void)
{
	if (timerCallbackTable[5].pFunc != 0) {
		timerCallbackTable[5].pFunc(timerCallbackTable[5].arg);
	}
}

//============================================================================= 
// Function Name : Timer06_Interrupt							
// Description   : 									
// Parameter     : 											
// Return        :										
// Note          : 
//=============================================================================
__interrupt static void Timer06_Interrupt(void)
{
	if (timerCallbackTable[6].pFunc != 0) {
		timerCallbackTable[6].pFunc(timerCallbackTable[6].arg);
	}
}

//============================================================================= 
// Function Name : Timer07_Interrupt							
// Description   : 									
// Parameter     : 											
// Return        :										
// Note          : 
//=============================================================================
__interrupt static void Timer07_Interrupt(void)
{
	if (timerCallbackTable[7].pFunc != 0) {
		timerCallbackTable[7].pFunc(timerCallbackTable[7].arg);
	}
}

#ifndef STARTER_KIT
//============================================================================= 
// Function Name : Timer10_Interrupt							
// Description   : 									
// Parameter     : 											
// Return        :										
// Note          : 
//=============================================================================
__interrupt static void Timer10_Interrupt(void)
{
	if (timerCallbackTable[8].pFunc != 0) {
		timerCallbackTable[8].pFunc(timerCallbackTable[8].arg);
	}
}

//============================================================================= 
// Function Name : Timer11_Interrupt							
// Description   : 									
// Parameter     : 											
// Return        :										
// Note          : 
//=============================================================================
__interrupt static void Timer11_Interrupt(void)
{
	if (timerCallbackTable[9].pFunc != 0) {
		timerCallbackTable[9].pFunc(timerCallbackTable[9].arg);
	}
}

//============================================================================= 
// Function Name : Timer12_Interrupt							
// Description   : 									
// Parameter     : 											
// Return        :										
// Note          : 
//=============================================================================
__interrupt static void Timer12_Interrupt(void)
{
	if (timerCallbackTable[10].pFunc != 0) {
		timerCallbackTable[10].pFunc(timerCallbackTable[10].arg);
	}
}

//============================================================================= 
// Function Name : Timer13_Interrupt							
// Description   : 									
// Parameter     : 											
// Return        :										
// Note          : 
//=============================================================================
__interrupt static void Timer13_Interrupt(void)
{
	if (timerCallbackTable[11].pFunc != 0) {
		timerCallbackTable[11].pFunc(timerCallbackTable[11].arg);
	}
}
#endif
