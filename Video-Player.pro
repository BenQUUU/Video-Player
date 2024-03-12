QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

INCLUDEPATH += /usr/include/opencv4
LIBS += -lopencv_core \
-lopencv_imgproc \
-lopencv_highgui \
-lopencv_videoio \
-lopencv_video \
-lopencv_imgcodecs \
-lopencv_objdetect \
-lopencv_features2d \
-lopencv_imgproc \
-lopencv_calib3d \
-lopencv_dnn


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    videoprocessor.cpp

HEADERS += \
    mainwindow.h \
    videoprocessor.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
