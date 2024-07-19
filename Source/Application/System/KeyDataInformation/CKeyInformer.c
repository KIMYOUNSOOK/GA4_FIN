// File Name:	CKeyInformer.c
// Description:	Implimentation File of Key Data Informer
// Engineer:	Yuichi Hoshino
// Last Edit:	16.02.09
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CKeyInformer.h"

#include "KeyArea.h"

#include "CSysManager.h"
#include "CNmaHandler.h" 


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Type Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of Key Data Informer ---
typedef struct /*CKeyInformerMember*/
{
	KEY_AREA_TYPE mKeyRamArea;
	UC mNvmChange;
} CKeyInformerMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of Key Data Informer ---
static CKeyInformerMember gKey_InformerMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

static void LoadNvmComplete(SS result);
static void SaveNvmComplete(SS result);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CKeyInformer_Constructor()
{
	UC index;

	gKey_InformerMember.mKeyRamArea.SIZE.KEYCODE = 0;
	gKey_InformerMember.mKeyRamArea.SIZE.DLMODE  = DOWNLOAD_MODE_COMPLETE;
	gKey_InformerMember.mKeyRamArea.SIZE.MAJOR_V = 0;
	gKey_InformerMember.mKeyRamArea.SIZE.MINOR_V = 0;
	gKey_InformerMember.mKeyRamArea.SIZE.PATCH_V = 0;
	gKey_InformerMember.mKeyRamArea.SIZE.SEQ_BIT = 0;
	gKey_InformerMember.mKeyRamArea.SIZE.WRTCNT  = 0;
	gKey_InformerMember.mKeyRamArea.SIZE.IPL_V	 = 0;

	for ( index = 0; index < (KEY_AREA_SIZE - KEY_AREA_USE_SIZE); index++ ){
		gKey_InformerMember.mKeyRamArea.SIZE.CONFIG[index] = 0;
	}

	gKey_InformerMember.mNvmChange = KEY_NO_CHANGE;
}

///////////////////////////////////////////////////////////////////
// Function Name : LoadNvmComplete
// Description   : NVMのロード完了時処理
// Parameter     : result
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LoadNvmComplete(SS result)
{
	CNmaHandler_LoadNvmComplete(NMA_INFORMER_KEY, result);
}

