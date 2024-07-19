// File Name:	CHfsiInformer.c
// Description:	Implimentation File of HFSI Informer
// Engineer:	Xxxx Xxxx
// Last Edit:	XX.XX.XX
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CHfsiInformer.h"

#include "HfsiInst.h"
#include "HfsiRam.h"
#include "HfsiAutoInitialize.h"
#include "CNmaHandler.h"

#include "DD_NVM.h"

#include "CSysManager.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- VersionUp/Down ---
#define HFSI_VERSION_DOWN			(-1)		// VersionDown
#define HFSI_VERSION_NO_CHANGE		0			// Version変化なし
#define HFSI_VERSION_UP				1			// VersionUp

// --- HFSI Change ---
#define HFSI_NO_CHANGE				0			// HFSI変化なし
#define HFSI_CHANGE					1			// HFSI変化あり

static US g_HfsiDataindex;
static UC g_HfsiChange;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
SS CHfsiInformer_CompareVersion(US currentMajor, US currentMinor, US previousMajor, US previousMinor);
void CHfsiInformer_LoadNvmComplete(SS result);
void CHfsiInformer_SaveNvmComplete(SS result);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiInformer_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CHfsiInformer_Constructor()
{
	US i;
	
	/* HFSIアイテム初期化 */
	for ( i = 0; i < HFSIID_NUM; i++ ) {
		CHfsiHFSI_Constructor(&(hfsiAllData[i]));
	}
	
	/* ローカルデータ初期化 */
	g_HfsiDataindex = 0;
	g_HfsiChange = HFSI_NO_CHANGE;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiInformer_LoadNvm
// Description   : ロード処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CHfsiInformer_LoadNvm()
{
	DD_NVM_Load(DD_NVM_AREA_HFSI, (UC *)(&hfsiRamArea), (sizeof(HFSIRamData) * HFSIID_NUM), CHfsiInformer_LoadNvmComplete);
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiInformer_SaveNvm
// Description   : セーブ処理
// Parameter     : ‐
// Return        : true = Save処理実施
//               : false = Save処理未実施
///////////////////////////////////////////////////////////////////
bool CHfsiInformer_SaveNvm()
{
	if ( g_HfsiChange == HFSI_CHANGE ) {
		DD_NVM_Save(DD_NVM_AREA_HFSI, (UC *)(&hfsiRamArea), (sizeof(HFSIRamData) * HFSIID_NUM), CHfsiInformer_SaveNvmComplete);
		return true;
	}
	else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiInformer_FirstPowerOnInitialize
// Description   : FirstPowerOn初期化処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CHfsiInformer_FirstPowerOnInitialize()
{
	CHfsiInformer_DiagInitialize();
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiInformer_VersionUpInitialize
// Description   : Version Up初期化処理
// Parameter     : previousMajor (Previous Major Version)
//               : previousMinor (Previous Minor Version)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CHfsiInformer_VersionUpInitialize(US previousMajor, US previousMinor)
{
	/* Version Up初期化 */
	if ( CHfsiInformer_CompareVersion(MAJOR_VERSION, MINOR_VERSION, previousMajor, previousMinor) == HFSI_VERSION_UP ) {
		CHfsiInformer_AutoInitialize(previousMajor, previousMinor);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiInformer_DiagInitialize
// Description   : 全HFSIアイテムを初期化する
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CHfsiInformer_DiagInitialize()
{
	US i;
	
    for ( i = 0; i < HFSIID_NUM; i++ ) {
        CHfsiHFSI_Initialize(&hfsiAllData[i]);
    }
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiInformer_Get
// Description   : HFSI IDに対応したインスタンスポインタを返す
// Parameter     : id (HFSI ID)
// Return        : インスタンスポインタ
///////////////////////////////////////////////////////////////////
CHfsiHFSI* CHfsiInformer_Get(EHfsiItemID id)
{
	if ( id >= HFSIID_NUM ) {
		return NULL;
	}
	return &hfsiAllData[id];
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiInformer_DiagGet
// Description   : Chain linkに対応したインスタンスポインタを返す
// Parameter     : chain (ChainNo)
//               : link (LinkNo)
// Return        : インスタンスポインタ
///////////////////////////////////////////////////////////////////
CHfsiHFSI* CHfsiInformer_DiagGet(US chain, US link)
{
	US i;
	
	for ( i = 0; i < HFSIID_NUM; i++ ) {
		if ( ( hfsiAllData[i].mpSpec->chain == chain ) && ( hfsiAllData[i].mpSpec->link == link ) ) {
			return &hfsiAllData[i];
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiInformer_ResetIterate
// Description   : 先頭ののHFSIへのポインタを返す
// Parameter     : ‐
// Return        : インスタンスポインタ
///////////////////////////////////////////////////////////////////
CHfsiHFSI* CHfsiInformer_ResetIterate()
{
	g_HfsiDataindex = 0;
	
	return &hfsiAllData[g_HfsiDataindex];
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiInformer_NextIterate
// Description   : 次のHFSIへのポインタを返す
// Parameter     : ‐
// Return        : インスタンスポインタ
///////////////////////////////////////////////////////////////////
CHfsiHFSI* CHfsiInformer_NextIterate()
{
	if ( g_HfsiDataindex + 1 < HFSIID_NUM ) {
		g_HfsiDataindex++;
		return &hfsiAllData[g_HfsiDataindex];
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiInformer_SetValueChange
// Description   : NVMの変更があったことを登録
// Parameter     : -
// Return        : -
///////////////////////////////////////////////////////////////////
void CHfsiInformer_SetValueChange()
{
	g_HfsiChange = HFSI_CHANGE;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiInformer_GetAreaSize
// Description   : HFSIエリアで使用している領域のサイズを返す
// Parameter     : ‐
// Return        : HFSIエリアのサイズ(Byte単位)
///////////////////////////////////////////////////////////////////
US CHfsiInformer_GetAreaSize()
{
	return sizeof(HFSIRamData) * HFSIID_NUM;
}
///////////////////////////////////////////////////////////////////
// Function Name : CHfsiInformer_GetAreaAddress
// Description   : HFSIエリアで使用している領域のアドレスを返す
// Parameter     : ‐
// Return        : HFSIエリアの先頭アドレス
///////////////////////////////////////////////////////////////////
UC* CHfsiInformer_GetAreaAddress()
{
	return (UC*)&hfsiRamArea;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiInformer_AutoInitialize
// Description   : Version Up初期化処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CHfsiInformer_AutoInitialize(US previousMajor, US previousMinor)
{
	US i;
	
	for ( i = 0; i < HFSIID_NUM; i++ ) {
		if ( hfsiAutoInitializeData[i].major < previousMajor ) {
			continue;
		}
		if ( ( hfsiAutoInitializeData[i].major == previousMajor ) && ( hfsiAutoInitializeData[i].minor <= previousMinor ) ) {
			continue;
		}
		CHfsiHFSI_Initialize(CHfsiInformer_Get(hfsiAutoInitializeData[i].id));
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiInformer_CompareVersion
// Description   : 現在のバージョンと以前のバージョンを比較する
// Parameter     : currentMajor :今回のMajorVersion
//                 currentMinor :今回のMinorVersion
//                 previousMajor:前回のMajorVersion
//                 previousMinor:前回のMinorVersion
// Return        : -1 : VersionDown
//                  0 : Version変化なし
//                 +1 : VersionUp
///////////////////////////////////////////////////////////////////
SS CHfsiInformer_CompareVersion(US currentMajor, US currentMinor, US previousMajor, US previousMinor)
{
	if ( currentMajor > previousMajor ) {
		return HFSI_VERSION_UP;
	} 
	else if ( currentMajor == previousMajor ) {
		if ( currentMinor > previousMinor ) {
			return HFSI_VERSION_UP;
		} 
		else if ( currentMinor == previousMinor ) {
			return HFSI_VERSION_NO_CHANGE;
		}
		else {
			return HFSI_VERSION_DOWN;
		}
	}
	else {
		return HFSI_VERSION_DOWN;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiInformer_LoadNvmComplete
// Description   : 
// Parameter     : result
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CHfsiInformer_LoadNvmComplete(SS result)
{
	CNmaHandler_LoadNvmComplete(NMA_INFORMER_HFSI, result);
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiInformer_SaveNvmComplete
// Description   : 
// Parameter     : result
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CHfsiInformer_SaveNvmComplete(SS result)
{
	CNmaHandler_SaveNvmComplete(NMA_INFORMER_HFSI, result);
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file based on Base SW VXX
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.05.08	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.06.19	| Yuichi Hoshino	| Introduction Spec [ NVM Fail原因ログ送信 ]
// XX.XX.XX	| Yuichi Hoshino	| Release V2 (Revision 01)
//
