#include "testprocessor.h"

#include <opencv2/imgproc.hpp>

void TestProcessor::ProcessImage(Mat &input, Mat &output)
{
    Mat intermediate;
    flip(input, intermediate, 0);
    flip(intermediate, output, 1);
}