#include "genomcommandlistwidgetitem.h"

void GenomCommandListWidgetItem::colorItem(QListWidgetItem *item, Bot::GenomCommands command)
{
    if(command == Bot::PHOTO || command == Bot::EATSUGAR || command == Bot::SHARESUGAR || command == Bot::USESUGAR){
        item->setTextColor(QColor(10,128,10));
    }else if(command == Bot::GO){
        item->setTextColor(QColor(0,200,200));
    }else if(command == Bot::EATMINERALS || command == Bot::SHAREMINERALS || command == Bot::USEMINERALS){
        item->setTextColor(QColor(0,0,255));
    }else if(command == Bot::ATTACK){
        item->setTextColor(QColor(255,0,0));
    }else if(command == Bot::EATTALLOW || command == Bot::USETALLOW || command == Bot::SHARETALLOW){
        item->setTextColor(QColor(139,94,73));
    }else if(command == Bot::CLONE){
        item->setTextColor(QColor(200,200,0));
    }else{
        item->setTextColor(QColor(0,0,0));
    }
}

GenomCommandListWidgetItem::GenomCommandListWidgetItem() : QListWidgetItem()
{

}

GenomCommandListWidgetItem::~GenomCommandListWidgetItem()
{

}
