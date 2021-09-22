#include "stdafx.h"
#include "GxSingleCam.h"
#include "GxSingleCamDlg.h"
#include "FileVersion.h"
#include "ImageCapturedHandler.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SERIAL_PORT_NAME "COM1"

CGxSingleCamDlg::CGxSingleCamDlg(CWnd* pParent) : CDialog(CGxSingleCamDlg::IDD, pParent),
	isConnected(false),
	savePath(""),
	handler(new ImageCapturedHandler()),
	bitmap(NULL),
	camera(new Camera()),
	serialPort(NULL),
	checkSaveBmp(false)
{
}

BEGIN_MESSAGE_MAP(CGxSingleCamDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_START_DEVICE, &CGxSingleCamDlg::OnBnClickedBtnStartDevice)
	ON_BN_CLICKED(IDC_BTN_STOP_DEVICE, &CGxSingleCamDlg::OnBnClickedBtnStopDevice)
	ON_BN_CLICKED(IDC_BTN_CONNECT_ARDU, &CGxSingleCamDlg::OnBnClickedBtnConnectArduino)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT_ARDU, &CGxSingleCamDlg::OnBnClickedBtnDisconnectArduino)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHK_SAVE, &CGxSingleCamDlg::OnBnClickedChkSave)
END_MESSAGE_MAP()

// CGxSingleCamDlg message handlers
BOOL CGxSingleCamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	char buffer[100];
	serialPort = new SerialPort(SERIAL_PORT_NAME, CBR_9600);

	__UpdateUI();

	serialPort->WriteSerialPort(buffer, 100);
	cout << "Message sent by arduino: [" << buffer << "]" << endl;

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

void CGxSingleCamDlg::DoDataExchange(CDataExchange* dataExchange)
{
	CDialog::DoDataExchange(dataExchange);
	int save;
	DDX_Check(dataExchange, IDC_CHK_SAVE, save);
	checkSaveBmp = save;
}

void CGxSingleCamDlg::OnBnClickedBtnStartDevice()
{
	try
	{
		camera->Connect();

		CWnd* imageDisplay = GetDlgItem(IDC_SHOW_PICTURE_STATIC);
		bitmap = new CGXBitmap(imageDisplay, camera->GetWidth(), camera->GetHeight());

		camera->StartAcquisition(handler, this);

		__UpdateUI();
	}
	catch (exception& e)
	{
		MessageBox(e.what());
	}
}

void CGxSingleCamDlg::__UpdateUI()
{
	GetDlgItem(IDC_BTN_START_DEVICE)->EnableWindow(!camera->IsConnected());
	GetDlgItem(IDC_BTN_STOP_DEVICE)->EnableWindow(camera->IsConnected());
	GetDlgItem(IDC_BTN_CONNECT_ARDU)->EnableWindow(camera->IsConnected() && !isConnected);
	GetDlgItem(IDC_BTN_DISCONNECT_ARDU)->EnableWindow(camera->IsConnected() && isConnected);
}

void CGxSingleCamDlg::OnBnClickedBtnStopDevice()
{
	SetFocus();
	try
	{
		camera->StopAcquisition();
		delete bitmap;
		camera->Disconnect();

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
	char buffer[100];
	serialPort = new SerialPort(SERIAL_PORT_NAME, CBR_9600);

	__UpdateUI();

	serialPort->WriteSerialPort(buffer, 100);
	cout << "Message sent by arduino: [" << buffer << "]" << endl;
}

void CGxSingleCamDlg::OnBnClickedBtnDisconnectArduino()
{
	delete serialPort;
	serialPort = NULL;
}

void CGxSingleCamDlg::OnClose()
{
	try
	{
		camera->StopAcquisition();
		delete bitmap;
		camera->Disconnect();

		delete camera;
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


void CGxSingleCamDlg::OnBnClickedChkSave()
{
	UpdateData(TRUE);
}
