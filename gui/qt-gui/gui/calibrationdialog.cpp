#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"
#include "displayutils.h"
#include "pixmaputils.h"
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

CalibrationDialog::CalibrationDialog(Camera *camera, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationDialog),
    camera(camera)
{
    ui->setupUi(this);
    timer.start(20);
    display = setupAsDisplay(ui->grpCameraView);

    calibrationFramesModel = new QStandardItemModel();
    ui->lstCalibrationFrames->setModel(calibrationFramesModel);

    connect(&timer, SIGNAL(timeout()), this, SLOT(OnNewFrame()));
    connect(ui->btnAddFrame, SIGNAL(clicked()), this, SLOT(OnAddFrameClicked()));
    connect(ui->btnRemoveFrame, SIGNAL(clicked()), this, SLOT(OnRemoveFrameClicked()));
    connect(ui->btnRunCalibration, SIGNAL(clicked()), this, SLOT(OnRunCalibrationClicked()));
}

CalibrationDialog::~CalibrationDialog()
{
    delete ui;
    delete display;
}

void CalibrationDialog::readCalibrationResult(Mat &cameraMatrix, Mat &distortionCoefficients)
{
    cameraMatrix = this->cameraMatrix;
    distortionCoefficients = this->distortionCoefficients;
}

void CalibrationDialog::OnNewFrame()
{
    camera->AcquireNextFrame(currentFrame);
    currentFrameData.clear();
    patternFoundOnCurrentFrame = calibrationProcess.detectPattern(currentFrame, currentFrameData);
    if (patternFoundOnCurrentFrame)
    {
        calibrationProcess.drawPattern(currentFrame, currentFrameData);
    }
    display->setOpencvImage(currentFrame);
}

void CalibrationDialog::OnAddFrameClicked()
{
    if (!patternFoundOnCurrentFrame)
    {
        return;
    }
    calibrationProcess.addFrame(currentFrameData);
    QPixmap pixmap;
    pixmapFromOpencvImage(currentFrame, pixmap);
    QIcon icon(pixmap);
    calibrationFramesModel->appendRow(new QStandardItem(icon, "Frame"));
}

void CalibrationDialog::OnRemoveFrameClicked()
{
    if (!ui->lstCalibrationFrames->selectionModel()->hasSelection())
    {
        return;
    }

    int selectedIndex = ui->lstCalibrationFrames->selectionModel()->currentIndex().row();
    calibrationProcess.removeFrame(selectedIndex);
    calibrationFramesModel->removeRow(selectedIndex);
}

void CalibrationDialog::OnRunCalibrationClicked()
{
    calibrationProcess.runCalibration(currentFrame.size(), ui->spnSquareSize->value(), cameraMatrix, distortionCoefficients);
    close();
}
