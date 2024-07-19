// File Name:	TRAY_Manager.h
// Description:	Header File of Output Tray Module Manager
// Engineer:	Yuichi Hoshino
// Last Edit:	14.09.02
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef TRAY_Manager_h
#define TRAY_Manager_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////
//Debug Log用情報 (V3.0)
typedef enum {
	eSTK_LOG_PLS_RESET=0,  	//Reset時のパルス
	eSTK_LOG_PLS_CURRENT,		//現在パルス
	eSTK_LOG_UPDOWN,			//Compile位置へ移動時、UpDown　：　UP=0x00、DOWN=0xFF
	eSTK_LOG_LOWLIMIT		=0x10,	//LowLimitFail
	eSTK_LOG_FULLSTK_FULL=0x80,	//FULLStack発生
	eSTK_LOG_FULLSTK_MIXSIZE_FULL,//MixFullStack発生
	eSTK_LOG_FULLSTK_SETOVER,		//SetOverCount発生
	eSTK_LOG_FULLSTK_RELEASE=0x90,  //FullStack解除
	eSTK_LOG_FULLSTK_CLEAR,			 //Clear条件に当たる場合
	eSTK_LOG_FULLSTK_PREVUPDATE,   //V3.0.90 Test Previous pulse update(Compile LiftDown ...)
	eSTK_LOG_PROCESSEND_CHECK=0xA0,  //V3.3.92 ProcessEndCheck
	//eSTK_LOG_PROFILE,  //V3.3.92
	eSTK_LOG_DO_ORNOT=0xB0, //Compile 位置へ移動動作をするかしないかの結果
	eSTK_LOG_STATUS=0xF0,   //Stacker Status  
}ESTK_LogCategory;


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void TRAY_Manager_Constructor();

void TRAY_Manager_PowerUp();
void TRAY_Manager_ChangeDiag();
void TRAY_Manager_ChangeNormal();
void TRAY_Manager_PowerDown();
void TRAY_Manager_CycleUp();
void TRAY_Manager_HardDown();
void TRAY_Manager_CycleDown();
void TRAY_Manager_ReceiveStandbyExitNotify();

void TRAY_Manager_AnyCoverOpenNotify();
void TRAY_Manager_AllCoverCloseNotify();
void TRAY_Manager_ReceiveComponentControlStartReq(US aChainNo, US aLinkNo);
void  TRAY_Manager_InformedStackerActionComplete(US  aStkAction, UC aSheetNo);

//void TRAY_Manager_FullStackStatusNotify(UL notifierAddress, US status);
//[GLOBAL_A4]
void TRAY_Manager_LowerLimitCheck(UC aSheetNo);
void TRAY_Manager_SetOverCountCheck(UC aSheetNo);
void TRAY_Manager_MixFullStackCheckAfterCompile(UC aSheetNo);  //V3.0  導入
void  TRAY_Manager_SaveNVM(UC  aSheetNo);   //V3.0  導入
void TRAY_Manager_FullStackCheckAfterCompile(UC aSheetNo);
UC	TRAY_Manager_FullStackReleaseCondition();
UC  TRAY_Manager_ClearCheck();
void  TRAY_Manager_PreviousPulseUpdate(UL   currentPulse); //V03.00.90 

void TRAY_Manager_SheetEventNotify(US aEventID, UC aSheetNo);
void TRAY_Manager_SheetInitializeRequest(const UC* aEvt);

void TRAY_Manager_StartSheetInitialize(UC aSheetNo);
void  TRAY_Manager_StopSheetInitialize(UC aSheetNo);
void TRAY_Manager_CompleteSheetInitialize(void);
void TRAY_Manager_ProcessEnd(const UC* aEvt);
void TRAY_Manager_StartProcessEnd();
void TRAY_Manager_StopProcessEnd();
void TRAY_Manager_CompleteProcessEnd();
void TRAY_Manager_ComponentControl(const UC* evt, EAction aAction);
UC TRAY_Manager_GetStatus(void);


void PrintStackerTrayLog_Char(UC para1, UC para2, UC para3, UC para4);
void PrintStackerTrayLog_Short(UC para1, US para2, US para3, US para4);




#endif // #ifndef TRAY_Manager_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
