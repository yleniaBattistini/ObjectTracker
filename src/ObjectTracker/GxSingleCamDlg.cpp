#include "stdafx.h"
#include "GxSingleCam.h"
#include "GxSingleCamDlg.h"
#include "FileVersion.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CGxSingleCamDlg::CGxSingleCamDlg(CWnd* pParent)
	: CDialog(CGxSingleCamDlg::IDD, pParent)
	, m_bIsStart(false)
	, m_bIsConnected(false)
	, m_strSavePath("")
	, m_pWnd(NULL)
    , m_pSampleCaptureEventHandle(NULL)
	, m_pBitmap(NULL)
	, m_bCheckSaveBmp(FALSE)
{
}

BEGIN_MESSAGE_MAP(CGxSingleCamDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START_DEVICE, &CGxSingleCamDlg::OnBnClickedBtnStartDevice)
	ON_BN_CLICKED(IDC_BTN_STOP_DEVICE, &CGxSingleCamDlg::OnBnClickedBtnStopDevice)
	ON_BN_CLICKED(IDC_BTN_CONNECT_ARDU, &CGxSingleCamDlg::OnBnClickedBtnConnectArduino)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT_ARDU, &CGxSingleCamDlg::OnBnClickedBtnDisconnectArduino)
	ON_WM_CLOSE()
	ON_WM_TIMER()

	ON_BN_CLICKED(1, &CGxSingleCamDlg::OnBnClicked1)
END_MESSAGE_MAP()

// CGxSingleCamDlg message handlers
BOOL CGxSingleCamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	try
	{
		// Before using any GxIAPICPP methods, the GxIAPICPP must be initialized. 
		IGXFactory::GetInstance().Init();

		//Initialize the window pointer with the picture widget
		m_pWnd = GetDlgItem(IDC_SHOW_PICTURE_STATIC);
		m_pSampleCaptureEventHandle = new CSampleCaptureEventHandler();

		//Get the current path of app
		char    strFileName[MAX_PATH] = {0};
		string  strSavePath           = "";
		size_t  nPos                  = 0;

		GetModuleFileName(NULL,(LPCH)strFileName,MAX_PATH);
		strSavePath = strFileName;
		nPos = strSavePath.find_last_of('\\');
		m_strSavePath = strSavePath.substr(0,nPos);
		m_strSavePath = m_strSavePath +"\\GxSingleCamImages";

		__UpdateUI();
	}
	catch (CGalaxyException& e)
	{
		if (m_pSampleCaptureEventHandle != NULL)
		{
			delete m_pSampleCaptureEventHandle;
			m_pSampleCaptureEventHandle = NULL;
		}

		MessageBox(e.what());
		return FALSE;
		
	}
	catch (std::exception& e)
	{
		if (m_pSampleCaptureEventHandle != NULL)
		{
			delete m_pSampleCaptureEventHandle;
			m_pSampleCaptureEventHandle = NULL;
		}

		MessageBox(e.what());
		return FALSE;

	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGxSingleCamDlg::OnBnClickedBtnStartDevice()
{
	bool bIsDeviceOpen = false;         ///< The flag indicates whether the device has been opened.
	bool bIsStreamOpen = false;         ///< The flag indicates whether the stream has been opened.

	try
	{
		//Enumerate all camera devices
		GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
		IGXFactory::GetInstance().UpdateDeviceList(1000,vectorDeviceInfo);

	    //Check it has found device or not.If not, will be hited with a dialog
		if (vectorDeviceInfo.size() <= 0)
		{
			MessageBox("No device found!");
			return;
		}

		//Open device
		m_objDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(vectorDeviceInfo[0].GetSN(),GX_ACCESS_EXCLUSIVE);
		bIsDeviceOpen = true;
		m_objFeatureControlPtr = m_objDevicePtr->GetRemoteFeatureControl();

		//Check whether the pointer of drawing image is null or not
		if (m_pBitmap != NULL)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}

		// Create memory for the object of drawing image
		m_pBitmap = new CGXBitmap(m_objDevicePtr,m_pWnd);

		//Check the device has stream or not.If more than zero, will be opend the stream.
		int nStreamCount = m_objDevicePtr->GetStreamCount();

		if (nStreamCount > 0)
		{
			m_objStreamPtr = m_objDevicePtr->OpenStream(0);
			bIsStreamOpen = true;
		}
		else
		{
			throw exception("The device has been opend doesn't has stream!");
		}

		__InitParam();

		try
		{
			//Register the CaptureCallback function
			m_objStreamPtr->RegisterCaptureCallback(m_pSampleCaptureEventHandle, this);

			//Start stream capturing
			m_objStreamPtr->StartGrab();

			//Send AcquisitionStart command 
			m_objFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();
			m_bIsStart = true;

			//Update UI
			__UpdateUI();
		}
		catch (CGalaxyException& e)
		{
			MessageBox(e.what());
			return;
		}
		catch (std::exception& e)
		{
			MessageBox(e.what());
			return;
		}
	}
	catch (CGalaxyException& e)
	{
		//Check whether the device stream has been opened or not
		if (bIsStreamOpen)
		{
			m_objStreamPtr->Close();
		}

		//Check whether the device has been opened or not
		if (bIsDeviceOpen)
		{
			m_objDevicePtr->Close();
		}

		if (m_pBitmap != NULL)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}

		MessageBox(e.what());
		return;
	}
	catch (std::exception& e)
	{
		//Check whether the device stream has been opened or not
		if (bIsStreamOpen)
		{
			m_objStreamPtr->Close();
		}

		//Check whether the device has been opened or not
		if (bIsDeviceOpen)
		{
			m_objDevicePtr->Close();
		}

		if (m_pBitmap != NULL)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}

		MessageBox(e.what());
		return;
	}
}

