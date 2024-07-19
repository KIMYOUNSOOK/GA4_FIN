// File Name: DD_INT.c
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 13.10.15
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev005 Base)
// Revision 000

#pragma interrupt INTP0 DD_INT0_interrupt
#pragma interrupt INTP1 DD_INT1_interrupt
#pragma interrupt INTP2 DD_INT2_interrupt
#pragma interrupt INTP3 DD_INT3_interrupt
#pragma interrupt INTP6 DD_INT6_interrupt
#pragma interrupt INTP7 DD_INT7_interrupt
#pragma interrupt INTP8 DD_INT8_interrupt
#pragma interrupt INTP9 DD_INT9_interrupt
#pragma interrupt INTP10 DD_INT10_interrupt
#pragma interrupt INTP11 DD_INT11_interrupt

#include "Common.h"
#include "DD_Common.h"
#include "DD_ResourceAssign.h"
#include "DD_Event.h"
#include "LIB_Lock.h"
#include "DD_INT.h"

#define DD_INT_MAX_CALLBACK_NUM		2

// Data
static DD_CALLBACK_TYPE intInfo[DD_INT_NUM][2][DD_INT_MAX_CALLBACK_NUM];	//RISEとFALLで２つ持つ

// ProtoType
// static void Callback(UC intCH, UC edge);
void Callback(UC intCH, UC edge);
static void EnableINT(UC sensorID, UC edge);

static UC selectedTimeMeasuring[DD_INT_NUM] = {EDGE_NONE, EDGE_NONE, EDGE_NONE, EDGE_NONE};
static UC is1stEnd = false;
static SS time1st = 0;

//=============================================================================
// Function Name : DD_INT_Init
// Description   : CPU外部割込みの初期化
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
void DD_INT_Init(void)
{
    EGN0 = 0;	 // エッジ検出禁止
    EGP0 = 0;	 // エッジ検出禁止

    PIF0 = 0U;    /* clear INTP0 interrupt flag */
    PMK0 = 0U;    /* enable INTP0 operation */
    PIF1 = 0U;    /* clear INTP1 interrupt flag */
    PMK1 = 0U;    /* enable INTP1 operation */
    PIF2 = 0U;    /* clear INTP2 interrupt flag */
    PMK2 = 0U;    /* enable INTP2 operation */
    PIF3 = 0U;    /* clear INTP3 interrupt flag */
    PMK3 = 0U;    /* enable INTP3 operation */
    PIF4 = 0U;    /* clear INTP4 interrupt flag */
    PMK4 = 0U;    /* enable INTP4 operation */
    PIF5 = 0U;    /* clear INTP5 interrupt flag */
    PMK5 = 0U;    /* enable INTP5 operation */
    PIF6 = 0U;    /* clear INTP6 interrupt flag */
    PMK6 = 0U;    /* enable INTP6 operation */
    PIF7 = 0U;    /* clear INTP7 interrupt flag */
    PMK7 = 0U;    /* enable INTP7 operation */
    PIF8 = 0U;    /* clear INTP8 interrupt flag */
    PMK8 = 0U;    /* enable INTP8 operation */
    PIF9 = 0U;    /* clear INTP9 interrupt flag */
    PMK9 = 0U;    /* enable INTP9 operation */
    PIF10 = 0U;    /* clear INTP10 interrupt flag */
    PMK10 = 0U;    /* enable INTP10 operation */
    PIF11 = 0U;    /* clear INTP11 interrupt flag */
    PMK11 = 0U;    /* enable INTP11 operation */

    /* Set INTP0 level 1 priority */
    PPR10 = 0U;
    PPR00 = 1U;
    /* Set INTP1 level 1 priority */
    PPR11 = 0U;
    PPR01 = 1U;
    /* Set INTP2 level 1 priority */
    PPR12 = 0U;
    PPR02 = 1U;
    /* Set INTP3 level 1 priority */
    PPR13 = 0U;
    PPR03 = 1U;
    /* Set INTP6 level 1 priority */
    PPR16 = 0U;
    PPR06 = 1U;
    /* Set INTP7 level 1 priority */
    PPR17 = 0U;
    PPR07 = 1U;
    /* Set INTP8 level 1 priority */
    PPR18 = 0U;
    PPR08 = 1U;
    /* Set INTP9 level 1 priority */
    PPR19 = 0U;
    PPR09 = 1U;
    /* Set INTP10 level 1 priority */
    PPR110 = 0U;
    PPR010 = 1U;
    /* Set INTP11 level 1 priority */
    PPR111 = 0U;
    PPR011 = 1U;

    //intInfo[INT_CHANNEL_NUM][2][INT_ENTRY_MAX]を初期化
}

