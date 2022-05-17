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
#include <QElapsedTimer>
#include <algorithm>
#include <initializer_list>
#include <QDebug>
#include <QMessageBox>

using namespace std;

MainWindow::MainWindow(FaceDetection *faceDetector, QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow),
    camera(NULL),
    controller(NULL),
    faceDetector(faceDetector)
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
    ui->cmbDetectionType->setVisible(camera != NULL);
    ui->chkRemoveDistortion->setVisible(camera != NULL);
}

bool MainWindow::calibrateCamera()
{
    if (controller != NULL)
    {
        controller->setCalibrationState(true);
    }

    CalibrationDialog calibrationDialog(camera, &poseController);
    calibrationDialog.setWindowTitle("Calibration");
    int result = calibrationDialog.exec();

    if (controller != NULL)
    {
        controller->setCalibrationState(false);
    }

    return result == QDialog::Accepted;
}

void MainWindow::onNewFrame()
{
    Mat frame;
    camera->acquireNextFrame(frame, ui->chkRemoveDistortion->isChecked());
    rawImageViewer->setOpencvImage(frame);

    Mat warpedFrame;
    vector<Rect> rectangles;
    if (ui->cmbDetectionType->currentText().compare("Face Detection") == 0)
    {
        faceDetector->detection(frame, rectangles);
    }
    else
    {
        HoughTransform::houghTransform(frame, rectangles);
    }

    bool detectedState = !rectangles.empty();
    if (controller != NULL && detectedState != lastDetectedState)
    {
        controller->setDetectedState(detectedState);
    }
    lastDetectedState = detectedState;

    if (detectedState)
    {
        DrawElement::drawRectangle(frame, rectangles);
        Rect biggestRect = *std::max_element(rectangles.begin(), rectangles.end(), [](const Rect a, const Rect b)
        {
            return a.area() < b.area();
        });

        float x = (float) biggestRect.x;
        float y = (float) biggestRect.y;
        float w = (float) biggestRect.width;
        float h = (float) biggestRect.height;
        vector<Point2f> corners = {
            Point2f(x, y + h),
            Point2f(x, y),
            Point2f(x + w, y + h),
            Point2f(x + w, y)
        };
        poseController.setObjectPose(corners);
    }
    poseController.applyRotationToFrame(frame, warpedFrame);
    processedImageViewer->setOpencvImage(warpedFrame);
}

void MainWindow::onStartCameraClicked()
{
    camera = new WebCam(0);
    if (!calibrateCamera())
    {
        delete camera;
        camera = NULL;
        return;
    }
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

