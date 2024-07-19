// File Name:	CMPL_Manager.c
// Description:	Compiler Module Manager
// Engineer:	Takahiko Minaga
// Last Edit:	14.08.12
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"
#include "MSP_EventID.h"
#include "MSP_Service.h"
#include "CCdiSysOpeMode.h"
#include "CMPL_Manager.h"
#include "CMPL_STPL_Boundary.h"
#include "CMPL_EJCT_Boundary.h"
#include "CMPL_TMP_Boundary.h"
#include "CNvmInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CMPL_EJECT_PROCESS_SIZE_THRESH 		2970 //2160								// 用紙排紙実施の用紙長閾値(×0.1[mm])
#define CMPL_DISTANCE_CMPEXTSNRTOEXITROLL 	30.0								// Compiler Exit SensorからExit Rollまでの距離(×1[mm])
#define CMPL_DISTANCE_EXITROLLRAD			10.0								// Exit Rollの半径(×1[mm])
#define CMPL_PUNCHFILTERTIME				30									// パンチ穴後端誤検知フィルタ時間(×1[ms])

typedef enum
{
	CMPL_MDL_ID_EJCT,
	CMPL_MDL_ID_STPL,
	CMPL_MDL_ID_TMP,
	CMPL_MDL_ID_NUM,
} ECMPL_ModuleID;

typedef struct
{
	UC link;
	ECMPL_ModuleID moduleID;
} SCMPL_CompoCont;

typedef enum /* ECMPL_SequenceId */
{
	CMPL_SEQ_ID_POWER_ON_INIT,
	CMPL_SEQ_ID_RESUME_INIT,
	CMPL_SEQ_ID_NORMAL_INIT,
	CMPL_SEQ_ID_SHEET_INIT,
	CMPL_SEQ_ID_COMPILE_FINISHING,
	CMPL_SEQ_ID_EJECT_FINISHING,
	CMPL_SEQ_ID_STANDBY_EJECT_FINISHING,
	CMPL_SEQ_ID_PROC_END,
	CMPL_SEQ_ID_JAM_CLEAR_ASSIST,

	CMPL_SEQ_ID_NUM,
} ECMPL_SequenceId;

typedef struct
{
	US actID;
	US nextBit;
	US condBit;
} SCMPL_MdlActSeq;


typedef struct
{
	ECMPL_SequenceId sequenceId;
	UL  actionLog; //US -> UL 
	UC sheetNo;
} SCMPL_SeqMng;

#define CMPL_COMPO_CHAIN	12
// Component Control Table
static const SCMPL_CompoCont cCMPL_CompoContTbl[] = {
//	  Link		Compiler Module ID
	{ 20,		CMPL_MDL_ID_EJCT	},
	{ 21,		CMPL_MDL_ID_EJCT	},
	{ 22,		CMPL_MDL_ID_EJCT	},
	{ 30,		CMPL_MDL_ID_TMP		},
	{ 31,		CMPL_MDL_ID_TMP		},
	{ 32,		CMPL_MDL_ID_TMP		},
	{ 33,		CMPL_MDL_ID_TMP		},
	{ 34,		CMPL_MDL_ID_TMP		},
	{ 35,		CMPL_MDL_ID_TMP		},
	{ 36,		CMPL_MDL_ID_TMP		},	// For Test
	{ 40,		CMPL_MDL_ID_TMP		},
	{ 41,		CMPL_MDL_ID_TMP		},
	{ 42,		CMPL_MDL_ID_TMP		},
	{ 43,		CMPL_MDL_ID_TMP		},
	{ 44,		CMPL_MDL_ID_TMP		},
	{ 45,		CMPL_MDL_ID_TMP		},
	{ 46,		CMPL_MDL_ID_TMP		},	// For Test
	{ 50,		CMPL_MDL_ID_STPL	},
	{ 51,		CMPL_MDL_ID_STPL	},
	{ 52,		CMPL_MDL_ID_STPL	},	// For Test
};

#define CMPL_SHTMNGNUM	4

typedef struct
{
	ECMPL_State state;
	UC ejectPossible;
	SCMPL_SeqMng seqMngInfo[CMPL_SHTMNGNUM];
	UC lastStaSeqIndex;
	UC compileSheet;
	US sheetInitAction[SHEETNO_NUM];
	UC  lastOffset;    //(TB- )SetTopのOffset情報がNVMに正しく書いていない 
	// ↓↓↓(V3.0.6) CompileJobの処理前、Standbyに遷移するTBの対策
	UC  finishingWaitSheet; 	
} SCMPL_Manager;

static SCMPL_Manager gCMPL_Manager;

#define CMPLMDLACTBIT_STAPLEHEADHOME		GET_BIT_PATTERN(CMPL_MDL_ACT_STAPLE_HEAD_HOME)
#define CMPLMDLACTBIT_EJECTORHOME			GET_BIT_PATTERN(CMPL_MDL_ACT_EJECTOR_HOME)
#define CMPLMDLACTBIT_STAPLEHARIOUT			GET_BIT_PATTERN(CMPL_MDL_ACT_STAPLE_HARIOUT)
#define CMPLMDLACTBIT_PADDLEHOME			GET_BIT_PATTERN(CMPL_MDL_ACT_PADDLE_HOME)
#define CMPLMDLACTBIT_PADDLECOMPILE			GET_BIT_PATTERN(CMPL_MDL_ACT_PADDLE_COMPILE)
#define CMPLMDLACTBIT_EJECT					GET_BIT_PATTERN(CMPL_MDL_ACT_EJECT)
#define CMPLMDLACTBIT_TAMPSIZEPOS			GET_BIT_PATTERN(CMPL_MDL_ACT_TAMP_SIZE_POS)
#define CMPLMDLACTBIT_CURRENTOFF           	GET_BIT_PATTERN(CMPL_MDL_ACT_CURRENT_OFF)  //V3.6.93
#define CMPLMDLACTBIT_TAMPMISSREGI		GET_BIT_PATTERN(CMPL_MDL_ACT_TAMP_MISS_REGI)  //V3.5.95
//#define CMPLMDLACTBIT_SHEETCOMPILEWAIT		GET_BIT_PATTERN(CMPL_MDL_ACT_SHEET_COMPILE_WAIT)
#define CMPLMDLACTBIT_NORMAL_TAMPING		GET_BIT_PATTERN(CMPL_MDL_ACT_NORMAL_TAMPING)
#define CMPLMDLACTBIT_EJECT_TAMPING			GET_BIT_PATTERN(CMPL_MDL_ACT_EJECT_TAMPING)
#define CMPLMDLACTBIT_STAPLE				GET_BIT_PATTERN(CMPL_MDL_ACT_STAPLE)
#define CMPLMDLACTBIT_OFFSET				GET_BIT_PATTERN(CMPL_MDL_ACT_OFFSET)
#define CMPLMDLACTBIT_RETURN				GET_BIT_PATTERN(CMPL_MDL_ACT_RETURN)
#define CMPLMDLACTBIT_END					GET_BIT_PATTERN(CMPL_MDL_ACT_END)

// Sequence table of "Compiler Module Action"
static const SCMPL_MdlActSeq cCMPL_PowerOnInitSeq[] = {
	{ CMPL_MDL_ACT_START,					CMPLMDLACTBIT_STAPLEHEADHOME,		0,								},
	{ CMPL_MDL_ACT_STAPLE_HEAD_HOME,		CMPLMDLACTBIT_STAPLEHARIOUT,		0,								},
	{ CMPL_MDL_ACT_STAPLE_HARIOUT,			CMPLMDLACTBIT_END,			0,								},
	{ CMPL_MDL_ACT_END,						0,									0,								}
};

static const SCMPL_MdlActSeq cCMPL_ResumeInitSeq[] = {
	{ CMPL_MDL_ACT_START,					CMPLMDLACTBIT_STAPLEHEADHOME,		0,								},
	{ CMPL_MDL_ACT_STAPLE_HEAD_HOME,		CMPLMDLACTBIT_STAPLEHARIOUT,			0,								},
	{ CMPL_MDL_ACT_STAPLE_HARIOUT,		CMPLMDLACTBIT_END,						0,								},
	{ CMPL_MDL_ACT_END,						0,									0,								}
};

static const SCMPL_MdlActSeq cCMPL_NormalInitSeq[] = {
	{ CMPL_MDL_ACT_START,					CMPLMDLACTBIT_STAPLEHEADHOME,		0,								},
	{ CMPL_MDL_ACT_STAPLE_HEAD_HOME,		CMPLMDLACTBIT_STAPLEHARIOUT,		0,								},
	{ CMPL_MDL_ACT_STAPLE_HARIOUT,			CMPLMDLACTBIT_EJECTORHOME,			0,								},
	{ CMPL_MDL_ACT_EJECTOR_HOME,			CMPLMDLACTBIT_END,					0,								},
	{ CMPL_MDL_ACT_END,						0,									0,								}
};

static const SCMPL_MdlActSeq cCMPL_SheetInitSeq[] = {
	{ CMPL_MDL_ACT_START,					CMPLMDLACTBIT_TAMPSIZEPOS,			0,								},
	{ CMPL_MDL_ACT_TAMP_SIZE_POS,			CMPLMDLACTBIT_PADDLEHOME,				0,								},
	{ CMPL_MDL_ACT_PADDLE_HOME,				CMPLMDLACTBIT_EJECTORHOME,			0,								},
	{ CMPL_MDL_ACT_EJECTOR_HOME,			CMPLMDLACTBIT_END,					0,								},
	{ CMPL_MDL_ACT_END,						0,									0,								}
};

static const SCMPL_MdlActSeq cCMPL_CompileFinishingSeq[] = {
	{ CMPL_MDL_ACT_START,					CMPLMDLACTBIT_PADDLECOMPILE | CMPLMDLACTBIT_NORMAL_TAMPING ,	0},
	{ CMPL_MDL_ACT_PADDLE_COMPILE, 			CMPLMDLACTBIT_END,		CMPLMDLACTBIT_NORMAL_TAMPING		},	
	{ CMPL_MDL_ACT_NORMAL_TAMPING,			CMPLMDLACTBIT_END,		CMPLMDLACTBIT_PADDLECOMPILE		},
	{ CMPL_MDL_ACT_END, 					0,									0		}

};
static const SCMPL_MdlActSeq cCMPL_EjectFinishingSeq[] = {

	{ CMPL_MDL_ACT_START,					CMPLMDLACTBIT_PADDLECOMPILE | CMPLMDLACTBIT_EJECT_TAMPING,		0	},
	{ CMPL_MDL_ACT_PADDLE_COMPILE,			CMPLMDLACTBIT_STAPLE | CMPLMDLACTBIT_TAMPMISSREGI,		CMPLMDLACTBIT_EJECT_TAMPING	},
	{ CMPL_MDL_ACT_EJECT_TAMPING,			CMPLMDLACTBIT_STAPLE | CMPLMDLACTBIT_TAMPMISSREGI,		CMPLMDLACTBIT_PADDLECOMPILE	},
	
	{ CMPL_MDL_ACT_STAPLE,					CMPLMDLACTBIT_OFFSET,									CMPLMDLACTBIT_TAMPMISSREGI },
	{ CMPL_MDL_ACT_TAMP_MISS_REGI, 			CMPLMDLACTBIT_OFFSET, 									CMPLMDLACTBIT_STAPLE },
	{ CMPL_MDL_ACT_OFFSET,					CMPLMDLACTBIT_RETURN | CMPLMDLACTBIT_EJECT	,	0					},
	{ CMPL_MDL_ACT_RETURN,					CMPLMDLACTBIT_END,					CMPLMDLACTBIT_EJECT				},
	{ CMPL_MDL_ACT_EJECT,					CMPLMDLACTBIT_END,					CMPLMDLACTBIT_RETURN			},
	{ CMPL_MDL_ACT_END,						0,									0								}

};

static const SCMPL_MdlActSeq cCMPL_StandbyEjectFinishingSeq[] = {

	{ CMPL_MDL_ACT_START,					CMPLMDLACTBIT_PADDLECOMPILE | CMPLMDLACTBIT_EJECT_TAMPING,		0	},
	{ CMPL_MDL_ACT_PADDLE_COMPILE,			CMPLMDLACTBIT_STAPLE | CMPLMDLACTBIT_TAMPMISSREGI,		CMPLMDLACTBIT_EJECT_TAMPING },
	{ CMPL_MDL_ACT_EJECT_TAMPING,			CMPLMDLACTBIT_STAPLE | CMPLMDLACTBIT_TAMPMISSREGI,		CMPLMDLACTBIT_PADDLECOMPILE },

	{ CMPL_MDL_ACT_STAPLE,					CMPLMDLACTBIT_OFFSET,									CMPLMDLACTBIT_TAMPMISSREGI },
	{ CMPL_MDL_ACT_TAMP_MISS_REGI,			CMPLMDLACTBIT_OFFSET,									CMPLMDLACTBIT_STAPLE },
	{ CMPL_MDL_ACT_OFFSET,					CMPLMDLACTBIT_RETURN | CMPLMDLACTBIT_EJECT	,	0					},
	{ CMPL_MDL_ACT_RETURN,					CMPLMDLACTBIT_END,					CMPLMDLACTBIT_EJECT 			},
	{ CMPL_MDL_ACT_EJECT,					CMPLMDLACTBIT_END,					CMPLMDLACTBIT_RETURN			},
	{ CMPL_MDL_ACT_END, 					0,									0								}
	
};

