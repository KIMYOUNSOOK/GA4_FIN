// File Name:	ECMPL_STPL_HEAD_Action.h
// Description:	staple Device の動作を定義
// Engineer:	Takahiko Minaga
// Last Edit:	14.09.01
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(ECMPL_STPL_HEAD_Action_h)
#define ECMPL_STPL_HEAD_Action_h


typedef enum /* ECMPL_STPL_HEAD_Action */
{
	// ステープルヘッド停止
	CMPL_STPL_HEAD_ACT_IDLE,													// 0:停止状態

	CMPL_STPL_HEAD_COMPO_FORWARD = 50,
	CMPL_STPL_HEAD_COMPO_REVERSE = 51,

	// ステープルヘッドDiag動作
	CMPL_STPL_HEAD_COMPO = 0x0100,
	CMPL_STPL_HEAD_COMPO_st1,
	CMPL_STPL_HEAD_COMPO_st2,
	CMPL_STPL_HEAD_COMPO_st3,
	CMPL_STPL_HEAD_COMPO_st4,


	// ステープルヘッドホーム動作
	CMPL_STPL_HEAD_HOME = 0x0300,
	CMPL_STPL_HEAD_HOME_st1,			// Stapleモータ駆動
	CMPL_STPL_HEAD_HOME_st2,			// StapleHomeSnr待ち
	CMPL_STPL_HEAD_HOME_st3,			// StapleHomeSnrOn状態　　処理
	CMPL_STPL_HEAD_HOME_st10,			// HomeSnrOnFail発生処理
	CMPL_STPL_HEAD_HOME_st11,			// StapleMotorReverse方向駆動
	CMPL_STPL_HEAD_HOME_st12,			// StapleHomeSnr待ち
	CMPL_STPL_HEAD_HOME_st13,			// HomeSnrOn処理(StapleNG処理)
	CMPL_STPL_HEAD_HOME_st14,			// Fail処理

	// ステープル動作
	CMPL_STPL_HEAD_STAPLE = 0x0400,
	CMPL_STPL_HEAD_STAPLE_st1,			// Stapleモータ駆動　で処理
	CMPL_STPL_HEAD_STAPLE_st2, 			// StapleHomeSnr待ち
	CMPL_STPL_HEAD_STAPLE_st3, 			// StapleHomeSnrOn状態(動作完了)
	CMPL_STPL_HEAD_STAPLE_st10, 		// HomeSnrOnFail発生処理
	CMPL_STPL_HEAD_STAPLE_st11,			// StapleMotorReverse方向駆動
	CMPL_STPL_HEAD_STAPLE_st12,			// StapleHomeSnr待ち
	CMPL_STPL_HEAD_STAPLE_st13,			// HomeSnrOn待ち(StapleNG通知)
	CMPL_STPL_HEAD_STAPLE_st14,			// 　Fail処理

	// staple針出し動作
	CMPL_STPL_HEAD_HARIOUT = 0x0500,
	CMPL_STPL_HEAD_HARIOUT_st1,			// Stapleモータ駆動
	CMPL_STPL_HEAD_HARIOUT_st2,			// StapleHomeSnr待ち
	CMPL_STPL_HEAD_HARIOUT_st3, 		// StapleHomeSnrOn状態
	CMPL_STPL_HEAD_HARIOUT_st4, 		// Self Priming Sensor分岐
	CMPL_STPL_HEAD_HARIOUT_st10,		// HomeSnrOnFail発生処理
	CMPL_STPL_HEAD_HARIOUT_st11,		// StapleMotorReverse方向駆動
	CMPL_STPL_HEAD_HARIOUT_st12,		// StapleHomeSnr待ち
	CMPL_STPL_HEAD_HARIOUT_st13,		// HomeSnrOn状態
	CMPL_STPL_HEAD_HARIOUT_st14,		// Fail処理

} ECMPL_STPL_HEAD_Action;

#define CMPL_STPL_HEAD_ACTION_MASK 0xFF00


#endif //#if !defined(ECMPL_STPL_HEAD_Action_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

