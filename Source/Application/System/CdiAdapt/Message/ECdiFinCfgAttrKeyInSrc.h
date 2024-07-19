/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinCfgAttrKeyInSrc.h
//  Implementation of the Class ECdiFinCfgAttrKeyInSrc
//  Created on:      2012/05/24 20:42:08
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_2FC5E7D4_2347_4049_8A84_7413697B39B0__INCLUDED_)
#define EA_2FC5E7D4_2347_4049_8A84_7413697B39B0__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Capabilities and Configuration::ECdiFinCfgAttrKeyInSrc
// Input Source Configuration Attr Key
////////////////////////////////////////////////////////////////////////
enum ECdiFinCfgAttrKeyInSrc
{
	// All
	eCdiFinCfgKeyInSrc_All = 0x00,
	// Selectability
	eCdiFinCfgKeyInSrc_Select = 0x03,
	// Media Size Alternate
	eCdiFinCfgKeyInSrc_SizeAlt = 0xC0,
	// Irregular Media Size
	eCdiFinCfgKeyInSrc_IregSize = 0xC1,
	// PB Media Size
	eCdiFinCfgKeyInSrc_PbMediaSize = 0xC2,
	// PB Manual Input Media Size
	eCdiFinCfgKeyInSrc_PbManualSize = 0xC3,
	// PB Manual Input Media Type
	eCdiFinCfgKeyInSrc_PbManualType = 0xC4
};
#endif // !defined(EA_2FC5E7D4_2347_4049_8A84_7413697B39B0__INCLUDED_)
