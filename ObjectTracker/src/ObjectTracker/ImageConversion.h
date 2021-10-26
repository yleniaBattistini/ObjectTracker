#pragma once

#include <opencv2/core.hpp>
#include "GalaxyIncludes.h"

using namespace cv;

class ImageConversion
{
public:
	static Mat ToOpenCvImage(BYTE* image, int w, int h);
	static BYTE* ToGalaxyImage(Mat& image);
};
