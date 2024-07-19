// File Name:	CHfsiHFSI.c
// Description:	Implimentation File of HFSI
// Engineer:	Xxxxxx Xxxxxx
// Last Edit:	XX.XX.XX
// Revision:	XX
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CHfsiHFSI.h"

#include "CHfsiInformer.h"
#include "HfsiRam.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_Constructor
// Description   : コンストラクタ
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CHfsiHFSI_Constructor(CHfsiHFSI* pThis)
{
	/* 何もしない */
	/* HFSIの初期化はPowerOnInitializeにて行う */
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetChainNo
// Description   : ChainNoを返す
// Parameter     : pThis (インスタンスポインタ)
// Return        : ChainNo
///////////////////////////////////////////////////////////////////
US CHfsiHFSI_GetChainNo(CHfsiHFSI* pThis)
{
	return pThis->mpSpec->chain;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetLinkNo
// Description   : LinkNoを返す
// Parameter     : pThis (インスタンスポインタ)
// Return        : LinkNo
///////////////////////////////////////////////////////////////////
US CHfsiHFSI_GetLinkNo(CHfsiHFSI* pThis)
{
	return pThis->mpSpec->link;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetValue
// Description   : HFSI現在カウンタを返す
// Parameter     : pThis (インスタンスポインタ)
// Return        : 現在カウンタ
///////////////////////////////////////////////////////////////////
UL CHfsiHFSI_GetValue(CHfsiHFSI* pThis)
{
	return pThis->mpSpec->address->currentCounter;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetSignedValue
// Description   : HFSI現在カウンタを返す
// Parameter     : pThis (インスタンスポインタ)
// Return        : 現在カウンタ
///////////////////////////////////////////////////////////////////
SL CHfsiHFSI_GetSignedValue(CHfsiHFSI* pThis)
{
	return (SL)CHfsiHFSI_GetValue(pThis);
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_SetValue
// Description   : HFSI現在カウンタの更新
// Parameter     : pThis (インスタンスポインタ)
// Return        : HFSI_RESULT_OK 成功、HFSI_RESULT_NG 失敗
///////////////////////////////////////////////////////////////////
SC CHfsiHFSI_SetValue(CHfsiHFSI* pThis, UL value)
{
	/* HFSI Save実施判断の為、カウンタ値に変更があるかをチェックする */
	if ( CHfsiHFSI_GetValue(pThis) != value ) {
		pThis->mpSpec->address->currentCounter = value;
		CHfsiInformer_SetValueChange();
	}
    return HFSI_RESULT_OK;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_Increment
// Description   : HFSI現在カウンタを+1する
// Parameter     : pThis (インスタンスポインタ)
// Return        : HFSI_RESULT_OK 成功、HFSI_RESULT_NG 失敗
///////////////////////////////////////////////////////////////////
SC CHfsiHFSI_Increment(CHfsiHFSI* pThis)
{
	/* 現在カウンタが最大値 */
	if ( CHfsiHFSI_GetValue(pThis) >= CHfsiHFSI_GetMaxValue(pThis) ) {
		return HFSI_RESULT_NG;
	}
	/* 現在カウンタが最大値以外 */
	else {
		pThis->mpSpec->address->currentCounter++;
		CHfsiInformer_SetValueChange();
		return HFSI_RESULT_OK;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetDataType
// Description   : HFSIのデータ型を返す
// Parameter     : pThis (インスタンスポインタ)
// Return        : データ型
///////////////////////////////////////////////////////////////////
enum ECdiFinDataType CHfsiHFSI_GetDataType(CHfsiHFSI* pThis)
{
	return eCdiFinDataType_UnsignLong;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetMinValue
// Description   : HFSI最小値を返す
// Parameter     : pThis (インスタンスポインタ)
// Return        : 最小値
///////////////////////////////////////////////////////////////////
UL CHfsiHFSI_GetMinValue(CHfsiHFSI* pThis)
{
	return 0;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetMaxValue
// Description   : HFSI最大値を返す
// Parameter     : pThis (インスタンスポインタ)
// Return        : 最大値
///////////////////////////////////////////////////////////////////
UL CHfsiHFSI_GetMaxValue(CHfsiHFSI* pThis)
{
	return 0xFFFFFFFF;;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_Initialize
// Description   : HFSI情報初期化
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CHfsiHFSI_Initialize(CHfsiHFSI* pThis)
{
	UC i = 0;
	
	/* 現在カウンタ初期化 */
	CHfsiHFSI_SetValue(pThis, 0);
	
	/* 交換閾値を初期化 */
	CHfsiHFSI_InitializeLifeSpec(pThis);

	/* 過去3回分の交換時のカウンタ・理由を初期化 */
	for ( i = 0; i < HFSI_EXCHANGE_HISTORY_NUM; i++ ) {
		pThis->mpSpec->address->lastCounter[i] = 0;
		pThis->mpSpec->address->exchangeReason[i] = 0;
	}
	
	/* HFSIに変更があった事を登録 */
	CHfsiInformer_SetValueChange();
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_InitializeLifeSpec
// Description   : HFSI交換閾値初期化
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CHfsiHFSI_InitializeLifeSpec(CHfsiHFSI* pThis)
{
	CHfsiHFSI_SetLifeSpec(pThis, pThis->mpSpec->threshold);
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_Reset
// Description   : HFSI情報リセット
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CHfsiHFSI_Reset(CHfsiHFSI* pThis, UC exchangeReason)
{
	/* 現在のカウンタをリセット */
	CHfsiHFSI_SetValue(pThis, 0);
	
	/* 過去3回分の交換時カウンタを更新 */
	pThis->mpSpec->address->lastCounter[2] = pThis->mpSpec->address->lastCounter[1];
	pThis->mpSpec->address->lastCounter[1] = pThis->mpSpec->address->lastCounter[0];
	pThis->mpSpec->address->lastCounter[0] = pThis->mpSpec->address->currentCounter;
	
	/* 過去3回分の交換理由を更新 */
	pThis->mpSpec->address->exchangeReason[2] = pThis->mpSpec->address->exchangeReason[1];
	pThis->mpSpec->address->exchangeReason[1] = pThis->mpSpec->address->exchangeReason[0];
	pThis->mpSpec->address->exchangeReason[0] = exchangeReason;
	
	/* HFSIに変更があった事を登録 */
	CHfsiInformer_SetValueChange();
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetLastCount
// Description   : HFSI交換時カウンタを返す
// Parameter     : pThis (インスタンスポインタ)
//                 lastCount (何回前の交換時カウンタかを示す)
// Return        : 交換時のカウンタ
///////////////////////////////////////////////////////////////////
UL CHfsiHFSI_GetLastCount(CHfsiHFSI* pThis, UC lastCount)
{
	return pThis->mpSpec->address->lastCounter[lastCount];
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetExchangeReason
// Description   : HFSI交換理由を返す
// Parameter     : pThis (インスタンスポインタ)
//                 lastCount (何回前の交換時カウンタかを示す)
// Return        : 交換理由
///////////////////////////////////////////////////////////////////
UC CHfsiHFSI_GetExchangeReason(CHfsiHFSI* pThis, UC lastCount)
{
	return pThis->mpSpec->address->exchangeReason[lastCount];
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_SetLifeSpec
// Description   : HFSI交換閾値を更新する
// Parameter     : pThis (インスタンスポインタ)
//                 spec (交換閾値の更新値)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CHfsiHFSI_SetLifeSpec(CHfsiHFSI* pThis, UL spec)
{
	/* HFSI Save実施判断の為、カウンタ値に変更があるかをチェックする */
	if ( CHfsiHFSI_GetLifeSpec(pThis) != spec ) {
		pThis->mpSpec->address->lifeSpec = spec;
		CHfsiInformer_SetValueChange();
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetLifeSpec
// Description   : HFSI交換閾値を返す
// Parameter     : pThis (インスタンスポインタ)
// Return        : 交換閾値
///////////////////////////////////////////////////////////////////
UL CHfsiHFSI_GetLifeSpec(CHfsiHFSI* pThis)
{
	return pThis->mpSpec->address->lifeSpec;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetPartsName
// Description   : HFSIパーツ名称を返す
// Parameter     : pThis (インスタンスポインタ)
// Return        : パーツ名称
///////////////////////////////////////////////////////////////////
UC const* CHfsiHFSI_GetPartsName(CHfsiHFSI* pThis)
{
	return pThis->mpSpec->partsName;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetPartsNameSize
// Description   : HFSIパーツ名称のサイズを返す
// Parameter     : pThis (インスタンスポインタ)
// Return        : パーツ名称のサイズ
///////////////////////////////////////////////////////////////////
UC CHfsiHFSI_GetPartsNameSize(CHfsiHFSI* pThis)
{
	return pThis->mpSpec->partsNameSize;
}

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 15.03.30	| Makoto Tanabe		| Created this file
// 15.03.30	| Makoto Tanabe		| Release V1 (Revision 00)
// 15.04.02	| Makoto Tanabe		| Design Improvement [ スペル間違い(CHfsiHFSI_Increment)修正 ]
// 15.04.02	| Makoto Tanabe		| Design Improvement [ CHfsiHFSI_Increment()戻り値見直し ]
// 15.XX.XX	| Makoto Tanabe		| Release V2 (Revision 01)
//
