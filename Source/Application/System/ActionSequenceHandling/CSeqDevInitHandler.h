// File Name:	CSeqDevInitHandler.h
// Description:	Header File of Device Initialize Handler
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.22
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CSeqDevInitHandler_h
#define CSeqDevInitHandler_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ESeqActionState.h"
#include "ESysPowerUpMode.h"

#include "SRVC_MsgActivity.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Spec data of Device Initialize Handler ---
typedef struct /*SeqDevInitHandlerSpec*/
{
	const SrvcMsgActivity* pPwrOnInitBeforeJamCheckScenario;					// パワーオンイニシャライズ動作(残留紙検出動作前)アクティビティシナリオデータポインタ
	UC pwrOnInitBeforeJamCheckScenarioSize;										// パワーオンイニシャライズ動作(残留紙検出動作前)アクティビティシナリオデータサイズ
	const SrvcMsgActivity* pPwrOnInitAfterJamCheckScenario;						// パワーオンイニシャライズ動作(残留紙検出動作後)アクティビティシナリオデータポインタ
	UC pwrOnInitAfterJamCheckScenarioSize;										// パワーオンイニシャライズ動作(残留紙検出動作後)アクティビティシナリオデータサイズ
	const SrvcMsgActivity* pPwrRsmInitBeforeJamCheckScenario;					// パワーリズームイニシャライズ動作(残留紙検出動作前)アクティビティシナリオデータポインタ
	UC pwrRsmInitBeforeJamCheckScenarioSize;									// パワーリズームイニシャライズ動作(残留紙検出動作前)アクティビティシナリオデータサイズ
	const SrvcMsgActivity* pPwrRsmInitAfterJamCheckScenario;					// パワーリズームイニシャライズ動作(残留紙検出動作後)アクティビティシナリオデータポインタ
	UC pwrRsmInitAfterJamCheckScenarioSize;										// パワーリズームイニシャライズ動作(残留紙検出動作後)アクティビティシナリオデータサイズ
	const SrvcMsgActivity* pNormalInitBeforeJamCheckScenario;					// 通常手順イニシャライズ動作(残留紙検出動作前)アクティビティシナリオデータポインタ
	UC normalInitBeforeJamCheckScenarioSize;									// 通常手順イニシャライズ動作(残留紙検出動作前)アクティビティシナリオデータサイズ
	const SrvcMsgActivity* pNormalInitAfterJamCheckScenario;					// 通常手順イニシャライズ動作(残留紙検出動作後)アクティビティシナリオデータポインタ
	UC normalInitAfterJamCheckScenarioSize;										// 通常手順イニシャライズ動作(残留紙検出動作後)アクティビティシナリオデータサイズ
	const SrvcMsgActivity* pJamCheckScenario;									// 残留紙検出動作アクティビティシナリオデータポインタ
	UC jamCheckScenarioSize;													// 残留紙検出動作アクティビティシナリオデータサイズ
	const SrvcMsgActivity* pJamClrAssistScenario;								// ジャムクリアアシスト動作アクティビティシナリオデータポインタ
	UC jamClrAssistScenarioSize;												// ジャムクリアアシスト動作アクティビティシナリオデータサイズ
} SeqDevInitHandlerSpec;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CSeqDevInitHandler_Constructor();

ESeqActionState CSeqDevInitHandler_GetState();
ESeqActionState CSeqDevInitHandler_Reset();
ESeqActionState CSeqDevInitHandler_Start(ESysPowerUpMode mode);
ESeqActionState CSeqDevInitHandler_Stop();
ESeqActionState CSeqDevInitHandler_DeviceActionResponse(US eventID, UC moduleID);
ESeqActionState CSeqDevInitHandler_Restart();


#endif // #ifndef CSeqDevInitHandler_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
