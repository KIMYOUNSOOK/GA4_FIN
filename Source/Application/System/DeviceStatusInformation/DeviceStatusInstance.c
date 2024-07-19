// File Name:	DeviceStatusInstance.c
// Description:	Instance of Device Status
// Engineer:	Shin.KS
// Last Edit:	2015/04/14
// Revision:	001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
////変更履歴
//2015/11/09 FXK　　Specデータの不具合修正
//			SubNoは中腹される値があったため、Categoryごとに順次的に再定義(FAILは要らないので除く)
//			StopTypeがN.Aの場合、ｎgThresh値を０にして動作上に問題なく動くように変更。
//			(ちなみに、発生してもSetBitが０になって、処理される)
//

#include "CDvstDeviceStatus.h"

// [ Global A4-Fin Download Mode Fail (B040H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB040H[] = {
	{	0,	0    },         // 0 = Not Occur
	{   12,	334  },         // 1 = Occur
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo0 = {
	0xB040,
	0,
	1,
	DVST_CTGRY_FAIL,
	(
		(0x00000001 << 0)	// Finisher
	),
	0,
	false,
	SYS_PROC_STOP_HARD,
	cDvst_StatusValueInfoB040H,
	(sizeof(cDvst_StatusValueInfoB040H)/sizeof(DvstStatusValueInfo))
};

// [Global A4-Fin Static Jam(3100H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB140H[] = {
	{   0,  0    },         // 0 = Not Occur
	{   12,  912    },         // 1 = Occur
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo1 = {
	0xB140,
	0,
	1,
	DVST_CTGRY_JAM,
	(
		(0x00000001 << 0)	// Finisher
	),
	1, //0,
	false,
	SYS_PROC_STOP_CYCLE,
	cDvst_StatusValueInfoB140H,
	(sizeof(cDvst_StatusValueInfoB140H)/sizeof(DvstStatusValueInfo))
};

// [ Global A4-Fin Compile Exit Sensor ON Jam(3102H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB141H[] = {
	{   0,  0    },         // 0 = Not Occur
	{   12,  152    },         // 1 = Occur
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo2 = {
	0xB141,
	0,
	1,
	DVST_CTGRY_JAM,
	(
		(0x00000001 << 0)	// Finisher
	),
	2,
	false,
	SYS_PROC_STOP_CYCLE,
	cDvst_StatusValueInfoB141H,
	(sizeof(cDvst_StatusValueInfoB141H)/sizeof(DvstStatusValueInfo))
};

// [Global A4-Fin Compile Exit Sensor OFF Jam(3103H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB142H[] = {
	{   0,  0    },         // 0 = Not Occur
	{   12,  122    },         // 1 = Occur
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo3 = {
	0xB142,
	0,
	1,
	DVST_CTGRY_JAM,
	(
		(0x00000001 << 0)	// Finisher
	),
	3,
	false,
	SYS_PROC_STOP_CYCLE,
	cDvst_StatusValueInfoB142H,
	(sizeof(cDvst_StatusValueInfoB142H)/sizeof(DvstStatusValueInfo))
};

// [ Global A4-Fin Set Eject Jam(3104H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB143H[] = {
	{   0,  0    },         // 0 = Not Occur
	{   12,  161    },         // 1 = Occur
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo4 = {
	0xB143,
	0,
	1,
	DVST_CTGRY_JAM,
	(
		(0x00000001 << 0)	// Finisher
	),
	4,
	false,
	SYS_PROC_STOP_CYCLE,
	cDvst_StatusValueInfoB143H,
	(sizeof(cDvst_StatusValueInfoB143H)/sizeof(DvstStatusValueInfo))
};

// [Global A4-Fin NVM Fail (B240H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB240H[] = {
	{   0,  0    },         // 0 = Not Occur
	{   12,  210    },         // 1 = Occur
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo5 = {
	0xB240,
	0,
	1,
	DVST_CTGRY_FAIL,
	(
		(0x00000001 << 0)	// Finisher   //(0x00000001 << 2)	// Stapler
	),
	4,
	false,
	SYS_PROC_STOP_HARD,
	cDvst_StatusValueInfoB240H,
	(sizeof(cDvst_StatusValueInfoB240H)/sizeof(DvstStatusValueInfo))
};

