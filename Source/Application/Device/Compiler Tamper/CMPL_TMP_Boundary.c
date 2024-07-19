// File Name:	CMPL_TMP_Boundary.c
// Description:	Compiler Tamper Module Boundary
// Engineer:	Takahiko Minaga
// Last Edit:	14.09.23
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"
#include "ECMPL_TMP_Action.h"
#include "ECMPL_TMP_State.h"
#include "CMPL_TMP_Boundary.h"

#include "CMPL_TMP_COMM_Device.h"
#include "CMPL_TMP_LEFT_Device.h"
#include "CMPL_TMP_RIGHT_Device.h"
#include "CMPL_Manager.h"
#include "NvmService.h"
//#include "CMPL_EJCT_Boundary.h"
/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////
ECMPL_TMP_State gCMPL_TMP_State;
UC gCMPL_TMP_ActionState;

#define CMPL_TMP_LEFT_ACTION_COMPLETE					0x01
#define CMPL_TMP_RIGHT_ACTION_COMPLETE					0x02
#define CMPL_TMP_EJECT_THROUGH							0x04
#define CMPL_TMP_EJECT_START 							0x08

#define CMPL_TMP_COMPO_CHAIN							12


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
static void CMPL_TMP_ChangeStandbyEvent();
static void CMPL_TMP_ChangeReadyEvent();
static void CMPL_TMP_ChangeDiagEvent();

static void CMPL_TMP_StartCompo_CWCCW_LowMidHi(US aLinkNo);
static void CMPL_TMP_StopCompo_CWCCW_LowMidHi(US aLinkNo);

static void CMPL_TMP_StartSizePositioning(UC aSheetNo);
static void CMPL_TMP_CompleteSizePositioning(UC aSheetNo);

static void CMPL_TMP_StartNormalTamping(UC aSheetNo);
static void CMPL_TMP_CompleteNormalTamping(UC aSheetNo);

static void CMPL_TMP_StartEjectTamping(UC aSheetNo);
static void CMPL_TMP_CompleteEjectTamping(UC aSheetNo);

static void CMPL_TMP_StartMissRegi(UC aSheetNo);
static void CMPL_TMP_CompleteMissRegi(UC aSheetNo);

static void CMPL_TMP_StartOffset(UC aSheetNo);
static void CMPL_TMP_CompleteOffset(UC aSheetNo);

static void CMPL_TMP_StartReturn(UC aSheetNo);
static void CMPL_TMP_CompleteReturn(UC aSheetNo);

static void CMPL_TMP_StartOpen(UC aSheetNo);
static void CMPL_TMP_CompleteOpen(UC aSheetNo);

