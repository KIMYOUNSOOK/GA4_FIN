// File Name:	CCapaDevContCapabilities.h
// Description:	Header File of Device Controller Capabilities
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.18
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CCapaDevContCapabilities_h
#define CCapaDevContCapabilities_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinDevCont.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Spec Data of Device Controller Capabilities ---
typedef struct /*CapaDevContCapabilitiesSpec*/
{
	UC instanceID;											// Instance ID

	UC majorVersion;										// Major Version
	UC minorVersion;										// Minor Version
	UC patchVersion;										// Patch Version
} CapaDevContCapabilitiesSpec;

// --- Member of Device Controller Capabilities ---
typedef struct /*CCapaDevContCapabilities*/
{
	const CapaDevContCapabilitiesSpec* mpSpec;

	// --- ˆÈ‰ºAŽæ‚è‚¤‚é’l‚ªBitŠ„‚è•t‚¯‚ÅÏ‚Þ‚à‚Ì ---

} CCapaDevContCapabilities;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCapaDevContCapabilities_Constructor(CCapaDevContCapabilities* pThis, const CapaDevContCapabilitiesSpec* pSpec);

#define CCapaDevContCapabilities_GetInstanceID(pThis) ((enum ECdiFinDevCont)((pThis)->mpSpec->instanceID))
																				// ECdiFinDevCont (CCapaDevContCapabilities*)

#define CCapaDevContCapabilities_GetMajorVersion(pThis) ((pThis)->mpSpec->majorVersion)
																				// UC (CCapaDevContCapabilities*)
#define CCapaDevContCapabilities_GetMinorVersion(pThis) ((pThis)->mpSpec->minorVersion)
																				// UC (CCapaDevContCapabilities*)
#define CCapaDevContCapabilities_GetPatchVersion(pThis) ((pThis)->mpSpec->patchVersion)
																				// UC (CCapaDevContCapabilities*)


#endif // #ifndef CCapaDevContCapabilities_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

