// File Name:	COputInformer.h
// Description:	Header File of Option Unit Informer
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.14
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef COputInformer_h
#define COputInformer_h

////////////////////////////////////////////////////////////
// Include File
////////////////////////////////////////////////////////////

#include "FinisherCommon.h"


////////////////////////////////////////////////////////////
// Function Prototype Definition
////////////////////////////////////////////////////////////

void COputInformer_Constructor();

void COputInformer_MakeInformation();
bool COputInformer_GetOptionDetect(EOptionID optionID);
UC COputInformer_GetOptionType(EOptionID optionID);


#endif // #ifndef COputInformer_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
