#include "displayutils.h"

#include <QLayout>

AspectRatioLabel *setupAsDisplay(QWidget *widget)
{
    AspectRatioLabel *viewer = new AspectRatioLabel();
    widget->layout()->addWidget(viewer);
    return viewer;
}
