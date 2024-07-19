// File Name:	CCdiAdapter.c
// Description:	Implimentation File of CDI Adapter
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCdiAdapter.h"

#include "CCdiSysOpeMode.h"
#include "CCdiProductionState.h"
#include "CCdiTransitionState.h"
#include "CShtSheetAttribute.h"

#include "CCdiFinMessage.h"
#include "CCdiFinDevCapaReqMsg.h"
#include "CCdiFinDevCapaUpdateMsg.h"
#include "CCdiFinDevCfgReqMsg.h"
#include "CCdiFinDevCfgUpdateMsg.h"
#include "CCdiFinSetDevCfgMsg.h"
#include "CCdiFinInitReqMsg.h"
#include "CCdiFinInitAckMsg.h"
#include "CCdiFinPwrOffAckMsg.h"
#include "CCdiFinSlctFinAttrMsg.h"
#include "CCdiFinShtFinReqMsg.h"
#include "CCdiFinRdyForNxtShtMsg.h"
#include "CCdiFinChgShtFinReqMsg.h"
#include "CCdiFinCnclShtFinReqMsg.h"
#include "CCdiFinSheetExitMsg.h"
#include "CCdiFinShtDelvToOutMsg.h"
#include "CCdiFinSetFinReqMsg.h"
#include "CCdiFinSetFinAtOutMsg.h"
#include "CCdiFinStopFinMsg.h"
#include "CCdiFinSetRecoveryMsg.h"
#include "CCdiFinJamSttsCheckMsg.h"
#include "CCdiFinDevPauseReqMsg.h"
#include "CCdiFinDevContSttUpdateMsg.h"
#include "CCdiFinChgDevContModeMsg.h"
#include "CCdiFinDevContModeUpdateMsg.h"
#include "CCdiFinDevSttsReqMsg.h"
#include "CCdiFinDevSttsUpdateMsg.h"
#include "CCdiFinJamZoneUpdateMsg.h"
#include "CCdiFinAbnormalFinishingMsg.h"
#include "CCdiFinSysContInfoMsg.h"
#include "CCdiFinDevContInfoMsg.h"
#include "CCdiFinExecDiagMsg.h"
#include "CCdiFinDiagRespMsg.h"
#ifndef FXK_TEST
#include "CCdiFinLogDataMsg.h"
#include "CCdiFinLogDataInfoCommInCdiFin.h"
#endif

#include "CCdiFinDevContCapabilitiesAttr.h"
#include "CCdiFinOutDestCapabilitiesAttr.h"
#include "CCdiFinInSrcCapabilitiesAttr.h"
#include "CCdiFinDevContConfigurationAttr.h"
#include "CCdiFinOutDestConfigurationAttr.h"
#include "CCdiFinInSrcConfigurationAttr.h"
#include "CCdiFinSheetFinishAttr.h"

#include "CCdiFinDiagDataExecDc131Read.h"
#include "CCdiFinDiagDataExecDc131Write.h"
#include "CCdiFinDiagDataExecDc301Start.h"
#include "CCdiFinDiagDataExecDc361Start.h"
#include "CCdiFinDiagDataExecDc361Read.h"
#include "CCdiFinDiagDataExecDc361Write.h"
#include "CCdiFinDiagDataExecDc135Read.h"
#include "CCdiFinDiagDataExecDc135Reset.h"
#include "CCdiFinDiagDataExecDc135Write.h"
#include "CCdiFinDiagDataExecDc330Monitor.h"
#include "CCdiFinDiagDataExecDc330Write.h"

#include "CCdiFinDiagDataRespDc131Read.h"
#include "CCdiFinDiagDataRespDc131Write.h"
#include "CCdiFinDiagDataRespDc301Start.h"
#include "CCdiFinDiagDataRespDc361Start.h"
#include "CCdiFinDiagDataRespDc361Read.h"
#include "CCdiFinDiagDataRespDc361Write.h"
#include "CCdiFinDiagDataRespDc135Read.h"
#include "CCdiFinDiagDataRespDc135Reset.h"
#include "CCdiFinDiagDataRespDc135Write.h"
#include "CCdiFinDiagDataRespDc330Monitor.h"
#include "CCdiFinDiagDataRespDc330Write.h"

#include "CSysManager.h"
#include "CDiagManager.h"
#include "CShtManager.h"
#include "CJamChecker.h"
#include "CCapaInformer.h"
#include "CCfgInformer.h"
#include "CDvstInformer.h"
#include "CDltManager.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Enumeration of CDI Adapter Event ID ---
typedef enum ECdiAdapterEvent
{
	CDI_ADAPT_EVT_SEND_INIT_INFO,
	CDI_ADAPT_EVT_CAPA_REQ,
	CDI_ADAPT_EVT_CFG_REQ,
	CDI_ADAPT_EVT_DEV_STTS_REQ,
	CDI_ADAPT_EVT_JAM_ZONE_UPDT,
	CDI_ADAPT_EVT_NUM
} ECdiAdapterEvent;

#define CDI_ADAPT_EVT_CNV_BIT(event)	(0x01 << event)

// --- Instance Data of Finisher Device Controller ---
extern CdiInstanceData const cCdi_DevContDataArray[];

// --- Instance Data of Output Destination ---
extern CdiInstanceData const cCdi_OutDestDataArray[];

// --- Instance Data of Input Source ---
extern CdiInstanceData const cCdi_InSrcDataArray[];

// --- Configuration Update All Response Macro ---
#define CDI_CFG_UPDATE_ALL_CONV_BYTE(attrKey)	((UC)(attrKey) / 8)
#define CDI_CFG_UPDATE_ALL_CONV_BIT(attrKey)	((UC)0x01 << ((attrKey) % 8))

// --- Finisher Device Controller Configuration Update All Response ---	※ Update All Requestに対して付加するAttribute DataのBitがセットされたデータ(Update All時に付加されるAttribute追加時は編集が必要)
static UC const cCdi_DevContCfgUpdateAll[(256 / 8)] = {
	0x00,		// Attr Key = 0x00 ～ 0x07
	0x00,		// Attr Key = 0x08 ～ 0x0f
	0x00,		// Attr Key = 0x10 ～ 0x17
	0x00,		// Attr Key = 0x18 ～ 0x1f
	0x00,		// Attr Key = 0x20 ～ 0x27
	0x00,		// Attr Key = 0x28 ～ 0x2f
	0x00,		// Attr Key = 0x30 ～ 0x37
	0x00,		// Attr Key = 0x38 ～ 0x3f
	0x00,		// Attr Key = 0x40 ～ 0x47
	0x00,		// Attr Key = 0x48 ～ 0x4f
	0x00,		// Attr Key = 0x50 ～ 0x57
	0x00,		// Attr Key = 0x58 ～ 0x5f
	0x00,		// Attr Key = 0x60 ～ 0x67
	0x00,		// Attr Key = 0x68 ～ 0x6f
	0x00,		// Attr Key = 0x70 ～ 0x77
	0x00,		// Attr Key = 0x78 ～ 0x7f
	0x00,		// Attr Key = 0x80 ～ 0x87
	0x00,		// Attr Key = 0x88 ～ 0x8f
	0x00,		// Attr Key = 0x90 ～ 0x97
	0x00,		// Attr Key = 0x98 ～ 0x9f
	0x00,		// Attr Key = 0xa0 ～ 0xa7
	0x00,		// Attr Key = 0xa8 ～ 0xaf
	0x00,		// Attr Key = 0xb0 ～ 0xb7
	0x00,		// Attr Key = 0xb8 ～ 0xbf
	0x00,		// Attr Key = 0xc0 ～ 0xc7
	0x00,		// Attr Key = 0xc8 ～ 0xcf
	0x00,		// Attr Key = 0xd0 ～ 0xd7
	0x00,		// Attr Key = 0xd8 ～ 0xdf
	0x00,		// Attr Key = 0xe0 ～ 0xe7
	0x00,		// Attr Key = 0xe8 ～ 0xef
	0x00,		// Attr Key = 0xf0 ～ 0xf7
	0x00,		// Attr Key = 0xf8 ～ 0xff
};

// --- Output Destination Configuration Update All Response ---	※ Update All Requestに対して付加するAttribute DataのBitがセットされたデータ(Update All時に付加されるAttribute追加時は編集が必要)
#if OUT_DEST_NUM != 0
static UC const cCdi_OutDestCfgUpdateAll[(256/8)] = {
	CDI_CFG_UPDATE_ALL_CONV_BIT(eCdiFinCfgKeyOutDest_Select)|
	0x00,		// Attr Key = 0x00 ～ 0x07
	0x00,		// Attr Key = 0x08 ～ 0x0f
	0x00,		// Attr Key = 0x10 ～ 0x17
	0x00,		// Attr Key = 0x18 ～ 0x1f
	0x00,		// Attr Key = 0x20 ～ 0x27
	0x00,		// Attr Key = 0x28 ～ 0x2f
	0x00,		// Attr Key = 0x30 ～ 0x37
	0x00,		// Attr Key = 0x38 ～ 0x3f
	CDI_CFG_UPDATE_ALL_CONV_BIT(eCdiFinCfgKeyOutDest_Offset)|
	// CDI_CFG_UPDATE_ALL_CONV_BIT(eCdiFinCfgKeyOutDest_Fold)| // 20150901 LJE
	CDI_CFG_UPDATE_ALL_CONV_BIT(eCdiFinCfgKeyOutDest_Staple)|
	0x00,		// Attr Key = 0x40 ～ 0x47
	// CDI_CFG_UPDATE_ALL_CONV_BIT(eCdiFinCfgKeyOutDest_Punch)|
	// CDI_CFG_UPDATE_ALL_CONV_BIT(eCdiFinCfgKeyOutDest_Book)|
	0x00,		// Attr Key = 0x48 ～ 0x4f
	0x00,		// Attr Key = 0x50 ～ 0x57
	0x00,		// Attr Key = 0x58 ～ 0x5f
	0x00,		// Attr Key = 0x60 ～ 0x67
	0x00,		// Attr Key = 0x68 ～ 0x6f
	0x00,		// Attr Key = 0x70 ～ 0x77
	0x00,		// Attr Key = 0x78 ～ 0x7f
	0x00,		// Attr Key = 0x80 ～ 0x87
	0x00,		// Attr Key = 0x88 ～ 0x8f
	0x00,		// Attr Key = 0x90 ～ 0x97
	0x00,		// Attr Key = 0x98 ～ 0x9f
	0x00,		// Attr Key = 0xa0 ～ 0xa7
	0x00,		// Attr Key = 0xa8 ～ 0xaf
	0x00,		// Attr Key = 0xb0 ～ 0xb7
	0x00,		// Attr Key = 0xb8 ～ 0xbf
	// CDI_CFG_UPDATE_ALL_CONV_BIT(eCdiFinCfgKeyOutDest_Trimming)|// 20150901 LJE
	// CDI_CFG_UPDATE_ALL_CONV_BIT(eCdiFinCfgKeyOutDest_StplCartridge)|// 20150901 LJE
	0x00,		// Attr Key = 0xc0 ～ 0xc7
	0x00,		// Attr Key = 0xc8 ～ 0xcf
	0x00,		// Attr Key = 0xd0 ～ 0xd7
	0x00,		// Attr Key = 0xd8 ～ 0xdf
	0x00,		// Attr Key = 0xe0 ～ 0xe7
	0x00,		// Attr Key = 0xe8 ～ 0xef
	0x00,		// Attr Key = 0xf0 ～ 0xf7
	0x00,		// Attr Key = 0xf8 ～ 0xff
};
#endif	// OUT_DEST_NUM != 0

// --- Input Source Configuration Update All Response ---	※ Update All Requestに対して付加するAttribute DataのBitがセットされたデータ(Update All時に付加されるAttribute追加時は編集が必要)
#if IN_SRC_NUM != 0
static UC const cCdi_InSrcCfgUpdateAll[(256/8)] = {
	CDI_CFG_UPDATE_ALL_CONV_BIT(eCdiFinCfgKeyInSrc_Select)|
	0x00,		// Attr Key = 0x00 ～ 0x07
	0x00,		// Attr Key = 0x08 ～ 0x0f
	0x00,		// Attr Key = 0x10 ～ 0x17
	0x00,		// Attr Key = 0x18 ～ 0x1f
	0x00,		// Attr Key = 0x20 ～ 0x27
	0x00,		// Attr Key = 0x28 ～ 0x2f
	0x00,		// Attr Key = 0x30 ～ 0x37
	0x00,		// Attr Key = 0x38 ～ 0x3f
	0x00,		// Attr Key = 0x40 ～ 0x47
	0x00,		// Attr Key = 0x48 ～ 0x4f
	0x00,		// Attr Key = 0x50 ～ 0x57
	0x00,		// Attr Key = 0x58 ～ 0x5f
	0x00,		// Attr Key = 0x60 ～ 0x67
	0x00,		// Attr Key = 0x68 ～ 0x6f
	0x00,		// Attr Key = 0x70 ～ 0x77
	0x00,		// Attr Key = 0x78 ～ 0x7f
	0x00,		// Attr Key = 0x80 ～ 0x87
	0x00,		// Attr Key = 0x88 ～ 0x8f
	0x00,		// Attr Key = 0x90 ～ 0x97
	0x00,		// Attr Key = 0x98 ～ 0x9f
	0x00,		// Attr Key = 0xa0 ～ 0xa7
	0x00,		// Attr Key = 0xa8 ～ 0xaf
	0x00,		// Attr Key = 0xb0 ～ 0xb7
	0x00,		// Attr Key = 0xb8 ～ 0xbf
	CDI_CFG_UPDATE_ALL_CONV_BIT(eCdiFinCfgKeyInSrc_SizeAlt)|
	0x00,		// Attr Key = 0xc0 ～ 0xc7
	0x00,		// Attr Key = 0xc8 ～ 0xcf
	0x00,		// Attr Key = 0xd0 ～ 0xd7
	0x00,		// Attr Key = 0xd8 ～ 0xdf
	0x00,		// Attr Key = 0xe0 ～ 0xe7
	0x00,		// Attr Key = 0xe8 ～ 0xef
	0x00,		// Attr Key = 0xf0 ～ 0xf7
	0x00,		// Attr Key = 0xf8 ～ 0xff
};
#endif	// IN_SRC_NUM != 0
// --- Jam Zone Data ---
extern CdiJamZoneData const cCdi_JamZoneDataArray[];

// --- Member of CDI Adapter ---
typedef struct /*CCdiAdapterMember*/
{
	CCdiSysOpeMode mSysOpeMode;
	CCdiProductionState mProductState;
	CCdiTransitionState mTransState;
	UC mEventBit;
	CShtSheetAttribute mSheetAttr;
} CCdiAdapterMember;

static CCdiAdapterMember gCdi_AdapterMember;

/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

static void ExportDevCapaContAttr03(CCapaDevContCapabilities* pCapa, CCdiFinDevContCapabilitiesAttr* pAttr);
static void ExportDevCapaDestAttr02(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr);
static void ExportDevCapaDestAttr04(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr);
static void ExportDevCapaDestAttr47(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr);
static void ExportDevCapaDestAttr4B(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr);
static void ExportDevCapaDestAttr50(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr);
static void ExportDevCapaDestAttr55(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr);
static void ExportDevCapaDestAttr5B(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr);
static void ExportDevCapaDestAttr5E(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr);
static void ExportDevCapaDestAttrC2(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr);
static void ExportDevCapaDestAttrC6(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr);
static void ExportDevCapaDestAttrC7(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr);
static void ExportDevCapaDestAttrC8(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr);
static void ExportDevCapaSrcAttr01(CCapaInSrcCapabilities* pCapa, CCdiFinInSrcCapabilitiesAttr* pAttr);
static void ExportDevCapaSrcAttr04(CCapaInSrcCapabilities* pCapa, CCdiFinInSrcCapabilitiesAttr* pAttr);
static void ExportDevCfgContAttr01(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr);
static void ExportDevCfgContAttr40(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr);
static void ExportDevCfgContAttr41(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr);
static void ExportDevCfgContAttrC0(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr);
static void ExportDevCfgContAttrFA(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr);
static void ExportDevCfgContAttrFB(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr);
static void ExportDevCfgContAttrFC(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr);
static void ExportDevCfgContAttrFD(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr);
static void ExportDevCfgContAttrFE(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr);
static void ExportDevCfgContAttrFF(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr);
static void ExportDevCfgDestAttr03(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr);
static void ExportDevCfgDestAttr45(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr);
static void ExportDevCfgDestAttr46(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr);
static void ExportDevCfgDestAttr47(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr);
static void ExportDevCfgDestAttr48(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr);
static void ExportDevCfgDestAttr49(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr);
static void ExportDevCfgSrcAttr03(CCfgInSrcConfiguration* pCfg, CCdiFinInSrcConfigurationAttr* pAttr);
static void ExportDevCfgSrcAttrC0(CCfgInSrcConfiguration* pCfg, CCdiFinInSrcConfigurationAttr* pAttr);
static void ExportDevCfgSrcAttrC1(CCfgInSrcConfiguration* pCfg, CCdiFinInSrcConfigurationAttr* pAttr);
static void ImportDevCfgContAttr01(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr);
static void ImportDevCfgContAttr40(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr);
static void ImportDevCfgContAttr41(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr);
static void ImportDevCfgContAttrC0(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr);
static void ImportDevCfgContAttrFA(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr);
static void ImportDevCfgContAttrFB(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr);
static void ImportDevCfgContAttrFC(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr);
static void ImportDevCfgContAttrFD(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr);
static void ImportDevCfgContAttrFE(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr);
static void ImportDevCfgContAttrFF(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr);
static void ImportDevCfgDestAttr03(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr);
static void ImportDevCfgDestAttr45(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr);
static void ImportDevCfgDestAttr46(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr);
static void ImportDevCfgDestAttr47(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr);
static void ImportDevCfgDestAttr48(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr);
static void ImportDevCfgDestAttr49(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr);
static void ImportDevCfgSrcAttr03(CCfgInSrcConfiguration* pCfg, CCdiFinInSrcConfigurationAttr* pAttr);
static void ImportDevCfgSrcAttrC0(CCfgInSrcConfiguration* pCfg, CCdiFinInSrcConfigurationAttr* pAttr);
static void ImportDevCfgSrcAttrC1(CCfgInSrcConfiguration* pCfg, CCdiFinInSrcConfigurationAttr* pAttr);
static bool SetDevCapabilitiesAttributeDevCont(UC attrKey, UC instance, CCdiFinDevCapaUpdateMsg* pMessage);
static bool SetDevCapabilitiesAttributeOutDest(UC attrKey, UC instance, CCdiFinDevCapaUpdateMsg* pMessage);
static bool SetDevCapabilitiesAttributeInSrc(UC attrKey, UC instance, CCdiFinDevCapaUpdateMsg* pMessage);
static bool SetDevConfigurationAttributeDevCont(UC attrKey, UC instance, CCdiFinDevCfgUpdateMsg* pMessage);
static bool SetDevConfigurationAttributeOutDest(UC attrKey, UC instance, CCdiFinDevCfgUpdateMsg* pMessage);
static bool SetDevConfigurationAttributeInSrc(UC attrKey, UC instance, CCdiFinDevCfgUpdateMsg* pMessage);
static void SetSheetAttribute(CShtSheetAttribute* pShtAttr, CCdiFinSheetFinishAttr* pShtFinAttr);

static void FwdDevCapabilitiesUpdate(UC subsystem, UC instance, UC attrKey);
static void FwdDevConfigurationUpdate(UC subsystem, UC instance, UC attrKey);
static void FwdInitRequest();
static void FwdInitAcknowledge();
static void FwdPwrOffAcknowledge();
static void FwdReadyForNextSheet(UC sheetNo);
static void FwdSheetFed(UC sheetNo);
static void FwdFeedAborted(UC sheetNo);
static void FwdSheetDelvToOutput(UC sheetNo);
static void FwdSetFinAtOutput(UC sheetNo);
static void FwdSetRecoveryReq(UC sheetNo);
static void FwdReportPauseStts(UC lineStts, UC request);
static void FwdDevContStateUpdate();
static void FwdDevContModeUpdate();
static void FwdDevSttsUpdate(US statusNo, US statusValue);
static void FwdJamZoneUpdate(UC devContID);
static void FwdAbnormalFinishing(UC abortShtNo);
static void FwdDevContInfoFaultSheet(UC sheetNo);
static void FwdDevContInfoCancelStaple(UC sheetNo);
static void FwdDevContInfoPbMediaMismatch(UC sheetNo);
static void FwdDevContInfoDynamicSample(UC devContID);
static void FwdDevContInfoCommInCdiFin();

static void FwdDiagResponse(US diagProgram, UC diagCommand, UC result);
static void FwdDiagResponseDc131Read(UC result, US chain, US link, UC dataType, UC readAllowed, UC writeAllowed, UC factoryInitAllowed, UL crntVal, UL minVal, UL maxVal);
static void FwdDiagResponseDc131Write(UC result, US chain, US link, UC updateResponse);
static void FwdDiagResponseDc301Start(UC result, US chain, US link, UC devContID, UC attrInitResp);
static void FwdDiagResponseDc361Start(UC result, UC nvmArea, UC devContID, US nvmAreaSize);
static void FwdDiagResponseDc361Read(UC result, UC nvmArea, UC devContID, UC presenceRemain, UC allocateSize, UC* pAllocateValue);
static void FwdDiagResponseDc361Write(UC result, UC nvmArea, UC devContID);
static void FwdDiagResponseDc135Read(UC result, US chain, US link, UL counterValue, UL lifeSpec, UC reasonLast, UL counterValLast, UC reasonLastButOne, UL counterValLastButOne, UC reasonLastButTwo, UL counterValLastButTwo, UC const* pCounterName, UC counterNameSize);
static void FwdDiagResponseDc135Reset(UC result, US chain, US link, UC devContID);
static void FwdDiagResponseDc135Write(UC result, US chain, US link);
static void FwdDiagResponseDc330Monitor(UC result, US chain, US link, UC componentState, UC transitionCounter);
static void FwdDiagResponseDc330Write(UC result, US chain, US link, UC componentResponse);
static void RcvSysContInfoCommInCdiFin(CCdiFinSysContInfoMsg* pMessage);
static void RcvExecuteDiagDc131Read(const CCdiFinExecDiagMsg* pMessage);
static void RcvExecuteDiagDc131Write(const CCdiFinExecDiagMsg* pMessage);
static void RcvExecuteDiagDc301Start(const CCdiFinExecDiagMsg* pMessage);
static void RcvExecuteDiagDc361Start(const CCdiFinExecDiagMsg* pMessage);
static void RcvExecuteDiagDc361Read(const CCdiFinExecDiagMsg* pMessage);
static void RcvExecuteDiagDc361Write(const CCdiFinExecDiagMsg* pMessage);
static void RcvExecuteDiagDc135Read(const CCdiFinExecDiagMsg* pMessage);
static void RcvExecuteDiagDc135Reset(const CCdiFinExecDiagMsg* pMessage);
static void RcvExecuteDiagDc135Write(const CCdiFinExecDiagMsg* pMessage);
static void RcvExecuteDiagDc140Monitor(const CCdiFinExecDiagMsg* pMessage);
static void RcvExecuteDiagDc330Monitor(const CCdiFinExecDiagMsg* pMessage);
static void RcvExecuteDiagDc330Write(const CCdiFinExecDiagMsg* pMessage);
static void RcvExecuteDiagDc726Read(const CCdiFinExecDiagMsg* pMessage);
static void RcvExecuteDiagDc726Monitor(const CCdiFinExecDiagMsg* pMessage);
static void RcvExecuteDiagDc1100Read(const CCdiFinExecDiagMsg* pMessage);
static void RcvExecuteDiagDc1201Read(const CCdiFinExecDiagMsg* pMessage);
static void RcvExecuteDiagDc1201Write(const CCdiFinExecDiagMsg* pMessage);
static void RcvExecuteDiagDc1202Read(const CCdiFinExecDiagMsg* pMessage);
static void RcvExecuteDiagDc1202Write(const CCdiFinExecDiagMsg* pMessage);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_Constructor()
{
	gCdi_AdapterMember.mEventBit = 0x00;
	CShtSheetAttribute_Constructor(&gCdi_AdapterMember.mSheetAttr);

	CCdiSysOpeMode_Constructor(&(gCdi_AdapterMember.mSysOpeMode));
	CCdiProductionState_Constructor(&(gCdi_AdapterMember.mProductState));
	CCdiTransitionState_Constructor(&(gCdi_AdapterMember.mTransState));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCapaContAttr03
// Description   : Finisher Device Controller Capabilities Attr 
//               : "Device Controller Software Version"情報取り出し
//               : 処理
// Parameter     : pCapa (Device Controller Capabilitiesインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCapaContAttr03(CCapaDevContCapabilities* pCapa, CCdiFinDevContCapabilitiesAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinDevContCapabilitiesAttr_AllocateAttrPair(pAttr, CCdiFinCapaAttrDevCont03_GetAttributeKey(), CCdiFinCapaAttrDevCont03_GetAttributeSize(NULL));
	pAttrData = CCdiFinDevContCapabilitiesAttr_GetAttributePointer(pAttr, CCdiFinCapaAttrDevCont03_GetAttributeKey());

	CCdiFinCapaAttrDevCont03_SetMajorVersion(pAttrData, CCapaDevContCapabilities_GetMajorVersion(pCapa));
	CCdiFinCapaAttrDevCont03_SetMinorVersion(pAttrData, CCapaDevContCapabilities_GetMinorVersion(pCapa));
	CCdiFinCapaAttrDevCont03_SetPatchVersion(pAttrData, CCapaDevContCapabilities_GetPatchVersion(pCapa));
}
#if OUT_DEST_NUM != 0

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCapaDestAttr02
// Description   : Output Destination Capabilities Attr "Output 
//               : Max Capacity"情報取り出し処理
// Parameter     : pCapa (Output Destination Capabilitiesインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCapaDestAttr02(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinOutDestCapabilitiesAttr_AllocateAttrPair(pAttr, CCdiFinCapaAttrOutDest02_GetAttributeKey(), CCdiFinCapaAttrOutDest02_GetAttributeSize(NULL));
	pAttrData = CCdiFinOutDestCapabilitiesAttr_GetAttributePointer(pAttr, CCdiFinCapaAttrOutDest02_GetAttributeKey());

	CCdiFinCapaAttrOutDest02_SetMaxSmallSizeCapacity(pAttrData, CCapaOutDestCapabilities_GetOutputMaxCapacity(pCapa, CAPA_SIZE_CTGRY_SMALL));
	CCdiFinCapaAttrOutDest02_SetMaxLargeSizeCapacity(pAttrData, CCapaOutDestCapabilities_GetOutputMaxCapacity(pCapa, CAPA_SIZE_CTGRY_LARGE));
	CCdiFinCapaAttrOutDest02_SetMixedSizeCapacity(pAttrData, CCapaOutDestCapabilities_GetOutputMaxCapacity(pCapa, CAPA_SIZE_CTGRY_MIXED));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCapaDestAttr04
// Description   : Output Destination Capabilities Attr "Output 
//               : Destination Unit Type"情報取り出し処理
// Parameter     : pCapa (Output Destination Capabilitiesインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCapaDestAttr04(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinOutDestCapabilitiesAttr_AllocateAttrPair(pAttr, CCdiFinCapaAttrOutDest04_GetAttributeKey(), CCdiFinCapaAttrOutDest04_GetAttributeSize(NULL));
	pAttrData = CCdiFinOutDestCapabilitiesAttr_GetAttributePointer(pAttr, CCdiFinCapaAttrOutDest04_GetAttributeKey());

	CCdiFinCapaAttrOutDest04_SetDeviceUnitType(pAttrData, CCapaOutDestCapabilities_GetDeviceUnitType(pCapa));
	CCdiFinCapaAttrOutDest04_SetPurge(pAttrData, CCapaOutDestCapabilities_GetPurgeOutput(pCapa));
	CCdiFinCapaAttrOutDest04_SetVirtual(pAttrData, CCapaOutDestCapabilities_GetVirtualOutput(pCapa));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCapaDestAttr47
