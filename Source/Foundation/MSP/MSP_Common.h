// ������ Module Software Platform Frame ������
/// @file    MSP_Common.h
/// @author  Mitsuyuki Nishizawa
/// @date    13.11.05
/// @version 001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef MSP_Common_h
#define MSP_Common_h


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

// ========================  <<< �R���t�B�M�����[�V�����֘A���ʒ�` >>>  ========================				������ �v���_�N�g����j�[�N ������

// ----- �I�v�V�����E�R���t�B�O -----
#define CONFIG_BASE			0x00		// �I�v�V�����Ȃ�
#define CONFIG_BOOK			0x01		// �I�v�V�����Ȃ� (w/ �u�b�N���b�g)
#define CONFIG_PFIM			0x02		// �I�v�V�����Ȃ� (for PFIM)

#define CONFIG_PUNCH		0x04		// �p���`����j�b�g (w/ �g�����X�|�[�g)
#define CONFIG_2IP_1		0x08		// 2�i�C���^�[�|�[�U�[ (prev. Finisher)
#define CONFIG_FOLDER		0x10		// �t�H���_�[����j�b�g
#define CONFIG_SQ_FLD		0x20		// �g���}�[&�X�N�G�A��t�H�[���h
#define CONFIG_TCBM			0x40		// �g���}�[��N���[�X��o�b�t�@����W���[��
#define CONFIG_2IP_2		0x80		// 2�i�C���^�[�|�[�U�[ (prev. TCBM)

// ----- �o�b�t�@���� -----
#define BUFF_NOT_EXEC		0			// �o�b�t�@��Ώۗp��
#define BUFF_AT_FIN			1			// �o�b�t�@�Ώۗp�����t�B�j�b�V���[
#define BUFF_AT_TCBM		3			// �o�b�t�@�Ώۗp����TCBM

// ----- �����g���C -----
#define IOT					0			// IOT (or Other Device)
#define INSERTER1			1			// Inserter Tray1 (Upper)
#define INSERTER2			2			// Inserter Tray2 (Lower)

// ----- �p���r�o�� -----
#define TOP					0			// Top Tray
#define STACKER				1			// Stacker Tray
#define BOOKLET				2			// Booklet
#define FOLDER 				3			// Folder (Envelope)
#define MAILBOX 			4			// Mailbox Bin
#define PFIM				5			// PFIM


// ========================  <<< MSP/IOT�A�v���P�[�V�������ʒ�` >>>  ========================

// ----- �ėp��` -----
#define UNKNOWN		0

#define CLEAR		0
#define SET			1

#define INACTIVE	0
#define ACTIVE		1

// ----- I/O�G�b�W��` -----
//#define OFF			0
//#define ON			1

//#define FALL		0
//#define RISE		1

// ----- �f�o�b�O����O�o�͐؂�ւ� -----
#define LOG_OFF		0
#define LOG_ON		1

// ----- �ʐM�v���C�I���e�B�[ -----
#define	PRI_NORM	0
#define PRI_HIGH	1

// ===========================  <<< �}�N����` >>>  ===========================

// ----- �������� (�ėp) -----
#define	MSP_OK				1																// ��������(OK)
#define	MSP_NG				0																// �������s(NG)

// ----- �^�C�}�[�������� -----
#define	MSP_TMR_OK			0
#define	MSP_TMR_ERR			(-1)

// ----- ���P�[�V������^�C�}�[�֘A -----
#define	MSP_LOC_TMR_VOID	0x7FFF															// �^�C�}�[ID�F�����^�C�}�[ID
#define	MSP_LOC_TMR_FULL	0xFFFF															// �^�C�}�[����\�[�X�ɋ󂫂Ȃ�

// ----- �����ȃC�x���g��i���o�[ -----
#define	MSP_EVT_NUM_VOID	0x03FF															// �C�x���g��i���o�[��"1023"�͎g�p�s�I�I�I


#endif			// MSP_Common_h


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 13.05.05 | Mitsuyuki Nishizawa | Module Software Platform Frame�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬.
// 13.05.05 | Mitsuyuki Nishizawa | �^�C�}�[�������ʂ�ǉ�.�iEvent Timer�w�b�_�[����ړ��j
//
//


// *****************************************************************  [ MSP_Common.h : EOF ]  *****************************************************************
