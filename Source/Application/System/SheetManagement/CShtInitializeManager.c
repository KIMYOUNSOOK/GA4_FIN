// File Name:	CShtInitializeManager.c
// Description:	Implimentation File of Sheet Initialize
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CShtInitializeManager.h"

#include "EShtEject.h"
#include "ECdiFinOutDest.h"

#include "CShtSheetAttribute.h"
#include "CShtInitRequestWaitTime.h"

#include "CShtManager.h"
#include "CCvrChecker.h"
#include "CSeqDevInitHandler.h"
#include "COputInformer.h"
#include "CDvstInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define SHT_INIT_ID_CNV_BIT(id)	(UC)(0x01 << id)

// --- Sheet Initialize Condition ---
typedef enum /*EShtInitializeCondition*/
{
	SHT_INIT_COND_NORMAL,			// Normal Condition
	SHT_INIT_COND_ABNORMAL			// Abnormal Condition
} EShtInitializeCondition;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Spec data of Sheet Initialize Handler ---
extern SeqShtInitHandlerSpec cSeq_ShtInitHandlerSpec[SHT_INIT_ID_NUM];

// --- Instance of Sheet Initialize Handler ---
extern CSeqShtInitHandler gSeq_ShtInitHandler[SHT_INIT_ID_NUM];

// --- Sheet Initialize Spec ---
extern ShtInitializeManagerSpec const cSht_InitializeManagerSpec;

// --- Member of Sheet Initialize Manager ---
typedef struct /*CShtInitializeManagerMember*/
{
	UC mCondition;											// �V�[�g�C�j�V�����C�Y�Ǘ���Ԃ������B
	UC mPlanBit[SHEETNO_NUM];								// ���{�\��̃V�[�g�C�j�V�����C�Y������(Bit�Ǘ�)�������B
	UC mCompBit[SHEETNO_NUM];								// ���{�����ς݂̃V�[�g�C�j�V�����C�Y������(Bit�Ǘ�)�������B
	UC mNextShtInitTmgBit[SHT_EJECT_NUM][SHEETNO_NUM];		// ���p���̃V�[�g�C�j�V�����C�Y���J�n�\�ȃ^�C�~���O�ɂȂ��Ă��邩�ǂ����������B
															// �eBit���e�V�[�g�C�j�V�����C�Y�������A�r�b�g���Z�b�g����Ă���͎̂��{�\�A�Z�b�g����Ă��Ȃ��͎̂��{�s�������B
	UC mLastShtReqShtNo;									// ���߂̃V�[�g�t�B�j�b�V���v����Sheet No.�������B
	UC mLastExecShtNo;										// ���߂ŃV�[�g�C�j�V�����C�Y����(�V�[�g�C�j�V�����C�Y����̎�ޖ�킸)�����s(�J�n)�����p����Sheet No.�������B
	UC mLastExecOutDest;									// ���߂ŃV�[�g�C�j�V�����C�Y����(�V�[�g�C�j�V�����C�Y����̎�ޖ�킸)�����s(�J�n)�����p���̔r����w��������B
	CShtSheetAttribute mLastExecShtAttr;					// ���߂ŃV�[�g�C�j�V�����C�Y����(�V�[�g�C�j�V�����C�Y����̎�ޖ�킸)�����s(�J�n)�����p���̃V�[�g�t�B�j�b�V���w�����������B
	UC mCanceledShtNo;										// ���߂ŃL�����Z�����ꂽ�p����Sheet No.�������B
															// �L�����Z����������L�����Z���Ώۗp���ȊO�̗p���̃V�[�g�����v����M�܂ł̊ԕێ�����B
	CShtInitRequestWaitTime mWaitTime;						// �V�[�g�C�j�V�����C�Y����J�n�^�C�~���O����̌o�ߎ��Ԃ������B
} CShtInitializeManagerMember;

static CShtInitializeManagerMember gSht_InitializeMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

static void ClearSheetInfo(UC sheetNo);
static void Reset();
static void SetSheetInitPlan(EShtInitID shtInitID, UC sheetNo);
static bool CheckSheetInitPlan(EShtInitID shtInitID, UC sheetNo);
static US GetSheetInitializeTime(EShtInitID shtInitID, UC sheetNo, UC lastShtNo);
static US GetSheetInitStartAdjustTime(EShtInitID shtInitID, UC sheetNo, UC lastShtNo);
static bool CheckSheetInitExecPossibility(EShtInitID shtInitID);
static bool CheckStartTiming(EShtInitID shtInitID, UC sheetNo, UC prevShtNo);
static bool CheckStartCondition(UC sheetNo);
static void StartSheetInitialize(EShtInitID shtInitID, UC sheetNo);
static void NextSheetInitializeStartTiming(EShtInitID shtInitID, UC sheetNo, EShtEject eject);


