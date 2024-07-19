// File Name:	CTempInformer.c
// Description:	Implimentation File of Temp Data Informer
// Engineer:	Xxxx Xxxx
// Last Edit:	XX.XX.XX
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CTempInformer.h"

#include "CTempData.h"
#include "CSysManager.h"
#include "DD_NVM.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////
extern const CTempData tempAllData[];

/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CTempInformer_LoadNvmComplete(SS result);
void CTempInformer_SaveNvmComplete(SS result);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CTempInformer_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CTempInformer_Constructor()
{
	// データはNVMからロードされる為、初期化しない
}

///////////////////////////////////////////////////////////////////
// Function Name : CTempInformer_LoadNvmComplete
// Description   : TempDataのロード完了時処理
// Parameter     : result
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CTempInformer_LoadNvmComplete(SS result)
{
	CSysManager_LoadNvmComplete(SYS_NVM_DATA_INFO_TEMP, result);
}

///////////////////////////////////////////////////////////////////
// Function Name : CTempInformer_SaveNvmComplete
// Description   : TempDataのセーブ完了時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CTempInformer_SaveNvmComplete(SS result)
{
	CSysManager_SaveNvmComplete(SYS_NVM_DATA_INFO_TEMP, result);
}

///////////////////////////////////////////////////////////////////
// Function Name : CTempInformer_LoadNvm
// Description   : TempDataのロード依頼
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CTempInformer_LoadNvm()
{
	DD_NVM_Load(DD_NVM_AREA_TEMP, &temp_saveFlag, TEMP_DATA_RAM_SIZE, CTempInformer_LoadNvmComplete);
}

///////////////////////////////////////////////////////////////////
// Function Name : CTempInformer_SaveNvm
// Description   : TempDataのセーブ依頼
// Parameter     : ‐
// Return        : true = Save処理実施
//               : false = Save処理未実施
///////////////////////////////////////////////////////////////////
bool CTempInformer_SaveNvm()
{
	if( temp_saveFlag ) {
		temp_saveFlag = 0;
		DD_NVM_Save(DD_NVM_AREA_TEMP, &temp_saveFlag, TEMP_DATA_RAM_SIZE, CTempInformer_SaveNvmComplete);
		return true;
	}
	else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CTempInformer_FirstPowerOnInitialize
// Description   : FirstPowerOn初期化処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CTempInformer_FirstPowerOnInitialize()
{
	US i;
	
	/* 全てのTempDataを初期化 */
	for ( i = 0; i < TEMP_DATA_NUM; i++ ) {
		CTempData_Initialize(&tempAllData[i]);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CTempInformer_VersionInitialize
// Description   : Version Up初期化処理
// Parameter     : previousMajor 前回MajorVersion
//                 previousMinor 前回MinorVersion
//                 previousPatch 前回PatchVersion
// Return        : -
///////////////////////////////////////////////////////////////////
void CTempInformer_VersionInitialize(US previousMajor, US previousMinor, US previousPatch)
{
	US i;
	
	/* Version変更時に初期化を行う */
	if ( ( MAJOR_VERSION != previousMajor ) || ( MINOR_VERSION != previousMinor ) || ( PATCH_VERSION != previousPatch ) ) {
		for ( i = 0; i < TEMP_DATA_NUM; i++ ) {
			CTempData_Initialize(&tempAllData[i]);
		}
	}
}

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
