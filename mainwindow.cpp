#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initPlot();
    ui->emptyGroupBox->setVisible(false);
    ui->botGroupBox->setVisible(false);

    gameThread = new QThread;
    game = new Game;
    game->colorMap = colorMap;
    game->moveToThread(gameThread);

    connect(this,SIGNAL(startGame()),game,SLOT(infinitGamePlaying()),Qt::QueuedConnection);
    connect(this,SIGNAL(playOneTurn()),game,SLOT(playOneTurn()),Qt::QueuedConnection);
    connect(game,SIGNAL(updateLabels(uint,uint,uint)),this,SLOT(updateLabels(uint,uint,uint)),Qt::QueuedConnection);
    qRegisterMetaType<QCustomPlot::RefreshPriority>("QCustomPlot::RefreshPriority");
    connect(game,SIGNAL(emitReplotWorld(QCustomPlot::RefreshPriority)),ui->worldPlot,SLOT(replot(QCustomPlot::RefreshPriority)),Qt::QueuedConnection);

    gameThread->start();
    game->resetWorld();
    game->drawWorld();


    fpsTimer = new QTimer;
    fpsTimer->setInterval(250);
    connect(fpsTimer,SIGNAL(timeout()),this,SLOT(fpsCount()));
    fpsTimer->start();

}

MainWindow::~MainWindow()
{
    delete ui;
    game->deleteLater();
    gameThread->deleteLater();
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

    QSharedPointer<QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
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
/*
    for(int y = 0; y < Game::worldHeight; y++){
        for(int x = 0; x < Game::worldWidth; x++){
            colorMap->data()->setCell(x,y,rand()%RAND_MAX);
        }
    }*/
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
    p.setWidth(1);
    pt.setStyle(Qt::DashLine);
    traceLeftTop->setPen(pt);
    traceRightBottom->setPen(pt);


    traceLeftTop->setVisible(false);
    traceRightBottom->setVisible(false);

    dirArrow = new QCPItemLine(ui->worldPlot);
    dirArrow->setPen(p);
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
}

void MainWindow::on_newWorldButton_clicked()
{
    locker.lockForWrite();
    game->isPlaying = false;
    locker.unlock();
    this->thread()->msleep(10);
    ui->startStopButton->setText("Старт");
    if(QMessageBox::question(this,"Старт нового мира","Это приведет к удалению текущего мира. Удалять?") == QMessageBox::Yes){
        game->resetWorld();
        game->drawWorld();
        ui->worldPlot->replot();

        aliveGraph->data()->clear();
        deadGraph->data()->clear();
    }

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

void MainWindow::updateLabels(uint turn, uint alive, uint dead)
{
    ui->turnLabel->setText(QString::number(turn));
    ui->aliveCountLabel->setText(QString::number(alive));
    ui->deadCountLabel->setText(QString::number(dead));
    aliveGraph->addData(turn,alive);
    deadGraph->addData(turn,dead);
    ui->aliveDeadPlot->replot();
    ui->aliveDeadPlot->rescaleAxes();

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
}

void MainWindow::on_skipReplotCheck_clicked(bool checked)
{
    locker.lockForWrite();
    game->skipDisplay = checked;
    locker.unlock();
}

void MainWindow::fpsCount()
{
    locker.lockForRead();
    int newFps = game->currentTurn;
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
        trackingCell = game->world[y][x];
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
    game->displayMode = (Game::DisplayMode)index;
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
    }
    if(!game->isPlaying){
        game->drawWorld();
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
    game->isPlaying = false;
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
    ui->genomList->clear();
    for(uchar i = 0; i < Bot::genomSize; i++){
        ui->genomList->addItem(QString::number(i)+": "+Bot::genomCommandsToString((Bot::GenomCommands)bot->genom[i]));

        if(bot->genom[i] == Bot::PHOTO || bot->genom[i] == Bot::EATSUGAR || bot->genom[i] == Bot::SHARESUGAR || bot->genom[i] == Bot::USESUGAR){
            ui->genomList->item(ui->genomList->count()-1)->setTextColor(QColor(10,128,10));
        }else if(bot->genom[i] == Bot::GO){
            ui->genomList->item(ui->genomList->count()-1)->setTextColor(QColor(0,200,200));
        }else if(bot->genom[i] == Bot::EATMINERALS || bot->genom[i] == Bot::SHAREMINERALS || bot->genom[i] == Bot::USEMINERALS){
            ui->genomList->item(ui->genomList->count()-1)->setTextColor(QColor(0,0,255));
        }else if(bot->genom[i] == Bot::ATTACK){
            ui->genomList->item(ui->genomList->count()-1)->setTextColor(QColor(255,0,0));
        }else if(bot->genom[i] == Bot::EATTALLOW || bot->genom[i] == Bot::USETALLOW || bot->genom[i] == Bot::SHARETALLOW){
            ui->genomList->item(ui->genomList->count()-1)->setTextColor(QColor(139,94,73));
        }else if(bot->genom[i] == Bot::CLONE){
            ui->genomList->item(ui->genomList->count()-1)->setTextColor(QColor(200,200,0));
        }
        if(i == (bot->genomIndex+1)%Bot::genomSize){
            ui->genomList->item(ui->genomList->count()-1)->setFont(QFont("Times", 10, QFont::Bold));
        }
    }

}



void MainWindow::on_worldParameternsButton_clicked()
{
    locker.lockForWrite();
    game->isPlaying = false;
    locker.unlock();
    ui->startStopButton->setText("Старт");
    WorldParametersDialog *dialog = new WorldParametersDialog();
    dialog->setAttribute(Qt::WA_ShowModal);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}
