// File Name:	CShtSheetAttribute.h
// Description:	Header File of Sheet Attribute
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.10
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CShtSheetAttribute_h
#define CShtSheetAttribute_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

typedef struct /*CShtSheetAttribute*/
{
	UC mOffsetMode;						// Offset Mode
	UC mStapleMode;						// Staple Mode
	UC mPunchMode;						// Punch Mode
	UC mBookletMode;					// Booklet Maker Mode
	UC mFoldMode;						// Fold Mode
	UC mMediaType;						// Media Type
	US mMediaWeight;					// Media Weight[x1gsm]
	UC mMediaInfo;						// Bit0：Envelope
										// Bit1：Tabbed Paper
										// Bit2：Reused Paper
										// Bit3：Prepunched Material
	US mProcessSize;					// Process Direction Media Size[x0.1mm]
	US mCrossProcessSize;				// Cross Process Direction Media Size[x0.1mm]
	UC mCurl;							// Sheet Curl
	UC mCurlQuantity;					// Sheet Curl Quantity
	US mInitialSpeed;					// Initial Sheet Exit Speed[x0.1mm/s]
	UC mSpeedChangeTimes;				// Speed Change Times
	US mChangeSpeedTmg[5];				// Change Speed Timing *
	US mChangeSpeed[5];					// Change Speed *
	UC mSheetIntegrity;					// Sheet Integrity
	UC mSetAction;						// Set Action
	UC mOffsetPosition;					// Offset Position
	US mExitSheetInterval;				// Exit Sheet Interval[x1ms]
} CShtSheetAttribute;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CShtSheetAttribute_Constructor(CShtSheetAttribute* pThis);

void CShtSheetAttribute_Reset(CShtSheetAttribute* pThis);
bool CShtSheetAttribute_CheckDiff(CShtSheetAttribute* pThis, CShtSheetAttribute* pAttr);
bool CShtSheetAttribute_CheckDiffOutSheet(CShtSheetAttribute* pThis, CShtSheetAttribute* pAttr);

#define CShtSheetAttribute_GetOffsetMode(apThis) ((UC)((apThis)->mOffsetMode))	// UC (CShtSheetAttribute*)
#define CShtSheetAttribute_SetOffsetMode(apThis, aMode) ((apThis)->mOffsetMode = aMode)
																				// void (CShtSheetAttribute*, UC)
#define CShtSheetAttribute_GetStapleMode(apThis) ((UC)((apThis)->mStapleMode))	// UC (CShtSheetAttribute*)
#define CShtSheetAttribute_SetStapleMode(apThis, aMode) ((apThis)->mStapleMode = aMode)
																				// void (CShtSheetAttribute*, UC)
#define CShtSheetAttribute_GetPunchMode(apThis) ((UC)((apThis)->mPunchMode))	// UC (CShtSheetAttribute*)
#define CShtSheetAttribute_SetPunchMode(apThis, aMode) ((apThis)->mPunchMode = aMode)
																				// void (CShtSheetAttribute*, UC)
#define CShtSheetAttribute_GetBookletMakerMode(apThis) ((UC)((apThis)->mBookletMode))
																				// UC (CShtSheetAttribute*)
#define CShtSheetAttribute_SetBookletMakerMode(apThis, aMode) ((apThis)->mBookletMode = aMode)
																				// void (CShtSheetAttribute*, UC)
#define CShtSheetAttribute_GetFoldMode(apThis) ((UC)((apThis)->mFoldMode))		// UC (CShtSheetAttribute*)
#define CShtSheetAttribute_SetFoldMode(apThis, aMode) ((apThis)->mFoldMode = aMode)
																				// void (CShtSheetAttribute*, UC)
#define CShtSheetAttribute_GetMediaType(apThis) ((UC)((apThis)->mMediaType))	// UC (CShtSheetAttribute*)
#define CShtSheetAttribute_SetMediaType(apThis, aType) ((apThis)->mMediaType = aType)
																				// void (CShtSheetAttribute*, UC)
#define CShtSheetAttribute_GetMediaWeight(apThis) ((US)((apThis)->mMediaWeight))// US (CShtSheetAttribute*)
#define CShtSheetAttribute_SetMediaWeight(apThis, aWeight) ((apThis)->mMediaWeight = aWeight)
																				// void (CShtSheetAttribute*, US)
#define CShtSheetAttribute_GetEnvelope(apThis) ((UC)(((apThis)->mMediaInfo >> 0) & 0x01))
																				// UC (CShtSheetAttribute*)
#define CShtSheetAttribute_SetEnvelope(apThis, aEnvelope) ((apThis)->mMediaInfo = ((apThis)->mMediaInfo & ~(0x01 << 0)) | ((aEnvelope & 0x01) << 0))
																				// void (CShtSheetAttribute*, UC)
#define CShtSheetAttribute_GetTabbedPaper(apThis) ((UC)(((apThis)->mMediaInfo >> 1) & 0x01))
																				// UC (CShtSheetAttribute*)
#define CShtSheetAttribute_SetTabbedPaper(apThis, aTabbedPpr) ((apThis)->mMediaInfo = ((apThis)->mMediaInfo & ~(0x01 << 1)) | ((aTabbedPpr & 0x01) << 1))
																				// void (CShtSheetAttribute*, UC)
