#pragma once

#include <QLabel>
#include <QMainWindow>
#include <QTimer>

#include <model/controller.h>

#include <processor/imageprocessor.h>

#include <gui/aspectratiolabel.h>

#include <opencv2/videoio.hpp>
#include "model/camera.h"

using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(ImageProcessor *processor, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Camera *camera;
    Controller *controller;
    ImageProcessor *processor;
    QTimer timer;
    Mat cameraMatrix;
    Mat distortionCoefficients;

    AspectRatioLabel *rawImageViewer;
    AspectRatioLabel *processedImageViewer;

    void updateUiState();
    void calibrateCamera();

private slots:
    void onNewFrame();

    void onStartCameraClicked();
    void onStopCameraClicked();
    void onConnectArduinoClicked();
    void onDisconnectArduinoClicked();

    void onCalibrationClicked();
};
