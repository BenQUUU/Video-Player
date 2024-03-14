#include "mainwindow.h"
#include "ui_mainwindow.h"


/**
 * @brief MainWindow constructor.
 *
 * Initializes the main window UI components and connects signals and slots.
 *
 * @param parent Pointer to the parent widget.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set window title
    this->setWindowTitle("VIDEO PLAYER");

    // Initialize media player and audio output
    Player = new QMediaPlayer();
    Audio = new QAudioOutput();
    Player->setAudioOutput(Audio);

    // Set icons for buttons
    ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pushButton_Stop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->pushButton_Seek_Backward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->pushButton_Seek_Forward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

    // Set volume slider range
    ui->horizontalSlider_Volume->setMinimum(0);
    ui->horizontalSlider_Volume->setMaximum(100);
    ui->horizontalSlider_Volume->setValue(30);
    Audio->setVolume(ui->horizontalSlider_Volume->value());

    // Connect signals and slots
    connect(Player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);
    connect(Player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);

    ui->horizontalSlider_Duration->setRange(0, Player->duration() / 1000);
}

/**
 * @brief MainWindow destructor.
 *
 * Cleans up resources and deletes UI and player objects.
 */
MainWindow::~MainWindow()
{
    delete ui;
    delete Player;
    delete Audio;
    delete Video;
}

/**
 * @brief Updates the maximum value of the duration slider when the duration changes.
 *
 * @param duration The duration of the video in milliseconds.
 */
void MainWindow::durationChanged(qint64 duration)
{
    mDuration = duration / 1000;
    ui->horizontalSlider_Duration->setMaximum(mDuration);
}

/**
 * @brief Updates the position of the duration slider and calls updateDuration function.
 *
 * @param duration The current position of the video in milliseconds.
 */
void MainWindow::positionChanged(qint64 duration)
{
    if(!ui->horizontalSlider_Duration->isSliderDown()){
        ui->horizontalSlider_Duration->setValue(duration / 1000);
    }

    updateDuration(duration / 1000);
}

/**
 * @brief Updates the duration label with the current and total duration.
 *
 * @param Duration The current duration of the video.
 */
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

/**
 * @brief Opens a file dialog to select a video file and sets it to play.
 */
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

/**
 * @brief Sets the position of the video player according to the value of the duration slider.
 *
 * @param value The value of the duration slider.
 */
void MainWindow::on_horizontalSlider_Duration_valueChanged(int value)
{
    Player->setPosition(value * 1000);
}

/**
 * @brief Plays or pauses the video player when the play/pause button is clicked.
 */
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

/**
 * @brief Stops the video player when the stop button is clicked.
 */
void MainWindow::on_pushButton_Stop_clicked()
{
    Player->stop();
}

/**
 * @brief Mutes or unmutes the video player when the volume button is clicked.
 */
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

/**
 * @brief Sets the volume of the video player according to the value of the volume slider.
 *
 * @param value The value of the volume slider.
 */
void MainWindow::on_horizontalSlider_Volume_valueChanged(int value)
{
    Audio->setVolume(value);
}

/**
 * @brief Seeks backward in the video when the seek backward button is clicked.
 */
void MainWindow::on_pushButton_Seek_Backward_clicked()
{
    ui->horizontalSlider_Duration->setValue(ui->horizontalSlider_Duration->value() - 20);
    Player->setPosition(ui->horizontalSlider_Duration->value() * 1000);
}

/**
 * @brief Seeks forward in the video when the seek forward button is clicked.
 */
void MainWindow::on_pushButton_Seek_Forward_clicked()
{
    ui->horizontalSlider_Duration->setValue(ui->horizontalSlider_Duration->value() + 20);
}

/**
 * @brief Executes video processing using a provided function.
 *
 * @param func The function to be executed for video processing.
 */
void MainWindow::performVideoProcessing(std::function<void(VideoProcessor&, const QString&, const QString&)> func){
    QMessageBox processingMsg(QMessageBox::Information, "Info", "Processing video. Please wait...", QMessageBox::Cancel, this);
    processingMsg.setModal(true);
    processingMsg.show();

    QString fileName = QFileDialog::getOpenFileName(this, "Select Video File", "", "MP4 Files (*.mp4)");
    if (fileName.isEmpty()) {
        processingMsg.close();
        return;
    }

    QString outputFileName = QFileDialog::getSaveFileName(this, "Save Video", "", "MP4 Files (*.mp4)");
    if (outputFileName.isEmpty()) {
        processingMsg.close();
        return;
    }

    VideoProcessor videoProcessor;
    func(videoProcessor, fileName, outputFileName);

    processingMsg.close();
}

/**
 * @brief Executes human detection processing for a selected video file.
 */
void MainWindow::on_actionHuman_detection_triggered()
{
    performVideoProcessing([](VideoProcessor& processor, const QString& inputFile, const QString& outputFile) {
        processor.processHumanDetection(inputFile, outputFile);
    });
}

/**
 * @brief Executes motion detection processing for a selected video file.
 */
void MainWindow::on_actionMotion_detection_triggered()
{
    performVideoProcessing([](VideoProcessor& processor, const QString& inputFile, const QString& outputFile) {
        processor.processMotionDetection(inputFile, outputFile);
    });
}

