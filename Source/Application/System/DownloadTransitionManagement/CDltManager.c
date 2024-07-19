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
// Description   : �R���X�g���N�^
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDltManager_Constructor()
{
	gDlt_ManagerMember.mTransition = false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CDltManager_Transition
// Description   : �_�E�����[�h���[�h�ւ̈ڍs�w����M����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDltManager_Transition()
{
	UC informerID;

	gDlt_ManagerMember.mTransition = true;

	// --- �ʐM��~(Sequence Bit�ω��h�~) ---
	// ��~���Ȃ��Ă����藧���ʂ��̂��ߍH���팸�̂��߂ɑΉ����Ȃ�

	// --- Seq����NVM�ւ̊i�[ ---
	CKeyInformer_SetRxSequenceBit(DD_COM_GetRxSequenceBit());
	CKeyInformer_SetTxSequenceBit(DD_COM_GetTxSequenceBit());

	// --- Download Mode�i�[ ---
	CKeyInformer_SetDownloadMode();

	// --- NVM�������� ---
	CNmaHandler_Save();															// Download Mode NVM���������ނ̂�NVM Save���������s����Ȃ����Ƃ͂Ȃ����ߏ������݂��Ȃ��P�[�X��No Care
}

///////////////////////////////////////////////////////////////////
// Function Name : CDltManager_SaveNvmComplete
// Description   : NVM�f�[�^�������݊����ʒm��M������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDltManager_SaveNvmComplete(SS result)
{
	static const UC cLogMsg[] = {"DL_CPU_Reset_NG"};
	static const __far fsl_descriptor_t fsl_descriptor_pstr = { 0x00, 0x20, 0x01 };
	fsl_u08 retCode;
	UC key;

	if ( gDlt_ManagerMember.mTransition == true ) {
		if ( result == DD_NVM_OK ){
			// --- ���Z�b�g���� ---
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
			// --- NVM Fail���m(Download Sequence���~�܂�Controller���G���[�����m�����u�[�g) ---
			CDvstDeviceStatus_SetStatusValue(CDvstInformer_GetDeviceStatus(DSNO_NVM_FAIL), FAULT_OCCUR);
			CSysManager_FaultStatusChange(DSNO_NVM_FAIL, FAULT_OCCUR);
		}

		// --- ���̏����܂Ń��Z�b�g���ꂸ�Ɏ��s���ꂽ��ُ� ---
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
