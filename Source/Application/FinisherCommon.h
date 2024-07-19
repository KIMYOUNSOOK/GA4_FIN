// File Name:	FinisherCommon.h
// Description:	Header File of Finisher Common
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef FinisherCommon_h
#define FinisherCommon_h

////////////////////////////////////////////////////////////
// Include Common Header File
////////////////////////////////////////////////////////////

#include "Common.h"
#include "SoftwareVersion.h"
#include "FinisherMacro.h"
#include "DD_LOG.h"
#include "MSP_Service.h"
#include "TempDataInst.h"

////////////////////////////////////////////////////////////
// Application Module Common Definition
////////////////////////////////////////////////////////////

// --- Product Specification ---
#include "FinisherProductSpec.h"

// --- Subsystem I/F Direct Call Prototype ---
#include "FinisherIfDirectCall.h"

// --- Finisher State --- 
typedef enum /*EFinisherState*/
{
	FIN_STT_UNKNOWN		  = 0,
	FIN_STT_POWER_UP	  = 1,
	FIN_STT_POWER_ON	  = 2,
	FIN_STT_POWER_DOWN	  = 3,
	FIN_STT_POWER_OFF	  = 4,
	FIN_STT_CHANGE_DIAG	  = 5,
	FIN_STT_DIAGNOSTIC	  = 6,
	FIN_STT_CHANGE_NORMAL = 7,
	FIN_STT_STANDBY		  = 8,
	FIN_STT_CYCLE_UP	  = 9,
	FIN_STT_READY		  = 10,
	FIN_STT_CYCLE_DOWN	  = 11,
	FIN_STT_HARD_DOWN	  = 12,
	FIN_STT_NUM							// Number of finisher state
} EFinisherState;

#define FIN_STT_CNV_BIT(state)	(0x0001 << state)

// --- Module ID ---
#define MID_CNV_BIT(moduleID)	(0x0001 << moduleID)
#define MID_BIT_ALL				(~(0xFFFF << MID_NUM))
#define MID_DEV_MDL_BIT_ALL		(~(0xFFFF << MID_NUM) ^ (MID_CNV_BIT(MID_NON)|MID_CNV_BIT(SID_IOT)|MID_CNV_BIT(MID_FRAME)|MID_CNV_BIT(MID_SYS)))

// --- Option ID ---
#include "EOptionID.h"

#define OPTID_CNV_BIT(optionID)	(0x0001 << optionID)

// --- Device ID ---
#include "EDeviceID.h"

#define DEVID_CNV_BIT(deviceID)	(0x0001 << deviceID)

// --- Device Condition(MSP_MessageType.hにて定義) ---
//typedef enum /*EDeviceCondition*/
//{
//	DEV_COND_OK,						// Condition is OK.
//	DEV_COND_TEMP_NG,					// Temporary NG (Matter isn't subsytem & local fail.)
//	DEV_COND_PERM_NG,					// Permanent NG (Matter is subsytem or local fail.)
//	DEV_COND_NUM						// Number of Device Condition
//} EDeviceCondition;

// --- Device Status No. ---
#include "EDeviceStatusNo.h"
// --- Device Status Value ---
#include "DeviceStatusValue.h"

// --- Fault Status ---
typedef enum /*EFaultStatus*/
{
	FAULT_NOT_OCCUR = 0,				// Not occur
	FAULT_OCCUR		= 1,				// Occur
	FAULT_STTS_NUM
} EFaultStatus;

// --- Action ---
typedef enum /*EAction*/
{
	ACT_STOP  = 0,						// Stop
	ACT_START = 1,						// Start
	ACT_NUM
} EAction;

// --- Result ---
typedef enum /*EResult*/
{
	RESULT_OK = 0,						// OK
	RESULT_NG = 1,						// NG
	RESULT_NUM
} EResult;


////////////////////////////////////////////////////////////
// System Service
////////////////////////////////////////////////////////////

#include "ConfigurationService.h"
#include "ConditionService.h"
#include "SheetService.h"
#include "NvmService.h"
#include "SRVC_Moview.h"


#endif // #ifndef FinisherCommon_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.02.04	| Yuichi Hoshino	| Created this file
// 14.12.05	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.03.30	| Yuichi Hoshino	| Result Define追加
// 15.03.30 | Yuichi Hoshino	| NVMサービスインクルード追加
// 15.03.30 | Yuichi Hoshino	| テンポラリパラメータインクルード追加
// 15.03.30 | Yuichi Hoshino	| Moview APIインクルード追加
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
