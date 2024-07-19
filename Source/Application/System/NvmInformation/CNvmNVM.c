// File Name:	CNvmNVM.c
// Description:	Implimentation File of NVM
// Engineer:	Xxxx Xxxx
// Last Edit:	XX.XX.XX
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CNvmNVM.h"
#include "CNvmInformer.h"

#include "CCfgDevContConfiguration.h"
#include "CCfgInformer.h"

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
// Function Name : CNvmNVM_Constructor
// Description   : コンストラクタ
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CNvmNVM_Constructor(CNvmNVM* pThis)
{
	/* 何もしない */
	/* NVMの初期化はPowerOnInitializeにて行う */
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_GetValue
// Description   : NVM値取得
// Parameter     : pThis (インスタンスポインタ)
// Return        : NVM値
///////////////////////////////////////////////////////////////////
UL CNvmNVM_GetValue(CNvmNVM* pThis)
{
	
	switch ( CNvmNVM_GetDataType(pThis) ) {
	case eCdiFinDataType_SignChar:
		return (UL)(*((SC*)(pThis->mpSpec->address)));
	case eCdiFinDataType_UnsignChar:
		return (UL)(*(pThis->mpSpec->address));
	case eCdiFinDataType_SignShort:
		return (UL)(*((SS*)(pThis->mpSpec->address)));
	case eCdiFinDataType_UnsignShort:
		return (UL)(*((US*)(pThis->mpSpec->address)));
	case eCdiFinDataType_SignLong:
		return (UL)(*((SL*)(pThis->mpSpec->address)));
	case eCdiFinDataType_UnsignLong:
		return *((UL*)(pThis->mpSpec->address));
	default:
		return 0;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_GetSignedValue
// Description   : NVM値取得
// Parameter     : pThis (インスタンスポインタ)
// Return        : NVM値
///////////////////////////////////////////////////////////////////
SL CNvmNVM_GetSignedValue(CNvmNVM* pThis)
{
	return (SL)CNvmNVM_GetValue(pThis);
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_SetValue
// Description   : NVM値設定処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : NVM_RESULT_OK 成功、NVM_RESULT_NG 失敗
// Note          : 範囲内チェックは行わない
///////////////////////////////////////////////////////////////////
SC CNvmNVM_SetValue(CNvmNVM* pThis, UL value)
{
	
	switch ( CNvmNVM_GetDataType(pThis) ) {
	case eCdiFinDataType_SignChar:
		if( *((SC*)(pThis->mpSpec->address)) != (SC)value ) {
			*((SC*)(pThis->mpSpec->address)) = (SC)value;
			CNvmInformer_SetValueChange();
		}
		break;
	case eCdiFinDataType_UnsignChar:
		if( *(pThis->mpSpec->address) != (UC)value ) {
			*(pThis->mpSpec->address) = (UC)value;
			CNvmInformer_SetValueChange();
		}
		break;
	case eCdiFinDataType_SignShort:
		if( *((SS*)(pThis->mpSpec->address)) != (SS)value ) {
			*((SS*)(pThis->mpSpec->address)) = (SS)value;
			CNvmInformer_SetValueChange();
		}
		break;
	case eCdiFinDataType_UnsignShort:
		if( *((US*)(pThis->mpSpec->address)) != (US)value ) {
			*((US*)(pThis->mpSpec->address)) = (US)value;
			CNvmInformer_SetValueChange();
		}
		break;
	case eCdiFinDataType_SignLong:
		if( *((SL*)(pThis->mpSpec->address)) != (SL)value ) {
			*((SL*)(pThis->mpSpec->address)) = (SL)value;
			CNvmInformer_SetValueChange();
		}
		break;
	case eCdiFinDataType_UnsignLong:
		if( *((UL*)(pThis->mpSpec->address)) != value ) {
			*((UL*)(pThis->mpSpec->address)) = value;
			CNvmInformer_SetValueChange();
		}
		break;
	default:
		return NVM_RESULT_NG;
	}
	return NVM_RESULT_OK;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_Increament
// Description   : NVM値をインクリメント
// Parameter     : pThis (インスタンスポインタ)
// Return        : NVM_RESULT_OK 成功、NVM_RESULT_NG 失敗
// Note          : 範囲内チェックは行わない
///////////////////////////////////////////////////////////////////
SC CNvmNVM_Increment(CNvmNVM* pThis)
{
	UL value = CNvmNVM_GetValue(pThis);
	
	switch ( CNvmNVM_GetDataType(pThis) ) {
	case eCdiFinDataType_SignChar:
	case eCdiFinDataType_SignShort:
	case eCdiFinDataType_SignLong:
		if ( (SL)value >= (SL)CNvmNVM_GetMaxValue(pThis) ) {
			return NVM_RESULT_NG;
		}
		value = (UL)((SL)(value)+1);
		break;
	case eCdiFinDataType_UnsignChar:
	case eCdiFinDataType_UnsignShort:
	case eCdiFinDataType_UnsignLong:
		if ( value >= CNvmNVM_GetMaxValue(pThis) ) {
			return NVM_RESULT_NG;
		}
		value++;
		break;
	default:
		return NVM_RESULT_NG;
	}
	return CNvmNVM_SetValue(pThis, value);
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_Initialize
// Description   : NVM初期化処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : NVM_RESULT_OK 成功、NVM_RESULT_NG 失敗
// Note          : Marketごとの初期値を設定する
///////////////////////////////////////////////////////////////////
SC CNvmNVM_Initialize(CNvmNVM* pThis)
{
	UL value;
	CCfgDevContConfiguration* config = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);
	
	/* Marketごとの初期値を設定 */
	switch (CCfgDevContConfiguration_GetMarketRegion(config)) {
	case eCdiFinMarket_FX:
		value = pThis->mpSpec->initialValueFx;
		break;
	case eCdiFinMarket_AP:
		value = pThis->mpSpec->initialValueAp;
		break;
    case eCdiFinMarket_XC:
		value = pThis->mpSpec->initialValueXc;
		break;
    case eCdiFinMarket_XE:
		value = pThis->mpSpec->initialValueXe;
		break;
    case eCdiFinMarket_SA:
		value = pThis->mpSpec->initialValueSa;
		break;
	default:
		return NVM_RESULT_NG;
	}
	return CNvmNVM_SetValue(pThis, value);
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_GetDataType
// Description   : NVMの型を取得する
// Parameter     : pThis (インスタンスポインタ)
// Return        : 型
///////////////////////////////////////////////////////////////////
enum ECdiFinDataType CNvmNVM_GetDataType(CNvmNVM* pThis)
{
	return (enum ECdiFinDataType)(pThis->mpSpec->dataType);
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_GetMinValue
// Description   : NVMの最小値を取得する
// Parameter     : pThis (インスタンスポインタ)
// Return        : 最小値
///////////////////////////////////////////////////////////////////
UL CNvmNVM_GetMinValue(CNvmNVM* pThis)
{
	return pThis->mpSpec->minValue;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_GetMaxValue
// Description   : NVMの最大値を取得する
// Parameter     : pThis (インスタンスポインタ)
// Return        : 最大値
///////////////////////////////////////////////////////////////////
UL CNvmNVM_GetMaxValue(CNvmNVM* pThis)
{
	return pThis->mpSpec->maxValue;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_IsReadAllowed
// Description   : Read許可/禁止を返す
// Parameter     : pThis (インスタンスポインタ)
// Return        : Read許可/禁止
///////////////////////////////////////////////////////////////////
UC CNvmNVM_IsReadAllowed(CNvmNVM* pThis)
{
	return pThis->mpSpec->readAllowed;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_IsWriteAllowed
// Description   : Write許可/禁止を返す
// Parameter     : pThis (インスタンスポインタ)
// Return        : Write許可/禁止
///////////////////////////////////////////////////////////////////
UC CNvmNVM_IsWriteAllowed(CNvmNVM* pThis)
{
	return pThis->mpSpec->writeAllowed;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_IsFactoryInitAllowed
// Description   : 初期化許可/禁止を返す
// Parameter     : pThis (インスタンスポインタ)
// Return        : 初期化許可/禁止
///////////////////////////////////////////////////////////////////
UC CNvmNVM_IsFactoryInitAllowed(CNvmNVM* pThis)
{
	return pThis->mpSpec->initAllowed;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_IsOutOfRange
// Description   : 範囲外チェック
// Parameter     : pThis (インスタンスポインタ)
// Return        : true 範囲外、false 範囲内
///////////////////////////////////////////////////////////////////
bool CNvmNVM_IsOutRange(CNvmNVM* pThis)
{
	switch ( CNvmNVM_GetDataType(pThis) ) {
	case eCdiFinDataType_SignChar:
	case eCdiFinDataType_SignShort:
	case eCdiFinDataType_SignLong:
		return ( (SL)CNvmNVM_GetValue(pThis) > (SL)CNvmNVM_GetMaxValue(pThis) ) ||
			   ( (SL)CNvmNVM_GetValue(pThis) < (SL)CNvmNVM_GetMinValue(pThis) ) ;
	case eCdiFinDataType_UnsignChar:
	case eCdiFinDataType_UnsignShort:
	case eCdiFinDataType_UnsignLong:
		return ( CNvmNVM_GetValue(pThis) > CNvmNVM_GetMaxValue(pThis) ) ||
			   ( CNvmNVM_GetValue(pThis) < CNvmNVM_GetMinValue(pThis) ) ;
	default:
		return true;
	}
}

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
