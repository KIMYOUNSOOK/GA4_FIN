/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataRespDc135ReadFrmt.h
//  Implementation of the Class SCdiFinDiagDataRespDc135ReadFrmt
//  Created on:      2012/11/15 13:30:52
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataRespDc135ReadFrmt_h
#define SCdiFinDiagDataRespDc135ReadFrmt_h

#define CDIFIN_DIAG_DATA_RESP_DC135_READ_SIZE				47


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Counters Messages::SCdiFinDiagDataRespDc135ReadFrmt
// Diagnostic Data Format of "Diagnostics
// Response / Device HFSI Counter Access
// (dC135) / Read" Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataRespDc135ReadFrmt
{

	// Chain(Bit15 ~ Bit8)
	UC chain_Octet0;
	// Chain(Bit7 ~ Bit0)
	UC chain_Octet1;
	// Link(Bit15 ~ Bit8)
	UC link_Octet0;
	// Link(Bit7 ~ Bit0)
	UC link_Octet1;
	// Counter Value(Bit31 ~ Bit24)
	UC counterVal_Octet0;
	// Counter Value(Bit23 ~ Bit16)
	UC counterVal_Octet1;
	// Counter Value(Bit15 ~ Bit8)
	UC counterVal_Octet2;
	// Counter Value(Bit7 ~ Bit0)
	UC counterVal_Octet3;
	// Life Spec(Bit31 ~ Bit24)
	UC lifeSpec_Octet0;
	// Life Spec(Bit23 ~ Bit16)
	UC lifeSpec_Octet1;
	// Life Spec(Bit15 ~ Bit8)
	UC lifeSpec_Octet2;
	// Life Spec(Bit7 ~ Bit0)
	UC lifeSpec_Octet3;
	// Reason For Last Exchange
	UC reasonLastExchg;
	// Counter Value At Last Exchange(Bit31 ~
	// Bit24)
	UC counterValAtLast_Octet0;
	// Counter Value At Last Exchange(Bit23 ~
	// Bit16)
	UC counterValAtLast_Octet1;
	// Counter Value At Last Exchange(Bit15 ~
	// Bit8)
	UC counterValAtLast_Octet2;
	// Counter Value At Last Exchange(Bit7 ~
	// Bit0)
	UC counterValAtLast_Octet3;
	// Reason For Last Exchange But One
	UC reasonLastExchgButOne;
	// Counter Value At Last Exchange But
	// One(Bit31 ~ Bit24)
	UC counterValLastButOne_Octet0;
	// Counter Value At Last Exchange But
	// One(Bit23 ~ Bit16)
	UC counterValLastButOne_Octet1;
	// Counter Value At Last Exchange But
	// One(Bit15 ~ Bit8)
	UC counterValLastButOne_Octet2;
	// Counter Value At Last Exchange But
	// One(Bit7 ~ Bit0)
	UC counterValLastButOne_Octet3;
	// Reason For Last Exchange But Two
	UC reasonLastExchgButTwo;
	// Counter Value At Last Exchange But
	// Two(Bit31 ~ Bit24)
	UC counterValLastButTwo_Octet0;
	// Counter Value At Last Exchange But
	// Two(Bit23 ~ Bit16)
	UC counterValLastButTwo_Octet1;
	// Counter Value At Last Exchange But
	// Two(Bit15 ~ Bit8)
	UC counterValLastButTwo_Octet2;
	// Counter Value At Last Exchange But
	// Two(Bit7 ~ Bit0)
	UC counterValLastButTwo_Octet3;
	// Counter Name
	UC counterName[20];

};
#endif // #ifndef SCdiFinDiagDataRespDc135ReadFrmt_h
