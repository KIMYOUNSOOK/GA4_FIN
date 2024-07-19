/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinFoldPosSttsSaddleStpl.h
//  Implementation of the Class ECdiFinFoldPosSttsSaddleStpl
//  Created on:      2013/05/22 15:02:47
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_FB54532D_4E66_4536_B34F_821F4DDC217D__INCLUDED_)
#define EA_FB54532D_4E66_4536_B34F_821F4DDC217D__INCLUDED_

enum ECdiFinFoldPosSttsSaddleStpl
{
	// Tail Edge Side Longer & Staple On Tail
	// Edge Side 
	eCdiFinFoldPosSttsSaddleStpl_TailLongStplTail = 1,
	// Lead Edge Side Longer & Staple On Tail
	// Edge Side 
	eCdiFinFoldPosSttsSaddleStpl_LeadLongStplTail = 2,
	// Tail Edge Side Longer & Staple On Lead
	// Edge Side
	eCdiFinFoldPosSttsSaddleStpl_TailLongStplLead = 3,
	// Lead Edge Side Longer & Staple On Lead
	// Edge Side 
	eCdiFinFoldPosSttsSaddleStpl_LeadLongStplLead,
	// Default
	eCdiFinFoldPosSttsSaddleStpl_Default = 0xFF
};
#endif // !defined(EA_FB54532D_4E66_4536_B34F_821F4DDC217D__INCLUDED_)
