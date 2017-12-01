#ifndef BOT_H
#define BOT_H

#include "cell.h"

class Bot : Cell
{
public:
    enum GenomCommands{
        STAND,
        PHOTO,
        MINERALS,
        DEFENCE,
        MOVE,
        ATTACK,
        CHANGEDIRPOSITIVE,
        CHANGEDIRNEGAITIVE,
        LOOKENERGY,
        LOOKUP,
        CLONE,
        GenomCommandsLast
    };
    enum Directions{
        RIGHT,
        UPRIGHT,
        UP,
        UPLEFT,
        LEFT,
        BOTTOMLEFT,
        BOTTOM,
        BOTTOMRIGHT,
        DirectionsLast
    };

    Bot(unsigned short x,unsigned short y);
    ~Bot();
    char intentionCommand;
    char direction;
};

#endif // BOT_H
