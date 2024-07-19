/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinMediaType.h
//  Implementation of the Class ECdiFinMediaType
//  Created on:      2013/05/28 15:14:18
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_784CFCC2_1608_4f00_8272_48E1549C711B__INCLUDED_)
#define EA_784CFCC2_1608_4f00_8272_48E1549C711B__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Sheet Finish Attributes::ECdiFinMediaType
// Media Type
////////////////////////////////////////////////////////////////////////
enum ECdiFinMediaType
{
	// Plain Paper
	eCdiFinMediaType_Plain = 1,
	// Recycled Paper
	eCdiFinMediaType_Recycled = 2,
	// Coated Paper
	eCdiFinMediaType_Coated = 3,
	// Embossed Paper
	eCdiFinMediaType_Embossed = 4,
	// Label Paper
	eCdiFinMediaType_Label = 5,
	// Transfer Paper
	eCdiFinMediaType_Transfer = 6,
	// Dedicated Gloss Paper
	eCdiFinMediaType_DedicatedGloss = 7,
	// Transparency
	eCdiFinMediaType_Transparency = 8,
	// Tack-Film
	eCdiFinMediaType_TackFilm = 9,
	// Post Card Paper
	eCdiFinMediaType_PostCard = 10,
	// Envelope Paper
	eCdiFinMediaType_Envelope = 11,
	// Tracing Paper
	eCdiFinMediaType_Tracing = 12
};
#endif // !defined(EA_784CFCC2_1608_4f00_8272_48E1549C711B__INCLUDED_)
