#include "stdafx.h"
#include "ImageCapturedHandler.h"

void ImageCapturedHandler::DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam)
{
	try
	{
		CGxSingleCamDlg* pSingleCamDlg = (CGxSingleCamDlg*)pUserParam;

		//Display image
		pSingleCamDlg->bitmap->Show(objImageDataPointer);

		//Check whether is need to save image or not.
		if (pSingleCamDlg->checkSaveBmp == TRUE)
		{
			pSingleCamDlg->SavePicture(objImageDataPointer);
		}
	}
	catch (std::exception)
	{
		//do nothing
	}
}