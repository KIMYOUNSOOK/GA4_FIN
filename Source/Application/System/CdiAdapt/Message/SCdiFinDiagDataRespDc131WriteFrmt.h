/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataRespDc131WriteFrmt.h
//  Implementation of the Class SCdiFinDiagDataRespDc131WriteFrmt
//  Created on:      2012/11/22 14:37:05
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataRespDc131WriteFrmt_h
#define SCdiFinDiagDataRespDc131WriteFrmt_h

#define CDIFIN_DIAG_DATA_RESP_DC131_WRITE_SIZE				5


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Attributes Messages::SCdiFinDiagDataRespDc131WriteFrmt
// Diagnostic Data Format of "Diagnostics
// Response / NVM Access (dC131) / Write"
// Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataRespDc131WriteFrmt
{

	// Chain(Bit15 ~ Bit8)
	UC chain_Octet0;
	// Chain(Bit7 ~ Bit0)
	UC chain_Octet1;
	// Link(Bit15 ~ Bit8)
	UC link_Octet0;
	// Link(Bit7 ~ Bit0)
	UC link_Octet1;
	// Update Response
	UC updateResp;

};

#endif // #ifndef SCdiFinDiagDataRespDc131WriteFrmt_h
