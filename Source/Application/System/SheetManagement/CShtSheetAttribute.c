// File Name:	CShtSheetAttribute.c
// Description:	Implimentation File of Sheet Attribute
// Engineer:	Yuichi Hoshino
// Last Edit:	14.02.10
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CShtSheetAttribute.h"


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
// Function Name : CShtSheetAttribute_Constructor
// Description   : �R���X�g���N�^
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtSheetAttribute_Constructor(CShtSheetAttribute* pThis)
{
	CShtSheetAttribute_Reset(pThis);
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheetAttribute_Reset
// Description   : ���Z�b�g����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtSheetAttribute_Reset(CShtSheetAttribute* pThis)
{
	UC index;

	pThis->mOffsetMode = eCdiFinOffset_Off;										// Offset Mode
	pThis->mStapleMode = eCdiFinStaple_Off;										// Staple Mode
	pThis->mPunchMode = eCdiFinPunch_Off;										// Punch Mode
	pThis->mBookletMode = eCdiFinBook_Off;										// Booklet Maker Mode
	pThis->mFoldMode = eCdiFinFold_Off;											// Fold Mode
	pThis->mMediaType = eCdiFinMediaType_Plain;									// Media Type
	pThis->mMediaWeight = 80;													// Media Weight[x1gsm]
	pThis->mMediaInfo = 0x00;													// Bit0�FEnvelope
																				// Bit1�FTabbed Paper
																				// Bit2�FReused Paper
																				// Bit3�FPrepunched Material
	pThis->mProcessSize = 2100;													// Process Direction Media Size[x0.1mm]
	pThis->mCrossProcessSize = 2970;											// Cross Process Direction Media Size[x0.1mm]
	pThis->mCurl = 0;															// Sheet Curl
	pThis->mCurlQuantity = 0;													// Sheet Curl Quantity
	pThis->mInitialSpeed = 3500;												// Initial Sheet Exit Speed[x0.1mm/s]
	pThis->mSpeedChangeTimes = 0;												// Speed Change Times
// 20160225 V3.4.0 Merge [ Change Speed��񏉊����~�X ]
	for (index = 0; index < 5; index++){
		pThis->mChangeSpeedTmg[index] = 0;										// Change Speed Timing *
		pThis->mChangeSpeed[index] = 0;											// Change Speed *
	}

	pThis->mSheetIntegrity = eCdiFinIntegrity_Normal;							// Sheet Integrity
	pThis->mSetAction = eCdiFinSetAct_NoAct;									// Set Action
	pThis->mOffsetPosition = eCdiFinOffsetPos_Current;							// Offset Position
	pThis->mExitSheetInterval = 0;												// Exit Sheet Interval[x1ms]
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheetAttribute_CheckDiff
// Description   : �A�g���r���[�g���Ԃ̍����`�F�b�N����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : pAttr (��r�ΏۃC���X�^���X�|�C���^)
// Return        : true = ��������
//               : false = �����Ȃ�
///////////////////////////////////////////////////////////////////
bool CShtSheetAttribute_CheckDiff(CShtSheetAttribute* pThis, CShtSheetAttribute* pAttr)
{
	if ( (pThis->mOffsetMode != pAttr->mOffsetMode) ||
		 (pThis->mStapleMode != pAttr->mStapleMode) ||
		 (pThis->mPunchMode != pAttr->mPunchMode) ||
		 (pThis->mBookletMode != pAttr->mBookletMode) ||
		 (pThis->mFoldMode != pAttr->mFoldMode) ||
		 (pThis->mMediaType != pAttr->mMediaType) ||
		 (pThis->mMediaWeight != pAttr->mMediaWeight) ||
		 (pThis->mMediaInfo != pAttr->mMediaInfo) ||
		 (pThis->mProcessSize != pAttr->mProcessSize) ||
		 (pThis->mCrossProcessSize != pAttr->mCrossProcessSize) ||
		 (pThis->mCurl != pAttr->mCurl) ||
		 (pThis->mCurlQuantity != pAttr->mCurlQuantity) ||
		 (pThis->mInitialSpeed != pAttr->mInitialSpeed) ||
		 (pThis->mSpeedChangeTimes != pAttr->mSpeedChangeTimes)  ||
		 (pThis->mChangeSpeedTmg[0] != pAttr->mChangeSpeedTmg[0]) ||
		 (pThis->mChangeSpeedTmg[1] != pAttr->mChangeSpeedTmg[1]) ||
		 (pThis->mChangeSpeedTmg[2] != pAttr->mChangeSpeedTmg[2]) ||
		 (pThis->mChangeSpeedTmg[3] != pAttr->mChangeSpeedTmg[3]) ||
		 (pThis->mChangeSpeedTmg[4] != pAttr->mChangeSpeedTmg[4]) ||
		 (pThis->mChangeSpeed[0] != pAttr->mChangeSpeed[0]) ||
		 (pThis->mChangeSpeed[1] != pAttr->mChangeSpeed[1]) ||
		 (pThis->mChangeSpeed[2] != pAttr->mChangeSpeed[2]) ||
		 (pThis->mChangeSpeed[3] != pAttr->mChangeSpeed[3]) ||
		 (pThis->mChangeSpeed[4] != pAttr->mChangeSpeed[4]) ||
		 (pThis->mSheetIntegrity != pAttr->mSheetIntegrity) ||
		 (pThis->mSetAction != pAttr->mSetAction) ||
		 (pThis->mOffsetPosition != pAttr->mOffsetPosition) ||
		 (pThis->mExitSheetInterval != pAttr->mExitSheetInterval) ){
		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheetAttribute_CheckDiffOutSheet
// Description   : �A�g���r���[�g���Ԃ̍����`�F�b�N����(�㏈����
//               : ���O�����Ɋւ����̂�)
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : pAttr (��r�ΏۃC���X�^���X�|�C���^)
// Return        : true = ��������
//               : false = �����Ȃ�
///////////////////////////////////////////////////////////////////
bool CShtSheetAttribute_CheckDiffOutSheet(CShtSheetAttribute* pThis, CShtSheetAttribute* pAttr)
{
	if ( (pThis->mOffsetMode != pAttr->mOffsetMode) ||
		 (pThis->mStapleMode != pAttr->mStapleMode) ||
		 (pThis->mPunchMode != pAttr->mPunchMode) ||
		 (pThis->mBookletMode != pAttr->mBookletMode) ||
		 (pThis->mFoldMode != pAttr->mFoldMode) ||
		 (pThis->mMediaType != pAttr->mMediaType) ||
		 (pThis->mMediaWeight != pAttr->mMediaWeight) ||
		 (pThis->mMediaInfo != pAttr->mMediaInfo) ||
		 (pThis->mProcessSize != pAttr->mProcessSize) ||
		 (pThis->mCrossProcessSize != pAttr->mCrossProcessSize) ||
		 (pThis->mCurl != pAttr->mCurl) ||
		 (pThis->mCurlQuantity != pAttr->mCurlQuantity) ||
		 (pThis->mInitialSpeed != pAttr->mInitialSpeed) ||
		 (pThis->mOffsetPosition != pAttr->mOffsetPosition) ||
		 (pThis->mExitSheetInterval != pAttr->mExitSheetInterval) ){
		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheetAttribute_GetChangeSpeedTiming
// Description   : �C�O�W�b�g���x�ύX�^�C�~���O�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : times (���x�ύX�񐔂̎w��ԍ�)
// Return        : ���x�ύX�^�C�~���O[�~1ms]
///////////////////////////////////////////////////////////////////
US CShtSheetAttribute_GetChangeSpeedTiming(CShtSheetAttribute* pThis, UC times)
{
	if ( (times > 0) && (times <= 5) ) return pThis->mChangeSpeedTmg[times - 1];
	else							   return 0;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheetAttribute_SetChangeSpeedTiming
// Description   : �C�O�W�b�g���x�ύX�^�C�~���O�Z�b�g����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : timing (���x�ύX�^�C�~���O[�~1ms])
//               : times (���x�ύX�񐔂̎w��ԍ�)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtSheetAttribute_SetChangeSpeedTiming(CShtSheetAttribute* pThis, US timing, UC times)
{
	if ( (times > 0) && (times <= 5) ) pThis->mChangeSpeedTmg[times - 1] = timing;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheetAttribute_GetChangeSpeed
// Description   : �C�O�W�b�g���x�̕ύX���x�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : times (���x�ύX�񐔂̎w��ԍ�)
// Return        : �ύX���x[�~1mm/s]
///////////////////////////////////////////////////////////////////
US CShtSheetAttribute_GetChangeSpeed(CShtSheetAttribute* pThis, UC times)
{
	if ( (times > 0) && (times <= 5) ) return pThis->mChangeSpeed[times - 1];
	else							   return 0;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheetAttribute_SetChangeSpeed
// Description   : �C�O�W�b�g���x�̕ύX���x�Z�b�g����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : speed (�ύX���x[�~1mm/s])
//               : times (���x�ύX�񐔂̎w��ԍ�)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtSheetAttribute_SetChangeSpeed(CShtSheetAttribute* pThis, US speed, UC times)
{
	if ( (times > 0) && (times <= 5) ) pThis->mChangeSpeed[times - 1] = speed;
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
// 15.06.19	| Yuichi Hoshino	| Trouble Action [ Change Speed��񏉊����~�X ]
