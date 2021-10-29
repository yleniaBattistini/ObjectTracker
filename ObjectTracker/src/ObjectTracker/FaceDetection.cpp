#include "stdafx.h"
#include "FaceDetection.h"
#include<iostream>
#include<stdlib.h>
#include<vector>
#include<direct.h>

FaceDetection::FaceDetection() : scale(1) {

	if (cascade.load("./haarcascade_frontalface_alt.xml") == true) {
		cout << "Face Detection Started.." << endl;
	}
	else
		throw std::runtime_error("File not loaded");
}

void FaceDetection::Detection(Mat& image)
{
	Mat frame;
	cout << "Face Detection Started.." << endl;

	if (image.empty())
		throw std::runtime_error("Empty image");

	Mat imageToDetect = image.clone();
	DetectFace(imageToDetect);

}


void FaceDetection::DetectFace(Mat& img) {

	vector<cv::Rect> faces;
	Mat gray, smallImg;

	cvtColor(img, gray, COLOR_BGR2GRAY);
	double fx = 1 / scale;
	Size_<int> size =  Size_<int>(0,0);

	resize(gray, smallImg, size, fx, fx, INTER_LINEAR_EXACT);
	equalizeHist(smallImg, smallImg);

	cascade.detectMultiScale(smallImg, faces , 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	DrawElement::DrawCircle(faces, scale, img);
	int i = 0;
	i++;
	imwrite("C:\\Users\\ylenia\\Desktop\\output.jpg", img);

}