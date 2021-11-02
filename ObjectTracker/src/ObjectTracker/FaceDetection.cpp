#include "stdafx.h"
#include "FaceDetection.h"
#include<iostream>
#include<stdlib.h>
#include<vector>
#include<direct.h>

FaceDetection::FaceDetection() : scale(1) {

	if (cascade.load("./haarcascade_frontalface_default.xml") == true) {
		cout << "Face Detection Started.." << endl;
	}
	else
		throw std::runtime_error("File not loaded");
}

void FaceDetection::Detection(Mat& image)
{
	cout << "Face Detection Started.." << endl;
	
	if (image.empty())
		throw std::runtime_error("Empty image");

	DetectFace(image);

}


void FaceDetection::DetectFace(Mat& img) {

	vector<cv::Rect> faces;
	Mat gray, smallImg;
	Mat equalizeImg;

	cvtColor(img, gray, COLOR_BGR2GRAY);
	double fx = 1 / scale;

	resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR_EXACT);
	equalizeHist(smallImg, equalizeImg);

	cascade.detectMultiScale(equalizeImg, faces);

	DrawElement::DrawCircle(faces, scale, img);
	int i = 0;
	i++;
	//imwrite("C:\\Users\\ylenia\\Desktop\\output.jpg", img);

}