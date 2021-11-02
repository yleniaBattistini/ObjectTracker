#include "stdafx.h"
#include "DrawElement.h"


void DrawElement::DrawRectangle(Mat& output, vector<Rect>& faces)
{
	for (int i = 0; i < faces.size(); i++)
	{
		Point pt1(faces[i].x, faces[i].y);
		Point pt2((faces[i].x + faces[i].height), (faces[i].y + faces[i].width));
		cv::rectangle(output, pt1, pt2, cv::Scalar(0, 0, 255), 2, 8, 0);
	}
}

void DrawElement::DrawCircle(Mat& output, vector<Rect>& faces)
{
	for (size_t i = 0; i < faces.size(); i++)
	{
		Rect r = faces[i];
		Point center(cvRound(r.x + r.width / 2), cvRound(r.y + r.height / 2));
		int radius = cvRound((r.width + r.height) / 4);
		circle(output, center, radius, Scalar(255, 0, 0), 3, 8, 0);
	}
}