// [Global A4-Fin Right Tamper Home Sensor ON Fail(3204H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB241H[] = {
	{   0,  0    },         // 0 = Not Occur
	{   12,  286    },         // 1 = Occur
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo6 = {
	0xB241,
	0,
	1,
	DVST_CTGRY_FAIL,
	(
		(0x00000001 << 0)	// Finisher    //(0x00000001 << 1)	// OutTray
	),
	5,
	false,
	SYS_PROC_STOP_CYCLE,
	cDvst_StatusValueInfoB241H,
	(sizeof(cDvst_StatusValueInfoB241H)/sizeof(DvstStatusValueInfo))
};

// [ Global A4-Fin Right Tamper Home Sensor OFF Fail(3205H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB242H[] = {
	{   0,  0    },         // 0 = Not Occur
	{   12,  287    },         // 1 = Occur
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo7 = {
	0xB242,
	0,
	1,
	DVST_CTGRY_FAIL,
	(
		(0x00000001 << 0)	// Finisher    //(0x00000001 << 1)	// OutTray
	),
	5,  
	false,
	SYS_PROC_STOP_CYCLE,
	cDvst_StatusValueInfoB242H,
	(sizeof(cDvst_StatusValueInfoB242H)/sizeof(DvstStatusValueInfo))
};

// [ Global A4-Fin Left Tamper Home Sensor ON Fail(3202H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB243H[] = {
	{   0,  0    },         // 0 = Not Occur
	{   12,  288    },         // 1 = Occur
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo8 = {
	0xB243,
	0,
	1,
	DVST_CTGRY_FAIL,
	(
		(0x00000001 << 0)	// Finisher    //(0x00000001 << 1)	// OutTray
	),
	5, 
	false,
	SYS_PROC_STOP_CYCLE,
	cDvst_StatusValueInfoB243H,
	(sizeof(cDvst_StatusValueInfoB243H)/sizeof(DvstStatusValueInfo))
};

// [ Global A4-Fin Left Tamper Home Sensor OFF Fail(3203H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB244H[] = {
	{   0,  0    },         // 0 = Not Occur
	{   12,  289    },         // 1 = Occur
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo9 = {
	0xB244,
	0,
	1,
	DVST_CTGRY_FAIL,
	(
		(0x00000001 << 0)	// Finisher    //(0x00000001 << 1)	// OutTray
	),
	5, 
	false,
	SYS_PROC_STOP_CYCLE,
	cDvst_StatusValueInfoB244H,
	(sizeof(cDvst_StatusValueInfoB244H)/sizeof(DvstStatusValueInfo))
};

