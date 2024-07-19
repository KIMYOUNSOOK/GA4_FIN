/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinSetEndAct.h
//  Implementation of the Class ECdiFinSetEndAct
//  Created on:      12-3-2012 21:10:02
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_065490D3_78E0_4f4b_9CED_62640B4A20DE__INCLUDED_)
#define EA_065490D3_78E0_4f4b_9CED_62640B4A20DE__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Finisher Device Function::ECdiFinSetEndAct
// Set End Action
////////////////////////////////////////////////////////////////////////
enum ECdiFinSetEndAct
{
	// Normal End (Eject)
	eCdiFinSetEndAct_NrmlEnd = 1,
	// Abort Set (Purge)
	eCdiFinSetEndAct_AbortSet = 2
};
#endif // !defined(EA_065490D3_78E0_4f4b_9CED_62640B4A20DE__INCLUDED_)
