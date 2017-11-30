#include "game.h"

uchar Game::ternsToUpdateInSkipMode = 64;
unsigned short Game::worldWidth = 512;
unsigned short Game::worldHeight = 512;

Game::Game(QObject *parent) : QObject(parent)
{

}

void Game::tern()
{
    lastTime = QTime::currentTime();
    //-------------------------------------------
    this->thread()->msleep(50);
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
    }
}

void Game::resetWorld()
{
    currentTern = 0;
    if(world != nullptr){
        for(uint y = 0; y < worldHeight; y++){
            for(uint x = 0; x < worldWidth; x++){
                delete world[y][x];
            }
            delete world[y];
        }
        delete world;
        world = nullptr;
    }
    world = new Cell**[worldHeight];
    for(uint y = 0; y < worldHeight; y++){
        world[y] = new Cell*[worldWidth];
        for(uint x = 0; x < worldWidth; x++){
            world[y][x] = new Empty(x,y);
        }
    }
}

void Game::infinitGamePlaying()
{
    locker.lockForRead();
    while(isPlaying){
        locker.unlock();
        tern();
        locker.lockForRead();
    }
    locker.unlock();
}
