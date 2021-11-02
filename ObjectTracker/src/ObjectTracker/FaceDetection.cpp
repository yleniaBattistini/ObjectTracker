#include "stdafx.h"
#include "FaceDetection.h"
#include<iostream>
#include<stdlib.h>
#include<vector>
#include<direct.h>

FaceDetection::FaceDetection() : scale(1)
{
	if (cascade.load("./haarcascade_frontalface_default.xml") == true) {
		cout << "Face Detection Started.." << endl;
	}
	else
		throw std::runtime_error("File not loaded");
}

void FaceDetection::Detection(Mat& image, vector<Rect>& faces)
{
	cout << "Face Detection Started.." << endl;
	
	if (image.empty())
		throw std::runtime_error("Empty image");

	DetectFace(image, faces);
}


void FaceDetection::DetectFace(Mat& img, vector<Rect>& faces)
{
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	equalizeHist(gray, gray);
	cascade.detectMultiScale(gray, faces);
}