#include "calibrationprocess.h"

#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

CalibrationProcess::CalibrationProcess(Camera *camera) : camera(camera)
{
}

bool CalibrationProcess::detectPattern(const Mat &image, FrameData &frameData)
{
    int flags = CALIB_CB_ADAPTIVE_THRESH
            | CALIB_CB_NORMALIZE_IMAGE
            | CALIB_CB_FAST_CHECK;
    bool found = findChessboardCorners(image, CHESSBOARD_SIZE, frameData, flags);
    if (found)
    {
        Mat viewGray;
        cvtColor(image, viewGray, COLOR_BGR2GRAY);
        cornerSubPix(
            viewGray,
            frameData,
            Size(11, 11),
            Size(-1, -1),
            TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.1)
        );
    }
    return found;
}

void CalibrationProcess::drawPattern(Mat &image, const FrameData &frameData)
{
    drawChessboardCorners(image, CHESSBOARD_SIZE, frameData, true);
}

void CalibrationProcess::addFrame(const FrameData &frameData)
{
    frames.push_back(frameData);
}

void CalibrationProcess::removeFrame(int index)
{
    if (index < 0 || index >= frames.size())
    {
        return;
    }
    frames.erase(frames.begin() + index);
}

int CalibrationProcess::frameCount()
{
    return frames.size();
}

FrameData CalibrationProcess::frameAt(int index)
{
    return frames.at(index);
}

bool CalibrationProcess::runCalibration(Size imageSize, double squareSize)
{
    Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
    Mat distortionCoefficients = Mat::zeros(8, 1, CV_64F);

    vector<Point3f> cornerPositions;
    get3dCornerPositions(squareSize, cornerPositions);

    vector<vector<Point3f>> objectPoints;
    objectPoints.resize(frames.size(), cornerPositions);

    vector<Mat> rvecs, tvecs;
    int flags = CALIB_USE_LU | CALIB_ZERO_TANGENT_DIST;
    calibrateCamera(objectPoints, frames, imageSize, cameraMatrix, distortionCoefficients, rvecs, tvecs, flags);

    bool valid = checkRange(cameraMatrix) && checkRange(distortionCoefficients);
    if (valid)
    {
        camera->calibrate(cameraMatrix, distortionCoefficients);
    }
    return valid;
}

void CalibrationProcess::get3dCornerPositions(double squareSize, vector<Point3f> &corners)
{
    corners.clear();
    for (int i = 0; i < CHESSBOARD_SIZE.height; i++)
    {
        for (int j = 0; j < CHESSBOARD_SIZE.width; j++)
        {
            corners.push_back(Point3f(j * squareSize, i * squareSize, 0));
        }
    }
}
