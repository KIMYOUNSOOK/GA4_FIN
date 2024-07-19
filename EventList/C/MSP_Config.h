// ������ Module Software Platform Frame ������
/// @file    MSP_Config.h
/// @author  Tsuyoshi Seto
/// @date    16/03/11
/// @time    16:55:06
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
#define	MSP_NUM_OF_AREA		1																	// �|�[�g��G���A�̐�

// ----- �R�}���h/�X�e�[�^�X�ő�p�P�b�g�T�C�Y -----
#define	MSP_MAX_CMD_SIZE	256																	// ��M�R�}���h�i�X�e�[�^�X�j�̍ő�p�P�b�g�T�C�Y�i�ő�:1024�j

// ----- �C�x���g�ő�p�P�b�g�T�C�Y -----
#define	MSP_MAX_MSG_SIZE	258																	// ����M�C�x���g�̍ő�p�P�b�g�T�C�Y�i�ő�:1024�j

// ----- ���P�[�V�����E�C�x���g�� -----
#define	MSP_NUM_OF_LOC		4																	// ���P�[�V�����E�C�x���g��(�X�P�W���[���E�e�[�u���̗v�f���Ƃ��Ďg�p)

// ----- ���P�[�V�����E�^�C�}�[�� -----
#define	MSP_NUM_OF_TMR		64																	// ���P�[�V������C�x���g�𐶐����邽�߂Ɏg�p����Ǘ��^�C�}�[���i�ő�:255�j

// ----- �p�t�H�[�}���X�E���O���� -----
#define	ISP_NUM_OF_LOG		64																	// �p�t�H�[�}���X����p�̃��O�̈�m�ې��i�ő�:128�j

// ----- �C�x���g��C���f�b�N�X��` -----
typedef enum /*EEventIndex*/ {																	// <<< Event Index ID >>>	8�`62�̓v���_�N�g�Ŋ���t�������{
	EVT_FRM_SYS  =  0,																			//  0: ���b�Z�[�W �yFrame System�z				�i���ʒ�`�j��� < �\��ԍ� >
	EVT_SNR_EDGE =  1,																			//  1: �Z���T�[�G�b�W							�i���ʒ�`�j��� < �\��ԍ� >
	EVT_LOC_TMNG =  2,																			//  2: ���P�[�V�����^�C�~���O					�i���ʒ�`�j��� < �\��ԍ� >
	EVT_CDI_CMD  =  3,																			//  3: �R�}���h���X�e�[�^�X�yIOT�z				�i���ʒ�`�j��� < �\��ԍ� >
	EIDX_STT     =  4,																			//  4: ���b�Z�[�W �yFinisher State�z			�i���ʒ�`�j��� < �\��ԍ� >
	EIDX_DEV_REQ =  5,																			//  5: ���b�Z�[�W �yDevice Action Request�z		�i���ʒ�`�j��� < �\��ԍ� >
	EIDX_DEV_RES =  6,																			//  6: ���b�Z�[�W �yDevice Action Response�z	�i���ʒ�`�j��� < �\��ԍ� >
	EIDX_DEV_EVT =  7,																			//  7: ���b�Z�[�W �yDevice Event�z				�i���ʒ�`�j��� < �\��ԍ� >
	EIDX_COND    =  8,																			//  8: ���b�Z�[�W �yCondition�z					�i���ʒ�`�j��� < �\��ԍ� >
	EIDX_SHT     =  9,																			//  9: ���b�Z�[�W �ySheet Information�z			�i���ʒ�`�j��� < �\��ԍ� >
	EIDX_SHT_REQ = 10,																			// 10: ���b�Z�[�W �ySheet Action Request�z		�i���ʒ�`�j��� < �\��ԍ� >
	EIDX_SHT_RES = 11,																			// 11: ���b�Z�[�W �ySheet Action Response�z		�i���ʒ�`�j��� < �\��ԍ� >
	EIDX_SHT_EVT = 12,																			// 12: ���b�Z�[�W �ySheet Event Notify�z		�i���ʒ�`�j��� < �\��ԍ� >
	EIDX_DIAG    = 13,																			// 13: ���b�Z�[�W �yDiagnostics�z				�i���ʒ�`�j��� < �\��ԍ� >
	EVT_TIME_OUT = 63																			// 63: �C�x���g�^�C�}�[							�i���ʒ�`�j��� < �\��ԍ� >
} EEventIndex ;

// ----- �C�x���g��n���h���[��` -----															// �C�x���g��n���h���[�Ƃ��Ē�`���ꂽ�֐���S�ċL�q����
extern void SYS_StartUpEvent(const UC*, UC) ;
extern void CMPL_StartUpEvent(const UC*, UC) ;
extern void TRNS_StartUpEvent(const UC*, UC) ;
extern void TRAY_StartUpEvent(const UC*, UC) ;
extern void CDI_ComSyncNotify(const UC*, UC) ;
extern void CDI_ComFailNotify(const UC*, UC) ;


