/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataRespDc330WriteFrmt.h
//  Implementation of the Class SCdiFinDiagDataRespDc330WriteFrmt
//  Created on:      2012/11/22 15:02:33
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataRespDc330WriteFrmt_h
#define SCdiFinDiagDataRespDc330WriteFrmt_h

#define CDIFIN_DIAG_DATA_RESP_DC330_WRITE_SIZE				5


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Component Control Messages::SCdiFinDiagDataRespDc330WriteFrmt
// Diagnostic Data Format of "Diagnostics
// Response / NVM Access(dC330) / Write"
// Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataRespDc330WriteFrmt
{

	// Chain(Bit15 ~ Bit8)
	UC chain_Octet0;
	// Chain(Bit7 ~ Bit0)
	UC chain_Octet1;
	// Link(Bit15 ~ Bit8)
	UC link_Octet0;
	// Link(Bit7 ~ Bit0)
	UC link_Octet1;
	// Component Response
	UC componentResp;

};

#endif // #ifndef SCdiFinDiagDataRespDc330WriteFrmt_h
