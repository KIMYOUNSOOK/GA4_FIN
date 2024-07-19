/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinShtFinAttr.h
//  Implementation of the Class ECdiFinShtFinAttr
//  Created on:      2012/11/12 9:52:54
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_A89F4D20_C999_4a97_839D_9D7E787A4310__INCLUDED_)
#define EA_A89F4D20_C999_4a97_839D_9D7E787A4310__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Finisher Device Function::ECdiFinShtFinAttr
// Sheet Finish Attribute ID
////////////////////////////////////////////////////////////////////////
enum ECdiFinShtFinAttr
{
	// Offset Mode
	eCdiFinShtFinAttr_OfstMode = 0x01,
	// Staple Mode
	eCdiFinShtFinAttr_StplMode = 0x02,
	// Punch Mode
	eCdiFinShtFinAttr_PnchMode = 0x03,
	// Booklet Maker Mode
	eCdiFinShtFinAttr_BkltMode = 0x04,
	// Insert Sheet Mode
	eCdiFinShtFinAttr_InsertMode = 0x06,
	// Fold Mode
	eCdiFinShtFinAttr_FoldMode = 0x07,
	// Media Type
	eCdiFinShtFinAttr_MediaType = 0x08,
	// Media Size
	eCdiFinShtFinAttr_MediaSize = 0x09,
	// Sheet Curl
	eCdiFinShtFinAttr_SheetCurl = 0x0B,
	// Exit Speed
	eCdiFinShtFinAttr_ExitSpd = 0x0C,
	// Sheet Integrity
	eCdiFinShtFinAttr_ShtIntegrity = 0x0D,
	// Set Action
	eCdiFinShtFinAttr_SetAction = 0x20,
	// Offset Position
	eCdiFinShtFinAttr_OfstPos = 0xC0,
	// Booklet Maker Custom Adjust Table No.
	eCdiFinShtFinAttr_BookCustTblNo = 0xC2,
	// Fold Custom Adjust Table No.
	eCdiFinShtFinAttr_FoldCustTblNo = 0xC3,
	// Exit Sheet Interval
	eCdiFinShtFinAttr_ExitShtInterval = 0xC4
};
#endif // !defined(EA_A89F4D20_C999_4a97_839D_9D7E787A4310__INCLUDED_)
