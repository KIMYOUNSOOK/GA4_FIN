// File Name:	CMPL_STPL_Boundary.c
// Description:	Compiler Stapler Module Boundary
// Engineer:	Takahiko Minaga
// Last Edit:	14.09.01
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"
#include "ECMPL_STPL_State.h"
#include "CMPL_STPL_Boundary.h"
#include "CMPL_STPL_HEAD_Device.h"
#include "CMPL_Manager.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CMPL_STPL_COMPO_CHAIN				12
#define CMPL_STPL_COMPO_HEAD_FORWARD_LINK	19
#define CMPL_STPL_COMPO_HEAD_REVERSE_LINK	20
#define CMPL_STPL_STAPLE_WIDTH_SIZE_MIN	1820
#define CMPL_STPL_STAPLE_WIDTH_SIZE_MAX	2159
#define CMPL_STPL_STAPLE_LENGTH_SIZE_MIN	2540
#define CMPL_STPL_STAPLE_LENGTH_SIZE_MAX	3556



ECMPL_STPL_State gCMPL_STPL_State;

/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
static void CMPL_STPL_ChangeStandbyEvent();
static void CMPL_STPL_ChangeReadyEvent();
static void CMPL_STPL_ChangeDiagEvent();


static void CMPL_STPL_StartStapleHeadHome(UC aSheetNo);
static void CMPL_STPL_CompleteStapleHeadHome(UC aSheetNo);
static void CMPL_STPL_StartStapleHeadHariout(UC aSheetNo);
static void CMPL_STPL_CompleteStapleHeadHariout(UC aSheetNo);

static void CMPL_STPL_StartCompoStaplerMotorForward();
static void CMPL_STPL_StopCompoStaplerMotorForward();
static void CMPL_STPL_CompleteCompoStaplerMotorForward();
static void CMPL_STPL_StartCompoStaplerMotorReverse();
static void CMPL_STPL_StopCompoStaplerMotorReverse();
static void CMPL_STPL_StartCompoStaplerMotor(US aLinkNo);
static void CMPL_STPL_StopCompoStaplerMotor(US aLinkNo);

static void CMPL_STPL_StartStaple(aSheetNo);
static void CMPL_STPL_CompleteStaple(aSheetNo);
static bool CMPL_STPL_IsCanStapleSheetSize(UC aSheetNo);

