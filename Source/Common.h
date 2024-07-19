// File Name: Common.h
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
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev009 Base)
// Revision 000

#ifndef COMMON_H
#define COMMON_H


// �v����
#pragma SFR
#pragma DI
#pragma EI
#pragma NOP
#pragma HALT
#pragma STOP
#pragma inline

#include <string.h>

typedef  unsigned char	UC;
typedef  signed char	SC;
typedef  unsigned short	US;
typedef  signed short	SS;
typedef  unsigned long	UL;
typedef  signed long	SL;
typedef	 unsigned short	ER;
typedef	 unsigned short	ER_UINT;
typedef  void*			VP;
typedef  boolean		bool;

#define NG			(0)
#define OK			(1)

#define false		(0)					// RX�R���p�C���̗\��� true/false�Ƌ��ʉ���}�邽�߂ɏ�����
#define true		(1)

#define RISE		(1)					// Low Active
#define FALL		(0)
#define EDGE_NONE	(2)

#define OFF			(0)
#define	ON			(1)

#define FORWARD		(0)
#define REVERSE		(1)

#define LOW			(0)
#define HIGH		(1)

#define DOWN		(0)
#define UP			(1)

#define CANCELED 	(0xFF)

#define E_OK		0					//!< ����I��.
#define E_NG		(-1)				//!< �G���[
#define E_SYS		(-5)				//!< �V�X�e���G���[.
#define E_NOSPT 	(-9)				//!< ���T�|�[�g�@�\.
#define E_RSFN		(-10)				//!< �\��@�\�R�[�h�ԍ�.
#define E_RSATR 	(-11)				//!< �\�񑮐�.
#define E_PAR		(-17)				//!< �p�����[�^�G���[.
#define E_ID		(-18)				//!< �s��ID�ԍ�.
#define E_CTX		(-25)				//!< �R���e�L�X�g�G���[.
#define E_MACV		(-26)				//!< �������A�N�Z�X�s�\�A�������A�N�Z�X���ᔽ.
#define E_OACV		(-27)				//!< �I�u�W�F�N�g�A�N�Z�X���ᔽ.
#define E_ILUSE		(-28)				//!< �T�[�r�X�R�[���s���g�p.
#define E_NOMEM 	(-33)				//!< �������s��.
#define E_NOID	 	(-34)				//!< ID�ԍ��s��.
#define E_OBJ		(-41)				//!< �I�u�W�F�N�g�̏�Ԃ��s��.
#define E_NOEXS 	(-42)				//!< �I�u�W�F�N�g�����݂��Ȃ�.
#define E_QOVR		(-43)				//!< �L���[�C���O�܂��̓l�X�g�̃I�[�o�[�t���[.
#define E_RLWAI 	(-49)				//!< �҂���ԋ�������.
#define E_TMOUT 	(-50)				//!< �|�[�����O���s�܂��̓^�C���A�E�g.
#define E_DLT		(-51)				//!< �҂��I�u�W�F�N�g���폜���ꂽ.
#define E_CLS		(-52)				//!< �҂��I�u�W�F�N�g�̏�Ԃ��ω�����.
#define E_WBLK		(-57)				//!< �m���u���b�L���O��t��.
#define E_BOVR		(-58)				//!< �o�b�t�@�I�[�o�[�t���[.

#define UC2US(ucAddr) (((US)*(ucAddr)) | ((US)*((ucAddr) + 1)) << 8)
#define GET_BIT_PATTERN(bitNumber) ((UL)0x00000001 << (bitNumber))

// Option��ʂ̌^
typedef enum{
	opBooklet_ID,
	opFolder_ID,
	opNoCare_ID,
	opNone_ID
}OptionID;

extern OptionID SubCPU_OptionID;

#define ANS_ACTIVE_CTRL_TYPE_SIZE 	(sizeof(ANS_ACTIVE_CTRL_TYPE) - 1)		// �o�E���_���̒�����-1

#ifdef DEBUG
#define  DEBUG_PORT_ON		(P4 |=  0x08)	// P43(J1 P02) Port On
#define	 DEBUG_PORT_OFF		(P4 &= ~0x08)	// P43(J1 P02) Port Off
#else
#define  DEBUG_PORT_ON
#define	 DEBUG_PORT_OFF
#endif

extern __far const UL Version;

#endif