// Description   : Output Destination Capabilities Attr " Offset 
//               : Ability"情報取り出し処理
// Parameter     : pCapa (Output Destination Capabilitiesインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCapaDestAttr47(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinOutDestCapabilitiesAttr_AllocateAttrPair(pAttr, CCdiFinCapaAttrOutDest47_GetAttributeKey(), CCdiFinCapaAttrOutDest47_GetAttributeSize(NULL));
	pAttrData = CCdiFinOutDestCapabilitiesAttr_GetAttributePointer(pAttr, CCdiFinCapaAttrOutDest47_GetAttributeKey());

	CCdiFinCapaAttrOutDest47_SetSelectability(pAttrData, CCapaOutDestCapabilities_GetOffsetSelectability(pCapa));
	CCdiFinCapaAttrOutDest47_Set3PositionOffset(pAttrData, CCapaOutDestCapabilities_Get3PositionOffsetSupport(pCapa));
	CCdiFinCapaAttrOutDest47_SetOffsetMethod(pAttrData, CCapaOutDestCapabilities_GetOffsetMethod(pCapa));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCapaDestAttr4B
// Description   : Output Destination Capabilities Attr "Folding 
//               : Ability"情報取り出し処理
// Parameter     : pCapa (Output Destination Capabilitiesインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCapaDestAttr4B(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinOutDestCapabilitiesAttr_AllocateAttrPair(pAttr, CCdiFinCapaAttrOutDest4B_GetAttributeKey(), CCdiFinCapaAttrOutDest4B_GetAttributeSize());
	pAttrData = CCdiFinOutDestCapabilitiesAttr_GetAttributePointer(pAttr, CCdiFinCapaAttrOutDest4B_GetAttributeKey());

	CCdiFinCapaAttrOutDest4B_SetSelectability(pAttrData, CCapaOutDestCapabilities_GetFoldingSelectability(pCapa));
	CCdiFinCapaAttrOutDest4B_SetZFoldSupport(pAttrData, CCapaOutDestCapabilities_GetZFoldSupport(pCapa));
	CCdiFinCapaAttrOutDest4B_SetTriFoldCSupport(pAttrData, CCapaOutDestCapabilities_GetTriFoldCSupport(pCapa));
	CCdiFinCapaAttrOutDest4B_SetTriFoldZSupport(pAttrData, CCapaOutDestCapabilities_GetTriFoldZSupport(pCapa));
	CCdiFinCapaAttrOutDest4B_SetSaddleFoldSupport(pAttrData, CCapaOutDestCapabilities_GetSaddleFoldSupport(pCapa));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCapaDestAttr50
// Description   : Output Destination Capabilities Attr "Stapling 
//               : Ability"情報取り出し処理
// Parameter     : pCapa (Output Destination Capabilitiesインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCapaDestAttr50(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinOutDestCapabilitiesAttr_AllocateAttrPair(pAttr, CCdiFinCapaAttrOutDest50_GetAttributeKey(), CCdiFinCapaAttrOutDest50_GetAttributeSizeByNumberOfReported(3));
	pAttrData = CCdiFinOutDestCapabilitiesAttr_GetAttributePointer(pAttr, CCdiFinCapaAttrOutDest50_GetAttributeKey());

	CCdiFinCapaAttrOutDest50_SetSelectability(pAttrData, CCapaOutDestCapabilities_GetStaplingSelectability(pCapa));
	CCdiFinCapaAttrOutDest50_SetSingleStitch(pAttrData, CCapaOutDestCapabilities_GetSingleStitchSupport(pCapa));
	CCdiFinCapaAttrOutDest50_SetDoubleStitch(pAttrData, CCapaOutDestCapabilities_GetDoubleStitchSupport(pCapa));
	CCdiFinCapaAttrOutDest50_SetTripleStitch(pAttrData, CCapaOutDestCapabilities_GetTripleStitchSupport(pCapa));
	CCdiFinCapaAttrOutDest50_SetQuadStitch(pAttrData, CCapaOutDestCapabilities_GetQuadStitchSupport(pCapa));
	CCdiFinCapaAttrOutDest50_SetSingleRear(pAttrData, CCapaOutDestCapabilities_GetSingleRearStitchSupport(pCapa));
	CCdiFinCapaAttrOutDest50_SetSingleFront(pAttrData, CCapaOutDestCapabilities_GetSingleFrontStitchSupport(pCapa));
	CCdiFinCapaAttrOutDest50_SetPinCut(pAttrData, CCapaOutDestCapabilities_GetPinCutSupport(pCapa));
	CCdiFinCapaAttrOutDest50_SetNumberOfReported(pAttrData, 3);
	CCdiFinCapaAttrOutDest50_SetMinNumOfShtPerSet1Set(pAttrData, CCapaOutDestCapabilities_GetMinNumberSheetsPerStaple(pCapa, CAPA_SIZE_CTGRY_STANDARD));
	CCdiFinCapaAttrOutDest50_SetMaxNumOfShtPerSet1Set(pAttrData, CCapaOutDestCapabilities_GetMaxNumberSheetsPerStaple(pCapa, CAPA_SIZE_CTGRY_STANDARD));
	CCdiFinCapaAttrOutDest50_SetTrayCapacity1Set(pAttrData, CCapaOutDestCapabilities_GetStapledSetCapacity(pCapa, CAPA_SIZE_CTGRY_STANDARD));
	CCdiFinCapaAttrOutDest50_SetMinNumOfShtPerSetSmallSize(pAttrData, CCapaOutDestCapabilities_GetMinNumberSheetsPerStaple(pCapa, CAPA_SIZE_CTGRY_SMALL));
	CCdiFinCapaAttrOutDest50_SetMaxNumOfShtPerSetSmallSize(pAttrData, CCapaOutDestCapabilities_GetMaxNumberSheetsPerStaple(pCapa, CAPA_SIZE_CTGRY_SMALL));
	CCdiFinCapaAttrOutDest50_SetTrayCapacitySmallSize(pAttrData, CCapaOutDestCapabilities_GetStapledSetCapacity(pCapa, CAPA_SIZE_CTGRY_SMALL));
	CCdiFinCapaAttrOutDest50_SetMinNumOfShtPerSetLargeSize(pAttrData, CCapaOutDestCapabilities_GetMinNumberSheetsPerStaple(pCapa, CAPA_SIZE_CTGRY_LARGE));
	CCdiFinCapaAttrOutDest50_SetMaxNumOfShtPerSetLargeSize(pAttrData, CCapaOutDestCapabilities_GetMaxNumberSheetsPerStaple(pCapa, CAPA_SIZE_CTGRY_LARGE));
	CCdiFinCapaAttrOutDest50_SetTrayCapacityLargeSize(pAttrData, CCapaOutDestCapabilities_GetStapledSetCapacity(pCapa, CAPA_SIZE_CTGRY_LARGE));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCapaDestAttr55
// Description   : Output Destination Capabilities Attr "Punching 
//               : Ability"情報取り出し処理
// Parameter     : pCapa (Output Destination Capabilitiesインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCapaDestAttr55(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinOutDestCapabilitiesAttr_AllocateAttrPair(pAttr, CCdiFinCapaAttrOutDest55_GetAttributeKey(), CCdiFinCapaAttrOutDest55_GetAttributeSize(NULL));
	pAttrData = CCdiFinOutDestCapabilitiesAttr_GetAttributePointer(pAttr, CCdiFinCapaAttrOutDest55_GetAttributeKey());

	CCdiFinCapaAttrOutDest55_SetSelectability(pAttrData, CCapaOutDestCapabilities_GetPunchingSelectability(pCapa));
	CCdiFinCapaAttrOutDest55_SetDoublePunch(pAttrData, CCapaOutDestCapabilities_GetDoublePunchSupport(pCapa));
	CCdiFinCapaAttrOutDest55_SetTriplePunch(pAttrData, CCapaOutDestCapabilities_GetTriplePunchSupport(pCapa));
	CCdiFinCapaAttrOutDest55_SetQuadPunch(pAttrData, CCapaOutDestCapabilities_GetQuadPunchSupport(pCapa));
	CCdiFinCapaAttrOutDest55_SetMultiPunch(pAttrData, CCapaOutDestCapabilities_GetMultiPunchSupport(pCapa));
	CCdiFinCapaAttrOutDest55_SetDoublePunchType(pAttrData, CCapaOutDestCapabilities_GetDoublePunchType(pCapa));
	CCdiFinCapaAttrOutDest55_SetQuadPunchType(pAttrData, CCapaOutDestCapabilities_GetQuadPunchType(pCapa));
	CCdiFinCapaAttrOutDest55_SetMultiPunchType(pAttrData, CCapaOutDestCapabilities_GetMultiPunchType(pCapa));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCapaDestAttr5B
// Description   : Output Destination Capabilities Attr "Folding 
//               : Booklet Maker Ability"情報取り出し処理
// Parameter     : pCapa (Output Destination Capabilitiesインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCapaDestAttr5B(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinOutDestCapabilitiesAttr_AllocateAttrPair(pAttr, CCdiFinCapaAttrOutDest5B_GetAttributeKey(), CCdiFinCapaAttrOutDest5B_GetAttributeSize(NULL));
	pAttrData = CCdiFinOutDestCapabilitiesAttr_GetAttributePointer(pAttr, CCdiFinCapaAttrOutDest5B_GetAttributeKey());

	CCdiFinCapaAttrOutDest5B_SetSelectability(pAttrData, CCapaOutDestCapabilities_GetFoldingBookletSelectability(pCapa));
	CCdiFinCapaAttrOutDest5B_SetSaddleFoldAndStaple(pAttrData, CCapaOutDestCapabilities_GetSaddleFoldStapleSupport(pCapa));
	CCdiFinCapaAttrOutDest5B_SetSaddleFoldAndNoStaple(pAttrData, CCapaOutDestCapabilities_GetSaddleFoldNoStapleSupport(pCapa));
	CCdiFinCapaAttrOutDest5B_SetCoverOrder(pAttrData, CCapaOutDestCapabilities_GetBookletCoverOrder(pCapa));
	CCdiFinCapaAttrOutDest5B_SetSquareFolding(pAttrData, CCapaOutDestCapabilities_GetSquareFoldingSupport(pCapa));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCapaDestAttr5E
// Description   : Output Destination Capabilities Attr "Booklet 
//               : Maker Tray Capacity"情報取り出し処理
// Parameter     : pCapa (Output Destination Capabilitiesインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCapaDestAttr5E(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinOutDestCapabilitiesAttr_AllocateAttrPair(pAttr, CCdiFinCapaAttrOutDest5E_GetAttributeKey(), CCdiFinCapaAttrOutDest5E_GetAttributeSizeByNumberOfReported(2));
	pAttrData = CCdiFinOutDestCapabilitiesAttr_GetAttributePointer(pAttr, CCdiFinCapaAttrOutDest5E_GetAttributeKey());

	CCdiFinCapaAttrOutDest5E_SetNumberOfReported(pAttrData, 2);
	CCdiFinCapaAttrOutDest5E_SetTypeOfReported(pAttrData, eCdiFinTypBookRept_StplFold, 1);
	CCdiFinCapaAttrOutDest5E_SetMinNumOfShtPerBooklet(pAttrData, CCapaOutDestCapabilities_GetMinNumberSheetsSaddleFoldStaple(pCapa), 1);
	CCdiFinCapaAttrOutDest5E_SetMaxNumOfShtPerBooklet(pAttrData, CCapaOutDestCapabilities_GetMaxNumberSheetsSaddleFoldStaple(pCapa), 1);
	CCdiFinCapaAttrOutDest5E_SetBookletTrayCapacity(pAttrData, CCapaOutDestCapabilities_GetBookletTrayCapacitySaddleFoldStaple(pCapa), 1);
	CCdiFinCapaAttrOutDest5E_SetTypeOfReported(pAttrData, eCdiFinTypBookRept_FoldOnly, 2);
	CCdiFinCapaAttrOutDest5E_SetMinNumOfShtPerBooklet(pAttrData, CCapaOutDestCapabilities_GetMinNumberSheetsSaddleFoldNoStaple(pCapa), 2);
	CCdiFinCapaAttrOutDest5E_SetMaxNumOfShtPerBooklet(pAttrData, CCapaOutDestCapabilities_GetMaxNumberSheetsSaddleFoldNoStaple(pCapa), 2);
	CCdiFinCapaAttrOutDest5E_SetBookletTrayCapacity(pAttrData, CCapaOutDestCapabilities_GetBookletTrayCapacitySaddleFoldNoStaple(pCapa), 2);
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCapaDestAttrC2
// Description   : Output Destination Capabilities Attr "Count Of 
//               : Sheet In Stapled Set"情報取り出し処理
// Parameter     : pCapa (Output Destination Capabilitiesインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCapaDestAttrC2(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinOutDestCapabilitiesAttr_AllocateAttrPair(pAttr, CCdiFinCapaAttrOutDestC2_GetAttributeKey(), CCdiFinCapaAttrOutDestC2_GetAttributeSize(NULL));
	pAttrData = CCdiFinOutDestCapabilitiesAttr_GetAttributePointer(pAttr, CCdiFinCapaAttrOutDestC2_GetAttributeKey());

	CCdiFinCapaAttrOutDestC2_SetCountOfSheetZFold(pAttrData, CCapaOutDestCapabilities_GetZFoldSheetCountLong(pCapa));
	CCdiFinCapaAttrOutDestC2_SetCountOfSheetZFoldExcept(pAttrData, CCapaOutDestCapabilities_GetZFoldSheetCountShort(pCapa));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCapaDestAttrC6
// Description   : Output Destination Capabilities Attr "Staple 
//               : Type"情報取り出し処理
// Parameter     : pCapa (Output Destination Capabilitiesインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCapaDestAttrC6(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinOutDestCapabilitiesAttr_AllocateAttrPair(pAttr, CCdiFinCapaAttrOutDestC6_GetAttributeKey(), CCdiFinCapaAttrOutDestC6_GetAttributeSize(NULL));
	pAttrData = CCdiFinOutDestCapabilitiesAttr_GetAttributePointer(pAttr, CCdiFinCapaAttrOutDestC6_GetAttributeKey());

	CCdiFinCapaAttrOutDestC6_SetStapleType(pAttrData, CCapaOutDestCapabilities_GetStapleType(pCapa));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCapaDestAttrC7
// Description   : Output Destination Capabilities Attr "Booklet 
//               : Maker Staple Type"情報取り出し処理
// Parameter     : pCapa (Output Destination Capabilitiesインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCapaDestAttrC7(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinOutDestCapabilitiesAttr_AllocateAttrPair(pAttr, CCdiFinCapaAttrOutDestC7_GetAttributeKey(), CCdiFinCapaAttrOutDestC7_GetAttributeSize(NULL));
	pAttrData = CCdiFinOutDestCapabilitiesAttr_GetAttributePointer(pAttr, CCdiFinCapaAttrOutDestC7_GetAttributeKey());

	CCdiFinCapaAttrOutDestC7_SetStapleType(pAttrData, CCapaOutDestCapabilities_GetBookletStapleType(pCapa));
}
///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCapaDestAttrC8
// Description   : Output Destination Capabilities Attr "Booklet 
//               : Maker Staple Type"情報取り出し処理
// Parameter     : pCapa (Output Destination Capabilitiesインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCapaDestAttrC8(CCapaOutDestCapabilities* pCapa, CCdiFinOutDestCapabilitiesAttr* pAttr)
{
	UC* pAttrData;
	UC i,k;
	
	CCdiFinOutDestCapabilitiesAttr_AllocateAttrPair(pAttr, CCdiFinCapaAttrOutDestC8_GetAttributeKey(), CCdiFinCapaAttrOutDestC8_GetAttributeSize(NULL));
	pAttrData = CCdiFinOutDestCapabilitiesAttr_GetAttributePointer(pAttr, CCdiFinCapaAttrOutDestC8_GetAttributeKey());

	CCdiFinCapaAttrOutDestC8_SetNumberOfSpecificMediaTypes(pAttrData, CDIFIN_CAPA_ATTR_OUT_DEST_C8_NUM_MEDIATYPE); //Number Of Specific Media Types
	pAttrData++;
	for ( i =0, k =0  ; i <CDIFIN_CAPA_ATTR_OUT_DEST_C8_NUM_MEDIATYPE; i++, k=k+4 ){
		
		CCdiFinCapaAttrOutDestC8_SetStplSpecMediaType(pAttrData, k+1,  CCapaOutDestCapabilities_GetStaplingMediaType(pCapa, 1, i)); // Media Type1  (pThis, type, posi)
		CCdiFinCapaAttrOutDestC8_SetStplSpecMediaWeightUp(pAttrData, k+2, CCapaOutDestCapabilities_GetStaplingMediaType(pCapa, 2, i) ); // Media Weight 1 
		CCdiFinCapaAttrOutDestC8_SetStplSpecMediaWeightDown(pAttrData, k+3, CCapaOutDestCapabilities_GetStaplingMediaType(pCapa, 3, i) ); // Media Weight 1 
		CCdiFinCapaAttrOutDestC8_SetStplSpecMediaSizeCategory (pAttrData, k+2,  CCapaOutDestCapabilities_GetStaplingMediaType(pCapa, 4, i)); // Size Category 1
		CCdiFinCapaAttrOutDestC8_SetNumberOfSheets (pAttrData, k+4, CCapaOutDestCapabilities_GetStaplingMediaType(pCapa, 5, i));// SRVC_GetNvmValueUC(pThis->mpSpec->ssmtNumOfSheet[posi]);) ; // SRVC_GetNvmValueUS(NVMID_MAX_SHEET_UNCOAT_80_STAPLE_50) ; // Size Category 1
	}
      // pThis->mpSpec->relativeOptionID
	//#define CCapaOutDestCapabilities_GetStaplingMediaType(pThis, type, weight) ((pThis)->mpSpec->pMethod->GetStaplingMediaType(pThis, type, weight))

}
#endif	// #if OUT_DEST_NUM != 0

#if IN_SRC_NUM != 0

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCapaSrcAttr01
// Description   : Input Source Capabilities Attr "Input Max 
//               : Capacity"情報取り出し処理
// Parameter     : pCapa (Input Source Capabilitiesインスタンスポイ
//               :ンタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCapaSrcAttr01(CCapaInSrcCapabilities* pCapa, CCdiFinInSrcCapabilitiesAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinInSrcCapabilitiesAttr_AllocateAttrPair(pAttr, CCdiFinCapaAttrInSrc01_GetAttributeKey(), CCdiFinCapaAttrInSrc01_GetAttributeSize(NULL));
	pAttrData = CCdiFinInSrcCapabilitiesAttr_GetAttributePointer(pAttr, CCdiFinCapaAttrInSrc01_GetAttributeKey());

	CCdiFinCapaAttrInSrc01_SetInputMaxCapacity(pAttrData, CCapaInSrcCapabilities_GetInputMaxCapacity(pCapa));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCapaSrcAttr04
// Description   : Input Source Capabilities Attr "Input Source 
//               : Unit Type"情報取り出し処理
// Parameter     : pCapa (Input Source Capabilitiesインスタンスポイ
//               : ンタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCapaSrcAttr04(CCapaInSrcCapabilities* pCapa, CCdiFinInSrcCapabilitiesAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinInSrcCapabilitiesAttr_AllocateAttrPair(pAttr, CCdiFinCapaAttrInSrc04_GetAttributeKey(), CCdiFinCapaAttrInSrc04_GetAttributeSize(NULL));
	pAttrData = CCdiFinInSrcCapabilitiesAttr_GetAttributePointer(pAttr, CCdiFinCapaAttrInSrc04_GetAttributeKey());

	CCdiFinCapaAttrInSrc04_SetDeviceUnitType(pAttrData, CCapaInSrcCapabilities_GetDeviceUnitType(pCapa));
}
#endif	// IN_SRC_NUM != 0

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCfgContAttr01
// Description   : Finisher Device Controller Configuration Attr 
//               : "Market Region"情報取り出し
//               : 処理
// Parameter     : pCfg (Device Controller Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCfgContAttr01(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinDevContConfigurationAttr_AllocateAttrPair(pAttr, CCdiFinCfgAttrDevCont01_GetAttributeKey(), CCdiFinCfgAttrDevCont01_GetAttributeSize(NULL));
	pAttrData = CCdiFinDevContConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrDevCont01_GetAttributeKey());

	CCdiFinCfgAttrDevCont01_SetMarketRegion(pAttrData, CCfgDevContConfiguration_GetMarketRegion(pCfg));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCfgContAttr40
// Description   : Finisher Device Controller Configuration Attr 
//               : "Set Date Time"情報取り出し
//               : 処理
// Parameter     : pCfg (Device Controller Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCfgContAttr40(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinDevContConfigurationAttr_AllocateAttrPair(pAttr, CCdiFinCfgAttrDevCont40_GetAttributeKey(), CCdiFinCfgAttrDevCont40_GetAttributeSize(NULL));
	pAttrData = CCdiFinDevContConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrDevCont40_GetAttributeKey());

	CCdiFinCfgAttrDevCont40_SetYear(pAttrData, CCfgDevContConfiguration_GetSetDateYear(pCfg));
	CCdiFinCfgAttrDevCont40_SetMonth(pAttrData, CCfgDevContConfiguration_GetSetDateMonth(pCfg));
	CCdiFinCfgAttrDevCont40_SetDay(pAttrData, CCfgDevContConfiguration_GetSetDateDay(pCfg));
	CCdiFinCfgAttrDevCont40_SetHour(pAttrData, CCfgDevContConfiguration_GetSetDateHour(pCfg));
	CCdiFinCfgAttrDevCont40_SetMinute(pAttrData, CCfgDevContConfiguration_GetSetDateMinute(pCfg));
	CCdiFinCfgAttrDevCont40_SetSecond(pAttrData, CCfgDevContConfiguration_GetSetDateSecond(pCfg));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCfgContAttr41
// Description   : Finisher Device Controller Configuration Attr 
//               : "System Configuration"情報取り出し
//               : 処理
// Parameter     : pCfg (Device Controller Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCfgContAttr41(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinDevContConfigurationAttr_AllocateAttrPair(pAttr, CCdiFinCfgAttrDevCont41_GetAttributeKey(), CCdiFinCfgAttrDevCont41_GetAttributeSize(NULL));
	pAttrData = CCdiFinDevContConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrDevCont41_GetAttributeKey());

	CCdiFinCfgAttrDevCont41_SetSystemConfiguration(pAttrData, CCfgDevContConfiguration_GetSystemConfiguration(pCfg));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCfgContAttrC0
// Description   : Finisher Device Controller Configuration Attr 
//               : "Media Size Group"情報取り出し
//               : 処理
// Parameter     : pCfg (Device Controller Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCfgContAttrC0(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinDevContConfigurationAttr_AllocateAttrPair(pAttr, CCdiFinCfgAttrDevContC0_GetAttributeKey(), CCdiFinCfgAttrDevContC0_GetAttributeSize(NULL));
	pAttrData = CCdiFinDevContConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrDevContC0_GetAttributeKey());

	CCdiFinCfgAttrDevContC0_SetMediaSieGroup(pAttrData, CCfgDevContConfiguration_GetMediaSizeGroup(pCfg));
	CCdiFinCfgAttrDevContC0_SetSelect8K16K(pAttrData, CCfgDevContConfiguration_GetSelect8K16K(pCfg));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCfgContAttrFA
// Description   : Finisher Device Controller Configuration Attr 
//               : "Sheet Exit Height"情報取り出し
//               : 処理
// Parameter     : pCfg (Device Controller Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCfgContAttrFA(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinDevContConfigurationAttr_AllocateAttrPair(pAttr, CCdiFinCfgAttrDevContFA_GetAttributeKey(), CCdiFinCfgAttrDevContFA_GetAttributeSize(NULL));
	pAttrData = CCdiFinDevContConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrDevContFA_GetAttributeKey());

	CCdiFinCfgAttrDevContFA_SetSheetExitHeight(pAttrData, CCfgDevContConfiguration_GetSheetExitHeight(pCfg));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCfgContAttrFB
// Description   : Finisher Device Controller Configuration Attr 
//               : "Productivity"情報取り出し処理
// Parameter     : pCfg (Device Controller Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCfgContAttrFB(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinDevContConfigurationAttr_AllocateAttrPair(pAttr, CCdiFinCfgAttrDevContFB_GetAttributeKey(), CCdiFinCfgAttrDevContFB_GetAttributeSizeByNumberOfReported(1));
	pAttrData = CCdiFinDevContConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrDevContFB_GetAttributeKey());

	CCdiFinCfgAttrDevContFB_SetProductivity(pAttrData, CCfgDevContConfiguration_GetProductivity(pCfg));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCfgContAttrFC
// Description   : Finisher Device Controller Configuration Attr 
//               : "Exit Speed Range"情報取り出し処理
// Parameter     : pCfg (Device Controller Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCfgContAttrFC(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinDevContConfigurationAttr_AllocateAttrPair(pAttr, CCdiFinCfgAttrDevContFC_GetAttributeKey(), CCdiFinCfgAttrDevContFC_GetAttributeSizeByDataLength(4));
	pAttrData = CCdiFinDevContConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrDevContFC_GetAttributeKey());

	CCdiFinCfgAttrDevContFC_SetMinimumExitSpeed(pAttrData, CCfgDevContConfiguration_GetMinimumExitSpeed(pCfg));
	CCdiFinCfgAttrDevContFC_SetMaximumExitSpeed(pAttrData, CCfgDevContConfiguration_GetMaximumExitSpeed(pCfg));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCfgContAttrFD
// Description   : Finisher Device Controller Configuration Attr 
//               : "Sheet Exit Layout"情報取り出し処理
// Parameter     : pCfg (Device Controller Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCfgContAttrFD(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinDevContConfigurationAttr_AllocateAttrPair(pAttr, CCdiFinCfgAttrDevContFD_GetAttributeKey(), CCdiFinCfgAttrDevContFD_GetAttributeSizeByDataLength(4));
	pAttrData = CCdiFinDevContConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrDevContFD_GetAttributeKey());

	CCdiFinCfgAttrDevContFD_SetExitHotLineDistance(pAttrData, CCfgDevContConfiguration_GetHotLineDistance(pCfg));
	CCdiFinCfgAttrDevContFD_SetReleasePointDistance(pAttrData, CCfgDevContConfiguration_GetReleasePointDistance(pCfg));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCfgContAttrFE
// Description   : Finisher Device Controller Configuration Attr 
//               : "Jam Bypass"情報取り出し処理
// Parameter     : pCfg (Device Controller Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCfgContAttrFE(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinDevContConfigurationAttr_AllocateAttrPair(pAttr, CCdiFinCfgAttrDevContFE_GetAttributeKey(), CCdiFinCfgAttrDevContFE_GetAttributeSize(NULL));
	pAttrData = CCdiFinDevContConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrDevContFE_GetAttributeKey());

	CCdiFinCfgAttrDevContFE_SetJamBypass(pAttrData, CCfgDevContConfiguration_GetJamBypass(pCfg));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCfgContAttrFF
// Description   : Finisher Device Controller Configuration Attr 
//               : "No Paper Run"情報取り出し処理
// Parameter     : pCfg (Device Controller Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCfgContAttrFF(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinDevContConfigurationAttr_AllocateAttrPair(pAttr, CCdiFinCfgAttrDevContFF_GetAttributeKey(), CCdiFinCfgAttrDevContFF_GetAttributeSize(NULL));
	pAttrData = CCdiFinDevContConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrDevContFF_GetAttributeKey());

	CCdiFinCfgAttrDevContFF_SetNoPaperRun(pAttrData, CCfgDevContConfiguration_GetNoPaperRun(pCfg));
}

#if OUT_DEST_NUM != 0
///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCfgDestAttr03
// Description   : Output Destination Configuration Attr "
//               : Selectability"情報取り出し処理
// Parameter     : pCfg (Output Destination Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCfgDestAttr03(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinOutDestConfigurationAttr_AllocateAttrPair(pAttr, CCdiFinCfgAttrOutDest03_GetAttributeKey(), CCdiFinCfgAttrOutDest03_GetAttributeSize(NULL));
	pAttrData = CCdiFinOutDestConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrOutDest03_GetAttributeKey());

	CCdiFinCfgAttrOutDest03_SetSelectability(pAttrData, CCfgOutDestConfiguration_GetSelectability(pCfg));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCfgDestAttr45
