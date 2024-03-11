#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("VIDEO PLAYER");

    Player = new QMediaPlayer();
    Audio = new QAudioOutput();

    Player->setAudioOutput(Audio);

    ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pushButton_Stop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->pushButton_Seek_Backward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->pushButton_Seek_Forward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

    ui->horizontalSlider_Volume->setMinimum(0);
    ui->horizontalSlider_Volume->setMaximum(100);
    ui->horizontalSlider_Volume->setValue(30);


    Audio->setVolume(ui->horizontalSlider_Volume->value());

    connect(Player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);
    connect(Player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);

    ui->horizontalSlider_Duration->setRange(0, Player->duration() / 1000);
}


MainWindow::~MainWindow()
{
    delete ui;
    delete Player;
    delete Audio;
    delete Video;
}


void MainWindow::durationChanged(qint64 duration)
{
    mDuration = duration / 1000;
    ui->horizontalSlider_Duration->setMaximum(mDuration);
}


void MainWindow::positionChanged(qint64 duration)
{
    if(!ui->horizontalSlider_Duration->isSliderDown()){
        ui->horizontalSlider_Duration->setValue(duration / 1000);
    }

    updateDuration(duration / 1000);
}


void MainWindow::updateDuration(qint64 Duration)
{
    if(Duration || mDuration){
        QTime CurrentTime((Duration / 3600) % 60, (Duration / 60) % 60, Duration % 60, (Duration * 1000) % 1000);
        QTime TotalTime((mDuration / 3600) % 60, (mDuration / 60) % 60, mDuration % 60, (mDuration * 1000) % 1000);
        QString Format = "";

        if(mDuration > 3600){
            Format = "hh:mm:ss";
        } else {
            Format = "mm:ss";
        }

        ui->label_Current_Time->setText(CurrentTime.toString(Format));
        ui->label_Total_Time->setText(TotalTime.toString(Format));
    }
}


void MainWindow::on_actionOpen_triggered()
{
    QString FileName = QFileDialog::getOpenFileName(this, tr("Select Video File"), "", tr("MP4 Files (*.mp4)"));
    Video = new QVideoWidget();

    Video->setGeometry(5, 5, ui->groupBox_Video->width() - 10, ui->groupBox_Video->height() - 10);

    Video->setParent(ui->groupBox_Video);

    Player->setVideoOutput(Video);

    Player->setSource(QUrl(FileName));

    Video->setVisible(true);

    Video->show();
}


void MainWindow::on_horizontalSlider_Duration_valueChanged(int value)
{
    Player->setPosition(value * 1000);
}


void MainWindow::on_pushButton_Play_Pause_clicked()
{
    if(IS_Pause){
        IS_Pause = false;
        Player->play();
        ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    } else {
        IS_Pause = true;
        Player->pause();
        ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}


void MainWindow::on_pushButton_Stop_clicked()
{
    Player->stop();
}


void MainWindow::on_pushButton_Volume_clicked()
{
    if(!IS_Muted){
        IS_Muted = true;
        ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
        Audio->setMuted(true);
    } else {
        IS_Muted = false;
        ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
        Audio->setMuted(false);
    }
}


void MainWindow::on_horizontalSlider_Volume_valueChanged(int value)
{
    Audio->setVolume(value);
}


void MainWindow::on_pushButton_Seek_Backward_clicked()
{
    ui->horizontalSlider_Duration->setValue(ui->horizontalSlider_Duration->value() - 20);
    Player->setPosition(ui->horizontalSlider_Duration->value() * 1000);
}


void MainWindow::on_pushButton_Seek_Forward_clicked()
{
    ui->horizontalSlider_Duration->setValue(ui->horizontalSlider_Duration->value() + 20);
}


void MainWindow::on_actionHuman_detection_triggered()
{
    QMessageBox processingMsg(QMessageBox::Information, "Info", "Processing video. Please wait...", QMessageBox::Cancel, this);
    processingMsg.setModal(true);
    processingMsg.show();


    // Pozwól użytkownikowi wybrać plik wideo
    QString fileName = QFileDialog::getOpenFileName(this, "Select Video File", "", "MP4 Files (*.mp4)");
    if (fileName.isEmpty())
        return; // Użytkownik anulował wybór pliku

    // Wykonaj detekcję ruchu i zapisz do nowego pliku MP4
    QString outputFileName = QFileDialog::getSaveFileName(this, "Save Video", "", "MP4 Files (*.mp4)");
    if (outputFileName.isEmpty())
        return;

    // Otwórz plik wideo
    cv::VideoCapture cap(fileName.toStdString());
    if (!cap.isOpened()) {
        QMessageBox::critical(this, "Error", "Failed to open video file.");
        return;
    }

    cv::Mat frame;
    cv::Ptr<cv::BackgroundSubtractorMOG2> bg_subtractor = cv::createBackgroundSubtractorMOG2();
    cv::VideoWriter output(outputFileName.toStdString(), cv::VideoWriter::fourcc('m','p','4','v'), 30, cv::Size((int)cap.get(cv::CAP_PROP_FRAME_WIDTH),(int)cap.get(cv::CAP_PROP_FRAME_HEIGHT)));

    while (cap.read(frame)) {
        // Wykonaj detekcję ruchu
        cv::Mat fg_mask;
        bg_subtractor->apply(frame, fg_mask);

        // Znajdź kontury
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(fg_mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // Sprawdź, czy są jakiekolwiek kontury (ruch)
        if (!contours.empty()) {
            // Zapisz ramkę z ruchem do nowego pliku MP4
            output.write(frame);
        }
    }

    output.release(); // Zwolnij zasoby
    processingMsg.close(); // Zamknij okno dialogowe o przetwarzaniu

    QMessageBox::information(this, "Info", "Human detection completed. Saved as: " + outputFileName);
}


