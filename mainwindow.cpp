#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gameThread = new QThread;
    game = new Game;
    game->moveToThread(gameThread);
    connect(this,SIGNAL(startGame()),game,SLOT(infinitGamePlaying()),Qt::QueuedConnection);
    connect(game,SIGNAL(updateLabels(uint,uint,uint,uint)),this,SLOT(updateLabels(uint,uint,uint,uint)),Qt::QueuedConnection);
    initPlot();
    gameThread->start();

    game->resetWorld();
}

MainWindow::~MainWindow()
{
    delete ui;
    game->deleteLater();
    gameThread->deleteLater();
}

void MainWindow::initPlot()
{

}

void MainWindow::on_newWorldButton_clicked()
{
    locker.lockForWrite();
    game->isPlaying = false;
    locker.unlock();
    ui->startStopButton->setText("Старт");
    game->resetWorld();
}

void MainWindow::on_startStopButton_clicked()
{
    locker.lockForWrite();
    if(!game->isPlaying){
        game->isPlaying = true;
        ui->startStopButton->setText("Стоп");
        emit startGame();
    }else{
        game->isPlaying = false;
        ui->startStopButton->setText("Старт");
    }
    locker.unlock();
}

void MainWindow::updateLabels(uint tern, uint ternDuration, uint alive, uint dead)
{
    ui->ternLabel->setText(QString::number(tern));
    ui->ternsPerSecondLabel->setText(QString::number(qRound(1000.0/ternDuration)));
    ui->aliveCountLabel->setText(QString::number(alive));
    ui->deadCountLabel->setText(QString::number(dead));
}



void MainWindow::on_skipReplotCheck_clicked(bool checked)
{
    locker.lockForWrite();
        game->skipDisplay = checked;
    locker.unlock();
}
