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
	UC mCondition;											// シートイニシャライズ管理状態を示す。
	UC mPlanBit[SHEETNO_NUM];								// 実施予定のシートイニシャライズ動作情報(Bit管理)を示す。
	UC mCompBit[SHEETNO_NUM];								// 実施完了済みのシートイニシャライズ動作情報(Bit管理)を示す。
	UC mNextShtInitTmgBit[SHT_EJECT_NUM][SHEETNO_NUM];		// 次用紙のシートイニシャライズを開始可能なタイミングになっているかどうかを示す。
															// 各Bitが各シートイニシャライズを示し、ビットがセットされているのは実施可能、セットされていないのは実施不可を示す。
	UC mLastShtReqShtNo;									// 直近のシートフィニッシュ要求のSheet No.を示す。
	UC mLastExecShtNo;										// 直近でシートイニシャライズ動作(シートイニシャライズ動作の種類問わず)を実行(開始)した用紙のSheet No.を示す。
	UC mLastExecOutDest;									// 直近でシートイニシャライズ動作(シートイニシャライズ動作の種類問わず)を実行(開始)した用紙の排紙先指定を示す。
	CShtSheetAttribute mLastExecShtAttr;					// 直近でシートイニシャライズ動作(シートイニシャライズ動作の種類問わず)を実行(開始)した用紙のシートフィニッシュ指示情報を示す。
	UC mCanceledShtNo;										// 直近でキャンセルされた用紙のSheet No.を示す。
															// キャンセル発生からキャンセル対象用紙以外の用紙のシート処理要求受信までの間保持する。
	CShtInitRequestWaitTime mWaitTime;						// シートイニシャライズ動作開始タイミングからの経過時間を示す。
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
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
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
// Description   : シートイニシャライズ動作管理情報のクリア
// Parameter     : sheetNo (Sheet No.)
// Return        : ‐
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
// Description   : データメンバのリセット
// Parameter     : ‐
// Return        : ‐
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
// Description   : 指定シートの実施計画への指定シートイニシャライズ
//               : 動作のセット処理
// Parameter     : shtInitID (Sheet Initailize ID)
//               : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SetSheetInitPlan(EShtInitID shtInitID, UC sheetNo)
{
	gSht_InitializeMember.mPlanBit[sheetNo] |= SHT_INIT_ID_CNV_BIT(shtInitID);
}

