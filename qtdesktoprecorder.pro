QT       += core gui

win32: QT += winextras
unix:!macx: LIBS += -lX11 -lXext -lXfixes

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

CONFIG += c++11 console

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    MainWindow.h

FORMS += \
    MainWindow.ui

LIBS += $$PWD/ffmpeg/lib/avcodec.lib \
        $$PWD/ffmpeg/lib/avdevice.lib \
        $$PWD/ffmpeg/lib/avfilter.lib \
        $$PWD/ffmpeg/lib/avformat.lib \
        $$PWD/ffmpeg/lib/avutil.lib \
        $$PWD/ffmpeg/lib/postproc.lib \
        $$PWD/ffmpeg/lib/swresample.lib \
        $$PWD/ffmpeg/lib/swscale.lib

INCLUDEPATH += $$PWD/ffmpeg/include
DEPENDPATH += $$PWD/ffmpeg/include

include($$PWD/capture/capture.pri)
include($$PWD/encode/encode.pri)
include($$PWD/muxer/muxer.pri)
include($$PWD/common/common.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
