// File Name: DD_DIO.c
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
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev008 Base)
// Revision 000

#include "Common.h"
#include "DD_Common.h"
#include "LIB_Service.h"
#include "MSP_Sensor.h"
#include "DD_DIO.h"

// �� typdef
typedef struct{
	UL filteredLevel;
	UL directLevel;
} DD_DIO_INPUT_INFO;

// �� Prototype(Used Only in this file)
static void CheckLevelAndEdge(void);
static UL GetCurrentLevel(void);

// �� Digital Output Set Function
static void Set_TRANSPORT_MOT_DIR(UC value) { P0.4 = value; }
static void Set_TRANSPORT_MOT_BRAKE(UC value) { P10.0 = value; }
static void Set_TRANSPORT_MOT_GATE_SOL_PULL(UC value) { P2.2 = value; }
static void Set_TRANSPORT_MOT_GATE_SOL_PUSH(UC value) { P2.3 = value; }

static void Set_LEFT_TAMPER_MOT_CUR(UC value) { P4.4 = value; }
static void Set_LEFT_TAMPER_MOT_DIR(UC value) { P4.3 = value; }
static void Set_LEFT_TAMPER_MOT_ENB(UC value) { P4.5 = value; }

static void Set_RIGHT_TAMPER_MOT_CUR(UC value) { P11.1 = value; }
static void Set_RIGHT_TAMPER_MOT_DIR(UC value) { P11.0 = value; }
static void Set_RIGHT_TAMPER_MOT_ENB(UC value) { P1.0 = value; }

static void Set_STACKER_MOT_CUR(UC value) { P14.4 = value; }
static void Set_STACKER_MOT_DIR(UC value) { P0.0 = value; }
static void Set_STACKER_MOT_ENB(UC value) { P14.3 = value; }

static void Set_EJECT_BELT_MOT_DIR(UC value) { P1.3 = value; }
static void Set_EJECT_BELT_MOT_ENB(UC value) { P1.5 = value; }
static void Set_EJECT_BELT_MOT_PHASE1(UC value) { P15.2 = value; }
static void Set_EJECT_BELT_MOT_PHASE2(UC value) { P15.3 = value; }

static void Set_SET_CLAMP_CLUTCH_ENB(UC value) { P6.7 = value; }

static void Set_STAPLER_MOT_IN1(UC value) { P7.0 = value; }

static void Set_RESET_OUT(UC value) { P13.0 = value; }
static void Set_WDT_CLR(UC value) { P14.2 = value; }
static void Set_INTLK_24_ON(UC value) { P1.4 = value; }


// �� DD_DIO_OutputPortID�Ə��Ԃ����킹�邱��
static const void (*SetOrderFunc[])(UC value) = {

	Set_TRANSPORT_MOT_DIR,
	Set_TRANSPORT_MOT_BRAKE,
	Set_TRANSPORT_MOT_GATE_SOL_PULL,
	Set_TRANSPORT_MOT_GATE_SOL_PUSH,

	Set_LEFT_TAMPER_MOT_CUR,
	Set_LEFT_TAMPER_MOT_DIR,
	Set_LEFT_TAMPER_MOT_ENB,

	Set_RIGHT_TAMPER_MOT_CUR,
	Set_RIGHT_TAMPER_MOT_DIR,
	Set_RIGHT_TAMPER_MOT_ENB,

	Set_STACKER_MOT_CUR,
	Set_STACKER_MOT_DIR,
	Set_STACKER_MOT_ENB,

	Set_EJECT_BELT_MOT_DIR,
	Set_EJECT_BELT_MOT_ENB,
	Set_EJECT_BELT_MOT_PHASE1,
	Set_EJECT_BELT_MOT_PHASE2,

	Set_SET_CLAMP_CLUTCH_ENB,

	Set_STAPLER_MOT_IN1,

	Set_RESET_OUT,
	Set_WDT_CLR,
	Set_INTLK_24_ON,

};

static DD_DIO_INPUT_INFO	DD_DIO_InputInfo;
static UL GetCurrentLevel(void);
void   (*EDGEFUNC)(UC, UC, UL);     					 	/* �ω��ʒm�֐��ւ̃|�C���^     */

//============================================================================= 
// Function Name : DD_DIO_Init
// Description   : initializes the Port I/O.
// Parameter     : -
// Return        : -
// Note          : -
//=============================================================================
void DD_DIO_Init(void)
{
	EDGEFUNC = NULL;

	DD_DIO_InputInfo.directLevel = GetCurrentLevel();
	DD_DIO_InputInfo.filteredLevel = DD_DIO_InputInfo.directLevel;

	DD_EVT_Put(evStart_ID, evuDD_DIO_ID, 0, 0);
}

