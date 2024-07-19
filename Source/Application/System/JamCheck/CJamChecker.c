// File Name:	CJamChecker.c
// Description:	Implimentation File of Jam Checker
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CJamChecker.h"

#include "DD_DIO.h"

#include "CSysManager.h"
#include "CShtManager.h"
#include "CCvrChecker.h"
#include "CSeqDevInitHandler.h"
#include "COputInformer.h"
#include "CCfgInformer.h"
#include "CDvstInformer.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define JAM_CYCLIC_CHEAK_INTERVAL	100											// ジャムゾーンクリアチェック周期[×1ms]

#define JAM_DJAM_ID_CNV_BIT(jamID)	(0x00000001 << jamID)

// --- Device Status No. Table of Dynamic Jam ---
extern US const cJam_DJamDevSttsNo[JAM_DJAM_ID_NUM];

// --- Jam Zone Check Config Table ---
extern UC const cJam_ZoneOptionID[JAM_ZONE_ID_NUM];

// --- Input ID table of path sensor ---
extern UC const cJam_PathSensorInputID[SHT_PATH_SNR_ID_NUM];

// --- Spec data table of set Jam Zone from path sensor condition ---
extern JamZoneSetSpecPathSensor const cJam_ZoneSetSpecSensor[];

// --- Spec data table of set Jam Zone from sheet location ---
extern JamZoneSetSpecSheetLocation const cJam_ZoneSetSpecLocation[];

// --- Spec data table of clear Jam Zone from cover close ---
extern JamZoneClearSpecCover const cJam_ZoneClearSpecCover[];

// --- Spec data table of clear Jam Zone from entrance path enviroment change ---
extern JamZoneClearSpecEntrance const cJam_ZoneClearSpecEntrance[];

// --- Spec data table of clear Jam Zone from cyclic check ---
extern JamZoneClearSpecCyclic const cJam_ZoneClearSpecCyclic[];

// --- Dynamic Jam select data table from Sheet Location ---
extern DJamSelectSpec const *cpJam_DynamicJamSelectTableLocation[SHT_LOC_ID_NUM];

// --- Dynamic Jam select data table from Sheet Event ----
extern DJamSelectSpec const *cpJam_DynamicJamSelectTableEvent[SHT_EVT_ID_NUM];

// --- Dynamic Jam detect spec data table ---
extern DJamDetectSpec const cJam_DJamDetectSpec[JAM_DJAM_ID_NUM];

// --- Dynamic Jam Timer value[×1ms] get funtion pointer table ---
extern UL const (*cpJam_DynamicJamTimerGetTable[JAM_DJAM_ID_NUM])(UC sheetNo);

// --- Member of Jam Checker ---
typedef struct /*CJamCheckerMember*/
{
	UC mJamZoneClearCount[JAM_ZONE_ID_NUM];
	US mCoverSttsBit;
	UL mDynamicJamSttsBit;
	UL mJamZoneBit;
	US mDiscardOptionBit;
} CJamCheckerMember;

static CJamCheckerMember gJam_CheckerMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define SetJamStatus(aJamID)	(gJam_CheckerMember.mDynamicJamSttsBit |= (0x00000001 << (aJamID)))
#define ClearJamStatus(aJamID)	(gJam_CheckerMember.mDynamicJamSttsBit &= ~(0x00000001 << (aJamID)))
#define SetJamZone(aZoneID)		(gJam_CheckerMember.mJamZoneBit |= (0x00000001 << (aZoneID)))
#define ClearJamZone(aZoneID)	(gJam_CheckerMember.mJamZoneBit &= ~(0x00000001 << (aZoneID)))

static bool CheckJamZoneCheckConditionOption(EOptionID optionID);
static bool CheckJamZoneSetConditionSensor();
static bool CheckJamZoneSetConditionLocation();
static bool CheckJamZoneClearConditionCommon();
static bool CheckJamZoneClearConditionCover(ECvrCoverID coverID);
static bool CheckJamZoneClearConditionEntrance();
static void JudgeStartJamZoneClearCheckCycle();
static bool CheckJamZoneClearConditionCycle();
static US CalculateJamTimer(UC sheetNo, UC jamID);
static void StartJamTimer(UC sheetNo, UC jamID, US jamTimer);
static bool JudgeDynamicJam(UC sheetNo, UC jamID);


