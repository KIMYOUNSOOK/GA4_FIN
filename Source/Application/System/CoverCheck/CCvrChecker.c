// File Name:	CCvrChecker.c
// Description:	Implimentation File of Cover Checker
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCvrChecker.h"

#include "CSysManager.h"
#include "COputInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Cover Spec ---
extern CvrCoverSpec const cCvr_CoverSpec[CVR_ID_NUM];

// --- Device Status No. of Cover ---
extern US const cCvr_DeviceStatusNo[CVR_ID_NUM];

// --- Option ID that cover is in --- 
extern UC const cCvr_OptionID[CVR_ID_NUM];

// --- Member of Cover Checker ---
typedef struct /*CCvrCheckerMember*/
{
	CCvrCover mCover[CVR_ID_NUM];
} CCvrCheckerMember;

static CCvrCheckerMember gCvr_CheckerMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Timer Event Call Back Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CVR_CloseCyclicCheck(SS timerID, UL notUsed);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CCvrChecker_Constructor
// Description   : �R���X�g���N�^
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CCvrChecker_Constructor()
{
	UC coverID;

	for ( coverID = 0; coverID < CVR_ID_NUM; coverID++ ){
		CCvrCover_Constructor(&(gCvr_CheckerMember.mCover[coverID]), &(cCvr_CoverSpec[coverID]));
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CCvrChecker_GetDeviceStatusNo
// Description   : �e�J�o�[�ɑΉ�����Device Status No.�擾����
// Parameter     : coverID (Cover ID)
// Return        : Device Status No.
///////////////////////////////////////////////////////////////////
US CCvrChecker_GetDeviceStatusNo(ECvrCoverID coverID)
{
//	if ( coverID < CVR_ID_NUM ){												// System Module���ł̂ݎg�p���郁�\�b�h�Ȃ̂Ń`�F�b�N���Ȃ�(���W���[�������ŗ}������)
		return cCvr_DeviceStatusNo[coverID];
//	}
//	else{
//		return DSNO_NUM;
//	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CCvrChecker_GetCoverStatus
// Description   : �J�o�[��Ԏ擾����
// Parameter     : coverID (Cover ID)
// Return        : Cover Status
///////////////////////////////////////////////////////////////////
ECvrCoverStatus CCvrChecker_GetCoverStatus(ECvrCoverID coverID)
{
	UC index;

	if ( coverID < CVR_ID_NUM ){
		return CCvrCover_GetStatus(&(gCvr_CheckerMember.mCover[coverID]));
	}
	else{																		// Check all cover close
		for ( index = 0; index < CVR_ID_NUM; index++ ){
			if ( CCvrCover_GetStatus(&(gCvr_CheckerMember.mCover[index])) == CVR_STTS_OPEN ){
				return CVR_STTS_OPEN;
			}
			else{ /* Go for loop */ }
		}

		return CVR_STTS_CLOSE;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CCvrChecker_DetectInitialStatus
// Description   : ������Ԍ��菈��
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CCvrChecker_DetectInitialStatus()
{
	UC currentAllStts, coverID, sttsChange, anyChange;

	anyChange = false;

	currentAllStts = CCvrChecker_GetCoverStatus(CVR_ID_NUM);

	for ( coverID = 0; coverID < CVR_ID_NUM; coverID++ ){
		if ( COputInformer_GetOptionDetect((EOptionID)(cCvr_OptionID[coverID])) == true ){
			sttsChange = CCvrCover_Initialize(&(gCvr_CheckerMember.mCover[coverID]));
		}
		else{																	// ����I�v�V�������ڑ�
			sttsChange = CCvrCover_DesignateStatus(&(gCvr_CheckerMember.mCover[coverID]), CVR_STTS_CLOSE);
		}

		if ( sttsChange == true ){
			CSysManager_CoverStatusChange((ECvrCoverID)coverID, CCvrCover_GetStatus(&(gCvr_CheckerMember.mCover[coverID])));
			anyChange = true;
		}
		else{
			// No Action
		}
	}

	if ( (currentAllStts == CVR_STTS_CLOSE) && (anyChange == true) ){
		MSP_SetEventTimer(100, CVR_CloseCyclicCheck, 0);						// 100[ms] cycle
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CCvrChecker_EndCheck
// Description   : �J�o�[��Ԍ��m�����̏I���w����M������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CCvrChecker_EndCheck()
{
	UC coverID;

	for ( coverID = 0; coverID < CVR_ID_NUM; coverID++ ){
		CCvrCover_DesignateStatus(&(gCvr_CheckerMember.mCover[coverID]), CCvrCover_GetStatus(&(gCvr_CheckerMember.mCover[coverID])));
	}																			// ����Ԃ̂܂܌��m������~
}

///////////////////////////////////////////////////////////////////
// Function Name : CCvrChecker_DiscardOption
// Description   : �w��I�v�V�����̃J�o�[��Ԍ��m�����̒�~�w����M
//               : ������
// Parameter     : optionID (Option ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CCvrChecker_DiscardOption(EOptionID optionID)
{
	UC coverID;

	for ( coverID = 0; coverID < CVR_ID_NUM; coverID++ ){
		if ( cCvr_OptionID[coverID] == optionID ){
			if ( CCvrCover_DesignateStatus(&(gCvr_CheckerMember.mCover[coverID]), CVR_STTS_CLOSE) == true ){
				CSysManager_CoverStatusChange((ECvrCoverID)coverID, CCvrCover_GetStatus(&(gCvr_CheckerMember.mCover[coverID])));
			}
			else{
				// No Action
			}
		}
		else{
			// No Action
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CCvrChecker_NotifyEdge
// Description   : �Z���T���x���ω��ʒm��M����
// Parameter     : coverID (Cover ID)
//               : edge (Sensor Edge)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CCvrChecker_NotifyEdge(ECvrCoverID coverID, UC edge)
{
	UC currentAllStts;

	currentAllStts = CCvrChecker_GetCoverStatus(CVR_ID_NUM);

	if ( CCvrCover_NotifyEdge(&(gCvr_CheckerMember.mCover[coverID]), edge) == true ){
		CSysManager_CoverStatusChange((ECvrCoverID)coverID, CCvrCover_GetStatus(&(gCvr_CheckerMember.mCover[coverID])));

		if ( currentAllStts == CVR_STTS_CLOSE ){
			MSP_SetEventTimer(100, CVR_CloseCyclicCheck, 0);					// 100[ms] cycle
		}
		else{
			// No Action
		}
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CCvrChecker_CloseCyclicCheck
// Description   : �N���[�Y�m������`�F�b�N����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CCvrChecker_CloseCyclicCheck()
{
	UC coverID;

	for ( coverID = 0; coverID < CVR_ID_NUM; coverID++ ){
		if( CCvrCover_CloseCheck(&(gCvr_CheckerMember.mCover[coverID])) == true ){
			CSysManager_CoverStatusChange((ECvrCoverID)coverID, CCvrCover_GetStatus(&(gCvr_CheckerMember.mCover[coverID])));
		}
		else{
			// No Action
		}
	}

	if ( CCvrChecker_GetCoverStatus(CVR_ID_NUM) == CVR_STTS_OPEN ){
		MSP_SetEventTimer(100, CVR_CloseCyclicCheck, 0);						// 100[ms] cycle
	}
	else{
		// No Action
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
// Timer Event Call Back Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CVR_CloseCyclicCheck
// Description   : �^�C�}�[�C�x���g�R�[���o�b�N
// Parameter     : timerID (Timer ID)
//               : notUsed (Not Used)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CVR_CloseCyclicCheck(SS timerID, UL notUsed)
{
	CCvrChecker_CloseCyclicCheck();
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.04.14	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.03.30	| Yuichi Hoshino	| Modify by Trouble Action [ cCvr_OptionID[]�̔z��T�C�Y�w��ԈႢ ]
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ Power Down��X�e�[�^�X��ʒm�h�~ ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
