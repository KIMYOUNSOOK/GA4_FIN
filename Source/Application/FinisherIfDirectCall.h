// File Name:	FinisherIfDirectCall.h
// Description:	Header File of Finisher Subsytstem I/F Direct Call Prototype Definition
// Engineer:	Xxxxxx Xxxxxx
// Last Edit:	XX.XX.XX
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef FinisherIfDirectCall_h
#define FinisherIfDirectCall_h

////////////////////////////////////////////////////////////
// Include Common Header File
////////////////////////////////////////////////////////////

#include "EShtEject.h"
#include "EShtOffsetFinish.h"


////////////////////////////////////////////////////////////
// I/F Direct Call Prottype Definition
////////////////////////////////////////////////////////////

// --- Device Parameter ---
//UL AAA_GetBbb(void);

// --- Eject Execution Sheet Request ---
//EShtEject AAA_GetEjectExecutionSheetReq(UC sheetNo);
EShtEject CMPL_GetEjectExecutionSheetReq(UC sheetNo);

// --- Eject Execution Next Sheet Request ---
//EShtEject AAA_GetEjectExecutionNextSheetReq(UC sheetNo, UC nextShtNo);
EShtEject CMPL_GetEjectExecutionNextSheetReq(UC sheetNo, UC nextShtNo);

// --- Eject Execution Standby ---
//EShtEject AAA_GetEjectExecutionStandby(UC sheetNo);
EShtEject CMPL_GetEjectExecutionStandby(UC sheetNo);

// --- Weighted Compile Count ---
//US AAA_GetWeightedCompileCount(UC sheetNo);
US CMPL_GetWeightedCompileCount(UC sheetNo);

// --- Offset Finish Position ---
//EShtOffsetFinish AAA_GetOffsetFinishPosition(UC sheetNo);
EShtOffsetFinish CMPL_GetOffsetFinishPosition(UC sheetNo);

// --- Sheet Initialize Time ---
//US AAA_GetBbbSheetInitializeTimeCcc(UC sheetNo, UC lastShtNo);
US CMPL_GetCompilerSheetInitializeTimeStep1(UC sheetNo, UC lastShtNo);
US PNCH_GetPuncherSheetInitializeTimeStep1(UC sheetNo, UC lastShtNo);

// --- Sheet Parameter ---
//UL AAA_GetBbb(UC sheetNo);
// PreDVT: V3.0.0
US CMPL_GetBase_CompileFinishingNeedTime(UC sheetNo);  //��{����[ms] : SetPitch
US  CMPL_GetAdd_CompileFinishingNeedTime(UC sheetNo); // ���Z���� :  �R���p�C���t�B�j�b�V���O���쎞��
US  CMPL_GetAdd_TamperOpenNeedTime(UC  sheetNo); //���Z���� :  �^���p�[����������K�v����
US  CMPL_GetAdd_SubPaddleNeedTime(UC sheetNo);  //���Z���� :  �T�u�p�h���p������ ���̓���񐔂�3��̏ꍇ�̉��Z����     
US  CMPL_GetEjectHomeNeedTime(UC sheetNo); //EjectHome ���쎞�� (V3.6.2) 
US  CMPL_GetEjectNeedTime(UC sheetNo); //Eject ���쎞�� (V3.6.2) 

// --- Action Mode ---
//UC AAA_GetActionModeBbb(UC sheetNo);
UC CMPL_GetActionModeTamperOpen(UC sheetNo);

#endif // #ifndef FinisherIfDirectCall_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