//============================================================================= 
// Function Name : DD_DIO_GetInput
// Description   : �\�t�g�E�F�A�t�B���^��̎w����͐M���̃��x�����擾����
// Parameter     : inputID	"���͐M��ID"
// Return        : level	"���x��( HIGH / LOW )"
// Note          : -
//=============================================================================
UC DD_DIO_GetInput(DD_DIO_InputPortID inputID)
{
	UC ret = LOW;
	
	if( inputID < DD_DI_ID_MAX ) {
		if( (DD_DIO_InputInfo.filteredLevel & ((UL)0x00000001 << inputID)) != 0) {
			ret = HIGH;	
		}
		else {
			ret = LOW;
		}
	}
	return ret;
}

//============================================================================= 
// Function Name : DD_DIO_GetInput
// Description   : �\�t�g�E�F�A�t�B���^��̎w����͐M���̃|�[�g�[���x�����擾����(Diag�@�\�p)
// Parameter     : inputID	"���͐M��ID"
// Return        : level	"���x��( HIGH / LOW )"
// Note          : -
//=============================================================================
UC DD_DIO_GetInputPortLevel(DD_DIO_InputPortID inputID)
{
	UC ret = LOW;

	if( inputID < DD_DI_ID_MAX ) {
		if( (DD_DIO_InputInfo.filteredLevel & ((UL)0x00000001 << inputID)) != 0) {
			ret = HIGH;
		}
		else {
			ret = LOW;
		}

		if( DD_DIO_InputConvertSwLevelBit & DD_DIO_BIT_CONV(inputID) ) {		// ���]�Ώ�
			ret = (ret + 1) & 0x01;
		}
	}

	return ret;
}

//============================================================================= 
// Function Name : DD_DIO_GetDirectInput
// Description   : �\�t�g�E�F�A�t�B���^�O�̎w����͐M���̃��x�����擾����
// Parameter     : inputID	"���͐M��ID"
// Return        : level	"���x��( HIGH / LOW )"
// Note          : -
//=============================================================================
UC DD_DIO_GetDirectInput(DD_DIO_InputPortID inputID)
{
	UL nowLevel = 0x00;
	UC ret = LOW;
	
	if( inputID < DD_DI_ID_MAX ) {
		nowLevel = GetCurrentLevel();
		
		if((nowLevel & ((UL)0x00000001 << inputID)) != 0) {
			ret = HIGH;
		}
		else {
			ret = LOW;
		}
	}
	return ret;
}

//============================================================================= 
// Function Name : DD_DIO_SetOutput 
// Description   : �|�[�g�֒l�̃Z�b�g
// Parameter     : portID ���䖽��
// Parameter     : value 0/1�_���l
// Return        : -
// Note          : ROM:2f11H RAM:DCCH SPEED:4��s
//=============================================================================
void DD_DIO_SetOutput(DD_DIO_OutputPortID outputID, UC value)
{
	if( outputID < DD_DO_ID_MAX ) {
		if( DD_DIO_OutputConvertSwLevelBit & DD_DIO_BIT_CONV(outputID) ) {		// ���]�Ώ�(Softeware Level �� Port Level)
			value = (value + 1) & 0x01;
		}

		SetOrderFunc[outputID](value);
	}
}

//============================================================================= 
// Function Name : DD_DIO_EntryCallback
// Description   : �R�[���o�b�N�֐��̓o�^
// Parameter     : 
// Return        : -
// Note          : 
//=============================================================================
void DD_DIO_EntryCallback(void (*func)(UC, UC, UL))
{
	EDGEFUNC = func ;
}

//============================================================================= 
// Function Name : DD_DIO_Notify
// Description   : 
// Parameter     : eventId		�C�x���gID
// Parameter     : arg			����
// Return        : -
// Note          : -
//=============================================================================
void DD_DIO_Notify(DD_EventID eventId, void *arg)
{
	switch (eventId) {
	case evStart_ID:
		CheckLevelAndEdge();
		break;
		
	default:
		break;
	}
}

