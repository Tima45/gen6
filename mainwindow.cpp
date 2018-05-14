#include "mainwindow.h"
#include "ui_mainwindow.h"

const int MainWindow::appVersion = 64;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initListWidget();
    initPlot();

    ui->emptyGroupBox->setVisible(false);
    ui->botGroupBox->setVisible(false);

    gameThread = new QThread;

    Game::singleGame().colorMap = colorMap;
    Game::singleGame().moveToThread(gameThread);



    connect(this,SIGNAL(startGame()),&Game::singleGame(),SLOT(infinitGamePlaying()),Qt::QueuedConnection);
    connect(this,SIGNAL(playOneTurn()),&Game::singleGame(),SLOT(playOneTurn()),Qt::QueuedConnection);
    connect(&Game::singleGame(),SIGNAL(updateLabels(uint,uint,uint)),this,SLOT(updateLabels(uint,uint,uint)),Qt::QueuedConnection);
    qRegisterMetaType<QCustomPlot::RefreshPriority>("QCustomPlot::RefreshPriority");
    connect(&Game::singleGame(),SIGNAL(emitReplotWorld(QCustomPlot::RefreshPriority)),ui->worldPlot,SLOT(replot(QCustomPlot::RefreshPriority)),Qt::QueuedConnection);



    Game::singleGame().resetWorld();
    Game::singleGame().drawWorld();


    fpsTimer = new QTimer;
    fpsTimer->setInterval(250);
    connect(fpsTimer,SIGNAL(timeout()),this,SLOT(fpsCount()));
    fpsTimer->start();

    gameThread->start(QThread::NormalPriority);


    dialog = new WorldParametersDialog();
    dialog->setAttribute(Qt::WA_ShowModal);
    connect(dialog,SIGNAL(changeWorldSize(int)),this,SLOT(resetWorldRange(int)));
    connect(&Game::singleGame(),SIGNAL(resetColorMap(int)),this,SLOT(resetColorMapRange(int)),Qt::DirectConnection);
}

MainWindow::~MainWindow()
{
    stopGame();
    this->thread()->msleep(10);
    gameThread->quit();
    gameThread->deleteLater();
    dialog->deleteLater();
    delete ui;
}

