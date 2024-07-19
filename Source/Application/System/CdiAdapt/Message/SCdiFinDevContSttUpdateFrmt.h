///////////////////////////////////////////////////////////
//  SCdiFinDevContSttUpdateFrmt.h
//  Implementation of the Class SCdiFinDevContSttUpdateFrmt
//  Created on:      2013/12/06 17:12:27
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinDevContSttUpdateFrmt_h)
#define SCdiFinDevContSttUpdateFrmt_h

#define CDIFIN_DEV_CONT_STT_UPDT_MSG_SIZE					2


struct SCdiFinDevContSttUpdateFrmt
{
	UC id;								// Message ID
	UC devContState;					// Device Controller State
										// - Production State (Bi0 Å` Bit1)
										// - Transition State (Bi5 Å` Bit6)
};

#endif // !defined(SCdiFinDevContSttUpdateFrmt_h)