//============================================================================= 
// Function Name : CheckLevelAndEdge
// Description   : �Z���T���x���G�b�W�ω��̃`�F�b�N
// Parameter     : -
// Return        : -
// Note          : -
//=============================================================================
static void CheckLevelAndEdge(void)
{
	UL rise					= 0;
	UL fall					= 0;
	UL currentLevel			= GetCurrentLevel();								// ���݃Z���T���x���̎擾
	UL prevLevel			= DD_DIO_InputInfo.directLevel;						// �O��Z���T���x��
	UL prevFilteredLevel	= DD_DIO_InputInfo.filteredLevel;					// �O��t�B���^��̃��x��
	UL currentFilteredLevel = (currentLevel & prevLevel) 
							| ((currentLevel ^ prevLevel) & prevFilteredLevel);	// �t�B���^��̌��݂̃Z���T���x���擾

	DD_DIO_InputInfo.directLevel = currentLevel;
	DD_DIO_InputInfo.filteredLevel = currentFilteredLevel;
	
	rise = (currentFilteredLevel ^ prevFilteredLevel) & currentFilteredLevel;
	fall = (currentFilteredLevel ^ prevFilteredLevel) & prevFilteredLevel;
	
	if( rise != 0 ) {
		if( EDGEFUNC != NULL ) {												// <�R�[���o�b�N�֐��o�^�ς݂̂Ƃ�>
			EDGEFUNC(MSG_SNR_ON_EDGE, 0, rise) ;								// MSP�Z���T�[�ω��ʒm�֐����R�[��
		}
	}
	
	if( fall != 0 ) {
		if( EDGEFUNC != NULL ) {												// <�R�[���o�b�N�֐��o�^�ς݂̂Ƃ�>
			EDGEFUNC(MSG_SNR_OFF_EDGE, 0, fall) ;								// MSP�Z���T�[�ω��ʒm�֐����R�[��
		}
	}
}

//============================================================================= 
// Function Name : GetCurrentPortLevel
// Description   : ���݂̃Z���T�l(Port Level)�̎擾
// Parameter     : -
// Return        : ���݂̃Z���T�l
// Note          : �t�B���^�����O�O
//=============================================================================
static UL GetCurrentPortLevel(void)
{
	UL nowLevel = 0;

	nowLevel |= ((UL)(P2.0) << 0);		// DD_DI_STAPLE_DOOR_INTERLOCK_SNR
	nowLevel |= ((UL)(P3.0) << 1);		// DD_DI_IOT_EXIT_SNR
	nowLevel |= ((UL)(P5.0) << 2);		// DD_DI_COMPILE_EXIT_SNR
	nowLevel |= ((UL)(P5.1) << 3);		// DD_DI_STAPLE_HOME_SNR
	nowLevel |= ((UL)(P5.2) << 4);		// DD_DI_SELF_PRIMING_SNR
	nowLevel |= ((UL)(P5.3) << 5);		// DD_DI_LOW_STAPLE_SNR
	nowLevel |= ((UL)(P7.2) << 6);		// DD_DI_STACKER_TRAY_HEIGHT2_SNR
	nowLevel |= ((UL)(P7.3) << 7);		// DD_DI_STACKER_TRAY_HEIGHT1_SNR
	nowLevel |= ((UL)(P7.4) << 8);		// DD_DI_RIGHT_TAMPER_HOME_SNR
	nowLevel |= ((UL)(P7.5) << 9);		// DD_DI_LEFT_TAMPER_HOME_SNR
	nowLevel |= ((UL)(P7.6) << 10);		// DD_DI_SET_CLAMP_HOME_SNR
	nowLevel |= ((UL)(P12.0) << 11);	// DD_DI_REAR_COVER_OPEN_SNR
	nowLevel |= ((UL)(P12.1) << 12);	// DD_DI_UNIT0_SNR
	nowLevel |= ((UL)(P14.0) << 13);  	// DD_DI_STACKER_NO_PAPER_SNR
	nowLevel |= ((UL)(P14.1) << 14);	// DD_DI_SUB_PADDLE_HOME_SNR
	nowLevel |= ((UL)(P14.7) << 15);	// DD_DI_EJECT_HOME_SNR
	nowLevel |= ((UL)(P14.6) << 16);	// DD_DI_REGI_CL_ON_SNR

	return nowLevel;
}

//============================================================================= 
// Function Name : GetCurrentLevel
// Description   : ���݂̃Z���T�l(Software�Ǘ����x��)�̎擾
// Parameter     : -
// Return        : ���݂̃Z���T�l
// Note          : �t�B���^�����O�O
//=============================================================================
static UL GetCurrentLevel(void)
{
	UL nowLevel = GetCurrentPortLevel();

	return ( (~nowLevel) & DD_DIO_InputConvertSwLevelBit ) | ( nowLevel & (~DD_DIO_InputConvertSwLevelBit) );
}
