#ifndef GENOMCOMMANDLISTWIDGETITEM_H
#define GENOMCOMMANDLISTWIDGETITEM_H

#include <QListWidgetItem>
#include <bot.h>

class GenomCommandListWidgetItem : public QListWidgetItem
{
public:
    GenomCommandListWidgetItem();
    ~GenomCommandListWidgetItem();
    unsigned short command;
};

#endif // GENOMCOMMANDLISTWIDGETITEM_H
