#pragma once

#include "resource.h"
#include "GalaxyIncludes.h"
#include "GXBitmap.h"
#include "Camera.h"
#include "SerialPort.h"
#include "Arduino.h"

class MainDialog : public CDialog
{
private:
	void __UpdateUI();

	Camera *camera;
	Arduino *arduino;
	ICaptureEventHandler *handler;
	std::string savePath;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* dataExchange);
	DECLARE_MESSAGE_MAP()

public:
	MainDialog(CWnd* pParent = NULL);

	enum { IDD = IDD_GXSINGLECAM_DIALOG };

	bool checkSaveBmp;

	afx_msg void OnBnClickedBtnStartDevice();
	afx_msg void OnBnClickedBtnStopDevice();
	afx_msg void OnBnClickedBtnConnectArduino();
	afx_msg void OnBnClickedBtnDisconnectArduino();
	afx_msg void OnClose();
	afx_msg void OnBnClickedChkSave();

	void SavePicture(CImageDataPointer& objImageDataPointer);
	CGXBitmap* bitmap;
};