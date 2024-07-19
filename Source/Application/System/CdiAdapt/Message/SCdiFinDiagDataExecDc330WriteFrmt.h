/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataExecDc330WriteFrmt.h
//  Implementation of the Class SCdiFinDiagDataExecDc330WriteFrmt
//  Created on:      2012/11/22 15:01:54
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataExecDc330WriteFrmt_h
#define SCdiFinDiagDataExecDc330WriteFrmt_h

#define CDIFIN_DIAG_DATA_EXEC_DC330_WRITE_SIZE				5


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Component Control Messages::SCdiFinDiagDataExecDc330WriteFrmt
// Diagnostic Data Format of "Execute
// Diagnostics / Digital Component Control
// (dC330) / Write" Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataExecDc330WriteFrmt
{

	// Chain(Bit15 ~ Bit8)
	UC chain_Octet0;
	// Chain(Bit7 ~ Bit0)
	UC chain_Octet1;
	// Link(Bit15 ~ Bit8)
	UC link_Octet0;
	// Link(Bit7 ~ Bit0)
	UC link_Octet1;
	// Component State
	UC componentStt;

};

#endif // #ifndef SCdiFinDiagDataExecDc330WriteFrmt_h
