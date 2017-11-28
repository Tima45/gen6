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
    initPlot();
    gameThread->start();
    QReadWriteLock locker;
    locker.unlock();
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

}

void MainWindow::on_startStopButton_clicked()
{
    QReadWriteLock lock;
    lock.lockForWrite();
    if(!game->isPlaying){
        game->isPlaying = true;
        emit startGame();
    }else{
        game->isPlaying = false;
    }
    lock.unlock();
}
