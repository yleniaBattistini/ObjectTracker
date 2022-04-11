#pragma once

#include "aspectratiolabel.h"

#include <QDialog>
#include <QStandardItemModel>
#include <QTimer>

#include "model/camera.h"

namespace Ui {
class CalibrationDialog;
}

class CalibrationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrationDialog(Camera *camera, QWidget *parent = nullptr);
    ~CalibrationDialog();

private:
    Ui::CalibrationDialog *ui;
    QTimer timer;
    Camera *camera;
    Mat currentFrame;
    QStandardItemModel *calibrationFramesModel;

    AspectRatioLabel *display;

private slots:
    void OnNewFrame();
    void OnAddFrameClicked();
    void OnRemoveFrameClicked();
    void OnRunCalibrationClicked();
};
