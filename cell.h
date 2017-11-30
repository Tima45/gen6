#ifndef CELL_H
#define CELL_H
#include <QtCore>

class Cell
{
public:
    Cell(uint x,uint y);
    void setCoords(uint x,uint y);
    virtual ~Cell();
    uint x;
    uint y;

};

#endif // CELL_H
