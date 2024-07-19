///////////////////////////////////////////////////////////
//  SCdiFinRdyForNxtShtFrmt.h
//  Implementation of the Class SCdiFinRdyForNxtShtFrmt
//  Created on:      2013/12/06 15:45:03
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinRdyForNxtShtFrmt_h)
#define SCdiFinRdyForNxtShtFrmt_h

#define CDIFIN_RDY_FOR_NEXT_SHT_MSG_SIZE					6


struct SCdiFinRdyForNxtShtFrmt
{
	UC id;								// Message ID
	UC setID;							// Set ID
	UC sheetID;							// Sheet ID
	UC prepTime_Octet0;					// Prep Time (Bit 15 Å` Bit 8)
	UC prepTime_Octet1;					// Prep Time (Bit 7 Å` Bit 0)
	UC disperTime;						// Dispersion Time
};

#endif // !defined(SCdiFinRdyForNxtShtFrmt_h)
