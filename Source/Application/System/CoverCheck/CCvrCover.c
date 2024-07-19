// File Name:	CCvrCover.c
// Description:	Implimentation File of Cover
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCvrCover.h"

#include "DD_DIO.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

typedef enum /*ECvrCoverState*/
{
	CVR_STT_CLOSE,						// close
	CVR_STT_OPEN,						// open
	CVR_STT_CLOSE_CHECK,				// close detect check
	CVR_STT_FORCED_CLOSE,				// forced close
	CVR_STT_FORCED_OPEN					// forced open
} ECvrCoverState;


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
// Function Name : CCvrCover_Constructor
// Description   : �R���X�g���N�^
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : pSpec (�X�y�b�N�f�[�^�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CCvrCover_Constructor(CCvrCover* pThis, const CvrCoverSpec* pSpec)
{
	pThis->mpSpec = pSpec;

	pThis->mState = CVR_STT_CLOSE;
	pThis->mCount = pThis->mpSpec->detectCount;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCvrCover_GetInputID
// Description   : �C���v�b�g�M�����ʎq�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Input ID
///////////////////////////////////////////////////////////////////
DD_DIO_InputPortID CCvrCover_GetInputID(CCvrCover* pThis)
{
	return pThis->mpSpec->inputID;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCvrCover_IsSafetyInterlock
// Description   : �C���^�[���b�N���ǂ����̎擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : true = �C���^�[���b�N�ł���
//               : false = �C���^�[���b�N�łȂ�
///////////////////////////////////////////////////////////////////
bool CCvrCover_IsSafetyInterlock(CCvrCover* pThis)
{
	return (bool)(pThis->mpSpec->saftyInterlock);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCvrCover_GetStatus
// Description   : �X�e�[�^�X�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Cover Status
///////////////////////////////////////////////////////////////////
ECvrCoverStatus CCvrCover_GetStatus(CCvrCover* pThis)
{
	if ( (pThis->mState == CVR_STT_CLOSE) || (pThis->mState == CVR_STT_FORCED_CLOSE) ) return CVR_STTS_CLOSE;
	else																			   return CVR_STTS_OPEN;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCvrCover_Initialize
// Description   : ������Ԍ��菈��
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
bool CCvrCover_Initialize(CCvrCover* pThis)
{
	if( DD_DIO_GetInput(pThis->mpSpec->inputID) == OFF ){
		pThis->mState = CVR_STT_CLOSE;

		return false;															// Default Status
	}
	else{
		pThis->mState = CVR_STT_OPEN;

		return true;
	}	
}

///////////////////////////////////////////////////////////////////
// Function Name : CCvrCover_NotifyEdge
// Description   : �Z���T�ω��ʒm��M����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : edge (�Z���T�G�b�W)
// Return        : true = Cover Status�ɕω��������� /
//               : false = Cover Status�ɕω����Ȃ�����
///////////////////////////////////////////////////////////////////
bool CCvrCover_NotifyEdge(CCvrCover* pThis, UC edge)
{
	ECvrCoverStatus status;

	status = CCvrCover_GetStatus(pThis);

	if ( pThis->mState < CVR_STT_CLOSE_CHECK ){
		if( edge == ON ){														// Open Level
			pThis->mState = CVR_STT_OPEN;
			pThis->mCount = pThis->mpSpec->detectCount;
		}
		else{																	// Close Level
			if( pThis->mState == CVR_STT_OPEN ){
				pThis->mState = CVR_STT_CLOSE_CHECK;
				pThis->mCount = pThis->mpSpec->detectCount;
			}
			else{
				// No Action.
			}
		}
	}
	else{																		// Forced state
		// No Action
	}

	if ( status != CCvrCover_GetStatus(pThis) ) return true;
	else										return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCvrCover_CloseCheck
// Description   : �N���[�Y�m��`�F�b�N����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : true = Cover Status�ɕω��������� /
//               : false = Cover Status�ɕω����Ȃ�����
///////////////////////////////////////////////////////////////////
bool CCvrCover_CloseCheck(CCvrCover* pThis)
{
	if( pThis->mState == CVR_STT_CLOSE_CHECK ){
		if( DD_DIO_GetInput(pThis->mpSpec->inputID) == OFF ){					// Close Level
			if( --(pThis->mCount) == 0 ){
				pThis->mState = CVR_STT_CLOSE;									// Close�m��
				return true;
			}
			else{
				// No Action
			}
		}
		else{																	// Open Level
			// No Action(�G�b�W�ʒm��M����(NotifyEdge())�ŏ�������)
		}
	}
	else{
		// No Action
	}

	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCvrCover_DesignateStatus
// Description   : �J�o�[��ԋ����w��w����M������
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : status (�X�e�[�^�X)
// Return        : true = Cover Status�ɕω���������
//               : false = Cover Status�ɕω����Ȃ�����
///////////////////////////////////////////////////////////////////
bool CCvrCover_DesignateStatus(CCvrCover* pThis, ECvrCoverStatus status)
{
	ECvrCoverStatus currentStatus;

	currentStatus = CCvrCover_GetStatus(pThis);

	if ( status == CVR_STTS_CLOSE ) pThis->mState = CVR_STT_FORCED_CLOSE;
	else							pThis->mState = CVR_STT_FORCED_OPEN;

	if ( currentStatus != CCvrCover_GetStatus(pThis) ) return true;
	else											   return false;
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.04.11	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 14.12.29	| Yuichi Hoshino	| Design Improvement [ �����X�e�[�^�X�ύX���������� ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
