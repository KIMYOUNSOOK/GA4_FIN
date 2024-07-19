// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_MessageType.h
/// @author  Tsuyoshi Seto
/// @date    14/04/02
/// @time    16:37:26
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆
//  本ヘッダーファイルはイベント管理リストから自動生成されます。（プロダクト･ユニーク）
// ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆

#ifndef MSP_MessageType_h
#define MSP_MessageType_h


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************
enum ESeqPowerDownType {
	SEQ_PWR_DOWN_TYPE_PWR_OFF,
	SEQ_PWR_DOWN_TYPE_SLEEP
} ;

struct MsgComponentControl {
	US chain ;									//Chain No.(1～999) 
	US link ;									//Link No.(1～999)
} ;

struct MsgStaplePositioning {
	UC sheetNo ;								// Finisher内で管理しているシート番号
	UC previousSheetNo ;						// Finisher内で管理している前セットシート番号
} ;

struct SDeviceStatusUpdateInf {
	US statusNo ;
	US  value ;
} ;

enum ETmpHoldReserve {
	TMP_HOLD_SHEET_UNKNOWN ,
	TMP_HOLD_SHEET_RESERVED,
	TMP_HOLD_SHEET_PASS
} ;

enum EStapleFinish {
	UNSTAPLE             ,						// アンステープル
	FRONT_STRAIGHT_STAPLE,						// フロントストレイトステープル
	FRONT_CORNER_STAPLE  ,						// フロントコーナーステープル
	REAR_STRAIGHT_STAPLE ,						// リアストレイトステープル
	REAR_CORNER_STAPLE   ,						// リアコーナーステープル
	DUAL_STAPLE          ,						// デュアルステープル
	FOUR_POSITION_STAPLE ,						// 4ポジションステープル
	CENTER_STAPLE        ,						// センターステープル
	NUM_OF_STAPLE								// Number of staple finish kind
} ;


#endif          // MSP_MessageType_h

// ****************************************************************  [ MSP_MessageType.h : EOF ]  *****************************************************************
