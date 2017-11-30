#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initPlot();

    gameThread = new QThread;
    game = new Game;
    game->colorMap = colorMap;
    game->moveToThread(gameThread);
    connect(this,SIGNAL(startGame()),game,SLOT(infinitGamePlaying()),Qt::QueuedConnection);
    connect(game,SIGNAL(updateLabels(uint,uint,uint,uint)),this,SLOT(updateLabels(uint,uint,uint,uint)),Qt::QueuedConnection);
    qRegisterMetaType<QCustomPlot::RefreshPriority>("QCustomPlot::RefreshPriority");
    connect(game,SIGNAL(emitReplotWorld(QCustomPlot::RefreshPriority)),ui->worldPlot,SLOT(replot(QCustomPlot::RefreshPriority)),Qt::QueuedConnection);
    timer = new QTimer;
    timer->setInterval(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(fps()));
    timer->start();
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
    //ui->worldPlot->setOpenGl(true,1);
    ui->worldPlot->setNoAntialiasingOnDrag(true);
    ui->worldPlot->setInteraction(QCP::iRangeDrag, true);
    ui->worldPlot->setInteraction(QCP::iRangeZoom, true);
    ui->worldPlot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    ui->worldPlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    ui->worldPlot->replot();

    QSharedPointer<QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
    fixedTicker->setTickCount(Game::worldWidth);
    fixedTicker->setTickOrigin(0);
    fixedTicker->setTickStep(Game::worldHeight); // tick step shall be 1.0
    fixedTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone); // and no scaling of the tickstep e.commandsGradient. multiples are allowed

    ui->worldPlot->xAxis->setRange(0,Game::worldWidth);
    ui->worldPlot->xAxis->setTicker(fixedTicker);
    ui->worldPlot->xAxis->setSubTicks(false);

    ui->worldPlot->yAxis->setRange(0,Game::worldHeight);
    ui->worldPlot->yAxis->setTicker(fixedTicker);
    ui->worldPlot->yAxis->setSubTicks(false);

    //-----------------------------------------------------------------------
    colorMap = new QCPColorMap(ui->worldPlot->xAxis,ui->worldPlot->yAxis);
    colorMap->data()->setSize(Game::worldHeight,Game::worldWidth);
    colorMap->data()->setRange(QCPRange(0,Game::worldWidth),QCPRange(0,Game::worldHeight));
    colorMap->setInterpolate(false);
    //-----------------------------------------------------------------------

    commandsGradient.clearColorStops();
    commandsGradient.setColorInterpolation(QCPColorGradient::ColorInterpolation::ciRGB);
    commandsGradient.setColorStopAt(0,QColor(255,255,255));    //minerals
    commandsGradient.setColorStopAt(0.5,QColor(150,150,255));
    commandsGradient.setColorStopAt(0.52,QColor(230,230,230)); //Стоячий
    commandsGradient.setColorStopAt(0.53,QColor(50,200,255)); //Движение
    commandsGradient.setColorStopAt(0.54,QColor(200,0,0)); //Атака
    commandsGradient.setColorStopAt(0.55,QColor(255,255,0)); //Клонирование
    commandsGradient.setColorStopAt(0.56,QColor(100,100,0)); //Труп
    commandsGradient.setColorStopAt(0.57,QColor(240,0,255)); //Защита
    commandsGradient.setColorStopAt(0.58,QColor(0,255,0)); //Фотосинтез
    commandsGradient.setColorStopAt(0.59,QColor(70,30,255));
    commandsGradient.setColorStopAt(1,QColor(0,255,0)); //Фотосинтез
    colorMap->setGradient(commandsGradient);
    colorMap->data()->setCell(0,0,0);
    colorMap->data()->setCell(0,1,1);
    colorMap->rescaleDataRange(true);
}

void MainWindow::on_newWorldButton_clicked()
{
    locker.lockForWrite();
    game->isPlaying = false;
    locker.unlock();
    this->thread()->msleep(10);
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

void MainWindow::fps()
{
    bool ok = false;
    int tern = ui->ternLabel->text().toInt(&ok);
    if(ok){
        qDebug() << tern-lastTern;
        lastTern = tern;
    }
}
