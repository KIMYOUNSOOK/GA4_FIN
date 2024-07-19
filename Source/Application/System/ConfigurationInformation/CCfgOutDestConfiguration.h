// File Name:	CCfgOutDestConfiguration.h
// Description:	Header File of Output Destination Configuration
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CCfgOutDestConfiguration_h
#define CCfgOutDestConfiguration_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyOutDest.h"
#include "ECdiSelectability.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

struct CfgOutDestConfigurationOverrideMethod;

// --- Spec Data of Output Destination Configuration ---
typedef struct /*CfgOutDestConfigurationSpec*/
{
	const struct CfgOutDestConfigurationOverrideMethod *pMethod;
															// Pointer of method table
	UC instanceID;											// Instance ID
	UC relativeOptionID;									// Relative Option ID
} CfgOutDestConfigurationSpec;

// --- Member of Output Destination Configuration ---
typedef struct /*CCfgOutDestConfiguration*/
{
	const CfgOutDestConfigurationSpec* mpSpec;

	// --- ˆÈ‰ºAŽæ‚è‚¤‚é’l‚ªBitŠ„‚è•t‚¯‚ÅÏ‚Þ‚à‚Ì ---
	UC mSelectability					:1;
	UC mOffset							:1;
	UC mZFold							:1;
	UC mTriFoldZ						:1;
	UC mTriFoldC						:1;
	UC mSaddleFold						:1;
	UC mSingleStitch					:1;
	UC mDoubleStitch					:1;
	UC mTripleStitch					:1;
	UC mQuadStitch						:1;
	UC mSingleRear						:1;
	UC mSingleFront						:1;
	UC mDoublePunch						:1;
	UC mTriplePunch						:1;
	UC mQuadPunch						:1;
	UC mMultiPunch						:1;
	UC mSaddleFoldAndStaple				:1;
	UC mSaddleFoldAndNoStaple			:1;
	UC mSquareFolding					:1;
} CCfgOutDestConfiguration;

// --- Override Function Table ---
struct CfgOutDestConfigurationOverrideMethod
{
	void (*MakeInformation)(CCfgOutDestConfiguration* pThis);
	enum ECdiFinCfgAttrKeyOutDest (*UpdateSelectability)(CCfgOutDestConfiguration* pThis, EDeviceID deviceID, EDeviceCondition condition);
};

// --- Standard Override Method Table of Output Destination Capabilities ---
extern struct CfgOutDestConfigurationOverrideMethod const cCfg_OutDestConfigurationMethodStandard;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCfgOutDestConfiguration_Constructor(CCfgOutDestConfiguration* pThis, const CfgOutDestConfigurationSpec* pSpec);

// --- Method of Super Class ---
void CCfgOutDestConfiguration_MakeInformation_Super(CCfgOutDestConfiguration* pThis);
enum ECdiFinCfgAttrKeyOutDest CCfgOutDestConfiguration_UpdateSelectability_Super(CCfgOutDestConfiguration* pThis, EDeviceID deviceID, EDeviceCondition condition);

// --- I/F Function Prototype Definition ---
#define CCfgOutDestConfiguration_MakeInformation(apThis) ((apThis)->mpSpec->pMethod->MakeInformation(apThis))
																				// void (CCfgOutDestConfiguration*)
#define CCfgOutDestConfiguration_UpdateSelectability(apThis, aDeviceID, aCondition) ((apThis)->mpSpec->pMethod->UpdateSelectability(apThis, aDeviceID, aCondition))
																				// enum ECdiFinCfgAttrKeyOutDest (CCfgOutDestConfiguration*, EDeviceID, EDeviceCondition)
#define CCfgOutDestConfiguration_GetInstanceID(pThis) ((pThis)->mpSpec->instanceID)
																				// enum ECdiFinOutDest (CCfgOutDestConfiguration*)
#define CCfgOutDestConfiguration_GetRelativeOptionID(pThis) ((EOptionID)((pThis)->mpSpec->relativeOptionID))
																				// EOptionID (CCfgOutDestConfiguration*)

#define CCfgOutDestConfiguration_GetSelectability(pThis) ((pThis)->mSelectability)
																				// UC (CCfgOutDestConfiguration*)
#define CCfgOutDestConfiguration_SetSelectability(pThis, select) ((pThis)->mSelectability = select)
																				// void (CCfgOutDestConfiguration*, UC)
#define CCfgOutDestConfiguration_GetOffsetSelectability(pThis) ((pThis)->mOffset)
																				// UC (CCfgOutDestConfiguration*)
#define CCfgOutDestConfiguration_SetOffsetSelectability(pThis, select) ((pThis)->mOffset = select)
																				// void (CCfgOutDestConfiguration*, UC)
#define CCfgOutDestConfiguration_GetZFoldSelectability(pThis) ((pThis)->mZFold)	// UC (CCfgOutDestConfiguration*)
#define CCfgOutDestConfiguration_SetZFoldSelectability(pThis, select) ((pThis)->mZFold = select)
																				// void (CCfgOutDestConfiguration*, UC))
#define CCfgOutDestConfiguration_GetTriFoldZSelectability(pThis) ((pThis)->mTriFoldZ)
																				// UC (CCfgOutDestConfiguration*)
#define CCfgOutDestConfiguration_SetTriFoldZSelectability(pThis, select) ((pThis)->mTriFoldZ = select)
																				// void (CCfgOutDestConfiguration*, UC)
#define CCfgOutDestConfiguration_GetTriFoldCSelectability(pThis) ((pThis)->mTriFoldC)
																				// UC (CCfgOutDestConfiguration*)
