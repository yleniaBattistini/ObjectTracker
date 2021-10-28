#pragma once

#include "GalaxyIncludes.h"

class ImageViewer
{
public:
	/// construction
	ImageViewer(CWnd* display);

	/// destruction
	~ImageViewer(void);
	
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
	ImageViewer& operator=(const ImageViewer&);
	ImageViewer(const ImageViewer&);
};

