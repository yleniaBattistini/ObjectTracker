#include "mainwindow.h"
#include "calibrationdialog.h"
#include "ui_mainwindow.h"
#include <opencv2/core/core.hpp>
#include <serial/serialportshelper.h>
#include <QString>
#include <model/webcam.h>
#include <string>
#include <gui/aspectratiolabel.h>
#include <QDialog>
#include <QPixmap>
#include <QSizePolicy>
#include "displayutils.h"
#include <algorithm>
#include <initializer_list>

using namespace std;

MainWindow::MainWindow(ImageProcessor *processor, FaceDetection *faceDetector, HoughTransform *houghTransform, ComputePose *computePose, QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow),
    camera(NULL),
    controller(NULL),
    processor(processor),
    faceDetector(faceDetector),
    houghTransform(houghTransform),
    computePose(computePose)
{
    ui->setupUi(this);

    for (string& port : SerialPortsHelper::listAvailablePorts())
    {
        ui->cmbSerialPort->addItem(QString::fromStdString(port));
    }

    updateUiState();

    connect(&timer, SIGNAL(timeout()), this, SLOT(onNewFrame()));
    connect(ui->btnStartCamera, SIGNAL(clicked()), this, SLOT(onStartCameraClicked()));
    connect(ui->btnStopCamera, SIGNAL(clicked()), this, SLOT(onStopCameraClicked()));
    connect(ui->btnConnectArduino, SIGNAL(clicked()), this, SLOT(onConnectArduinoClicked()));
    connect(ui->btnDisconnectArduino, SIGNAL(clicked()), this, SLOT(onDisconnectArduinoClicked()));
    connect(ui->btnCalibration, SIGNAL(clicked()), this, SLOT(onCalibrationClicked()));

    rawImageViewer = setupAsDisplay(ui->rawImageBox);
    processedImageViewer = setupAsDisplay(ui->processedImageBox);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete rawImageViewer;
    delete processedImageViewer;
    if (camera != NULL)
    {
        delete camera;
    }
    if (controller != NULL)
    {
        delete controller;
    }
}

void MainWindow::updateUiState()
{
    ui->arduinoBox->setEnabled(camera != NULL);
    ui->btnStartCamera->setVisible(camera == NULL);
    ui->btnStopCamera->setVisible(camera != NULL);
    ui->btnCalibration->setVisible(camera != NULL);
    ui->btnConnectArduino->setVisible(controller == NULL);
    ui->btnDisconnectArduino->setVisible(controller != NULL);
    ui->cmbSerialPort->setEnabled(controller == NULL);
}

void MainWindow::calibrateCamera()
{
    if (controller != NULL)
    {
        controller->setCalibrationState(true);
    }

    CalibrationDialog calibrationDialog(camera, computePose);
    calibrationDialog.setWindowTitle("Calibration");
    calibrationDialog.exec();

    if (controller != NULL)
    {
        controller->setCalibrationState(false);
    }
}

void MainWindow::onNewFrame()
{
    Mat frame;
    camera->acquireNextFrame(frame);
    rawImageViewer->setOpencvImage(frame);

    Mat imageComputePose;
    vector<Rect> faces;
    faceDetector->detection(frame, faces);

    bool detectedState = !faces.empty();
    if (controller != NULL && detectedState != lastDetectedState)
    {
        controller->setDetectedState(detectedState);
    }
    lastDetectedState = detectedState;

    if (detectedState)
    {
        DrawElement::drawRectangle(frame, faces);
        Rect biggestFace = *std::max_element(faces.begin(), faces.end(), [](const Rect a, const Rect b) { return a.area() < b.area(); });

        float x = (float) biggestFace.x;
        float y = (float) biggestFace.y;
        float w = (float) biggestFace.width;
        float h = (float) biggestFace.height;
        vector<Point2f> corners = {
            Point2f(x, y + h),
            Point2f(x, y),
            Point2f(x + w, y + h),
            Point2f(x + w, y)
        };

        computePose->computePose(frame, imageComputePose, corners);
        processedImageViewer->setOpencvImage(imageComputePose);
    }
    else
    {
        processedImageViewer->setOpencvImage(frame);
    }

//     Mat imageHough;
//     houghTransform->houghTransform(frame, imageHough, computePose);
//     processedImageViewer->setOpencvImage(imageHough);
}

void MainWindow::onStartCameraClicked()
{
    camera = new WebCam(0);
    calibrateCamera();
    timer.start(50);
    updateUiState();
}

void MainWindow::onStopCameraClicked()
{
    timer.stop();
    delete camera;
    camera = NULL;
    updateUiState();
}

void MainWindow::onConnectArduinoClicked()
{
    string portName = ui->cmbSerialPort->currentText().toStdString();
    controller = new Controller(portName);
    controller->connect();
    updateUiState();
}

void MainWindow::onDisconnectArduinoClicked()
{
    controller->disconnect();
    delete controller;
    controller = NULL;
    updateUiState();
}

void MainWindow::onCalibrationClicked()
{
    calibrateCamera();
}