static void CMPL_TMP_InformedCompoControlStart(SS aTimerID, UL aChainLink);
static void CMPL_TMP_InformedCompoControlComplete(SS aTimerID, UL aChainLink);
static void CMPL_TMP_InformedModuleActionComplete(SS aTimerID, UL aActionData);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_Reset
// Description   : タンパーリセット
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_Reset()
{
	gCMPL_TMP_State = CMPL_TMP_STT_UNKNOWN;
	CMPL_TMP_COMM_SetProfile();
	CMPL_TMP_RIGHT_Reset();
	CMPL_TMP_LEFT_Reset();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_SetUp
// Description   : タンパーセットアップ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_SetUp()
{
	/* No Action */
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_PowerOff
// Description   : タンパーパワーオフ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_PowerOff()
{
	CMPL_TMP_StopModuleAction();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_Normal
// Description   : タンパーノーマル
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_Normal()
{
	CMPL_TMP_ChangeStandbyEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_Diag
// Description   : タンパーダイアグ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_Diag()
{
	CMPL_TMP_ChangeDiagEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_ProcStart
// Description   : プロセススタート
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_ProcStart()
{
	CMPL_TMP_ChangeReadyEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_StopSteadyFinishing
// Description   : タンパーフィニッシングプロセス終了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_StopSteadyFinishing()
{
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, OFF);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_ENB, OFF);

	CMPL_TMP_ChangeStandbyEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_StandbyEjectStartNotify
// Description   : スタンバイイジェクト開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_StandbyEjectStartNotify()
{
	// DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, ON);
	// DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_ENB, ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_StandbyEjectCompNotify
// Description   : スタンバイイジェクトComplete
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_StandbyEjectCompNotify()
{
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, OFF);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_ENB, OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_ChangeStandbyEvent
// Description   : スタンバイ遷移イベント
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_ChangeStandbyEvent()
{
	if( (gCMPL_TMP_State == CMPL_TMP_STT_UNKNOWN) ||
		(gCMPL_TMP_State == CMPL_TMP_STT_DIAG) ||
		(gCMPL_TMP_State == CMPL_TMP_STT_READY) ) {
		gCMPL_TMP_State = CMPL_TMP_STT_STANDBY;
	} else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_ChangeReadyEvent
// Description   : レディー遷移イベント
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_ChangeReadyEvent()
{
	if( gCMPL_TMP_State == CMPL_TMP_STT_STANDBY ) {
		gCMPL_TMP_State = CMPL_TMP_STT_READY;
	} else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_ChangeDiagEvent
// Description   : ダイアグ遷移イベント
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_ChangeDiagEvent()
{
	if( (gCMPL_TMP_State == CMPL_TMP_STT_UNKNOWN) ||
		(gCMPL_TMP_State == CMPL_TMP_STT_STANDBY) ) {
		gCMPL_TMP_State = CMPL_TMP_STT_DIAG;
	} 	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_AnyCoverOpen
// Description   : いずれかのカバーが開いた
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_AnyCoverOpen()
{
	if( gCMPL_TMP_State != CMPL_TMP_STT_DIAG ) {
		CMPL_TMP_StopModuleAction();
	} else {																		// = FIN_STT_DIAGNOSTICS
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_AllCoverClose
// Description   : すべてのカバーが閉まった
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_AllCoverClose()
{
	/* No Action */
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_StopModuleAction
// Description   : タンパー動作停止
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_StopModuleAction()
{
	CMPL_TMP_LEFT_DeviceStop();
	CMPL_TMP_RIGHT_DeviceStop();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_StopSheet
// Description   : 指定シートの動作停止
// Parameter     : aSheetNo (指定シート番号)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_StopSheet(UC aSheetNo)
{
	UC aLeftSheetNo = CMPL_TMP_LEFT_GetControlSheetNo();
	UC aRightSheetNo = CMPL_TMP_RIGHT_GetControlSheetNo();

	if( aSheetNo == SHEETNO_NUM) {												// 制御中の用紙のコンディションをチェック
		if( aLeftSheetNo != SHEETNO_NUM ) {
			if( SRVC_CheckSheetConditionNormal(aLeftSheetNo) == false ) {
				CMPL_TMP_RIGHT_DeviceStop();
			} else {																// = Condition Normal
				/* No Action */
			}
		} else {																	// シートに対する制御をしていない
			/* No Action */
		}

		if( aRightSheetNo != SHEETNO_NUM ) {
			if( SRVC_CheckSheetConditionNormal(aRightSheetNo) == false ) {
				CMPL_TMP_LEFT_DeviceStop();
			} else {																// = Condition Normal
				/* No Action */
			}
		} else {																	// シートに対する制御をしていない
			/* No Action */
		}

	} else {																		// 指定シートの制御を停止する
		if( aSheetNo == aLeftSheetNo ) {
			CMPL_TMP_RIGHT_DeviceStop();
		} else {																	// 指定シートと制御シートが異なる
			/* No Action */
		}

		if( aSheetNo == aRightSheetNo ) {
			CMPL_TMP_LEFT_DeviceStop();
		} else {																	// 指定シートと制御シートが異なる
			/* No Action */
		}

	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_IsExecutingModuleAction
// Description   : イジェクタの動作状態を取得
// Parameter     : ‐
// Return        : true：動作中、false：停止中
///////////////////////////////////////////////////////////////////
bool CMPL_TMP_IsExecutingModuleAction()
{
	if( (CMPL_TMP_RIGHT_IsExecutiongDeviceAction() == false) &&
		(CMPL_TMP_LEFT_IsExecutiongDeviceAction() == false) ) {
		return false;
	} else {
		return true;
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_IsEjectSheetThrough
// Description :  EjectMot減速開始通知の受信Check
// Parameter     :
//
// Return        :  true=受信
///////////////////////////////////////////////////////////////////
bool CMPL_TMP_IsEjectSheetThrough()
{
	if( (gCMPL_TMP_ActionState & CMPL_TMP_EJECT_THROUGH) != 0 ) {
		return true;
	} else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_EjectSheetThrough
// Description :  EjectMot減速開始通知
// Parameter     : aSheetNo
//
// Return        :
///////////////////////////////////////////////////////////////////
void CMPL_TMP_EjectSheetThrough(UC aSheetNo)
{
	gCMPL_TMP_ActionState |= CMPL_TMP_EJECT_THROUGH;

	CMPL_TMP_LEFT_InformedEjectSheetThrough();
	CMPL_TMP_RIGHT_InformedEjectSheetThrough();
}

bool CMPL_TMP_IsCanTampingSheetSize(UC aSheetNo)
{
	US aLength = SRVC_GetProcessSize(aSheetNo);
	US aWidth = SRVC_GetCrossProcessSize(aSheetNo);

	if( (aLength >= 1760) && (aLength <= 3560) && (aWidth >= 1760) && (aWidth <= 2160) ) {
		return true;
	} else {
		return false;
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_IsEjectStart
// Description :  Eject 動作開始通知の受信Check
// Parameter     :
//
// Return        :  true=受信
///////////////////////////////////////////////////////////////////
bool CMPL_TMP_IsEjectStart()
{
	if( (gCMPL_TMP_ActionState & CMPL_TMP_EJECT_START) != 0 ) {
		return true;
	} else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_EjectStart
// Description   : Eject 動作開始通知の受信
// Parameter     : aSheetNo
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_EjectStart(UC aSheetNo)
{
	gCMPL_TMP_ActionState |= CMPL_TMP_EJECT_START;

	CMPL_TMP_LEFT_InformedEjectStart();
	CMPL_TMP_RIGHT_InformedEjectStart();
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_ComponentControl
// Description   : コンポーネントコントロール(開始/停止)
// Parameter     : aChainNo (Chain No.)
//				 : aLinkNo (Link No.)
//				 : aAction (開始/停止 指示)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_ComponentControl(US aChainNo, US aLinkNo, EAction aAction)
{

	if( aAction == ACT_START ) {
		CMPL_TMP_StartCompo_CWCCW_LowMidHi(aLinkNo);
	}
	else { // ACT_STOP
		CMPL_TMP_StopCompo_CWCCW_LowMidHi(aLinkNo);
	}

}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_StartCompo_CWCCW_LowMidHi
// Description   :RightTamperを内外側方向に移動開始(Compo Cont)
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_StartCompo_CWCCW_LowMidHi(US aLinkNo)
{
	ECMPL_TMP_Action aAction;

	UL aChainLink = (UL)(CMPL_TMP_COMPO_CHAIN) | ((UL)(aLinkNo) << 16);
	MSP_SetEventTimer(0, CMPL_TMP_InformedCompoControlStart, aChainLink);

	aAction = (ECMPL_TMP_Action)aLinkNo;
	switch( aAction ) {

		case CMPL_TMP_COMPO_RIGHT_CW_LOW :
		case CMPL_TMP_COMPO_RIGHT_CW_MID :
		case CMPL_TMP_COMPO_RIGHT_CW_HI :
		case CMPL_TMP_COMPO_RIGHT_CCW_LOW :
		case CMPL_TMP_COMPO_RIGHT_CCW_MID :
		case CMPL_TMP_COMPO_RIGHT_CCW_HI :
		case CMPL_TMP_COMPO_RIGHT_SENSOR :			// For Test
			CMPL_TMP_RIGHT_StartAct(aAction, 0);
			break;

		case CMPL_TMP_COMPO_LEFT_CW_LOW :
		case CMPL_TMP_COMPO_LEFT_CW_MID :
		case CMPL_TMP_COMPO_LEFT_CW_HI :
		case CMPL_TMP_COMPO_LEFT_CCW_LOW :
		case CMPL_TMP_COMPO_LEFT_CCW_MID :
		case CMPL_TMP_COMPO_LEFT_CCW_HI :
		case CMPL_TMP_COMPO_LEFT_SENSOR :
			CMPL_TMP_LEFT_StartAct(aAction, 0);
			break;

		default:
			break;

	}
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_StopCompo_CWCCW_LowMidHi
// Description   : Tamperを停止(Compo Cont)
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void  CMPL_TMP_StopCompo_CWCCW_LowMidHi(US aLinkNo)
{
	UL aChainLink = (UL)(CMPL_TMP_COMPO_CHAIN) | ((UL)(aLinkNo) << 16);
	CMPL_TMP_RIGHT_DeviceStop();
	MSP_SetEventTimer(0, CMPL_TMP_InformedCompoControlComplete, aChainLink);

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_SizePositioning
// Description   : サイズポジショニング動作
// Parameter     : aSheetNo(対象シート)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_SizePositioning(UC aSheetNo)
{
	if( (CMPL_TMP_LEFT_IsOccurFault() == false) &&
		(CMPL_TMP_RIGHT_IsOccurFault() == false) ) {

	// V3.5.93
	//	CMPL_TMP_StartSizePositioning(aSheetNo);	
		if( (SRVC_CheckSheetConditionNormal(aSheetNo) == true) &&
			(CMPL_IsSheetInitExec(aSheetNo, CMPL_MDL_ACT_TAMP_SIZE_POS) == true) ) {

			CMPL_TMP_StartSizePositioning(aSheetNo);
		} else {
			CMPL_TMP_CompleteSizePositioning(aSheetNo);
		}
	// SheetInitExec( ) Check ↑↑
	} else {
		CMPL_TMP_CompleteSizePositioning(aSheetNo);
	}
}

void CMPL_TMP_StartSizePositioning(UC aSheetNo)
{
	gCMPL_TMP_ActionState = 0;

	CMPL_TMP_LEFT_StartAct(CMPL_TMP_SIZE_POS, aSheetNo);
	CMPL_TMP_RIGHT_StartAct(CMPL_TMP_SIZE_POS, aSheetNo);
}

void CMPL_TMP_CompleteSizePositioning(UC aSheetNo)
{
	UL aActionData = (UL)(aSheetNo) | ((UL)(CMPL_MDL_ACT_TAMP_SIZE_POS) << 8);

	MSP_SetEventTimer(0, CMPL_TMP_InformedModuleActionComplete, aActionData);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_GetSizePositioningTime
// Description   : SizePositioning 動作時間
// Parameter     : aSheetNo(対象シート) , aLastShtNo(no use..)
// Return        :  aTime( 動作時間)
///////////////////////////////////////////////////////////////////
US CMPL_TMP_GetSizePositioningTime(UC aSheetNo, UC aLastShtNo)
{
	US aTime = 0;  //return value ..
	SL aHomeSL = 0;  
	SL aSizeSL = 0; 

	SS aLeftHome = CMPL_TMP_LEFT_GetTamperPosition(aLastShtNo);
	SS aRightHome = 	CMPL_TMP_RIGHT_GetTamperPosition(aLastShtNo);
	SS aLeftSize = (SS)CMPL_TMP_LEFT_GetTamperPosition(aSheetNo);
	SS aRightSize = (SS)CMPL_TMP_RIGHT_GetTamperPosition(aSheetNo);

	//ホーム動作時間　(Left)
	if (aLeftHome <= 62) { //【前セットのサイズ動作での移動量Aが62パルス以下】
		//V3.3.94 騒音対策		
		aLeftHome = 8380;  // 838 *10   //aLeftHome = 4775;  // 477.5 *10
	} else {
		//aLeftHome = (SS)((aLeftHome -60) * 9.091 + 1328); //((A-60)×0.9091+132.8) * 10
		aLeftHome = (SS)((aLeftHome -60) * 9.091 + 3128);  //((A-60)×0.9091+312.8) * 10
	}

	//ホーム動作時間　(Right)
	if (aRightHome <= 62) { //【前セットのサイズ動作での移動量Aが62パルス以下】
		//V3.3.94 騒音対策	
		aRightHome = 8380;	// 838 *10  //	aRightHome = 4775;  // 477.5 *10
	} else {
		//aRightHome = (SS)((aRightHome -60) * 9.091 + 1328); //((A-60)×0.9091+132.8) * 10
		aRightHome = (SS)((aRightHome -60) * 9.091 + 3128); //((A-60)×0.9091+312.8) * 10
	}
	//大きい方にする。(小数点以下切り上げ)
	if ( aLeftHome >= aRightHome) {
		aHomeSL = (SL)((aLeftHome + 9)/ 10);
	} else {
		aHomeSL = (SL)((aRightHome + 9) / 10);
	}
	
	//サイズ動作時間 : (A-60)×0.9091+102.8(小数点以下切り上げ)
	if ( aLeftSize >= aRightSize ) {
		aLeftSize = (SS)((aLeftSize - 60) * 9.091);
		//V3.3.94 騒音対策
		//aSizeSL = (SL)(((aLeftSize + 1028) + 9) / 10);
		aSizeSL = (SL)(((aLeftSize + 2828) + 9) / 10);
		
	} else {
		aRightSize = (SS)((aRightSize - 60) * 9.091);
		//V3.3.94 騒音対策
		//aSizeSL = (SL)(((aRightSize + 1028) + 9) / 10);
		aSizeSL = (SL)(((aRightSize + 2828) + 9) / 10);
	}

	// ホーム動作時間　+ サイズ動作時間
	aTime = (US) (aHomeSL + aSizeSL);
	
	return aTime;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_NormalTamping
// Description   : ノーマルタンピング動作
// Parameter     : aSheetNo(対象シート)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_NormalTamping(UC aSheetNo)
{
	if( (CMPL_TMP_LEFT_IsOccurFault() == false) &&
		(CMPL_TMP_RIGHT_IsOccurFault() == false) ) {

		if( (SRVC_CheckSheetConditionNormal(aSheetNo) == true) &&
			(CMPL_TMP_IsCanTampingSheetSize(aSheetNo) == true) &&
			(SRVC_GetCrossProcessSizeMixSet(aSheetNo) == SHT_SIZE_UNMIX) ) {

			CMPL_TMP_StartNormalTamping(aSheetNo);
		} else {
			CMPL_TMP_CompleteNormalTamping(aSheetNo);
		}
	} else {
		CMPL_TMP_CompleteNormalTamping(aSheetNo);
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_StartNormalTamping
// Description   : ノーマルタンピング動作開始
// Parameter     : aSheetNo(対象シート)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_StartNormalTamping(UC aSheetNo)
{
	gCMPL_TMP_ActionState = 0;

// CR008 - Normal Tamping開始TimingのTimeout以降に状態を持たせるため、変更
	CMPL_TMP_LEFT_PreStartAct(CMPL_TMP_PRE_NORMAL_TAMPING, aSheetNo); //

	CMPL_TMP_RIGHT_StartAct(CMPL_TMP_NORMAL_TAMPING, aSheetNo); 

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_CompleteNormalTamping
// Description   : ノーマルタンピング動作完了
// Parameter     : aSheetNo(対象シート)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_CompleteNormalTamping(UC aSheetNo)
{
	UL aActionData = (UL)(aSheetNo) | ((UL)(CMPL_MDL_ACT_NORMAL_TAMPING) << 8);

	MSP_SetEventTimer(0, CMPL_TMP_InformedModuleActionComplete, aActionData);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_GetNormalTampingActionTime
// Description   : ノーマルタンピング動作時間取得
// Parameter     : aSheetNo(対象シート)
// Return        : aTime(動作時間)
///////////////////////////////////////////////////////////////////
US CMPL_TMP_GetNormalTampingActionTime(UC aSheetNo)
{
	US aTotalTime = CMPL_TMP_LEFT_GetNormalTampingTime(aSheetNo);
			
	return aTotalTime;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_GetEjectTampingActionTime
// Description   : Ejectタンピング動作時間取得
// Parameter     : aSheetNo(対象シート)
// Return        : aTime(動作時間)
///////////////////////////////////////////////////////////////////
US CMPL_TMP_GetEjectTampingActionTime(UC aSheetNo)
{
	US aTotalTime = CMPL_TMP_LEFT_GetEjectTampingTime(aSheetNo);
		
	return aTotalTime;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_GetNormalTampingDelayTime
// Description   : ノーマルタンピング Start動作時間取得
// Parameter     : aSheetNo(対象シート)
// Return        : aTime(動作時間)
///////////////////////////////////////////////////////////////////
US CMPL_TMP_GetNormalTampingDelayTime(UC aSheetNo)
{
	US  aTime=0;
	US aLength = SRVC_GetProcessSize(aSheetNo);

	US aMediaType = SRVC_GetMediaType(aSheetNo);
	US aMediaWeight =  SRVC_GetMediaWeight(aSheetNo);
	US NVM_TIME;

	 //CR008
	if(aLength <= 2970) {

		// MediaType＝コート紙　
		if ( aMediaType == eCdiFinMediaType_Coated ) {

	 		NVM_TIME =SRVC_GetNvmValueUS(NVMID_TAMPING_START_SETTING5);
			
		} else if ((aMediaType ==eCdiFinMediaType_Plain) && (aMediaWeight >= 106))  {

		//　　MediaType=普通紙＆厚紙(106～221)
			NVM_TIME =SRVC_GetNvmValueUS(NVMID_TAMPING_START_SETTING3);

		} else { //それ以外
		
			NVM_TIME = SRVC_GetNvmValueUS(NVMID_TAMPING_START_SETTING1);
		}
		
	} else { //  >2970

		// MediaType＝コート紙　
		if ( aMediaType == eCdiFinMediaType_Coated ) {

			NVM_TIME =SRVC_GetNvmValueUS(NVMID_TAMPING_START_SETTING6);
			
		} else if ((aMediaType ==eCdiFinMediaType_Plain) && (aMediaWeight >= 106))  {

		//　　MediaType=普通紙＆厚紙(106～221)
			NVM_TIME =SRVC_GetNvmValueUS(NVMID_TAMPING_START_SETTING4);

		} else { //それ以外
		
			NVM_TIME = SRVC_GetNvmValueUS(NVMID_TAMPING_START_SETTING2);
		}

	}
	aTime = NVM_TIME;

	return aTime;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_GetEjectTampingDelayTime
// Description   :セットラストタンピング 動作開始時間取得
// Parameter     : aSheetNo(対象シート)
// Return        : aTime(動作時間)
///////////////////////////////////////////////////////////////////
US CMPL_TMP_GetEjectTampingDelayTime(UC aSheetNo)
{
	US  aTime=0;
	US aLength = SRVC_GetProcessSize(aSheetNo);

	US aMediaType = SRVC_GetMediaType(aSheetNo);
	US aMediaWeight =  SRVC_GetMediaWeight(aSheetNo);
	US NVM_TIME;

	 //CR008
	if(aLength <= 2970) {

		// MediaType＝コート紙　
		if ( aMediaType == eCdiFinMediaType_Coated ) {

	 		NVM_TIME =SRVC_GetNvmValueUS(NVMID_SETLAST_TAMPING_START_SETTING5);
			
		} else if ((aMediaType ==eCdiFinMediaType_Plain) && (aMediaWeight >= 106))  {

		//　　MediaType=普通紙＆厚紙(106～221)
			NVM_TIME =SRVC_GetNvmValueUS(NVMID_SETLAST_TAMPING_START_SETTING3);

		} else { //それ以外
		
			NVM_TIME = SRVC_GetNvmValueUS(NVMID_SETLAST_TAMPING_START_SETTING1);
		}
		
	} else { //  >2970

		// MediaType＝コート紙　
		if ( aMediaType == eCdiFinMediaType_Coated ) {

			NVM_TIME =SRVC_GetNvmValueUS(NVMID_SETLAST_TAMPING_START_SETTING6);
			
		} else if ((aMediaType ==eCdiFinMediaType_Plain) && (aMediaWeight >= 106))  {

		//　　MediaType=普通紙＆厚紙(106～221)
			NVM_TIME =SRVC_GetNvmValueUS(NVMID_SETLAST_TAMPING_START_SETTING4);

		} else { //それ以外
		
			NVM_TIME = SRVC_GetNvmValueUS(NVMID_SETLAST_TAMPING_START_SETTING2);
		}

	}
	aTime = NVM_TIME;

	 if( (CMPL_TMP_IsCanTampingSheetSize(aSheetNo) == false) &&
	 	(SRVC_GetCrossProcessSizeMixSet(aSheetNo) == SHT_SIZE_MIX) ) { //Mix Eject ..
		aTime = 251;
	}
	return aTime;

}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_EjectTamping
// Description   : イジェクトタンピング動作
// Parameter     : aSheetNo(対象シート)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_EjectTamping(UC aSheetNo)
{
	if( (CMPL_TMP_LEFT_IsOccurFault() == false)
		&& (CMPL_TMP_RIGHT_IsOccurFault() == false) ) {
		if( (SRVC_CheckSheetConditionNormal(aSheetNo) == true) &&
			(CMPL_TMP_IsCanTampingSheetSize(aSheetNo) == true) &&
			(SRVC_GetCrossProcessSizeMixSet(aSheetNo) == SHT_SIZE_UNMIX) ) {

			CMPL_TMP_StartEjectTamping(aSheetNo);
		} else {
			CMPL_TMP_CompleteEjectTamping(aSheetNo);
		}
	} else {
		CMPL_TMP_CompleteEjectTamping(aSheetNo);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_StartEjectTamping
// Description   : イジェクトタンピング動作開始
// Parameter     : aSheetNo(対象シート)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_StartEjectTamping(UC aSheetNo)
{
	gCMPL_TMP_ActionState = 0;

	CMPL_TMP_LEFT_StartAct(CMPL_TMP_EJECT_TAMPING, aSheetNo);
	CMPL_TMP_RIGHT_StartAct(CMPL_TMP_EJECT_TAMPING, aSheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_CompleteEjectTamping
// Description   : ノーマルタンピング動作完了
// Parameter     : aSheetNo(対象シート)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_CompleteEjectTamping(UC aSheetNo)
{
	UL aActionData = (UL)(aSheetNo) | ((UL)(CMPL_MDL_ACT_EJECT_TAMPING) << 8);

	MSP_SetEventTimer(0, CMPL_TMP_InformedModuleActionComplete, aActionData);
}

//V3.5.95  MissRegi防止動作
///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_MissRegi
// Description   : MissRegi防止動作
// Parameter     : aSheetNo(対象シート)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_MissRegi(UC aSheetNo)
{
	if( (CMPL_TMP_LEFT_IsOccurFault() == false)
		&& (CMPL_TMP_RIGHT_IsOccurFault() == false) ) {
		if( (SRVC_CheckSheetConditionNormal(aSheetNo) == true) &&
			(CMPL_TMP_IsCanTampingSheetSize(aSheetNo) == true) &&
			(SRVC_GetCrossProcessSizeMixSet(aSheetNo) == SHT_SIZE_UNMIX) &&
			(SRVC_GetStapleMode(aSheetNo) == eCdiFinStaple_SingleFront ) ){  //StapleMode 

			CMPL_TMP_StartMissRegi(aSheetNo);
		} else {
			CMPL_TMP_CompleteMissRegi(aSheetNo);
		}
	} else {
		CMPL_TMP_CompleteMissRegi(aSheetNo);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_StartMissRegi
// Description   :MissRegi防止動作開始
// Parameter     : aSheetNo(対象シート)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_StartMissRegi(UC aSheetNo)
{
	gCMPL_TMP_ActionState = 0;

	CMPL_TMP_LEFT_StartAct(CMPL_TMP_MISSREGI, aSheetNo);

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_CompleteMissRegi
// Description   : MissRegi防止動作完了
// Parameter     : aSheetNo(対象シート)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_CompleteMissRegi(UC aSheetNo)
{
	UL aActionData = (UL)(aSheetNo) | ((UL)(CMPL_MDL_ACT_TAMP_MISS_REGI) << 8);

	MSP_SetEventTimer(0, CMPL_TMP_InformedModuleActionComplete, aActionData);
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_Offset
// Description   : オフセット動作
// Parameter     : aSheetNo(対象シート)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_Offset(UC aSheetNo)
{	//t.b.d.
	UL aActionData = (UL)(aSheetNo) | ((UL)(CMPL_MDL_ACT_OFFSET) << 8);

	if( (CMPL_TMP_LEFT_IsOccurFault() == false)
		&& (CMPL_TMP_RIGHT_IsOccurFault() == false) ) {
		if( (SRVC_CheckSheetConditionNormal(aSheetNo) == true) &&
			(CMPL_TMP_IsCanTampingSheetSize(aSheetNo) == true) &&
			(SRVC_GetCrossProcessSizeMixSet(aSheetNo) == SHT_SIZE_UNMIX)&&
			( SRVC_GetOffsetFinish(aSheetNo) != SHT_OFST_FIN_NOT )) { //No MixSize & Offset Finish

			CMPL_TMP_StartOffset(aSheetNo);
		} else {
			CMPL_TMP_CompleteOffset(aSheetNo);
		}
	} else {
		CMPL_TMP_CompleteOffset(aSheetNo);
	}

}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_StartOffset
// Description   : Offset 動作開始
// Parameter     : aSheetNo(対象シート)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_StartOffset(UC aSheetNo)
{
	gCMPL_TMP_ActionState = 0;

	CMPL_TMP_LEFT_StartAct(CMPL_TMP_OFFSET, aSheetNo);
	CMPL_TMP_RIGHT_StartAct(CMPL_TMP_OFFSET, aSheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_CompleteOffset
// Description   : Offset動作完了
// Parameter     : aSheetNo(対象シート)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_CompleteOffset(UC aSheetNo)
{
	UL aActionData = (UL)(aSheetNo) | ((UL)(CMPL_MDL_ACT_OFFSET) << 8);

	MSP_SetEventTimer(0, CMPL_TMP_InformedModuleActionComplete, aActionData);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_Return
// Description   : リターン動作
// Parameter     : aSheetNo(対象シート)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_Return(UC aSheetNo)
{
	if( (CMPL_TMP_LEFT_IsOccurFault() == false)
		&& (CMPL_TMP_RIGHT_IsOccurFault() == false) ) {
		if( (SRVC_CheckSheetConditionNormal(aSheetNo) == true) &&
			(CMPL_TMP_IsCanTampingSheetSize(aSheetNo) == true) &&
			(SRVC_GetCrossProcessSizeMixSet(aSheetNo) == SHT_SIZE_UNMIX) ) {

			CMPL_TMP_StartReturn(aSheetNo);
		} else {
			CMPL_TMP_CompleteReturn(aSheetNo);
		}
	} else {
		CMPL_TMP_CompleteReturn(aSheetNo);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_StartReturn
// Description   : リターン動作
// Parameter     : aSheetNo(対象シート)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_StartReturn(UC aSheetNo)
{
	gCMPL_TMP_ActionState = 0;

	CMPL_TMP_LEFT_StartAct(CMPL_TMP_LEFT_RETURN, aSheetNo);
	CMPL_TMP_RIGHT_StartAct(CMPL_TMP_RIGHT_RETURN, aSheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_CompleteReturn
// Description   : リターン動作
// Parameter     : aSheetNo(対象シート)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_CompleteReturn(UC aSheetNo)
{
	UL aActionData = (UL)(aSheetNo) | ((UL)(CMPL_MDL_ACT_RETURN) << 8);

	MSP_SetEventTimer(0, CMPL_TMP_InformedModuleActionComplete, aActionData);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_Open
// Description   : オープン動作
// Parameter     : aSheetNo(対象シート)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_Open(UC aSheetNo)
{
	UC aPrvShtNo = SRVC_GetPrevRequestSheet(aSheetNo);

	if( (CMPL_TMP_LEFT_IsOccurFault() == false)
		&& (CMPL_TMP_RIGHT_IsOccurFault() == false) ) {
		if( CMPL_TMP_IsOpenExec(aSheetNo) == true ) {
			CMPL_TMP_StartOpen(aSheetNo);
		} else {
			CMPL_TMP_CompleteOpen(aSheetNo);
		}
	} else {
		CMPL_TMP_CompleteOpen(aSheetNo);										// Irregular Case
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_StartOpen
// Description   : オープン動作開始
// Parameter     : aSheetNo(対象シート)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_StartOpen(UC aSheetNo)
{
	gCMPL_TMP_ActionState = 0;

	CMPL_TMP_LEFT_StartAct(CMPL_TMP_OPEN, aSheetNo);
	CMPL_TMP_RIGHT_StartAct(CMPL_TMP_OPEN, aSheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_CompleteOpen
// Description   : オープン動作完了
// Parameter     : aSheetNo(対象シート)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_CompleteOpen(UC aSheetNo)
{
	UL aActionData = (UL)(aSheetNo) | ((UL)(CMPL_MDL_ACT_OPEN) << 8);

	MSP_SetEventTimer(0, CMPL_TMP_InformedModuleActionComplete, aActionData);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_IsOpenExec
// Description   : オープン動作実施有無を判断
// Parameter     : sheetNo(対象シート)
// Return        : true…オープン動作を実施する、false…オープン動作を実施しない
///////////////////////////////////////////////////////////////////
bool CMPL_TMP_IsOpenExec(UC sheetNo)
{
	UC prvShtNo = SRVC_GetPrevRequestSheet(sheetNo);

	if( prvShtNo != SHEETNO_NUM ) {
		if( (SRVC_CheckSheetConditionNormal(sheetNo) == true) &&
			(CMPL_TMP_IsCanTampingSheetSize(prvShtNo) == true) &&
			(SRVC_GetCrossProcessSizeMixSet(prvShtNo) == SHT_SIZE_UNMIX) &&
			(SRVC_GetCrossProcessSizeMixSet(sheetNo) == SHT_SIZE_MIX) ) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_InformedCompoControlStart
// Description   : Compiler ModuleにComponent Control Action開始を通知する
// Parameter     : aChainLink(0-15bit:ChainNo、16-31bit:LinkNo)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_InformedCompoControlStart(SS aTimerID, UL aChainLink)
{
	US aChainNo = (US)(aChainLink & 0x0000FFFF);
	US aLinkNo = (US)((aChainLink >> 16) & 0x0000FFFF);
	CMPL_InformedCompoControlComplete(aChainNo, aLinkNo, ACT_START);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_InformedCompoControlComplete
// Description   : Compiler ModuleにComponent Control Action終了を通知する
// Parameter     : aChainLink(0-15bit:ChainNo、16-31bit:LinkNo)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_InformedCompoControlComplete(SS aTimerID, UL aChainLink)
{
	US aChainNo = (US)(aChainLink & 0x0000FFFF);
	US aLinkNo = (US)((aChainLink >> 16) & 0x0000FFFF);
	CMPL_InformedCompoControlComplete(aChainNo, aLinkNo, ACT_STOP);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_InformedLeftTamperComplete
// Description   : Deviceからパドルソレノイド動作完了を受信
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_InformedLeftTamperComplete(ECMPL_TMP_Action aAction, UC aSheetNo)
{
	switch( aAction ) {

	case CMPL_TMP_SIZE_POS:
		if( (gCMPL_TMP_ActionState & CMPL_TMP_RIGHT_ACTION_COMPLETE) != 0 ) {
			gCMPL_TMP_ActionState = 0;
			CMPL_TMP_CompleteSizePositioning(aSheetNo);
		} else {
			gCMPL_TMP_ActionState |= CMPL_TMP_LEFT_ACTION_COMPLETE;
		}
		break;

	case CMPL_TMP_NORMAL_TAMPING:
		if( (gCMPL_TMP_ActionState & CMPL_TMP_RIGHT_ACTION_COMPLETE) != 0 ) {
			gCMPL_TMP_ActionState = 0;
			CMPL_TMP_CompleteNormalTamping(aSheetNo);
		} else {
			gCMPL_TMP_ActionState |= CMPL_TMP_LEFT_ACTION_COMPLETE;
		}
		break;

	case CMPL_TMP_EJECT_TAMPING:
		if( (gCMPL_TMP_ActionState & CMPL_TMP_RIGHT_ACTION_COMPLETE) != 0 ) {
			gCMPL_TMP_ActionState = 0;
			CMPL_TMP_CompleteEjectTamping(aSheetNo);
		} else {
			gCMPL_TMP_ActionState |= CMPL_TMP_LEFT_ACTION_COMPLETE;
		}
		break;
		
	case CMPL_TMP_MISSREGI :  //MissRegi防止
		CMPL_TMP_CompleteMissRegi(aSheetNo);
		break;
		
	case CMPL_TMP_OFFSET:
		if( (gCMPL_TMP_ActionState & CMPL_TMP_RIGHT_ACTION_COMPLETE) != 0 ) {
			gCMPL_TMP_ActionState = 0;
			CMPL_TMP_CompleteOffset(aSheetNo);
		} else {
			gCMPL_TMP_ActionState |= CMPL_TMP_LEFT_ACTION_COMPLETE;
		}
		break;

	case CMPL_TMP_LEFT_RETURN:
		if( (gCMPL_TMP_ActionState & CMPL_TMP_RIGHT_ACTION_COMPLETE) != 0 ) {
			gCMPL_TMP_ActionState = 0;
			CMPL_TMP_CompleteReturn(aSheetNo);
		} else {
			gCMPL_TMP_ActionState |= CMPL_TMP_LEFT_ACTION_COMPLETE;
		}
		break;

	case CMPL_TMP_OPEN:
		if( (gCMPL_TMP_ActionState & CMPL_TMP_RIGHT_ACTION_COMPLETE) != 0 ) {
			gCMPL_TMP_ActionState = 0;
			CMPL_TMP_CompleteOpen(aSheetNo);
		} else {
			gCMPL_TMP_ActionState |= CMPL_TMP_LEFT_ACTION_COMPLETE;
		}
		break;
	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_InformedRightTamperComplete
// Description   : Deviceからパドルソレノイド動作完了を受信
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_InformedRightTamperComplete(ECMPL_TMP_Action aAction, UC aSheetNo)
{

	switch( aAction ) {

	case CMPL_TMP_SIZE_POS:
		if( (gCMPL_TMP_ActionState & CMPL_TMP_LEFT_ACTION_COMPLETE) != 0 ) {
			gCMPL_TMP_ActionState = 0;
			CMPL_TMP_CompleteSizePositioning(aSheetNo);
		} else {
			gCMPL_TMP_ActionState |= CMPL_TMP_RIGHT_ACTION_COMPLETE;
		}
		break;

	case CMPL_TMP_NORMAL_TAMPING:
		if( (gCMPL_TMP_ActionState & CMPL_TMP_LEFT_ACTION_COMPLETE) != 0 ) {
			gCMPL_TMP_ActionState = 0;
			CMPL_TMP_CompleteNormalTamping(aSheetNo);
		} else {
			gCMPL_TMP_ActionState |= CMPL_TMP_RIGHT_ACTION_COMPLETE;
		}
		break;

	case CMPL_TMP_EJECT_TAMPING:
		if( (gCMPL_TMP_ActionState & CMPL_TMP_LEFT_ACTION_COMPLETE) != 0 ) {
			gCMPL_TMP_ActionState = 0;
			CMPL_TMP_CompleteEjectTamping(aSheetNo);
		} else {
			gCMPL_TMP_ActionState |= CMPL_TMP_RIGHT_ACTION_COMPLETE;
		}
		break;

	case CMPL_TMP_OFFSET:
		if( (gCMPL_TMP_ActionState & CMPL_TMP_LEFT_ACTION_COMPLETE) != 0 ) {
			gCMPL_TMP_ActionState = 0;
			CMPL_TMP_CompleteOffset(aSheetNo);
		} else {
			gCMPL_TMP_ActionState |= CMPL_TMP_RIGHT_ACTION_COMPLETE;
		}
		break;

	case CMPL_TMP_RIGHT_RETURN:
		if( (gCMPL_TMP_ActionState & CMPL_TMP_LEFT_ACTION_COMPLETE) != 0 ) {
			gCMPL_TMP_ActionState = 0;
			CMPL_TMP_CompleteReturn(aSheetNo);
		} else {
			gCMPL_TMP_ActionState |= CMPL_TMP_RIGHT_ACTION_COMPLETE;
		}
		break;

	case CMPL_TMP_OPEN:
		if( (gCMPL_TMP_ActionState & CMPL_TMP_LEFT_ACTION_COMPLETE) != 0 ) {
			gCMPL_TMP_ActionState = 0;
			CMPL_TMP_CompleteOpen(aSheetNo);
		} else {
			gCMPL_TMP_ActionState |= CMPL_TMP_RIGHT_ACTION_COMPLETE;
		}
		break;

	default:
		break;

	}

}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_InformedCompoComplete
// Description   : Deviceからパドルソレノイド動作完了を受信
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_InformedCompoComplete(US aLinkNo)
{
	UL aChainLink = (UL)(CMPL_TMP_COMPO_CHAIN) | ((UL)(aLinkNo) << 16);
	MSP_SetEventTimer(0, CMPL_TMP_InformedCompoControlComplete, aChainLink);	
}


void CMPL_TMP_InformedLeftTamperHomeSensorOnFail(UC aSheetNo)
{
	CMPL_TMP_StopModuleAction();
	CMPL_InformedDeviceStatusChange(DSNO_LEFT_TAMPER_ON_FAIL, FAULT_OCCUR);
}

void CMPL_TMP_InformedLeftTamperHomeSensorOffFail(UC aSheetNo)
{
	CMPL_TMP_StopModuleAction();
	CMPL_InformedDeviceStatusChange(DSNO_LEFT_TAMPER_OFF_FAIL, FAULT_OCCUR);
}

void CMPL_TMP_InformedRightTamperHomeSensorOnFail(UC aSheetNo)
{
	CMPL_TMP_StopModuleAction();
	CMPL_InformedDeviceStatusChange(DSNO_RIGHT_TAMPER_ON_FAIL, FAULT_OCCUR);
}

void CMPL_TMP_InformedRightTamperHomeSensorOffFail(UC aSheetNo)
{
	CMPL_TMP_StopModuleAction();
	CMPL_InformedDeviceStatusChange(DSNO_RIGHT_TAMPER_OFF_FAIL, FAULT_OCCUR);
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_InformedModuleActionComplete
// Description   : Compiler ModuleにModule Action終了を通知する
// Parameter     : aActionData(0-7bit:SheetNo、8-15bit:指定動作)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_InformedModuleActionComplete(SS aTimerID, UL aActionData)
{
	UC aSheetNo = (UC)(aActionData & 0x000000FF);
	UC aAction = (UC)((aActionData >> 8) & 0x000000FF);

	CMPL_InformedModuleActionComp((ECMPL_ModuleAction)(aAction), aSheetNo);
}