// [ Global A4-Fin Staple Fail(320AH) ] Spec Data  ################################## No. 10
static const DvstStatusValueInfo cDvst_StatusValueInfoB245H[] = {
	{   0,  0    },         // 0 = Not Occur
	{   12,  291    },         // 1 = Occur
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo10 = {
	0xB245,
	0,
	1,
	DVST_CTGRY_FAIL,
	(
		(0x00000001 << 0)	// Finisher  //(0x00000001 << 1)	// OutTray
	),
	9,
	false,
	SYS_PROC_STOP_CYCLE,
	cDvst_StatusValueInfoB245H,
	(sizeof(cDvst_StatusValueInfoB245H)/sizeof(DvstStatusValueInfo))
};

// [ Global A4-Fin Eject Home Sensor ON Fail(3206H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB246H[] = {
	{   0,  0    },         // 0 = Not Occur
	{   12,  259    },         // 1 = Occur
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo11 = {
	0xB246,
	0,
	1,
	DVST_CTGRY_FAIL,
	(
		(0x00000001 << 0)	// Finisher    //(0x00000001 << 1)	// OutTray
	),
	7,
	false,
	SYS_PROC_STOP_CYCLE,
	cDvst_StatusValueInfoB246H,
	(sizeof(cDvst_StatusValueInfoB246H)/sizeof(DvstStatusValueInfo))
};

// [ Global A4-Fin Eject Home Sensor OFF Fail(3207H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB247H[] = {
	{   0,  0    },         // 0 = Not Occur
	{   12,  280    },         // 1 = Occur
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo12 = {
	0xB247,
	0,
	1,
	DVST_CTGRY_FAIL,
	(
		(0x00000001 << 0)	// Finisher   //(0x00000001 << 1)	// OutTray
	),
	7, 
	false,
	SYS_PROC_STOP_CYCLE,
	cDvst_StatusValueInfoB247H,
	(sizeof(cDvst_StatusValueInfoB247H)/sizeof(DvstStatusValueInfo))
};

// [ Global A4-Fin Set Clamp Home SNR ON Fail (3702H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB248H[] = {
	{   0,  0    },         // 0 = Not Occur
	{   12,  283    },         // 1 = Occur
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo13 = {
	0xB248,
	0,
	1,
	DVST_CTGRY_FAIL,
	(
		(0x00000001 << 0)	// Finisher    //(0x00000001 << 2)	// Stapler
	),
	7, 
	false,
	SYS_PROC_STOP_CYCLE,
	cDvst_StatusValueInfoB248H,
	(sizeof(cDvst_StatusValueInfoB248H)/sizeof(DvstStatusValueInfo))
};

// [ Global A4-Fin Set Clamp Home SNR OFF Fail (3702H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB249H[] = {
	{   0,  0    },         // 0 = Not Occur
	{   12,  284    },         // 1 = Occur
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo14 = {
	0xB249,
	0,
	1,
	DVST_CTGRY_FAIL,
	(
		(0x00000001 << 0)	// Finisher    /./(0x00000001 << 2)	// Stapler
	),
	7,
	false,
	SYS_PROC_STOP_CYCLE,
	cDvst_StatusValueInfoB248H,
	(sizeof(cDvst_StatusValueInfoB248H)/sizeof(DvstStatusValueInfo))
};

// [Global A4-Fin Subpaddle Home Snr On Fail (3702H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB24AH[] = {
	{   0,  0    },         // 0 = Not Occur
	{   12,  239    },         // 1 = Occur
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo15 = {
	0xB24A,
	0,
	1,
	DVST_CTGRY_FAIL,
	(
		(0x00000001 << 0)	// Finisher    //(0x00000001 << 2)	// Stapler
	),
	6,
	false,
	SYS_PROC_STOP_CYCLE,
	NULL,
	0
};

// [ Global A4-Fin Subpaddle Home Snr OFF Fail(3702H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB24BH[] = {
	{   0,  0    },         // 0 = Not Occur
	{   12,  240    },         // 1 = Occur
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo16 = {
	0xB24B,
	0,
	1,
	DVST_CTGRY_FAIL,
	(
		(0x00000001 << 0)	// Finisher   //(0x00000001 << 2)	// Stapler
	),
	6,
	false,
	SYS_PROC_STOP_CYCLE,
	NULL,
	0
};

// DSNO_STACKER_TRAY_FAIL,
// [ Global A4-Fin STACKER_TRAY_FAIL (3207H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB24CH[] = {
	{   0,  0    },         // 0 = Not Occur
	{   12,  211    },         // 1 = Occur
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo17 = {
	0xB24C,
	0,
	1,
	DVST_CTGRY_FAIL,
	(
		(0x00000001 << 0)	// Finisher  //(0x00000001 << 1)	// OutTray
	),
	8,
	false,
	SYS_PROC_STOP_CYCLE,
	cDvst_StatusValueInfoB24CH,
	(sizeof(cDvst_StatusValueInfoB24CH)/sizeof(DvstStatusValueInfo))
};

// DSNO_STACKER_UPLIMIT_FAIL,
// [ Global A4-Fin Eject Home Sensor OFF Fail(3207H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB24DH[] = {
	{   0,  0    },         // 0 = Not Occur
	{   12,  212    },         // 1 = Occur
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo18 = {
	0xB24D,
	0,
	1,
	DVST_CTGRY_FAIL,
	(
		(0x00000001 << 0)	// Finisher  //(0x00000001 << 1)	// OutTray
	),
	8,
	false,
	SYS_PROC_STOP_CYCLE,
	cDvst_StatusValueInfoB24DH,
	(sizeof(cDvst_StatusValueInfoB24DH)/sizeof(DvstStatusValueInfo))
};

// DSNO_STACKER_LOWLIMIT_FAIL,
// [ Global A4-Fin STACKER_LOWLIMIT_FAIL (3207H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB24EH[] = {
	{   0,  0    },         // 0 = Not Occur
	{   12,  213    },         // 1 = Occur
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo19 = {
	0xB24E,
	0,
	1,
	DVST_CTGRY_FAIL,
	(
		(0x00000001 << 0)	// Finisher   //(0x00000001 << 1)	// OutTray
	),
	8,
	false,
	SYS_PROC_STOP_CYCLE,
	cDvst_StatusValueInfoB24EH,
	(sizeof(cDvst_StatusValueInfoB24EH)/sizeof(DvstStatusValueInfo))
};

// DSNO_STAPLE_COVER_24V_FAIL,
// [ Global A4-Fin STAPLE_COVER_24V_DISCONNECT FAIL (3207H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB24FH[] = {
	{   0,  0    },         // 0 = Not Occur
	{   12,  290    },         // 1 = Occur
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo20 = {
	0xB24F,
	0,
	1,
	DVST_CTGRY_FAIL,
	(
		(0x00000001 << 0)	// Finisher    //(0x00000001 << 1)	// OutTray
	),
	9, 
	false,
	SYS_PROC_STOP_HARD,
	cDvst_StatusValueInfoB24FH,
	(sizeof(cDvst_StatusValueInfoB24FH)/sizeof(DvstStatusValueInfo))
};

// [ Global A4-Fin Finisher Rear Cover Open(3400H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB440H[] = {
	{   0,  0    },         // 0 = Close
	{   12,  302    },         // 1 = Open
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo21 = {
	0xB440,
	0,
	1,
	DVST_CTGRY_COVER,
	(
		(0x00000001 << 0)	// Finisher
	),
	1, 
	false,
	SYS_PROC_STOP_HARD,
	cDvst_StatusValueInfoB440H,
	(sizeof(cDvst_StatusValueInfoB440H)/sizeof(DvstStatusValueInfo))
};

// [ Global A4-Fin Finisher Staple Cover Open(3400H) ] Spec Data
static const DvstStatusValueInfo cDvst_StatusValueInfoB441H[] = {
	{   0,  0    },         // 0 = Close
	{   12,  303    },         // 1 = Open
};

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo22 = {
	0xB441,
	0,
	1,
	DVST_CTGRY_COVER,
	(
		(0x00000001 << 0)	// Finisher
	),
	2,
	false,
	SYS_PROC_STOP_HARD,
	cDvst_StatusValueInfoB441H,
	(sizeof(cDvst_StatusValueInfoB441H)/sizeof(DvstStatusValueInfo))
};

// [ Global A4-Fin  Initialization State(3600H) ] Spec Data
static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo23 = {
	0xB640,
	0,
	0, //1,
	DVST_CTGRY_OTHER,
	(
		(0x00000001 << 0)	// Finisher
	),
	1, 
	false,
	SYS_PROC_STOP_NO_ACT,
	NULL,
	0
};

// [ Global A4-Fin Staple Ready Sensor Fail(3641H) ] Spec Data
static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo24 = {
	0xB641,
	0,
	0,
	DVST_CTGRY_OTHER,
	(
		(0x00000001 << 2)	// Stapler
	),
	2,
	false,
	SYS_PROC_STOP_NO_ACT,
	NULL,
	0
};

// [ Global A4-Fin Staple Supply Level(3640H) ] Spec Data
static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo25 = {
	0xB642,
	0,
	0,
	DVST_CTGRY_OTHER,
	(
		(0x00000001 << 2)	// Stapler
	),
	3, 
	false,
	SYS_PROC_STOP_NO_ACT,
	NULL,
	0
};

// [ Global A4-Fin Output Tray Full Stack(3700H) ] Spec Data
static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo26 = {
	0xB740,
	0,
	0, //1,
	DVST_CTGRY_OTHER,
	(
		(0x00000001 << 0)	// Finisher
	),
	4,
	false,
	SYS_PROC_STOP_NO_ACT,
	NULL,
	0
};

// DSNO_STACKER_LOW_SAFETY

// [Global A4-Fin STACKER_LOW_SAFETY(3702H) ] Spec Data
static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo27 = {
	0xB741,
	0,
	0, //1,
	DVST_CTGRY_OTHER,
	(
		(0x00000001 << 0)	// Finisher    //(0x00000001 << 2)	// Stapler
	),
	5,
	false,
	SYS_PROC_STOP_NO_ACT, 
	NULL,
	0
};
// [ Global A4-Fin Staple Status NG(3702H) ] Spec Data
static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo28 = {
	0xB742,
	0,
	1,
	DVST_CTGRY_OTHER,
	(
		(0x00000001 << 0)	// Finisher    //(0x00000001 << 2)	// Stapler
	),
	6,
	false,
	SYS_PROC_STOP_CYCLE,
	NULL,
	0
};

// DSNO_SCARCH_SHEET_COMPILE,

// [ Global A4-Fin Finisher SCARCH_SHEET_COMPILE, (XX00H) ] Spec Data

static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo29 = {
	0xB743,
	0,
	1,
	DVST_CTGRY_COVER,
	(
		(0x00000001 << 0)	// Finisher
	),
	3, 
	false,
	SYS_PROC_STOP_NO_ACT,
	NULL,
	0
};
// [ Global A4-Fin System Controller Communication Fail(30FFH) ] Spec Data
static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo30 = {
	0x30FF,
	0,
	1,
	DVST_CTGRY_FAIL,
	(
		(0x00000001 << 0)	// Finisher
	),
	2,
	false,
	SYS_PROC_STOP_HARD,
	NULL,
	0
};
// [Global A4-Fin Logic Fail(30FEH) ] Spec Data
static const DvstDeviceStatusSpec cDvst_DeviceStatusSpecNo31 = {
	0x30FE,
	0,
	1,
	DVST_CTGRY_FAIL,
	(
		(0x00000001 << 0)	// Finisher
	),
	1,
	false,
	SYS_PROC_STOP_HARD,
	NULL,
	0
};

// --- Device Status Value ---
US gDvst_DeviceStatusValue[DSNO_NUM];

// --- Device Status Instance ---
CDvstDeviceStatus const cDvst_DeviceStatus[DSNO_NUM] = {
	{ &cDvst_DeviceStatusSpecNo0,	&(gDvst_DeviceStatusValue[DSNO_DOWNLOAD_MODE_FAIL])  },
	{ &cDvst_DeviceStatusSpecNo1,	&(gDvst_DeviceStatusValue[DSNO_STATIC_JAM])  },
	{ &cDvst_DeviceStatusSpecNo2,	&(gDvst_DeviceStatusValue[DSNO_CMPL_EXIT_SNR_ON_JAM])  },
	{ &cDvst_DeviceStatusSpecNo3,	&(gDvst_DeviceStatusValue[DSNO_CMPL_EXIT_SNR_OFF_JAM])  },
	{ &cDvst_DeviceStatusSpecNo4,	&(gDvst_DeviceStatusValue[DSNO_SET_EJECT_JAM])  },
	{ &cDvst_DeviceStatusSpecNo5,	&(gDvst_DeviceStatusValue[DSNO_NVM_FAIL])  },
	{ &cDvst_DeviceStatusSpecNo6,	&(gDvst_DeviceStatusValue[DSNO_RIGHT_TAMPER_ON_FAIL])  },
	{ &cDvst_DeviceStatusSpecNo7,	&(gDvst_DeviceStatusValue[DSNO_RIGHT_TAMPER_OFF_FAIL])  },
	{ &cDvst_DeviceStatusSpecNo8,	&(gDvst_DeviceStatusValue[DSNO_LEFT_TAMPER_ON_FAIL])  },
	{ &cDvst_DeviceStatusSpecNo9,	&(gDvst_DeviceStatusValue[DSNO_LEFT_TAMPER_OFF_FAIL])  },
	{ &cDvst_DeviceStatusSpecNo10,	&(gDvst_DeviceStatusValue[DSNO_STAPLE_FAIL])	},
	{ &cDvst_DeviceStatusSpecNo11,	&(gDvst_DeviceStatusValue[DSNO_EJECT_BELT_HOME_ON_FAIL])  },
	{ &cDvst_DeviceStatusSpecNo12,	&(gDvst_DeviceStatusValue[DSNO_EJECT_BELT_HOME_OFF_FAIL])  },
	{ &cDvst_DeviceStatusSpecNo13,	&(gDvst_DeviceStatusValue[DSNO_SETCLAMP_HOME_ON_FAIL])  },
	{ &cDvst_DeviceStatusSpecNo14,	&(gDvst_DeviceStatusValue[DSNO_SETCLAMP_HOME_OFF_FAIL])  },
	{ &cDvst_DeviceStatusSpecNo15,	&(gDvst_DeviceStatusValue[DSNO_PADDLE_HOME_ON_FAIL])  },
	{ &cDvst_DeviceStatusSpecNo16,	&(gDvst_DeviceStatusValue[DSNO_PADDLE_HOME_OFF_FAIL])  },
	{ &cDvst_DeviceStatusSpecNo17,	&(gDvst_DeviceStatusValue[DSNO_STACKER_TRAY_FAIL])  },
	{ &cDvst_DeviceStatusSpecNo18,	&(gDvst_DeviceStatusValue[DSNO_STACKER_UPLIMIT_FAIL])  },
	{ &cDvst_DeviceStatusSpecNo19,	&(gDvst_DeviceStatusValue[DSNO_STACKER_LOWLIMIT_FAIL])  },
	{ &cDvst_DeviceStatusSpecNo20,	&(gDvst_DeviceStatusValue[DSNO_STAPLE_COVER_24V_FAIL])  },
	{ &cDvst_DeviceStatusSpecNo21,	&(gDvst_DeviceStatusValue[DSNO_REAR_COVER_ILK_OPEN])  },
	{ &cDvst_DeviceStatusSpecNo22,	&(gDvst_DeviceStatusValue[DSNO_STAPLE_COVER_ILK_OPEN])  },
	{ &cDvst_DeviceStatusSpecNo23,	&(gDvst_DeviceStatusValue[DSNO_INIT_STATE])  },
	{ &cDvst_DeviceStatusSpecNo24,	&(gDvst_DeviceStatusValue[DSNO_STAPLE_READY_FAIL])  },
	{ &cDvst_DeviceStatusSpecNo25,	&(gDvst_DeviceStatusValue[DSNO_STAPLE_SUPPLY_LEVEL])  },
	{ &cDvst_DeviceStatusSpecNo26,	&(gDvst_DeviceStatusValue[DSNO_STACKER_FULL_STACK])  },
	{ &cDvst_DeviceStatusSpecNo27,	&(gDvst_DeviceStatusValue[DSNO_STACKER_LOW_SAFETY])  },
	{ &cDvst_DeviceStatusSpecNo28,	&(gDvst_DeviceStatusValue[DSNO_STAPLE_STATUS_NG])  },
	{ &cDvst_DeviceStatusSpecNo29,	&(gDvst_DeviceStatusValue[DSNO_SCARCH_SHEET_COMPILE])  },	
	{ &cDvst_DeviceStatusSpecNo30,	&(gDvst_DeviceStatusValue[DSNO_SYS_CONT_COMM_FAIL])  },		
	{ &cDvst_DeviceStatusSpecNo31,	&(gDvst_DeviceStatusValue[DSNO_LOGIC_FAIL])  },	
};
