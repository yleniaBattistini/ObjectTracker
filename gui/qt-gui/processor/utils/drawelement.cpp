#include "drawelement.h"

using namespace cv;
void DrawElement::drawRectangle(Mat& output, vector<Rect>& rectangles)
{
    for (int i = 0; i < rectangles.size(); i++)
    {
        cv::rectangle(output, rectangles[i], Scalar(0, 0, 255), 2, 8, 0);
    }
}
