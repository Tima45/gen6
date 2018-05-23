#ifndef EMPTY_H
#define EMPTY_H

#include "cell.h"
#include "bot.h"

class Empty : public Cell
{
public:
    static float mineralsMax;
    static float mineralsMin;

    static float mineralsGrowSpeedMax;
    static float mineralsGrowSpeedMin;

    static float produciveProbabilityMax;
    static float produciveProbabilityMin;

    static float mineralsGrowBorder;

    Empty();
    Empty(unsigned short x,unsigned short y);
    void setCoords(unsigned short x,unsigned short y);
    bool producive = false;
    float minerals;
    void mineralsGrowUp();

    float localMineralsMax;
    float mineralsGrowSpeed;
    void recalculateGrowSpeed(float awayFromCenter);
    void recalculateLocalMineralsMax(float awayFromCenter);
    void recalculateProductivable();
};

#endif // EMPTY_H
