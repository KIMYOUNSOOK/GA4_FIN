// File Name:	CCfgInSrcConfiguration.h
// Description:	Header File of Input Source Configuration
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.17
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CCfgInSrcConfiguration_h
#define CCfgInSrcConfiguration_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinCfgAttrKeyInSrc.h"
#include "ECdiSelectability.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

struct CfgInSrcConfigurationOverrideMethod;

// --- Spec Data of Input Source Configuration ---
typedef struct /*CfgInSrcConfigurationSpec*/
{
	const struct CfgInSrcConfigurationOverrideMethod *pMethod;					// Pointer of method table
	UC instanceID;																// Instance ID
	UC relativeOptionID;														// Relative Option ID
} CfgInSrcConfigurationSpec;

// --- Member of Input Source Configuration ---
typedef struct /*CfgInSrcConfigurationSpec*/
{
	const CfgInSrcConfigurationSpec* mpSpec;

	US IrregularProcSize;
	US IrregularCrossProcSize;
	// --- ˆÈ‰ºAŽæ‚è‚¤‚é’l‚ªBitŠ„‚è•t‚¯‚ÅÏ‚Þ‚à‚Ì ---
	UC selectability					:1;
	UC mediaSizeAlt1					:2;
	UC mediaSizeAlt2					:2;
	UC mediaSizeAlt3					:2;
	UC mediaSizeAlt4					:2;
	UC mediaSizeAlt5					:2;
} CCfgInSrcConfiguration;

// --- Override Function Table ---
struct CfgInSrcConfigurationOverrideMethod
{
	void (*MakeInformation)(CCfgInSrcConfiguration* pThis);
	enum ECdiFinCfgAttrKeyInSrc (*UpdateSelectability)(CCfgInSrcConfiguration* pThis, EDeviceID deviceID, EDeviceCondition condition);
};

// --- Standard Override Method Table of Output Destination Capabilities ---
extern struct CfgInSrcConfigurationOverrideMethod const cCfg_InSrcConfigurationMethodStandard;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCfgInSrcConfiguration_Constructor(CCfgInSrcConfiguration* pThis, const CfgInSrcConfigurationSpec* pSpec);

// --- Method of Super Class ---
void CCfgInSrcConfiguration_Super_MakeInformation(CCfgInSrcConfiguration* pThis);
enum ECdiFinCfgAttrKeyInSrc CCfgInSrcConfiguration_Super_UpdateSelectability(CCfgInSrcConfiguration* pThis, EDeviceID deviceID, EDeviceCondition condition);

// --- I/F Function Prototype Definition ---
#define CCfgInSrcConfiguration_MakeInformation(apThis) ((apThis)->mpSpec->pMethod->MakeInformation(apThis))
																				// void (CCfgInSrcConfiguration*)
#define CCfgInSrcConfiguration_UpdateSelectability(apThis, aDeviceID, aCondition) ((apThis)->mpSpec->pMethod->UpdateSelectability(apThis, aDeviceID, aCondition))
																				// enum ECdiFinCfgAttrKeyInSrc (CCfgInSrcConfiguration*, EDeviceID, EDeviceCondition)
#define CCfgInSrcConfiguration_GetInstanceID(pThis) ((pThis)->mpSpec->instanceID)
																				// enum ECdiFinInSrc (CCfgInSrcConfiguration*)
#define CCfgInSrcConfiguration_GetRelativeOptionID(pThis) ((EOptionID)((pThis)->mpSpec->relativeOptionID))
																				// EOptionID (CCfgInSrcConfiguration*)

#define CCfgInSrcConfiguration_GetSelectability(pThis) ((pThis)->selectability)	// UC (CCfgInSrcConfiguration*)
#define CCfgInSrcConfiguration_SetSelectability(pThis, select) ((pThis)->selectability = select)
																				// void (CCfgInSrcConfiguration*, UC)
#define CCfgInSrcConfiguration_GetSizeAlternate1(pThis) ((pThis)->mediaSizeAlt1)// UC (CCfgInSrcConfiguration*)
#define CCfgInSrcConfiguration_SetSizeAlternate1(pThis, alt) ((pThis)->mediaSizeAlt1 = alt)
																				// void (CCfgInSrcConfiguration*, UC)
#define CCfgInSrcConfiguration_GetSizeAlternate2(pThis) ((pThis)->mediaSizeAlt2)// UC (CCfgInSrcConfiguration*)
#define CCfgInSrcConfiguration_SetSizeAlternate2(pThis, alt) ((pThis)->mediaSizeAlt2 = alt)
																				// void (CCfgInSrcConfiguration*, UC)
#define CCfgInSrcConfiguration_GetSizeAlternate3(pThis) ((pThis)->mediaSizeAlt3)// UC (CCfgInSrcConfiguration*)
#define CCfgInSrcConfiguration_SetSizeAlternate3(pThis, alt) ((pThis)->mediaSizeAlt3 = alt)
																				// void (CCfgInSrcConfiguration*, UC)
#define CCfgInSrcConfiguration_GetSizeAlternate4(pThis) ((pThis)->mediaSizeAlt4)// UC (CCfgInSrcConfiguration*)
#define CCfgInSrcConfiguration_SetSizeAlternate4(pThis, alt) ((pThis)->mediaSizeAlt4 = alt)
																				// void (CCfgInSrcConfiguration*, UC)
#define CCfgInSrcConfiguration_GetSizeAlternate5(pThis) ((pThis)->mediaSizeAlt5)// UC (CCfgInSrcConfiguration*)
#define CCfgInSrcConfiguration_SetSizeAlternate5(pThis, alt) ((pThis)->mediaSizeAlt5 = alt)
																				// void (CCfgInSrcConfiguration*, UC)
#define CCfgInSrcConfiguration_GetIrregularProcessSize(pThis) ((pThis)->IrregularProcSize)
																				// UC (CCfgInSrcConfiguration*)
#define CCfgInSrcConfiguration_SetIrregularProcSize(pThis, size) ((pThis)->IrregularProcSize = size)
																				// void (CCfgInSrcConfiguration*, US)
#define CCfgInSrcConfiguration_GetIrregularCrossProcSize(pThis) ((pThis)->IrregularCrossProcSize)
																				// UC (CCfgInSrcConfiguration*)
#define CCfgInSrcConfiguration_SetIrregularCrossProcSize(pThis, size) ((pThis)->IrregularCrossProcSize = size)
																				// void (CCfgInSrcConfiguration*, US)


#endif // #ifndef CCfgInSrcConfiguration_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

