#pragma once

#include "GalaxyIncludes.h"

class CGXBitmap
{
public:
	/// construction
	CGXBitmap(CWnd* display);

	/// destruction
	~CGXBitmap(void);
	
	 /// Display image
	 void Show(BYTE* pBuffer, int imageWidth, int imageHeight);

private:
	/// Prepare resources for displaying color camera images
	void __ColorPrepareForShowImg(BITMAPINFO& bitmapInfo, int imageWidht, int imageHeight);

	/// Display image to UI
	void __DrawImg(BYTE* pBuffer, BITMAPINFO& bitmapInfo);

	/// Calculate the number of bytes occupied by width
	int64_t __GetStride(int64_t nWidth);


private:
	CWnd* imageDisplay;
	HDC hdc;
private:
	CGXBitmap& operator=(const CGXBitmap&);
	CGXBitmap(const CGXBitmap&);
};

