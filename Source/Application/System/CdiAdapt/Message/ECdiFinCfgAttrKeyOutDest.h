/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinCfgAttrKeyOutDest.h
//  Implementation of the Class ECdiFinCfgAttrKeyOutDest
//  Created on:      2012/11/12 9:58:46
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_D3215220_1571_40d0_A426_12E9C88CF11F__INCLUDED_)
#define EA_D3215220_1571_40d0_A426_12E9C88CF11F__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Capabilities and Configuration Attributes::ECdiFinCfgAttrKeyOutDest
// Output Destination Configuration Attr
// Key
////////////////////////////////////////////////////////////////////////
enum ECdiFinCfgAttrKeyOutDest
{
	// All
	eCdiFinCfgKeyOutDest_All = 0x00,
	// Selectability
	eCdiFinCfgKeyOutDest_Select = 0x03,
	// Current Offset Configuration
	eCdiFinCfgKeyOutDest_Offset = 0x45,
	// Current Sheet Folding Configuration
	eCdiFinCfgKeyOutDest_Fold = 0x46,
	// Current Staple Configuration
	eCdiFinCfgKeyOutDest_Staple = 0x47,
	// Current Punch Configuration
	eCdiFinCfgKeyOutDest_Punch = 0x48,
	// Current Booklet Maker Configuration
	eCdiFinCfgKeyOutDest_Book = 0x49,
	// Current Trimming Configuration
	eCdiFinCfgKeyOutDest_Trimming = 0xC1,
	// Staple Cartridge Type
	eCdiFinCfgKeyOutDest_StplCartridge = 0xC3
};
#endif // !defined(EA_D3215220_1571_40d0_A426_12E9C88CF11F__INCLUDED_)
