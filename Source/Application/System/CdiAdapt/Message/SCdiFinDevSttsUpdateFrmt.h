///////////////////////////////////////////////////////////
//  SCdiFinDevSttsUpdateFrmt.h
//  Implementation of the Class SCdiFinDevSttsUpdateFrmt
//  Created on:      2013/12/06 17:31:46
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinDevSttsUpdateFrmt_h)
#define SCdiFinDevSttsUpdateFrmt_h

#define CDIFIN_DEV_STTS_UPDATE_MSG_SIZE						9					// [Å~1Byte]


struct SCdiFinDevSttsUpdateFrmt
{
	UC id;								// Message ID
	UC statusID_Octet0;					// Status ID(Bit15 Å` Bit8)
	UC statusID_Octet1;					// Status ID(Bit7 Å` Bit0)
	UC sttsVal_Octet0;					// Status Value(Bit15 Å` Bit8)
	UC sttsVal_Octet1;					// Status Value(Bit7 Å` Bit0)
	UC chain_Octet0;					// Chain(Bit15 Å` Bit8)
	UC chain_Octet1;					// Chain(Bit7 Å` Bit0)
	UC link_Octet0;						// Link(Bit15 Å` Bit8)
	UC link_Octet1;						// Link(Bit7 Å` Bit0)
};

#endif // !defined(SCdiFinDevSttsUpdateFrmt_h)
