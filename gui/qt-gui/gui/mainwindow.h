#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

    AspectRatioLabel *rawImageViewer;
    AspectRatioLabel *processedImageViewer;

    void UpdateUiState();
    AspectRatioLabel *SetupAsDisplay(QWidget *widget);

    void DisplayImage(Mat &image, AspectRatioLabel *viewer);
private slots:
    void OnNewFrame();

    void OnStartCameraClicked();
    void OnStopCameraClicked();
    void OnConnectArduinoClicked();
    void OnDisconnectArduinoClicked();
};
#endif // MAINWINDOW_H
