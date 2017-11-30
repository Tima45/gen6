#ifndef CELL_H
#define CELL_H
#include <QtCore>

class Cell
{
public:
    enum ChildType{
        empty = 0,
        bot = 1
    };
    Cell(uint x,uint y);
    void setCoords(uint x,uint y);
    virtual ~Cell();
    uint x;
    uint y;
    ChildType childType;
};

#endif // CELL_H
