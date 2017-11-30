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

    Empty(uint x,uint y);
    void setCoords(uint x,uint y);
    float minerals;
    void mineralsGrowUp();

    float localMineralsMax;
    float mineralsGrowSpeed;
    void recalculateGrowSpeed();
    void recalculateLocalMineralsMax();

    float randVal = 0;
};

#endif // EMPTY_H
