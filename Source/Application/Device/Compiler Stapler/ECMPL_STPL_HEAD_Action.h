// File Name:	ECMPL_STPL_HEAD_Action.h
// Description:	staple Device �̓�����`
// Engineer:	Takahiko Minaga
// Last Edit:	14.09.01
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(ECMPL_STPL_HEAD_Action_h)
#define ECMPL_STPL_HEAD_Action_h


typedef enum /* ECMPL_STPL_HEAD_Action */
{
	// �X�e�[�v���w�b�h��~
	CMPL_STPL_HEAD_ACT_IDLE,													// 0:��~���

	CMPL_STPL_HEAD_COMPO_FORWARD = 50,
	CMPL_STPL_HEAD_COMPO_REVERSE = 51,

	// �X�e�[�v���w�b�hDiag����
	CMPL_STPL_HEAD_COMPO = 0x0100,
	CMPL_STPL_HEAD_COMPO_st1,
	CMPL_STPL_HEAD_COMPO_st2,
	CMPL_STPL_HEAD_COMPO_st3,
	CMPL_STPL_HEAD_COMPO_st4,


	// �X�e�[�v���w�b�h�z�[������
	CMPL_STPL_HEAD_HOME = 0x0300,
	CMPL_STPL_HEAD_HOME_st1,			// Staple���[�^�쓮
	CMPL_STPL_HEAD_HOME_st2,			// StapleHomeSnr�҂�
	CMPL_STPL_HEAD_HOME_st3,			// StapleHomeSnrOn��ԁ@�@����
	CMPL_STPL_HEAD_HOME_st10,			// HomeSnrOnFail��������
	CMPL_STPL_HEAD_HOME_st11,			// StapleMotorReverse�����쓮
	CMPL_STPL_HEAD_HOME_st12,			// StapleHomeSnr�҂�
	CMPL_STPL_HEAD_HOME_st13,			// HomeSnrOn����(StapleNG����)
	CMPL_STPL_HEAD_HOME_st14,			// Fail����

	// �X�e�[�v������
	CMPL_STPL_HEAD_STAPLE = 0x0400,
	CMPL_STPL_HEAD_STAPLE_st1,			// Staple���[�^�쓮�@�ŏ���
	CMPL_STPL_HEAD_STAPLE_st2, 			// StapleHomeSnr�҂�
	CMPL_STPL_HEAD_STAPLE_st3, 			// StapleHomeSnrOn���(���슮��)
	CMPL_STPL_HEAD_STAPLE_st10, 		// HomeSnrOnFail��������
	CMPL_STPL_HEAD_STAPLE_st11,			// StapleMotorReverse�����쓮
	CMPL_STPL_HEAD_STAPLE_st12,			// StapleHomeSnr�҂�
	CMPL_STPL_HEAD_STAPLE_st13,			// HomeSnrOn�҂�(StapleNG�ʒm)
	CMPL_STPL_HEAD_STAPLE_st14,			// �@Fail����

	// staple�j�o������
	CMPL_STPL_HEAD_HARIOUT = 0x0500,
	CMPL_STPL_HEAD_HARIOUT_st1,			// Staple���[�^�쓮
	CMPL_STPL_HEAD_HARIOUT_st2,			// StapleHomeSnr�҂�
	CMPL_STPL_HEAD_HARIOUT_st3, 		// StapleHomeSnrOn���
	CMPL_STPL_HEAD_HARIOUT_st4, 		// Self Priming Sensor����
	CMPL_STPL_HEAD_HARIOUT_st10,		// HomeSnrOnFail��������
	CMPL_STPL_HEAD_HARIOUT_st11,		// StapleMotorReverse�����쓮
	CMPL_STPL_HEAD_HARIOUT_st12,		// StapleHomeSnr�҂�
	CMPL_STPL_HEAD_HARIOUT_st13,		// HomeSnrOn���
	CMPL_STPL_HEAD_HARIOUT_st14,		// Fail����

} ECMPL_STPL_HEAD_Action;

#define CMPL_STPL_HEAD_ACTION_MASK 0xFF00


#endif //#if !defined(ECMPL_STPL_HEAD_Action_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

