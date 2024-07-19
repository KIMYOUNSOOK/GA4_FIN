// File Name:	CMPL_EJCT_Boundary.c
// Description:	Compiler Ejector Module Boundary
// Engineer:	Takahiko Minaga
// Last Edit:	14.09.04
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"
#include "ECMPL_EJCT_State.h"
#include "CMPL_EJCT_Boundary.h"
#include "CMPL_EJCT_BELT_Device.h"
#include "CMPL_EJCT_SCLMP_Device.h"
#include "CMPL_Manager.h"
#include "DD_STM.h"
#include "DD_DIO.h"
#include "DD_PWM.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////
#define CMPL_EJCT_COMPO_CHAIN 12

ECMPL_EJCT_State gCMPL_EJCT_State;
UC gCMPL_EJCT_CompileSheet;
US gCMPL_EJCT_EjectClampResultChk; //Eject動作時、SetClampも一緒にチェックするための変数


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
static void CMPL_EJCT_ChangeStandbyEvent();
static void CMPL_EJCT_ChangeReadyEvent();
static void CMPL_EJCT_ChangeDiagEvent();
static void CMPL_EJCT_StartCompoEjcterMotor(US aLinkNo);
static void CMPL_EJCT_StopCompoEjcterMotor(US aLinkNo);
// static void CMPL_EJCT_CompleteCompoEjcterMotor(US aLinkNo);


static void CMPL_EJCT_StartEjectorHome(UC aSheetNo);
static void CMPL_EJCT_CompleteEjectorHome(UC aSheetNo);

static void CMPL_EJCT_StartPaddleHome(UC aSheetNo);
static void CMPL_EJCT_CompletePaddleHome(UC aSheetNo);

static void CMPL_EJCT_StartPaddleCompile(UC aSheetNo);
static void CMPL_EJCT_CompletePaddleCompile(UC aSheetNo);

static void CMPL_EJCT_StartEject(UC aSheetNo);
static void CMPL_EJCT_CompleteEject(UC aSheetNo);

static void CMPL_EJCT_InformedCompoControlStart(SS aTimerID, UL aChainLink);
static void CMPL_EJCT_InformedCompoControlComplete(SS aTimerID, UL aChainLink);
static void CMPL_EJCT_InformedModuleActionComplete(SS aTimerID, UL aActionData);

