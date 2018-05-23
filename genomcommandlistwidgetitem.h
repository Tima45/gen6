#ifndef GENOMCOMMANDLISTWIDGETITEM_H
#define GENOMCOMMANDLISTWIDGETITEM_H

#include <QListWidgetItem>
#include "bot.h"

class GenomCommandListWidgetItem : public QListWidgetItem
{
public:
    static void colorItem(QListWidgetItem * item,Bot::GenomCommands command);
    GenomCommandListWidgetItem();
    ~GenomCommandListWidgetItem();
    unsigned short command;
};

#endif // GENOMCOMMANDLISTWIDGETITEM_H
