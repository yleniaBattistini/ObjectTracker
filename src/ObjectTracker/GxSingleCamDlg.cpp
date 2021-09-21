#include "stdafx.h"
#include "GxSingleCam.h"
#include "GxSingleCamDlg.h"
#include "FileVersion.h"
#include "ImageCapturedHandler.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CGxSingleCamDlg::CGxSingleCamDlg(CWnd* pParent)
	: CDialog(CGxSingleCamDlg::IDD, pParent)
	, isConnected(false)
	, savePath("")
    , handler(new ImageCapturedHandler())
	, bitmap(NULL)
	, checkSaveBmp(false)
{
}

BEGIN_MESSAGE_MAP(CGxSingleCamDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_START_DEVICE, &CGxSingleCamDlg::OnBnClickedBtnStartDevice)
	ON_BN_CLICKED(IDC_BTN_STOP_DEVICE, &CGxSingleCamDlg::OnBnClickedBtnStopDevice)
	ON_BN_CLICKED(IDC_BTN_CONNECT_ARDU, &CGxSingleCamDlg::OnBnClickedBtnConnectArduino)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT_ARDU, &CGxSingleCamDlg::OnBnClickedBtnDisconnectArduino)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CGxSingleCamDlg message handlers
BOOL CGxSingleCamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	try
	{
		// Before using any GxIAPICPP methods, the GxIAPICPP must be initialized. 
		IGXFactory::GetInstance().Init();

		//Get the current path of app
		char strFileName[MAX_PATH] = {0};
		string strSavePath = "";
		size_t nPos = 0;

		GetModuleFileName(NULL,(LPCH)strFileName,MAX_PATH);
		strSavePath = strFileName;
		nPos = strSavePath.find_last_of('\\');
		savePath = strSavePath.substr(0,nPos);
		savePath = savePath +"\\ObjectTrackerImages";

		__UpdateUI();
	}
	catch (std::exception& e)
	{
		MessageBox(e.what());
		return FALSE;
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGxSingleCamDlg::OnBnClickedBtnStartDevice()
{
	try
	{
		camera.Connect();

		CWnd* imageDisplay = GetDlgItem(IDC_SHOW_PICTURE_STATIC);
		bitmap = new CGXBitmap(imageDisplay, camera.GetWidth(), camera.GetHeight());

		camera.StartAcquisition(handler, this);

		__UpdateUI();
	}
	catch (exception& e)
	{
		MessageBox(e.what());
	}
}

void CGxSingleCamDlg::__UpdateUI()
{
	GetDlgItem(IDC_BTN_START_DEVICE)->EnableWindow(!camera.IsConnected());
	GetDlgItem(IDC_BTN_STOP_DEVICE)->EnableWindow(camera.IsConnected());
	GetDlgItem(IDC_BTN_CONNECT_ARDU)->EnableWindow(camera.IsConnected() && !isConnected);
	GetDlgItem(IDC_BTN_DISCONNECT_ARDU)->EnableWindow(camera.IsConnected() && isConnected);
}

void CGxSingleCamDlg::OnBnClickedBtnStopDevice()
{
	SetFocus();
	try
	{
		camera.StopAcquisition();
		delete bitmap;
		camera.Disconnect();

		__UpdateUI();
	}
	catch (std::exception& e)
	{
		MessageBox(e.what());
		return;
	}
}

void CGxSingleCamDlg::OnBnClickedBtnConnectArduino()
{
	
}

void CGxSingleCamDlg::OnBnClickedBtnDisconnectArduino()
{
	
}

void CGxSingleCamDlg::OnBnClickedCheckSaveBmp()
{
	UpdateData(TRUE);
}

void CGxSingleCamDlg::OnClose()
{
	try
	{
		camera.StopAcquisition();
		delete bitmap;
		camera.Disconnect();
		IGXFactory::GetInstance().Uninit();
	}
	catch (std::exception)
	{
	}

	CDialog::OnClose();
}

void CGxSingleCamDlg::SavePicture(CImageDataPointer& objImageDataPointer)
{
	try
	{
		SYSTEMTIME   sysTime;                   ///< system time
		CString      strFileName = "";          ///< the name of path to saving image
		CString      strFilePath = "";          ///< the file path
		strFilePath = savePath.c_str();

		//Create the folder to saving images.
		BOOL bRet = CreateDirectory(strFilePath,NULL);

		//Get the current system time as the default image name will be saved.
		GetLocalTime(&sysTime);
		strFileName.Format("%s\\%d_%d_%d_%d_%d_%d_%d.bmp",strFilePath,  
			                                              sysTime.wYear,
			                                              sysTime.wMonth,
			                                              sysTime.wDay,
			                                              sysTime.wHour,
			                                              sysTime.wMinute,
			                                              sysTime.wSecond,
			                                              sysTime.wMilliseconds);

		//Save image as BMP
		bitmap->SaveBmp(objImageDataPointer,strFileName.GetBuffer(0));
	}
	catch (std::exception)
	{
		//Due to saving image is completed in the capture callback function of the acquisition thread, if it throw
		//exception,the acquisition thread will be stoped. So in order to avoid stoping the thread, in the process of saving
		//image doesn't throw exception.
		return;
	}
}