/////////////////////////////////////////////////////////////////////////////////////////
// Timer Event Call Back Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void JAM_ClearCyclicCheck(SS timerID, UL notUsed);
void JAM_JamTimerTimeUp(SS timerID, UL parameter);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CJamChecker_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CJamChecker_Constructor()
{
	UC index;

	gJam_CheckerMember.mCoverSttsBit	  = 0x0000;
	gJam_CheckerMember.mDynamicJamSttsBit = 0x00000000;
	gJam_CheckerMember.mJamZoneBit		  = 0x00000000;
	gJam_CheckerMember.mDiscardOptionBit  = 0x0000;

	for ( index = 0; index < JAM_ZONE_ID_NUM; index++ ){
		gJam_CheckerMember.mJamZoneClearCount[index] = 0;
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : CheckJamZoneCheckConditionOption
// Description   : ジャム検知におけるオプション装置条件の確認処理
// Parameter     : optionID　(Option ID)
// Return        : true = ジャム検知可
//               : false = ジャム検知付加
///////////////////////////////////////////////////////////////////
bool CheckJamZoneCheckConditionOption(EOptionID optionID)
{
	if ( (COputInformer_GetOptionDetect(optionID) == true) &&					// オプション接続あり
		 ( (gJam_CheckerMember.mDiscardOptionBit == 0) ||						// 処理から切り離されたオプションがない
		   ((gJam_CheckerMember.mDiscardOptionBit & (0x0001 << optionID)) == 0) ) ){
		return true;															// 指定オプションが処理から切り離されていない
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CheckJamZoneSetConditionSensor
// Description   : 搬送パスセンサの状態に応じたジャムゾーンセット処
//               : 理
// Parameter     : ‐
// Return        : true = ジャムゾーンステータスに変化あり
//               : false = ジャムゾーンステータスに変化なし
///////////////////////////////////////////////////////////////////
bool CheckJamZoneSetConditionSensor()
{
	UC sttsChg, index, pathID;

	sttsChg = false;

	if ( CCvrChecker_GetCoverStatus(CVR_ID_NUM) == CVR_STTS_CLOSE ){			// All cover close.
		for( index = 0; cJam_ZoneSetSpecSensor[index].zoneID != JAM_ZONE_ID_NUM; index++ ){
			if ( CheckJamZoneCheckConditionOption(cJam_ZoneOptionID[cJam_ZoneSetSpecSensor[index].zoneID]) == true ){
				if ( CJamChecker_GetJamZoneStatus(cJam_ZoneSetSpecSensor[index].zoneID) == JAM_ZONE_STTS_CLEAR ){
					for ( pathID = 0; pathID < SHT_PATH_SNR_ID_NUM; pathID++ ){
						if ( cJam_ZoneSetSpecSensor[index].pathSensorBit & JAM_PATH_SNR_ID_CNV_BIT(pathID) ){
							if ( (DD_DIO_GetInput(cJam_PathSensorInputID[pathID]) == ON) &&
								 ( ((cJam_ZoneSetSpecSensor[index].compileCheckPathBit & JAM_PATH_SNR_ID_CNV_BIT(pathID)) == 0) ||
								   (CShtManager_CheckUnejectedSheetExist(cJam_ZoneSetSpecSensor[index].compileCheckOutDest) == false) ) ){
								SetJamZone(cJam_ZoneSetSpecSensor[index].zoneID);
								sttsChg = true;

								break;
							}
							else{ /* No Action */ }
						}
						else{ /* Go for loop */ }
					}
				}
				else{ /* No Action */ }
			}
			else{ /* No Action */ }
		}
	}
	else{ /* No Action */ }														// Any cover open.

	return sttsChg;
}

///////////////////////////////////////////////////////////////////
// Function Name : CheckJamZoneSetConditionLocation
// Description   : フィニッシングプロセス完了時の機内残留紙位置に応
//               : じたジャムゾーンセット処理
// Parameter     : ‐
// Return        : true = ジャムゾーンステータスに変化あり
//               : false = ジャムゾーンステータスに変化なし
///////////////////////////////////////////////////////////////////
bool CheckJamZoneSetConditionLocation()
{
	const CShtSheet *pSheet;
	UC sttsChg, checkShtNo, shtCond, nextLoc, index;

	sttsChg = false;

	checkShtNo = CShtManager_GetTopExitSheet();

	while( checkShtNo != SHEETNO_NUM ){
		pSheet	= CShtManager_GetSheet(checkShtNo);
		shtCond = CShtSheet_GetCondition(pSheet);

		if ( (shtCond == SHT_COND_REMAIN) || (shtCond == SHT_COND_JAM) ){
			nextLoc = CShtSheet_GetNextLocation(pSheet, SHT_EDGE_LEAD);

			for ( index = 0; cJam_ZoneSetSpecLocation[index].zoneID != JAM_ZONE_ID_NUM; index++ ){
				if ( cJam_ZoneSetSpecLocation[index].locationID == nextLoc ){
					if ( CheckJamZoneCheckConditionOption(cJam_ZoneOptionID[cJam_ZoneSetSpecLocation[index].zoneID]) == true ){
						if ( CJamChecker_GetJamZoneStatus(cJam_ZoneSetSpecLocation[index].zoneID) == JAM_ZONE_STTS_CLEAR ){
							SetJamZone(cJam_ZoneSetSpecLocation[index].zoneID);
							sttsChg = true;
						}
						else{ /* No Action */ }
					}
					else{ /* No Action */ }
				}
				else{ /* No Action */ }
			}
		}
		else{ /* No Action */ }

		checkShtNo = CShtManager_GetNextExitSheet(checkShtNo);
	}

	return sttsChg;
}

///////////////////////////////////////////////////////////////////
// Function Name : CheckJamZoneClearConditionCommon
// Description   : ジャムゾーンクリア共通条件確認処理
// Parameter     : ‐
// Return        : true = ジャムゾーンクリア可能
//               : false = ジャムゾーンlクリア不可
///////////////////////////////////////////////////////////////////
bool CheckJamZoneClearConditionCommon()
{
	EFinisherState finState;

	finState = CSysManager_GetFinisherState();
 
	if ( (finState != FIN_STT_CYCLE_UP ) &&										// Finisher State
		 (finState != FIN_STT_READY ) &&
		 (finState != FIN_STT_CYCLE_DOWN) &&
		 (finState != FIN_STT_HARD_DOWN) &&
		 (CSeqDevInitHandler_GetState() != SEQ_ACT_STT_RUNNING) ){				// Not running initialize action sequence.
		return true;
	}
	else{ /* NG */ }

	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CheckJamZoneClearConditionCover
// Description   : カバークローズ時チェックによるジャムゾーンクリア
//               : 処理
// Parameter     : coverID (Cover ID)
// Return        : true = ジャムゾーンステータスに変化あり
//               : false = ジャムゾーンステータスに変化なし
///////////////////////////////////////////////////////////////////
bool CheckJamZoneClearConditionCover(ECvrCoverID coverID)
{
	UC sttsChg, index, aSpecNum, pathBit, pathID;

	sttsChg = false;

	if ( coverID < CVR_ID_NUM ){
		for ( index = 0; cJam_ZoneClearSpecCover[index].zoneID != JAM_ZONE_ID_NUM; index++ ){
			if ( cJam_ZoneClearSpecCover[index].coverIdBit & JAM_CVR_ID_CNV_BIT(coverID) ){
				if ( CJamChecker_GetJamZoneStatus(cJam_ZoneClearSpecCover[index].zoneID) == JAM_ZONE_STTS_SET ){
					if ( CheckJamZoneCheckConditionOption(cJam_ZoneOptionID[cJam_ZoneClearSpecCover[index].zoneID]) == true ){
						pathBit = cJam_ZoneClearSpecCover[index].pathSensorBit;

						for ( pathID = 0; pathID < SHT_PATH_SNR_ID_NUM; pathID++ ){
							if ( pathBit != 0 ){
								if ( pathBit & 1 ){								// チェック対象パスセンサ
									if ( (DD_DIO_GetInput(cJam_PathSensorInputID[pathID]) == OFF) ||
										 ( (cJam_ZoneClearSpecCover[index].compileCheckPathBit & JAM_PATH_SNR_ID_CNV_BIT(pathID)) &&
										   (CShtManager_CheckUnejectedSheetExist(cJam_ZoneClearSpecCover[index].compileCheckOutDest) == true ) ) ){
										// No Action
									}
									else{
										break;
									}
								}
								else{ /* No Action */ }
							}
							else{												// For shortning process.
								break;
							}

							pathBit >>= 1;
						}

						if ( pathBit == 0 ){
							ClearJamZone(cJam_ZoneClearSpecCover[index].zoneID);
							gJam_CheckerMember.mJamZoneClearCount[cJam_ZoneClearSpecCover[index].zoneID] = 0;
							sttsChg = true;
						}
						else{ /* Not Clear */ }
					}
					else{ /* No Action */ }										// Cover module don't exist(Irregular case).
				}
				else{ /* No Action */ }											// Don't set Jam Zone.
			}
			else{ /* No Action */ }
		}
	}
	else{ /* No Action */ }

	return sttsChg;
}

///////////////////////////////////////////////////////////////////
// Function Name : CheckJamZoneClearConditionEntrance
// Description   : エントランス部ユーザー介入(IOT側カバークローズ)
//               : 時チェックによるジャムゾーンクリア処理
// Parameter     : ‐
// Return        : true = ジャムゾーンステータスに変化あり
//               : false = ジャムゾーンステータスに変化なし
///////////////////////////////////////////////////////////////////
bool CheckJamZoneClearConditionEntrance()
{
	UL pathBit;
	UC sttsChg, index, pathID;

	sttsChg = false;

	for ( index = 0; cJam_ZoneClearSpecEntrance[index].zoneID != JAM_ZONE_ID_NUM; index++ ){
		if ( CJamChecker_GetJamZoneStatus(cJam_ZoneClearSpecEntrance[index].zoneID) == JAM_ZONE_STTS_SET ){
			if ( CheckJamZoneCheckConditionOption(cJam_ZoneOptionID[cJam_ZoneClearSpecEntrance[index].zoneID]) == true ){
				pathBit = cJam_ZoneClearSpecEntrance[index].pathSensorBit;

				for ( pathID = 0; pathID < SHT_PATH_SNR_ID_NUM; pathID++ ){
					if ( pathBit != 0 ){
						if ( pathBit & 1 ){										// チェック対象パスセンサ
							if ( DD_DIO_GetInput(cJam_PathSensorInputID[pathID]) == ON ){
								break;
							}
							else{ /* No Action */ }
						}
						else{ /* No Action */ }
					}
					else{														// For shortning process.
						break;
					}

					pathBit >>= 1;
				}

				if ( pathBit == 0 ){
					ClearJamZone(cJam_ZoneClearSpecEntrance[index].zoneID);
					gJam_CheckerMember.mJamZoneClearCount[cJam_ZoneClearSpecEntrance[index].zoneID] = 0;
					sttsChg = true;
				}
				else{ /* Not Clear */ }
			}
			else{ /* No Action */ }
		}
		else{ /* No Action */ }													// Don't set Jam Zone.
	}

	return sttsChg;
}

///////////////////////////////////////////////////////////////////
// Function Name : JudgeStartJamZoneClearCheckCycle
// Description   : 周期チェックによるジャムゾーンクリア実施条件チェ
//               : ック処理(センサを踏んでいるかチェック)
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void JudgeStartJamZoneClearCheckCycle()
{
	UL pathBit;
	UC index, pathID;

	for ( index = 0; cJam_ZoneClearSpecCyclic[index].zoneID != JAM_ZONE_ID_NUM; index++ ){
		if ( CJamChecker_GetJamZoneStatus(cJam_ZoneClearSpecCyclic[index].zoneID) == JAM_ZONE_STTS_SET ){
			if ( CheckJamZoneCheckConditionOption(cJam_ZoneOptionID[cJam_ZoneClearSpecCyclic[index].zoneID]) == true ){
				pathBit = cJam_ZoneClearSpecCyclic[index].pathSensorBit;

				for ( pathID = 0; pathID < SHT_PATH_SNR_ID_NUM; pathID++ ){
					if ( pathBit != 0 ){
						if ( pathBit & 1 ){										// チェック対象パスセンサ
							if ( DD_DIO_GetInput(cJam_PathSensorInputID[pathID]) == ON ){
								break;	
							}
							else{ /* Go for loop. */ }
						}
						else{ /* No Action */ }
					}
					else{														// For shortning process.
						break;
					}

					pathBit >>= 1;
				}

				if ( pathBit == 0 ){											// All sensor is OFF.
					gJam_CheckerMember.mJamZoneClearCount[cJam_ZoneClearSpecCyclic[index].zoneID] = 0xFF;
				}
				else{ /* No Action */ }
			}
			else{ /* No Action */ }
		}
		else{ /* No Action */ }													// Don't set Jam Zone.
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CheckJamZoneClearConditionCycle
// Description   : 周期チェックによるジャムゾーンクリア処理
// Parameter     : ‐
// Return        : true = ジャムゾーンステータスに変化あり
//               : false = ジャムゾーンステータスに変化なし
///////////////////////////////////////////////////////////////////
bool CheckJamZoneClearConditionCycle()
{
	UC sttsChg;
	UL pathBit;
	UC index, pathID;

	sttsChg = false;

	for ( index = 0; cJam_ZoneClearSpecCyclic[index].zoneID != JAM_ZONE_ID_NUM; index++ ){
		if ( CJamChecker_GetJamZoneStatus(cJam_ZoneClearSpecCyclic[index].zoneID) == JAM_ZONE_STTS_SET ){
			if ( CheckJamZoneCheckConditionOption(cJam_ZoneOptionID[cJam_ZoneClearSpecCyclic[index].zoneID]) == true ){
				if ( gJam_CheckerMember.mJamZoneClearCount[cJam_ZoneClearSpecCyclic[index].zoneID] != 0xFF ){
					pathBit = cJam_ZoneClearSpecCyclic[index].pathSensorBit;

					for ( pathID = 0; pathID < SHT_PATH_SNR_ID_NUM; pathID++ ){
						if ( pathBit != 0 ){
							if ( pathBit & 1 ){									// チェック対象パスセンサ
								if ( (DD_DIO_GetInput(cJam_PathSensorInputID[pathID]) == OFF) ||
									 ( (cJam_ZoneClearSpecCyclic[index].compileCheckPathBit & JAM_PATH_SNR_ID_CNV_BIT(pathID)) &&
									   (CShtManager_CheckUnejectedSheetExist(cJam_ZoneClearSpecCyclic[index].compileCheckOutDest) == true ) ) ){
									// No Action
								}
								else{
									break;
								}
							}
							else{ /* No Action */ }
						}
						else{													// For shortning process.
							break;
						}

						pathBit >>= 1;
					}

					if ( pathBit == 0 ){
						gJam_CheckerMember.mJamZoneClearCount[cJam_ZoneClearSpecCyclic[index].zoneID]++;

						if ( gJam_CheckerMember.mJamZoneClearCount[cJam_ZoneClearSpecCyclic[index].zoneID] >= cJam_ZoneClearSpecCyclic[index].clearCount ){
							ClearJamZone(cJam_ZoneClearSpecCyclic[index].zoneID);
							gJam_CheckerMember.mJamZoneClearCount[cJam_ZoneClearSpecCyclic[index].zoneID] = 0;
							sttsChg = true;
						}
						else{ /* No Action */ }
					}
					else{														// Not clear condition
						gJam_CheckerMember.mJamZoneClearCount[cJam_ZoneClearSpecCyclic[index].zoneID] = 0;
					}															// Reset Counter
				}
				else{ /* No Action */ }
			}
			else{ /* No Action */ }
		}
		else{ /* No Action */ }													// Don't set Jam Zone.
	}

	return sttsChg;
}

///////////////////////////////////////////////////////////////////
// Function Name : CalculateJamTimer
// Description   : ジャムタイマー算出処理
// Parameter     : sheetNo (Sheet No.)
//               : jamID (Jam ID)
// Return        : Jam Timer[×1ms]
///////////////////////////////////////////////////////////////////
US CalculateJamTimer(UC sheetNo, UC jamID)
{
	CCfgDevContConfiguration* pConfigInfo;
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pShtAttr;
	US timer, paramSD;

	timer = 0;

	if ( (jamID < JAM_DJAM_ID_NUM) && (cpJam_DynamicJamTimerGetTable[jamID] != NULL) ){
		timer = (US)((cpJam_DynamicJamTimerGetTable[jamID])(sheetNo));
	}
	else{ /* No Action */ }

	return timer;
}

///////////////////////////////////////////////////////////////////
// Function Name : StartJamTimer
// Description   : ジャムタイマー監視開始
// Parameter     : sheetNo (Sheet No.)
//               : jamID (Jam ID)
//               : jamTimer (Jam Timer[×1ms])
// Return        : ‐
///////////////////////////////////////////////////////////////////
void StartJamTimer(UC sheetNo, UC jamID, US jamTimer)
{
	UL parameter;

	parameter = (((US)jamID << 8)|sheetNo);

	MSP_SetEventTimer(jamTimer, JAM_JamTimerTimeUp, parameter);
}

///////////////////////////////////////////////////////////////////
// Function Name : JudgeDynamicJam
// Description   : タイマータイムアップ時のジャム発生判断処理
// Parameter     : sheetNo (Sheet No.)
//               : jamID (Jam ID)
// Return        : true = ジャムである／false = ジャムでない
///////////////////////////////////////////////////////////////////
bool JudgeDynamicJam(UC sheetNo, UC jamID)
{
	CCfgDevContConfiguration *pConfig;
	const CShtSheet *pSheet;
	UC nextSht;

	pConfig = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);

	if ( CCfgDevContConfiguration_GetJamBypass(pConfig) == 0 ){					// Not execute jam bypass
		pSheet = CShtManager_GetSheet(sheetNo);

		if ( CShtSheet_CheckConditionDelivery(pSheet) == true ){
			if ( CShtSheet_CheckLocationArrival(pSheet, cJam_DJamDetectSpec[jamID].locationID, cJam_DJamDetectSpec[jamID].edge) == false ){
				return true;
			}
			else{ /* false */ }
		}
		else{ /* false */ }
	}
	else{ /* false */ }															// Execute Jam Bypass

	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CJamChecker_GetDynamicJamStatus
// Description   : ダイナミックジャムステータス取得処理
// Parameter     : jamID (Dynamic Jam ID)
// Return        : Fault Status
///////////////////////////////////////////////////////////////////
EFaultStatus CJamChecker_GetDynamicJamStatus(EJamDynamicJamID jamID)
{
	if ( jamID < JAM_DJAM_ID_NUM ){
		if ( gJam_CheckerMember.mDynamicJamSttsBit & JAM_DJAM_ID_CNV_BIT(jamID) ) return FAULT_OCCUR;
		else																	  return FAULT_NOT_OCCUR;
	}
	else{
		if ( gJam_CheckerMember.mDynamicJamSttsBit == 0 ) return FAULT_NOT_OCCUR;
		else											  return FAULT_OCCUR;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CJamChecker_GetJamZoneStatus
// Description   : ジャムゾーンステータス取得処理
// Parameter     : zoneID (Jam Zone ID)
// Return        : Jam Zone Status
///////////////////////////////////////////////////////////////////
EJamZoneStatus CJamChecker_GetJamZoneStatus(EJamZoneID zoneID)
{
	if ( zoneID < JAM_ZONE_ID_NUM ){
		if ( gJam_CheckerMember.mJamZoneBit & JAM_ZONE_ID_CNV_BIT(zoneID) ) return JAM_ZONE_STTS_SET;
		else																return JAM_ZONE_STTS_CLEAR;
	}
	else{
		if ( gJam_CheckerMember.mJamZoneBit == 0 ) return JAM_ZONE_STTS_CLEAR;
		else									   return JAM_ZONE_STTS_SET;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CJamChecker_EndCheck
// Description   : ジャム検知処理の終了指示受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CJamChecker_EndCheck()
{
	UC optionID;

	for ( optionID = 0; optionID < OPTID_NUM; optionID++ ){
		gJam_CheckerMember.mDiscardOptionBit |= (0x0001 << optionID);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CJamChecker_DiscardOption
// Description   : 指定オプションのジャム検知処理の停止指示受信時処
//               : 理
// Parameter     : optionID (Option ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CJamChecker_DiscardOption(EOptionID optionID)
{
	UC sttsChg, zoneID, jamID;

	if ( optionID < OPTID_NUM ){
		gJam_CheckerMember.mDiscardOptionBit |= (0x0001 << optionID);

		// --- Clear specified module's jam zone ---
		sttsChg = false;

		for ( zoneID = 0; zoneID < JAM_ZONE_ID_NUM; zoneID++ ){
			if ( cJam_ZoneOptionID[zoneID] == optionID ){
				if ( gJam_CheckerMember.mJamZoneBit & JAM_ZONE_ID_CNV_BIT(zoneID) ){
					SetJamZone(zoneID);
					gJam_CheckerMember.mJamZoneClearCount[zoneID] = 0;
					sttsChg = true;
				}
				else{
					// Not Clear
				}
			}
			else{
				// Not Clear
			}
		}

		// --- Clear Timing Operation(Update Status/Start Initialize) ---
		if ( sttsChg == true ){
			if ( gJam_CheckerMember.mJamZoneBit == 0 ){
				if ( gJam_CheckerMember.mDynamicJamSttsBit != 0 ){
					for ( jamID = 0; jamID < JAM_DJAM_ID_NUM; jamID++ ){
						if ( CJamChecker_GetDynamicJamStatus(jamID) == FAULT_OCCUR ){
							ClearJamStatus(jamID);

							CSysManager_FaultStatusChange(cJam_DJamDevSttsNo[jamID], FAULT_NOT_OCCUR);
						}
						else{
							// Go for loop
						}
					}
				}
				else{
					CSysManager_FaultStatusChange(DSNO_STATIC_JAM, FAULT_NOT_OCCUR);
				}
			}
			else{
				// No Action
			}

			CSysManager_JamZoneStatusChange(OWN_DEV_CONT_ID);
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
// Function Name : CJamChecker_PowerOn
// Description   : 電源ON通知受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CJamChecker_PowerOn()
{
	UL beforeZoneBit;

	beforeZoneBit = gJam_CheckerMember.mJamZoneBit;

	if ( CheckJamZoneSetConditionSensor() == true ){
		if ( CJamChecker_GetDynamicJamStatus(JAM_DJAM_ID_NUM) == FAULT_NOT_OCCUR ){
			CSysManager_FaultStatusChange(DSNO_STATIC_JAM, FAULT_OCCUR);
		}
		else{ /* No Action */ }

		CSysManager_JamZoneStatusChange(OWN_DEV_CONT_ID);

		if ( beforeZoneBit == 0 ){
			MSP_SetEventTimer(JAM_CYCLIC_CHEAK_INTERVAL, JAM_ClearCyclicCheck, 0);
		}
		else{ /* No Action */ }
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CJamChecker_CoverClose
// Description   : カバークローズ通知受信時処理
// Parameter     : coverID (Cover ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CJamChecker_CoverClose(ECvrCoverID coverID)
{
	EFinisherState finStt;
	UL beforeZoneBit;
	UC setCheckChg, clrCheckChg, jamID;

	setCheckChg = false;
	clrCheckChg = false;

	beforeZoneBit = gJam_CheckerMember.mJamZoneBit;

	finStt = CSysManager_GetFinisherState();

	if ( ( (finStt == FIN_STT_STANDBY) ||										// システム動作モードがノーマルモードである、
		   (finStt == FIN_STT_POWER_UP) ||										// フィニッシングプロセスでない
		   (finStt == FIN_STT_POWER_ON) ||
		   (finStt == FIN_STT_CHANGE_NORMAL) ) &&
		 (CCvrChecker_GetCoverStatus(CVR_ID_NUM) == CVR_STTS_CLOSE) ){			// 全てのカバーとドロアがクローズされている
		setCheckChg = CheckJamZoneSetConditionSensor();
	}
	else{ /* No Action */ }

	if ( CheckJamZoneClearConditionCommon() == true ){
		clrCheckChg = CheckJamZoneClearConditionCover(coverID);
	}
	else{ /* No Action */ }

	if ( (setCheckChg == true) || (clrCheckChg == true) ){
		if ( CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_SET ){
			if ( CJamChecker_GetDynamicJamStatus(JAM_DJAM_ID_NUM) == FAULT_NOT_OCCUR ){
				CSysManager_FaultStatusChange(DSNO_STATIC_JAM, FAULT_OCCUR);
			}
			else{ /* No Action */ }
		}
		else{																	// All jam zone are cleared.
			if ( CJamChecker_GetDynamicJamStatus(JAM_DJAM_ID_NUM) == FAULT_OCCUR ){
				for ( jamID = 0; jamID < JAM_DJAM_ID_NUM; jamID++ ){
					if ( CJamChecker_GetDynamicJamStatus(jamID) == FAULT_OCCUR ){
						ClearJamStatus(jamID);
						CSysManager_FaultStatusChange(cJam_DJamDevSttsNo[jamID], FAULT_NOT_OCCUR);
					}
					else{ /* Go for loop */ }
				}
			}
			else{
				CSysManager_FaultStatusChange(DSNO_STATIC_JAM, FAULT_NOT_OCCUR);
			}
		}

		CSysManager_JamZoneStatusChange(OWN_DEV_CONT_ID);

		if ( beforeZoneBit == 0 ){
			MSP_SetEventTimer(JAM_CYCLIC_CHEAK_INTERVAL, JAM_ClearCyclicCheck, 0);
		}
		else{ /* No Action */ }
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CJamChecker_DeviceInitializeComplete
// Description   : イニシャライズ動作完了通知受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CJamChecker_DeviceInitializeComplete()
{
	UL beforeZoneBit;

	beforeZoneBit = gJam_CheckerMember.mJamZoneBit;

	if ( CheckJamZoneSetConditionSensor() == true ){
		if ( CJamChecker_GetDynamicJamStatus(JAM_DJAM_ID_NUM) == FAULT_NOT_OCCUR ){
			CSysManager_FaultStatusChange(DSNO_STATIC_JAM, FAULT_OCCUR);
		}
		else{ /* No Action */ }

		CSysManager_JamZoneStatusChange(OWN_DEV_CONT_ID);

		if ( beforeZoneBit == 0 ){
			MSP_SetEventTimer(JAM_CYCLIC_CHEAK_INTERVAL, JAM_ClearCyclicCheck, 0);
		}
		else{ /* No Action */ }
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CJamChecker_FinishingProcessEnd
// Description   : フィニッシングプロセス完了通知受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CJamChecker_FinishingProcessEnd()
{
	UL beforeZoneBit;

	beforeZoneBit = gJam_CheckerMember.mJamZoneBit;

	if ( CheckJamZoneSetConditionSensor() == true ){
		if ( CJamChecker_GetDynamicJamStatus(JAM_DJAM_ID_NUM) == FAULT_NOT_OCCUR ){
			CSysManager_FaultStatusChange(DSNO_STATIC_JAM, FAULT_OCCUR);
		}
		else{ /* No Action */ }

		CSysManager_JamZoneStatusChange(OWN_DEV_CONT_ID);

		if ( beforeZoneBit == 0 ){
			MSP_SetEventTimer(JAM_CYCLIC_CHEAK_INTERVAL, JAM_ClearCyclicCheck, 0);
		}
		else{ /* No Action */ }
	}
	else{ /* No Action */ }

	JudgeStartJamZoneClearCheckCycle();
}

///////////////////////////////////////////////////////////////////
// Function Name : CJamChecker_AllSheetFinishComplete
// Description   : フィニッシングプロセスにおける全ての搬送用紙に対
//               : する制御の完了通知受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CJamChecker_AllSheetFinishComplete()
{
	CCfgDevContConfiguration *pConfig;
	UL beforeZoneBit;
	UC checkRsltSensor, checkRsltLocation;

	beforeZoneBit = gJam_CheckerMember.mJamZoneBit;

	pConfig = CCfgInformer_GetDeviceControllerConfiguration(eCdiFinDevCont_All);

	checkRsltSensor = CheckJamZoneSetConditionSensor();
	checkRsltLocation = false;

	if ( CCfgDevContConfiguration_GetNoPaperRun(pConfig) == 0 ){				// Not No Paper Run
		checkRsltLocation = CheckJamZoneSetConditionLocation();
	}
	else{ /* No Action */ }

	if ( (checkRsltSensor == true) || (checkRsltLocation == true) ){
		if ( CJamChecker_GetDynamicJamStatus(JAM_DJAM_ID_NUM) == FAULT_NOT_OCCUR ){
			CSysManager_FaultStatusChange(DSNO_STATIC_JAM, FAULT_OCCUR);
		}
		else{ /* No Action */ }

		CSysManager_JamZoneStatusChange(OWN_DEV_CONT_ID);

		if ( beforeZoneBit == 0 ){
			MSP_SetEventTimer(JAM_CYCLIC_CHEAK_INTERVAL, JAM_ClearCyclicCheck, 0);
		}
		else{ /* No Action */ }
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CJamChecker_ChangeNormal
// Description   : ノーマルモードへの移行完了通知受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CJamChecker_ChangeNormal()
{
	UL beforeZoneBit;

	beforeZoneBit = gJam_CheckerMember.mJamZoneBit;

	if ( CheckJamZoneSetConditionSensor() == true ){
		if ( CJamChecker_GetDynamicJamStatus(JAM_DJAM_ID_NUM) == FAULT_NOT_OCCUR ){
			CSysManager_FaultStatusChange(DSNO_STATIC_JAM, FAULT_OCCUR);
		}
		else{ /* No Action */ }

		CSysManager_JamZoneStatusChange(OWN_DEV_CONT_ID);

		if ( beforeZoneBit == 0 ){
			MSP_SetEventTimer(JAM_CYCLIC_CHEAK_INTERVAL, JAM_ClearCyclicCheck, 0);
		}
		else{ /* No Action */ }
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CJamChecker_DropEjectSheet
// Description   : コンパイルトレイにあるシートの排紙断念通知受信時
//               : 処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CJamChecker_DropEjectSheet()
{
	UL beforeZoneBit;

	beforeZoneBit = gJam_CheckerMember.mJamZoneBit;

	if ( CSysManager_GetFinisherState() == FIN_STT_STANDBY ){
		if ( CheckJamZoneSetConditionSensor() == true ){
			if ( CJamChecker_GetDynamicJamStatus(JAM_DJAM_ID_NUM) == FAULT_NOT_OCCUR ){
				CSysManager_FaultStatusChange(DSNO_STATIC_JAM, FAULT_OCCUR);
			}
			else{ /* No Action */ }

			CSysManager_JamZoneStatusChange(OWN_DEV_CONT_ID);

			if ( beforeZoneBit == 0 ){
				MSP_SetEventTimer(JAM_CYCLIC_CHEAK_INTERVAL, JAM_ClearCyclicCheck, 0);
			}
			else{ /* No Action */ }
		}
		else{ /* No Action */ }
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CJamChecker_EntranceEnvironmentChange
// Description   : エントランス部ユーザー介入(IOT側カバークローズ)
//               : 通知受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CJamChecker_EntranceEnvironmentChange()
{
	UC index;

	if ( CheckJamZoneClearConditionCommon() == true ){
		if ( CheckJamZoneClearConditionEntrance() == true ){
			if ( CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_CLEAR ){
																				// All Jam Zone cleared.
				if ( CJamChecker_GetDynamicJamStatus(JAM_DJAM_ID_NUM) == FAULT_OCCUR ){
																				// Any Dynamic Jam is occurring.
					for ( index = 0; index < JAM_DJAM_ID_NUM; index++ ){
						if ( CJamChecker_GetDynamicJamStatus(index) == FAULT_OCCUR ){
							ClearJamStatus(index);

							CSysManager_FaultStatusChange(cJam_DJamDevSttsNo[index], FAULT_NOT_OCCUR);
						}
						else{ /* Go for loop */ }
					}
				}
				else{															// Any Dynamic Jam isn't occur(Staict Jam is occurring).
					CSysManager_FaultStatusChange(DSNO_STATIC_JAM, FAULT_NOT_OCCUR);
				}
			}
			else{ /* No Action */ }

			CSysManager_JamZoneStatusChange(OWN_DEV_CONT_ID);
		}
		else{ /* No Action */ }
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CJamChecker_CyclicCheck
// Description   : 周期チェックタイミング通知受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CJamChecker_CyclicCheck()
{
	UC jamID;

	if ( CheckJamZoneClearConditionCommon() == true ){
		if ( CheckJamZoneClearConditionCycle() == true ){
			if ( CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_CLEAR ){
																				// All Jam Zone are cleared in cyclic check.
				if ( CJamChecker_GetDynamicJamStatus(JAM_DJAM_ID_NUM) == FAULT_OCCUR ){
																				// Any Dynamic Jam are occuring. 
					for ( jamID = 0; jamID < JAM_DJAM_ID_NUM; jamID++ ){
						if ( CJamChecker_GetDynamicJamStatus(jamID) == FAULT_OCCUR ){
							ClearJamStatus(jamID);

							CSysManager_FaultStatusChange(cJam_DJamDevSttsNo[jamID], FAULT_NOT_OCCUR);
						}
						else{ /* Go for loop */ }
					}
				}
				else{
					CSysManager_FaultStatusChange(DSNO_STATIC_JAM, FAULT_NOT_OCCUR);
				}
			}
			else{ /* No Action */ }

			CSysManager_JamZoneStatusChange(OWN_DEV_CONT_ID);
		}
		else{ /* No Action */ }													// All Jam Zone don't change.
	}
	else{ /* No Action */ }

	if ( CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_SET ){
		MSP_SetEventTimer(JAM_CYCLIC_CHEAK_INTERVAL, JAM_ClearCyclicCheck, 0);
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CJamChecker_SheetLocation
// Description   : シートロケーション発生時処理
// Parameter     : sheetNo (Sheet No.)
//               : edge (シートエッジ識別子)
//               : location (Sheet Location ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CJamChecker_SheetLocation(UC sheetNo, EShtSheetEdge edge, EShtSheetLocationID location)
{
	const DJamSelectSpec *pJamSlctTbl;
	const CShtSheet *pSheet;
	US jamTimer;
	UC nextLoc, outDest, index;

	if ( (location < SHT_LOC_ID_NUM) && (edge == SHT_EDGE_LEAD) ){
		pSheet = CShtManager_GetSheet(sheetNo);
		nextLoc = CShtSheet_GetNextLocation(pSheet, SHT_EDGE_LEAD);
		outDest = CShtSheet_GetOutputDestFinish(pSheet);

		if ( cpJam_DynamicJamSelectTableLocation[location] != NULL ){
			pJamSlctTbl = cpJam_DynamicJamSelectTableLocation[location];

			index = 0;
			while ( pJamSlctTbl[index].jamID != JAM_DJAM_ID_NUM ){
				if ( ( (pJamSlctTbl[index].nextLocID == SHT_LOC_ID_NUM) || (pJamSlctTbl[index].nextLocID == nextLoc) ) &&
					( (pJamSlctTbl[index].planLocID == SHT_LOC_ID_NUM) || (CShtSheet_CheckLocationPlan(pSheet, pJamSlctTbl[index].planLocID) == true) ) ){
					jamTimer = CalculateJamTimer(sheetNo, pJamSlctTbl[index].jamID);
					StartJamTimer(sheetNo, pJamSlctTbl[index].jamID, jamTimer);
				}
				else{ /* Go while loop */ }

				index++;
			}
		}
		else{ /* No Action */ }
	}
	else{ /* No Action */ }		
}

///////////////////////////////////////////////////////////////////
// Function Name : CJamChecker_SheetEvent
// Description   : シートイベント発生時処理
// Parameter     : sheetNo (Sheet No.)
//               : event (Sheet Event ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CJamChecker_SheetEvent(UC sheetNo, EShtSheetEventID event)
{
	const DJamSelectSpec *pJamSlctTbl;
	const CShtSheet *pSheet;
	US jamTimer;
	UC nextLoc, index;

	if ( event < SHT_EVT_ID_NUM ){
		pSheet = CShtManager_GetSheet(sheetNo);
		nextLoc = CShtSheet_GetNextLocation(pSheet, SHT_EDGE_LEAD);

		if ( cpJam_DynamicJamSelectTableEvent[event] != NULL ){
			pJamSlctTbl = cpJam_DynamicJamSelectTableEvent[event];

			index = 0;
			while ( pJamSlctTbl[index].jamID != JAM_DJAM_ID_NUM ){
				if ( (pJamSlctTbl[index].nextLocID == SHT_LOC_ID_NUM) ||
					 (pJamSlctTbl[index].nextLocID == nextLoc) ){
					jamTimer = CalculateJamTimer(sheetNo, pJamSlctTbl[index].jamID);
					StartJamTimer(sheetNo, pJamSlctTbl[index].jamID, jamTimer);
				}
				else{ /* Go while loop */ }

				index++;
			}
		}
		else{ /* No Action */ }
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CJamChecker_JamOccurrence
// Description   : ジャム発生指示受信時処理(デバイスモジュールでジ
//               : ャムを検知する際の処理)
// Parameter     : statusNo (Device Status No.)
//               : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CJamChecker_JamOccurrence(US statusNo, UC sheetNo)
{
	CCfgDevContConfiguration *pConfig;
	const CShtSheet *pSheet;
	UL checkBit;
	UC jamID, zoneID;

	pConfig = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);

	if ( CCfgDevContConfiguration_GetJamBypass(pConfig) == 0 ){					// Not execute jam bypass
		pSheet = CShtManager_GetSheet(sheetNo);

		if ( CShtSheet_CheckConditionDelivery(pSheet) == true ){
			// --- Jam IDマッピング ---
			for ( jamID = 0; jamID < JAM_DJAM_ID_NUM; jamID++ ){
				if ( cJam_DJamDevSttsNo[jamID] == statusNo ){
					break;
				}
				else{ /* Go For Loop */ }
			}

			// --- Dynamic Jam発生処理 ---
			if ( jamID < JAM_DJAM_ID_NUM ){
				SetJamStatus(jamID);

				checkBit = cJam_DJamDetectSpec[jamID].jamZoneBit;

				for ( zoneID = 0; zoneID < JAM_ZONE_ID_NUM; zoneID++ ){
					if ( checkBit != 0 ){
						if ( (checkBit & 0x00000001) != 0 ){
							SetJamZone(zoneID);
						}
						else{ /* No Action */ }

						checkBit >>= 1;
					}
					else{
						break;
					}
				}

				CShtManager_DynamicJamStop(cJam_DJamDevSttsNo[jamID], sheetNo);
				CSysManager_FaultStatusChange(cJam_DJamDevSttsNo[jamID], FAULT_OCCUR);
				CSysManager_JamZoneStatusChange(OWN_DEV_CONT_ID);

				SRVC_Moview_SetJamOccur(sheetNo);
			}
			else{ /* No Action */ }
		}
		else{ /* No Action */ }
	}
	else{ /* No Action */ }														// Execute Jam Bypass
}

///////////////////////////////////////////////////////////////////
// Function Name : CJamChecker_JamTimerTimeUp
// Description   : ジャムタイマータイムアップ時処理
// Parameter     : jamID (Dynamic Jam ID)
//               : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CJamChecker_JamTimerTimeUp(EJamDynamicJamID jamID, UC sheetNo)
{
	const CShtSheet *pSheet;
	UL checkBit;
	UC zoneID;

	if ( JudgeDynamicJam(sheetNo, jamID) == true ){
		SetJamStatus(jamID);

		checkBit = cJam_DJamDetectSpec[jamID].jamZoneBit;

		for ( zoneID = 0; zoneID < JAM_ZONE_ID_NUM; zoneID++ ){
			if ( checkBit != 0 ){
				if ( (checkBit & 0x00000001) != 0 ){
					SetJamZone(zoneID);
				}
				else{ /* No Action */ }

				checkBit >>= 1;
			}
			else{
				break;
			}
		}

		CShtManager_DynamicJamStop(cJam_DJamDevSttsNo[jamID], sheetNo);
		CSysManager_FaultStatusChange(cJam_DJamDevSttsNo[jamID], FAULT_OCCUR);
		CSysManager_JamZoneStatusChange(OWN_DEV_CONT_ID);

		SRVC_Moview_SetJamOccur(sheetNo);
	}
	else{ /* No Action */ }
}


/////////////////////////////////////////////////////////////////////////////////////////
// Timer Event Call Back Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : JAM_ClearCyclicCheck
// Description   : ジャムクリア周期チェックイベントコールバック
// Parameter     : timerID (MSP Timer ID)
//               : notUsed (Not Used)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void JAM_ClearCyclicCheck(SS timerID, UL notUsed)
{
	CJamChecker_CyclicCheck();
}

///////////////////////////////////////////////////////////////////
// Function Name : JAM_JamTimerTimeUp
// Description   : ジャムタイマータイムアップイベントコールバック
// Parameter     : timerID (Timer ID)
//               : parameter (Parameter：Bit0～7 = Sheet No./Bit8～15 = Jam ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void JAM_JamTimerTimeUp(SS timerID, UL parameter)
{
	UC jamID, sheetNo;

	jamID   = (UC)(parameter >> 8);
	sheetNo = (UC)parameter;

	CJamChecker_JamTimerTimeUp(jamID, sheetNo);
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.02.10	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ Power Down後ステータス誤通知防止 ]
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ Moview ]
// 15.03.30	| Yuichi Hoshino	| Design Improvement [ ジャムチェックオプション条件確認処理関数化 ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
