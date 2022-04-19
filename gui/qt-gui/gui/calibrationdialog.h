#pragma once

#include "aspectratiolabel.h"

#include <QDialog>
#include <QStandardItemModel>
#include <QTimer>

#include "model/camera.h"

#include <calibration/calibrationprocess.h>

using namespace cv;
using namespace std;

namespace Ui {
class CalibrationDialog;
}

class CalibrationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrationDialog(Camera *camera, QWidget *parent = nullptr);
    ~CalibrationDialog();

    void readCalibrationResult(Mat &cameraMatrix, Mat &distortionCoefficients);

private:
    Ui::CalibrationDialog *ui;
    QTimer timer;
    Camera *camera;
    Mat currentFrame;
    FrameData currentFrameData;
    bool patternFoundOnCurrentFrame;
    CalibrationProcess calibrationProcess;
    QStandardItemModel *calibrationFramesModel;
    AspectRatioLabel *display;
    Mat cameraMatrix;
    Mat distortionCoefficients;

private slots:
    void onNewFrame();
    void onAddFrameClicked();
    void onRemoveFrameClicked();
    void onRunCalibrationClicked();
};