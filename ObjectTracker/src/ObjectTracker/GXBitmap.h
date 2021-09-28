#pragma once

#include "GalaxyIncludes.h"

class CGXBitmap
{
public:
	/// construction
	CGXBitmap(CWnd* display, int64_t width, int64_t height);

	/// destruction
	~CGXBitmap(void);
	
	 /// Display image
	 void Show(CImageDataPointer& objCImageDataPointer);

	 /// the image are stored as BMP format
	 void SaveBmp(CImageDataPointer& objCImageDataPointer,const std::string& strFilePath);
private:
	/// Prepare resources for displaying color camera images
	void __ColorPrepareForShowImg();

	/// whether compatible
	bool __IsCompatible(BITMAPINFO *pBmpInfo, uint64_t nWidth, uint64_t nHeight);

	/// Update Bitmap information
	void __UpdateBitmap(CImageDataPointer& objCImageDataPointer);

	/// Display image to UI
	void __DrawImg(BYTE* pBuffer);

	/// Calculate the number of bytes occupied by width
	int64_t __GetStride(int64_t nWidth);

	/// Get the best Bit by GX_PIXEL_FORMAT_ENTRY
	GX_VALID_BIT_LIST GetBestValueBit(GX_PIXEL_FORMAT_ENTRY emPixelFormatEntry);


private:
	CWnd* imageDisplay;
	int64_t imageHeight;
	int64_t imageWidth;
	BITMAPINFO *bitmapInfo;
	char bitmapBuffer[2048];
	HDC hdc;
private:
	CGXBitmap& operator=(const CGXBitmap&);
	CGXBitmap(const CGXBitmap&);
};

