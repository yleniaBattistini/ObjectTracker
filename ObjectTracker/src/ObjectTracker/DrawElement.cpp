#include "stdafx.h"
#include "DrawElement.h"


void DrawElement::DrawRectangle(vector<Rect>& faces, Mat& output)
{
	for (int i = 0; i < faces.size(); i++) {
		Point pt1(faces[i].x, faces[i].y);
		Point pt2((faces[i].x + faces[i].height), (faces[i].y + faces[i].width));
		cv::rectangle(output, pt1, pt2, cv::Scalar(0, 0, 255), 2, 8, 0);
	}

}

void DrawElement::DrawCircle(vector<Rect>& faces, double scale, Mat& output)
{
	for (size_t i = 0; i < faces.size(); i++)
	{
		Rect r = faces[i];
		Mat smallImgROI;
		vector<Rect> nestedObjects;
		Point center;
		Scalar color = Scalar(255, 0, 0); // Color for Drawing tool
		int radius;

		double aspect_ratio = (double)r.width / r.height;
		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		{
			center.x = cvRound((r.x + r.width * 0.5) * scale);
			center.y = cvRound((r.y + r.height * 0.5) * scale);
			radius = cvRound((r.width + r.height) * 0.25 * scale);
			circle(output, center, radius, color, 3, 8, 0);
		}

	}
}
