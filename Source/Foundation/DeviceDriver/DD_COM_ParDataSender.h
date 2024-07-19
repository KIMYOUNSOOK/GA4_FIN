// File Name: DD_COM_ParDataSender.h
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 13.10.15
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev000 Base)
// Revision 000

#ifndef COM_PAR_DATA_SENDER_H
#define COM_PAR_DATA_SENDER_H

void DD_COM_InitParDataSender();
void DD_COM_ParDataSenderNotify(DD_EventID eventId,void *arg);
	
#ifndef PARDATASENDER_C
extern US gComPacketMaxLength;
extern US ms_nMaxPacketSize;
#endif
	
#endif // COM_PAR_DATA_SENDER_H
