#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QReadWriteLock>
#include <QDebug>
#include <QTime>
#include <QThread>
class Cell{
public:
    int a = 1;
};
class Game : public QObject
{
    Q_OBJECT
public:
    static uint ternsToUpdateInSkipMode;

    static uint worldWidth;
    static uint worldHeight;

    explicit Game(QObject *parent = 0);
    QReadWriteLock locker;

    bool isPlaying = false;
    bool skipDisplay = false;

    uint currentTern = 0;
    uint ternsPerSecond = 0;
    uint aliveBots = 0;
    uint deadBots = 0;

    QTime lastTime;
    void tern();
    Cell ***world = nullptr;
    void resetWorld();

signals:
    void updateLabels(uint currentTern,uint ternDurationMs,uint aliveBots,uint deadBots);
public slots:
    void infinitGamePlaying();
};

#endif // GAME_H
