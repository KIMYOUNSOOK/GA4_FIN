// ������ Module Software Platform Frame ������
/// @file    MSP_ProductSpec.h
/// @author  Mitsuyuki Nishizawa
/// @date    13.05.05
/// @version 000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef MSP_ProductSpec_h
#define MSP_ProductSpec_h


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

// ===========================  <<< �f�[�^�^��` >>>  ===========================

// ----- �C�x���gID -----																	// <<< �C�x���gID�̌^ >>>
typedef US		EventID ;																	// ���6bit:Event Index / ����10bit:Event Number

US MSP_GetEventID(US idx, US num) ;															// Event�ϊ��}�N���F Index,Number �� ID�ϊ�
US MSP_GetEventIndex(US id) ;																// Event�ϊ��}�N���F ID �� Index�ϊ�
US MSP_GetEventNumber(US id) ;																// Event�ϊ��}�N���F ID �� Number�ϊ�

// ----- �C�x���g��n���h����e�[�u�� -----
typedef struct EventHandler {																// <<< �C�x���g��n���h���[��e�[�u���̌^ >>>
	void (*mpHandler)(const UC*, UC) ;														// �n���h���[�̃A�h���X
} EventHandler ;

// ----- �C�x���g��R���g���[����e�[�u�� -----
typedef struct {																			// <<< �n���h���[��e�[�u���ւ̃A�N�Z�X��� >>>
	US	mStart : 10 ;																		// �R�[���o�b�N�J�n�ʒu
	US	mCount : 6 ;																		// �R�[���o�b�N�֐��o�^��
} EventControl ;

// ----- �C�x���g��C���f�b�N�X��e�[�u�� -----
typedef struct {																			// Index�ɑΉ����饥�
	const EventControl *mpControlTable ;													// �R���g���[����e�[�u���̐擪�A�h���X
	const EventHandler *mpHandlerTable ;													// �n���h���[��e�[�u���̐擪�A�h���X
} EventIndexTable;

extern const EventIndexTable msp_EvtIndexTable[] ;

// ----- �|�[�g��G���A���e�[�u�� -----
typedef struct {																			// <<< �Z���T�[��C�x���g��e�[�u���ւ̃A�N�Z�X��� >>>
	US	mStart : 10 ;																		// �|�[�g�`�F�b�N�J�n�ʒu
	US	mCount : 6 ;																		// �L���|�[�g�o�^��
} PortAreaInfo ;

extern const PortAreaInfo msp_PortAreaInfo[][2] ;

// ----- �Z���T�[��C�x���g���e�[�u�� -----												// <<< �Z���T�[��G�b�W�˃C�x���g��i���o�[�ϊ���� >>>
typedef UC SensorEventInfo ;																// �|�[�g�̃r�b�g�ʒu�i0�`15�j

extern const SensorEventInfo msp_SensorEdgeEvent[] ;

// ----- �Z���T�[��G�b�W���e�[�u�� -----
typedef struct {																			// <<< �Z���T�[��G�b�W��e�[�u���̌^ >>>
	UC	mIndex ;																			// �|�[�g��C���f�b�N�X�ԍ��iDD�񋟂̔ԍ��j
	UC	mAreaID ;																			// �|�[�g��G���AID
	UL	mMask[2] ;																			// �|�[�g��G�b�W��}�X�N[RISE/FALL]
} PortEdgeInfo ;

extern const PortEdgeInfo msp_PortEdgeInfo[] ;

// ----- �T�u�V�X�e����Q�[�g�E�F�C��X�y�b�N -----
typedef struct {
	UC	mSubsystemID ;																		// �ʐM�ΏۂƂȂ�T�u�V�X�e���̒ʐM�pID
	UC	mEvtIndex ;																			// �R�}���h��C�x���g�����p�̃C�x���g��C���f�b�N�X
	UC	mListSize ;																			// "�R�}���h�˃C�x���g�ϊ����X�g"�̃T�C�Y
	const UC  *mpCommandList ;																// �R�}���h�˃C�x���g�ϊ����X�g
//	ComDataSender  *mpSender ;																// �R�}���h���M�ΏۃT�u�V�X�e���̃|�C���^
//	ComDataReceiver  *mpReceiver ;															// �R�}���h��M�ΏۃT�u�V�X�e���̃|�C���^
} GatewaySpec ;

extern const GatewaySpec msp_IotGwSpec ;													// ��IOT�̎d�l�L�q

// ----- ���P�[�V������C�x���g��X�y�b�N -----
typedef struct {
	US	mTrigger ;																			// �g���K�[�ƂȂ郍�P�[�V����ID�i=�C�x���gID�j
	US	mDelay[MSP_NUM_OF_SCH] ;															// �g���K�[����̒x�����ԁi�X�P�W���[�������j
	UL	mFilter ;																			// ���P�[�V�����̃t�B���^�����O��f�[�^
	UL	mParameter ;																		// ���P�[�V������C�x���g�������̃R�[���o�b�N��p�����[�^
} LocationSpec ;

extern const LocationSpec msp_LocationSpec[] ;												// ���P�[�V������C�x���g�̎d�l�ݒ�e�[�u��


#endif			// MSP_ProductSpec_h


// Change History
// Date:    | Engineer:          | Note:
// ---------+--------------------+-----------------------------------------------------------------------------------------------------------------------------
// 13.05.05 | Mitsuyuki Nishizawa| Module Software Platform Frame�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬.
//


// **************************************************************  [ MSP_ProductSpec.h : EOF ]  ***************************************************************
