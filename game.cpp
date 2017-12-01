#include "game.h"

uchar Game::ternsToUpdateInSkipMode = 64;
unsigned short Game::worldWidth = 512;
unsigned short Game::worldHeight = 512;

Game::Game(QObject *parent) : QObject(parent)
{
    botHell.reserve(worldWidth*worldHeight);
    emptyHell.reserve(worldWidth*worldHeight);
    for(int i = 0; i < worldWidth*worldHeight; i++){
        botHell.push(new Bot(0,0));
        emptyHell.push(new Empty(0,0));
    }
}

void Game::tern()
{
    lastTime = QTime::currentTime();
    //-------------------------------------------
    for(unsigned short y = 0; y < worldHeight; y++){
        for(unsigned short x = 0; x < worldWidth;x++){
            if(world[y][x]->childType == Cell::empty){
                Empty *cell = (Empty*)world[y][x];
                cell->mineralsGrowUp();
            }
        }
    }
    //-------------------------------------------
    currentTern++;

    locker.lockForRead();
    if(skipDisplay){
        locker.unlock();
        if(currentTern%ternsToUpdateInSkipMode == 0){
            emit updateLabels(currentTern,lastTime.msecsTo(QTime::currentTime()),aliveBots,deadBots);
        }
    }else{
        locker.unlock();
        emit updateLabels(currentTern,lastTime.msecsTo(QTime::currentTime()),aliveBots,deadBots);
        drawWorld();
        emit emitReplotWorld(QCustomPlot::rpQueuedRefresh);

    }
}

void Game::resetWorld()
{
    currentTern = 0;
    if(world == nullptr){
        for(unsigned short y = 0; y < worldHeight; y++){
            world[y] = new Cell*[worldWidth];
            for(unsigned short x = 0; x < worldWidth; x++){
                Empty *cell = emptyHell.pop();
                cell->setCoords(x,y);
                cell->recalculateGrowSpeed();
                cell->recalculateLocalMineralsMax();
                world[y][x] = cell;
            }
        }
    }
    world = new Cell**[worldHeight];
    for(unsigned short y = 0; y < worldHeight; y++){
        world[y] = new Cell*[worldWidth];
        for(unsigned short x = 0; x < worldWidth; x++){
            world[y][x] = new Empty(x,y);
        }
    }
}

void Game::drawWorld()
{
    for(unsigned short y = 0; y < worldHeight; y++){
        for(unsigned short x = 0; x < worldWidth;x++){
            if(world[y][x]->childType == Cell::empty){
                Empty *cell = (Empty*)world[y][x];
                colorMap->data()->setCell(x,y,cell->minerals/(2.0*Empty::mineralsMax));
            }
        }
    }
}

void Game::infinitGamePlaying()
{
    srand(time(NULL));
    locker.lockForRead();
    while(isPlaying){
        locker.unlock();
        tern();
        locker.lockForRead();
    }
    locker.unlock();
}
