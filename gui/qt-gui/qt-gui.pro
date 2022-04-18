QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gui/aspectratiolabel.cpp            \
    gui/mainwindow.cpp                  \
    model/camera.cpp                    \
    model/controller.cpp                \
    model/webcam.cpp                    \
    processor/facedetection.cpp \
    processor/houghtransform.cpp \
    processor/imageprocessor.cpp        \
    processor/testprocessor.cpp         \
    processor/utils/drawelement.cpp \
    processor/utils/geometrictransformation.cpp \
    serial/console.cpp                  \
    serial/serialport.cpp               \
    serial/serialportshelper.cpp        \
    main.cpp

HEADERS += \
    gui/aspectratiolabel.h              \
    gui/mainwindow.h                    \
    model/camera.h                      \
    model/controller.h                  \
    model/webcam.h                      \
    processor/facedetection.h \
    processor/houghtransform.h \
    processor/imageprocessor.h          \
    processor/testprocessor.h           \
    processor/utils/drawelement.h \
    processor/utils/geometrictransformation.h \
    serial/console.h                    \
    serial/serialport.h                 \
    serial/serialportshelper.h

FORMS += \
    gui/mainwindow.ui

INCLUDEPATH += $$(OPENCV_DIR)/include

CONFIG(debug, debug|release): OPENCV_VERSION = 453d
CONFIG(release, debug|release): OPENCV_VERSION = 453

LIBS += -L$$(OPENCV_DIR)/x64/vc16/lib         \
        -lopencv_core$${OPENCV_VERSION}       \
        -lopencv_highgui$${OPENCV_VERSION}    \
        -lopencv_imgcodecs$${OPENCV_VERSION}  \
        -lopencv_imgproc$${OPENCV_VERSION}    \
        -lopencv_features2d$${OPENCV_VERSION} \
        -lopencv_calib3d$${OPENCV_VERSION}    \
        -lopencv_video$${OPENCV_VERSION}      \
        -lopencv_videoio$${OPENCV_VERSION}    \
        -lopencv_photo$${OPENCV_VERSION}      \
        -lopencv_objdetect$${OPENCV_VERSION}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    haarcascade_frontalface_alt.xml \
    haarcascade_frontalface_default.xml