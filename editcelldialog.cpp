#include "editcelldialog.h"
#include "ui_editcelldialog.h"

EditCellDialog::EditCellDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditCellDialog)
{
    ui->setupUi(this);
    for(int i = 0; i < Bot::genomSize; i++){
        GenomCommandListWidgetItem *newItem = new GenomCommandListWidgetItem();
        newItem->setText(QString::number(i)+": "+Bot::genomCommandsToString(Bot::STAND));
        newItem->command = Bot::STAND;
        ui->genomList->addItem(newItem);
    }
    for(int i = 0; i < ((int)Bot::MOVEINDEX50+1); i++){
        GenomCommandListWidgetItem *newItem = new GenomCommandListWidgetItem();
        newItem->setText(Bot::genomCommandsToString((Bot::GenomCommands)i));
        newItem->command = i;
        GenomCommandListWidgetItem::colorItem(newItem,(Bot::GenomCommands)i);
        ui->genomCommandsList->addItem(newItem);
    }
}

EditCellDialog::~EditCellDialog()
{
    delete ui;
}

void EditCellDialog::setCell(Cell *cell)
{
    ui->localMineralsMaxBox->setMaximum(Empty::mineralsMax);
    ui->mineralsBox->setMaximum(Empty::mineralsMax);
    editingCell = cell;
    if(cell->childType == Cell::empty){
        Empty *eCell = (Empty*)cell;
        ui->mineralsBox->setValue(eCell->minerals);
        ui->localMineralsMaxBox->setValue(eCell->localMineralsMax);
        ui->productiveBox->setChecked(eCell->producive);
        ui->cellTypeBox->setCurrentIndex(0);
        ui->botGroup->setVisible(false);
        ui->emptyGroup->setVisible(true);
    }else if(cell->childType == Cell::bot){
        Bot *bot = (Bot*)cell;
        setFieldsByBot(bot);
        ui->cellTypeBox->setCurrentIndex(1);
        ui->botGroup->setVisible(true);
        ui->emptyGroup->setVisible(false);
    }
}

void EditCellDialog::setBotsFields(Bot *bot)
{
    bot->health = ui->healthBox->value();
    bot->energy = ui->energyBox->value();
    bot->specialColor = ui->specialColorBox->isChecked();
    bot->noClone = ui->noCloneBox->isChecked();

    bot->defenceMinerals = ui->defenceMineralsBox->value();
    bot->longLiveSugar = ui->longLiveSugarBox->value();
    bot->genomIndex = ui->genomIndexBox->value();

    bot->carrySugar = ui->carrySugarBox->value();
    bot->carryMinerals = ui->carryMineralsBox->value();
    bot->carryTallow = ui->carryTallowBox->value();

    bot->eatSugarKof = ui->eatSugarKofBox->value();
    bot->eatMineralsKof = ui->eatMineralsKofBox->value();
    bot->eatTallowKof = ui->eatTallowKofBox->value();

    bot->useSugarKof = ui->useSugarKofBox->value();
    bot->useMineralsKof = ui->useMineralsKofBox->value();
    bot->useTallowKof = ui->useTallowKofBox->value();

    bot->shareSugarKof = ui->shareSugarKofBox->value();
    bot->shareMineralsKof = ui->shareMineralsKofBox->value();
    bot->shareTallowKof = ui->shareTallowKofBox->value();

    bot->turnCount = ui->lifeTimeBox->value();

    bot->direction = ui->directionBox->value();

    bot->noMutation = ui->noMutationBox->isChecked();

    for(int i = 0; i < Bot::genomSize; i++){
        GenomCommandListWidgetItem *commandItem = (GenomCommandListWidgetItem *)ui->genomList->item(i);
        bot->genom[i] = commandItem->command;
    }
    bot->calculateLifeStyle();

    bot->setCoords(editingCell->x,editingCell->y);
}

void EditCellDialog::setEmptysFields(Empty *cell)
{
    cell->minerals = ui->mineralsBox->value();
    cell->localMineralsMax = ui->localMineralsMaxBox->value();
    cell->producive = ui->productiveBox->isChecked();
    cell->setCoords(editingCell->x,editingCell->y);
}

void EditCellDialog::setFieldsByBot(Bot *bot)
{
    ui->healthBox->setValue(bot->health);
    ui->energyBox->setValue(bot->energy);

    ui->carrySugarBox->setValue(bot->carrySugar);
    ui->carryMineralsBox->setValue(bot->carryMinerals);
    ui->carryTallowBox->setValue(bot->carryTallow);

    ui->eatSugarKofBox->setValue(bot->eatSugarKof);
    ui->eatMineralsKofBox->setValue(bot->eatMineralsKof);
    ui->eatTallowKofBox->setValue(bot->eatTallowKof);

    ui->useSugarKofBox->setValue(bot->useSugarKof);
    ui->useMineralsKofBox->setValue(bot->useMineralsKof);
    ui->useTallowKofBox->setValue(bot->useTallowKof);

    ui->shareSugarKofBox->setValue(bot->shareSugarKof);
    ui->shareMineralsKofBox->setValue(bot->shareMineralsKof);
    ui->shareTallowKofBox->setValue(bot->shareTallowKof);

    ui->defenceMineralsBox->setValue(bot->defenceMinerals);
    ui->longLiveSugarBox->setValue(bot->longLiveSugar);

    ui->genomIndexBox->setValue(bot->genomIndex);

    ui->noCloneBox->setChecked(bot->noClone);
    ui->specialColorBox->setChecked(bot->specialColor);
    ui->noMutationBox->setChecked(bot->noMutation);

    ui->lifeTimeBox->setValue(bot->turnCount);

    ui->directionBox->setValue(bot->direction);



    for(int i = 0; i < Bot::genomSize; i++){
       GenomCommandListWidgetItem *item = (GenomCommandListWidgetItem *)ui->genomList->item(i);
       item->command = bot->genom[i];
       item->setText(QString::number(i)+": "+Bot::genomCommandsToString((Bot::GenomCommands)item->command));
       GenomCommandListWidgetItem::colorItem(item,(Bot::GenomCommands)item->command);
    }
}

