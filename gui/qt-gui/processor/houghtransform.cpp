#include "houghtransform.h"
#include <opencv2\imgcodecs.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\core\types.hpp>
#include <vector>

using namespace cv;
using namespace std;
void HoughTransform::houghTransform(Mat& input, Mat& output, ComputePose *cp)
{
        Mat imgCircles;
        imgCircles = input;
        imgCircles.copyTo(output);

        // Circle Detection Hough Transform
        Mat gray;
        cvtColor(imgCircles, gray, COLOR_RGB2GRAY);

        medianBlur(gray, gray, 5);

        vector<Vec3f> circles;
        HoughCircles(
            gray,
            circles,
            HOUGH_GRADIENT,
            1.15, //or 1.5, without 100 & 30
            gray.rows / 5 // change this value to detect circles with different distances to each other
        );

        if (circles.size() != 0) {
            DrawElement::circleToRectangle(cp, circles, output);
        }
}
