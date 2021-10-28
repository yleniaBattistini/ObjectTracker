#include "stdafx.h"
#include "Resource.h"
#include "MainDialog.h"
#include "ImageCapturedHandler.h"
#include "SerialPortsHelper.h"
#include <iostream>
#include<stdlib.h>

using namespace std;
using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MainDialog::MainDialog(CWnd* pParent) : CDialog(MainDialog::IDD, pParent),
	savePath(""),
	handler(new ImageCapturedHandler()),
	acquiredImageViewer(NULL),
	processedImageViewer(NULL),
	camera(new Camera()),
	arduino(NULL),
	checkSaveBmp(false)
{
}

BEGIN_MESSAGE_MAP(MainDialog, CDialog)
	ON_BN_CLICKED(IDC_BTN_START_DEVICE, &MainDialog::OnBnClickedBtnStartDevice)
	ON_BN_CLICKED(IDC_BTN_STOP_DEVICE, &MainDialog::OnBnClickedBtnStopDevice)
	ON_BN_CLICKED(IDC_BTN_CONNECT_ARDU, &MainDialog::OnBnClickedBtnConnectArduino)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT_ARDU, &MainDialog::OnBnClickedBtnDisconnectArduino)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHK_SAVE, &MainDialog::OnBnClickedChkSave)
	ON_CBN_SELCHANGE(IDC_ARDUINOPORTS, &MainDialog::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()

BOOL MainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	/*CComboBox* arduinoPorts = (CComboBox*) GetDlgItem(IDC_ARDUINOPORTS);
	arduinoPorts->ResetContent();
	vector<string> ports = SerialPortsHelper::ListAvailablePorts();
	for (int i = 0; i < ports.size(); i++) {
		arduinoPorts->SetItemData(arduinoPorts->AddString(ports[i].c_str()), i);
	}
	if (ports.size() > 0) {
		arduinoPorts->SetCurSel(0);
	}*/

	try
	{
		IGXFactory::GetInstance().Init();

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

	return TRUE;
}

void MainDialog::DoDataExchange(CDataExchange* dataExchange)
{
	CDialog::DoDataExchange(dataExchange);
	int save;
	DDX_Check(dataExchange, IDC_CHK_SAVE, save);
	checkSaveBmp = save;
}

void MainDialog::OnBnClickedBtnStartDevice()
{
	try
	{
		camera->Connect();

		CWnd* imageDisplay = GetDlgItem(IDC_SHOW_PICTURE_STATIC);
		CWnd* imageOpencvDisplay = GetDlgItem(IDC_SHOW_OPENCV_PICTURE);
		acquiredImageViewer = new ImageViewer(imageDisplay);
		processedImageViewer = new ImageViewer(imageOpencvDisplay);

		camera->StartAcquisition(handler, this);

		__UpdateUI();
	}
	catch (exception& e)
	{
		MessageBox(e.what());
	}
}

void MainDialog::__UpdateUI()
{
	GetDlgItem(IDC_BTN_START_DEVICE)->EnableWindow(!camera->IsConnected());
	GetDlgItem(IDC_BTN_STOP_DEVICE)->EnableWindow(camera->IsConnected());
	GetDlgItem(IDC_BTN_CONNECT_ARDU)->EnableWindow(camera->IsConnected());
	GetDlgItem(IDC_BTN_DISCONNECT_ARDU)->EnableWindow(camera->IsConnected());
}

void MainDialog::OnBnClickedBtnStopDevice()
{
	SetFocus();
	try
	{
		camera->StopAcquisition();
		delete acquiredImageViewer;
		delete processedImageViewer;
		camera->Disconnect();

		__UpdateUI();
	}
	catch (std::exception& e)
	{
		MessageBox(e.what());
		return;
	}
}

void MainDialog::OnBnClickedBtnConnectArduino()
{
	
}

void MainDialog::OnBnClickedBtnDisconnectArduino()
{
	
}

void MainDialog::OnClose()
{
	try
	{
		if (camera->IsCapturing())
		{
			camera->StopAcquisition();
		}
		if (camera->IsConnected())
		{
			camera->Disconnect();
		}

		IGXFactory::GetInstance().Uninit();
	}
	catch (std::exception)
	{
	}

	CDialog::OnClose();
}

void MainDialog::SavePicture(cv::Mat& image)
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

		imwrite(strFileName.GetBuffer(0), image);
	}
	catch (std::exception)
	{
		//Due to saving image is completed in the capture callback function of the acquisition thread, if it throw
		//exception,the acquisition thread will be stoped. So in order to avoid stoping the thread, in the process of saving
		//image doesn't throw exception.
		return;
	}
}

void MainDialog::ShowAcquiredImage(BYTE* image, int width, int height)
{
	acquiredImageViewer->Show(image, width, height);
}

void MainDialog::ShowProcessedImage(BYTE* image, int width, int height)
{
	processedImageViewer->Show(image, width, height);
}

void MainDialog::OnBnClickedChkSave()
{
	UpdateData(TRUE);
}


void MainDialog::OnCbnSelchangeCombo1()
{
	CComboBox* arduinoPorts = (CComboBox*)GetDlgItem(IDC_ARDUINOPORTS); 
	int selected = arduinoPorts->GetCurSel();
	CString port;
	arduinoPorts->GetLBText(selected, port);
	serialPortName = port;
}
