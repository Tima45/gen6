#include "empty.h"
#include "game.h"

float Empty::mineralsMax = 200;
float Empty::mineralsMin = 0;

float Empty::mineralsGrowSpeedMax = 1;
float Empty::mineralsGrowSpeedMin = 0;

float Empty::produciveProbabilityMax = 0.5;
float Empty::produciveProbabilityMin = 0.1;

float Empty::mineralsGrowBorder = 100;



Empty::Empty() : Cell()
{
    childType = Cell::empty;
}

Empty::Empty(unsigned short x, unsigned short y):Cell(x,y)
{
    childType = Cell::empty;
    randVal = 0.1+(rand()%10000)/10000.0;

    recalculateProductivable();
}

void Empty::setCoords(unsigned short x, unsigned short y)
{
    this->x = x;
    this->y = y;
    float awayFromCenter = sqrt(powf(x-Game::worldWidth/2.0,2.0)+powf(y-Game::worldHeight/2.0,2.0));
    recalculateGrowSpeed(awayFromCenter);
    recalculateLocalMineralsMax(awayFromCenter);
}
void Empty::mineralsGrowUp()
{
    if(producive){
        minerals += minerals < localMineralsMax ? mineralsGrowSpeed*randVal : 0;
    }
}

void Empty::recalculateGrowSpeed(float awayFromCenter)
{
    if(awayFromCenter > mineralsGrowBorder){
        mineralsGrowSpeed = (mineralsGrowSpeedMax-mineralsGrowSpeedMin)*(1-(awayFromCenter-mineralsGrowBorder)/sqrt(powf(Game::worldWidth/2.0,2.0)+powf(Game::worldHeight/2.0,2.0))) + mineralsGrowSpeedMin;
    }else{
        mineralsGrowSpeed = 0;
    }
}

void Empty::recalculateLocalMineralsMax(float awayFromCenter)
{
    if(awayFromCenter > mineralsGrowBorder){
        localMineralsMax = (mineralsMax-mineralsMin)*(awayFromCenter-mineralsGrowBorder)/sqrt(powf(Game::worldWidth/2.0,2.0)+powf(Game::worldHeight/2.0,2.0)) + mineralsMin;
    }else{
        localMineralsMax = 0;
    }
}

void Empty::recalculateProductivable()
{
    float awayFromCenter = sqrt(powf(x-Game::worldWidth/2.0,2.0)+powf(y-Game::worldHeight/2.0,2.0));
    if(awayFromCenter > mineralsGrowBorder){
        producive = 1.0*rand()/RAND_MAX < (produciveProbabilityMax-produciveProbabilityMin)*(awayFromCenter-mineralsGrowBorder)/sqrt(powf(Game::worldWidth/2.0,2.0)+powf(Game::worldHeight/2.0,2.0)) + produciveProbabilityMin ? true : false;
    }else{
        producive = false;
    }
}

void Empty::clearCell()
{

}

