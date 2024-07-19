/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataExecDc131WriteFrmt.h
//  Implementation of the Class SCdiFinDiagDataExecDc131WriteFrmt
//  Created on:      2012/11/22 14:34:19
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataExecDc131WriteFrmt_h
#define SCdiFinDiagDataExecDc131WriteFrmt_h

#define CDIFIN_DIAG_DATA_EXEC_DC131_WRITE_SIZE				9


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Attributes Messages::SCdiFinDiagDataExecDc131WriteFrmt
// Diagnostic Data Format of "Execute
// Diagnostics / NVM Access (dC131) /
// Write" Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataExecDc131WriteFrmt
{

	// Chain(Bit15 ~ Bit8)
	UC chain_Octet0;
	// Chain(Bit7 ~ Bit0)
	UC chain_Octet1;
	// Link(Bit15 ~ Bit8)
	UC link_Octet0;
	// Link(Bit7 ~ Bit0)
	UC link_Octet1;
	// Data Type
	UC dataType;
	// NVM Value(Bit31 ~ Bit24)
	UC nvmVal_Octet0;
	// NVM Value(Bit23 ~ Bit16)
	UC nvmVal_Octet1;
	// NVM Value(Bit15 ~ Bit8)
	UC nvmVal_Octet2;
	// NVM ValueLink(Bit7 ~ Bit0)
	UC nvmVal_Octet3;

};

#endif // #ifndef SCdiFinDiagDataExecDc131WriteFrmt_h
