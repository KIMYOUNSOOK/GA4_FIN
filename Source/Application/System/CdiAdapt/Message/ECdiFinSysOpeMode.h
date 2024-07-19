/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinSysOpeMode.h
//  Implementation of the Class ECdiFinSysOpeMode
//  Created on:      2012/05/24 21:14:22
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_7026F397_FB65_4428_98AC_4267237B7FC4__INCLUDED_)
#define EA_7026F397_FB65_4428_98AC_4267237B7FC4__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Controller Condition::ECdiFinSysOpeMode
// System Operating Mode
////////////////////////////////////////////////////////////////////////
enum ECdiFinSysOpeMode
{
	// Normal
	eCdiFinSysOpeMode_Nrml = 1,
	// Service Diagnostics
	eCdiFinSysOpeMode_SrvcDiag = 3,
	// Software Download
	eCdiFinSysOpeMode_SoftDL = 4,
	// Unknown
	eCdiFinSysOpeMode_Unknown = 5
};
#endif // !defined(EA_7026F397_FB65_4428_98AC_4267237B7FC4__INCLUDED_)
