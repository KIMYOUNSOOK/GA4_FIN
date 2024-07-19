/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinOutUnitType.h
//  Implementation of the Class ECdiFinOutUnitType
//  Created on:      2012/11/12 9:54:57
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_6B5D0E9F_06A2_4625_B4E4_5EF3E4BF0101__INCLUDED_)
#define EA_6B5D0E9F_06A2_4625_B4E4_5EF3E4BF0101__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Capabilities and Configuration Attributes::ECdiFinOutUnitType
// Output Destination Device Unit Type
////////////////////////////////////////////////////////////////////////
enum ECdiFinOutUnitType
{
	// Other
	eCdiFinOutUnitType_Other = 1,
	// Normal
	eCdiFinOutUnitType_Normal = 4,
	// Continuous Roll
	eCdiFinOutUnitType_ContinuousRoll = 5,
	// Mailbox
	eCdiFinOutUnitType_Mailbox = 6,
	// High Cap Stacker
	eCdiFinOutUnitType_HighCap = 17
};
#endif // !defined(EA_6B5D0E9F_06A2_4625_B4E4_5EF3E4BF0101__INCLUDED_)
