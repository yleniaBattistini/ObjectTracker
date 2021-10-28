#include "stdafx.h"
#include "ImageCapturedHandler.h"
#include "MainDialog.h"

void ImageCapturedHandler::DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam)
{
	try
	{
		frame++;
		MainDialog* mainDialog = (MainDialog*)pUserParam;
		GX_VALID_BIT_LIST emValidBits = GetBestValueBit(objImageDataPointer->GetPixelFormat());
		BYTE* pBuffer = (BYTE*)objImageDataPointer->ConvertToRGB24(emValidBits, GX_RAW2RGB_NEIGHBOUR, true);
		int width = objImageDataPointer->GetWidth();
		int height = objImageDataPointer->GetHeight();

		BYTE* newImage = new BYTE[width * height * 3];

		Mat imageConverted;
		ImageConversion::ToOpenCvImage(pBuffer, width, height, imageConverted);
		ImageConversion::ToGalaxyImage(imageConverted, width, height, newImage);

		mainDialog->ShowAcquiredImage(pBuffer, width, height);
		mainDialog->ShowProcessedImage(newImage, width, height);

		if (mainDialog->checkSaveBmp == TRUE && frame % 15 == 0)
		{
			mainDialog->SavePicture(imageConverted);
		}

		delete[] newImage;

		mainDialog->OnImageProcessingCompleted();
	}
	catch (std::exception)
	{
		//do nothing
	}
}


GX_VALID_BIT_LIST ImageCapturedHandler::GetBestValueBit(GX_PIXEL_FORMAT_ENTRY emPixelFormatEntry)
{
	GX_VALID_BIT_LIST emValidBits = GX_BIT_0_7;
	switch (emPixelFormatEntry)
	{
	case GX_PIXEL_FORMAT_MONO8:
	case GX_PIXEL_FORMAT_BAYER_GR8:
	case GX_PIXEL_FORMAT_BAYER_RG8:
	case GX_PIXEL_FORMAT_BAYER_GB8:
	case GX_PIXEL_FORMAT_BAYER_BG8:
	{
		emValidBits = GX_BIT_0_7;
		break;
	}
	case GX_PIXEL_FORMAT_MONO10:
	case GX_PIXEL_FORMAT_BAYER_GR10:
	case GX_PIXEL_FORMAT_BAYER_RG10:
	case GX_PIXEL_FORMAT_BAYER_GB10:
	case GX_PIXEL_FORMAT_BAYER_BG10:
	{
		emValidBits = GX_BIT_2_9;
		break;
	}
	case GX_PIXEL_FORMAT_MONO12:
	case GX_PIXEL_FORMAT_BAYER_GR12:
	case GX_PIXEL_FORMAT_BAYER_RG12:
	case GX_PIXEL_FORMAT_BAYER_GB12:
	case GX_PIXEL_FORMAT_BAYER_BG12:
	{
		emValidBits = GX_BIT_4_11;
		break;
	}
	case GX_PIXEL_FORMAT_MONO14:
	{
		//There is no such data format 
		break;
	}
	case GX_PIXEL_FORMAT_MONO16:
	case GX_PIXEL_FORMAT_BAYER_GR16:
	case GX_PIXEL_FORMAT_BAYER_RG16:
	case GX_PIXEL_FORMAT_BAYER_GB16:
	case GX_PIXEL_FORMAT_BAYER_BG16:
	{
		//There is no such data format 
		break;
	}
	default:
		break;
	}
	return emValidBits;
}