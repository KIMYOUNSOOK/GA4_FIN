// File Name: DD_INT.h
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
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev001 Base)
// Revision 000
#ifndef DD_INT_H
#define DD_INT_H

typedef union {
	UC BYTE;
	struct {
		UC id					: 4;	// 
		UC reserved				: 1;
		UC index				: 2;	// �o�^��̃R�[���o�b�N�z��Index(Cancel���ɕK�v)
		UC edge					: 1;
	}BIT;
}DD_INT_TYPE;

void DD_INT_Init(void);
UC DD_INT_EntryCallback(UC intCH, UC edge, void (*func)(void*), void* arg);
void DD_INT_CancelCallback(UC intCH, UC edge, UC index);
UC DD_INT_GetCurrentLevel(UC intCH);

#endif
