#ifndef POSECONTROLLER_H
#define POSECONTROLLER_H


#include <vector>
#include <opencv2/core.hpp>

using namespace cv;
using namespace std;

class PoseController
{
private:
    Mat cameraMatrix;
    Mat distortionCoefficients;
    vector<Point3f> objectPoints;
    Mat homography;
    double currentThetaX = 0.0;
    double currentThetaY = 0.0;
    const double CAMERA_ROTATION_FACTOR = 0.2;

public:
    PoseController();

    void setCalibration(Mat &cameraMatrix, Mat &distortionCoefficients);
    void setObjectPose(vector<Point2f> &imagePoints);
    void applyRotationToFrame(Mat &frame, Mat &output);
};

#endif // POSECONTROLLER_H