//=============================================================================
// Function Name : DD_INT_EntryCallback
// Description   : 指定された割込みチャネルにコールバック関数を登録する
// Parameter     : intCH : 割込みチャネル
//				 : edge	 : エッジ(RISE/FALL)
//				 : func  : コールバック関数のポインタ
//				 : arg	 : コールバック関数の引数データのポインタ
// Return        : index : 登録したコールバック関数配列のINDEX(0～2)、3の場合は未使用
// Note          : なし
//=============================================================================
UC DD_INT_EntryCallback(UC intCH, UC edge, void (*func)(void*), void* arg)
{
	UC index = DD_INT_MAX_CALLBACK_NUM;					// 引数が範囲外の場合はINDEX=3を戻すのでINT_ENTRY_MAX(3)で初期化する
	UC key = 0;
	UC mask = 0;

	if(intCH < DD_INT_NUM && ((edge == RISE) || (edge == FALL))) {
		for(index = 0; index < DD_INT_MAX_CALLBACK_NUM; index++) {
			if(intInfo[intCH][edge][index].pFunc == 0) {
				key = LIB_GetKey();
				LIB_Lock();
				intInfo[intCH][edge][index].pFunc = func;
				intInfo[intCH][edge][index].arg = arg;

				// EGP0またはEGN0のマスクを作成
				if( intCH < 8 ) {
					mask = (UC)(0x01 << intCH);			// intCHは0～3なのでかならずUCの方に収まる
					if( edge == RISE) {
						EGP0 |= mask;					// 立ち上がり検出設定
					} else {
						EGN0 |= mask;					// 立ち下がり検出設定
					}
					LIB_Unlock(key);
					break;

				} else {
					mask = (UC)(0x01 << (intCH - 8));
					if( edge == RISE) {
						EGP1 |= mask;					// 立ち上がり検出設定
					} else {
						EGN1 |= mask;					// 立ち下がり検出設定
					}
					LIB_Unlock(key);
					break;
				}
			}
		}
	}
	return index;
}

//=============================================================================
// Function Name : DD_INT_CancelCallback
// Description   : 指定された割込みチャネルにコールバック関数を登録を解除
// Parameter     : intCH	"割込みチャネル"
//				 : edge		"エッジ(RISE/FALL)"
//				 : func		"コールバック関数のポインタ"
//				 : arg		"コールバック関数の引数データのポインタ"
// Return        : なし
// Note          : なし
//=============================================================================
void DD_INT_CancelCallback(UC intCH, UC edge, UC index)
{
	if((intCH < DD_INT_NUM)
	&& ((edge == RISE) || (edge == FALL))
	&& (index < DD_INT_MAX_CALLBACK_NUM)) {
		intInfo[intCH][edge][index].pFunc = 0;
		intInfo[intCH][edge][index].arg = 0;
	}
}

//=============================================================================
// Function Name : DD_INT_GetCurrentLevel
// Description   : 指定された割込みチャネルの現在のレベルを取得する
// Parameter     : intCH	"割込みチャネル"
// Return        : level	"現在のレベル(ON/OFF)"
// Note          : なし
//=============================================================================
UC DD_INT_GetCurrentLevel(UC intCH)
{
	UC ret = OFF;

	switch (intCH) {
	case DD_INT_xxxx1_SENSOR:				ret = DD_INTP0_PORT; 	break;		// DD_INTP0 = 0
	case DD_INT_COMPILE_EXIT_SENSOR:		ret = DD_INTP1_PORT; 	break;		// DD_INTP1 = 1
	case DD_INT_STAPLE_HOME_SENSOR:			ret = DD_INTP2_PORT; 	break;		// DD_INTP2 = 2
	case DD_INT_IOT_EXIT_SENSOR:			ret = DD_INTP3_PORT; 	break;		// DD_INTP3 = 3
	case DD_INT_STACKER_NO_PAPER_SENSOR:	ret = DD_INTP6_PORT; 	break;		// DD_INTP6 = 4
	case DD_INT_SUB_PADDLE_HOME_SENSOR:  	ret = DD_INTP7_PORT; 	break;		// DD_INTP7 = 5
	case DD_INT_RIGHT_TAMPER_HOME_SENSOR:	ret = DD_INTP8_PORT; 	break;		// DD_INTP8 = 6
	case DD_INT_LEFT_TAMPER_HOME_SENSOR:	ret = DD_INTP9_PORT; 	break;		// DD_INTP9 = 7
	case DD_INT_SET_CLAMP_HOME_SENSOR:		ret = DD_INTP10_PORT;	break;		// DD_INTP10 = 8
	case DD_INT_xxxx2_SENSOR:				ret = DD_INTP11_PORT;	break;		// DD_INTP11 = 9
	case DD_INT_EJECT_HOME_SENSOR:			ret = DD_INTP12_PORT; 	break;		//
	default:						break;
	}
	return ret;
}


