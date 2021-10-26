#include "stdafx.h"
#include "ImageConversion.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

Mat ImageConversion::ToOpenCvImage(BYTE* image, int w, int h)
{
	auto size = w * h * 3;
	BYTE* buffer = new BYTE[size];
	for (int i = 0; i < size; i++)
	{
		buffer[i] = image[size - i - 1];
	}
	Mat mat = Mat(h, w, CV_8UC3, (unsigned*)buffer);
	Mat output;
	cvtColor(mat, output, ColorConversionCodes::COLOR_RGB2BGR);
	delete buffer;
	return output;
}

BYTE* ImageConversion::ToGalaxyImage(Mat& image)
{
	return nullptr;
}
