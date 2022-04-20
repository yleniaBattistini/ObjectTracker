#pragma once

#include "model/camera.h"
#include <opencv2/core.hpp>
#include <vector>

using namespace cv;
using namespace std;

class CalibrationProcess
{
public:
    CalibrationProcess(double squareSize);

    const Size CHESSBOARD_SIZE = Size(9, 6);

    bool detectPattern(const Mat &image, vector<Point2f> &corners);
    void drawPattern(Mat &image, const vector<Point2f> &corners);

    void setSquareSize(double size);

    void addView(const Mat &view, const vector<Point2f> &corners);
    void removeView(int index);
    void applyCalibration(Camera *camera);

    double getReprojectionError(int viewIndex);

private:
    vector<Mat> views;
    vector<Point3f> cornerPositions;
    vector<vector<Point2f>> cornersPerView;
    vector<double> reprojectionErrors;
    Mat cameraMatrix;
    Mat distortionCoefficients;

    void get3dCornerPositions(double squareSize, vector<Point3f> &corners);
    void recomputeCalibration();
};
