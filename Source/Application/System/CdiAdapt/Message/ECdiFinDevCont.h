/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinDevCont.h
//  Implementation of the Class ECdiFinDevCont
//  Created on:      2012/05/24 20:40:20
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_2473BDEB_C302_4751_90A4_2931B496B094__INCLUDED_)
#define EA_2473BDEB_C302_4751_90A4_2931B496B094__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Capabilities and Configuration::ECdiFinDevCont
// Finisher Device Controller ID(Instance
// ID)
////////////////////////////////////////////////////////////////////////
enum ECdiFinDevCont
{
	// Update All
	eCdiFinDevCont_All = 0x00,
	// CH-Finisher
	eCdiFinDevCont_ChFin = 0x01,
	// D2G-Finisher
	eCdiFinDevCont_D2gFin = 0x02,
	// A-Finisher
	eCdiFinDevCont_AFin = 0x03,
	// SB-Finisher
	eCdiFinDevCont_SbFin = 0x04,
	// D-Finisher
	eCdiFinDevCont_DFin = 0x05,
	// Primary HCS
	eCdiFinDevCont_PriHcs = 0x06,
	// Secondary HCS
	eCdiFinDevCont_SecHcs = 0x07,
	// Perfect Binder
	eCdiFinDevCont_Pb = 0x08,
	// Global A-Finisher
	eCdiFinDevCont_GAFin = 0x09,
	// Global A4-Finisher(Stacker)
	eCdiFinDevCont_GA4Fin = 0x0A,
	// Global A4-Finisher(Mailbox)
	eCdiFinDevCont_GA4Mbx = 0x0B,
	// PFIM
	eCdiFinDevCont_Pfim = 0xFD,
	// I/F Module
	eCdiFinDevCont_Ifm = 0xFE,
	// IOT
	eCdiFinDevCont_Iot = 0xFF
};
#endif // !defined(EA_2473BDEB_C302_4751_90A4_2931B496B094__INCLUDED_)
