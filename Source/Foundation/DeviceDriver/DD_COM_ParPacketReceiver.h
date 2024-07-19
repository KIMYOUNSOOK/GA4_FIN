// File Name: DD_COM_ParPacketReceiver.h
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
// 13.10.15 | Mitsuyuki Nishizawa	| Created this file.(CH-Finisher SubCPU Rev000 Base)
// Revision 000
// 16.02.09	| Yuichi Hoshino		| Download対応のためのシーケンスビット取得関数追加
// Revision 001


#ifndef DD_COM_PAR_PACKET_RECWIVER_H
#define DD_COM_PAR_PACKET_RECWIVER_H

void DD_COM_InitParPacketReceiver( );
void DD_COM_RecvErr();		//受信エラー処理
UC DD_COM_GetReceiverSequenceBit();

//EStatus state;
void DD_COM_ParPacketReceiverNotify(DD_EventID eventId, UC* pMsg);

#endif // DD_COM_PAR_PACKET_RECWIVER_H
