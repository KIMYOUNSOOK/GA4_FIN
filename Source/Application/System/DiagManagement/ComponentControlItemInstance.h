// File Name:   ComponentControlItemInstance.h
// Description: Header File of Component Control Item
// Engineer:    Xxxx Xxxx
// Last Edit:         2014/09/01 9:32:45
// Revision: XX
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef ComponentControlItemInstance_h
#define ComponentControlItemInstance_h


// --- Input Component Item Spec ---
static const DiagInputComponentItemSpec cDiag_InputComponentItemSpec[] = {
	{ 12, 100, DD_DI_IOT_EXIT_SNR, OPTID_BASE},					// IOT Exit Sensor(Hot Line)
	{ 12, 101, DD_DI_COMPILE_EXIT_SNR, OPTID_BASE},				// Compile Exit Sensor
	{ 12, 102, DD_DI_REGI_CL_SNR, OPTID_BASE},					// Regi Clutch Sensor
	{ 12, 103, DD_DI_UNIT0_SNR, OPTID_BASE},					// Finisher/Mailbox Detect Sensor

	{ 12, 200, DD_DI_EJECT_HOME_SNR, OPTID_BASE},				// Eject  Home Sensor
	{ 12, 201, DD_DI_SUB_PADDLE_HOME_SNR, OPTID_BASE},			// Sub Paddle Home Sensor
	{ 12, 202, DD_DI_SET_CLAMP_HOME_SNR, OPTID_BASE},			// Set Clamp Home Sensor

	{ 12, 210, DD_DI_RIGHT_TAMPER_HOME_SNR, OPTID_BASE},		// Right Tamper Home Sensor
	{ 12, 211, DD_DI_LEFT_TAMPER_HOME_SNR, OPTID_BASE},			// Left Tamper Home Sensor

	{ 12, 220, DD_DI_LOW_STAPLE_SNR, OPTID_BASE},				// Low Staple Sensor
	{ 12, 221, DD_DI_SELF_PRIMING_SNR, OPTID_BASE},				// Self Priming Sensor
	{ 12, 222, DD_DI_STAPLE_HOME_SNR, OPTID_BASE},				// Staple Home Sensor

	{ 12, 230, DD_DI_STACKER_NO_PAPER_FULL_SNR, OPTID_BASE},	// Stacker No Paper Full Sensor
	{ 12, 231, DD_DI_STACKER_TRAY_HEIGHT1_SNR, OPTID_BASE},		// Stacker Height Sensor 1
	{ 12, 232, DD_DI_STACKER_TRAY_HEIGHT2_SNR, OPTID_BASE},		// Stacker Height Sensor 2

	{ 12, 300, DD_DI_REAR_COVER_OPEN_SNR, OPTID_BASE},			// Finisher Rear Door Sensor
	{ 12, 301, DD_DI_STAPLE_DOOR_INTERLOCK_SW, OPTID_BASE}		// Finisher Staple Door Switch
};


