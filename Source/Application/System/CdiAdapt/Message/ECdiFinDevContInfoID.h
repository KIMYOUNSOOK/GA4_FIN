/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinDevContInfoID.h
//  Implementation of the Class ECdiFinDevContInfoID
//  Created on:      2012/11/01 15:53:54
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_AFF45465_7E4A_4621_8E4B_A99D49D78F69__INCLUDED_)
#define EA_AFF45465_7E4A_4621_8E4B_A99D49D78F69__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Controller Condition::ECdiFinDevContInfoID
// Information ID
////////////////////////////////////////////////////////////////////////
enum ECdiFinDevContInfoID
{
	// Fault Causal Sheet
	eCdiFinDevContInfoID_FaultCausalSht = 0x00,
	// Cancel Staple
	eCdiFinDevContInfoID_CancelStaple = 0x01,
	// PB Media Mismatch
	eCdiFinDevContInfoID_PBMediaMismatch = 0x02,
	// Dynamic Sample Request
	eCdiFinDevContInfoID_DynamicSampleReq = 0x03,
	// Communicate In CDI-Finisher
	eCdiFinDevContInfoID_CommInCdiFin = 0xFF
};
#endif // !defined(EA_AFF45465_7E4A_4621_8E4B_A99D49D78F69__INCLUDED_)
