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

using namespace cv;
using namespace std;

class DrawElement
{
public:
    static void DrawRectangle(Mat& output, vector<Rect>& faces);

    static void DrawCircle(Mat& output, vector<Rect>& faces);
};

#endif // DRAWELEMENT_H
