#include "drawelement.h"

using namespace cv;
void DrawElement::drawRectangle(Mat& output, vector<Rect>& faces)
{
    for (int i = 0; i < faces.size(); i++)
    {
        cv::rectangle(output, faces[i], Scalar(0, 0, 255), 2, 8, 0);
    }
}

void DrawElement::drawCircle(Mat& output, vector<Rect>& faces)
{
    for (size_t i = 0; i < faces.size(); i++)
    {
        Rect r = faces[i];
        Point center(cvRound(r.x + r.width / 2), cvRound(r.y + r.height / 2));
        int radius = cvRound((r.width + r.height) / 4);
        circle(output, center, radius, Scalar(255, 0, 0), 3, 8, 0);
    }
}

void DrawElement::circleToRectangle(vector<Vec3f> circles, Mat& output)
{
    for (size_t i = 0; i < circles.size(); i++)
    {
        Vec3i c = circles[i];
        int radius = c[2];

        Point pt1 = Point(c[0]-radius, c[1]+radius);
        Point pt2 = Point(c[0]-radius, c[1]-radius);
        Point pt3 = Point(c[0]+radius, c[1]+radius);
        Point pt4 = Point(c[0]+radius, c[1]-radius);
        vector<Point2f> corners{pt1, pt2, pt3, pt4};
        cv::rectangle(output, pt2, pt3, Scalar(0, 255, 0), 2, 8, 0);
        // circle outline
        //circle(detectImgCircles, center, radius, Scalar(0, 255, 0), 4, LINE_AA);
    }
}
