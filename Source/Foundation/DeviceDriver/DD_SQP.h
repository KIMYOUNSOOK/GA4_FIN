// File Name: DD_SQP.h
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 12.10.04
// Revision:  006
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 12.7.5   | Nishizawa Mitsuyuki | Created this file.
// Revision 000

#ifndef DD_SQP_H
#define DD_SQP_H

typedef enum{
	DD_SQP_ID_TRANSPORT,			// Transport Motor Clock
	DD_SQP_ID_NUM
} DD_SQP_ID;

// ƒAƒvƒŠ’ñ‹ŸAPI
void DD_SQP_Init(void);
void DD_SQP_SetFrequency(UC motorID, UL frequency);
void DD_SQP_Start(UC motorID);
void DD_SQP_Stop(UC motorID);

#endif