///////////////////////////////////////////////////////////////////
// Function Name : SaveNvmComplete
// Description   : NVMのセーブ完了時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SaveNvmComplete(SS result)
{
	CNmaHandler_SaveNvmComplete(NMA_INFORMER_KEY, result);
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_LoadNvm
// Description   : NVMのロード依頼
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CKeyInformer_LoadNvm()
{
	DD_NVM_Load(DD_NVM_AREA_KEY, (UC *)(&(gKey_InformerMember.mKeyRamArea)), KEY_AREA_SIZE, LoadNvmComplete);
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_SaveNvm
// Description   : NVMのセーブ依頼
// Parameter     : ‐
// Return        : true = セーブ依頼受理(セーブ処理開始)
//               : false = セーブ処理拒絶
///////////////////////////////////////////////////////////////////
bool CKeyInformer_SaveNvm()
{
	if ( gKey_InformerMember.mNvmChange == KEY_CHANGE ){
		(gKey_InformerMember.mKeyRamArea.SIZE.WRTCNT)++;

		DD_NVM_Save(DD_NVM_AREA_KEY, (UC *)(&(gKey_InformerMember.mKeyRamArea)), KEY_AREA_SIZE, SaveNvmComplete);

		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_CheckKeyCode
// Description   : KeyCodeの初期化済みチェック
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
bool CKeyInformer_CheckKeyCode()
{
	if ( gKey_InformerMember.mKeyRamArea.SIZE.KEYCODE == KEY_CODE ) return true;
	else															return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_SetKeyCode
// Description   : KeyCodeへ初期化済みを設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CKeyInformer_SetKeyCode()
{
	if ( gKey_InformerMember.mKeyRamArea.SIZE.KEYCODE != KEY_CODE ){
		gKey_InformerMember.mKeyRamArea.SIZE.KEYCODE = KEY_CODE;

		gKey_InformerMember.mNvmChange = KEY_CHANGE;
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_ClearKeyCode
// Description   : KeyCodeへ未初期化を設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CKeyInformer_ClearKeyCode()
{
	if ( gKey_InformerMember.mKeyRamArea.SIZE.KEYCODE == KEY_CODE ){
		gKey_InformerMember.mKeyRamArea.SIZE.KEYCODE = 0;

		gKey_InformerMember.mNvmChange = KEY_CHANGE;
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_CheckDownloadMode
// Description   : DownloadModeチェック
// Parameter     : ‐
// Return        : 
///////////////////////////////////////////////////////////////////
bool CKeyInformer_CheckDownloadMode()
{
	if ( gKey_InformerMember.mKeyRamArea.SIZE.DLMODE == DOWNLOAD_MODE_COMPLETE ) return false;
	else																		 return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_SetDownloadMode
// Description   : DownloadMode設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CKeyInformer_SetDownloadMode()
{
	if ( gKey_InformerMember.mKeyRamArea.SIZE.DLMODE != DOWNLOAD_MODE_CHANGE ){
		gKey_InformerMember.mKeyRamArea.SIZE.DLMODE = DOWNLOAD_MODE_CHANGE;

		gKey_InformerMember.mNvmChange = KEY_CHANGE;
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_ClearDownloadMode
// Description   : DownloadMode解除
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CKeyInformer_ClearDownloadMode()
{
	if ( gKey_InformerMember.mKeyRamArea.SIZE.DLMODE != DOWNLOAD_MODE_COMPLETE ){
		gKey_InformerMember.mKeyRamArea.SIZE.DLMODE = DOWNLOAD_MODE_COMPLETE;

		gKey_InformerMember.mNvmChange = KEY_CHANGE;
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_SetMajorVersion
// Description   : Major Version設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CKeyInformer_SetMajorVersion(UC version)
{
	if ( gKey_InformerMember.mKeyRamArea.SIZE.MAJOR_V != version ){
		gKey_InformerMember.mKeyRamArea.SIZE.MAJOR_V = version;

		gKey_InformerMember.mNvmChange = KEY_CHANGE;
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_GetMajorVersion
// Description   : Major Version取得
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
UC CKeyInformer_GetMajorVersion()
{
	return gKey_InformerMember.mKeyRamArea.SIZE.MAJOR_V;
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_SetMinorVersion
// Description   : Minor Version設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CKeyInformer_SetMinorVersion(UC version)
{
	if ( gKey_InformerMember.mKeyRamArea.SIZE.MINOR_V != version ){
		gKey_InformerMember.mKeyRamArea.SIZE.MINOR_V = version;

		gKey_InformerMember.mNvmChange = KEY_CHANGE;
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_GetMinorVersion
// Description   : Minor Version取得
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
UC CKeyInformer_GetMinorVersion()
{
	return gKey_InformerMember.mKeyRamArea.SIZE.MINOR_V;
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_SetPatchVersion
// Description   : Patch Version設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CKeyInformer_SetPatchVersion(UC version)
{
	if ( gKey_InformerMember.mKeyRamArea.SIZE.PATCH_V != version ){
		gKey_InformerMember.mKeyRamArea.SIZE.PATCH_V = version;

		gKey_InformerMember.mNvmChange = KEY_CHANGE;
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_GetPatchVersion
// Description   : Patch Version取得
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
UC CKeyInformer_GetPatchVersion()
{
	return gKey_InformerMember.mKeyRamArea.SIZE.PATCH_V;
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_GetWriteCount
// Description   : 書き込み回数取得
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
UL CKeyInformer_GetWriteCount()
{
	return gKey_InformerMember.mKeyRamArea.SIZE.WRTCNT;
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_SetRxSequenceBit 
// Description   : Rxシーケンスビットセット処理
// Parameter     : seqBit (シーケンスビット) [0/1]
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CKeyInformer_SetRxSequenceBit(UC seqBit)
{
//	if ( ((gKey_InformerMember.mKeyRamArea.SIZE.SEQ_BIT >> SEQUENCE_BIT_SAVE_POS_RX) & 0x01) != seqBit ){
//																				// Download時はDownload Mode書き込みがあり、必ず書き込み処理対象となるため変化チェックは行わない
		gKey_InformerMember.mKeyRamArea.SIZE.SEQ_BIT &= ~(SEQUENCE_BIT_SAVE_POS_PTN(SEQUENCE_BIT_SAVE_POS_RX));
																				// 対象ビットをクリア
		gKey_InformerMember.mKeyRamArea.SIZE.SEQ_BIT |= ((seqBit & 0x01) << SEQUENCE_BIT_SAVE_POS_RX);

		gKey_InformerMember.mNvmChange = KEY_CHANGE;
//	}
//	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_GetRxSequenceBit
// Description   : Rxシーケンスビット取得処理
// Parameter     : ‐
// Return        : シーケンスビット [0/1]
///////////////////////////////////////////////////////////////////
UC CKeyInformer_GetRxSequenceBit()
{
	return (gKey_InformerMember.mKeyRamArea.SIZE.SEQ_BIT >> SEQUENCE_BIT_SAVE_POS_RX) & 0x01;
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_SetTxSequenceBit
// Description   : Txシーケンスビットセット処理
// Parameter     : seqBit (シーケンスビット) [0/1]
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CKeyInformer_SetTxSequenceBit(UC seqBit)
{
//	if ( ((gKey_InformerMember.mKeyRamArea.SIZE.SEQ_BIT >> SEQUENCE_BIT_SAVE_POS_TX) & 0x01) != seqBit ){
//																				// Download時はDownload Mode書き込みがあり、必ず書き込み処理対象となるため変化チェックは行わない
		gKey_InformerMember.mKeyRamArea.SIZE.SEQ_BIT &= ~(SEQUENCE_BIT_SAVE_POS_PTN(SEQUENCE_BIT_SAVE_POS_TX));
																				// 対象ビットをクリア
		gKey_InformerMember.mKeyRamArea.SIZE.SEQ_BIT |= ((seqBit & 0x01) << SEQUENCE_BIT_SAVE_POS_TX);

		gKey_InformerMember.mNvmChange = KEY_CHANGE;
//	}
//	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_GetTxSequenceBit
// Description   : Txシーケンスビット取得処理
// Parameter     : ‐
// Return        : シーケンスビット [0/1]
///////////////////////////////////////////////////////////////////
UC CKeyInformer_GetTxSequenceBit()
{
	return (gKey_InformerMember.mKeyRamArea.SIZE.SEQ_BIT >> SEQUENCE_BIT_SAVE_POS_TX) & 0x01;
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_SetIplVersion
// Description   : IPL Versionセット処理
// Parameter     : version (IPL Version)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CKeyInformer_SetIplVersion(UC version)
{
	if ( gKey_InformerMember.mKeyRamArea.SIZE.IPL_V != version ){ 
		gKey_InformerMember.mKeyRamArea.SIZE.IPL_V = version;

		gKey_InformerMember.mNvmChange = KEY_CHANGE;
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_GetIplVersion
// Description   : IPL Version取得処理
// Parameter     : ‐
// Return        : IPL Version
///////////////////////////////////////////////////////////////////
UC CKeyInformer_GetIplVersion()
{
	return gKey_InformerMember.mKeyRamArea.SIZE.IPL_V;
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 15.03.31	| Yuichi Hoshino	| Created this file based on Base SW V1
// 15.06.19	| Yuichi Hoshino	| Introduction Spec [ NVM Fail原因ログ送信 ]
// 15.06.19	| Yuichi Hoshino	| Release Revision 00 to GA-Finisher V0.1.0 Make.
// 16.02.09	| Yuichi Hoshino	| Introduction Spec [ Download ]
// 16.03.01	| Yuichi Hoshino	| Release Revision 01 to GA-Finisher V1.20.0 Make.
//