extern void CVR_StapleDoorILKSwitchON(const UC*, UC) ;
extern void SHT_IotExitSensorON(const UC*, UC) ;
extern void SHT_CompileExitSensorON(const UC*, UC) ;
extern void CMPL_STPL_HomeSensorON(const UC*, UC) ;
extern void TRAY_STK_Height2SensorON(const UC*, UC) ;
extern void TRAY_STK_Height1SensorON(const UC*, UC) ;
extern void CMPL_TMP_RIGHT_RTHomeSensorON(const UC*, UC) ;
extern void CMPL_TMP_LEFT_LTHomeSensorON(const UC*, UC) ;
extern void CMPL_EJCT_SCLMP_SetClampHomeSensorON(const UC*, UC) ;
extern void CVR_RearCoverSwitchON(const UC*, UC) ;
extern void TRAY_STK_NoPaperFullSensorON(const UC*, UC) ;
extern void CMPL_EJCT_SubPaddleHomeSensorON(const UC*, UC) ;
extern void CMPL_EJCT_BELT_EjecterHomeSensorON(const UC*, UC) ;
extern void CMPL_EJCT_SCLMP_EjecterHomeSensorON(const UC*, UC) ;
extern void TRNS_IOTGATE_SignalON(const UC*, UC) ;
extern void CVR_StapleDoorILKSwitchOFF(const UC*, UC) ;
extern void SHT_IotExitSensorOFF(const UC*, UC) ;
extern void SHT_CompileExitSensorOFF(const UC*, UC) ;
extern void CMPL_STPL_HomeSensorOff(const UC*, UC) ;
extern void TRAY_STK_Height2SensorOFF(const UC*, UC) ;
extern void TRAY_STK_Height1SensorOFF(const UC*, UC) ;
extern void CMPL_TMP_RIGHT_RTHomeSensorOFF(const UC*, UC) ;
extern void CMPL_TMP_LEFT_LTHomeSensorOFF(const UC*, UC) ;
extern void CMPL_EJCT_SCLMP_SetClampHomeSensorOFF(const UC*, UC) ;
extern void CVR_RearCoverSwitchOFF(const UC*, UC) ;
extern void TRAY_STK_NoPaperFullSensorOFF(const UC*, UC) ;
extern void CMPL_EJCT_SubPaddleHomeSensorOFF(const UC*, UC) ;
extern void CMPL_EJCT_BELT_EjecterHomeSensorOFF(const UC*, UC) ;
extern void CMPL_EJCT_SCLMP_EjecterHomeSensorOFF(const UC*, UC) ;
extern void TRNS_IOTGATE_SignalOFF(const UC*, UC) ;


extern void TRNS_IotExitLocationLead(const UC*, UC) ;
extern void CMPL_CompileExitSensorLeadEvent(const UC*, UC) ;
extern void TRNS_CompileExitLocationLead(const UC*, UC) ;
extern void CMPL_CompileExitSensorTailEvent(const UC*, UC) ;
extern void TRNS_CompileExitLocationTail(const UC*, UC) ;


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


extern void TRNS_PowerUpRequestEvent(const UC*, UC) ;
extern void TRAY_PowerUpRequestEvent(const UC*, UC) ;
extern void CMPL_PowerUpReqEvent(const UC*, UC) ;
extern void SYS_PowerUpResponseEvent(const UC*, UC) ;
extern void TRNS_PowerDownRequestEvent(const UC*, UC) ;
extern void TRAY_PowerDownRequestEvent(const UC*, UC) ;
extern void CMPL_PowerDownReqEvent(const UC*, UC) ;
extern void SYS_PowerDownResponseEvent(const UC*, UC) ;
extern void TRNS_ChangeDiagRequestEvent(const UC*, UC) ;
extern void TRAY_ChangeDiagRequestEvent(const UC*, UC) ;
extern void CMPL_ChangeDiagReqEvent(const UC*, UC) ;
extern void SYS_ChangeDiagResponseEvent(const UC*, UC) ;
extern void TRNS_ChangeNormalRequestEvent(const UC*, UC) ;
extern void TRAY_ChangeNormalRequestEvent(const UC*, UC) ;
extern void CMPL_ChangeNormalReqEvent(const UC*, UC) ;
extern void SYS_ChangeNormalResponseEvent(const UC*, UC) ;
extern void CMPL_StandbyNotifyEvent(const UC*, UC) ;
extern void TRNS_CycleUpRequestEvent(const UC*, UC) ;
extern void TRAY_CycleUpRequestEvent(const UC*, UC) ;
extern void CMPL_CycleUpReqEvent(const UC*, UC) ;
extern void SYS_CycleUpResponseEvent(const UC*, UC) ;
extern void TRNS_CycleDownRequestEvent(const UC*, UC) ;
extern void TRAY_CycleDownRequestEvent(const UC*, UC) ;
extern void CMPL_CycleDownReqEvent(const UC*, UC) ;
extern void TRNS_HardDownRequestEvent(const UC*, UC) ;
extern void TRAY_HardDownRequestEvent(const UC*, UC) ;
extern void CMPL_HardDownReqEvent(const UC*, UC) ;
extern void SYS_ProcessDownResponseEvent(const UC*, UC) ;
extern void CMPL_StandbyExitNotifyEvent(const UC*, UC) ;
extern void TRAY_StandbyExitNotifyEvent(const UC*, UC) ;