/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_Reset
// Description   : イジェクタリセット
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_Reset()
{
	gCMPL_EJCT_State = CMPL_EJCT_STT_UNKNOWN;
	gCMPL_EJCT_CompileSheet = SHEETNO_NUM;

	gCMPL_EJCT_EjectClampResultChk = 0;

	CMPL_EJCT_BELT_Reset();
	CMPL_EJCT_SCLMP_Reset();

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SetUp
// Description   : イジェクタセットアップ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_SetUp()
{
	/* No Action */
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_PowerOff
// Description   : イジェクタパワーオフ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_PowerOff()
{
	CMPL_EJCT_StopModuleAction();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_Normal
// Description   : イジェクタノーマル
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_Normal()
{
	CMPL_EJCT_ChangeStandbyEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_Diag
// Description   : イジェクタダイアグ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_Diag()
{
	CMPL_EJCT_ChangeDiagEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_ProcStart
// Description   : プロセススタート
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_ProcStart()
{
	CMPL_EJCT_ChangeReadyEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_StopSteadyFinishing
// Description   : イジェクタフィニッシングプロセス終了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_StopSteadyFinishing()
{
	CMPL_EJCT_ChangeStandbyEvent();
}

void CMPL_EJCT_StandbyEjectStartNotify()
{
	/* No Action */
}

void CMPL_EJCT_StandbyEjectCompNotify()
{
	/* No Action */
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_CurrentOFF
// Description   :  Eject CurrentOFF 
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_CurrentOFF()
{
	//Eject励磁OFF
	DD_PWM_Stop(DD_PWM_ID_EJECT_BELT_CUR);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_ENB, OFF);	
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_ChangeStandbyEvent
// Description   : スタンバイ遷移イベント
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_ChangeStandbyEvent()
{
	if( (gCMPL_EJCT_State == CMPL_EJCT_STT_UNKNOWN) ||
		(gCMPL_EJCT_State == CMPL_EJCT_STT_DIAG) ||
		(gCMPL_EJCT_State == CMPL_EJCT_STT_READY) ) {
		gCMPL_EJCT_State = CMPL_EJCT_STT_STANDBY;
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_ChangeReadyEvent
// Description   : レディー遷移イベント
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_ChangeReadyEvent()
{
	if( gCMPL_EJCT_State == CMPL_EJCT_STT_STANDBY ) {
		gCMPL_EJCT_State = CMPL_EJCT_STT_READY;
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_ChangeDiagEvent
// Description   : ダイアグ遷移イベント
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_ChangeDiagEvent()
{
	if( (gCMPL_EJCT_State == CMPL_EJCT_STT_UNKNOWN) ||
		(gCMPL_EJCT_State == CMPL_EJCT_STT_STANDBY) ) {
		gCMPL_EJCT_State = CMPL_EJCT_STT_DIAG;
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_AnyCoverOpen
// Description   : いずれかのカバーが開いた
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_AnyCoverOpen()
{
	if( gCMPL_EJCT_State != CMPL_EJCT_STT_DIAG ) {
		CMPL_EJCT_StopModuleAction();
	}
	else {																		// = FIN_STT_DIAGNOSTICS
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_AllCoverClose
// Description   : すべてのカバーが閉まった
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_AllCoverClose()
{
	/* No Action */
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_StopModuleAction
// Description   : イジェクタ動作停止
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_StopModuleAction()
{
	CMPL_EJCT_BELT_DeviceStop();
	CMPL_EJCT_SCLMP_DeviceStop();	
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_StopSheet
// Description   : 指定シートの動作停止
// Parameter     : aSheetNo (指定シート番号)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_StopSheet(UC aSheetNo)
{
	CMPL_EJCT_BELT_StopSheet(aSheetNo);
	CMPL_EJCT_SCLMP_StopSheet(aSheetNo);	
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SheetAbort
// Description   : SheetAbortが発生した旨の通知
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_SheetAbort()
{
	if( SRVC_CheckSheetConditionNormal(gCMPL_EJCT_CompileSheet) == false ) {
		gCMPL_EJCT_CompileSheet = SHEETNO_NUM;
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_IsExecutingModuleAction
// Description   : イジェクタの動作状態を取得
// Parameter     : ‐
// Return        : true：動作中、false：停止中
///////////////////////////////////////////////////////////////////
bool CMPL_EJCT_IsExecutingModuleAction()
{
	if( (CMPL_EJCT_BELT_IsExecutiongDeviceAction() == false) &&
	     (CMPL_EJCT_SCLMP_IsExecutiongDeviceAction() == false) ) {

		return false;
	}
	else {
		return true;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_ComponentControl
// Description   : コンポーネントコントロール(開始/停止)
// Parameter     : aChainNo (Chain No.)
//				 : aLinkNo (Link No.)
//				 : aAction (開始/停止 指示)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_ComponentControl(US aChainNo, US aLinkNo, EAction aAction)
{
	if( aAction == ACT_START ) {
		CMPL_EJCT_StartCompoEjcterMotor(aLinkNo);
	}
	else { // ACT_STOP
		CMPL_EJCT_StopCompoEjcterMotor(aLinkNo);
	}
}



///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_StartCompoEjcterMotor
// Description   : Eject Motor 動作開始(Compo Cont動作)
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_StartCompoEjcterMotor(US aLinkNo)
{

	ECMPL_EJCT_BELT_Action aAction;
	UL aChainLink = (UL)(CMPL_EJCT_COMPO_CHAIN) | ((UL)(aLinkNo) << 16);
	MSP_SetEventTimer(0, CMPL_EJCT_InformedCompoControlStart, aChainLink);

	aAction = (ECMPL_EJCT_BELT_Action)aLinkNo;
	switch ( aAction ) {
	case CMPL_EJCT_COMPO_FWD_EJECT :
		CMPL_EJCT_BELT_StartAct(CMPL_EJCT_COMPO_FWD_EJECT , 0);	// Link No = 20		
		CMPL_EJCT_SCLMP_StartAct(CMPL_EJCT_SCLMP_COMPO_CLAMP, 0);
		gCMPL_EJCT_EjectClampResultChk=0;
		break;
	case CMPL_EJCT_COMPO_REV_PADDLE :
		CMPL_EJCT_BELT_StartAct(CMPL_EJCT_COMPO_REV_PADDLE , 0);	// Link No = 21
		break;

	case CMPL_EJCT_COMPO_CLUTCH :  // Clutch
		CMPL_EJCT_SCLMP_StartAct(CMPL_EJCT_COMPO_CLUTCH, 0);			
		break;

	default:
		break;
	}

}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_StopCompoEjcterMotor
// Description   : Eject Motor動作停止(Compo Cont動作)
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_StopCompoEjcterMotor(US aLinkNo)
{
	CMPL_EJCT_BELT_DeviceStop();  // Eject Mot
	CMPL_EJCT_SCLMP_DeviceStop(); // SetClamp
	gCMPL_EJCT_EjectClampResultChk = 0;

	CMPL_EJCT_CompleteCompoEjcterMotor(aLinkNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_CompleteCompoEjcterMotor
// Description   : Eject Motor 動作完了(Compo Cont動作)
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_CompleteCompoEjcterMotor(US aLinkNo)
{
	UL aChainLink = (UL)(CMPL_EJCT_COMPO_CHAIN) | ((UL)(aLinkNo) << 16);

	MSP_SetEventTimer(0, CMPL_EJCT_InformedCompoControlComplete, aChainLink);
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_EjectorHome
// Description   : Ejecter ホーム動作
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_EjectorHome(UC aSheetNo)
{
	//V3.6.93
	if(( CMPL_EJCT_BELT_IsOccurFault() == false ) &&
		( CMPL_EJCT_SCLMP_IsOccurFault() == false ) ){

		if( aSheetNo != SHEETNO_NUM ) {
			if( (SRVC_CheckSheetConditionNormal(aSheetNo) == true) &&
				(CMPL_IsSheetInitExec(aSheetNo, CMPL_MDL_ACT_EJECTOR_HOME) == true) ) {
				
				CMPL_EJCT_StartEjectorHome(aSheetNo);
				// Sheet Initialize --> Stacker Job Start 
				CMPL_InformedModuleEvent(CMPL_EVT_STACKER_INIT_TRIGGER, aSheetNo);
			}
			else {
				CMPL_EJCT_CompleteEjectorHome(aSheetNo);
			} 
		}
		else {
			// NormalInitialize動作の開始条件 
			//1. EjectHomeSNR=OFF＆CompileTrayに用紙が無い＆SelfPrimingSensor=OFF
			//2. StapleReadySensorFail未発生 		//3. StapleNGクリア
			if( (gCMPL_EJCT_CompileSheet == SHEETNO_NUM) &&
			     (DD_DIO_GetInput(DD_DI_EJECT_HOME_SNR) == ON ) &&
			     (SRVC_CheckUnejectedSheetExist(eCdiFinOutDest_OutTray) == false) &&  // false = 未排出のシートがない
			    ( DD_DIO_GetInput(DD_DI_SELF_PRIMING_SNR) == ON ) && 
			    (SRVC_GetDeviceStatus(DSNO_STAPLE_READY_FAIL) ==FAULT_NOT_OCCUR) &&
			    (SRVC_GetDeviceStatus(DSNO_STAPLE_STATUS_NG) ==FAULT_NOT_OCCUR) &&
			   (SRVC_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_CLEAR) ) {
		
				CMPL_EJCT_StartEjectorHome(aSheetNo);
				// Sheet Initialize --> Stacker Job Start 
				CMPL_InformedModuleEvent(CMPL_EVT_STACKER_INIT_TRIGGER, aSheetNo); 
			}
			else {
				//Log Output
				PrintEjectLog_Short(0xECEC, 
								(US)((DD_DIO_GetInput(DD_DI_EJECT_HOME_SNR) <<8) | DD_DIO_GetInput(DD_DI_SELF_PRIMING_SNR)), 
							       (US) (SRVC_CheckUnejectedSheetExist(eCdiFinOutDest_OutTray))  );  //Error
				CMPL_EJCT_CompleteEjectorHome(aSheetNo);
			}
		}
	}
	else {
		PrintEjectLog_Short(0xECEB,  (US)CMPL_EJCT_BELT_IsOccurFault() , (US)CMPL_EJCT_SCLMP_IsOccurFault()  );  //Fail
		CMPL_EJCT_CompleteEjectorHome(aSheetNo);
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_StartEjectorHome
// Description   : Ejecter ホーム動作 開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_StartEjectorHome(UC aSheetNo)
{

	gCMPL_EJCT_EjectClampResultChk = 0; 

	CMPL_EJCT_BELT_StartAct(CMPL_EJCT_EJTHOME, aSheetNo);
	CMPL_EJCT_SCLMP_StartAct(CMPL_EJCT_SCLMP, aSheetNo); 

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_CompleteEjectorHome
// Description   : Ejecter ホーム動作 完了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_CompleteEjectorHome(UC aSheetNo)
{
	UL aActionData = (UL)(aSheetNo) | ((UL)(CMPL_MDL_ACT_EJECTOR_HOME) << 8);
	MSP_SetEventTimer(0, CMPL_EJCT_InformedModuleActionComplete, aActionData);
}

/*US CMPL_EJCT_EjectorHomeTime(UC aSheetNo, UC aLastShtNo)
{
	US aTime = 0;
	
	if( CMPL_EJCT_BELT_IsOccurFault() == false ) {
		if( (SRVC_CheckSheetConditionNormal(aSheetNo) == true) && 
			(SRVC_GetProcessStartSheet() == aSheetNo) &&
			(SRVC_GetSetTop(aSheetNo) == SHT_SET_TOP) ) {
			aTime = 1000;
		}
		else {
			aTime = 0;
		}
	}
	else {
		aTime = 0;																// Failが発生しているため
	}
	return aTime;
}*/

//CMPL_Managerから呼び出される ->	CMPL_EJCT_PaddleHome()
///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_PaddleCompile
// Description   : パドルHOME 動作
// Parameter     : aSheetNo(指定Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_PaddleHome(UC aSheetNo)
{

	if( CMPL_EJCT_BELT_IsOccurFault() == false ) {
		
		if( aSheetNo != SHEETNO_NUM ) {
			//V3.5.95
			if( (SRVC_CheckSheetConditionNormal(aSheetNo) == true) && 
				(CMPL_IsSheetInitExec(aSheetNo, CMPL_MDL_ACT_PADDLE_HOME) == true) ) {
				CMPL_EJCT_StartPaddleHome(aSheetNo);
			}
			else {
				CMPL_EJCT_CompletePaddleHome(aSheetNo);
			}
		
		}
		else {
			if( (gCMPL_EJCT_CompileSheet == SHEETNO_NUM) &&
				(SRVC_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_CLEAR) ) {
				CMPL_EJCT_StartPaddleHome(aSheetNo);
			}
			else {
				CMPL_EJCT_CompletePaddleHome(aSheetNo);
			}
		}
	}
	else {
		CMPL_EJCT_CompletePaddleHome(aSheetNo);
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_StartPaddleCompile
// Description   : パドルHOME 動作 開始
// Parameter     : aSheetNo(指定Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_StartPaddleHome(UC aSheetNo)
{
	CMPL_EJCT_BELT_StartAct(CMPL_EJCT_PADHOME, aSheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_CompletePaddleHome
// Description   :: パドルHOME 動作 完了
// Parameter     : aSheetNo(指定Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_CompletePaddleHome(UC aSheetNo)
{
	UL aActionData = (UL)(aSheetNo) | ((UL)(CMPL_MDL_ACT_PADDLE_HOME) << 8);
	MSP_SetEventTimer(0, CMPL_EJCT_InformedModuleActionComplete, aActionData);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_GetSubPaddleActionCount :  (V3.0.0)
// Description   : SubPaddle 動作  回数 取得
// Parameter     : aSheetNo(対象シート)
// Return        :paddleCnt 
///////////////////////////////////////////////////////////////////
UC  CMPL_EJCT_GetSubPaddleActionCount(UC aSheetNo)
{

	UC	paddleCnt = 1;
	US 	length = SRVC_GetProcessSize(aSheetNo);
	US   width = SRVC_GetCrossProcessSize(aSheetNo);
	US 	aMediaType = SRVC_GetMediaType(aSheetNo);
	US 	aMediaWeight =  SRVC_GetMediaWeight(aSheetNo);
	UC   aCompileCount = SRVC_GetCompileCount(aSheetNo);
	UC   aN_Count;
	UC   aP2;   //CR-014
	
	if(length <= 2970) {
	
		// MediaType＝コート紙　
		if ( aMediaType == eCdiFinMediaType_Coated ) {

			paddleCnt = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_COUNT5);
			aN_Count = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_CHANGE_COUNT_N_3);  
			
		} else if ((aMediaType ==eCdiFinMediaType_Plain) && (aMediaWeight >= 106))	{

		//　　MediaType=普通紙＆厚紙(106～221)
			paddleCnt = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_COUNT3);
			aN_Count = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_CHANGE_COUNT_N);  
		
		} else { //それ以外
		
			paddleCnt = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_COUNT1);
			 //(CR-XXX) 150716 B5 シートずれ対策
			 if ( width < 2000 ) {   // 200mm 未満
				aN_Count = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_CHANGE_COUNT_N_2_B5);  
			 } else {  // 200mm 以上
				aN_Count = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_CHANGE_COUNT_N_2); 
			 }
		}
		
	} else { //  >2970	
	
		// MediaType＝コート紙　
		if ( aMediaType == eCdiFinMediaType_Coated ) {

			paddleCnt = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_COUNT6);
			aN_Count = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_CHANGE_COUNT_N_3);  
			
		} else if ((aMediaType ==eCdiFinMediaType_Plain) && (aMediaWeight >= 106))	{

		//　　MediaType=普通紙＆厚紙(106～221)
			paddleCnt = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_COUNT4);
			aN_Count = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_CHANGE_COUNT_N); 
		
		} else { //それ以外
		
			paddleCnt = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_COUNT2);
			 //(CR-XXX) 150716 B5 シートずれ対策
			 if ( width < 2000 ) {   // 200mm 未満
				aN_Count = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_CHANGE_COUNT_N_2_B5);  
			 } else {  // 200mm 以上
				aN_Count = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_CHANGE_COUNT_N_2); 
			 }
		}
	}

	//セット枚数がN枚以上
	if ( aCompileCount >= aN_Count) {

		 if ((aMediaType ==eCdiFinMediaType_Plain) && (aMediaWeight >= 106))	{
			//(CR-014) //　　MediaType=普通紙＆厚紙(106～221) --> P-P2回
			aP2 = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_CHANGE_COUNT_P2);

			if ( paddleCnt <= aP2 ) {
				paddleCnt = 1;
			} else {
				paddleCnt = paddleCnt - aP2;
			}
		
		 } else { //それ以外 --> P-1回
			 if ( paddleCnt < 2 ) {
				paddleCnt = 1;
			 } else {
				paddleCnt = paddleCnt - 1;
			 }
		 }
	}	
	return  paddleCnt;	
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_PaddleCompile
// Description   : パドルコンパイル動作
// Parameter     : aSheetNo(指定Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_PaddleCompile(UC aSheetNo)
{
	//V3.6.93
	if( (SRVC_CheckSheetConditionNormal(aSheetNo) == true ) &&
		(CMPL_EJCT_BELT_IsOccurFault() == false)){

		if( aSheetNo != gCMPL_EJCT_CompileSheet ) {
			gCMPL_EJCT_CompileSheet = aSheetNo;
			CMPL_EJCT_StartPaddleCompile(aSheetNo);
		}
		else {
			CMPL_EJCT_CompletePaddleCompile(aSheetNo);
		}
	}
	else {
		CMPL_EJCT_CompletePaddleCompile(aSheetNo);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_StartPaddleCompile
// Description   : パドルコンパイル動作 開始
// Parameter     : aSheetNo(指定Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_StartPaddleCompile(UC aSheetNo)
{
	CMPL_EJCT_BELT_StartAct(CMPL_EJCT_COMPILE, aSheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_CompletePaddleCompile
// Description   : パドルコンパイル動作 完了
// Parameter     : aSheetNo(指定Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_CompletePaddleCompile(UC aSheetNo)
{
	UL aActionData = (UL)(aSheetNo) | ((UL)(CMPL_MDL_ACT_PADDLE_COMPILE) << 8);
	MSP_SetEventTimer(0, CMPL_EJCT_InformedModuleActionComplete, aActionData);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_Eject
// Description   : イジェクト動作
// Parameter     : aSheetNo(指定Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_Eject(UC aSheetNo)
{
	gCMPL_EJCT_CompileSheet = SHEETNO_NUM;

	 //V3.6.93 
	if(( CMPL_EJCT_BELT_IsOccurFault() == false ) &&
	    ( CMPL_EJCT_SCLMP_IsOccurFault() == false )){

		if( SRVC_CheckSheetConditionNormal(aSheetNo) == true ) {
			CMPL_EJCT_StartEject(aSheetNo);
		}
		else {
			CMPL_EJCT_CompleteEject(aSheetNo);
		}
	}
	else {
		CMPL_EJCT_CompleteEject(aSheetNo);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_StartEject
// Description   : イジェクト動作
// Parameter     : aSheetNo(指定Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_StartEject(UC aSheetNo)
{
	gCMPL_EJCT_EjectClampResultChk = 0;
	CMPL_EJCT_BELT_StartAct(CMPL_EJCT_EJECT, aSheetNo);
	CMPL_EJCT_SCLMP_StartAct(CMPL_EJCT_SCLMP, aSheetNo);
	
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_CompleteEject
// Description   : イジェクト動作
// Parameter     : aSheetNo(指定Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_CompleteEject(UC aSheetNo)
{
	UL aActionData = (UL)(aSheetNo) | ((UL)(CMPL_MDL_ACT_EJECT) << 8);
	MSP_SetEventTimer(0, CMPL_EJCT_InformedModuleActionComplete, aActionData);
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_InformedCompoControlStart
// Description   : Compiler ModuleにComponent Control Action開始を通知する
// Parameter     : aChainLink(0-15bit:ChainNo、16-31bit:LinkNo)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_InformedCompoControlStart(SS aTimerID, UL aChainLink)
{	
	US aChainNo = (US)(aChainLink & 0x0000FFFF);
	US aLinkNo = (US)((aChainLink >> 16) & 0x0000FFFF);
	CMPL_InformedCompoControlComplete(aChainNo, aLinkNo, ACT_START);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_InformedCompoControlComplete
// Description   : Compiler ModuleにComponent Control Action終了を通知する
// Parameter     : aChainLink(0-15bit:ChainNo、16-31bit:LinkNo)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_InformedCompoControlComplete(SS aTimerID, UL aChainLink)
{
	US aChainNo = (US)(aChainLink & 0x0000FFFF);
	US aLinkNo = (US)((aChainLink >> 16) & 0x0000FFFF);
	CMPL_InformedCompoControlComplete(aChainNo, aLinkNo, ACT_STOP);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_InformedModuleActionComplete
// Description   : Compiler ModuleにModule Action終了を通知する
// Parameter     : aActionData(0-7bit:SheetNo、8-15bit:指定動作)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_InformedModuleActionComplete(SS aTimerID, UL aActionData)
{
	UC aSheetNo = (UC)(aActionData & 0x000000FF);
	UC aAction = (UC)((aActionData >> 8) & 0x000000FF);

	CMPL_InformedModuleActionComp((ECMPL_ModuleAction)(aAction), aSheetNo);
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_InformedEjectMotorComp
// Description   : Deviceからイジェクトモーター動作(Compocont)完了を受信
// Parameter     : aAction , aSheetNo
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_InformedEjectMotorComp(US aAction, UC aSheetNo)
{
	switch ( aAction ) {
		
	case CMPL_EJCT_COMPO_FWD_EJECT : 	// Eject 
	case CMPL_EJCT_SCLMP_COMPO_CLAMP : 		// SetClamp 
		gCMPL_EJCT_EjectClampResultChk  |= aAction;
		// Eject動作とSetClamp動作がすべて終わったのか確認
		if ( gCMPL_EJCT_EjectClampResultChk == (CMPL_EJCT_COMPO_FWD_EJECT | CMPL_EJCT_SCLMP_COMPO_CLAMP) ) {
			CMPL_EJCT_CompleteCompoEjcterMotor(CMPL_EJCT_COMPO_FWD_EJECT);
			gCMPL_EJCT_EjectClampResultChk = 0;
		}
		break;

	case CMPL_EJCT_COMPO_REV_PADDLE : //Compile
		CMPL_EJCT_CompleteCompoEjcterMotor(CMPL_EJCT_COMPO_REV_PADDLE);
		break;		

	case CMPL_EJCT_PADHOME: //Paddle Home 
		CMPL_EJCT_CompletePaddleHome(aSheetNo);
		break;

	case CMPL_EJCT_EJTHOME:  //Eject Home
		gCMPL_EJCT_EjectClampResultChk  |= aAction;
		// Eject動作とSetClamp動作がすべて終わったのか確認
		if(gCMPL_EJCT_EjectClampResultChk == (CMPL_EJCT_EJTHOME | CMPL_EJCT_SCLMP))
		{	
			CMPL_InformedModuleEvent(CMPL_EVT_STACKER_INIT_WAITING_POS_TRIGGER, aSheetNo);
			CMPL_EJCT_CompleteEjectorHome(aSheetNo);
			gCMPL_EJCT_EjectClampResultChk = 0;
		}	
		break;
	case CMPL_EJCT_COMPILE: //Compile
		CMPL_EJCT_CompletePaddleCompile(aSheetNo);
		break;
	case CMPL_EJCT_EJECT: //Eject 
		gCMPL_EJCT_EjectClampResultChk  |= aAction;
		// Eject動作とSetClamp動作がすべて終わったのか確認
		if(gCMPL_EJCT_EjectClampResultChk == (CMPL_EJCT_EJECT | CMPL_EJCT_SCLMP))
		{
			CMPL_InformedModuleEvent(CMPL_EVT_STACKER_JOB_TRIGGER1, aSheetNo);
			CMPL_EJCT_CompleteEject(aSheetNo);
			gCMPL_EJCT_EjectClampResultChk = 0;
		}
		break;
		
	case CMPL_EJCT_SCLMP : //SetClamp 
		gCMPL_EJCT_EjectClampResultChk  |= aAction;

		 //Eject Home 動作 + SetClamp 
		if(gCMPL_EJCT_EjectClampResultChk == (CMPL_EJCT_EJTHOME | CMPL_EJCT_SCLMP))
		{	
			CMPL_InformedModuleEvent(CMPL_EVT_STACKER_INIT_WAITING_POS_TRIGGER, aSheetNo);
			CMPL_EJCT_CompleteEjectorHome(aSheetNo);
			gCMPL_EJCT_EjectClampResultChk = 0;
		}
		 //Eject 動作 + SetClamp 
		else if(gCMPL_EJCT_EjectClampResultChk == (CMPL_EJCT_EJECT | CMPL_EJCT_SCLMP))
		{
			CMPL_InformedModuleEvent(CMPL_EVT_STACKER_JOB_TRIGGER1, aSheetNo);
			CMPL_EJCT_CompleteEject(aSheetNo);
			gCMPL_EJCT_EjectClampResultChk = 0;
		}
		break;
		
	default:
		/* Irregular Case */
		break;
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_InformedEjectStart
// Description   : 排出動作開始を通知
// Parameter     : aSheetNo (対象シートナンバー)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_InformedEjectStart(UC aSheetNo)
{
	CMPL_InformedModuleEvent(CMPL_EVT_EJECT_START, aSheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_InformedEjectComp
// Description   : 排出動作完了を通知
// Parameter     : aSheetNo (対象シートナンバー)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_InformedEjectComp(UC aSheetNo)
{
	CMPL_InformedModuleEvent(CMPL_EVT_EJECT_COMP, aSheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_InformedEjectComp
// Description   : 用紙排出完了を通知
// Parameter     : aSheetNo (対象シートナンバー)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_InformedEjectSheetThrough(UC aSheetNo)
{
	CMPL_InformedModuleEvent(CMPL_EVT_EJECT_SHEET_THROUGH, aSheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_InformedEjectHomeSensorOnFail
// Description   : Ejector Home Sensor ON Failを通知
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_InformedEjectHomeSensorOnFail()
{
	CMPL_EJCT_BELT_DeviceStop();
	CMPL_EJCT_SCLMP_DeviceStop();
	CMPL_InformedDeviceStatusChange(DSNO_EJECT_BELT_HOME_ON_FAIL, FAULT_OCCUR);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_InformedEjectHomeSensorOffFail
// Description   : Ejector Home Sensor OFF Failを通知
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_InformedEjectHomeSensorOffFail()
{
	CMPL_EJCT_BELT_DeviceStop();
	CMPL_EJCT_SCLMP_DeviceStop();
	CMPL_InformedDeviceStatusChange(DSNO_EJECT_BELT_HOME_OFF_FAIL, FAULT_OCCUR);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_InformedPaddleHomeSensorOnFail
// Description   : SubPaddle Home Sensor ON Failを通知
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_InformedPaddleHomeSensorOnFail()
{

	CMPL_EJCT_BELT_DeviceStop();

	CMPL_InformedDeviceStatusChange(DSNO_PADDLE_HOME_ON_FAIL, FAULT_OCCUR);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_InformedPaddleHomeSensorOffFail
// Description   : SubPaddle Home Sensor OFF Failを通知
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_InformedPaddleHomeSensorOffFail()
{

	CMPL_EJCT_BELT_DeviceStop();

	CMPL_InformedDeviceStatusChange(DSNO_PADDLE_HOME_OFF_FAIL, FAULT_OCCUR);
}



///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_InformedSetClampHomeSensorOnFail
// Description   : SetClamp Home Sensor ON Failを通知
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_InformedSetClampHomeSensorOnFail()
{
	CMPL_EJCT_SCLMP_DeviceStop();
	CMPL_EJCT_BELT_DeviceStop();

	CMPL_InformedDeviceStatusChange(DSNO_SETCLAMP_HOME_ON_FAIL, FAULT_OCCUR);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_InformedSetClampHomeSensorOffFail
// Description   : SetClamp Home Sensor OFF Failを通知
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_InformedSetClampHomeSensorOffFail()
{
	CMPL_EJCT_SCLMP_DeviceStop();
	CMPL_EJCT_BELT_DeviceStop();

	CMPL_InformedDeviceStatusChange(DSNO_SETCLAMP_HOME_OFF_FAIL, FAULT_OCCUR);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_InformedSetEjectJam
// Description   : Set Eject Jamを通知
// Parameter     : aSheetNo(Jam対象紙のSheetNo)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_InformedSetEjectJam(UC aSheetNo)
{
	CMPL_InformedModuleEvent(CMPL_EVT_SET_EJECT_JAM_OCCUR, aSheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : PrintEjectLog_Short
// Description   :  Pulse
// Parameter     : para1 (数字列の最初に付加する値):Compile/ Eject /SetClamp 状態情報
//               : para2 (数字列の２番目に付加する値) : Sheet No
//               : para3 (数字列の３番目に付加する値) : 
// Return        : ‐
///////////////////////////////////////////////////////////////////
void PrintEjectLog_Short(US para1, US para2, US para3)
{

	UC 	log[6];

	log[0] =  (UC)(para1 >>8 );
	log[1] =  (UC)(para1 & 0x00FF);
	log[2] =  (UC)(para2 >>8 );	
	log[3] =  (UC)(para2 & 0x00FF);

	log[4] =  (UC)(para3 >>8 );
	log[5] =  (UC)(para3 & 0x00FF);

	
//#define	LOGID_EJCT=10, 'S'=Send,   LOG_CATEGORY_DEBUG=2
	DD_LOG_Put(LOGID_EJCT, 'S', log, sizeof(log), LOG_CATEGORY_DEBUG);

}

