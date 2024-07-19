// File Name:	CDvstInformer.c
// Description:	Implimentation File of Device Status Informer
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.14
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CDvstInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Instance of Device Status  ---
extern CDvstDeviceStatus const cDvst_DeviceStatus[DSNO_NUM];


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of Device Status Informer ---
typedef struct /*CDvstInformerMember*/
{
	US mFailCondition;
	UL mJamCondition;
	US mCoverCondition;
	UL mDeviceCondition[DEVID_NUM];
} CDvstInformerMember;

static CDvstInformerMember gDvst_InformerMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CDvstInformer_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDvstInformer_Constructor()
{
	US statusNo;
	UC deviceID;

	for ( statusNo = 0; statusNo < DSNO_NUM; statusNo++ ){
		CDvstDeviceStatus_Constructor(&(cDvst_DeviceStatus[statusNo]));
	}

	gDvst_InformerMember.mFailCondition	 = 0x0000;
	gDvst_InformerMember.mJamCondition	 = 0x00000000;
	gDvst_InformerMember.mCoverCondition = 0x0000;

	for ( deviceID = 0; deviceID < DEVID_NUM; deviceID++ ){
		gDvst_InformerMember.mDeviceCondition[deviceID] = 0x00000000;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CDvstInformer_GetDeviceStatusNo
// Description   : 指定Device Status IDに対応するDevice Status No.
//               : の取得処理
// Parameter     : statusID (Device Status ID)
// Return        : Device Status No,
///////////////////////////////////////////////////////////////////
US CDvstInformer_GetDeviceStatusNo(US statusID)
{
	US statusNo;

	for ( statusNo = 0; statusNo < DSNO_NUM; statusNo++ ){
		if ( CDvstDeviceStatus_GetStatusID(&(cDvst_DeviceStatus[statusNo])) == statusID ){
			return statusNo;
		}
		else{
			// Go loop
		}
	}

	return DSNO_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CDvstInformer_GetDeviceStatus
// Description   : Device Status取得処理
// Parameter     : statusNo (Device Status No.)
// Return        : Device Statusインスタンスポインタ
///////////////////////////////////////////////////////////////////
CDvstDeviceStatus* CDvstInformer_GetDeviceStatus(US statusNo)
{
	if ( statusNo < DSNO_NUM ) return &(cDvst_DeviceStatus[statusNo]);
	else					   return NULL;
}

///////////////////////////////////////////////////////////////////
// Function Name : CDvstInformer_GetDeviceStatusInOrder
// Description   : Device Status No.の採番順番における次のDevice 
//               : Statusの取得処理
// Parameter     : statusNo (Device Status No.)
// Return        : Device Statusインスタンスポインタ
///////////////////////////////////////////////////////////////////
CDvstDeviceStatus* CDvstInformer_GetDeviceStatusInOrder(US statusNo)
{
	if ( statusNo < DSNO_NUM ){
		if ( (statusNo + 1) < DSNO_NUM ) return &(cDvst_DeviceStatus[statusNo + 1]);
		else							 return NULL;
	}
	else{
		return cDvst_DeviceStatus;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CDvstInformer_GetDeviceCondtion
// Description   : Device Condition取得処理
// Parameter     : deviceID (Device ID)
// Return        : Device Condition
///////////////////////////////////////////////////////////////////
EDeviceCondition CDvstInformer_GetDeviceCondtion(EDeviceID deviceID)
{
	if ( deviceID < DEVID_NUM ){
		if ( (gDvst_InformerMember.mFailCondition & (0x0001 << deviceID)) == 0 ){
			if ( gDvst_InformerMember.mDeviceCondition[deviceID] == 0 ){
				return DEV_COND_OK;
			}
			else{ /* Temporary NG */ }

			return DEV_COND_TEMP_NG;
		}
		else{ /* Permanent NG */ }
	}
	else{ /* Permanent NG */ }

	return DEV_COND_PERM_NG;
}

///////////////////////////////////////////////////////////////////
// Function Name : CDvstInformer_UpdateDeviceCondition
// Description   : Device Status Value変化によるデバイスコンディシ
//               : ョン更新処理
// Parameter     : statusNo (Device Status No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDvstInformer_UpdateDeviceCondition(US statusNo)
{
	UL setBit;
	UC mngDev;

	if ( statusNo < DSNO_NUM ){
		if ( CDvstDeviceStatus_CheckDeviceCondition(&(cDvst_DeviceStatus[statusNo]), CDvstDeviceStatus_GetStatusValue(&(cDvst_DeviceStatus[statusNo]))) == true ){
			setBit = 0;
		}
		else{																	// Condition NG
			setBit = 1;
		}

		// --- 管理情報更新 ---
		for ( mngDev = DEVID_FINISHER; mngDev < DEVID_NUM; mngDev++ ){
			if ( CDvstDeviceStatus_CheckRelativeDevice(&(cDvst_DeviceStatus[statusNo]), (EDeviceID)mngDev) == true ){
				switch ( CDvstDeviceStatus_GetStatusCategory(&(cDvst_DeviceStatus[statusNo])) ){
				case DVST_CTGRY_FAIL:											// Fail
					if ( (gDvst_InformerMember.mFailCondition & (0x0001 << mngDev)) == 0 ){
																				// 未故障(新規に故障)
						gDvst_InformerMember.mFailCondition |= ((US)setBit << mngDev);
					}
					else{ /* No Action */ }										// 既に故障(通知)済み

					break;
				case DVST_CTGRY_JAM:											// Staic/Dynamic Jam
					gDvst_InformerMember.mJamCondition &= ~((UL)0x00000001 << CDvstDeviceStatus_GetSubNo(&(cDvst_DeviceStatus[statusNo])));
					gDvst_InformerMember.mJamCondition |= (setBit << CDvstDeviceStatus_GetSubNo(&(cDvst_DeviceStatus[statusNo])));

					break;
				case DVST_CTGRY_COVER:											// Interlock & Cover
					gDvst_InformerMember.mCoverCondition &= ~((US)0x0001 << CDvstDeviceStatus_GetSubNo(&(cDvst_DeviceStatus[statusNo])));
					gDvst_InformerMember.mCoverCondition |= ((US)setBit << CDvstDeviceStatus_GetSubNo(&(cDvst_DeviceStatus[statusNo])));

					break;
				case DVST_CTGRY_OTHER:
					gDvst_InformerMember.mDeviceCondition[mngDev] &= ~((UL)0x00000001 << CDvstDeviceStatus_GetSubNo(&(cDvst_DeviceStatus[statusNo])));
					gDvst_InformerMember.mDeviceCondition[mngDev] |= (setBit << CDvstDeviceStatus_GetSubNo(&(cDvst_DeviceStatus[statusNo])));

					break;
				default:														// Dynamic Jam/Static Jam
					// No Action
					break;
				}
			}
			else{ /* No Action */ }
		}
	}
	else{
		// No Action
	}
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