// Description   : Output Destination Configuration Attr "
//               : Current Offset Configuration"情報取り出し処理
// Parameter     : pCfg (Output Destination Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCfgDestAttr45(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinOutDestConfigurationAttr_AllocateAttrPair(pAttr, CCdiFinCfgAttrOutDest45_GetAttributeKey(), CCdiFinCfgAttrOutDest45_GetAttributeSize(NULL));
	pAttrData = CCdiFinOutDestConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrOutDest45_GetAttributeKey());

	CCdiFinCfgAttrOutDest45_SetOffset(pAttrData, CCfgOutDestConfiguration_GetOffsetSelectability(pCfg));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCfgDestAttr46
// Description   : Output Destination Configuration Attr "
//               : Current Sheet Folding Configuration"情報取り出し
//               : 処理
// Parameter     : pCfg (Output Destination Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCfgDestAttr46(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinOutDestConfigurationAttr_AllocateAttrPair(pAttr, CCdiFinCfgAttrOutDest46_GetAttributeKey(), CCdiFinCfgAttrOutDest46_GetAttributeSize(NULL));
	pAttrData = CCdiFinOutDestConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrOutDest46_GetAttributeKey());

	CCdiFinCfgAttrOutDest46_SetZFold(pAttrData, CCfgOutDestConfiguration_GetZFoldSelectability(pCfg));
	CCdiFinCfgAttrOutDest46_SetTriFoldZ(pAttrData, CCfgOutDestConfiguration_GetTriFoldZSelectability(pCfg));
	CCdiFinCfgAttrOutDest46_SetTriFoldC(pAttrData, CCfgOutDestConfiguration_GetTriFoldCSelectability(pCfg));
	CCdiFinCfgAttrOutDest46_SetSaddleFold(pAttrData, CCfgOutDestConfiguration_GetSaddleFoldSelectability(pCfg));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCfgDestAttr47
// Description   : Output Destination Configuration Attr "
//               : Current Staple Configuration"情報取り出し処理
// Parameter     : pCfg (Output Destination Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCfgDestAttr47(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinOutDestConfigurationAttr_AllocateAttrPair(pAttr, CCdiFinCfgAttrOutDest47_GetAttributeKey(), CCdiFinCfgAttrOutDest47_GetAttributeSize(NULL));
	pAttrData = CCdiFinOutDestConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrOutDest47_GetAttributeKey());

	CCdiFinCfgAttrOutDest47_SetSingleStitch(pAttrData, CCfgOutDestConfiguration_GetSingleStitchSelectability(pCfg));
	CCdiFinCfgAttrOutDest47_SetDoubleStitch(pAttrData, CCfgOutDestConfiguration_GetDoubleStitchSelectability(pCfg));
	CCdiFinCfgAttrOutDest47_SetTripleStitch(pAttrData, CCfgOutDestConfiguration_GetTripleStitchSelectability(pCfg));
	CCdiFinCfgAttrOutDest47_SetQuadStitch(pAttrData, CCfgOutDestConfiguration_GetQuadStitchSelectability(pCfg));
	CCdiFinCfgAttrOutDest47_SetSingleRear(pAttrData, CCfgOutDestConfiguration_GetSingleRearSelectability(pCfg));
	CCdiFinCfgAttrOutDest47_SetSingleFront(pAttrData, CCfgOutDestConfiguration_GetSingleFrontSelectability(pCfg));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCfgDestAttr48
// Description   : Output Destination Configuration Attr "
//               : Current Punch Configuration"情報取り出し処理
// Parameter     : pCfg (Output Destination Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCfgDestAttr48(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinOutDestConfigurationAttr_AllocateAttrPair(pAttr, CCdiFinCfgAttrOutDest48_GetAttributeKey(), CCdiFinCfgAttrOutDest48_GetAttributeSize(NULL));
	pAttrData = CCdiFinOutDestConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrOutDest48_GetAttributeKey());

	CCdiFinCfgAttrOutDest48_SetDoublePunch(pAttrData, CCfgOutDestConfiguration_GetDoublePunchSelectability(pCfg));
	CCdiFinCfgAttrOutDest48_SetTriplePunch(pAttrData, CCfgOutDestConfiguration_GetTriplePunchSelectability(pCfg));
	CCdiFinCfgAttrOutDest48_SetQuadPunch(pAttrData, CCfgOutDestConfiguration_GetQuadPunchSelectability(pCfg));
	CCdiFinCfgAttrOutDest48_SetMultiPunch(pAttrData, CCfgOutDestConfiguration_GetMultiPunchSelectability(pCfg));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCfgDestAttr49
// Description   : Output Destination Configuration Attr "
//               : Current Booklet Maker Configuration"情報取り出し
//               : "情報取り出し処理
// Parameter     : pCfg (Output Destination Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCfgDestAttr49(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinOutDestConfigurationAttr_AllocateAttrPair(pAttr, CCdiFinCfgAttrOutDest49_GetAttributeKey(), CCdiFinCfgAttrOutDest49_GetAttributeSize(NULL));
	pAttrData = CCdiFinOutDestConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrOutDest49_GetAttributeKey());

	CCdiFinCfgAttrOutDest49_SetSaddleFoldAndStaple(pAttrData, CCfgOutDestConfiguration_GetSaddleFoldAndStapleSelectability(pCfg));
	CCdiFinCfgAttrOutDest49_SetSaddleFoldAndNoStaple(pAttrData, CCfgOutDestConfiguration_GetSaddleFoldAndNoStapleSelectability(pCfg));
	CCdiFinCfgAttrOutDest49_SetSquareFolding(pAttrData, CCfgOutDestConfiguration_GetSquareFoldingSelectability(pCfg));
}

#endif	// #if OUT_DEST_NUM != 0
#if IN_SRC_NUM != 0
///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCfgSrcAttr03
// Description   : Input Source Configuration Attr "
//               : Selectability"情報取り出し処理
// Parameter     : pCfg (Input Source Configurationインスタンスポイ
//               : ンタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCfgSrcAttr03(CCfgInSrcConfiguration* pCfg, CCdiFinInSrcConfigurationAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinInSrcConfigurationAttr_AllocateAttrPair(pAttr, CCdiFinCfgAttrInSrc03_GetAttributeKey(), CCdiFinCfgAttrInSrc03_GetAttributeSize(NULL));
	pAttrData = CCdiFinInSrcConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrInSrc03_GetAttributeKey());

	CCdiFinCfgAttrInSrc03_SetSelectability(pAttrData, CCfgInSrcConfiguration_GetSelectability(pCfg));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCfgSrcAttrC0
// Description   : Input Source Configuration Attr "
//               : Media Size Alternate"情報取り出し処理
// Parameter     : pCfg (Input Source Configurationインスタンスポイ
//               : ンタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCfgSrcAttrC0(CCfgInSrcConfiguration* pCfg, CCdiFinInSrcConfigurationAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinInSrcConfigurationAttr_AllocateAttrPair(pAttr, CCdiFinCfgAttrInSrcC0_GetAttributeKey(), CCdiFinCfgAttrInSrcC0_GetAttributeSizeByNumberOfAlternate(5));
	pAttrData = CCdiFinInSrcConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrInSrcC0_GetAttributeKey());

	CCdiFinCfgAttrInSrcC0_SetSizeAlternate1(pAttrData, CCfgInSrcConfiguration_GetSizeAlternate1(pCfg));
	CCdiFinCfgAttrInSrcC0_SetSizeAlternate2(pAttrData, CCfgInSrcConfiguration_GetSizeAlternate2(pCfg));
	CCdiFinCfgAttrInSrcC0_SetSizeAlternate3(pAttrData, CCfgInSrcConfiguration_GetSizeAlternate3(pCfg));
	CCdiFinCfgAttrInSrcC0_SetSizeAlternate4(pAttrData, CCfgInSrcConfiguration_GetSizeAlternate4(pCfg));
	CCdiFinCfgAttrInSrcC0_SetSizeAlternate5(pAttrData, CCfgInSrcConfiguration_GetSizeAlternate5(pCfg));
}

///////////////////////////////////////////////////////////////////
// Function Name : ExportDevCfgSrcAttrC1
// Description   : Input Source Configuration Attr "
//               : Irregular Media Size"情報取り出し処理
// Parameter     : pCfg (Input Source Configurationインスタンスポイ
//               : ンタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ExportDevCfgSrcAttrC1(CCfgInSrcConfiguration* pCfg, CCdiFinInSrcConfigurationAttr* pAttr)
{
	UC* pAttrData;

	CCdiFinInSrcConfigurationAttr_AllocateAttrPair(pAttr, CCdiFinCfgAttrInSrcC1_GetAttributeKey(), CCdiFinCfgAttrInSrcC1_GetAttributeSize(NULL));
	pAttrData = CCdiFinInSrcConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrInSrcC1_GetAttributeKey());

	CCdiFinCfgAttrInSrcC1_SetProcessSize(pAttrData, CCfgInSrcConfiguration_GetIrregularProcessSize(pCfg));
	CCdiFinCfgAttrInSrcC1_SetCrossProcessSize(pAttrData, CCfgInSrcConfiguration_GetIrregularCrossProcSize(pCfg));
}
#endif	// #if IN_SRC_NUM != 0

///////////////////////////////////////////////////////////////////
// Function Name : ImportDevCfgContAttr01
// Description   : Finisher Device Controller Configuration Attr
//               : "Market Region"情報取りこみ処理
// Parameter     : pCfg (Device Controller Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ImportDevCfgContAttr01(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr)
{
	UC* pAttrData;

	pAttrData = CCdiFinDevContConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrDevCont01_GetAttributeKey());

	CCfgDevContConfiguration_SetMarketRegion(pCfg, CCdiFinCfgAttrDevCont01_GetMarketRegion(pAttrData));
}

///////////////////////////////////////////////////////////////////
// Function Name : ImportDevCfgContAttr40
// Description   : Finisher Device Controller Configuration Attr
//               : "Set Date Time"情報取りこみ処理
// Parameter     : pCfg (Device Controller Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ImportDevCfgContAttr40(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr)
{
	UC* pAttrData;

	pAttrData = CCdiFinDevContConfigurationAttr_GetAttributePointer(pAttr,CCdiFinCfgAttrDevCont40_GetAttributeKey());

	CCfgDevContConfiguration_SetSetDateYear(pCfg, CCdiFinCfgAttrDevCont40_GetYear(pAttrData));
	CCfgDevContConfiguration_SetSetDateMonth(pCfg, CCdiFinCfgAttrDevCont40_GetMonth(pAttrData));
	CCfgDevContConfiguration_SetSetDateDay(pCfg, CCdiFinCfgAttrDevCont40_GetDay(pAttrData));
	CCfgDevContConfiguration_SetSetDateHour(pCfg, CCdiFinCfgAttrDevCont40_GetHour(pAttrData));
	CCfgDevContConfiguration_SetSetDateMinute(pCfg, CCdiFinCfgAttrDevCont40_GetMinute(pAttrData));
	CCfgDevContConfiguration_SetSetDateSecond(pCfg, CCdiFinCfgAttrDevCont40_GetSecond(pAttrData));
}

///////////////////////////////////////////////////////////////////
// Function Name : ImportDevCfgContAttr41
// Description   : Finisher Device Controller Configuration Attr
//               : "System Configuration"情報取りこみ処理
// Parameter     : pCfg (Device Controller Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ImportDevCfgContAttr41(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr)
{
	UC* pAttrData;

	pAttrData = CCdiFinDevContConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrDevCont41_GetAttributeKey());

	CCfgDevContConfiguration_SetSystemConfiguration(pCfg, CCdiFinCfgAttrDevCont41_GetSystemConfiguration(pAttrData));
}

///////////////////////////////////////////////////////////////////
// Function Name : ImportDevCfgContAttrC0
// Description   : Finisher Device Controller Configuration Attr
//               : "Media Size Group"情報取りこみ処理
// Parameter     : pCfg (Device Controller Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ImportDevCfgContAttrC0(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr)
{
	UC* pAttrData;

	pAttrData = CCdiFinDevContConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrDevContC0_GetAttributeKey());

	CCfgDevContConfiguration_SetMediaSizeGroup(pCfg, CCdiFinCfgAttrDevContC0_GetMediaSieGroup(pAttrData));
	CCfgDevContConfiguration_SetSelect8K16K(pCfg, CCdiFinCfgAttrDevContC0_GetSelect8K16K(pAttrData));
}

///////////////////////////////////////////////////////////////////
// Function Name : ImportDevCfgContAttrFA
// Description   : Finisher Device Controller Configuration Attr
//               : "Sheet Exit Height"情報取りこみ処理
// Parameter     : pCfg (Device Controller Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ImportDevCfgContAttrFA(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr)
{
	UC* pAttrData;

	pAttrData = CCdiFinDevContConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrDevContFA_GetAttributeKey());

	CCfgDevContConfiguration_SetSheetExitHeight(pCfg, CCdiFinCfgAttrDevContFA_GetSheetExitHeight(pAttrData));
}

///////////////////////////////////////////////////////////////////
// Function Name : ImportDevCfgContAttrFB
// Description   : Finisher Device Controller Configuration Attr
//               : "Productivity"情報取りこみ処理
// Parameter     : pCfg (Device Controller Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ImportDevCfgContAttrFB(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr)
{
	UC* pAttrData;

	pAttrData = CCdiFinDevContConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrDevContFB_GetAttributeKey());

	CCfgDevContConfiguration_SetProductivity(pCfg, CCdiFinCfgAttrDevContFB_GetProductivity(pAttrData));
}

///////////////////////////////////////////////////////////////////
// Function Name : ImportDevCfgContAttrFC
// Description   : Finisher Device Controller Configuration Attr
//               : "Exit Speed Range"情報取りこみ処理
// Parameter     : pCfg (Device Controller Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ImportDevCfgContAttrFC(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr)
{
	UC* pAttrData;

	pAttrData = CCdiFinDevContConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrDevContFC_GetAttributeKey());

	CCfgDevContConfiguration_SetMinimumExitSpeed(pCfg, CCdiFinCfgAttrDevContFC_GetMinimumExitSpeed(pAttrData));
	CCfgDevContConfiguration_SetMaximumExitSpeed(pCfg, CCdiFinCfgAttrDevContFC_GetMaximumExitSpeed(pAttrData));
}

///////////////////////////////////////////////////////////////////
// Function Name : ImportDevCfgContAttrFD
// Description   : Finisher Device Controller Configuration Attr
//               : "Sheet Exit Layout"情報取りこみ処理
// Parameter     : pCfg (Device Controller Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ImportDevCfgContAttrFD(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr)
{
	UC* pAttrData;

	pAttrData = CCdiFinDevContConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrDevContFD_GetAttributeKey());

	CCfgDevContConfiguration_SetHotLineDistance(pCfg, CCdiFinCfgAttrDevContFD_GetExitHotLineDistance(pAttrData));
	CCfgDevContConfiguration_SetReleasePointDistance(pCfg,CCdiFinCfgAttrDevContFD_GetReleasePointDistance(pAttrData));
}

///////////////////////////////////////////////////////////////////
// Function Name : ImportDevCfgContAttrFE
// Description   : Finisher Device Controller Configuration Attr
//               : "Jam Bypass"情報取りこみ処理
// Parameter     : pCfg (Device Controller Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ImportDevCfgContAttrFE(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr)
{
	UC* pAttrData;

	pAttrData = CCdiFinDevContConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrDevContFE_GetAttributeKey());

	CCfgDevContConfiguration_SetJamBypass(pCfg, CCdiFinCfgAttrDevContFE_GetJamBypass(pAttrData));
}

///////////////////////////////////////////////////////////////////
// Function Name : ImportDevCfgContAttrFF
// Description   : Finisher Device Controller Configuration Attr
//               : "No Paper Run"情報取りこみ処理
// Parameter     : pCfg (Device Controller Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ImportDevCfgContAttrFF(CCfgDevContConfiguration* pCfg, CCdiFinDevContConfigurationAttr* pAttr)
{
	UC* pAttrData;

	pAttrData = CCdiFinDevContConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrDevContFF_GetAttributeKey());

	CCfgDevContConfiguration_SetNoPaperRun(pCfg, CCdiFinCfgAttrDevContFF_GetNoPaperRun(pAttrData));
}

#if OUT_DEST_NUM != 0
///////////////////////////////////////////////////////////////////
// Function Name : ImportDevCfgDestAttr03
// Description   : Output Destination Configuration Attr
//               : "Selectability"情報取りこみ処理
// Parameter     : pCfg (Output Destination Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ImportDevCfgDestAttr03(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr)
{
	UC* pAttrData;

	pAttrData = CCdiFinOutDestConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrOutDest03_GetAttributeKey());

	CCfgOutDestConfiguration_SetSelectability(pCfg, CCdiFinCfgAttrOutDest03_GetSelectability(pAttrData));
}

///////////////////////////////////////////////////////////////////
// Function Name : ImportDevCfgDestAttr45
// Description   : Output Destination Configuration Attr "Current 
//               : Offset Configura"情報取りこみ処理
// Parameter     : pCfg (Output Destination Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ImportDevCfgDestAttr45(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr)
{
	UC* pAttrData;

	pAttrData = CCdiFinOutDestConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrOutDest45_GetAttributeKey());

	CCfgOutDestConfiguration_SetOffsetSelectability(pCfg, CCdiFinCfgAttrOutDest45_GetOffset(pAttrData));
}

///////////////////////////////////////////////////////////////////
// Function Name : ImportDevCfgDestAttr46
// Description   : Output Destination Configuration Attr "Current 
//               : Sheet Folding Configuration"情報取りこみ処理
// Parameter     : pCfg (Output Destination Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ImportDevCfgDestAttr46(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr)
{
	UC* pAttrData;

	pAttrData = CCdiFinOutDestConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrOutDest46_GetAttributeKey());

	CCfgOutDestConfiguration_SetZFoldSelectability(pCfg, CCdiFinCfgAttrOutDest46_GetZFold(pAttrData));
	CCfgOutDestConfiguration_SetTriFoldZSelectability(pCfg, CCdiFinCfgAttrOutDest46_GetTriFoldZ(pAttrData));
	CCfgOutDestConfiguration_SetTriFoldCSelectability(pCfg, CCdiFinCfgAttrOutDest46_GetTriFoldC(pAttrData));
	CCfgOutDestConfiguration_SetSaddleFoldSelectability(pCfg, CCdiFinCfgAttrOutDest46_GetSaddleFold(pAttrData));
}

///////////////////////////////////////////////////////////////////
// Function Name : ImportDevCfgDestAttr47
// Description   : Output Destination Configuration Attr "Current 
//               : Staple Configuration"情報取りこみ処理
// Parameter     : pCfg (Output Destination Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ImportDevCfgDestAttr47(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr)
{
	UC* pAttrData;

	pAttrData = CCdiFinOutDestConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrOutDest47_GetAttributeKey());

	CCfgOutDestConfiguration_SetSingleStitchSelectability(pCfg, CCdiFinCfgAttrOutDest47_GetSingleStitch(pAttrData));
	CCfgOutDestConfiguration_SetDoubleStitchSelectability(pCfg, CCdiFinCfgAttrOutDest47_GetDoubleStitch(pAttrData));
	CCfgOutDestConfiguration_SetTripleStitchSelectability(pCfg, CCdiFinCfgAttrOutDest47_GetTripleStitch(pAttrData));
	CCfgOutDestConfiguration_SetQuadStitchSelectability(pCfg, CCdiFinCfgAttrOutDest47_GetQuadStitch(pAttrData));
	CCfgOutDestConfiguration_SetSingleRearStitchSelectability(pCfg, CCdiFinCfgAttrOutDest47_GetSingleRear(pAttrData));
	CCfgOutDestConfiguration_SetSingleFrontStitchSelectability(pCfg, CCdiFinCfgAttrOutDest47_GetSingleFront(pAttrData));
}

///////////////////////////////////////////////////////////////////
// Function Name : ImportDevCfgDestAttr48
// Description   : Output Destination Configuration Attr "Current 
//               : Punch Configuration"情報取りこみ処理
// Parameter     : pCfg (Output Destination Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ImportDevCfgDestAttr48(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr)
{
	UC* pAttrData;

	pAttrData = CCdiFinOutDestConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrOutDest48_GetAttributeKey());

	CCfgOutDestConfiguration_SetDoublePunchSelectability(pCfg, CCdiFinCfgAttrOutDest48_GetDoublePunch(pAttrData));
	CCfgOutDestConfiguration_SetTriplePunchSelectability(pCfg, CCdiFinCfgAttrOutDest48_GetTriplePunch(pAttrData));
	CCfgOutDestConfiguration_SetQuadPunchSelectability(pCfg, CCdiFinCfgAttrOutDest48_GetQuadPunch(pAttrData));
	CCfgOutDestConfiguration_SetMultiPunchSelectability(pCfg, CCdiFinCfgAttrOutDest48_GetMultiPunch(pAttrData));
}

///////////////////////////////////////////////////////////////////
// Function Name : ImportDevCfgDestAttr49
// Description   : Output Destination Configuration Attr "Current 
//               : Booklet Maker Configuraion"情報取りこみ処理
// Parameter     : pCfg (Output Destination Configurationインスタン
//               : スポインタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ImportDevCfgDestAttr49(CCfgOutDestConfiguration* pCfg, CCdiFinOutDestConfigurationAttr* pAttr)
{
	UC* pAttrData;

	pAttrData = CCdiFinOutDestConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrOutDest49_GetAttributeKey());

	CCfgOutDestConfiguration_SetSaddleFoldAndStapleSelectability(pCfg, CCdiFinCfgAttrOutDest49_GetSaddleFoldAndStaple(pAttrData));
	CCfgOutDestConfiguration_SetSaddleFoldAndNoStapleSelectability(pCfg, CCdiFinCfgAttrOutDest49_GetSaddleFoldAndNoStaple(pAttrData));
	CCfgOutDestConfiguration_SetSquareFoldingSelectability(pCfg, CCdiFinCfgAttrOutDest49_GetSquareFolding(pAttrData));
}

#endif	// #if OUT_DEST_NUM != 0
#if IN_SRC_NUM != 0
///////////////////////////////////////////////////////////////////
// Function Name : ImportDevCfgSrcAttr03
// Description   : Input Source Configuration Attr "Selectability"
//               : 情報取りこみ処理
// Parameter     : pCfg (Input Source Configurationインスタンスポイ
//               : ンタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ImportDevCfgSrcAttr03(CCfgInSrcConfiguration* pCfg, CCdiFinInSrcConfigurationAttr* pAttr)
{
	UC* pAttrData;

	pAttrData = CCdiFinInSrcConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrInSrc03_GetAttributeKey());

	CCfgInSrcConfiguration_SetSelectability(pCfg, CCdiFinCfgAttrInSrc03_GetSelectability(pAttrData));
}

///////////////////////////////////////////////////////////////////
// Function Name : ImportDevCfgSrcAttrC0
// Description   : Input Source Configuration Attr "Media Size 
//               : Alternate"情報取りこみ処理
// Parameter     : pCfg (Input Source Configurationインスタンスポイ
//               : ンタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ImportDevCfgSrcAttrC0(CCfgInSrcConfiguration* pCfg, CCdiFinInSrcConfigurationAttr* pAttr)
{
	UC* pAttrData;

	pAttrData = CCdiFinInSrcConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrInSrcC0_GetAttributeKey());

	CCfgInSrcConfiguration_SetSizeAlternate1(pCfg, CCdiFinCfgAttrInSrcC0_GetSizeAlternate1(pAttrData));
	CCfgInSrcConfiguration_SetSizeAlternate2(pCfg, CCdiFinCfgAttrInSrcC0_GetSizeAlternate2(pAttrData));
	CCfgInSrcConfiguration_SetSizeAlternate3(pCfg, CCdiFinCfgAttrInSrcC0_GetSizeAlternate3(pAttrData));
	CCfgInSrcConfiguration_SetSizeAlternate4(pCfg, CCdiFinCfgAttrInSrcC0_GetSizeAlternate4(pAttrData));
	CCfgInSrcConfiguration_SetSizeAlternate5(pCfg, CCdiFinCfgAttrInSrcC0_GetSizeAlternate5(pAttrData));
}

///////////////////////////////////////////////////////////////////
// Function Name : ImportDevCfgSrcAttrC1
// Description   : Input Source Configuration Attr"Irregular Media 
//               : Size"情報取りこみ処理
// Parameter     : pCfg (Input Source Configurationインスタンスポイ
//               : ンタ)
//               : pAttr (メッセージアトリビュートポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void ImportDevCfgSrcAttrC1(CCfgInSrcConfiguration* pCfg, CCdiFinInSrcConfigurationAttr* pAttr)
{
	UC* pAttrData;

	pAttrData = CCdiFinInSrcConfigurationAttr_GetAttributePointer(pAttr, CCdiFinCfgAttrInSrcC1_GetAttributeKey());

	CCfgInSrcConfiguration_SetIrregularProcSize(pCfg, CCdiFinCfgAttrInSrcC1_GetProcessSize(pAttrData));
	CCfgInSrcConfiguration_SetIrregularCrossProcSize(pCfg, CCdiFinCfgAttrInSrcC1_GetCrossProcessSize(pAttrData));
}
#endif	// #if IN_SRC_NUM != 0

///////////////////////////////////////////////////////////////////
// Function Name : SetDevCapabilitiesAttributeDevCont
// Description   : 保持情報をDevice Controller Capabilities 
//               : Attributeにセットする処理
// Parameter     : attrKey (Attr Key)
//               : instance (Instance ID)
//               : pMessage (Message Data Pointer)
// Return        : true = セット実施
//               : false = セット未実施(セットできる情報なし)
///////////////////////////////////////////////////////////////////
bool SetDevCapabilitiesAttributeDevCont(UC attrKey, UC instance, CCdiFinDevCapaUpdateMsg* pMessage)
{
	CCapaDevContCapabilities *pDevContCapa;
	CCdiFinDevContCapabilitiesAttr attribute;
	const UC *pAttr;
	UC instCheckIndex, keyIndex;

	pDevContCapa = CCapaInformer_GetDeviceControllerCapabilities(instance);

	if ( pDevContCapa != NULL ){
		for ( instCheckIndex = 0; cCdi_DevContDataArray[instCheckIndex].instance != eCdiFinDevCont_Iot; instCheckIndex++ ){
			if ( cCdi_DevContDataArray[instCheckIndex].instance == instance ){
				pAttr = (&(cCdi_DevContDataArray[instCheckIndex]))->pCapaAttrArray;
		
				CCdiFinDevContCapabilitiesAttr_Reset(&attribute);
				CCdiFinDevContCapabilitiesAttr_SetAttrPairsPointer(&attribute, CCdiFinDevCapaUpdateMsg_GetAttrPairs(pMessage), CCdiFinDevCapaUpdateMsg_GetAttrPairsSize(pMessage));

				for ( keyIndex = 0; pAttr[keyIndex] != eCdiFinCapaKeyDevCont_All; keyIndex++ ){
					if ( (attrKey == eCdiFinCapaKeyDevCont_All) || (attrKey == pAttr[keyIndex]) ){
						switch ( pAttr[keyIndex] ){
						case eCdiFinCapaKeyDevCont_SoftVersion:	ExportDevCapaContAttr03(pDevContCapa, &attribute);	break;
						default:								break;
						}
					}
					else{ /* No Action */ }
				}

				CCdiFinDevCapaUpdateMsg_SetNumAttr(pMessage, CCdiFinDevContCapabilitiesAttr_GetNumAttr(&attribute));
				CCdiFinDevCapaUpdateMsg_SetAttrPairsSize(pMessage, CCdiFinDevContCapabilitiesAttr_GetAttrPairsSize(&attribute));

				return true;
			}
			else{
				// No Action
			}
		}
	}
	else{
		// No Action
	}

	CCdiFinDevCapaUpdateMsg_SetNumAttr(pMessage, 0);
	CCdiFinDevCapaUpdateMsg_SetAttrPairsSize(pMessage, 0);

	return false;
}

