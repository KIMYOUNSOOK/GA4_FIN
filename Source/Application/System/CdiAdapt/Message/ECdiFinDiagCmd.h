/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinDiagCmd.h
//  Implementation of the Class ECdiFinDiagCmd
//  Created on:      12-2-2012 15:54:54
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_CB5A00CE_630A_4b84_B425_95660D5CD9D6__INCLUDED_)
#define EA_CB5A00CE_630A_4b84_B425_95660D5CD9D6__INCLUDED_

enum ECdiFinDiagCmd
{
	eCdiFinDiagCmd_Start = 0x00,
	eCdiFinDiagCmd_Stop = 0x01,
	eCdiFinDiagCmd_Read = 0x02,
	eCdiFinDiagCmd_Write = 0x03,
	eCdiFinDiagCmd_Monitor = 0x04,
	eCdiFinDiagCmd_Reset = 0x09
};
#endif // !defined(EA_CB5A00CE_630A_4b84_B425_95660D5CD9D6__INCLUDED_)
