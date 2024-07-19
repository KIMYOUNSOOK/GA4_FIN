/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinDiagResult.h
//  Implementation of the Class ECdiFinDiagResult
//  Created on:      2012/05/25 16:10:31
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_D8FCC438_CFFF_467d_AFF1_0D6C99A18289__INCLUDED_)
#define EA_D8FCC438_CFFF_467d_AFF1_0D6C99A18289__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Controller Diagnostics::ECdiFinDiagResult
// Diagnostic Result
////////////////////////////////////////////////////////////////////////
enum ECdiFinDiagResult
{
	eCdiFinDiagRslt_Successful = 0x00,
	eCdiFinDiagRslt_NoSuchDiagPrgrm = 0x02,
	eCdiFinDiagRslt_NoSuchDiagCmd = 0x03,
	eCdiFinDiagRslt_FailedData = 0x08,
	eCdiFinDiagRslt_FailedIdentifier = 0x09,
	eCdiFinDiagRslt_AbnormalEnd = 0x0A
};
#endif // !defined(EA_D8FCC438_CFFF_467d_AFF1_0D6C99A18289__INCLUDED_)
