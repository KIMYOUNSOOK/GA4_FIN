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
// Description   : コンストラクタ
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtSheet_Constructor(CShtSheet* pThis)
{
	CShtSheet_Reset(pThis);
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_Reset
// Description   : リセット処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
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
// V3.4.0 	RAM削減
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
// V3.4.0 T/B対策　Merge No.41
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
// Description   : コンディション遷移指示受信処理
// Parameter     : pThis (インスタンスポインタ)
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
// Description   : 現在のシートコンディションが、ノーマルなコンディ
//               : ションとしてカテゴライズされた分類に属しているか
//               : の確認処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : true = 本カテゴリに適合するコンディションである
//               : false = 本カテゴリに適合するコンディションでない
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
// Description   : 現在のシートコンディションが、フィニッシングプロ
//               : セスの処理対象コンディションとしてカテゴライズさ
//               : れた分類に属しているかの確認処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : true = 本カテゴリに適合するコンディションである
//               : false = 本カテゴリに適合するコンディションでない
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
// Description   : 現在のシートコンディションが、搬送対象(デリバー
//               : ド前)のコンディションとしてカテゴライズされた分
//               : 類に属しているかの確認処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : true = 本カテゴリに適合するコンディションである
//               : false = 本カテゴリに適合するコンディションでない
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
// Description   : 現在のシートコンディションが、ノーマルな搬送対象
//               : (デリバード前)のコンディションとしてカテゴライズ
//               : された分類に属しているかの確認処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : true = 本カテゴリに適合するコンディションである
//               : false = 本カテゴリに適合するコンディションでない
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
// Description   : 現在のシートコンディションが、アブノーマルな搬送
//               : (パージ搬送)対象(デリバード前)のコンディションと
//               : してカテゴライズされた分類に属しているかの確認処
//               : 理
// Parameter     : pThis (インスタンスポインタ)
// Return        : true = 本カテゴリに適合するコンディションである
//               : false = 本カテゴリに適合するコンディションでない
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
// Description   : 現在のシートコンディションが、イジェクトを待ちの
//               : コンディションとしてカテゴライズされた分類に属し
//               : ているかの確認処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : true = 本カテゴリに適合するコンディションである
//               : false = 本カテゴリに適合するコンディションでない
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
// Description   : 現在のシートコンディションが、セットフィニッシュ
//               : 待ちのコンディションとしてカテゴライズされた分類
//               : に属しているかの確認処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : true = 本カテゴリに適合するコンディションである
//               : false = 本カテゴリに適合するコンディションでない
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
// Description   : 現在のシートコンディションが、Set Finish Receive 確認処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : true = 本カテゴリに適合するコンディションである
//               : false = 本カテゴリに適合するコンディションでない
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
// Description   : 現在のシートコンディションが、異常終了のコンディ
//               : ションとしてカテゴライズされた分類に属しているか
//               : の確認処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : true = 本カテゴリに適合するコンディションである
//               : false = 本カテゴリに適合するコンディションでない
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
// Description   : 現在のシートコンディションが、制御を停止すべきコ
//               : ンディションとしてカテゴライズされた分類に属して
//               : いるかの確認処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : true = 本カテゴリに適合するコンディションである
//               : false = 本カテゴリに適合するコンディションでない
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
// Description   : シートが指定のロケーションを通過する予定があるか
//               : どうかの確認処理
// Parameter     : pThis (インスタンスポインタ)
//               : locationID (Sheet Location ID)
// Return        : true = 通過予定あり
//               : false = 通過予定なし
///////////////////////////////////////////////////////////////////
bool CShtSheet_CheckLocationPlan(CShtSheet* pThis, EShtSheetLocationID locationID)
{
	UL checkBit = SHT_LOC_CNV_BIT(locationID);

	if ( pThis->mPlan & checkBit ) return true;
	else						   return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_GetNextLocationPlan
// Description   : シートが指定のロケーションの次に到達予定のロケー
//               : ションを取得する処理
// Parameter     : pThis (インスタンスポインタ)
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
// Description   : シートが指定のロケーションの前に到達予定のロケー
//               : ションを取得する処理
// Parameter     : pThis (インスタンスポインタ)
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
// Description   : ロケーション発生実績(履歴)をセットする処理
// Parameter     : pThis (インスタンスポインタ)
//               : locationID (Sheet Location ID)
//               : edge (シートエッジ識別子)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtSheet_SetLocation(CShtSheet* pThis, EShtSheetLocationID locationID, EShtSheetEdge edge)
{
	pThis->mLocation[edge] |= SHT_LOC_CNV_BIT(locationID);
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_CheckLocationArrival
// Description   : ロケーションの発生状況を確認する処理
// Parameter     : pThis (インスタンスポインタ)
//               : locationID (Sheet Location ID)
//               : edge (シートエッジ識別子)
// Return        : true = 発生済み
//               : false = 未発生
///////////////////////////////////////////////////////////////////
bool CShtSheet_CheckLocationArrival(CShtSheet* pThis, EShtSheetLocationID locationID, EShtSheetEdge edge)
{
	if ( pThis->mLocation[edge] & SHT_LOC_CNV_BIT(locationID) ) return true;
	else														return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_GetNextLocation
// Description   : シートが次に到達予定のロケーションを取得する処理
// Parameter     : pThis (インスタンスポインタ)
//               : edge (シートエッジ識別子)
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
// Description   : シートが直近で到達したロケーションを取得する処理
// Parameter     : pThis (インスタンスポインタ)
//               : edge (シートエッジ識別子)
// Return        : 0 ～ (SHT_LOC_ID_NUM - 1) = 直近で到達したロケー
//               : ションのSheet Location ID
//               : SHT_LOC_ID_NUM = 該当ロケーションなし
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
// Description   : デリバード済みかどうかの確認処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : true = デリバード済み
//               : false = 未デリバード
///////////////////////////////////////////////////////////////////V3.4.0 Merge
//bool CShtSheet_CheckDelivereded(CShtSheet* pThis)
//{
//	if ( pThis->mDeliveredLoc != SHT_LOC_ID_NUM ) return CShtSheet_CheckLocationArrival(pThis, pThis->mDeliveredLoc, SHT_EDGE_TAIL);
//	else										  return false;
//}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_CheckEventOccurence
// Description   : イベントの発生状況を確認する処理
// Parameter     : pThis (インスタンスポインタ)
//               : eventID (Sheet Event ID)
// Return        : true = 発生済み
//               : false = 未発生
///////////////////////////////////////////////////////////////////
bool CShtSheet_CheckEventOccurence(CShtSheet* pThis, EShtSheetEventID eventID)
{
	if ( pThis->mEvent & SHT_EVT_ID_CNV_BIT(eventID) ) return true;
	else											   return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_GetSizeClass
// Description   : サイズクラス取得処理
// Parameter     : pThis (インスタンスポインタ)
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
// Description   : 用紙送り長方向のサイズクラス取得処理
// Parameter     : pThis (インスタンスポインタ)
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
// Description   : 用紙幅方向のサイズクラス取得処理
// Parameter     : pThis (インスタンスポインタ)
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
// Description   : サイズミックス情報取得処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : Size Mix
///////////////////////////////////////////////////////////////////
EShtSizeMix CShtSheet_GetSizeMix(CShtSheet* pThis)
{
	if ( (pThis->mProcSizeMix == SHT_SIZE_MIX) || (pThis->mCrossProcSizeMix == SHT_SIZE_MIX) ) return SHT_SIZE_MIX;
	else																					   return SHT_SIZE_UNMIX;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtSheet_GetSheetIntegrity
// Description   : シートインテグリティ取得処理
// Parameter     : pThis (インスタンスポインタ)
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
// Description   : セットインテグリティ取得処理
// Parameter     : pThis (インスタンスポインタ)
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
// 15.03.30	| Yuichi Hoshino	| Design Improvement [ 異常停止時のイニシャライズトリガイベント通知無し対応(CShtSheet_CheckConditionAbnormalEnd()追加) ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
