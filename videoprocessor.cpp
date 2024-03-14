#include "videoprocessor.h"

/**
 * @brief Constructs a new VideoProcessor object.
 */
VideoProcessor::VideoProcessor() {}


/**
 * @brief Performs human detection on a video file.
 *
 * This method detects humans in a video file using the HOG (Histogram of Oriented Gradients) descriptor.
 * Detected humans are saved to a new video file.
 *
 * @param inputFileName Path to the input video file.
 * @param outputFileName Path to save the output video file with detected humans.
 */
void VideoProcessor::processHumanDetection(const QString& inputFileName, const QString& outputFileName)
{
    cv::HOGDescriptor hog;
    hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

    cv::VideoCapture cap(inputFileName.toStdString());

    cv::VideoWriter out(outputFileName.toStdString(), cv::VideoWriter::fourcc('m','p','4','v'), 30.0, cv::Size(320, 240));

    cv::Mat frame;
    while (true) {
        cap.read(frame);
        if (frame.empty()){
            break;
        }

        cv::resize(frame, frame, cv::Size(320, 240));

        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        std::vector<cv::Rect> found;
        hog.detectMultiScale(frame, found, 0, cv::Size(8,8), cv::Size(32,32), 1.05, 2);

        if (!found.empty()) {
            out.write(frame);
        }
    }

    cap.release();
    out.release();

    QMessageBox::information(nullptr, "Info", "Human detection completed. Saved as: " + outputFileName);
}


/**
 * @brief Performs motion detection on a video file.
 *
 * This method detects motion in a video file by comparing consecutive frames.
 * Frames with detected motion are saved to a new video file.
 *
 * @param inputFileName Path to the input video file.
 * @param outputFileName Path to save the output video file with detected motion.
 */
void VideoProcessor::processMotionDetection(const QString& inputFileName, const QString& outputFileName)
{
    cv::VideoCapture cap(inputFileName.toStdString());
    cv::VideoWriter out(outputFileName.toStdString(), cv::VideoWriter::fourcc('m','p','4','v'), 30.0, cv::Size(320, 240));

    if (!cap.isOpened()) {
        QMessageBox::critical(nullptr, "Error", "Failed to open input video file.");
        return;
    }

    cv::Mat frame, prevFrame, diffFrame, gray;
    bool isFirstFrame = true;

    while (true) {
        cap.read(frame);
        if (frame.empty()) {
            break;
        }

        cv::resize(frame, frame, cv::Size(320, 240));
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        if (!isFirstFrame) {
            cv::absdiff(gray, prevFrame, diffFrame);
            cv::threshold(diffFrame, diffFrame, 30, 255, cv::THRESH_BINARY);

            if (cv::countNonZero(diffFrame) > 0) {
                out.write(frame);
            }
        }

        prevFrame = gray.clone();
        isFirstFrame = false;
    }

    cap.release();
    out.release();

    QMessageBox::information(nullptr, "Info", "Motion detection completed. Saved as: " + outputFileName);
}
