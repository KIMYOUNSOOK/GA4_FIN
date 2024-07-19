// File Name: DD_COM_Commondef.h
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 15.03.27
// Revision:  001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev005 Base)
// Revision 000
// 15.03.27 | Hoshino Yuichi      | IOT�̃t�B�j�b�V���[�ԒʐM�K�蔽�f
// Revision 001

#ifndef DD_COM_COMMON_DEF_H 
#define DD_COM_COMMON_DEF_H
//#define DEBUG

#define DD_COM_CONNECTED				0
#define DD_COM_NOT_CONNECTED			0xFE
#define DD_COM_FAIL						0xFF
#define DD_COM_NULL 					0
#define DD_COM_TRUE 					1
#define DD_COM_FALSE 					0

// 1�p�P�b�g�̍ő呗�M�T�C�Y �f�t�H���g
#define DD_COM_MAX_SEND_PACKET_SIZE		64

// �}���`�p�P�b�g���g�p�����p�P�b�g�̍ő��M�T�C�Y�@�f�t�H���g
#define DD_COM_MAX_RECV_DATA_SIZE		256

// 1�p�P�b�g�̍ő�T�C�Y[�~1Byte]
#define DD_COM_MAX_PACKET_SIZE			64
#define DD_COM_MAX_S_PACKET_SIZE		64

// �}���`�p�P�b�g�ő��M����ő呗�M�T�C�Y[�~1Byte]
#define DD_COM_MAX_SEND_DATA_SIZE		256

// ����M�p�P�b�g�L���[��
#define DD_COM_NUMBER_S_COM_PACKET		( 5 ) 
#define DD_COM_NUMBER_S_SEND_COM_PACKET	( 17 )

// SYN���M�J�n����܂ł̎���(ms)
#define DD_COM_SYN_SEND_START_TIME		40

// SYN�đ�����܂ł̎���(ms)
#define DD_COM_SYN_RESEND_TIME			1200

// SYN�đ���
#define DD_COM_MAX_SYN_RESEND			0

// �đ�����(ms)
#define DD_COM_RESEND_TIME				41

// �đ�����(ms)
#define DD_COM_RESEND_CNT				2

// Byte��TimeOut����(9600bps�ȉ��F20ms/19.2Kbps�ȏ�F10ms��6ms)
#define DD_COM_INTER_BYTE_TIME			6

// Polling�Ԋu
#define DD_COM_POLLING_INTARVAL			60000

// ��ԑJ�ڂ̗���(REV.001)
#define DD_COM_STS_LOG_SIZE				64

#endif
