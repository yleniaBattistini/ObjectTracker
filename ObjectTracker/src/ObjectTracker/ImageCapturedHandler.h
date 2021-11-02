#pragma once

#include "GalaxyIncludes.h"
#include"ImageConversion.h"
#include"FaceDetection.h"

class ImageCapturedHandler : public ICaptureEventHandler
{
private:
	int frame = 0;
	FaceDetection faceDetection = FaceDetection();
	vector<Rect> faces;

	/// Get the best Bit by GX_PIXEL_FORMAT_ENTRY
	GX_VALID_BIT_LIST GetBestValueBit(GX_PIXEL_FORMAT_ENTRY emPixelFormatEntry);
public:
	void DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam);

};