///////////////////////////////////////////////////////////////////
// Function Name : CheckSheetInitPlan
// Description   : 指定のシートとシートイニシャライズ動作の実施計画
//               : の確認処理
// Parameter     : shtInitID (Sheet Initailize ID)
//               : sheetNo (Sheet No.)
// Return        : true = 実施予定あり／false = 実施予定なし
///////////////////////////////////////////////////////////////////
bool CheckSheetInitPlan(EShtInitID shtInitID, UC sheetNo)
{
	 if ( gSht_InitializeMember.mPlanBit[sheetNo] & SHT_INIT_ID_CNV_BIT(shtInitID) ) return true;
	 else																			return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : GetSheetInitStartAdjustTime
// Description   : シートイニシャライズ動作時間取得処理。
// Parameter     : shtInitID (Sheet Initialize ID)
//               : sheetNo (シートイニシャライズ動作制御対象紙のSheet No.)
//               : lastShtNo (シートイニシャライズ動作制御対象紙の先行紙のSheet No.)
// Return        : シートイニシャライズ動作時間[×1ms]
///////////////////////////////////////////////////////////////////
US GetSheetInitializeTime(EShtInitID shtInitID, UC sheetNo, UC lastShtNo)
{
	US time=0;

	switch( shtInitID ){
	// !!! Product Specific !!!
	case SHT_INIT_ID_COMPILER:
	
		// SelectFin受信時のみ実行の対策 (V3.1.6)
		// セット間異なるジョブ時の対応(V3.1.7)
		/*if ( (sheetNo == SRVC_GetProcessStartSheet()) || (lastShtNo == SHEETNO_NUM)) {  
		else if( (SRVC_GetSetTop(sheetNo) == SHT_SET_TOP) &&				// "制御対象用紙がセット先頭用紙であり、以下のいずれかの条件を満たしている、
			 ((SRVC_GetCrossProcessSize(sheetNo) != SRVC_GetCrossProcessSize(lastShtNo)) ||	//    →制御対象用紙の用紙幅と先行紙の用紙幅が異なる
	 		  (SRVC_GetStapleMode(sheetNo) != SRVC_GetStapleMode(lastShtNo)) ||	//    →制御対象用紙のステープル指定と先行紙のステープル指定が異なる
	 		  (SRVC_GetCrossProcessSizeMixSet(lastShtNo) == SHT_SIZE_MIX)) ){		//    →先行紙のセットにおいてセット内異幅が発生している 	
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
// Description   : シートイニシャライズ動作時間補正値の取得処理。
// Parameter     : shtInitID (取得対象のSheet Initialize ID)
//               : sheetNo (シートイニシャライズ動作制御対象紙のSheet No.)
//               : lastShtNo (シートイニシャライズ動作制御対象紙の先行紙のSheet No.)
// Return        : シートイニシャライズ動作時間補正値[×1ms]
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
// Description   : 指定シートイニシャライズ動作の実施可否の確認処理
// Parameter     : shtInitID (確認対象のSheet Initialize ID)
// Return        : true = 実施可能／false = 実施不可
///////////////////////////////////////////////////////////////////
bool CheckSheetInitExecPossibility(EShtInitID shtInitID)
{
//	if ( shtInitID < SHT_INIT_ID_NUM ){											// 自モジュールで使用する関数なのでガード不要のためコメントアウト
		return COputInformer_GetOptionDetect((EOptionID)(cSht_InitializeManagerSpec.shtInitSpec[shtInitID].optionID));
//	}
//	else{
//		return false;
//	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CheckStartTiming
// Description   : 先行紙が制御対象シートのシートイニシャライズ動作
//               : が実施可能なポイントまで制御が済んでいるかどうか
//               : の確認処理。
// Parameter     : shtInitID (Sheet Initialize ID)
//               : sheetNo (制御対象シートのSheet No.)
//               : prevShtNo (制御対象シートの先行紙のSheet No.)
// Return        : true = 制御済み／false = 制御未完了
///////////////////////////////////////////////////////////////////
bool CheckStartTiming(EShtInitID shtInitID, UC sheetNo, UC prevShtNo)
{
	const CShtSheet *pPrevSht;

	if ( prevShtNo < SHEETNO_NUM ){												// 先行紙がある(管理対象である)
		pPrevSht = CShtManager_GetSheet(prevShtNo);

		if ( CShtSheet_CheckConditionAbnormalEnd(pPrevSht) == false ){			// 異常終了シートはチェック対象外(次プロセスで正常に引き継げるシートのみチェック)
			if ( cSht_InitializeManagerSpec.shtInitSpec[shtInitID].ejectDepend == true ){
																				// 先行紙がイジェクト対象かどうかに依存する
				if ( gSht_InitializeMember.mNextShtInitTmgBit[CShtSheet_GetEject(pPrevSht)][prevShtNo] & SHT_INIT_ID_CNV_BIT(shtInitID) ) return true;
				else																													  return false;
			}
			else{																// 先行紙がイジェクト対象かどうかに依存しない
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
	else{																		// 先行紙がない(処理が完了して管理対象から外れている場合も含む)
		return true;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CheckStartCondition
// Description   : 指定シートにおけるシートイニシャライズ動作の実施
//               : 可否の確認処理
// Parameter     : sheetNo (確認対象シートのSheet No.)
// Return        : true = 実施可能／false = 実施不可
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
// Description   : シートイニシャライズ動作開始処理
// Parameter     : shtInitID (Sheet Initialize ID)
//               : sheetNo (制御対象シートのSheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void StartSheetInitialize(EShtInitID shtInitID, UC sheetNo)
{
	const CShtSheet *pSheet;

//	if ( shtInitID < SHT_INIT_ID_NUM ){											// 自モジュールで使用する関数なのでガード不要のためコメントアウト
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
// Description   : 対象シートが、後続紙のシートイニシャライズ動作を
//               : 開始できるポイントまで制御が完了した際の処理
// Parameter     : shtInitID (制御対象シートイニシャライズ動作識別子)
//               : sheetNo (後続紙のシートイニシャライズ動作を開始で
//               : きるポイントまで制御が完了したシートのSheet No.)
//               : nextShtNo (後続紙のSheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void NextSheetInitializeStartTiming(EShtInitID shtInitID, UC sheetNo, EShtEject eject)
{
	const CShtSheet *pSheet, *pLastSheet;
	UC nextShtNo;

	pSheet = CShtManager_GetSheet(sheetNo);

	// --- 対象シートの後続紙のシートイニシャライズ動作開始可能ポイント制御完了情報更新 ---
	gSht_InitializeMember.mNextShtInitTmgBit[eject][sheetNo] |= SHT_INIT_ID_CNV_BIT(shtInitID);

	// --- 次用紙の取得 ---
	nextShtNo = CShtManager_GetNextRequestSheet(sheetNo);

	// イジェクトフィニッシング動作とプロセススタートの重なり対応(次用紙がシート管理キュー未登録)
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

	// --- 次用紙のシートイニシャライズ動作開始 ---
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

	// --- シートフィニッシュリクエスト待ち時間計測開始 ---
	CShtInitRequestWaitTime_StartWaiting(&(gSht_InitializeMember.mWaitTime), sheetNo);
}


///////////////////////////////////////////////////////////////////
// Function Name : CShtInitializeManager_CheckPlan
// Description   : 指定のシートとシートイニシャライズ動作における実
//               : 施計画の確認処理
// Parameter     : sheetNo (Sheet No.)
//               : shtInitID (Sheet Initailize ID)
// Return        : true = 実施予定あり／false = 実施予定なし
///////////////////////////////////////////////////////////////////
bool CShtInitializeManager_CheckPlan(UC sheetNo, EShtInitID shtInitID)
{
	return CheckSheetInitPlan(shtInitID, sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtInitializeManager_CheckComplete
// Description   : 指定のシートとシートイニシャライズ動作における実
//               : 施状況の確認処理
// Parameter     : sheetNo (Sheet No.)
//               : shtInitID (Sheet Initailize ID)
// Return        : true = 完了済み／false = 未完了
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
// Description   : シートイニシャライズ実施判断処理
// Parameter     : sheetNo (対象シートのSheet No.)
//               : prevShtNo (対象シートの先行紙のSheet No.)
// Return        : シートイニシャライズ動作時間情報
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
		// --- シートイニシャライズ動作開始済みシートの情報変化チェック ---
		if ( sheetNo == gSht_InitializeMember.mLastExecShtNo ){					// シートイニシャライズ開始済のシートである
			if ( (CShtSheet_GetOutputDestFinish(pSheet) != gSht_InitializeMember.mLastExecOutDest) ||
				 (CShtSheetAttribute_CheckDiffOutSheet(pShtAttr, &(gSht_InitializeMember.mLastExecShtAttr)) == true) ){
					 															// 情報に変更がある
				// --- 情報変化(キャンセル)発生時処理 ---
				gSht_InitializeMember.mCanceledShtNo = sheetNo;					// キャンセル対象シート識別子保持
				gSht_InitializeMember.mCondition	 = SHT_INIT_COND_ABNORMAL;	// 管理状態をAbnormalに変更
			}
			else{ /* No Action */ }
		}
		else{ /* No Action */ }
	}
	else{																		// Abnormal condition
		// --- ノーマルコンディションへの復帰可否チェック  ---
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
			// --- ノーマルコンディションへの復帰処理 ---
			gSht_InitializeMember.mLastShtReqShtNo = SHEETNO_NUM;
			CShtInitRequestWaitTime_Reset(&(gSht_InitializeMember.mWaitTime));
			gSht_InitializeMember.mCondition = SHT_INIT_COND_NORMAL;
		}
		else{ /* No Action */ }
	}

	if ( gSht_InitializeMember.mCondition == SHT_INIT_COND_NORMAL ){
		anyInitExec = false;
		// --- シートイニシャライズ動作情報クリア(最初のリクエスト受信時のみ) ---
		if ( sheetNo != gSht_InitializeMember.mLastShtReqShtNo ){
			ClearSheetInfo(sheetNo);
		}
		else{
			// No Action
		}

		// --- イニシャライズ動作時間取得＆イニシャライズ動作計画生成(毎リクエスト受信時に実行) ---
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
				SetSheetInitPlan(shtInitID, sheetNo);							// 何度問い合わせても同じ内容が応答される前提で動作実施中かどうかをケアせずに毎回書き込み実施

				// --- シートイニシャライズ動作開始補正時間加算 ---
				initTime += GetSheetInitStartAdjustTime(shtInitID, sheetNo, lastShtNo);

				anyInitExec = true;
			}
			else{ /* No Action */ }

			timeInfo.sheetInitTime[shtInitID] = initTime;
		}

		// --- リクエスト遅れ時間加算 ---
		if ( (anyInitExec == true) &&											// イニシャライズ動作実施
			 (prevShtNo < SHEETNO_NUM) ){										// 用紙管理上に先行紙あり
			waitTime = CShtInitRequestWaitTime_GetWaitTime(&(gSht_InitializeMember.mWaitTime), prevShtNo);

			if ( waitTime > 60000 ) timeInfo.waitTime = 60000;					// イニシャライズ動作時間を加算した際に60secを超える値になるよう60secにする。
			else					timeInfo.waitTime = (US)waitTime;
		}
		else{
			timeInfo.waitTime = 0;
		}

	
		// --- シートイニシャライズ動作完了チェック(Unknown、Considerable Time応答からの復帰) --- 
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
// Description   : シートイニシャライズ動作管理の開始処理
// Parameter     : ‐
// Return        : ‐
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
// Description   : シートイニシャライズ動作管理の終了処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtInitializeManager_End()
{
	gSht_InitializeMember.mLastShtReqShtNo = SHEETNO_NUM;
}


