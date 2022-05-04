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
        double x = faces[i].x;
        double y = faces[i].y;
        double w = faces[i].width;
        double h = faces[i].height;
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

void DrawElement::drawAxis(Mat& img, vector<Mat> rvecs, vector<Mat> tvecs, Mat& cameraMatrix)
{
//       :param img - rgb numpy array
//       :rotation_vec - euler rotations, numpy array of length 3,
//                       use cv2.Rodrigues(R)[0] to convert from rotation matrix
//       :t - 3d translation vector, in meters (dtype must be float)
//       :K - intrinsic calibration matrix , 3x3
//       :scale - factor to control the axis lengths
//       :dist - optional distortion coefficients, numpy array of length 4. If None distortion is ignored.

       /*img = img.astype(np.float32)
       dist = np.zeros(4, dtype=float) if dist is None else dist
       points = scale * np.float32([[1, 0, 0], [0, 1, 0], [0, 0, 1], [0, 0, 0]]).reshape(-1, 3)
       axis_points, _ = cv2.projectPoints(points, rotation_vec, t, K, dist)
       img = cv2.line(img, tuple(axis_points[3].ravel()), tuple(axis_points[0].ravel()), (255, 0, 0), 3)
       img = cv2.line(img, tuple(axis_points[3].ravel()), tuple(axis_points[1].ravel()), (0, 255, 0), 3)
       img = cv2.line(img, tuple(axis_points[3].ravel()), tuple(axis_points[2].ravel()), (0, 0, 255), 3)
       return img*/
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
        cv::rectangle(output, pt1, pt2, Scalar(0, 255, 0));
        cp->computePose(output, corners);
        // circle outline
        //circle(detectImgCircles, center, radius, Scalar(0, 255, 0), 4, LINE_AA);
    }
}
