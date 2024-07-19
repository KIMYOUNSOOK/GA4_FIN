/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinCfgAttrKeyDevCont.h
//  Implementation of the Class ECdiFinCfgAttrKeyDevCont
//  Created on:      2013/09/24 12:21:14
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_3E96DB39_62E5_42ab_A07A_414C57C8F8CC__INCLUDED_)
#define EA_3E96DB39_62E5_42ab_A07A_414C57C8F8CC__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Capabilities and Configuration Attributes::ECdiFinCfgAttrKeyDevCont
// Finisher Device Controller
// Configuration Attr Key
////////////////////////////////////////////////////////////////////////
enum ECdiFinCfgAttrKeyDevCont
{
	// All
	eCdiFinCfgKeyDevCont_All = 0x00,
	// Market Region
	eCdiFinCfgKeyDevCont_Market = 0x01,
	// Set Date Time
	eCdiFinCfgKeyDevCont_DateTime = 0x40,
	// System Configuration
	eCdiFinCfgKeyDevCont_SysCfg = 0x41,
	// Media Size Group
	eCdiFinCfgKeyDevCont_MediaSizeGrp = 0xC0,
	// Previous Device Controller
	eCdiFinCfgKeyDevCont_PrevDevCont = 0xC1,
	// Product Code
	eCdiFinCfgKeyDevCont_ProductCode = 0xC2,
	// IFM IOT Exit Motor Profile
	eCdiFinCfgKeyDevCont_IfmIotExitMotPrfl = 0xC5,
	// Dynamic Sample Master Control
	eCdiFinCfgKeyDevCont_DynamicSample = 0xC6,
	// DFA Profile  Data
	eCdiFinCfgKeyDevCont_DfaProfile = 0xC7,
	// Sheet Temperature
	eCdiFinCfgKeyDevCont_ShtTemperature = 0xC8,
	// Perfect Binder Time Limit for Warm-Up
	eCdiFinCfgKeyDevCont_PBTimeLimitWarmUp = 0xCA,
	// Glue Spread Adjust Data
	eCdiFinCfgKeyDevCont_GlueSpreadAdj = 0xCB,
	// Manual Input Control
	eCdiFinCfgKeyDevCont_ManualInputCont = 0xCC,
	// Perfect Bind Adjust Data
	eCdiFinCfgKeyDevCont_PBAdj = 0xCD,
	// Booklet Maker Custom Adjust Data
	eCdiFinCfgKeyDevCont_BookCustAdj = 0xCE,
	// Fold Custom Adjust Data
	eCdiFinCfgKeyDevCont_FoldCustAdj = 0xCF,
	// Sheet Exit Height
	eCdiFinCfgKeyDevCont_ShtExtHeight = 0xFA,
	// Productivity
	eCdiFinCfgKeyDevCont_Productivity = 0xFB,
	// Exit Speed Range
	eCdiFinCfgKeyDevCont_ExitSpdRange = 0xFC,
	// Sheet Exit Layout
	eCdiFinCfgKeyDevCont_ExitLayout = 0xFD,
	// Jam Bypass
	eCdiFinCfgKeyDevCont_JamBypass = 0xFE,
	// No Paper Run
	eCdiFinCfgKeyDevCont_NoPprRun = 0xFF
};
#endif // !defined(EA_3E96DB39_62E5_42ab_A07A_414C57C8F8CC__INCLUDED_)
