#include "videoprocessor.h"

VideoProcessor::VideoProcessor() {}

void VideoProcessor::processVideo(const QString& inputFileName, const QString& outputFileName) {
    cv::HOGDescriptor hog;
    hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

    cv::VideoCapture cap(inputFileName.toStdString());

    cv::VideoWriter out(outputFileName.toStdString(), cv::VideoWriter::fourcc('m','p','4','v'), 30.0, cv::Size(320, 240));

    cv::Mat frame;
    while (true) {
        cap.read(frame);
        if (frame.empty())
            break;

        cv::resize(frame, frame, cv::Size(320, 240));

        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        std::vector<cv::Rect> found;
        hog.detectMultiScale(frame, found, 0, cv::Size(8,8), cv::Size(32,32), 1.05, 2);

        if (!found.empty())
            out.write(frame);
    }

    cap.release();
    out.release();

    QMessageBox::information(nullptr, "Info", "Human detection completed. Saved as: " + outputFileName);
}
