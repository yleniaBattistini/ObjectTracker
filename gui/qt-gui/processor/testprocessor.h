#pragma once

#include "imageprocessor.h"

using namespace cv;

class TestProcessor : public ImageProcessor
{
public:
    void ProcessImage(Mat &input, Mat &output);
};
