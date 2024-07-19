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
	SHT_COND_CHG_EVT_ENTRY,				// エントリー
	SHT_COND_CHG_EVT_NORMAL_EXIT,		// ノーマルイグジット
	SHT_COND_CHG_EVT_COMPLETE,			// コンプリート
	SHT_COND_CHG_EVT_DELIVERED,			// デリバード
	SHT_COND_CHG_EVT_EJECT_DETECT,		// イジェクトディテクト
	SHT_COND_CHG_EVT_ABNORMAL_EXIT,		// アブノーマルイグジット
	SHT_COND_CHG_EVT_PURGE,				// パージ
	SHT_COND_CHG_EVT_CANCEL,			// キャンセル
	SHT_COND_CHG_EVT_STOP,				// ストップ
	SHT_COND_CHG_EVT_JAM,				// ジャム
	SHT_COND_CHG_EVT_ABORT,				// アボート
	SHT_COND_CHG_EVT_NUM
} EShtSheetConditionEvent;


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Sheet ---
typedef struct /*CShtSheet*/
{
	CShtSheetAttribute mAttribute;		// IOTから通知されたAttribute情報を示す。※通知されたままの情報を保持する。
	// 4Byte Data
	UL mPlan;							// 基本搬送計画(バッファー制御なし搬送)を示す。(両エッジ（先端／後端）共通)
	UL mLocation[SHT_EDGE_NUM];			// シート先端(ON)/後端(OFF)の単純搬送履歴を示す。
	UL mEvent;							// 用紙イベントの履歴を示す。
	// 2Byte Data
	US mWeightedCompileCount;			// コンパイルトレイへの積載されている用紙束の、紙種やサイズにより重み付けされた枚数にて
	US mMaxMediaWeightOfCompile;		// 対象シートを含むコンパイルシート束内で最も重い坪量を示す。V3.4.0 Merge
	US mMinMediaWeightOfCompile;		// 対象シートを含むコンパイルシート束内で最も軽い坪量を示す。V3.4.0 Merge
	US mMaxProcSizeOfCompile;			// 対象シートを含むコンパイルシート束内で最も長い用紙送り長を示す。
	US mMaxCProcSizeOfCompile;			// 対象シートを含むコンパイルシート束内で最も長い用紙幅を示す。
	US mMinProcSizeOfCompile;			// 対象シートを含むコンパイルシート束内で最も短い用紙送り長を示す。
	US mMinCProcSizeOfCompile;			// 対象シートを含むコンパイルシート束内で最も短い用紙幅を示す。
	US mPrepTime;						// Preparation Time
//	US mBufferFeedWaitTime;				// Wait time of buffer feed	(RAM削減)
	// 1Byte Data(8Bit必要なもの or 取りうる値が変動するもの)
	UC mSetID;							// Set IDを示す。(IOTから通知されるセットの識別子)
	UC mSheetID;						// Sheet IDを示す。(IOTから通知されるシートの識別子)
	UC mOutputDestination;				// 排紙先を示す。
	UC mBinSelection;					// Bin Selectionを示す。
	UC mCondition;						// シートコンディションを示す。
//	UC mDeliveredLoc;					// delivered point location //V3.4.0 Merge
//	UC mSetFinishedLoc;					// set finished point location
//	UC mSetFinishedEvent;				// set finished point event
	UC mOutputDestFinish;				// IOTから通知された情報とその他の事象を考慮した最終的は排紙先を保持する。
	UC mSpeedID;						// フィニッシャー搬送速度を示す。
	UC mCompileCount;					// コンパイルトレイへの積載されている用紙束の積載枚数を示す。
										// コンパイルトレイがない排紙先に排出されるシートは常に1である。
	UC mDisperTime;						// Dispersion Time
	// 1Byte Data(Bit割り付け可能な(取りうる値が決定していて8Bit必要としない)もの)
	UC mOffsetFinish:3;					// 最終的な(用紙情報等を鑑みて判断した結果の)オフセットポジションを示す。
	UC mBuffer:2;						// バッファ制御対象紙情報を示す。
	UC mSetEndAction:2;					// Set End Actionを示す。 20160225 V3.4.0 Merge [ セットアボート時にステープルがキャンセルされない ]
	UC mEject:1;						// このシートの搬送完了をトリガーに排紙動作を実施するかどうかを示す。
	UC mSetTop:1;						// セット先頭紙かどうかを示す。
	UC mSetEnd:1;						// セット最終紙かどうかを示す。
	UC mDevidedSet:1;					// 対象シートが積載された際の積載枚数を示す。
										// コンパイルトレイがない排紙先に排出されるシートは常に1である。
										// 分割排紙実施セットに属するシートであるかを示す。
	UC mProcSizeMix:1;					// 対象シートを含むセットの用紙送り長がサイズミックスしているかどうかを示す。
										// ※後続のシートでサイズミックスが判明した際に、先行シートの情報は更新されないため
										// 注意が必要！(対象セットでサイズミックスになっているかどうかはチェックタイミング
										// における最後尾のシートの情報から判断すること)
										// なお、ディバイディッドイジェクトではサイズミックス情報はクリアされない。
	UC mCrossProcSizeMix:1;				// 対象シートを含むセットの用紙幅がサイズミックスしているかどうかを示す。
//	UC mInclusionFoldInCompile:1;		// 対象シートを含むコンパイルシート束内に折りシートが含まれているかを示す。
//	UC mInclusionFoldInSet:1;			// 対象シートを含むセット内に折りシートが含まれているかを示す。
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
//bool CShtSheet_CheckDelivereded(CShtSheet* pThis);							// (RAM削減)
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
#define CShtSheet_GetInclusionFoldInCompile(apThis) (SHT_FOLD_NOT_INCLUDE)		// EShtFoldInclusion (CShtSheet*) (RAM削減)
#define CShtSheet_SetInclusionFoldInCompile(apThis, aInclusion) (NULL)			// void (CShtSheet*, EShtFoldInclusion) (RAM削減)
#define CShtSheet_GetInclusionFoldInSet(apThis) (SHT_FOLD_NOT_INCLUDE)			// EShtFoldInclusion (CShtSheet*) (RAM削減)
#define CShtSheet_SetInclusionFoldInSet(apThis, aInclusion) (NULL)				// void (CShtSheet*, EShtFoldInclusion) (RAM削減)
																				// void (CShtSheet*, EShtFoldInclusion)
#define CShtSheet_GetPrepTime(apThis) ((apThis)->mPrepTime)						// US (CShtSheet*)
#define CShtSheet_SetPrepTime(apThis, aTime) ((apThis)->mPrepTime = aTime)		// void (CShtSheet*, US)
#define CShtSheet_GetBufferFeedWaitTime(apThis) (0)								// US (CShtSheet*) (RAM削減)
#define CShtSheet_SetBufferFeedWaitTime(apThis, aTime) (NULL)					// void (CShtSheet*, US) (RAM削減)
																				// void (CShtSheet*, US)
#define CShtSheet_GetDispersionTime(apThis) (0)									// UC (CShtSheet*) (RAM削減)
#define CShtSheet_SetDispersionTime(apThis, aTime) (NULL)						// void (CShtSheet*, UC) (RAM削減)
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
// 15.03.30	| Yuichi Hoshino	| Design Improvement [ 異常停止時のイニシャライズトリガイベント通知無し対応(CShtSheet_CheckConditionAbnormalEnd()追加) ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
// 15.09.01	| Yuichi Hoshino	| Trouble Action [ セットアボート時にステープルがキャンセルされない ]
//
