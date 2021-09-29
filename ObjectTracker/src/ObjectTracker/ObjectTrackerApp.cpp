// GxSingleCam.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ObjectTrackerApp.h"
#include "MainDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ObjectTrackerApp theApp;

BOOL ObjectTrackerApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX initCtrls = { sizeof(initCtrls), ICC_WIN95_CLASSES };
	InitCommonControlsEx(&initCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	MainDialog mainDialog;
	m_pMainWnd = &mainDialog;
	mainDialog.DoModal();
	return TRUE;
}
