/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataExecDc135ResetFrmt.h
//  Implementation of the Class SCdiFinDiagDataExecDc135ResetFrmt
//  Created on:      2012/11/15 14:05:24
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataExecDc135ResetFrmt_h
#define SCdiFinDiagDataExecDc135ResetFrmt_h

#define CDIFIN_DIAG_DATA_EXEC_DC135_RESET_SIZE				6


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Counters Messages::SCdiFinDiagDataExecDc135ResetFrmt
// Diagnostic Data Format of "Execute
// Diagnostics / Device HFSI Counter
// Access (dC135) / Reset" Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataExecDc135ResetFrmt
{

	// Chain(Bit15 ~ Bit8)
	UC chain_Octet0;
	// Chain(Bit7 ~ Bit0)
	UC chain_Octet1;
	// Link(Bit15 ~ Bit8)
	UC link_Octet0;
	// Link(Bit7 ~ Bit0)
	UC link_Octet1;
	// Reason For Exchange
	UC reasonExchg;
	// Finisher Device Controller ID
	UC finDevContID;

};
#endif // #ifndef SCdiFinDiagDataExecDc135ResetFrmt_h
