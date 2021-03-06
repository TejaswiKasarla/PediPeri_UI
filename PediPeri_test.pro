#-------------------------------------------------
#
# Project created by QtCreator 2015-09-16T10:22:12
#
#-------------------------------------------------

QT       += core gui
CONFIG += boost

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

INCLUDEPATH += C:/Boost/include/boost-1_59
LIBS += C:/Boost/lib/libboost_system-mgw51-mt-1_59.dll
LIBS += C:/Boost/lib/libboost_thread-mgw51-mt-1_59.dll

INCLUDEPATH += C:/opencv/sources/build_mingw/install/include
LIBS += -L"C:/opencv/sources/build_mingw/install/x64/mingw/bin"
LIBS +=  -llibopencv_core2411 -llibopencv_highgui2411 -llibopencv_imgproc2411



QT += serialport

RESOURCES += \
    resource.qrc


