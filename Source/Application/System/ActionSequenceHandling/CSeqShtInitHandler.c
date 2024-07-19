// File Name:	CSeqShtInitHandler.c
// Description:	Implimentation File of Sheet Initialize Handler
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.22
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSeqShtInitHandler.h"

#include "CShtManager.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void StartInitialize(CSeqShtInitHandler* pThis);
void StopInitialize(CSeqShtInitHandler* pThis);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CSeqShtInitHandler_Constructor
// Description   : �R���X�g���N�^
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : pSpec (�X�y�b�N�f�[�^�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSeqShtInitHandler_Constructor(CSeqShtInitHandler* pThis, const SeqShtInitHandlerSpec* pSpec)
{
	pThis->mpSpec			   = pSpec;
	pThis->mEventParam.sheetNo = SHEETNO_NUM;
	pThis->mState			   = SEQ_ACT_STT_IDLE;

	CSrvcMsgActivity_Constructor(&(pThis->mActivity), MID_SYS, pThis->mpSpec->msgActivityNum, pThis->mpSpec->pMsgActivity, pThis->mpSpec->TimerCallBack);
}

///////////////////////////////////////////////////////////////////
// Function Name : StartInitialize
// Description   : �C�j�V�����C�Y����J�n����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
void StartInitialize(CSeqShtInitHandler* pThis)
{
	CSrvcMsgActivity_SetData(&(pThis->mActivity), (UC *)&(pThis->mEventParam), sizeof(EvtParamSheetActionRequest));

	CSrvcMsgActivity_Start(&(pThis->mActivity));
}

///////////////////////////////////////////////////////////////////
// Function Name : StopInitialize
// Description   : �C�j�V�����C�Y�����~����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
void StopInitialize(CSeqShtInitHandler* pThis)
{
	CSrvcMsgActivity_Terminate(&(pThis->mActivity));
}

///////////////////////////////////////////////////////////////////
// Function Name : CSeqShtInitHandler_GetState
// Description   : �V�[�g�C�j�V�����C�Y�����Ԏ擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �V�[�g�C�j�V�����C�Y������
///////////////////////////////////////////////////////////////////
ESeqActionState CSeqShtInitHandler_GetState(CSeqShtInitHandler* pThis)
{
	return (ESeqActionState)pThis->mState;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSeqShtInitHandler_GetSheetNo
// Description   : �V�[�g�C�j�V�����C�Y���쐧��ΏۃV�[�g��Sheet 
//               : No. �擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Sheet No.
///////////////////////////////////////////////////////////////////
UC CSeqShtInitHandler_GetSheetNo(CSeqShtInitHandler* pThis)
{
	return pThis->mEventParam.sheetNo;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSeqShtInitHandler_Start
// Description   : �V�[�g�C�j�V�����C�Y����J�n�w����M����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : sheetNo (Sheet No.)
// Return        : SEQ_ACT_STT_NUM = �C�j�V�����C�Y�����ԕω��Ȃ�
//               : SEQ_ACT_STT_NUM �ȊO = �J�ڐ�C�j�V�����C�Y����
//               : ���
///////////////////////////////////////////////////////////////////
ESeqActionState CSeqShtInitHandler_Start(CSeqShtInitHandler* pThis, UC sheetNo)
{
	UC stateChange;

	stateChange = SEQ_ACT_STT_NUM;

	if ( pThis->mState == SEQ_ACT_STT_IDLE ){
		pThis->mState			   = SEQ_ACT_STT_RUNNING;
		pThis->mEventParam.sheetNo = sheetNo;
		StartInitialize(pThis);

		stateChange = SEQ_ACT_STT_RUNNING;
	}
	else{
		// No Action
	}
	SRVC_INITManagerLog_Wide(0xC3, pThis->mState, (US)stateChange, (US)pThis->mEventParam.sheetNo);

	return stateChange;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSeqShtInitHandler_Stop
// Description   : �V�[�g�C�j�V�����C�Y�����~�w����M����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : sheetNo (Sheet No.)
// Return        : SEQ_ACT_STT_NUM = �C�j�V�����C�Y�����ԕω��Ȃ�
//               : SEQ_ACT_STT_NUM �ȊO = �J�ڐ�C�j�V�����C�Y����
//               : ���
///////////////////////////////////////////////////////////////////
ESeqActionState CSeqShtInitHandler_Stop(CSeqShtInitHandler* pThis, UC sheetNo)
{
	UC stateChange;

	stateChange = SEQ_ACT_STT_NUM;

	if ( (pThis->mState == SEQ_ACT_STT_RUNNING) &&
		 ( (sheetNo == pThis->mEventParam.sheetNo) || (sheetNo == SHEETNO_NUM) ) ){
		// --- �A�N�e�B�r�e�B���䒆�~ ---(V3.4.0 Merge)
		CSrvcMsgActivity_Abort(&(pThis->mActivity));
	}
	else{
		// No Action
	}
	SRVC_INITManagerLog_Wide(0xC4, pThis->mState, (US)stateChange, (US)pThis->mEventParam.sheetNo);

	return stateChange;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSeqShtInitHandler_SheetActionResponse
// Description   : Sheet Action Response�C�x���g��M������
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : sheetNo (Sheet No.)
//               : eventID (MSP Event ID)
//               : moduleID (Device Module ID)
// Return        : SEQ_ACT_STT_NUM = �C�j�V�����C�Y�����ԕω��Ȃ�
//               : SEQ_ACT_STT_NUM �ȊO = �J�ڐ�C�j�V�����C�Y����
//               : ���
///////////////////////////////////////////////////////////////////
ESeqActionState CSeqShtInitHandler_SheetActionResponse(CSeqShtInitHandler* pThis, UC sheetNo, US eventID, UC moduleID)
{
	UC stateChange;

	stateChange = SEQ_ACT_STT_NUM;

	if ( (pThis->mEventParam.sheetNo == sheetNo) &&
		 (pThis->mState == SEQ_ACT_STT_RUNNING) ){
		CSrvcMsgActivity_SetTrigger(&(pThis->mActivity), eventID, moduleID);

		if ( CSrvcMsgActivity_CheckEnd(&(pThis->mActivity)) == true ){			// �V�[�P���X����
			pThis->mEventParam.sheetNo = SHEETNO_NUM;
			pThis->mState			   = SEQ_ACT_STT_IDLE;

			stateChange = SEQ_ACT_STT_IDLE;
		}
		else{
			CSrvcMsgActivity_Progress(&(pThis->mActivity));
		}
	}
	else{
		// No Action
	}

	return stateChange;
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
