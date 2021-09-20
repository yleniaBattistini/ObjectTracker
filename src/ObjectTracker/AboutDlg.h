#pragma once

#include "GalaxyIncludes.h"
#include "GXBitmap.h"
#include "GxSingleCam.h"

using namespace std;

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
public:
	CString m_strStaticProductVersion;
	CString m_strStaticLegalCopyright;
};
