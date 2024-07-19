/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinDiagProgram.h
//  Implementation of the Class ECdiFinDiagProgram
//  Created on:      28-2-2012 17:56:18
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_77AC3F06_1296_4f7c_8EB9_72E1FBA496AA__INCLUDED_)
#define EA_77AC3F06_1296_4f7c_8EB9_72E1FBA496AA__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Controller Diagnostics::ECdiFinDiagProgram
// Diagnostic Program
////////////////////////////////////////////////////////////////////////
enum ECdiFinDiagProgram
{
	// Fold Position Adjust
	eCdiFinDiagPrgrm_dC128 = 128,
	// NVM Access
	eCdiFinDiagPrgrm_dC131 = 131,
	// Device HFSI Counter Access
	eCdiFinDiagPrgrm_dC135 = 135,
	// Analog Component Monitor
	eCdiFinDiagPrgrm_dC140 = 140,
	// NVM Initialization
	eCdiFinDiagPrgrm_dC301 = 301,
	// Component Control
	eCdiFinDiagPrgrm_dC330 = 330,
	// NVM Area Access
	eCdiFinDiagPrgrm_dC361 = 361,
	// Paper Path Timing
	eCdiFinDiagPrgrm_dC726 = 726,
	// Recycle Information Access
	eCdiFinDiagPrgrm_dC1100 = 1100,
	// User Fold Position Adjust
	eCdiFinDiagPrgrm_dC1201 = 1201,
	// User Punch Position Adjust
	eCdiFinDiagPrgrm_dC1202 = 1202
};
#endif // !defined(EA_77AC3F06_1296_4f7c_8EB9_72E1FBA496AA__INCLUDED_)
