#include "aspectratiolabel.h"
#include "pixmaputils.h"

AspectRatioLabel::AspectRatioLabel(QWidget* parent, Qt::WindowFlags f) : QLabel(parent, f),
    originalPixmap(QPixmap())
{
}

AspectRatioLabel::~AspectRatioLabel()
{
}

void AspectRatioLabel::setOpencvImage(const Mat& image)
{
    QPixmap pixmap;
    pixmapFromOpencvImage(image, pixmap);
    setPixmapWithAspectRatio(pixmap);
}

void AspectRatioLabel::setPixmapWithAspectRatio(const QPixmap& pm)
{
    aspectRatio = computeAspectRatio(pm.width(), pm.height());
    originalPixmap = pm.copy();

    updatePixmap();
}

void AspectRatioLabel::updatePixmap()
{
    if (originalPixmap.isNull())
    {
        return;
    }

    int labelWidth = width();
    int labelHeight = height();
    int targetWidth = labelWidth;
    int targetHeight = labelHeight;

    if (aspectRatio > computeAspectRatio(labelWidth, labelHeight))
    {
        targetHeight = (int)(labelWidth / aspectRatio);
    }
    else
    {
        targetWidth = (int)(labelHeight * aspectRatio);
    }

    setPixmap(originalPixmap.scaled(targetWidth, targetHeight));

    int hMargin = (labelWidth - targetWidth) / 2;
    int vMargin = (labelHeight - targetHeight) / 2;
    setContentsMargins(hMargin, vMargin, hMargin, vMargin);
}

double AspectRatioLabel::computeAspectRatio(int w, int h)
{
    return (double)w / h;
}
