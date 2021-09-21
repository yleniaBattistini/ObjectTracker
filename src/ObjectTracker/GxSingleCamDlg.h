// GxSingleCamDlg.h : header file
//
#pragma once

#include "GalaxyIncludes.h"
#include "GXBitmap.h"


// CGxSingleCamDlg Dialog
class CGxSingleCamDlg : public CDialog
{
	//---------------------------------------------------------------------------------
	/**
	\brief   inherit the acquisition event handling class
	*/
	//----------------------------------------------------------------------------------
	class CSampleCaptureEventHandler :public ICaptureEventHandler
	{
		//---------------------------------------------------------------------------------
		/**
		\brief   The capture callback function
		\param   objImageDataPointer      image process parameter
		\param   pFrame                   custom defined parameter

		\return  void
		*/
		//----------------------------------------------------------------------------------
		void DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam)
		{
			try
			{
				CGxSingleCamDlg* pSingleCamDlg = (CGxSingleCamDlg*)pUserParam;

				//Display image
				pSingleCamDlg->m_pBitmap->Show(objImageDataPointer);

				//Check whether is need to save image or not.
				if (pSingleCamDlg->m_bCheckSaveBmp == TRUE)
				{
					pSingleCamDlg->SavePicture(objImageDataPointer);
				}
			}
			catch (CGalaxyException)
			{
				//do nothing
			}
			catch (std::exception)
			{
				//do nothing
			}
		}
	};
public:
	CGxSingleCamDlg(CWnd* pParent = NULL);	// standard Construction
	enum { IDD = IDD_GXSINGLECAM_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnStartDevice();
	afx_msg void OnBnClickedBtnStopDevice();
	afx_msg void OnBnClickedBtnConnectArduino();
	afx_msg void OnBnClickedBtnDisconnectArduino();
	afx_msg void OnBnClickedCheckSaveBmp();

public:
	afx_msg void OnClose();
	BOOL   m_bCheckSaveBmp;

public:
	void SavePicture(CImageDataPointer& objImageDataPointer);
	CGXBitmap*  m_pBitmap;

private:
	void __InitParam();
	void __UpdateUI();

	CGXDevicePointer                  m_objDevicePtr;             ///< The device handle
	CGXStreamPointer                  m_objStreamPtr;             ///< The stream handle
	CGXFeatureControlPointer          m_objFeatureControlPtr;     ///< The object for device feature control
	CWnd*                             m_pWnd;                     ///< The pointer of Window
	CSampleCaptureEventHandler*       m_pSampleCaptureEventHandle;///< The pointer of capture callback function

	bool                              m_bIsStart;                  ///< The flag indicates whether the device has been opened.
	bool                              m_bIsConnected;                  ///< The flag indicates whether the device has been started acquisition.
	std::string                       m_strSavePath;              ///< Record the path for saving images
public:
	afx_msg void OnBnClicked1();
};
