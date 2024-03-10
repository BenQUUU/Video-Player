#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pushButton_Stop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->pushButton_Seek_Backward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->pushButton_Seek_Forward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{

}


void MainWindow::on_horizontalSlider_Duration_valueChanged(int value)
{

}


void MainWindow::on_pushButton_Seek_Backward_toggled(bool checked)
{

}


void MainWindow::on_pushButton_Seek_Forward_toggled(bool checked)
{

}


void MainWindow::on_pushButton_Play_Pause_clicked()
{

}


void MainWindow::on_pushButton_Stop_clicked()
{

}


void MainWindow::on_pushButton_Volume_clicked()
{

}


void MainWindow::on_horizontalSlider_Volume_valueChanged(int value)
{

}

