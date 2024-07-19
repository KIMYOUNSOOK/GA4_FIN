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

#define CMPL_EJECT_PROCESS_SIZE_THRESH 		2970 //2160								// �p���r�����{�̗p����臒l(�~0.1[mm])
#define CMPL_DISTANCE_CMPEXTSNRTOEXITROLL 	30.0								// Compiler Exit Sensor����Exit Roll�܂ł̋���(�~1[mm])
#define CMPL_DISTANCE_EXITROLLRAD			10.0								// Exit Roll�̔��a(�~1[mm])
#define CMPL_PUNCHFILTERTIME				30									// �p���`����[�댟�m�t�B���^����(�~1[ms])

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
	UC  lastOffset;    //(TB- )SetTop��Offset���NVM�ɐ����������Ă��Ȃ� 
	// ������(V3.0.6) CompileJob�̏����O�AStandby�ɑJ�ڂ���TB�̑΍�
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
// Description   : �ΏۃV�[�g�ɑ΂��āA�����r�������{���邩/���Ȃ������擾
//				 : (�ΏۃV�[�g�̎��p���̏��Ȃ�)
// Parameter     : sheetNo(�ΏۃV�[�g)
// Return        : eject(���{����FSHT_EJECT_EXEC�A���{���Ȃ��FSHT_EJECT_NOT)
///////////////////////////////////////////////////////////////////
EShtEject CMPL_GetEjectExecutionSheetReq(UC sheetNo)
{
	EShtEject eject;
	US ejectThresh, stackCount, maxProcSize;

	if( SRVC_GetDestinationFinish(sheetNo) == eCdiFinOutDest_OutTray ) {
		stackCount = SRVC_GetWeightedCompileCount(sheetNo);						// �~0.1[��]
		maxProcSize = SRVC_GetMaxProcessSizeCompile(sheetNo);

		if( SRVC_GetStapleMode(sheetNo) == eCdiFinStaple_Off ) {
			ejectThresh = CMPL_GetUnStapleEjectThresh(maxProcSize, sheetNo);
		}
		else {																	// Staple���{
			ejectThresh = CMPL_GetStapleEjectThresh(sheetNo);
		}

		if( stackCount > ejectThresh ) {
			eject = SHT_EJECT_EXEC;
		}
		else {																	// 50������
			eject = SHT_EJECT_NOT;
		}
	}
	else {																		// �r���悪OutTray�ȊO
		eject = SHT_EJECT_EXEC;
	}
//	PutCMPLManagerLog(0x33,eject,stackCount,ejectThresh);

	return eject;
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetEjectExecutionNextSheetReq
// Description   : �ΏۃV�[�g�ɑ΂��āA�����r�������{���邩/���Ȃ������擾
//				 : (�ΏۃV�[�g�̎��p���̏�񂠂�)
// Parameter     : sheetNo(�ΏۃV�[�g)
// Return        : eject(���{����FSHT_EJECT_EXEC�A���{���Ȃ��FSHT_EJECT_NOT)
///////////////////////////////////////////////////////////////////
EShtEject CMPL_GetEjectExecutionNextSheetReq(UC sheetNo, UC nextShtNo)
{
	EShtEject eject;
	US ejectThresh, stackCount, maxProcSize, nextShtProcSize;

	if( SRVC_GetDestinationFinish(sheetNo) == eCdiFinOutDest_OutTray ) {
		stackCount = SRVC_GetWeightedCompileCount(sheetNo) + CMPL_GetWeightedCompileCount(nextShtNo);	// ���p�����l�������d�݂Â��ύږ���(�~0.1[��])
		nextShtProcSize = SRVC_GetProcessSize(nextShtNo);
		maxProcSize = SRVC_GetMaxProcessSizeCompile(sheetNo);
		if( nextShtProcSize > maxProcSize ) {									// ���p�����l�������ύڍő�p����(�~0.1[mm])
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
	else {																		// �r���悪OutTray�ȊO
		eject = SHT_EJECT_EXEC;
	}
//	PutCMPLManagerLog(0x44,eject,stackCount,ejectThresh);
	return eject;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetUnStapleEjectThresh
// Description   : UnStaple�w�莞�̕����r����臒l���擾
// Parameter     : aMaxProcSize(�R���p�C���ύڗp�����ŁA�ő�̗p�����蒷)
// Return        : �����r����臒l(�~0.1[��])
// History 		: 
//�d�l�ύX(V3.6.0�j�����ύX
//�X�e�[�v���Ȃ�CompileTrayStack�������(�p�����蒷�@<=297mm��90gsm�ȉ��j
//�X�e�[�v���Ȃ�CompileTrayStack�������(�p�����蒷�@��297mm�@�܂��@�p�����蒷�@<=297mm��90gsm�����j
///////////////////////////////////////////////////////////////////
//US CMPL_GetUnStapleEjectThresh(US aMaxProcSize)
US CMPL_GetUnStapleEjectThresh(US aMaxProcSize, UC  aSheetNo)
{
	US	aEjectThresh;
	US	aMediaWeight =	SRVC_GetMediaWeight(aSheetNo); 

	//�X�e�[�v���Ȃ�CompileTrayStack�������(�p�����蒷�@<=297mm��90gsm�ȉ��j
	if( (aMaxProcSize <= CMPL_EJECT_PROCESS_SIZE_THRESH ) &&  (aMediaWeight <= 90)) {
		aEjectThresh = (US)(SRVC_GetNvmValueUS(NVMID_MAX_COMPILE_SHEET_FOR_UNSTAPLE_SIZE_S) * 10); //100;
	}
	else {																		// 297.0[mm]����
		aEjectThresh = (US)(SRVC_GetNvmValueUS(NVMID_MAX_COMPILE_SHEET_FOR_UNSTAPLE_SIZE_L) * 10); //100;
	}

	return aEjectThresh;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetStapleEjectThresh
// Description   : Staple�w�莞�̕����r����臒l���擾
// Parameter     : �]
// Return        : �����r����臒l(�~0.1[��])
///////////////////////////////////////////////////////////////////
US CMPL_GetStapleEjectThresh(UC aSheetNo)
{
	// V3.0  ( StackerTray�r��Staple�\����)
		/* �p���@1��������̊��Z�����@=�@A�@/�@B
			A�@�F�@StaplerUnit��ޖ���Staple�\����[��] : �P��= �~0.1[��])
			B�@�F�@����A�ؗʂɉ�����Staple�\����[��]  : NVM
		*/
		US aEjectThresh;
		US	aMediaType = SRVC_GetMediaType(aSheetNo);  //
		US	aMediaWeight =	SRVC_GetMediaWeight(aSheetNo);  //
	
		US	 aWeightEjectThresh = 0; //NVM
		US	 retValue =0;
		aEjectThresh = (US)(SRVC_GetNvmValueUS(NVMID_MAX_COMPILE_SHEET_COUNT_FOR_STAPLE_50) * 10);// 65���X�e�[�v��
		//�@��R�[�g��
		if ( aMediaType != eCdiFinMediaType_Coated ) { 
	
			if ( aMediaWeight >= 60  &&  aMediaWeight <= 80 ) {  //�ؗ�(gsm) = 60 ~ 80
	
				aWeightEjectThresh = SRVC_GetNvmValueUS(NVMID_MAX_SHEET_UNCOAT_80_STAPLE_50);
				
			} else if ( aMediaWeight >= 81	&&	aMediaWeight <= 90 ) {	//�ؗ�(gsm) = 81 ~ 90
	
				aWeightEjectThresh = SRVC_GetNvmValueUS(NVMID_MAX_SHEET_UNCOAT_90_STAPLE_50);
				
			} else if ( aMediaWeight >= 91	&&	aMediaWeight <= 105 ) {  //�ؗ�(gsm) = 91 ~ 105 
	
				aWeightEjectThresh = SRVC_GetNvmValueUS(NVMID_MAX_SHEET_UNCOAT_105_STAPLE_50);
				
			} else if ( aMediaWeight >= 106  &&  aMediaWeight <= 128 ) {  //�ؗ�(gsm) = 106 ~ 128 
	
				aWeightEjectThresh = SRVC_GetNvmValueUS(NVMID_MAX_SHEET_UNCOAT_128_STAPLE_50);
				
			} else if ( aMediaWeight >= 129  &&  aMediaWeight <= 150 ) {  //�ؗ�(gsm) = 129 ~ 150 
	
				aWeightEjectThresh = SRVC_GetNvmValueUS(NVMID_MAX_SHEET_UNCOAT_150_STAPLE_50);
				
			} else if ( aMediaWeight >= 151  &&  aMediaWeight <= 176 ) {  //�ؗ�(gsm) = 151 ~ 176 
	
				aWeightEjectThresh = SRVC_GetNvmValueUS(NVMID_MAX_SHEET_UNCOAT_176_STAPLE_50);
				
			} else if ( aMediaWeight >= 177  &&  aMediaWeight <= 220 ) {  //�ؗ�(gsm) = 177 ~ 220 
	
				aWeightEjectThresh = SRVC_GetNvmValueUS(NVMID_MAX_SHEET_UNCOAT_220_STAPLE_50);
				
			}
			
		} else {	//�@�R�[�g��
	
			if ( aMediaWeight >= 106  &&  aMediaWeight <= 128 ) {  //�ؗ�(gsm) = 106 ~ 128
	
				aWeightEjectThresh = SRVC_GetNvmValueUS(NVMID_MAX_SHEET_COAT_128_STAPLE_50);
				
			} else if ( aMediaWeight >= 129 &&	aMediaWeight <= 150 ) {  //�ؗ�(gsm) = 129 ~ 150 
	
				aWeightEjectThresh = SRVC_GetNvmValueUS(NVMID_MAX_SHEET_COAT_150_STAPLE_50);
				
			} else if ( aMediaWeight >= 151 &&	aMediaWeight <= 176 ) {  //�ؗ�(gsm) = 151 ~ 176 
	
				aWeightEjectThresh = SRVC_GetNvmValueUS(NVMID_MAX_SHEET_COAT_176_STAPLE_50);
				
			} else if ( aMediaWeight >= 177  &&  aMediaWeight <= 220 ) {  //�ؗ�(gsm) = 177 ~ 220 
	
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
// Description   : Standby�J�ڎ��ɗp���r�o�����邩/���Ȃ����𔻒f
// Parameter     : aSheetNo (�ΏۃV�[�g�i���o�[)
// Return        : SHT_EJECT_EXEC�F�r�o������ASHT_EJECT_NOT�F�r�o�����Ȃ�
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
// Description   : �ΏۃV�[�g�̏d�ݖ������擾
// Parameter     : aSheetNo(�ΏۃV�[�g��SheetNo)
// Return        : �d�ݖ���(�~0.1[��])
///////////////////////////////////////////////////////////////////
US CMPL_GetWeightedCompileCount(UC sheetNo)
{
	return 10;

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetOffsetFinishPosition
// Description   : �ΏۃV�[�g�̃I�t�Z�b�g�t�B�j�b�V���|�W�V�������擾
// Parameter     : aSheetNo(�ΏۃV�[�g��SheetNo)
// Return        : �ΏۃV�[�g�̃I�t�Z�b�g�t�B�j�b�V���|�W�V����
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
	else {																		// != eCdiFinOutDest_OutTray (�֗^���Ȃ����W���[�����w��)
		aOfstFin = SHT_OFST_FIN_NOT;
	}

	return aOfstFin;
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetCompilerSheetInitializeTimeStep1
// Description   : Sheet Initialize ���쎞��
// Parameter     : aSheetNo(�ΏۃV�[�g��SheetNo)
// Return        :  aTotalTime 
//History	 : V3.5.93 Job�������J�n�����C��
///////////////////////////////////////////////////////////////////
US CMPL_GetCompilerSheetInitializeTimeStep1(UC sheetNo, UC lastShtNo)
{
	US aTotalTime = 0;
	US  aPara1, aPara2, aPara3;
	US  aEjectHomeTime=0;

	gCMPL_Manager.sheetInitAction[sheetNo] = (US)0x0000;

	if( lastShtNo == SHEETNO_NUM) {	// �Z�b�g�c�肪�����v���Z�X�X�^�[�g �� �V�[�g�C�j�V�����C�Y������{��̃L�����Z���P�[�X���P�A
	
		//Tamper SizePosition Times
		aPara1 = CMPL_TMP_GetSizePositioningTime(sheetNo, lastShtNo); 

		// SubPaddle HomePosition Time
		aPara2 = 229; //(V3.6.0)

		// Stacker(1621ms) > Eject Home(1147ms) �̂��߁AStacker���쎞�Ԃ��g���B
		aPara3 = 1621;
		aEjectHomeTime = CMPL_GetEjectHomeNeedTime(sheetNo);

		if ( aPara3 < aEjectHomeTime){
			aPara3 = aEjectHomeTime;
		}

		aTotalTime = aPara1 + aPara2 + aPara3 ;

		gCMPL_Manager.sheetInitAction[sheetNo] |= GET_BIT_PATTERN(CMPL_MDL_ACT_EJECTOR_HOME);
		gCMPL_Manager.sheetInitAction[sheetNo] |= GET_BIT_PATTERN(CMPL_MDL_ACT_TAMP_SIZE_POS);
		gCMPL_Manager.sheetInitAction[sheetNo] |= GET_BIT_PATTERN(CMPL_MDL_ACT_PADDLE_HOME);	
		
	} else if ( (SRVC_GetSetTop(sheetNo) == SHT_SET_TOP) &&				// "����Ώۗp�����Z�b�g�擪�p���ł���A�ȉ��̂����ꂩ�̏����𖞂����Ă���A
			 ((SRVC_GetCrossProcessSize(sheetNo) != SRVC_GetCrossProcessSize(lastShtNo)) ||	//    ������Ώۗp���̗p�����Ɛ�s���̗p�������قȂ�
	 		  (SRVC_GetStapleMode(sheetNo) != SRVC_GetStapleMode(lastShtNo)) ||	//    ������Ώۗp���̃X�e�[�v���w��Ɛ�s���̃X�e�[�v���w�肪�قȂ�
	 		  (SRVC_GetCrossProcessSizeMixSet(lastShtNo) == SHT_SIZE_MIX)) ){		//    ����s���̃Z�b�g�ɂ����ăZ�b�g���ٕ����������Ă��� 	
		
		//Tamper SizePosition Times
		aPara1 = CMPL_TMP_GetSizePositioningTime(sheetNo, lastShtNo); 

		aTotalTime = aPara1;
		
		gCMPL_Manager.sheetInitAction[sheetNo] |= GET_BIT_PATTERN(CMPL_MDL_ACT_TAMP_SIZE_POS);	
	}
	return aTotalTime;
}

///////////////////////////////////////////////////////////////////V3.5.93
// Function Name : CMPL_IsSheetInitExec
// Description   : �e����ɂ�����V�[�g�C�j�V�����C�Y����̎��{���f
// Parameter     : sheetNo (�ΏۃV�[�g�̃V�[�g�i���o�[)
// Return        : true �c�V�[�g�C�j�V�����C�Y��������{����
//				 : fasel�c�V�[�g�C�j�V�����C�Y��������{���Ȃ�
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
		return false;															// �����̒l�ُ�
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetEjectHomeNeedTime
// Description   : 
// Return        :  �C�W�F�N�g Home���쎞��[ms]
///////////////////////////////////////////////////////////////////
US  CMPL_GetEjectHomeNeedTime(UC  sheetNo)
{
	return   (US)1147;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetEjectNeedTime
// Description   : 
// Return        :  �C�W�F�N�g���쎞��[ms]
///////////////////////////////////////////////////////////////////
US  CMPL_GetEjectNeedTime(UC  sheetNo)
{
	return   (US)1167;
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetBase_CompileFinishingNeedTime
// Description   : SetPitch  ��{����[ms]
// Parameter     : sheetNo (����Ώۂ̃V�[�g�i���o�[)
// Return        :  a + b 
// a	�FCompile Exit Sensor OFF����^���s���O����J�n�܂ł̎���[ms]
// b	�C�W�F�N�g���쎞��[ms]
// c : Compile ExitSnr On~ Off
// d : 
///////////////////////////////////////////////////////////////////
US CMPL_GetBase_CompileFinishingNeedTime(UC sheetNo)
{
	US	paraA  ;  //  a	�FCompile Exit Sensor OFF����^���s���O����J�n�܂ł̎���[ms]
	US    paraB  ; //  b	�F�C�W�F�N�g���쎞��[ms]
	US    ret;
	
	paraA = CMPL_TMP_GetEjectTampingDelayTime(sheetNo);
	paraB =   CMPL_GetEjectNeedTime(sheetNo); //1167; //1147 ;  (V3.6.2)
	ret = paraA + paraB;
	
	return  ret;	
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetAdd_CompileFinishingNeedTime
// Description   :  SetPitch ���Z���ԁ@(CompilerFinishing���쎞��)
// Parameter     : sheetNo (����Ώۂ̃V�[�g�i���o�[)
// Return        :  A 
//A[ms] �������_�ȉ��͐؂�̂�
//Staple Mode Off  :  A �� a �{ c �{ ��
//Single Front       :  A�� a �{ b �{ c �{ ��
//
//�Z�o���Ŏg�p���Ă���e�p�����[�^�̏ڍׂ͈ȉ��̒ʂ�ł���B
//�Ȃ��A�e����̎��Ԃ͂��̓���̎��{�����𖞂����ꍇ�̂݉��Z����B
//
//a	�F�^���s���O����(�Z�b�g���X�g�^���s���O)����[ms] (��:�㎥���Ԋ܂�)
//b 	�F�X�e�[�v�����쎞��(�X�e�[�v����ł���)[ms]
//c	�F�^���p�[�I�t�Z�b�g���쎞��[ms]
//d	�F�T�u�p�h���p������ ���̓���񐔂ɂ�铮�쎞��[ms]
///////////////////////////////////////////////////////////////////
US CMPL_GetAdd_CompileFinishingNeedTime(UC sheetNo)
{
        US	paraA  ;  //a	�F�^���s���O����(�Z�b�g���X�g�^���s���O)����[ms] (��:�㎥���Ԋ܂�)
	US    paraB  ;//b 	�F�X�e�[�v�����쎞��(�X�e�[�v����ł���)[ms]
	US    paraC  ;//c	�F�^���p�[�I�t�Z�b�g���쎞��[ms]
	US    paraD  ;//d	�F�T�u�p�h���p������ ���̓���񐔂ɂ�铮�쎞��[ms]	
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
// Description   :  ���Z���ԁ@(Tamper ���������쎞��)
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : sheetNo (�ΏۃV�[�g�̃V�[�g���ʎq)
//               : prevShtNo (��s�V�[�g�̃V�[�g���ʎq)
//
// ���Z���� :  �^���p�[����������K�v����
// ���Z����[ms] �� a �{ b  
//a �FCompile Exit Sensor OFF����^���s���O����J�n�܂ł̎��ԁi�p�����e�҂����ԁj[ms] 
//b �F�^���s���O����(�m�[�}���^���s���O)����[ms] (��:�㎥���Ԋ܂�) 
///////////////////////////////////////////////////////////////////////
US  CMPL_GetAdd_TamperOpenNeedTime(UC  sheetNo)
{
	US   ret;
	US   paraA;
	US   paraB;
		
	if ( CMPL_GetActionModeTamperOpen(sheetNo)) {  //�^���p�[�I�[�v����������{����
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
// Description   :  ���Z���ԁ@(SubPaddle 3��ȏ�̏ꍇ, ���쎞��)
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : sheetNo (�ΏۃV�[�g�̃V�[�g���ʎq)
//               : prevShtNo (��s�V�[�g�̃V�[�g���ʎq)
// ���Z���� :  �T�u�p�h���p������ ���̓���񐔂�3��̏ꍇ�̉��Z����     
//
//���Z���ԁ�(P��-2)�~tms
//��ms��360 �~�e�T�u�p�h���p������ �v���t�@�C����30STEP���̎��s���ԁ�1.722
//��13.767��T1 PULSE���̉�]�p�x
//     �A���A�v�Z���ʂ��}�C�i�X�ł����0ms�ɂ���
///////////////////////////////////////////////////////////////////////
US  CMPL_GetAdd_SubPaddleNeedTime(UC sheetNo)
{
	UC    paddleCnt; 
	US    aTotalTime, aTemp;
	
	paddleCnt =  CMPL_EJCT_GetSubPaddleActionCount(sheetNo);

	if ( paddleCnt >= 3 )  { // 
		aTemp = (US)paddleCnt;   //
		aTotalTime = (US)((aTemp - 2) * 150);  // (P�� - 2) * tms ..

	} else {
		aTotalTime = 0;
	}
	
//	PutCMPLManagerLog(0x56,0x56, paddleCnt,aTotalTime);
	return aTotalTime;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_GetActionModeTamperOpen
// Description   : �^���p�[�I�[�v������̎��{�L�����擾
// Parameter     : sheetNo (�ΏۃV�[�g�̃V�[�g�i���o�[)
// Return        : �^���p�[�I�[�v������̎��{�L��
//				 : �͈́F0�`1
//				 : �Ӗ��F0�c�^���p�[�I�[�v����������{���Ȃ�
//				 : 		 1�c�^���p�[�I�[�v����������{����
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
// Description   : �t�B�j�b�V���O�v���Z�X���������`�F�b�N
// Parameter     : �]
// Return        : true�F�����Afalse�F����
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
// Description   : �p���r�o���ł��邩�̔��f
// Parameter     : �]
// Return        : true�F�r�o�Afalse�F�r�o�s��
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
// Description   : Sequence Manage Info�N���A
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ClearSequenceManageInfo(UC aIndex)
{
	gCMPL_Manager.seqMngInfo[aIndex].sequenceId = CMPL_SEQ_ID_NUM;
	gCMPL_Manager.seqMngInfo[aIndex].actionLog  = 0x0000;
	gCMPL_Manager.seqMngInfo[aIndex].sheetNo    = SHEETNO_NUM;	
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_PowerOnInit
// Description   : Power On Initialize����(�J�n/��~)
// Parameter     : aAction (����̊J�n/��~)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_PowerOnInit(EAction aAction)
{
	if( aAction == ACT_START )	CMPL_StartPowerOnInit();
	else					 	CMPL_StopPowerOnInit();							// Stop
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_StartPowerOnInit
// Description   : Power On Initialize����J�n
// Parameter     : �]
// Return        : �]
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
// Description   : Power On Initialize���슮��
// Parameter     : �]
// Return        : �]
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
// Description   : Power On Initialize�����~
// Parameter     : �]
// Return        : �]
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
// Description   : Resume Initialize����(�J�n/��~)
// Parameter     : aAction (����̊J�n/��~)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ResumeInit(EAction aAction)
{
	if( aAction == ACT_START )	CMPL_StartResumeInit();
	else					 	CMPL_StopResumeInit();							// Stop
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_StartPowerResumeInit
// Description   : Resume Initialize����J�n
// Parameter     : �]
// Return        : �]
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
// Description   : Resume Initialize���슮��
// Parameter     : �]
// Return        : �]
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
// Description   : Resume Initialize�����~
// Parameter     : �]
// Return        : �]
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
// Description   : �m�[�}���C�j�V�����C�Y����(�J�n/��~)
// Parameter     : aAction (�J�n/��~)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_NormalInit(EAction aAction)
{
	if( aAction == ACT_START )	CMPL_StartNormalInit();
	else						CMPL_StopNormalInit();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_StartNormalInit
// Description   : �m�[�}���C�j�V�����C�Y����J�n
// Parameter     : �]
// Return        : �]
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
// Description   : �m�[�}���C�j�V�����C�Y���슮��
// Parameter     : �]
// Return        : �]
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
// Description   : �m�[�}���C�j�V�����C�Y�����~
// Parameter     : �]
// Return        : �]
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
// Description   : �V�[�g�C�j�V�����C�Y����(�J�n/��~)
// Parameter     : aSheetNo(�w��Sheet No.)
//				 : aAction(����w��(�J�n/��~))
// Return        : �]
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
// Description   : �V�[�g�C�j�V�����C�Y����J�n
// Parameter     : aSheetNo(�w��Sheet No.)
// Return        : �]
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
// Description   : �V�[�g�C�j�V�����C�Y���슮��
// Parameter     : aIndex(�V�[�P���X�Ǘ��ԍ�)
// Return        : �]
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
// Description   : �V�[�g�C�j�V�����C�Y�����~
// Parameter     : aIndex(�V�[�P���X�Ǘ��ԍ�)
// Return        : �]
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
// Description   : �v���Z�X�G���h����(�J�n/��~)
// Parameter     : aAction(����w��(�J�n/��~))
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ProcessEnd(EAction aAction)
{
	if( aAction == ACT_START )	CMPL_StartProcessEnd();
	else						CMPL_StopProcessEnd();							// ACT_STOP
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_StartProcessEnd
// Description   : �v���Z�X�G���h����J�n
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_StartProcessEnd()
{

       if( gCMPL_Manager.seqMngInfo[0].sequenceId == CMPL_SEQ_ID_NUM ) {

		gCMPL_Manager.seqMngInfo[0].sequenceId = CMPL_SEQ_ID_PROC_END;
		gCMPL_Manager.seqMngInfo[0].actionLog = 0x0000;
		gCMPL_Manager.seqMngInfo[0].sheetNo = SHEETNO_NUM;

		gCMPL_Manager.lastStaSeqIndex = 0;

		//V3.1.7  StapleCoverILK Open���́A���삵�Ȃ�
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
// Description   : �v���Z�X�G���h���슮��
// Parameter     : �]
// Return        : �]
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
// Description   : �v���Z�X�G���h�����~
// Parameter     : �]
// Return        : �]
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
// Description   : �W�����N���A�⏕�C�j�V�����C�Y����(�J�n/��~)
// Parameter     : aAction(����w��(�J�n/��~))
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_JamClearAssist(EAction aAction)
{
	if( aAction == ACT_START )	CMPL_StartJamClearAssist();
	else					 	CMPL_StopJamClearAssist();						// ACT_STOP
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_StartJamClearAssist
// Description   : �W�����N���A�⏕�C�j�V�����C�Y����J�n
// Parameter     : �]
// Return        : �]
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
// Description   : �W�����N���A�⏕�C�j�V�����C�Y���슮��
// Parameter     : �]
// Return        : �]
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
// Description   : �W�����N���A�⏕�C�j�V�����C�Y�����~
// Parameter     : �]
// Return        : �]
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
// Description   : �R���p�C���A�N�V��������(�J�n/��~)
// Parameter     : aSheetNo(�w��Sheet No.)
//				 : aAction(����w��(�J�n/��~))
// Return        : �]
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
// Description   : �R���p�C���A�N�V��������J�n
// Parameter     : aSheetNo(�w��Sheet No.)
// Return        : �]
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

	if( SRVC_CheckSheetConditionNormal(aSheetNo) == true ) {						// Leave�p�����r���ΏۂƂȂ����ꍇ�́ACondition��Remain�ɏ�������邽�߁A
		if( gCMPL_Manager.seqMngInfo[aIndex].sequenceId == CMPL_SEQ_ID_NUM ) {	// ConditionNormal�̃`�F�b�N��OK�Ƃ��Ă���
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
		//V3.6.93  EjectFail -> CompileSNR OFF ���ACondition=NG�Ƃ��ď�������Ȃ����߁AStandby�ɍs���Ȃ��s�
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
// Description   : �R���p�C���A�N�V�������슮��
// Parameter     : aIndex(Sequence Index)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_CompleteCompilerAction(UC aIndex)
{
	UC aSheetNo;
	US aEventId;
	EvtParamSheetEventNotify aMessage;
	
	if( (gCMPL_Manager.seqMngInfo[aIndex].sequenceId == CMPL_SEQ_ID_COMPILE_FINISHING)	||
		(gCMPL_Manager.seqMngInfo[aIndex].sequenceId == CMPL_SEQ_ID_EJECT_FINISHING) 	||
		(gCMPL_Manager.seqMngInfo[aIndex].sequenceId == CMPL_SEQ_ID_STANDBY_EJECT_FINISHING) ) {
		aSheetNo = gCMPL_Manager.seqMngInfo[aIndex].sheetNo;							// ���b�Z�[�W���M�p�ɕێ�

		if( gCMPL_Manager.seqMngInfo[aIndex].sequenceId == CMPL_SEQ_ID_COMPILE_FINISHING ) {
			gCMPL_Manager.compileSheet = gCMPL_Manager.seqMngInfo[aIndex].sheetNo;

			CMPL_ClearSequenceManageInfo(aIndex);

			aMessage.sheetNo = aSheetNo;
			MSP_SendMessage(MID_CMPL, EVT_SHT_EVT_OUT_TRAY_COMPILE_FINISH_COMP, (UC*)(&aMessage), sizeof(aMessage), LOG_OFF);

			if( SRVC_GetEject(aSheetNo) == SHT_EJECT_EXEC ) {	// �R���p�C���t�B�j�b�V���O���ɔr�o�ΏۂƂȂ����P�[�X
				CMPL_StartCompilerAction(aSheetNo);
			}
		}
		else {																	// CMPL_SEQ_ID_EJECT_FINISHING, CMPL_SEQ_ID_STANDBY_EJECT_FINISHING
			if( gCMPL_Manager.seqMngInfo[aIndex].sequenceId == CMPL_SEQ_ID_STANDBY_EJECT_FINISHING ) {
				CMPL_EJCT_StandbyEjectCompNotify;									// StandbyEjectFinishing
				CMPL_STPL_StandbyEjectCompNotify;
				CMPL_TMP_StandbyEjectCompNotify();
			}

			//(TB- )SetTop��Offset���NVM�ɐ����������Ă��Ȃ�
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
// Description   : �R���p�C���A�N�V���������~
// Parameter     : aIndex(Sequence Index)
// Return        : �]
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
// Description   : �V�[�P���X������s
// Parameter     : aCompAct (���슮����������)
//				 : aSeqMngIdx (SequenceManageInfo��Index)
// Return        : �]
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
// Description   : Module Action Complete(Compiler Internal I/F)�ʒm��M����
// Parameter     : �]
// Return        : �]
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
					if( (apMdlActSeq[aActIdx].actID == aAction) &&				// aAction���V�[�P���X���ɂ��邩�A���łɊ������Ă��Ȃ������`�F�b�N
						((gCMPL_Manager.seqMngInfo[aIndex].actionLog & GET_BIT_PATTERN(aAction)) == 0) ) {
						aLoopCnt = CMPL_SHTMNGNUM;
						CMPL_ExecuteSequence(aAction, aIndex);

						break;
					}
					else { /* No Action */ }
				}
			}
			else { /* No Action */ }

			aIndex = (UC)((aIndex + 1) & (CMPL_SHTMNGNUM - 1));					// index�X�V
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
// Description   : ���W���[���C�x���g�ʒm
// Parameter     : aEvent (���W���[���C�x���g)
//				 : aSheetNo (�ΏۃV�[�g�i���o�[)
// Return        : �]
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
// Description   : Device Status�̕ω���ʒm
// Parameter     : aStatusNo
//				 : aStatusValue
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_InformedDeviceStatusChange(EDeviceStatusNo aStatusNo, US aStatusValue)
{
	SRVC_UpdateDeviceStatus(aStatusValue, aStatusNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_CurrentOFF
// Description   : �㎥OFF���� �F Tamper��Eject�㎥OFF����
// Parameter     : aSheetno(�ΏۃV�[�g�i���o�[)
// History :  V3.6.93 �d�l�ύX ProcessEnd Sequence.
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
// Description   : �C�x���g�^�C�}�̃^�C���A�b�v������
// Parameter     : aSheetno(�ΏۃV�[�g�i���o�[)
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
// Description   : Component Control Stop Request�w����M����
// Parameter     : evt (�C�x���g���b�Z�[�W(Chain Link No.) )
// Return        : �]
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
				default:														// ����`��Link No.
					/* No Action */
					break;
				}
			}
		}
	}
	else {																		// ����`��Chain No.
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_InformedCompoControlComplete
// Description   : Module Action Complete(Compiler Internal I/F)�ʒm��M����
// Parameter     : �]
// Return        : �]
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
// Description   : Compile Exit Sensor ON���m����
// Parameter     : aSheetNo(�w��Sheet No.)
// Return        : �]
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
// Description   : Compile Exit Sensor OFF���m����
// Parameter     : aSheetNo(�w��Sheet No.)
// Return        : �]
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
// Description   : �V�[�g�A�g���r���[�g�X�V������
// Parameter     : aSheetNo(�w��Sheet No.)
// Return        : �]
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
// Description   : �V�[�g�A�{�[�g����
// Parameter     : aSheetNo(�w��Sheet No.)
// Return        : �]
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

	// V3.4.0  T/B �΍�
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
// Description   : �V�[�g�f���o�[�h����
// Parameter     : aSheetNo(�w��Sheet No.)
// Return        : �]
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
// Description   : Start Up�w����M����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveStartUp()
{
	CMPL_ChangeStartUpEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceivePowerUpReq
// Description   : Power Up Req��M����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ReceivePowerUpReq()
{
	CMPL_ChangePowerUpEvent();
	CMPL_ChangePowerOnEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceivePowerDownReq
// Description   : Power Down Req��M����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ReceivePowerDownReq()
{
	CMPL_ChangePowerDownEvent();
	CMPL_ChangePowerOffEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveChangeNormalReq
// Description   : Change Normal Req��M����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveChangeNormalReq()
{
	CMPL_ChangeChangingStandbyEvent();
	CMPL_ChangeStandbyEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveChangeDiagReq
// Description   : Change Diag Req��M����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveChangeDiagReq()
{
	CMPL_ChangeChangingDiagEvent();
	CMPL_ChangeDiagEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveHardDownReq
// Description   : Hard Down Req��M����
// Parameter     : �]
// Return        : �]
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
// Description   : Cycle Down Req��M����
// Parameter     : �]
// Return        : �]
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
// Description   : Device Action Request(Power On Init)�w����M����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveDevActReqPowerOnInit()
{
	CMPL_PowerOnInit(ACT_START);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveDevActReqResumeInit
// Description   : Device Action Request(Resume Init)�w����M����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveDevActReqResumeInit()
{
	CMPL_ResumeInit(ACT_START);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveDevActReqNormalInit
// Description   : Device Action Request(Normal Init)�w����M����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveDevActReqNormalInit()
{
	CMPL_NormalInit(ACT_START);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveDevActReqProcessEnd
// Description   : Device Action Request(Process End)�w����M����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveDevActReqProcessEnd()
{
	CMPL_ProcessEnd(ACT_START);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveDevActReqJamClearAssist
// Description   : Device Action Request(Jam Clear Assist)�w����M����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveDevActReqJamClearAssist()
{
	CMPL_JamClearAssist(ACT_START);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveDevEvtAnyCoverOpen
// Description   : Device Event Notify(Any Cover Open)�w����M����
// Parameter     : �]
// Return        : �]
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
// Description   : Device Event Notify(All Cover Close)�w����M����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveDevEvtAllCoverClose()
{
	CMPL_EJCT_AllCoverClose();
	CMPL_STPL_AllCoverClose();
	CMPL_TMP_AllCoverClose();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveDevEvtDevInitComp
// Description   : Device Event Notify(Device Initialize Complete)�w����M����
// Parameter     : �]
// Return        : �]
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
// Description   : Device Event Notify(24V Cut)�w����M����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveDevEvt24VCut()
{
	/* T.B.D. */
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveDevEvt24VCunnect
// Description   : Device Event Notify(24V Cunnect)�w����M����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveDevEvt24VCunnect()
{
	/* T.B.D. */
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveCompileExitSensorLead
// Description   : Compile Exit Senosr ON�ʒm��M����
// Parameter     : evt (�C�x���g���b�Z�[�W(Location Event) )
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveCompileExitSensorLead(const UC* evt)
{
	LocationEvent* apLocationEvent = (LocationEvent*)evt;

	CMPL_CompileExitSensorON((UC)(apLocationEvent->mSheetNo));
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveCompileExitSensorTail
// Description   : Compile Exit Senosr OFF�ʒm��M����
// Parameter     : evt (�C�x���g���b�Z�[�W(Location Event) )
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveCompileExitSensorTail(const UC* evt)
{
	LocationEvent* apLocationEvent = (LocationEvent*)evt;

	CMPL_CompileExitSensorOFF((UC)(apLocationEvent->mSheetNo));
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveSheetInitializeRequest
// Description   : Sheet Action Request(Sheet Initialize Request)��M����
// Parameter     : evt (�C�x���g���b�Z�[�W(Sheet Action Request) )
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveSheetInitializeRequest(const UC* evt)
{
	EvtParamSheetActionRequest* apSheetAction = (EvtParamSheetActionRequest*)evt;

	CMPL_SheetInitialize(apSheetAction->sheetNo, ACT_START);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveCycleUpReq
// Description   : Cycle Up Req��M����
// Parameter     : evt (Sheet Information )
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveCycleUpReq(const UC* evt)
{
	CMPL_ChangeCycleUpEvent();
	CMPL_ChangeReadyEvent();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveStandbyNotify
// Description   : Standby Notify��M����
// Parameter     : evt (Sheet Information )
// Return        : �]
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
// Description   : Standby->CycleUp �O����(DeviceStop)
// Parameter     : evt (Sheet Information )
// Return        : �]
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
// Description   : �V�[�g�A�g���r���[�g�X�V�ʒm��M����
// Parameter     : evt (Sheet Information )
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveSheetInformationUpdate(const UC* evt)
{
	EvtParamSheetAttributeUpdate* apAttributeUpdate = (EvtParamSheetAttributeUpdate*)evt;

	CMPL_SheetAttributeUpdate((UC)(apAttributeUpdate->sheetNo));
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveSheetInformationAbort
// Description   : �V�[�g�A�{�[�g�ʒm��M����
// Parameter     : evt (Sheet Information )
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_ReceiveSheetInformationAbort(const UC* evt)
{
	CMPL_SheetAbort();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_ReceiveSheetInfomationDelivered
// Description   : �V�[�g�f���o�[�h�ʒm��M����
// Parameter     : evt (Sheet Information )
// Return        : �]
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
// Description   : Start Up�J�ڃC�x���g����
// Parameter     : �]
// Return        : �]
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
// Description   : Power Up�J�ڃC�x���g����
// Parameter     : �]
// Return        : �]
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
// Description   : Power On�J�ڃC�x���g����
// Parameter     : �]
// Return        : �]
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
// Description   : Power Down�J�ڃC�x���g����
// Parameter     : �]
// Return        : �]
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
// Description   : Power Off�J�ڃC�x���g����
// Parameter     : �]
// Return        : �]
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
// Description   : ChaingingStandby�J�ڃC�x���g����
// Parameter     : �]
// Return        : �]
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
// Description   : ProcDown�J�ڃC�x���g����
// Parameter     : �]
// Return        : �]
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
// Description   : Standby�J�ڃC�x���g����
// Parameter     : �]
// Return        : �]
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
// Description   : CycleUp�J�ڃC�x���g����
// Parameter     : �]
// Return        : �]
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
// Description   : Ready�J�ڃC�x���g����
// Parameter     : �]
// Return        : �]
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
// Description   : ChangingDiag�J�ڃC�x���g����
// Parameter     : �]
// Return        : �]
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
// Description   : Diag�J�ڃC�x���g����
// Parameter     : �]
// Return        : �]
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
// Description   : Hard Down�J�ڃC�x���g����
// Parameter     : �]
// Return        : �]
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
// Description   : Cycle Down�J�ڃC�x���g����
// Parameter     : �]
// Return        : �]
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
// Description   : �V�[�g���P�[�V�������O�o�͎w������
// Parameter     : para1 (������̍ŏ��ɕt������l)
//               : para2 (������̂Q�Ԗڂɕt������l)
//               : para3 (������̂R�Ԗڂɕt������l)
//               : para4 (������̂S�Ԗڂɕt������l)
// Return        : �]
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





