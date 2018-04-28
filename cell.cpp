#include "cell.h"

Cell::Cell()
{
    x = 0;
    y = 0;
}

Cell::Cell(unsigned short x, unsigned short y):x(x),y(y)
{

}

void Cell::setCoords(unsigned short x, unsigned short y)
{
    this->x = x;
    this->y = y;
}

Cell::~Cell()
{

}
