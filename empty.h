#ifndef EMPTY_H
#define EMPTY_H

#include "cell.h"
#include "game.h"

class Empty : public Cell
{
public:
    static float mineralsMax;
    static float mineralsMin;

    static float mineralsGrowSpeedMax;
    static float mineralsGrowSpeedMin;

    Empty(unsigned short x,unsigned short y);
    void setCoords(unsigned short x,unsigned short y);
    float minerals;
    void mineralsGrowUp();

    float localMineralsMax;
    float mineralsGrowSpeed;
    void recalculateGrowSpeed();
    void recalculateLocalMineralsMax();

    float randVal = 0;
    void clearCell();
};

#endif // EMPTY_H