void MainWindow::initPlot()
{
    ui->worldPlot->addLayer("low");
    ui->worldPlot->addLayer("mid");
    ui->worldPlot->addLayer("top");


    //ui->worldPlot->setOpenGl(true,1);
    ui->worldPlot->setNoAntialiasingOnDrag(true);
    ui->worldPlot->setInteraction(QCP::iRangeDrag, true);
    ui->worldPlot->setInteraction(QCP::iRangeZoom, true);
    ui->worldPlot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    ui->worldPlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical); 

    fixedTicker = QSharedPointer<QCPAxisTickerFixed>(new QCPAxisTickerFixed);
    fixedTicker->setTickCount(Game::worldWidth);
    fixedTicker->setTickOrigin(0);
    fixedTicker->setTickStep(Game::worldWidth);
    fixedTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone); // and no scaling of the tickstep e.commandsGradient. multiples are allowed

    ui->worldPlot->xAxis->setRange(0,Game::worldWidth);
    ui->worldPlot->xAxis->setTicker(fixedTicker);
    ui->worldPlot->xAxis->setSubTicks(false);

    ui->worldPlot->yAxis->setRange(0,Game::worldHeight);
    ui->worldPlot->yAxis->setTicker(fixedTicker);
    ui->worldPlot->yAxis->setSubTicks(false);

    //-----------------------------------------------------------------------
    colorMap = new QCPColorMap(ui->worldPlot->xAxis,ui->worldPlot->yAxis);
    colorMap->setLayer("low");
    colorMap->data()->setSize(Game::worldWidth,Game::worldHeight);
    colorMap->data()->setRange(QCPRange(0,Game::worldWidth-1),QCPRange(0,Game::worldHeight-1));

    colorMap->setInterpolate(false);
    colorMap->setAntialiased(false);
    colorMap->setAntialiasedFill(false);
    colorMap->setAntialiasedScatters(false);
    //-----------------------------------------------------------------------
    styleGradient.clearColorStops();
    styleGradient.setColorInterpolation(QCPColorGradient::ColorInterpolation::ciRGB);
    styleGradient.setColorStopAt(DoubleColors::MineralsMinColor,QColor(255,255,255));    //minerals
    styleGradient.setColorStopAt(DoubleColors::MineralsMaxColor,QColor(128,128,255));
    styleGradient.setColorStopAt(DoubleColors::StandingColor,QColor(69,69,69));
    styleGradient.setColorStopAt(DoubleColors::DeadBotColor,QColor(139,94,73));
    styleGradient.setColorStopAt(DoubleColors::PhotoUserMinColor,QColor(69,69,69));
    styleGradient.setColorStopAt(DoubleColors::PhotoUserMaxColor,QColor(0,180,50));
    styleGradient.setColorStopAt(DoubleColors::MineralsUserMinColor,QColor(69,69,69));
    styleGradient.setColorStopAt(DoubleColors::MineralsUserMaxColor,QColor(20,20,255));
    styleGradient.setColorStopAt(DoubleColors::TallowUserMinColor,QColor(69,69,69));
    styleGradient.setColorStopAt(DoubleColors::TallowUserMaxColor,QColor(255,10,50));

    energyGradient.clearColorStops();
    energyGradient.setColorInterpolation(QCPColorGradient::ColorInterpolation::ciRGB);
    energyGradient.setColorStopAt(DoubleColors::MineralsMinColor,QColor(255,255,255));
    energyGradient.setColorStopAt(DoubleColors::MineralsMaxColor,QColor(128,128,255));
    energyGradient.setColorStopAt(DoubleColors::StandingColor,QColor(180,180,180));
    energyGradient.setColorStopAt(DoubleColors::DeadBotColor,QColor(139,94,73));
    energyGradient.setColorStopAt(DoubleColors::BotEnergyMaxColor,QColor(255,255,0));
    energyGradient.setColorStopAt(DoubleColors::BotEnergyMinColor,QColor(175,68,15));

    killGradient.clearColorStops();
    killGradient.setColorInterpolation(QCPColorGradient::ColorInterpolation::ciRGB);
    killGradient.setColorStopAt(DoubleColors::MineralsMinColor,QColor(255,255,255));
    killGradient.setColorStopAt(DoubleColors::MineralsMaxColor,QColor(128,128,255));
    killGradient.setColorStopAt(DoubleColors::DeadBotColor,QColor(139,94,73));
    killGradient.setColorStopAt(DoubleColors::NoKillColor,QColor(0,255,0));
    killGradient.setColorStopAt(DoubleColors::KillMinColor,QColor(100,0,0));
    killGradient.setColorStopAt(DoubleColors::KillMaxColor,QColor(255,0,0));

    cloneGradient.clearColorStops();
    cloneGradient.setColorInterpolation(QCPColorGradient::ColorInterpolation::ciRGB);
    cloneGradient.setColorStopAt(DoubleColors::MineralsMinColor,QColor(255,255,255));
    cloneGradient.setColorStopAt(DoubleColors::MineralsMaxColor,QColor(128,128,255));
    cloneGradient.setColorStopAt(DoubleColors::DeadBotColor,QColor(139,94,73));
    cloneGradient.setColorStopAt(DoubleColors::CloneMinColor,QColor(QColor(175,68,15)));
    cloneGradient.setColorStopAt(DoubleColors::CloneMaxColor,QColor(QColor(255,255,0)));

    defenceGradient.clearColorStops();
    defenceGradient.setColorInterpolation(QCPColorGradient::ColorInterpolation::ciRGB);
    defenceGradient.setColorStopAt(DoubleColors::MineralsMinColor,QColor(255,255,255));
    defenceGradient.setColorStopAt(DoubleColors::MineralsMaxColor,QColor(128,128,255));
    defenceGradient.setColorStopAt(DoubleColors::DeadBotColor,QColor(139,94,73));
    defenceGradient.setColorStopAt(DoubleColors::DefenceMinColor,QColor(QColor(175,68,15)));
    defenceGradient.setColorStopAt(DoubleColors::DefenceMaxColor,QColor(QColor(255,255,0)));

    longLiveGradient.clearColorStops();
    longLiveGradient.setColorInterpolation(QCPColorGradient::ColorInterpolation::ciRGB);
    longLiveGradient.setColorStopAt(DoubleColors::MineralsMinColor,QColor(255,255,255));
    longLiveGradient.setColorStopAt(DoubleColors::MineralsMaxColor,QColor(128,128,255));
    longLiveGradient.setColorStopAt(DoubleColors::DeadBotColor,QColor(139,94,73));
    longLiveGradient.setColorStopAt(DoubleColors::DefenceMinColor,QColor(QColor(175,68,15)));
    longLiveGradient.setColorStopAt(DoubleColors::DefenceMaxColor,QColor(QColor(255,255,0)));

    amountGradient.clearColorStops();
    amountGradient.setColorInterpolation(QCPColorGradient::ColorInterpolation::ciRGB);
    amountGradient.setColorStopAt(DoubleColors::MineralsMinColor,QColor(255,255,255));
    amountGradient.setColorStopAt(DoubleColors::MineralsMaxColor,QColor(128,128,255));
    amountGradient.setColorStopAt(DoubleColors::DeadBotColor,QColor(139,94,73));
    amountGradient.setColorStopAt(DoubleColors::DefenceMinColor,QColor(QColor(175,68,15)));
    amountGradient.setColorStopAt(DoubleColors::DefenceMaxColor,QColor(QColor(255,255,0)));

    //----
    colorMap->setGradient(styleGradient);
    colorMap->data()->setCell(0,0,0);
    colorMap->data()->setCell(0,1,1);
    colorMap->rescaleDataRange(true);



    cursor = new QCPItemRect(ui->worldPlot);
    cursor->setLayer("mid");
    cursor->setAntialiased(false);
    QPen p(QColor(255,10,50,200));
    p.setWidth(3);
    cursor->setPen(p);
    cursor->setVisible(false);

    traceLeftTop = new QCPItemTracer(ui->worldPlot);
    traceLeftTop->setLayer("mid");

    traceRightBottom = new QCPItemTracer(ui->worldPlot);
    traceRightBottom->setLayer("mid");



    QPen pt(QColor(10,10,255,100));
    pt.setStyle(Qt::DashLine);
    traceLeftTop->setPen(pt);
    traceRightBottom->setPen(pt);


    traceLeftTop->setVisible(false);
    traceRightBottom->setVisible(false);


    /*redPen = QPen(QColor(255,10,50));
    redPen.setWidth(2);
    blackPen = QPen(QColor(0,0,0));
    blackPen.setWidth(2);*/
    dirArrow = new QCPItemLine(ui->worldPlot);
    //dirArrow->setPen(redPen);
    dirArrow->setLayer("mid");
    dirArrow->setVisible(false);
    dirArrow->setHead(QCPLineEnding::esSpikeArrow);


    ui->worldPlot->replot();
    //connect(ui->worldPlot->xAxis,SIGNAL(rangeChanged(QCPRange)),this,SLOT(rangeLimitation(QCPRange)));
    //connect(ui->worldPlot->yAxis,SIGNAL(rangeChanged(QCPRange)),this,SLOT(rangeLimitation(QCPRange)));

    connect(ui->worldPlot,SIGNAL(mouseDoubleClick(QMouseEvent*)),this,SLOT(handleMouseClicking(QMouseEvent*)));

    ui->worldPlot->moveLayer(ui->worldPlot->layer("grid"),ui->worldPlot->layer("top"));

    aliveGraph = ui->aliveDeadPlot->addGraph();
    deadGraph = ui->aliveDeadPlot->addGraph();
    aliveGraph->setPen(QPen(QColor(Qt::green)));
    deadGraph->setPen(QPen(QColor(139,94,73)));

    aliveTracer = new QCPItemTracer(ui->aliveDeadPlot);
    aliveTracer->setPen(Qt::NoPen);
    aliveTracer->setBrush(QBrush(QColor(Qt::green)));
    aliveTracer->setStyle(QCPItemTracer::tsCircle);
    aliveGraph->addData(0,0);
    aliveTracer->setGraph(aliveGraph);

    deadTracer = new QCPItemTracer(ui->aliveDeadPlot);
    deadTracer->setPen(Qt::NoPen);
    deadTracer->setBrush(QBrush(QColor(139,94,73)));
    deadTracer->setStyle(QCPItemTracer::tsCircle);
    deadGraph->addData(0,0);
    deadTracer->setGraph(deadGraph);

    ui->aliveDeadPlot->setInteraction(QCP::iRangeDrag, true);
    ui->aliveDeadPlot->setInteraction(QCP::iRangeZoom, true);
    ui->aliveDeadPlot->axisRect()->setRangeDrag(Qt::Horizontal);
    ui->aliveDeadPlot->axisRect()->setRangeZoom(Qt::Horizontal);
    ui->aliveDeadPlot->xAxis->setRange(-1000,100);
}

