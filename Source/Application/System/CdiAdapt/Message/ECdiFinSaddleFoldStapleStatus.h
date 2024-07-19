/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinSaddleFoldStapleStatus.h
//  Implementation of the Class ECdiFinSaddleFoldStapleStatus
//  Created on:      2012/05/31 21:05:29
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_64FF60B3_E04A_4362_B261_19A9D43AE115__INCLUDED_)
#define EA_64FF60B3_E04A_4362_B261_19A9D43AE115__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Capabilities and Configuration Attributes::ECdiFinSaddleFoldStapleStatus
// Fold Position Status In Saddle Fold And
// Staple
////////////////////////////////////////////////////////////////////////
enum ECdiFinSaddleFoldStapleStatus
{
	// Tail Edge Side Longer & Staple On Tail
	// Edge Side
	eCdiFinSaddleStplStts_TailLongStapleTail = 1,
	// Lead Edge Side Longer & Staple On Tail
	// Edge Side
	eCdiFinSaddleStplStts_LeadLongStapleTail = 2,
	// Tail Edge Side Longer & Staple On Lead
	// Edge Side
	eCdiFinSaddleStplStts_TailLongStapleLead = 3,
	// Lead Edge Side Longer & Staple On Lead
	// Edge Side
	eCdiFinSaddleStplStts_LeadLongStapleLead = 4,
	eCdiFinSaddleStplStts_Default = 255
};
#endif // !defined(EA_64FF60B3_E04A_4362_B261_19A9D43AE115__INCLUDED_)
