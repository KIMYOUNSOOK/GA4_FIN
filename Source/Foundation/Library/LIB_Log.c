// File Name: LIB_Log.c
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 15.03.27
// Revision:  001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define LIB_Log_c

#include "Common.h"
#include "DD_LOG.h"
#include "LIB_Log.h"

// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************


// ************************************************************  <<<<< Program Code Area >>>>>  ***************************************************************

//=====================================================================================
// Function Name : Printf()
// Description   : �f�o�b�O��������o�͂���.
// Parameter     : id			"�f�o�b�O������o�̓��W���[��ID"
//               : SR			"����M�敪 ["S":���M / "R":��M / "0":�f�o�b�O]
//               : *data		"�f�o�b�O������i�[��A�h���X"
//               : size			"�f�o�b�O������̃T�C�Y [byte�P��]"
// Return        : �Ȃ�
//=====================================================================================
#ifndef Win32
void Printf(UC id, UC SR, UC *data, US size)
{
	DD_LOG_Put(id, SR, data, size, 0);
}
#endif


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev001 Base)
// 15.03.27 | Yuichi Hoshino      | Printf()�Ȃ�����������
// 15.03.30 | Yuichi Hoshino      | Release Revision 001
//
//


// ****************************************************************  [ LIB_Log.c : EOF ]  *********************************************************************
