/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinOffsetPos.h
//  Implementation of the Class ECdiFinOffsetPos
//  Created on:      2012/05/21 18:56:23
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_35F5C34D_D482_4e0a_B7E8_D20B9733680A__INCLUDED_)
#define EA_35F5C34D_D482_4e0a_B7E8_D20B9733680A__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Finisher Device Function::ECdiFinOffsetPos
// Offset Position
////////////////////////////////////////////////////////////////////////
enum ECdiFinOffsetPos
{
	// Current Position
	eCdiFinOffsetPos_Current = 0x00,
	// LEFT Position
	eCdiFinOffsetPos_Left = 0x01,
	// RIGHT Position
	eCdiFinOffsetPos_Right = 0x02,
	// Center Position From Front
	eCdiFinOffsetPos_CenterFront = 0x03,
	// Center Position From Rear
	eCdiFinOffsetPos_CenterRear = 0x04
};
#endif // !defined(EA_35F5C34D_D482_4e0a_B7E8_D20B9733680A__INCLUDED_)
