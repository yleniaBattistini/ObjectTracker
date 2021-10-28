#pragma once

#include "GalaxyIncludes.h"

class Camera
{
private:
	bool connected;
	bool capturing;
	CGXDevicePointer device;
	CGXStreamPointer stream;
	CGXFeatureControlPointer featureControl;

public:
	Camera();

	void Connect();
	void Disconnect();

	void StartAcquisition(ICaptureEventHandler* imageCapturedHandler, void* userData);
	void StopAcquisition();
	void Trigger();

	bool IsConnected();
	bool IsCapturing();

	int GetWidth();
	int GetHeight();
};
