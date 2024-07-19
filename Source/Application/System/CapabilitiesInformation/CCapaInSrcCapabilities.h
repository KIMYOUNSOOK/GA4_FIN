// File Name:	CCapaInSrcCapabilities.h
// Description:	Header File of Input Source Capabilities
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.18
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CCapaInSrcCapabilities_h
#define CCapaInSrcCapabilities_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinInSrc.h"
#include "ECdiFinInUnitType.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Spec of Input Source Capabilities ---
typedef struct /*CapaInSrcCapabilitiesSpec*/
{
	UC instanceID;											// Instance ID
	UC relativeOptionID;									// Relative Option ID

	US inputMaxCapacity;									// Input Max Capacity
	UC deviceUnitType;										// Device Unit Type
} CapaInSrcCapabilitiesSpec;

// --- Instance of Input Source Capabilities ---
typedef struct /*CCapaInSrcCapabilities*/
{
	const CapaInSrcCapabilitiesSpec* mpSpec;

	// --- ˆÈ‰ºAŽæ‚è‚¤‚é’l‚ªBitŠ„‚è•t‚¯‚ÅÏ‚Þ‚à‚Ì ---

} CCapaInSrcCapabilities;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCapaInSrcCapabilities_Constructor(CCapaInSrcCapabilities* pThis, const CapaInSrcCapabilitiesSpec* pSpec);

#define CCapaInSrcCapabilities_GetInstanceID(pThis) ((pThis)->mpSpec->instanceID)
																				// enum ECdiFinInSrc (CCapaInSrcCapabilities*)
#define CCapaInSrcCapabilities_GetRelativeOptionID(pThis) ((EOptionID)((pThis)->mpSpec->relativeOptionID))
																				// EOptionID (CCapaInSrcCapabilities*)

#define CCapaInSrcCapabilities_GetInputMaxCapacity(pThis) ((pThis)->mpSpec->inputMaxCapacity)
																				// US (CCapaInSrcCapabilities*)
#define CCapaInSrcCapabilities_GetDeviceUnitType(pThis) ((pThis)->mpSpec->deviceUnitType)
																				// UC (CCapaInSrcCapabilities*)


#endif // #ifndef CCapaInSrcCapabilities_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

