/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinComponentResp.h
//  Implementation of the Class ECdiFinComponentResp
//  Created on:      28-2-2012 18:01:38
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_29F66C85_D7CD_4b03_B202_D8F1846E7322__INCLUDED_)
#define EA_29F66C85_D7CD_4b03_B202_D8F1846E7322__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Component Control Messages::ECdiFinComponentResp
// Component Response
////////////////////////////////////////////////////////////////////////
enum ECdiFinComponentResp
{
	// Successful On
	eCdiFinComponentResp_SuccessOn = 0x01,
	// Already On
	eCdiFinComponentResp_AlreadyOn = 0x02,
	// Successful Off
	eCdiFinComponentResp_SuccessOff = 0x05,
	// Already Off
	eCdiFinComponentResp_AlreadyOff = 0x06
};
#endif // !defined(EA_29F66C85_D7CD_4b03_B202_D8F1846E7322__INCLUDED_)
