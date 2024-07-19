// File Name:	CSeqProcEndHandler.c
// Description:	Implimentation File of Process End Action Handler
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.22
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSeqProcEndHandler.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Spec data of Process End Action Handler ---
extern SeqProcEndHandlerSpec const cSeq_ProcEndHandlerSpec;

// --- Member of Process End Action Handler ---
typedef struct /*CSeqProcEndHandlerMember*/
{
	CSrvcMsgActivity mActivity;								// Activity
	UC mState;												// Device Initialize State
} CSeqProcEndHandlerMember;

static CSeqProcEndHandlerMember gSeq_ProcEndHandlerMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Timer Event Call Back Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void SEQ_ProcEndActivityTimeOut(SS timerID, UL idx);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CSeqProcEndHandler_Constructor
// Description   : �R���X�g���N�^
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CSeqProcEndHandler_Constructor()
{
	gSeq_ProcEndHandlerMember.mState = SEQ_ACT_STT_IDLE;

	CSrvcMsgActivity_Constructor(&(gSeq_ProcEndHandlerMember.mActivity), MID_SYS, cSeq_ProcEndHandlerSpec.msgActivityNum, cSeq_ProcEndHandlerSpec.pActivity, SEQ_ProcEndActivityTimeOut);
}

///////////////////////////////////////////////////////////////////
// Function Name : CSeqProcEndHandler_GetStatus
// Description   : �v���Z�X�G���h�����Ԏ擾����
// Parameter     : �]
// Return        : �v���Z�X�G���h������
///////////////////////////////////////////////////////////////////
ESeqActionState CSeqProcEndHandler_GetState()
{
	return gSeq_ProcEndHandlerMember.mState;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSeqProcEndHandler_Start
// Description   : �v���Z�X�G���h����J�n�w����M������
// Parameter     : �]
// Return        : SEQ_ACT_STT_NUM = �C�j�V�����C�Y�����ԕω��Ȃ�
//               : SEQ_ACT_STT_NUM �ȊO = �J�ڐ�C�j�V�����C�Y����
//               : ���
///////////////////////////////////////////////////////////////////
ESeqActionState CSeqProcEndHandler_Start()
{
	UC stateChange;

	stateChange = SEQ_ACT_STT_NUM;

	if ( gSeq_ProcEndHandlerMember.mState == SEQ_ACT_STT_IDLE ){
		gSeq_ProcEndHandlerMember.mState = SEQ_ACT_STT_RUNNING;
		CSrvcMsgActivity_Start(&(gSeq_ProcEndHandlerMember.mActivity));

		stateChange = SEQ_ACT_STT_RUNNING;
	}
	else{
		// No Action
	}

	return stateChange;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSeqProcEndHandler_DeviceActionResponse
// Description   : Device Action Response�C�x���g��M������
// Parameter     : eventID (MSP Event ID)
//               : moduleID (Device Module ID)
// Return        : SEQ_ACT_STT_NUM = �C�j�V�����C�Y�����ԕω��Ȃ�
//               : SEQ_ACT_STT_NUM �ȊO = �J�ڐ�C�j�V�����C�Y����
//               : ���
///////////////////////////////////////////////////////////////////
ESeqActionState CSeqProcEndHandler_DeviceActionResponse(US eventID, UC moduleID)
{
	UC stateChange;

	stateChange = SEQ_ACT_STT_NUM;

	if ( gSeq_ProcEndHandlerMember.mState == SEQ_ACT_STT_RUNNING ){
		CSrvcMsgActivity_SetTrigger(&(gSeq_ProcEndHandlerMember.mActivity), eventID, moduleID);

		if ( CSrvcMsgActivity_CheckEnd(&(gSeq_ProcEndHandlerMember.mActivity)) == true ){
																				// �V�[�P���X����
			gSeq_ProcEndHandlerMember.mState = SEQ_ACT_STT_IDLE;
			stateChange = SEQ_ACT_STT_IDLE;
		}
		else{
			CSrvcMsgActivity_Progress(&(gSeq_ProcEndHandlerMember.mActivity));
		}
	}
	else{
		// No Action
	}

	return stateChange;
}


/////////////////////////////////////////////////////////////////////////////////////////
// Timer Event Call Back Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : SEQ_ProcEndActivityTimeOut
// Description   : �A�N�e�B�r�e�B�J�n�҂����Ԍo�ߒʒm�R�[���o�b�N��
//               : ��
// Parameter     : timerID (MSP Timer ID)
//               : idx (�A�N�e�B�r�e�B�Ǘ��e�[�u���C���f�b�N�X)
// Return        : �]
///////////////////////////////////////////////////////////////////
void SEQ_ProcEndActivityTimeOut(SS timerID, UL idx)
{
	CSrvcMsgActivity_TimeOut(&(gSeq_ProcEndHandlerMember.mActivity), idx);
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
