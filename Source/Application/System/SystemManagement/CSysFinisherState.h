// File Name:	CSysFinisherState.h
// Description:	Header File of Finisher State
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CSysFinisherState_h
#define CSysFinisherState_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ESysPowerUpMode.h"
#include "ESysNvmDataInformerID.h"
#include "ECvrCoverID.h"
#include "ECvrCoverStatus.h"
#include "ESysProcStopType.h"
#include "ESeqActionState.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Device Condition Notify Event ID Convert Table ---
extern US const cSys_DeviceConditionEventID[DEVID_NUM];

// --- Common member structure of Finisher State Class ---
typedef struct /*SysFinisherStateCommonMember*/
{
	US mWaitingModuleBit;
	UC mInitializeReserve;
	UC mAllCoverStatus;
} SysFinisherStateCommonMember;

extern SysFinisherStateCommonMember gSys_FinisherStateCommonMember;

// --- Finisher State ---
typedef struct /*CSysFinisherState*/ {
	// Virtual Method
	void (*Constructor)();
	bool (*IsTransitionState)();
	bool (*CheckTransitionPossibility)(EFinisherState nextState);
	bool (*CheckExitCondition)();
	EFinisherState (*GetNextState)();
	void (*Entry)(EFinisherState prevState);
	void (*Exit)(EFinisherState nextState);
	void (*SendDeviceActionRequest)(US eventID);
	void (*SendDeviceEventNotify)(US eventID);
	bool (*ReceivePowerUpResponse)(UC moduleID);
	bool (*ReceivePowerDownResponse)(UC moduleID);
	bool (*ReceiveChangeDiagResponse)(UC moduleID);
	bool (*ReceiveChangeNormalResponse)(UC moduleID);
	bool (*ReceiveCycleUpResponse)(UC moduleID);
	bool (*ReceiveProcessDownResponse)(UC moduleID);
	bool (*ReceiveDeviceActionResponse)(UC moduleID, US eventID);
	void (*ReceiveDeviceEventNotify)(UC moduleID, US eventID);
	ESysProcStopType (*ReceiveDeviceStatusNotify)(UC moduleID, US statusNo, US value);
	void (*SystemConfigurationSet)();
	bool (*PowerUpModeSet)(ESysPowerUpMode pwrUpMode);
	void (*EntranceAreaTampered)();
	bool (*LoadNvmComplete)(SS result);
	bool (*SaveNvmComplete)(SS result);
	bool (*SheetManagerReady)();
	ESysProcStopType (*CoverStatusChange)(ECvrCoverID coverID, ECvrCoverStatus status);
	ESysProcStopType (*FaultStatusChange)(US statusNo, US value);
	void (*JamZoneStatusChange)(UC devContID);
} CSysFinisherState;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CSysFinisherState_Constructor(apThis) ((apThis)->Constructor())			// void (CSysFinisherState*)

#define CSysFinisherState_IsTransitionState(apThis) ((apThis)->IsTransitionState())
																				// bool (CSysFinisherState*)
#define CSysFinisherState_CheckTransitionPossibility(apThis, aNextState) ((apThis)->CheckTransitionPossibility(aNextState))
																				// bool (CSysFinisherState*, EFinisherState)
#define CSysFinisherState_CheckExitCondition(apThis) ((apThis)->CheckExitCondition())
																				// bool (CSysFinisherState*)
#define CSysFinisherState_GetNextState(apThis) ((apThis)->GetNextState())		// EFinisherState (CSysFinisherState*)
#define CSysFinisherState_Entry(apThis, aPrevState) ((apThis)->Entry(aPrevState))
																				// void (CSysFinisherState*, EFinisherState)
#define CSysFinisherState_Exit(apThis, aNextState) ((apThis)->Exit(aNextState))	// void (CSysFinisherState*, EFinisherState)
#define CSysFinisherState_SendDeviceActionRequest(apThis, aEventID) ((apThis)->SendDeviceActionRequest(aEventID))
																				// void (CSysFinisherState*, US)
#define CSysFinisherState_SendDeviceEventNotify(apThis, aEventID) ((apThis)->SendDeviceEventNotify(aEventID))
																				// void (CSysFinisherState*, US)
#define CSysFinisherState_ReceivePowerUpResponse(apThis, aModuleID) ((apThis)->ReceivePowerUpResponse(aModuleID))
																				// bool (CSysFinisherState*, UC)
#define CSysFinisherState_ReceivePowerDownResponse(apThis, aModuleID) ((apThis)->ReceivePowerDownResponse(aModuleID))
																				// bool (CSysFinisherState*, UC)
#define CSysFinisherState_ReceiveChangeDiagResponse(apThis, aModuleID) ((apThis)->ReceiveChangeDiagResponse(aModuleID))
																				// bool (CSysFinisherState*, UC)
#define CSysFinisherState_ReceiveChangeNormalResponse(apThis, aModuleID) ((apThis)->ReceiveChangeNormalResponse(aModuleID))
																				// bool (CSysFinisherState*, UC)
#define CSysFinisherState_ReceiveCycleUpResponse(apThis, aModuleID) ((apThis)->ReceiveCycleUpResponse(aModuleID))
																				// bool (CSysFinisherState*, UC)
#define CSysFinisherState_ReceiveProcessDownResponse(apThis, aModuleID) ((apThis)->ReceiveProcessDownResponse(aModuleID))
																				// bool (CSysFinisherState*, UC)
