#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QReadWriteLock>
#include <QDebug>
#include <QTime>
#include <QThread>
#include <QLinkedList>
#include <QDataStream>
#include "plot/qcustomplot.h"
#include "bot.h"
#include "empty.h"
#include "doublecolors.h"



class Game : public QObject
{
    Q_OBJECT
public:
    enum DisplayMode{
        Style,
        Energy,
        KillCount,
        CloneCount,
        DefenceAmount,
        LongLiveAmount,
        SugarAmount,
        MineralsAmount,
        TallowAmount
    };

    static Game &singleGame();

    static uchar turnsToUpdateInSkipMode;

    Cell ***world;
    static const unsigned short worldSizeMax;
    static unsigned short worldWidth;
    static unsigned short worldHeight;

    explicit Game(QObject *parent = 0);
    ~Game();

    QLinkedList<Empty*> emptyHell;
    QLinkedList<Bot*> botHell;

    QReadWriteLock locker;

    bool isPlaying = false;
    bool inTurn = false;
    bool skipDisplay = false;

    uint currentTurn = 0;
    uint turnsPerSecond = 0;
    uint aliveBotsCount = 0;
    uint deadBotsCount = 0;


    void turn();


    Bot **botsToDoSimpleThings = nullptr;
    uint botsToDoSimpleThingsCount = 0;
    Bot **botsToMove = nullptr;
    uint botsToMoveCount = 0;
    Bot **botsToAttack = nullptr;
    uint botsToAttackCount = 0;
    Bot **botsToClone = nullptr;
    uint botsToCloneCount = 0;

    Bot **deadBots = nullptr;


    void resetWorld();
    QCPColorMap *colorMap;
    DisplayMode displayMode;
    void drawWorld();

signals:
    void updateLabels(uint currentTurn,uint aliveBots,uint deadBots);
    void emitReplotWorld(QCustomPlot::RefreshPriority);
public slots:
    void infinitGamePlaying();
    void playOneTurn();
    void recalculateMinerals();
    void saveWorld(QDataStream &str);
    void loadWorld(QDataStream &str);
};

#endif // GAME_H
