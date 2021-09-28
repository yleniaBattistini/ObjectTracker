#pragma once

#include "GalaxyIncludes.h"
#include "GxSingleCamDlg.h"

class ImageCapturedHandler : public ICaptureEventHandler
{
public:
	void DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam);
};