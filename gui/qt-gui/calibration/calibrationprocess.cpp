#include "calibrationprocess.h"

#include <opencv2/calib3d.hpp>

CalibrationProcess::CalibrationProcess()
{
}

bool CalibrationProcess::detectChessboardCorners(const Mat &image, vector<Point2f> &corners)
{
    return findChessboardCorners(image, CHESSBOARD_SIZE, corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
}

void CalibrationProcess::addFrame(const CalibrationFrame &frame)
{
    frames.push_back(frame);
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

CalibrationFrame CalibrationProcess::frameAt(int index)
{
    return frames.at(index);
}

bool CalibrationProcess::runCalibration(Size imageSize, double squareSize, Mat &cameraMatrix, Mat &distortionCoefficients)
{
    cameraMatrix = Mat::eye(3, 3, CV_64F);
    distortionCoefficients = Mat::zeros(8, 1, CV_64F);

    vector<Point3f> cornerPositions;
    get3dCornerPositions(squareSize, cornerPositions);
//    double gridWidth = squareSize * (CHESSBOARD_SIZE.width - 1);
//    cornerPositions[CHESSBOARD_SIZE.width - 1].x = cornerPositions[0].x + gridWidth;

    vector<vector<Point3f>> objectPoints;
    objectPoints.resize(frames.size(), cornerPositions);

    vector<Mat> rvecs, tvecs;
    int flags = CALIB_USE_LU | CALIB_ZERO_TANGENT_DIST;
    calibrateCamera(objectPoints, frames, imageSize, cameraMatrix, distortionCoefficients, rvecs, tvecs, flags);

    return checkRange(cameraMatrix) && checkRange(distortionCoefficients);
}

void CalibrationProcess::get3dCornerPositions(double squareSize, vector<Point3f> &corners)
{
    corners.clear();
    for(int i = 0; i < CHESSBOARD_SIZE.height; i++)
    {
        for(int j = 0; j < CHESSBOARD_SIZE.width; j++)
        {
            corners.push_back(Point3f(j*squareSize, i*squareSize, 0));
        }
    }
}
