#include "stdafx.h"
#include "ImageConversion.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

void ImageConversion::ToOpenCvImage(BYTE* image, int w, int h, Mat& output)
{
	auto size = w * h * 3;
	BYTE* buffer = new BYTE[size];
	for (int i = 0; i < size; i++)
	{
		buffer[i] = image[size - i - 1];
	}
	Mat mat = Mat(h, w, CV_8UC3, (unsigned*)buffer);
	cvtColor(mat, output, ColorConversionCodes::COLOR_RGB2BGR);
	delete buffer;
}

void ImageConversion::ToGalaxyImage(Mat& image, BYTE* output)
{
	uint8_t* pixelPtr = (uint8_t*)image.data;
	int channels = image.channels();
	Scalar_<uint8_t> bgrPixel;
	int count = 0;

	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			output[count] = pixelPtr[j * channels + 0]; // B
			output[count + 1] = pixelPtr[j * channels + 1]; // G
			output[count + 2] = pixelPtr[j * channels + 2]; // R
			count += 3;
		}
	}
}
