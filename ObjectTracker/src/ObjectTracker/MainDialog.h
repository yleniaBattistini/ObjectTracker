#pragma once

#include "resource.h"
#include "GalaxyIncludes.h"
#include "ImageViewer.h"
#include "Camera.h"
#include "SerialPort.h"
#include "Arduino.h"
#include <opencv2\opencv.hpp>

using namespace std;

class MainDialog : public CDialog
{
private:
	void __UpdateUI();

	Camera *camera;
	Arduino *arduino;
	ICaptureEventHandler *handler;
	string savePath;
	string serialPortName;// "COM5" //"COM3"
	ImageViewer* acquiredImageViewer;
	ImageViewer* processedImageViewer;
	vector<string> ports;
	bool manually;
	char keyword_trigger;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* dataExchange);
	DECLARE_MESSAGE_MAP()

public:
	MainDialog(CWnd* pParent = NULL);

	enum { IDD = IDD_GXSINGLECAM_DIALOG };

	bool checkSaveBmp;

	void SavePicture(cv::Mat& image);
	void ShowAcquiredImage(BYTE* image, int width, int height);
	void ShowProcessedImage(BYTE* image, int width, int height);
	void OnImageProcessingCompleted();
	void ObjectRegognized(vector<Rect>& faces);

	afx_msg void OnBnClickedBtnStartDevice();
	afx_msg void OnBnClickedBtnStopDevice();
	afx_msg void OnBnClickedBtnConnectArduino();
	afx_msg void OnBnClickedBtnDisconnectArduino();
	afx_msg void OnClose();
	afx_msg void OnBnClickedChkSave();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedChkMovement();
	afx_msg void OnStnClickedLblRecognition();
};
