#ifndef TESTPROCESSOR_H
#define TESTPROCESSOR_H

#include "imageprocessor.h"

using namespace cv;

class TestProcessor : public ImageProcessor
{
public:
    void ProcessImage(Mat &input, Mat &output);
};

#endif // TESTPROCESSOR_H
