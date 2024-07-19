// File Name:	CShtSheet.c
// Description:	Implimentation File of Sheet
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CShtSheet.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Convert Bit Information ---
#define SHT_LOC_CNV_BIT(locationID)		(0x00000001 << locationID)
#define SHT_EVT_ID_CNV_BIT(eventID)		(0x00000001 << eventID)

// --- Threshold of Size Class ---
#define SHT_SIZE_CLASS_SMALL_THRESH		2160
#define SHT_SIZE_CLASS_LARGE_THRESH		2970

// --- Sheet Condition Change Table [current condition][destination condition] ---
static const UC cSht_ShtCondTable[SHT_COND_NUM][SHT_COND_CHG_EVT_NUM] = {
//	| Entry				| Normal Exit		| Complete			| Delivered			| Eject Detect		| Abnormal Exit		| Purge				| Cancel			| Stop				| Jam				| Abort				|
	{ SHT_COND_CANDIDATE, SHT_COND_VOID		, SHT_COND_VOID		, SHT_COND_VOID		, SHT_COND_VOID		, SHT_COND_VOID		, SHT_COND_VOID		, SHT_COND_VOID		, SHT_COND_VOID		, SHT_COND_VOID		, SHT_COND_VOID		},	// Void
	{ SHT_COND_CANDIDATE, SHT_COND_DELIVERY	, SHT_COND_CANDIDATE, SHT_COND_CANDIDATE, SHT_COND_CANDIDATE, SHT_COND_PURGE	, SHT_COND_CANDIDATE, SHT_COND_CANCEL	, SHT_COND_REMAIN	, SHT_COND_CANDIDATE, SHT_COND_CANDIDATE},	// Candidate
	{ SHT_COND_DELIVERY	, SHT_COND_DELIVERY	, SHT_COND_COMPLETE	, SHT_COND_COMPILE	, SHT_COND_DELIVERY	, SHT_COND_DELIVERY	, SHT_COND_PURGE	, SHT_COND_DELIVERY	, SHT_COND_REMAIN	, SHT_COND_JAM		, SHT_COND_REMAIN	},	// Delivery
	{ SHT_COND_PURGE	, SHT_COND_PURGE	, SHT_COND_REJECT	, SHT_COND_PURGE	, SHT_COND_PURGE	, SHT_COND_PURGE	, SHT_COND_PURGE	, SHT_COND_PURGE	, SHT_COND_REMAIN	, SHT_COND_JAM		, SHT_COND_REMAIN	},	// Purge
	{ SHT_COND_EJECT	, SHT_COND_EJECT	, SHT_COND_COMPLETE	, SHT_COND_EJECT	, SHT_COND_EJECT	, SHT_COND_EJECT	, SHT_COND_EJECT	, SHT_COND_EJECT	, SHT_COND_REMAIN	, SHT_COND_JAM		, SHT_COND_REMAIN	}, 	// Eject
	{ SHT_COND_COMPILE	, SHT_COND_COMPILE	, SHT_COND_COMPILE	, SHT_COND_COMPILE	, SHT_COND_EJECT	, SHT_COND_COMPILE	, SHT_COND_COMPILE	, SHT_COND_COMPILE	, SHT_COND_LEAVE	, SHT_COND_COMPILE	, SHT_COND_REMAIN	},	// Compile
	{ SHT_COND_CANCEL	, SHT_COND_CANCEL	, SHT_COND_CANCEL	, SHT_COND_CANCEL	, SHT_COND_CANCEL	, SHT_COND_CANCEL	, SHT_COND_CANCEL	, SHT_COND_CANCEL	, SHT_COND_CANCEL	, SHT_COND_CANCEL	, SHT_COND_CANCEL	},	// Cancel
	{ SHT_COND_LEAVE	, SHT_COND_LEAVE	, SHT_COND_LEAVE	, SHT_COND_LEAVE	, SHT_COND_REMAIN	, SHT_COND_LEAVE	, SHT_COND_LEAVE	, SHT_COND_LEAVE	, SHT_COND_LEAVE	, SHT_COND_LEAVE	, SHT_COND_REMAIN	},	// Leave
	{ SHT_COND_JAM		, SHT_COND_JAM		, SHT_COND_JAM		, SHT_COND_JAM		, SHT_COND_JAM		, SHT_COND_JAM		, SHT_COND_JAM		, SHT_COND_JAM		, SHT_COND_JAM		, SHT_COND_JAM		, SHT_COND_JAM		},	// Jam
	{ SHT_COND_REMAIN	, SHT_COND_REMAIN	, SHT_COND_REMAIN	, SHT_COND_REMAIN	, SHT_COND_REMAIN	, SHT_COND_REMAIN	, SHT_COND_REMAIN	, SHT_COND_REMAIN	, SHT_COND_REMAIN	, SHT_COND_REMAIN	, SHT_COND_REMAIN	},	// Remain
	{ SHT_COND_COMPLETE	, SHT_COND_COMPLETE	, SHT_COND_COMPLETE	, SHT_COND_COMPLETE	, SHT_COND_COMPLETE	, SHT_COND_COMPLETE	, SHT_COND_COMPLETE	, SHT_COND_COMPLETE	, SHT_COND_COMPLETE	, SHT_COND_COMPLETE	, SHT_COND_COMPLETE	},	// Complete
	{ SHT_COND_REJECT	, SHT_COND_REJECT	, SHT_COND_REJECT	, SHT_COND_REJECT	, SHT_COND_REJECT	, SHT_COND_REJECT	, SHT_COND_REJECT	, SHT_COND_REJECT	, SHT_COND_REJECT	, SHT_COND_REJECT	, SHT_COND_REJECT	}	// Reject
};

