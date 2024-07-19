// File Name:	CCapaOutDestCapabilitiesOutTray.c
// Description:	Implimentation File of Output Destination Capabilities
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.18
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCapaOutDestCapabilities.h"

#include "COputInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

UC GetMaxNumberSheetsPerStaple(CCapaOutDestCapabilities* pThis, ECapaSizeCategory category);
enum ECdiFinSupport GetPunchingSelectability(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport GetDoublePunchSupport(CCapaOutDestCapabilities* pThis);


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Standard Oveerride Method Table of Output Destination Capabilities ---
struct CapaOutDestCapabilitiesOverrideMethod const cCapa_OutDestCapabilitiesMethodOutTray = {
	CCapaOutDestCapabilities_Constructor_Super,
	CCapaOutDestCapabilities_GetInstanceID_Super,
	CCapaOutDestCapabilities_GetRelativeOptionID_Super,
	CCapaOutDestCapabilities_GetOutputMaxCapacity_Super,
	CCapaOutDestCapabilities_GetDeviceUnitType_Super,
	CCapaOutDestCapabilities_GetPurgeOutput_Super,
	CCapaOutDestCapabilities_GetVirtualOutput_Super,
	CCapaOutDestCapabilities_GetOffsetSelectability_Super,
	CCapaOutDestCapabilities_Get3PositionOffsetSupport_Super,
	CCapaOutDestCapabilities_GetOffsetMethod_Super,
	CCapaOutDestCapabilities_GetFoldingSelectability_Super,
	CCapaOutDestCapabilities_GetZFoldSupport_Super,
	CCapaOutDestCapabilities_GetTriFoldCSupport_Super,
	CCapaOutDestCapabilities_GetTriFoldZSupport_Super,
	CCapaOutDestCapabilities_GetSaddleFoldSupport_Super,
	CCapaOutDestCapabilities_GetStaplingSelectability_Super,
	CCapaOutDestCapabilities_GetSingleStitchSupport_Super,
	CCapaOutDestCapabilities_GetDoubleStitchSupport_Super,
	CCapaOutDestCapabilities_GetTripleStitchSupport_Super,
	CCapaOutDestCapabilities_GetQuadStitchSupport_Super,
	CCapaOutDestCapabilities_GetSingleRearStitchSupport_Super,
	CCapaOutDestCapabilities_GetSingleFrontStitchSupport_Super,
	CCapaOutDestCapabilities_GetPinCutSupport_Super,
	CCapaOutDestCapabilities_GetMinNumberSheetsPerStaple_Super,
	GetMaxNumberSheetsPerStaple,
	CCapaOutDestCapabilities_GetStapledSetCapacity_Super,
	GetPunchingSelectability,
	GetDoublePunchSupport,
	CCapaOutDestCapabilities_GetTriplePunchSupport_Super,
	CCapaOutDestCapabilities_GetQuadPunchSupport_Super,
	CCapaOutDestCapabilities_GetMultiPunchSupport_Super,
	CCapaOutDestCapabilities_GetDoublePunchType_Super,
	CCapaOutDestCapabilities_GetQuadPunchType_Super,
	CCapaOutDestCapabilities_GetMultiPunchType_Super,
	CCapaOutDestCapabilities_GetFoldingBookletSelectability_Super,
	CCapaOutDestCapabilities_GetSaddleFoldStapleSupport_Super,
	CCapaOutDestCapabilities_GetSaddleFoldNoStapleSupport_Super,
	CCapaOutDestCapabilities_GetBookletCoverOrder_Super,
	CCapaOutDestCapabilities_GetSquareFoldingSupport_Super,
	CCapaOutDestCapabilities_GetMinNumberSheetsSaddleFoldStaple_Super,
	CCapaOutDestCapabilities_GetMaxNumberSheetsSaddleFoldStaple_Super,
	CCapaOutDestCapabilities_GetBookletTrayCapacitySaddleFoldStaple_Super,
	CCapaOutDestCapabilities_GetMinNumberSheetsSaddleFoldNoStaple_Super,
	CCapaOutDestCapabilities_GetMaxNumberSheetsSaddleFoldNoStaple_Super,
	CCapaOutDestCapabilities_GetBookletTrayCapacitySaddleFoldNoStaple_Super,
	CCapaOutDestCapabilities_GetBinsNumber_Super,
	CCapaOutDestCapabilities_GetZFoldSheetCountLong_Super,
	CCapaOutDestCapabilities_GetZFoldSheetCountShort_Super,
	CCapaOutDestCapabilities_GetStapleType_Super,
	CCapaOutDestCapabilities_GetBookletStapleType_Super,
	CCapaOutDestCapabilities_GetStaplingMediaType_Super,
	CCapaOutDestCapabilities_GetSaddleFoldStapleMediaType_Super
};


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : GetMaxNumberSheetsPerStaple
// Description   : 
// Parameter     : Å]
// Return        : Å]
///////////////////////////////////////////////////////////////////
UC GetMaxNumberSheetsPerStaple(CCapaOutDestCapabilities* pThis, ECapaSizeCategory category)
{
	switch ( category ) {
	case CAPA_SIZE_CTGRY_STANDARD:
		return 50;
	case CAPA_SIZE_CTGRY_SMALL:
		// if( COputInformer_GetOptionType(OPTID_STAPLER) == 0 ) return nCmpl_Max_Sheet_Count_For_Staple_50;
		// else												   return nCmpl_Max_Sheet_Count_For_Staple_65;

	case CAPA_SIZE_CTGRY_LARGE:
		// if( COputInformer_GetOptionType(OPTID_STAPLER) == 0 ) return nCmpl_Max_Sheet_Count_For_Staple_50;
		// else												   return nCmpl_Max_Sheet_Count_For_Staple_65;

	default:
		return 0;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : GetPunchingSelectability
// Description   : 
// Parameter     : Å]
// Return        : Å]
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport GetPunchingSelectability(CCapaOutDestCapabilities* pThis)
{
	// if( COputInformer_GetOptionDetect(OPTID_PUNCHER) == true ) return eCdiFin_Support;
	// else														return eCdiFin_NotSupport;
	return eCdiFin_NotSupport;
}

///////////////////////////////////////////////////////////////////
// Function Name : GetDoublePunchSupport
// Description   : 
// Parameter     : Å]
// Return        : Å]
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport GetDoublePunchSupport(CCapaOutDestCapabilities* pThis)
{
	// if( COputInformer_GetOptionDetect(OPTID_PUNCHER) == true ) return eCdiFin_Support;
	// else														return eCdiFin_NotSupport;
	return eCdiFin_NotSupport;
}





// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