void MainWindow::initListWidget()
{
    for(int i = 0; i < Bot::genomSize; i++){
        GenomCommandListWidgetItem *newItem = new GenomCommandListWidgetItem();
        ui->genomList->addItem(newItem);
    }
}

void MainWindow::resetColorMapRange(int size)
{
    colorMap->data()->clear();
    colorMap->data()->setSize(size,size);
    colorMap->data()->setRange(QCPRange(0,size-1),QCPRange(0,size-1));
    ui->worldPlot->xAxis->setRange(QCPRange(0,size));
    ui->worldPlot->yAxis->setRange(QCPRange(0,size));

    fixedTicker->setTickCount(Game::worldWidth);
    fixedTicker->setTickOrigin(0);
    fixedTicker->setTickStep(Game::worldWidth);
}
void MainWindow::resetWorldRange(int size){
    resetColorMapRange(size);

    Game::singleGame().resetWorld();
    Game::singleGame().drawWorld();
    ui->worldPlot->replot();

    aliveGraph->data()->clear();
    deadGraph->data()->clear();
    lastTurn = 0;
    ui->aliveDeadPlot->xAxis->setRange(-1000,100);
}

void MainWindow::on_newWorldButton_clicked()
{
    stopGame();
    this->thread()->msleep(10);

    if(QMessageBox::question(this,"Старт нового мира","Это приведет к удалению текущего мира. Удалять?") == QMessageBox::Yes){
        Game::singleGame().resetWorld();
        Game::singleGame().drawWorld();
        ui->worldPlot->replot();

        aliveGraph->data()->clear();
        deadGraph->data()->clear();
        lastTurn = 0;
        ui->aliveDeadPlot->xAxis->setRange(-1000,100);
    }

}

