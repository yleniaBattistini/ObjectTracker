#include "displayutilities.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <QPixmap>

void DisplayUtilities::DisplayImage(Mat &image, QLabel *display)
{
    Mat converted;
    cvtColor(image, converted, COLOR_BGR2RGB);

    QImage qImage = QImage((uchar*) converted.data, converted.cols, converted.rows, converted.step, QImage::Format_RGB888);
    display->setPixmap(QPixmap::fromImage(qImage));
}
