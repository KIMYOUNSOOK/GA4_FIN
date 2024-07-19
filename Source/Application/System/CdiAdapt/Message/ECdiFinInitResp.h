/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinInitResp.h
//  Implementation of the Class ECdiFinInitResp
//  Created on:      2012/08/07 13:22:10
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_B9FE677A_F175_403d_8AE5_46CA37942A92__INCLUDED_)
#define EA_B9FE677A_F175_403d_8AE5_46CA37942A92__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Attributes Messages::ECdiFinInitResp
// Attribute Init Response
////////////////////////////////////////////////////////////////////////
enum ECdiFinInitResp
{
	// Successful Initialization
	eCdiFinInitResp_Success = 0x00,
	// Failed No Such Attribute
	eCdiFinInitResp_NoSuchAttr = 0x03,
	// Failed No Such Attribute Chain
	eCdiFinInitResp_NoSuchAttrChain = 0x04,
	// Attribute Initialize-Protect
	eCdiFinInitResp_AttrInitProtect = 0x07
};
#endif // !defined(EA_B9FE677A_F175_403d_8AE5_46CA37942A92__INCLUDED_)
