#include "stdafx.h"
#include "Camera.h"

Camera::Camera() : connected(false), capturing(false)
{
}

void Camera::Connect()
{
	if (connected)
	{
		throw std::runtime_error("Device is already connected");
	}

	bool isDeviceOpen = false;
	bool isStreamOpen = false;
	try
	{
		GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
		IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);

		if (vectorDeviceInfo.empty())
		{
			throw std::runtime_error("No device found");
		}

		device = IGXFactory::GetInstance().OpenDeviceBySN(vectorDeviceInfo[0].GetSN(), GX_ACCESS_EXCLUSIVE);
		isDeviceOpen = true;
		featureControl = device->GetRemoteFeatureControl();

		const int nStreamCount = device->GetStreamCount();

		if (nStreamCount > 0)
		{
			stream = device->OpenStream(0);
			isStreamOpen = true;
		}
		else
		{
			throw std::runtime_error("The device doesn't have any streams!");
		}

		featureControl->GetEnumFeature("AcquisitionMode")->SetValue("Continuous");
		featureControl->GetIntFeature("Width")->SetValue(1920);
		featureControl->GetIntFeature("Height")->SetValue(1080);
		featureControl->GetEnumFeature("TriggerMode")->SetValue("On");

		connected = true;
	}
	catch (std::exception& e)
	{
		if (isStreamOpen)
		{
			stream->Close();
		}
		if (isDeviceOpen)
		{
			device->Close();
		}
		throw;
	}
}

void Camera::Disconnect()
{
	if (!connected)
	{
		throw std::runtime_error("Device is not connected yet");
	}

	if (capturing)
	{
		throw std::runtime_error("Device is still capturing");
	}

	stream->Close();
	device->Close();

	connected = false;
}

void Camera::StartAcquisition(ICaptureEventHandler* handler, void* userData)
{
	if (!connected)
	{
		throw std::runtime_error("Device is not connected yet");
	}

	if (capturing)
	{
		throw std::runtime_error("Device is already capturing");
	}

	stream->RegisterCaptureCallback(handler, userData);
	stream->StartGrab();

	featureControl->GetCommandFeature("AcquisitionStart")->Execute();
	capturing = true;

	Trigger();
}

void Camera::StopAcquisition()
{
	if (!capturing)
	{
		throw std::runtime_error("Device is not capturing yet");
	}

	featureControl->GetCommandFeature("AcquisitionStop")->Execute();

	stream->StopGrab();
	stream->UnregisterCaptureCallback();

	capturing = false;
}

void Camera::Trigger()
{
	featureControl->GetCommandFeature("TriggerSoftware")->Execute();
}

bool Camera::IsConnected()
{
	return connected;
}

bool Camera::IsCapturing()
{
	return capturing;
}

int Camera::GetWidth()
{
	return featureControl->GetIntFeature("Width")->GetValue();
}

int Camera::GetHeight()
{
	return featureControl->GetIntFeature("Height")->GetValue();
}