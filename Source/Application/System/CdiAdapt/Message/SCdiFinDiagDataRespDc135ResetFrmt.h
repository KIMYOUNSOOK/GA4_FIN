/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagRespDc135ResetFrmt.h
//  Implementation of the Class SCdiFinDiagDataRespDc135ResetFrmt
//  Created on:      2012/11/15 14:14:16
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagRespDc135ResetFrmt_h
#define SCdiFinDiagRespDc135ResetFrmt_h

#define CDIFIN_DIAG_DATA_RESP_DC135_RESET_SIZE				5


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Counters Messages::SCdiFinDiagDataRespDc135ResetFrmt
// Diagnostic Data Format of "Diagnostics
// Response / Device HFSI Counter Access
// (dC135) / Reset" Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataRespDc135ResetFrmt
{

	// Chain(Bit15 ~ Bit8)
	UC chain_Octet0;
	// Chain(Bit7 ~ Bit0)
	UC chain_Octet1;
	// Link(Bit15 ~ Bit8)
	UC link_Octet0;
	// Link(Bit7 ~ Bit0)
	UC link_Octet1;
	// Finisher Device Controller ID
	UC finDevContID;

};

#endif // #ifndef SCdiFinDiagRespDc135ResetFrmt_h
