// File Name: LIB_Lock.h
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 13.10.15
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef LIB_Lock_h
#define LIB_Lock_h


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

// ===========================  <<< 関数プロトタイプ宣言 >>>  ===========================
UC LIB_GetKey(void) ;																		// 割り込み禁止鍵番号取得
void LIB_Lock(void) ;																		// 割り込み禁止処理
void LIB_Unlock(UC key) ;																	// 割り込み禁止解除処理

#endif			// LIB_Lock_h


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev000 Base)
//
//


// ****************************************************************  [ LIB_Lock.h : EOF ]  ********************************************************************
