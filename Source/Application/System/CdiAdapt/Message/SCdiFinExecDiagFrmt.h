///////////////////////////////////////////////////////////
//  SCdiFinExecDiagFrmt.h
//  Implementation of the Class SCdiFinExecDiagFrmt
//  Created on:      2013/12/04 11:47:06
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinExecDiagFrmt_h)
#define SCdiFinExecDiagFrmt_h

//#define CDIFIN_EXEC_DIAG_MSG_SIZE	// ‰Â•Ï
#define CDIFIN_EXEC_DIAG_MIN_SIZE	5


struct SCdiFinExecDiagFrmt
{
	UC id;																		// Message ID
	UC diagProgram_MSB;															// Diagnostic Program(Bit15 ~ Bit8)
	UC diagProgram_LSB;															// Diagnostic Program(Bit7 ~ Bit0)
	UC diagCommand;																// Diagnostic Command
	UC diagDataLength;															// Diagnostic Data Length
	UC diagData[CDIFIN_MAXMSGSIZE - CDIFIN_EXEC_DIAG_MIN_SIZE];					// Diagnostic Data
};

#endif // !defined(SCdiFinExecDiagFrmt_h)
