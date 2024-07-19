// ������ Module Software Platform Frame ������
/// @file    MSP_MessageType.h
/// @author  Tsuyoshi Seto
/// @date    14/04/02
/// @time    16:37:26
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// ������������������������������������������������������������������������������������������������������������������������������������������������������������
//  �{�w�b�_�[�t�@�C���̓C�x���g�Ǘ����X�g���玩����������܂��B�i�v���_�N�g����j�[�N�j
// ������������������������������������������������������������������������������������������������������������������������������������������������������������

#ifndef MSP_MessageType_h
#define MSP_MessageType_h


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************
enum ESeqPowerDownType {
	SEQ_PWR_DOWN_TYPE_PWR_OFF,
	SEQ_PWR_DOWN_TYPE_SLEEP
} ;

struct MsgComponentControl {
	US chain ;									//Chain No.(1�`999) 
	US link ;									//Link No.(1�`999)
} ;

struct MsgStaplePositioning {
	UC sheetNo ;								// Finisher���ŊǗ����Ă���V�[�g�ԍ�
	UC previousSheetNo ;						// Finisher���ŊǗ����Ă���O�Z�b�g�V�[�g�ԍ�
} ;

struct SDeviceStatusUpdateInf {
	US statusNo ;
	US  value ;
} ;

enum ETmpHoldReserve {
	TMP_HOLD_SHEET_UNKNOWN ,
	TMP_HOLD_SHEET_RESERVED,
	TMP_HOLD_SHEET_PASS
} ;

enum EStapleFinish {
	UNSTAPLE             ,						// �A���X�e�[�v��
	FRONT_STRAIGHT_STAPLE,						// �t�����g�X�g���C�g�X�e�[�v��
	FRONT_CORNER_STAPLE  ,						// �t�����g�R�[�i�[�X�e�[�v��
	REAR_STRAIGHT_STAPLE ,						// ���A�X�g���C�g�X�e�[�v��
	REAR_CORNER_STAPLE   ,						// ���A�R�[�i�[�X�e�[�v��
	DUAL_STAPLE          ,						// �f���A���X�e�[�v��
	FOUR_POSITION_STAPLE ,						// 4�|�W�V�����X�e�[�v��
	CENTER_STAPLE        ,						// �Z���^�[�X�e�[�v��
	NUM_OF_STAPLE								// Number of staple finish kind
} ;


#endif          // MSP_MessageType_h

// ****************************************************************  [ MSP_MessageType.h : EOF ]  *****************************************************************
