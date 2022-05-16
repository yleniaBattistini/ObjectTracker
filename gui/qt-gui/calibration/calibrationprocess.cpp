#include "calibrationprocess.h"

#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

CalibrationProcess::CalibrationProcess(double squareSize)
{
    setSquareSize(squareSize);
}

bool CalibrationProcess::detectPattern(const Mat &image, vector<Point2f> &corners)
{
    int flags = CALIB_CB_ADAPTIVE_THRESH
            | CALIB_CB_NORMALIZE_IMAGE
            | CALIB_CB_FAST_CHECK;
    bool found = findChessboardCorners(image, CHESSBOARD_SIZE, corners, flags);
    if (found)
    {
        Mat viewGray;
        cvtColor(image, viewGray, COLOR_BGR2GRAY);
        cornerSubPix(
            viewGray,
            corners,
            Size(11, 11),
            Size(-1, -1),
            TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.1)
        );
    }
    return found;
}

void CalibrationProcess::drawPattern(Mat &image, const vector<Point2f> &corners)
{
    drawChessboardCorners(image, CHESSBOARD_SIZE, corners, true);
}

void CalibrationProcess::setSquareSize(double size)
{
    squareSize = size;
    cornerPositions.clear();
    for (int i = 0; i < CHESSBOARD_SIZE.height; i++)
    {
        for (int j = 0; j < CHESSBOARD_SIZE.width; j++)
        {
            cornerPositions.push_back(Point3f(j * size, i * size, 0));
        }
    }
}

double CalibrationProcess::getSquareSize()
{
    return squareSize;
}

void CalibrationProcess::addView(const Mat &view, const vector<Point2f> &corners)
{
    Size viewSize = view.size();
    if (!cornersPerView.empty() && viewSize != imageSize)
    {
        throw runtime_error("Cannot use different image sizes");
    }
    imageSize = viewSize;
    cornersPerView.push_back(corners);
}

void CalibrationProcess::removeView(int index)
{
    if (index < 0 || index >= cornersPerView.size())
    {
        return;
    }
    cornersPerView.erase(cornersPerView.begin() + index);
}

void CalibrationProcess::clearViews()
{
    cornersPerView.clear();
}

void CalibrationProcess::getCalibrationResult(Mat &cameraMatrix, Mat &distortionCoefficients)
{
    cameraMatrix = this->cameraMatrix;
    distortionCoefficients = this->distortionCoefficients;
}

double CalibrationProcess::getReprojectionError(int viewIndex)
{
    return reprojectionErrors[viewIndex];
}

void CalibrationProcess::recomputeCalibration()
{
    if (cornersPerView.empty())
    {
        return;
    }

    cameraMatrix = Mat::eye(3, 3, CV_64F);
    distortionCoefficients = Mat::zeros(8, 1, CV_64F);

    vector<vector<Point3f>> objectPoints;
    objectPoints.resize(cornersPerView.size(), cornerPositions);

    Mat stdIntrinsics;
    Mat stdExtrinsics;
    calibrateCamera(
        objectPoints,
        cornersPerView,
        imageSize,
        cameraMatrix,
        distortionCoefficients,
        rvecs,
        tvecs,
        stdIntrinsics,
        stdExtrinsics,
        reprojectionErrors,
        CALIB_USE_LU | CALIB_ZERO_TANGENT_DIST
    );
}
