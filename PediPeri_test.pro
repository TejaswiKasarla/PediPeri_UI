#-------------------------------------------------
#
# Project created by QtCreator 2015-09-16T10:22:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PediPeri_test
TEMPLATE = app


SOURCES += main.cpp\
        pediperi.cpp \
    mywindow.cpp


HEADERS  += pediperi.h \
    mywindow.h


FORMS    += pediperi.ui \
    mywindow.ui
INCLUDEPATH += C:/opencv/sources/build_mingw/install/include
LIBS += -L"C:/opencv/sources/build_mingw/install/x64/mingw/bin"
LIBS += -llibopencv_calib3d2411 -llibopencv_contrib2411 -llibopencv_core2411 -llibopencv_features2d2411 -llibopencv_gpu2411 -llibopencv_highgui2411 -llibopencv_imgproc2411 -llibopencv_legacy2411 -llibopencv_photo2411 -llibopencv_video2411 -llibopencv_videostab2411 -lopencv_ffmpeg2411_64

QT += serialport

RESOURCES += \
    resource.qrc


