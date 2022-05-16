#pragma once

#include <opencv2/core.hpp>

using namespace cv;

Mat rot2euler(const Mat& rotationMatrix);

Mat euler2rot(const Mat& euler);

Mat rotBetweenVectors(const Mat &v1, const Mat &v2);

Mat rotationAroundX(double theta);

Mat rotationAroundY(double theta);

double linearInterpolation(double a, double b, double percentage);
