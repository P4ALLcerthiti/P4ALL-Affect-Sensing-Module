
// Affect_Sensing_Desctop_App.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CAffect_Sensing_Desctop_AppApp:
// See Affect_Sensing_Desctop_App.cpp for the implementation of this class
//

class CAffect_Sensing_Desctop_AppApp : public CWinApp
{
public:
	CAffect_Sensing_Desctop_AppApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CAffect_Sensing_Desctop_AppApp theApp;