// File Name:	CShtPathSensor.c
// Description:	Implimentation File of Path Sensor
// Engineer:	Yuichi Hoshino
// Last Edit:	15.01.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CShtPathSensor.h"

#include "DD_DIO.h"

#include "CShtManager.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Step of late detect ---
typedef enum /*EShtPathSnrLateDetectStep*/
{
	SHT_LATE_DETECT_IDLE,				// �x�~���
	SHT_LATE_DETECT_WAIT,				// �G�b�W�m��҂����
	SHT_LATE_DETECT_TIMEUP,
	SHT_LATE_DETECT_NUM
} EShtPathSnrLateDetectStep;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Timer Event Call Back Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void SHT_PathSensorLateDetectTimeUp(SS timerID, UL parameter);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_Constructor
// Description   : �R���X�g���N�^
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : pSpec (�X�y�b�N�f�[�^�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtPathSensor_Constructor(CShtPathSensor* pThis, const ShtPathSensorSpec* pSpec)
{
	UC index;

	pThis->mpSpec				 = pSpec;
	pThis->mWaitEdge			 = SHT_EDGE_LEAD;
	pThis->mReverseFilterBit	 = 0x00;
	pThis->mLateDetectStep		 = SHT_LATE_DETECT_IDLE;
	pThis->mLateDetectErrorCount = 0;
	pThis->mLateDetectTimerID	 = MSP_LOC_TMR_VOID;

	CStopWatch_CStopWatch(&(pThis->mStopWatch));

	for (index = 0; index < SHT_EDGE_NUM; index++ ){
		pThis->mNextEdgeSheetNo[index] = SHEETNO_NUM;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_IsRevereseFilterPath
// Description   : �t���t�B���^�����ΏۃZ���T���ǂ����̎擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : reverseKind (�t�����ʎq)
// Return        : true = �ΏۃZ���T�ł���
//               : false = �ΏۃZ���T�łȂ�
///////////////////////////////////////////////////////////////////
bool CShtPathSensor_IsRevereseFilterPath(CShtPathSensor* pThis, EShtReverseKind reverseKind)
{
	if ( reverseKind < SHT_REVERSE_KIND_NUM ){
		if ( pThis->mReverseFilterBit & (0x01 << reverseKind) ) return true;
		else													return false;
	}
	else{
		if ( pThis->mReverseFilterBit != 0 ) return true;
		else								 return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_IsLateDetectFilterPath
// Description   : ���ԑ҂��m��t�B���^�����ΏۃZ���T���ǂ����̎擾
//               : ����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : edge (�V�[�g�G�b�W���ʎq)
// Return        : true = �ΏۃZ���T�ł���
//               : false = �ΏۃZ���T�łȂ�
///////////////////////////////////////////////////////////////////
bool CShtPathSensor_IsLateDetectFilterPath(CShtPathSensor* pThis, EShtSheetEdge edge)
{
	if ( pThis->mpSpec->lateDetectWaitTime[edge] != 0 ) return true;
	else												return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_GetLevel
// Description   : �p�X�Z���T�Ǘ����x���擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �p�X�Z���T���x��
///////////////////////////////////////////////////////////////////
EShtPathSnrLevel CShtPathSensor_GetLevel(CShtPathSensor* pThis)
{
	if ( pThis->mWaitEdge == SHT_EDGE_TAIL ) return SHT_PATH_SNR_LVL_ON;
	else									 return SHT_PATH_SNR_LVL_OFF;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_GetActualLevel
// Description   : �p�X�Z���T���x��(DD�t�B���^��������)�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �p�X�Z���T���x��
///////////////////////////////////////////////////////////////////
EShtPathSnrLevel CShtPathSensor_GetActualLevel(CShtPathSensor* pThis)
{
	if ( DD_DIO_GetInput(pThis->mpSpec->inputID) == ON ) return SHT_PATH_SNR_LVL_ON;
	else												 return SHT_PATH_SNR_LVL_OFF;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_GetLateDetectFilterTime
// Description   : ���ԑ҂��m��t�B���^���Ԏ擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : edge (�V�[�g�G�b�W���ʎq)
// Return        : �]
///////////////////////////////////////////////////////////////////
UC CShtPathSensor_GetLateDetectFilterTime(CShtPathSensor* pThis, UC edge)
{
	return pThis->mpSpec->lateDetectWaitTime[edge];
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_Reset
// Description   : ���Z�b�g����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtPathSensor_Reset(CShtPathSensor* pThis)
{
	UC index;

	pThis->mWaitEdge			 = SHT_EDGE_LEAD;
	pThis->mReverseFilterBit	 = 0x00;
	pThis->mLateDetectStep		 = SHT_LATE_DETECT_IDLE;
	pThis->mLateDetectErrorCount = 0;
	pThis->mLateDetectTimerID	 = MSP_LOC_TMR_VOID;

	CStopWatch_Start(&(pThis->mStopWatch));

	for ( index = 0; index < SHT_EDGE_NUM; index++ ) {
		pThis->mNextEdgeSheetNo[index] = SHEETNO_NUM;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_EntrySheet
// Description   : �Z���T�ω��\��V�[�g�̓o�^����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : sheetNo (Sheet No.)
//               : edge (�V�[�g�G�b�W���ʎq)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtPathSensor_EntrySheet(CShtPathSensor* pThis, UC sheetNo, EShtSheetEdge edge)
{
	if ( pThis->mNextEdgeSheetNo[edge] == SHEETNO_NUM ){
		pThis->mNextEdgeSheetNo[edge] = sheetNo;
	}
	else { /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_FilterReverse
// Description   : �t���t�B���^���{�w����M������
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : action (Action)
//               : reverseKind (�t�����ʎq)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtPathSensor_FilterReverse(CShtPathSensor* pThis, EAction action, EShtReverseKind reverseKind)
{
	UC reverseKindBit = (UC)(0x01 << reverseKind);								// �r�b�g���ɕϊ�

	if ( reverseKindBit & pThis->mpSpec->reverseFilterBit ){					// �t�B���^�������s�Ώۂ̋t������
		if ( action == ACT_START ){												// �t�B���^�����J�n�w��
			pThis->mReverseFilterBit |= reverseKindBit;
		}
		else{																	// �t�B���^������~�w��
			pThis->mReverseFilterBit &= (UC)(~reverseKindBit);
		}
	}
	else { /* No Action */ }	
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_InformedEdge
// Description   : �Z���T�ω��ʒm��M����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : edge (�V�[�g�G�b�W���ʎq)
// Return        : �Z���T�ω������V�[�g��Sheet No.
///////////////////////////////////////////////////////////////////
UC CShtPathSensor_InformedEdge(CShtPathSensor* pThis, EShtSheetEdge edge)
{
	UL elapsedTime;
	UC returnShtNo = SHEETNO_NUM;

	static const UC cNextWaitEdge[SHT_EDGE_NUM] = { SHT_EDGE_TAIL, SHT_EDGE_LEAD };

	if ( pThis->mNextEdgeSheetNo[edge] != SHEETNO_NUM ){						// ���p�X�Z���T���B�\��V�[�g�̓o�^����
		if ( pThis->mWaitEdge == edge ){										// �҂��G�b�W�ƍ��v
			if ( pThis->mLateDetectStep != SHT_LATE_DETECT_TIMEUP ){
				if ( pThis->mReverseFilterBit == 0x00 ){						// �t���t�B���^���������{
					CStopWatch_Split(&(pThis->mStopWatch), &elapsedTime);// �o�ߎ��Ԏ擾

					if ( (pThis->mpSpec->filterTime[edge] < elapsedTime) ||		// Filter���Ԍo�ߍς�
						 (pThis->mpSpec->filterTime[edge] == 0) ){				// Filter�����s�v
						if ( pThis->mpSpec->lateDetectWaitTime[pThis->mWaitEdge] == 0 ){
																				// �G�b�W�m��ΏۃZ���T�̏ꍇ
							returnShtNo = pThis->mNextEdgeSheetNo[edge];

							CStopWatch_Start(&(pThis->mStopWatch));

							pThis->mWaitEdge			  = cNextWaitEdge[pThis->mWaitEdge];
							pThis->mNextEdgeSheetNo[edge] = SHEETNO_NUM;
						}
						else{													// ���Ԍo�ߊm��ΏۃZ���T�̏ꍇ
							if ( pThis->mLateDetectStep == SHT_LATE_DETECT_IDLE ){
								pThis->mLateDetectStep = SHT_LATE_DETECT_WAIT;

								pThis->mLateDetectTimerID = MSP_SetEventTimer(pThis->mpSpec->lateDetectWaitTime[pThis->mWaitEdge], SHT_PathSensorLateDetectTimeUp, (UL)pThis);
							}
							else{ /* No Action */ }
						}
					}
					else {  /* No Action */ }
				}
				else{ /* No Action */ }
			}
			else{																// ���Ԍo�ߊm��҂����Ԍo�ߍς�
				returnShtNo = pThis->mNextEdgeSheetNo[edge];

				CStopWatch_Start(&(pThis->mStopWatch));

				pThis->mWaitEdge			  = cNextWaitEdge[pThis->mWaitEdge];
				pThis->mNextEdgeSheetNo[edge] = SHEETNO_NUM;

				pThis->mLateDetectStep		 = SHT_LATE_DETECT_IDLE;
				pThis->mLateDetectErrorCount = 0;
				pThis->mLateDetectTimerID	 = MSP_LOC_TMR_VOID;
			}
		}
		else{																	// �҂��G�b�W�ƍ��v���Ȃ�
			if ( (pThis->mLateDetectStep == SHT_LATE_DETECT_WAIT) &&			// ���ԑ҂���
				 (pThis->mLateDetectErrorCount < pThis->mpSpec->lateDetectErrorCheckTimes[pThis->mWaitEdge]) ){
				pThis->mLateDetectStep = SHT_LATE_DETECT_IDLE;
				MSP_CancelEventTimer(pThis->mLateDetectTimerID);
				pThis->mLateDetectTimerID = MSP_LOC_TMR_VOID;
				pThis->mLateDetectErrorCount++;
			}
			else{ /* No Action */ }
		}
	}
	else{ /* No Action */ }

	return returnShtNo;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_InformedLateDetectTimeUp
// Description   : ���ԑ҂��m��̎��Ԍo�ߒʒm��M����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : timerID (�^�C�}�[���ʎq)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtPathSensor_InformedLateDetectTimeUp(CShtPathSensor* pThis, SS timerID)
{
	if ( (pThis->mLateDetectStep == SHT_LATE_DETECT_WAIT) &&					// ���ԑ҂���
		 (pThis->mLateDetectTimerID == timerID) ){
		pThis->mLateDetectStep = SHT_LATE_DETECT_TIMEUP;

		CShtManager_PathSensorLateDetect(pThis, pThis->mWaitEdge);
	}
	else{ /* No Action */ }
}


/////////////////////////////////////////////////////////////////////////////////////////
// Timer Event Call Back Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : SHT_PathSensorLateDetectTimeUp
// Description   : ���ԑ҂��m��̎��Ԍo�߃C�x���g�R�[���o�b�N
// Parameter     : timerID (MSP Event Timer ID)
//               : parameter (�p�X�Z���T�C���X�^���X�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
void SHT_PathSensorLateDetectTimeUp(SS timerID, UL parameter)
{
	CShtPathSensor_InformedLateDetectTimeUp((CShtPathSensor *)parameter, timerID);
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.05.20	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.01.30	| Yuichi Hoshino	| Design Improvement [ Stop Watch���C�u�����݌v�ύX�Ή� ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
