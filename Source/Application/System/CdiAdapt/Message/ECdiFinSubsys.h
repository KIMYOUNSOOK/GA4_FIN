/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinSubsys.h
//  Implementation of the Class ECdiFinSubsys
//  Created on:      2012/08/15 18:13:24
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_AA411796_5A56_4c32_9851_3EFB1CC8A4AE__INCLUDED_)
#define EA_AA411796_5A56_4c32_9851_3EFB1CC8A4AE__INCLUDED_

enum ECdiFinSubsys
{
	// Update All
	eCdiFinSubsys_UpdateAll = 0x00,
	// Finisher Device Controller
	eCdiFinSubsys_DevCont = 0x01,
	// Output Destination
	eCdiFinSubsys_OutDest = 0x02,
	// Input Source
	eCdiFinSubsys_InSrc = 0x03,
	// Power Saver Update
	eCdiFinSubsys_PwrSavrUpdate = 0xFE,
	// Update Complete
	eCdiFinSubsys_UpdateComp = 0xFF
};
#endif // !defined(EA_AA411796_5A56_4c32_9851_3EFB1CC8A4AE__INCLUDED_)
