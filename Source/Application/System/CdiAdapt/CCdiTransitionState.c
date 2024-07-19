// File Name:	CCdiTransitionState.c
// Description:	Implimentation File of Transition State
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.24
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCdiTransitionState.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CCdiTransitionState_Constructor
// Description   : �R���X�g���N�^
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CCdiTransitionState_Constructor(CCdiTransitionState* pThis)
{
	pThis->mState = eCdiFinTransStt_Idle;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiTransitionState_GetState
// Description   : ��Ԏ擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Transition State
///////////////////////////////////////////////////////////////////
UC CCdiTransitionState_GetState(CCdiTransitionState* pThis)
{
	return pThis->mState;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiTransitionState_InformedEvent
// Description   : ��ԑJ�ڃC�x���g�ʒm��M����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : event (��ԑJ�ڃC�x���g���ʎq)
// Return        : true = ��ԑJ�ڂ���
//               : false = ��ԑJ�ڂȂ�
///////////////////////////////////////////////////////////////////
bool CCdiTransitionState_InformedEvent(CCdiTransitionState* pThis, ECdiTransSttEvt event)
{
//															| CycleUpStart Event		| CycleUpComp Event			| CycleDownStart Event		| HardDownStart Event		| DownComp Event
	static const UC cIdleChg[CDI_TRANS_STT_EVT_NUM] = {		eCdiFinTransStt_CycUp,		eCdiFinTransStt_Idle,		eCdiFinTransStt_CycDwn,		eCdiFinTransStt_HardDwn,	eCdiFinTransStt_Idle	};
	static const UC cCycleUpChg[CDI_TRANS_STT_EVT_NUM] = {	eCdiFinTransStt_CycUp,		eCdiFinTransStt_Idle,		eCdiFinTransStt_CycDwn,		eCdiFinTransStt_HardDwn,	eCdiFinTransStt_CycUp	};
	static const UC cCycleDownChg[CDI_TRANS_STT_EVT_NUM] = {eCdiFinTransStt_CycDwn,		eCdiFinTransStt_CycDwn,		eCdiFinTransStt_CycDwn,		eCdiFinTransStt_HardDwn,	eCdiFinTransStt_Idle	};
	static const UC cHardDownChg[CDI_TRANS_STT_EVT_NUM] = {	eCdiFinTransStt_HardDwn,	eCdiFinTransStt_HardDwn,	eCdiFinTransStt_HardDwn,	eCdiFinTransStt_HardDwn,	eCdiFinTransStt_Idle	};

	UC currentState = pThis->mState;

	if ( event < CDI_TRANS_STT_EVT_NUM ){
		switch ( pThis->mState ){
		case eCdiFinTransStt_Idle:		pThis->mState = cIdleChg[event];		break;
		case eCdiFinTransStt_CycUp:		pThis->mState = cCycleUpChg[event];		break;
		case eCdiFinTransStt_CycDwn:	pThis->mState = cCycleDownChg[event];	break;
		case eCdiFinTransStt_HardDwn:	pThis->mState = cHardDownChg[event];	break;
		default:						/* No Action */							break;
		}
	}
	else{ /* No Action */ }

	if ( currentState != pThis->mState ) return true;							// State change
	else								 return false;							// State don't change
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
