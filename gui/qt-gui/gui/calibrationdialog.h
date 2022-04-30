#pragma once

#include "aspectratiolabel.h"

#include <QDialog>
#include <QStandardItemModel>
#include <QTimer>

#include "model/camera.h"

#include <calibration/calibrationprocess.h>
#include "processor/utils/computepose.h"

using namespace cv;
using namespace std;

namespace Ui {
class CalibrationDialog;
}

class CalibrationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrationDialog(Camera *camera, ComputePose *computePose, QWidget *parent = nullptr);
    ~CalibrationDialog();

private:
    const QString VIEWS_FOLDER_NAME = "views";
    const QString DATA_FILE_NAME = "data.xml";
    const QString IMAGE_FILES_EXTENSION = "png";

    Ui::CalibrationDialog *ui;
    QTimer timer;
    Camera *camera;
    vector<Mat> views;
    Mat currentFrame;
    vector<Point2f> currentCorners;
    bool patternFoundOnCurrentFrame;
    CalibrationProcess calibrationProcess;
    QStandardItemModel *calibrationViewsModel;
    AspectRatioLabel *display;
    ComputePose *computePose;

    void recomputeCalibration();
    QString selectNewFolder();
    void setCurrentFolderPath(QString path);

    void addView(Mat &view, vector<Point2f> &corners);
    void removeView(int index);

    void saveInFolder(QString folderName);
    void openFolder(QString folderName);

private slots:
    void onNewFrame();
    void onSaveClicked();
    void onSaveAsClicked();
    void onOpenClicked();
    void onAddViewClicked();
    void onRemoveViewClicked();
    void onRunCalibrationClicked();
    void onSquareSizeChanged(double newValue);
};