#if OUT_DEST_NUM != 0
///////////////////////////////////////////////////////////////////
// Function Name : SetDevCapabilitiesAttributeOutDest
// Description   : 保持情報をOutput Destination Capabilities 
//               : Attributeにセットする処理
// Parameter     : attrKey (Attr Key)
//               : instance (Instance ID)
//               : pMessage (Message Data Pointer)
// Return        : true = セット実施
//               : false = セット未実施(セットできる情報なし)
///////////////////////////////////////////////////////////////////
bool SetDevCapabilitiesAttributeOutDest(UC attrKey, UC instance, CCdiFinDevCapaUpdateMsg* pMessage)
{
	CCapaOutDestCapabilities *pOutDestCapa;
	CCdiFinOutDestCapabilitiesAttr attribute;
	const UC *pAttr;
	UC instCheckIndex, keyIndex;

	pOutDestCapa = CCapaInformer_GetOutputDestinationCapabilities(instance);

	if ( pOutDestCapa != NULL ){
		for ( instCheckIndex = 0; cCdi_OutDestDataArray[instCheckIndex].instance != eCdiFinOutDest_All; instCheckIndex++ ){
			if ( cCdi_OutDestDataArray[instCheckIndex].instance == instance ){
				pAttr = (&(cCdi_OutDestDataArray[instCheckIndex]))->pCapaAttrArray;

				CCdiFinOutDestCapabilitiesAttr_Reset(&attribute);
				CCdiFinOutDestCapabilitiesAttr_SetAttrPairsPointer(&attribute, CCdiFinDevCapaUpdateMsg_GetAttrPairs(pMessage), CCdiFinDevCapaUpdateMsg_GetAttrPairsSize(pMessage));

				for ( keyIndex = 0; pAttr[keyIndex] != eCdiFinCapaKeyOutDest_All; keyIndex++ ){
					if ( (attrKey == eCdiFinCapaKeyOutDest_All) || (attrKey == pAttr[keyIndex]) ){
						switch ( pAttr[keyIndex] ){
						case eCdiFinCapaKeyOutDest_MaxCapa:		ExportDevCapaDestAttr02(pOutDestCapa, &attribute);	break;
						case eCdiFinCapaKeyOutDest_UnitType:		ExportDevCapaDestAttr04(pOutDestCapa, &attribute);	break;
						case eCdiFinCapaKeyOutDest_Offset:			ExportDevCapaDestAttr47(pOutDestCapa, &attribute);	break;
						case eCdiFinCapaKeyOutDest_Fold:			ExportDevCapaDestAttr4B(pOutDestCapa, &attribute);	break;
						case eCdiFinCapaKeyOutDest_Staple:			ExportDevCapaDestAttr50(pOutDestCapa, &attribute);	break;
						case eCdiFinCapaKeyOutDest_Punch:			ExportDevCapaDestAttr55(pOutDestCapa, &attribute);	break;
						case eCdiFinCapaKeyOutDest_Book:			ExportDevCapaDestAttr5B(pOutDestCapa, &attribute);	break;
						case eCdiFinCapaKeyOutDest_BookTrayCapa:	ExportDevCapaDestAttr5E(pOutDestCapa, &attribute);	break;
						case eCdiFinCapaKeyOutDest_CountInStplSet:	ExportDevCapaDestAttrC2(pOutDestCapa, &attribute);	break;
						case eCdiFinCapaKeyOutDest_StapleType:		ExportDevCapaDestAttrC6(pOutDestCapa, &attribute);	break;
						case eCdiFinCapaKeyOutDest_BookStapleType:	ExportDevCapaDestAttrC7(pOutDestCapa, &attribute);	break;
						case eCdiFinCapaKeyOutDest_StplSpecMediaType:	ExportDevCapaDestAttrC8(pOutDestCapa, &attribute);	break;
						default:																						break;
						}
					}
					else{ /* No Action */ }
				}

				CCdiFinDevCapaUpdateMsg_SetNumAttr(pMessage, CCdiFinOutDestCapabilitiesAttr_GetNumAttr(&attribute));
				CCdiFinDevCapaUpdateMsg_SetAttrPairsSize(pMessage, CCdiFinOutDestCapabilitiesAttr_GetAttrPairsSize(&attribute));

				return true;
			}
			else{
				// No Action
			}
		}
	}
	else{
		// No Action
	}

	CCdiFinDevCapaUpdateMsg_SetNumAttr(pMessage, 0);
	CCdiFinDevCapaUpdateMsg_SetAttrPairsSize(pMessage, 0);

	return false;
}
#endif	// #if OUT_DEST_NUM != 0
#if IN_SRC_NUM != 0

///////////////////////////////////////////////////////////////////
// Function Name : SetDevCapabilitiesAttributeInSrc
// Description   : 保持情報をInput Source Capabilities Attributeに
//               : セットする処理
// Parameter     : attrKey (Attr Key)
//               : instance (Instance ID)
//               : pMessage (Message Data Pointer)
// Return        : true = セット実施
//               : false = セット未実施(セットできる情報なし)
///////////////////////////////////////////////////////////////////
bool SetDevCapabilitiesAttributeInSrc(UC attrKey, UC instance, CCdiFinDevCapaUpdateMsg* pMessage)
{
	CCapaInSrcCapabilities *pInSrcCapa;
	CCdiFinInSrcCapabilitiesAttr attribute;
	const UC *pAttr;
	UC instCheckIndex, keyIndex;

	pInSrcCapa = CCapaInformer_GetInputSourceCapabilities(instance);

	if ( pInSrcCapa != NULL ){
		for ( instCheckIndex = 0; cCdi_InSrcDataArray[instCheckIndex].instance != eCdiFinInSrc_All; instCheckIndex++ ){
			if ( cCdi_InSrcDataArray[instCheckIndex].instance == instance ){
				pAttr = (&(cCdi_InSrcDataArray[instance]))->pCapaAttrArray;

				CCdiFinInSrcCapabilitiesAttr_Reset(&attribute);
				CCdiFinInSrcCapabilitiesAttr_SetAttrPairsPointer(&attribute, CCdiFinDevCapaUpdateMsg_GetAttrPairs(pMessage), CCdiFinDevCapaUpdateMsg_GetAttrPairsSize(pMessage));

				for ( keyIndex = 0; pAttr[keyIndex] != eCdiFinCapaKeyInSrc_All; keyIndex++ ){
					if ( (attrKey == eCdiFinCapaKeyInSrc_All) || (attrKey == pAttr[keyIndex]) ){
						switch ( pAttr[keyIndex] ){
						case eCdiFinCapaKeyInSrc_MaxCapa:	ExportDevCapaSrcAttr01(pInSrcCapa, &attribute);	break;
						case eCdiFinCapaKeyInSrc_UnitType:	ExportDevCapaSrcAttr04(pInSrcCapa, &attribute);	break;
						default:																			break;
						}
					}
					else{ /* No Action */ }
				}

				CCdiFinDevCapaUpdateMsg_SetNumAttr(pMessage, CCdiFinInSrcCapabilitiesAttr_GetNumAttr(&attribute));
				CCdiFinDevCapaUpdateMsg_SetAttrPairsSize(pMessage, CCdiFinInSrcCapabilitiesAttr_GetAttrPairsSize(&attribute));

				return true;
			}
			else{
				// No Action
			}
		}
	}
	else{
		// No Action
	}

	CCdiFinDevCapaUpdateMsg_SetNumAttr(pMessage, 0);
	CCdiFinDevCapaUpdateMsg_SetAttrPairsSize(pMessage, 0);

	return false;
}
#endif	// IN_SRC_NUM != 0

///////////////////////////////////////////////////////////////////
// Function Name : SetDevConfigurationAttributeDevCont
// Description   : 保持情報をDevice Controller Configuration 
//               : Attributeにセットする処理
// Parameter     : attrKey (Attr Key)
//               : instance (Instance ID)
//               : pMessage (Message Data Pointer)
// Return        : true = セット実施
//               : false = セット未実施(セットできる情報なし)
///////////////////////////////////////////////////////////////////
bool SetDevConfigurationAttributeDevCont(UC attrKey, UC instance, CCdiFinDevCfgUpdateMsg* pMessage)
{
	CCfgDevContConfiguration *pDevContCfg;
	CCdiFinDevContConfigurationAttr attribute;
	const UC *pAttr;
	UC instCheckIndex, keyIndex;

	pDevContCfg = CCfgInformer_GetDeviceControllerConfiguration(instance);

	if ( pDevContCfg != NULL ){
		for ( instCheckIndex = 0; cCdi_DevContDataArray[instCheckIndex].instance != eCdiFinDevCont_Iot; instCheckIndex++ ){
			if ( cCdi_DevContDataArray[instCheckIndex].instance == instance ){
				pAttr = (&(cCdi_DevContDataArray[instCheckIndex]))->pCfgAttrArray;

				CCdiFinDevContConfigurationAttr_Reset(&attribute);
				CCdiFinDevContConfigurationAttr_SetAttrPairsPointer(&attribute, CCdiFinDevCfgUpdateMsg_GetAttrPairs(pMessage), CCdiFinDevCfgUpdateMsg_GetAttrPairsSize(pMessage));

				for ( keyIndex = 0; pAttr[keyIndex] != eCdiFinCfgKeyDevCont_All; keyIndex++ ){
					if ( ( (attrKey == eCdiFinCfgKeyDevCont_All) &&
						   (cCdi_DevContCfgUpdateAll[CDI_CFG_UPDATE_ALL_CONV_BYTE(pAttr[keyIndex])] & CDI_CFG_UPDATE_ALL_CONV_BIT(pAttr[keyIndex])) ) ||
						 (attrKey == pAttr[keyIndex]) ){
						switch ( pAttr[keyIndex] ){
						case eCdiFinCfgKeyDevCont_Market:		ExportDevCfgContAttr01(pDevContCfg, &attribute);	break;
						case eCdiFinCfgKeyDevCont_DateTime:		ExportDevCfgContAttr40(pDevContCfg, &attribute);	break;
						case eCdiFinCfgKeyDevCont_SysCfg:		ExportDevCfgContAttr41(pDevContCfg, &attribute);	break;
						case eCdiFinCfgKeyDevCont_MediaSizeGrp:	ExportDevCfgContAttrC0(pDevContCfg, &attribute);	break;
						case eCdiFinCfgKeyDevCont_ShtExtHeight:	ExportDevCfgContAttrFA(pDevContCfg, &attribute);	break;
						case eCdiFinCfgKeyDevCont_Productivity:	ExportDevCfgContAttrFB(pDevContCfg, &attribute);	break;
						case eCdiFinCfgKeyDevCont_ExitSpdRange:	ExportDevCfgContAttrFC(pDevContCfg, &attribute);	break;
						case eCdiFinCfgKeyDevCont_ExitLayout:	ExportDevCfgContAttrFD(pDevContCfg, &attribute);	break;
						case eCdiFinCfgKeyDevCont_JamBypass:	ExportDevCfgContAttrFE(pDevContCfg, &attribute);	break;
						case eCdiFinCfgKeyDevCont_NoPprRun:		ExportDevCfgContAttrFF(pDevContCfg, &attribute);	break;
						default:								break;
						}
					}
					else{ /* No Action */ }
				}

				CCdiFinDevCfgUpdateMsg_SetNumAttr(pMessage, CCdiFinDevContConfigurationAttr_GetNumAttr(&attribute));
				CCdiFinDevCfgUpdateMsg_SetAttrPairsSize(pMessage, CCdiFinDevContConfigurationAttr_GetAttrPairsSize(&attribute));

				return true;
			}
			else{
				// No Action
			}
		}
	}
	else{
		// No Action
	}

	CCdiFinDevCfgUpdateMsg_SetNumAttr(pMessage, 0);
	CCdiFinDevCfgUpdateMsg_SetAttrPairsSize(pMessage, 0);

	return false;
}

#if OUT_DEST_NUM != 0
///////////////////////////////////////////////////////////////////
// Function Name : SetDevConfigurationAttributeOutDest
// Description   : 保持情報をOutput Destination Configuration 
//               : Attributeにセットする処理
// Parameter     : attrKey (Attr Key)
//               : instance (Instance ID)
//               : pMessage (Message Data Pointer)
// Return        : true = セット実施
//               : false = セット未実施(セットできる情報なし)
///////////////////////////////////////////////////////////////////
bool SetDevConfigurationAttributeOutDest(UC attrKey, UC instance, CCdiFinDevCfgUpdateMsg* pMessage)
{
	CCfgOutDestConfiguration *pOutDestCfg;
	CCdiFinOutDestConfigurationAttr attribute;
	const UC *pAttr;
	UC instCheckIndex, keyIndex;

	pOutDestCfg = CCfgInformer_GetOutputDestinationConfiguration(instance);

	if ( pOutDestCfg != NULL ){
		for ( instCheckIndex = 0; cCdi_OutDestDataArray[instCheckIndex].instance != eCdiFinOutDest_All; instCheckIndex++ ){
			if ( cCdi_OutDestDataArray[instCheckIndex].instance == instance ){
				pAttr = (&(cCdi_OutDestDataArray[instCheckIndex]))->pCfgAttrArray;

				CCdiFinOutDestConfigurationAttr_Reset(&attribute);
				CCdiFinOutDestConfigurationAttr_SetAttrPairsPointer(&attribute, CCdiFinDevCfgUpdateMsg_GetAttrPairs(pMessage), CCdiFinDevCfgUpdateMsg_GetAttrPairsSize(pMessage));

				for ( keyIndex = 0; pAttr[keyIndex] != eCdiFinCfgKeyOutDest_All; keyIndex++ ){
					if ( ( (attrKey == eCdiFinCfgKeyOutDest_All) &&
						   (cCdi_OutDestCfgUpdateAll[CDI_CFG_UPDATE_ALL_CONV_BYTE(pAttr[keyIndex])] & CDI_CFG_UPDATE_ALL_CONV_BIT(pAttr[keyIndex])) ) ||
						 (attrKey == pAttr[keyIndex]) ){
						switch ( pAttr[keyIndex] ){
						case eCdiFinCfgKeyOutDest_Select:	ExportDevCfgDestAttr03(pOutDestCfg, &attribute);	break;
						case eCdiFinCfgKeyOutDest_Offset:	ExportDevCfgDestAttr45(pOutDestCfg, &attribute);	break;
						case eCdiFinCfgKeyOutDest_Fold:		ExportDevCfgDestAttr46(pOutDestCfg, &attribute);	break;
						case eCdiFinCfgKeyOutDest_Staple:	ExportDevCfgDestAttr47(pOutDestCfg, &attribute);	break;
						case eCdiFinCfgKeyOutDest_Punch:	ExportDevCfgDestAttr48(pOutDestCfg, &attribute);	break;
						case eCdiFinCfgKeyOutDest_Book:		ExportDevCfgDestAttr49(pOutDestCfg, &attribute);	break;
						default:																				break;
						}
					}
					else{ /* No Action */ }
				}

				CCdiFinDevCfgUpdateMsg_SetNumAttr(pMessage, CCdiFinOutDestConfigurationAttr_GetNumAttr(&attribute));
				CCdiFinDevCfgUpdateMsg_SetAttrPairsSize(pMessage, CCdiFinOutDestConfigurationAttr_GetAttrPairsSize(&attribute));

				return true;
			}
			else{
				// No Action
			}
		}
	}
	else{
		// No Action
	}

	CCdiFinDevCfgUpdateMsg_SetNumAttr(pMessage, 0);
	CCdiFinDevCfgUpdateMsg_SetAttrPairsSize(pMessage, 0);

	return false;
}

#endif	// OUT_DEST_NUM != 0
#if IN_SRC_NUM != 0
///////////////////////////////////////////////////////////////////
// Function Name : SetDevConfigurationAttributeInSrc
// Description   : 保持情報をInput Source Configuration Attributeに
//               : セットする処理
// Parameter     : attrKey (Attr Key)
//               : instance (Instance ID)
//               : pMessage (Message Data Pointer)
// Return        : true = セット実施
//               : false = セット未実施(セットできる情報なし)
///////////////////////////////////////////////////////////////////
bool SetDevConfigurationAttributeInSrc(UC attrKey, UC instance, CCdiFinDevCfgUpdateMsg* pMessage)
{
	CCfgInSrcConfiguration *pInSrcCfg;
	CCdiFinInSrcConfigurationAttr attribute;
	const UC *pAttr;
	UC instCheckIndex, keyIndex;

	pInSrcCfg = CCfgInformer_GetInputSourceConfiguration(instance);

	if ( pInSrcCfg != NULL ){
		for ( instCheckIndex = 0; cCdi_InSrcDataArray[instCheckIndex].instance != eCdiFinInSrc_All; instCheckIndex++ ){
			if ( cCdi_InSrcDataArray[instCheckIndex].instance == instance ){
				pAttr = (&(cCdi_InSrcDataArray[instCheckIndex]))->pCfgAttrArray;

				CCdiFinInSrcConfigurationAttr_Reset(&attribute);
				CCdiFinInSrcConfigurationAttr_SetAttrPairsPointer(&attribute, CCdiFinDevCfgUpdateMsg_GetAttrPairs(pMessage), CCdiFinDevCfgUpdateMsg_GetAttrPairsSize(pMessage));

				for ( keyIndex = 0; pAttr[keyIndex] != eCdiFinCfgKeyInSrc_All; keyIndex++ ){
					if ( ( (attrKey == eCdiFinCfgKeyInSrc_All) &&
						   (cCdi_InSrcCfgUpdateAll[CDI_CFG_UPDATE_ALL_CONV_BYTE(pAttr[keyIndex])] & CDI_CFG_UPDATE_ALL_CONV_BIT(pAttr[keyIndex])) ) ||
						 (attrKey == pAttr[keyIndex]) ){
						switch ( pAttr[keyIndex] ){
						case eCdiFinCfgKeyInSrc_Select:		ExportDevCfgSrcAttr03(pInSrcCfg, &attribute);	break;
						case eCdiFinCfgKeyInSrc_SizeAlt:	ExportDevCfgSrcAttrC0(pInSrcCfg, &attribute);	break;
						case eCdiFinCfgKeyInSrc_IregSize:	ExportDevCfgSrcAttrC1(pInSrcCfg, &attribute);	break;
						default:																			break;
						}
					}
					else{ /* No Action */ }
				}

				CCdiFinDevCfgUpdateMsg_SetNumAttr(pMessage, CCdiFinInSrcConfigurationAttr_GetNumAttr(&attribute));
				CCdiFinDevCfgUpdateMsg_SetAttrPairsSize(pMessage, CCdiFinInSrcConfigurationAttr_GetAttrPairsSize(&attribute));

				return true;
			}
			else{
				// No Action
			}
		}
	}
	else{
		// No Action
	}

	CCdiFinDevCfgUpdateMsg_SetNumAttr(pMessage, 0);
	CCdiFinDevCfgUpdateMsg_SetAttrPairsSize(pMessage, 0);

	return false;
}
#endif	// IN_SRC_NUM != 0

///////////////////////////////////////////////////////////////////
// Function Name : SetSheetAttribute
// Description   : 受信したSheet Finish Attributeをシート情報にセッ
//               : トする処理
// Parameter     : pShtAttr (シートアトリビュートポインタ)
//               : pShtFinAttr (Pointer of Sheet Finish Attribute)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SetSheetAttribute(CShtSheetAttribute* pShtAttr, CCdiFinSheetFinishAttr* pShtFinAttr)
{
	UC* pAttrData;
	UC index, times;

	// --- Offset Mode(01H) ---
	pAttrData = CCdiFinSheetFinishAttr_GetAttributePointer(pShtFinAttr, eCdiFinShtFinAttr_OfstMode);
	if ( pAttrData != NULL ){
		CShtSheetAttribute_SetOffsetMode(pShtAttr, CCdiFinShtFinAttr01_GetOffsetMode(pAttrData));
	}
	else{ /* No Action */ }

	// --- Staple Mode(02H) ---
	pAttrData = CCdiFinSheetFinishAttr_GetAttributePointer(pShtFinAttr, eCdiFinShtFinAttr_StplMode);
	if ( pAttrData != NULL ){
		CShtSheetAttribute_SetStapleMode(pShtAttr, CCdiFinShtFinAttr02_GetOffsetMode(pAttrData));
	}
	else{ /* No Action */ }

	// --- Punch Mode(03H) ---
	pAttrData = CCdiFinSheetFinishAttr_GetAttributePointer(pShtFinAttr, eCdiFinShtFinAttr_PnchMode);
	if ( pAttrData != NULL ){
		CShtSheetAttribute_SetPunchMode(pShtAttr, CCdiFinShtFinAttr03_GetPunchMode(pAttrData));
	}
	else{ /* No Action */ }

	// --- Booklet Maker Mode(04H) ---
	pAttrData = CCdiFinSheetFinishAttr_GetAttributePointer(pShtFinAttr, eCdiFinShtFinAttr_BkltMode);
	if ( pAttrData != NULL ){
		CShtSheetAttribute_SetBookletMakerMode(pShtAttr, CCdiFinShtFinAttr04_GetBookletMode(pAttrData));
	}
	else{ /* No Action */ }

	// --- Insert Sheet Mode(06H) ---
	// Not Supported

	// --- Fold Mode(07H) ---
	pAttrData = CCdiFinSheetFinishAttr_GetAttributePointer(pShtFinAttr, eCdiFinShtFinAttr_FoldMode);
	if ( pAttrData != NULL ){
		CShtSheetAttribute_SetFoldMode(pShtAttr, CCdiFinShtFinAttr07_GetFoldMode(pAttrData));
	}
	else{ /* No Action */ }

	// --- Media Type(08H) ---
	pAttrData = CCdiFinSheetFinishAttr_GetAttributePointer(pShtFinAttr, eCdiFinShtFinAttr_MediaType);
	if ( pAttrData != NULL ){
		CShtSheetAttribute_SetMediaType(pShtAttr, CCdiFinShtFinAttr08_GetMediaType(pAttrData));
		CShtSheetAttribute_SetMediaWeight(pShtAttr, CCdiFinShtFinAttr08_GetMediaWeight(pAttrData));
		CShtSheetAttribute_SetEnvelope(pShtAttr, CCdiFinShtFinAttr08_GetEnvelope(pAttrData));
		CShtSheetAttribute_SetTabbedPaper(pShtAttr, CCdiFinShtFinAttr08_GetTabbedPaper(pAttrData));
		CShtSheetAttribute_SetReusedPaper(pShtAttr, CCdiFinShtFinAttr08_GetReusedPaper(pAttrData));
		CShtSheetAttribute_SetPrepunchedMaterial(pShtAttr, CCdiFinShtFinAttr08_GetPrepunchedMaterial(pAttrData));
	}
	else{ /* No Action */ }

	// --- Media Size(09H) ---
	pAttrData = CCdiFinSheetFinishAttr_GetAttributePointer(pShtFinAttr, eCdiFinShtFinAttr_MediaSize);
	if ( pAttrData != NULL ){
		CShtSheetAttribute_SetProcessSize(pShtAttr, CCdiFinShtFinAttr09_GetProcessSize(pAttrData));
		CShtSheetAttribute_SetCrossProcSize(pShtAttr, CCdiFinShtFinAttr09_GetCrossProcSize(pAttrData));
	}
	else{ /* No Action */ }

	// --- Sheet Curl(0BH) ---
	pAttrData = CCdiFinSheetFinishAttr_GetAttributePointer(pShtFinAttr, eCdiFinShtFinAttr_SheetCurl);
	if ( pAttrData != NULL ){
		CShtSheetAttribute_SetCurl(pShtAttr, CCdiFinShtFinAttr0B_GetSheetCurl(pAttrData));
		CShtSheetAttribute_SetCurlQuantity(pShtAttr, CCdiFinShtFinAttr0B_GetCurlQuantity(pAttrData));
	}
	else{ /* No Action */ }

	// --- Exit Speed(0CH) ---
	pAttrData = CCdiFinSheetFinishAttr_GetAttributePointer(pShtFinAttr, eCdiFinShtFinAttr_ExitSpd);
	if ( pAttrData != NULL ){
		times = CCdiFinShtFinAttr0C_GetSpeedChangeTimes(pAttrData);

		CShtSheetAttribute_SetInitialSpeed(pShtAttr, CCdiFinShtFinAttr0C_GetInitialSpeed(pAttrData));
		CShtSheetAttribute_SetSpeedChangeTimes(pShtAttr, times);

		for ( index = 1; index <= times; index++ ){
			CShtSheetAttribute_SetChangeSpeedTiming(pShtAttr, CCdiFinShtFinAttr0C_GetChangeSpeedTiming(pAttrData, index), index);
			CShtSheetAttribute_SetChangeSpeed(pShtAttr, CCdiFinShtFinAttr0C_GetChangeSpeed(pAttrData, index), index);
		}
	}
	else{ /* No Action */ }

	// --- Sheet Integrity(0DH) ---
	pAttrData = CCdiFinSheetFinishAttr_GetAttributePointer(pShtFinAttr, eCdiFinShtFinAttr_ShtIntegrity);
	if ( pAttrData != NULL ){
		CShtSheetAttribute_SetSheetIntegrity(pShtAttr, CCdiFinShtFinAttr0D_GetSheetIntegrity(pAttrData));
	}
	else{ /* No Action */ }

	// --- Set Action(20H) ---
	pAttrData = CCdiFinSheetFinishAttr_GetAttributePointer(pShtFinAttr, eCdiFinShtFinAttr_SetAction);
	if ( pAttrData != NULL ){
		CShtSheetAttribute_SetSetAction(pShtAttr, CCdiFinShtFinAttr20_GetSetAction(pAttrData));
	}
	else{ /* No Action */ }

	// --- Offset Position(C0H) ---
	pAttrData = CCdiFinSheetFinishAttr_GetAttributePointer(pShtFinAttr, eCdiFinShtFinAttr_OfstPos);
	if ( pAttrData != NULL ){
		CShtSheetAttribute_SetOffsetPosition(pShtAttr, CCdiFinShtFinAttrC0_GetOffsetPosition(pAttrData));
	}
	else{ /* No Action */ }

	// --- Booklet Maker Custom Adjust Table No.(C2H) ---
	// Not Supported

	// --- Fold Custom Adjust Table No.(C3H) ---
	// Not Supported

	// --- Exit Sheet Interval(C4H) ---
	pAttrData = CCdiFinSheetFinishAttr_GetAttributePointer(pShtFinAttr, eCdiFinShtFinAttr_ExitShtInterval);
	if ( pAttrData != NULL ){
		CShtSheetAttribute_SetExitSheetInterval(pShtAttr, CCdiFinShtFinAttrC4_GetExitSheetInterval(pAttrData));
	}
	else{ /* No Action */ }

	// --- Insert Sheet Interval(C5H) ---
	// Not Supported

	// --- Square Fold Reset(C6H) ---
	// Not Support

	// --- Square Folding Level(C7H) ---
	// Not Support

	// --- Booklet Edge Trimming Size(C8H) ---
	// Not Support

	// --- PB Kind Of Book Sheet(C9H) ---
	// Not Support

	// --- DFA Profile No.(CAH) ---
	// Not Support

	// --- DFA Job End(CBH) ---
	// Not Support

	// --- DFA Set End(CCH) ---
	// Not Support

	// --- Perfect Bind Adjust Data(CDH) ---
	// Not Support

	// --- Cross Process Direction Trimming Size(CFH) ---
	// Not Support

	// --- Crease Position(D0H) ---
	// Not Support

	// --- Blocking Countermeasure Exit Mode(D4H) ---
	// Not Support
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdDevCapabilitiesUpdate
// Description   : Device Capabilities Updateメッセージ送信処理
// Parameter     : subsystem (Subsytem ID)
//               : instance (Instance ID)
//               : attrKey (Attr Key)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdDevCapabilitiesUpdate(UC subsystem, UC instance, UC attrKey)
{
	static const UC cFwdCond = CDI_ADAPT_EVT_CNV_BIT(CDI_ADAPT_EVT_SEND_INIT_INFO)|CDI_ADAPT_EVT_CNV_BIT(CDI_ADAPT_EVT_CAPA_REQ);
	CCdiFinDevCapaUpdateMsg message;
	const CdiInstanceData *pSubSysData;
	UC msgInstance[CDIFIN_MAXMSGSIZE];
	UC prdctStt, keyAllFlg, instIndex;

	CCdiFinDevCapaUpdateMsg_Constructor(&message);

	prdctStt = CCdiProductionState_GetState(&(gCdi_AdapterMember.mProductState));

	if ( (prdctStt == eCdiFinPrdctStt_Standby) ||
		 (prdctStt == eCdiFinPrdctStt_Running) ||
		 ((gCdi_AdapterMember.mEventBit & cFwdCond) == cFwdCond) ){				// Power Up Sequence中は情報確定済みでRequest受信済みかどうかをチェック
		keyAllFlg = false;

		// --- Finisher Device Controller ---
		if ( (subsystem == eCdiFinSubsys_DevCont) || (subsystem == eCdiFinSubsys_UpdateAll) || (subsystem == eCdiFinSubsys_PwrSavrUpdate) ){
			if ( attrKey == eCdiFinCapaKeyDevCont_All ) keyAllFlg = true;

			for ( instIndex = 0; (&(cCdi_DevContDataArray[instIndex]))->pCapaAttrArray != NULL; instIndex++ ){
				pSubSysData = &(cCdi_DevContDataArray[instIndex]);

				if ( (instance == pSubSysData->instance ) || (subsystem == eCdiFinSubsys_UpdateAll) || (subsystem == eCdiFinSubsys_PwrSavrUpdate) ){
					CCdiFinDevCapaUpdateMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));
					CCdiFinDevCapaUpdateMsg_SetSubsystem(&message, eCdiFinSubsys_DevCont);
					CCdiFinDevCapaUpdateMsg_SetInstance(&message, pSubSysData->instance);

					if ( SetDevCapabilitiesAttributeDevCont(attrKey, pSubSysData->instance, &message) == true ){
						if ( (attrKey == eCdiFinCapaKeyDevCont_All) || (CCdiFinDevCapaUpdateMsg_GetNumAttr(&message) != 0 ) ){
							MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDevCapaUpdateMsg_GetMessage(&message), CCdiFinDevCapaUpdateMsg_GetSize(&message), LOG_ON);
						}
						else{ /* Not Send */ }
					}
					else{ /* No Action */ }
				}
				else{ /* No Action */ }
			}
		}
		else{ /* No Action */ }

		// --- Output Destination ---
