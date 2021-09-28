#pragma once

#include "GalaxyIncludes.h"

class ImageCapturedHandler : public ICaptureEventHandler
{
public:
	void DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam);
};