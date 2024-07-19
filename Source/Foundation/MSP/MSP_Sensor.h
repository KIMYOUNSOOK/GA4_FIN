// ������ Module Software Platform Frame ������
/// @file    MSP_Sensor.h
/// @author  Mitsuyuki Nishizawa
/// @date    14.11.04
/// @version 001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


#ifndef MSP_Sensor_h
#define MSP_Sensor_h

#include "DD_Event.h"


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

// ===========================  <<< �}�N����` >>>  ===========================

// ----- �����ȃ|�[�g��G���AID -----
#define	MSP_PORT_AREA_VOID	0xFF															// �|�[�g��G���AID�͍ő�254�܂Ŏg�p�\�I�I�I

// ----- �|�[�g��G�b�W��C���f�b�N�X -----
#define	MSP_PORT_IDX_ON		0																// �|�[�g��G�b�W��C���f�b�N�X [ON]
#define	MSP_PORT_IDX_OFF	1																// �|�[�g��G�b�W��C���f�b�N�X [OFF]


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Class Name    : CSensorEvent
// Description   : �Z���T�[�ω��ɉ������Z���T�[��C�x���g�̐�����ʒm�����{����.
// Note          : �Ȃ�
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//typedef struct SensorEvent {
/*public:*/
	//=[[[ Constructor ]]]=================================================================
	// Function Name : CSensorEvent::CSensorEvent()                                        
	// Description   : ���̃N���X�̃R���X�g���N�^.                                         
	// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
	// Return        : -                                                                   
	//=====================================================================================
	// void CSensorEvent(CSensorEvent *self) {}

	//=====================================================================================
	// Function Name : CSensorEvent::NotifyEdge()                                          
	// Description   : ���̓|�[�g�̕ω���ʒm����.                                         
	// Parameter     : message		"�G�b�W�ʒm���b�Z�[�W�iON/OFF�j"                       
	//               : area			"�|�[�g�G���AID"                                       
	//               : port			"�ω����m�|�[�g���"                                   
	// Return        : -                                                                   
	//=====================================================================================
	// void NotifyEdge(UC message, UC area, UL port) {}

/*private:*/

//} CSensorEvent ;


// ===========================  <<< �֐��v���g�^�C�v�錾 >>>  ===========================
void CSensorEvent_NotifyEdge(UC, UC, UL) ;													// �Z���T�[�ω����m�̒ʒm����()					: void


#endif			// MSP_Sensor_h


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 13.05.05 | Mitsuyuki Nishizawa | Module Software Platform Frame�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬.
// 14.11.04 | Mitsuyuki Nishizawa | �ω��ʒm�֐����̂���ш�����ύX.
//
//


// ***************************************************************  [ MSP_Sensor.h : EOF ]  *******************************************************************