#define CSysFinisherState_ReceiveDeviceActionResponse(apThis, aModuleID, aEventID) ((apThis)->ReceiveDeviceActionResponse(aModuleID, aEventID))
																				// bool (CSysFinisherState*, UC, US)
#define CSysFinisherState_ReceiveDeviceEventNotify(apThis, aModuleID, aEventID) ((apThis)->ReceiveDeviceEventNotify(aModuleID, aEventID))
																				// void (CSysFinisherState*, UC, US)
#define CSysFinisherState_ReceiveDeviceStatusNotify(apThis, aModuleID, aStatusNo, aValue) ((apThis)->ReceiveDeviceStatusNotify(aModuleID, aStatusNo, aValue))
																				// ESysProcStopType (CSysFinisherState*, UC, US, US)
#define CSysFinisherState_SystemConfigurationSet(apThis) ((apThis)->SystemConfigurationSet())
																				// void (CSysFinisherState*)
#define CSysFinisherState_PowerUpModeSet(apThis, aPwrUpMode) ((apThis)->PowerUpModeSet(aPwrUpMode))
																				// bool (CSysFinisherState*, ESysPowerUpMode)
#define CSysFinisherState_EntranceAreaTampered(apThis) ((apThis)->EntranceAreaTampered())
																				// void (CSysFinisherState*)
#define CSysFinisherState_LoadNvmComplete(apThis, aResult) ((apThis)->LoadNvmComplete(aResult))
																				// bool (CSysFinisherState*, SS)
#define CSysFinisherState_SaveNvmComplete(apThis, aResult) ((apThis)->SaveNvmComplete(aResult))
																				// bool (CSysFinisherState*, SS)
#define CSysFinisherState_SheetManagerReady(apThis) ((apThis)->SheetManagerReady())
																				// bool (CSysFinisherState*)
#define CSysFinisherState_CoverStatusChange(apThis, aCoverID, aStatus) ((apThis)->CoverStatusChange(aCoverID, aStatus))
																				// ESysProcStopType (CSysFinisherState*, ECvrCoverID, ECvrCoverStatus)
#define CSysFinisherState_FaultStatusChange(apThis, aStatusNo, aValue) ((apThis)->FaultStatusChange(aStatusNo, aValue))
																				// ESysProcStopType (CSysFinisherState*, US, US)
#define CSysFinisherState_JamZoneStatusChange(apThis, aDevContID) ((apThis)->JamZoneStatusChange(aDevContID))
																				// void (CSysFinisherState*, UC)

// --- Virtual Method ---
void CSysFinisherState_StdConstructor();

bool CSysFinisherState_StdIsTransitionState();
bool CSysFinisherState_StdCheckTransitionPossibility(EFinisherState nextState);
bool CSysFinisherState_StdCheckExitCondition();
EFinisherState CSysFinisherState_StdGetNextState();
void CSysFinisherState_StdEntry(EFinisherState prevState);
void CSysFinisherState_StdExit(EFinisherState nextState);
void CSysFinisherState_StdSendDeviceActionRequest(US eventID);
void CSysFinisherState_StdSendDeviceEventNotify(US eventID);
bool CSysFinisherState_StdReceivePowerUpResponse(UC moduleID);
bool CSysFinisherState_StdReceivePowerDownResponse(UC moduleID);
bool CSysFinisherState_StdReceiveChangeDiagResponse(UC moduleID);
bool CSysFinisherState_StdReceiveChangeNormalResponse(UC moduleID);
bool CSysFinisherState_StdReceiveCycleUpResponse(UC moduleID);
bool CSysFinisherState_StdReceiveProcessDownResponse(UC moduleID);
bool CSysFinisherState_StdReceiveDeviceActionResponse(UC moduleID, US eventID);
void CSysFinisherState_StdReceiveDeviceEventNotify(UC moduleID, US eventID);
ESysProcStopType CSysFinisherState_StdReceiveDeviceStatusNotify(UC moduleID, US statusNo, US value);
void CSysFinisherState_StdSystemConfigurationSet();
bool CSysFinisherState_StdPowerUpModeSet(ESysPowerUpMode pwrUpMode);
void CSysFinisherState_StdEntranceAreaTampered();
bool CSysFinisherState_StdLoadNvmComplete(SS result);
bool CSysFinisherState_StdSaveNvmComplete(SS result);
bool CSysFinisherState_StdSheetManagerReady();
ESysProcStopType CSysFinisherState_StdCoverStatusChange(ECvrCoverID coverID, ECvrCoverStatus status);
ESysProcStopType CSysFinisherState_StdFaultStatusChange(US statusNo, US value);
void CSysFinisherState_StdJamZoneStatusChange(UC devContID);

// --- Protected Method ---
ESysProcStopType CSysFinisherState_DeviceStatusChangeProcess(US statusNo, US value);


#endif // #ifndef CSysFinisherState_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.02.08	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 14.12.12	| Yuichi Hoshino	| Trouble Action [ 電源ON時イニシャライズ動作実施せず ]
// 14.12.29	| Yuichi Hoshino	| Design Improvement [ Power Upサブシステム設計見直し ]
// 15.03.30	| Yuichi Hoshino	| Trouble Action [ カバークローズイニシャライズ動作実施せず ]
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVM ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
