// File Name:	TRAY_Manager.c
// Description:	Source File of Output TRAY Manager
// Engineer:	Yuichi Hoshino
// Last Edit:	14.09.02
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "TRAY_Manager.h"
#include "TRAY_STK_Device.h"

#include "CNvmInformer.h"
#include "DD_DIO.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of Output Tray Manager ---
typedef struct /*TRAY_ManagerMember*/
{
	UC    isInitCondition;  //[SheetInitialize]�ő҂�(HIGH),  SubPaddleHome���슮��Event��M����J�n
	UC    sheetNO;   //Sheet No 
	UC    status;    // Tray Status (US -> UC)
	UC    setOverCount;  //SetOverCount
	UC    stapleSetCount;  //(CR-011) StapleSetCount(�PSet10���ȉ�)
	UL    previousPulseCnt;  //(CR-013) �ȑO�����PulseCount���i�[
} TRAY_ManagerMember;

static TRAY_ManagerMember gTrayManagerMember;

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_Constructor
// Description   : �R���X�g���N?
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_Manager_Constructor()
{
	TRAY_STK_Reset();
	gTrayManagerMember.isInitCondition = false;
	gTrayManagerMember.status = FIN_STT_UNKNOWN;
	gTrayManagerMember.sheetNO = SHEETNO_NUM;
	gTrayManagerMember.previousPulseCnt = 0;
}

