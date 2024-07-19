// File Name:	CShtSheet.h
// Description:	Header File of Sheet
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CShtSheet_h
#define CShtSheet_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "CShtSheetAttribute.h"
#include "EShtSheetEdge.h"
#include "EShtSheetCondition.h"
#include "EShtSheetLocationID.h"
#include "EShtSheetEventID.h"
#include "ECdiFinOutDest.h"
#include "EShtOffsetFinish.h"
#include "EShtBuffer.h"
#include "EShtEject.h"
#include "EShtSetTop.h"
#include "EShtSetEnd.h"
#include "EShtFinisherSpeedID.h"
#include "ECdiFinSetEndAct.h"
#include "EShtDevidedSet.h"
#include "EShtSizeClass.h"
#include "EShtSizeMix.h"
#include "EShtFoldInclusion.h"
#include "ECdiFinPrepTime.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Sheet Condition Change Event ---
typedef enum /*EShtSheetConditionEvent*/
{
	SHT_COND_CHG_EVT_ENTRY,				// �G���g���[
	SHT_COND_CHG_EVT_NORMAL_EXIT,		// �m�[�}���C�O�W�b�g
	SHT_COND_CHG_EVT_COMPLETE,			// �R���v���[�g
	SHT_COND_CHG_EVT_DELIVERED,			// �f���o�[�h
	SHT_COND_CHG_EVT_EJECT_DETECT,		// �C�W�F�N�g�f�B�e�N�g
	SHT_COND_CHG_EVT_ABNORMAL_EXIT,		// �A�u�m�[�}���C�O�W�b�g
	SHT_COND_CHG_EVT_PURGE,				// �p�[�W
	SHT_COND_CHG_EVT_CANCEL,			// �L�����Z��
	SHT_COND_CHG_EVT_STOP,				// �X�g�b�v
	SHT_COND_CHG_EVT_JAM,				// �W����
	SHT_COND_CHG_EVT_ABORT,				// �A�{�[�g
	SHT_COND_CHG_EVT_NUM
} EShtSheetConditionEvent;


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Sheet ---
typedef struct /*CShtSheet*/
{
	CShtSheetAttribute mAttribute;		// IOT����ʒm���ꂽAttribute���������B���ʒm���ꂽ�܂܂̏���ێ�����B
	// 4Byte Data
	UL mPlan;							// ��{�����v��(�o�b�t�@�[����Ȃ�����)�������B(���G�b�W�i��[�^��[�j����)
	UL mLocation[SHT_EDGE_NUM];			// �V�[�g��[(ON)/��[(OFF)�̒P�����������������B
	UL mEvent;							// �p���C�x���g�̗����������B
	// 2Byte Data
	US mWeightedCompileCount;			// �R���p�C���g���C�ւ̐ύڂ���Ă���p�����́A�����T�C�Y�ɂ��d�ݕt�����ꂽ�����ɂ�
	US mMaxMediaWeightOfCompile;		// �ΏۃV�[�g���܂ރR���p�C���V�[�g�����ōł��d���ؗʂ������BV3.4.0 Merge
	US mMinMediaWeightOfCompile;		// �ΏۃV�[�g���܂ރR���p�C���V�[�g�����ōł��y���ؗʂ������BV3.4.0 Merge
	US mMaxProcSizeOfCompile;			// �ΏۃV�[�g���܂ރR���p�C���V�[�g�����ōł������p�����蒷�������B
	US mMaxCProcSizeOfCompile;			// �ΏۃV�[�g���܂ރR���p�C���V�[�g�����ōł������p�����������B
	US mMinProcSizeOfCompile;			// �ΏۃV�[�g���܂ރR���p�C���V�[�g�����ōł��Z���p�����蒷�������B
	US mMinCProcSizeOfCompile;			// �ΏۃV�[�g���܂ރR���p�C���V�[�g�����ōł��Z���p�����������B
	US mPrepTime;						// Preparation Time
//	US mBufferFeedWaitTime;				// Wait time of buffer feed	(RAM�팸)
	// 1Byte Data(8Bit�K�v�Ȃ��� or ��肤��l���ϓ��������)
	UC mSetID;							// Set ID�������B(IOT����ʒm�����Z�b�g�̎��ʎq)
	UC mSheetID;						// Sheet ID�������B(IOT����ʒm�����V�[�g�̎��ʎq)
	UC mOutputDestination;				// �r����������B
	UC mBinSelection;					// Bin Selection�������B
	UC mCondition;						// �V�[�g�R���f�B�V�����������B
//	UC mDeliveredLoc;					// delivered point location //V3.4.0 Merge
//	UC mSetFinishedLoc;					// set finished point location
//	UC mSetFinishedEvent;				// set finished point event
	UC mOutputDestFinish;				// IOT����ʒm���ꂽ���Ƃ��̑��̎��ۂ��l�������ŏI�I�͔r�����ێ�����B
	UC mSpeedID;						// �t�B�j�b�V���[�������x�������B
	UC mCompileCount;					// �R���p�C���g���C�ւ̐ύڂ���Ă���p�����̐ύږ����������B
										// �R���p�C���g���C���Ȃ��r����ɔr�o�����V�[�g�͏��1�ł���B
	UC mDisperTime;						// Dispersion Time
	// 1Byte Data(Bit����t���\��(��肤��l�����肵�Ă���8Bit�K�v�Ƃ��Ȃ�)����)
	UC mOffsetFinish:3;					// �ŏI�I��(�p����񓙂��ӂ݂Ĕ��f�������ʂ�)�I�t�Z�b�g�|�W�V�����������B
	UC mBuffer:2;						// �o�b�t�@����Ώێ����������B
	UC mSetEndAction:2;					// Set End Action�������B 20160225 V3.4.0 Merge [ �Z�b�g�A�{�[�g���ɃX�e�[�v�����L�����Z������Ȃ� ]
	UC mEject:1;						// ���̃V�[�g�̔����������g���K�[�ɔr����������{���邩�ǂ����������B
	UC mSetTop:1;						// �Z�b�g�擪�����ǂ����������B
	UC mSetEnd:1;						// �Z�b�g�ŏI�����ǂ����������B
	UC mDevidedSet:1;					// �ΏۃV�[�g���ύڂ��ꂽ�ۂ̐ύږ����������B
										// �R���p�C���g���C���Ȃ��r����ɔr�o�����V�[�g�͏��1�ł���B
										// �����r�����{�Z�b�g�ɑ�����V�[�g�ł��邩�������B
	UC mProcSizeMix:1;					// �ΏۃV�[�g���܂ރZ�b�g�̗p�����蒷���T�C�Y�~�b�N�X���Ă��邩�ǂ����������B
										// ���㑱�̃V�[�g�ŃT�C�Y�~�b�N�X�����������ۂɁA��s�V�[�g�̏��͍X�V����Ȃ�����
										// ���ӂ��K�v�I(�ΏۃZ�b�g�ŃT�C�Y�~�b�N�X�ɂȂ��Ă��邩�ǂ����̓`�F�b�N�^�C�~���O
										// �ɂ�����Ō���̃V�[�g�̏�񂩂画�f���邱��)
										// �Ȃ��A�f�B�o�C�f�B�b�h�C�W�F�N�g�ł̓T�C�Y�~�b�N�X���̓N���A����Ȃ��B
	UC mCrossProcSizeMix:1;				// �ΏۃV�[�g���܂ރZ�b�g�̗p�������T�C�Y�~�b�N�X���Ă��邩�ǂ����������B
//	UC mInclusionFoldInCompile:1;		// �ΏۃV�[�g���܂ރR���p�C���V�[�g�����ɐ܂�V�[�g���܂܂�Ă��邩�������B
//	UC mInclusionFoldInSet:1;			// �ΏۃV�[�g���܂ރZ�b�g���ɐ܂�V�[�g���܂܂�Ă��邩�������B
} CShtSheet;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CShtSheet_Constructor(CShtSheet* pThis);

