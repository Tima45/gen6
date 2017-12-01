#include "empty.h"

float Empty::mineralsMax = 100;
float Empty::mineralsMin = 1;

float Empty::mineralsGrowSpeedMax = 0.2;
float Empty::mineralsGrowSpeedMin = 0;

Empty::Empty(unsigned short x, unsigned short y):Cell(x,y)
{
    childType = Cell::empty;
    randVal = 0.00001+(rand()%10000)/10000.0;
    recalculateGrowSpeed();
    recalculateLocalMineralsMax();
}

void Empty::setCoords(unsigned short x, unsigned short y)
{
    this->x = x;
    this->y = y;
    recalculateGrowSpeed();
    recalculateLocalMineralsMax();
}
void Empty::mineralsGrowUp()
{
    minerals += mineralsGrowSpeed*randVal;
    minerals = (minerals > localMineralsMax?localMineralsMax:minerals);
}

void Empty::recalculateGrowSpeed()
{
    float awayFromCenter = sqrt(powf(x-Game::worldWidth/2.0,2.0)+powf(y-Game::worldHeight/2.0,2.0));
    mineralsGrowSpeed = (mineralsGrowSpeedMax-mineralsGrowSpeedMin)*(1-awayFromCenter/sqrt(powf(Game::worldWidth/2.0,2.0)+powf(Game::worldHeight/2.0,2.0)));
}

void Empty::recalculateLocalMineralsMax()
{
    float awayFromCenter = sqrt(powf(x-Game::worldWidth/2.0,2.0)+powf(y-Game::worldHeight/2.0,2.0));
    localMineralsMax = (mineralsMax-mineralsMin)*awayFromCenter/sqrt(powf(Game::worldWidth/2.0,2.0)+powf(Game::worldHeight/2.0,2.0));
}