static const SCMPL_MdlActSeq cCMPL_ProcEndSeq[] = {
	{ CMPL_MDL_ACT_START,					CMPLMDLACTBIT_STAPLEHEADHOME,		0,								},
//	{ CMPL_MDL_ACT_STAPLE_HEAD_HOME,		CMPLMDLACTBIT_END,					0,								},
	{ CMPL_MDL_ACT_STAPLE_HEAD_HOME,		CMPLMDLACTBIT_CURRENTOFF,			0,								},
	{ CMPL_MDL_ACT_CURRENT_OFF,			CMPLMDLACTBIT_END,					0,								},
	{ CMPL_MDL_ACT_END,						0,									0,								}
};

static const SCMPL_MdlActSeq cCMPL_JamClearAssist[] = {
	{ CMPL_MDL_ACT_START,					CMPLMDLACTBIT_STAPLEHEADHOME,		0,								},
	{ CMPL_MDL_ACT_STAPLE_HEAD_HOME,		CMPLMDLACTBIT_END,					0,								},
	{ CMPL_MDL_ACT_END,						0,									0,								}
};

static const SCMPL_MdlActSeq* cCMPL_ModuleActionSequence[CMPL_SEQ_ID_NUM] = {
	cCMPL_PowerOnInitSeq,
	cCMPL_ResumeInitSeq,
	cCMPL_NormalInitSeq,
	cCMPL_SheetInitSeq,
	cCMPL_CompileFinishingSeq,
	cCMPL_EjectFinishingSeq,
	cCMPL_StandbyEjectFinishingSeq,
	cCMPL_ProcEndSeq,
	cCMPL_JamClearAssist,
};

/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
static US CMPL_GetUnStapleEjectThresh(US aMaxProcSize, UC  aSheetNo);
static US CMPL_GetStapleEjectThresh(UC aSheetNo);

static bool CMPL_DownCompCheck();
static bool CMPL_CheckEjectPossible();
static void CMPL_ClearSequenceManageInfo(UC aIndex);
static void CMPL_PowerOnInit(EAction aAction);
static void CMPL_StartPowerOnInit();
static void CMPL_CompletePowerOnInit();
static void CMPL_StopPowerOnInit();
static void CMPL_ResumeInit(EAction aAction);
static void CMPL_StartResumeInit();
static void CMPL_CompleteResumeInit();
static void CMPL_StopResumeInit();
static void CMPL_NormalInit(EAction aAction);
static void CMPL_StartNormalInit();
static void CMPL_CompleteNormalInit();
static void CMPL_StopNormalInit();
static void CMPL_SheetInitialize(UC aSheetNo, EAction aAction);
static void CMPL_StartSheetInitialize(UC aSheetNo);
static void CMPL_CompleteSheetInitialize(UC aIndex);
static void CMPL_StopSheetInitialize(UC aSheetNo);
static void CMPL_ProcessEnd(EAction aAction);
static void CMPL_StartProcessEnd();
static void CMPL_CompleteProcessEnd();
static void CMPL_StopProcessEnd();
static void CMPL_JamClearAssist(EAction aAction);
static void CMPL_StartJamClearAssist();
static void CMPL_CompleteJamClearAssist();
static void CMPL_StopJamClearAssist();
static void CMPL_CompilerAction(UC aSheetNo, EAction aAction);
static void CMPL_StartCompilerAction(UC aSheetNo);
static void CMPL_CompleteCompilerAction(UC aIndex);
static void CMPL_StopCompilerAction(UC aSheetNo);

static void CMPL_ExecuteSequence(ECMPL_ModuleAction aCompAct, UC aSeqMngIdx);

//static void CMPL_ExitRollReleaseWait(UC aSheetNo);
//static US CMPL_GetExitRollReleaseTime(UC aSheetNo);
//static void CMPL_SheetCompileWait(UC aSheetNo);
//static US CMPL_GetSheetCompileTime(UC aSheetNo);

static void CMPL_CurrentOFF(UC aSheetNo);
static void CMPL_EventTimerTimeUp(SS aTimerID, UL aSheetNo);

static void CMPL_CompileExitSensorON(UC aSheetNo);
static void CMPL_CompileExitSensorOFF(UC aSheetNo);
static void CMPL_CompileSheetEject();
static void CMPL_SheetAttributeUpdate(UC aSheetNo);
static void CMPL_SheetAbort();
static void CMPL_SheetDelivered();

static void CMPL_ChangeStartUpEvent();
static void CMPL_ChangePowerUpEvent();
static void CMPL_ChangePowerOnEvent();
static void CMPL_ChangePowerDownEvent();
static void CMPL_ChangePowerOffEvent();
static void CMPL_ChangeChangingStandbyEvent();
static void CMPL_ChangeStandbyEvent();
static void CMPL_ChangeCycleUpEvent();
static void CMPL_ChangeReadyEvent();

static void CMPL_ChangeChangingDiagEvent();
static void CMPL_ChangeDiagEvent();
static void CMPL_ChangeHardDownEvent();
static void CMPL_ChangeCycleDownEvent();
static void CMPL_ChangeProcDownEvent();



