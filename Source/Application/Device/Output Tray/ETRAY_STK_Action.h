// File Name:	ECMPL_EJCT_BELT_Action.h
// Description:	Ejector Mot Device の動作を定義
// Engineer:	FXK  KimYS
// Last Edit:	15.02.26
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(ETRAY_STK_Action_h)
#define ETRAY_STK_Action_h

#define  TRAY_COMPO_CHAIN 12

typedef enum /* ETRAY_STK_Action */
{

	// STACKER 停止
	TRAY_STK_st0,	//IDLE 

	TRAY_STK_COMPO_COMPILE = 60,
	TRAY_STK_COMPO_EJECT = 61,

	// STACKER動作(Compo) 開始
	// STACKER MOT (Compile位置へ移動)
	// TRAY_STK_COMPO_COMPILE = 0x0100, 

	// STACKER MOT (Eject位置へ移動)
	// TRAY_STK_COMPO_EJECT = 0x0200, 

	//Stacker Home位置へ移動()
	TRAY_STK_HOME = 0x0300,
	TRAY_STK_HOME_st1_1,  //開始タイミング設定
	TRAY_STK_HOME_st1_2,  //HeightSNR1，2による位置判断
	TRAY_STK_HOME_st2_UP,   //モータ駆動設定
	TRAY_STK_HOME_st3_UP,  //モータ駆動開始(上向き)
	TRAY_STK_HOME_st4_UP_ONWait,  //NoPaperSNR　ON待ち
	TRAY_STK_HOME_st4_UP_OFFWait, //NoPaperSNR　OFF待ち 
	TRAY_STK_HOME_st5_UP,  //　Reset開始
	TRAY_STK_HOME_st6_UP,  //　Reset完了
	TRAY_STK_HOME_st7_UP,  //　SNR1　OFF受信(10Pulse開始)
	TRAY_STK_HOME_st8_UP,  //　StepDown開始
	TRAY_STK_HOME_st9_UP,  //　タイマ設定
	
	TRAY_STK_HOME_st2_DN,  //モータ駆動設定
	TRAY_STK_HOME_st3_DN,  //モータ駆動開始(下向き)
	TRAY_STK_HOME_st4_DN,  //SNR1　ON受信(H3Pulse開始)
	TRAY_STK_HOME_st5_DN,  //StepDown開始　
	TRAY_STK_HOME_st6_DN,  //Stacker待機位置へ移動動作開始Trigger待ち
	TRAY_STK_HOME_st7_DN,  //モータ駆動開始(上向き)
	TRAY_STK_HOME_st8_DN,  //SNR1　OFF受信(10Pulse開始)
	TRAY_STK_HOME_st9_DN,  //StepDown開始
	TRAY_STK_HOME_st10_DN,  //後励磁
	TRAY_STK_HOME_st11_DN,  //StackerHome動作完了
	
      //Stacker待機位置へ移動動作開始TriggerからReset動作開始(0519)
	TRAY_STK_HOME_st20_ONWait,  //NoPaperSNR　ON待ち
	TRAY_STK_HOME_st20_OFFWait,  //NoPaperSNR　OFF待ち	
	TRAY_STK_HOME_st21,  //　Reset開始
	TRAY_STK_HOME_st22,  //　Reset完了

	//Compile位置へ移動
	TRAY_STK_COMPILE = 0x0400,
	TRAY_STK_COMPILE_st1,  //StackerHeightSNRによる分岐判断
	TRAY_STK_COMPILE_st2_UP,  //モータ駆動設定
	TRAY_STK_COMPILE_st3_UP,  //モータ駆動開始
	TRAY_STK_COMPILE_st4_UP_ONWait,  //NoPaperSNR　ON待ち
	TRAY_STK_COMPILE_st4_UP_OFFWait,  //NoPaperSNR　OFF待ち
	TRAY_STK_COMPILE_st5_UP,  //Reset開始
	TRAY_STK_COMPILE_st6_UP,  //Reset完了
	TRAY_STK_COMPILE_st7_UP,  //SNR1　OFF受信(10Pulse開始)
	TRAY_STK_COMPILE_st8_UP,  //StepDown開始
	TRAY_STK_COMPILE_st9_UP,  //タイマ(方向転換用)設定
	TRAY_STK_COMPILE_st10_UP,  //モータ駆動開始(下向き)
	TRAY_STK_COMPILE_st11_UP,  //SNR1　ON受信(G-Pulse開始)
	TRAY_STK_COMPILE_st12_UP,  //StepDown開始
	
	TRAY_STK_COMPILE_st2_DN,  //モータ駆動設定
	TRAY_STK_COMPILE_st3_DN,  //モータ駆動開始
	TRAY_STK_COMPILE_st4_DN,  //SNR1 ON受信(G-Pulse開始)
	TRAY_STK_COMPILE_st5_DN,  //StepDown開始
	
	TRAY_STK_COMPILE_st13,  //Compile位置へ移動完了

	
	//Eject位置へ移動(ADJUST)
	TRAY_STK_EJECT = 0x0500,
	TRAY_STK_EJECT_st1,  //T1タイマ待ち
	TRAY_STK_EJECT_st2,  //モータ駆動設定
	TRAY_STK_EJECT_st3,  //モータ駆動開始(H-Pulse+StepDown分移動)
	TRAY_STK_EJECT_st4,  //Eject位置へ移動完了


	//待機位置へ移動(ProcessEnd)
	TRAY_STK_END = 0x0600,
	TRAY_STK_END_st1,  //モータ駆動開始
	TRAY_STK_END_st2_ONWait,  //NoPaperSNR　ON待ち
	TRAY_STK_END_st2_OFFWait,  //NoPaperSNR　OFF待ち
	TRAY_STK_END_st3,  //Reset開始
	TRAY_STK_END_st4,  //Reset完了
	TRAY_STK_END_st5,  //SNR1　OFF受信(10Pulse開始)
	TRAY_STK_END_st6,  //StepDown開始
	TRAY_STK_END_st7,  //後励磁
	TRAY_STK_END_st8,  //待機位置へ移動完了

	// Number of Module Action
	TRAY_STK_ACT_NUM,

} ETRAY_STK_Action;

#define  TRAY_STK_ACTION_MASK 0xFF00

#endif //#if !defined(ETRAY_STK_Action_h)

/*#if !defined(Tray_Action_h)
#define Tray_Action_h

#include "DD_STM.h"

typedef enum
{
	//Eject Mot 停止
	TRAY_st0,   //IDLE

	//Component 動作(Tray Motor) 開始(Link No)
	TRAY_COMPO_UP = 60,   	// Stacker Trayを上昇させる。
	TRAY_COMPO_DOWN	= 61,	//Stacker Trayを下降させる。
	TRAY_COMPO_EJECTMOT,
	TRAY_COMPO_st1,
	TRAY_COMPO_st2,
	TRAY_COMPO_st3,
	TRAY_COMPO_st4, 

} STK_TRAY_Action;


typedef struct {
	STK_TRAY_Action    action;    //Main  State
	STK_TRAY_Action	  compoAction; //Component ActionState

	UC  	 sheetNo;
	SS 	 timerIndex;
	SS    interruptIndex;
	UC 	 direction;
	DD_STM_PROF_ID profile;

} STK_TRAY_Control;

static STK_TRAY_Control     gSTK_TRAY;

#endif //#if !defined(Tray_Action_h) */

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