//=============================================================================
// Function Name : DD_INT0_interrupt
// Description   : 登録されているコールバック関数を呼ぶ
// Parameter     : なし
// Return        : なし
// Note          : DD_INTP0_PORTの定義はDD_ResourceAssign.h
//=============================================================================
__interrupt static void DD_INT0_interrupt(void)
{
	if(DD_INTP0_PORT == ON) {									// RISE(ON) Edge
		Callback(0x00, RISE);
		EGP0 &= ~0x01;
	}
	else {														// FALL(OFF) Edge
		Callback(0x00, FALL);
		EGN0 &= ~0x01;
	}
    PIF0 = 0U;    /* clear INTP0 interrupt flag */
}

//=============================================================================
// Function Name : DD_INT1_interrupt
// Description   : 登録されているコールバック関数を呼ぶ
// Parameter     : なし
// Return        : なし
// Note          : DD_INTP1_PORTの定義はDD_ResourceAssign.h
//=============================================================================
__interrupt static void DD_INT1_interrupt(void)
{
	if(DD_INTP1_PORT == ON) {								   	// RISE(ON) Edge
		Callback(0x01, RISE);
		EGP0 &= ~0x02;
	}
	else {														// FALL(OFF) Edge
		Callback(0x01, FALL);
		EGN0 &= ~0x02;
	}
    PIF1 = 0U;    /* clear INTP0 interrupt flag */
}

//=============================================================================
// Function Name : DD_INT2_interrupt
// Description   : 登録されているコールバック関数を呼ぶ
// Parameter     : なし
// Return        : なし
// Note          : DD_INTP2_PORTの定義はDD_ResourceAssign.h
//=============================================================================
__interrupt static void DD_INT2_interrupt(void)
{
	if(DD_INTP2_PORT == ON) {									// RISE(ON) Edge
		Callback(0x02, RISE);
		EGP0 &= ~0x04;
	}
	else {														// FALL(OFF) Edge
		Callback(0x02, FALL);
		EGN0 &= ~0x04;
	}
    PIF2 = 0U;    /* clear INTP0 interrupt flag */
}

//=============================================================================
// Function Name : DD_INT3_interrupt
// Description   : 登録されているコールバック関数を呼ぶ
// Parameter     : なし
// Return        : なし
// Note          : DD_INTP2_PORTの定義はDD_ResourceAssign.h
//=============================================================================
__interrupt static void DD_INT3_interrupt(void)
{
	if(DD_INTP3_PORT == ON) {									// RISE(ON) Edge
		Callback(0x03, RISE);
		EGP0 &= ~0x08;
	}
	else {														// FALL(OFF) Edge
		Callback(0x03, FALL);
		EGN0 &= ~0x08;
	}
    PIF3 = 0U;    /* clear INTP0 interrupt flag */
}

//=============================================================================
// Function Name : DD_INT6_interrupt
// Description   : 登録されているコールバック関数を呼ぶ
// Parameter     : なし
// Return        : なし
// Note          : DD_INTP2_PORTの定義はDD_ResourceAssign.h
//=============================================================================
__interrupt static void DD_INT6_interrupt(void)
{
	if(DD_INTP6_PORT == ON) {									// RISE(ON) Edge
		Callback(0x06, RISE);
		EGP0 &= ~0x40;
	}
	else {														// FALL(OFF) Edge
		Callback(0x06, FALL);
		EGN0 &= ~0x40;
	}
    PIF6 = 0U;    /* clear INTP0 interrupt flag */
}

//=============================================================================
// Function Name : DD_INT7_interrupt
// Description   : 登録されているコールバック関数を呼ぶ
// Parameter     : なし
// Return        : なし
// Note          : DD_INTP2_PORTの定義はDD_ResourceAssign.h
//=============================================================================
__interrupt static void DD_INT7_interrupt(void)
{
	if(DD_INTP7_PORT == ON) {									// RISE(ON) Edge
		Callback(0x07, RISE);
		EGP0 &= ~0x80;
	}
	else {														// FALL(OFF) Edge
		Callback(0x07, FALL);
		EGN0 &= ~0x80;
	}
    PIF7 = 0U;    /* clear INTP0 interrupt flag */
}

