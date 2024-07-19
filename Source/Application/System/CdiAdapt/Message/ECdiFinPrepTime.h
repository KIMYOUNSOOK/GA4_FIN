/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinPrepTime.h
//  Implementation of the Class ECdiFinPrepTime
//  Created on:      12-3-2012 21:09:32
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_9224B6DD_51E4_4704_B99F_DA8291E4D390__INCLUDED_)
#define EA_9224B6DD_51E4_4704_B99F_DA8291E4D390__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Finisher Device Function::ECdiFinPrepTime
// Prep Time
////////////////////////////////////////////////////////////////////////
enum ECdiFinPrepTime
{
	// No Wait
	eCdiFinPrepTime_NoWait = 0,
	// Considerable Time
//	eCdiFinPrepTime_ConsiTime = 0xFFFE,
	// Unknown
//	eCdiFinPrepTime_Unknown = 0xFFFF
};

#define eCdiFinPrepTime_ConsiTime		0xFFFE
#define eCdiFinPrepTime_Unknown			0xFFFF

#endif // !defined(EA_9224B6DD_51E4_4704_B99F_DA8291E4D390__INCLUDED_)
