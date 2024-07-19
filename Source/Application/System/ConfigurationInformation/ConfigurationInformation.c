// File Name:	ConfigurationInformation.c
// Description:	Implimentation File of Configuration Information
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.14
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCfgInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Spec data of Finsher Device Controller Configuration ---
CfgDevContConfigurationSpec const cCfg_DevContConfigurationSpecAll = {
	// !!! Product Specific !!!
	eCdiFinDevCont_All,
};

CfgDevContConfigurationSpec const cCfg_DevContConfigurationSpecAries = {
	// !!! Product Specific !!!
	// eCdiFinDevCont_Aries,
	eCdiFinDevCont_GA4Fin,
};

// --- Spec data of Output Destination Configuration ---
extern struct CfgOutDestConfigurationOverrideMethod const cCfg_OutDestConfigurationMethodOutTray;

CfgOutDestConfigurationSpec const cCfg_OutDestConfigurationSpecOutTray = {
	// !!! Product Specific !!!
	&cCfg_OutDestConfigurationMethodOutTray,
	eCdiFinOutDest_OutTray,
	OPTID_BASE
};

// --- Spec data of Input Source Configuration ---
//CfgInSrcConfigurationSpec const cCfg_InSrcConfigurationSpecXxx = {
//	// !!! Product Specific !!!
//	&cCfg_InSrcConfigurationMethodStandard,
//	eCdiFinInSrc_All,
//	OPTID_BASE
//};

// --- Configuration Informer Spec ---
CfgInformerSpec const cCfg_InformerSpec = {
	// !!! Product Specific !!!
	// --- Device Controller ---
	{
		&cCfg_DevContConfigurationSpecAll,		// All
		&cCfg_DevContConfigurationSpecAries		// Aries-Finisher
	},
	// --- Output Destination ---
	{
		&cCfg_OutDestConfigurationSpecOutTray	// Output Tray
	},
	// --- Input Source ---
//	{
//		&cCfg_InSrcConfigurationSpecXxx
//	}
};


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
