#include "posecontroller.h"

#include "utils/computepose.h"
#include "utils/geometrictransformation.h"

PoseController::PoseController() : homography(Mat::eye(3, 3, CV_64F))
{
    this->objectPoints = {
        Point3f(-1.0, -1.0, 0.0),
        Point3f(-1.0, 1.0, 0.0),
        Point3f(1.0, -1.0, 0.0),
        Point3f(1.0, 1.0, 0.0)
    };
}

void PoseController::setCalibration(Mat &cameraMatrix, Mat &distortionCoefficients)
{
    this->cameraMatrix = cameraMatrix;
    this->distortionCoefficients = distortionCoefficients;
}

void PoseController::setObjectPose(vector<Point2f> &imagePoints)
{
    Mat t;
    Mat rotVec;
    double targetThetaX;
    double targetThetaY;
    computePose(cameraMatrix, distortionCoefficients, objectPoints, imagePoints, t, rotVec, targetThetaX, targetThetaY);

    Mat R;
    Rodrigues(rotVec, R);

    currentThetaX = linearInterpolation(currentThetaX, targetThetaX, CAMERA_ROTATION_FACTOR);
    currentThetaY = linearInterpolation(currentThetaY, targetThetaY, CAMERA_ROTATION_FACTOR);
    Mat targetRotation = rotationAroundY(currentThetaY) * rotationAroundX(currentThetaX);

    vector<Point3f> transformedObjectPoints;
    for (Point3f &p : this->objectPoints)
    {
        Mat point = (Mat_<double>(3, 1) << p.x, p.y, p.z);
        Mat coordinatesInCameraWorld = (R * point) + t;
        Mat transformed = targetRotation * coordinatesInCameraWorld;

        transformedObjectPoints.push_back(Point3f(transformed));
    }

    vector<Point2f> projectedPoints;
    projectPoints(transformedObjectPoints, Vec3d::zeros(), Vec3d::zeros(), cameraMatrix, distortionCoefficients, projectedPoints);

    homography = findHomography(imagePoints, projectedPoints, RANSAC, 1.0);
}

void PoseController::applyRotationToFrame(Mat &frame, Mat &output)
{
    warpPerspective(frame, output, homography, frame.size());
}
