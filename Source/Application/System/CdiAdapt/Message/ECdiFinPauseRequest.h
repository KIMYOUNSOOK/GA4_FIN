/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinPauseRequest.h
//  Implementation of the Class ECdiFinPauseRequest
//  Created on:      2012/11/22 16:12:49
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_D9BBD7BA_E625_4043_8B8D_7781AA050C77__INCLUDED_)
#define EA_D9BBD7BA_E625_4043_8B8D_7781AA050C77__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Finisher Device Function::ECdiFinPauseRequest
// Pause Request
////////////////////////////////////////////////////////////////////////
enum ECdiFinPauseRequest
{
	// Pause To Unload Envelope Folder Tray
	eCdiFinPauseReq_UnloadEnvTray = 3,
	// Pause To Unlock Primary HCS Stacker
	eCdiFinPauseReq_UnlockPriHcsStk = 4,
	// Pause To Unlock Secondary HCS Stacker
	eCdiFinPauseReq_UnlockSecHcsStk = 5,
	// Perfect Bind Manual Input Mode
	eCdiFinPauseReq_PBManualInput = 6
};
#endif // !defined(EA_D9BBD7BA_E625_4043_8B8D_7781AA050C77__INCLUDED_)
