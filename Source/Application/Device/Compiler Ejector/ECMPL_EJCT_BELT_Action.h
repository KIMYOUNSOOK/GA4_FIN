// File Name:	ECMPL_EJCT_BELT_Action.h
// Description:	Ejector Mot Device の動作を定義
// Engineer:	FXK  KimYS
// Last Edit:	15.02.26
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(ECMPL_EJCT_BELT_Action_h)
#define ECMPL_EJCT_BELT_Action_h

typedef enum /* ECMPL_EJCT_BELT_Action */
{
	//Eject Mot 停止
	CMPL_EJCT_st0,   //IDLE

	CMPL_EJCT_COMPO_FWD_EJECT = 20,	// Eject Compile Motorを正転方向に駆動する。
	CMPL_EJCT_COMPO_REV_PADDLE = 21,	// Eject Compile Motorを逆転方向に駆動する。

	//Component 動作(Eject Mot - CW)
	CMPL_EJCT_COMPO_FWD = 0x0100,  
	CMPL_EJCT_COMPO_FWD_st1,		// モータ駆動設定
	CMPL_EJCT_COMPO_FWD_st2,		// モータ駆動開始
	CMPL_EJCT_COMPO_FWD_st3_ONWait,//EjectHomeSNR　ON待ち中 
	CMPL_EJCT_COMPO_FWD_st3_OFFWait,// EjectHomeSNR　OFF待ち中
	CMPL_EJCT_COMPO_FWD_st4,		// OFF受信(T2開始)
	CMPL_EJCT_COMPO_FWD_st5,		//　T2後、SpeedDown設定
	CMPL_EJCT_COMPO_FWD_st6_ONWait,// EjectHomeSNR　ON待ち中
	CMPL_EJCT_COMPO_FWD_st7,		// ON受信(T3開始)
	CMPL_EJCT_COMPO_FWD_st8,		// T3後、StepDown設定
	CMPL_EJCT_COMPO_FWD_st9,		// 後励磁
	CMPL_EJCT_COMPO_FWD_st10,		//完了

	//Component 動作(Eject Mot - CCW )
	CMPL_EJCT_COMPO_REV = 0x0200,  
	CMPL_EJCT_COMPO_REV_st1,		// モータ駆動設定
	CMPL_EJCT_COMPO_REV_st2,		// モータ駆動開始
	CMPL_EJCT_COMPO_REV_st3_ONWait,		//　SubPaddleHomeSNR　ON待ち
	CMPL_EJCT_COMPO_REV_st4,		//　T1 Timer
	CMPL_EJCT_COMPO_REV_st5,	//StepDown
	CMPL_EJCT_COMPO_REV_st6,//後励磁
	CMPL_EJCT_COMPO_REV_st7,	//完了

	//SubPaddle Home動作
	CMPL_EJCT_PADHOME  = 0x0300,
	CMPL_EJCT_PADHOME_st1,		 //モータ駆動 設定
	CMPL_EJCT_PADHOME_st2,		//モータ駆動開始
	CMPL_EJCT_PADHOME_st3_ONWait,		//SNR ON待ち
	CMPL_EJCT_PADHOME_st4,		//ON割込み受信(T1設定)
	CMPL_EJCT_PADHOME_st5,		//StepDown設定
	CMPL_EJCT_PADHOME_st6,		//後励磁
	CMPL_EJCT_PADHOME_st7,		//SubPaddleHome動作完了
	CMPL_EJCT_PADHOME_st11_1,		//DummyPulse設定 (V3.5.91)
	CMPL_EJCT_PADHOME_st11_2,		//１Clock後Timer設定 (V3.5.91)

	// EjectHome動作
	CMPL_EJCT_EJTHOME  = 0x0400,
	CMPL_EJCT_EJTHOME_st1,		 //Mot駆動設定
	CMPL_EJCT_EJTHOME_st2,		//Mot駆動開始
//	CMPL_EJCT_EJTHOME_st3_ONWait,//EjectHome SNR　ON待ち中
	CMPL_EJCT_EJTHOME_st3_OFFWait,//EjectHomeSNR　OFF待ち中
	CMPL_EJCT_EJTHOME_st4,		//SNR OFF受信（T2開始）
	CMPL_EJCT_EJTHOME_st5,		//T2後、SpeedDown設定
	CMPL_EJCT_EJTHOME_st6_ONWait,//EjectHomeSNR　On待ち中
	CMPL_EJCT_EJTHOME_st7,		//ON受信(T3開始)
	CMPL_EJCT_EJTHOME_st8,		// T3後、StepDown設定
	CMPL_EJCT_EJTHOME_st9,		//完了
	CMPL_EJCT_EJTHOME_st11_1,	//DummyPulse設定 (V3.5.91)
	CMPL_EJCT_EJTHOME_st11_2,	//１Clock後Timer設定 (V3.5.91)

	//Compile動作
	CMPL_EJCT_COMPILE = 0x0500,
	CMPL_EJCT_COMPILE_st1,	//Mot 駆動設定
	CMPL_EJCT_COMPILE_st2,	//Compile動作開始
	CMPL_EJCT_COMPILE_st3,	//P回分処理
	CMPL_EJCT_COMPILE_st4,	//StepDown開始
	CMPL_EJCT_COMPILE_st5,	//後励磁
	CMPL_EJCT_COMPILE_st6,	//MotOFF


	//Eject動作
	CMPL_EJCT_EJECT = 0x0600,
	CMPL_EJCT_EJECT_st1,	//Mot 駆動設定
	CMPL_EJCT_EJECT_st2,	//Eject動作開始
	CMPL_EJCT_EJECT_st3_OFFWait,//EjectHomeSNR　OFF待ち
	CMPL_EJCT_EJECT_st4,	//OFF受信(T2設定）
	CMPL_EJCT_EJECT_st5,	//T2後、SpeedDown設定
	CMPL_EJCT_EJECT_st6_ONWait,//EjectHomeSNR　On待ち
	CMPL_EJCT_EJECT_st7,	//ON受信(T3開始)
	CMPL_EJCT_EJECT_st8,	//T3後、StepDown設定
	CMPL_EJCT_EJECT_st9,	//後励磁設定
	CMPL_EJCT_EJECT_st10,	//完了
	CMPL_EJCT_EJECT_st11_1,	//DummyPulse設定 (V3.5.91)
	CMPL_EJCT_EJECT_st11_2,	//１Clock後Timer設定 (V3.5.91)


	CMPL_EJCT_ACT_NUM,

} ECMPL_EJCT_BELT_Action;

#define CMPL_EJCT_BELT_ACTION_MASK 0xFF00

//#define CMPL_EJCT_PAD_ACTION_MASK 0xFF00



#endif //#if !defined(ECMPL_EJCT_PAD_Action_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

