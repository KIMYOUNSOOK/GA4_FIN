///////////////////////////////////////////////////////////
//  SCdiFinStopFinFrmt.h
//  Implementation of the Class SCdiFinStopFinFrmt
//  Created on:      2013/12/06 16:47:56
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinStopFinFrmt_h)
#define SCdiFinStopFinFrmt_h

#define CDIFIN_STOP_FIN_MSG_SIZE		2


struct SCdiFinStopFinFrmt
{
	UC id;								// Message ID
	UC shutDownType;					// Shut Down Type
};

#endif // !defined(SCdiFinStopFinFrmt_h)
