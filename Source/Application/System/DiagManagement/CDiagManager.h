// File Name:	CDiagManager.h
// Description:	Header File of Diag Manager
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CDiagManager_h
#define CDiagManager_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinDataType.h"
#include "ECdiFinDevCont.h"
#include "ECdiFinMonOpe.h"
#include "ECdiFinNvmArea.h"
#include "ECdiFinComponentStt.h"
#include "ECdiFinExist.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Diag Manager Spec ---
//typedef struct /*DiagManagerSpec*/
//{
//	不要
//} DiagManagerSpec;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CDiagManager_Constructor();

void CDiagManager_DiagnosticsModeStart();
void CDiagManager_DiagnosticsModeEnd();
void CDiagManager_PowerUp();
void CDiagManager_ExecuteDiagDc131Read(US chain, US link);
void CDiagManager_ExecuteDiagDc131Write(US chain, US link, enum ECdiFinDataType dataType, UL nvmValue);
void CDiagManager_ExecuteDiagDc135Read(US chain, US link);
void CDiagManager_ExecuteDiagDc135Reset(US chain, US link, UC reason, enum ECdiFinDevCont devContID);
void CDiagManager_ExecuteDiagDc135Write(US chain, US link, UL lifeSpec);
void CDiagManager_ExecuteDiagDc301Start(US chain, US link, enum ECdiFinDevCont devContID);
void CDiagManager_ExecuteDiagDc330Monitor(US chain, US link, enum ECdiFinMonOpe operation);
void CDiagManager_ExecuteDiagDc330Write(US chain, US link, UC compoState);
void CDiagManager_ExecuteDiagDc361Start(enum ECdiFinNvmArea nvmArea, enum ECdiFinDevCont devContID);
void CDiagManager_ExecuteDiagDc361Read(enum ECdiFinNvmArea nvmArea, enum ECdiFinDevCont devContID);
void CDiagManager_ExecuteDiagDc361Write(enum ECdiFinNvmArea nvmArea, enum ECdiFinDevCont devContID, UC presenceRemain, UC allocateSize, UC* pAllocateValue);
void CDiagManager_ResponseDiagDc131Read(UC result, US chain, US link, UC dataType, UC readAllowed, UC writeAllowed, UC factoryInitAllowed, UL crntVal, UL minVal, UL maxVal);
void CDiagManager_ResponseDiagDc131Write(UC result, US chain, US link, UC updateResponse);
void CDiagManager_ResponseDiagDc135Read(UC result, US chain, US link, UL counterValue, UL lifeSpec, UC reasonLast, UL counterValLast, UC reasonLastButOne, UL counterValLastButOne, UC reasonLastButTwo, UL counterValLastButTwo, UC const* pCounterName, UC counterNameSize);
void CDiagManager_ResponseDiagDc135Reset(UC result, US chain, US link, UC devContID);
void CDiagManager_ResponseDiagDc135Write(UC result, US chain, US link);
void CDiagManager_ResponseDiagDc301Start(UC result, US chain, US link, UC devContID, UC attrInitResp);
void CDiagManager_ResponseDiagDc330Monitor(UC result, US chain, US link, UC componentState, UC transitionCounter);
void CDiagManager_ResponseDiagDc330Write(UC result, US chain, US link, UC componentResponse);
void CDiagManager_ResponseDiagDc361Start(UC result, UC nvmArea, UC devContID, US nvmAreaSize);
void CDiagManager_ResponseDiagDc361Read(UC result, UC nvmArea, UC devContID, UC presenceRemain, UC allocateSize, UC* pAllocateValue);
void CDiagManager_ResponseDiagDc361Write(UC result, UC nvmArea, UC devContID);
void CDiagManager_SendComponentControlStartRequest(US chain, US link);
void CDiagManager_SendComponentControlStopRequest(US chain, US link);
void CDiagManager_ReceiveComponentControlStartNotify(UC moduleID, US chain, US link);
void CDiagManager_ReceiveComponentControlStopNotify(UC moduleID, US chain, US link);


#endif // #ifndef CDiagManager_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//


// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.05.08	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.03.30	| Yuichi Hoshino	| Design Improvement [ NVM Area Access残エリア管理データ初期化タイミング見直し ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