void CShtSheet_Reset(CShtSheet* pThis);

#define CShtSheet_GetSetID(apThis) ((apThis)->mSetID)							// UC (CShtSheet*)
#define CShtSheet_SetSetID(apThis, aSetID) ((apThis)->mSetID = aSetID)			// void (CShtSheet*, UC)
#define CShtSheet_GetSheetID(apThis) ((apThis)->mSheetID)						// UC (CShtSheet*)
#define CShtSheet_SetSheetID(apThis, aSheetID) ((apThis)->mSheetID = aSheetID)	// void (CShtSheet*, UC)
#define CShtSheet_GetOutputDestination(apThis) ((enum ECdiFinOutDest)((apThis)->mOutputDestination))
																				// ECdiFinOutDest (CShtSheet*)
#define CShtSheet_SetOutputDestination(apThis, aOutDest) ((apThis)->mOutputDestination = aOutDest)
																				// void (CShtSheet*, ECdiFinOutDest)
#define CShtSheet_GetBinSelection(apThis) ((apThis)->mBinSelection)				// UC (CShtSheet*)
#define CShtSheet_SetBinSelection(apThis, aBinSlct) ((apThis)->mBinSelection = aBinSlct)
																				// void (CShtSheet*, UC)
#define CShtSheet_GetSetEndAction(apThis) ((enum ECdiFinSetEndAct)((apThis)->mSetEndAction))
																				// ECdiFinSetEndAct (CShtSheet*)
