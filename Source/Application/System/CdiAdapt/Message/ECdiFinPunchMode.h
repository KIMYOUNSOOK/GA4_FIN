/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinPunchMode.h
//  Implementation of the Class ECdiFinPunchMode
//  Created on:      2012/05/21 18:56:56
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_1C18B20B_EA01_43a3_A66B_A11905603FEA__INCLUDED_)
#define EA_1C18B20B_EA01_43a3_A66B_A11905603FEA__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Sheet Finish Attributes::ECdiFinPunchMode
// Punch Mode
////////////////////////////////////////////////////////////////////////
enum ECdiFinPunchMode
{
	// Punch Mode Off
	eCdiFinPunch_Off = 0x00,
	// Double Punch
	eCdiFinPunch_Double,
	// Triple Punch
	eCdiFinPunch_Triple,
	// Quad Punch
	eCdiFinPunch_Quad,
	// Multi Punch
	eCdiFinPunch_Multi
};
#endif // !defined(EA_1C18B20B_EA01_43a3_A66B_A11905603FEA__INCLUDED_)