//=============================================================================
// Function Name : DD_INT8_interrupt
// Description   : 登録されているコールバック関数を呼ぶ
// Parameter     : なし
// Return        : なし
// Note          : DD_INTP2_PORTの定義はDD_ResourceAssign.h
//=============================================================================
__interrupt static void DD_INT8_interrupt(void)
{
	if(DD_INTP8_PORT == ON) {									// RISE(ON) Edge
		Callback(0x08, RISE);
		EGP1 &= ~0x01;
	}
	else {														// FALL(OFF) Edge
		Callback(0x08, FALL);
		EGN1 &= ~0x01;
	}
    PIF8 = 0U;    /* clear INTP0 interrupt flag */
}

//=============================================================================
// Function Name : DD_INT9_interrupt
// Description   : 登録されているコールバック関数を呼ぶ
// Parameter     : なし
// Return        : なし
// Note          : DD_INTP2_PORTの定義はDD_ResourceAssign.h
//=============================================================================
__interrupt static void DD_INT9_interrupt(void)
{
	if(DD_INTP9_PORT == ON) {									// RISE(ON) Edge
		Callback(0x09, RISE);
		EGP1 &= ~0x02;
	}
	else {														// FALL(OFF) Edge
		Callback(0x09, FALL);
		EGN1 &= ~0x02;
	}
    PIF9 = 0U;    /* clear INTP0 interrupt flag */
}

//=============================================================================
// Function Name : DD_INT10_interrupt
// Description   : 登録されているコールバック関数を呼ぶ
// Parameter     : なし
// Return        : なし
// Note          : DD_INTP2_PORTの定義はDD_ResourceAssign.h
//=============================================================================
__interrupt static void DD_INT10_interrupt(void)
{
	if(DD_INTP10_PORT == ON) {									// RISE(ON) Edge
		Callback(0x0A, RISE);
		EGP1 &= ~0x04;
	}
	else {														// FALL(OFF) Edge
		Callback(0x0A, FALL);
		EGN1 &= ~0x04;
	}
    PIF10 = 0U;    /* clear INTP0 interrupt flag */
}

//=============================================================================
// Function Name : DD_INT11_interrupt
// Description   : 登録されているコールバック関数を呼ぶ
// Parameter     : なし
// Return        : なし
// Note          : DD_INTP2_PORTの定義はDD_ResourceAssign.h
//=============================================================================
__interrupt static void DD_INT11_interrupt(void)
{
	if(DD_INTP11_PORT == ON) {									// RISE(ON) Edge
		Callback(0x0B, RISE);
		EGP1 &= ~0x08;
	}
	else {														// FALL(OFF) Edge
		Callback(0x0B, FALL);
		EGN1 &= ~0x08;
	}
    PIF11 = 0U;    /* clear INTP0 interrupt flag */
}

//=============================================================================
// Function Name : Callback
// Description   : 登録されているコールバック関数を呼ぶ
// Parameter     : intCH	"割込みチャネル(0~3)"
//				 : edge		"エッジ(RISE/FALL)"
// Return        : なし
// Note          : なし
//=============================================================================
// static void Callback(UC intCH, UC edge)
void Callback(UC intCH, UC edge)
{
	UC index = 0;
	DD_CALLBACK_TYPE* pCB;

	pCB = &intInfo[intCH][edge][0];
	for(index = 0; index < DD_INT_MAX_CALLBACK_NUM; index++) {
		if(pCB->pFunc != 0 ) {
			pCB->pFunc(pCB->arg);								// 登録されているCallback関数呼び出し

			pCB->pFunc = 0;										// Callback初期化
			// pCB->arg = 0;									// argは関数有無の判別に使用されない&&毎回必ず再設定されるため初期化不要
		}
		pCB++;
	}
}

//=============================================================================
// Function Name : EnableINT
// Description   : 指定された割込みチャネルにコールバック関数を登録する
// Parameter     : intCH	"割込みチャネル"
//				 : edge		"エッジ(RISE/FALL)"
// Return        : なし
// Note          : なし
//=============================================================================
static void EnableINT(UC intCH, UC edge)
{
	UC key = 0;
	UC mask = 0;

	if((intCH < DD_INT_NUM) && ((edge == RISE) || (edge == FALL))) {
		key = LIB_GetKey();
		LIB_Lock();

		// EGP0またはEGN0のマスクを作成
		mask = (UC)(0x01 << intCH);			// intCHは0～3なのでかならずUCの型に収まる

		if(edge == RISE) {
			EGP0 |= mask;					// 立ち上がり検出設定
		}
		else {
			EGN0 |= mask;					// 立ち下がり検出設定
		}
		LIB_Unlock(key);
	}
}

