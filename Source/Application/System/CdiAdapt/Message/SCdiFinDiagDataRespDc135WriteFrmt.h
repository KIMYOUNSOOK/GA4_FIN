/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataRespDc135WriteFrmt.h
//  Implementation of the Class SCdiFinDiagDataRespDc135WriteFrmt
//  Created on:      2012/11/15 14:23:00
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataRespDc135WriteFrmt_h
#define SCdiFinDiagDataRespDc135WriteFrmt_h

#define CDIFIN_DIAG_DATA_RESP_DC135_WRITE_SIZE				4


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Counters Messages::SCdiFinDiagDataRespDc135WriteFrmt
// Diagnostic Data Format of "Diagnostics
// Response / Device HFSI Counter Access
// (dC135) / Write" Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataRespDc135WriteFrmt
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

#endif // #ifndef SCdiFinDiagDataRespDc135WriteFrmt_h