// --- Parallel Execution Prohibit Item Identifier ---

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_10[] = {
	{ 12, 11 },
	{ 12, 12 },
	{ 0,  0  }	// End
};
static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_11[] = {
	{ 12, 10 },
	{ 12, 12 },
	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_12[] = {
	{ 12, 10 },
	{ 12, 11 },
	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_13[] = {
	{ 12, 14 },
	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_14[] = {
	{ 12, 13 },
	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_20[] = {
	{ 12, 21 },
	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_21[] = {
	{ 12, 20 },
	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_30[] = {
	{ 12, 31 },
	{ 12, 32 },
	{ 12, 33 },
	{ 12, 34 },
	{ 12, 35 },
	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_31[] = {
 	{ 12, 30 },
	{ 12, 32 },
	{ 12, 33 },
	{ 12, 34 },
	{ 12, 35 },
	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_32[] = {
	{ 12, 30 },
	{ 12, 31 },
	{ 12, 33 },
	{ 12, 34 },
	{ 12, 35 },
	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_33[] = {
	{ 12, 30 },
	{ 12, 31 },
	{ 12, 32 },
	{ 12, 34 },
	{ 12, 35 },
	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_34[] = {
	{ 12, 30 },
	{ 12, 31 },
	{ 12, 32 },
	{ 12, 33 },
	{ 12, 35 },
	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_35[] = {
	{ 12, 30 },
	{ 12, 31 },
	{ 12, 32 },
	{ 12, 33 },
	{ 12, 34 },
	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_40[] = {
	{ 12, 41 },
	{ 12, 42 },
	{ 12, 43 },
	{ 12, 44 },
	{ 12, 45 },
	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_41[] = {
	{ 12, 40 },
	{ 12, 42 },
	{ 12, 43 },
	{ 12, 44 },
	{ 12, 45 },
	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_42[] = {
	{ 12, 40 },
	{ 12, 41 },
	{ 12, 43 },
	{ 12, 44 },
	{ 12, 45 },
	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_43[] = {
	{ 12, 40 },
	{ 12, 41 },
	{ 12, 42 },
	{ 12, 44 },
	{ 12, 45 },
	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_44[] = {
	{ 12, 40 },
	{ 12, 41 },
	{ 12, 42 },
	{ 12, 43 },
	{ 12, 45 },
	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_45[] = {
	{ 12, 40 },
	{ 12, 41 },
	{ 12, 42 },
	{ 12, 43 },
	{ 12, 44 },
	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_50[] = {
	{ 12, 51 },
	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_51[] = {
	{ 12, 50 },
 	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_60[] = {
	{ 12, 61 },
	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_61[] = {
	{ 12, 60 },
	{ 0,  0  }	// End
};

static const DiagParallelExecProhNgItem cDiag_ParallelExecProhNgItem_12_70[] = {
	{ 0,  0  },	// End
};


// --- Output Component Item Spec ---
static const DiagOutputComponentItemSpec cDiag_OutputComponentItemSpec[] = {
	{ 12, 10, false, MID_TRNS, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_10 },		// Transport Motor(Forward/Low Speed)
	{ 12, 11, false, MID_TRNS, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_11 },		// Transport Motor(Forward/Middle Speed)
	{ 12, 12, false, MID_TRNS, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_12 },		// Transport Motor(Forward/High Speed)
	{ 12, 13, false, MID_TRNS, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_13 },		// Transport Gate Solenoid(Pull)
	{ 12, 14, false, MID_TRNS, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_14 },		// Transport Gate Solenoid(Push)

	{ 12, 20, false, MID_CMPL, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_20 },	// Eject Compile Motor(Forward/Eject)
	{ 12, 21, false, MID_CMPL, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_21 },	// Eject Compile Motor(Release/Sub Paddle Speed)
	{ 12, 22, false, MID_CMPL, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_21 },	// Set Clutch ON/OFF

	{ 12, 30, false, MID_CMPL, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_30 },	// Right Tamper Motor(Front/Low Speed)
	{ 12, 31, false, MID_CMPL, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_31 },	// Right Tamper Motor(Front/Middle Speed)
	{ 12, 32, false, MID_CMPL, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_32 },	// Right Tamper Motor(Front/High Speed)
	{ 12, 33, false, MID_CMPL, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_33 },	// Right Tamper Motor(Rear/Low Speed)
	{ 12, 34, false, MID_CMPL, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_34 },	// Right Tamper Motor(Rear/Middle Speed)
	{ 12, 35, false, MID_CMPL, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_35 },	// Right Tamper Motor(Rear/High Speed)
	{ 12, 36, false, MID_CMPL, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_35 },	// For Test

	{ 12, 40, false, MID_CMPL, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_40 },	// Left Tamper Motor(Front/Low Speed)
	{ 12, 41, false, MID_CMPL, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_41 },	// Left Tamper Motor(Front/Middle Speed)
	{ 12, 42, false, MID_CMPL, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_42 },	// Left Tamper Motor(Front/High Speed)
	{ 12, 43, false, MID_CMPL, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_43 },	// Left Tamper Motor(Rear/Low Speed)
	{ 12, 44, false, MID_CMPL, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_44 },	// Left Tamper Motor(Rear/Middle Speed)
	{ 12, 45, false, MID_CMPL, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_45 },	// Left TamperÇäOë§ï˚å¸Ç…çÇë¨(1337pps)Ç≈à⁄ìÆÇ≥ÇπÇÈÅB
	{ 12, 46, false, MID_CMPL, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_45 },	// For Test

	{ 12, 50, false, MID_CMPL, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_50 },	// Stapler Motor(Forward)
	{ 12, 51, false, MID_CMPL, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_51 },	// Stapler Motor(Reverse)
	{ 12, 52, false, MID_CMPL, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_51 },	// Stapler Motor(Reverse, For Test)
	{ 12, 60, false, MID_TRAY, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_60 },	// Stacker Motor(Lift Up/Low Speed)
	{ 12, 61, false, MID_TRAY, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_61 },	// Stacker Motor(Lift Down/1417pps)

	{ 12, 70, false, MID_SYS, OPTID_BASE, cDiag_ParallelExecProhNgItem_12_70 },		// Finisher Main 24V Relay OFF
};


#endif          // #ifndef ComponentControlItemInstance_h
