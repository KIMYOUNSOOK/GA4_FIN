// ������ Module Software Platform Frame ������
/// @file    MDL_Main.c
/// @author  Mitsuyuki Nishizawa
/// @date    13.05.05
/// @version 000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define MDL_Main_c

#include "Common.h"
#include "MSP_Service.h"																	// ���̃w�b�_�[�t�@�C���ɂ̓v���_�N�g�ˑ������L�q����Ă���B


//=====================================================================================
// Function Name : MDL_PrintFunctionChange()							[EVENT HANDLER]
// Description   : Print Function Change�J�ڎ��ɃX�P�W���[���̃��Z�b�g�����{����.
// Parameter     : *event (�C�x���g���̎Q�Ɛ�A�h���X)
//               : from (�C�x���g���s�����:��Frame)
// Return        : �Ȃ�
// Caution       : �X�e�[�g��"Print Function Change"�ɑJ�ڂ����ۂ̍ŏ��Ɏ��s�����悤
//               : �Ƀn���h���̓o�^���s�����ƁI�I�I
//=====================================================================================
void MDL_PrintFunctionChange(const UC *pEvent, UC from)
{
	MSP_ResetLocationSchedule() ;															// ���P�[�V������C�x���g��X�P�W���[���̃��Z�b�g����
}


//=====================================================================================
// Function Name : MDL_PrintStandby()									[EVENT HANDLER]
// Description   : Print Standby�J�ڎ��ɃX�P�W���[���̐��������{����.
// Parameter     : *event (�C�x���g���̎Q�Ɛ�A�h���X)
//               : from (�C�x���g���s�����:��Frame)
// Return        : �Ȃ�
// Caution       : �X�e�[�g��"Print Standby"�ɑJ�ڂ����ۂ̍ŏ��Ɏ��s�����悤��
//               : �n���h���̓o�^���s�����ƁI�I�I
//=====================================================================================
void MDL_PrintStandby(const UC *pEvent, UC from)
{
	MSP_CreateLocationSchedule() ;															// ���P�[�V������C�x���g��X�P�W���[���̐�������
	MSP_AbortLocationScheduling() ;															// ���P�[�V������C�x���g���������𒆎~
}


// Change History
// Date:    | Engineer:            | Note:
// ---------+----------------------+---------------------------------------------------------------------------------------------------------------------------
// 13.05.05 | Mitsuyuki Nishizawa  | Module Software Platform Frame�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬.
//


// ******************************************************************  [ MDL_Main.c : EOF ]  ******************************************************************
