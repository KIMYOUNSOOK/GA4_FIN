// File Name:	NvmService.c
// Description:	Implimentation File of NVM Service
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "NvmService.h"

#include "CNvmInformer.h"
#include "CHfsiInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetNvmValue
// Description   : NVMパラメータの値を公開する。
// Parameter     : id (NVM ID)
// Return        : NVM Value
///////////////////////////////////////////////////////////////////
UL SRVC_GetNvmValue(ENvmID id)
{
	CNvmNVM* pNvm = CNvmInformer_Get(id);

	return CNvmNVM_GetValue(pNvm);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetNvmValueUC
// Description   : Unsigned Character型NVMパラメータの値を公開する。
// Parameter     : id (NVM ID)
// Return        : Unsigned Character NVM Value
///////////////////////////////////////////////////////////////////
UC SRVC_GetNvmValueUC(ENvmID id)
{
	CNvmNVM* pNvm = CNvmInformer_Get(id);

	return (UC)(CNvmNVM_GetValue(pNvm));
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetNvmValueSC
// Description   : Signed Character型NVMパラメータの値を公開する。
// Parameter     : id (NVM ID)
// Return        : Signed Character NVM Value
///////////////////////////////////////////////////////////////////
SC SRVC_GetNvmValueSC(ENvmID id)
{
	CNvmNVM* pNvm = CNvmInformer_Get(id);

	return (SC)(CNvmNVM_GetValue(pNvm));
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetNvmValueUS
// Description   : Unsigned Short型NVMパラメータの値を公開する。
// Parameter     : id (NVM ID)
// Return        : Unsigned Short NVM Value
///////////////////////////////////////////////////////////////////
US SRVC_GetNvmValueUS(ENvmID id)
{
	CNvmNVM* pNvm = CNvmInformer_Get(id);

	return (US)(CNvmNVM_GetValue(pNvm));
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetNvmValueSS
// Description   : Signed Short型NVMパラメータの値を公開する。
// Parameter     : id (NVM ID)
// Return        : Signed Short NVM Value
///////////////////////////////////////////////////////////////////
SS SRVC_GetNvmValueSS(ENvmID id)
{
	CNvmNVM* pNvm = CNvmInformer_Get(id);

	return (SS)(CNvmNVM_GetValue(pNvm));
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetNvmValueUL
// Description   : Unsigned Long型NVMパラメータの値を公開する。
// Parameter     : id (NVM ID)
// Return        : Unsigned Long NVM Value
///////////////////////////////////////////////////////////////////
UL SRVC_GetNvmValueUL(ENvmID id)
{
	CNvmNVM* pNvm = CNvmInformer_Get(id);

	return CNvmNVM_GetValue(pNvm);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetNvmValueSL
// Description   : Signed Long型NVMパラメータの値を公開する。
// Parameter     : id (NVM ID)
// Return        : Signed Long NVM Value
///////////////////////////////////////////////////////////////////
SL SRVC_GetNvmValueSL(ENvmID id)
{
	CNvmNVM* pNvm = CNvmInformer_Get(id);

	return (SL)(CNvmNVM_GetValue(pNvm));
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_SetNvmValue
// Description   : NVMパラメータの値を更新する。
// Parameter     : value (Set Value)
//               : id (NVM ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SRVC_SetNvmValue(UL value, ENvmID id)
{
	CNvmNVM* pNvm = CNvmInformer_Get(id);

	CNvmNVM_SetValue(pNvm, value);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_SetNvmValueUC
// Description   : 
// Parameter     : value (Set Value)
//               : id (NVM ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SRVC_SetNvmValueUC(UC value, ENvmID id)
{
	CNvmNVM* pNvm = CNvmInformer_Get(id);

	CNvmNVM_SetValue(pNvm, (UL)value);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_SetNvmValueSC
// Description   : 
// Parameter     : value (Set Value)
//               : id (NVM ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SRVC_SetNvmValueSC(SC value, ENvmID id)
{
	CNvmNVM* pNvm = CNvmInformer_Get(id);

	CNvmNVM_SetValue(pNvm, (UL)value);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_SetNvmValueUS
// Description   : 
// Parameter     : value (Set Value)
//               : id (NVM ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SRVC_SetNvmValueUS(US value, ENvmID id)
{
	CNvmNVM* pNvm = CNvmInformer_Get(id);

	CNvmNVM_SetValue(pNvm, (UL)value);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_SetNvmValueSS
// Description   : 
// Parameter     : value (Set Value)
//               : id (NVM ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SRVC_SetNvmValueSS(SS value, ENvmID id)
{
	CNvmNVM* pNvm = CNvmInformer_Get(id);

	CNvmNVM_SetValue(pNvm, (UL)value);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_SetNvmValueUL
// Description   : 
// Parameter     : value (Set Value)
//               : id (NVM ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SRVC_SetNvmValueUL(UL value, ENvmID id)
{
	CNvmNVM* pNvm = CNvmInformer_Get(id);

	CNvmNVM_SetValue(pNvm, value);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_SetNvmValueSL
// Description   : 
// Parameter     : value (Set Value)
//               : id (NVM ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SRVC_SetNvmValueSL(SL value, ENvmID id)
{
	CNvmNVM* pNvm = CNvmInformer_Get(id);

	CNvmNVM_SetValue(pNvm, (UL)value);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetHfsiValue
// Description   : HFSIカウンタの現在地を公開する。
// Parameter     : id (HFSI ID)
// Return        : HFSI Current Value
///////////////////////////////////////////////////////////////////
UL SRVC_GetHfsiValue(EHfsiItemID id)
{
	CHfsiHFSI* pHfsi = CHfsiInformer_Get(id);

	return CHfsiHFSI_GetValue(pHfsi);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_IncrementHfsi
// Description   : HFSIカウンタをインクリメントする。
// Parameter     : id (HFSI ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SRVC_IncrementHfsi(EHfsiItemID id)
{
	CHfsiHFSI* pHfsi = CHfsiInformer_Get(id);

	CHfsiHFSI_Increment(pHfsi);
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 15.03.30	| Yuichi Hoshino	| Created this file
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 00)
//