void CGxSingleCamDlg::__InitParam()
{
	//Set the continuous frame acquisition mode
	m_objFeatureControlPtr->GetEnumFeature("AcquisitionMode")->SetValue("Continuous");
}

void CGxSingleCamDlg::__UpdateUI()
{
	GetDlgItem(IDC_BTN_START_DEVICE)->EnableWindow(!m_bIsStart);
	GetDlgItem(IDC_BTN_STOP_DEVICE)->EnableWindow(m_bIsStart);
	GetDlgItem(IDC_BTN_CONNECT_ARDU)->EnableWindow(m_bIsStart && !m_bIsConnected);
	GetDlgItem(IDC_BTN_DISCONNECT_ARDU)->EnableWindow(m_bIsStart && m_bIsConnected);

}

void CGxSingleCamDlg::OnBnClickedBtnStopDevice()
{
	//lose focus
	SetFocus();
	try
	{
		//Send AcquisitionStop command 
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

		//Stop stream capturing
		m_objStreamPtr->StopGrab();

		//Unregister the CaptureCallback function
		m_objStreamPtr->UnregisterCaptureCallback();

		try
		{
			//If the device is acquiring image then stop it.
			if (m_bIsStart)
			{
				//Send AcquisitionStop command 
				m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

				//Stop stream capturing
				m_objStreamPtr->StopGrab();

				//Unregister the CaptureCallback function
				m_objStreamPtr->UnregisterCaptureCallback();
			}
		}
		catch (CGalaxyException)
		{
			//do noting
		}

		try
		{
			//Close stream
			m_objStreamPtr->Close();

		}
		catch (CGalaxyException)
		{
			//do noting
		}
		try
		{
			//Close device
			m_objDevicePtr->Close();
		}
		catch (CGalaxyException)
		{
			//do noting
		}

		m_bIsStart = false;

		//Update UI
		__UpdateUI();
		if (m_pBitmap != NULL)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}
	}
	catch (CGalaxyException& e)
	{
		MessageBox(e.what());
		return;
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
		//If the device is acquiring image then stop it.
		if (m_bIsStart)
		{
			//Send AcquisitionStop command 
			m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

			//Stop stream capturing
			m_objStreamPtr->StopGrab();

			//Unregister the CaptureCallback function
			m_objStreamPtr->UnregisterCaptureCallback();

			//Close stream
			m_objStreamPtr->Close();

			//Close device
			m_objDevicePtr->Close();

			m_bIsStart = false;
		}
	}
	catch (CGalaxyException)
	{
		//do noting
	}
	catch (std::exception)
	{
		//do noting
	}


	try
	{
		//Release the resoources
		IGXFactory::GetInstance().Uninit();
	}
	catch (CGalaxyException)
	{
		//do noting
	}
	catch (std::exception)
	{
		//do noting
	}

	if (m_pSampleCaptureEventHandle != NULL)
	{
		delete m_pSampleCaptureEventHandle;
		m_pSampleCaptureEventHandle = NULL;
	}

	if (m_pBitmap != NULL)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
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
		strFilePath = m_strSavePath.c_str();

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
		m_pBitmap->SaveBmp(objImageDataPointer,strFileName.GetBuffer(0));
	}
	catch (std::exception)
	{
		//Due to saving image is completed in the capture callback function of the acquisition thread, if it throw
		//exception,the acquisition thread will be stoped. So in order to avoid stoping the thread, in the process of saving
		//image doesn't throw exception.
		return;
		
	}

}


void CGxSingleCamDlg::OnBnClicked1()
{
	// TODO: aggiungere qui il codice del gestore di notifiche del controllo
}