void MainWindow::on_startStopButton_clicked()
{
    locker.lockForWrite();
    if(!Game::singleGame().isPlaying){
        Game::singleGame().isPlaying = true;
        ui->startStopButton->setText("Стоп");
        emit startGame();
    }else{
        Game::singleGame().isPlaying = false;
        ui->startStopButton->setText("Старт");
    }
    locker.unlock();
}

void MainWindow::updateLabels(uint turn, uint alive, uint dead)
{

    ui->turnLabel->setText(QString::number(turn));
    ui->aliveCountLabel->setText(QString::number(alive));
    ui->deadCountLabel->setText(QString::number(dead));
    aliveGraph->addData(turn,alive);
    deadGraph->addData(turn,dead);
    aliveTracer->setGraphKey(turn);
    deadTracer->setGraphKey(turn);
    ui->aliveDeadPlot->xAxis->setRange(ui->aliveDeadPlot->xAxis->range().lower+(turn-lastTurn),ui->aliveDeadPlot->xAxis->range().upper+(turn-lastTurn));
    ui->aliveDeadPlot->yAxis->rescale();
    ui->aliveDeadPlot->replot(QCustomPlot::rpQueuedReplot);


    if(tracking){
        if(trackingCell->tracking){
            if(trackingCell->childType == Cell::empty){
                Empty *cell = (Empty*)trackingCell;
                ui->mineralsLabel->setText(QString::number(cell->minerals));
                ui->xLabel->setText(QString::number(cell->x));
                ui->yLabel->setText(QString::number(cell->y));
            }

            if(trackingCell->childType == Cell::bot){
                displayBotInfo((Bot*)trackingCell);

                switch (((Bot*)trackingCell)->direction) {
                    case Bot::RIGHT:{
                        dirArrow->start->setCoords(trackingCell->x-0.5,trackingCell->y);
                        dirArrow->end->setCoords(trackingCell->x+0.5,trackingCell->y);
                        break;
                    }
                    case Bot::UPRIGHT:{
                        dirArrow->start->setCoords(trackingCell->x-0.5,trackingCell->y-0.5);
                        dirArrow->end->setCoords(trackingCell->x+0.5,trackingCell->y+0.5);
                        break;
                    }
                    case Bot::UP:{
                        dirArrow->start->setCoords(trackingCell->x,trackingCell->y-0.5);
                        dirArrow->end->setCoords(trackingCell->x,trackingCell->y+0.5);
                        break;
                    }
                    case Bot::UPLEFT:{
                        dirArrow->start->setCoords(trackingCell->x+0.5,trackingCell->y-0.5);
                        dirArrow->end->setCoords(trackingCell->x-0.5,trackingCell->y+0.5);
                        break;
                    }
                    case Bot::LEFT:{
                        dirArrow->start->setCoords(trackingCell->x+0.5,trackingCell->y);
                        dirArrow->end->setCoords(trackingCell->x-0.5,trackingCell->y);
                        break;
                    }
                    case Bot::DOWNLEFT:{
                        dirArrow->start->setCoords(trackingCell->x+0.5,trackingCell->y+0.5);
                        dirArrow->end->setCoords(trackingCell->x-0.5,trackingCell->y-0.5);
                        break;
                    }
                    case Bot::DOWN:{
                        dirArrow->start->setCoords(trackingCell->x,trackingCell->y+0.5);
                        dirArrow->end->setCoords(trackingCell->x,trackingCell->y-0.5);
                        break;
                    }
                    case Bot::DOWNRIGHT:{
                        dirArrow->start->setCoords(trackingCell->x-0.5,trackingCell->y+0.5);
                        dirArrow->end->setCoords(trackingCell->x+0.5,trackingCell->y-0.5);
                        break;
                    }
                    default:{
                        break;
                    }
                }
                dirArrow->setVisible(true);


            }


            cursor->topLeft->setCoords(trackingCell->x-0.5,trackingCell->y+0.5);
            cursor->bottomRight->setCoords(trackingCell->x+0.5,trackingCell->y-0.5);

            traceLeftTop->position->setCoords(trackingCell->x-0.5,trackingCell->y+0.5);
            traceRightBottom->position->setCoords(trackingCell->x+0.5,trackingCell->y-0.5);

        }else{
            tracking = false;
            cursor->setVisible(false);
            traceLeftTop->setVisible(false);
            traceRightBottom->setVisible(false);
            dirArrow->setVisible(false);
        }
    }
    lastTurn = turn;
    if(alive == 0){
        stopGame();
    }
}