///////////////////////////////////////////////////////////////////
// Function Name : CShtInitializeManager_CycleDown
// Description   : シートイニシャライズ動作の穏当停止処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtInitializeManager_CycleDown()
{
	const CShtSheet *pSheet;
	UC shtInitID;

	// --- 用紙管理未登録用紙に対するシートイニシャライズ動作中止 ---
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
// Description   : シートイニシャライズ動作の緊急停止処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtInitializeManager_HardDown()
{
	UC shtInitID;

	// --- シートイニシャライズ動作中止 ---
	for ( shtInitID = 0; shtInitID < SHT_INIT_ID_NUM; shtInitID++ ){
		CSeqShtInitHandler_Stop(&(gSeq_ShtInitHandler[shtInitID]), SHEETNO_NUM);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtInitializeManager_SheetRequest
// Description   : シート処理依頼受信時の処理
// Parameter     : sheetNo (依頼対象シートのSheet No.)
//               : prevShtNo (依頼対象シートの先行紙のSheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtInitializeManager_SheetRequest(UC sheetNo, UC prevShtNo)
{
	UC shtInitID, checkResult[SHT_INIT_ID_NUM];

	shtInitID = false;

	if ( gSht_InitializeMember.mLastShtReqShtNo != sheetNo ){				// 前回のリクエストと同じシートの場合はシートイニシャライズ動作開始条件成立待ちなので無視する
		if ( (gSht_InitializeMember.mCondition == SHT_INIT_COND_NORMAL) &&	// ノーマルコンディションでない間はイニシャライズ動作実施しない
			 (CheckStartCondition(sheetNo) == true) && 						// シートコンディションのチェック(シートコンディションが復活することはないため前回リクエストシートチェックの内側に配置する)
			 (CDvstInformer_GetDeviceCondtion(DEVID_FINISHER) == DEV_COND_OK) &&
			 																// フィニッシャーコンディションのチェック(フォルト検知とプロセス開始の交錯対応)
			 (CCvrChecker_GetCoverStatus(CVR_ID_NUM) == CVR_STTS_CLOSE) ){	// カバー状態のチェック(フォルト検知とプロセス開始の交錯対応)
			for ( shtInitID = 0; shtInitID < SHT_INIT_ID_NUM; shtInitID++ ){
				if ( CheckSheetInitPlan(shtInitID, sheetNo) == true ){
					if ( (CheckStartTiming(shtInitID, sheetNo, prevShtNo) == true) &&
						 (CSeqShtInitHandler_GetState(&(gSeq_ShtInitHandler[shtInitID])) != SEQ_ACT_STT_RUNNING) ){
							 												// シートイニシャライズ動作未実施
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
// Description   : シート搬送管理開始時の処理
// Parameter     : sheetNo (管理開始シートのSheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtInitializeManager_SheetExecution(UC sheetNo)
{
	CShtInitRequestWaitTime_SetWaitSheet(&(gSht_InitializeMember.mWaitTime), sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtInitializeManager_SheetDelivered
// Description   : シート搬送管理完了(シートデリバード)時の処理
// Parameter     : sheetNo (管理完了シートのSheet No.)
// Return        : ‐
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
// Description   : シート処理依頼キャンセル時の処理
// Parameter     : sheetNo (キャンセル対象シートのSheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtInitializeManager_SheetCancel(UC sheetNo)
{
	if ( sheetNo == gSht_InitializeMember.mLastExecShtNo ){
		gSht_InitializeMember.mCanceledShtNo = sheetNo;
		gSht_InitializeMember.mCondition	 = SHT_INIT_COND_ABNORMAL;
	}
	else{ 
		//TB No.69. シートキャンセル後のリスケジューリング用紙に対するシートイニシャライズ動作を実施しない
		// --- リスケジューリング後のシート処理依頼を受付られるようにクリア実施 ---
		gSht_InitializeMember.mLastShtReqShtNo = SHEETNO_NUM;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtInitializeManager_SheetLocation
// Description   : Sheet Location通知受信時の処理。
// Parameter     : sheetNo (ロケーション更新シートのSheet No.)
//               : edge (シートエッジ識別子)
//               : location (Sheet Location ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtInitializeManager_SheetLocation(UC sheetNo, EShtSheetEdge edge, EShtSheetLocationID location)
{
	const CShtSheet *pSheet;
	UL parameter;
	UC  eject, shtInitID;

	pSheet    = CShtManager_GetSheet(sheetNo);
	eject     = CShtSheet_GetEject(pSheet);

	// --- ロケーション情報に応じたシートイニシャライズ動作実施 ---
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
// Description   : Sheet Event通知受信時の処理。
// Parameter     : sheetNo (Sheet Event更新シートのSheet No.)
//               : event (Sheet Event識別子)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtInitializeManager_SheetEvent(UC sheetNo, EShtSheetEventID event)
{
	const CShtSheet *pSheet;
	UL parameter;
	UC outDest, eject, shtInitID;

	pSheet    = CShtManager_GetSheet(sheetNo);
	eject     = CShtSheet_GetEject(pSheet);

	// --- イベントに応じたシートイニシャライズ動作実施 ---
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
// Description   : Sheet Action通知受信処理。
// Parameter     : sheetNo (Sheet No.)
//               : eventID (MSP Event ID)
//               : moduleID (Module ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtInitializeManager_SheetAction(UC sheetNo, US eventID, UC moduleID)
{
	UC shtInitID;

	for ( shtInitID = 0; shtInitID < SHT_INIT_ID_NUM; shtInitID++ ){
		if ( CSeqShtInitHandler_SheetActionResponse(&(gSeq_ShtInitHandler[shtInitID]), sheetNo, eventID, moduleID) == SEQ_ACT_STT_IDLE) {
			// --- シートイニシャライズ動作完了 ---
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
// Description   : シートイニシャライズ開始タイマータイムアップイベントコールバック
// Parameter     : timerID (Timer ID)
//               : parameter (Parameter：Bit0～7 = Next Sheet No./
//               : Bit8～15 = Previous Sheet No./Bit16～23 = Sheet 
//               : Initialize ID)
// Return        : ‐
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
// 15.03.30	| Yuichi Hoshino	| Design Improvement [ 異常停止時のイニシャライズトリガイベント通知無し対応 ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
// 15.10.06	| Yuichi Hoshino	| Trouble Action [ Cover Open状態でPrint指示実施時にDevice Failが発生 ]
// 16.01.07	| Yuichi Hoshino	| Trouble Action [ シートイニシャライズ動作開始タイミング不正 ]
// 16.03.04	| Yuichi Hoshino	| Trouble Action [ シートキャンセル後のリスケジューリング用紙に対するシートイニシャライズ動作を実施しない ]


