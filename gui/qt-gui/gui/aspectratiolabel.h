#pragma once

#include <QLabel>
#include <opencv2/core.hpp>

using namespace cv;

class AspectRatioLabel : public QLabel
{
public:
    explicit AspectRatioLabel(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~AspectRatioLabel();

public slots:
    void setOpencvImage(const Mat &image);
    void setPixmapWithAspectRatio(const QPixmap& pm);

private:
    double aspectRatio;
    QPixmap originalPixmap;

    void updatePixmap();
    double computeAspectRatio(int w, int h);
};
