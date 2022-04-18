#pragma once

#include <QPixmap>
#include <opencv2/core.hpp>

using namespace cv;

void pixmapFromOpencvImage(const Mat &image, QPixmap &pixmap);
