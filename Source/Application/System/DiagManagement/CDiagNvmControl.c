#include "CDiagNvmControl.h"


void CDiagNvmControl_Constructor()
{
	//@@@
}

void CDiagNvmControl_Initialize()
{
	//@@@
}

void CDiagNvmControl_InitializeAll()
{
	//@@@
}

void CDiagNvmControl_AutoInitialize(US major, US minor)
{
	//@@@
}

void CDiagNvmControl_ReadArea(US offset, UC* data, UC size)
{
	//@@@
}

void CDiagNvmControl_WriteArea(US offset, UC* data, UC size)
{
	//@@@
}

//CDiagNVM* CDiagNvmControl_Get(ENvmID id)
//{
//	static CDiagNVM nvm;
//
//	return &nvm;		//@@@
//}

CDiagNVM* CDiagNvmControl_GetDiag(US chain, US link)
{
	static CDiagNVM nvm;

	return &nvm;		//@@@
}

US CDiagNvmControl_GetAreaSize()
{
	return 0;//@@@
}


