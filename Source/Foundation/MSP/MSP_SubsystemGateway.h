// ������ Module Software Platform Frame ������
/// @file    MSP_SubsystemGateway.h
/// @author  Mitsuyuki Nishizawa
/// @date    14.11.05
/// @version 001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


#ifndef MSP_SubsystemGateway_h
#define MSP_SubsystemGateway_h


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

// ===========================  <<< �}�N����` >>>  ===========================

// ----- �����ȃQ�[�g�E�F�CID -----
#define	MSP_GW_VOID			0xFF													// ���݂��Ȃ�Option��Gateway�p��Subsystem ID�Ƃ��ăX�y�b�N���ɐݒ肷��


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Class Name    : CSubsystemGateway
// Description   : �T�u�V�X�e���ւ̃R�}���h�i�X�e�[�^�X�j���M����уR�}���h��C�x���g�̐�����ʒm�����{����.
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
typedef struct SubsystemGateway {
/*public:*/
	//=[[[ Constructor ]]]=================================================================
	// Function Name : CSubsystemGateway::CSubsystemGateway()
	// Description   : ���̃N���X�̃R���X�g���N�^.
	// Parameter     : pSpec		"�X�y�b�N��e�[�u���ւ̃|�C���^"
	// Return        : -
	//=====================================================================================
	//CSubsystemGateway(const GatewaySpec *const pSpec) : mpSpec(pSpec) {}

	//=====================================================================================
	// Method Name   : CSubsystemGateway::SendCommand()
	// Description   : �R�}���h�i�X�e�[�^�X�j�𑗐M����.
	// Parameter     : senderID		"���M�����W���[��ID"
	//               : pPacket		"���M�p�P�b�g�i�[��A�h���X"
	//               : length		"���M�p�P�b�g�T�C�Y"
	//               : log			"���M�p�P�b�g�̃��O���ێ��̗L��"
	// Return        : �R�}���h���M�̌��ʁi0:���M���� / 1:���M���s�j
	//=====================================================================================
	//UC SendCommand(UC senderID, const UC *pPacket, US length, UC log) {}

	//=====================================================================================
	// Method Name   : CSubsystemGateway::ReceiveCommand()
	// Description   : �R�}���h�i�X�e�[�^�X�j�̎�M����уR�}���h��C�x���g�̐���������.
	// Parameter     : pPacket		"�T�u�V�X�e������̎�M�p�P�b�g�i�[�A�h���X�i�Q�Ɛ�j"
	//               : length		"��M�p�P�b�g�T�C�Y"
	// Return        : �Ȃ�
	//=====================================================================================
	//void ReceiveCommand(const UC *pPacket, US length) {}

/*private:*/
	const GatewaySpec *mpSpec ;																// [Member] Subsystem Gateway�̃X�y�b�N
	CommandEvent  mSendBuffer ;																// [Member] �C�x���g���M�o�b�t�@�i�R�}���h�˃C�x���g�ϊ��p�j
} CSubsystemGateway ;


// ===========================  <<< �֐��v���g�^�C�v�錾 >>>  ===========================
void CSubsystemGateway_CSubsystemGateway(CSubsystemGateway*, const GatewaySpec*) ;			// �Q�[�g�E�F�C�����������i�R���X�g���N�^�j
UC CSubsystemGateway_SendCommand(CSubsystemGateway*, UC, const UC*, US, UC) ;				// �R�}���h�i�X�e�[�^�X�j���M ()
void CSubsystemGateway_ReceiveCommand(CSubsystemGateway*, const UC*, US) ;					// �R�}���h��M���R�}���h��C�x���g����&�ʒm ()

#endif			// MSP_SubsystemGateway_h


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 13.05.05 | Mitsuyuki Nishizawa | Module Software Platform Frame�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬.
// 14.11.05 | Mitsuyuki Nishizawa | ����M�֐����̂���ш�����ύX&������������ǉ�.
//
//


// **********************************************************  [ MSP_SubsystemGateway.h : EOF ]  **************************************************************
