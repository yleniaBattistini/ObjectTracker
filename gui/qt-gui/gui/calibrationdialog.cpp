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
    camera(camera),
    calibrationProcess(CalibrationProcess(camera))
{
    ui->setupUi(this);
    timer.start(20);
    display = setupAsDisplay(ui->grpCameraView);

    calibrationFramesModel = new QStandardItemModel();
    ui->lstCalibrationFrames->setModel(calibrationFramesModel);

    connect(&timer, SIGNAL(timeout()), this, SLOT(onNewFrame()));
    connect(ui->btnAddFrame, SIGNAL(clicked()), this, SLOT(onAddFrameClicked()));
    connect(ui->btnRemoveFrame, SIGNAL(clicked()), this, SLOT(onRemoveFrameClicked()));
    connect(ui->btnRunCalibration, SIGNAL(clicked()), this, SLOT(onRunCalibrationClicked()));
}

CalibrationDialog::~CalibrationDialog()
{
    delete ui;
    delete display;
}

void CalibrationDialog::onNewFrame()
{
    camera->acquireNextFrame(currentFrame, true);
    currentFrameData.clear();
    patternFoundOnCurrentFrame = calibrationProcess.detectPattern(currentFrame, currentFrameData);
    if (patternFoundOnCurrentFrame)
    {
        calibrationProcess.drawPattern(currentFrame, currentFrameData);
    }
    display->setOpencvImage(currentFrame);
}

void CalibrationDialog::onAddFrameClicked()
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

void CalibrationDialog::onRemoveFrameClicked()
{
    if (!ui->lstCalibrationFrames->selectionModel()->hasSelection())
    {
        return;
    }

    int selectedIndex = ui->lstCalibrationFrames->selectionModel()->currentIndex().row();
    calibrationProcess.removeFrame(selectedIndex);
    calibrationFramesModel->removeRow(selectedIndex);
}

void CalibrationDialog::onRunCalibrationClicked()
{
    calibrationProcess.runCalibration(currentFrame.size(), ui->spnSquareSize->value());
    close();
}
