#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QReadWriteLock>
#include <QDebug>
#include <QTime>
#include <QThread>
#include <QLinkedList>
#include "plot/qcustomplot.h"
#include "bot.h"
#include "empty.h"
#include "doublecolors.h"



class Game : public QObject
{
    Q_OBJECT
public:
    enum DisplayMode{
        Commands,
        Energy,
        Genomdiff
    };

    static Game* singleGame;

    static uchar ternsToUpdateInSkipMode;

    Cell ***world;
    static unsigned short worldWidth;
    static unsigned short worldHeight;

    explicit Game(QObject *parent = 0);
    ~Game();

    QLinkedList<Empty*> emptyHell;
    QLinkedList<Bot*> botHell;

    QReadWriteLock locker;

    bool isPlaying = false;
    bool inTern = false;
    bool skipDisplay = false;

    uint currentTern = 0;
    uint ternsPerSecond = 0;
    uint aliveBotsCount = 0;
    uint deadBotsCount = 0;

    void tern();


    Bot **botsToDoSimpleThings = nullptr;
    uint botsToDoSimpleThingsCount = 0;
    Bot **botsToMove = nullptr;
    uint botsToMoveCount = 0;
    Bot **botsToAttack = nullptr;
    uint botsToAttackCount = 0;
    Bot **botsToClone = nullptr;
    uint botsToCloneCount = 0;




    void resetWorld();
    QCPColorMap *colorMap;
    DisplayMode displayMode;
    void drawWorld();

signals:
    void updateLabels(uint currentTern,uint aliveBots,uint deadBots);
    void emitReplotWorld(QCustomPlot::RefreshPriority);
public slots:
    void infinitGamePlaying();
    void playOneTern();
};

#endif // GAME_H
