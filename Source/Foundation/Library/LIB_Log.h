// File Name: LIB_Log.h
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 15.03.30
// Revision:  001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef LIB_Log_h
#define LIB_Log_h

#include "DD_LOG.h"

// ******************************************************  <<<<< Definition & Declaration Area >>>>>  *********************************************************


// ********************************************************  <<<<< Class Declaration Area >>>>>  **************************************************************

// ===========================  <<< 関数プロトタイプ宣言 >>>  ===========================
#ifndef Win32
void Printf(UC id, UC SR, UC *data, US size) ;
#endif

#endif			// LIB_Log_h


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev001 Base)
// 15.02.12 | Yuichi Hoshino      | Log ID定義取得のためにDD_LOG.hをインクルード
// 15.03.30 | Yuichi Hoshino      | Release Revision 001
//
//


// ****************************************************************  [ LIB_Log.h : EOF ]  *********************************************************************
