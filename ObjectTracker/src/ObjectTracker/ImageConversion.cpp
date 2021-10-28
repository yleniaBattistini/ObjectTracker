#include "stdafx.h"
#include "ImageConversion.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

void ImageConversion::ToOpenCvImage(BYTE* image, int w, int h, Mat& output)
{
	auto size = w * h * 3;
	output = Mat(h, w, CV_8UC3);
	for (int i = 0; i < size; i += 3)
	{
		output.data[i + 0] = image[size - i - 3];
		output.data[i + 1] = image[size - i - 2];
		output.data[i + 2] = image[size - i - 1];
	}
}

void ImageConversion::ToGalaxyImage(Mat& image, int w, int h, BYTE* output)
{
	uint8_t* pixelPtr = (uint8_t*)image.data;
	int channels = image.channels();
	int count = 0;

	for (int i = image.rows - 1; i >= 0; i--)
	{
		for (int j = image.cols - 1; j >= 0; j--)
		{
			output[count + 0] = pixelPtr[i * image.cols * channels + j * channels + 0];
			output[count + 1] = pixelPtr[i * image.cols * channels + j * channels + 1];
			output[count + 2] = pixelPtr[i * image.cols * channels + j * channels + 2];
			count += 3;
		}
	}
}
