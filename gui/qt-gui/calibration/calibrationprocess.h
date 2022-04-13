#pragma once

#include "model/camera.h"
#include <opencv2/core.hpp>
#include <vector>

using namespace cv;
using namespace std;

typedef vector<Point2f> FrameData;

class CalibrationProcess
{
public:
    CalibrationProcess();

    const Size CHESSBOARD_SIZE = Size(9, 6);

    bool detectPattern(const Mat &image, FrameData &frameData);
    void drawPattern(Mat &image, const FrameData &frameData);

    void addFrame(const FrameData &frameData);
    void removeFrame(int index);
    int frameCount();
    FrameData frameAt(int index);
    bool runCalibration(Size imageSize, double squareSize, Mat &cameraMatrix, Mat &distortionCoefficients);

private:
    vector<FrameData> frames;

    void get3dCornerPositions(double squareSize, vector<Point3f> &corners);
};
