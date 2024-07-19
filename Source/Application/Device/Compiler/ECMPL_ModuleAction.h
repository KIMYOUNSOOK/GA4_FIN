// File Name:	ECMPL_ModuleAction.h
// Description:	Compiler Moduleの子部品Actionを定義
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
	/* 0～15 は、Sequenceに使用する */
	
	// ステープルヘッドホーム動作
	CMPL_MDL_ACT_STAPLE_HEAD_HOME,	//0
	// イジェクターホーム動作
	CMPL_MDL_ACT_EJECTOR_HOME,		//
	// ステープルヘッド針出し動作
	CMPL_MDL_ACT_STAPLE_HARIOUT,	//
	// SubPaddle Home
	CMPL_MDL_ACT_PADDLE_HOME,
	// パドルコンパイル動作
	CMPL_MDL_ACT_PADDLE_COMPILE,	     //
	// イジェクト動作
	CMPL_MDL_ACT_EJECT,				//5			
	// タンパーサイズポジショニング動作
	CMPL_MDL_ACT_TAMP_SIZE_POS,		//4
 	// Tamper MissRegi (V3.5.95)
	CMPL_MDL_ACT_TAMP_MISS_REGI,   
	// 用紙収容待ち
//	CMPL_MDL_ACT_SHEET_COMPILE_WAIT,		//
	  // ProcessEnd 励磁 OFF 動作
	CMPL_MDL_ACT_CURRENT_OFF,  //
	// ノーマルタンピング動作
	CMPL_MDL_ACT_NORMAL_TAMPING,			//
	// イジェクトタンピング動作
	CMPL_MDL_ACT_EJECT_TAMPING,		//
	// ステープル動作
	CMPL_MDL_ACT_STAPLE,			//
	// オフセット動作
	CMPL_MDL_ACT_OFFSET,			// 
	// リターン動作
	CMPL_MDL_ACT_RETURN,			//
	// Sequence Start
	CMPL_MDL_ACT_START,
	// Sequence End
	CMPL_MDL_ACT_END,
	// オープン動作
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

