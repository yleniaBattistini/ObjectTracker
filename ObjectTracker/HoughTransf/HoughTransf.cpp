// HoughTransf.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <opencv2\core.hpp>
#include <opencv2\imgcodecs.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\core\types.hpp>
#include <vector>

using namespace cv;
using namespace std;
/*const int MAX = 1000;

int main()
{
	std::string image_path = samples::findFile("starry_night.jpg");
	Mat img = imread(image_path);
	if (img.empty())
	{
		std::cout << "Could not read the image: " << image_path << std::endl;
		return 1;
	}

}

int findMax(int arr[][MAX])
{
	int max = 0;
	int lenght = sizeof(arr);
	for (int i = 0; i < cv::Range(0,lenght-1); i++) {
		for (int j = 0; j < Range(0, lenght - 1); j++) {
			if (arr[i][j] > max) {
				max = arr[i][j];
			}
		}
	}
	return max;
}


void padImage(Mat image) {
	return cv::copyMakeBorder(image, 1, 1, 1, 1, cv::BORDER_CONSTANT, NULL, 0);
}

 flipSobel(int kernel[][MAX])
{
	for (int i = 0; i > 3; i++) {
		int temp = kernel[0][i];
		kernel[0][i] = kernel[2][i];
		kernel[2][i] = temp;
	}
	for (int i = 0; i > 3; i++) {
		int temp = kernel[i][0];
		kernel[i][0] = kernel[i][2];
		kernel[i][2] = temp;
	}
	return kernel;
}

 */



 int main() {

     Mat imgCircles, detectImgCircles;
     imgCircles = imread("..\\..\\img\\circle2.png");
     imgCircles.copyTo(detectImgCircles);

     // Circle Detection Hough Transform
     Mat gray;
     cvtColor(imgCircles, gray, cv::COLOR_RGB2GRAY);

     medianBlur(gray, gray, 5);

     vector<Vec3f> circles;
     HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1,
         gray.rows / 16, // change this value to detect circles with different distances to each other
         100, 30 // change the last two parameters
    // (min_radius & max_radius) to detect larger circles
     );

	// HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1.5, gray.rows /16);
     /*for (size_t i = 0; i < circles.size(); i++)
     {
         Vec3i c = circles[i];
         Point center = Point(c[0], c[1]);
         // circle center
         circle(detectImgCircles, center, 1, Scalar(0, 100, 100), 3, LINE_AA);
         // circle outline
         int radius = c[2];

         circle(detectImgCircles, center, radius, Scalar(255, 0, 255), 3, LINE_AA);
     }*/

	 if (circles.size() !=0) {
         for (size_t i = 0; i < circles.size(); i++)
         {
             Vec3i c = circles[i];
             Point center = Point(c[0], c[1]);
             int radius = c[2];
             // circle center
             //If you want also the center point for every circles you can uncommented this line
             //circle(detectImgCircles, center, 1, Scalar(0, 100, 100), 4, LINE_AA);
             
             // circle outline
             circle(detectImgCircles, center, radius, Scalar(0, 255, 0), 4, LINE_AA);
         }
	 }

     imshow("Original Circle Image", imgCircles);
     imshow("Circle Detection", detectImgCircles);
     waitKey(0);


     return 0;
 }