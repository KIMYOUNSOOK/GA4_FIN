// File Name: DD_COM_ParPacketSender.h
// Description:
// Engineer:  Yuichi Hoshino
// Last Edit: 16.02.09
// Revision:  001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:				| Note:
// ---------+-----------------------+------------------------------------------------------------------------------------
// 13.10.15 | Nishizawa Mitsuyuki	| Created this file.(CH-Finisher SubCPU Rev000 Base)
// Revision 000
// 16.02.09	| Yuichi Hoshino		| Download対応のためのシーケンスビット取得関数追加
// Revision 001

#ifndef DD_COM_PARPACKET_SENDER_H
#define DD_COM_PARPACKET_SENDER_H

void DD_COM_InitParPacketSender();
UC DD_COM_GetSenderSequenceBit();

void DD_COM_ParPacketSenderNotify(DD_EventID eventId, UC* pMsg);

void DD_COM_OpComplete_INTERRUPT(void);

#endif // COMPACKETSENDER_H
