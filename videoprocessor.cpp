#include "videoprocessor.h"

VideoProcessor::VideoProcessor() {}

void VideoProcessor::processVideo(const QString& inputFileName, const QString& outputFileName) {
    // Inicjalizacja deskryptora HOG i detektora ludzkich sylwetek
    cv::HOGDescriptor hog;
    hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

    // Otwarcie pliku wideo
    cv::VideoCapture cap(inputFileName.toStdString());
    if (!cap.isOpened()) {
        std::cerr << "Error: Failed to open video file." << std::endl;
        return;
    }

    // Utworzenie obiektu do zapisu wideo
    cv::VideoWriter out(outputFileName.toStdString(), cv::VideoWriter::fourcc('m','p','4','v'), 30.0, cv::Size(320, 240));

    cv::Mat frame;
    while (true) {
        // Odczyt klatki z pliku wideo
        cap.read(frame);
        if (frame.empty())
            break;

        // Zmniejszenie rozmiaru klatki dla szybszej detekcji
        cv::resize(frame, frame, cv::Size(320, 240));

        // Konwersja do obrazu w odcieniach szarości
        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        // Detekcja ludzkich sylwetek
        std::vector<cv::Rect> found;
        hog.detectMultiScale(frame, found, 0, cv::Size(8,8), cv::Size(32,32), 1.05, 2);

        // Zapis klatki do pliku wideo tylko, gdy wykryto ruch
        if (!found.empty())
            out.write(frame);
    }

    // Zwolnienie zasobów
    cap.release();
    out.release();

    QMessageBox::information(nullptr, "Info", "Human detection completed. Saved as: " + outputFileName);
}