void EditCellDialog::on_cellTypeBox_currentIndexChanged(int index)
{
    if(index == 0){
        ui->botGroup->setVisible(false);
        ui->emptyGroup->setVisible(true);
    }else if(index == 1){
        ui->botGroup->setVisible(true);
        ui->emptyGroup->setVisible(false);
        ui->genomList->setCurrentRow(-1);
    }
}

void EditCellDialog::on_genomCommandsList_itemDoubleClicked(QListWidgetItem *item)
{
    if(ui->genomList->currentRow() != -1){
        GenomCommandListWidgetItem *commandItem = (GenomCommandListWidgetItem *)item;
        GenomCommandListWidgetItem *genomItem = (GenomCommandListWidgetItem *)ui->genomList->currentItem();
        genomItem->command = commandItem->command;
        genomItem->setText(QString::number(ui->genomList->currentRow())+": "+commandItem->text());
        GenomCommandListWidgetItem::colorItem(genomItem,(Bot::GenomCommands)genomItem->command);
    }
}

void EditCellDialog::on_buttonBox_accepted()
{
    if(editingCell != nullptr){
        if(editingCell->childType == Cell::empty){
            if(ui->cellTypeBox->currentIndex() == Cell::empty){
                Empty *cell = (Empty*)editingCell;
                setEmptysFields(cell);
            }else{
                Bot* bot = Game::singleGame().botHell.takeLast();
                setBotsFields(bot);
                Game::singleGame().world[editingCell->y][editingCell->x] = bot;
                Game::singleGame().emptyHell.append((Empty *)editingCell);

            }
        }else if(editingCell->childType == Cell::bot){
            if(ui->cellTypeBox->currentIndex() == Cell::bot){
                Bot* bot = (Bot*)editingCell;
                setBotsFields(bot);
            }else{
                Empty *newEmpty = Game::singleGame().emptyHell.takeLast();
                setEmptysFields(newEmpty);
                Game::singleGame().world[editingCell->y][editingCell->x] = newEmpty;
                Game::singleGame().botHell.append((Bot*)editingCell);
            }
        }
        emit redrawWorld();
    }
}

void EditCellDialog::on_genomList_itemDoubleClicked(QListWidgetItem *item)
{
    if(ui->genomCommandsList->currentRow() != -1){
        GenomCommandListWidgetItem *genomItem = (GenomCommandListWidgetItem *)item;
        GenomCommandListWidgetItem *commandItem = (GenomCommandListWidgetItem *)ui->genomCommandsList->currentItem();
        genomItem->command = commandItem->command;
        genomItem->setText(commandItem->text());
        genomItem->setText(QString::number(ui->genomList->currentRow())+": "+commandItem->text());
        GenomCommandListWidgetItem::colorItem(genomItem,(Bot::GenomCommands)genomItem->command);
    }
}

void EditCellDialog::on_randomizeGenomButton_clicked()
{
    for(int i = 0; i < Bot::genomSize; i++){
        GenomCommandListWidgetItem *item = (GenomCommandListWidgetItem *)ui->genomList->item(i);
        int randValue = rand()%(Bot::MOVEINDEX1 +1);
        if(randValue == Bot::MOVEINDEX1){
            randValue += rand()%((Bot::MOVEINDEX50-Bot::MOVEINDEX1) +1);
        }
        item->setText(QString::number(i)+": "+Bot::genomCommandsToString(Bot::GenomCommands(randValue)));
        GenomCommandListWidgetItem::colorItem(item,Bot::GenomCommands(randValue));
        item->command = Bot::GenomCommands(randValue);
    }
}

void EditCellDialog::on_saveBotButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this,"Сохранить бота","","Bot (*.bot);");
    if(!filePath.isEmpty()){
        QFile f(filePath);
        f.open(QIODevice::WriteOnly);
        QDataStream str(&f);
        str << Game::gameVersion;
        Bot *tmpBot = new Bot(0,0);
        setBotsFields(tmpBot);
        Game::saveBot(str,tmpBot);
        delete tmpBot;
        f.close();
    }
}

void EditCellDialog::on_loadBotButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,"Загрузить бота","","Bot (*.bot);");
    if(!filePath.isEmpty()){
        QFile f(filePath);
        f.open(QIODevice::ReadOnly);
        QDataStream str(&f);
        int version = 0;
        str >> version;
        if(version != Game::gameVersion){
            f.close();
            QMessageBox::critical(this,"Ошибка","Не удалось короче\nВерсия не так");
            return;
        }else{
            Bot *tmpBot = new Bot(0,0);
            Game::loadBot(str,tmpBot);
            setFieldsByBot(tmpBot);
            delete tmpBot;
            f.close();
        }
    }
}
