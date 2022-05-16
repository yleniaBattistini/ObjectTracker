#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"
#include "displayutils.h"
#include "pixmaputils.h"
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <QFileDialog>
#include <QDir>
#include <opencv2/imgcodecs.hpp>

#define INITIAL_SQUARE_SIZE 50.0

CalibrationDialog::CalibrationDialog(Camera *camera, PoseController *poseController, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationDialog),
    camera(camera),
    calibrationProcess(CalibrationProcess(INITIAL_SQUARE_SIZE)),
    poseController(poseController)
{
    ui->setupUi(this);
    timer.start(50);
    display = setupAsDisplay(ui->grpCameraView);

    calibrationViewsModel = new QStandardItemModel();
    ui->lstCalibrationViews->setModel(calibrationViewsModel);

    ui->spnSquareSize->setValue(INITIAL_SQUARE_SIZE);

    connect(&timer, SIGNAL(timeout()), this, SLOT(onNewFrame()));
    connect(ui->btnAddView, SIGNAL(clicked()), this, SLOT(onAddViewClicked()));
    connect(ui->btnRemoveView, SIGNAL(clicked()), this, SLOT(onRemoveViewClicked()));
    connect(ui->btnRunCalibration, SIGNAL(clicked()), this, SLOT(onRunCalibrationClicked()));
    connect(ui->spnSquareSize, SIGNAL(valueChanged(double)), this, SLOT(onSquareSizeChanged(double)));
    connect(ui->btnSave, SIGNAL(clicked()), this, SLOT(onSaveClicked()));
    connect(ui->btnSaveAs, SIGNAL(clicked()), this, SLOT(onSaveAsClicked()));
    connect(ui->btnOpen, SIGNAL(clicked()), this, SLOT(onOpenClicked()));
}

CalibrationDialog::~CalibrationDialog()
{
    delete ui;
    delete display;
}

void CalibrationDialog::recomputeCalibration()
{
    calibrationProcess.recomputeCalibration();
    int rows = calibrationViewsModel->rowCount();
    for (int i = 0; i < rows; i++)
    {
        QString reprojectionErrorText = QString("Reprojection Error: %1").arg(calibrationProcess.getReprojectionError(i));
        QStandardItem *item = calibrationViewsModel->item(i);
        item->setText(reprojectionErrorText);
    }
}

QString CalibrationDialog::selectNewFolder()
{
    QString oldPath = ui->lblFolderPath->text();
    QString selectedPath = QFileDialog::getExistingDirectory(this, "Select a folder", oldPath);
    if (!selectedPath.isNull())
    {
        setCurrentFolderPath(selectedPath);
    }
    return selectedPath;
}

void CalibrationDialog::setCurrentFolderPath(QString path)
{
    ui->lblFolderPath->setText(path);
    ui->btnSave->setEnabled(!path.isEmpty());
}

void CalibrationDialog::addView(Mat &view, vector<Point2f> &corners)
{
    calibrationProcess.addView(view, corners);
    views.push_back(view);
    QPixmap pixmap;
    pixmapFromOpencvImage(view, pixmap);
    QIcon icon(pixmap);
    calibrationViewsModel->appendRow(new QStandardItem(icon, ""));
}

void CalibrationDialog::removeView(int index)
{
    calibrationProcess.removeView(index);
    views.erase(views.begin() + index);
    calibrationViewsModel->removeRow(index);
}

void CalibrationDialog::saveInFolder(QString folderName)
{
    QDir dir = QDir(folderName);
    QString calibrationFile = dir.filePath(DATA_FILE_NAME);
    FileStorage fs(calibrationFile.toStdString(), FileStorage::WRITE);
    fs << "SquareSize" << (double) calibrationProcess.getSquareSize();
    fs << "NumberOfViews" << (int) views.size();

    if (dir.exists(VIEWS_FOLDER_NAME))
    {
        dir.cd(VIEWS_FOLDER_NAME);
        dir.removeRecursively();
        dir.cdUp();
    }
    dir.mkdir(VIEWS_FOLDER_NAME);
    dir.cd(VIEWS_FOLDER_NAME);
    for (int i = 0; i < views.size(); i++)
    {
        QString viewFile = QString("%1.%2").arg(i).arg(IMAGE_FILES_EXTENSION);
        QString filePath = dir.filePath(viewFile);
        imwrite(filePath.toStdString(), views[i]);
    }
}

void CalibrationDialog::openFolder(QString folderName)
{
    QDir dir = QDir(folderName);
    QString calibrationFile = dir.filePath(DATA_FILE_NAME);
    FileStorage fs(calibrationFile.toStdString(), FileStorage::READ);

    int numberOfViews;
    double squareSize;
    fs["SquareSize"] >> squareSize;
    fs["NumberOfViews"] >> numberOfViews;

    calibrationProcess.clearViews();
    calibrationViewsModel->clear();
    views.clear();

    calibrationProcess.setSquareSize(squareSize);
    ui->spnSquareSize->setValue(squareSize);

    dir.cd(VIEWS_FOLDER_NAME);
    for (int i = 0; i < numberOfViews; i++)
    {
        QString viewFile = QString("%1.%2").arg(i).arg(IMAGE_FILES_EXTENSION);
        QString filePath = dir.filePath(viewFile);
        Mat view = imread(filePath.toStdString());
        vector<Point2f> corners;
        calibrationProcess.detectPattern(view, corners);
        addView(view, corners);
    }
    recomputeCalibration();
}

void CalibrationDialog::onNewFrame()
{
    camera->acquireNextFrame(currentFrame);
    currentCorners.clear();
    patternFoundOnCurrentFrame = calibrationProcess.detectPattern(currentFrame, currentCorners);
    if (patternFoundOnCurrentFrame)
    {
        Mat frameWithPattern = currentFrame.clone();
        calibrationProcess.drawPattern(frameWithPattern, currentCorners);
        display->setOpencvImage(frameWithPattern);
    }
    else
    {
        display->setOpencvImage(currentFrame);
    }
}

void CalibrationDialog::onSaveClicked()
{
    saveInFolder(ui->lblFolderPath->text());
}

void CalibrationDialog::onSaveAsClicked()
{
    QString selectedPath = selectNewFolder();
    if (selectedPath.isNull())
    {
        return;
    }
    saveInFolder(selectedPath);
}

void CalibrationDialog::onOpenClicked()
{
    QString selectedPath = selectNewFolder();
    if (selectedPath.isNull())
    {
        return;
    }
    openFolder(selectedPath);
}

void CalibrationDialog::onAddViewClicked()
{
    if (!patternFoundOnCurrentFrame)
    {
        return;
    }
    Mat frameClone = currentFrame.clone();
    addView(frameClone, currentCorners);
    recomputeCalibration();
}

void CalibrationDialog::onRemoveViewClicked()
{
    if (!ui->lstCalibrationViews->selectionModel()->hasSelection())
    {
        return;
    }

    int selectedIndex = ui->lstCalibrationViews->selectionModel()->currentIndex().row();
    removeView(selectedIndex);
    recomputeCalibration();
}

void CalibrationDialog::onRunCalibrationClicked()
{
    Mat cameraMatrix;
    Mat distortionCoefficients;
    calibrationProcess.getCalibrationResult(cameraMatrix, distortionCoefficients);
    camera->calibrate(cameraMatrix, distortionCoefficients);
    poseController->setCalibration(cameraMatrix, distortionCoefficients);
    close();
}

void CalibrationDialog::onSquareSizeChanged(double newValue)
{
    calibrationProcess.setSquareSize(newValue);
    calibrationProcess.recomputeCalibration();
    recomputeCalibration();
}
