// File Name:	CCfgDevContConfiguration.h
// Description:	Header File of Device Controller Configuration
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CCfgDevContConfiguration_h
#define CCfgDevContConfiguration_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyDevCont.h"
#include "ECdiFinMarket.h"
#include "ECdiFinSystemCfg.h"
#include "ECdiFinSizeGroup.h"
#include "ECdiFinSelect8K16K.h"
#include "ECdiFinExec.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Spec Data of Device Controller Configuration ---
typedef struct /*CfgDevContConfigurationSpec*/
{
	UC instanceID;											// Instance ID
} CfgDevContConfigurationSpec;

// --- Member of Device Controller Configuration ---
typedef struct /*CCfgDevContConfiguration*/
{
	const CfgDevContConfigurationSpec* mpSpec;

	US mYear;												// Set Date Time:Year
	UC mMonth;												// Set Date Time:Month
	UC mDay;												// Set Date Time:Day
	UC mHour;												// Set Date Time:Hour
	UC mMinute;												// Set Date Time:Minute
	UC mSecond;												// Set Date Time:Second
	US mSheetExitHeight;									// Sheet Exit Height
	US minExitSpeed;										// Exit Speed Range:Minimum Exit Speed
	US maxExitSpeed;										// Exit Speed Range:Maximum Exit Speed
	US hotLineDistance;										// Sheet Exit Layout:Exit Hot Line Distance
	US releasePointDistance;								// Sheet Exit Layout:Release Point Distance
	UC mProductivity;
	// --- ˆÈ‰ºAŽæ‚è‚¤‚é’l‚ªBitŠ„‚è•t‚¯‚ÅÏ‚Þ‚à‚Ì ---
	UC mMarketRegion					:3;
	UC mSelect8K16K						:1;
	UC mSysConfig						:3;
	UC mMediaSizeGroup					:3;
	UC mJamBypass						:1;
	UC mNoPaperRun						:1;
} CCfgDevContConfiguration;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCfgDevContConfiguration_Constructor(CCfgDevContConfiguration* pThis, const CfgDevContConfigurationSpec* pSpec);

void CCfgDevContConfiguration_MakeInformation(CCfgDevContConfiguration* pThis);
enum ECdiFinCfgAttrKeyDevCont CCfgDevContConfiguration_UpdateSelectability(CCfgDevContConfiguration* pThis, EDeviceID deviceID, EDeviceCondition condition);

#define CCfgDevContConfiguration_GetInstanceID(pThis) ((enum ECdiFinDevCont)((pThis)->mpSpec->instanceID))
																				// ECdiFinDevCont (CCfgDevContConfiguration*)
#define CCfgDevContConfiguration_GetRelativeOptionID(pThis) ((EOptionID)((pThis)->mpSpec->relativeOptionID))
																				// EOptionID (CCfgDevContConfiguration*)

#define CCfgDevContConfiguration_GetMarketRegion(pThis) ((pThis)->mMarketRegion)// UC (CCfgDevContConfiguration*)
#define CCfgDevContConfiguration_SetMarketRegion(pThis, marketRegion) ((pThis)->mMarketRegion = marketRegion)
																				// void (CCfgDevContConfiguration*, UC)
#define CCfgDevContConfiguration_GetSetDateYear(pThis) ((pThis)->mYear)			// US (CCfgDevContConfiguration*)
#define CCfgDevContConfiguration_SetSetDateYear(pThis, year) ((pThis)->mYear = year)
																				// void (CCfgDevContConfiguration*, SetDateTime)
#define CCfgDevContConfiguration_GetSetDateMonth(pThis) ((pThis)->mMonth)		// UC (CCfgDevContConfiguration*)
#define CCfgDevContConfiguration_SetSetDateMonth(pThis, month) ((pThis)->mMonth = month)
																				// void (CCfgDevContConfiguration*, SetDateTime)
#define CCfgDevContConfiguration_GetSetDateDay(pThis) ((pThis)->mDay)			// UC (CCfgDevContConfiguration*)
#define CCfgDevContConfiguration_SetSetDateDay(pThis, day) ((pThis)->mDay = day)// void (CCfgDevContConfiguration*, SetDateTime)
#define CCfgDevContConfiguration_GetSetDateHour(pThis) ((pThis)->mHour)			// UC (CCfgDevContConfiguration*)
#define CCfgDevContConfiguration_SetSetDateHour(pThis, hour) ((pThis)->mHour = hour)
																				// void (CCfgDevContConfiguration*, SetDateTime)
