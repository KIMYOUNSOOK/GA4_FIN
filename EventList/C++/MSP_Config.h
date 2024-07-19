// ������ Module Software Platform Frame ������
/// @file    MSP_Config.h
/// @author  Tsuyoshi Seto
/// @date    14/04/02
/// @time    16:37:23
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// ������������������������������������������������������������������������������������������������������������������������������������������������������������
//  �{�w�b�_�[�t�@�C���̓C�x���g�Ǘ����X�g���玩����������܂��B�i�v���_�N�g����j�[�N�j
// ������������������������������������������������������������������������������������������������������������������������������������������������������������

#ifndef MSP_Config_h
#define MSP_Config_h


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

// ----- �|�[�g��G���A�� -----
#define	MSP_NUM_OF_AREA		0																	// �|�[�g��G���A�̐�

// ----- �R�}���h/�X�e�[�^�X�ő�p�P�b�g�T�C�Y -----
#define	MSP_MAX_CMD_SIZE	256																	// ��M�R�}���h�i�X�e�[�^�X�j�̍ő�p�P�b�g�T�C�Y�i�ő�:1024�j

// ----- �C�x���g�ő�p�P�b�g�T�C�Y -----
#define	MSP_MAX_MSG_SIZE	258																	// ����M�C�x���g�̍ő�p�P�b�g�T�C�Y�i�ő�:1024�j

// ----- ���P�[�V�����E�C�x���g�� -----
#define	MSP_NUM_OF_LOC		53																	// ���P�[�V�����E�C�x���g��(�X�P�W���[���E�e�[�u���̗v�f���Ƃ��Ďg�p)

// ----- ���P�[�V�����E�^�C�}�[�� -----
#define	MSP_NUM_OF_TMR		64																	// ���P�[�V������C�x���g�𐶐����邽�߂Ɏg�p����Ǘ��^�C�}�[���i�ő�:255�j

// ----- �p�t�H�[�}���X�E���O���� -----
#define	ISP_NUM_OF_LOG		64																	// �p�t�H�[�}���X����p�̃��O�̈�m�ې��i�ő�:128�j

// ----- �C�x���g��C���f�b�N�X��` -----
enum EEventIndex {																				// <<< Event Index ID >>>	8�`62�̓v���_�N�g�Ŋ���t�������{
	EVT_FRM_SYS    =  0,																		//  0: ���b�Z�[�W �yFrame System�z				�i���ʒ�`�j��� < �\��ԍ� >
	EVT_SNR_EDGE   =  1,																		//  1: �Z���T�[�G�b�W							�i���ʒ�`�j��� < �\��ԍ� >
	EVT_LOC_TMNG   =  2,																		//  2: ���P�[�V�����^�C�~���O					�i���ʒ�`�j��� < �\��ԍ� >
	EVT_CDI_CMD    =  3,																		//  3: �R�}���h���X�e�[�^�X�yIOT�z				�i���ʒ�`�j��� < �\��ԍ� >
	EVT_TCBM_CMD   =  4,																		//  4: �R�}���h���X�e�[�^�X�yTCBM�z				�i���ʒ�`�j��� < �\��ԍ� >
	EVT_SUBSYS_STT =  5,																		//  5: ���b�Z�[�W �ySubsystem State�z			�i���ʒ�`�j��� < �\��ԍ� >
	EVT_ACT_REQ    =  6,																		//  6: ���b�Z�[�W �yDevice Action Request�z		�i���ʒ�`�j��� < �\��ԍ� >
	EVT_ACT_RES    =  7,																		//  7: ���b�Z�[�W �yDevice Action Response�z	�i���ʒ�`�j��� < �\��ԍ� >
	EVT_DEV_EVT    =  8,																		//  8: ���b�Z�[�W �yDevice Event�z				�i���ʒ�`�j��� < �\��ԍ� >
	EVT_SHT_EVT    =  9,																		//  9: ���b�Z�[�W �ySheet Event�z				�i���ʒ�`�j��� < �\��ԍ� >
	EVT_TIME_OUT   = 63																			// 63: �C�x���g�^�C�}�[							�i���ʒ�`�j��� < �\��ԍ� >
} ;

// ----- �C�x���g��n���h���[��` -----															// �C�x���g��n���h���[�Ƃ��Ē�`���ꂽ�֐���S�ċL�q����
extern void CDI_RcvSheetExit(const UC*, UC) ;
extern void CDI_RcvSheetFinRequest(const UC*, UC) ;
extern void CDI_RcvChangeShtFinRequest(const UC*, UC) ;
extern void CDI_RcvCancelShtFinRequest(const UC*, UC) ;
extern void CDI_RcvSetFinRequest(const UC*, UC) ;
extern void CDI_RcvSelectFinAttribute(const UC*, UC) ;
extern void CDI_RcvStopFinisher(const UC*, UC) ;
extern void CDI_RcvDevSttsRequest(const UC*, UC) ;
extern void CDI_RcvDevicePauseReq(const UC*, UC) ;
extern void CDI_RcvReqJamSttsCheck(const UC*, UC) ;
extern void CDI_RcvDevCapabilitiesRequest(const UC*, UC) ;
extern void CDI_RcvDevConfigurationRequest(const UC*, UC) ;
extern void CDI_RcvSetDevConfiguration(const UC*, UC) ;
extern void CDI_RcvChangeDevContMode(const UC*, UC) ;
extern void CDI_RcvInitComplete(const UC*, UC) ;
extern void CDI_RcvInitRequest(const UC*, UC) ;
extern void CDI_RcvInitAcknowledge(const UC*, UC) ;
extern void CDI_RcvPollRequest(const UC*, UC) ;
extern void CDI_RcvPwrOffAnnouncement(const UC*, UC) ;
extern void CDI_RcvExecuteDiag(const UC*, UC) ;
extern void CDI_RcvDevContActionReq(const UC*, UC) ;
extern void CDI_RcvSysContInfo(const UC*, UC) ;


#endif          // MSP_Config_h

// ****************************************************************  [ MSP_Config.h : EOF ]  *****************************************************************
