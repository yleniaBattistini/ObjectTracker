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

		//Display image
		pSingleCamDlg->bitmap->Show(objImageDataPointer);

		//Check whether is need to save image or not.
		if (pSingleCamDlg->checkSaveBmp == TRUE && frame%15==0)
		{
			pSingleCamDlg->SavePicture(objImageDataPointer);
		}
	}
	catch (std::exception)
	{
		//do nothing
	}
}