/////////////////////////////////////////////////////////////////////////////////////////
// Timer Event Call Back Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void SHT_SheetInitializeStartTimerTimeUp(SS timerID, UL parameter);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CShtInitializeManager_Constructor
// Description   : �R���X�g���N�^
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtInitializeManager_Constructor()
{
	UC shtInitID;

	CShtInitRequestWaitTime_Constructor(&(gSht_InitializeMember.mWaitTime));

	Reset();

	for ( shtInitID = 0; shtInitID < SHT_INIT_ID_NUM; shtInitID++ ){
		CSeqShtInitHandler_Constructor(&(gSeq_ShtInitHandler[shtInitID]), &(cSeq_ShtInitHandlerSpec[shtInitID]));
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : ClearSheetInfo
// Description   : �V�[�g�C�j�V�����C�Y����Ǘ����̃N���A
// Parameter     : sheetNo (Sheet No.)
// Return        : �]
///////////////////////////////////////////////////////////////////
void ClearSheetInfo(UC sheetNo)
{
	UC eject;

	gSht_InitializeMember.mPlanBit[sheetNo] = 0x00;
	gSht_InitializeMember.mCompBit[sheetNo] = 0x00;

	for ( eject = 0; eject < SHT_EJECT_NUM; eject++ ){
		gSht_InitializeMember.mNextShtInitTmgBit[eject][sheetNo] = 0x00;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : Reset
// Description   : �f�[�^�����o�̃��Z�b�g
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void Reset()
{
	UC sheetNo;

	gSht_InitializeMember.mCondition = SHT_INIT_COND_NORMAL;

	gSht_InitializeMember.mLastShtReqShtNo = SHEETNO_NUM;
	gSht_InitializeMember.mLastExecShtNo = SHEETNO_NUM;	
	gSht_InitializeMember.mLastExecOutDest = eCdiFinOutDest_All;

	CShtSheetAttribute_Reset(&(gSht_InitializeMember.mLastExecShtAttr));

	gSht_InitializeMember.mCanceledShtNo = SHEETNO_NUM;

	CShtInitRequestWaitTime_Reset(&(gSht_InitializeMember.mWaitTime));

	for ( sheetNo = 0; sheetNo < SHEETNO_NUM; sheetNo++ ){
		ClearSheetInfo(sheetNo);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : SetSheetInitPlan
// Description   : �w��V�[�g�̎��{�v��ւ̎w��V�[�g�C�j�V�����C�Y
//               : ����̃Z�b�g����
// Parameter     : shtInitID (Sheet Initailize ID)
//               : sheetNo (Sheet No.)
// Return        : �]
///////////////////////////////////////////////////////////////////
void SetSheetInitPlan(EShtInitID shtInitID, UC sheetNo)
{
	gSht_InitializeMember.mPlanBit[sheetNo] |= SHT_INIT_ID_CNV_BIT(shtInitID);
}

///////////////////////////////////////////////////////////////////
// Function Name : CheckSheetInitPlan
// Description   : �w��̃V�[�g�ƃV�[�g�C�j�V�����C�Y����̎��{�v��
//               : �̊m�F����
// Parameter     : shtInitID (Sheet Initailize ID)
//               : sheetNo (Sheet No.)
// Return        : true = ���{�\�肠��^false = ���{�\��Ȃ�
///////////////////////////////////////////////////////////////////
bool CheckSheetInitPlan(EShtInitID shtInitID, UC sheetNo)
{
	 if ( gSht_InitializeMember.mPlanBit[sheetNo] & SHT_INIT_ID_CNV_BIT(shtInitID) ) return true;
	 else																			return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : GetSheetInitStartAdjustTime
// Description   : �V�[�g�C�j�V�����C�Y���쎞�Ԏ擾�����B
// Parameter     : shtInitID (Sheet Initialize ID)
//               : sheetNo (�V�[�g�C�j�V�����C�Y���쐧��Ώێ���Sheet No.)
//               : lastShtNo (�V�[�g�C�j�V�����C�Y���쐧��Ώێ��̐�s����Sheet No.)
// Return        : �V�[�g�C�j�V�����C�Y���쎞��[�~1ms]
///////////////////////////////////////////////////////////////////
US GetSheetInitializeTime(EShtInitID shtInitID, UC sheetNo, UC lastShtNo)
{
	US time=0;

	switch( shtInitID ){
	// !!! Product Specific !!!
	case SHT_INIT_ID_COMPILER:
	
		// SelectFin��M���̂ݎ��s�̑΍� (V3.1.6)
		// �Z�b�g�ԈقȂ�W���u���̑Ή�(V3.1.7)
		/*if ( (sheetNo == SRVC_GetProcessStartSheet()) || (lastShtNo == SHEETNO_NUM)) {  
		else if( (SRVC_GetSetTop(sheetNo) == SHT_SET_TOP) &&				// "����Ώۗp�����Z�b�g�擪�p���ł���A�ȉ��̂����ꂩ�̏����𖞂����Ă���A
			 ((SRVC_GetCrossProcessSize(sheetNo) != SRVC_GetCrossProcessSize(lastShtNo)) ||	//    ������Ώۗp���̗p�����Ɛ�s���̗p�������قȂ�
	 		  (SRVC_GetStapleMode(sheetNo) != SRVC_GetStapleMode(lastShtNo)) ||	//    ������Ώۗp���̃X�e�[�v���w��Ɛ�s���̃X�e�[�v���w�肪�قȂ�
	 		  (SRVC_GetCrossProcessSizeMixSet(lastShtNo) == SHT_SIZE_MIX)) ){		//    ����s���̃Z�b�g�ɂ����ăZ�b�g���ٕ����������Ă��� 	
			time = CMPL_GetCompilerSheetInitializeTimeStep1(sheetNo, lastShtNo);	
		}else {
			time =0;
		} */
		time = CMPL_GetCompilerSheetInitializeTimeStep1(sheetNo, lastShtNo);	//V3.5.93
		break;
	default :
		break;	
	}

	return time;
}

///////////////////////////////////////////////////////////////////
// Function Name : GetSheetInitStartAdjustTime
// Description   : �V�[�g�C�j�V�����C�Y���쎞�ԕ␳�l�̎擾�����B
// Parameter     : shtInitID (�擾�Ώۂ�Sheet Initialize ID)
//               : sheetNo (�V�[�g�C�j�V�����C�Y���쐧��Ώێ���Sheet No.)
//               : lastShtNo (�V�[�g�C�j�V�����C�Y���쐧��Ώێ��̐�s����Sheet No.)
// Return        : �V�[�g�C�j�V�����C�Y���쎞�ԕ␳�l[�~1ms]
///////////////////////////////////////////////////////////////////
US GetSheetInitStartAdjustTime(EShtInitID shtInitID, UC sheetNo, UC lastShtNo)
{
	US time=0;

	if ( lastShtNo != SHEETNO_NUM ){
		switch( shtInitID ){
		// !!! Product Specific !!!
//		case SHT_INIT_ID_XXX:
//
//			break;
		default :
			break;
		}
	}
	else{ /* No Action */ }

	return time;
}

///////////////////////////////////////////////////////////////////
// Function Name : CheckSheetInitExecPossibility
// Description   : �w��V�[�g�C�j�V�����C�Y����̎��{�ۂ̊m�F����
// Parameter     : shtInitID (�m�F�Ώۂ�Sheet Initialize ID)
// Return        : true = ���{�\�^false = ���{�s��
///////////////////////////////////////////////////////////////////
bool CheckSheetInitExecPossibility(EShtInitID shtInitID)
{
//	if ( shtInitID < SHT_INIT_ID_NUM ){											// �����W���[���Ŏg�p����֐��Ȃ̂ŃK�[�h�s�v�̂��߃R�����g�A�E�g
		return COputInformer_GetOptionDetect((EOptionID)(cSht_InitializeManagerSpec.shtInitSpec[shtInitID].optionID));
//	}
//	else{
//		return false;
//	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CheckStartTiming
// Description   : ��s��������ΏۃV�[�g�̃V�[�g�C�j�V�����C�Y����
//               : �����{�\�ȃ|�C���g�܂Ő��䂪�ς�ł��邩�ǂ���
//               : �̊m�F�����B
// Parameter     : shtInitID (Sheet Initialize ID)
//               : sheetNo (����ΏۃV�[�g��Sheet No.)
//               : prevShtNo (����ΏۃV�[�g�̐�s����Sheet No.)
// Return        : true = ����ς݁^false = ���䖢����
///////////////////////////////////////////////////////////////////
bool CheckStartTiming(EShtInitID shtInitID, UC sheetNo, UC prevShtNo)
{
	const CShtSheet *pPrevSht;

	if ( prevShtNo < SHEETNO_NUM ){												// ��s��������(�Ǘ��Ώۂł���)
		pPrevSht = CShtManager_GetSheet(prevShtNo);

		if ( CShtSheet_CheckConditionAbnormalEnd(pPrevSht) == false ){			// �ُ�I���V�[�g�̓`�F�b�N�ΏۊO(���v���Z�X�Ő���Ɉ����p����V�[�g�̂݃`�F�b�N)
			if ( cSht_InitializeManagerSpec.shtInitSpec[shtInitID].ejectDepend == true ){
																				// ��s�����C�W�F�N�g�Ώۂ��ǂ����Ɉˑ�����
				if ( gSht_InitializeMember.mNextShtInitTmgBit[CShtSheet_GetEject(pPrevSht)][prevShtNo] & SHT_INIT_ID_CNV_BIT(shtInitID) ) return true;
				else																													  return false;
			}
			else{																// ��s�����C�W�F�N�g�Ώۂ��ǂ����Ɉˑ����Ȃ�
				if ( (gSht_InitializeMember.mNextShtInitTmgBit[SHT_EJECT_EXEC][prevShtNo] & SHT_INIT_ID_CNV_BIT(shtInitID)) ||
					 (gSht_InitializeMember.mNextShtInitTmgBit[SHT_EJECT_NOT][prevShtNo] & SHT_INIT_ID_CNV_BIT(shtInitID)) ){
					return true;
				}
				else{
					return false;
				}
			}
		}
		else{
			return true;
		}
	}
	else{																		// ��s�����Ȃ�(�������������ĊǗ��Ώۂ���O��Ă���ꍇ���܂�)
		return true;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CheckStartCondition
// Description   : �w��V�[�g�ɂ�����V�[�g�C�j�V�����C�Y����̎��{
//               : �ۂ̊m�F����
// Parameter     : sheetNo (�m�F�ΏۃV�[�g��Sheet No.)
// Return        : true = ���{�\�^false = ���{�s��
///////////////////////////////////////////////////////////////////
bool CheckStartCondition(UC sheetNo)
{
	const CShtSheet *pSheet;

	if ( sheetNo < SHEETNO_NUM ){
		pSheet = CShtManager_GetSheet(sheetNo);

		if ( CShtSheet_CheckConditionProcess(pSheet) == true ){
			return true;
		}
		else{ /* NG */ }
	}
	else{ /* NG */ }

	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : StartSheetInitialize
// Description   : �V�[�g�C�j�V�����C�Y����J�n����
// Parameter     : shtInitID (Sheet Initialize ID)
//               : sheetNo (����ΏۃV�[�g��Sheet No.)
// Return        : �]
///////////////////////////////////////////////////////////////////
void StartSheetInitialize(EShtInitID shtInitID, UC sheetNo)
{
	const CShtSheet *pSheet;

//	if ( shtInitID < SHT_INIT_ID_NUM ){											// �����W���[���Ŏg�p����֐��Ȃ̂ŃK�[�h�s�v�̂��߃R�����g�A�E�g
		pSheet = CShtManager_GetSheet(sheetNo);

		gSht_InitializeMember.mLastExecShtNo = sheetNo;
		gSht_InitializeMember.mLastExecOutDest = CShtSheet_GetOutputDestFinish(pSheet);
		gSht_InitializeMember.mLastExecShtAttr = *(CShtSheet_GetAttribute(pSheet));

		CSeqShtInitHandler_Start(&(gSeq_ShtInitHandler[shtInitID]), sheetNo);
//	}
//	else{
//		return false;
//	}
}

///////////////////////////////////////////////////////////////////
// Function Name : NextSheetInitializeStartTiming
// Description   : �ΏۃV�[�g���A�㑱���̃V�[�g�C�j�V�����C�Y�����
//               : �J�n�ł���|�C���g�܂Ő��䂪���������ۂ̏���
// Parameter     : shtInitID (����ΏۃV�[�g�C�j�V�����C�Y���쎯�ʎq)
//               : sheetNo (�㑱���̃V�[�g�C�j�V�����C�Y������J�n��
//               : ����|�C���g�܂Ő��䂪���������V�[�g��Sheet No.)
//               : nextShtNo (�㑱����Sheet No.)
// Return        : �]
///////////////////////////////////////////////////////////////////
void NextSheetInitializeStartTiming(EShtInitID shtInitID, UC sheetNo, EShtEject eject)
{
	const CShtSheet *pSheet, *pLastSheet;
	UC nextShtNo;

	pSheet = CShtManager_GetSheet(sheetNo);

	// --- �ΏۃV�[�g�̌㑱���̃V�[�g�C�j�V�����C�Y����J�n�\�|�C���g���䊮�����X�V ---
	gSht_InitializeMember.mNextShtInitTmgBit[eject][sheetNo] |= SHT_INIT_ID_CNV_BIT(shtInitID);

	// --- ���p���̎擾 ---
	nextShtNo = CShtManager_GetNextRequestSheet(sheetNo);

	// �C�W�F�N�g�t�B�j�b�V���O����ƃv���Z�X�X�^�[�g�̏d�Ȃ�Ή�(���p�����V�[�g�Ǘ��L���[���o�^)
	if ( nextShtNo == SHEETNO_NUM ){
		if ( gSht_InitializeMember.mLastShtReqShtNo < SHEETNO_NUM ){
			pLastSheet = CShtManager_GetSheet(gSht_InitializeMember.mLastShtReqShtNo);

			if ( (CShtSheet_GetCondition(pLastSheet) == SHT_COND_VOID) &&
				 (gSht_InitializeMember.mLastShtReqShtNo != sheetNo) ){
				nextShtNo = gSht_InitializeMember.mLastShtReqShtNo;
			}
			else{ /* No Action */ }	 
		}
		else{ /* No Action */ }	 
	}
	else{ /* No Action */ }

	// --- ���p���̃V�[�g�C�j�V�����C�Y����J�n ---
	if ( nextShtNo < SHEETNO_NUM ){
		if ( CheckStartCondition(nextShtNo) == true ){
			if ( CheckSheetInitPlan(shtInitID, nextShtNo) == true ){
				StartSheetInitialize(shtInitID, nextShtNo);
			}
			else{ /* No Action */ }
		}
		else{ /* No Action */ }
	}
	else{ /* No Action */ }

	// --- �V�[�g�t�B�j�b�V�����N�G�X�g�҂����Ԍv���J�n ---
	CShtInitRequestWaitTime_StartWaiting(&(gSht_InitializeMember.mWaitTime), sheetNo);
}


///////////////////////////////////////////////////////////////////
// Function Name : CShtInitializeManager_CheckPlan
// Description   : �w��̃V�[�g�ƃV�[�g�C�j�V�����C�Y����ɂ������
//               : �{�v��̊m�F����
// Parameter     : sheetNo (Sheet No.)
//               : shtInitID (Sheet Initailize ID)
// Return        : true = ���{�\�肠��^false = ���{�\��Ȃ�
///////////////////////////////////////////////////////////////////
bool CShtInitializeManager_CheckPlan(UC sheetNo, EShtInitID shtInitID)
{
	return CheckSheetInitPlan(shtInitID, sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtInitializeManager_CheckComplete
// Description   : �w��̃V�[�g�ƃV�[�g�C�j�V�����C�Y����ɂ������
//               : �{�󋵂̊m�F����
// Parameter     : sheetNo (Sheet No.)
//               : shtInitID (Sheet Initailize ID)
// Return        : true = �����ς݁^false = ������
///////////////////////////////////////////////////////////////////
bool CShtInitializeManager_CheckComplete(UC sheetNo, EShtInitID shtInitID)
{
	if ( shtInitID < SHT_INIT_ID_NUM ){
		if ( (CheckSheetInitPlan(shtInitID, sheetNo) == false) ||
			 (gSht_InitializeMember.mCompBit[sheetNo] & SHT_INIT_ID_CNV_BIT(shtInitID)) ){
			return true;
		}
		else{
			return false;
		}
	}
	else{																		// Check all sheet initialize
		for ( shtInitID = 0; shtInitID < SHT_INIT_ID_NUM; shtInitID++ ){
			if ( (CheckSheetInitPlan(shtInitID, sheetNo) == true) &&
				 ((gSht_InitializeMember.mCompBit[sheetNo] & SHT_INIT_ID_CNV_BIT(shtInitID)) == 0x00) ){
				return false;	 
			}
			else{
				// Go for loop
			}
		}

		return true;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtInitializeManager_DetectInitExecution
// Description   : �V�[�g�C�j�V�����C�Y���{���f����
// Parameter     : sheetNo (�ΏۃV�[�g��Sheet No.)
//               : prevShtNo (�ΏۃV�[�g�̐�s����Sheet No.)
// Return        : �V�[�g�C�j�V�����C�Y���쎞�ԏ��
///////////////////////////////////////////////////////////////////
ShtInitTimeInfo CShtInitializeManager_DetectInitExecution(UC sheetNo, UC prevShtNo)
{
	const CShtSheet *pSheet;
	CShtSheetAttribute *pShtAttr;
	ShtInitTimeInfo timeInfo;
	UL waitTime;
	US initTime;
	UC moduleID, shtInitID, lastShtNo, anyInitExec;


	pSheet = CShtManager_GetSheet(sheetNo);
	pShtAttr = CShtSheet_GetAttribute(pSheet);

	if ( gSht_InitializeMember.mCondition == SHT_INIT_COND_NORMAL ){
		// --- �V�[�g�C�j�V�����C�Y����J�n�ς݃V�[�g�̏��ω��`�F�b�N ---
		if ( sheetNo == gSht_InitializeMember.mLastExecShtNo ){					// �V�[�g�C�j�V�����C�Y�J�n�ς̃V�[�g�ł���
			if ( (CShtSheet_GetOutputDestFinish(pSheet) != gSht_InitializeMember.mLastExecOutDest) ||
				 (CShtSheetAttribute_CheckDiffOutSheet(pShtAttr, &(gSht_InitializeMember.mLastExecShtAttr)) == true) ){
					 															// ���ɕύX������
				// --- ���ω�(�L�����Z��)���������� ---
				gSht_InitializeMember.mCanceledShtNo = sheetNo;					// �L�����Z���ΏۃV�[�g���ʎq�ێ�
				gSht_InitializeMember.mCondition	 = SHT_INIT_COND_ABNORMAL;	// �Ǘ���Ԃ�Abnormal�ɕύX
			}
			else{ /* No Action */ }
		}
		else{ /* No Action */ }
	}
	else{																		// Abnormal condition
		// --- �m�[�}���R���f�B�V�����ւ̕��A�ۃ`�F�b�N  ---
		for ( shtInitID = 0; shtInitID < SHT_INIT_ID_NUM; shtInitID++ ){
			if ( (CheckSheetInitExecPossibility(shtInitID) == true) &&
				 ( (CheckStartTiming(shtInitID, sheetNo, prevShtNo) == false) ||
				   (CSeqShtInitHandler_GetState(&(gSeq_ShtInitHandler[shtInitID])) == SEQ_ACT_STT_RUNNING) ) ){
				break;
			}
			else{ /* Go Loop */ }
		}

		if ( (shtInitID >= SHT_INIT_ID_NUM) &&									// All sheet initialize execution is possible.
			 (CShtManager_CheckUnejectedSheetExist((UC)eCdiFinOutDest_All) == false) ){
																				// Undelivered sheet don't exist.		
			// --- �m�[�}���R���f�B�V�����ւ̕��A���� ---
			gSht_InitializeMember.mLastShtReqShtNo = SHEETNO_NUM;
			CShtInitRequestWaitTime_Reset(&(gSht_InitializeMember.mWaitTime));
			gSht_InitializeMember.mCondition = SHT_INIT_COND_NORMAL;
		}
		else{ /* No Action */ }
	}

	if ( gSht_InitializeMember.mCondition == SHT_INIT_COND_NORMAL ){
		anyInitExec = false;
		// --- �V�[�g�C�j�V�����C�Y������N���A(�ŏ��̃��N�G�X�g��M���̂�) ---
		if ( sheetNo != gSht_InitializeMember.mLastShtReqShtNo ){
			ClearSheetInfo(sheetNo);
		}
		else{
			// No Action
		}

		// --- �C�j�V�����C�Y���쎞�Ԏ擾���C�j�V�����C�Y����v�搶��(�����N�G�X�g��M���Ɏ��s) ---
		if ( gSht_InitializeMember.mCanceledShtNo == sheetNo ){					// Canceled sheet
			lastShtNo = SHEETNO_NUM;
		}
		else{																	// Not canceled sheet
			lastShtNo = prevShtNo;

			gSht_InitializeMember.mCanceledShtNo = SHEETNO_NUM;
		}

		for ( shtInitID = 0; shtInitID < SHT_INIT_ID_NUM; shtInitID++ ){
			initTime = GetSheetInitializeTime(shtInitID, sheetNo, lastShtNo);

			if ( initTime != 0 ){
				SetSheetInitPlan(shtInitID, sheetNo);							// ���x�₢���킹�Ă��������e�����������O��œ�����{�����ǂ������P�A�����ɖ��񏑂����ݎ��{

				// --- �V�[�g�C�j�V�����C�Y����J�n�␳���ԉ��Z ---
				initTime += GetSheetInitStartAdjustTime(shtInitID, sheetNo, lastShtNo);

				anyInitExec = true;
			}
			else{ /* No Action */ }

			timeInfo.sheetInitTime[shtInitID] = initTime;
		}

		// --- ���N�G�X�g�x�ꎞ�ԉ��Z ---
		if ( (anyInitExec == true) &&											// �C�j�V�����C�Y������{
			 (prevShtNo < SHEETNO_NUM) ){										// �p���Ǘ���ɐ�s������
			waitTime = CShtInitRequestWaitTime_GetWaitTime(&(gSht_InitializeMember.mWaitTime), prevShtNo);

			if ( waitTime > 60000 ) timeInfo.waitTime = 60000;					// �C�j�V�����C�Y���쎞�Ԃ����Z�����ۂ�60sec�𒴂���l�ɂȂ�悤60sec�ɂ���B
			else					timeInfo.waitTime = (US)waitTime;
		}
		else{
			timeInfo.waitTime = 0;
		}

	
		// --- �V�[�g�C�j�V�����C�Y���슮���`�F�b�N(Unknown�AConsiderable Time��������̕��A) --- 
		if ( gSht_InitializeMember.mPlanBit[sheetNo] == gSht_InitializeMember.mCompBit[sheetNo] ){
			for ( shtInitID = 0; shtInitID < SHT_INIT_ID_NUM; shtInitID++ ){
				timeInfo.sheetInitTime[shtInitID] = 0;
			}

			timeInfo.waitTime = 0;
		}
	}
	else{
		for ( shtInitID = 0; shtInitID < SHT_INIT_ID_NUM; shtInitID++ ){
			timeInfo.sheetInitTime[shtInitID] = 0xFFFF;
		}

		timeInfo.waitTime = 0xFFFF;
	}

	return timeInfo;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtInitializeManager_Start
// Description   : �V�[�g�C�j�V�����C�Y����Ǘ��̊J�n����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtInitializeManager_Start()
{
	gSht_InitializeMember.mCondition = SHT_INIT_COND_NORMAL;

	gSht_InitializeMember.mLastShtReqShtNo = SHEETNO_NUM;
	gSht_InitializeMember.mLastExecShtNo = SHEETNO_NUM;	
	gSht_InitializeMember.mLastExecOutDest = eCdiFinOutDest_All;
	CShtSheetAttribute_Reset(&(gSht_InitializeMember.mLastExecShtAttr));

	gSht_InitializeMember.mCanceledShtNo = SHEETNO_NUM;

	CShtInitRequestWaitTime_Reset(&(gSht_InitializeMember.mWaitTime));
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtInitializeManager_End
// Description   : �V�[�g�C�j�V�����C�Y����Ǘ��̏I������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtInitializeManager_End()
{
	gSht_InitializeMember.mLastShtReqShtNo = SHEETNO_NUM;
}


///////////////////////////////////////////////////////////////////
// Function Name : CShtInitializeManager_CycleDown
// Description   : �V�[�g�C�j�V�����C�Y����̉�����~����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtInitializeManager_CycleDown()
{
	const CShtSheet *pSheet;
	UC shtInitID;

	// --- �p���Ǘ����o�^�p���ɑ΂���V�[�g�C�j�V�����C�Y���쒆�~ ---
	for ( shtInitID = 0; shtInitID < SHT_INIT_ID_NUM; shtInitID++ ){
		pSheet = CShtManager_GetSheet(CSeqShtInitHandler_GetSheetNo(&(gSeq_ShtInitHandler[shtInitID])));

		if ( (pSheet != NULL) &&
			 (CShtSheet_GetCondition(pSheet) == SHT_COND_VOID) ){
			CSeqShtInitHandler_Stop(&(gSeq_ShtInitHandler[shtInitID]), SHEETNO_NUM);
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtInitializeManager_HardDown
// Description   : �V�[�g�C�j�V�����C�Y����ً̋}��~����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtInitializeManager_HardDown()
{
	UC shtInitID;

	// --- �V�[�g�C�j�V�����C�Y���쒆�~ ---
	for ( shtInitID = 0; shtInitID < SHT_INIT_ID_NUM; shtInitID++ ){
		CSeqShtInitHandler_Stop(&(gSeq_ShtInitHandler[shtInitID]), SHEETNO_NUM);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtInitializeManager_SheetRequest
// Description   : �V�[�g�����˗���M���̏���
// Parameter     : sheetNo (�˗��ΏۃV�[�g��Sheet No.)
//               : prevShtNo (�˗��ΏۃV�[�g�̐�s����Sheet No.)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtInitializeManager_SheetRequest(UC sheetNo, UC prevShtNo)
{
	UC shtInitID, checkResult[SHT_INIT_ID_NUM];

	shtInitID = false;

	if ( gSht_InitializeMember.mLastShtReqShtNo != sheetNo ){				// �O��̃��N�G�X�g�Ɠ����V�[�g�̏ꍇ�̓V�[�g�C�j�V�����C�Y����J�n���������҂��Ȃ̂Ŗ�������
		if ( (gSht_InitializeMember.mCondition == SHT_INIT_COND_NORMAL) &&	// �m�[�}���R���f�B�V�����łȂ��Ԃ̓C�j�V�����C�Y������{���Ȃ�
			 (CheckStartCondition(sheetNo) == true) && 						// �V�[�g�R���f�B�V�����̃`�F�b�N(�V�[�g�R���f�B�V�������������邱�Ƃ͂Ȃ����ߑO�񃊃N�G�X�g�V�[�g�`�F�b�N�̓����ɔz�u����)
			 (CDvstInformer_GetDeviceCondtion(DEVID_FINISHER) == DEV_COND_OK) &&
			 																// �t�B�j�b�V���[�R���f�B�V�����̃`�F�b�N(�t�H���g���m�ƃv���Z�X�J�n�̌����Ή�)
			 (CCvrChecker_GetCoverStatus(CVR_ID_NUM) == CVR_STTS_CLOSE) ){	// �J�o�[��Ԃ̃`�F�b�N(�t�H���g���m�ƃv���Z�X�J�n�̌����Ή�)
			for ( shtInitID = 0; shtInitID < SHT_INIT_ID_NUM; shtInitID++ ){
				if ( CheckSheetInitPlan(shtInitID, sheetNo) == true ){
					if ( (CheckStartTiming(shtInitID, sheetNo, prevShtNo) == true) &&
						 (CSeqShtInitHandler_GetState(&(gSeq_ShtInitHandler[shtInitID])) != SEQ_ACT_STT_RUNNING) ){
							 												// �V�[�g�C�j�V�����C�Y���얢���{
						StartSheetInitialize(shtInitID, sheetNo);
					}
					else{ /* No Action */ }
				}
				else{ /* No Action */ }
			}
		}
		else{ /* No Action */ }

		gSht_InitializeMember.mLastShtReqShtNo = sheetNo;
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtInitializeManager_SheetExecution
// Description   : �V�[�g�����Ǘ��J�n���̏���
// Parameter     : sheetNo (�Ǘ��J�n�V�[�g��Sheet No.)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtInitializeManager_SheetExecution(UC sheetNo)
{
	CShtInitRequestWaitTime_SetWaitSheet(&(gSht_InitializeMember.mWaitTime), sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtInitializeManager_SheetDelivered
// Description   : �V�[�g�����Ǘ�����(�V�[�g�f���o�[�h)���̏���
// Parameter     : sheetNo (�Ǘ������V�[�g��Sheet No.)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtInitializeManager_SheetDelivered(UC sheetNo)
{
	if ( sheetNo == gSht_InitializeMember.mLastExecShtNo ){
		gSht_InitializeMember.mLastExecShtNo = SHEETNO_NUM;
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtInitializeManager_SheetCancel
// Description   : �V�[�g�����˗��L�����Z�����̏���
// Parameter     : sheetNo (�L�����Z���ΏۃV�[�g��Sheet No.)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtInitializeManager_SheetCancel(UC sheetNo)
{
	if ( sheetNo == gSht_InitializeMember.mLastExecShtNo ){
		gSht_InitializeMember.mCanceledShtNo = sheetNo;
		gSht_InitializeMember.mCondition	 = SHT_INIT_COND_ABNORMAL;
	}
	else{ 
		//TB No.69. �V�[�g�L�����Z����̃��X�P�W���[�����O�p���ɑ΂���V�[�g�C�j�V�����C�Y��������{���Ȃ�
		// --- ���X�P�W���[�����O��̃V�[�g�����˗�����t����悤�ɃN���A���{ ---
		gSht_InitializeMember.mLastShtReqShtNo = SHEETNO_NUM;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtInitializeManager_SheetLocation
// Description   : Sheet Location�ʒm��M���̏����B
// Parameter     : sheetNo (���P�[�V�����X�V�V�[�g��Sheet No.)
//               : edge (�V�[�g�G�b�W���ʎq)
//               : location (Sheet Location ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtInitializeManager_SheetLocation(UC sheetNo, EShtSheetEdge edge, EShtSheetLocationID location)
{
	const CShtSheet *pSheet;
	UL parameter;
	UC  eject, shtInitID;

	pSheet    = CShtManager_GetSheet(sheetNo);
	eject     = CShtSheet_GetEject(pSheet);

	// --- ���P�[�V�������ɉ������V�[�g�C�j�V�����C�Y������{ ---
	for ( shtInitID = 0; shtInitID < SHT_INIT_ID_NUM; shtInitID++ ){
		if ( (location == cSht_InitializeManagerSpec.shtInitSpec[shtInitID].startSpec[eject].startLocation) &&
			 (edge == cSht_InitializeManagerSpec.shtInitSpec[shtInitID].startSpec[eject].startLocationEdge) ){
			parameter = (((UL)shtInitID << 16)|((UL)sheetNo << 8) |eject);

			MSP_SetEventTimer(cSht_InitializeManagerSpec.shtInitSpec[shtInitID].startSpec[eject].startWaitTime, SHT_SheetInitializeStartTimerTimeUp, parameter);
		}
		else{
			// Go for loop
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtInitializeManager_SheetEvent
// Description   : Sheet Event�ʒm��M���̏����B
// Parameter     : sheetNo (Sheet Event�X�V�V�[�g��Sheet No.)
//               : event (Sheet Event���ʎq)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtInitializeManager_SheetEvent(UC sheetNo, EShtSheetEventID event)
{
	const CShtSheet *pSheet;
	UL parameter;
	UC outDest, eject, shtInitID;

	pSheet    = CShtManager_GetSheet(sheetNo);
	eject     = CShtSheet_GetEject(pSheet);

	// --- �C�x���g�ɉ������V�[�g�C�j�V�����C�Y������{ ---
	for ( shtInitID = 0; shtInitID < SHT_INIT_ID_NUM; shtInitID++ ){
		if ( event == cSht_InitializeManagerSpec.shtInitSpec[shtInitID].startSpec[eject].startEvent ){
			parameter = (((UL)shtInitID << 16)|((UL)sheetNo << 8)|eject);

			MSP_SetEventTimer(cSht_InitializeManagerSpec.shtInitSpec[shtInitID].startSpec[eject].startWaitTime, SHT_SheetInitializeStartTimerTimeUp, parameter);
		}
		else{
			// Go for loop
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtInitializeManager_SheetAction
// Description   : Sheet Action�ʒm��M�����B
// Parameter     : sheetNo (Sheet No.)
//               : eventID (MSP Event ID)
//               : moduleID (Module ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtInitializeManager_SheetAction(UC sheetNo, US eventID, UC moduleID)
{
	UC shtInitID;

	for ( shtInitID = 0; shtInitID < SHT_INIT_ID_NUM; shtInitID++ ){
		if ( CSeqShtInitHandler_SheetActionResponse(&(gSeq_ShtInitHandler[shtInitID]), sheetNo, eventID, moduleID) == SEQ_ACT_STT_IDLE) {
			// --- �V�[�g�C�j�V�����C�Y���슮�� ---
			gSht_InitializeMember.mCompBit[sheetNo] |= SHT_INIT_ID_CNV_BIT(shtInitID);
		}
		else{
			// No Action
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
// Timer Event Call Back Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : SHT_SheetInitializeStartTimerTimeUp
// Description   : �V�[�g�C�j�V�����C�Y�J�n�^�C�}�[�^�C���A�b�v�C�x���g�R�[���o�b�N
// Parameter     : timerID (Timer ID)
//               : parameter (Parameter�FBit0�`7 = Next Sheet No./
//               : Bit8�`15 = Previous Sheet No./Bit16�`23 = Sheet 
//               : Initialize ID)
// Return        : �]
///////////////////////////////////////////////////////////////////
void SHT_SheetInitializeStartTimerTimeUp(SS timerID, UL parameter)
{
	UC shtInitID, sheetNo, eject;

	shtInitID = (UC)(parameter >> 16);
	sheetNo	  = (UC)(parameter >> 8);
	eject	  = (UC)parameter;

	NextSheetInitializeStartTiming(shtInitID, sheetNo, eject);
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
// 15.03.30	| Yuichi Hoshino	| Design Improvement [ �ُ��~���̃C�j�V�����C�Y�g���K�C�x���g�ʒm�����Ή� ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
// 15.10.06	| Yuichi Hoshino	| Trouble Action [ Cover Open��Ԃ�Print�w�����{����Device Fail������ ]
// 16.01.07	| Yuichi Hoshino	| Trouble Action [ �V�[�g�C�j�V�����C�Y����J�n�^�C�~���O�s�� ]
// 16.03.04	| Yuichi Hoshino	| Trouble Action [ �V�[�g�L�����Z����̃��X�P�W���[�����O�p���ɑ΂���V�[�g�C�j�V�����C�Y��������{���Ȃ� ]


