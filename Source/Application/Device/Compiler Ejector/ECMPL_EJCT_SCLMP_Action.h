// File Name:	ECMPL_EJCT_SCLMP_Action.h
// Description:	Set ClampDevice の動作を定義
// Engineer:	KimYS
// Last Edit:	15.03.03
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(ECMPL_EJCT_SCLMP_Action_h)
#define ECMPL_EJCT_SCLMP_Action_h

typedef enum /* ECMPL_EJCT_SCLMP_Action */
{
	// SetClamp
	CMPL_EJCT_SCLMP_st0, //Idle状態

   	// Component 動作(Eject Mot) 開始(Link No)
	CMPL_EJCT_COMPO_CLUTCH 	= 22,
	CMPL_EJCT_SCLMP_COMPO_CLAMP = 23,   // Set Clamp ClutchをON/OFFする。

	CMPL_EJCT_SCLMP_COMPO = 0x0800, //Component(Eject動作+SetClamp)
	CMPL_EJCT_SCLMP_COMPO_st1, //Eject HomeSNR状態チェック
	CMPL_EJCT_SCLMP_COMPO_st2_ONWAIT, //Eject HomeSNR　On待ち
	CMPL_EJCT_SCLMP_COMPO_st2_OFFWAIT, //Eject HomeSNR　Off待ち
	CMPL_EJCT_SCLMP_COMPO_st3, //T4設定
	CMPL_EJCT_SCLMP_COMPO_st4, //SetClampHomeSNR　OFF待ち
	CMPL_EJCT_SCLMP_COMPO_st5, //T5タイマ設定
	CMPL_EJCT_SCLMP_COMPO_st6, //T6タイマ設定
	CMPL_EJCT_SCLMP_COMPO_st7, //SetClampHomeSNR　ON待ち
	CMPL_EJCT_SCLMP_COMPO_st8, //T7タイマ設定
	CMPL_EJCT_SCLMP_COMPO_st9, //SetClamp動作完了

	// SetClamp動作
	CMPL_EJCT_SCLMP = 0x0A00,
	CMPL_EJCT_SCLMP_st1,		//
	CMPL_EJCT_SCLMP_st2_OFFWAIT,// Eject HomeSNR　Off待ち	CMPL_EJCT_SCLMP_st3, //T4設定
	CMPL_EJCT_SCLMP_st3,		// T4設定
	CMPL_EJCT_SCLMP_st4, 		// SetClampHomeSNR　OFF待ち
	CMPL_EJCT_SCLMP_st5, 		// T5タイマ設定
	CMPL_EJCT_SCLMP_st6, 		// T6タイマ設定
	CMPL_EJCT_SCLMP_st7, 		// SetClampHomeSNR　ON待ち
	CMPL_EJCT_SCLMP_st8, 		// T7タイマ設定
	CMPL_EJCT_SCLMP_st9, 		// SetClamp動作完了
	// CMPL_EJCT_SCLMP_st10_FAIL,// Fail発生
	// CMPL_EJCT_SCLMP_st11,	 // Fail停止

	// Number of Module Action
	CMPL_EJCT_SCLMP_ACT_NUM,

} ECMPL_EJCT_SCLMP_Action;

#define CMPL_EJCT_SCLMP_ACTION_MASK 0xFF00


#endif //#if !defined(ECMPL_EJCT_SCLMP_Action_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

