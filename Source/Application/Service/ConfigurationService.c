// File Name:	ConfigurationService.c
// Description:	Implimentation File of Configuration Service
// Engineer:	Yuichi Hoshino
// Last Edit:	14.12.29
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "ConfigurationService.h"

#include "COputInformer.h"
#include "CCfgInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckOptionDetection
// Description   : オプションの接続有無を公開する。
// Parameter     : optionID (Option ID)
// Return        : オプション接続有無
//               :  true = 接続あり
//               :  false = 接続なし
///////////////////////////////////////////////////////////////////
bool SRVC_CheckOptionDetection(EOptionID optionID)
{
	return COputInformer_GetOptionDetect(optionID);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetOptionType
// Description   : オプションの種別を公開する。
// Parameter     : optionID (Option ID)
// Return        : オプション種別(オプション毎に値の意味の定義が異
//               : なる)
///////////////////////////////////////////////////////////////////
UC SRVC_GetOptionType(EOptionID optionID)
{
	return COputInformer_GetOptionType(optionID);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetPowerUpYear
// Description   : 電源ON時にシステムコントローラーより通知された日
//               : 付情報の年数を公開する。
// Parameter     : ‐
// Return        : 0～65534 = 年数[×1年]
//               : 0xFFFF = 不明(システムが年数を管理していない)
///////////////////////////////////////////////////////////////////
US SRVC_GetPowerUpYear(void)
{
	CCfgDevContConfiguration* pCfg;

	pCfg = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);

	return CCfgDevContConfiguration_GetSetDateYear(pCfg);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetPowerUpMonth
// Description   : 電源ON時にシステムコントローラーより通知された日
//               : 付情報の月数を公開する。
// Parameter     : ‐
// Return        : 1～12 = 月[×1月]
//               : 0xFF = 不明(システムが月数を管理していない)
///////////////////////////////////////////////////////////////////
UC SRVC_GetPowerUpMonth(void)
{
	CCfgDevContConfiguration* pCfg;

	pCfg = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);

	return CCfgDevContConfiguration_GetSetDateMonth(pCfg);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetPowerUpDay
// Description   : 電源ON時にシステムコントローラーより通知された日
//               : 付情報の日数を公開する。
// Parameter     : ‐
// Return        : 1～31 = 日[×1日]
//               : 0xFF = 不明(システムが日数を管理していない)
///////////////////////////////////////////////////////////////////
UC SRVC_GetPowerUpDay(void)
{
	CCfgDevContConfiguration* pCfg;

	pCfg = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);

	return CCfgDevContConfiguration_GetSetDateDay(pCfg);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetPowerUpHour
// Description   : 電源ON時にシステムコントローラーより通知された時
//               : 刻情報の時を公開する。
// Parameter     : ‐
// Return        : 0～23 = 時[×1時]
//               : 0xFF = 不明(システムが時を管理していない)
///////////////////////////////////////////////////////////////////
UC SRVC_GetPowerUpHour(void)
{
	CCfgDevContConfiguration* pCfg;

	pCfg = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);

	return CCfgDevContConfiguration_GetSetDateHour(pCfg);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetPowerUpMinute
// Description   : 電源ON時にシステムコントローラーより通知された時
//               : 刻情報の分を公開する。
// Parameter     : ‐
// Return        : 0～59 = 分[×1分]
//               : 0xFF = 不明(システムが分を管理していない)
///////////////////////////////////////////////////////////////////
UC SRVC_GetPowerUpMinute(void)
{
	CCfgDevContConfiguration* pCfg;

	pCfg = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);

	return CCfgDevContConfiguration_GetSetDateMinute(pCfg);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetPowerUpSecond
// Description   : 電源ON時にシステムコントローラーより通知された時
//               : 刻情報の秒を公開する。
// Parameter     : ‐
// Return        : 0～59 = 秒[×1秒]
//               : 0xFF = 不明(システムが秒を管理していない)
///////////////////////////////////////////////////////////////////
UC SRVC_GetPowerUpSecond(void)
{
	CCfgDevContConfiguration* pCfg;

	pCfg = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);

	return CCfgDevContConfiguration_GetSetDateSecond(pCfg);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetHotLineDistance
// Description   : ホットライン信号の変化が示す搬送経路上のポイント
//               : までの距離情報を公開する。
// Parameter     : ‐
// Return        : 1～4000[×0.1mm]
///////////////////////////////////////////////////////////////////
US SRVC_GetHotLineDistance(void)
{
	CCfgDevContConfiguration* pCfg;

	pCfg = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);

//	if ( pCfg != NULL ){
		return CCfgDevContConfiguration_GetHotLineDistance(pCfg);
//	}
//	else{
//		return 1;
//	}
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetReleasePointDistance
// Description   : シートリリース可能ポイントまでの距離情報を公開する。
// Parameter     : ‐
// Return        : 1～4000[×0.1mm]
///////////////////////////////////////////////////////////////////
US SRVC_GetReleasePointDistance(void)
{
	CCfgDevContConfiguration* pCfg;

	pCfg = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);

//	if ( pCfg != NULL ){
		return CCfgDevContConfiguration_GetReleasePointDistance(pCfg);
//	}
//	else{
//		return 1;
//	}
}
///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetProductivity
// Description   : 
// Parameter     : ‐
// Return        : 1～200[×1PPM]
///////////////////////////////////////////////////////////////////
UC SRVC_GetProductivity(void)
{
	CCfgDevContConfiguration* pCfg;

	pCfg = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);

		return CCfgDevContConfiguration_GetProductivity(pCfg);
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.12.05	| Yuichi Hoshino	| Created this file
// 14.12.05	| Yuichi Hoshino	| Release V0 (Revision 00)
// 14.12.29	| Yuichi Hoshino	| Introduction Spec [ 電源ON時刻取得サービス追加 ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//
