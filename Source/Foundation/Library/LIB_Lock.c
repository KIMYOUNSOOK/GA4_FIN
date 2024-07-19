// File Name: LIB_Lock.c
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 13.10.15
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define LIB_Lock_c

#include "Common.h"
#include "LIB_Lock.h"


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

// ===========================  <<< マクロ定義 >>>  =========================== 

// ----- レジスター定義 -----
#define REG_PSW	(*(volatile UC*) 0xFFFFA) ;													// PSWレジスタ (Program Status Word Register)
#define IE_BIT	0x80																		// 割り込み要求受け付け許可・禁止ビット (1:許可/0:禁止)


// ************************************************************  <<<<< Program Code Area >>>>>  ***************************************************************

//============================================================================= 
// Function Name : LIB_GetKey()
// Description   : 割り込み禁止用の鍵番号を取得する（PSWの状態を取得）
// Parameter     : なし
// Parameter     : なし
// Return        : -
//=============================================================================
UC LIB_GetKey(void)
{
	UC lockKey = REG_PSW ;																	// PSWレジスタの内容を返す

	return lockKey ;
}


//============================================================================= 
// Function Name : LIB_Lock()
// Description   : 割り込み禁止状態にする
// Parameter     : なし
// Parameter     : なし
// Return        : -
//=============================================================================
void LIB_Lock(void)
{
	DI() ;																					// 割り込みを禁止する
}


//============================================================================= 
// Function Name : LIB_Unlock()
// Description   : 割り込み禁止状態を解除する
// Parameter     : key			"割り込み禁止鍵番号"
// Parameter     : なし
// Return        : -
//=============================================================================
void LIB_Unlock(UC key)
{
	if ( (key & IE_BIT) != 0 ) {															// 割り込み禁止処理コール前の状態が“割り込み許可”
		EI() ;																				// ⇒ 割り込み禁止状態を解除する
	}
}


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev000 Base)
//
//


// ****************************************************************  [ LIB_Lock.c : EOF ]  ********************************************************************
