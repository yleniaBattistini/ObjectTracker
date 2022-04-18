#include "houghtransform.h"
#include <opencv2\imgcodecs.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\core\types.hpp>
#include <vector>

using namespace cv;
using namespace std;
void HoughTransform::houghTransform(Mat& input, Mat& output)
{
        Mat imgCircles, detectImgCircles;
        imgCircles = input;
        imgCircles.copyTo(detectImgCircles);

        // Circle Detection Hough Transform
        Mat gray;
        cvtColor(imgCircles, gray, cv::COLOR_RGB2GRAY);

        medianBlur(gray, gray, 5);

        vector<Vec3f> circles;
        HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, //or 1.5, without 100 & 30
            gray.rows / 16, // change this value to detect circles with different distances to each other
            100, 30 // change the last two parameters
        );

        if (circles.size() != 0) {
            for (size_t i = 0; i < circles.size(); i++)
            {
                Vec3i c = circles[i];
                Point center = Point(c[0], c[1]);
                int radius = c[2];

                // circle outline
                circle(detectImgCircles, center, radius, Scalar(0, 255, 0), 4, LINE_AA);
            }
        }

        detectImgCircles.copyTo(output);
}
