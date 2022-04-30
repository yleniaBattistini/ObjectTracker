#ifndef HOUGHTRANSFORM_H
#define HOUGHTRANSFORM_H

#include <opencv2/core.hpp>
#include "utils/drawelement.h"
#include "utils/computepose.h"

using namespace cv;

class HoughTransform
{
public:
    static void houghTransform(Mat& input, Mat& output, ComputePose *cp);
};

#endif // HOUGHTRANSFORM_H
