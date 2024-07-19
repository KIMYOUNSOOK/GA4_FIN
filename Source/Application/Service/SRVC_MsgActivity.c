// File Name: SRVC_MsgActivity.cpp
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 14.05.14
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#include "FinisherCommon.h"
#include "MSP_Service.h"
#include "SRVC_MsgActivity.h"

//==============================================================================
// Function Name : CSrvcMsgActivity_Constructor
// Description   : �R���X�g���N�^
// Parameter     : 
// Return        : -
//==============================================================================
void CSrvcMsgActivity_Constructor(CSrvcMsgActivity* pThis, UC userMID, UC tableSize, SrvcMsgActivity* pTable, void (*pCallBack)(SS timerID, UL idx))
{
	pThis->mUserMID = userMID;
	pThis->mTableSize = tableSize;
	pThis->mCrntPattern = 0;
	pThis->mStartPattern = 0;
	pThis->mIsActive = false;
	pThis->mpMsgData = NULL;
	pThis->mMsgDataSize = 0;
	pThis->mpTable = pTable;
	pThis->mpTimerCallBack = pCallBack;
}

//==============================================================================
// Function Name : CSrvcMsgActivity::Start
// Description   : �֘A�p�����[�^������������Activity���J�n������
// Parameter     : -
// Return        : -
//==============================================================================
void CSrvcMsgActivity_Start(CSrvcMsgActivity* pThis)
{ 
	pThis->mCrntPattern = 0;
	pThis->mStartPattern = 0;
	pThis->mIsActive = true;
	CSrvcMsgActivity_Progress(pThis);
}

//==============================================================================
// Function Name : CSrvcMsgActivity::Progress
// Description   : Activity��i�s������
// Parameter     : -
// Return        : -
//==============================================================================
void CSrvcMsgActivity_Progress(CSrvcMsgActivity* pThis)
{
	UC idx;

	if (pThis->mIsActive) {
		for (idx = 0; idx < pThis->mTableSize; idx++) {
			if ((pThis->mStartPattern & SRVC_ID2BIT(idx)) == 0) {										// Action�����J�n�̏ꍇ
				if ((pThis->mCrntPattern & pThis->mpTable[idx].startTrgPattern) == pThis->mpTable[idx].startTrgPattern) {
																										// �J�n��������
					pThis->mStartPattern |= SRVC_ID2BIT(idx);

					if (pThis->mpTable[idx].pWaitTime != 0) {											// �҂����Ԃ���
						MSP_SetEventTimer(*(pThis->mpTable[idx].pWaitTime), pThis->mpTimerCallBack, idx);
					}
					else {																				// �҂����ԂȂ�
						if (pThis->mpTable[idx].sendEventID != MSP_EVT_NUM_VOID) {
							MSP_SendMessage(pThis->mUserMID, pThis->mpTable[idx].sendEventID, (const UC*)(pThis->mpMsgData), pThis->mMsgDataSize, LOG_ON);
						}
					}
				}
				else {/* �J�n�����s�����ł���Ή������Ȃ� */}
			}
			else {/* ����Action���������Ă���̂ŉ������Ȃ� */}
		}
	}
}

//==============================================================================
// Function Name : CSrvcMsgActivity::SetTrigger
// Description   : ��M�g���K��ݒ肷��
// Parameter     : recvEventID	"��M�g���KID"
//               : senderID		"���M���̃��W���[��ID"
// Return        : -
//==============================================================================
void CSrvcMsgActivity_SetTrigger(CSrvcMsgActivity* pThis, US recvEventID, UC senderID)
{
	UC idx;

	if (pThis->mIsActive) {
		for (idx = 0; idx < pThis->mTableSize; idx++) {
			if (( recvEventID == pThis->mpTable[idx].recvEventID ) 
				&& ( senderID == pThis->mpTable[idx].senderID )){
				pThis->mCrntPattern |= SRVC_ID2BIT(idx);
				break;
			}
		}
	}
}

//==============================================================================
// Function Name : CSrvcMsgActivity::TimeOut
// Description   : ���b�Z�[�W���M�҂����Ԍチ�b�Z�[�W�𑗐M����
// Parameter     : idx	"Activity�e�[�u���̃C���f�b�N�X"
// Return        : -
//==============================================================================
void CSrvcMsgActivity_TimeOut(CSrvcMsgActivity* pThis, UL idx)
{
	if (pThis->mIsActive) {
		if (pThis->mpTable[idx].sendEventID != MSP_EVT_NUM_VOID) {
			MSP_SendMessage(pThis->mUserMID, pThis->mpTable[(UC)idx].sendEventID, (const UC*)pThis->mpMsgData, pThis->mMsgDataSize, LOG_ON);
		}
	}
}

//==============================================================================
// Function Name : CSrvcMsgActivity::CheckEnd
// Description   : Activity�̊������m�F����
// Parameter     : -
// Return        : ����:true / ������:false
//==============================================================================
bool CSrvcMsgActivity_CheckEnd(CSrvcMsgActivity* pThis)
{
	if (pThis->mIsActive) {
//		return (mCrntPattern == pThis->mEndPattern);
		return (pThis->mCrntPattern == SRVC_END_BIT_PATTERN(pThis->mTableSize));
	}
	else {
		return true;
	}
}

//==============================================================================
// Function Name : CSrvcMsgActivity::CSrvcMsgActivity_Abort
// Description   : Activity�𒆎~������
// Parameter     : -
// Return        : -
//==============================================================================
void CSrvcMsgActivity_Abort(CSrvcMsgActivity* pThis)
{
	UL pattern;

	if (pThis->mIsActive) {
		pThis->mCrntPattern |= (~(pThis->mStartPattern) & SRVC_END_BIT_PATTERN(pThis->mTableSize));
	}
}

//==============================================================================
// Function Name : CSrvcMsgActivity::Terminate
// Description   : Activity���I��������
// Parameter     : -
// Return        : -
//==============================================================================
void CSrvcMsgActivity_Terminate(CSrvcMsgActivity* pThis)
{ 
	pThis->mIsActive = false;
}

//==============================================================================
// Function Name : CSrvcMsgActivity::SetData
// Description   : ���b�Z�[�W�f�[�^���Z�b�g����B
// Parameter     : -
// Return        : -
//==============================================================================
void CSrvcMsgActivity_SetData(CSrvcMsgActivity* pThis, UC* pData, UC size)
{ 
	pThis->mpMsgData	= pData;
	pThis->mMsgDataSize = size;
}
// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+-----------------------------------------------------------------------------------------------------------------------------
// 14.05.14 | Nishizawa Mitsuyuki | Create This File.
// 15.06.19	| Yuichi Hoshino      | Trouble Action [ Abort�w����ɃA�N�e�B�r�e�B��~���� ]
// 15.12.24	| Yuichi Hoshino      | Design Improvement [ MSP_SendMessage()�̃��O���I�t�ɕύX ]

