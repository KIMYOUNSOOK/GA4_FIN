// File Name:	ECMPL_TMP_Action.h
// Description:	Tamper Device �̓�����`
// Engineer:	FXK
// Last Edit:	15.3.12.
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(ECMPL_TMP_Action_h)
#define ECMPL_TMP_Action_h

typedef enum /* ECMPL_TMP_Action */
{

	// �^���p�[��~
	CMPL_TMP_st0,	//IDLE

	// �^���p�[����(Compo) �J�n
	// Right Tamper Component���� (Link No)
	CMPL_TMP_COMPO_RIGHT_CW_LOW  = 30,	// RIGHT Tamper���O������(CW)�ɒᑬ(643PPS)�ňړ�
	CMPL_TMP_COMPO_RIGHT_CW_MID  = 31,	// RIGHT Tamper���O������(CW)�ɒ���(800PPS)�ňړ�
	CMPL_TMP_COMPO_RIGHT_CW_HI   = 32,	// RIGHT Tamper���O������(CW)�ɍ���(1337PPS)�ňړ�

	CMPL_TMP_COMPO_RIGHT_CCW_LOW = 33,	// RIGHT Tamper���������(CCW)�ɒᑬ(643PPS)�ňړ�
	CMPL_TMP_COMPO_RIGHT_CCW_MID = 34,	// RIGHT Tamper���������(CCW)�ɒ���(800PPS)�ňړ�
	CMPL_TMP_COMPO_RIGHT_CCW_HI  = 35,	// RIGHTTamper���������(CCW)�ɍ���(1337PPS)�ňړ�
	CMPL_TMP_COMPO_RIGHT_SENSOR  = 36,	// For Test

	// Left Tamper Component���� (Link No)
	CMPL_TMP_COMPO_LEFT_CW_LOW   = 40, 	// LEFT Tamper���O������(CW)�ɒᑬ(643PPS)�ňړ�
	CMPL_TMP_COMPO_LEFT_CW_MID   = 41, 	// LEFT Tamper���O������(CW)�ɒ���(800PPS)�ňړ�
	CMPL_TMP_COMPO_LEFT_CW_HI    = 42, 	// LEFT Tamper���O������(CW)�ɍ���(1337PPS)�ňړ�

	CMPL_TMP_COMPO_LEFT_CCW_LOW  = 43, 	// LEFT Tamper���������(CCW)�ɒᑬ(643PPS)�ňړ�
	CMPL_TMP_COMPO_LEFT_CCW_MID  = 44, 	// LEFT Tamper���������(CCW)�ɒ���(800PPS)�ňړ�
	CMPL_TMP_COMPO_LEFT_CCW_HI   = 45, 	// LEFT Tamper���������(CCW)�ɍ���(1337PPS)�ňړ�
	CMPL_TMP_COMPO_LEFT_SENSOR   = 46,	// For Test

	// Component����
	CMPL_TMP_COMPO_TAMPER_MOT = 0x0100,
	CMPL_TMP_COMPO_TAMPER_MOT_st1,  // Mot �쓮�ݒ�, �O�㎥����
	CMPL_TMP_COMPO_TAMPER_MOT_st2,  // ����J�n100Pulse ��, StepDown
	CMPL_TMP_COMPO_TAMPER_MOT_st3,	// ��㎥����
	CMPL_TMP_COMPO_TAMPER_MOT_st4, 	// Mot Off

	// �T�C�Y�|�W�V���j���O����
	CMPL_TMP_SIZE_POS = 0x0200,
	CMPL_TMP_SIZE_POS_st1, 			// Tamper HomeSNR�@ON�`�F�b�N
	CMPL_TMP_SIZE_POS_st2_CCW, 		// Home�O������ݒ�
	CMPL_TMP_SIZE_POS_st3_CCW, 		// CCW�����Ƀ��[�^�쓮�J�n
	CMPL_TMP_SIZE_POS_st4_CCW, 		// Home�O�����슮��
	CMPL_TMP_SIZE_POS_st2_CW, 		// Home����ݒ�
	CMPL_TMP_SIZE_POS_st3_CW, 		// CW�����Ƀ��[�^�쓮�J�n
	CMPL_TMP_SIZE_POS_st4_CW, 		// StepUp���A�����ݎ�M
	CMPL_TMP_SIZE_POS_st4_2_CW, 	// StepUp���A�����ݎ�M ���� StepDown����
	CMPL_TMP_SIZE_POS_st5_CW, 		// StepUp������M
	CMPL_TMP_SIZE_POS_st6_CW, 		// �����ԂŊ����ݎ�M
	CMPL_TMP_SIZE_POS_st7_CW, 		// Home���슮��
	CMPL_TMP_SIZE_POS_st8,   		// SizePositioning�J�n
	CMPL_TMP_SIZE_POS_st9,   		// ���[�^�쓮�J�n
	CMPL_TMP_SIZE_POS_st10,   		// SizePosition�ֈړ�����



	// �m�[�}���^���s���O����
	CMPL_TMP_NORMAL_TAMPING = 0x0300,
	CMPL_TMP_NORMAL_TAMPING_st1, 	// ����J�n�x�����Ԑݒ�
	CMPL_TMP_NORMAL_TAMPING_st2, 	// ���[�^�쓮�ݒ�
	CMPL_TMP_NORMAL_TAMPING_st3, 	// ���[�^�쓮�J�n
	CMPL_TMP_NORMAL_TAMPING_st4, 	// Tamping�ʒu�ֈړ�����
	CMPL_TMP_NORMAL_TAMPING_st5, 	// Return�����֐؂�ւ�
	CMPL_TMP_NORMAL_TAMPING_st6, 	// ���[�^�쓮(Return)�J�n
	CMPL_TMP_NORMAL_TAMPING_st7, 	// NormalTamping����
	CMPL_TMP_NORMAL_TAMPING_st8, 	// ���̓���҂��ݒ�


	// �C�W�F�N�g�^���s���O����(SetLastTamping)
	CMPL_TMP_EJECT_TAMPING = 0x0400,
	CMPL_TMP_EJECT_TAMPING_st1, 	// ����J�n�x�����Ԑݒ�
	CMPL_TMP_EJECT_TAMPING_st2, 	// ���[�^�쓮�ݒ�
	CMPL_TMP_EJECT_TAMPING_st3, 	// ���[�^�쓮�J�n
	CMPL_TMP_EJECT_TAMPING_st4, 	// Tamping�ʒu�ֈړ�����
	CMPL_TMP_EJECT_TAMPING_st5, 	// Return�����֐؂�ւ�
	CMPL_TMP_EJECT_TAMPING_st6, 	// ���[�^�쓮(Return)�J�n
	CMPL_TMP_EJECT_TAMPING_st7, 	// Return���슮��
	CMPL_TMP_EJECT_TAMPING_st8, 	// SetLastTamping�J�n
	CMPL_TMP_EJECT_TAMPING_st9, 	// ���[�^�쓮(SetLast)�J�n
	CMPL_TMP_EJECT_TAMPING_st10, 	// SetLast���슮��
	CMPL_TMP_EJECT_TAMPING_st11, 	// SetLast�J��Ԃ��`�F�b�N
	CMPL_TMP_EJECT_TAMPING_st12, 	// ���̓���҂��ݒ�

	// �I�t�Z�b�g����
	CMPL_TMP_OFFSET = 0x0500,
	CMPL_TMP_OFFSET_st1, 		// �J�nTiming����
	CMPL_TMP_OFFSET_st2, 		// ���[�^�쓮�ݒ�
	CMPL_TMP_OFFSET_st3, 		// ���[�^�쓮�J�n
	CMPL_TMP_OFFSET_st4, 		// ���莞�ԃ^�C�}�ݒ�
	CMPL_TMP_OFFSET_st5, 		// ������҂�

	// Left Tamper���^�[������
	CMPL_TMP_LEFT_RETURN = 0x0600,	
	CMPL_TMP_LEFT_RETURN_st1, 	// ����J�nTrigger�҂�
	CMPL_TMP_LEFT_RETURN_st2, 	// �^�C�}�ݒ�
	CMPL_TMP_LEFT_RETURN_st3, 	// ���[�^�쓮�ݒ�
	CMPL_TMP_LEFT_RETURN_st4, 	// ���[�^�쓮�J�n
	CMPL_TMP_LEFT_RETURN_st5, 	// �^�C�}�ݒ�
	CMPL_TMP_LEFT_RETURN_st6, 	// ������҂�
	CMPL_TMP_LEFT_RETURN_st7, 	// SizePos�J�n(CR-008) 

	//Right Tamper���^�[������
	CMPL_TMP_RIGHT_RETURN = 0x0700,
	CMPL_TMP_RIGHT_RETURN_st1, 		// �쓮�������f
	CMPL_TMP_RIGHT_RETURN_st2_CW, 	// ����J�nTrigger�҂�(LeftOffset)
	CMPL_TMP_RIGHT_RETURN_st3_CW, 	//	�^�C�}�ݒ�(LeftOffset)
	CMPL_TMP_RIGHT_RETURN_st4_CW, 	// ���[�^�쓮�ݒ�(LeftOffset)
	CMPL_TMP_RIGHT_RETURN_st2_CCW, 	// ����J�nTrigger�҂�(RightOffset)
	CMPL_TMP_RIGHT_RETURN_st3_CCW, 	// �^�C�}�ݒ�(RightOffset)
	CMPL_TMP_RIGHT_RETURN_st4_CCW, 	// ���[�^�쓮�ݒ�(RightOffset)
	CMPL_TMP_RIGHT_RETURN_st5, 		// ���[�^�쓮�J�n
	CMPL_TMP_RIGHT_RETURN_st6, 		// �^�C�}�ݒ�
	CMPL_TMP_RIGHT_RETURN_st7, 		// ������҂�
	CMPL_TMP_RIGHT_RETURN_st8, 	// SizePos�J�n(CR-008) 

	// �I�[�v������(Nigasi)
	CMPL_TMP_OPEN = 0x0800,
	CMPL_TMP_OPEN_st1, 		// ���[�^�쓮�ݒ�
	CMPL_TMP_OPEN_st2, 		// ���[�^�쓮�J�n
	CMPL_TMP_OPEN_st3, 		// �^�C�}�ݒ�
	CMPL_TMP_OPEN_st4, 		// ������҂�
	CMPL_TMP_OPEN_st5,		//

	// Pre-NormalTamping
	CMPL_TMP_PRE_NORMAL_TAMPING = 0x0900,

	// MissRegi�h�~���� (V3.5.95)
	CMPL_TMP_MISSREGI = 0x0A00,
	CMPL_TMP_MISSREGI_st1,  //MissRegi_����Timer�ݒ�
	CMPL_TMP_MISSREGI_st2, // MissRegi_�w��p���X�o��
	CMPL_TMP_MISSREGI_st3, // MissRegi_��㎥�ݒ�
	CMPL_TMP_MISSREGI_st4, // MissRegi_����	

	// Number of Module Action
	CMPL_TMP_ACT_NUM,

} ECMPL_TMP_Action;

#define CMPL_TMP_ACTION_MASK 0xFF00

#endif //#if !defined(ECMPL_TMP_Action_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

