#ifndef DRAWELEMENT_H
#define DRAWELEMENT_H

#include<iostream>
#include<stdlib.h>
#include<vector>
#include<direct.h>
#include<opencv2/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/objdetect.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include "computepose.h"

using namespace cv;
using namespace std;

class DrawElement
{
public:
    static void drawRectangle(Mat& output, vector<Rect>& faces);

    static void drawCircle(Mat& output, vector<Rect>& faces);

    static void circleToRectangle(vector<Vec3f> circles, Mat& output);
};

#endif // DRAWELEMENT_H
