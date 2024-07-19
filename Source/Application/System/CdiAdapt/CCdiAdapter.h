// File Name:	CCdiAdapter.h
// Description:	Header File of CDI Adapter
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.09
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CCdiAdapter_h
#define CCdiAdapter_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinSubsys.h"
#include "ECdiFinDevCont.h"
#include "ECdiFinOutDest.h"
#include "ECdiFinInSrc.h"
#include "ECdiFinCapaAttrKeyDevCont.h"
#include "ECdiFinCfgAttrKeyDevCont.h"
#include "ECdiFinCapaAttrKeyOutDest.h"
#include "ECdiFinCfgAttrKeyOutDest.h"
#include "ECdiFinCapaAttrKeyInSrc.h"
#include "ECdiFinCfgAttrKeyInSrc.h"
#include "EShtSheetEventID.h"
#include "ECdiFinUpdateResp.h"
#include "ECdiFinInitResp.h"

#include "CCdiFinMessage.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Instance Data ---
typedef struct /*CdiInstanceData*/
{
	UC instance;						// Instance ID
	const UC* pCapaAttrArray;			// Capabilities Attribute Key配列ポインタ
	const UC* pCfgAttrArray;			// Configuration Attribute Key配列ポインタ
} CdiInstanceData;

// --- Jam Zone Status Map ---
typedef struct /*CdiJamZoneStatusMap*/
{
	UC jamZoneID;						// Jam Zone ID
	UC octet;							// Octet of Jam Zone Status set position(メッセージフォーマットのOctet2が(Jam Zone Statusの)0となる)
	UC bitPos;							// Bit of Jam Zone Status set position
} CdiJamZoneStatusMap;

// --- Jam Zone Data ---
typedef struct /*CdiJamZoneData*/
{
	UC devContID;						// Finisher Device Controller ID
	UC zoneIDNumber;					// Number of Jam Zone ID
	UC zoneStatusSize;					// Size of Jam Zone Status[×1Byte]
	const CdiJamZoneStatusMap* pSttsMapArray;
} CdiJamZoneData;

// --- Spec Data of CDI Adapter ---
//typedef struct /*CdiAdapterSpec*/
//{
//	//　不要
//} CdiAdapterSpec;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCdiAdapter_Constructor();

void CCdiAdapter_Sync();
void CCdiAdapter_CompleteSync();
void CCdiAdapter_FailedCommunication();
void CCdiAdapter_SendFinisherState(EFinisherState finState);
void CCdiAdapter_SendInitialInformation();
void CCdiAdapter_SendDeviceCapabilities(enum ECdiFinSubsys subsystem, UC instance, UC attrKey);
void CCdiAdapter_SendDeviceConfiguration(enum ECdiFinSubsys subsystem, UC instance, UC attrKey);
void CCdiAdapter_SendDeviceStatus(US statusNo, US statusValue);
void CCdiAdapter_SendJamZoneStatus(UC devContID);
void CCdiAdapter_SendAbnormalFinishing(UC abortShtNo);
void CCdiAdapter_SendDeviceActionResponse(US eventID);
void CCdiAdapter_SendDeviceEventNotify(US eventID);
void CCdiAdapter_SendSheetActionResponse(UC sheetNo, US eventID);
void CCdiAdapter_SendSheetEventNotify(UC sheetNo, US eventID);
void CCdiAdapter_SendSheetDelivered(UC sheetNo);
void CCdiAdapter_SendSetFinished(UC sheetNo);
void CCdiAdapter_SendSetRecoveryRequest(UC sheetNo);
void CCdiAdapter_SendDiagResponseDc131Read(UC result, US chain, US link, UC dataType, UC readAllowed, UC writeAllowed, UC factoryInitAllowed, UL crntVal, UL minVal, UL maxVal);
void CCdiAdapter_SendDiagResponseDc131Write(UC result, US chain, US link, UC updateResponse);
void CCdiAdapter_SendDiagResponseDc135Read(UC result, US chain, US link, UL counterValue, UL lifeSpec, UC reasonLast, UL counterValLast, UC reasonLastButOne, UL counterValLastButOne, UC reasonLastButTwo, UL counterValLastButTwo, UC const* pCounterName, UC counterNameSize);
void CCdiAdapter_SendDiagResponseDc135Reset(UC result, US chain, US link, UC devContID);
void CCdiAdapter_SendDiagResponseDc135Write(UC result, US chain, US link);
void CCdiAdapter_SendDiagResponseDc301Start(UC result, US chain, US link, UC devContID, UC attrInitResp);
void CCdiAdapter_SendDiagResponseDc330Monitor(UC result, US chain, US link, UC componentState, UC transitionCounter);
void CCdiAdapter_SendDiagResponseDc330Write(UC result, US chain, US link, UC componentResponse);
void CCdiAdapter_SendDiagResponseDc361Start(UC result, UC nvmArea, UC devContID, US nvmAreaSize);
void CCdiAdapter_SendDiagResponseDc361Read(UC result, UC nvmArea, UC devContID, UC presenceRemain, UC allocateSize, UC* pAllocateValue);
void CCdiAdapter_SendDiagResponseDc361Write(UC result, UC nvmArea, UC devContID);
void CCdiAdapter_RcvDevCapabilitiesRequest(CCdiFinMessage* pMessage);
void CCdiAdapter_RcvDevConfigurationRequest(CCdiFinMessage* pMessage);
void CCdiAdapter_RcvSetDevConfiguration(CCdiFinMessage* pMessage);
void CCdiAdapter_RcvInitRequest(CCdiFinMessage* pMessage);
void CCdiAdapter_RcvInitAcknowledge(CCdiFinMessage* pMessage);
void CCdiAdapter_RcvInitComplete(CCdiFinMessage* pMessage);
void CCdiAdapter_RcvPwrOffAnnouncement(CCdiFinMessage* pMessage);
void CCdiAdapter_RcvPollRequest(CCdiFinMessage* pMessage);
void CCdiAdapter_RcvSelectFinAttribute(CCdiFinMessage* pMessage);
void CCdiAdapter_RcvSheetFinRequest(CCdiFinMessage* pMessage);
void CCdiAdapter_RcvChangeShtFinRequest(CCdiFinMessage* pMessage);
void CCdiAdapter_RcvCancelShtFinRequest(CCdiFinMessage* pMessage);
void CCdiAdapter_RcvSheetExit(CCdiFinMessage* pMessage);
void CCdiAdapter_RcvSetFinRequest(CCdiFinMessage* pMessage);
void CCdiAdapter_RcvStopFinisher(CCdiFinMessage* pMessage);
void CCdiAdapter_RcvReqJamSttsCheck(CCdiFinMessage* pMessage);
void CCdiAdapter_RcvDevicePauseReq(CCdiFinMessage* pMessage);
void CCdiAdapter_RcvDevContActionReq(CCdiFinMessage* pMessage);
void CCdiAdapter_RcvChangeDevContMode(CCdiFinMessage* pMessage);
void CCdiAdapter_RcvDeviceStatusRequest(CCdiFinMessage* pMessage);
void CCdiAdapter_RcvSysContInfo(CCdiFinMessage* pMessage);
void CCdiAdapter_RcvExecuteDiag(CCdiFinMessage* pMessage);
#ifndef FXK_TEST
void FwdFinLogDataMsgCdiFin(UC SensorID); 
#endif


#endif // #ifndef CCdiAdapter_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
