/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataExecDc135WriteFrmt.h
//  Implementation of the Class SCdiFinDiagDataExecDc135WriteFrmt
//  Created on:      2012/11/15 14:18:29
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataExecDc135WriteFrmt_h
#define SCdiFinDiagDataExecDc135WriteFrmt_h

#define CDIFIN_DIAG_DATA_EXEC_DC135_WRITE_SIZE				8


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Counters Messages::SCdiFinDiagDataExecDc135WriteFrmt
// Diagnostic Data Format of "Execute
// Diagnostics / Device HFSI Counter
// Access (dC135) / Write" Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataExecDc135WriteFrmt
{

	// Chain(Bit15 ~ Bit8)
	UC chain_Octet0;
	// Chain(Bit7 ~ Bit0)
	UC chain_Octet1;
	// Link(Bit15 ~ Bit8)
	UC link_Octet0;
	// Link(Bit7 ~ Bit0)
	UC link_Octet1;
	// Life Spec(Bit31 ~ Bit24)
	UC lifeSpec_Octet0;
	// Life Spec(Bit23 ~ Bit16)
	UC lifeSpec_Octet1;
	// Life Spec(Bit15 ~ Bit8)
	UC lifeSpec_Octet2;
	// Life Spec(Bit7 ~ Bit0)
	UC lifeSpec_Octet3;

};

#endif // #ifndef SCdiFinDiagDataExecDc135WriteFrmt_h