extern void CMPL_DevActReqPowerOnInitEvent(const UC*, UC) ;
extern void CMPL_DevActReqResumeInitEvent(const UC*, UC) ;
extern void CMPL_DevActReqNormalInitEvent(const UC*, UC) ;
extern void TRNS_DeviceActionRequestJamCheck(const UC*, UC) ;
extern void TRNS_DeviceActionRequestProcessEnd(const UC*, UC) ;
extern void CMPL_DevActReqProcessEndEvent(const UC*, UC) ;
extern void TRAY_DeviceActionRequestProcessEnd(const UC*, UC) ;
extern void TRNS_DeviceActionRequestJamClearAssist(const UC*, UC) ;
extern void CMPL_DevActReqJamClearAssistEvent(const UC*, UC) ;


extern void SYS_DeviceActionResponseEventPowerOnInitializeCMPL(const UC*, UC) ;
extern void SYS_DeviceActionResponseEventPowerResumeInitializeCMPL(const UC*, UC) ;
extern void SYS_DeviceActionResponseEventNormalInitializeCMPL(const UC*, UC) ;
extern void SYS_DeviceActionResponseEventJamCheckTRNS(const UC*, UC) ;
extern void SYS_DeviceActionResponseEventProcessEndActionTRNS(const UC*, UC) ;
extern void SYS_DeviceActionResponseEventProcessEndActionCMPL(const UC*, UC) ;
extern void SYS_DeviceActionResponseEventProcessEndActionTRAY(const UC*, UC) ;
extern void SYS_DeviceActionResponseEventJamClearAssistInitializeTRNS(const UC*, UC) ;
extern void SYS_DeviceActionResponseEventJamClearAssistInitializeCMPL(const UC*, UC) ;


extern void TRNS_DeviceEventAnyCoverOpen(const UC*, UC) ;
extern void TRAY_DeviceEventAnyCoverOpen(const UC*, UC) ;
extern void CMPL_DevEvtAnyCoverOpenEvent(const UC*, UC) ;
extern void TRNS_DeviceEventAllCoverClose(const UC*, UC) ;
extern void TRAY_DeviceEventAllCoverClose(const UC*, UC) ;
extern void CMPL_DevEvtAllCoverCloseEvent(const UC*, UC) ;
extern void CMPL_DevEvtDevInitCompEvent(const UC*, UC) ;


extern void SYS_DeviceStatusNotifyEvent(const UC*, UC) ;


extern void CMPL_SheetInformationEventUpdate(const UC*, UC) ;
extern void CMPL_SheetInfomationEventDelivered(const UC*, UC) ;
extern void TRNS_SheetAbortedNotify(const UC*, UC) ;
extern void CMPL_SheetInformationEventAbort(const UC*, UC) ;


extern void CMPL_SheetActionRequestEventCompilerShtInitCMPL(const UC*, UC) ;
extern void TRAY_SheetActionRequestEventStackerShtInitTRAY(const UC*, UC) ;


extern void SHT_SheetActionResponseEventCompilerShtInitCMPL(const UC*, UC) ;
extern void SHT_SheetActionResponseEventStackerShtInitTRAY(const UC*, UC) ;


extern void SHT_SheetEventNotifyEventOutTrayEjectFinishStart(const UC*, UC) ;
extern void SHT_SheetEventNotifyEventOutTrayEjectFinishComp(const UC*, UC) ;
extern void SHT_SheetEventNotifyEventOutTrayEjectStart(const UC*, UC) ;
extern void TRAY_SheetEventNotifyEventOutTrayEjectStart(const UC*, UC) ;
extern void SHT_SheetEventNotifyEventOutTrayCompileFinishComp(const UC*, UC) ;
extern void SHT_SheetEventNotifyEventOutTrayEjectComp(const UC*, UC) ;
extern void JAM_SheetEventNotifyEventOutTrayJamOccurrence(const UC*, UC) ;
extern void TRAY_SheetEventNotifyShtInitStackerJobStart(const UC*, UC) ;
extern void TRAY_SheetEventNotifyShtInitStackerWaitingPositionStart(const UC*, UC) ;
extern void TRAY_SheetEventNotifyTrayJobTrigger_1(const UC*, UC) ;
extern void TRAY_SheetEventNotifyTrayJobTrigger_2(const UC*, UC) ;


extern void TRNS_ComponentControlStartReq(const UC*, UC) ;
extern void TRAY_ComponentControlStartReq(const UC*, UC) ;
extern void CMPL_ComponentControlStartReq(const UC*, UC) ;
extern void DIAG_ComponentControlStartNotifyEvent(const UC*, UC) ;
extern void TRNS_ComponentControlStopReq(const UC*, UC) ;
extern void TRAY_ComponentControlStopReq(const UC*, UC) ;
extern void CMPL_ComponentControlStopReq(const UC*, UC) ;
extern void DIAG_ComponentControlStopNotifyEvent(const UC*, UC) ;


#endif          // MSP_Config_h

// ****************************************************************  [ MSP_Config.h : EOF ]  *****************************************************************