///////////////////////////////////////////////////////////////////
// Function Name :TRAY_Manager_PowerUp
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_Manager_PowerUp()
{
	gTrayManagerMember.status = FIN_STT_POWER_UP;
	gTrayManagerMember.isInitCondition = false;

	MSP_SendMessage(MID_TRAY, EVT_STT_POWER_UP_RES, NULL, 0, LOG_OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_ChangeDiag
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_Manager_ChangeDiag()
{
	gTrayManagerMember.status =  FIN_STT_CHANGE_DIAG;

	MSP_SendMessage(MID_TRAY, EVT_STT_CHANGE_DIAG_RES, NULL, 0, LOG_OFF);
	TRAY_STK_StandbyFailCheckStop();  //V3.4.0 
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_ChangeNormal
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_Manager_ChangeNormal()
{

	gTrayManagerMember.status = FIN_STT_CHANGE_NORMAL;

	MSP_SendMessage(MID_TRAY, EVT_STT_CHANGE_NORMAL_RES, NULL, 0, LOG_OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_PowerDown
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_Manager_PowerDown()
{
	TRAY_STK_DeviceStop();

	gTrayManagerMember.status = FIN_STT_POWER_DOWN;

	MSP_SendMessage(MID_TRAY, EVT_STT_POWER_DOWN_RES, NULL, 0, LOG_OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_CycleUp
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_Manager_CycleUp()
{
	gTrayManagerMember.status = FIN_STT_CYCLE_UP;

	MSP_SendMessage(MID_TRAY, EVT_STT_CYCLE_UP_RES, NULL, 0, LOG_OFF);

	gTrayManagerMember.status = FIN_STT_READY;  //V3.6.92
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_HardDown
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_Manager_HardDown()
{
	gTrayManagerMember.status =FIN_STT_HARD_DOWN;
	
	//V3.2.0 Job���������AHardDown��M�̏ꍇ�ADeviceStop
	//gTrayManagerMember.isInitCondition = false;	
	if ( gTrayManagerMember.isInitCondition == true ) {

		//TRAY_STK_DeviceStop();  //V3.1.7
		gTrayManagerMember.isInitCondition = false;
		TRAY_Manager_StopSheetInitialize(gTrayManagerMember.sheetNO);
	}

	MSP_SendMessage(MID_TRAY, EVT_STT_PROCESS_DOWN_RES, NULL, 0, LOG_OFF);
	SRVC_UserDebugLog(0x72, 0x72);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_CycleDown
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_Manager_CycleDown()
{
	gTrayManagerMember.status =FIN_STT_CYCLE_DOWN;

	//V3.2.0 Job���������ACycleDown��M�̏ꍇ�ADeviceStop
	//gTrayManagerMember.isInitCondition = false;	
	if ( gTrayManagerMember.isInitCondition == true ) {

		//TRAY_STK_DeviceStop();  //V3.1.7
		gTrayManagerMember.isInitCondition = false;
		TRAY_Manager_StopSheetInitialize(gTrayManagerMember.sheetNO);
	}
	MSP_SendMessage(MID_TRAY, EVT_STT_PROCESS_DOWN_RES, NULL, 0, LOG_OFF);
	SRVC_UserDebugLog(0x70, 0x70);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_ReceiveStandbyExitNotify
// Description   : Standby->CycleUp �O����
// Parameter     : evt (Sheet Information )
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_Manager_ReceiveStandbyExitNotify()
{
	SRVC_UserDebugLog(0x71, 0x71);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_AnyCoverOpenNotify
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_Manager_AnyCoverOpenNotify()
{
	//V3.1.7 StapleCover Open���̂ݏ���  	
 	if (SRVC_GetCoverStatus_ID(CVR_ID_FIN_STAPLE_ILK) ==  CVR_STTS_OPEN) {

		TRAY_STK_DeviceStop();  //V3.1.7
		SRVC_UserDebugLog(0x44, 0x44);
	} 
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_AllCoverCloseNotify
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_Manager_AllCoverCloseNotify()
{
 
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_ComponentControlStartRequest
// Description   : 
// Parameter     : chainNo, linkNo
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_Manager_ComponentControl(const UC* evt, EAction aAction)
{
	EvtParamComponentControlStartRequest *pRcvEvtParameter;
	EvtParamComponentControlStartNotify evtParameter;

	pRcvEvtParameter = (EvtParamComponentControlStartRequest *)evt;
	evtParameter.chainNo = pRcvEvtParameter->chainNo;
	evtParameter.linkNo = pRcvEvtParameter->linkNo;

	if ( evtParameter.chainNo == TRAY_COMPO_CHAIN ) {
		switch ( (ETRAY_STK_Action)evtParameter.linkNo ) {
		case TRAY_STK_COMPO_COMPILE :
		case TRAY_STK_COMPO_EJECT :
			TRAY_Device_ComponentControl(evtParameter.chainNo, evtParameter.linkNo, aAction);
			break;

		default :	// ����`��Link No. No Action
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_InformedStackerActionComplete
// Description   : TRAY_STK_Device ���猋�ʎ�M
// Parameter     : aStkAction,  aSheetNo
// Return        : 
///////////////////////////////////////////////////////////////////
void  TRAY_Manager_InformedStackerActionComplete(US  aStkAction, UC aSheetNo)
{
	switch ( aStkAction ) {
		
	case TRAY_STK_COMPO_COMPILE :
		TRAY_InformedCompoControlComplete((US)TRAY_COMPO_CHAIN, TRAY_STK_COMPO_COMPILE, ACT_STOP);
		break;

	case TRAY_STK_COMPO_EJECT :
		TRAY_InformedCompoControlComplete((US)TRAY_COMPO_CHAIN, TRAY_STK_COMPO_EJECT, ACT_STOP);
		break;

	case TRAY_STK_HOME :  // Stacker Home�ʒu�ֈړ�() - Sheet Initialize 
		if( !TRAY_STK_IsOccurFault() ) {
			 TRAY_STK_StartAct(TRAY_STK_COMPILE,  aSheetNo); //HOME�ʒu�֊�����Compile�ʒu�ֈړ��J�n
		}
		break;	
		
	case TRAY_STK_COMPILE : //Stacker Compile�ʒu�ֈړ�()
		if ( gTrayManagerMember.isInitCondition ) { // Sheet Initialize Condition -> CompleteSheet Initialize 

			TRAY_Manager_LowerLimitCheck(gTrayManagerMember.sheetNO); 
			TRAY_Manager_MixFullStackCheckAfterCompile(gTrayManagerMember.sheetNO);  //MixFullStack 
			TRAY_Manager_FullStackCheckAfterCompile(gTrayManagerMember.sheetNO); //FullStack check
			//(TB-XX)150603 Job���������ASetOverCount���`�F�b�N����
			TRAY_Manager_SetOverCountCheck(gTrayManagerMember.sheetNO); //SetOverCount check
			TRAY_Manager_CompleteSheetInitialize();

		} else {	
			TRAY_Manager_LowerLimitCheck(aSheetNo);			
			TRAY_Manager_MixFullStackCheckAfterCompile(aSheetNo); //V3.0����
			TRAY_Manager_SaveNVM(aSheetNo);  //V3.0����			
			TRAY_Manager_FullStackCheckAfterCompile(aSheetNo); //FullStack check
			TRAY_Manager_SetOverCountCheck(aSheetNo); //SetOverCount check
		}		
		break;
		
	case TRAY_STK_EJECT :  //Stacker Eject�ʒu�ֈړ�()
		
		break;
		
	case TRAY_STK_END : //�ҋ@�ʒu�ֈړ�(ProcessEnd)
	//	TRAY_Manager_CompleteProcessEnd();
		break;
	default :
		break;
	}	
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_LowerLimitCheck
// Description   :  Compile�ʒu�ֈړ�������ELowLimitCheck����
// Parameter     : ?// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_Manager_LowerLimitCheck(UC aSheetNo)
{
	US  length = SRVC_GetProcessSize(aSheetNo);
	UL  pulseCnt = TRAY_STK_GetPulse();
	US  nvmLimit=0;
	US  nvmLowerLimit=0;

	if ( aSheetNo == SHEETNO_NUM)  	return;

	if ( SRVC_GetDeviceStatus( DSNO_STACKER_LOWLIMIT_FAIL ) == FAULT_NOT_OCCUR ) {

		if ( length > 2970) {  //��T�C�Y			
			nvmLimit = SRVC_GetNvmValueUS(NVMID_L_Full_STACK_POS_PULSE_LIMIT);	
		} else { //�@���T�C�Y			
			nvmLimit = SRVC_GetNvmValueUS(NVMID_S_Full_STACK_POS_PULSE_LIMIT) ;
		}
		nvmLowerLimit = SRVC_GetNvmValueUS(NVMID_LOW_PULSE_LIMIT);

		//Lower Limit Check
		if ( pulseCnt >= nvmLimit + nvmLowerLimit) {		
			SRVC_UpdateDeviceStatus(FAULT_OCCUR, DSNO_STACKER_LOWLIMIT_FAIL) ; //LowerLimitFail
			PrintStackerTrayLog_Short(eSTK_LOG_LOWLIMIT, (US)pulseCnt, (US)nvmLimit+nvmLowerLimit, aSheetNo);  //Log
		}
	}	
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_MixFullStackCheckAfterCompile
// Description   :  Compile�ʒu�ֈړ�������AMixFullStackCheck����
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_Manager_MixFullStackCheckAfterCompile(UC aSheetNo)
{	
	UL  pulseCnt = TRAY_STK_GetPulse();

	//�ŏI�r�o�p���̒���
	US	length_last_ejected = SRVC_GetProcessSize(aSheetNo);	
	//�ŏI�r�o�p���̕�  : 
	US	width_last_ejected = SRVC_GetCrossProcessSize(aSheetNo);	
	
	US  width_maximum = 0;
	US  length_maximum = 0;
	
	if ( aSheetNo == SHEETNO_NUM) 	//���m���Ȃ�
		return;

	if ( SRVC_GetNvmValueUC(NVMID_MIX_STACK_ENABLE) == 0 ) //MixFullStack���m�L��
		return;

	// FullStack = Not Occur => FullStack�`�F�b�N
	if ( SRVC_GetDeviceStatus( DSNO_STACKER_FULL_STACK ) == DVST_TRAY_STK_STTS_NOT_FULL ) {
	
		//�ύڗp���̍ő�T�C�Y(���j
		width_maximum = SRVC_GetNvmValueUS(NVMID_SHEET_WIDTH_MAXIMUM_SIZE_SHEET);
		//�ύڗp���̍ő�T�C�Y(�����j
	 	length_maximum = SRVC_GetNvmValueUS(NVMID_SHEET_LENGTH_MAXIMUM_SIZE_SHEET);

		if ( (width_maximum == 0) || (length_maximum == 0) )
			return;
		
		//	���L�𖞂����Ă���B ->MixFull Check 
		//	�u�ύڗp���ő�T�C�Y�v���u�ŏI�r�o�p���T�C�Y�v�i�p����/�p�����̂ǂ��炩�j���傫�� 	
		if ((width_last_ejected > width_maximum) || (length_last_ejected > length_maximum)) {
		
			//	StackerMotor�̃p���X�J�E���g�@>=�@MixFullStack�p���X�J�E���g�l�@�̏ꍇ, MixFullStack�Ɣ��f����
			if ( pulseCnt >= SRVC_GetNvmValueUS(NVMID_MIX_FULL_STACK_POS_PULSE_LIMIT) ) {
				SRVC_UpdateDeviceStatus(DVST_TRAY_STK_STTS_MIX_SIZE_FULL, DSNO_STACKER_FULL_STACK) ; //Mix FullStack
				PrintStackerTrayLog_Short(eSTK_LOG_FULLSTK_MIXSIZE_FULL, (US)pulseCnt,  0, aSheetNo);  //Log
				PrintStackerTrayLog_Short(eSTK_LOG_FULLSTK_MIXSIZE_FULL, (US)width_last_ejected,  width_maximum, aSheetNo);  //Log
				PrintStackerTrayLog_Short(eSTK_LOG_FULLSTK_MIXSIZE_FULL, (US)length_last_ejected,  length_maximum, aSheetNo);  //Log

				 SRVC_SetNvmValueUC(1,  NVMID_MIX_STACK); //MixFullStack�����L���F1=����
			}
		}
	}

}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_SaveNVM
// Description   :  Compile�ʒu�ֈړ�������A�e��NVM( StackerTray NVM)���i�[����B
// Parameter     : aSheetNo
// Return        : �� 
///////////////////////////////////////////////////////////////////
void TRAY_Manager_SaveNVM(UC  aSheetNo)
{
	US  length = SRVC_GetProcessSize(aSheetNo);
	US  width = SRVC_GetCrossProcessSize(aSheetNo);	
	
	US  width_maximum = 0;
	US  length_maximum = 0;
	UL  totalCount=0;

	if ( aSheetNo == SHEETNO_NUM)
		return;
	
	//�ŏI�r�o�p���̕�	: 
	SRVC_SetNvmValueUS( width,	NVMID_SHEET_WIDTH_LAST_EJECTED_SHEET); 
	//�ŏI�r�o�p���̒���
	SRVC_SetNvmValueUS( length,  NVMID_SHEET_LENGTH_LAST_EJECTED_SHEET);

	//�ύڗp���̍ő�T�C�Y(���j
	width_maximum = SRVC_GetNvmValueUS(NVMID_SHEET_WIDTH_MAXIMUM_SIZE_SHEET);

	if ( width > width_maximum ) {
		SRVC_SetNvmValueUS(width,  NVMID_SHEET_WIDTH_MAXIMUM_SIZE_SHEET);
	}
	
	//�ύڗp���̍ő�T�C�Y(�����j
	length_maximum = SRVC_GetNvmValueUS(NVMID_SHEET_LENGTH_MAXIMUM_SIZE_SHEET);

	if ( length > length_maximum ) {
		SRVC_SetNvmValueUS(length,  NVMID_SHEET_LENGTH_MAXIMUM_SIZE_SHEET);
	}	

	//�ŏI�r�o�Z�b�g��StaplePosition�@�F0=UnStaple�A1=Corner
	if ( SRVC_GetStapleMode(aSheetNo) ==  eCdiFinStaple_SingleFront )
		SRVC_SetNvmValueUC( 1 , NVMID_NUM_SET_STAPLE_POSITION);
	else 
		SRVC_SetNvmValueUC( 0 , NVMID_NUM_SET_STAPLE_POSITION);
	
	//StackerTray�r�o����
	totalCount = SRVC_GetNvmValueUL(NVMID_STACK_PAPER_LIMIT) ;
	totalCount += SRVC_GetCompileCount(aSheetNo);	
	SRVC_SetNvmValueUL(totalCount,   NVMID_STACK_PAPER_LIMIT);

	return;
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_FullStackCheckAfterCompile
// Description   :  Compile�ʒu�ֈړ�������AFullStackCheck����
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_Manager_FullStackCheckAfterCompile(UC aSheetNo)
{	
	US  length = SRVC_GetProcessSize(aSheetNo);
	UL  pulseCnt = TRAY_STK_GetPulse();


	if ( aSheetNo == SHEETNO_NUM)  		return;
		
	// FullStack = Not Occur => FullStack�`�F�b�N
	if ( SRVC_GetDeviceStatus( DSNO_STACKER_FULL_STACK ) == DVST_TRAY_STK_STTS_NOT_FULL ) {

		if ( length > 2970) {  //��T�C�Y
			if ( pulseCnt >= SRVC_GetNvmValueUS(NVMID_L_Full_STACK_POS_PULSE_LIMIT) ) {
				SRVC_UpdateDeviceStatus(DVST_TRAY_STK_STTS_FULL, DSNO_STACKER_FULL_STACK) ; //FullStack
				PrintStackerTrayLog_Short(eSTK_LOG_FULLSTK_FULL, (US)pulseCnt,  0, aSheetNo);  //Log	
			}
		} else { //�@���T�C�Y			
			if  ( pulseCnt >= SRVC_GetNvmValueUS(NVMID_S_Full_STACK_POS_PULSE_LIMIT) ) {
				SRVC_UpdateDeviceStatus(DVST_TRAY_STK_STTS_FULL, DSNO_STACKER_FULL_STACK) ; //FullStack
				PrintStackerTrayLog_Short(eSTK_LOG_FULLSTK_FULL, (US)pulseCnt,  0, aSheetNo);  //Log	
			}
		}
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_SetOverCountCheck
// Description   :  Compile�ʒu�ֈړ�������ASetOverCountCheck����
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_Manager_SetOverCountCheck(UC aSheetNo)
{
	US  length=SRVC_GetProcessSize(aSheetNo);	  		  
	UL  pulseCnt =TRAY_STK_GetPulse(); 
	US  nvmLimit =0;  //NVM - FullStack��������

	if ( aSheetNo == SHEETNO_NUM)  		return;

	//FullStackCheck .->. No FullStack 
	if ( SRVC_GetDeviceStatus( DSNO_STACKER_FULL_STACK ) == DVST_TRAY_STK_STTS_NOT_FULL )  {
		
		// Job���������́ACount���Ȃ�
		if ( gTrayManagerMember.isInitCondition == false ) {
			
			switch(SRVC_GetStapleMode(aSheetNo)) {

			case  eCdiFinStaple_SingleFront :
				gTrayManagerMember.setOverCount ++;
				//(CR-011)  �R�[�i�[StapleSetCount�iN1)�̃J�E���g�F�PSet10���ȉ���StapleJob�̃Z�b�g�����J�E���g����
				if ( SRVC_GetCompileCount(aSheetNo) <= 10 ) {				
					gTrayManagerMember.stapleSetCount++;
				}	
				PrintStackerTrayLog_Short(eSTK_LOG_FULLSTK_SETOVER, 
					(US)gTrayManagerMember.setOverCount, (US) gTrayManagerMember.stapleSetCount, aSheetNo);  //Log	
			break;					
			case  eCdiFinStaple_Off : 
			default :	 			
					return;				
			}		
		}
		
		 if ( length > 2970) {	//��T�C�Y
			 nvmLimit = SRVC_GetNvmValueUS(NVMID_L_CORNER_STAPLE_STACK_SET_LIMIT);
		 } else {
			 nvmLimit = SRVC_GetNvmValueUS(NVMID_S_CORNER_STAPLE_STACK_SET_LIMIT);
		 }

		// SetOverCount Check ..
		if ( gTrayManagerMember.setOverCount  > nvmLimit ) {
			//FullStack(SetOverCount Message)
			SRVC_UpdateDeviceStatus(DVST_TRAY_STK_STTS_SET_OVER_COUNT, DSNO_STACKER_FULL_STACK) ; 
			PrintStackerTrayLog_Short(eSTK_LOG_FULLSTK_SETOVER, (US)pulseCnt,  0, aSheetNo);  //Log	
		}
		 		  
		  SRVC_SetNvmValueUC(gTrayManagerMember.setOverCount , NVMID_NUM_STAPLE_SET); 
		  SRVC_SetNvmValueUC(gTrayManagerMember.stapleSetCount, NVMID_CORNER_STAPLE_COUNT_N1 ); 
		  CNvmInformer_SaveNvm();	
		  
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_FullStackReleaseCondition
// Description   : FullStack  ��������Check (CR-013)
// Parameter     :  
// Return        : true = ����(Count All Clear),  false=������ or ����(Count ����Clear)
///////////////////////////////////////////////////////////////////
UC     TRAY_Manager_FullStackReleaseCondition(void)
{
	UL  pulseCnt = SRVC_GetNvmValueUL(NVMID_ELEVATOR_MOT_PULSE_COUNT);  
	UC  retValue = false;

	if ( pulseCnt <= SRVC_GetNvmValueUS(NVMID_FULL_STACK_POSITION2) ) { //臒l2

		// �@FullStack����Message .. 
		SRVC_UpdateDeviceStatus(DVST_TRAY_STK_STTS_NOT_FULL, DSNO_STACKER_FULL_STACK) ; 
	
		if ( pulseCnt <= SRVC_GetNvmValueUS(NVMID_FULL_STACK_POSITION1 )) {   // 臒l1

			PrintStackerTrayLog_Short(eSTK_LOG_FULLSTK_RELEASE, 
									(US)pulseCnt, 
									(US)gTrayManagerMember.previousPulseCnt,
									(US)((gTrayManagerMember.setOverCount<<8)|0x0011)); //Log
			
			gTrayManagerMember.setOverCount = 0;  //Set OverCount=0				
  			gTrayManagerMember.stapleSetCount =0;	 //N1 Clear =0
			 retValue = true; 
				
		} else {  //  臒l1 < pulseCnt <=  臒l2 
		
			//SetOverCount��N1�𔼕��ɂ���
			if ( gTrayManagerMember.setOverCount > 1 ) {
				gTrayManagerMember.setOverCount = gTrayManagerMember.setOverCount / 2;  //SetOVerCount�����ɂ���					
			}
			
			if ( gTrayManagerMember.stapleSetCount > 1) {
				gTrayManagerMember.stapleSetCount =gTrayManagerMember.stapleSetCount / 2;	 //N1 �����ɂ��� 
			}
			
			PrintStackerTrayLog_Short(eSTK_LOG_FULLSTK_RELEASE, 
									(US)pulseCnt, 
									(US)gTrayManagerMember.previousPulseCnt,
									(US)((gTrayManagerMember.setOverCount<<8) | 0x00FF) ); //Log
		
		}		
		SRVC_SetNvmValueUC(gTrayManagerMember.setOverCount , NVMID_NUM_STAPLE_SET); 
		SRVC_SetNvmValueUC(gTrayManagerMember.stapleSetCount, NVMID_CORNER_STAPLE_COUNT_N1 ); 

		// �ύڗp���̍ő�T�C�Y Clear 
		SRVC_SetNvmValueUS(0,	NVMID_SHEET_WIDTH_MAXIMUM_SIZE_SHEET);		
		SRVC_SetNvmValueUS(0,	NVMID_SHEET_LENGTH_MAXIMUM_SIZE_SHEET);
		//MixFullStack�����L���@Clear
		 SRVC_SetNvmValueUC(0,  NVMID_MIX_STACK); 

	} 
	
	// �ȑO�����PulseCount Save..
	gTrayManagerMember.previousPulseCnt  = pulseCnt;
	SRVC_SetNvmValueUL(gTrayManagerMember.previousPulseCnt, NVMID_BEFORE_ELEVATOR_MOT_PULSE_COUNT);
	CNvmInformer_SaveNvm();

	return  retValue;
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_ClearCheck
// Description   : FullStack  Clear Check (CR-013)
// Parameter     :  
// Return        : 
///////////////////////////////////////////////////////////////////
UC   TRAY_Manager_ClearCheck(void)
{
	UL  pulseCnt = SRVC_GetNvmValueUL(NVMID_ELEVATOR_MOT_PULSE_COUNT);  
	US  position1 = SRVC_GetNvmValueUS(NVMID_FULL_STACK_POSITION1 ) ;  
	UC  retValue = false;

	//Count Clear Check ..
	 // Clear�����F (PulseCnt <  ����1)  && ( N1 <= 30) &&	(�ȑO�����PulseCount�l >= ����1)
	if ( pulseCnt < position1) { // 臒l1

	   // ( �ȑO�����PulseCount�l >= ����1 )
	   if ( gTrayManagerMember.previousPulseCnt >= (position1 + 41) ) {// PF-QETFull���m�œK��
	    
	   	  PrintStackerTrayLog_Short(eSTK_LOG_FULLSTK_CLEAR, 
					(US)pulseCnt, 
					(US)gTrayManagerMember.previousPulseCnt,
					(US)((gTrayManagerMember.setOverCount<<8) | (gTrayManagerMember.stapleSetCount)) ); //Log
			
		
		   gTrayManagerMember.setOverCount = 0;  //SetOver Count Clear				   
		   gTrayManagerMember.stapleSetCount =0;	//N1 Clear //(CR-012)Clear�����F   

	   	  SRVC_SetNvmValueUC(gTrayManagerMember.setOverCount , NVMID_NUM_STAPLE_SET); 
		  SRVC_SetNvmValueUC(gTrayManagerMember.stapleSetCount, NVMID_CORNER_STAPLE_COUNT_N1 ); 
		  retValue = true;
	   }

	}  
	
	// �ȑO�����PulseCount Save..
	gTrayManagerMember.previousPulseCnt  = pulseCnt;
	SRVC_SetNvmValueUL(gTrayManagerMember.previousPulseCnt, NVMID_BEFORE_ELEVATOR_MOT_PULSE_COUNT);
	return 	retValue;
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_PreviousPulseUpdate
// Description   : Compile�ʒu�ֈړ����삪LiftDown����̎��A�ȑO�p���X�X�V�݂̂���E
//			(V03.00.90  PreDVT T/B �΍�)
// Parameter     :  currentPulse
// Return        : -
///////////////////////////////////////////////////////////////////
void  TRAY_Manager_PreviousPulseUpdate(UL   currentPulse)
{
	US   log=0;  //V3.5.1 Log change .. 
	
	// Job���������́AUpdate���Ȃ�..... 
	if ( gTrayManagerMember.isInitCondition == false ) {

		log = gTrayManagerMember.setOverCount;
		log = (US)((log<<8) |(gTrayManagerMember.stapleSetCount));
		

		PrintStackerTrayLog_Short(eSTK_LOG_FULLSTK_PREVUPDATE, 
							(US)currentPulse, 
							(US)gTrayManagerMember.previousPulseCnt,
					//		(US)((gTrayManagerMember.setOverCount<<8) | (gTrayManagerMember.stapleSetCount) )); //Log
							log );

		// �ȑO�����PulseCount Save..
		gTrayManagerMember.previousPulseCnt  = currentPulse;
		SRVC_SetNvmValueUL(gTrayManagerMember.previousPulseCnt, NVMID_BEFORE_ELEVATOR_MOT_PULSE_COUNT);
	}			
	return;	
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_ReceiveSheetEventNotify
// Description   : Sheet Event Notify�C�x���g���s��M�������B
// Parameter     :  eventID (MSP Event ID)
//                   : sheetNo (Sheet No.)
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_Manager_SheetEventNotify(US eventID, UC sheetNo)
{
	//Cover Open Check : V3.1.7 & Fault Occur check (V3.6.92)
	if ((SRVC_GetCoverStatus_ID(CVR_ID_FIN_STAPLE_ILK) ==  CVR_STTS_OPEN) ||
		(TRAY_STK_IsOccurFault() == FAULT_OCCUR)){
		return;
	}
	
	switch (eventID) {
		
	case EVT_SHT_EVT_OUT_TRAY_EJECT_START :  //EJECT�ʒu�ֈړ��J�n 

		if ( TRAY_STK_IsExecutionDeviceAction() == false) {
			TRAY_STK_StartAct(TRAY_STK_EJECT,  sheetNo);
		}
		break;
		
	case EVT_SHT_EVT_SHT_INIT_STACKER_JOBSTART :	// EJECT Home����Start -> Stacker�J�n )	
	
		if ( TRAY_STK_IsExecutionDeviceAction() == false) {
			TRAY_Manager_StartSheetInitialize(sheetNo);  //Stacker Home����J�n 
		}
		break;

	// SetClamp ���슮��-> �ҋ@�ʒu�֏グ����J�nTrigger
	case EVT_SHT_EVT_SHT_INIT_STACKER_WAITING_POS_START : 

		if (TRAY_STK_IsExecutionDeviceAction() == true) {  // Waiting 		
			TRAY_STK_InformedStackerTrigger(); //�ҋ@�ʒu�֏グ����J�n
		}	
		break;

	case EVT_SHT_EVT_TRAYJOB_TRIGGER_1 :  //EjectWait
	
		if (TRAY_STK_IsExecutionDeviceAction() == false) {					
			TRAY_STK_StartAct(TRAY_STK_COMPILE,  sheetNo);
		}
		break;
		
	default:
		break;
	} 

}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_SheetInitializeRequest
// Description   : Sheet Action Request(Sheet Initialize Request)��M����
// Parameter     : evt (�C�x���g���b�Z�[�W(Sheet Action Request) )
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_Manager_SheetInitializeRequest(const UC* evt)
{
	EvtParamSheetActionRequest* apSheetAction = (EvtParamSheetActionRequest*)evt;

//Tray Failstatus ..
	if (TRAY_STK_IsOccurFault() == FAULT_NOT_OCCUR) {

		gTrayManagerMember.sheetNO =  apSheetAction->sheetNo;
		TRAY_Manager_CompleteSheetInitialize(); 
		  return;

	}	
		
	gTrayManagerMember.isInitCondition = true;
	gTrayManagerMember.sheetNO =  apSheetAction->sheetNo;

	if ( TRAY_STK_IsExecutionDeviceAction() == true) {  //: true�F���쒆�Afalse�F��~��
		TRAY_STK_DeviceStop();  //V3.1.7
	}
	
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_StartSheetInitialize
// Description   : �V�[�g�C�j�V�����C�Y����J�n
// Parameter     : aSheetNo(�w��Sheet No.)
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_Manager_StartSheetInitialize(UC aSheetNo)
{

	// Job Initialize ����̊J�n���� :  ����
	if (gTrayManagerMember.isInitCondition = true ) {

		if (  gTrayManagerMember.status == FIN_STT_READY) { //Normal Initialize ���͉������Ȃ�
			TRAY_STK_StandbyFailCheckStop();  //V3.4.0 
		}	
		
	}else {	
		// NormalInitialize����̊J�n���� 
		//1. EjectHomeSNR=OFF��CompileTray�ɗp����������SelfPrimingSensor=OFF
		//2. StapleReadySensorFail������ 		//3. StapleNG�N���A
		if( (DD_DIO_GetInput(DD_DI_EJECT_HOME_SNR) == ON ) &&
		    (SRVC_CheckUnejectedSheetExist(eCdiFinOutDest_OutTray) == false) &&  // false = ���r�o�̃V�[�g���Ȃ�
		    ( DD_DIO_GetInput(DD_DI_SELF_PRIMING_SNR) == ON ) && 
		    (SRVC_GetDeviceStatus(DSNO_STAPLE_READY_FAIL) ==FAULT_NOT_OCCUR) &&
		    (SRVC_GetDeviceStatus(DSNO_STAPLE_STATUS_NG) ==FAULT_NOT_OCCUR) &&
		    (SRVC_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_CLEAR) ) {

				
		} else {//NormalInitialize����̊J�n���� �ɓ��Ă͂܂�Ȃ�
			return;
		}

	}	
	
	gTrayManagerMember.setOverCount = SRVC_GetNvmValueUC(NVMID_NUM_STAPLE_SET);
	gTrayManagerMember.stapleSetCount = SRVC_GetNvmValueUC(NVMID_CORNER_STAPLE_COUNT_N1);
	gTrayManagerMember.previousPulseCnt = SRVC_GetNvmValueUL(NVMID_BEFORE_ELEVATOR_MOT_PULSE_COUNT);  

	
	//V3.1.7
	if (SRVC_GetCoverStatus_ID(CVR_ID_FIN_STAPLE_ILK) ==  CVR_STTS_CLOSE) {

		TRAY_STK_StartAct(TRAY_STK_HOME, aSheetNo);
	}	
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_StopSheetInitialize
// Description   : �V�[�g�C�j�V�����C�Y�����~
// Parameter     : aIndex(�V�[�P���X�Ǘ��ԍ�)
// Return        : �]
///////////////////////////////////////////////////////////////////
void  TRAY_Manager_StopSheetInitialize(UC aSheetNo)
{	
	TRAY_STK_DeviceStop();

	TRAY_Manager_CompleteSheetInitialize();	
	
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_CompleteSheetInitialize
// Description   : �V�[�g�C�j�V�����C�Y���슮��
// Parameter     : aIndex(�V�[�P���X�Ǘ��ԍ�)
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_Manager_CompleteSheetInitialize(void)
{
	EvtParamSheetActionResponse aMessage;

	aMessage.sheetNo = gTrayManagerMember.sheetNO; // 150427 

	MSP_SendMessage(MID_TRAY, EVT_SHT_RES_STACKER_SHT_INIT_TRAY, (UC*)(&aMessage), sizeof(aMessage), LOG_OFF);

	gTrayManagerMember.isInitCondition = false;	
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_ProcessEnd
// Description   : �v���Z�X�G���h����(�J�n/��~)
// Parameter     : aAction(����w��(�J�n/��~))
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_Manager_ProcessEnd(const UC* evt)
{
	EvtParamSheetActionRequest* apSheetAction = (EvtParamSheetActionRequest*)evt;

	gTrayManagerMember.sheetNO = apSheetAction->sheetNo;

	//(V3.1.7) StapleCover Open/Close��Ԋm�F���K�v�BTB�΍�(Job���ACoverOpen��StopFinisher��ProcessEnd���쁨StackerFail����)
	if (SRVC_GetCoverStatus_ID(CVR_ID_FIN_STAPLE_ILK) ==  CVR_STTS_OPEN) {
		
		TRAY_Manager_CompleteProcessEnd();		
		SRVC_UserDebugLog(0x43, 0x43);
		return;
	}
	
	if ( TRAY_STK_IsExecutionDeviceAction() == false) {  //: true�F���쒆�Afalse�F��~��

		if (SRVC_CheckUnfinishedSheetExist_2(eCdiFinOutDest_OutTray) == true) {
			
			TRAY_STK_PreStartAct(TRAY_STK_END, gTrayManagerMember.sheetNO );
			SRVC_UserDebugLog(0x41, 0x41);
		
		} else {  //Set Finish Request �� ���Ȃ�
			TRAY_STK_StartAct(TRAY_STK_END, gTrayManagerMember.sheetNO );
			SRVC_UserDebugLog(0x42, 0x42);
		}
	
	} else {
		TRAY_STK_PreStartAct(TRAY_STK_END, gTrayManagerMember.sheetNO );
		SRVC_UserDebugLog(0x40, 0x40);
	}	
	
	TRAY_Manager_CompleteProcessEnd();

	//�������d�l�@FullStack/LowerSafety������AStandby����SNR�ω�����ꍇ�AStacker����
	// Normal Mode & FailCheck & Standby
	//FullStackCheck .->. No FullStack || LowerSafety����
	if (( SRVC_GetDeviceStatus( DSNO_STACKER_FULL_STACK ) != DVST_TRAY_STK_STTS_NOT_FULL ) ||
		 (SRVC_GetDeviceStatus(DSNO_STACKER_LOW_SAFETY) == FAULT_OCCUR) ){
		 
		TRAY_STK_StandbyFailCheckStart();  //V3.4.0 
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_StartProcessEnd
// Description   : �v���Z�X�G���h����J�n
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_Manager_StartProcessEnd()
{

}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_StopProcessEnd
// Description   : �v���Z�X�G���h�����~
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_Manager_StopProcessEnd()
{
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_CompleteProcessEnd
// Description   : �v���Z�X�G���h���슮��
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void TRAY_Manager_CompleteProcessEnd()
{
	EvtParamSheetActionResponse aMessage;

	aMessage.sheetNo = gTrayManagerMember.sheetNO; 

	MSP_SendMessage(MID_TRAY, EVT_DEV_RES_PROCESS_END_TRAY, (UC*)(&aMessage), sizeof(aMessage), LOG_OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_GetStatus
// Description   : Tray Status .. 
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
UC TRAY_Manager_GetStatus()
{
	return  gTrayManagerMember.status;
}

/*	//Debug Log�p��� (V3.0)
	typedef enum {
		eSTK_LOG_PLS_RESET=0,  	//Reset���̃p���X
		eSTK_LOG_PLS_CURRENT,		//���݃p���X
		eSTK_LOG_UPDOWN,			//Compile�ʒu�ֈړ����AUpDown�@�F�@UP=0x00�ADOWN=0xFF
		eSTK_LOG_LOWLIMIT=0x10,	//LowLimit
		eSTK_LOG_FULLSTK_FULL,	//FULLStack����
		eSTK_LOG_FULLSTK_MIXSIZE_FULL,//MixFullStack����
		eSTK_LOG_FULLSTK_SETOVER,	//SetOverCount����
	}ESTK_LogCategory;
*/
///////////////////////////////////////////////////////////////////
// Function Name : PrintStackerTrayLog_Short
// Description   :  Pulse
// Parameter     : para1 (������̍ŏ��ɕt������l)
//               : para2 (������̂Q�Ԗڂɕt������l)
//               : para3 (������̂R�Ԗڂɕt������l)
//               : para4 (������̂S�Ԗڂɕt������l)
// Return        : �]
///////////////////////////////////////////////////////////////////
void PrintStackerTrayLog_Short(UC para1, US para2, US para3, US para4)
{

	UC 	log[7];

	log[0] =  (UC)para1 ;
	log[1] =  (UC)(para2 >>8 );
	log[2] =  (UC)(para2 & 0x00FF);

	log[3] =  (UC)(para3 >>8 );
	log[4] =  (UC)(para3 & 0x00FF);
	log[5] =  (UC)(para4 >>8 );	
	log[6] =  (UC)(para4 & 0x00FF);
	
//#define	LOGID_STK=13, 'S'=Send,   LOG_CATEGORY_DEBUG=2
	DD_LOG_Put(LOGID_STK, 'S', log, sizeof(log), LOG_CATEGORY_DEBUG);

}

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
