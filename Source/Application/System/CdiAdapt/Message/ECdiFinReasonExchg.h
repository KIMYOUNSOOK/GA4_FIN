/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinReasonExchg.h
//  Implementation of the Class ECdiFinReasonExchg
//  Created on:      03-3-2012 13:20:41
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_BDF66A5D_4599_4247_B63C_C4C75ED58501__INCLUDED_)
#define EA_BDF66A5D_4599_4247_B63C_C4C75ED58501__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Counters Messages::ECdiFinReasonExchg
// Reason For Exchange
////////////////////////////////////////////////////////////////////////
enum ECdiFinReasonExchg
{
	// Unscheduled Maintenance
	eCdiFinReasonExchg_UnschMainte = 0x00,
	// Scheduled Maintenance
	eCdiFinReasonExchg_SchMainte = 0x01,
	// Other
	eCdiFinReasonExchg_Other = 0x03
};
#endif // !defined(EA_BDF66A5D_4599_4247_B63C_C4C75ED58501__INCLUDED_)