void MainWindow::on_skipReplotCheck_clicked(bool checked)
{
    locker.lockForWrite();
    Game::singleGame().skipDisplay = checked;
    locker.unlock();
}

void MainWindow::fpsCount()
{
    locker.lockForRead();
    int newFps = Game::singleGame().currentTurn;
    locker.unlock();
    ui->turnsPerSecondLabel->setText(QString::number((newFps-lastFps)*4.0));
    lastFps = newFps;
}

void MainWindow::rangeLimitation(QCPRange newRange)
{
    if(newRange.lower < -50 || newRange.upper > Game::worldWidth+50){
        ui->worldPlot->xAxis->setRange(QCPRange(0,Game::worldWidth));
        ui->worldPlot->yAxis->setRange(QCPRange(0,Game::worldHeight));
    }
}

void MainWindow::handleMouseClicking(QMouseEvent *event)
{
    int x = qRound(ui->worldPlot->xAxis->pixelToCoord(event->x()));
    int y = qRound(ui->worldPlot->yAxis->pixelToCoord(event->y()));
    if(x < Game::worldWidth && y < Game::worldHeight && x >= 0 && y >= 0){
        trackingCell = Game::singleGame().world[y][x];
        tracking = true;
        trackingCell->tracking = true;
        if(trackingCell->childType == Cell::empty){
            Empty *cell = (Empty*)trackingCell;
            ui->emptyGroupBox->setVisible(true);
            ui->botGroupBox->setVisible(false);
            //ui->trackingGroupBox->setTitle("Пустота");
            ui->mineralsLabel->setText(QString::number(cell->minerals));
            ui->xLabel->setText(QString::number(x));
            ui->yLabel->setText(QString::number(y));
            dirArrow->setVisible(false);
        }
        if(trackingCell->childType == Cell::bot){
            ui->emptyGroupBox->setVisible(false);
            ui->botGroupBox->setVisible(true);
            displayBotInfo((Bot*)trackingCell);

            switch (((Bot*)trackingCell)->direction) {
                case Bot::RIGHT:{
                    dirArrow->start->setCoords(x-0.5,y);
                    dirArrow->end->setCoords(x+0.5,y);
                    break;
                }
                case Bot::UPRIGHT:{
                    dirArrow->start->setCoords(x-0.5,y-0.5);
                    dirArrow->end->setCoords(x+0.5,y+0.5);
                    break;
                }
                case Bot::UP:{
                    dirArrow->start->setCoords(x,y-0.5);
                    dirArrow->end->setCoords(x,y+0.5);
                    break;
                }
                case Bot::UPLEFT:{
                    dirArrow->start->setCoords(x+0.5,y-0.5);
                    dirArrow->end->setCoords(x-0.5,y+0.5);
                    break;
                }
                case Bot::LEFT:{
                    dirArrow->start->setCoords(x+0.5,y);
                    dirArrow->end->setCoords(x-0.5,y);
                    break;
                }
                case Bot::DOWNLEFT:{
                    dirArrow->start->setCoords(x+0.5,y+0.5);
                    dirArrow->end->setCoords(x-0.5,y-0.5);
                    break;
                }
                case Bot::DOWN:{
                    dirArrow->start->setCoords(x,y+0.5);
                    dirArrow->end->setCoords(x,y-0.5);
                    break;
                }
                case Bot::DOWNRIGHT:{
                    dirArrow->start->setCoords(x-0.5,y+0.5);
                    dirArrow->end->setCoords(x+0.5,y-0.5);
                    break;
                }
                default:{
                    break;
                }
            }
            dirArrow->setVisible(true);
        }
        cursor->setVisible(true);
        cursor->topLeft->setCoords(x-0.5,y+0.5);
        cursor->bottomRight->setCoords(x+0.5,y-0.5);
        traceLeftTop->position->setCoords(x-0.5,y+0.5);
        traceRightBottom->position->setCoords(x+0.5,y-0.5);


        traceLeftTop->setVisible(true);
        traceRightBottom->setVisible(true);
    }
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    locker.lockForWrite();
    Game::singleGame().displayMode = (Game::DisplayMode)index;
    locker.unlock();
    switch(index){
        case 0:{
            colorMap->setGradient(styleGradient);
            break;
        }
        case 1:{
            colorMap->setGradient(energyGradient);
            break;
        }
        case 2:{
            colorMap->setGradient(killGradient);
            break;
        }
        case 3:{
            colorMap->setGradient(cloneGradient);
            break;
        }
        case 4:{
            colorMap->setGradient(defenceGradient);
            break;
        }
        case 5:{
            colorMap->setGradient(longLiveGradient);
            break;
        }
        case 6:
        case 7:
        case 8:{
            colorMap->setGradient(amountGradient);
        }
    }
    /*
    if(tracking){
        if(index == Game::KillCount){
            dirArrow->setPen(blackPen);
        }else{
            dirArrow->setPen(redPen);
        }
    }*/
    if(!Game::singleGame().isPlaying){
        Game::singleGame().drawWorld();
        ui->worldPlot->replot();
    }


}

