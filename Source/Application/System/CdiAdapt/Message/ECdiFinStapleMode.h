/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinStapleMode.h
//  Implementation of the Class ECdiFinStapleMode
//  Created on:      2012/05/21 18:56:43
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_F0AEE8C1_F87D_4c3a_912E_2C343A2DDBC3__INCLUDED_)
#define EA_F0AEE8C1_F87D_4c3a_912E_2C343A2DDBC3__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Sheet Finish Attributes::ECdiFinStapleMode
// Staple Mode
////////////////////////////////////////////////////////////////////////
enum ECdiFinStapleMode
{
	// Staple Mode Off
	eCdiFinStaple_Off = 0x00,
	// Single Stitch
	eCdiFinStaple_Single = 0x01,
	// Double Stitch
	eCdiFinStaple_Double = 0x02,
	// Quad Stitch
	eCdiFinStaple_Quad = 0x04,
	// Single Front
	eCdiFinStaple_SingleFront = 0x06,
	// Single Rear
	eCdiFinStaple_SingleRear = 0x07
};
#endif // !defined(EA_F0AEE8C1_F87D_4c3a_912E_2C343A2DDBC3__INCLUDED_)
