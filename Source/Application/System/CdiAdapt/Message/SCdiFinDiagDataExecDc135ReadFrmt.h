/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataExecDc135ReadFrmt.h
//  Implementation of the Class SCdiFinDiagDataExecDc135ReadFrmt
//  Created on:      2012/11/22 14:56:50
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataExecDc135ReadFrmt_h
#define SCdiFinDiagDataExecDc135ReadFrmt_h

#define CDIFIN_DIAG_DATA_EXEC_DC135_READ_SIZE				4


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Counters Messages::SCdiFinDiagDataExecDc135ReadFrmt
// Diagnostic Data Format of "Execute
// Diagnostics / Device HFSI Counter
// Access (dC135) / Read" Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataExecDc135ReadFrmt
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

#endif // #ifndef SCdiFinDiagDataExecDc135ReadFrmt_h
