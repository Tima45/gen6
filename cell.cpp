#include "cell.h"

Cell::Cell(uint x, uint y):x(x),y(y)
{

}

void Cell::setCoords(uint x, uint y)
{
    this->x = x;
    this->y = y;
}

Cell::~Cell()
{

}
