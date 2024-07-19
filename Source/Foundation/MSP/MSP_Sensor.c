// ������ Module Software Platform Frame ������
/// @file    MSP_Sensor.c
/// @author  Mitsuyuki Nishizawa
/// @date    15.02.12
/// @version 002
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define MSP_Sensor_c

#include "Common.h"
#include "LIB_Service.h"
#include "DD_DIO.h"

#include "MSP_Common.h"
#include "MSP_Config.h"
#include "MSP_EventID.h"
#include "MSP_ProductSpec.h"
#include "MSP_EventMain.h"
#include "MSP_Sensor.h"


//=====================================================================================
// Function Name : CSensorEvent::NotifyEdge()                                          
// Description   : ���̓|�[�g�̕ω���ʒm����.                                         
// Parameter     : message		"�G�b�W�ʒm���b�Z�[�W�iON/OFF�j"                       
//               : area			"�|�[�g�G���AID"                                       
//               : port			"�ω����m�|�[�g���"                                   
// Return        : -                                                                   
//=====================================================================================
void CSensorEvent_NotifyEdge(UC message, UC area, UL port)
{
	static const char  eMsg1[] = {"FRAME ERROR: Invalid Port Area"} ;
	
	US	start, count, index, eventID ;
	UC	edge ;
	
	if ( message == MSG_SNR_ON_EDGE ) edge = MSP_PORT_IDX_ON ;								// �G�b�W���˃C���f�b�N�X�ϊ�
	else                              edge = MSP_PORT_IDX_OFF ;
		
	if ( ( area < MSP_NUM_OF_AREA ) && ( edge < 2 ) ) {										// �|�[�g��G���A���A�N�Z�X��f�[�^�͈̔̓`�F�b�N

	// ----- �|�[�g�ω��f�[�^����Ή�����C�x���gID���������� -----							// <<<<<<  �|�[�g��͏���  >>>>>>
		start = msp_PortAreaInfo[area][edge].mStart ;										// �Z���T�[��C�x���g��e�[�u���ւ̃A�N�Z�X�����擾
		count = msp_PortAreaInfo[area][edge].mCount ;

		for ( index = start ; index < ((US)(start + count)) ; index++ ) {					// [[[ LOOP ]]] �L���|�[�g�����̃`�F�b�N�����{����i���[�v�j
			if ( (port & GET_BIT_PATTERN(msp_SensorEdgeEvent[index])) != 0x0000 ) {			// �|�[�g�ω��f�[�^���`�F�b�N����
				eventID = MSP_GetEventID(EVT_SNR_EDGE, index) ;
				MSP_SendEvent(MID_FRAME, eventID, (const UC*)&eventID, sizeof(US), OFF) ;	// �Z���T�[��C�x���g�𑗐M����
			}
		}																					// [[[ LOOP ]]] �i�����܂Ń��[�v�j
	}
	else {
		Printf(LOGID_FRAME, 0, (UC*)eMsg1, (US)sizeof(eMsg1)) ;
	}
}


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 13.04.01 | Mitsuyuki Nishizawa | Module Software Platform Frame�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬.
// 14.11.04 | Mitsuyuki Nishizawa | �ω��ʒm�֐��̈�����ύX.
// 14.12.04 | Yuichi Hoshino      | Release version 001
// 15.02.12 | Yuichi Hoshino      | Printf()�ɓn��ID��Log ID�ɕύX
// 15.03.30 | Yuichi Hoshino      | Release version 002
//
//


// ****************************************************************  [ MSP_Sensor.c : EOF ]  ******************************************************************
