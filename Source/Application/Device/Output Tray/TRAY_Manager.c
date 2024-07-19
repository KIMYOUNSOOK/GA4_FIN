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
	UC    isInitCondition;  //[SheetInitialize]で待ち(HIGH),  SubPaddleHome動作完了Event受信から開始
	UC    sheetNO;   //Sheet No 
	UC    status;    // Tray Status (US -> UC)
	UC    setOverCount;  //SetOverCount
	UC    stapleSetCount;  //(CR-011) StapleSetCount(１Set10枚以下)
	UL    previousPulseCnt;  //(CR-013) 以前動作のPulseCountを格納
} TRAY_ManagerMember;

static TRAY_ManagerMember gTrayManagerMember;

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_Constructor
// Description   : コンストラク?
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
	
	//V3.2.0 Job初期化中、HardDown受信の場合、DeviceStop
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

	//V3.2.0 Job初期化中、CycleDown受信の場合、DeviceStop
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
// Description   : Standby->CycleUp 前処理
// Parameter     : evt (Sheet Information )
// Return        : ‐
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
	//V3.1.7 StapleCover Open時のみ処理  	
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

		default :	// 未定義のLink No. No Action
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_InformedStackerActionComplete
// Description   : TRAY_STK_Device から結果受信
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

	case TRAY_STK_HOME :  // Stacker Home位置へ移動() - Sheet Initialize 
		if( !TRAY_STK_IsOccurFault() ) {
			 TRAY_STK_StartAct(TRAY_STK_COMPILE,  aSheetNo); //HOME位置へ完了でCompile位置へ移動開始
		}
		break;	
		
	case TRAY_STK_COMPILE : //Stacker Compile位置へ移動()
		if ( gTrayManagerMember.isInitCondition ) { // Sheet Initialize Condition -> CompleteSheet Initialize 

			TRAY_Manager_LowerLimitCheck(gTrayManagerMember.sheetNO); 
			TRAY_Manager_MixFullStackCheckAfterCompile(gTrayManagerMember.sheetNO);  //MixFullStack 
			TRAY_Manager_FullStackCheckAfterCompile(gTrayManagerMember.sheetNO); //FullStack check
			//(TB-XX)150603 Job初期化時、SetOverCountをチェックする
			TRAY_Manager_SetOverCountCheck(gTrayManagerMember.sheetNO); //SetOverCount check
			TRAY_Manager_CompleteSheetInitialize();

		} else {	
			TRAY_Manager_LowerLimitCheck(aSheetNo);			
			TRAY_Manager_MixFullStackCheckAfterCompile(aSheetNo); //V3.0導入
			TRAY_Manager_SaveNVM(aSheetNo);  //V3.0導入			
			TRAY_Manager_FullStackCheckAfterCompile(aSheetNo); //FullStack check
			TRAY_Manager_SetOverCountCheck(aSheetNo); //SetOverCount check
		}		
		break;
		
	case TRAY_STK_EJECT :  //Stacker Eject位置へ移動()
		
		break;
		
	case TRAY_STK_END : //待機位置へ移動(ProcessEnd)
	//	TRAY_Manager_CompleteProcessEnd();
		break;
	default :
		break;
	}	
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_LowerLimitCheck
// Description   :  Compile位置へ移動完了後・LowLimitCheckする
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

		if ( length > 2970) {  //大サイズ			
			nvmLimit = SRVC_GetNvmValueUS(NVMID_L_Full_STACK_POS_PULSE_LIMIT);	
		} else { //　小サイズ			
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
// Description   :  Compile位置へ移動完了後、MixFullStackCheckする
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_Manager_MixFullStackCheckAfterCompile(UC aSheetNo)
{	
	UL  pulseCnt = TRAY_STK_GetPulse();

	//最終排出用紙の長さ
	US	length_last_ejected = SRVC_GetProcessSize(aSheetNo);	
	//最終排出用紙の幅  : 
	US	width_last_ejected = SRVC_GetCrossProcessSize(aSheetNo);	
	
	US  width_maximum = 0;
	US  length_maximum = 0;
	
	if ( aSheetNo == SHEETNO_NUM) 	//検知しない
		return;

	if ( SRVC_GetNvmValueUC(NVMID_MIX_STACK_ENABLE) == 0 ) //MixFullStack検知有無
		return;

	// FullStack = Not Occur => FullStackチェック
	if ( SRVC_GetDeviceStatus( DSNO_STACKER_FULL_STACK ) == DVST_TRAY_STK_STTS_NOT_FULL ) {
	
		//積載用紙の最大サイズ(幅）
		width_maximum = SRVC_GetNvmValueUS(NVMID_SHEET_WIDTH_MAXIMUM_SIZE_SHEET);
		//積載用紙の最大サイズ(長さ）
	 	length_maximum = SRVC_GetNvmValueUS(NVMID_SHEET_LENGTH_MAXIMUM_SIZE_SHEET);

		if ( (width_maximum == 0) || (length_maximum == 0) )
			return;
		
		//	下記を満たしている。 ->MixFull Check 
		//	「積載用紙最大サイズ」より「最終排出用紙サイズ」（用紙幅/用紙長のどちらか）が大きい 	
		if ((width_last_ejected > width_maximum) || (length_last_ejected > length_maximum)) {
		
			//	StackerMotorのパルスカウント　>=　MixFullStackパルスカウント値　の場合, MixFullStackと判断する
			if ( pulseCnt >= SRVC_GetNvmValueUS(NVMID_MIX_FULL_STACK_POS_PULSE_LIMIT) ) {
				SRVC_UpdateDeviceStatus(DVST_TRAY_STK_STTS_MIX_SIZE_FULL, DSNO_STACKER_FULL_STACK) ; //Mix FullStack
				PrintStackerTrayLog_Short(eSTK_LOG_FULLSTK_MIXSIZE_FULL, (US)pulseCnt,  0, aSheetNo);  //Log
				PrintStackerTrayLog_Short(eSTK_LOG_FULLSTK_MIXSIZE_FULL, (US)width_last_ejected,  width_maximum, aSheetNo);  //Log
				PrintStackerTrayLog_Short(eSTK_LOG_FULLSTK_MIXSIZE_FULL, (US)length_last_ejected,  length_maximum, aSheetNo);  //Log

				 SRVC_SetNvmValueUC(1,  NVMID_MIX_STACK); //MixFullStack発生有無：1=発生
			}
		}
	}

}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_SaveNVM
// Description   :  Compile位置へ移動完了後、各種NVM( StackerTray NVM)を格納する。
// Parameter     : aSheetNo
// Return        : 無 
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
	
	//最終排出用紙の幅	: 
	SRVC_SetNvmValueUS( width,	NVMID_SHEET_WIDTH_LAST_EJECTED_SHEET); 
	//最終排出用紙の長さ
	SRVC_SetNvmValueUS( length,  NVMID_SHEET_LENGTH_LAST_EJECTED_SHEET);

	//積載用紙の最大サイズ(幅）
	width_maximum = SRVC_GetNvmValueUS(NVMID_SHEET_WIDTH_MAXIMUM_SIZE_SHEET);

	if ( width > width_maximum ) {
		SRVC_SetNvmValueUS(width,  NVMID_SHEET_WIDTH_MAXIMUM_SIZE_SHEET);
	}
	
	//積載用紙の最大サイズ(長さ）
	length_maximum = SRVC_GetNvmValueUS(NVMID_SHEET_LENGTH_MAXIMUM_SIZE_SHEET);

	if ( length > length_maximum ) {
		SRVC_SetNvmValueUS(length,  NVMID_SHEET_LENGTH_MAXIMUM_SIZE_SHEET);
	}	

	//最終排出セットのStaplePosition　：0=UnStaple、1=Corner
	if ( SRVC_GetStapleMode(aSheetNo) ==  eCdiFinStaple_SingleFront )
		SRVC_SetNvmValueUC( 1 , NVMID_NUM_SET_STAPLE_POSITION);
	else 
		SRVC_SetNvmValueUC( 0 , NVMID_NUM_SET_STAPLE_POSITION);
	
	//StackerTray排出枚数
	totalCount = SRVC_GetNvmValueUL(NVMID_STACK_PAPER_LIMIT) ;
	totalCount += SRVC_GetCompileCount(aSheetNo);	
	SRVC_SetNvmValueUL(totalCount,   NVMID_STACK_PAPER_LIMIT);

	return;
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_FullStackCheckAfterCompile
// Description   :  Compile位置へ移動完了後、FullStackCheckする
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_Manager_FullStackCheckAfterCompile(UC aSheetNo)
{	
	US  length = SRVC_GetProcessSize(aSheetNo);
	UL  pulseCnt = TRAY_STK_GetPulse();


	if ( aSheetNo == SHEETNO_NUM)  		return;
		
	// FullStack = Not Occur => FullStackチェック
	if ( SRVC_GetDeviceStatus( DSNO_STACKER_FULL_STACK ) == DVST_TRAY_STK_STTS_NOT_FULL ) {

		if ( length > 2970) {  //大サイズ
			if ( pulseCnt >= SRVC_GetNvmValueUS(NVMID_L_Full_STACK_POS_PULSE_LIMIT) ) {
				SRVC_UpdateDeviceStatus(DVST_TRAY_STK_STTS_FULL, DSNO_STACKER_FULL_STACK) ; //FullStack
				PrintStackerTrayLog_Short(eSTK_LOG_FULLSTK_FULL, (US)pulseCnt,  0, aSheetNo);  //Log	
			}
		} else { //　小サイズ			
			if  ( pulseCnt >= SRVC_GetNvmValueUS(NVMID_S_Full_STACK_POS_PULSE_LIMIT) ) {
				SRVC_UpdateDeviceStatus(DVST_TRAY_STK_STTS_FULL, DSNO_STACKER_FULL_STACK) ; //FullStack
				PrintStackerTrayLog_Short(eSTK_LOG_FULLSTK_FULL, (US)pulseCnt,  0, aSheetNo);  //Log	
			}
		}
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_SetOverCountCheck
// Description   :  Compile位置へ移動完了後、SetOverCountCheckする
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRAY_Manager_SetOverCountCheck(UC aSheetNo)
{
	US  length=SRVC_GetProcessSize(aSheetNo);	  		  
	UL  pulseCnt =TRAY_STK_GetPulse(); 
	US  nvmLimit =0;  //NVM - FullStack部数制限

	if ( aSheetNo == SHEETNO_NUM)  		return;

	//FullStackCheck .->. No FullStack 
	if ( SRVC_GetDeviceStatus( DSNO_STACKER_FULL_STACK ) == DVST_TRAY_STK_STTS_NOT_FULL )  {
		
		// Job初期化時は、Countしない
		if ( gTrayManagerMember.isInitCondition == false ) {
			
			switch(SRVC_GetStapleMode(aSheetNo)) {

			case  eCdiFinStaple_SingleFront :
				gTrayManagerMember.setOverCount ++;
				//(CR-011)  コーナーStapleSetCount（N1)のカウント：１Set10枚以下のStapleJobのセット数をカウントする
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
		
		 if ( length > 2970) {	//大サイズ
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
// Description   : FullStack  解除条件Check (CR-013)
// Parameter     :  
// Return        : true = 解除(Count All Clear),  false=未解除 or 解除(Count 半分Clear)
///////////////////////////////////////////////////////////////////
UC     TRAY_Manager_FullStackReleaseCondition(void)
{
	UL  pulseCnt = SRVC_GetNvmValueUL(NVMID_ELEVATOR_MOT_PULSE_COUNT);  
	UC  retValue = false;

	if ( pulseCnt <= SRVC_GetNvmValueUS(NVMID_FULL_STACK_POSITION2) ) { //閾値2

		// 　FullStack解除Message .. 
		SRVC_UpdateDeviceStatus(DVST_TRAY_STK_STTS_NOT_FULL, DSNO_STACKER_FULL_STACK) ; 
	
		if ( pulseCnt <= SRVC_GetNvmValueUS(NVMID_FULL_STACK_POSITION1 )) {   // 閾値1

			PrintStackerTrayLog_Short(eSTK_LOG_FULLSTK_RELEASE, 
									(US)pulseCnt, 
									(US)gTrayManagerMember.previousPulseCnt,
									(US)((gTrayManagerMember.setOverCount<<8)|0x0011)); //Log
			
			gTrayManagerMember.setOverCount = 0;  //Set OverCount=0				
  			gTrayManagerMember.stapleSetCount =0;	 //N1 Clear =0
			 retValue = true; 
				
		} else {  //  閾値1 < pulseCnt <=  閾値2 
		
			//SetOverCountとN1を半分にする
			if ( gTrayManagerMember.setOverCount > 1 ) {
				gTrayManagerMember.setOverCount = gTrayManagerMember.setOverCount / 2;  //SetOVerCount半分にする					
			}
			
			if ( gTrayManagerMember.stapleSetCount > 1) {
				gTrayManagerMember.stapleSetCount =gTrayManagerMember.stapleSetCount / 2;	 //N1 半分にする 
			}
			
			PrintStackerTrayLog_Short(eSTK_LOG_FULLSTK_RELEASE, 
									(US)pulseCnt, 
									(US)gTrayManagerMember.previousPulseCnt,
									(US)((gTrayManagerMember.setOverCount<<8) | 0x00FF) ); //Log
		
		}		
		SRVC_SetNvmValueUC(gTrayManagerMember.setOverCount , NVMID_NUM_STAPLE_SET); 
		SRVC_SetNvmValueUC(gTrayManagerMember.stapleSetCount, NVMID_CORNER_STAPLE_COUNT_N1 ); 

		// 積載用紙の最大サイズ Clear 
		SRVC_SetNvmValueUS(0,	NVMID_SHEET_WIDTH_MAXIMUM_SIZE_SHEET);		
		SRVC_SetNvmValueUS(0,	NVMID_SHEET_LENGTH_MAXIMUM_SIZE_SHEET);
		//MixFullStack発生有無　Clear
		 SRVC_SetNvmValueUC(0,  NVMID_MIX_STACK); 

	} 
	
	// 以前動作のPulseCount Save..
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
	 // Clear条件： (PulseCnt <  解除1)  && ( N1 <= 30) &&	(以前動作のPulseCount値 >= 解除1)
	if ( pulseCnt < position1) { // 閾値1

	   // ( 以前動作のPulseCount値 >= 解除1 )
	   if ( gTrayManagerMember.previousPulseCnt >= (position1 + 41) ) {// PF-QETFull検知最適化
	    
	   	  PrintStackerTrayLog_Short(eSTK_LOG_FULLSTK_CLEAR, 
					(US)pulseCnt, 
					(US)gTrayManagerMember.previousPulseCnt,
					(US)((gTrayManagerMember.setOverCount<<8) | (gTrayManagerMember.stapleSetCount)) ); //Log
			
		
		   gTrayManagerMember.setOverCount = 0;  //SetOver Count Clear				   
		   gTrayManagerMember.stapleSetCount =0;	//N1 Clear //(CR-012)Clear条件：   

	   	  SRVC_SetNvmValueUC(gTrayManagerMember.setOverCount , NVMID_NUM_STAPLE_SET); 
		  SRVC_SetNvmValueUC(gTrayManagerMember.stapleSetCount, NVMID_CORNER_STAPLE_COUNT_N1 ); 
		  retValue = true;
	   }

	}  
	
	// 以前動作のPulseCount Save..
	gTrayManagerMember.previousPulseCnt  = pulseCnt;
	SRVC_SetNvmValueUL(gTrayManagerMember.previousPulseCnt, NVMID_BEFORE_ELEVATOR_MOT_PULSE_COUNT);
	return 	retValue;
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_PreviousPulseUpdate
// Description   : Compile位置へ移動動作がLiftDown動作の時、以前パルス更新のみする・
//			(V03.00.90  PreDVT T/B 対策)
// Parameter     :  currentPulse
// Return        : -
///////////////////////////////////////////////////////////////////
void  TRAY_Manager_PreviousPulseUpdate(UL   currentPulse)
{
	US   log=0;  //V3.5.1 Log change .. 
	
	// Job初期化時は、Updateしない..... 
	if ( gTrayManagerMember.isInitCondition == false ) {

		log = gTrayManagerMember.setOverCount;
		log = (US)((log<<8) |(gTrayManagerMember.stapleSetCount));
		

		PrintStackerTrayLog_Short(eSTK_LOG_FULLSTK_PREVUPDATE, 
							(US)currentPulse, 
							(US)gTrayManagerMember.previousPulseCnt,
					//		(US)((gTrayManagerMember.setOverCount<<8) | (gTrayManagerMember.stapleSetCount) )); //Log
							log );

		// 以前動作のPulseCount Save..
		gTrayManagerMember.previousPulseCnt  = currentPulse;
		SRVC_SetNvmValueUL(gTrayManagerMember.previousPulseCnt, NVMID_BEFORE_ELEVATOR_MOT_PULSE_COUNT);
	}			
	return;	
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_ReceiveSheetEventNotify
// Description   : Sheet Event Notifyイベント発行受信時処理。
// Parameter     :  eventID (MSP Event ID)
//                   : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_Manager_SheetEventNotify(US eventID, UC sheetNo)
{
	//Cover Open Check : V3.1.7 & Fault Occur check (V3.6.92)
	if ((SRVC_GetCoverStatus_ID(CVR_ID_FIN_STAPLE_ILK) ==  CVR_STTS_OPEN) ||
		(TRAY_STK_IsOccurFault() == FAULT_OCCUR)){
		return;
	}
	
	switch (eventID) {
		
	case EVT_SHT_EVT_OUT_TRAY_EJECT_START :  //EJECT位置へ移動開始 

		if ( TRAY_STK_IsExecutionDeviceAction() == false) {
			TRAY_STK_StartAct(TRAY_STK_EJECT,  sheetNo);
		}
		break;
		
	case EVT_SHT_EVT_SHT_INIT_STACKER_JOBSTART :	// EJECT Home動作ｹStart -> Stacker開始 )	
	
		if ( TRAY_STK_IsExecutionDeviceAction() == false) {
			TRAY_Manager_StartSheetInitialize(sheetNo);  //Stacker Home動作開始 
		}
		break;

	// SetClamp 動作完了-> 待機位置へ上げ動作開始Trigger
	case EVT_SHT_EVT_SHT_INIT_STACKER_WAITING_POS_START : 

		if (TRAY_STK_IsExecutionDeviceAction() == true) {  // Waiting 		
			TRAY_STK_InformedStackerTrigger(); //待機位置へ上げ動作開始
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
// Description   : Sheet Action Request(Sheet Initialize Request)受信処理
// Parameter     : evt (イベントメッセージ(Sheet Action Request) )
// Return        : ‐
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

	if ( TRAY_STK_IsExecutionDeviceAction() == true) {  //: true：動作中、false：停止中
		TRAY_STK_DeviceStop();  //V3.1.7
	}
	
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_StartSheetInitialize
// Description   : シートイニシャライズ動作開始
// Parameter     : aSheetNo(指定Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_Manager_StartSheetInitialize(UC aSheetNo)
{

	// Job Initialize 動作の開始条件 :  無し
	if (gTrayManagerMember.isInitCondition = true ) {

		if (  gTrayManagerMember.status == FIN_STT_READY) { //Normal Initialize 時は解除しない
			TRAY_STK_StandbyFailCheckStop();  //V3.4.0 
		}	
		
	}else {	
		// NormalInitialize動作の開始条件 
		//1. EjectHomeSNR=OFF＆CompileTrayに用紙が無い＆SelfPrimingSensor=OFF
		//2. StapleReadySensorFail未発生 		//3. StapleNGクリア
		if( (DD_DIO_GetInput(DD_DI_EJECT_HOME_SNR) == ON ) &&
		    (SRVC_CheckUnejectedSheetExist(eCdiFinOutDest_OutTray) == false) &&  // false = 未排出のシートがない
		    ( DD_DIO_GetInput(DD_DI_SELF_PRIMING_SNR) == ON ) && 
		    (SRVC_GetDeviceStatus(DSNO_STAPLE_READY_FAIL) ==FAULT_NOT_OCCUR) &&
		    (SRVC_GetDeviceStatus(DSNO_STAPLE_STATUS_NG) ==FAULT_NOT_OCCUR) &&
		    (SRVC_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_CLEAR) ) {

				
		} else {//NormalInitialize動作の開始条件 に当てはまらない
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
// Description   : シートイニシャライズ動作停止
// Parameter     : aIndex(シーケンス管理番号)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void  TRAY_Manager_StopSheetInitialize(UC aSheetNo)
{	
	TRAY_STK_DeviceStop();

	TRAY_Manager_CompleteSheetInitialize();	
	
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_CompleteSheetInitialize
// Description   : シートイニシャライズ動作完了
// Parameter     : aIndex(シーケンス管理番号)
// Return        : ‐
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
// Description   : プロセスエンド動作(開始/停止)
// Parameter     : aAction(動作指示(開始/停止))
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_Manager_ProcessEnd(const UC* evt)
{
	EvtParamSheetActionRequest* apSheetAction = (EvtParamSheetActionRequest*)evt;

	gTrayManagerMember.sheetNO = apSheetAction->sheetNo;

	//(V3.1.7) StapleCover Open/Close状態確認が必要。TB対策(Job中、CoverOpen→StopFinisherでProcessEnd動作→StackerFail発生)
	if (SRVC_GetCoverStatus_ID(CVR_ID_FIN_STAPLE_ILK) ==  CVR_STTS_OPEN) {
		
		TRAY_Manager_CompleteProcessEnd();		
		SRVC_UserDebugLog(0x43, 0x43);
		return;
	}
	
	if ( TRAY_STK_IsExecutionDeviceAction() == false) {  //: true：動作中、false：停止中

		if (SRVC_CheckUnfinishedSheetExist_2(eCdiFinOutDest_OutTray) == true) {
			
			TRAY_STK_PreStartAct(TRAY_STK_END, gTrayManagerMember.sheetNO );
			SRVC_UserDebugLog(0x41, 0x41);
		
		} else {  //Set Finish Request が 来ない
			TRAY_STK_StartAct(TRAY_STK_END, gTrayManagerMember.sheetNO );
			SRVC_UserDebugLog(0x42, 0x42);
		}
	
	} else {
		TRAY_STK_PreStartAct(TRAY_STK_END, gTrayManagerMember.sheetNO );
		SRVC_UserDebugLog(0x40, 0x40);
	}	
	
	TRAY_Manager_CompleteProcessEnd();

	//未導入仕様　FullStack/LowerSafety発生後、Standby時にSNR変化ある場合、Stacker動作
	// Normal Mode & FailCheck & Standby
	//FullStackCheck .->. No FullStack || LowerSafety発生
	if (( SRVC_GetDeviceStatus( DSNO_STACKER_FULL_STACK ) != DVST_TRAY_STK_STTS_NOT_FULL ) ||
		 (SRVC_GetDeviceStatus(DSNO_STACKER_LOW_SAFETY) == FAULT_OCCUR) ){
		 
		TRAY_STK_StandbyFailCheckStart();  //V3.4.0 
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_StartProcessEnd
// Description   : プロセスエンド動作開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_Manager_StartProcessEnd()
{

}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_StopProcessEnd
// Description   : プロセスエンド動作停止
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_Manager_StopProcessEnd()
{
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Manager_CompleteProcessEnd
// Description   : プロセスエンド動作完了
// Parameter     : ‐
// Return        : ‐
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
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
UC TRAY_Manager_GetStatus()
{
	return  gTrayManagerMember.status;
}

/*	//Debug Log用情報 (V3.0)
	typedef enum {
		eSTK_LOG_PLS_RESET=0,  	//Reset時のパルス
		eSTK_LOG_PLS_CURRENT,		//現在パルス
		eSTK_LOG_UPDOWN,			//Compile位置へ移動時、UpDown　：　UP=0x00、DOWN=0xFF
		eSTK_LOG_LOWLIMIT=0x10,	//LowLimit
		eSTK_LOG_FULLSTK_FULL,	//FULLStack発生
		eSTK_LOG_FULLSTK_MIXSIZE_FULL,//MixFullStack発生
		eSTK_LOG_FULLSTK_SETOVER,	//SetOverCount発生
	}ESTK_LogCategory;
*/
///////////////////////////////////////////////////////////////////
// Function Name : PrintStackerTrayLog_Short
// Description   :  Pulse
// Parameter     : para1 (数字列の最初に付加する値)
//               : para2 (数字列の２番目に付加する値)
//               : para3 (数字列の３番目に付加する値)
//               : para4 (数字列の４番目に付加する値)
// Return        : ‐
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
