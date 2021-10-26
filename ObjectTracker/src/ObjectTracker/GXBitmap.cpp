#include "stdafx.h"
#include "GXBitmap.h"
#include <iostream>
#include<string> 
#include "ImageConversion.h"
using namespace std;

CGXBitmap::CGXBitmap(CWnd* display) :
	imageDisplay(display),
	hdc(NULL)
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
}

CGXBitmap::~CGXBitmap(void)
{
	::ReleaseDC(imageDisplay->m_hWnd, hdc);
}


void CGXBitmap::__ColorPrepareForShowImg(BITMAPINFO& bitmapInfo, int imageWidth, int imageHeight)
{
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth = (LONG)imageWidth;
	bitmapInfo.bmiHeader.biHeight = (LONG)imageHeight;

	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 24;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biSizeImage = 0;
	bitmapInfo.bmiHeader.biXPelsPerMeter = 0;
	bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
	bitmapInfo.bmiHeader.biClrUsed = 0;
	bitmapInfo.bmiHeader.biClrImportant = 0;
}

void CGXBitmap::__DrawImg(BYTE* pBuffer, BITMAPINFO& bitmapInfo)
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
		bitmapInfo.bmiHeader.biWidth,
		bitmapInfo.bmiHeader.biHeight,
		pBuffer,
		&bitmapInfo,
		DIB_RGB_COLORS,
		SRCCOPY
		);
}

int64_t CGXBitmap::__GetStride(int64_t nWidth)
{
	return nWidth * 3;
}

void CGXBitmap::Show(BYTE* pBuffer, int imageWidth, int imageHeight)
{
	BITMAPINFO bitmapInfo;

	__ColorPrepareForShowImg(bitmapInfo, imageWidth, imageHeight);
	
	
	__DrawImg(pBuffer, bitmapInfo);
	
}
