/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinTransStt.h
//  Implementation of the Class ECdiFinTransStt
//  Created on:      2012/05/24 21:04:50
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_D4A5B227_2C70_495d_9B5E_79F44B3312B1__INCLUDED_)
#define EA_D4A5B227_2C70_495d_9B5E_79F44B3312B1__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Controller Condition::ECdiFinTransStt
// Transition State
////////////////////////////////////////////////////////////////////////
enum ECdiFinTransStt
{
	// Idle
	eCdiFinTransStt_Idle = 0,
	// Cycle Up
	eCdiFinTransStt_CycUp = 1,
	// Cycle Down
	eCdiFinTransStt_CycDwn = 2,
	// Hard Down
	eCdiFinTransStt_HardDwn = 3
};
#endif // !defined(EA_D4A5B227_2C70_495d_9B5E_79F44B3312B1__INCLUDED_)
