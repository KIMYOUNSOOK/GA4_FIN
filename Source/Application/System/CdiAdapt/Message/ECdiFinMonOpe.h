/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinMonOpe.h
//  Implementation of the Class ECdiFinMonOpe
//  Created on:      28-2-2012 19:28:25
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_759E3B00_ED6B_4b59_933F_10980E816E5B__INCLUDED_)
#define EA_759E3B00_ED6B_4b59_933F_10980E816E5B__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Controller Diagnostics::ECdiFinMonOpe
// Monitor Operation
////////////////////////////////////////////////////////////////////////
enum ECdiFinMonOpe
{
	// Start
	eCdiFinMonOpe_Start = 0x01,
	// Stop
	eCdiFinMonOpe_Stop = 0x02,
	// All Stop
	eCdiFinMonOpe_AllStop = 0x04
};
#endif // !defined(EA_759E3B00_ED6B_4b59_933F_10980E816E5B__INCLUDED_)
