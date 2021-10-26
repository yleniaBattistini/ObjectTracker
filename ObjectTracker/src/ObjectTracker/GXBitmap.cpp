#include "stdafx.h"
#include "GXBitmap.h"
#include <iostream>
#include<string> 
#include "ImageConversion.h"
using namespace std;

CGXBitmap::CGXBitmap(CWnd* display, int64_t width, int64_t height) :
	imageDisplay(display),
	hdc(NULL),
	imageWidth(width),
	imageHeight(height),
	bitmapInfo(NULL)
{
	if (imageDisplay == NULL)
	{
		throw std::runtime_error("Argument is error");
	}

	HWND hWnd = display->m_hWnd;
	if (!::IsWindow(hWnd))
	{
		throw std::runtime_error("The HWND must be form");
	}

	hdc = ::GetDC(imageDisplay->m_hWnd);
	memset(bitmapBuffer, 0, sizeof(bitmapBuffer));

	__ColorPrepareForShowImg();
}

CGXBitmap::~CGXBitmap(void)
{
	::ReleaseDC(imageDisplay->m_hWnd, hdc);
}

GX_VALID_BIT_LIST CGXBitmap::GetBestValueBit(GX_PIXEL_FORMAT_ENTRY emPixelFormatEntry)
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

void CGXBitmap::__ColorPrepareForShowImg()
{
	bitmapInfo = (BITMAPINFO *)bitmapBuffer;
	bitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo->bmiHeader.biWidth = (LONG)imageWidth;
	bitmapInfo->bmiHeader.biHeight = (LONG)imageHeight;

	bitmapInfo->bmiHeader.biPlanes = 1;
	bitmapInfo->bmiHeader.biBitCount = 24;
	bitmapInfo->bmiHeader.biCompression = BI_RGB;
	bitmapInfo->bmiHeader.biSizeImage = 0;
	bitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	bitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	bitmapInfo->bmiHeader.biClrUsed = 0;
	bitmapInfo->bmiHeader.biClrImportant = 0;
}

bool CGXBitmap::__IsCompatible(BITMAPINFO *pBmpInfo, uint64_t nWidth, uint64_t nHeight)
{
	if (pBmpInfo == NULL
		|| pBmpInfo->bmiHeader.biHeight != nHeight
		|| pBmpInfo->bmiHeader.biWidth != nWidth
		)
	{
		return false;
	}
	return true;
}

void CGXBitmap::__UpdateBitmap(CImageDataPointer& objCImageDataPointer)
{
	if (!__IsCompatible(bitmapInfo, objCImageDataPointer->GetWidth(), objCImageDataPointer->GetHeight()))
	{
		imageWidth = objCImageDataPointer->GetWidth();
		imageHeight = objCImageDataPointer->GetHeight();
		__ColorPrepareForShowImg();
	}
}

void CGXBitmap::__DrawImg(BYTE* pBuffer)
{
	int nWndWidth = 0;
	int nWndHeight = 0;

	// Prepare for drawing images
	RECT objRect;
	imageDisplay->GetClientRect(&objRect);	
	nWndWidth  = objRect.right - objRect.left;
	nWndHeight = objRect.bottom - objRect.top;

	::SetStretchBltMode(hdc, COLORONCOLOR);
	::StretchDIBits(hdc,
		0,
		0,
		nWndWidth,
		nWndHeight,
		0,
		0,
		(int)imageWidth,
		(int)imageHeight,
		pBuffer,
		bitmapInfo,
		DIB_RGB_COLORS,
		SRCCOPY
		);
}

int64_t CGXBitmap::__GetStride(int64_t nWidth)
{
	return nWidth * 3;
}

void CGXBitmap::Show(CImageDataPointer& objCImageDataPointer)
{
	GX_VALID_BIT_LIST emValidBits = GX_BIT_0_7;
	BYTE* pBuffer = NULL;

	if (objCImageDataPointer.IsNull())
	{
		throw std::runtime_error("NULL pointer dereferenced");
	}

	//Check whether update bitmap information
	__UpdateBitmap(objCImageDataPointer);

	emValidBits = GetBestValueBit(objCImageDataPointer->GetPixelFormat());
	pBuffer = (BYTE*)objCImageDataPointer->ConvertToRGB24(emValidBits, GX_RAW2RGB_NEIGHBOUR, true);

	ImageConversion::ToOpenCvImage(pBuffer, imageWidth, imageHeight);

	__DrawImg(pBuffer);
}

void CGXBitmap::SaveBmp(CImageDataPointer& objCImageDataPointer,const std::string& strFilePath)
{
	GX_VALID_BIT_LIST emValidBits = GX_BIT_0_7;
	BYTE* pBuffer = NULL;

	if ((objCImageDataPointer.IsNull())||(strFilePath == ""))
	{
		throw std::runtime_error("Argument is error");
	}

	//Check whether update bitmap information
	__UpdateBitmap(objCImageDataPointer);

	emValidBits = GetBestValueBit(objCImageDataPointer->GetPixelFormat());
	pBuffer = (BYTE*)objCImageDataPointer->ConvertToRGB24(emValidBits, GX_RAW2RGB_NEIGHBOUR, true);

	DWORD dwImageSize = (DWORD)(__GetStride(imageWidth) * imageHeight);
	BITMAPFILEHEADER stBfh = {0};
	DWORD dwBytesRead = 0;

	stBfh.bfType	= (WORD)'M' << 8 | 'B';			 //Define file type
	stBfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	stBfh.bfSize	= stBfh.bfOffBits + dwImageSize; //File size

	DWORD dwBitmapInfoHeader = sizeof(BITMAPINFOHEADER);

	//Create file
	HANDLE hFile = ::CreateFile(strFilePath.c_str(),
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,														
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE) 
	{
		throw std::runtime_error("Handle is invalid");
	}

	::WriteFile(hFile, &stBfh, sizeof(BITMAPFILEHEADER), &dwBytesRead, NULL);
	::WriteFile(hFile, bitmapInfo, dwBitmapInfoHeader, &dwBytesRead, NULL);
	::WriteFile(hFile, pBuffer, dwImageSize, &dwBytesRead, NULL);

	CloseHandle(hFile);
}
