#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"

class ObjectTrackerApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};

extern ObjectTrackerApp theApp;