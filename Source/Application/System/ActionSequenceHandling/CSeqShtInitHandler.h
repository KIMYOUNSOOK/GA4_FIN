// File Name:	CSeqShtInitHandler.h
// Description:	Header File of Sheet Initialize Handler
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.22
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CSeqShtInitHandler_h
#define CSeqShtInitHandler_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ESeqActionState.h"
#include "SRVC_MsgActivity.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Spec data of Device Initialize Handler ---
typedef struct /*SeqShtInitHandlerSpec*/
{
	const SrvcMsgActivity* pMsgActivity;					// �A�N�e�B�r�e�B�N���X�C���X�^���X�|�C���^
	UC msgActivityNum;										// �A�N�e�B�r�e�B�N���X�C���X�^���X�̃A�N�e�B�r�e�B��
	void (*TimerCallBack)(SS, UL);							// �҂����Ԍo�ߒʒm�R�[���o�b�N�֐��|�C���^
} SeqShtInitHandlerSpec;

// --- Sheet Initialize Handler ---
typedef struct /*CSeqShtInitHandler*/
{
	const SeqShtInitHandlerSpec* mpSpec;					// Pointer of Spec Data
	CSrvcMsgActivity mActivity;								// Activity
	EvtParamSheetActionRequest mEventParam;					// Event Paraeter of Sheet Action Request
	UC mState;												// Sheet Initialize State
} CSeqShtInitHandler;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CSeqShtInitHandler_Constructor(CSeqShtInitHandler* pThis, const SeqShtInitHandlerSpec* pSpec);

ESeqActionState CSeqShtInitHandler_GetState(CSeqShtInitHandler* pThis);
UC CSeqShtInitHandler_GetSheetNo(CSeqShtInitHandler* pThis);
ESeqActionState CSeqShtInitHandler_Start(CSeqShtInitHandler* pThis, UC sheetNo);
ESeqActionState CSeqShtInitHandler_Stop(CSeqShtInitHandler* pThis, UC sheetNo);
ESeqActionState CSeqShtInitHandler_SheetActionResponse(CSeqShtInitHandler* pThis, UC sheetNo, US eventID, UC moduleID);


#endif // #ifndef CSeqShtInitHandler_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
