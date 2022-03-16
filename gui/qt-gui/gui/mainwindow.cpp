#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/core/core.hpp>
#include <serial/serialportshelper.h>
#include <QString>
#include <model/webcam.h>
#include <string>
#include <gui/aspectratiolabel.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <QPixmap>
#include <QSizePolicy>

using namespace std;

MainWindow::MainWindow(ImageProcessor *processor, QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow),
    camera(NULL),
    controller(NULL),
    processor(processor)
{
    ui->setupUi(this);

    for (string& port : SerialPortsHelper::ListAvailablePorts())
    {
        ui->cmbSerialPort->addItem(QString::fromStdString(port));
    }

    UpdateUiState();

    connect(&timer, SIGNAL(timeout()), this, SLOT(OnNewFrame()));
    connect(ui->btnStartCamera, SIGNAL(clicked()), this, SLOT(OnStartCameraClicked()));
    connect(ui->btnStopCamera, SIGNAL(clicked()), this, SLOT(OnStopCameraClicked()));
    connect(ui->btnConnectArduino, SIGNAL(clicked()), this, SLOT(OnConnectArduinoClicked()));
    connect(ui->btnDisconnectArduino, SIGNAL(clicked()), this, SLOT(OnDisconnectArduinoClicked()));

    rawImageViewer = SetupAsDisplay(ui->rawImageBox);
    processedImageViewer = SetupAsDisplay(ui->processedImageBox);
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

void MainWindow::UpdateUiState()
{
    ui->arduinoBox->setEnabled(camera != NULL);
    ui->btnStartCamera->setVisible(camera == NULL);
    ui->btnStopCamera->setVisible(camera != NULL);
    ui->btnConnectArduino->setVisible(controller == NULL);
    ui->btnDisconnectArduino->setVisible(controller != NULL);
    ui->cmbSerialPort->setEnabled(controller == NULL);
}

AspectRatioLabel *MainWindow::SetupAsDisplay(QWidget *widget)
{
    AspectRatioLabel *viewer = new AspectRatioLabel();
    widget->layout()->addWidget(viewer);
    return viewer;
}

void MainWindow::DisplayImage(Mat &image, AspectRatioLabel *viewer)
{
    Mat converted;
    cvtColor(image, converted, COLOR_BGR2RGB);

    QImage qImage = QImage((uchar*) converted.data, converted.cols, converted.rows, converted.step, QImage::Format_RGB888);
    viewer->setPixmapWithAspectRatio(QPixmap::fromImage(qImage));
}

void MainWindow::OnNewFrame()
{
    Mat frame;
    camera->AcquireNextFrame(frame);
    DisplayImage(frame, rawImageViewer);

    Mat processed;
    processor->ProcessImage(frame, processed);
    DisplayImage(processed, processedImageViewer);
}

void MainWindow::OnStartCameraClicked()
{
    camera = new WebCam(0);
    timer.start(20);
    UpdateUiState();
}

void MainWindow::OnStopCameraClicked()
{
    timer.stop();
    delete camera;
    camera = NULL;
    UpdateUiState();
}

void MainWindow::OnConnectArduinoClicked()
{
    string portName = ui->cmbSerialPort->currentText().toStdString();
    controller = new Controller(portName);
    controller->Connect();
    UpdateUiState();
}

void MainWindow::OnDisconnectArduinoClicked()
{
    controller->Disconnect();
    delete controller;
    controller = NULL;
    UpdateUiState();
}

