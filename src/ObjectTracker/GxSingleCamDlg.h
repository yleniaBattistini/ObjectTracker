#pragma once

#include "GalaxyIncludes.h"
#include "GXBitmap.h"
#include "Camera.h"

class CGxSingleCamDlg : public CDialog
{
private:
	void __UpdateUI();

	Camera camera;
	ICaptureEventHandler* handler;
	bool isConnected;
	std::string savePath;

protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	CGxSingleCamDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_GXSINGLECAM_DIALOG };

	bool checkSaveBmp;

	afx_msg void OnBnClickedBtnStartDevice();
	afx_msg void OnBnClickedBtnStopDevice();
	afx_msg void OnBnClickedBtnConnectArduino();
	afx_msg void OnBnClickedBtnDisconnectArduino();
	afx_msg void OnBnClickedCheckSaveBmp();
	afx_msg void OnClose();

	void SavePicture(CImageDataPointer& objImageDataPointer);
	CGXBitmap* bitmap;
};