#define CShtSheet_SetSetEndAction(apThis, aSetEndAct) ((apThis)->mSetEndAction = aSetEndAct)
																				// void (CShtSheet*, ECdiFinSetEndAct)
#define CShtSheet_GetAttribute(apThis) ((const CShtSheetAttribute*)(&((apThis)->mAttribute)))
																				// const CShtSheetAttribute* (CShtSheet*)
#define CShtSheet_SetAttribute(apThis, apAttr) ((apThis)->mAttribute = *apAttr)	// void (CShtSheet*, CShtSheetAttribute* )
#define CShtSheet_GetCondition(apThis) ((EShtSheetCondition)((apThis)->mCondition))
																				// EShtSheetCondition (CShtSheet*)
EShtSheetCondition CShtSheet_ChangeCondition(CShtSheet* pThis, EShtSheetConditionEvent event);
bool CShtSheet_CheckConditionNormal(CShtSheet* pThis);
bool CShtSheet_CheckConditionProcess(CShtSheet* pThis);
bool CShtSheet_CheckConditionDelivery(CShtSheet* pThis);
bool CShtSheet_CheckConditionNormalDelivery(CShtSheet* pThis);
bool CShtSheet_CheckConditionAbnormalDelivery(CShtSheet* pThis);
bool CShtSheet_CheckConditionWaitEject(CShtSheet* pThis);
bool CShtSheet_CheckConditionWaitFinish(CShtSheet* pThis);
bool CShtSheet_CheckConditionSetFinishReceive(CShtSheet* pThis);  //V3.0.4  

bool CShtSheet_CheckConditionAbnormalEnd(CShtSheet* pThis);
bool CShtSheet_CheckConditionStop(CShtSheet* pThis);
#define CShtSheet_GetLocationPlan(apThis) ((apThis)->mPlan) 					// UL (CShtSheet*)
#define CShtSheet_SetLocationPlan(apThis, aPlan) ((apThis)->mPlan = aPlan)		// void (CShtSheet*, UL)
bool CShtSheet_CheckLocationPlan(CShtSheet* pThis, EShtSheetLocationID locationID);
EShtSheetLocationID CShtSheet_GetNextLocationPlan(CShtSheet* pThis, EShtSheetLocationID locationID);
EShtSheetLocationID CShtSheet_GetLastLocationPlan(CShtSheet* pThis, EShtSheetLocationID locationID);
#define CShtSheet_GetLocation(apThis, aEdge) ((apThis)->mLocation[aEdge]) 		// UL (CShtSheet*, EShtSheetEdge)
void CShtSheet_SetLocation(CShtSheet* pThis, EShtSheetLocationID locationID, EShtSheetEdge edge);
bool CShtSheet_CheckLocationArrival(CShtSheet* pThis, EShtSheetLocationID locationID, EShtSheetEdge edge);
EShtSheetLocationID CShtSheet_GetNextLocation(CShtSheet* pThis, EShtSheetEdge edge);
EShtSheetLocationID CShtSheet_GetLastLocation(CShtSheet* pThis, EShtSheetEdge edge);
#define CShtSheet_SetEvent(apThis, aEventID) ((apThis)->mEvent |= (0x00000001 << aEventID))
																				// void (CShtSheet*, EShtSheetEventID)
bool CShtSheet_CheckEventOccurence(CShtSheet* pThis, EShtSheetEventID eventID);
//V3.4.0 Merge
//#define CShtSheet_GetDeliveredLocation(apThis) ((apThis)->mDeliveredLoc)		// EShtSheetLocationID (CShtSheet*)
//#define CShtSheet_SetDeliveredLocation(apThis, aLocationID) ((apThis)->mDeliveredLoc = aLocationID)
																				// void (CShtSheet*, EShtSheetLocationID)
//#define CShtSheet_GetSetFinishedLocation(apThis) ((apThis)->mSetFinishedLoc)	// EShtSheetLocationID (CShtSheet*)
//#define CShtSheet_SetSetFinishedLocation(apThis, aLocationID) ((apThis)->mSetFinishedLoc = aLocationID)
																				// void (CShtSheet*, EShtSheetLocationID)
