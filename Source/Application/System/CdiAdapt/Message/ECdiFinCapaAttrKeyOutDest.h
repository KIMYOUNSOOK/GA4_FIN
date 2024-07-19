/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinCapaAttrKeyOutDest.h
//  Implementation of the Class ECdiFinCapaAttrKeyOutDest
//  Created on:      2013/03/08 18:03:02
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_4EEE0A13_4238_4b43_8DE9_93D128BBD2C7__INCLUDED_)
#define EA_4EEE0A13_4238_4b43_8DE9_93D128BBD2C7__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Capabilities and Configuration Attributes::ECdiFinCapaAttrKeyOutDest
// Output Destination Capabilities Attr Key
////////////////////////////////////////////////////////////////////////
enum ECdiFinCapaAttrKeyOutDest
{
	// All
	eCdiFinCapaKeyOutDest_All = 0x00,
	// Output Max Capacity
	eCdiFinCapaKeyOutDest_MaxCapa = 0x02,
	// Output Destination Unit Type
	eCdiFinCapaKeyOutDest_UnitType = 0x04,
	// Offset Ability
	eCdiFinCapaKeyOutDest_Offset = 0x47,
	// Folding Ability
	eCdiFinCapaKeyOutDest_Fold = 0x4B,
	// Stapling Ability
	eCdiFinCapaKeyOutDest_Staple = 0x50,
	// Punching Ability
	eCdiFinCapaKeyOutDest_Punch = 0x55,
	// Folding Booklet Maker Ability
	eCdiFinCapaKeyOutDest_Book = 0x5B,
	// Booklet Maker Tray Capacity
	eCdiFinCapaKeyOutDest_BookTrayCapa = 0x5E,
	// Number Of Bins
	eCdiFinCapaKeyOutDest_NumOfBins = 0x65,
	// Binding Booklet Maker Ability
	eCdiFinCapaKeyOutDest_BindBook = 0xC0,
	// Trimming Ability
	eCdiFinCapaKeyOutDest_Trimming = 0xC1,
	// Count of Sheet In Stapled Set
	eCdiFinCapaKeyOutDest_CountInStplSet = 0xC2,
	// Maximum Number Of Sheets Per Stapled
	// Set By Media Type
	eCdiFinCapaKeyOutDest_MaxShtStplMediaType = 0xC3,
	// Maximum Number Of Sheets Per Saddle
	// Folded Set
	eCdiFinCapaKeyOutDest_MaxShtSaddleFold = 0xC4,
	// Number Of Sheets Per Bound Set
	eCdiFinCapaKeyOutDest_NumShtBound = 0xC5,
	// Staple Type
	eCdiFinCapaKeyOutDest_StapleType = 0xC6,
	// Booklet Maker Staple Type
	eCdiFinCapaKeyOutDest_BookStapleType = 0xC7,
	// Staple Specific Media Type
	eCdiFinCapaKeyOutDest_StplSpecMediaType = 0xC8,
	// Saddle Fold & Staple Specific Media Type
	eCdiFinCapaKeyOutDest_SFoldStplSpecMediaType = 0xC9
};
#endif // !defined(EA_4EEE0A13_4238_4b43_8DE9_93D128BBD2C7__INCLUDED_)