#define CShtSheetAttribute_GetReusedPaper(apThis) ((UC)(((apThis)->mMediaInfo >> 2) & 0x01))
																				// UC (CShtSheetAttribute*)
#define CShtSheetAttribute_SetReusedPaper(apThis, aReusedPpr) ((apThis)->mMediaInfo = ((apThis)->mMediaInfo & ~(0x01 << 2)) | ((aReusedPpr & 0x01) << 2))
																				// void (CShtSheetAttribute*, UC)
#define CShtSheetAttribute_GetPrepunchedMaterial(apThis) ((UC)(((apThis)->mMediaInfo >> 3) & 0x01))
																				// UC (CShtSheetAttribute*)
#define CShtSheetAttribute_SetPrepunchedMaterial(apThis, aPrepunchMaterial) ((apThis)->mMediaInfo = ((apThis)->mMediaInfo & ~(0x01 << 3)) | ((aPrepunchMaterial & 0x01) << 3))
																				// void (CShtSheetAttribute*, UC)
#define CShtSheetAttribute_GetProcessSize(apThis) ((US)((apThis)->mProcessSize))// US (CShtSheetAttribute*)
#define CShtSheetAttribute_SetProcessSize(apThis, aSize) ((apThis)->mProcessSize = aSize)
																				// void (CShtSheetAttribute*, US)
#define CShtSheetAttribute_GetCrossProcSize(apThis) ((US)((apThis)->mCrossProcessSize))
																				// US (CShtSheetAttribute*)
#define CShtSheetAttribute_SetCrossProcSize(apThis, aSize) ((apThis)->mCrossProcessSize = aSize)
																				// void (CShtSheetAttribute*, US)
#define CShtSheetAttribute_GetCurl(apThis) ((UC)((apThis)->mCurl))				// UC (CShtSheetAttribute*)
#define CShtSheetAttribute_SetCurl(apThis, aCurl) ((apThis)->mCurl = aCurl)		// void (CShtSheetAttribute*, UC)
#define CShtSheetAttribute_GetCurlQuantity(apThis) ((UC)((apThis)->mCurlQuantity))
																				// UC (CShtSheetAttribute*)
#define CShtSheetAttribute_SetCurlQuantity(apThis, aQuantity) ((apThis)->mCurlQuantity = aQuantity)
																				// void (CShtSheetAttribute*, UC)
#define CShtSheetAttribute_GetInitialSpeed(apThis) ((US)((apThis)->mInitialSpeed))
																				// US (CShtSheetAttribute*)
#define CShtSheetAttribute_SetInitialSpeed(apThis, aSpeed) ((apThis)->mInitialSpeed = aSpeed)
																				// void (CShtSheetAttribute*, US)
#define CShtSheetAttribute_GetSpeedChangeTimes(apThis) ((UC)((apThis)->mSpeedChangeTimes))
																				// UC (CShtSheetAttribute*)
#define CShtSheetAttribute_SetSpeedChangeTimes(apThis, aTimes) ((apThis)->mSpeedChangeTimes = aTimes)
																				// void (CShtSheetAttribute*, UC)

US CShtSheetAttribute_GetChangeSpeedTiming(CShtSheetAttribute* pThis, UC times);
void CShtSheetAttribute_SetChangeSpeedTiming(CShtSheetAttribute* pThis, US aTmg, UC times);
US CShtSheetAttribute_GetChangeSpeed(CShtSheetAttribute* pThis, UC times);
void CShtSheetAttribute_SetChangeSpeed(CShtSheetAttribute* pThis, US aSpd, UC times);

#define CShtSheetAttribute_GetSheetIntegrity(apThis) ((UC)((apThis)->mSheetIntegrity))
																				// UC (struct CShtSheetAttribute*)
#define CShtSheetAttribute_SetSheetIntegrity(apThis, aIntegrity) ((apThis)->mSheetIntegrity = aIntegrity)
																				// void (struct CShtSheetAttribute*, UC)
#define CShtSheetAttribute_GetSetAction(apThis) ((UC)((apThis)->mSetAction))	// UC (struct CShtSheetAttribute*)
#define CShtSheetAttribute_SetSetAction(apThis, aSetAction) ((apThis)->mSetAction = aSetAction)
																				// void (struct CShtSheetAttribute*, UC)
#define CShtSheetAttribute_GetOffsetPosition(apThis) ((UC)((apThis)->mOffsetPosition))
																				// UC (struct CShtSheetAttribute*)
#define CShtSheetAttribute_SetOffsetPosition(apThis, aPosition) ((apThis)->mOffsetPosition = aPosition)
																				// void (struct CShtSheetAttribute*, UC)
#define CShtSheetAttribute_GetExitSheetInterval(apThis) ((US)((apThis)->mExitSheetInterval))
																				// US (struct CShtSheetAttribute*)
#define CShtSheetAttribute_SetExitSheetInterval(apThis, aInterval) ((apThis)->mExitSheetInterval = aInterval)
																				// void (struct CShtSheetAttribute*, US)


/////////////////////////////////////////////////////////////////////////////////////////
// Function Table
/////////////////////////////////////////////////////////////////////////////////////////


#endif // #ifndef CShtSheetAttribute_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
// 15.10.19	| Yuichi Hoshino	| Trouble Action [ 封筒が一度搬送されるとフィニッシングプロセスが止まるまで後続紙を封筒と認識して制御してしまう ]
