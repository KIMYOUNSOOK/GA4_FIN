// File Name:	CNvmInformer.c
// Description:	Implimentation File of NVM Informer
// Engineer:	Yuichi Hoshino
// Last Edit:	16.02.26
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CNvmInformer.h"

#include "NvmInst.h"
#include "NvmRam.h"
#include "NvmAutoInitialize.h"
#include "NvmAllRamTable.h"

#include "CNmaHandler.h"

#include "DD_NVM.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- VersionUp/Down ---
#define NVM_VERSION_DOWN			(-1)		// VersionDown
#define NVM_VERSION_NO_CHANGE		0			// Version変化なし
#define NVM_VERSION_UP				1			// VersionUp

// --- NVM Change ---
#define NVM_NO_CHANGE				0			// NVM変化なし
#define NVM_CHANGE					1			// NVM変化あり

static UC g_NvmChange;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
SS CNvmInformer_CompareVersion(US currentMajor, US currentMinor, US previousMajor, US previousMinor);
void CNvmInformer_AutoInitialize(US previousMajor, US previousMinor);
void CNvmInformer_LoadNvmComplete(SS result);
void CNvmInformer_SaveNvmComplete(SS result);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CNvmInformer_Constructor()
{
	US nvmID;
	
	/* NVM初期化 */
	for ( nvmID = 0; nvmID < NVMID_NUM; nvmID++ ){
		CNvmNVM_Constructor(&(nvmAllData[nvmID]));
	}
	
	/* ローカルデータ初期化 */
	g_NvmChange = NVM_NO_CHANGE;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_LoadNvm
// Description   : NVMデータロード処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CNvmInformer_LoadNvm()
{
	DD_NVM_Load(DD_NVM_AREA_NVM, nvmRamArea, NVM_RAM_SIZE, CNvmInformer_LoadNvmComplete);
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_SaveNvm
// Description   : NVMデータセーブ処理
// Parameter     : ‐
// Return        : true = Save処理実施
//               : false = Save処理未実施
///////////////////////////////////////////////////////////////////
bool CNvmInformer_SaveNvm()
{
	if ( g_NvmChange == NVM_CHANGE ) {
		DD_NVM_Save(DD_NVM_AREA_NVM, nvmRamArea, NVM_RAM_SIZE, CNvmInformer_SaveNvmComplete);
		return true;
	}
	else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_FirstPowerOnInitialize
// Description   : FirstPowerOn初期化処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CNvmInformer_FirstPowerOnInitialize()
{
	US i;
	
	/* 全てのNVMを初期化 */
	for ( i = 0; i < NVMID_NUM; i++ ) {
		CNvmNVM_Initialize(&nvmAllData[i]);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_VersionUpInitialize
// Description   : Version Up初期化(Marketごと)処理
// Parameter     : previousMajor 前回MajorVersion
//                 previousMinor 前回MinorVersion
// Return        : ‐
// Note          : Marketごとの初期値を設定する
///////////////////////////////////////////////////////////////////
void CNvmInformer_VersionUpInitialize(US previousMajor, US previousMinor)
{
	US i;
	
	/* Version Up時のみ初期化を行う */
	if ( CNvmInformer_CompareVersion(MAJOR_VERSION, MINOR_VERSION, previousMajor, previousMinor) == NVM_VERSION_UP ) {
		CNvmInformer_AutoInitialize(previousMajor, previousMinor);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_OutRangeInitialize
// Description   : 範囲外NVM初期化処理
// Parameter     : ‐
// Return        : ‐
// Note          : ‐
///////////////////////////////////////////////////////////////////
void CNvmInformer_OutRangeInitialize()
{
	US i;
	
	/* 範囲外のNVMを初期化する */
	for ( i = 0; i < NVMID_NUM; i++ ) {
		if ( CNvmNVM_IsOutRange(&nvmAllData[i]) ) {
			CNvmNVM_Initialize(&nvmAllData[i]);
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_DiagInitialize
// Description   : 初期化禁止でないNVMの全初期化処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CNvmInformer_DiagInitialize()
{
	US i;
	
	/* 初期化禁止でないNVMのみ初期化 */
	for ( i = 0; i < NVMID_NUM; i++ ) {
		if ( CNvmNVM_IsFactoryInitAllowed(&nvmAllData[i]) ) {
			CNvmNVM_Initialize(&nvmAllData[i]);
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_Get
// Description   : NVM IDに対応したインスタンスポインタの取得
// Parameter     : id NVM ID
// Return        : インスタンスポインタ
///////////////////////////////////////////////////////////////////
CNvmNVM* CNvmInformer_Get(ENvmID id)
{
	if ( id >= NVMID_NUM ) {
		return NULL;
	}
	return &nvmAllData[id];
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_DiagGet
// Description   : Chain Linkに対応したインスタンスポインタの取得
// Parameter     : chain Chain番号
//                 link Link番号
// Return        : インスタンスのポインタ(NVMが無い場合はNULL)
///////////////////////////////////////////////////////////////////
CNvmNVM* CNvmInformer_DiagGet(US chain, US link)
{
	US i;
	
	for ( i = 0; i < NVMID_NUM; i++ ) {
		if ( ( nvmAllData[i].mpSpec->chain == chain ) && ( nvmAllData[i].mpSpec->link == link ) ) {
			return &nvmAllData[i];
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_ReadArea
// Description   : NVMエリアをオフセット位置から指定サイズ分読み出す
// Parameter     : offset 読み込み開始位置
//                 data 読み出し先データ
//                 size 読み出しデータサイズ
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CNvmInformer_ReadArea(US offset, UC* pData, UC size)
{
	US i;
	
	for ( i = 0; i < size; i++ ) {
		if ( offset + i >= NVM_RAM_TABLE_SIZE ) {
			break;
		}
		pData[i] = *(nvmRamTable[offset+i].address);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_WriteArea
// Description   : NVMエリアへオフセット位置から指定サイズ分書き込む
// Parameter     : offset 書き込み開始位置
//                 data 書き込み元データ
//                 size 書き込みデータサイズ
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CNvmInformer_WriteArea(US offset, UC* pData, UC size)
{
	US i;
	
	for ( i = 0; i < size; i++ ) {
		if ( ( offset + i ) >= NVM_RAM_TABLE_SIZE ) {
			break;
		}
		*(nvmRamTable[offset+i].address) = pData[i];
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_GetAreaSize
// Description   : NVMエリアのサイズ取得
// Parameter     : ‐
// Return        : NVMエリアのサイズ(Byte単位)
///////////////////////////////////////////////////////////////////
US CNvmInformer_GetAreaSize()
{
	return NVM_RAM_SIZE;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_GetNvmAreaAddress
// Description   : NVMエリアで使用している領域のアドレスを返す
// Parameter     : ‐
// Return        : NVMエリアのアドレス
///////////////////////////////////////////////////////////////////
UC* CNvmInformer_GetAreaAddress()
{
	return &nvmRamArea[0];
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_SetValueChange
// Description   : NVMの変更があったことを登録
// Parameter     : -
// Return        : -
///////////////////////////////////////////////////////////////////
void CNvmInformer_SetValueChange()
{
	g_NvmChange = NVM_CHANGE;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_CompareVersion
// Description   : 現在のバージョンと以前のバージョンを比較する
// Parameter     : currentMajor:今回のメジャーバージョン
//                 currentMinor:今回のマイナーバージョン
//                 previousMajor:以前のメジャーバージョン
//                 previousMinor:以前のマイナーバージョン
// Return        : -1: バージョンダウン
//                  0: バージョン変化なし
//                 +1: バージョンアップ
///////////////////////////////////////////////////////////////////
SS CNvmInformer_CompareVersion(US currentMajor, US currentMinor, US previousMajor, US previousMinor)
{
	if ( currentMajor > previousMajor ) {
		return NVM_VERSION_UP;
	} 
	else if ( currentMajor == previousMajor ) {
		if ( currentMinor > previousMinor ) {
			return NVM_VERSION_UP;
		} 
		else if ( currentMinor == previousMinor ) {
			return NVM_VERSION_NO_CHANGE;
		}
		else {
			return NVM_VERSION_DOWN;
		}
	}
	else {
		return NVM_VERSION_DOWN;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_AutoInitialize
// Description   : Version Up初期化処理
// Parameter     : ‐
// Return        : ‐
// Note          : Marketごとの初期値で初期化を行う
///////////////////////////////////////////////////////////////////
void CNvmInformer_AutoInitialize(US previousMajor, US previousMinor)
{
	US i;
	
	for ( i = 0; i < NVMID_NUM; i++ ) {
		if ( nvmAutoInitializeData[i].major < previousMajor ) {
			continue;
		}
		if ( ( nvmAutoInitializeData[i].major == previousMajor ) && ( nvmAutoInitializeData[i].minor <= previousMinor ) ) {
			continue;
		}
		CNvmNVM_Initialize(CNvmInformer_Get(nvmAutoInitializeData[i].id));
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_LoadNvmComplete
// Description   : 
// Parameter     : ‐
// Return        : ‐
// Note          : ‐
///////////////////////////////////////////////////////////////////
void CNvmInformer_LoadNvmComplete(SS result)
{
	CNmaHandler_LoadNvmComplete(NMA_INFORMER_NVM, result);
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_SaveNvmComplete
// Description   : 
// Parameter     : ‐
// Return        : ‐
// Note          : ‐
///////////////////////////////////////////////////////////////////
void CNvmInformer_SaveNvmComplete(SS result)
{
	CNmaHandler_SaveNvmComplete(NMA_INFORMER_NVM, result);
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_SetMajorVersion
// Description   : Major Version設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CNvmInformer_SetMajorVersion(UC version)
{
	if ( SRVC_GetNvmValueUS(NVMID_FINISHER_MAJOR_VERSION) != version ) {
		SRVC_SetNvmValueUS(version,	NVMID_FINISHER_MAJOR_VERSION);
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_SetMinorVersion
// Description   : Minor Version設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CNvmInformer_SetMinorVersion(UC version)
{
	if ( SRVC_GetNvmValueUS(NVMID_FINISHER_MINOR_VERSION) != version ) {
		SRVC_SetNvmValueUS(version,	NVMID_FINISHER_MINOR_VERSION);
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_SetPatchVersion
// Description   : Patch Version設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CNvmInformer_SetPatchVersion(UC version)
{
	if ( SRVC_GetNvmValueUS(NVMID_FINISHER_PATCH_VERSION) != version ) {
		SRVC_SetNvmValueUS(version,	NVMID_FINISHER_PATCH_VERSION);
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_SetIplVersion
// Description   : IPL Version設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CNvmInformer_SetIplVersion(UC version)
{
	if ( SRVC_GetNvmValueUS(NVMID_NVM_IPL_VERSION) != version ) {
		SRVC_SetNvmValueUS(version,	NVMID_NVM_IPL_VERSION);
	}
	else{ /* No Action */ }
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 15.03.31	| Makoto Tanabe		| Created this file based on Base SW V1
// 15.06.19	| Yuichi Hoshino	| Introduction Spec [ NVM Fail原因ログ送信 ]
// 15.06.19	| Yuichi Hoshino	| Release Revision 00 to GA-Finisher V0.1.0 Make.
// 15.07.16	| Makoto Tanabe		| 機能改善[NVMリストのマクロ修正に伴う変更]
// 16.02.26	| Yuichi Hoshino	| Introduction Spec [ Download ]
// 16.03.01	| Yuichi Hoshino	| Release Revision 01 to GA-Finisher V1.20.0 Make.
//
