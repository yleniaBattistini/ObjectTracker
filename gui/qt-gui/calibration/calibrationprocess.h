#pragma once

#include "model/camera.h"
#include <opencv2/core.hpp>
#include <vector>

using namespace cv;
using namespace std;

typedef vector<Point2f> CalibrationFrame;

class CalibrationProcess
{
public:
    CalibrationProcess();

    const Size CHESSBOARD_SIZE = Size(9, 6);

    bool detectChessboardCorners(const Mat &image, vector<Point2f> &corners);

    void addFrame(const CalibrationFrame &frame);
    void removeFrame(int index);
    int frameCount();
    CalibrationFrame frameAt(int index);
    bool runCalibration(Size imageSize, double squareSize, Mat &cameraMatrix, Mat &distortionCoefficients);

private:
    vector<CalibrationFrame> frames;

    void get3dCornerPositions(double squareSize, vector<Point3f> &corners);
};
