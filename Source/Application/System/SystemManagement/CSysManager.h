// File Name:	CSysManager.h
// Description:	Header File of System Manager
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CSysManager_h
#define CSysManager_h

////////////////////////////////////////////////////////////
// Include File
////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ESysPowerUpMode.h"
#include "ESysNvmDataInformerID.h"
#include "ECvrCoverID.h"
#include "ECvrCoverStatus.h"
#include "ECdiFinDevCont.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CSysManager_Constructor();

EFinisherState CSysManager_GetFinisherState();
bool CSysManager_CheckInformationDetect();
void CSysManager_PowerUp();
void CSysManager_PowerDown();
void CSysManager_ChangeNormal();
void CSysManager_ChangeDiag();
void CSysManager_CycleUp();
void CSysManager_CycleDown();
void CSysManager_HardDown();
void CSysManager_SendDeviceActionRequest(US eventID);
void CSysManager_SendDeviceEventNotify(US eventID);
void CSysManager_ReceivePowerUpResponse(UC moduleID);
void CSysManager_ReceivePowerDownResponse(UC moduleID);
void CSysManager_ReceiveChangeDiagResponse(UC moduleID);
void CSysManager_ReceiveChangeNormalResponse(UC moduleID);
void CSysManager_ReceiveCycleUpResponse(UC moduleID);
void CSysManager_ReceiveProcessDownResponse(UC moduleID);
void CSysManager_ReceiveDeviceActionResponse(UC moduleID, US eventID);
void CSysManager_ReceiveDeviceEventNotify(UC moduleID, US eventID);
void CSysManager_ReceiveDeviceStatusNotify(UC moduleID, US statusNo, US value);
void CSysManager_SystemConfigurationSet();
void CSysManager_PowerUpModeSet(ESysPowerUpMode pwrUpMode);
void CSysManager_EntranceAreaTampered();
void CSysManager_LoadNvmComplete(SS result);
void CSysManager_SaveNvmComplete(SS result);
void CSysManager_SheetManagerReady();
void CSysManager_CoverStatusChange(ECvrCoverID coverID, ECvrCoverStatus status);
void CSysManager_FaultStatusChange(US statusNo, US value);
void CSysManager_JamZoneStatusChange(UC devContID);


#endif // #ifndef CSysManager_h

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
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVM ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
