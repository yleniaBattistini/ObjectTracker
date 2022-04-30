#include "gui/mainwindow.h"

#include <QApplication>

#include <model/webcam.h>

#include <processor/testprocessor.h>
#include <processor/utils/computepose.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(new TestProcessor(), new FaceDetection(), new HoughTransform(), new ComputePose());
    w.show();
    return a.exec();
}
