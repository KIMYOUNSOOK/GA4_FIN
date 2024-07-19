/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinDataType.h
//  Implementation of the Class ECdiFinDataType
//  Created on:      05-3-2012 15:44:45
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_85B37F30_CD6A_4280_A33D_91A8A2493527__INCLUDED_)
#define EA_85B37F30_CD6A_4280_A33D_91A8A2493527__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Controller Diagnostics::ECdiFinDataType
// Data Type
////////////////////////////////////////////////////////////////////////
enum ECdiFinDataType
{
	// Signed Char
	eCdiFinDataType_SignChar = 0x00,
	// Unsigned Char
	eCdiFinDataType_UnsignChar = 0x01,
	// Signed Short
	eCdiFinDataType_SignShort = 0x02,
	// Unsigned Short
	eCdiFinDataType_UnsignShort = 0x03,
	// Signed Long
	eCdiFinDataType_SignLong = 0x04,
	// Unsigned Long
	eCdiFinDataType_UnsignLong = 0x05
};
#endif // !defined(EA_85B37F30_CD6A_4280_A33D_91A8A2493527__INCLUDED_)
