// File Name:	CapabilitiesInformation.c
// Description:	Implimentation File of Capabilities Information
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.14
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCapaInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Spec data of Finsher Device Controller Capabilities ---
CapaDevContCapabilitiesSpec const cCapa_DevContCapabilitiesSpecAll = {			// AllÇÕDummy DataÇ≈Ç†ÇËÅAïœçXïsóv
	// !!! Product Specific !!!
	eCdiFinDevCont_All,
	0,
	0,
	0
};

CapaDevContCapabilitiesSpec const cCapa_DevContCapabilitiesSpecAries = {
	// !!! Product Specific !!!
	// eCdiFinDevCont_Aries,
	eCdiFinDevCont_GA4Fin,
	MAJOR_VERSION,
	MINOR_VERSION,
	PATCH_VERSION
};

extern struct CapaOutDestCapabilitiesOverrideMethod const cCapa_OutDestCapabilitiesMethodOutTray;


// --- Spec data of Output Destination Capabilities ---
CapaOutDestCapabilitiesSpec const cCapa_OutDestCapabilitiesSpecOutTray = {
	// !!! Product Specific !!!
	&cCapa_OutDestCapabilitiesMethodOutTray,				// Pointer of method table
//	&cCapa_StapleSpecificMediaType,
	11, 								// Number Of Specific Media Types		
	{														// Stapled Set Tray Capacity
		eCdiFinMediaType_Plain,													// ?????
		eCdiFinMediaType_Plain,
		eCdiFinMediaType_Plain,		
		eCdiFinMediaType_Plain,
		eCdiFinMediaType_Plain,
		eCdiFinMediaType_Plain,
		eCdiFinMediaType_Plain, //eCdiFinMediaType_Plain,
		eCdiFinMediaType_Coated, //eCdiFinMediaType_Coated,
		eCdiFinMediaType_Coated,
		eCdiFinMediaType_Coated,
		eCdiFinMediaType_Coated
	},

	{													// Media Weight Up1	
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0
	},

	{													// Media Weight Down1 
		80,
		90,
		105,
		128,
		150,
		176,
		220,
		128,
		150,
		176,
		220
	},
	{													// Size Category 1	
		CAPA_SIZE_CTGRY_STANDARD,
		CAPA_SIZE_CTGRY_STANDARD,
		CAPA_SIZE_CTGRY_STANDARD,
		CAPA_SIZE_CTGRY_STANDARD,
		CAPA_SIZE_CTGRY_STANDARD,
		CAPA_SIZE_CTGRY_STANDARD,
		CAPA_SIZE_CTGRY_STANDARD,
		CAPA_SIZE_CTGRY_STANDARD,
		CAPA_SIZE_CTGRY_STANDARD,
		CAPA_SIZE_CTGRY_STANDARD,
		CAPA_SIZE_CTGRY_STANDARD //CAPA_SIZE_CTGRY_STANDARD												
	},
	
	{													// Number Of Sheets 1  STAPLE_COUNT ??
		NVMID_MAX_SHEET_UNCOAT_80_STAPLE_50,				// NVMID_MAX_SHEET_UNCOAT_80_STAPLE_50
		NVMID_MAX_SHEET_UNCOAT_90_STAPLE_50,				// NVMID_MAX_SHEET_UNCOAT_90_STAPLE_50												
		NVMID_MAX_SHEET_UNCOAT_105_STAPLE_50,				// NVMID_MAX_SHEET_UNCOAT_105_STAPLE_50												
		NVMID_MAX_SHEET_UNCOAT_128_STAPLE_50,				// NVMID_MAX_SHEET_UNCOAT_128_STAPLE_50
		NVMID_MAX_SHEET_UNCOAT_150_STAPLE_50,				// NVMID_MAX_SHEET_UNCOAT_150_STAPLE_50
		NVMID_MAX_SHEET_UNCOAT_176_STAPLE_50,				// NVMID_MAX_SHEET_UNCOAT_176_STAPLE_50												
		NVMID_MAX_SHEET_UNCOAT_220_STAPLE_50,				// NVMID_MAX_SHEET_UNCOAT_220_STAPLE_50												
		NVMID_MAX_SHEET_COAT_128_STAPLE_50,					// NVMID_MAX_SHEET_COAT_128_STAPLE_50
		NVMID_MAX_SHEET_COAT_150_STAPLE_50,					// NVMID_MAX_SHEET_COAT_150_STAPLE_50
		NVMID_MAX_SHEET_COAT_176_STAPLE_50,					// NVMID_MAX_SHEET_COAT_176_STAPLE_50												
		NVMID_MAX_SHEET_COAT_220_STAPLE_50					// NVMID_MAX_SHEET_COAT_220_STAPLE_50												
	},

	eCdiFinOutDest_OutTray,									// Instance ID
	OPTID_BASE,												// Relative Option ID
	{														// Output Max Capacity
		0,
		500, //2015/09/01 LJE
		500,//2015/09/01 LJE
		250
	},
	eCdiFinOutUnitType_HighCap,								// Device Unit Type 2015/10/13 LJE
	eCdiFinPurge_Also,										// Purge
	eCdiFin_Physical,										// Virtual
	eCdiFin_Support,										// Offset Selectability
	eCdiFin_NotSupport,										// Three Position Offset
	eCdiFinOfstMthd_SelfOnly,								// Offset Method  //2015/09/01 LJE
	eCdiFin_NotSupport,										// Foding Selecatbility
	eCdiFin_NotSupport,										// Z Fold Support
	eCdiFin_NotSupport,										// Tri-Fold ÅhCÅh Support
	eCdiFin_NotSupport,										// Tri-Fold ÅgZÅh Support
	eCdiFin_NotSupport,										// Saddle Fold Support
	eCdiFin_Support,										// Stapling Selectability
	eCdiFin_NotSupport,										// Single Stitch
	eCdiFin_NotSupport,										// Double Stitch
	eCdiFin_NotSupport,										// Triple Stitch
	eCdiFin_NotSupport,										// Quad Stitch
	eCdiFin_NotSupport,										// Single Rear
	eCdiFin_Support,										// Single Front
	eCdiFin_NotSupport,										// Pin Cut
	{														// Minimum Number Of Sheets Per Stapled Set
		2,
		0,													//2015/09/01 LJE
		0,													//2015/09/01 LJE
		0
	},
	{														// Maximum Number Of Sheets Per Stapled Set
		50,													//2015/09/01 LJE
		0,
		0,
		0
	},
	{														// Stapled Set Tray Capacity
		0,													//2015/10/13 LJE
		0,													//2015/10/13 LJE
		0,
		0
	},
	eCdiFin_NotSupport,										// Punching Selectability
	eCdiFin_NotSupport,										// Double Punch
	eCdiFin_NotSupport,										// Triple Punch
	eCdiFin_NotSupport,										// Quad Punch
	eCdiFin_NotSupport,										// Multi Punch
	eCdiFinDPunchType_Generally,							// Double Punch Type
	eCdiFinQPunchType_Generally,							// Quad Punch Type
	eCdiFinMPunchType_Generally,							// Multi Punch Type
	eCdiFin_NotSupport,										// Folding Booklet Maker Selectability
	eCdiFin_NotSupport,										// Saddle Fold And Staple
	eCdiFin_NotSupport,										// Saddle Fold And No Staple
	eCdiFinCvrOrder_First,									// Cover Order
	eCdiFin_NotSupport,										// Square Folding
	1,														// Staple And Folded Minimum Number Of Sheets Per Booklet
	1,														// Staple And Folded Maximum Number Of Sheets Per Booklet
	1,														// Staple And Folded Booklet Tray Capacity
	1,														// Folded Only Minimum Number Of Sheets Per Booklet
	1,														// Folded Only Maximum Number Of Sheets Per Booklet
	1,														// Folded Only Booklet Tray Capacity
	1,														// Number Of Bins
	1,														// Count Of Z-Fold Sheet
	eCdiFinStapleType_XE,									// Staple Type
	eCdiFinStapleType_XE,									// Booklet Maker Staple Type
};

// --- Spec data of Input Source Capabilities ---
//CapaInSrcCapabilitiesSpec const cCapa_InSrcCapabilitiesSpecXxx = {
//	// !!! Product Specific !!!
//	eCdiFinInSrc_All,										// Instance ID
//	OPTID_BASE,												// Relative Option ID
//	1,														// Input Max Capacity
//	eCdiFinInUnitType_Inserter								// Device Unit Type
//};

// --- Capabilities Informer Spec ---
CapaInformerSpec const cCapa_InformerSpec = {
	// !!! Product Specific !!!
	// --- Device Controller ---
	{
		&cCapa_DevContCapabilitiesSpecAll,						// All
		&cCapa_DevContCapabilitiesSpecAries
	},
	// --- Output Destination ---
	{
		&cCapa_OutDestCapabilitiesSpecOutTray
	},
//	// --- Input Source ---
//	{
//		&cCapa_InSrcCapabilitiesSpecXxx
//	}
};


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
