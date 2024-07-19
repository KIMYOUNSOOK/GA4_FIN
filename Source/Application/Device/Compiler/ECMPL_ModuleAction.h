// File Name:	ECMPL_ModuleAction.h
// Description:	Compiler Module�̎q���iAction���`
// Engineer:	Takahiko Minaga
// Last Edit:	14.08.18
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(ECMPL_ModuleAction_h)
#define ECMPL_ModuleAction_h

typedef enum /* ECMPL_ModuleAction */
{
	/* 0�`15 �́ASequence�Ɏg�p���� */
	
	// �X�e�[�v���w�b�h�z�[������
	CMPL_MDL_ACT_STAPLE_HEAD_HOME,	//0
	// �C�W�F�N�^�[�z�[������
	CMPL_MDL_ACT_EJECTOR_HOME,		//
	// �X�e�[�v���w�b�h�j�o������
	CMPL_MDL_ACT_STAPLE_HARIOUT,	//
	// SubPaddle Home
	CMPL_MDL_ACT_PADDLE_HOME,
	// �p�h���R���p�C������
	CMPL_MDL_ACT_PADDLE_COMPILE,	     //
	// �C�W�F�N�g����
	CMPL_MDL_ACT_EJECT,				//5			
	// �^���p�[�T�C�Y�|�W�V���j���O����
	CMPL_MDL_ACT_TAMP_SIZE_POS,		//4
 	// Tamper MissRegi (V3.5.95)
	CMPL_MDL_ACT_TAMP_MISS_REGI,   
	// �p�����e�҂�
//	CMPL_MDL_ACT_SHEET_COMPILE_WAIT,		//
	  // ProcessEnd �㎥ OFF ����
	CMPL_MDL_ACT_CURRENT_OFF,  //
	// �m�[�}���^���s���O����
	CMPL_MDL_ACT_NORMAL_TAMPING,			//
	// �C�W�F�N�g�^���s���O����
	CMPL_MDL_ACT_EJECT_TAMPING,		//
	// �X�e�[�v������
	CMPL_MDL_ACT_STAPLE,			//
	// �I�t�Z�b�g����
	CMPL_MDL_ACT_OFFSET,			// 
	// ���^�[������
	CMPL_MDL_ACT_RETURN,			//
	// Sequence Start
	CMPL_MDL_ACT_START,
	// Sequence End
	CMPL_MDL_ACT_END,
	// �I�[�v������
	CMPL_MDL_ACT_OPEN,

	// Number of Module Action
	CMPL_MDL_ACT_NUM,

} ECMPL_ModuleAction;

#endif //#if !defined(ECMPL_ModuleAction_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

