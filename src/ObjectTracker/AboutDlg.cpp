#include "stdafx.h"
#include "AboutDlg.h"

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_strStaticProductVersion(_T(""))
, m_strStaticLegalCopyright(_T(""))
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_PRODUCTVERSION, m_strStaticProductVersion);
	DDX_Text(pDX, IDC_STATIC_LEGALCOPYRIGHT, m_strStaticLegalCopyright);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()