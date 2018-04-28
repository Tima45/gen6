#ifndef CELL_H
#define CELL_H

#include <QtCore>

class Cell
{
public:
    enum ChildType{
        empty,
        bot
    };
    Cell();
    Cell(unsigned short x,unsigned short y);
    void setCoords(unsigned short x,unsigned short y);
    virtual ~Cell();
    unsigned short x;
    unsigned short y;
    ChildType childType;
    bool tracking = false;
    Cell *botToMoveOn = nullptr;
};

#endif // CELL_H
