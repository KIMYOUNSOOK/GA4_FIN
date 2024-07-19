/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataRespDc301StartFrmt.h
//  Implementation of the Class SCdiFinDiagDataRespDc301StartFrmt
//  Created on:      2012/11/22 14:51:41
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataRespDc301StartFrmt_h
#define SCdiFinDiagDataRespDc301StartFrmt_h

#define CDIFIN_DIAG_DATA_RESP_DC301_START_SIZE				7


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Attributes Messages::SCdiFinDiagDataRespDc301StartFrmt
// Diagnostic Data Format of "Diagnostics
// Response / NVM Initialization (dC301) /
// Start" Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataRespDc301StartFrmt
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
	// Attribute Init Response
	UC attrInitResp;
	// Finisher Device Controller ID
	UC finDevContID;

};

#endif // #ifndef SCdiFinDiagDataRespDc301StartFrmt_h
