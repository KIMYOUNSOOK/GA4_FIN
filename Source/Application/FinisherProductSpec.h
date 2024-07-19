// File Name:	FinisherProductSpec.h
// Description:	Header File of Finisher Product Specification
// Engineer:	Xxxxxx Xxxxxx
// Last Edit:	XX.XX.XX
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef FinisherProductSpec_h
#define FinisherProductSpec_h

////////////////////////////////////////////////////////////
// Include File
////////////////////////////////////////////////////////////

#include "ECdiFinDevCont.h"


////////////////////////////////////////////////////////////
// Definition
////////////////////////////////////////////////////////////

// --- Number of Subsystem Instance ---
#define DEV_CONT_NUM					2										// 重連しないフィニッシャーは2(All、Own)
#define OUT_DEST_NUM					1
#define IN_SRC_NUM						0

// --- Own Finisher Device Controller ID ---
// #define OWN_DEV_CONT_ID					eCdiFinDevCont_Aries
#define OWN_DEV_CONT_ID					eCdiFinDevCont_GA4Fin

// --- Number of Sheet Data ---
#define SHEETNO_NUM						16

// --- Diag Operation ---
#define DIAG_SWITCH_DC128				false
#define DIAG_SWITCH_DC131				true
#define DIAG_SWITCH_DC135				true
#define DIAG_SWITCH_DC140				false
#define DIAG_SWITCH_DC301				true
#define DIAG_SWITCH_DC330				true
#define DIAG_SWITCH_DC361				false
#define DIAG_SWITCH_DC726				false
#define DIAG_SWITCH_DC1100				false
#define DIAG_SWITCH_DC1201				false
#define DIAG_SWITCH_DC1202				false


#endif // #ifndef FinisherProductSpec_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
