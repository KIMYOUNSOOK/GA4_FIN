// File Name: DD_COM_Service.h
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 16.02.06
// Revision:  001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev000 Base)
// Revision 000
// 16.02.08	| Hoshino Yuichi      | Download�Ή��̂��߂̃V�[�P���X�r�b�g�擾�֐��ǉ�
// Revision 001

#ifndef DD_COM_SERVICE_H 
#define DD_COM_SERVICE_H 

// MSP�ւ̒񋟊֐�
void DD_COM_Init(void);
void DD_COM_EntryReceiver (void (*func)( UC * RcvDataAddress, US datalength));

// �A�v���ւ̒񋟊֐�
SL DD_COM_SendMsg(const UC* a_pchDataAddress, US a_nDataLength);
UC DD_COM_GetRxSequenceBit(void);
UC DD_COM_GetTxSequenceBit(void);

#endif 
