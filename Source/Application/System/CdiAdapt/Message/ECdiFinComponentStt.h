/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinComponentStt.h
//  Implementation of the Class ECdiFinComponentStt
//  Created on:      28-2-2012 18:00:50
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_1B01C601_E25A_4b24_BA4A_2B808D8AEFFE__INCLUDED_)
#define EA_1B01C601_E25A_4b24_BA4A_2B808D8AEFFE__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Controller Diagnostics::ECdiFinComponentStt
// Component State
////////////////////////////////////////////////////////////////////////
enum ECdiFinComponentStt
{
	// Off
	eCdiFinComponentStt_Off = 0x00,
	// On
	eCdiFinComponentStt_On = 0x01,
	// Cycle
	eCdiFinComponentStt_Cycle = 0x02,
	// All Off
	eCdiFinComponentStt_AllOff = 0x03
};
#endif // !defined(EA_1B01C601_E25A_4b24_BA4A_2B808D8AEFFE__INCLUDED_)
