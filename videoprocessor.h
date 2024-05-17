#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <QObject>
#include <QWidget>
#include <QtGui>
#include <QtWidgets>
#include "opencv2/opencv.hpp"
#include <vector>

class VideoProcessor {
public:
    VideoProcessor();
    void processHumanDetection(const QString& inputFileName, const QString& outputFileName);
    void processMotionDetection(const QString& inputFileName, const QString& outputFileName);
private:
    void mergeAudioVideo_(const QString& inputFileName, const QString& outputFileName);
    double getVideoDuration(const QString& fileName);
};

#endif // VIDEOPROCESSOR_H
