#include "game.h"

Game::Game(QObject *parent) : QObject(parent)
{

}

void Game::tern()
{

}

void Game::infinitGamePlaying()
{
    QReadWriteLock locker;
    locker.lockForRead();
    while(isPlaying){
        locker.unlock();
        tern();
        locker.lockForRead();
    }
    locker.unlock();
}
