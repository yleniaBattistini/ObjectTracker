#include "stdafx.h"
#include "ImageCapturedHandler.h"
#include "MainDialog.h"

int frame = 0;
void ImageCapturedHandler::DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam)
{

	try
	{
		frame++;
		MainDialog* pSingleCamDlg = (MainDialog*)pUserParam;
		GX_VALID_BIT_LIST emValidBits = GetBestValueBit(objImageDataPointer->GetPixelFormat());
		BYTE* pBuffer = (BYTE*)objImageDataPointer->ConvertToRGB24(emValidBits, GX_RAW2RGB_NEIGHBOUR, true);

		Mat imageConverted;
		BYTE* newImage;
		int imageWidth = objImageDataPointer->GetWidth();
		int imageHeight = objImageDataPointer->GetHeight();

		ImageConversion::ToOpenCvImage(pBuffer, imageWidth, imageHeight, imageConverted);
		ImageConversion::ToGalaxyImage(imageConverted, newImage);

		//Display image
		pSingleCamDlg->bitmap->Show(pBuffer, imageWidth, imageHeight);

		//Check whether is need to save image or not.
		if (pSingleCamDlg->checkSaveBmp == TRUE && frame % 15 == 0)
		{
			pSingleCamDlg->SavePicture(imageConverted);
		}
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