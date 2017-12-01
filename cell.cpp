#include "cell.h"

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
