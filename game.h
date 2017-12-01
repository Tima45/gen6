#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QReadWriteLock>
#include <QDebug>
#include <QTime>
#include <QThread>
#include <QStack>
#include "plot/qcustomplot.h"
#include "cell.h"
#include "empty.h"
#include "bot.h"

class Game : public QObject
{
    Q_OBJECT
public:
    static uchar ternsToUpdateInSkipMode;

    static unsigned short worldWidth;
    static unsigned short worldHeight;

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

    QStack<Bot*> botHell;
    QStack<Empty*> emptyHell;

    void resetWorld();
    QCPColorMap *colorMap;
    void drawWorld();

signals:
    void updateLabels(uint currentTern,uint ternDurationMs,uint aliveBots,uint deadBots);
    void emitReplotWorld(QCustomPlot::RefreshPriority);
public slots:
    void infinitGamePlaying();
};

#endif // GAME_H
