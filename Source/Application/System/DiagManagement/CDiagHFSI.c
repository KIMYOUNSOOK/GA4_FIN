
#include "CDiagHFSI.h"

void CDiagHFSI_Constructor(CDiagHFSI* pThis)
{
	
}

US CDiagHFSI_GetChainNo(CDiagHFSI* pThis)
{
	return 0;
}

US CDiagHFSI_GetLinkNo(CDiagHFSI* pThis)
{
	return 0;
}

UL CDiagHFSI_GetValue(CDiagHFSI* pThis)
{
	return 0;
}

SL CDiagHFSI_GetSignedValue(CDiagHFSI* pThis)
{
	return 0;
}

SC CDiagHFSI_SetValue(CDiagHFSI* pThis, UL value)
{
	return 0;
}

SC CDiagHFSI_Increament(CDiagHFSI* pThis)
{
	return 0;
}

enum ECdiFinDataType CDiagHFSI_GetDataType(CDiagHFSI* pThis)
{
	return eCdiFinDataType_UnsignChar;
}

UL CDiagHFSI_GetMinValue(CDiagHFSI* pThis)
{
	return 0;
}

UL CDiagHFSI_GetMaxValue(CDiagHFSI* pThis)
{
	return 0;
}

void CDiagHFSI_Initialize(CDiagHFSI* pThis)
{
	
}

void CDiagHFSI_Reset(CDiagHFSI* pThis, UC exchangeReason)
{
	
}

UL CDiagHFSI_GetLastCount(CDiagHFSI* pThis, UC lastCount)
{
	return 0;
}

UC CDiagHFSI_GetExchangeReason(CDiagHFSI* pThis, UC lastCount)
{
	return 0;
}

void CDiagHFSI_SetLifeSpec(CDiagHFSI* pThis, UL spec)
{
	
}

UL CDiagHFSI_GetLifeSpec(CDiagHFSI* pThis)
{
	return 0;
}

UC const * CDiagHFSI_GetPartsName(CDiagHFSI* pThis)
{
	UC dummy[1];
	return dummy;
}

UC CDiagHFSI_GetPartsNameSize(CDiagHFSI* pThis)
{
	return 1;
}