static void CMPL_STPL_InformedModuleActionComplete(SS aTimerID, UL aActionData);
static void CMPL_STPL_InformedCompoControlStart(SS aTimerID, UL aChainLink);
static void CMPL_STPL_InformedCompoControlComplete(SS aTimerID, UL aActionData);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_Reset
// Description   : �X�e�[�v�����Z�b�g
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_Reset()
{
	gCMPL_STPL_State = CMPL_STPL_STT_UNKNOWN;
	CMPL_STPL_HEAD_Reset();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_SetUp
// Description   : �X�e�[�v���Z�b�g�A�b�v
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_SetUp()
{
	/* No Action */
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_PowerOff
// Description   : �X�e�[�v���p���[�I�t
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_PowerOff()
{
	CMPL_STPL_StopModuleAction();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_Normal
// Description   : �X�e�[�v���m�[�}��
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_Normal()
{
	CMPL_STPL_ChangeStandbyEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_Diag
// Description   : �X�e�[�v���_�C�A�O
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_Diag()
{
	CMPL_STPL_ChangeDiagEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_ProcStart
// Description   : �v���Z�X�X�^�[�g
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_ProcStart()
{
	CMPL_STPL_ChangeReadyEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_StopSteadyFinishing
// Description   : �X�e�[�v���t�B�j�b�V���O�v���Z�X�I��
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_StopSteadyFinishing()
{
	CMPL_STPL_ChangeStandbyEvent();
}

void CMPL_STPL_StandbyEjectStartNotify()
{
	/* No Action */
}

void CMPL_STPL_StandbyEjectCompNotify()
{
	/* No Action */
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_ChangeStandbyEvent
// Description   : �X�^���o�C�J�ڃC�x���g
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_ChangeStandbyEvent()
{
	if( (gCMPL_STPL_State == CMPL_STPL_STT_UNKNOWN) ||
		(gCMPL_STPL_State == CMPL_STPL_STT_DIAG) ||
		(gCMPL_STPL_State == CMPL_STPL_STT_READY) ) {
		gCMPL_STPL_State = CMPL_STPL_STT_STANDBY;
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_ChangeReadyEvent
// Description   : ���f�B�[�J�ڃC�x���g
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_ChangeReadyEvent()
{
	if( gCMPL_STPL_State == CMPL_STPL_STT_STANDBY ) {
		gCMPL_STPL_State = CMPL_STPL_STT_READY;
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_ChangeDiagEvent
// Description   : �_�C�A�O�J�ڃC�x���g
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_ChangeDiagEvent()
{
	if( (gCMPL_STPL_State == CMPL_STPL_STT_UNKNOWN) ||
		(gCMPL_STPL_State == CMPL_STPL_STT_STANDBY) ) {
		gCMPL_STPL_State = CMPL_STPL_STT_DIAG;
	}
	else {
		/* No Action */
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_AnyCoverOpen
// Description   : �����ꂩ�̃J�o�[���J����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_AnyCoverOpen()
{
	if( gCMPL_STPL_State != CMPL_STPL_STT_DIAG ) {
		CMPL_STPL_StopModuleAction();
	}
	else {																		// = FIN_STT_DIAGNOSTICS
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_AllCoverClose
// Description   : ���ׂẴJ�o�[���܂���
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_AllCoverClose()
{
	CMPL_STPL_HEAD_AllCoverClose();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_StopModuleAction
// Description   : �X�e�[�v�������~
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_StopModuleAction()
{
	CMPL_STPL_HEAD_DeviceStop();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_StopSheet
// Description   : �w��V�[�g�̓����~
// Parameter     : aSheetNo (�w��V�[�g�ԍ�)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_StopSheet(UC aSheetNo)
{
	CMPL_STPL_HEAD_StopSheet(aSheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_IsExecutingModuleAction
// Description   : �X�e�[�v���̓����Ԃ��擾
// Parameter     : �]
// Return        : true�F���쒆�Afalse�F��~��
///////////////////////////////////////////////////////////////////
bool CMPL_STPL_IsExecutingModuleAction()
{
	if( CMPL_STPL_HEAD_IsExecutiongDeviceAction() == false ) {
		return false;
	}
	else {
		return true;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_StapleHeadHome
// Description   : �X�e�[�v���w�b�h�z�[������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_StapleHeadHome(UC aSheetNo)
{
	if( (CMPL_STPL_HEAD_IsOccurFault() == false) && (SRVC_GetCoverStatus() == CVR_STTS_CLOSE) && (SRVC_CheckUnejectedSheetExist(eCdiFinOutDest_OutTray) == false)) {
		CMPL_STPL_StartStapleHeadHome(aSheetNo);
	}
	else {
		CMPL_STPL_CompleteStapleHeadHome(aSheetNo);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_StartStapleHeadHome
// Description   : �X�e�[�v���w�b�h�z�[������ ����J�n
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_StartStapleHeadHome(UC aSheetNo)
{
	CMPL_STPL_HEAD_StartAct(CMPL_STPL_HEAD_HOME_st1 , aSheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_CompleteStapleHeadHome
// Description   : �X�e�[�v���w�b�h�z�[������ ���슮��
//				 : ���s������x��������߂ɁA0ms�^�C�}���d�|���Ă���
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_CompleteStapleHeadHome(UC aSheetNo)
{
	UL aActionData = (UL)(aSheetNo) | ((UL)(CMPL_MDL_ACT_STAPLE_HEAD_HOME) << 8);
	MSP_SetEventTimer(0, CMPL_STPL_InformedModuleActionComplete, aActionData);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_ComponentControl
// Description   : �R���|�[�l���g�R���g���[��(�J�n/��~)
// Parameter     : aChainNo (Chain No.)
//				 : aLinkNo (Link No.)
//				 : aAction (�J�n/��~ �w��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_ComponentControl(US aChainNo, US aLinkNo, EAction aAction)
{
	if( aAction == ACT_START ) {
		CMPL_STPL_StartCompoStaplerMotor(aLinkNo);
	} else {																	// ACT_STOP
		CMPL_STPL_StopCompoStaplerMotor(aLinkNo);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_StartCompoStaplerMotorForward
// Description   : Stapler Motor���]����J�n(Compo Cont����)
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_StartCompoStaplerMotor(US aLinkNo)
{
	ECMPL_STPL_HEAD_Action aAction;

	UL aChainLink = (UL)(CMPL_STPL_COMPO_CHAIN) | ((UL)(aLinkNo) << 16);
	MSP_SetEventTimer(0, CMPL_STPL_InformedCompoControlStart, aChainLink);

	aAction = (ECMPL_STPL_HEAD_Action)aLinkNo;
	switch ( aAction ) {
	case CMPL_STPL_HEAD_COMPO_FORWARD :
	case CMPL_STPL_HEAD_COMPO_REVERSE :
		CMPL_STPL_HEAD_StartAct(aAction, 0);
		break;

	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_StopCompoStaplerMotorForward
// Description   : Stapler Motor���]�����~(Compo Cont����)
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_StopCompoStaplerMotor(US aLinkNo)
{
	CMPL_STPL_StopModuleAction();
	CMPL_STPL_InformedCompoComplete(aLinkNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_CompleteCompoStaplerMotorForward
// Description   : Stapler Motor���]���슮��(Compo Cont����)
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_CompleteCompoStaplerMotorForward()
{
	UL aChainLink = (UL)(CMPL_STPL_COMPO_CHAIN) | ((UL)(CMPL_STPL_COMPO_HEAD_FORWARD_LINK) << 16);

	MSP_SetEventTimer(0, CMPL_STPL_InformedCompoControlComplete, aChainLink);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_Staple
// Description   : �X�e�[�v������
// Parameter     : aSheetNo (�ΏۃV�[�g�i���o�[)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_Staple(UC aSheetNo)
{	
	UC tempMediaType;

	tempMediaType = SRVC_GetMediaType(aSheetNo);

	if( CMPL_STPL_HEAD_IsOccurFault() == false ) {
		if( (SRVC_CheckSheetConditionNormal(aSheetNo) == true) &&
			(SRVC_GetStapleMode(aSheetNo) != eCdiFinStaple_Off) &&
			(SRVC_GetSetTop(aSheetNo) == SHT_SET_TOP_NOT) &&
			(SRVC_GetSetEnd(aSheetNo) == SHT_SET_END) &&
			(SRVC_GetSetEndAction(aSheetNo) == eCdiFinSetEndAct_NrmlEnd) &&
			(SRVC_GetDevidedSet(aSheetNo) == SHT_SET_UNDEVIDED) &&
			(CMPL_STPL_IsCanStapleSheetSize(aSheetNo) == true) &&
			(SRVC_GetCrossProcessSizeMixSet(aSheetNo) == SHT_SIZE_UNMIX) ) {
				if ( (tempMediaType == eCdiFinMediaType_Coated) && (SRVC_GetNvmValue(NVMID_STAPLE_COAT_CHANGE) == 1)) {
					CMPL_STPL_PrintLog(0x21, (UC)SRVC_GetStapleMode(aSheetNo), (UC)SRVC_GetDevidedSet(aSheetNo) , (US)SRVC_CheckSheetConditionNormal(aSheetNo) );
					CMPL_STPL_CompleteStaple(aSheetNo);
				} else {
					CMPL_STPL_StartStaple(aSheetNo);
				}
		}
		else {
			CMPL_STPL_CompleteStaple(aSheetNo);
			CMPL_STPL_PrintLog(0x20, (UC)SRVC_GetStapleMode(aSheetNo), (UC)SRVC_GetDevidedSet(aSheetNo) , (US)SRVC_CheckSheetConditionNormal(aSheetNo) );
		}
	}
	else {
		CMPL_STPL_CompleteStaple(aSheetNo);
	}
}

bool CMPL_STPL_IsCanStapleSheetSize(UC aSheetNo)
{
	US aLength, aWidth;

	aLength = SRVC_GetProcessSize(aSheetNo);
	aWidth = SRVC_GetCrossProcessSize(aSheetNo);

	if( (aLength >= CMPL_STPL_STAPLE_LENGTH_SIZE_MIN) && (aLength <= CMPL_STPL_STAPLE_LENGTH_SIZE_MAX) && (aWidth >= CMPL_STPL_STAPLE_WIDTH_SIZE_MIN) && (aWidth <= CMPL_STPL_STAPLE_WIDTH_SIZE_MAX) ) {
		return true;
	}
	else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_StartStaple
// Description   : �X�e�[�v������
// Parameter     : aSheetNo (�ΏۃV�[�g�i���o�[)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_StartStaple(UC aSheetNo)
{
	CMPL_STPL_HEAD_StartAct(CMPL_STPL_HEAD_STAPLE_st1, aSheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_CompleteStaple
// Description   : �X�e�[�v������
// Parameter     : aSheetNo (�ΏۃV�[�g�i���o�[)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_CompleteStaple(UC aSheetNo)
{
	UL aActionData = (UL)(aSheetNo) | ((UL)(CMPL_MDL_ACT_STAPLE) << 8);
	MSP_SetEventTimer(0, CMPL_STPL_InformedModuleActionComplete, aActionData);
}
///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_StapleHeadHariout
// Description   : �X�e�[�v���w�b�h�z�[������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_StapleHeadHariout(UC aSheetNo)
{
	if( (CMPL_STPL_HEAD_IsOccurFault() == false) && (SRVC_GetCoverStatus() == CVR_STTS_CLOSE) && (SRVC_CheckUnejectedSheetExist(eCdiFinOutDest_OutTray) == false)) {
		CMPL_STPL_StartStapleHeadHariout(aSheetNo);
	}
	else {
		CMPL_STPL_CompleteStapleHeadHariout(aSheetNo);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_StartStapleHeadHome
// Description   : �X�e�[�v���w�b�h�z�[������ ����J�n
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_StartStapleHeadHariout(UC aSheetNo)
{
	CMPL_STPL_HEAD_StartAct(CMPL_STPL_HEAD_HARIOUT_st1 , aSheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_CompleteStapleHeadHome
// Description   : �X�e�[�v���w�b�h�z�[������ ���슮��
//				 : ���s������x��������߂ɁA0ms�^�C�}���d�|���Ă���
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_CompleteStapleHeadHariout(UC aSheetNo)
{
	UL aActionData = (UL)(aSheetNo) | ((UL)(CMPL_MDL_ACT_STAPLE_HARIOUT) << 8);
	MSP_SetEventTimer(0, CMPL_STPL_InformedModuleActionComplete, aActionData);
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_InformedModuleActionComplete
// Description   : Compiler Module��Module Action�I����ʒm����
// Parameter     : aActionData(0-7bit:SheetNo�A8-15bit:�w�蓮��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_InformedModuleActionComplete(SS aTimerID, UL aActionData)
{
	UC aSheetNo = (UC)(aActionData & 0x000000FF);
	UC aAction = (UC)((aActionData >> 8) & 0x000000FF);

	CMPL_InformedModuleActionComp((ECMPL_ModuleAction)(aAction), aSheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_InformedCompoControlStart
// Description   : Compiler Module��Component Control Action�J�n��ʒm����
// Parameter     : aChainLink(0-15bit:ChainNo�A16-31bit:LinkNo)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_InformedCompoControlStart(SS aTimerID, UL aChainLink)
{
	US aChainNo = (US)(aChainLink & 0x0000FFFF);
	US aLinkNo = (US)((aChainLink >> 16) & 0x0000FFFF);
	CMPL_InformedCompoControlComplete(aChainNo, aLinkNo, ACT_START);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_InformedCompoComplete
// Description   : Component Control Action�I����ʒm����
// Parameter     : aChainLink(0-15bit:ChainNo�A16-31bit:LinkNo)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_InformedCompoComplete(US aLinkNo)
{	
	UL aChainLink = (UL)(CMPL_STPL_COMPO_CHAIN) | ((UL)(aLinkNo) << 16);
	MSP_SetEventTimer(0, CMPL_STPL_InformedCompoControlComplete, aChainLink);	
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_InformedCompoControlComplete
// Description   : Compiler Module��Component Control Action�I����ʒm����
// Parameter     : aChainLink(0-15bit:ChainNo�A16-31bit:LinkNo)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_InformedCompoControlComplete(SS aTimerID, UL aChainLink)
{
	US aChainNo = (US)(aChainLink & 0x0000FFFF);
	US aLinkNo = (US)((aChainLink >> 16) & 0x0000FFFF);
	CMPL_InformedCompoControlComplete(aChainNo, aLinkNo, ACT_STOP);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_InformedStapleHeadHomeComp
// Description   : Device����X�e�[�v���w�b�h�z�[�����슮������M
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_InformedStapleHeadComp(ECMPL_STPL_HEAD_Action aAction, UC aSheetNo)
{
	switch ( aAction ) {

	case CMPL_STPL_HEAD_HOME :
		CMPL_STPL_CompleteStapleHeadHome(aSheetNo);
		break;

	case CMPL_STPL_HEAD_STAPLE :
		CMPL_STPL_CompleteStaple(aSheetNo);
		break;

	case CMPL_STPL_HEAD_HARIOUT :
		CMPL_STPL_CompleteStapleHeadHariout(aSheetNo);
		break;	

	default:
		break;
	}
}


void CMPL_STPL_InformedStapleSupplyLevel(UC aStapleSupply)
{
	CMPL_InformedDeviceStatusChange(DSNO_STAPLE_SUPPLY_LEVEL, aStapleSupply);
}

void CMPL_STPL_InformedStapleNG(EFaultStatus faultStatus)
{
	CMPL_InformedDeviceStatusChange(DSNO_STAPLE_STATUS_NG, faultStatus);
}

void CMPL_STPL_InformedStapleFail(EFaultStatus faultStatus)
{
	if( faultStatus == FAULT_OCCUR ) {
		CMPL_STPL_HEAD_DeviceStop();
		CMPL_InformedDeviceStatusChange(DSNO_STAPLE_FAIL, faultStatus);
	}
	else {
		CMPL_InformedDeviceStatusChange(DSNO_STAPLE_FAIL, faultStatus);
	}
}

void CMPL_STPL_InformedStapleReadySensorFail(EFaultStatus faultStatus)
{
	CMPL_InformedDeviceStatusChange(DSNO_STAPLE_READY_FAIL, faultStatus);
}
///////////////////////////////////////////////////////////////////
// Function Name : PrintTransLog
// Description   :  
// Parameter     : para1 (������̍ŏ��ɕt������l) : SNR���(0x01: Iot ExitSNR, 0x02:CompileExitSNR)
//               : para2 (������̂Q�Ԗڂɕt������l) : ON/OFF
//               : para3 (������̂R�Ԗڂɕt������l) : SheetNo
//		  : para4(������� 4�Ԗڂɕt������l) : Trans Status
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_PrintLog(UC para1, UC para2, UC para3, US para4)
{

	UC 	log[5];

	log[0] =  para1;
	// log[1] =  (UC)(para1 & 0x00FF);
	log[1] =  para2;	
	log[2] =  para3;

	log[3] =  (UC)(para4 >>8 );
	log[4] =  (UC)(para4 & 0x00FF);

	
//#define	LOGID_TRNS=14, 'S'=Send,   LOG_CATEGORY_DEBUG=2
	DD_LOG_Put(LOGID_STPL, 'S', log, sizeof(log), LOG_CATEGORY_DEBUG);

}