void MainWindow::on_rescaleButton_clicked()
{
    ui->worldPlot->xAxis->setRange(0,Game::worldWidth);
    ui->worldPlot->yAxis->setRange(0,Game::worldHeight);
}

void MainWindow::on_nextTurnButton_clicked()
{
    stopGame();
    emit playOneTurn();
    ui->startStopButton->setText("Старт");;
}

void MainWindow::displayBotInfo(Bot *bot)
{
    ui->xLabel->setText(QString::number(bot->x));
    ui->yLabel->setText(QString::number(bot->y));
    ui->healthLabel->setText(QString::number(bot->health));
    ui->energyLabel->setText(QString::number(bot->energy));
    ui->carrySugarLabel->setText(QString::number(bot->carrySugar));
    ui->carryMineralsLabel->setText(QString::number(bot->carryMinerals));
    ui->carryTallowLabel->setText(QString::number(bot->carryTallow));
    ui->eatMineralsLabel->setText(QString::number(bot->eatMineralsKof));
    ui->eatSugarLabel->setText(QString::number(bot->eatSugarKof));
    ui->eatTallowLabel->setText(QString::number(bot->eatTallowKof));
    ui->useMineralsLabel->setText(QString::number(bot->useMineralsKof));
    ui->useSugarLabel->setText(QString::number(bot->useSugarKof));
    ui->useTallowLabel->setText(QString::number(bot->useTallowKof));
    ui->shareMineralsLabel->setText(QString::number(bot->shareMineralsKof));
    ui->shareSugarLabel->setText(QString::number(bot->shareSugarKof));
    ui->shareTallowLabel->setText(QString::number(bot->shareTallowKof));
    ui->defenceLabel->setText(QString::number(Bot::defenceFromDefenceMinerals(bot->defenceMinerals)));
    ui->longLiveLabel->setText(QString::number(Bot::longLiveValueFromSugar(bot->longLiveSugar)));
    ui->lifeTimeLabel->setText(QString::number(bot->turnCount));
    ui->cloneCountLabel->setText(QString::number(bot->cloneCount));
    ui->killCountLabel->setText(QString::number(bot->killCount));


    ui->genomIndexLabel->setText(QString::number(bot->genomIndex));


    for(uchar i = 0; i < Bot::genomSize; i++){
        GenomCommandListWidgetItem *item = (GenomCommandListWidgetItem *)ui->genomList->item(i);
        item->setText(QString::number(i)+": "+Bot::genomCommandsToString((Bot::GenomCommands)bot->genom[i]));
        item->command = bot->genom[i];
        if(bot->genom[i] == Bot::PHOTO || bot->genom[i] == Bot::EATSUGAR || bot->genom[i] == Bot::SHARESUGAR || bot->genom[i] == Bot::USESUGAR){
            item->setTextColor(QColor(10,128,10));
        }else if(bot->genom[i] == Bot::GO){
            item->setTextColor(QColor(0,200,200));
        }else if(bot->genom[i] == Bot::EATMINERALS || bot->genom[i] == Bot::SHAREMINERALS || bot->genom[i] == Bot::USEMINERALS){
            item->setTextColor(QColor(0,0,255));
        }else if(bot->genom[i] == Bot::ATTACK){
            item->setTextColor(QColor(255,0,0));
        }else if(bot->genom[i] == Bot::EATTALLOW || bot->genom[i] == Bot::USETALLOW || bot->genom[i] == Bot::SHARETALLOW){
            item->setTextColor(QColor(139,94,73));
        }else if(bot->genom[i] == Bot::CLONE){
            item->setTextColor(QColor(200,200,0));
        }else{
            item->setTextColor(QColor(0,0,0));
        }
        if(i == (bot->genomIndex+1)%Bot::genomSize){
            item->setFont(QFont("Times", 10, QFont::Bold));
        }else{
            item->setFont(QFont("Times", 8, QFont::Normal));
        }
    }

}



