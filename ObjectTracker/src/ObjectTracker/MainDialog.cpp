#include "stdafx.h"
#include "Resource.h"
#include "MainDialog.h"
#include "ImageCapturedHandler.h"
#include "SerialPortsHelper.h"
#include <iostream>
#include<stdlib.h>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MainDialog::MainDialog(CWnd* pParent) : CDialog(MainDialog::IDD, pParent),
	savePath(""),
	handler(new ImageCapturedHandler()),
	bitmap(NULL),
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
	cv::CascadeClassifier faceDetection;
	if (faceDetection.load("C:\opencv\sources\data\haarcascades\haarcascade_frontalface_default.xml")) {
		cout << "\n File is not loaded properly";
		exit(0); // stdlib.h
	}
	char path[100];
	cout << "\n Enter the path to image for face detection : ";
	cin.getline(path, 100);

	cv::Mat img = imread(path, cv::IMREAD_UNCHANGED);

	if (img.empty()) {
		cout << " \n Image is not loaded properly";
	}
	else
	{
		cout << "\n Image is found";
		cout << "\n Processing..";

		vector<cv::Rect> faces;
		faceDetection.detectMultiScale(img, faces); //detecting here

		for (int i = 0; i < faces.size(); i++) {
			cv::Point pt1(faces[i].x, faces[i].y);
			cv::Point pt2((faces[i].x + faces[i].height), (faces[i].y + faces[i].width));
			rectangle(img, pt1, pt2, cv::Scalar(0, 0, 255), 2, 8, 0);
		}

		cv::imwrite("C:\\gs\\output.jpg", img);
		cout << "\n Face detected ok";
	}

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
		bitmap = new CGXBitmap(imageDisplay, camera->GetWidth(), camera->GetHeight());

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

void MainDialog::SavePicture(CImageDataPointer& objImageDataPointer)
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
