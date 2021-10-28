#pragma once

#include <opencv2/core.hpp>
#include "GalaxyIncludes.h"

using namespace cv;

class ImageConversion
{
public:
	static void ToOpenCvImage(BYTE* image, int w, int h, Mat& output);
	static void ToGalaxyImage(Mat& image, int w, int h, BYTE* output);
};
