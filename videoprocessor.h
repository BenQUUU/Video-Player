#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <QObject>
#include <QWidget>
#include <QtGui>
#include <QtWidgets>
#include "opencv2/opencv.hpp"
#include <vector>

class VideoProcessor
{
public:
    VideoProcessor();
    void processHumanDetection(const QString& inputFileName, const QString& outputFileName);
    void processMotionDetection(const QString& inputFileName, const QString& outputFileName);
};

#endif // VIDEOPROCESSOR_H
