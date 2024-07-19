// File Name: DD_DMA.h
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

#ifndef DD_DMA_H
#define DD_DMA_H

#define DD_DMA_OK			0
#define DD_DMA_PARAM_NG		(-1)

typedef enum {										// DMA‹N“®—vˆöŽí•Ê
	DD_DMA_TRIGGER_TYPE_SOFTWARE,
	DD_DMA_TRIGGER_TYPE_AD,
	DD_DMA_TRIGGER_TYPE_TM,
	DD_DMA_TRIGGER_TYPE_UART_SEND,
	DD_DMA_TRIGGER_TYPE_UART_RECV
} DD_DMA_TRIGGER_TYPE;

SS DD_DMA_InitByChannel(UC dmaCh, DD_DMA_TRIGGER_TYPE type, UC trgCh);
SS DD_DMA_Start(UC ch, US* startAddr, US cnt);
SS DD_DMA_Stop(UC ch);
US DD_DMA_GetRemainTransferCnt(UC ch);
SS DD_DMA_EntryCallback(UC ch, void (*func)(void* arg), void* arg);

#endif
