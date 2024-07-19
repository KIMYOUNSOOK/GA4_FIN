/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinUpdateResp.h
//  Implementation of the Class ECdiFinUpdateResp
//  Created on:      2012/08/07 11:09:45
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_E4EADAA4_FA19_49e4_B1AB_564E650054A8__INCLUDED_)
#define EA_E4EADAA4_FA19_49e4_B1AB_564E650054A8__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Attributes Messages::ECdiFinUpdateResp
// Update Response
////////////////////////////////////////////////////////////////////////
enum ECdiFinUpdateResp
{
	// Successful Update
	eCdiFinUpdateResp_Success = 0x00,
	// Illegal Value
	eCdiFinUpdateResp_IllegalValue = 0x04,
	// Incorrect Data Type
	eCdiFinUpdateResp_IncorrectDataType = 0x02,
	// Value Below Minimum
	eCdiFinUpdateResp_ValBelowMin = 0x05,
	// Value Above Maximum
	eCdiFinUpdateResp_ValAboveMax = 0x06,
	// Attribute Read Only
	eCdiFinUpdateResp_AttrReadOnly = 0x07
};
#endif // !defined(EA_E4EADAA4_FA19_49e4_B1AB_564E650054A8__INCLUDED_)