// --- Size Class Table ---
static const UC cSht_ShtSizeClass[SHT_SIZE_CLASS_NUM][SHT_SIZE_CLASS_NUM] = {
	{ SHT_SIZE_CLASS_SMALL,	SHT_SIZE_CLASS_SMALL,	SHT_SIZE_CLASS_LARGE	},
	{ SHT_SIZE_CLASS_SMALL,	SHT_SIZE_CLASS_LARGE,	SHT_SIZE_CLASS_LARGE	},
	{ SHT_SIZE_CLASS_LARGE,	SHT_SIZE_CLASS_LARGE,	SHT_SIZE_CLASS_LARGE	}
};


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_Constructor
// Description   : �R���X�g���N�^
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtSheet_Constructor(CShtSheet* pThis)
{
	CShtSheet_Reset(pThis);
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_Reset
// Description   : ���Z�b�g����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtSheet_Reset(CShtSheet* pThis)
{
	UC index;

	pThis->mSetID					= 0;
	pThis->mSheetID					= 0;
	pThis->mOutputDestination		= eCdiFinOutDest_All;
	pThis->mBinSelection			= 1;
	pThis->mSetEndAction			= eCdiFinSetEndAct_NrmlEnd;
	pThis->mCondition				= SHT_COND_VOID;
	pThis->mPlan					= 0x00000000;
	pThis->mEvent					= 0x00000000;
// V3.4.0 	RAM�팸
//	pThis->mDeliveredLoc			= SHT_LOC_ID_NUM;
//	pThis->mSetFinishedLoc			= SHT_LOC_ID_NUM;
//	pThis->mSetFinishedEvent		= SHT_EVT_ID_NUM;
	pThis->mOutputDestFinish		= eCdiFinOutDest_All;
	pThis->mOffsetFinish			= SHT_OFST_FIN_NOT;
	pThis->mBuffer					= SHT_BUFFER_NOT;
	pThis->mEject					= SHT_EJECT_NOT;
	pThis->mSetTop					= SHT_SET_TOP_NOT;
	pThis->mSetEnd					= SHT_SET_END_NOT;
	pThis->mSpeedID					= 0;
	pThis->mCompileCount			= 1;
	pThis->mWeightedCompileCount	= 1;
	pThis->mDevidedSet				= SHT_SET_UNDEVIDED;
// V3.4.0 T/B�΍�@Merge No.41
	pThis->mMaxMediaWeightOfCompile	= 0;
	pThis->mMinMediaWeightOfCompile	= 0;
	pThis->mProcSizeMix				= SHT_SIZE_UNMIX;
	pThis->mCrossProcSizeMix		= SHT_SIZE_UNMIX;
	pThis->mMaxProcSizeOfCompile	= 0;
	pThis->mMaxCProcSizeOfCompile	= 0;
	pThis->mMinProcSizeOfCompile	= 0;
	pThis->mMinCProcSizeOfCompile	= 0;
//	pThis->mInclusionFoldInCompile	= SHT_FOLD_NOT_INCLUDE;
//	pThis->mInclusionFoldInSet		= SHT_FOLD_NOT_INCLUDE;
	pThis->mPrepTime				= eCdiFinPrepTime_Unknown;
//	pThis->mBufferFeedWaitTime		= 0;
	pThis->mDisperTime				= 0;

	for ( index = 0; index < SHT_EDGE_NUM; index++ ){
		pThis->mLocation[index] = 0x00000000;
	}

	CShtSheetAttribute_Reset(&(pThis->mAttribute));
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_ChangeCondition
// Description   : �R���f�B�V�����J�ڎw����M����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : event (Sheet Condition Event)
// Return        : Sheet Condition
///////////////////////////////////////////////////////////////////
EShtSheetCondition CShtSheet_ChangeCondition(CShtSheet* pThis, EShtSheetConditionEvent event)
{
	UC crntCond = pThis->mCondition;

	pThis->mCondition = cSht_ShtCondTable[pThis->mCondition][event];

	if ( crntCond != pThis->mCondition ) return (EShtSheetCondition)(pThis->mCondition);
	else								 return SHT_COND_NUM;
}


///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_CheckConditionNormal
// Description   : ���݂̃V�[�g�R���f�B�V�������A�m�[�}���ȃR���f�B
//               : �V�����Ƃ��ăJ�e�S���C�Y���ꂽ���ނɑ����Ă��邩
//               : �̊m�F����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : true = �{�J�e�S���ɓK������R���f�B�V�����ł���
//               : false = �{�J�e�S���ɓK������R���f�B�V�����łȂ�
///////////////////////////////////////////////////////////////////
bool CShtSheet_CheckConditionNormal(CShtSheet* pThis)
{
	static const UC cCheckResult[SHT_COND_NUM] = {
	//	| Void	| Candi	| Delv	| Purg	| Eject	| Cmpl	| Cancel| Leave	| Jam	| Rem	| Comp	| Reject|
		  true	, true	, true	, false	, true	, true	, false	, true	, false	, false	, true	, false
	};

	return (bool)cCheckResult[pThis->mCondition];
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_CheckConditionProcess
// Description   : ���݂̃V�[�g�R���f�B�V�������A�t�B�j�b�V���O�v��
//               : �Z�X�̏����ΏۃR���f�B�V�����Ƃ��ăJ�e�S���C�Y��
//               : �ꂽ���ނɑ����Ă��邩�̊m�F����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : true = �{�J�e�S���ɓK������R���f�B�V�����ł���
//               : false = �{�J�e�S���ɓK������R���f�B�V�����łȂ�
///////////////////////////////////////////////////////////////////
bool CShtSheet_CheckConditionProcess(CShtSheet* pThis)
{
	static const UC cCheckResult[SHT_COND_NUM] = {
	//	| Void	| Candi	| Delv	| Purg	| Eject	| Cmpl	| Cancel| Leave	| Jam	| Rem	| Comp	| Reject|
		  true	, true	, true	, true	, true	, true	, false	, false	, false	, false	, true	, true
	};

	return (bool)cCheckResult[pThis->mCondition];
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_CheckConditionDelivery
// Description   : ���݂̃V�[�g�R���f�B�V�������A�����Ώ�(�f���o�[
//               : �h�O)�̃R���f�B�V�����Ƃ��ăJ�e�S���C�Y���ꂽ��
//               : �ނɑ����Ă��邩�̊m�F����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : true = �{�J�e�S���ɓK������R���f�B�V�����ł���
//               : false = �{�J�e�S���ɓK������R���f�B�V�����łȂ�
///////////////////////////////////////////////////////////////////
bool CShtSheet_CheckConditionDelivery(CShtSheet* pThis)
{
	static const UC cCheckResult[SHT_COND_NUM] = {
	//	| Void	| Candi	| Delv	| Purg	| Eject	| Cmpl	| Cancel| Leave	| Jam	| Rem	| Comp	| Reject|
		  false	, false	, true	, true	, true	, false	, false	, false	, false	, false	, false	, false
	};

	return (bool)cCheckResult[pThis->mCondition];
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_CheckConditionNormalDelivery
// Description   : ���݂̃V�[�g�R���f�B�V�������A�m�[�}���Ȕ����Ώ�
//               : (�f���o�[�h�O)�̃R���f�B�V�����Ƃ��ăJ�e�S���C�Y
//               : ���ꂽ���ނɑ����Ă��邩�̊m�F����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : true = �{�J�e�S���ɓK������R���f�B�V�����ł���
//               : false = �{�J�e�S���ɓK������R���f�B�V�����łȂ�
///////////////////////////////////////////////////////////////////
bool CShtSheet_CheckConditionNormalDelivery(CShtSheet* pThis)
{
	static const UC cCheckResult[SHT_COND_NUM] = {
	//	| Void	| Candi	| Delv	| Purg	| Eject	| Cmpl	| Cancel| Leave	| Jam	| Rem	| Comp	| Reject|
		  false	, false	, true	, false	, true	, false	, false	, false	, false	, false	, false	, false
	};

	return (bool)cCheckResult[pThis->mCondition];
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_CheckConditionAbnormalDelivery
// Description   : ���݂̃V�[�g�R���f�B�V�������A�A�u�m�[�}���Ȕ���
//               : (�p�[�W����)�Ώ�(�f���o�[�h�O)�̃R���f�B�V������
//               : ���ăJ�e�S���C�Y���ꂽ���ނɑ����Ă��邩�̊m�F��
//               : ��
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : true = �{�J�e�S���ɓK������R���f�B�V�����ł���
//               : false = �{�J�e�S���ɓK������R���f�B�V�����łȂ�
///////////////////////////////////////////////////////////////////
bool CShtSheet_CheckConditionAbnormalDelivery(CShtSheet* pThis)
{
	static const UC cCheckResult[SHT_COND_NUM] = {
	//	| Void	| Candi	| Delv	| Purg	| Eject	| Cmpl	| Cancel| Leave	| Jam	| Rem	| Comp	| Reject|
		  false	, false	, false	, true	, false	, false	, false	, false	, false	, false	, false	, false
	};

	return (bool)cCheckResult[pThis->mCondition];
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_CheckConditionWaitEject
// Description   : ���݂̃V�[�g�R���f�B�V�������A�C�W�F�N�g��҂���
//               : �R���f�B�V�����Ƃ��ăJ�e�S���C�Y���ꂽ���ނɑ���
//               : �Ă��邩�̊m�F����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : true = �{�J�e�S���ɓK������R���f�B�V�����ł���
//               : false = �{�J�e�S���ɓK������R���f�B�V�����łȂ�
///////////////////////////////////////////////////////////////////
bool CShtSheet_CheckConditionWaitEject(CShtSheet* pThis)
{
	static const UC cCheckResult[SHT_COND_NUM] = {
	//	| Void	| Candi	| Delv	| Purg	| Eject	| Cmpl	| Cancel| Leave	| Jam	| Rem	| Comp	| Reject|
		  false	, false	, false	, false	, true	, true	, false	, true	, false	, false	, false	, false
	};

	return (bool)cCheckResult[pThis->mCondition];
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_CheckConditionWaitFinish
// Description   : ���݂̃V�[�g�R���f�B�V�������A�Z�b�g�t�B�j�b�V��
//               : �҂��̃R���f�B�V�����Ƃ��ăJ�e�S���C�Y���ꂽ����
//               : �ɑ����Ă��邩�̊m�F����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : true = �{�J�e�S���ɓK������R���f�B�V�����ł���
//               : false = �{�J�e�S���ɓK������R���f�B�V�����łȂ�
///////////////////////////////////////////////////////////////////
bool CShtSheet_CheckConditionWaitFinish(CShtSheet* pThis)
{
	static const UC cCheckResult[SHT_COND_NUM] = {
	//	| Void	| Candi	| Delv	| Purg	| Eject	| Cmpl	| Cancel| Leave	| Jam	| Rem	| Comp	| Reject|
		  false	, false	, false	, false	, true	, true	, false	, true	, false	, false	, true	, false
	};

	return (bool)cCheckResult[pThis->mCondition];
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_CheckConditionSetFinishReceive
// Description   : ���݂̃V�[�g�R���f�B�V�������ASet Finish Receive �m�F����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : true = �{�J�e�S���ɓK������R���f�B�V�����ł���
//               : false = �{�J�e�S���ɓK������R���f�B�V�����łȂ�
///////////////////////////////////////////////////////////////////
bool CShtSheet_CheckConditionSetFinishReceive(CShtSheet* pThis)
{
	static const UC cCheckResult[SHT_COND_NUM] = {
	//	| Void	| Candi	| Delv	| Purg	| Eject	| Cmpl	| Cancel| Leave	| Jam	| Rem	| Comp	| Reject|
		  false	, false	, false	, false	, true	, false	, false	, false	, false	, false	,false	, false
	};

	return (bool)cCheckResult[pThis->mCondition];
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_CheckConditionAbnormalEnd
// Description   : ���݂̃V�[�g�R���f�B�V�������A�ُ�I���̃R���f�B
//               : �V�����Ƃ��ăJ�e�S���C�Y���ꂽ���ނɑ����Ă��邩
//               : �̊m�F����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : true = �{�J�e�S���ɓK������R���f�B�V�����ł���
//               : false = �{�J�e�S���ɓK������R���f�B�V�����łȂ�
///////////////////////////////////////////////////////////////////
bool CShtSheet_CheckConditionAbnormalEnd(CShtSheet* pThis)
{
	static const UC cCheckResult[SHT_COND_NUM] = {
	//	| Void	| Candi	| Good	| Purg	| Eject	| Delv	| Cancel| Leave	| Jam	| Rem	| Comp	| Purged|
		  false	, false	, false	, false	, false	, false	, true	, true	, true	, true	, false	, true
	};

	return (bool)cCheckResult[pThis->mCondition];
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_CheckConditionStop
// Description   : ���݂̃V�[�g�R���f�B�V�������A������~���ׂ��R
//               : ���f�B�V�����Ƃ��ăJ�e�S���C�Y���ꂽ���ނɑ�����
//               : ���邩�̊m�F����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : true = �{�J�e�S���ɓK������R���f�B�V�����ł���
//               : false = �{�J�e�S���ɓK������R���f�B�V�����łȂ�
///////////////////////////////////////////////////////////////////
bool CShtSheet_CheckConditionStop(CShtSheet* pThis)
{
	static const UC cCheckResult[SHT_COND_NUM] = {
	//	| Void	| Candi	| Good	| Purg	| Eject	| Delv	| Cancel| Leave	| Jam	| Rem	| Comp	| Purged|
		  false	, false	, false	, false	, false	, false	, false	, true	, true	, true	, false	, false
	};

	return (bool)cCheckResult[pThis->mCondition];
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_CheckLocationPlan
// Description   : �V�[�g���w��̃��P�[�V������ʉ߂���\�肪���邩
//               : �ǂ����̊m�F����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : locationID (Sheet Location ID)
// Return        : true = �ʉߗ\�肠��
//               : false = �ʉߗ\��Ȃ�
///////////////////////////////////////////////////////////////////
bool CShtSheet_CheckLocationPlan(CShtSheet* pThis, EShtSheetLocationID locationID)
{
	UL checkBit = SHT_LOC_CNV_BIT(locationID);

	if ( pThis->mPlan & checkBit ) return true;
	else						   return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_GetNextLocationPlan
// Description   : �V�[�g���w��̃��P�[�V�����̎��ɓ��B�\��̃��P�[
//               : �V�������擾���鏈��
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : locationID (Sheet Location ID)
// Return        : Sheet Location ID
///////////////////////////////////////////////////////////////////
EShtSheetLocationID CShtSheet_GetNextLocationPlan(CShtSheet* pThis, EShtSheetLocationID locationID)
{
	UC index;

	if ( CShtSheet_CheckLocationPlan(pThis, locationID) == true ){
		index = (UC)(locationID + 1);

		while ( index < SHT_LOC_ID_NUM ){
			if ( pThis->mPlan & SHT_LOC_CNV_BIT(index) ){
				return index;
			}
			else{ /* Go while loop */ }

			index++;
		}
	}
	else{ /* No Action */ }

	return SHT_LOC_ID_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_GetLastLocationPlan
// Description   : �V�[�g���w��̃��P�[�V�����̑O�ɓ��B�\��̃��P�[
//               : �V�������擾���鏈��
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : locationID (Sheet Location ID)
// Return        : Sheet Location ID
///////////////////////////////////////////////////////////////////
EShtSheetLocationID CShtSheet_GetLastLocationPlan(CShtSheet* pThis, EShtSheetLocationID locationID)
{
	UC index;

	if ( CShtSheet_CheckLocationPlan(pThis, locationID) == true ){
		index = locationID;

		while ( index != 0 ){
			index--;

			if ( pThis->mPlan & SHT_LOC_CNV_BIT(index) ){
				return index;
			}
			else{ /* Go while loop */ }
		}
	}
	else{ /* No Action */ }

	return SHT_LOC_ID_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_SetLocation
// Description   : ���P�[�V������������(����)���Z�b�g���鏈��
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : locationID (Sheet Location ID)
//               : edge (�V�[�g�G�b�W���ʎq)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtSheet_SetLocation(CShtSheet* pThis, EShtSheetLocationID locationID, EShtSheetEdge edge)
{
	pThis->mLocation[edge] |= SHT_LOC_CNV_BIT(locationID);
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_CheckLocationArrival
// Description   : ���P�[�V�����̔����󋵂��m�F���鏈��
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : locationID (Sheet Location ID)
//               : edge (�V�[�g�G�b�W���ʎq)
// Return        : true = �����ς�
//               : false = ������
///////////////////////////////////////////////////////////////////
bool CShtSheet_CheckLocationArrival(CShtSheet* pThis, EShtSheetLocationID locationID, EShtSheetEdge edge)
{
	if ( pThis->mLocation[edge] & SHT_LOC_CNV_BIT(locationID) ) return true;
	else														return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_GetNextLocation
// Description   : �V�[�g�����ɓ��B�\��̃��P�[�V�������擾���鏈��
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : edge (�V�[�g�G�b�W���ʎq)
// Return        : Sheet Location ID
///////////////////////////////////////////////////////////////////
EShtSheetLocationID CShtSheet_GetNextLocation(CShtSheet* pThis, EShtSheetEdge edge)
{
	UL unarriveBit, checkBit;
	UC locationID;

	unarriveBit = pThis->mPlan & ~(pThis->mLocation[edge]);

	checkBit = 0x00000001;

	for ( locationID = 0; locationID < SHT_LOC_ID_NUM; locationID++ ){
		if ( unarriveBit & checkBit ) return locationID;
		else						  checkBit <<= 1;
	}

	return SHT_LOC_ID_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_GetLastLocation
// Description   : �V�[�g�����߂œ��B�������P�[�V�������擾���鏈��
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : edge (�V�[�g�G�b�W���ʎq)
// Return        : 0 �` (SHT_LOC_ID_NUM - 1) = ���߂œ��B�������P�[
//               : �V������Sheet Location ID
//               : SHT_LOC_ID_NUM = �Y�����P�[�V�����Ȃ�
///////////////////////////////////////////////////////////////////
EShtSheetLocationID CShtSheet_GetLastLocation(CShtSheet* pThis, EShtSheetEdge edge)
{
	UL checkBit;
	UC locationID;

	checkBit = pThis->mLocation[edge];

	for ( locationID = 0; locationID < SHT_LOC_ID_NUM; locationID++ ){
		if ( checkBit == 0x00000001 ){
			return locationID;
		}
		else{
			checkBit >>= 1;
		}
	}

	return SHT_LOC_ID_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_CheckDelivereded
// Description   : �f���o�[�h�ς݂��ǂ����̊m�F����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : true = �f���o�[�h�ς�
//               : false = ���f���o�[�h
///////////////////////////////////////////////////////////////////V3.4.0 Merge
//bool CShtSheet_CheckDelivereded(CShtSheet* pThis)
//{
//	if ( pThis->mDeliveredLoc != SHT_LOC_ID_NUM ) return CShtSheet_CheckLocationArrival(pThis, pThis->mDeliveredLoc, SHT_EDGE_TAIL);
//	else										  return false;
//}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_CheckEventOccurence
// Description   : �C�x���g�̔����󋵂��m�F���鏈��
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : eventID (Sheet Event ID)
// Return        : true = �����ς�
//               : false = ������
///////////////////////////////////////////////////////////////////
bool CShtSheet_CheckEventOccurence(CShtSheet* pThis, EShtSheetEventID eventID)
{
	if ( pThis->mEvent & SHT_EVT_ID_CNV_BIT(eventID) ) return true;
	else											   return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_GetSizeClass
// Description   : �T�C�Y�N���X�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Size Class
///////////////////////////////////////////////////////////////////
EShtSizeClass CShtSheet_GetSizeClass(CShtSheet* pThis)
{
	UC procSizeClass = CShtSheet_GetProcSizeClass(pThis);
	UC crossProcSizeClass = CShtSheet_GetCrossProcSizeClass(pThis);

	return (EShtSizeClass)(cSht_ShtSizeClass[procSizeClass][crossProcSizeClass]);
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_GetProcSizeClass
// Description   : �p�����蒷�����̃T�C�Y�N���X�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Size Class
///////////////////////////////////////////////////////////////////
EShtSizeClass CShtSheet_GetProcSizeClass(CShtSheet* pThis)
{
	US size = CShtSheetAttribute_GetProcessSize(&(pThis->mAttribute));

	if ( size <= SHT_SIZE_CLASS_SMALL_THRESH )		return SHT_SIZE_CLASS_SMALL;
	else if ( size <= SHT_SIZE_CLASS_LARGE_THRESH ) return SHT_SIZE_CLASS_MIDDLE;
	else											return SHT_SIZE_CLASS_LARGE;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_GetCrossProcSizeClass
// Description   : �p���������̃T�C�Y�N���X�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Size Class
///////////////////////////////////////////////////////////////////
EShtSizeClass CShtSheet_GetCrossProcSizeClass(CShtSheet* pThis)
{
	US size = CShtSheetAttribute_GetCrossProcSize(&(pThis->mAttribute));

	if ( size <= SHT_SIZE_CLASS_SMALL_THRESH )		return SHT_SIZE_CLASS_SMALL;
	else if ( size <= SHT_SIZE_CLASS_LARGE_THRESH ) return SHT_SIZE_CLASS_MIDDLE;
	else											return SHT_SIZE_CLASS_LARGE;	
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_GetSizeMix
// Description   : �T�C�Y�~�b�N�X���擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Size Mix
///////////////////////////////////////////////////////////////////
EShtSizeMix CShtSheet_GetSizeMix(CShtSheet* pThis)
{
	if ( (pThis->mProcSizeMix == SHT_SIZE_MIX) || (pThis->mCrossProcSizeMix == SHT_SIZE_MIX) ) return SHT_SIZE_MIX;
	else																					   return SHT_SIZE_UNMIX;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_GetSheetIntegrity
// Description   : �V�[�g�C���e�O���e�B�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Sheet Integrity
///////////////////////////////////////////////////////////////////
enum ECdiFinIntegrity CShtSheet_GetSheetIntegrity(CShtSheet* pThis)
{
	if ( (CShtSheet_CheckConditionNormal(pThis) == true) &&
		 (CShtSheetAttribute_GetSheetIntegrity(&(pThis->mAttribute)) == eCdiFinIntegrity_Normal) ){
		return eCdiFinIntegrity_Normal;
	}
	else{
		return eCdiFinIntegrity_Abnormal;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_GetSetIntegrity
// Description   : �Z�b�g�C���e�O���e�B�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Set Integrity
///////////////////////////////////////////////////////////////////
enum ECdiFinIntegrity CShtSheet_GetSetIntegrity(CShtSheet* pThis)
{
	if( pThis->mSetEndAction == eCdiFinSetEndAct_NrmlEnd ) return eCdiFinIntegrity_Normal;
	else												   return eCdiFinIntegrity_Abnormal;
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
// 15.03.30	| Yuichi Hoshino	| Design Improvement [ �ُ��~���̃C�j�V�����C�Y�g���K�C�x���g�ʒm�����Ή�(CShtSheet_CheckConditionAbnormalEnd()�ǉ�) ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