#define CCfgOutDestConfiguration_SetTriFoldCSelectability(pThis, select) ((pThis)->mTriFoldC = select)
																				// void (CCfgOutDestConfiguration*, UC)
#define CCfgOutDestConfiguration_GetSaddleFoldSelectability(pThis) ((pThis)->mSaddleFold)
																				// UC (CCfgOutDestConfiguration*)
#define CCfgOutDestConfiguration_SetSaddleFoldSelectability(pThis, select) ((pThis)->mSaddleFold = select)
																				// void (CCfgOutDestConfiguration*, UC)
#define CCfgOutDestConfiguration_GetSingleStitchSelectability(pThis) ((pThis)->mSingleStitch)
																				// UC (CCfgOutDestConfiguration*)
#define CCfgOutDestConfiguration_SetSingleStitchSelectability(pThis, select) ((pThis)->mSingleStitch = select)
																				// void (CCfgOutDestConfiguration*, UC)
#define CCfgOutDestConfiguration_GetDoubleStitchSelectability(pThis) ((pThis)->mDoubleStitch)
																				// UC (CCfgOutDestConfiguration*)
#define CCfgOutDestConfiguration_SetDoubleStitchSelectability(pThis, select) ((pThis)->mDoubleStitch = select)
																				// void (CCfgOutDestConfiguration*, UC)
#define CCfgOutDestConfiguration_GetTripleStitchSelectability(pThis) ((pThis)->mTripleStitch)
																				// UC (CCfgOutDestConfiguration*)
#define CCfgOutDestConfiguration_SetTripleStitchSelectability(pThis, select) ((pThis)->mTripleStitch = select)
																				// void (CCfgOutDestConfiguration*, UC)
#define CCfgOutDestConfiguration_GetQuadStitchSelectability(pThis) ((pThis)->mQuadStitch)
																				// UC (CCfgOutDestConfiguration*)
#define CCfgOutDestConfiguration_SetQuadStitchSelectability(pThis, select) ((pThis)->mQuadStitch = select)
																				// void (CCfgOutDestConfiguration*, UC)
#define CCfgOutDestConfiguration_GetSingleRearSelectability(pThis) ((pThis)->mSingleRear)
																				// UC (CCfgOutDestConfiguration*)
#define CCfgOutDestConfiguration_SetSingleRearStitchSelectability(pThis, select) ((pThis)->mSingleRear = select)
																				// void (CCfgOutDestConfiguration*, UC)
#define CCfgOutDestConfiguration_GetSingleFrontSelectability(pThis) ((pThis)->mSingleFront)
																				// UC (CCfgOutDestConfiguration*)
#define CCfgOutDestConfiguration_SetSingleFrontStitchSelectability(pThis, select) ((pThis)->mSingleFront = select)
																				// void (CCfgOutDestConfiguration*, UC)
#define CCfgOutDestConfiguration_GetDoublePunchSelectability(pThis) ((pThis)->mDoublePunch)
																				// UC (CCfgOutDestConfiguration*)
#define CCfgOutDestConfiguration_SetDoublePunchSelectability(pThis, select) ((pThis)->mDoublePunch = select)
																				// void (CCfgOutDestConfiguration*, UC)
#define CCfgOutDestConfiguration_GetTriplePunchSelectability(pThis) ((pThis)->mTriplePunch)
																				// UC (CCfgOutDestConfiguration*)
#define CCfgOutDestConfiguration_SetTriplePunchSelectability(pThis, select) ((pThis)->mTriplePunch = select)
																				// void (CCfgOutDestConfiguration*, UC)
#define CCfgOutDestConfiguration_GetQuadPunchSelectability(pThis) ((pThis)->mQuadPunch)
																				// UC (CCfgOutDestConfiguration*)
#define CCfgOutDestConfiguration_SetQuadPunchSelectability(pThis, select) ((pThis)->mQuadPunch = select)
																				// void (CCfgOutDestConfiguration*, UC)
#define CCfgOutDestConfiguration_GetMultiPunchSelectability(pThis) ((pThis)->mMultiPunch)
																				// UC (CCfgOutDestConfiguration*)
#define CCfgOutDestConfiguration_SetMultiPunchSelectability(pThis, select) ((pThis)->mMultiPunch = select)
																				// void (CCfgOutDestConfiguration*, UC)
#define CCfgOutDestConfiguration_GetSaddleFoldAndStapleSelectability(pThis) ((pThis)->mSaddleFoldAndStaple)
																				// UC (CCfgOutDestConfiguration*)
#define CCfgOutDestConfiguration_SetSaddleFoldAndStapleSelectability(pThis, select) ((pThis)->mSaddleFoldAndStaple = select)
																				// void (CCfgOutDestConfiguration*, UC)
#define CCfgOutDestConfiguration_GetSaddleFoldAndNoStapleSelectability(pThis) ((pThis)->mSaddleFoldAndNoStaple)
																				// UC (CCfgOutDestConfiguration*)
#define CCfgOutDestConfiguration_SetSaddleFoldAndNoStapleSelectability(pThis, select) ((pThis)->mSaddleFoldAndNoStaple = select)
																				// void (CCfgOutDestConfiguration*, UC)
#define CCfgOutDestConfiguration_GetSquareFoldingSelectability(pThis) ((pThis)->mSquareFolding)
																				// UC (CCfgOutDestConfiguration*)
#define CCfgOutDestConfiguration_SetSquareFoldingSelectability(pThis, select) ((pThis)->mSquareFolding = select)
																				// void (CCfgOutDestConfiguration*, UC)


#endif // #ifndef CCfgOutDestConfiguration_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