//#define CShtSheet_GetSetFinishedEvent(apThis) ((apThis)->mSetFinishedEvent)		// EShtSheetEventID (CShtSheet*)
//#define CShtSheet_SetSetFinishedEvent(apThis, aEventID) ((apThis)->mSetFinishedEvent = aEventID)
//bool CShtSheet_CheckDelivereded(CShtSheet* pThis);							// (RAM�팸)
#define CShtSheet_GetOutputDestFinish(apThis) ((enum ECdiFinOutDest)((apThis)->mOutputDestFinish))
																				// ECdiFinOutDest (CShtSheet*)
#define CShtSheet_SetOutputDestFinish(apThis, aOutDest) ((apThis)->mOutputDestFinish = aOutDest)
																				// void (CShtSheet*, ECdiFinOutDest)
#define CShtSheet_GetOffsetFinish(apThis) ((apThis)->mOffsetFinish)				// EShtOffsetFinish (CShtSheet*)
#define CShtSheet_SetOffsetFinish(apThis, aOffsetFin) ((apThis)->mOffsetFinish = aOffsetFin)
																				// void (CShtSheet*, EShtOffsetFinish)
#define CShtSheet_GetBuffer(apThis) ((EShtBuffer)((apThis)->mBuffer))			// EShtBuffer (CShtSheet*)
#define CShtSheet_SetBuffer(apThis, aBuffer) ((apThis)->mBuffer = aBuffer)		// void (CShtSheet*, EShtBuffer)
#define CShtSheet_GetEject(apThis) ((EShtEject)((apThis)->mEject))				// EShtEject (CShtSheet*)
#define CShtSheet_SetEject(apThis, aEject) ((apThis)->mEject = aEject)			// void (CShtSheet*, EShtEject)
#define CShtSheet_GetSetTop(apThis) ((EShtSetTop)((apThis)->mSetTop))			// EShtSetTop (CShtSheet*)
#define CShtSheet_SetSetTop(apThis, aSetTop) ((apThis)->mSetTop = aSetTop)		// void (CShtSheet*, EShtSetTop)
#define CShtSheet_GetSetEnd(apThis) ((EShtSetEnd)((apThis)->mSetEnd))			// EShtSetEnd (CShtSheet*)
#define CShtSheet_SetSetEnd(apThis, aSetEnd) ((apThis)->mSetEnd = aSetEnd)		// void (CShtSheet*, EShtSetEnd)

#define CShtSheet_GetFinisherSpeedID(apThis) ((apThis)->mSpeedID)				// EShtFinisherSpeedID (CShtSheet*)
#define CShtSheet_SetFinisherSpeedID(apThis, aSpdID) ((apThis)->mSpeedID = aSpdID)
																				// void (CShtSheet*, EShtFinisherSpeedID)
#define CShtSheet_GetCompileCount(apThis) ((apThis)->mCompileCount)				// UC (CShtSheet*)
#define CShtSheet_SetCompileCount(apThis, aCount) ((apThis)->mCompileCount = aCount)
																				// void (CShtSheet*, UC)
#define CShtSheet_GetWeightedCompileCount(apThis) ((apThis)->mWeightedCompileCount)
																				// US (CShtSheet*)
#define CShtSheet_SetWeightedCompileCount(apThis, aCount) ((apThis)->mWeightedCompileCount = aCount)
																				// void (CShtSheet*, US)
#define CShtSheet_GetDevidedSet(apThis) ((EShtDevidedSet)((apThis)->mDevidedSet))
																				// EShtDevidedSet (CShtSheet*)
#define CShtSheet_SetDevidedSet(apThis, aDevided) ((apThis)->mDevidedSet = aDevided)
																				// void (CShtSheet*, EShtDevidedSet)
//V3.4.0 Merge
#define CShtSheet_GetMaxMediaWeightOfCompile(apThis) ((apThis)->mMaxMediaWeightOfCompile)
																				// US (CShtSheet*)
#define CShtSheet_SetMaxMediaWeightOfCompile(apThis, aWeight) ((apThis)->mMaxMediaWeightOfCompile = aWeight)
																				// void (CShtSheet*, US)
#define CShtSheet_GetMinMediaWeightOfCompile(apThis) ((apThis)->mMinMediaWeightOfCompile)
																				// US (CShtSheet*)
#define CShtSheet_SetMinMediaWeightOfCompile(apThis, aWeight) ((apThis)->mMinMediaWeightOfCompile = aWeight)
																				// void (CShtSheet*, US)																				
																				
