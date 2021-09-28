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

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	MainDialog mainDialog;
	m_pMainWnd = &mainDialog;
	mainDialog.DoModal();
	return FALSE;
}
