/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinPrdctStt.h
//  Implementation of the Class ECdiFinPrdctStt
//  Created on:      2012/05/24 21:04:46
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_FB4D19F7_4399_49f1_B860_D2FCDEBDE00A__INCLUDED_)
#define EA_FB4D19F7_4399_49f1_B860_D2FCDEBDE00A__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Controller Condition::ECdiFinPrdctStt
// Production State
////////////////////////////////////////////////////////////////////////
enum ECdiFinPrdctStt
{
	// Initialization
	eCdiFinPrdctStt_Init = 0,
	// Standby
	eCdiFinPrdctStt_Standby = 1,
	// Running
	eCdiFinPrdctStt_Running = 2,
	// Unknown
	eCdiFinPrdctStt_Unknown = 3
};
#endif // !defined(EA_FB4D19F7_4399_49f1_B860_D2FCDEBDE00A__INCLUDED_)