#if OUT_DEST_NUM != 0
		if ( (subsystem == eCdiFinSubsys_OutDest) || (subsystem == eCdiFinSubsys_UpdateAll) || (subsystem == eCdiFinSubsys_PwrSavrUpdate) ){
			if ( attrKey == eCdiFinCapaKeyOutDest_All ) keyAllFlg = true;

			for ( instIndex = 0; (&(cCdi_OutDestDataArray[instIndex]))->pCapaAttrArray != NULL; instIndex++ ){
				pSubSysData = &(cCdi_OutDestDataArray[instIndex]);

				if ( (instance == pSubSysData->instance ) || (subsystem == eCdiFinSubsys_UpdateAll) || (subsystem == eCdiFinSubsys_PwrSavrUpdate) ){
					CCdiFinDevCapaUpdateMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));
					CCdiFinDevCapaUpdateMsg_SetSubsystem(&message, eCdiFinSubsys_OutDest);
					CCdiFinDevCapaUpdateMsg_SetInstance(&message, pSubSysData->instance);

					if ( SetDevCapabilitiesAttributeOutDest(attrKey, pSubSysData->instance, &message) == true ){
						if ( (attrKey == eCdiFinCapaKeyOutDest_All) || (CCdiFinDevCapaUpdateMsg_GetNumAttr(&message) != 0 ) ){
							MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDevCapaUpdateMsg_GetMessage(&message), CCdiFinDevCapaUpdateMsg_GetSize(&message), LOG_ON);
						}
						else{ /* Not Send */ }
					}
					else{ /* No Action */ }
				}
				else{ /* No Action */ }
			}
		}
		else{ /* No Action */ }
#endif	// #if OUT_DEST_NUM != 0

		// --- Input Source ---
#if IN_SRC_NUM != 0
		if ( (subsystem == eCdiFinSubsys_InSrc) || (subsystem == eCdiFinSubsys_UpdateAll) || (subsystem == eCdiFinSubsys_PwrSavrUpdate) ){
			if ( attrKey == eCdiFinCapaKeyInSrc_All ) keyAllFlg = true;

			for ( instIndex = 0; (&(cCdi_InSrcDataArray[instIndex]))->pCapaAttrArray != NULL; instIndex++ ){
				pSubSysData = &(cCdi_InSrcDataArray[instIndex]);

				if ( (instance == pSubSysData->instance ) || (subsystem == eCdiFinSubsys_UpdateAll) || (subsystem == eCdiFinSubsys_PwrSavrUpdate) ){
					CCdiFinDevCapaUpdateMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));
					CCdiFinDevCapaUpdateMsg_SetSubsystem(&message, eCdiFinSubsys_InSrc);
					CCdiFinDevCapaUpdateMsg_SetInstance(&message, pSubSysData->instance);

					if ( SetDevCapabilitiesAttributeInSrc(attrKey, pSubSysData->instance, &message) == true ){
						if ( (attrKey == eCdiFinCapaKeyInSrc_All) || (CCdiFinDevCapaUpdateMsg_GetNumAttr(&message) != 0 ) ){
							MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDevCapaUpdateMsg_GetMessage(&message), CCdiFinDevCapaUpdateMsg_GetSize(&message), LOG_ON);
						}
						else{ /* Not Send */ }
					}
					else{ /* No Action */ }
				}
				else{ /* No Action */ }
			}
		}
		else{ /* No Action */ }
#endif	// #if IN_SRC_NUM != 0

		// --- Update Complete ---
		if ( (subsystem == eCdiFinSubsys_UpdateAll) || (subsystem == eCdiFinSubsys_PwrSavrUpdate) || (keyAllFlg == true) ){
			CCdiFinDevCapaUpdateMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));
			CCdiFinDevCapaUpdateMsg_SetSubsystem(&message, eCdiFinSubsys_UpdateComp);
			CCdiFinDevCapaUpdateMsg_SetInstance(&message, (UC)0x00);
			CCdiFinDevCapaUpdateMsg_SetNumAttr(&message, 0);
			CCdiFinDevCapaUpdateMsg_SetAttrPairsSize(&message, 0);

			MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDevCapaUpdateMsg_GetMessage(&message), CCdiFinDevCapaUpdateMsg_GetSize(&message), LOG_ON);
																				// Forward Device Configuration Update(Complete) Message
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
// Function Name : FwdDevConfigurationUpdate
// Description   : Device Configuration Updateメッセージ送信処理
// Parameter     : subsystem (Subsytem ID)
//               : instance (Instance ID)
//               : attrKey (Attr Key)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdDevConfigurationUpdate(UC subsystem, UC instance, UC attrKey)
{
	static const UC cFwdCond = CDI_ADAPT_EVT_CNV_BIT(CDI_ADAPT_EVT_SEND_INIT_INFO)|CDI_ADAPT_EVT_CNV_BIT(CDI_ADAPT_EVT_CFG_REQ);
	CCdiFinDevCfgUpdateMsg message;
	const CdiInstanceData *pSubSysData;
	UC msgInstance[CDIFIN_MAXMSGSIZE];
	UC prdctStt, keyAllFlg, instIndex;

	CCdiFinDevCfgUpdateMsg_Constructor(&message);

	prdctStt = CCdiProductionState_GetState(&(gCdi_AdapterMember.mProductState));

	if ( (prdctStt == eCdiFinPrdctStt_Standby) ||
		 (prdctStt == eCdiFinPrdctStt_Running) ||
		 ((gCdi_AdapterMember.mEventBit & cFwdCond) == cFwdCond) ){				// Power Up Sequence中は情報確定済みでRequest受信済みかどうかをチェック
		keyAllFlg = false;

		// --- Finisher Device Controller ---
		if ( (subsystem == eCdiFinSubsys_DevCont) || (subsystem == eCdiFinSubsys_UpdateAll) || (subsystem == eCdiFinSubsys_PwrSavrUpdate) ){
			if ( attrKey == eCdiFinCfgKeyDevCont_All ) keyAllFlg = true;

			for ( instIndex = 0; (&(cCdi_DevContDataArray[instIndex]))->pCfgAttrArray != NULL; instIndex++ ){
				pSubSysData = &(cCdi_DevContDataArray[instIndex]);

				if ( (instance == pSubSysData->instance ) || (subsystem == eCdiFinSubsys_UpdateAll) || (subsystem == eCdiFinSubsys_PwrSavrUpdate) ){
					CCdiFinDevCfgUpdateMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));
					CCdiFinDevCfgUpdateMsg_SetSubsystem(&message, eCdiFinSubsys_DevCont);
					CCdiFinDevCfgUpdateMsg_SetInstance(&message, pSubSysData->instance);

					if ( SetDevConfigurationAttributeDevCont(attrKey, pSubSysData->instance, &message) == true ){
						if ( (attrKey == eCdiFinCfgKeyDevCont_All) || (CCdiFinDevCfgUpdateMsg_GetNumAttr(&message) != 0 ) ){
							MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDevCfgUpdateMsg_GetMessage(&message), CCdiFinDevCfgUpdateMsg_GetSize(&message), LOG_ON);
						}
						else{ /* Not Send */ }
					}
					else{ /* No Action */ }
				}				
				else{ /* No Action */ }
			}
		}
		else{ /* No Action */ }

		// --- Output Destination ---
#if OUT_DEST_NUM != 0
		if ( (subsystem == eCdiFinSubsys_OutDest) || (subsystem == eCdiFinSubsys_UpdateAll) || (subsystem == eCdiFinSubsys_PwrSavrUpdate) ){
			if ( attrKey == eCdiFinCfgKeyOutDest_All ) keyAllFlg = true;

			for ( instIndex = 0; (&(cCdi_OutDestDataArray[instIndex]))->pCfgAttrArray != NULL; instIndex++ ){
				pSubSysData = &(cCdi_OutDestDataArray[instIndex]);

				if ( (instance == pSubSysData->instance ) || (subsystem == eCdiFinSubsys_UpdateAll) || (subsystem == eCdiFinSubsys_PwrSavrUpdate) ){
					CCdiFinDevCfgUpdateMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));
					CCdiFinDevCfgUpdateMsg_SetSubsystem(&message, eCdiFinSubsys_OutDest);
					CCdiFinDevCfgUpdateMsg_SetInstance(&message, pSubSysData->instance);

					if ( SetDevConfigurationAttributeOutDest(attrKey, pSubSysData->instance, &message) == true ){
						if ( (attrKey == eCdiFinCfgKeyOutDest_All) || (CCdiFinDevCfgUpdateMsg_GetNumAttr(&message) != 0 ) ){
							MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDevCfgUpdateMsg_GetMessage(&message), CCdiFinDevCfgUpdateMsg_GetSize(&message), LOG_ON);
						}
						else{ /* Not Send */ }
					}
					else{ /* No Action */ }
				}
				else{ /* No Action */ }
			}
		}
		else{ /* No Action */ }
#endif	// #if OUT_DEST_NUM != 0

		// --- Input Source ---
#if IN_SRC_NUM != 0
		if ( (subsystem == eCdiFinSubsys_InSrc) || (subsystem == eCdiFinSubsys_UpdateAll) || (subsystem == eCdiFinSubsys_PwrSavrUpdate) ){
			if ( attrKey == eCdiFinCfgKeyInSrc_All ) keyAllFlg = true;

			for ( instIndex = 0; (&(cCdi_InSrcDataArray[instIndex]))->pCfgAttrArray != NULL; instIndex++ ){
				pSubSysData = &(cCdi_InSrcDataArray[instIndex]);

				if ( (instance == pSubSysData->instance ) || (subsystem == eCdiFinSubsys_UpdateAll) || (subsystem == eCdiFinSubsys_PwrSavrUpdate) ){
					CCdiFinDevCfgUpdateMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));
					CCdiFinDevCfgUpdateMsg_SetSubsystem(&message, eCdiFinSubsys_InSrc);
					CCdiFinDevCfgUpdateMsg_SetInstance(&message, pSubSysData->instance);

					if ( SetDevConfigurationAttributeInSrc(attrKey, pSubSysData->instance, &message) == true ){
						if ( (attrKey == eCdiFinCfgKeyInSrc_All) || (CCdiFinDevCfgUpdateMsg_GetNumAttr(&message) != 0 ) ){
							MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDevCfgUpdateMsg_GetMessage(&message), CCdiFinDevCfgUpdateMsg_GetSize(&message), LOG_ON);
						}
						else{ /* Not Send */ }
					}
					else{ /* No Action */ }
				}
				else{ /* No Action */ }
			}
		}
		else{ /* No Action */ }
