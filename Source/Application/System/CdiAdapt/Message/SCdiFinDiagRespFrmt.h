///////////////////////////////////////////////////////////
//  SCdiFinDiagRespFrmt.h
//  Implementation of the Class SCdiFinDiagRespFrmt
//  Created on:      2014/06/18 11:47:06
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinDiagRespFrmt_h)
#define SCdiFinDiagRespFrmt_h

//#define CDIFIN_DIAG_RESP_MSG_SIZE	// ‰Â•Ï
#define CDIFIN_DIAG_RESP_MIN_SIZE	6


struct SCdiFinDiagRespFrmt
{
	UC id;																		// Message ID
	UC diagProgram_MSB;															// Diagnostic Program(Bit15 ~ Bit8)
	UC diagProgram_LSB;															// Diagnostic Program(Bit7 ~ Bit0)
	UC diagCommand;																// Diagnostic Command
	UC diagResult;																// Diagnostic Result
	UC diagDataLength;															// Diagnostic Data Length
	UC diagData[CDIFIN_MAXMSGSIZE - CDIFIN_DIAG_RESP_MIN_SIZE];					// Diagnostic Data
};

#endif // !defined(SCdiFinDiagRespFrmt_h)
