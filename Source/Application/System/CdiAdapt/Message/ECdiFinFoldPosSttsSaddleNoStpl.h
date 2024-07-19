/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinFoldPosSttsSaddleNoStpl.h
//  Implementation of the Class ECdiFinFoldPosSttsSaddleNoStpl
//  Created on:      2013/05/22 15:02:42
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_EE735F48_5EE1_4847_8804_25AA550AEDD6__INCLUDED_)
#define EA_EE735F48_5EE1_4847_8804_25AA550AEDD6__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Controller Configuration Attribute::ECdiFinFoldPosSttsSaddleNoStpl
// Fold Position Status In Saddle Fold And
// No Staple
////////////////////////////////////////////////////////////////////////
enum ECdiFinFoldPosSttsSaddleNoStpl
{
	// Tail Edge Side Longer
	eCdiFinFoldPosSttsSaddleNoStpl_TailEdgeLonger = 1,
	// Lead Edge Side Longer
	eCdiFinFoldPosSttsSaddleNoStpl_LeadEdgeLonger = 2,
	// Default
	eCdiFinFoldPosSttsSaddleNoStpl_Default = 0xFF
};
#endif // !defined(EA_EE735F48_5EE1_4847_8804_25AA550AEDD6__INCLUDED_)
