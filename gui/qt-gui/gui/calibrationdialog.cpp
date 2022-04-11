#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"
#include "displayutils.h"
#include <opencv2/core.hpp>

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

void CalibrationDialog::OnNewFrame()
{
    camera->AcquireNextFrame(currentFrame);
    display->setOpencvImage(currentFrame);
}

void CalibrationDialog::OnAddFrameClicked()
{
    calibrationFramesModel->appendRow(new QStandardItem("Ciao"));
}

void CalibrationDialog::OnRemoveFrameClicked()
{
    if (!ui->lstCalibrationFrames->selectionModel()->hasSelection())
    {
        return;
    }

    int selected = ui->lstCalibrationFrames->selectionModel()->currentIndex().row();



    calibrationFramesModel->removeRow(selected);
}

void CalibrationDialog::OnRunCalibrationClicked()
{

}