#define CCfgDevContConfiguration_GetSetDateMinute(pThis) ((pThis)->mMinute)		// UC (CCfgDevContConfiguration*)
#define CCfgDevContConfiguration_SetSetDateMinute(pThis, minute) ((pThis)->mMinute = minute)
																				// void (CCfgDevContConfiguration*, SetDateTime)
#define CCfgDevContConfiguration_GetSetDateSecond(pThis) ((pThis)->mSecond)		// UC (CCfgDevContConfiguration*)
#define CCfgDevContConfiguration_SetSetDateSecond(pThis, second) ((pThis)->mSecond = second)
																				// void (CCfgDevContConfiguration*, SetDateTime)
#define CCfgDevContConfiguration_GetSystemConfiguration(pThis) ((pThis)->mSysConfig)
																				// UC (CCfgDevContConfiguration*)
#define CCfgDevContConfiguration_SetSystemConfiguration(pThis, config) ((pThis)->mSysConfig = config)
																				// void (CCfgDevContConfiguration*, UC)
#define CCfgDevContConfiguration_GetMediaSizeGroup(pThis) ((pThis)->mMediaSizeGroup)
																				// UC (CCfgDevContConfiguration*)
#define CCfgDevContConfiguration_SetMediaSizeGroup(pThis, group) ((pThis)->mMediaSizeGroup = group)
																				// void (CCfgDevContConfiguration*, UC)
#define CCfgDevContConfiguration_GetSelect8K16K(pThis) ((pThis)->mSelect8K16K)	// UC (CCfgDevContConfiguration*)
#define CCfgDevContConfiguration_SetSelect8K16K(pThis, select) ((pThis)->mSelect8K16K = select)
																				// void (CCfgDevContConfiguration*, UC)
#define CCfgDevContConfiguration_GetSheetExitHeight(pThis) ((US)(pThis)->mSheetExitHeight)
																				// US (CCfgDevContConfiguration*)
#define CCfgDevContConfiguration_SetSheetExitHeight(pThis, height) ((pThis)->mSheetExitHeight = height)
																				// void (CCfgDevContConfiguration*, US)
#define CCfgDevContConfiguration_GetProductivity(pThis) ((UC)(pThis)->mProductivity)
																				// UC (CCfgDevContConfiguration*)
#define CCfgDevContConfiguration_SetProductivity(pThis, productivity) ((pThis)->mProductivity = productivity)
																				// void (CCfgDevContConfiguration*, UC)
#define CCfgDevContConfiguration_GetMinimumExitSpeed(pThis) ((pThis)->minExitSpeed)
																				// US (CCfgDevContConfiguration*)
#define CCfgDevContConfiguration_SetMinimumExitSpeed(pThis, exitSpd) ((pThis)->minExitSpeed = exitSpd)
																				// void (CCfgDevContConfiguration*, ExitSpeedRange)
#define CCfgDevContConfiguration_GetMaximumExitSpeed(pThis) ((pThis)->maxExitSpeed)
																				// US (CCfgDevContConfiguration*)
#define CCfgDevContConfiguration_SetMaximumExitSpeed(pThis, exitSpd) ((pThis)->maxExitSpeed = exitSpd)
																				// void (CCfgDevContConfiguration*, ExitSpeedRange)
#define CCfgDevContConfiguration_GetHotLineDistance(pThis) ((pThis)->hotLineDistance)
																				// US (CCfgDevContConfiguration*)
#define CCfgDevContConfiguration_SetHotLineDistance(pThis, distance) ((pThis)->hotLineDistance = distance)
																				// void (CCfgDevContConfiguration*, US)
#define CCfgDevContConfiguration_GetReleasePointDistance(pThis) ((pThis)->releasePointDistance)
																				// US (CCfgDevContConfiguration*)
#define CCfgDevContConfiguration_SetReleasePointDistance(pThis, distance) ((pThis)->releasePointDistance = distance)
																				// void (CCfgDevContConfiguration*, US)
#define CCfgDevContConfiguration_GetJamBypass(pThis) ((pThis)->mJamBypass)		// UC (CCfgDevContConfiguration*)
#define CCfgDevContConfiguration_SetJamBypass(pThis, jamBypass) ((pThis)->mJamBypass = jamBypass)
																				// void (CCfgDevContConfiguration*, UC)
#define CCfgDevContConfiguration_GetNoPaperRun(pThis) ((pThis)->mNoPaperRun)	// UC (CCfgDevContConfiguration*)
#define CCfgDevContConfiguration_SetNoPaperRun(pThis, noPprRun) ((pThis)->mNoPaperRun = noPprRun)
																				// void (CCfgDevContConfiguration*, UC)


#endif // #ifndef CCfgDevContConfiguration_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

