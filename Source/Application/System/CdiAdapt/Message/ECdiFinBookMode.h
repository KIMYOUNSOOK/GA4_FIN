/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinBookMode.h
//  Implementation of the Class ECdiFinBookMode
//  Created on:      2012/05/21 18:56:59
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_D5E4C964_A4ED_4841_BDD8_D1583DD94CFC__INCLUDED_)
#define EA_D5E4C964_A4ED_4841_BDD8_D1583DD94CFC__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Sheet Finish Attributes::ECdiFinBookMode
// Booklet Maker Mode
////////////////////////////////////////////////////////////////////////
enum ECdiFinBookMode
{
	// Booklet Maker Off
	eCdiFinBook_Off = 0x00,
	// Glue Bound Booklet
	eCdiFinBook_GlueBound = 0x01,
	// Saddle Fold And Staple
	eCdiFinBook_SaddleStaple = 0x02,
	// Saddle Fold And No Staple
	eCdiFinBook_SaddleNoStaple = 0x03
};
#endif // !defined(EA_D5E4C964_A4ED_4841_BDD8_D1583DD94CFC__INCLUDED_)
