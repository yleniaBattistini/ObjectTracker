#include "pixmaputils.h"

#include <opencv2/imgproc/imgproc.hpp>

void pixmapFromOpencvImage(const Mat &image, QPixmap &pixmap)
{
    Mat converted;
    cvtColor(image, converted, COLOR_BGR2RGB);

    QImage qImage(converted.data, converted.cols, converted.rows, converted.step, QImage::Format_RGB888);
    pixmap = QPixmap::fromImage(qImage);
}
