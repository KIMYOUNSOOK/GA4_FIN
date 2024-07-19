/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataRespDc131ReadFrmt.h
//  Implementation of the Class SCdiFinDiagDataRespDc131ReadFrmt
//  Created on:      2012/11/22 14:29:57
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataRespDc131ReadFrmt_h
#define SCdiFinDiagDataRespDc131ReadFrmt_h

#define CDIFIN_DIAG_DATA_RESP_DC131_READ_SIZE				18


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Attributes Messages::SCdiFinDiagDataRespDc131ReadFrmt
// Diagnostic Data Format of "Diagnostics
// Response / NVM Access (dC131) / Read"
// Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataRespDc131ReadFrmt
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
	// Attribute Characteristic
	UC attrChar;
	// Current Value(Bit31Å`Bit24)
	UC crntVal_Octet0;
	// Current Value(Bit16Å`Bit23)
	UC crntVal_Octet1;
	// Current Value(Bit8Å`Bit15)
	UC crntVal_Octet2;
	// Current Value(Bit0Å`Bit7)
	UC crntVal_Octet3;
	// Minimum Value(Bit31Å`Bit24)
	UC minVal_Octet0;
	// Minimum Value(Bit16Å`Bit23)
	UC minVal_Octet1;
	// Minimum Value(Bit8Å`Bit15)
	UC minVal_Octet2;
	// Minimum Value(Bit0Å`Bit7)
	UC minVal_Octet3;
	// Max Value(Bit31Å`Bit24)
	UC maxVal_Octet0;
	// Max Value(Bit16Å`Bit23)
	UC maxVal_Octet1;
	// Max Value(Bit8Å`Bit15)
	UC maxVal_Octet2;
	// Max Value(Bit0Å`Bit7)
	UC maxVal_Octet3;
};

#endif // #ifndef SCdiFinDiagDataRespDc131ReadFrmt_h
