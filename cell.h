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
    Cell(unsigned short x,unsigned short y);
    void setCoords(unsigned short x,unsigned short y);
    virtual ~Cell();
    unsigned short x;
    unsigned short y;
    ChildType childType;
};

#endif // CELL_H
