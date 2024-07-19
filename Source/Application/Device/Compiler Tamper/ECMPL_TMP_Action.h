// File Name:	ECMPL_TMP_Action.h
// Description:	Tamper Device の動作を定義
// Engineer:	FXK
// Last Edit:	15.3.12.
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(ECMPL_TMP_Action_h)
#define ECMPL_TMP_Action_h

typedef enum /* ECMPL_TMP_Action */
{

	// タンパー停止
	CMPL_TMP_st0,	//IDLE

	// タンパー動作(Compo) 開始
	// Right Tamper Component動作 (Link No)
	CMPL_TMP_COMPO_RIGHT_CW_LOW  = 30,	// RIGHT Tamperを外側方向(CW)に低速(643PPS)で移動
	CMPL_TMP_COMPO_RIGHT_CW_MID  = 31,	// RIGHT Tamperを外側方向(CW)に中速(800PPS)で移動
	CMPL_TMP_COMPO_RIGHT_CW_HI   = 32,	// RIGHT Tamperを外側方向(CW)に高速(1337PPS)で移動

	CMPL_TMP_COMPO_RIGHT_CCW_LOW = 33,	// RIGHT Tamperを内側方向(CCW)に低速(643PPS)で移動
	CMPL_TMP_COMPO_RIGHT_CCW_MID = 34,	// RIGHT Tamperを内側方向(CCW)に中速(800PPS)で移動
	CMPL_TMP_COMPO_RIGHT_CCW_HI  = 35,	// RIGHTTamperを内側方向(CCW)に高速(1337PPS)で移動
	CMPL_TMP_COMPO_RIGHT_SENSOR  = 36,	// For Test

	// Left Tamper Component動作 (Link No)
	CMPL_TMP_COMPO_LEFT_CW_LOW   = 40, 	// LEFT Tamperを外側方向(CW)に低速(643PPS)で移動
	CMPL_TMP_COMPO_LEFT_CW_MID   = 41, 	// LEFT Tamperを外側方向(CW)に中速(800PPS)で移動
	CMPL_TMP_COMPO_LEFT_CW_HI    = 42, 	// LEFT Tamperを外側方向(CW)に高速(1337PPS)で移動

	CMPL_TMP_COMPO_LEFT_CCW_LOW  = 43, 	// LEFT Tamperを内側方向(CCW)に低速(643PPS)で移動
	CMPL_TMP_COMPO_LEFT_CCW_MID  = 44, 	// LEFT Tamperを内側方向(CCW)に中速(800PPS)で移動
	CMPL_TMP_COMPO_LEFT_CCW_HI   = 45, 	// LEFT Tamperを内側方向(CCW)に高速(1337PPS)で移動
	CMPL_TMP_COMPO_LEFT_SENSOR   = 46,	// For Test

	// Component動作
	CMPL_TMP_COMPO_TAMPER_MOT = 0x0100,
	CMPL_TMP_COMPO_TAMPER_MOT_st1,  // Mot 駆動設定, 前励磁処理
	CMPL_TMP_COMPO_TAMPER_MOT_st2,  // 動作開始100Pulse 後, StepDown
	CMPL_TMP_COMPO_TAMPER_MOT_st3,	// 後励磁処理
	CMPL_TMP_COMPO_TAMPER_MOT_st4, 	// Mot Off

	// サイズポジショニング動作
	CMPL_TMP_SIZE_POS = 0x0200,
	CMPL_TMP_SIZE_POS_st1, 			// Tamper HomeSNR　ONチェック
	CMPL_TMP_SIZE_POS_st2_CCW, 		// Home外し動作設定
	CMPL_TMP_SIZE_POS_st3_CCW, 		// CCW方向にモータ駆動開始
	CMPL_TMP_SIZE_POS_st4_CCW, 		// Home外し動作完了
	CMPL_TMP_SIZE_POS_st2_CW, 		// Home動作設定
	CMPL_TMP_SIZE_POS_st3_CW, 		// CW方向にモータ駆動開始
	CMPL_TMP_SIZE_POS_st4_CW, 		// StepUp中、割込み受信
	CMPL_TMP_SIZE_POS_st4_2_CW, 	// StepUp中、割込み受信 から StepDown完了
	CMPL_TMP_SIZE_POS_st5_CW, 		// StepUp完了受信
	CMPL_TMP_SIZE_POS_st6_CW, 		// 安定区間で割込み受信
	CMPL_TMP_SIZE_POS_st7_CW, 		// Home動作完了
	CMPL_TMP_SIZE_POS_st8,   		// SizePositioning開始
	CMPL_TMP_SIZE_POS_st9,   		// モータ駆動開始
	CMPL_TMP_SIZE_POS_st10,   		// SizePositionへ移動完了



	// ノーマルタンピング動作
	CMPL_TMP_NORMAL_TAMPING = 0x0300,
	CMPL_TMP_NORMAL_TAMPING_st1, 	// 動作開始遅延時間設定
	CMPL_TMP_NORMAL_TAMPING_st2, 	// モータ駆動設定
	CMPL_TMP_NORMAL_TAMPING_st3, 	// モータ駆動開始
	CMPL_TMP_NORMAL_TAMPING_st4, 	// Tamping位置へ移動完了
	CMPL_TMP_NORMAL_TAMPING_st5, 	// Return方向へ切り替え
	CMPL_TMP_NORMAL_TAMPING_st6, 	// モータ駆動(Return)開始
	CMPL_TMP_NORMAL_TAMPING_st7, 	// NormalTamping完了
	CMPL_TMP_NORMAL_TAMPING_st8, 	// 次の動作待ち設定


	// イジェクトタンピング動作(SetLastTamping)
	CMPL_TMP_EJECT_TAMPING = 0x0400,
	CMPL_TMP_EJECT_TAMPING_st1, 	// 動作開始遅延時間設定
	CMPL_TMP_EJECT_TAMPING_st2, 	// モータ駆動設定
	CMPL_TMP_EJECT_TAMPING_st3, 	// モータ駆動開始
	CMPL_TMP_EJECT_TAMPING_st4, 	// Tamping位置へ移動完了
	CMPL_TMP_EJECT_TAMPING_st5, 	// Return方向へ切り替え
	CMPL_TMP_EJECT_TAMPING_st6, 	// モータ駆動(Return)開始
	CMPL_TMP_EJECT_TAMPING_st7, 	// Return動作完了
	CMPL_TMP_EJECT_TAMPING_st8, 	// SetLastTamping開始
	CMPL_TMP_EJECT_TAMPING_st9, 	// モータ駆動(SetLast)開始
	CMPL_TMP_EJECT_TAMPING_st10, 	// SetLast動作完了
	CMPL_TMP_EJECT_TAMPING_st11, 	// SetLast繰り返しチェック
	CMPL_TMP_EJECT_TAMPING_st12, 	// 次の動作待ち設定

	// オフセット動作
	CMPL_TMP_OFFSET = 0x0500,
	CMPL_TMP_OFFSET_st1, 		// 開始Timing調整
	CMPL_TMP_OFFSET_st2, 		// モータ駆動設定
	CMPL_TMP_OFFSET_st3, 		// モータ駆動開始
	CMPL_TMP_OFFSET_st4, 		// 安定時間タイマ設定
	CMPL_TMP_OFFSET_st5, 		// 次動作待ち

	// Left Tamperリターン動作
	CMPL_TMP_LEFT_RETURN = 0x0600,	
	CMPL_TMP_LEFT_RETURN_st1, 	// 動作開始Trigger待ち
	CMPL_TMP_LEFT_RETURN_st2, 	// タイマ設定
	CMPL_TMP_LEFT_RETURN_st3, 	// モータ駆動設定
	CMPL_TMP_LEFT_RETURN_st4, 	// モータ駆動開始
	CMPL_TMP_LEFT_RETURN_st5, 	// タイマ設定
	CMPL_TMP_LEFT_RETURN_st6, 	// 次動作待ち
	CMPL_TMP_LEFT_RETURN_st7, 	// SizePos開始(CR-008) 

	//Right Tamperリターン動作
	CMPL_TMP_RIGHT_RETURN = 0x0700,
	CMPL_TMP_RIGHT_RETURN_st1, 		// 駆動方向判断
	CMPL_TMP_RIGHT_RETURN_st2_CW, 	// 動作開始Trigger待ち(LeftOffset)
	CMPL_TMP_RIGHT_RETURN_st3_CW, 	//	タイマ設定(LeftOffset)
	CMPL_TMP_RIGHT_RETURN_st4_CW, 	// モータ駆動設定(LeftOffset)
	CMPL_TMP_RIGHT_RETURN_st2_CCW, 	// 動作開始Trigger待ち(RightOffset)
	CMPL_TMP_RIGHT_RETURN_st3_CCW, 	// タイマ設定(RightOffset)
	CMPL_TMP_RIGHT_RETURN_st4_CCW, 	// モータ駆動設定(RightOffset)
	CMPL_TMP_RIGHT_RETURN_st5, 		// モータ駆動開始
	CMPL_TMP_RIGHT_RETURN_st6, 		// タイマ設定
	CMPL_TMP_RIGHT_RETURN_st7, 		// 次動作待ち
	CMPL_TMP_RIGHT_RETURN_st8, 	// SizePos開始(CR-008) 

	// オープン動作(Nigasi)
	CMPL_TMP_OPEN = 0x0800,
	CMPL_TMP_OPEN_st1, 		// モータ駆動設定
	CMPL_TMP_OPEN_st2, 		// モータ駆動開始
	CMPL_TMP_OPEN_st3, 		// タイマ設定
	CMPL_TMP_OPEN_st4, 		// 次動作待ち
	CMPL_TMP_OPEN_st5,		//

	// Pre-NormalTamping
	CMPL_TMP_PRE_NORMAL_TAMPING = 0x0900,

	// MissRegi防止動作 (V3.5.95)
	CMPL_TMP_MISSREGI = 0x0A00,
	CMPL_TMP_MISSREGI_st1,  //MissRegi_動作Timer設定
	CMPL_TMP_MISSREGI_st2, // MissRegi_指定パルス出力
	CMPL_TMP_MISSREGI_st3, // MissRegi_後励磁設定
	CMPL_TMP_MISSREGI_st4, // MissRegi_完了	

	// Number of Module Action
	CMPL_TMP_ACT_NUM,

} ECMPL_TMP_Action;

#define CMPL_TMP_ACTION_MASK 0xFF00

#endif //#if !defined(ECMPL_TMP_Action_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

