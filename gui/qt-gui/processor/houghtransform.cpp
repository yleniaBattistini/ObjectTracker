#include "houghtransform.h"
#include <opencv2\imgcodecs.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\core\types.hpp>
#include <vector>

using namespace cv;
using namespace std;
void HoughTransform::houghTransform(Mat& input, vector<Rect> &circles)
{
    // Circle Detection Hough Transform
    Mat gray;
    cvtColor(input, gray, COLOR_RGB2GRAY);

    medianBlur(gray, gray, 5);

    vector<Vec3f> circlesAsVectors;
    HoughCircles(
        gray,
        circlesAsVectors,
        HOUGH_GRADIENT,
        1.15, //or 1.5, without 100 & 30
        gray.rows / 5 // change this value to detect circles with different distances to each other
    );

    circles.clear();
    for (Vec3f &c : circlesAsVectors)
    {
        int radius = c[2];
        int x = c[0];
        int y = c[1];
        circles.push_back(Rect(x - radius, y - radius, radius * 2, radius * 2));
    }
}
