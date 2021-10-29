#pragma once

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
	static void DrawRectangle(vector<Rect>& faces, Mat& output);

	static void DrawCircle(vector<Rect>& faces, double scale, Mat& output);
};