void MainWindow::on_worldParameternsButton_clicked()
{
    stopGame();
    dialog->loadParametres();
    dialog->show();

}

void MainWindow::stopGame()
{
    locker.lockForWrite();
    Game::singleGame().isPlaying = false;
    locker.unlock();
    ui->startStopButton->setText("Старт");
}

void MainWindow::on_saveWorldButton_clicked()
{
    stopGame();

    QString filePath = QFileDialog::getSaveFileName(this,"Сохранить мир","","GenWorld (*.gw);");
    if(!filePath.isEmpty()){
        QFile f(filePath);
        f.open(QIODevice::WriteOnly);
        QDataStream str(&f);

        str << appVersion; //version
        QVector<QCPGraphData>::const_iterator i;
        str << aliveGraph->dataCount();
        for(i = aliveGraph->data()->constBegin(); i != aliveGraph->data()->constEnd(); i++){
            str << i->key;
            str << i->value;
        }
        str << deadGraph->dataCount();
        for(i = deadGraph->data()->constBegin(); i != deadGraph->data()->constEnd(); i++){
            str << i->key;
            str << i->value;
        }
        Game::singleGame().saveWorld(str);
        f.close();
    }
}

void MainWindow::on_loadWorldButton_clicked()
{
    stopGame();
    QString filePath = QFileDialog::getOpenFileName(this,"Загрузить мир","","GenWorld (*.gw);");
    if(!filePath.isEmpty()){
        QFile f(filePath);
        tracking = false;
        try{
            if(!f.open(QIODevice::ReadOnly)){
                throw 0;
            }
            QDataStream str(&f);

            int version = 0;
            str >> version;
            if(version != appVersion){
                throw 0;
            }
            QVector<QCPGraphData>::const_iterator i;
            int aliveBotsCount = 0;
            str >> aliveBotsCount;
            aliveGraph->data()->clear();
            for(int i = 0; i < aliveBotsCount; i++){
                double key;
                double value;
                str >> key;
                str >> value;
                aliveGraph->addData(key,value);
            }
            deadGraph->data()->clear();
            int deadBotsCount = 0;
            str >> deadBotsCount;
            for(int i = 0; i < deadBotsCount; i++){
                double key;
                double value;
                str >> key;
                str >> value;
                deadGraph->addData(key,value);
            }



            Game::singleGame().loadWorld(str);
            f.close();
            ui->turnLabel->setText(QString::number(Game::singleGame().currentTurn));
            ui->aliveCountLabel->setText(QString::number(Game::singleGame().aliveBotsCount));
            ui->deadCountLabel->setText(QString::number(Game::singleGame().deadBotsCount));
            Game::singleGame().drawWorld();
            ui->worldPlot->replot();

            lastTurn = Game::singleGame().currentTurn-1;

            ui->aliveDeadPlot->rescaleAxes();
            ui->aliveDeadPlot->replot();

        }
        catch(int a){
            QMessageBox::critical(this,"Ошибка","Не удалось загрузить файл");
        }
    }
}

