/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataExecDc131ReadFrmt.h
//  Implementation of the Class SCdiFinDiagDataExecDc131ReadFrmt
//  Created on:      2012/11/22 14:29:11
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataExecDc131ReadFrmt_h
#define SCdiFinDiagDataExecDc131ReadFrmt_h

#define CDIFIN_DIAG_DATA_EXEC_DC131_READ_SIZE				4


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Attributes Messages::SCdiFinDiagDataExecDc131ReadFrmt
// Diagnostic Data Format of "Execute
// Diagnostics / NVM Access (dC131) /
// Read" Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataExecDc131ReadFrmt
{
	// Chain(Bit15 ~ Bit8)
	UC chain_Octet0;
	// Chain(Bit7 ~ Bit0)
	UC chain_Octet1;
	// Link(Bit15 ~ Bit8)
	UC link_Octet0;
	// Link(Bit7 ~ Bit0)
	UC link_Octet1;
};

#endif // #ifndef SCdiFinDiagDataExecDc131ReadFrmt_h
