#include "gui/mainwindow.h"

#include <QApplication>

#include <model/webcam.h>

#include <processor/testprocessor.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(new TestProcessor());
    w.show();
    return a.exec();
}
