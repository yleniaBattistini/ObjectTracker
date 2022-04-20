#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"
#include "displayutils.h"
#include "pixmaputils.h"
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <format>

#define INITIAL_SQUARE_SIZE 50.0

CalibrationDialog::CalibrationDialog(Camera *camera, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationDialog),
    camera(camera),
    calibrationProcess(CalibrationProcess(INITIAL_SQUARE_SIZE))
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

void CalibrationDialog::updateReprojectionErrorLabels()
{
    int rows = calibrationFramesModel->rowCount();
    for (int i = 0; i < rows; i++)
    {
        QString reprojectionErrorText = QString("Reprojection Error: %1").arg(calibrationProcess.getReprojectionError(i));
        QStandardItem *item = calibrationFramesModel->item(i);
        item->setText(reprojectionErrorText);
    }
}

void CalibrationDialog::onNewFrame()
{
    camera->acquireNextFrame(currentFrame, true);
    currentCorners.clear();
    patternFoundOnCurrentFrame = calibrationProcess.detectPattern(currentFrame, currentCorners);
    if (patternFoundOnCurrentFrame)
    {
        calibrationProcess.drawPattern(currentFrame, currentCorners);
    }
    display->setOpencvImage(currentFrame);
}

void CalibrationDialog::onAddFrameClicked()
{
    if (!patternFoundOnCurrentFrame)
    {
        return;
    }
    calibrationProcess.addView(currentFrame, currentCorners);
    QPixmap pixmap;
    pixmapFromOpencvImage(currentFrame, pixmap);
    QIcon icon(pixmap);
    calibrationFramesModel->appendRow(new QStandardItem(icon, ""));
    updateReprojectionErrorLabels();
}

void CalibrationDialog::onRemoveFrameClicked()
{
    if (!ui->lstCalibrationFrames->selectionModel()->hasSelection())
    {
        return;
    }

    int selectedIndex = ui->lstCalibrationFrames->selectionModel()->currentIndex().row();
    calibrationProcess.removeView(selectedIndex);
    calibrationFramesModel->removeRow(selectedIndex);
    updateReprojectionErrorLabels();
}

void CalibrationDialog::onRunCalibrationClicked()
{
    calibrationProcess.applyCalibration(camera);
    close();
}
