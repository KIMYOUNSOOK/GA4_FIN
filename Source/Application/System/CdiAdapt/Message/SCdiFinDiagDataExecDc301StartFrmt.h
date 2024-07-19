/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataExecDc301StartFrmt.h
//  Implementation of the Class SCdiFinDiagDataExecDc301StartFrmt
//  Created on:      2012/11/22 14:38:09
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataExecDc301StartFrmt_h
#define SCdiFinDiagDataExecDc301StartFrmt_h

#define CDIFIN_DIAG_DATA_EXEC_DC301_STRAT_SIZE				6


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Attributes Messages::SCdiFinDiagDataExecDc301StartFrmt
// Diagnostic Data Format of "Execute
// Diagnostics / NVM Initialization
// (dC301) / Start" Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataExecDc301StartFrmt
{

	// Chain(Bit15 ~ Bit8)
	UC chain_Octet0;
	// Chain(Bit7 ~ Bit0)
	UC chain_Octet1;
	// Link(Bit15 ~ Bit8)
	UC link_Octet0;
	// Link(Bit7 ~ Bit0)
	UC link_Octet1;
	// Attribute Init Type
	UC attrInitType;
	// Finisher Device Controller ID
	UC finDevContID;

};

#endif // #ifndef SCdiFinDiagDataExecDc301StartFrmt_h
