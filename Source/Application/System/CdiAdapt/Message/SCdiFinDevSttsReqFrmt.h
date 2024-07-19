///////////////////////////////////////////////////////////
//  SCdiFinDevSttsReqFrmt.h
//  Implementation of the Class SCdiFinDevSttsReqFrmt
//  Created on:      2013/12/06 17:31:46
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinDevSttsReqFrmt_h)
#define SCdiFinDevSttsReqFrmt_h

#define CDIFIN_DEV_STTS_REQ_MSG_SIZE	3


struct SCdiFinDevSttsReqFrmt
{
	UC id;								// Message ID
	UC statusID_Octet0;					// Status ID(Bit15 Å` Bit8)
	UC statusID_Octet1;					// Status ID(Bit7 Å` Bit0)
};

#endif // !defined(SCdiFinDevSttsReqFrmt_h)
