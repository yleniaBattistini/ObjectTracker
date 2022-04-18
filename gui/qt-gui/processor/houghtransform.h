#ifndef HOUGHTRANSFORM_H
#define HOUGHTRANSFORM_H

#include <opencv2/core.hpp>

using namespace cv;

class HoughTransform
{
public:
    static void houghTransform(Mat& input, Mat& output);
};

#endif // HOUGHTRANSFORM_H
