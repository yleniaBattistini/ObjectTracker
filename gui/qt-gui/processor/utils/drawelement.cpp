#include "drawelement.h"

// ------> X
// |
// |
// v Y

using namespace cv;
void DrawElement::drawRectangle(ComputePose *cp, Mat& output, vector<Rect>& faces)
{
    for (int i = 0; i < faces.size(); i++)
    {
        float x = (float) faces[i].x;
        float y = (float) faces[i].y;
        float w = (float) faces[i].width;
        float h = (float) faces[i].height;
        Point2f pt1(x, y + h);
        Point2f pt2(x, y);
        Point2f pt3(x + w, y + h);
        Point2f pt4(x + w, y);
        cv::rectangle(output, pt2, pt3, Scalar(0, 0, 255), 2, 8, 0);
        vector<Point2f> corners{pt1, pt2, pt3, pt4};
        cp->computePose(output, corners);
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

void DrawElement::circleToRectangle(ComputePose *cp, vector<Vec3f> circles, Mat& output){
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
        cp->computePose(output, corners);
        // circle outline
        //circle(detectImgCircles, center, radius, Scalar(0, 255, 0), 4, LINE_AA);
    }
}