/////////////////////////////////////////////////////////////////////////////////////////
// Function Body (Sequence)
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetEjectExecutionSheetReq
// Description   : 対象シートに対して、分割排紙を実施するか/しないかを取得
//				 : (対象シートの次用紙の情報なし)
// Parameter     : sheetNo(対象シート)
// Return        : eject(実施する：SHT_EJECT_EXEC、実施しない：SHT_EJECT_NOT)
///////////////////////////////////////////////////////////////////
EShtEject CMPL_GetEjectExecutionSheetReq(UC sheetNo)
{
	EShtEject eject;
	US ejectThresh, stackCount, maxProcSize;

	if( SRVC_GetDestinationFinish(sheetNo) == eCdiFinOutDest_OutTray ) {
		stackCount = SRVC_GetWeightedCompileCount(sheetNo);						// ×0.1[枚]
		maxProcSize = SRVC_GetMaxProcessSizeCompile(sheetNo);

		if( SRVC_GetStapleMode(sheetNo) == eCdiFinStaple_Off ) {
			ejectThresh = CMPL_GetUnStapleEjectThresh(maxProcSize, sheetNo);
		}
		else {																	// Staple実施
			ejectThresh = CMPL_GetStapleEjectThresh(sheetNo);
		}

		if( stackCount > ejectThresh ) {
			eject = SHT_EJECT_EXEC;
		}
		else {																	// 50枚超え
			eject = SHT_EJECT_NOT;
		}
	}
	else {																		// 排紙先がOutTray以外
		eject = SHT_EJECT_EXEC;
	}
//	PutCMPLManagerLog(0x33,eject,stackCount,ejectThresh);

	return eject;
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetEjectExecutionNextSheetReq
// Description   : 対象シートに対して、分割排紙を実施するか/しないかを取得
//				 : (対象シートの次用紙の情報あり)
// Parameter     : sheetNo(対象シート)
// Return        : eject(実施する：SHT_EJECT_EXEC、実施しない：SHT_EJECT_NOT)
///////////////////////////////////////////////////////////////////
EShtEject CMPL_GetEjectExecutionNextSheetReq(UC sheetNo, UC nextShtNo)
{
	EShtEject eject;
	US ejectThresh, stackCount, maxProcSize, nextShtProcSize;

	if( SRVC_GetDestinationFinish(sheetNo) == eCdiFinOutDest_OutTray ) {
		stackCount = SRVC_GetWeightedCompileCount(sheetNo) + CMPL_GetWeightedCompileCount(nextShtNo);	// 次用紙を考慮した重みづけ積載枚数(×0.1[枚])
		nextShtProcSize = SRVC_GetProcessSize(nextShtNo);
		maxProcSize = SRVC_GetMaxProcessSizeCompile(sheetNo);
		if( nextShtProcSize > maxProcSize ) {									// 次用紙を考慮した積載最大用紙長(×0.1[mm])
			maxProcSize = nextShtProcSize;
		}

		if( SRVC_GetStapleMode(sheetNo) == eCdiFinStaple_Off ) {
			ejectThresh = CMPL_GetUnStapleEjectThresh(maxProcSize, sheetNo);
		}
		else {
			ejectThresh = CMPL_GetStapleEjectThresh(sheetNo);
		}

		if( stackCount > ejectThresh ) {
			eject = SHT_EJECT_EXEC;
		}
		else {
			eject = SHT_EJECT_NOT;
		}
	}
	else {																		// 排紙先がOutTray以外
		eject = SHT_EJECT_EXEC;
	}
//	PutCMPLManagerLog(0x44,eject,stackCount,ejectThresh);
	return eject;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetUnStapleEjectThresh
// Description   : UnStaple指定時の分割排紙の閾値を取得
// Parameter     : aMaxProcSize(コンパイラ積載用紙内で、最大の用紙送り長)
// Return        : 分割排紙の閾値(×0.1[枚])
// History 		: 
//仕様変更(V3.6.0）条件変更
//ステープルなしCompileTrayStack上限枚数(用紙送り長　<=297mm＆90gsm以下）
//ステープルなしCompileTrayStack上限枚数(用紙送り長　＞297mm　また　用紙送り長　<=297mm＆90gsm超え）
///////////////////////////////////////////////////////////////////
//US CMPL_GetUnStapleEjectThresh(US aMaxProcSize)
US CMPL_GetUnStapleEjectThresh(US aMaxProcSize, UC  aSheetNo)
{
	US	aEjectThresh;
	US	aMediaWeight =	SRVC_GetMediaWeight(aSheetNo); 

	//ステープルなしCompileTrayStack上限枚数(用紙送り長　<=297mm＆90gsm以下）
	if( (aMaxProcSize <= CMPL_EJECT_PROCESS_SIZE_THRESH ) &&  (aMediaWeight <= 90)) {
		aEjectThresh = (US)(SRVC_GetNvmValueUS(NVMID_MAX_COMPILE_SHEET_FOR_UNSTAPLE_SIZE_S) * 10); //100;
	}
	else {																		// 297.0[mm]超え
		aEjectThresh = (US)(SRVC_GetNvmValueUS(NVMID_MAX_COMPILE_SHEET_FOR_UNSTAPLE_SIZE_L) * 10); //100;
	}

	return aEjectThresh;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetStapleEjectThresh
// Description   : Staple指定時の分割排紙の閾値を取得
// Parameter     : ‐
// Return        : 分割排紙の閾値(×0.1[枚])
///////////////////////////////////////////////////////////////////
US CMPL_GetStapleEjectThresh(UC aSheetNo)
{
	// V3.0  ( StackerTray排紙Staple可能枚数)
		/* 用紙　1枚当たりの換算枚数　=　A　/　B
			A　：　StaplerUnit種類毎のStaple可能枚数[枚] : 単位= ×0.1[枚])
			B　：　紙種、坪量に応じたStaple可能枚数[枚]  : NVM
		*/
		US aEjectThresh;
		US	aMediaType = SRVC_GetMediaType(aSheetNo);  //
		US	aMediaWeight =	SRVC_GetMediaWeight(aSheetNo);  //
	
		US	 aWeightEjectThresh = 0; //NVM
		US	 retValue =0;
		aEjectThresh = (US)(SRVC_GetNvmValueUS(NVMID_MAX_COMPILE_SHEET_COUNT_FOR_STAPLE_50) * 10);// 65枚ステープラ
		//　非コート紙
		if ( aMediaType != eCdiFinMediaType_Coated ) { 
	
			if ( aMediaWeight >= 60  &&  aMediaWeight <= 80 ) {  //坪量(gsm) = 60 ~ 80
	
				aWeightEjectThresh = SRVC_GetNvmValueUS(NVMID_MAX_SHEET_UNCOAT_80_STAPLE_50);
				
			} else if ( aMediaWeight >= 81	&&	aMediaWeight <= 90 ) {	//坪量(gsm) = 81 ~ 90
	
				aWeightEjectThresh = SRVC_GetNvmValueUS(NVMID_MAX_SHEET_UNCOAT_90_STAPLE_50);
				
			} else if ( aMediaWeight >= 91	&&	aMediaWeight <= 105 ) {  //坪量(gsm) = 91 ~ 105 
	
				aWeightEjectThresh = SRVC_GetNvmValueUS(NVMID_MAX_SHEET_UNCOAT_105_STAPLE_50);
				
			} else if ( aMediaWeight >= 106  &&  aMediaWeight <= 128 ) {  //坪量(gsm) = 106 ~ 128 
	
				aWeightEjectThresh = SRVC_GetNvmValueUS(NVMID_MAX_SHEET_UNCOAT_128_STAPLE_50);
				
			} else if ( aMediaWeight >= 129  &&  aMediaWeight <= 150 ) {  //坪量(gsm) = 129 ~ 150 
	
				aWeightEjectThresh = SRVC_GetNvmValueUS(NVMID_MAX_SHEET_UNCOAT_150_STAPLE_50);
				
			} else if ( aMediaWeight >= 151  &&  aMediaWeight <= 176 ) {  //坪量(gsm) = 151 ~ 176 
	
				aWeightEjectThresh = SRVC_GetNvmValueUS(NVMID_MAX_SHEET_UNCOAT_176_STAPLE_50);
				
			} else if ( aMediaWeight >= 177  &&  aMediaWeight <= 220 ) {  //坪量(gsm) = 177 ~ 220 
	
				aWeightEjectThresh = SRVC_GetNvmValueUS(NVMID_MAX_SHEET_UNCOAT_220_STAPLE_50);
				
			}
			
		} else {	//　コート紙
	
			if ( aMediaWeight >= 106  &&  aMediaWeight <= 128 ) {  //坪量(gsm) = 106 ~ 128
	
				aWeightEjectThresh = SRVC_GetNvmValueUS(NVMID_MAX_SHEET_COAT_128_STAPLE_50);
				
			} else if ( aMediaWeight >= 129 &&	aMediaWeight <= 150 ) {  //坪量(gsm) = 129 ~ 150 
	
				aWeightEjectThresh = SRVC_GetNvmValueUS(NVMID_MAX_SHEET_COAT_150_STAPLE_50);
				
			} else if ( aMediaWeight >= 151 &&	aMediaWeight <= 176 ) {  //坪量(gsm) = 151 ~ 176 
	
				aWeightEjectThresh = SRVC_GetNvmValueUS(NVMID_MAX_SHEET_COAT_176_STAPLE_50);
				
			} else if ( aMediaWeight >= 177  &&  aMediaWeight <= 220 ) {  //坪量(gsm) = 177 ~ 220 
	
				aWeightEjectThresh = SRVC_GetNvmValueUS(NVMID_MAX_SHEET_COAT_220_STAPLE_50);
				
			}
			
		}
		
		aWeightEjectThresh = aWeightEjectThresh * (US)10;

		if ( aWeightEjectThresh != 0 ) {
			if ( aWeightEjectThresh < aEjectThresh ) {
				retValue = aWeightEjectThresh ;
			} else {
				retValue = aEjectThresh ;
			}
		} else {
			retValue = aEjectThresh ;	
		}

		return	retValue;

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetEjectExecutionStandby
// Description   : Standby遷移時に用紙排出をするか/しないかを判断
// Parameter     : aSheetNo (対象シートナンバー)
// Return        : SHT_EJECT_EXEC：排出をする、SHT_EJECT_NOT：排出をしない
///////////////////////////////////////////////////////////////////
EShtEject CMPL_GetEjectExecutionStandby(UC aSheetNo)
{
	if( (SRVC_GetDestinationFinish(aSheetNo) == eCdiFinOutDest_OutTray) &&
		(SRVC_GetCoverStatus() == CVR_STTS_CLOSE) &&
		(SRVC_GetDeviceCondition(DEVID_OUT_TRAY) != DEV_COND_PERM_NG) &&
		(SRVC_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_CLEAR) &&
		(SRVC_CheckSheetConditionStop(aSheetNo) == false) &&
		(SRVC_GetStapleMode(aSheetNo) == eCdiFinStaple_Off) ) {
		return SHT_EJECT_EXEC;
	}
	else {
		return SHT_EJECT_NOT;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetWeightedCompileCount
// Description   : 対象シートの重み枚数を取得
// Parameter     : aSheetNo(対象シートのSheetNo)
// Return        : 重み枚数(×0.1[枚])
///////////////////////////////////////////////////////////////////
US CMPL_GetWeightedCompileCount(UC sheetNo)
{
	return 10;

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetOffsetFinishPosition
// Description   : 対象シートのオフセットフィニッシュポジションを取得
// Parameter     : aSheetNo(対象シートのSheetNo)
// Return        : 対象シートのオフセットフィニッシュポジション
///////////////////////////////////////////////////////////////////
EShtOffsetFinish CMPL_GetOffsetFinishPosition(UC aSheetNo)
{
	UC aPrevShtNo;
	EShtOffsetFinish aPrevOfstFin, aOfstFin;

	if( SRVC_GetDestinationFinish(aSheetNo) == eCdiFinOutDest_OutTray ) {
		aPrevShtNo = SRVC_GetPrevRequestSheet(aSheetNo);

		while( aPrevShtNo != SHEETNO_NUM ) {
			if( SRVC_GetDestinationFinish(aPrevShtNo) == eCdiFinOutDest_OutTray ) {
				break;
			}
			else { /* No Action */ }

			aPrevShtNo = SRVC_GetPrevRequestSheet(aPrevShtNo);
		}

		if( aPrevShtNo != SHEETNO_NUM ) {
			aPrevOfstFin = SRVC_GetOffsetFinish(aPrevShtNo);
		}
		else {
			// aPrevOfstFin = nCmpl_Last_Offset_Position;
			aPrevOfstFin = SRVC_GetNvmValueUC(NVMID_LAST_OFFSET_POSITION);

			if( (aPrevOfstFin != SHT_OFST_FIN_LEFT) && (aPrevOfstFin != SHT_OFST_FIN_RIGHT) ) {
				aPrevOfstFin = SHT_OFST_FIN_LEFT;
			}

		}

		if( SRVC_GetSetTop(aSheetNo) == SHT_SET_TOP ) {
		
			if( SRVC_GetOffsetMode(aSheetNo) == eCdiFinOffset_Off ) {
				switch( SRVC_GetOffsetPosition(aSheetNo) ) {
				case eCdiFinOffsetPos_Left:
					aOfstFin = SHT_OFST_FIN_LEFT;
					break;
				case eCdiFinOffsetPos_Right:
					aOfstFin = SHT_OFST_FIN_RIGHT;
					break;
				default:														// eCdiFinOffsetPos_Current
				
					if( aPrevOfstFin == SHT_OFST_FIN_LEFT ) {
						aOfstFin = SHT_OFST_FIN_LEFT;
					}
					else {														// SHT_OFST_FIN_REAR
						aOfstFin = SHT_OFST_FIN_RIGHT;
					}
					break;
				}
			}
			else {
				switch( SRVC_GetOffsetPosition(aSheetNo) ) {
				case eCdiFinOffsetPos_Left:
					aOfstFin = SHT_OFST_FIN_RIGHT;
					break;
				case eCdiFinOffsetPos_Right:
					aOfstFin = SHT_OFST_FIN_LEFT;
					break;
				default:														// eCdiFinOffsetPos_Current
					if( aPrevOfstFin == SHT_OFST_FIN_LEFT ) {
						aOfstFin = SHT_OFST_FIN_RIGHT;
					}
					else {														// SHT_OFST_FIN_REAR
						aOfstFin = SHT_OFST_FIN_LEFT;
					}
					break;
				}
			}
			gCMPL_Manager.lastOffset = aOfstFin;  //(TB-)save the set-top offset info..
		}
		else {																	// SHT_SET_TOP_NOT
			aOfstFin = aPrevOfstFin;
		}
	}
	else {																		// != eCdiFinOutDest_OutTray (関与しないモジュールが指定)
		aOfstFin = SHT_OFST_FIN_NOT;
	}

	return aOfstFin;
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetCompilerSheetInitializeTimeStep1
// Description   : Sheet Initialize 動作時間
// Parameter     : aSheetNo(対象シートのSheetNo)
// Return        :  aTotalTime 
//History	 : V3.5.93 Job初期化開始条件修正
///////////////////////////////////////////////////////////////////
US CMPL_GetCompilerSheetInitializeTimeStep1(UC sheetNo, UC lastShtNo)
{
	US aTotalTime = 0;
	US  aPara1, aPara2, aPara3;
	US  aEjectHomeTime=0;

	gCMPL_Manager.sheetInitAction[sheetNo] = (US)0x0000;

	if( lastShtNo == SHEETNO_NUM) {	// セット残りが無いプロセススタート と シートイニシャライズ動作実施後のキャンセルケースをケア
	
		//Tamper SizePosition Times
		aPara1 = CMPL_TMP_GetSizePositioningTime(sheetNo, lastShtNo); 

		// SubPaddle HomePosition Time
		aPara2 = 229; //(V3.6.0)

		// Stacker(1621ms) > Eject Home(1147ms) のため、Stacker動作時間を使う。
		aPara3 = 1621;
		aEjectHomeTime = CMPL_GetEjectHomeNeedTime(sheetNo);

		if ( aPara3 < aEjectHomeTime){
			aPara3 = aEjectHomeTime;
		}

		aTotalTime = aPara1 + aPara2 + aPara3 ;

		gCMPL_Manager.sheetInitAction[sheetNo] |= GET_BIT_PATTERN(CMPL_MDL_ACT_EJECTOR_HOME);
		gCMPL_Manager.sheetInitAction[sheetNo] |= GET_BIT_PATTERN(CMPL_MDL_ACT_TAMP_SIZE_POS);
		gCMPL_Manager.sheetInitAction[sheetNo] |= GET_BIT_PATTERN(CMPL_MDL_ACT_PADDLE_HOME);	
		
	} else if ( (SRVC_GetSetTop(sheetNo) == SHT_SET_TOP) &&				// "制御対象用紙がセット先頭用紙であり、以下のいずれかの条件を満たしている、
			 ((SRVC_GetCrossProcessSize(sheetNo) != SRVC_GetCrossProcessSize(lastShtNo)) ||	//    →制御対象用紙の用紙幅と先行紙の用紙幅が異なる
	 		  (SRVC_GetStapleMode(sheetNo) != SRVC_GetStapleMode(lastShtNo)) ||	//    →制御対象用紙のステープル指定と先行紙のステープル指定が異なる
	 		  (SRVC_GetCrossProcessSizeMixSet(lastShtNo) == SHT_SIZE_MIX)) ){		//    →先行紙のセットにおいてセット内異幅が発生している 	
		
		//Tamper SizePosition Times
		aPara1 = CMPL_TMP_GetSizePositioningTime(sheetNo, lastShtNo); 

		aTotalTime = aPara1;
		
		gCMPL_Manager.sheetInitAction[sheetNo] |= GET_BIT_PATTERN(CMPL_MDL_ACT_TAMP_SIZE_POS);	
	}
	return aTotalTime;
}

///////////////////////////////////////////////////////////////////V3.5.93
// Function Name : CMPL_IsSheetInitExec
// Description   : 各動作におけるシートイニシャライズ動作の実施判断
// Parameter     : sheetNo (対象シートのシートナンバー)
// Return        : true …シートイニシャライズ動作を実施する
//				 : fasel…シートイニシャライズ動作を実施しない
///////////////////////////////////////////////////////////////////
bool CMPL_IsSheetInitExec(UC aSheetNo, ECMPL_ModuleAction aAction)
{
	US aInitAction;

	
	if( (aSheetNo < SHEETNO_NUM) && (aAction < CMPL_MDL_ACT_NUM) ) {
		aInitAction = gCMPL_Manager.sheetInitAction[aSheetNo];

		if( (aInitAction & GET_BIT_PATTERN(aAction)) != 0 ) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;															// 引数の値異常
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetEjectHomeNeedTime
// Description   : 
// Return        :  イジェクト Home動作時間[ms]
///////////////////////////////////////////////////////////////////
US  CMPL_GetEjectHomeNeedTime(UC  sheetNo)
{
	return   (US)1147;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetEjectNeedTime
// Description   : 
// Return        :  イジェクト動作時間[ms]
///////////////////////////////////////////////////////////////////
US  CMPL_GetEjectNeedTime(UC  sheetNo)
{
	return   (US)1167;
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetBase_CompileFinishingNeedTime
// Description   : SetPitch  基本時間[ms]
// Parameter     : sheetNo (制御対象のシートナンバー)
// Return        :  a + b 
// a	：Compile Exit Sensor OFFからタンピング動作開始までの時間[ms]
// b	イジェクト動作時間[ms]
// c : Compile ExitSnr On~ Off
// d : 
///////////////////////////////////////////////////////////////////
US CMPL_GetBase_CompileFinishingNeedTime(UC sheetNo)
{
	US	paraA  ;  //  a	：Compile Exit Sensor OFFからタンピング動作開始までの時間[ms]
	US    paraB  ; //  b	：イジェクト動作時間[ms]
	US    ret;
	
	paraA = CMPL_TMP_GetEjectTampingDelayTime(sheetNo);
	paraB =   CMPL_GetEjectNeedTime(sheetNo); //1167; //1147 ;  (V3.6.2)
	ret = paraA + paraB;
	
	return  ret;	
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetAdd_CompileFinishingNeedTime
// Description   :  SetPitch 加算時間　(CompilerFinishing動作時間)
// Parameter     : sheetNo (制御対象のシートナンバー)
// Return        :  A 
//A[ms] ※小数点以下は切り捨て
//Staple Mode Off  :  A ＝ a ＋ c ＋ ｄ
//Single Front       :  A＝ a ＋ b ＋ c ＋ ｄ
//
//算出式で使用している各パラメータの詳細は以下の通りである。
//なお、各動作の時間はその動作の実施条件を満たす場合のみ加算する。
//
//a	：タンピング動作(セットラストタンピング)時間[ms] (注:励磁時間含む)
//b 	：ステープル動作時間(ステープルを打つ時間)[ms]
//c	：タンパーオフセット動作時間[ms]
//d	：サブパドル用紙搬入 時の動作回数による動作時間[ms]
///////////////////////////////////////////////////////////////////
US CMPL_GetAdd_CompileFinishingNeedTime(UC sheetNo)
{
        US	paraA  ;  //a	：タンピング動作(セットラストタンピング)時間[ms] (注:励磁時間含む)
	US    paraB  ;//b 	：ステープル動作時間(ステープルを打つ時間)[ms]
	US    paraC  ;//c	：タンパーオフセット動作時間[ms]
	US    paraD  ;//d	：サブパドル用紙搬入 時の動作回数による動作時間[ms]	
	US    ret;
	
	paraA =  CMPL_TMP_GetEjectTampingActionTime(sheetNo);
//    
	if ( SRVC_GetStapleMode(sheetNo) == eCdiFinStaple_Off) {
		paraB = 0;  
	}else { 
		paraB = 400;  
	}
	paraC = 180;
	paraD = CMPL_GetAdd_SubPaddleNeedTime(sheetNo);
	ret = paraA + paraB + paraC + paraD;
	
	return  ret;	
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPrepTime_Add_TamperNigasi
// Description   :  加算時間　(Tamper 逃がし動作時間)
// Parameter     : pThis (インスタンスポインタ)
//               : sheetNo (対象シートのシート識別子)
//               : prevShtNo (先行シートのシート識別子)
//
// 加算時間 :  タンパー逃がし動作必要時間
// 加算時間[ms] ＝ a ＋ b  
//a ：Compile Exit Sensor OFFからタンピング動作開始までの時間（用紙収容待ち時間）[ms] 
//b ：タンピング動作(ノーマルタンピング)時間[ms] (注:励磁時間含む) 
///////////////////////////////////////////////////////////////////////
US  CMPL_GetAdd_TamperOpenNeedTime(UC  sheetNo)
{
	US   ret;
	US   paraA;
	US   paraB;
		
	if ( CMPL_GetActionModeTamperOpen(sheetNo)) {  //タンパーオープン動作を実施する
		paraA = CMPL_TMP_GetNormalTampingDelayTime(sheetNo);
		paraB = CMPL_TMP_GetNormalTampingActionTime(sheetNo);
		ret = paraA + paraB;
	} else {
		ret=0;
	}
	return  ret;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPrepTime_Add_SubPaddleTimes
// Description   :  加算時間　(SubPaddle 3回以上の場合, 動作時間)
// Parameter     : pThis (インスタンスポインタ)
//               : sheetNo (対象シートのシート識別子)
//               : prevShtNo (先行シートのシート識別子)
// 加算時間 :  サブパドル用紙搬入 時の動作回数が3回の場合の加算時間     
//
//加算時間＝(P回-2)×tms
//ｔms＝360 ×各サブパドル用紙搬入 プロファイルの30STEP時の実行時間÷1.722
//※13.767はT1 PULSE時の回転角度
//     但し、計算結果がマイナスであれば0msにする
///////////////////////////////////////////////////////////////////////
US  CMPL_GetAdd_SubPaddleNeedTime(UC sheetNo)
{
	UC    paddleCnt; 
	US    aTotalTime, aTemp;
	
	paddleCnt =  CMPL_EJCT_GetSubPaddleActionCount(sheetNo);

	if ( paddleCnt >= 3 )  { // 
		aTemp = (US)paddleCnt;   //
		aTotalTime = (US)((aTemp - 2) * 150);  // (P回 - 2) * tms ..

	} else {
		aTotalTime = 0;
	}
	
//	PutCMPLManagerLog(0x56,0x56, paddleCnt,aTotalTime);
	return aTotalTime;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetActionModeTamperOpen
// Description   : タンパーオープン動作の実施有無を取得
// Parameter     : sheetNo (対象シートのシートナンバー)
// Return        : タンパーオープン動作の実施有無
//				 : 範囲：0～1
//				 : 意味：0…タンパーオープン動作を実施しない
//				 : 		 1…タンパーオープン動作を実施する
///////////////////////////////////////////////////////////////////
UC CMPL_GetActionModeTamperOpen(UC sheetNo)
{
	if( CMPL_TMP_IsOpenExec(sheetNo) == true ) {
		return 1;
	}
	else {
		return 0;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_DownCompCheck
// Description   : フィニッシングプロセス処理完了チェック
// Parameter     : ‐
// Return        : true：完了、false：未了
///////////////////////////////////////////////////////////////////
bool CMPL_DownCompCheck()
{
	UC aIndex;

	if( (gCMPL_Manager.state == CMPL_STT_CYCLE_DOWN) || (gCMPL_Manager.state == CMPL_STT_HARD_DOWN) ) {
		
		for( aIndex = 0; aIndex < CMPL_SHTMNGNUM; aIndex++ ) {
			if( gCMPL_Manager.seqMngInfo[aIndex].sheetNo != SHEETNO_NUM ) {
				break;
			}
		}

		if( (aIndex >= CMPL_SHTMNGNUM) &&
			(CMPL_EJCT_IsExecutingModuleAction() == false) &&
			(CMPL_STPL_IsExecutingModuleAction() == false) &&
			(CMPL_TMP_IsExecutingModuleAction() == false) ) {
			if( (gCMPL_Manager.finishingWaitSheet == SHEETNO_NUM) &&
			    ( SRVC_CheckUndeliveredSheetExist(eCdiFinOutDest_OutTray) == false ) ){

				PutCMPLManagerLog(0x26, gCMPL_Manager.finishingWaitSheet , 
				 (US)aIndex, (US)SRVC_CheckUndeliveredSheetExist(eCdiFinOutDest_OutTray) );
				return true;
			}
			else {
				/* No Action */
				PutCMPLManagerLog(0x24, gCMPL_Manager.finishingWaitSheet , 
				 (US)aIndex, (US)SRVC_CheckUndeliveredSheetExist(eCdiFinOutDest_OutTray) );
			}
		}
		else {
			PutCMPLManagerLog(0x23, (UC)CMPL_EJCT_IsExecutingModuleAction() , (US)aIndex, (US)gCMPL_Manager.finishingWaitSheet);
			/* No Action */
		}
	}
	else {
		/* No Action */		
	}

	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_CheckEjectPossible
// Description   : 用紙排出ができるかの判断
// Parameter     : ‐
// Return        : true：排出可、false：排出不可
///////////////////////////////////////////////////////////////////
bool CMPL_CheckEjectPossible()
{
	if( (SRVC_GetCoverStatus() == CVR_STTS_CLOSE) &&
		(SRVC_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_CLEAR) ) {
		return true;
	}
	else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ClearSequenceManageInfo
// Description   : Sequence Manage Infoクリア
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ClearSequenceManageInfo(UC aIndex)
{
	gCMPL_Manager.seqMngInfo[aIndex].sequenceId = CMPL_SEQ_ID_NUM;
	gCMPL_Manager.seqMngInfo[aIndex].actionLog  = 0x0000;
	gCMPL_Manager.seqMngInfo[aIndex].sheetNo    = SHEETNO_NUM;	
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_PowerOnInit
// Description   : Power On Initialize動作(開始/停止)
// Parameter     : aAction (動作の開始/停止)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_PowerOnInit(EAction aAction)
{
	if( aAction == ACT_START )	CMPL_StartPowerOnInit();
	else					 	CMPL_StopPowerOnInit();							// Stop
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_StartPowerOnInit
// Description   : Power On Initialize動作開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_StartPowerOnInit()
{
	if( gCMPL_Manager.seqMngInfo[0].sequenceId == CMPL_SEQ_ID_NUM ) {
		gCMPL_Manager.seqMngInfo[0].sequenceId = CMPL_SEQ_ID_POWER_ON_INIT;
		gCMPL_Manager.seqMngInfo[0].actionLog = 0x0000;
		gCMPL_Manager.seqMngInfo[0].sheetNo = SHEETNO_NUM;	
		
		gCMPL_Manager.lastStaSeqIndex = 0;
		
		if( SRVC_GetDeviceCondition(DEVID_FINISHER) != DEV_COND_PERM_NG ) {
			CMPL_ExecuteSequence(CMPL_MDL_ACT_START, 0);
		}
		else {
			CMPL_CompletePowerOnInit();
		}
	}
	else {
		/* Irregular Case */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_CompletePowerOnInit
// Description   : Power On Initialize動作完了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_CompletePowerOnInit()
{
	if( gCMPL_Manager.seqMngInfo[0].sequenceId == CMPL_SEQ_ID_POWER_ON_INIT ) {
		CMPL_ClearSequenceManageInfo(0);
		MSP_SendMessage(MID_CMPL, EVT_DEV_RES_POWER_ON_INIT_CMPL, 0, 0, LOG_OFF);
	}
	else {
		/* Irregular Case */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_StopPowerOnInit
// Description   : Power On Initialize動作停止
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_StopPowerOnInit()
{
	if( gCMPL_Manager.seqMngInfo[0].sequenceId == CMPL_SEQ_ID_POWER_ON_INIT ) {
		CMPL_EJCT_StopModuleAction();
		CMPL_STPL_StopModuleAction();
		CMPL_TMP_StopModuleAction();

		CMPL_CompletePowerOnInit();
	}
	else {
		/* Irregular Case */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ResumeInit
// Description   : Resume Initialize動作(開始/停止)
// Parameter     : aAction (動作の開始/停止)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ResumeInit(EAction aAction)
{
	if( aAction == ACT_START )	CMPL_StartResumeInit();
	else					 	CMPL_StopResumeInit();							// Stop
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_StartPowerResumeInit
// Description   : Resume Initialize動作開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_StartResumeInit()
{
	if( gCMPL_Manager.seqMngInfo[0].sequenceId == CMPL_SEQ_ID_NUM ) {
		gCMPL_Manager.seqMngInfo[0].sequenceId = CMPL_SEQ_ID_RESUME_INIT;
		gCMPL_Manager.seqMngInfo[0].actionLog = 0x0000;
		gCMPL_Manager.seqMngInfo[0].sheetNo = SHEETNO_NUM;
		
		gCMPL_Manager.lastStaSeqIndex = 0;
		
		if( SRVC_GetDeviceCondition(DEVID_FINISHER) != DEV_COND_PERM_NG ) {
			CMPL_ExecuteSequence(CMPL_MDL_ACT_START, 0);
		}
		else {
			CMPL_CompleteResumeInit();
		}
	}
	else {
		/* Irregular Case */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_CompleteResumeInit
// Description   : Resume Initialize動作完了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_CompleteResumeInit()
{
	if( gCMPL_Manager.seqMngInfo[0].sequenceId == CMPL_SEQ_ID_RESUME_INIT ) {
		CMPL_ClearSequenceManageInfo(0);
		MSP_SendMessage(MID_CMPL, EVT_DEV_RES_RESUME_INIT_CMPL, 0, 0, LOG_OFF);
	}
	else {
		/* Irregular Case */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_StopResumeInit
// Description   : Resume Initialize動作停止
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_StopResumeInit()
{
	if( gCMPL_Manager.seqMngInfo[0].sequenceId == CMPL_SEQ_ID_RESUME_INIT ) {
		CMPL_EJCT_StopModuleAction();
		CMPL_STPL_StopModuleAction();
		CMPL_TMP_StopModuleAction();

		CMPL_CompleteResumeInit();
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_NormalInit
// Description   : ノーマルイニシャライズ動作(開始/停止)
// Parameter     : aAction (開始/停止)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_NormalInit(EAction aAction)
{
	if( aAction == ACT_START )	CMPL_StartNormalInit();
	else						CMPL_StopNormalInit();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_StartNormalInit
// Description   : ノーマルイニシャライズ動作開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_StartNormalInit()
{
	if( gCMPL_Manager.seqMngInfo[0].sequenceId == CMPL_SEQ_ID_NUM ) {
		gCMPL_Manager.seqMngInfo[0].sequenceId = CMPL_SEQ_ID_NORMAL_INIT;
		gCMPL_Manager.seqMngInfo[0].actionLog = 0x0000;
		gCMPL_Manager.seqMngInfo[0].sheetNo = SHEETNO_NUM;

		gCMPL_Manager.lastStaSeqIndex = 0;

		if( SRVC_GetDeviceCondition(DEVID_FINISHER) != DEV_COND_PERM_NG ) {
			CMPL_ExecuteSequence(CMPL_MDL_ACT_START, 0);
		}
		else {
			CMPL_CompleteNormalInit();
		}
	}
	else {
		/* Irregular Case */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_CompleteNormalInit
// Description   : ノーマルイニシャライズ動作完了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_CompleteNormalInit()
{
	if( gCMPL_Manager.seqMngInfo[0].sequenceId == CMPL_SEQ_ID_NORMAL_INIT ) {
		CMPL_ClearSequenceManageInfo(0);

		MSP_SendMessage(MID_CMPL, EVT_DEV_RES_NORMAL_INIT_CMPL, 0, 0, LOG_OFF);
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_StopNormalInit
// Description   : ノーマルイニシャライズ動作停止
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_StopNormalInit()
{
	if( gCMPL_Manager.seqMngInfo[0].sequenceId == CMPL_SEQ_ID_NORMAL_INIT ) {
		CMPL_EJCT_StopModuleAction();
		CMPL_STPL_StopModuleAction();
		CMPL_TMP_StopModuleAction();

		CMPL_CompleteNormalInit();
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_SheetInitialize
// Description   : シートイニシャライズ動作(開始/停止)
// Parameter     : aSheetNo(指定Sheet No.)
//				 : aAction(動作指示(開始/停止))
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_SheetInitialize(UC aSheetNo, EAction aAction)
{
	if( aAction == ACT_START ) {
		CMPL_StartSheetInitialize(aSheetNo);
	}
	else {																		// ACT_STOP
		CMPL_StopSheetInitialize(aSheetNo);

 		if( CMPL_DownCompCheck() == true ) {
			CMPL_ChangeProcDownEvent();
		}
		else {
			/* No Action */
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_StartSheetInitialize
// Description   : シートイニシャライズ動作開始
// Parameter     : aSheetNo(指定Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_StartSheetInitialize(UC aSheetNo)
{
	UC aIndex;

	aIndex = (UC)((gCMPL_Manager.lastStaSeqIndex + 1) & (CMPL_SHTMNGNUM - 1));

	if( gCMPL_Manager.seqMngInfo[aIndex].sequenceId == CMPL_SEQ_ID_NUM ) {
		gCMPL_Manager.seqMngInfo[aIndex].sequenceId = CMPL_SEQ_ID_SHEET_INIT;
		gCMPL_Manager.seqMngInfo[aIndex].actionLog = 0x0000;
		gCMPL_Manager.seqMngInfo[aIndex].sheetNo = aSheetNo;

		gCMPL_Manager.lastStaSeqIndex = aIndex;
		gCMPL_Manager.finishingWaitSheet = SHEETNO_NUM;
		
		if( (SRVC_CheckSheetConditionNormal(aSheetNo) == true) &&
			(SRVC_GetDeviceCondition(DEVID_FINISHER) != DEV_COND_PERM_NG) ) {
			
			CMPL_ExecuteSequence(CMPL_MDL_ACT_START, aIndex);
		}
		else {
			CMPL_CompleteSheetInitialize(aIndex);
		}
	}
	else {
		/* Irregular Case */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_CompleteSheetInitialize
// Description   : シートイニシャライズ動作完了
// Parameter     : aIndex(シーケンス管理番号)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_CompleteSheetInitialize(UC aIndex)
{
	EvtParamSheetActionResponse aMessage;
	UC aSheetNo;

	if( gCMPL_Manager.seqMngInfo[aIndex].sequenceId == CMPL_SEQ_ID_SHEET_INIT ) {
		aSheetNo = gCMPL_Manager.seqMngInfo[aIndex].sheetNo;

		CMPL_ClearSequenceManageInfo(aIndex);

		aMessage.sheetNo = aSheetNo;
		MSP_SendMessage(MID_CMPL, EVT_SHT_RES_COMPILER_SHT_INIT_CMPL, (UC*)(&aMessage), sizeof(aMessage), LOG_OFF);
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_StopSheetInitialize
// Description   : シートイニシャライズ動作停止
// Parameter     : aIndex(シーケンス管理番号)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_StopSheetInitialize(UC aSheetNo)
{
	UC aIndex;

	for( aIndex = 0; aIndex < CMPL_SHTMNGNUM; aIndex++ ) {
		if( gCMPL_Manager.seqMngInfo[aIndex].sheetNo == aSheetNo ) {
			if( gCMPL_Manager.seqMngInfo[aIndex].sequenceId == CMPL_SEQ_ID_SHEET_INIT ) {
				CMPL_EJCT_StopSheet(aSheetNo);
				CMPL_STPL_StopSheet(aSheetNo);
				CMPL_TMP_StopSheet(aSheetNo);

				CMPL_CompleteSheetInitialize(aIndex);
			}
			else {
				/* No Action */
			}
		}
		else {
			/* No Action */
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ProcessEnd
// Description   : プロセスエンド動作(開始/停止)
// Parameter     : aAction(動作指示(開始/停止))
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ProcessEnd(EAction aAction)
{
	if( aAction == ACT_START )	CMPL_StartProcessEnd();
	else						CMPL_StopProcessEnd();							// ACT_STOP
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_StartProcessEnd
// Description   : プロセスエンド動作開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_StartProcessEnd()
{

       if( gCMPL_Manager.seqMngInfo[0].sequenceId == CMPL_SEQ_ID_NUM ) {

		gCMPL_Manager.seqMngInfo[0].sequenceId = CMPL_SEQ_ID_PROC_END;
		gCMPL_Manager.seqMngInfo[0].actionLog = 0x0000;
		gCMPL_Manager.seqMngInfo[0].sheetNo = SHEETNO_NUM;

		gCMPL_Manager.lastStaSeqIndex = 0;

		//V3.1.7  StapleCoverILK Open時は、動作しない
		if (SRVC_GetCoverStatus_ID(CVR_ID_FIN_STAPLE_ILK) == CVR_STTS_OPEN) {
			CMPL_CompleteProcessEnd();
		} else { 
			CMPL_ExecuteSequence(CMPL_MDL_ACT_START, 0);
		}
		SRVC_UserDebugLog(0x50, 0x50);
	}
	else {
		/* Irregular Case */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_CompleteProcessEnd
// Description   : プロセスエンド動作完了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_CompleteProcessEnd()
{
			
	if( gCMPL_Manager.seqMngInfo[0].sequenceId == CMPL_SEQ_ID_PROC_END ) {

		SRVC_UserDebugLog(0x51, 0x51);
		CMPL_ClearSequenceManageInfo(0);
		MSP_SendMessage(MID_CMPL, EVT_DEV_RES_PROCESS_END_CMPL, 0, 0, LOG_OFF);
		
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_StopProcessEnd
// Description   : プロセスエンド動作停止
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_StopProcessEnd()
{
	if( gCMPL_Manager.seqMngInfo[0].sequenceId == CMPL_SEQ_ID_PROC_END ) {
		CMPL_EJCT_StopModuleAction();
		CMPL_STPL_StopModuleAction();
		CMPL_TMP_StopModuleAction();

		CMPL_CompleteProcessEnd();
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_JamClearAssist
// Description   : ジャムクリア補助イニシャライズ動作(開始/停止)
// Parameter     : aAction(動作指示(開始/停止))
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_JamClearAssist(EAction aAction)
{
	if( aAction == ACT_START )	CMPL_StartJamClearAssist();
	else					 	CMPL_StopJamClearAssist();						// ACT_STOP
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_StartJamClearAssist
// Description   : ジャムクリア補助イニシャライズ動作開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_StartJamClearAssist()
{
	if( gCMPL_Manager.seqMngInfo[0].sequenceId == CMPL_SEQ_ID_NUM ) {
		gCMPL_Manager.seqMngInfo[0].sequenceId = CMPL_SEQ_ID_JAM_CLEAR_ASSIST;
		gCMPL_Manager.seqMngInfo[0].actionLog = 0x0000;
		gCMPL_Manager.seqMngInfo[0].sheetNo = SHEETNO_NUM;

		gCMPL_Manager.lastStaSeqIndex = 0;

		CMPL_ExecuteSequence(CMPL_MDL_ACT_START, 0);
	}
	else {
		/* Irregular Case */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_CompleteJamClearAssist
// Description   : ジャムクリア補助イニシャライズ動作完了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_CompleteJamClearAssist()
{
	if( gCMPL_Manager.seqMngInfo[0].sequenceId == CMPL_SEQ_ID_JAM_CLEAR_ASSIST ) {
		CMPL_ClearSequenceManageInfo(0);

		MSP_SendMessage(MID_CMPL, EVT_DEV_RES_JAM_CLEAR_ASSIST_CMPL, 0, 0, LOG_OFF);
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_StopJamClearAssist
// Description   : ジャムクリア補助イニシャライズ動作停止
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_StopJamClearAssist()
{
	if( gCMPL_Manager.seqMngInfo[0].sequenceId == CMPL_SEQ_ID_JAM_CLEAR_ASSIST ) {
		CMPL_EJCT_StopModuleAction();
		CMPL_STPL_StopModuleAction();
		CMPL_TMP_StopModuleAction();

		CMPL_CompleteJamClearAssist();
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_CompilerAction
// Description   : コンパイラアクション動作(開始/停止)
// Parameter     : aSheetNo(指定Sheet No.)
//				 : aAction(動作指示(開始/停止))
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_CompilerAction(UC aSheetNo, EAction aAction)
{
	if( aAction == ACT_START ) {
	CMPL_StartCompilerAction(aSheetNo);
	//CMPL_TMP_NormalTamping(aSheetNo);
	}
	else {																		// ACT_STOP
	 	CMPL_StopCompilerAction(aSheetNo);
 
 		if( CMPL_DownCompCheck() == true ) {
			CMPL_ChangeProcDownEvent();
		}
		else {
			/* No Action */
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_StartCompilerAction
// Description   : コンパイラアクション動作開始
// Parameter     : aSheetNo(指定Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_StartCompilerAction(UC aSheetNo)
{
	UC aIndex;
	EShtEject aEject = SRVC_GetEject(aSheetNo);
	ECMPL_SequenceId aSeqId;
	EvtParamSheetEventNotify aMessage;

	if( gCMPL_Manager.state == CMPL_STT_STANDBY ) {
		aIndex = 0;
		aSeqId = CMPL_SEQ_ID_STANDBY_EJECT_FINISHING;
	}
	else {
		aIndex = (UC)((gCMPL_Manager.lastStaSeqIndex + 1) & (CMPL_SHTMNGNUM - 1));
		if( aEject == SHT_EJECT_EXEC ) {
			aSeqId = CMPL_SEQ_ID_EJECT_FINISHING;
		}
		else {
			aSeqId = CMPL_SEQ_ID_COMPILE_FINISHING;
		}
	}

	if( SRVC_CheckSheetConditionNormal(aSheetNo) == true ) {						// Leave用紙が排紙対象となった場合は、ConditionがRemainに書き換わるため、
		if( gCMPL_Manager.seqMngInfo[aIndex].sequenceId == CMPL_SEQ_ID_NUM ) {	// ConditionNormalのチェックでOKとしている
			if( aEject == SHT_EJECT_EXEC ) {
				gCMPL_Manager.compileSheet = SHEETNO_NUM;

				aMessage.sheetNo = aSheetNo;
				MSP_SendMessage(MID_CMPL, EVT_SHT_EVT_OUT_TRAY_EJECT_FINISH_START, (UC*)(&aMessage), sizeof(aMessage), LOG_OFF);
			}

			gCMPL_Manager.seqMngInfo[aIndex].sequenceId = aSeqId;
			gCMPL_Manager.seqMngInfo[aIndex].actionLog = 0x0000;
			gCMPL_Manager.seqMngInfo[aIndex].sheetNo = aSheetNo;

			gCMPL_Manager.lastStaSeqIndex = aIndex;

			if( SRVC_GetDeviceCondition(DEVID_FINISHER) != DEV_COND_PERM_NG ) {
				if( aSeqId == CMPL_SEQ_ID_STANDBY_EJECT_FINISHING ) {
					CMPL_EJCT_StandbyEjectStartNotify();
					CMPL_STPL_StandbyEjectStartNotify();
					CMPL_TMP_StandbyEjectStartNotify();
				}
				CMPL_ExecuteSequence(CMPL_MDL_ACT_START, aIndex);
			}
			else {
				CMPL_CompleteCompilerAction(aIndex);
			}
		}
		else {
			/* Irregular Case */
		}
	}
	else {
		//V3.6.93  EjectFail -> CompileSNR OFF 時、Condition=NGとして処理されないため、Standbyに行かない不具合
		if( CMPL_DownCompCheck() == true ) {
			CMPL_ChangeProcDownEvent();
		}
		else {
			/* No Action */
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_CompleteCompilerAction
// Description   : コンパイラアクション動作完了
// Parameter     : aIndex(Sequence Index)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_CompleteCompilerAction(UC aIndex)
{
	UC aSheetNo;
	US aEventId;
	EvtParamSheetEventNotify aMessage;
	
	if( (gCMPL_Manager.seqMngInfo[aIndex].sequenceId == CMPL_SEQ_ID_COMPILE_FINISHING)	||
		(gCMPL_Manager.seqMngInfo[aIndex].sequenceId == CMPL_SEQ_ID_EJECT_FINISHING) 	||
		(gCMPL_Manager.seqMngInfo[aIndex].sequenceId == CMPL_SEQ_ID_STANDBY_EJECT_FINISHING) ) {
		aSheetNo = gCMPL_Manager.seqMngInfo[aIndex].sheetNo;							// メッセージ送信用に保持

		if( gCMPL_Manager.seqMngInfo[aIndex].sequenceId == CMPL_SEQ_ID_COMPILE_FINISHING ) {
			gCMPL_Manager.compileSheet = gCMPL_Manager.seqMngInfo[aIndex].sheetNo;

			CMPL_ClearSequenceManageInfo(aIndex);

			aMessage.sheetNo = aSheetNo;
			MSP_SendMessage(MID_CMPL, EVT_SHT_EVT_OUT_TRAY_COMPILE_FINISH_COMP, (UC*)(&aMessage), sizeof(aMessage), LOG_OFF);

			if( SRVC_GetEject(aSheetNo) == SHT_EJECT_EXEC ) {	// コンパイルフィニッシング中に排出対象となったケース
				CMPL_StartCompilerAction(aSheetNo);
			}
		}
		else {																	// CMPL_SEQ_ID_EJECT_FINISHING, CMPL_SEQ_ID_STANDBY_EJECT_FINISHING
			if( gCMPL_Manager.seqMngInfo[aIndex].sequenceId == CMPL_SEQ_ID_STANDBY_EJECT_FINISHING ) {
				CMPL_EJCT_StandbyEjectCompNotify;									// StandbyEjectFinishing
				CMPL_STPL_StandbyEjectCompNotify;
				CMPL_TMP_StandbyEjectCompNotify();
			}

			//(TB- )SetTopのOffset情報がNVMに正しく書いていない
			//SRVC_SetNvmValueUC(CMPL_GetOffsetFinishPosition(gCMPL_Manager.seqMngInfo[aIndex].sheetNo), NVMID_LAST_OFFSET_POSITION);
			SRVC_SetNvmValueUC(gCMPL_Manager.lastOffset , NVMID_LAST_OFFSET_POSITION);
						
			CMPL_ClearSequenceManageInfo(aIndex);

			aMessage.sheetNo = aSheetNo;
			MSP_SendMessage(MID_CMPL, EVT_SHT_EVT_OUT_TRAY_EJECT_FINISH_COMP, (UC*)(&aMessage), sizeof(aMessage), LOG_OFF);
		}

	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_StopCompilerAction
// Description   : コンパイラアクション動作停止
// Parameter     : aIndex(Sequence Index)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_StopCompilerAction(UC aSheetNo)
{
	UC aIndex;

	for( aIndex = 0; aIndex < CMPL_SHTMNGNUM; aIndex++ ) {
		if( gCMPL_Manager.seqMngInfo[aIndex].sheetNo == aSheetNo ) {
			if( (gCMPL_Manager.seqMngInfo[aIndex].sequenceId == CMPL_SEQ_ID_COMPILE_FINISHING) ||
				(gCMPL_Manager.seqMngInfo[aIndex].sequenceId == CMPL_SEQ_ID_EJECT_FINISHING) ||
				(gCMPL_Manager.seqMngInfo[aIndex].sequenceId == CMPL_SEQ_ID_STANDBY_EJECT_FINISHING) ) {
				CMPL_EJCT_StopSheet(aSheetNo);
				CMPL_STPL_StopSheet(aSheetNo);
				CMPL_TMP_StopSheet(aSheetNo);

				CMPL_CompleteCompilerAction(aIndex);
			}
			else {
				/* No Action */
			}
		}
		else {
			/* No Action*/
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ExecuteSequence
// Description   : シーケンス制御実行
// Parameter     : aCompAct (動作完了した動作)
//				 : aSeqMngIdx (SequenceManageInfoのIndex)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ExecuteSequence(ECMPL_ModuleAction aCompAct, UC aSeqMngIdx)
{
	UL aNextAct;
	UC aActIdx, aSheetNo, aMdlActIdx, aMdlActRslt;
	const SCMPL_MdlActSeq* apMdlActSeq;

	apMdlActSeq = cCMPL_ModuleActionSequence[gCMPL_Manager.seqMngInfo[aSeqMngIdx].sequenceId];
	PutCMPLManagerLog(0x30,  aSeqMngIdx, 
		(US)gCMPL_Manager.seqMngInfo[aSeqMngIdx].sequenceId, (US)gCMPL_Manager.seqMngInfo[aSeqMngIdx].sheetNo );

	for( aActIdx = 0; apMdlActSeq[aActIdx].actID != CMPL_MDL_ACT_END; aActIdx++) {

		if( apMdlActSeq[aActIdx].actID == aCompAct ) {
			gCMPL_Manager.seqMngInfo[aSeqMngIdx].actionLog |= GET_BIT_PATTERN(aCompAct);

			if( (apMdlActSeq[aActIdx].condBit & gCMPL_Manager.seqMngInfo[aSeqMngIdx].actionLog) == apMdlActSeq[aActIdx].condBit ) {
				if( apMdlActSeq[aActIdx].nextBit != CMPLMDLACTBIT_END ) {
					aSheetNo = gCMPL_Manager.seqMngInfo[aSeqMngIdx].sheetNo;
					aNextAct = apMdlActSeq[aActIdx].nextBit;

					for( aMdlActIdx = 0; aMdlActIdx < CMPL_MDL_ACT_NUM; aMdlActIdx++ ) {
						if( aNextAct != 0) {
							if( (aNextAct & 1) != 0 ) {
								switch( aMdlActIdx ) {
								//case CMPL_MDL_ACT_EXIT_ROLL_RELEASE_WAIT:	CMPL_ExitRollReleaseWait(aSheetNo);	break;
								case CMPL_MDL_ACT_CURRENT_OFF:				CMPL_CurrentOFF(aSheetNo);			break;
								case CMPL_MDL_ACT_TAMP_MISS_REGI:			CMPL_TMP_MissRegi(aSheetNo);		break;
								case CMPL_MDL_ACT_STAPLE_HEAD_HOME:		CMPL_STPL_StapleHeadHome(aSheetNo);	break;
								case CMPL_MDL_ACT_EJECTOR_HOME:			CMPL_EJCT_EjectorHome(aSheetNo);	break;
								case CMPL_MDL_ACT_STAPLE_HARIOUT:			CMPL_STPL_StapleHeadHariout(aSheetNo); break;
								case CMPL_MDL_ACT_PADDLE_HOME:                  	CMPL_EJCT_PaddleHome(aSheetNo);	break;
								case CMPL_MDL_ACT_PADDLE_COMPILE:			CMPL_EJCT_PaddleCompile(aSheetNo);	break;
								case CMPL_MDL_ACT_TAMP_SIZE_POS:			CMPL_TMP_SizePositioning(aSheetNo);	break;
								case CMPL_MDL_ACT_NORMAL_TAMPING:			CMPL_TMP_NormalTamping(aSheetNo);	break;
								case CMPL_MDL_ACT_EJECT_TAMPING:			CMPL_TMP_EjectTamping(aSheetNo);	break;
								case CMPL_MDL_ACT_STAPLE:					CMPL_STPL_Staple(aSheetNo);			break;
								case CMPL_MDL_ACT_OFFSET:					CMPL_TMP_Offset(aSheetNo);			break;
								case CMPL_MDL_ACT_RETURN:					CMPL_TMP_Return(aSheetNo);			break;
								case CMPL_MDL_ACT_EJECT:					CMPL_EJCT_Eject(aSheetNo);			break;
								default:																			break;
								}
							}
							else { /* Go for loop */ }

							aNextAct >>= 1;
						}
						else {
							break;												// Next action none
						}
					}
				}
				else {

					PutCMPLManagerLog(0x31,  aSeqMngIdx, 
							(US)gCMPL_Manager.seqMngInfo[aSeqMngIdx].sequenceId, (US)gCMPL_Manager.seqMngInfo[aSeqMngIdx].sheetNo );
						
					switch( gCMPL_Manager.seqMngInfo[aSeqMngIdx].sequenceId ) {
					case CMPL_SEQ_ID_POWER_ON_INIT:				CMPL_CompletePowerOnInit();					break;
					case CMPL_SEQ_ID_RESUME_INIT:				CMPL_CompleteResumeInit();					break;
					case CMPL_SEQ_ID_NORMAL_INIT:				CMPL_CompleteNormalInit();					break;
					case CMPL_SEQ_ID_SHEET_INIT:				CMPL_CompleteSheetInitialize(aSeqMngIdx);	break;
					case CMPL_SEQ_ID_COMPILE_FINISHING:			CMPL_CompleteCompilerAction(aSeqMngIdx);	break;
					case CMPL_SEQ_ID_EJECT_FINISHING:			CMPL_CompleteCompilerAction(aSeqMngIdx);	break;
					case CMPL_SEQ_ID_STANDBY_EJECT_FINISHING:	CMPL_CompleteCompilerAction(aSeqMngIdx);	break;
					case CMPL_SEQ_ID_PROC_END:					CMPL_CompleteProcessEnd();					break;
					case CMPL_SEQ_ID_JAM_CLEAR_ASSIST:			CMPL_CompleteJamClearAssist();				break;
					default:									/* Irregular Case */						break;
					}
				}
			}
			else { /* Wait another action complete */ }

			break;
		}
		else { /* No action */ }
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_InformedModuleActionComp
// Description   : Module Action Complete(Compiler Internal I/F)通知受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_InformedModuleActionComp(ECMPL_ModuleAction aAction, UC aSheetNo)
{
	UC aIndex = (UC)((gCMPL_Manager.lastStaSeqIndex + 1) & (CMPL_SHTMNGNUM - 1));
	UC aLoopCnt, aActIdx;
	const SCMPL_MdlActSeq* apMdlActSeq;

	if( aSheetNo != SHEETNO_NUM ) {
		for( aLoopCnt = 0; aLoopCnt < CMPL_SHTMNGNUM; aLoopCnt++ ) {
			if( (gCMPL_Manager.seqMngInfo[aIndex].sheetNo == aSheetNo) && (gCMPL_Manager.seqMngInfo[aIndex].sequenceId != CMPL_SEQ_ID_NUM) ) {
				apMdlActSeq = cCMPL_ModuleActionSequence[gCMPL_Manager.seqMngInfo[aIndex].sequenceId];

				for( aActIdx = 0; apMdlActSeq[aActIdx].actID != CMPL_MDL_ACT_END; aActIdx++ ) {
					if( (apMdlActSeq[aActIdx].actID == aAction) &&				// aActionがシーケンス内にあるか、すでに完了していないかをチェック
						((gCMPL_Manager.seqMngInfo[aIndex].actionLog & GET_BIT_PATTERN(aAction)) == 0) ) {
						aLoopCnt = CMPL_SHTMNGNUM;
						CMPL_ExecuteSequence(aAction, aIndex);

						break;
					}
					else { /* No Action */ }
				}
			}
			else { /* No Action */ }

			aIndex = (UC)((aIndex + 1) & (CMPL_SHTMNGNUM - 1));					// index更新
		}
	}
	else {
		if( gCMPL_Manager.seqMngInfo[0].sequenceId != CMPL_SEQ_ID_NUM ) {
			if( (gCMPL_Manager.seqMngInfo[0].actionLog & GET_BIT_PATTERN(aAction)) == 0 ) {
				CMPL_ExecuteSequence(aAction, 0);
			}
		}
	}

	if( CMPL_DownCompCheck() == true ) {
		CMPL_ChangeProcDownEvent();
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_InformedModuleEvent
// Description   : モジュールイベント通知
// Parameter     : aEvent (モジュールイベント)
//				 : aSheetNo (対象シートナンバー)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_InformedModuleEvent(ECMPL_ModuleEvent aEvent, UC aSheetNo)
{
	EvtParamSheetEventNotify aSheetEventMessage;
	 EvtParamSheetEventJamOccurrenceNotify aJamOccurenceMessage;

	switch( aEvent ) {
	case CMPL_EVT_EJECT_START:
		aSheetEventMessage.sheetNo = aSheetNo;
		CMPL_TMP_EjectStart(aSheetNo);
		MSP_SendMessage(MID_CMPL, EVT_SHT_EVT_OUT_TRAY_EJECT_START, (UC*)(&aSheetEventMessage), sizeof(aSheetEventMessage), LOG_OFF);
		break;

 	case CMPL_EVT_EJECT_COMP:
		aSheetEventMessage.sheetNo = aSheetNo;
		MSP_SendMessage(MID_CMPL, EVT_SHT_EVT_OUT_TRAY_EJECT_COMP, (UC*)(&aSheetEventMessage), sizeof(aSheetEventMessage), LOG_OFF);
		break;

	case CMPL_EVT_EJECT_SHEET_THROUGH:
		CMPL_TMP_EjectSheetThrough(aSheetNo);
		break;

	case CMPL_EVT_SET_EJECT_JAM_OCCUR:
		 aJamOccurenceMessage.statusNo = DSNO_SET_EJECT_JAM;
		 aJamOccurenceMessage.sheetNo = aSheetNo;

		 MSP_SendMessage(MID_CMPL, EVT_SHT_EVT_OUT_TRAY_JAM_OCCURRENCE, (UC*)(&aJamOccurenceMessage), sizeof(aJamOccurenceMessage), LOG_OFF);
		break;

	case  CMPL_EVT_STACKER_INIT_TRIGGER:  //Stacker Job Initialize start ..(GLOBAL_A4)
		aSheetEventMessage.sheetNo = aSheetNo;
		MSP_SendMessage(MID_CMPL, EVT_SHT_EVT_SHT_INIT_STACKER_JOBSTART, (UC*)(&aSheetEventMessage), sizeof(aSheetEventMessage), LOG_OFF);
		break;
		
	case  CMPL_EVT_STACKER_INIT_WAITING_POS_TRIGGER :
		aSheetEventMessage.sheetNo = aSheetNo;
		MSP_SendMessage(MID_CMPL, EVT_SHT_EVT_SHT_INIT_STACKER_WAITING_POS_START, (UC*)(&aSheetEventMessage), sizeof(aSheetEventMessage), LOG_OFF);
		break;
		
	case  CMPL_EVT_STACKER_JOB_TRIGGER1 :
		aSheetEventMessage.sheetNo = aSheetNo;
		MSP_SendMessage(MID_CMPL, EVT_SHT_EVT_TRAYJOB_TRIGGER_1, (UC*)(&aSheetEventMessage), sizeof(aSheetEventMessage), LOG_OFF);
		break;	
		
	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_InformedDeviceStatusChange
// Description   : Device Statusの変化を通知
// Parameter     : aStatusNo
//				 : aStatusValue
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_InformedDeviceStatusChange(EDeviceStatusNo aStatusNo, US aStatusValue)
{
	SRVC_UpdateDeviceStatus(aStatusValue, aStatusNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_CurrentOFF
// Description   : 励磁OFF動作 ： Tamper→Eject励磁OFFする
// Parameter     : aSheetno(対象シートナンバー)
// History :  V3.6.93 仕様変更 ProcessEnd Sequence.
///////////////////////////////////////////////////////////////////
void CMPL_CurrentOFF(UC aSheetNo)
{	
	//Tamper OFF
	CMPL_TMP_StandbyEjectCompNotify();

	//Eject OFF
	CMPL_EJCT_CurrentOFF();

	CMPL_InformedModuleActionComp(CMPL_MDL_ACT_CURRENT_OFF, aSheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EventTimerTimeUp
// Description   : イベントタイマのタイムアップ時処理
// Parameter     : aSheetno(対象シートナンバー)
// Return        : 
///////////////////////////////////////////////////////////////////
void CMPL_EventTimerTimeUp(SS aTimerID, UL aEvtData)
{
	US aSheetNo = (US)(aEvtData & 0x000000FF);
	US aCompAct = (US)((aEvtData & 0xFFFFFF00) >> 8);

	CMPL_InformedModuleActionComp((ECMPL_ModuleAction)aCompAct, (UC)aSheetNo);
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ComponentControl
// Description   : Component Control Stop Request指示受信処理
// Parameter     : evt (イベントメッセージ(Chain Link No.) )
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ComponentControl(const UC* evt, EAction aAction)
{
	UC aIndex;
	EvtParamComponentControlStartRequest *pRcvEvtParameter;
	EvtParamComponentControlStartNotify evtParameter;

	pRcvEvtParameter = (EvtParamComponentControlStartRequest *)evt;

	evtParameter.chainNo = pRcvEvtParameter->chainNo;
	evtParameter.linkNo = pRcvEvtParameter->linkNo;

	if ( evtParameter.chainNo == CMPL_COMPO_CHAIN) {
		for( aIndex = 0; aIndex < (sizeof(cCMPL_CompoContTbl) / sizeof(SCMPL_CompoCont)); aIndex++ ) {
			if( pRcvEvtParameter->linkNo == (US)(cCMPL_CompoContTbl[aIndex].link) ) {
				switch( cCMPL_CompoContTbl[aIndex].moduleID ) {
				case CMPL_MDL_ID_EJCT:
					CMPL_EJCT_ComponentControl(evtParameter.chainNo, evtParameter.linkNo, aAction);
					break;
				case CMPL_MDL_ID_STPL:
					CMPL_STPL_ComponentControl(evtParameter.chainNo, evtParameter.linkNo, aAction);
					break;
				case CMPL_MDL_ID_TMP:
					CMPL_TMP_ComponentControl(evtParameter.chainNo, evtParameter.linkNo, aAction);
					break;
				default:														// 未定義のLink No.
					/* No Action */
					break;
				}
			}
		}
	}
	else {																		// 未定義のChain No.
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_InformedCompoControlComplete
// Description   : Module Action Complete(Compiler Internal I/F)通知受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_InformedCompoControlComplete(US aChainNo, US aLinkNo, EAction aAction)
{
	EvtParamComponentControlStopNotify aEvtParameter;

	aEvtParameter.chainNo = aChainNo;
	aEvtParameter.linkNo = aLinkNo;
	
	if( aAction == ACT_START ) {
		MSP_SendMessage(MID_CMPL, EVT_DIAG_COMPO_CONT_START, (const UC*)&aEvtParameter, sizeof(aEvtParameter), LOG_OFF);
	}
	else {																		// ACT_STOP
		MSP_SendMessage(MID_CMPL, EVT_DIAG_COMPO_CONT_STOP, (const UC*)&aEvtParameter, sizeof(aEvtParameter), LOG_OFF);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_CompileExitSensorON
// Description   : Compile Exit Sensor ON検知処理
// Parameter     : aSheetNo(指定Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_CompileExitSensorON(UC aSheetNo)
{
	if (gCMPL_Manager.state != CMPL_STT_HARD_DOWN) {

		CMPL_TMP_Open(aSheetNo);
		if( SRVC_CheckSheetConditionNormal(aSheetNo) == true ){
			gCMPL_Manager.finishingWaitSheet = aSheetNo;
		}

	} else {

	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_CompileExitSensorOFF
// Description   : Compile Exit Sensor OFF検知処理
// Parameter     : aSheetNo(指定Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_CompileExitSensorOFF(UC aSheetNo)
{

	if ( gCMPL_Manager.state != CMPL_STT_HARD_DOWN) {

		CMPL_CompilerAction(aSheetNo, ACT_START);		
	
	} else { 
	
		CMPL_CompilerAction(aSheetNo, ACT_STOP);
	}
	gCMPL_Manager.finishingWaitSheet = SHEETNO_NUM;
}

void CMPL_CompileSheetEject()
{	
	if( (gCMPL_Manager.compileSheet != SHEETNO_NUM) && (SRVC_GetEject(gCMPL_Manager.compileSheet) == SHT_EJECT_EXEC) ) {
		if( gCMPL_Manager.ejectPossible == true ) {

			SRVC_UserDebugLog(0x55, gCMPL_Manager.compileSheet);				
			CMPL_StartCompilerAction(gCMPL_Manager.compileSheet);
		}
		else {
			/* No Action */
		}
	}
	else {
		/* No Action */
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_SheetAttributeUpdate
// Description   : シートアトリビュート更新時処理
// Parameter     : aSheetNo(指定Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_SheetAttributeUpdate(UC aSheetNo)
{
	if( SRVC_GetDestinationFinish(aSheetNo) == eCdiFinOutDest_OutTray ) {
		if( aSheetNo == gCMPL_Manager.compileSheet ) {
			CMPL_CompileSheetEject();
		}
		else {
			/* No Action */
		}
	}
	else {																		// != eCdiFinOutDest_OutTray
		/* No Action */
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_SheetAbort
// Description   : シートアボート処理
// Parameter     : aSheetNo(指定Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_SheetAbort()
{
	CMPL_EJCT_StopSheet(SHEETNO_NUM);
	CMPL_STPL_StopSheet(SHEETNO_NUM);
	CMPL_TMP_StopSheet(SHEETNO_NUM);

	if( gCMPL_Manager.compileSheet != SHEETNO_NUM ) {
		if( SRVC_CheckSheetConditionStop(gCMPL_Manager.compileSheet) == true ) {
			gCMPL_Manager.compileSheet = SHEETNO_NUM;
		}
	}

	// V3.4.0  T/B 対策
	if( gCMPL_Manager.finishingWaitSheet != SHEETNO_NUM ) {
		if( SRVC_CheckSheetConditionStop(gCMPL_Manager.finishingWaitSheet) == true ) {
			gCMPL_Manager.finishingWaitSheet = SHEETNO_NUM; //
		}
	}
	//
	if( CMPL_DownCompCheck() == true ) {
		CMPL_ChangeProcDownEvent();
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_SheetDelivered
// Description   : シートデリバード処理
// Parameter     : aSheetNo(指定Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_SheetDelivered()
{
	if( CMPL_DownCompCheck() == true ) {
		CMPL_ChangeProcDownEvent();
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveStartUp
// Description   : Start Up指示受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveStartUp()
{
	CMPL_ChangeStartUpEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceivePowerUpReq
// Description   : Power Up Req受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceivePowerUpReq()
{
	CMPL_ChangePowerUpEvent();
	CMPL_ChangePowerOnEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceivePowerDownReq
// Description   : Power Down Req受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceivePowerDownReq()
{
	CMPL_ChangePowerDownEvent();
	CMPL_ChangePowerOffEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveChangeNormalReq
// Description   : Change Normal Req受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveChangeNormalReq()
{
	CMPL_ChangeChangingStandbyEvent();
	CMPL_ChangeStandbyEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveChangeDiagReq
// Description   : Change Diag Req受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveChangeDiagReq()
{
	CMPL_ChangeChangingDiagEvent();
	CMPL_ChangeDiagEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveHardDownReq
// Description   : Hard Down Req受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveHardDownReq()
{

	CMPL_ChangeHardDownEvent();

	if( CMPL_DownCompCheck() == true ) {
		CMPL_ChangeProcDownEvent();
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveCycleDownReq
// Description   : Cycle Down Req受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveCycleDownReq()
{
	CMPL_ChangeCycleDownEvent();

	if( CMPL_DownCompCheck() == true ) {
		CMPL_ChangeProcDownEvent();
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveDevActReqPowerOnInit
// Description   : Device Action Request(Power On Init)指示受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveDevActReqPowerOnInit()
{
	CMPL_PowerOnInit(ACT_START);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveDevActReqResumeInit
// Description   : Device Action Request(Resume Init)指示受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveDevActReqResumeInit()
{
	CMPL_ResumeInit(ACT_START);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveDevActReqNormalInit
// Description   : Device Action Request(Normal Init)指示受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveDevActReqNormalInit()
{
	CMPL_NormalInit(ACT_START);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveDevActReqProcessEnd
// Description   : Device Action Request(Process End)指示受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveDevActReqProcessEnd()
{
	CMPL_ProcessEnd(ACT_START);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveDevActReqJamClearAssist
// Description   : Device Action Request(Jam Clear Assist)指示受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveDevActReqJamClearAssist()
{
	CMPL_JamClearAssist(ACT_START);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveDevEvtAnyCoverOpen
// Description   : Device Event Notify(Any Cover Open)指示受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveDevEvtAnyCoverOpen()
{
	gCMPL_Manager.ejectPossible = false;

	CMPL_EJCT_AnyCoverOpen();
	CMPL_STPL_AnyCoverOpen();
	CMPL_TMP_AnyCoverOpen();

	CMPL_StopPowerOnInit();
	CMPL_StopResumeInit();
	CMPL_StopNormalInit();
	CMPL_StopProcessEnd();
	CMPL_StopJamClearAssist();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveDevEvtAllCoverClose
// Description   : Device Event Notify(All Cover Close)指示受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveDevEvtAllCoverClose()
{
	CMPL_EJCT_AllCoverClose();
	CMPL_STPL_AllCoverClose();
	CMPL_TMP_AllCoverClose();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveDevEvtDevInitComp
// Description   : Device Event Notify(Device Initialize Complete)指示受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveDevEvtDevInitComp()
{
	if( CMPL_CheckEjectPossible() == true ) {
		gCMPL_Manager.ejectPossible = true;
	}

	CMPL_CompileSheetEject();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveDevEvt24VCut
// Description   : Device Event Notify(24V Cut)指示受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveDevEvt24VCut()
{
	/* T.B.D. */
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveDevEvt24VCunnect
// Description   : Device Event Notify(24V Cunnect)指示受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveDevEvt24VCunnect()
{
	/* T.B.D. */
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveCompileExitSensorLead
// Description   : Compile Exit Senosr ON通知受信処理
// Parameter     : evt (イベントメッセージ(Location Event) )
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveCompileExitSensorLead(const UC* evt)
{
	LocationEvent* apLocationEvent = (LocationEvent*)evt;

	CMPL_CompileExitSensorON((UC)(apLocationEvent->mSheetNo));
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveCompileExitSensorTail
// Description   : Compile Exit Senosr OFF通知受信処理
// Parameter     : evt (イベントメッセージ(Location Event) )
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveCompileExitSensorTail(const UC* evt)
{
	LocationEvent* apLocationEvent = (LocationEvent*)evt;

	CMPL_CompileExitSensorOFF((UC)(apLocationEvent->mSheetNo));
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveSheetInitializeRequest
// Description   : Sheet Action Request(Sheet Initialize Request)受信処理
// Parameter     : evt (イベントメッセージ(Sheet Action Request) )
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveSheetInitializeRequest(const UC* evt)
{
	EvtParamSheetActionRequest* apSheetAction = (EvtParamSheetActionRequest*)evt;

	CMPL_SheetInitialize(apSheetAction->sheetNo, ACT_START);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveCycleUpReq
// Description   : Cycle Up Req受信処理
// Parameter     : evt (Sheet Information )
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveCycleUpReq(const UC* evt)
{
	CMPL_ChangeCycleUpEvent();
	CMPL_ChangeReadyEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveStandbyNotify
// Description   : Standby Notify受信処理
// Parameter     : evt (Sheet Information )
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveStandbyNotify(const UC* evt)
{
	CMPL_ChangeStandbyEvent();

	if( CMPL_CheckEjectPossible() == true ) {
		gCMPL_Manager.ejectPossible = true;
	}

	CMPL_CompileSheetEject();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveStandbyExitNotify
// Description   : Standby->CycleUp 前処理(DeviceStop)
// Parameter     : evt (Sheet Information )
// Return        : ‐
///////////////////////////////////////////////////////////////////
UC  CMPL_ReceiveStandbyExitNotify(const UC* evt)
{
/*	UC aIndex;
	UC aSheetNo = SHEETNO_NUM;

	for( aIndex = 0; aIndex < CMPL_SHTMNGNUM; aIndex++ ) {

		if( gCMPL_Manager.seqMngInfo[aIndex].sheetNo != SHEETNO_NUM ) {
		
			if( gCMPL_Manager.seqMngInfo[aIndex].sequenceId == CMPL_SEQ_ID_STANDBY_EJECT_FINISHING ) {
				SRVC_UserDebugLog(0x53, 0x53);
				return true;				
			}
		}
	}	*/
	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveSheetInformationUpdate
// Description   : シートアトリビュート更新通知受信処理
// Parameter     : evt (Sheet Information )
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveSheetInformationUpdate(const UC* evt)
{
	EvtParamSheetAttributeUpdate* apAttributeUpdate = (EvtParamSheetAttributeUpdate*)evt;

	CMPL_SheetAttributeUpdate((UC)(apAttributeUpdate->sheetNo));
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveSheetInformationAbort
// Description   : シートアボート通知受信処理
// Parameter     : evt (Sheet Information )
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveSheetInformationAbort(const UC* evt)
{
	CMPL_SheetAbort();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveSheetInfomationDelivered
// Description   : シートデリバード通知受信処理
// Parameter     : evt (Sheet Information )
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveSheetInfomationDelivered(const UC* evt)
{
	CMPL_SheetDelivered();
}



/////////////////////////////////////////////////////////////////////////////////////////
// Function Body (State)
/////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ChangeStartUpEvent
// Description   : Start Up遷移イベント処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ChangeStartUpEvent()
{
	UC aLoopCnt;
	
	gCMPL_Manager.state = CMPL_STT_UNKNOWN;
	gCMPL_Manager.ejectPossible = false;
	gCMPL_Manager.lastStaSeqIndex = 0;
	gCMPL_Manager.compileSheet = SHEETNO_NUM;
	gCMPL_Manager.finishingWaitSheet = SHEETNO_NUM;
	for( aLoopCnt = 0; aLoopCnt < CMPL_SHTMNGNUM; aLoopCnt++ ) {
		CMPL_ClearSequenceManageInfo(aLoopCnt);
	}

	CMPL_EJCT_Reset();
	CMPL_STPL_Reset();
	CMPL_TMP_Reset();

	CMPL_EJCT_SetUp();
	CMPL_STPL_SetUp();
	CMPL_TMP_SetUp();
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_ChangePowerUpEvent
// Description   : Power Up遷移イベント処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ChangePowerUpEvent()
{
	if( gCMPL_Manager.state == CMPL_STT_UNKNOWN ) {

		/* No Action */

		gCMPL_Manager.state = CMPL_STT_POWER_UP;

		/* No Action */
	}
	else {
		/* No Action */
	}
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_ChangePowerOnEvent
// Description   : Power On遷移イベント処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ChangePowerOnEvent()
{
	if( gCMPL_Manager.state == CMPL_STT_POWER_UP ) {

		/* No Action */

		gCMPL_Manager.state = CMPL_STT_POWER_ON;

		MSP_SendMessage(MID_CMPL, EVT_STT_POWER_UP_RES, 0, 0, LOG_OFF);
	}
	else {
		/* No Action */
	}
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_ChangePowerDownEvent
// Description   : Power Down遷移イベント処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ChangePowerDownEvent()
{
	if( (gCMPL_Manager.state == CMPL_STT_POWER_ON) || (gCMPL_Manager.state == CMPL_STT_STANDBY) || (gCMPL_Manager.state == CMPL_STT_DIAG) ) {
		
		/* No Action */
		
		gCMPL_Manager.state = CMPL_STT_POWER_DOWN;
		
		/* No Action */
		
	}
	else {
		/* No Action */
	}
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_ChangePowerOffEvent
// Description   : Power Off遷移イベント処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ChangePowerOffEvent()
{
	if( gCMPL_Manager.state == CMPL_STT_POWER_DOWN ) {
		CMPL_EJCT_PowerOff();
		CMPL_STPL_PowerOff();
		CMPL_TMP_PowerOff();

		CMPL_StopPowerOnInit();
		CMPL_StopResumeInit();
		CMPL_StopNormalInit();
		CMPL_StopJamClearAssist();

		gCMPL_Manager.state = CMPL_STT_POWER_OFF;

		MSP_SendMessage(MID_CMPL, EVT_STT_POWER_DOWN_RES, 0, 0, LOG_OFF);
	}
	else {
		/* No Action */
	}
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_ChangeChangingStandbyEvent
// Description   : ChaingingStandby遷移イベント処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ChangeChangingStandbyEvent()
{
	ECMPL_State aSourceState;

	if( (gCMPL_Manager.state == CMPL_STT_POWER_ON) || (gCMPL_Manager.state == CMPL_STT_DIAG) ) {
		aSourceState = gCMPL_Manager.state;

		/* No Action */

		gCMPL_Manager.state = CMPL_STT_CHANGING_STANDBY;

		if( aSourceState == CMPL_STT_DIAG ) {
			CMPL_EJCT_SetUp();
			CMPL_STPL_SetUp();
			CMPL_TMP_SetUp();
		}
		else {
			/* No Action */
		}
	}
	else {
		/* No Action */
	}
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_ChangeProcDownEvent
// Description   : ProcDown遷移イベント処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ChangeProcDownEvent()
{
	if( (gCMPL_Manager.state == CMPL_STT_CYCLE_DOWN) || (gCMPL_Manager.state == CMPL_STT_HARD_DOWN) ) {

		gCMPL_Manager.state = CMPL_STT_WAITING_STANDBY;
		SRVC_UserDebugLog(0x60, 0x60);
		MSP_SendMessage(MID_CMPL, EVT_STT_PROCESS_DOWN_RES, 0, 0, LOG_OFF);
	}
	else {
		/* No Action */
	}
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_ChangeStandbyEvent
// Description   : Standby遷移イベント処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ChangeStandbyEvent()
{
	ECMPL_State aSourceState;

	if( (gCMPL_Manager.state == CMPL_STT_WAITING_STANDBY) || (gCMPL_Manager.state == CMPL_STT_CHANGING_STANDBY) ) {
		aSourceState = gCMPL_Manager.state;

		if( aSourceState == CMPL_STT_CHANGING_STANDBY ) {
			CMPL_EJCT_Normal();
			CMPL_STPL_Normal();
			CMPL_TMP_Normal();
		}
		else {
			CMPL_EJCT_StopSteadyFinishing();
			CMPL_STPL_StopSteadyFinishing();
			CMPL_TMP_StopSteadyFinishing();
		}

		gCMPL_Manager.state = CMPL_STT_STANDBY;

		if( aSourceState == CMPL_STT_CHANGING_STANDBY ) {
			MSP_SendMessage(MID_CMPL, EVT_STT_CHANGE_NORMAL_RES, 0, 0, LOG_OFF);
		}
	}
	else {
		/* No Action */
	}
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_ChangeCycleUpEvent
// Description   : CycleUp遷移イベント処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ChangeCycleUpEvent()
{
	if( gCMPL_Manager.state == CMPL_STT_STANDBY ) {

		CMPL_EJCT_ProcStart();
		CMPL_STPL_ProcStart();
		CMPL_TMP_ProcStart();

		gCMPL_Manager.state = CMPL_STT_CYCLE_UP;

		MSP_SendMessage(MID_CMPL, EVT_STT_CYCLE_UP_RES, 0, 0, LOG_OFF);

	}
	else {
		/* No Action */
	}
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_ChangeReadyEvent
// Description   : Ready遷移イベント処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ChangeReadyEvent()
{
	if( gCMPL_Manager.state == CMPL_STT_CYCLE_UP ) {
		/* No Action */

		gCMPL_Manager.state = CMPL_STT_READY;

		/* No Action */
	}
	else {
		/* No Action */
	}
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_ChangeChangingDiagEvent
// Description   : ChangingDiag遷移イベント処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ChangeChangingDiagEvent()
{
	if( (gCMPL_Manager.state == CMPL_STT_POWER_ON) || (gCMPL_Manager.state == CMPL_STT_STANDBY) ) {
		
		/* No Action */
		
		gCMPL_Manager.state = CMPL_STT_CHANGING_DIAG;
		
		/* No Action */
		
	}
	else {
		/* No Action */
	}
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_ChangeDiagEvent
// Description   : Diag遷移イベント処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ChangeDiagEvent()
{
	if( gCMPL_Manager.state == CMPL_STT_CHANGING_DIAG ) {
		CMPL_StopPowerOnInit();
		CMPL_StopResumeInit();
		CMPL_StopNormalInit();
		CMPL_StopJamClearAssist();

		CMPL_EJCT_Diag();
		CMPL_STPL_Diag();
		CMPL_TMP_Diag();

		gCMPL_Manager.state = CMPL_STT_DIAG;

		MSP_SendMessage(MID_CMPL, EVT_STT_CHANGE_DIAG_RES, 0, 0, LOG_OFF);
	}
	else {
		/* No Action */
	}
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_ChangeHardDownEvent
// Description   : Hard Down遷移イベント処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ChangeHardDownEvent()
{
	UC aIndex;

	if( (gCMPL_Manager.state == CMPL_STT_READY) || (gCMPL_Manager.state == CMPL_STT_CYCLE_UP) || (gCMPL_Manager.state == CMPL_STT_CYCLE_DOWN) ) {

		gCMPL_Manager.ejectPossible = false;

		gCMPL_Manager.state = CMPL_STT_HARD_DOWN;

		CMPL_EJCT_StopModuleAction();
		CMPL_STPL_StopModuleAction();
		CMPL_TMP_StopModuleAction();

		for( aIndex = 0; aIndex < CMPL_SHTMNGNUM; aIndex++ ) {
			if( gCMPL_Manager.seqMngInfo[aIndex].sheetNo != SHEETNO_NUM ) {
				CMPL_StopSheetInitialize(gCMPL_Manager.seqMngInfo[aIndex].sheetNo);
				CMPL_StopCompilerAction(gCMPL_Manager.seqMngInfo[aIndex].sheetNo);
			}
			else {
				/* No Action */
			}
		}
		gCMPL_Manager.finishingWaitSheet = SHEETNO_NUM;
	}
	else {
		/* No Action */
	}
}

//////////////////////////////////////////////////////////////////
// Function Name : CMPL_ChangeCycleDownEvent
// Description   : Cycle Down遷移イベント処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_ChangeCycleDownEvent()
{
	if( (gCMPL_Manager.state == CMPL_STT_READY) || (gCMPL_Manager.state == CMPL_STT_CYCLE_UP) ) {

		gCMPL_Manager.ejectPossible = false;

		gCMPL_Manager.state = CMPL_STT_CYCLE_DOWN;

		/* No Action */
		PutCMPLManagerLog(0x22, 0x22, 0, 0);

	}
	else {
		/* No Action */
	}
}
///////////////////////////////////////////////////////////////////
// Function Name : PutCMPLManagerLog
// Description   : シートロケーションログ出力指示処理
// Parameter     : para1 (数字列の最初に付加する値)
//               : para2 (数字列の２番目に付加する値)
//               : para3 (数字列の３番目に付加する値)
//               : para4 (数字列の４番目に付加する値)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void PutCMPLManagerLog(UC para1, UC para2, US para3, US para4)
{
	UC log[6];

	log[0] = para1;
	log[1] = para2;
	log[2] = (UC) (para3 >>8 ) ;
	log[3] = (UC) (para3 & 0x00FF);	
	log[4] = (UC) (para4 >>8 ) ;
	log[5] = (UC) (para4 & 0x00FF);	

	DD_LOG_Put(LOGID_CMPL_Mgr, 'S', log, sizeof(log), LOG_CATEGORY_DEBUG);
}





