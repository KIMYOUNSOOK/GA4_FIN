// File Name:	CDltManager.c
// Description:	Implimentation File of Download Mode Transition Manager
// Engineer:	Yuichi Hoshino
// Last Edit:	16.02.26
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CDltManager.h"

#include "fsl.h"
#include "DD_COM_Service.h"
#include "LIB_Lock.h"

#include "CSysManager.h"
#include "CKeyInformer.h"
#include "CDvstInformer.h"
#include "CNmaHandler.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Structure Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of Power Up State ---
typedef struct /*CDltManagerMember*/ {
	UC mTransition;
} CDltManagerMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

static CDltManagerMember gDlt_ManagerMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CDltManager_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDltManager_Constructor()
{
	gDlt_ManagerMember.mTransition = false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CDltManager_Transition
// Description   : ダウンロードモードへの移行指示受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDltManager_Transition()
{
	UC informerID;

	gDlt_ManagerMember.mTransition = true;

	// --- 通信停止(Sequence Bit変化防止) ---
	// 停止しなくても成り立つ見通しのため工数削減のために対応しない

	// --- Seq情報のNVMへの格納 ---
	CKeyInformer_SetRxSequenceBit(DD_COM_GetRxSequenceBit());
	CKeyInformer_SetTxSequenceBit(DD_COM_GetTxSequenceBit());

	// --- Download Mode格納 ---
	CKeyInformer_SetDownloadMode();

	// --- NVM書き込み ---
	CNmaHandler_Save();															// Download Mode NVMを書き込むのでNVM Save処理が実行されないことはないため書き込みがないケースはNo Care
}

///////////////////////////////////////////////////////////////////
// Function Name : CDltManager_SaveNvmComplete
// Description   : NVMデータ書き込み完了通知受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDltManager_SaveNvmComplete(SS result)
{
	static const UC cLogMsg[] = {"DL_CPU_Reset_NG"};
	static const __far fsl_descriptor_t fsl_descriptor_pstr = { 0x00, 0x20, 0x01 };
	fsl_u08 retCode;
	UC key;

	if ( gDlt_ManagerMember.mTransition == true ) {
		if ( result == DD_NVM_OK ){
			// --- リセット処理 ---
			retCode = FSL_Init(&fsl_descriptor_pstr);

			if ( retCode == FSL_OK ) {
				FSL_Open();
				FSL_PrepareFunctions();
				FSL_PrepareExtFunctions();
				
				LIB_Lock();
				retCode = FSL_SwapBootCluster();
				LIB_Unlock(key);
			}
			else{ /* No Action */ }

			FSL_Close();
			
		}
		else{
			// --- NVM Fail検知(Download Sequenceが止まりControllerがエラーを検知しリブート) ---
			CDvstDeviceStatus_SetStatusValue(CDvstInformer_GetDeviceStatus(DSNO_NVM_FAIL), FAULT_OCCUR);
			CSysManager_FaultStatusChange(DSNO_NVM_FAIL, FAULT_OCCUR);
		}

		// --- この処理までリセットされずに実行されたら異常 ---
		DD_LOG_Put(LOGID_FINISHER, '0', cLogMsg, sizeof(cLogMsg), LOG_CATEGORY_DEF);
	}
	else{ /* No Action */ }
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 16.02.26	| Yuichi Hoshino	| Created this file
// 16.02.26	| Yuichi Hoshino	| Introduction Spec [ Download ]
// 16.03.01	| Yuichi Hoshino	| Release Revision 00 to GA-Finisher V1.20.0 Make.
//