#endif	// #if IN_SRC_NUM != 0

		// --- Update Complete ---
		if ( (subsystem == eCdiFinSubsys_UpdateAll) || (subsystem == eCdiFinSubsys_PwrSavrUpdate) || (keyAllFlg  == true) ){
			CCdiFinDevCfgUpdateMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));
			CCdiFinDevCfgUpdateMsg_SetSubsystem(&message, eCdiFinSubsys_UpdateComp);
			CCdiFinDevCfgUpdateMsg_SetInstance(&message, 0x00);
			CCdiFinDevCfgUpdateMsg_SetNumAttr(&message, 0);
			CCdiFinDevCfgUpdateMsg_SetAttrPairsSize(&message, 0);

			MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDevCfgUpdateMsg_GetMessage(&message), CCdiFinDevCfgUpdateMsg_GetSize(&message), LOG_ON);
		}
		else{ /* No Action */ }
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdInitRequest
// Description   : Initialization Requestメッセージ送信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdInitRequest()
{
	CCdiFinInitReqMsg message;

	CCdiFinInitReqMsg_Constructor(&message);

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinInitReqMsg_GetMessage(&message), CCdiFinInitReqMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdInitAcknowledge
// Description   : Initialization Acknowledgeメッセージ送信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdInitAcknowledge()
{
	CCdiFinInitAckMsg message;

	CCdiFinInitAckMsg_Constructor(&message);

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinInitAckMsg_GetMessage(&message), CCdiFinInitAckMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdPwrOffAcknowledge
// Description   : Power Off Acknowledgeメッセージ送信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdPwrOffAcknowledge()
{
	CCdiFinPwrOffAckMsg message;

	CCdiFinPwrOffAckMsg_Constructor(&message);

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinPwrOffAckMsg_GetMessage(&message), CCdiFinPwrOffAckMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdReadyForNextSheet
// Description   : Ready For Next Sheetメッセージ送信処理
// Parameter     : sheetNo (Sheet No.)
// Return        : ‐
// 20160225 V3.4.0 Merge [ CDIメッセージ送信処理スタック量削減 ]
///////////////////////////////////////////////////////////////////
void FwdReadyForNextSheet(UC sheetNo)
{
	CCdiFinRdyForNxtShtMsg message;
	UC msgInstance[CCdiFinRdyForNxtShtMsg_GetSize(&message)];
	const CShtSheet *pSheet;

	CCdiFinRdyForNxtShtMsg_Constructor(&message);
	CCdiFinRdyForNxtShtMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	pSheet = CShtManager_GetSheet(sheetNo);
	
	CCdiFinRdyForNxtShtMsg_SetSetID(&message, CShtSheet_GetSetID(pSheet));
	CCdiFinRdyForNxtShtMsg_SetSheetID(&message, CShtSheet_GetSheetID(pSheet));
	CCdiFinRdyForNxtShtMsg_SetPrepTime(&message, CShtSheet_GetPrepTime(pSheet));
	CCdiFinRdyForNxtShtMsg_SetDispersionTime(&message, CShtSheet_GetDispersionTime(pSheet));

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinRdyForNxtShtMsg_GetMessage(&message), CCdiFinRdyForNxtShtMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdSheetFed
// Description   : Sheet Fedメッセージ送信処理
// Parameter     : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdSheetFed(UC sheetNo)
{
	// Not Supported
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdFeedAborted
// Description   : Feed Abortedメッセージ送信処理
// Parameter     : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdFeedAborted(UC sheetNo)
{
	// Not Supported
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdSheetDelvToOutput
// Description   : Sheet Delivered To Outputメッセージ送信処理
// Parameter     : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdSheetDelvToOutput(UC sheetNo)
{
	CCdiFinShtDelvToOutMsg message;
	UC msgInstance[CCdiFinShtDelvToOutMsg_GetSize(&message)];
	const CShtSheet *pSheet;
	UC offsetPosition;

	CCdiFinShtDelvToOutMsg_Constructor(&message);
	CCdiFinShtDelvToOutMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	pSheet = CShtManager_GetSheet(sheetNo);
	offsetPosition = CShtManager_ConvOffsetPosition((EShtOffsetFinish)CShtSheet_GetOffsetFinish(pSheet));

	CCdiFinShtDelvToOutMsg_SetSetID(&message, CShtSheet_GetSetID(pSheet));
	CCdiFinShtDelvToOutMsg_SetSheetID(&message, CShtSheet_GetSheetID(pSheet));
	CCdiFinShtDelvToOutMsg_SetDestID(&message, CShtSheet_GetOutputDestFinish(pSheet));
	CCdiFinShtDelvToOutMsg_SetBinSelect(&message, CShtSheet_GetBinSelection(pSheet));
	CCdiFinShtDelvToOutMsg_SetSheetIntegrity(&message, (UC)CShtSheet_GetSheetIntegrity(pSheet));
	CCdiFinShtDelvToOutMsg_SetOffsetPosition(&message, offsetPosition);

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinShtDelvToOutMsg_GetMessage(&message), CCdiFinShtDelvToOutMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdSetFinAtOutput
// Description   : Set Finished At Outputメッセージ送信処理
// Parameter     : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdSetFinAtOutput(UC sheetNo)
{
	CCdiFinSetFinAtOutMsg message;
	UC msgInstance[CCdiFinSetFinAtOutMsg_GetSize(&message)];
	const CShtSheet *pSheet;

	CCdiFinSetFinAtOutMsg_Constructor(&message);
	CCdiFinSetFinAtOutMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	pSheet = CShtManager_GetSheet(sheetNo);

	CCdiFinSetFinAtOutMsg_SetSetID(&message, CShtSheet_GetSetID(pSheet));
	CCdiFinSetFinAtOutMsg_SetDestID(&message, CShtSheet_GetOutputDestFinish(pSheet));
	CCdiFinSetFinAtOutMsg_SetBinSelect(&message, CShtSheet_GetBinSelection(pSheet));
	CCdiFinSetFinAtOutMsg_SetSetIntegrity(&message, CShtSheet_GetSetIntegrity(pSheet));

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinSetFinAtOutMsg_GetMessage(&message), CCdiFinSetFinAtOutMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdSetRecoveryReq
// Description   : Set Recovery Requestメッセージ送信処理
// Parameter     : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdSetRecoveryReq(UC sheetNo)
{
	CCdiFinSetRecoveryMsg message;
	UC msgInstance[CCdiFinSetRecoveryMsg_GetSize(&message)];
	const CShtSheet *pSheet;

	CCdiFinSetRecoveryMsg_Constructor(&message);
	CCdiFinSetRecoveryMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	pSheet = CShtManager_GetSheet(sheetNo);

	CCdiFinSetRecoveryMsg_SetSetID(&message, CShtSheet_GetSetID(pSheet));

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinSetRecoveryMsg_GetMessage(&message), CCdiFinSetRecoveryMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdReportPauseStts
// Description   : Report Pause Statusメッセージ送信処理
// Parameter     : lineStts (Line Status)
//               : request (Pause Request)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdReportPauseStts(UC lineStts, UC request)
{
	// Not Supported
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdDevContStateUpdate
// Description   : Device Controller State Updateメッセージ送信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdDevContStateUpdate()
{
	CCdiFinDevContSttUpdateMsg message;
	UC msgInstance[CCdiFinDevContSttUpdateMsg_GetSize(&message)];

	CCdiFinDevContSttUpdateMsg_Constructor(&message);
	CCdiFinDevContSttUpdateMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	CCdiFinDevContSttUpdateMsg_SetProductionState(&message, CCdiProductionState_GetState(&(gCdi_AdapterMember.mProductState)));
	CCdiFinDevContSttUpdateMsg_SetTransitionState(&message, CCdiTransitionState_GetState(&(gCdi_AdapterMember.mTransState)));

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDevContSttUpdateMsg_GetMessage(&message), CCdiFinDevContSttUpdateMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdDevContModeUpdate
// Description   : Device Controller Mode Updateメッセージ送信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdDevContModeUpdate()
{
	CCdiFinDevContModeUpdateMsg message;
	UC msgInstance[CCdiFinDevContModeUpdateMsg_GetSize(&message)];

	CCdiFinDevContModeUpdateMsg_Constructor(&message);
	CCdiFinDevContModeUpdateMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	CCdiFinDevContModeUpdateMsg_SetSysOpeMode(&message, CCdiSysOpeMode_GetMode(&(gCdi_AdapterMember.mSysOpeMode)));

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDevContModeUpdateMsg_GetMessage(&message), CCdiFinDevContModeUpdateMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdDevSttsUpdate
// Description   : Device Status Updateメッセージ送信処理
// Parameter     : statusNo (Device Status No.)
//               : statusValue (Status Value)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdDevSttsUpdate(US statusNo, US statusValue)
{
	CCdiFinDevSttsUpdateMsg message;
	CDvstDeviceStatus* pDevStts;
	US statusVal;
	UC msgInstance[CCdiFinDevSttsUpdateMsg_GetSize(&message)];

	if ( (gCdi_AdapterMember.mEventBit & CDI_ADAPT_EVT_CNV_BIT(CDI_ADAPT_EVT_DEV_STTS_REQ)) != 0 ) {
		CCdiFinDevSttsUpdateMsg_Constructor(&message);
		CCdiFinDevSttsUpdateMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

		if ( statusNo < DSNO_NUM ){
			pDevStts = CDvstInformer_GetDeviceStatus(statusNo);

			if ( pDevStts != NULL ){
				statusVal = CDvstDeviceStatus_GetStatusValue(pDevStts);

				CCdiFinDevSttsUpdateMsg_SetStatusID(&message, CDvstDeviceStatus_GetStatusID(pDevStts));
				CCdiFinDevSttsUpdateMsg_SetStatusValue(&message, statusVal);
				CCdiFinDevSttsUpdateMsg_SetChain(&message, CDvstDeviceStatus_GetChainNo(pDevStts, statusVal));
				CCdiFinDevSttsUpdateMsg_SetLink(&message, CDvstDeviceStatus_GetLinkNo(pDevStts, statusVal));

				MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDevSttsUpdateMsg_GetMessage(&message), CCdiFinDevSttsUpdateMsg_GetSize(&message), LOG_ON);
			}
			else{ /* No Action */ }
		}
		else{
			CCdiFinDevSttsUpdateMsg_SetStatusID(&message, eCdiFinSttsID_Comp);
			CCdiFinDevSttsUpdateMsg_SetStatusValue(&message, 0);
			CCdiFinDevSttsUpdateMsg_SetChain(&message, 0);
			CCdiFinDevSttsUpdateMsg_SetLink(&message, 0);

			MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDevSttsUpdateMsg_GetMessage(&message), CCdiFinDevSttsUpdateMsg_GetSize(&message), LOG_ON);
		}
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdJamZoneUpdate
// Description   : Jam Zone Status Updateメッセージ送信処理
// Parameter     : devContID (Device Controller ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdJamZoneUpdate(UC devContID)
{
	CCdiFinJamZoneUpdateMsg message;
	UC *pJamZoneStts;
	UC msgInstance[CDIFIN_MAXMSGSIZE];
	UC index, zoneCount;

	if ( (gCdi_AdapterMember.mEventBit & CDI_ADAPT_EVT_CNV_BIT(CDI_ADAPT_EVT_JAM_ZONE_UPDT)) != 0 ) {
		CCdiFinJamZoneUpdateMsg_Constructor(&message);
		CCdiFinJamZoneUpdateMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

		for ( index = 0; cCdi_JamZoneDataArray[index].devContID != eCdiFinDevCont_Iot; index++ ){
			if ( cCdi_JamZoneDataArray[index].devContID == devContID ){
				CCdiFinJamZoneUpdateMsg_SetFinDevContID(&message, devContID);
				CCdiFinJamZoneUpdateMsg_SetJamZoneStatusSize(&message, cCdi_JamZoneDataArray[index].zoneStatusSize);

				for ( zoneCount = 0; zoneCount < cCdi_JamZoneDataArray[index].zoneIDNumber; zoneCount++ ){
					switch ( cCdi_JamZoneDataArray[index].devContID ){
					case OWN_DEV_CONT_ID:
						CCdiFinJamZoneUpdateMsg_SetJamZoneStatus(&message,
																 CJamChecker_GetJamZoneStatus(cCdi_JamZoneDataArray[index].pSttsMapArray[zoneCount].jamZoneID),
																 cCdi_JamZoneDataArray[index].pSttsMapArray[zoneCount].octet,
																 cCdi_JamZoneDataArray[index].pSttsMapArray[zoneCount].bitPos);

						break;
					// !!! Product Specific !!!
//					case eCdiFinDevCont_Xxx:
//						CCdiFinJamZoneUpdateMsg_SetJamZoneStatus(&message,
//																 XXXX(cCdi_JamZoneDataArray[index].pSttsMapArray[zoneCount].jamZoneID),
//																 cCdi_JamZoneDataArray[index].pSttsMapArray[zoneCount].octet,
//																 cCdi_JamZoneDataArray[index].pSttsMapArray[zoneCount].bitPos);
//
//						break;
					default:
						break;
					}
				}

				MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinJamZoneUpdateMsg_GetMessage(&message), CCdiFinJamZoneUpdateMsg_GetSize(&message), LOG_ON);

				break;
			}
			else{ /* Go for loop */ }
		}
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdAbnormalFinishing
// Description   : Abnormal Finishingメッセージ送信処理
// Parameter     : abortShtNo (Abort Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdAbnormalFinishing(UC abortShtNo)
{
	CCdiFinAbnormalFinishingMsg message;
	const CShtSheet *pSheet;
	UC msgInstance[CCdiFinAbnormalFinishingMsg_GetSize(&message)];

	pSheet = CShtManager_GetSheet(abortShtNo);

	CCdiFinAbnormalFinishingMsg_Constructor(&message);
	CCdiFinAbnormalFinishingMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	CCdiFinAbnormalFinishingMsg_SetPurgePossibility(&message, CShtManager_GetPurgePossibility());

	if ( abortShtNo < SHEETNO_NUM ) CCdiFinAbnormalFinishingMsg_SetAbortSheetID(&message, CShtSheet_GetSheetID(pSheet));
	else							CCdiFinAbnormalFinishingMsg_SetAbortSheetID(&message, 0);

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinAbnormalFinishingMsg_GetMessage(&message), CCdiFinAbnormalFinishingMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdDevContInfoFaultSheet
// Description   : Device Controller Information Notify / Fault
//               : Causal Sheetメッセージ送信処理
// Parameter     : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdDevContInfoFaultSheet(UC sheetNo)
{
	// Not Supported
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdDevContInfoCancelStaple
// Description   : Device Controller Information Notify / Cancel
//               : Stapleメッセージ送信処理
// Parameter     : sheetNo (Sheet No.)
// Return        : ‐
// 20160225 V3.4.0 Merge[ Device Controller Information Notify / Cancel Stapleメッセージ送信対応 ]
///////////////////////////////////////////////////////////////////
void FwdDevContInfoCancelStaple(UC sheetNo)
{
	const CShtSheet *pSheet;
	UC* pInfoData;
	CCdiFinDevContInfoMsg message;
	UC msgInstance[CDIFIN_MAXMSGSIZE];

	pSheet = CShtManager_GetSheet(sheetNo);

	CCdiFinDevContInfoMsg_Constructor(&message);
	CCdiFinDevContInfoMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	pInfoData = CCdiFinDevContInfoMsg_GetInfoData(&message);

	CCdiFinDevContInfoCancelStaple_SetSetID(pInfoData, CShtSheet_GetSetID(pSheet));

	CCdiFinDevContInfoMsg_SetInformationID(&message, CCdiFinDevContInfoCancelStaple_GetInformationID(pInfoData));

	CCdiFinDevContInfoMsg_SetInfoDataSize(&message, CCdiFinDevContInfoCancelStaple_GetInfoDataSize(pInfoData));

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDevContInfoMsg_GetMessage(&message), CCdiFinDevContInfoMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdDevContInfoPbMediaMismatch
// Description   : Device Controller Information Notify / PB Media
//               : Mismatchメッセージ送信処理
// Parameter     : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdDevContInfoPbMediaMismatch(UC sheetNo)
{
	// Not Supported
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdDevContInfoDynamicSample
// Description   : Device Controller Information Notify / Dynamic
//               : Sample Requestメッセージ送信処理
// Parameter     : devContID (Device Controller ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdDevContInfoDynamicSample(UC devContID)
{
	// Not Supported
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdDevContInfoCommInCdiFin
// Description   : Device Controller Information Notify / 
//               : Communicate In CDI-Finisherメッセージ送信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdDevContInfoCommInCdiFin()
{
	CCdiFinDevContInfoMsg message;
	UC msgInstance[CDIFIN_MAXMSGSIZE];

	CCdiFinDevContInfoMsg_Constructor(&message);
	CCdiFinDevContInfoMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	CCdiFinDevContInfoMsg_SetInformationID(&message, CCdiFinDevContInfoCommInCdiFin_GetInformationID(NULL));

	CCdiFinDevContInfoMsg_SetInfoDataSize(&message, CCdiFinDevContInfoCommInCdiFin_GetInfoDataSize(NULL));

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDevContInfoMsg_GetMessage(&message), CCdiFinDevContInfoMsg_GetSize(&message), LOG_ON);
}
///////////////////////////////////////////////////////////////////
// Function Name : FwdFinLogDataMsgCdiFin
// Description   : Device Controller Information Notify / 
//               : Communicate In CDI-Finisherメッセージ送信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdFinLogDataMsgCdiFin(UC SensorID)
{
	CCdiFinLogDataInfoMsg message;
	UC msgInstance[CDIFIN_MAXMSGSIZE];

	CCdiFinLogDataInfoMsg_Constructor(&message);
	CCdiFinLogDataInfoMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	CCdiFinLogDataInfoMsg_SetInformationID(&message, SensorID);//CCdiFinLogDataInfoCommInCdiFin_GetInformationID(NULL));

	CCdiFinLogDataInfoMsg_SetInfoDataSize(&message, CCdiFinLogDataInfoCommInCdiFin_GetInfoDataSize(NULL));

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinLogDataInfoMsg_GetMessage(&message), CCdiFinLogDataInfoMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdDiagResponse
// Description   : Diagnostics Responseメッセージ送信処理
// Parameter     : diagProgram (Diagnostic Program)
//               : diagCommand (Diagnostic Command)
//               : result (Diagnostic Result)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdDiagResponse(US diagProgram, UC diagCommand, UC result)
{
	UC *pDiagData;
	CCdiFinDiagRespMsg message;
	UC msgInstance[CDIFIN_MAXMSGSIZE];

	CCdiFinDiagRespMsg_Constructor(&message);
	CCdiFinDiagRespMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	pDiagData = CCdiFinDiagRespMsg_GetDiagData(&message);

	CCdiFinDiagRespMsg_SetDiagProgram(&message, diagProgram);
	CCdiFinDiagRespMsg_SetDiagCommand(&message, diagCommand);
	CCdiFinDiagRespMsg_SetDiagResult(&message, result);
	CCdiFinDiagRespMsg_SetDiagDataLength(&message, 0);

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDiagRespMsg_GetMessage(&message), CCdiFinDiagRespMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdDiagResponseDc131Read
// Description   : Diagnostics Response(dC131/Read)メッセージ送信処
//               : 理。
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
//               : dataType (Data Type)
//               : readAllowed (Read Allowed)
//               : writeAllowed (Write Allowed)
//               : factoryInitAllowed (Factory Init Allowed)
//               : crntVal (Current Value)
//               : minVal (Minimum Value)
//               : maxVal (Maximum Value)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdDiagResponseDc131Read(UC result, US chain, US link, UC dataType, UC readAllowed, UC writeAllowed, UC factoryInitAllowed, UL crntVal, UL minVal, UL maxVal)
{
	UC *pDiagData;
	CCdiFinDiagRespMsg message;
	UC msgInstance[CDIFIN_MAXMSGSIZE];

	CCdiFinDiagRespMsg_Constructor(&message);
	CCdiFinDiagRespMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	CCdiFinDiagRespMsg_SetDiagProgram(&message, CCdiFinDiagDataRespDc131Read_GetDiagProgram());
	CCdiFinDiagRespMsg_SetDiagCommand(&message, CCdiFinDiagDataRespDc131Read_GetDiagCommand());
	CCdiFinDiagRespMsg_SetDiagResult(&message, result);

	CCdiFinDiagRespMsg_ClearDiagData(&message);

	pDiagData = CCdiFinDiagRespMsg_GetDiagData(&message);

	CCdiFinDiagDataRespDc131Read_SetChain(pDiagData, chain);
	CCdiFinDiagDataRespDc131Read_SetLink(pDiagData, link);
	CCdiFinDiagDataRespDc131Read_SetDataType(pDiagData, dataType);
	CCdiFinDiagDataRespDc131Read_SetReadAllowed(pDiagData, readAllowed);
	CCdiFinDiagDataRespDc131Read_SetWriteAllowed(pDiagData, writeAllowed);
	CCdiFinDiagDataRespDc131Read_SetFactoryInitAllowed(pDiagData, factoryInitAllowed);
	CCdiFinDiagDataRespDc131Read_SetCurrentValue(pDiagData, crntVal);
	CCdiFinDiagDataRespDc131Read_SetMinimumValue(pDiagData, minVal);
	CCdiFinDiagDataRespDc131Read_SetMaximumValue(pDiagData, maxVal);

	CCdiFinDiagRespMsg_SetDiagDataLength(&message, CCdiFinDiagDataRespDc131Read_GetDiagDataLength(pDiagData));

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDiagRespMsg_GetMessage(&message), CCdiFinDiagRespMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdDiagResponseDc131Write
// Description   : Diagnostics Response(dC131/Write)メッセージ送信
//               : 処理。
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
//               : updateResponse (Update Response)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdDiagResponseDc131Write(UC result, US chain, US link, UC updateResponse)
{
	UC *pDiagData;
	CCdiFinDiagRespMsg message;
	UC msgInstance[CDIFIN_MAXMSGSIZE];

	CCdiFinDiagRespMsg_Constructor(&message);
	CCdiFinDiagRespMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	CCdiFinDiagRespMsg_SetDiagProgram(&message, CCdiFinDiagDataRespDc131Write_GetDiagProgram());
	CCdiFinDiagRespMsg_SetDiagCommand(&message, CCdiFinDiagDataRespDc131Write_GetDiagCommand());
	CCdiFinDiagRespMsg_SetDiagResult(&message, result);

	CCdiFinDiagRespMsg_ClearDiagData(&message);

	pDiagData = CCdiFinDiagRespMsg_GetDiagData(&message);

	CCdiFinDiagDataRespDc131Write_SetChain(pDiagData, chain);
	CCdiFinDiagDataRespDc131Write_SetLink(pDiagData, link);
	CCdiFinDiagDataRespDc131Write_SetUpdateResponse(pDiagData, updateResponse);

	CCdiFinDiagRespMsg_SetDiagDataLength(&message, CCdiFinDiagDataRespDc131Write_GetDiagDataLength(pDiagData));

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDiagRespMsg_GetMessage(&message), CCdiFinDiagRespMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdDiagResponseDc301Start
// Description   : Diagnostics Response(dC301/Start)メッセージ送信
//               : 処理。
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
//               : devContID (Device Controller ID)
//               : attrInitResp (Attribute Init Response)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdDiagResponseDc301Start(UC result, US chain, US link, UC devContID, UC attrInitResp)
{
	UC *pDiagData;
	CCdiFinDiagRespMsg message;
	UC msgInstance[CDIFIN_MAXMSGSIZE];

	CCdiFinDiagRespMsg_Constructor(&message);
	CCdiFinDiagRespMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	CCdiFinDiagRespMsg_SetDiagProgram(&message, CCdiFinDiagDataRespDc301Start_GetDiagProgram());
	CCdiFinDiagRespMsg_SetDiagCommand(&message, CCdiFinDiagDataRespDc301Start_GetDiagCommand());
	CCdiFinDiagRespMsg_SetDiagResult(&message, result);

	CCdiFinDiagRespMsg_ClearDiagData(&message);

	pDiagData = CCdiFinDiagRespMsg_GetDiagData(&message);

	CCdiFinDiagDataRespDc301Start_SetChain(pDiagData, chain);
	CCdiFinDiagDataRespDc301Start_SetLink(pDiagData, link);
	CCdiFinDiagDataRespDc301Start_SetAttributeInitResponse(pDiagData, attrInitResp);
	CCdiFinDiagDataRespDc301Start_SetDeviceControllerID(pDiagData, devContID);

	CCdiFinDiagRespMsg_SetDiagDataLength(&message, CCdiFinDiagDataRespDc301Start_GetDiagDataLength(pDiagData));

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDiagRespMsg_GetMessage(&message), CCdiFinDiagRespMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdDiagResponseDc361Start
// Description   : Diagnostics Response(dC361/Start)メッセージ送信
//               : 処理。
// Parameter     : result (Diagnostic Result)
//               : nvmArea (NVM Area)
//               : devContID (Device Controller ID)
//               : nvmAreaSize (NVM Area Size)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdDiagResponseDc361Start(UC result, UC nvmArea, UC devContID, US nvmAreaSize)
{
	UC *pDiagData;
	CCdiFinDiagRespMsg message;
	UC msgInstance[CDIFIN_MAXMSGSIZE];

	CCdiFinDiagRespMsg_Constructor(&message);
	CCdiFinDiagRespMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	CCdiFinDiagRespMsg_SetDiagProgram(&message, CCdiFinDiagDataRespDc361Start_GetDiagProgram());
	CCdiFinDiagRespMsg_SetDiagCommand(&message, CCdiFinDiagDataRespDc361Start_GetDiagCommand());
	CCdiFinDiagRespMsg_SetDiagResult(&message, result);

	CCdiFinDiagRespMsg_ClearDiagData(&message);

	pDiagData = CCdiFinDiagRespMsg_GetDiagData(&message);

	CCdiFinDiagDataRespDc361Start_SetNvmArea(pDiagData, nvmArea);
	CCdiFinDiagDataRespDc361Start_SetDeviceControllerID(pDiagData, devContID);
	CCdiFinDiagDataRespDc361Start_SetNvmAreaSize(pDiagData, nvmAreaSize);

	CCdiFinDiagRespMsg_SetDiagDataLength(&message, CCdiFinDiagDataRespDc361Start_GetDiagDataLength(pDiagData));

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDiagRespMsg_GetMessage(&message), CCdiFinDiagRespMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdDiagResponseDc361Read
// Description   : Diagnostics Response(dC361/Read)メッセージ送信処
//               : 理。
// Parameter     : result (Diagnostic Result)
//               : nvmArea (NVM Area)
//               : devContID (Device Controller ID)
//               : presenceRemain (Presence Of Remain Data)
//               : allocateSize (Allocated Data Size)
//               : pAllocateValue (Data pointer of Allocated Data 
//               : Value)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdDiagResponseDc361Read(UC result, UC nvmArea, UC devContID, UC presenceRemain, UC allocateSize, UC* pAllocateValue)
{
	UC *pDiagData, *pDataValue;
	CCdiFinDiagRespMsg message;
	UC msgInstance[CDIFIN_MAXMSGSIZE], index;

	CCdiFinDiagRespMsg_Constructor(&message);
	CCdiFinDiagRespMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	CCdiFinDiagRespMsg_SetDiagProgram(&message, CCdiFinDiagDataRespDc361Read_GetDiagProgram());
	CCdiFinDiagRespMsg_SetDiagCommand(&message, CCdiFinDiagDataRespDc361Read_GetDiagCommand());
	CCdiFinDiagRespMsg_SetDiagResult(&message, result);

	CCdiFinDiagRespMsg_ClearDiagData(&message);

	pDiagData = CCdiFinDiagRespMsg_GetDiagData(&message);

	CCdiFinDiagDataRespDc361Read_SetNvmArea(pDiagData, nvmArea);
	CCdiFinDiagDataRespDc361Read_SetDeviceControllerID(pDiagData, devContID);
	CCdiFinDiagDataRespDc361Read_SetPresenceOfRemainData(pDiagData, presenceRemain);
	CCdiFinDiagDataRespDc361Read_SetAllocatedDataSize(pDiagData, allocateSize);

	pDataValue = CCdiFinDiagDataRespDc361Read_GetAllocatedDataValue(pDiagData);

	for ( index = 0; index < allocateSize; index++ ){
		pDataValue[index] = pAllocateValue[index];
	}

	CCdiFinDiagRespMsg_SetDiagDataLength(&message, CCdiFinDiagDataRespDc361Read_GetDiagDataLength(pDiagData));

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDiagRespMsg_GetMessage(&message), CCdiFinDiagRespMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdDiagResponseDc361Write
// Description   : Diagnostics Response(dC361/Write)メッセージ送信
//               : 処理。
// Parameter     : result (Diagnostic Result)
//               : nvmArea (NVM Area)
//               : devContID (Device Controller ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdDiagResponseDc361Write(UC result, UC nvmArea, UC devContID)
{
	UC *pDiagData;
	CCdiFinDiagRespMsg message;
	UC msgInstance[CDIFIN_MAXMSGSIZE];

	CCdiFinDiagRespMsg_Constructor(&message);
	CCdiFinDiagRespMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	CCdiFinDiagRespMsg_SetDiagProgram(&message, CCdiFinDiagDataRespDc361Write_GetDiagProgram());
	CCdiFinDiagRespMsg_SetDiagCommand(&message, CCdiFinDiagDataRespDc361Write_GetDiagCommand());
	CCdiFinDiagRespMsg_SetDiagResult(&message, result);

	CCdiFinDiagRespMsg_ClearDiagData(&message);

	pDiagData = CCdiFinDiagRespMsg_GetDiagData(&message);

	CCdiFinDiagDataRespDc361Write_SetNvmArea(pDiagData, nvmArea);
	CCdiFinDiagDataRespDc361Write_SetDeviceControllerID(pDiagData, devContID);

	CCdiFinDiagRespMsg_SetDiagDataLength(&message, CCdiFinDiagDataRespDc361Write_GetDiagDataLength(pDiagData));

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDiagRespMsg_GetMessage(&message), CCdiFinDiagRespMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdDiagResponseDc135Read
// Description   : Diagnostics Response(dC135/Read)メッセージ送信処
//               : 理。
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
//               : counterValue (Counter Value)
//               : lifeSpec (Life Spec)
//               : reasonLast (Reason For Last Exchange)
//               : counterValLast (Counter Value At Last Exchange)
//               : reasonLastButOne (Reason For Last Exchange But 
//               : One)
//               : counterValLastButOne (Counter Value At Last 
//               : Exchange But One)
//               : reasonLastButTwo (Reason For Last Exchange But 
//               : Two)
//               : counterValLastButTwo (Counter Value At Last 
//               : Exchange But Two)
//               : pCounterName (Data pointer of Counter Name)
//               : counterNameSize (Data size of Counter Name)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdDiagResponseDc135Read(UC result, US chain, US link, UL counterValue, UL lifeSpec, UC reasonLast, UL counterValLast, UC reasonLastButOne, UL counterValLastButOne, UC reasonLastButTwo, UL counterValLastButTwo, UC const* pCounterName, UC counterNameSize)
{
	UC *pDiagData, *pName;
	CCdiFinDiagRespMsg message;
	UC msgInstance[CDIFIN_MAXMSGSIZE], index;

	CCdiFinDiagRespMsg_Constructor(&message);
	CCdiFinDiagRespMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	CCdiFinDiagRespMsg_SetDiagProgram(&message, CCdiFinDiagDataRespDc135Read_GetDiagProgram());
	CCdiFinDiagRespMsg_SetDiagCommand(&message, CCdiFinDiagDataRespDc135Read_GetDiagCommand());
	CCdiFinDiagRespMsg_SetDiagResult(&message, result);

	CCdiFinDiagRespMsg_ClearDiagData(&message);

	pDiagData = CCdiFinDiagRespMsg_GetDiagData(&message);

	CCdiFinDiagDataRespDc135Read_SetChain(pDiagData, chain);
	CCdiFinDiagDataRespDc135Read_SetLink(pDiagData, link);
	CCdiFinDiagDataRespDc135Read_SetCounterValue(pDiagData, counterValue);
	CCdiFinDiagDataRespDc135Read_SetLifeSpec(pDiagData, lifeSpec);
	CCdiFinDiagDataRespDc135Read_SetReasonForLast(pDiagData, reasonLast);
	CCdiFinDiagDataRespDc135Read_SetCounterValueAtLast(pDiagData, counterValLast);
	CCdiFinDiagDataRespDc135Read_SetReasonForLastButOne(pDiagData, reasonLastButOne);
	CCdiFinDiagDataRespDc135Read_SetCounterValueAtLastButOne(pDiagData, counterValLastButOne);
	CCdiFinDiagDataRespDc135Read_SetReasonForLastButTwo(pDiagData, reasonLastButTwo);
	CCdiFinDiagDataRespDc135Read_SetCounterValueAtLastButTwo(pDiagData, counterValLastButTwo);

	pName = CCdiFinDiagDataRespDc135Read_GetCounterName(pDiagData);

	for ( index = 0; index < counterNameSize; index++ ){
		pName[index] = pCounterName[index];
	}

	for ( ; index < 20; index++ ){
		pName[index] = 0x00;
	}

	CCdiFinDiagRespMsg_SetDiagDataLength(&message, CCdiFinDiagDataRespDc135Read_GetDiagDataLength(pDiagData));

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDiagRespMsg_GetMessage(&message), CCdiFinDiagRespMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdDiagResponseDc135Reset
// Description   : Diagnostics Response(dC135/Reset)メッセージ送信
//               : 処理。
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
//               : devContID (Device Controller ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdDiagResponseDc135Reset(UC result, US chain, US link, UC devContID)
{
	UC *pDiagData;
	CCdiFinDiagRespMsg message;
	UC msgInstance[CDIFIN_MAXMSGSIZE];

	CCdiFinDiagRespMsg_Constructor(&message);
	CCdiFinDiagRespMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	CCdiFinDiagRespMsg_SetDiagProgram(&message, CCdiFinDiagDataRespDc135Reset_GetDiagProgram());
	CCdiFinDiagRespMsg_SetDiagCommand(&message, CCdiFinDiagDataRespDc135Reset_GetDiagCommand());
	CCdiFinDiagRespMsg_SetDiagResult(&message, result);

	CCdiFinDiagRespMsg_ClearDiagData(&message);

	pDiagData = CCdiFinDiagRespMsg_GetDiagData(&message);

	CCdiFinDiagDataRespDc135Reset_SetChain(pDiagData, chain);
	CCdiFinDiagDataRespDc135Reset_SetLink(pDiagData, link);
	CCdiFinDiagDataRespDc135Reset_SetDeviceControllerID(pDiagData, devContID);

	CCdiFinDiagRespMsg_SetDiagDataLength(&message, CCdiFinDiagDataRespDc135Reset_GetDiagDataLength(pDiagData));

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDiagRespMsg_GetMessage(&message), CCdiFinDiagRespMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdDiagResponseDc135Write
// Description   : Diagnostics Response(dC135/Write)メッセージ送信
//               : 処理。
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdDiagResponseDc135Write(UC result, US chain, US link)
{
	UC *pDiagData;
	CCdiFinDiagRespMsg message;
	UC msgInstance[CDIFIN_MAXMSGSIZE];

	CCdiFinDiagRespMsg_Constructor(&message);
	CCdiFinDiagRespMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	CCdiFinDiagRespMsg_SetDiagProgram(&message, CCdiFinDiagDataRespDc135Write_GetDiagProgram());
	CCdiFinDiagRespMsg_SetDiagCommand(&message, CCdiFinDiagDataRespDc135Write_GetDiagCommand());
	CCdiFinDiagRespMsg_SetDiagResult(&message, result);

	CCdiFinDiagRespMsg_ClearDiagData(&message);

	pDiagData = CCdiFinDiagRespMsg_GetDiagData(&message);

	CCdiFinDiagDataRespDc135Write_SetChain(pDiagData, chain);
	CCdiFinDiagDataRespDc135Write_SetLink(pDiagData, link);

	CCdiFinDiagRespMsg_SetDiagDataLength(&message, CCdiFinDiagDataRespDc135Write_GetDiagDataLength(pDiagData));

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDiagRespMsg_GetMessage(&message), CCdiFinDiagRespMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdDiagResponseDc330Monitor
// Description   : Diagnostics Response(dC330/Monitor)メッセージ送
//               : 信処理。
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
//               : componentState (Component State)
//               : transitionCounter (Transition Counter)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdDiagResponseDc330Monitor(UC result, US chain, US link, UC componentState, UC transitionCounter)
{
	UC *pDiagData;
	CCdiFinDiagRespMsg message;
	UC msgInstance[CDIFIN_MAXMSGSIZE];

	CCdiFinDiagRespMsg_Constructor(&message);
	CCdiFinDiagRespMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	CCdiFinDiagRespMsg_SetDiagProgram(&message, CCdiFinDiagDataRespDc330Monitor_GetDiagProgram());
	CCdiFinDiagRespMsg_SetDiagCommand(&message, CCdiFinDiagDataRespDc330Monitor_GetDiagCommand());
	CCdiFinDiagRespMsg_SetDiagResult(&message, result);

	CCdiFinDiagRespMsg_ClearDiagData(&message);

	pDiagData = CCdiFinDiagRespMsg_GetDiagData(&message);

	CCdiFinDiagDataRespDc330Monitor_SetChain(pDiagData, chain);
	CCdiFinDiagDataRespDc330Monitor_SetLink(pDiagData, link);
	CCdiFinDiagDataRespDc330Monitor_SetComponentState(pDiagData, componentState);
	CCdiFinDiagDataRespDc330Monitor_SetTransitionCounter(pDiagData, transitionCounter);

	CCdiFinDiagRespMsg_SetDiagDataLength(&message, CCdiFinDiagDataRespDc330Monitor_GetDiagDataLength(pDiagData));

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDiagRespMsg_GetMessage(&message), CCdiFinDiagRespMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : FwdDiagResponseDc330Write
// Description   : Diagnostics Response(dC330/Write)メッセージ送信
//               : 処理。
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
//               : componentResponse (Component State)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void FwdDiagResponseDc330Write(UC result, US chain, US link, UC componentResponse)
{
	UC *pDiagData;
	CCdiFinDiagRespMsg message;
	UC msgInstance[CDIFIN_MAXMSGSIZE];

	CCdiFinDiagRespMsg_Constructor(&message);
	CCdiFinDiagRespMsg_SetMessage(&message, msgInstance, sizeof(msgInstance));

	CCdiFinDiagRespMsg_SetDiagProgram(&message, CCdiFinDiagDataRespDc330Write_GetDiagProgram());
	CCdiFinDiagRespMsg_SetDiagCommand(&message, CCdiFinDiagDataRespDc330Write_GetDiagCommand());
	CCdiFinDiagRespMsg_SetDiagResult(&message, result);

	CCdiFinDiagRespMsg_ClearDiagData(&message);

	pDiagData = CCdiFinDiagRespMsg_GetDiagData(&message);

	CCdiFinDiagDataRespDc330Write_SetChain(pDiagData, chain);
	CCdiFinDiagDataRespDc330Write_SetLink(pDiagData, link);
	CCdiFinDiagDataRespDc330Write_SetComponentResponse(pDiagData, componentResponse);

	CCdiFinDiagRespMsg_SetDiagDataLength(&message, CCdiFinDiagDataRespDc330Write_GetDiagDataLength(pDiagData));

	MSP_SendCommand(SID_IOT, MID_SYS, CCdiFinDiagRespMsg_GetMessage(&message), CCdiFinDiagRespMsg_GetSize(&message), LOG_ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : RcvSysContInfoCommInCdiFin
// Description   : System Controller Information Notify(Communicate
//               :  In CDI-Finisher)メッセージ受信時の処理を実施す
//               : る。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RcvSysContInfoCommInCdiFin(CCdiFinSysContInfoMsg* pMessage)
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : RcvExecuteDiagDc131Read
// Description   : Execute Diagnostics(dC131/Read)メッセージ受信時
// 				 : の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RcvExecuteDiagDc131Read(const CCdiFinExecDiagMsg* pMessage)
{
	UC *pDiagData;

	pDiagData = CCdiFinExecDiagMsg_GetDiagData(pMessage);

	CDiagManager_ExecuteDiagDc131Read(	CCdiFinDiagDataExecDc131Read_GetChain(pDiagData),
										CCdiFinDiagDataExecDc131Read_GetLink(pDiagData)	);
}

///////////////////////////////////////////////////////////////////
// Function Name : RcvExecuteDiagDc131Write
// Description   : Execute Diagnostics(dC131/Write)メッセージ受信時
// 				 : の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RcvExecuteDiagDc131Write(const CCdiFinExecDiagMsg* pMessage)
{
	UC *pDiagData;

	pDiagData = CCdiFinExecDiagMsg_GetDiagData(pMessage);

	CDiagManager_ExecuteDiagDc131Write(	CCdiFinDiagDataExecDc131Write_GetChain(pDiagData),
										CCdiFinDiagDataExecDc131Write_GetLink(pDiagData),
										(enum ECdiFinDataType)(CCdiFinDiagDataExecDc131Write_GetDataType(pDiagData)),
										CCdiFinDiagDataExecDc131Write_GetNvmValue(pDiagData)	);
}

///////////////////////////////////////////////////////////////////
// Function Name : RcvExecuteDiagDc301Start
// Description   : Execute Diagnostics(dC301/Start)メッセージ受信時
// 				 : の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RcvExecuteDiagDc301Start(const CCdiFinExecDiagMsg* pMessage)
{
	UC *pDiagData;

	pDiagData = CCdiFinExecDiagMsg_GetDiagData(pMessage);

	CDiagManager_ExecuteDiagDc301Start(	CCdiFinDiagDataExecDc301Start_GetChain(pDiagData),
										CCdiFinDiagDataExecDc301Start_GetLink(pDiagData),
										(enum ECdiFinDevCont)CCdiFinDiagDataExecDc301Start_GetDeviceControllerID(pDiagData)	);
}

///////////////////////////////////////////////////////////////////
// Function Name : RcvExecuteDiagDc361Start
// Description   : Execute Diagnostics(dC361/Start)メッセージ受信時
// 				 : の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RcvExecuteDiagDc361Start(const CCdiFinExecDiagMsg* pMessage)
{
	UC *pDiagData;

	pDiagData = CCdiFinExecDiagMsg_GetDiagData(pMessage);

	CDiagManager_ExecuteDiagDc361Start(	(enum ECdiFinNvmArea)CCdiFinDiagDataExecDc361Start_GetNvmArea(pDiagData),
										(enum ECdiFinDevCont)CCdiFinDiagDataExecDc361Start_GetDeviceControllerID(pDiagData)	);
}

///////////////////////////////////////////////////////////////////
// Function Name : RcvExecuteDiagDc361Read
// Description   : Execute Diagnostics(dC361/Read)メッセージ受信時
// 				 : の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RcvExecuteDiagDc361Read(const CCdiFinExecDiagMsg* pMessage)
{
	UC *pDiagData;

	pDiagData = CCdiFinExecDiagMsg_GetDiagData(pMessage);

	CDiagManager_ExecuteDiagDc361Read(	(enum ECdiFinNvmArea)CCdiFinDiagDataExecDc361Read_GetNvmArea(pDiagData),
										(enum ECdiFinDevCont)CCdiFinDiagDataExecDc361Read_GetDeviceControllerID(pDiagData)	);
}

///////////////////////////////////////////////////////////////////
// Function Name : RcvExecuteDiagDc361Write
// Description   : Execute Diagnostics(dC361/Write)メッセージ受信時
// 				 : の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RcvExecuteDiagDc361Write(const CCdiFinExecDiagMsg* pMessage)
{
	UC *pDiagData;

	pDiagData = CCdiFinExecDiagMsg_GetDiagData(pMessage);

	CDiagManager_ExecuteDiagDc361Write(	(enum ECdiFinNvmArea)CCdiFinDiagDataExecDc361Write_GetNvmArea(pDiagData),
										(enum ECdiFinDevCont)CCdiFinDiagDataExecDc361Write_GetDeviceControllerID(pDiagData),
										CCdiFinDiagDataExecDc361Write_GetPresenceOfRemainData(pDiagData),
										CCdiFinDiagDataExecDc361Write_GetAllocatedDataSize(pDiagData),
										CCdiFinDiagDataExecDc361Write_GetAllocatedDataValue(pDiagData)	);
}

///////////////////////////////////////////////////////////////////
// Function Name : RcvExecuteDiagDc135Read
// Description   : Execute Diagnostics(dC135/Read)メッセージ受信時
// 				 : の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RcvExecuteDiagDc135Read(const CCdiFinExecDiagMsg* pMessage)
{
	UC *pDiagData;

	pDiagData = CCdiFinExecDiagMsg_GetDiagData(pMessage);

	CDiagManager_ExecuteDiagDc135Read(	CCdiFinDiagDataExecDc135Read_GetChain(pDiagData),
										CCdiFinDiagDataExecDc135Read_GetLink(pDiagData)	);
}

///////////////////////////////////////////////////////////////////
// Function Name : RcvExecuteDiagDc135Reset
// Description   : Execute Diagnostics(dC135/Reset)メッセージ受信時
// 				 : の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RcvExecuteDiagDc135Reset(const CCdiFinExecDiagMsg* pMessage)
{
	UC *pDiagData;

	pDiagData = CCdiFinExecDiagMsg_GetDiagData(pMessage);

	CDiagManager_ExecuteDiagDc135Reset(	CCdiFinDiagDataExecDc135Reset_GetChain(pDiagData),
										CCdiFinDiagDataExecDc135Reset_GetLink(pDiagData),
										CCdiFinDiagDataExecDc135Reset_GetReasonForExchange(pDiagData),
										(enum ECdiFinDevCont)CCdiFinDiagDataExecDc135Reset_GetDeviceControllerID(pDiagData)	);
}

///////////////////////////////////////////////////////////////////
// Function Name : RcvExecuteDiagDc135Write
// Description   : Execute Diagnostics(dC135/Write)メッセージ受信時
// 				 : の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RcvExecuteDiagDc135Write(const CCdiFinExecDiagMsg* pMessage)
{
	UC *pDiagData;

	pDiagData = CCdiFinExecDiagMsg_GetDiagData(pMessage);

	CDiagManager_ExecuteDiagDc135Write(	CCdiFinDiagDataExecDc135Write_GetChain(pDiagData),
										CCdiFinDiagDataExecDc135Write_GetLink(pDiagData),
										CCdiFinDiagDataExecDc135Write_GetLifeSpec(pDiagData)	);
}

///////////////////////////////////////////////////////////////////
// Function Name : RcvExecuteDiagDc140Monitor
// Description   : Execute Diagnostics(dC140/Monitor)メッセージ受信
// 				 : 時の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RcvExecuteDiagDc140Monitor(const CCdiFinExecDiagMsg* pMessage)
{
	// Not Supported
}

///////////////////////////////////////////////////////////////////
// Function Name : RcvExecuteDiagDc330Monitor
// Description   : Execute Diagnostics(dC330/Monitor)メッセージ受信
// 				 : 時の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RcvExecuteDiagDc330Monitor(const CCdiFinExecDiagMsg* pMessage)
{
	UC *pDiagData;

	pDiagData = CCdiFinExecDiagMsg_GetDiagData(pMessage);

	CDiagManager_ExecuteDiagDc330Monitor(	CCdiFinDiagDataExecDc330Monitor_GetChain(pDiagData),
											CCdiFinDiagDataExecDc330Monitor_GetLink(pDiagData),
											(enum ECdiFinMonOpe)CCdiFinDiagDataExecDc330Monitor_GetMonitorOperation(pDiagData)	);
}

///////////////////////////////////////////////////////////////////
// Function Name : RcvExecuteDiagDc330Write
// Description   : Execute Diagnostics(dC330/Write)メッセージ受信時
// 				 : の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RcvExecuteDiagDc330Write(const CCdiFinExecDiagMsg* pMessage)
{
	UC *pDiagData;

	pDiagData = CCdiFinExecDiagMsg_GetDiagData(pMessage);

	CDiagManager_ExecuteDiagDc330Write(	CCdiFinDiagDataExecDc330Write_GetChain(pDiagData),
										CCdiFinDiagDataExecDc330Write_GetLink(pDiagData),
										CCdiFinDiagDataExecDc330Write_GetComponentState(pDiagData)	);
}

///////////////////////////////////////////////////////////////////
// Function Name : RcvExecuteDiagDc726Read
// Description   : Execute Diagnostics(dC726/Read)メッセージ受信時
// 				 : の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RcvExecuteDiagDc726Read(const CCdiFinExecDiagMsg* pMessage)
{
	// Not Supported
}

///////////////////////////////////////////////////////////////////
// Function Name : RcvExecuteDiagDc726Monitor
// Description   : Execute Diagnostics(dC726/Monitor)メッセージ受信
// 				 : 時の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RcvExecuteDiagDc726Monitor(const CCdiFinExecDiagMsg* pMessage)
{
	// Not Supported
}

///////////////////////////////////////////////////////////////////
// Function Name : RcvExecuteDiagDc1100Read
// Description   : Execute Diagnostics(dC1100/Read)メッセージ受信時
// 				 : の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RcvExecuteDiagDc1100Read(const CCdiFinExecDiagMsg* pMessage)
{
	// Not Supported
}

///////////////////////////////////////////////////////////////////
// Function Name : RcvExecuteDiagDc1201Read
// Description   : Execute Diagnostics(dC1201/Read)メッセージ受信時
// 				 : の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RcvExecuteDiagDc1201Read(const CCdiFinExecDiagMsg* pMessage)
{
	// Not Supported
}

///////////////////////////////////////////////////////////////////
// Function Name : RcvExecuteDiagDc1201Write
// Description   : Execute Diagnostics(dC1201/Write)メッセージ受信
// 				 : 時の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RcvExecuteDiagDc1201Write(const CCdiFinExecDiagMsg* pMessage)
{
	// Not Supported
}

///////////////////////////////////////////////////////////////////
// Function Name : RcvExecuteDiagDc1202Read
// Description   : Execute Diagnostics(dC1202/Read)メッセージ受信時
// 				 : の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RcvExecuteDiagDc1202Read(const CCdiFinExecDiagMsg* pMessage)
{
	// Not Supported
}

///////////////////////////////////////////////////////////////////
// Function Name : RcvExecuteDiagDc1202Write
// Description   : Execute Diagnostics(dC1202/Write)メッセージ受信
// 				 : 時の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RcvExecuteDiagDc1202Write(const CCdiFinExecDiagMsg* pMessage)
{
	// Not Supported
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_Sync
// Description   : CDI通信確立依頼受信時の処理を実施する。
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_Sync()
{
	MSP_ActivateCommunication(SID_IOT, (UC)NULL);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_CompleteSync
// Description   : CDI通信確立完了通知受信時の処理を実施する。
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_CompleteSync()
{
	FwdInitRequest();
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_FailedCommunication
// Description   : CDI通信失敗通知受信時の処理を実施する。
// Parameter     : ‐
// Return        : ‐
// 20160225 V3.4.0 Merge Trouble Action [ 通信フェイル発生時にフィニッシングプロセスからスタンバイに遷移しない ]
///////////////////////////////////////////////////////////////////
void CCdiAdapter_FailedCommunication()
{
	// --- 通信フェイルステータス変化時処理 ---
	CSysManager_FaultStatusChange(DSNO_SYS_CONT_COMM_FAIL, FAULT_OCCUR);

	// --- フィニッシングプロセス停止のための擬似Stop Finisher処理 ---
	CShtManager_SheetEnd();
	CSysManager_HardDown();

	// --- コンポーネントコントロール実施中のフェイル検知の為にSCからの停止指示が期待できない際の停止処理 ---
	CDiagManager_ExecuteDiagDc330Write(0, 0, eCdiFinComponentStt_AllOff);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendFinisherState
// Description   : フィニッシャー状態通知依頼受信時の処理を実施する
//               : 。
// Parameter     : finState (フィニッシャー状態)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendFinisherState(EFinisherState finState)
{
	UC sysOpeModeEvt, productSttEvt, transSttEvt, sysOpeModeChg, productSttChg, transSttChg;

	sysOpeModeEvt = CDI_SYS_OPE_MODE_EVT_NUM;
	productSttEvt = CDI_PRDCT_STT_EVT_NUM;
	transSttEvt	  = CDI_TRANS_STT_EVT_NUM;

	switch ( finState ){
	case FIN_STT_POWER_OFF:
		FwdPwrOffAcknowledge();
		break;
	case FIN_STT_DIAGNOSTIC:
		sysOpeModeEvt = CDI_SYS_OPE_MODE_EVT_CHG_DIAG;
		break;
	case FIN_STT_STANDBY:
		sysOpeModeEvt = CDI_SYS_OPE_MODE_EVT_CHG_NRML;
		productSttEvt = CDI_PRDCT_STT_EVT_DOWN_COMP;
		transSttEvt	  = CDI_TRANS_STT_EVT_DOWN_COMP;
		break;
	case FIN_STT_CYCLE_UP:
		productSttEvt = CDI_PRDCT_STT_EVT_CYC_UP_STA;
		transSttEvt	  = CDI_TRANS_STT_EVT_CYC_UP_STA;
		break;
	case FIN_STT_READY:
		transSttEvt = CDI_TRANS_STT_EVT_CYC_UP_COMP;
		break;
	case FIN_STT_CYCLE_DOWN:
		transSttEvt = CDI_TRANS_STT_EVT_CYC_DOWN_STA;
		break;
	case FIN_STT_HARD_DOWN:
		transSttEvt = CDI_TRANS_STT_EVT_HARD_DOWN_STA;
		break;
	default:		// FIN_STT_UNKNOWN/FIN_STT_POWER_UP/FIN_STT_POWER_ON/FIN_STT_POWER_DOWN/FIN_STT_POWER_OFF/FIN_STT_SETUP/FIN_STT_CHANGE_MODE
		// No Action
		break;
	}

	sysOpeModeChg = CCdiSysOpeMode_InformedEvent(&(gCdi_AdapterMember.mSysOpeMode), (ECdiSysOpeModeEvt)sysOpeModeEvt);
	if ( sysOpeModeChg == true ){
		FwdDevContModeUpdate();
	}
	else{ /* No Action */ }

	productSttChg = CCdiProductionState_InformedEvent(&(gCdi_AdapterMember.mProductState), (ECdiProductSttEvt)productSttEvt);
	transSttChg = CCdiTransitionState_InformedEvent(&(gCdi_AdapterMember.mTransState), transSttEvt);
	if ( (productSttChg == true) || (transSttChg == true) ){
		FwdDevContStateUpdate();
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendInitialInformation
// Description   : 情報初期値決定通知依頼受信時の処理を実施する。
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendInitialInformation()
{
	gCdi_AdapterMember.mEventBit |= CDI_ADAPT_EVT_CNV_BIT(CDI_ADAPT_EVT_SEND_INIT_INFO);

	FwdDevCapabilitiesUpdate(eCdiFinSubsys_UpdateAll, 0x00, 0);
	FwdDevConfigurationUpdate(eCdiFinSubsys_UpdateAll, 0x00, 0);				// Power Saver Update対応のためコール
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendDeviceCapabilities
// Description   : Device Capabilities通知依頼受信時の処理を実施す
//               : る。
// Parameter     : subsystem (Subsystem)
//               : instance (Instance)
//               : attrKey (Attr Key)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendDeviceCapabilities(enum ECdiFinSubsys subsystem, UC instance, UC attrKey)
{
	FwdDevCapabilitiesUpdate(subsystem, instance, attrKey);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendDeviceConfiguration
// Description   : Device Configuration通知依頼受信時の処理を実施す
//               : る。
// Parameter     : subsystem (Subsystem)
//               : instance (Instance)
//               : attrKey (Attr Key)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendDeviceConfiguration(enum ECdiFinSubsys subsystem, UC instance, UC attrKey)
{
	FwdDevConfigurationUpdate(subsystem, instance, attrKey);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendDeviceStatus
// Description   : Device Status通知依頼受信時の処理を実施する。
// Parameter     : statusNo (Device Status No.)
//               : statusValue (Status Value)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendDeviceStatus(US statusNo, US statusValue)
{
	FwdDevSttsUpdate(statusNo, statusValue);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendJamZoneStatus
// Description   : Jam Zone Status通知依頼受信時の処理を実施する。
// Parameter     : devContID (Device Controller ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendJamZoneStatus(UC devContID)
{
	FwdJamZoneUpdate(devContID);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendAbnormalFinishing
// Description   : Abnormal Finishing情報の通知依頼受信時の処理を実
//               : 施する。
// Parameter     : abortShtNo (Sheet No. of aborted sheet)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendAbnormalFinishing(UC abortShtNo)
{
	FwdAbnormalFinishing(abortShtNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendDeviceActionResponse
// Description   : Device Action Responseイベント発行通知依頼受信時
//               : の処理を実施する。
// Parameter     : eventID (MSP Event ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendDeviceActionResponse(US eventID)
{
	switch ( eventID ){
	// !!! Product Specific !!!
//	case EVT_DEV_RES_XXXX
//	
//		break;
	default:
		// No Action
		break;
	};
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendDeviceEventNotify
// Description   : Device Event Notifyイベント発行通知依頼受信時の
//               : 処理を実施する。
// Parameter     : eventID (MSP Event ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendDeviceEventNotify(US eventID)
{
	switch ( eventID ){
	// !!! Product Specific !!!
//	case EVT_DEV_EVT_XXXX
//	
//		break;
	default:
		// No Action
		break;
	};
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendSheetActionResponse
// Description   : Sheet Action Responseイベント発行通知依頼受信時
//               : の処理を実施する。
// Parameter     : eventID (MSP Event ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendSheetActionResponse(UC sheetNo, US eventID)
{
	if ( sheetNo < SHEETNO_NUM ){
		switch ( eventID ){
		// !!! Product Specific !!!
//		case EVT_SHT_RES_XXXX
//		
//			break;
		default:
			// No Action
			break;
		};
	}	
	else{ /* NG */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendSheetEventNotify
// Description   : Sheet Event Notifyイベント発行受信時の処理を実施
//               : する。
// Parameter     : sheetNo (Sheet No.)
//               : eventID (MSP Event ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendSheetEventNotify(UC sheetNo, US eventID)
{
	if ( sheetNo < SHEETNO_NUM ){
		switch ( eventID ){
		// !!! Product Specific !!!
//		case EVT_SHT_EVT_XXXX:
//
//			break;
		default:
			// No Action
			break;	
		}
	}
	else{ /* NG */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendSheetDelivered
// Description   : シートデリバードの通知依頼受信時の処理を実施する
//               : 。
// Parameter     : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendSheetDelivered(UC sheetNo)
{
	if ( sheetNo < SHEETNO_NUM ){
		FwdSheetDelvToOutput(sheetNo);
	}
	else{ /* NG */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendSetFinished
// Description   : セット処理完了の通知依頼受信時の処理を実施する。
// Parameter     : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendSetFinished(UC sheetNo)
{
	if ( sheetNo < SHEETNO_NUM ){
		FwdSetFinAtOutput(sheetNo);
	}
	else{ /* NG */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendSetRecoveryRequest
// Description   : Set Recovery送信依頼受信時の処理を実施する。
// Parameter     : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendSetRecoveryRequest(UC sheetNo)
{
	if ( sheetNo < SHEETNO_NUM ){
		FwdSetRecoveryReq(sheetNo);
	}
	else{ /* NG */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendDiagResponseDc131Read
// Description   : ダイアグオペレーション(dC131/Read)指示応答の送信
//               : 依頼受信時の処理を実施する。
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
//               : dataType (Data Type)
//               : readAllowed (Read Allowed )
//               : writeAllowed (Write Allowed )
//               : factoryInitAllowed (Factory Init Allowed )
//               : crntVal (Current Value)
//               : minVal (Minimum Value)
//               : maxVal (Maximum Value)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendDiagResponseDc131Read(UC result, US chain, US link, UC dataType, UC readAllowed, UC writeAllowed, UC factoryInitAllowed, UL crntVal, UL minVal, UL maxVal)
{
	FwdDiagResponseDc131Read(result, chain, link, dataType, readAllowed, writeAllowed, factoryInitAllowed, crntVal, minVal, maxVal);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendDiagResponseDc131Write
// Description   : ダイアグオペレーション(dC131/Write)指示応答の送
//               : 信依頼受信時の処理を実施する。
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
//               : updateResponse (Update Response)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendDiagResponseDc131Write(UC result, US chain, US link, UC updateResponse)
{
	FwdDiagResponseDc131Write(result, chain, link, updateResponse);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendDiagResponseDc135Read
// Description   : ダイアグオペレーション(dC135/Read)指示応答の送信
//               : 依頼受信時の処理を実施する。
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
//               : counterValue (Counter Value)
//               : lifeSpec (Life Spec)
//               : reasonLast (Reason For Last Exchange)
//               : counterValLast (Counter Value At Last Exchange)
//               : reasonLastButOne (Reason For Last Exchange But 
//               : One)
//               : counterValLastButOne (Counter Value At Last 
//               : Exchange But One)
//               : reasonLastButTwo (Reason For Last Exchange But 
//               : Two)
//               : counterValLastButTwo (Counter Value At Last 
//               : Exchange But Two)
//               : pCounterName (Data pointer of Counter Name)
//               : counterNameSize (Data size of Counter Name)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendDiagResponseDc135Read(UC result, US chain, US link, UL counterValue, UL lifeSpec, UC reasonLast, UL counterValLast, UC reasonLastButOne, UL counterValLastButOne, UC reasonLastButTwo, UL counterValLastButTwo, UC const* pCounterName, UC counterNameSize)
{
	FwdDiagResponseDc135Read(result, chain, link, counterValue, lifeSpec, reasonLast, counterValLast, reasonLastButOne, counterValLastButOne, reasonLastButTwo, counterValLastButTwo, pCounterName, counterNameSize);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendDiagResponseDc135Reset
// Description   : ダイアグオペレーション(dC135/Reset)指示応答の送
//               : 信依頼受信時の処理を実施する。
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
//               : devContID (Device Controller ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendDiagResponseDc135Reset(UC result, US chain, US link, UC devContID)
{
	FwdDiagResponseDc135Reset(result, chain, link, devContID);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendDiagResponseDc135Write
// Description   : ダイアグオペレーション(dC135/Write)指示応答の送
//               : 信依頼受信時の処理を実施する。
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendDiagResponseDc135Write(UC result, US chain, US link)
{
	FwdDiagResponseDc135Write(result, chain, link);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendDiagResponseDc301Start
// Description   : ダイアグオペレーション(dC301/Start)指示応答の送
//               : 信依頼受信時の処理を実施する。
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
//               : devContID (Device Controller ID)
//               : attrInitResp (Attribute Init Response)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendDiagResponseDc301Start(UC result, US chain, US link, UC devContID, UC attrInitResp)
{
	FwdDiagResponseDc301Start(result, chain, link, devContID, attrInitResp);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendDiagResponseDc330Monitor
// Description   : ダイアグオペレーション(dC330/Monitor)指示応答の
//               : 送信依頼受信時の処理を実施する。
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
//               : componentState (Component State)
//               : transitionCounter (Transition Counter)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendDiagResponseDc330Monitor(UC result, US chain, US link, UC componentState, UC transitionCounter)
{
	FwdDiagResponseDc330Monitor(result, chain, link, componentState, transitionCounter);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendDiagResponseDc330Write
// Description   : ダイアグオペレーション(dC330/Write)指示応答の送
//               : 信依頼受信時の処理を実施する。
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
//               : componentResponse (Component Response)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendDiagResponseDc330Write(UC result, US chain, US link, UC componentResponse)
{
	FwdDiagResponseDc330Write(result, chain, link, componentResponse);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendDiagResponseDc361Start
// Description   : ダイアグオペレーション(dC361/Start)指示応答の送
//               : 信依頼受信時の処理を実施する。
// Parameter     : result (Diagnostic Result)
//               : nvmArea (NVM Area)
//               : devContID (Device Controller ID)
//               : nvmAreaSize (NVM Area Size)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendDiagResponseDc361Start(UC result, UC nvmArea, UC devContID, US nvmAreaSize)
{
	FwdDiagResponseDc361Start(result, nvmArea, devContID, nvmAreaSize);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendDiagResponseDc361Read
// Description   : ダイアグオペレーション(dC361/Read)指示応答の送信
//               : 依頼受信時の処理を実施する。
// Parameter     : result (Diagnostic Result)
//               : nvmArea (NVM Area)
//               : devContID (Device Controller ID)
//               : presenceRemain (Presence Of Remain Data)
//               : allocateSize (Allocated Data Size)
//               : pAllocateValue (Data pointer of Allocated Data 
//               : Value)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendDiagResponseDc361Read(UC result, UC nvmArea, UC devContID, UC presenceRemain, UC allocateSize, UC* pAllocateValue)
{
	FwdDiagResponseDc361Read(result, nvmArea, devContID, presenceRemain, allocateSize, pAllocateValue);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_SendDiagResponseDc361Write
// Description   : ダイアグオペレーション(dC361/Write)指示応答の送
//               : 信依頼受信時の処理を実施する。
// Parameter     : result (Diagnostic Result)
//               : nvmArea (NVM Area)
//               : devContID (Device Controller ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_SendDiagResponseDc361Write(UC result, UC nvmArea, UC devContID)
{
	FwdDiagResponseDc361Write(result, nvmArea, devContID);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvDevCapabilitiesRequest
// Description   : Device Capabilities Requestメッセージ受信時の処
//               : 理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvDevCapabilitiesRequest(CCdiFinMessage* pMessage)
{
	CCdiFinDevCapaReqMsg message;
	UC subsystem, instance, attrKey;

	CCdiFinDevCapaReqMsg_Constructor(&message);
	CCdiFinDevCapaReqMsg_SetMessage(&message, CCdiFinMessage_GetMessage(pMessage), CCdiFinMessage_GetSize(pMessage));

	subsystem = CCdiFinDevCapaReqMsg_GetSubsystem(&message);
	instance  = CCdiFinDevCapaReqMsg_GetInstance(&message);
	attrKey   = CCdiFinDevCapaReqMsg_GetAttrKey(&message);

	if ( subsystem == eCdiFinSubsys_UpdateAll ){
		if ( (gCdi_AdapterMember.mEventBit & CDI_ADAPT_EVT_CNV_BIT(CDI_ADAPT_EVT_CAPA_REQ)) == 0x00 ){
																				// Power Up後一回だけ実施
			if ( (gCdi_AdapterMember.mEventBit & CDI_ADAPT_EVT_CNV_BIT(CDI_ADAPT_EVT_CFG_REQ)) == 0x00 ){
				CSysManager_SystemConfigurationSet();							// Power Up SequenceのDevice Capabilities Requestを省略していない
			}
			else{ /* No Action */ }

			gCdi_AdapterMember.mEventBit |= CDI_ADAPT_EVT_CNV_BIT(CDI_ADAPT_EVT_CAPA_REQ);
		}
		else{ /* No Action */ }
	}
	else{ /* No Action */ }

	FwdDevCapabilitiesUpdate(subsystem, instance, attrKey);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvDevConfigurationRequest
// Description   : Device Configuration Requestメッセージ受信時の処
//               : 理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvDevConfigurationRequest(CCdiFinMessage* pMessage)
{
	CCdiFinDevCfgReqMsg message;
	UC subsystem, instance, attrKey;

	CCdiFinDevCfgReqMsg_Constructor(&message);
	CCdiFinDevCfgReqMsg_SetMessage(&message, CCdiFinMessage_GetMessage(pMessage), CCdiFinMessage_GetSize(pMessage));

	subsystem = CCdiFinDevCfgReqMsg_GetSubsystem(&message);
	instance  = CCdiFinDevCfgReqMsg_GetInstance(&message);
	attrKey	  = CCdiFinDevCfgReqMsg_GetAttrKey(&message);

	if ( (subsystem == eCdiFinSubsys_UpdateAll) || (subsystem == eCdiFinSubsys_PwrSavrUpdate) ){
		if ( (gCdi_AdapterMember.mEventBit & CDI_ADAPT_EVT_CNV_BIT(CDI_ADAPT_EVT_CFG_REQ)) == 0x00 ){
																				// Power Up後一回だけ実施
			if ( (gCdi_AdapterMember.mEventBit & CDI_ADAPT_EVT_CNV_BIT(CDI_ADAPT_EVT_CAPA_REQ)) == 0x00 ){
				CSysManager_SystemConfigurationSet();							// Power Up SequenceのDevice Capabilities Request省略時
			}
			else{ /* No Action */ }

			if ( subsystem == eCdiFinSubsys_PwrSavrUpdate ){
				CSysManager_PowerUpModeSet(SYS_PWR_UP_MODE_SAVE);
			}
			else {																// subsystem == eCdiFinSubsys_UpdateAll
				CSysManager_PowerUpModeSet(SYS_PWR_UP_MODE_NORMAL);
			}

			gCdi_AdapterMember.mEventBit |= CDI_ADAPT_EVT_CNV_BIT(CDI_ADAPT_EVT_CFG_REQ);
		}
		else{ /* No Action */ }
	}
	else { /* No Action */ }

	FwdDevConfigurationUpdate(subsystem, instance, attrKey);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvSetDevConfiguration
// Description   : Set Device Configurationメッセージ受信時の処理を
//               : 実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvSetDevConfiguration(CCdiFinMessage* pMessage)
{
	CCdiFinSetDevCfgMsg message;
	US index;
	UC subsystem, instance, attrKey;

	CCdiFinSetDevCfgMsg_Constructor(&message);
	CCdiFinSetDevCfgMsg_SetMessage(&message, CCdiFinMessage_GetMessage(pMessage), CCdiFinMessage_GetSize(pMessage));

	subsystem = CCdiFinSetDevCfgMsg_GetSubsystem(&message);
	instance  = CCdiFinSetDevCfgMsg_GetInstance(&message);

	// --- Attribute情報解析 ---
	if ( subsystem == (UC)eCdiFinSubsys_DevCont ){
		CCdiFinDevContConfigurationAttr devContAttr;
		CCfgDevContConfiguration *pDevContCfg;

		CCdiFinDevContConfigurationAttr_Constructor(&devContAttr);

		pDevContCfg = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);

		CCdiFinDevContConfigurationAttr_SetAttrPairsPointer(&devContAttr, CCdiFinSetDevCfgMsg_GetAttrPairs(&message), (UC)(CCdiFinSetDevCfgMsg_GetAttrPairsSize(&message)));
		CCdiFinDevContConfigurationAttr_AnalyzeAttrPairs(&devContAttr, CCdiFinSetDevCfgMsg_GetNumAttr(&message));

		for ( index = 0; index < CDIFIN_ATTRKEYMAXNUM; index++ ){
			if ( CCdiFinDevContConfigurationAttr_CheckAttrSet(&devContAttr, (UC)index) == true ){
				switch ( index ){
				case eCdiFinCfgKeyDevCont_Market:		ImportDevCfgContAttr01(pDevContCfg, &devContAttr);	break;
				case eCdiFinCfgKeyDevCont_DateTime:		ImportDevCfgContAttr40(pDevContCfg, &devContAttr);	break;
				case eCdiFinCfgKeyDevCont_SysCfg:		ImportDevCfgContAttr41(pDevContCfg, &devContAttr);	break;
				case eCdiFinCfgKeyDevCont_MediaSizeGrp:	ImportDevCfgContAttrC0(pDevContCfg, &devContAttr);	break;
				case eCdiFinCfgKeyDevCont_ShtExtHeight:	ImportDevCfgContAttrFA(pDevContCfg, &devContAttr);	break;
				case eCdiFinCfgKeyDevCont_Productivity: ImportDevCfgContAttrFB(pDevContCfg, &devContAttr);	break;
				case eCdiFinCfgKeyDevCont_ExitSpdRange:	ImportDevCfgContAttrFC(pDevContCfg, &devContAttr);	break;
				case eCdiFinCfgKeyDevCont_ExitLayout:	ImportDevCfgContAttrFD(pDevContCfg, &devContAttr);	break;
				case eCdiFinCfgKeyDevCont_JamBypass:	ImportDevCfgContAttrFE(pDevContCfg, &devContAttr);	break;
				case eCdiFinCfgKeyDevCont_NoPprRun:		ImportDevCfgContAttrFF(pDevContCfg, &devContAttr);	break;
				default:																					break;
				}
			}
			else{ /* No Action */ }
		}
	}
	else if ( subsystem == (UC)eCdiFinSubsys_OutDest ){
#if OUT_DEST_NUM != 0
		CCdiFinOutDestConfigurationAttr outDestAttr;
		CCfgOutDestConfiguration *pOutDestCfg;

		CCdiFinOutDestConfigurationAttr_Constructor(&outDestAttr);

		pOutDestCfg = CCfgInformer_GetOutputDestinationConfiguration(instance);

		if ( pOutDestCfg != NULL ){
			CCdiFinOutDestConfigurationAttr_SetAttrPairsPointer(&outDestAttr, CCdiFinSetDevCfgMsg_GetAttrPairs(&message), (UC)(CCdiFinSetDevCfgMsg_GetAttrPairsSize(&message)));
			CCdiFinOutDestConfigurationAttr_AnalyzeAttrPairs(&outDestAttr, CCdiFinSetDevCfgMsg_GetNumAttr(&message));

			for ( index = 0; index < CDIFIN_ATTRKEYMAXNUM; index++ ){
				if ( CCdiFinOutDestConfigurationAttr_CheckAttrSet(&outDestAttr, (UC)index) == true ){
					switch ( index ){
					case eCdiFinCfgKeyOutDest_Select:	ImportDevCfgDestAttr03(pOutDestCfg, &outDestAttr);	break;	// Not receive Selectability
					case eCdiFinCfgKeyOutDest_Offset:	ImportDevCfgDestAttr45(pOutDestCfg, &outDestAttr);	break;	// Not receive Current Offset Configuration
					case eCdiFinCfgKeyOutDest_Fold:		ImportDevCfgDestAttr46(pOutDestCfg, &outDestAttr);	break;	// Not receive Current Sheet Folding Configuration
					case eCdiFinCfgKeyOutDest_Staple:	ImportDevCfgDestAttr47(pOutDestCfg, &outDestAttr);	break;	// Not receive Current Staple Configuration
					case eCdiFinCfgKeyOutDest_Punch:	ImportDevCfgDestAttr48(pOutDestCfg, &outDestAttr);	break;	// Not receive Current Punch Configuration
					case eCdiFinCfgKeyOutDest_Book:		ImportDevCfgDestAttr49(pOutDestCfg, &outDestAttr);	break;	// Not receive Current Booklet Maker Configuration
					default:																				break;
					}
				}
				else{ /* No Action */ }
			}
		}
		else{ /* No Action */ }
#endif
	}
	else if ( subsystem == (UC)eCdiFinSubsys_InSrc ){
#if IN_SRC_NUM != 0
		CCdiFinInSrcConfigurationAttr inSrcAttr;
		CCfgInSrcConfiguration *pInSrcCfg;

		CCdiFinInSrcConfigurationAttr_Constructor(&inSrcAttr);

		pInSrcCfg = CCfgInformer_GetInputSourceConfiguration(instance);

		if ( pInSrcCfg != NULL ){
			CCdiFinInSrcConfigurationAttr_SetAttrPairsPointer(&inSrcAttr, CCdiFinSetDevCfgMsg_GetAttrPairs(&message), (UC)(CCdiFinSetDevCfgMsg_GetAttrPairsSize(&message)));
			CCdiFinInSrcConfigurationAttr_AnalyzeAttrPairs(&inSrcAttr, CCdiFinSetDevCfgMsg_GetNumAttr(&message));

			for ( index = 0; index < CDIFIN_ATTRKEYMAXNUM; index++ ){
				if ( CCdiFinInSrcConfigurationAttr_CheckAttrSet(&inSrcAttr, (UC)index) == true ){
					switch ( index ){
					case eCdiFinCfgKeyInSrc_Select:		ImportDevCfgSrcAttr03(pInSrcCfg, &inSrcAttr);	break;	// Selectability
					case eCdiFinCfgKeyInSrc_SizeAlt:	ImportDevCfgSrcAttrC0(pInSrcCfg, &inSrcAttr);	break;	// Media Size Alternate
					case eCdiFinCfgKeyInSrc_IregSize:	ImportDevCfgSrcAttrC1(pInSrcCfg, &inSrcAttr);	break;	// Irregular Media Size
					default:																			break;
					}
				}
				else{ /* No Action */ }
			}
		}
		else{ /* No Action */ }
#endif
	}
	else { /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvInitRequest
// Description   : Initialization Requestメッセージ受信時の処理を実
//               : 施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvInitRequest(CCdiFinMessage* pMessage)
{
// --- メッセージ解読不要なためコメントアウト -------------------------------------------
//	CCdiFinInitReqMsg message;

// 	CCdiFinInitReqMsg_Constructor(&message);
//	CCdiFinInitReqMsg_SetMessage(&message, CCdiFinMessage_GetMessage(pMessage), CCdiFinMessage_GetSize(pMessage));
// --------------------------------------------------------------------------------------

	FwdInitAcknowledge();
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvInitAcknowledge
// Description   : Initialization Acknowledgeメッセージ受信時の処理
//               : を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvInitAcknowledge(CCdiFinMessage* pMessage)
{
// --- メッセージ解読不要なためコメントアウト -------------------------------------------
//	CCdiFinInitAckMsg message;

// 	CCdiFinInitAckMsg_Constructor(&message);
//	CCdiFinInitAckMsg_SetMessage(&message, CCdiFinMessage_GetMessage(pMessage), CCdiFinMessage_GetSize(pMessage));
// --------------------------------------------------------------------------------------

	if ( CCdiProductionState_InformedEvent(&(gCdi_AdapterMember.mProductState), CDI_PRDCT_STT_EVT_INIT_STA) == true ){
		FwdDevContInfoCommInCdiFin();

		FwdDevContStateUpdate();
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvInitComplete
// Description   : Initialization Completeメッセージ受信時の処理を
//               : 実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvInitComplete(CCdiFinMessage* pMessage)
{
// --- メッセージ解読不要なためコメントアウト -------------------------------------------
//	CCdiFinInitCompMsg message;

// 	CCdiFinInitCompMsg_Constructor(&message);
//	CCdiFinInitCompMsg_SetMessage(&message, CCdiFinMessage_GetMessage(pMessage), CCdiFinMessage_GetSize(pMessage));
// --------------------------------------------------------------------------------------

	if ( CCdiProductionState_InformedEvent(&(gCdi_AdapterMember.mProductState), CDI_PRDCT_STT_EVT_INIT_COMP) == true ){
																				// 状態遷移あり
		gCdi_AdapterMember.mEventBit |= CDI_ADAPT_EVT_CNV_BIT(CDI_ADAPT_EVT_JAM_ZONE_UPDT);

		FwdJamZoneUpdate(OWN_DEV_CONT_ID);

		FwdDevContStateUpdate();
	}
	else{ /* No Action */ }	
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvPwrOffAnnouncement
// Description   : Power Off Announcementメッセージ受信時の処理を実
//               : 施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvPwrOffAnnouncement(CCdiFinMessage* pMessage)
{
// --- メッセージ解読不要なためコメントアウト -------------------------------------------
//	CCdiFinPwrOffAnnounceMsg message;

// 	CCdiFinPwrOffAnnounceMsg_Constructor(&message);
//	CCdiFinPwrOffAnnounceMsg_SetMessage(&message, CCdiFinMessage_GetMessage(pMessage), CCdiFinMessage_GetSize(pMessage));
// --------------------------------------------------------------------------------------
	// --- Power Up Sequence中止通知 ---
	CSysManager_SystemConfigurationSet();	// Configuration情報は初期値のままPower Up完了させてPower Off Sequenceへ移行
	CSysManager_PowerUpModeSet(SYS_PWR_UP_MODE_ABORT);

	// --- Power Down指示 ---
	CSysManager_PowerDown();
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvPollRequest
// Description   : Poll Requestメッセージ受信時の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvPollRequest(CCdiFinMessage* pMessage)
{
// --- メッセージ解読不要なためコメントアウト -------------------------------------------
//	CCdiFinPollReqMsg message;

// 	CCdiFinPollReqMsg_Constructor(&message);
//	CCdiFinPollReqMsg_SetMessage(&message, CCdiFinMessage_GetMessage(pMessage), CCdiFinMessage_GetSize(pMessage));
// --------------------------------------------------------------------------------------

	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvSelectFinAttribute
// Description   : Select Finisher Attributeメッセージ受信時の処理
//               : を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvSelectFinAttribute(CCdiFinMessage* pMessage)
{
	CCdiFinSlctFinAttrMsg message;
	CCdiFinSheetFinishAttr shtFinAttr;

	CCdiFinSlctFinAttrMsg_Constructor(&message);
	CCdiFinSheetFinishAttr_Constructor(&shtFinAttr);

	CCdiFinSlctFinAttrMsg_SetMessage(&message, CCdiFinMessage_GetMessage(pMessage), CCdiFinMessage_GetSize(pMessage));

	CCdiFinSheetFinishAttr_SetAttrPairsPointer(&shtFinAttr, CCdiFinSlctFinAttrMsg_GetAttrPairs(&message), CCdiFinSlctFinAttrMsg_GetAttrPairsSize(&message));
	CCdiFinSheetFinishAttr_AnalyzeAttrPairs(&shtFinAttr, CCdiFinSlctFinAttrMsg_GetNumAttr(&message));

	CShtSheetAttribute_Reset(&gCdi_AdapterMember.mSheetAttr);

	SetSheetAttribute(&gCdi_AdapterMember.mSheetAttr, &shtFinAttr);

	CShtManager_CreateProcessStartSheet(CCdiFinSlctFinAttrMsg_GetDestID(&message), CCdiFinSlctFinAttrMsg_GetBinSelect(&message),  &(gCdi_AdapterMember.mSheetAttr));

	CSysManager_CycleUp();
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvSheetFinRequest
// Description   : Sheet Finish Requestメッセージ受信時の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvSheetFinRequest(CCdiFinMessage* pMessage)
{
	CCdiFinShtFinReqMsg message;
	CCdiFinSheetFinishAttr shtFinAttr;
	UC sheetNo;

	CCdiFinShtFinReqMsg_Constructor(&message);
	CCdiFinSheetFinishAttr_Constructor(&shtFinAttr);

	CCdiFinShtFinReqMsg_SetMessage(&message, CCdiFinMessage_GetMessage(pMessage), CCdiFinMessage_GetSize(pMessage));

	CCdiFinSheetFinishAttr_SetAttrPairsPointer(&shtFinAttr, CCdiFinShtFinReqMsg_GetAttrPairs(&message), CCdiFinShtFinReqMsg_GetAttrPairsSize(&message));
	CCdiFinSheetFinishAttr_AnalyzeAttrPairs(&shtFinAttr, CCdiFinShtFinReqMsg_GetNumAttr(&message));

	SetSheetAttribute(&gCdi_AdapterMember.mSheetAttr, &shtFinAttr);

	sheetNo = CShtManager_CreateSheet(CCdiFinShtFinReqMsg_GetSheetID(&message),
							CCdiFinShtFinReqMsg_GetSetID(&message),
							CCdiFinShtFinReqMsg_GetDestID(&message),
							CCdiFinShtFinReqMsg_GetBinSelect(&message),
							&(gCdi_AdapterMember.mSheetAttr));
	FwdReadyForNextSheet(sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvChangeShtFinRequest
// Description   : Change Sheet Finish Requestメッセージ受信時の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
// 20160225 V3.4.0 Merge Trouble Action [ Change Sheet Finish Request受信不正 ]
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvChangeShtFinRequest(CCdiFinMessage* pMessage)
{
	CCdiFinChgShtFinReqMsg message;
	CShtSheetAttribute shtAttr;
	CCdiFinSheetFinishAttr shtFinAttr;
	UC sheetNo;

	CCdiFinChgShtFinReqMsg_Constructor(&message);
	CCdiFinSheetFinishAttr_Constructor(&shtFinAttr);
	CCdiFinChgShtFinReqMsg_SetMessage(&message, CCdiFinMessage_GetMessage(pMessage), CCdiFinMessage_GetSize(pMessage));

	CCdiFinSheetFinishAttr_SetAttrPairsPointer(&shtFinAttr, CCdiFinChgShtFinReqMsg_GetAttrPairs(&message), CCdiFinChgShtFinReqMsg_GetAttrPairsSize(&message));
	CCdiFinSheetFinishAttr_AnalyzeAttrPairs(&shtFinAttr, CCdiFinChgShtFinReqMsg_GetNumAttr(&message));

	sheetNo = CShtManager_GetSheetNo(CCdiFinChgShtFinReqMsg_GetSheetID(&message));

	if ( sheetNo != SHEETNO_NUM ){
		shtAttr = *(CShtSheet_GetAttribute(CShtManager_GetSheet(sheetNo)));

		SetSheetAttribute(&shtAttr, &shtFinAttr);

		CShtManager_UpdateSheetAttribute(CCdiFinChgShtFinReqMsg_GetSheetID(&message), &shtAttr);
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvCancelShtFinRequest
// Description   : Cancel Sheet Finish Requestメッセージ受信時の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvCancelShtFinRequest(CCdiFinMessage* pMessage)
{
	CCdiFinCnclShtFinReqMsg message;

	CCdiFinCnclShtFinReqMsg_Constructor(&message);
	CCdiFinCnclShtFinReqMsg_SetMessage(&message, CCdiFinMessage_GetMessage(pMessage), CCdiFinMessage_GetSize(pMessage));

	CShtManager_CancelSheet(CCdiFinCnclShtFinReqMsg_GetSheetID(&message));
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvSheetExit
// Description   : Sheet Exit メッセージ受信時の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvSheetExit(CCdiFinMessage* pMessage)
{
	CCdiFinSheetExitMsg message;

	CCdiFinSheetExitMsg_Constructor(&message);
	CCdiFinSheetExitMsg_SetMessage(&message, CCdiFinMessage_GetMessage(pMessage), CCdiFinMessage_GetSize(pMessage));

	CShtManager_ExecuteSheet(CCdiFinSheetExitMsg_GetSheetID(&message));

	
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvSetFinRequest
// Description   : Set Finish Requestメッセージ受信時の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvSetFinRequest(CCdiFinMessage* pMessage)
{
	CCdiFinSetFinReqMsg message;

	CCdiFinSetFinReqMsg_Constructor(&message);
	CCdiFinSetFinReqMsg_SetMessage(&message, CCdiFinMessage_GetMessage(pMessage), CCdiFinMessage_GetSize(pMessage));

	CShtManager_SetEnd(CCdiFinSetFinReqMsg_GetSetEndAct(&message));
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvStopFinisher
// Description   : Stop Finisherメッセージ受信時の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvStopFinisher(CCdiFinMessage* pMessage)
{
	CCdiFinStopFinMsg message;

	CCdiFinStopFinMsg_Constructor(&message);
	CCdiFinStopFinMsg_SetMessage(&message, CCdiFinMessage_GetMessage(pMessage), CCdiFinMessage_GetSize(pMessage));

	CShtManager_SheetEnd();

	if ( CCdiFinStopFinMsg_GetShutDownType(&message) == eCdiFinDwnType_CycDwn ){
		CSysManager_CycleDown();
	}
	else{																		// ShutDown;
		CSysManager_HardDown();
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvReqJamSttsCheck
// Description   : Request Jam Status Checkメッセージ受信時の処理を
//               : 実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvReqJamSttsCheck(CCdiFinMessage* pMessage)
{
	CCdiFinJamSttsCheckMsg message;

	CCdiFinJamSttsCheckMsg_Constructor(&message);
	CCdiFinJamSttsCheckMsg_SetMessage(&message, CCdiFinMessage_GetMessage(pMessage), CCdiFinMessage_GetSize(pMessage));

	switch( CCdiFinJamSttsCheckMsg_GetCheckTrigger(&message) ){
	case eCdiFinChkTrgr_ExtAreaCvrClose:										// Exit Area Cover Close
		CSysManager_EntranceAreaTampered();

		break;
	default:
		// No Action
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvDevicePauseReq
// Description   : Device Pause Requestメッセージ受信時の処理を実施
//               : する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvDevicePauseReq(CCdiFinMessage* pMessage)
{
	CCdiFinDevPauseReqMsg message;

	CCdiFinDevPauseReqMsg_Constructor(&message);
	CCdiFinDevPauseReqMsg_SetMessage(&message, CCdiFinMessage_GetMessage(pMessage), CCdiFinMessage_GetSize(pMessage));

	switch ( CCdiFinDevPauseReqMsg_GetPauseRequest(&message) ){
	//	!!! Product Specific !!!
//	case eCdiFinPauseReq_Xxxx:
//	
//		break;
	default:
		// No Action
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvDevContActionReq
// Description   : Device Controller Action Requestメッセージ受信時
//               : の処理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvDevContActionReq(CCdiFinMessage* pMessage)
{
	// Not Supported
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvChangeDevContMode
// Description   : Change Device Controllerメッセージ受信時の処理を
//               : 実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvChangeDevContMode(CCdiFinMessage* pMessage)
{
	CCdiFinChgDevContModeMsg message;

	CCdiFinChgDevContModeMsg_Constructor(&message);
	CCdiFinChgDevContModeMsg_SetMessage(&message, CCdiFinMessage_GetMessage(pMessage), CCdiFinMessage_GetSize(pMessage));

	switch ( CCdiFinChgDevContModeMsg_GetSysOpeMode(&message) ){
	case eCdiFinSysOpeMode_Nrml:
		CSysManager_ChangeNormal();
		break;
	case eCdiFinSysOpeMode_SrvcDiag:
		CSysManager_ChangeDiag();
		break;
	case eCdiFinSysOpeMode_SoftDL:
		CDltManager_Transition();//20160314 Merge2

		//@ Downloadモード移行方法決定(Download設計完了)後に実装
		break;
	case eCdiFinSysOpeMode_Unknown:
		// No Action
		break;
	default:
		// No Action
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvDeviceStatusRequest
// Description   : Device Status Requestメッセージ受信時の処理を実
//               : 施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvDeviceStatusRequest(CCdiFinMessage* pMessage)
{
	CCdiFinDevSttsReqMsg message;
	CDvstDeviceStatus* pDevStts;
	US statusID, statusNo;

	CCdiFinDevSttsReqMsg_Constructor(&message);
	CCdiFinDevSttsReqMsg_SetMessage(&message, CCdiFinMessage_GetMessage(pMessage), CCdiFinMessage_GetSize(pMessage));

	statusID = CCdiFinDevSttsReqMsg_GetStatusID(&message);

	if ( statusID == eCdiFinSttsID_All ){
		gCdi_AdapterMember.mEventBit |= CDI_ADAPT_EVT_CNV_BIT(CDI_ADAPT_EVT_DEV_STTS_REQ);

		pDevStts = CDvstInformer_GetDeviceStatusInOrder(DSNO_NUM);

		while ( pDevStts != NULL ){
			statusID = CDvstDeviceStatus_GetStatusID(pDevStts);
			statusNo = CDvstInformer_GetDeviceStatusNo(statusID);

			if ( CDvstDeviceStatus_IsDefaultStatus(pDevStts, CDvstDeviceStatus_GetStatusValue(pDevStts)) == false ){
				FwdDevSttsUpdate(statusNo, CDvstDeviceStatus_GetStatusValue(pDevStts));
			}
			else{ /* No Action */ }

			pDevStts = CDvstInformer_GetDeviceStatusInOrder(statusNo);
		}

		FwdDevSttsUpdate(DSNO_NUM, 0);											// Send Complete
	}
	else if ( statusID == eCdiFinSttsID_Comp ){
		// No Action
	}
	else{
		statusNo = CDvstInformer_GetDeviceStatusNo(statusID);
		pDevStts = CDvstInformer_GetDeviceStatus(statusNo);

		FwdDevSttsUpdate(CDvstInformer_GetDeviceStatusNo(statusID), CDvstDeviceStatus_GetStatusValue(pDevStts));
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvSysContInfo
// Description   : System Controller Informationメッセージ受信時の処
//               : 理を実施する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvSysContInfo(CCdiFinMessage* pMessage)
{
	CCdiFinSysContInfoMsg message;

	CCdiFinSysContInfoMsg_Constructor(&message);
	CCdiFinSysContInfoMsg_SetMessage(&message, CCdiFinMessage_GetMessage(pMessage), CCdiFinMessage_GetSize(pMessage));

	switch ( CCdiFinSysContInfoMsg_GetInformationID(&message) ){
	case eCdiFinSysContInfoID_CommInCdiFin:
		RcvSysContInfoCommInCdiFin(&message);
		break;
	default:
		// No Action
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiAdapter_RcvExecuteDiag
// Description   : Execute Diagnosticsメッセージ受信時の処理を実施
// 				 : する。
// Parameter     : pMessage (Message Data Pointer)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiAdapter_RcvExecuteDiag(CCdiFinMessage* pMessage)
{
	CCdiFinExecDiagMsg message;
	US diagProgram;
	UC diagCommand;

	CCdiFinExecDiagMsg_Constructor(&message);
	CCdiFinExecDiagMsg_SetMessage(&message, CCdiFinMessage_GetMessage(pMessage), CCdiFinMessage_GetSize(pMessage));

	diagProgram = CCdiFinExecDiagMsg_GetDiagProgram(&message);
	diagCommand  = CCdiFinExecDiagMsg_GetDiagCommand(&message);

	switch ( diagProgram ){
#if DIAG_SWITCH_DC128 == true
	case eCdiFinDiagPrgrm_dC128:												// Fold Position Adjust

		switch( diagCommand ){
		case eCdiFinDiagCmd_Read:
			RcvExecuteDiagDc128Read(&message);
			break;
		case eCdiFinDiagCmd_Write:
			RcvExecuteDiagDc128Write(&message);
			break;
		default:																// No Such Diag Command
			FwdDiagResponse((US)eCdiFinDiagPrgrm_dC128, diagCommand, (UC)eCdiFinDiagRslt_NoSuchDiagCmd);
		}

		break;
#endif
#if DIAG_SWITCH_DC131 == true
	case eCdiFinDiagPrgrm_dC131:												// NVM Access
		switch ( diagCommand ){
		case eCdiFinDiagCmd_Read:
			RcvExecuteDiagDc131Read(&message);
			break;
		case eCdiFinDiagCmd_Write:
			RcvExecuteDiagDc131Write(&message);
			break;
		default:																// No Such Diag Command
			FwdDiagResponse((US)eCdiFinDiagPrgrm_dC131, diagCommand, (UC)eCdiFinDiagRslt_NoSuchDiagCmd);
		}

		break;
#endif
#if DIAG_SWITCH_DC135 == true
	case eCdiFinDiagPrgrm_dC135:												// Device HFSI Counter Access
		switch ( diagCommand ){
		case eCdiFinDiagCmd_Read:
			RcvExecuteDiagDc135Read(&message);
			break;
		case eCdiFinDiagCmd_Reset:
			RcvExecuteDiagDc135Reset(&message);
			break;
		case eCdiFinDiagCmd_Write:
			RcvExecuteDiagDc135Write(&message);
			break;
		default:
			FwdDiagResponse((US)eCdiFinDiagPrgrm_dC135, diagCommand, (UC)eCdiFinDiagRslt_NoSuchDiagCmd);
		}		

		break;
#endif
#if DIAG_SWITCH_DC140 == true
	case eCdiFinDiagPrgrm_dC140:												// Analog Component Monitor
		switch ( diagCommand ){
		case eCdiFinDiagCmd_Monitor:
			RcvExecuteDiagDc140Monitor(&message);
			break;
		default:
			FwdDiagResponse((US)eCdiFinDiagPrgrm_dC140, diagCommand, (UC)eCdiFinDiagRslt_NoSuchDiagCmd);
		}

		break;
#endif
#if DIAG_SWITCH_DC301 == true
	case eCdiFinDiagPrgrm_dC301:												// NVM Initialization
		switch ( diagCommand ){
		case eCdiFinDiagCmd_Start:
			RcvExecuteDiagDc301Start(&message);		
			break;
		default:																// No Such Diag Command
			FwdDiagResponse((US)eCdiFinDiagPrgrm_dC301, diagCommand, (UC)eCdiFinDiagRslt_NoSuchDiagCmd);
		}

		break;
#endif
#if DIAG_SWITCH_DC330 == true
	case eCdiFinDiagPrgrm_dC330:												// Component Control
		switch ( diagCommand ){
		case eCdiFinDiagCmd_Monitor:
			RcvExecuteDiagDc330Monitor(&message);
			break;
		case eCdiFinDiagCmd_Write:
			RcvExecuteDiagDc330Write(&message);
			break;
		default:
			FwdDiagResponse((US)eCdiFinDiagPrgrm_dC330, diagCommand, (UC)eCdiFinDiagRslt_NoSuchDiagCmd);
		}

		break;
#endif
#if DIAG_SWITCH_DC361 == true
	case eCdiFinDiagPrgrm_dC361:												// NVM Area Access
		switch ( diagCommand ){
		case eCdiFinDiagCmd_Start:
			RcvExecuteDiagDc361Start(&message);
			break;
		case eCdiFinDiagCmd_Read:
			RcvExecuteDiagDc361Read(&message);
			break;
		case eCdiFinDiagCmd_Write:
			RcvExecuteDiagDc361Write(&message);
			break;
		default:																// No Such Diag Command
			FwdDiagResponse((US)eCdiFinDiagPrgrm_dC361, diagCommand, (UC)eCdiFinDiagRslt_NoSuchDiagCmd);
		}

		break;
#endif
#if DIAG_SWITCH_DC726 == true
	case eCdiFinDiagPrgrm_dC726:												// Paper Path Timing
		switch ( diagCommand ){
		case eCdiFinDiagCmd_Start:
			RcvExecuteDiagDc726Read(&message);
			break;
		case eCdiFinDiagCmd_Monitor:
			RcvExecuteDiagDc726Monitor(&message);
			break;
		default:																// No Such Diag Command
			FwdDiagResponse((US)eCdiFinDiagPrgrm_dC726, diagCommand, (UC)eCdiFinDiagRslt_NoSuchDiagCmd);
		}

		break;
#endif
#if DIAG_SWITCH_DC1100 == true
	case eCdiFinDiagPrgrm_dC1100:												// Recycle Information Access
		switch ( diagCommand ){
		case eCdiFinDiagCmd_Read:
			RcvExecuteDiagDc1100Read(&message);
			break;
		default:																// No Such Diag Command
			FwdDiagResponse((US)eCdiFinDiagPrgrm_dC1100, diagCommand, (UC)eCdiFinDiagRslt_NoSuchDiagCmd);
		}

		break;		
#endif
#if DIAG_SWITCH_DC1201 == true
	case eCdiFinDiagPrgrm_dC1201:												// User Fold Position Adjust
		switch ( diagCommand ){
		case eCdiFinDiagCmd_Read:
			RcvExecuteDiagDc1201Read(&message);
			break;
		case eCdiFinDiagCmd_Write:
			RcvExecuteDiagDc1201Write(&message);
			break;
		default:																// No Such Diag Command
			FwdDiagResponse((US)eCdiFinDiagPrgrm_dC1201, diagCommand, (UC)eCdiFinDiagRslt_NoSuchDiagCmd);
		}

		break;
#endif
#if DIAG_SWITCH_DC1202 == true
	case eCdiFinDiagPrgrm_dC1202:												// User Fold Position Adjust
		switch ( diagCommand ){
		case eCdiFinDiagCmd_Read:
			RcvExecuteDiagDc1202Read(&message);
			break;
		case eCdiFinDiagCmd_Write:
			RcvExecuteDiagDc1202Write(&message);
			break;
		default:																// No Such Diag Command
			FwdDiagResponse((US)eCdiFinDiagPrgrm_dC1202, diagCommand, (UC)eCdiFinDiagRslt_NoSuchDiagCmd);
		}

		break;
#endif
	default:																	// Fail(No Such Diag Program)
		FwdDiagResponse(diagProgram, diagCommand, (UC)eCdiFinDiagRslt_NoSuchDiagPrgrm);
	}
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.12.04	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 14.12.29	| Yuichi Hoshino	| Design Improvement [ Power Upサブシステム設計見直し ]
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVMパラメータ ]
// 15.03.30	| Yuichi Hoshino	| Modify by Trouble Action [ 節電復帰時にDevice Capabilities Requestに応答せず ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
// 15.06.19	| Yuichi Hoshino	| Introduction Spec [ Device Controller Information Notify / Cancel Stapleメッセージ送信対応 ]
// 15.09.03	| Yuichi Hoshino	| Design Improvement [ Prep Time算出CPU負荷分散 ]
// 15.09.03	| Yuichi Hoshino	| Design Improvement [ CDIメッセージ送信処理スタック量削減 ]
// 15.10.20	| Yuichi Hoshino	| Trouble Action [ Change Sheet Finish Request受信不正 ]
// 15.12.09	| Yuichi Hoshino	| Design Improvement [ CDiagManger綴り修正 ]
// 15.12.18	| Yuichi Hoshino	| Trouble Action [ 通信フェイル発生時にフィニッシングプロセスからスタンバイに遷移しない ]
// 16.04.22	| Yuichi Hoshino	| Trouble Action [ T0151：Power Up Sequence中の電源OFF時にPower Off Acknowledgeを応答しない ]

