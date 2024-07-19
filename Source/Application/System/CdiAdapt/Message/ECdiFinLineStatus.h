/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinLineStatus.h
//  Implementation of the Class ECdiFinLineStatus
//  Created on:      2012/11/22 16:12:45
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_940D5E2C_A17C_44bd_938F_43671A2A16C5__INCLUDED_)
#define EA_940D5E2C_A17C_44bd_938F_43671A2A16C5__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Finisher Device Function::ECdiFinLineStatus
// Line Status
////////////////////////////////////////////////////////////////////////
enum ECdiFinLineStatus
{
	// Ready To Go Off Line
	eCdiFinLineStts_RdyToGoOffLine = 0,
	// Ready To Go On Line
	eCdiFinLineStts_RdyToGoOnLine = 1,
	// On Line
	eCdiFinLineStts_OnLine = 2,
	// Off Line
	eCdiFinLineStts_OffLine = 3,
	// Not Ready To Go Off Line
	eCdiFinLineStts_NotRdyToGoOffLine = 4
};
#endif // !defined(EA_940D5E2C_A17C_44bd_938F_43671A2A16C5__INCLUDED_)
