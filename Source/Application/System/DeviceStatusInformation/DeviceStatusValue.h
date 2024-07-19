// File Name:	DeviceStatusValue.h
// Description:	Device Status Value Define
// Engineer:	Xxxxxx Xxxxxx
// Last Edit:	XX.XX.XX
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef DeviceStatusValue_h
#define DeviceStatusValue_h

// GA-Fin Xxxx Xxxx(XXXXH)
typedef enum /*EDvstTrayFullStackValue*/
{
	DVST_TRAY_STK_STTS_NOT_FULL = 0,		// Not Full
	DVST_TRAY_STK_STTS_FULL = 1,			// Full
	DVST_TRAY_STK_STTS_MIX_SIZE_FULL = 2,
	DVST_TRAY_STK_STTS_SET_OVER_COUNT = 3,	
} EDvstTrayFullStackValue;

// GA-Fin Xxxx Xxxx(XXXXH)
typedef enum /*EDvstStapleSupplyLevelValue*/
{
	DVST_STPL_SUPPLY_LEVEL_STTS_HARIOK = 0,		// Not Full
	DVST_STPL_SUPPLY_LEVEL_STTS_NEAR_LOW = 1,			// Full
	DVST_STPL_SUPPLY_LEVEL_STTS_LOW = 2,
} EDvstStapleSupplyLevelValue;


#endif // #ifndef DeviceStatusValue_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file based on Base SW VXX
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 15.07.13	| Yuichi Hoshino	| Created this file
// 15.07.13	| Yuichi Hoshino	| Design Improvement [ Device Status Valueヘッダファイル作成 ]
// 16.XX.XX	| Yuichi Hoshino	| Release V2 (Revision 00)
//