void MainWindow::on_clearGraphButton_clicked()
{
    aliveGraph->data()->clear();
    deadGraph->data()->clear();
    deadGraph->addData(Game::singleGame().currentTurn,0);
}

void MainWindow::on_genomList_itemDoubleClicked(QListWidgetItem *item)
{
    GenomCommandListWidgetItem *gItem = (GenomCommandListWidgetItem*)item;
    switch (gItem->command){
        case Bot::GenomCommands::MOVEINDEX1:
        case Bot::GenomCommands::MOVEINDEX2:
        case Bot::GenomCommands::MOVEINDEX3:
        case Bot::GenomCommands::MOVEINDEX4:
        case Bot::GenomCommands::MOVEINDEX5:
        case Bot::GenomCommands::MOVEINDEX6:
        case Bot::GenomCommands::MOVEINDEX7:
        case Bot::GenomCommands::MOVEINDEX8:
        case Bot::GenomCommands::MOVEINDEX9:
        case Bot::GenomCommands::MOVEINDEX10:
        case Bot::GenomCommands::MOVEINDEX11:
        case Bot::GenomCommands::MOVEINDEX12:
        case Bot::GenomCommands::MOVEINDEX13:
        case Bot::GenomCommands::MOVEINDEX14:
        case Bot::GenomCommands::MOVEINDEX15:
        case Bot::GenomCommands::MOVEINDEX16:
        case Bot::GenomCommands::MOVEINDEX17:
        case Bot::GenomCommands::MOVEINDEX18:
        case Bot::GenomCommands::MOVEINDEX19:
        case Bot::GenomCommands::MOVEINDEX20:
        case Bot::GenomCommands::MOVEINDEX21:
        case Bot::GenomCommands::MOVEINDEX22:
        case Bot::GenomCommands::MOVEINDEX23:
        case Bot::GenomCommands::MOVEINDEX24:
        case Bot::GenomCommands::MOVEINDEX25:
        case Bot::GenomCommands::MOVEINDEX26:
        case Bot::GenomCommands::MOVEINDEX27:
        case Bot::GenomCommands::MOVEINDEX28:
        case Bot::GenomCommands::MOVEINDEX29:
        case Bot::GenomCommands::MOVEINDEX30:
        case Bot::GenomCommands::MOVEINDEX31:
        case Bot::GenomCommands::MOVEINDEX32:
        case Bot::GenomCommands::MOVEINDEX33:
        case Bot::GenomCommands::MOVEINDEX34:
        case Bot::GenomCommands::MOVEINDEX35:
        case Bot::GenomCommands::MOVEINDEX36:
        case Bot::GenomCommands::MOVEINDEX37:
        case Bot::GenomCommands::MOVEINDEX38:
        case Bot::GenomCommands::MOVEINDEX39:
        case Bot::GenomCommands::MOVEINDEX40:
        case Bot::GenomCommands::MOVEINDEX41:
        case Bot::GenomCommands::MOVEINDEX42:
        case Bot::GenomCommands::MOVEINDEX43:
        case Bot::GenomCommands::MOVEINDEX44:
        case Bot::GenomCommands::MOVEINDEX45:
        case Bot::GenomCommands::MOVEINDEX46:
        case Bot::GenomCommands::MOVEINDEX47:
        case Bot::GenomCommands::MOVEINDEX48:
        case Bot::GenomCommands::MOVEINDEX49:
        case Bot::GenomCommands::MOVEINDEX50:{
            unsigned short toMove = (unsigned short)gItem->command-(unsigned short)Bot::GenomCommands::MOVEINDEX1;
                ui->genomList->setCurrentRow((ui->genomList->currentRow()+toMove+1)%Bot::genomSize);
            break;
        }
        default:{
            break;
        }
    }
}