EShtSizeClass CShtSheet_GetSizeClass(CShtSheet* pThis);
EShtSizeClass CShtSheet_GetProcSizeClass(CShtSheet* pThis);
EShtSizeClass CShtSheet_GetCrossProcSizeClass(CShtSheet* pThis);
EShtSizeMix CShtSheet_GetSizeMix(CShtSheet* pThis);
#define CShtSheet_GetProcSizeMix(apThis) ((EShtSizeMix)((apThis)->mProcSizeMix))// EShtSizeMix (CShtSheet*)
#define CShtSheet_SetProcSizeMix(apThis, aSizeMix) ((apThis)->mProcSizeMix = aSizeMix)
																				// void (CShtSheet*, EShtSizeMix)
#define CShtSheet_GetCrossProcSizeMix(apThis) ((EShtSizeMix)((apThis)->mCrossProcSizeMix))
																				// EShtSizeMix (CShtSheet*)
#define CShtSheet_SetCrossProcSizeMix(apThis, aSizeMix) ((apThis)->mCrossProcSizeMix = aSizeMix)
																				// void (CShtSheet*, EShtSizeMix)
#define CShtSheet_GetMaxProcSizeOfCompile(apThis) ((apThis)->mMaxProcSizeOfCompile)
																				// US (CShtSheet*)
#define CShtSheet_SetMaxProcSizeOfCompile(apThis, aSize) ((apThis)->mMaxProcSizeOfCompile = aSize)
																				// void (CShtSheet*, US)
#define CShtSheet_GetMaxCrossProcSizeOfCompile(apThis) ((apThis)->mMaxCProcSizeOfCompile)
																				// US (CShtSheet*)
#define CShtSheet_SetMaxCrossProcSizeOfCompile(apThis, aSize) ((apThis)->mMaxCProcSizeOfCompile = aSize)
																				// void (CShtSheet*, US)
#define CShtSheet_GetMinProcSizeOfCompile(apThis) ((apThis)->mMinProcSizeOfCompile)
																				// US (CShtSheet*)
#define CShtSheet_SetMinProcSizeOfCompile(apThis, aSize) ((apThis)->mMinProcSizeOfCompile = aSize)
																				// void (CShtSheet*, US)
#define CShtSheet_GetMinCrossProcSizeOfCompile(apThis) ((apThis)->mMinCProcSizeOfCompile)
																				// US (CShtSheet*)
#define CShtSheet_SetMinCrossProcSizeOfCompile(apThis, aSize) ((apThis)->mMinCProcSizeOfCompile = aSize)
																			// void (CShtSheet*, US)
#define CShtSheet_GetInclusionFoldInCompile(apThis) (SHT_FOLD_NOT_INCLUDE)		// EShtFoldInclusion (CShtSheet*) (RAM�팸)
#define CShtSheet_SetInclusionFoldInCompile(apThis, aInclusion) (NULL)			// void (CShtSheet*, EShtFoldInclusion) (RAM�팸)
#define CShtSheet_GetInclusionFoldInSet(apThis) (SHT_FOLD_NOT_INCLUDE)			// EShtFoldInclusion (CShtSheet*) (RAM�팸)
#define CShtSheet_SetInclusionFoldInSet(apThis, aInclusion) (NULL)				// void (CShtSheet*, EShtFoldInclusion) (RAM�팸)
																				// void (CShtSheet*, EShtFoldInclusion)
#define CShtSheet_GetPrepTime(apThis) ((apThis)->mPrepTime)						// US (CShtSheet*)
#define CShtSheet_SetPrepTime(apThis, aTime) ((apThis)->mPrepTime = aTime)		// void (CShtSheet*, US)
#define CShtSheet_GetBufferFeedWaitTime(apThis) (0)								// US (CShtSheet*) (RAM�팸)
#define CShtSheet_SetBufferFeedWaitTime(apThis, aTime) (NULL)					// void (CShtSheet*, US) (RAM�팸)
																				// void (CShtSheet*, US)
#define CShtSheet_GetDispersionTime(apThis) (0)									// UC (CShtSheet*) (RAM�팸)
#define CShtSheet_SetDispersionTime(apThis, aTime) (NULL)						// void (CShtSheet*, UC) (RAM�팸)
																				// void (CShtSheet*, UC)
enum ECdiFinIntegrity CShtSheet_GetSheetIntegrity(CShtSheet* pThis);
enum ECdiFinIntegrity CShtSheet_GetSetIntegrity(CShtSheet* pThis);


#endif // #ifndef CShtSheet_h

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
// 15.09.01	| Yuichi Hoshino	| Trouble Action [ �Z�b�g�A�{�[�g���ɃX�e�[�v�����L�����Z������Ȃ� ]
//
